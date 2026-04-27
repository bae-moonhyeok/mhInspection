
// 01_ViewerDlg.h: 헤더 파일
//

#pragma once

#include "ImageViewEx.h"

#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"

#include "DlgVisionTest.h"

struct RawImageSettings
{
	int nWidth = 1280;
	int nHeight = 800;
	int nColorOrder = 0;		// 0=None, 1=RGB, 2=BGR
};

// CMy01ViewerDlg 대화 상자
class CMy01ViewerDlg : public CDialogEx
{
// 생성입니다.
public:
	CMy01ViewerDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

	void UserInit();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MY01_VIEWER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedBtnSave();
	afx_msg void OnBnClickedBtnLoad();
	afx_msg void OnBnClickedBtnRawApply();
	afx_msg void OnBnClickedBtnDlg();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

private:
	lt::CImageViewEx* m_WndImageView = nullptr;
	// TOBE: lt::CImageViewEx* 단일 / 복수 비교해볼 것. ^2026-04-26 13:05

	cv::Mat m_matCopy;

	RawImageSettings m_rawSettings;

	CStatic  m_stRawGroup;
	CStatic  m_stRawWidth, m_stRawHeight, m_stRawBpp, m_stRawCapFmt, m_stRawColorOrder;
	CEdit    m_edRawWidth, m_edRawHeight, m_edRawBpp;
	CComboBox m_cmbRawCapFmt, m_cmbRawColorOrder;
	CButton  m_btnRawApply;

	void CreateRawSettingsUI();
	CString GetSettingsFilePath() const;
	void LoadRawSettings();
	void SaveRawSettings();
	void UpdateUIFromSettings();
	bool UpdateSettingsFromUI();
	bool LoadRawFile(LPCTSTR szPath, cv::Mat& matOut);

	DlgVisionTest* m_DlgVisionTest = nullptr;
	lt::IMAGE_INFO m_imageInfo;
	BYTE* m_ImageBuffer = nullptr;
	cv::Mat m_matProcessed;
	size_t m_ProcessedImageSizeInBytes;
	cv::Mat m_matOverlaid;

	void CleanUpInpsectionDialog();
	void RefreshInpsectionDialogImage(size_t bytes);
	void InitProcessedImage();
	const cv::Mat& GetViewImage();

	// IDC_CHECK_KEEP_IMAGE: 체크 상태이면 자식 대화상자 종료/로드 시 처리 이미지 자동 정리를 건너뛴다.
	CButton m_chkKeepImage;
	bool IsKeepImage() const;

	CListBox m_listLog;
	CString m_strTime;

	// 자식 대화상자로부터 요청된 IDC_LIST_LOG 로깅은 WindowProc(WM_APPEND_LOG) 에서
	// AddLog() 로 포워딩한다. 외부(자식)에서 호출 가능하도록 public 에 그대로 둔다.
public:
	void AddLog(const wchar_t* str);
};

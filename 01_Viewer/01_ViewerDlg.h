
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

	void CleanUpInpsectionDialog();
	void RefreshInpsectionDialogImage(size_t bytes);
	void InitProcessedImage();
	const cv::Mat& GetViewImage();
};

#pragma once
#include "afxdialogex.h"

#include "ImageViewEx.h"
#include "opencv2/core.hpp"

#define WM_CLOSE_VISION_TEST_DLG	(WM_USER + 100)
#define WM_UPDATE_VIEW				(WM_USER + 101)
// 부모 대화상자의 IDC_LIST_LOG 에 로그를 추가하도록 요청한다.
// LPARAM: LPCTSTR 문자열 포인터(호출자가 수명 보장) — SendMessage 로 동기 처리.
#define WM_APPEND_LOG				(WM_USER + 102)

struct SImageProcess
{
	
};

enum class eViewTarget
{
	First = 0,
	Origin = First,
	Result,
	Last = Result
};
constexpr int operator+(eViewTarget eViewTarget)
{
	return static_cast<int>(eViewTarget);
}

// DlgVisionTest 대화 상자

class DlgVisionTest : public CDialogEx
{
	DECLARE_DYNAMIC(DlgVisionTest)

public:
	DlgVisionTest(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~DlgVisionTest();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_VISION_TEST };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedBtnClose();
	afx_msg void OnBnClickedBtnImageProcess();
	afx_msg void OnBnClickedRadioStatus(UINT ctrl_id);
	afx_msg void OnBnClickedBtnKernelSave();
	afx_msg void OnBnClickedBtnKernelLoad();
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();

private:
	void UpdateParameter();
	void UpdateViewer();

	// 3x3 커널 입력용 UI 를 IDC_STATIC_KERNER3X3 영역 안에 생성한다.
	void CreateKernelUI();
	// 현재 UI 에 입력된 값으로부터 3x3 커널 Mat 를 구성한다.
	cv::Mat BuildKernelFromUI() const;
	// 3x3 커널을 통해 컨볼루션 연산을 수행한다.
	void mhApplyConvolution3x3(const cv::Mat& src, const cv::Mat kernel);
	void ApplyConvolution3x3(cv::InputArray src, cv::OutputArray dst, cv::InputArray kernel);

	// 커널 설정 파일 입출력
	// - 인자 생략(기본) 시 exe 폴더의 viewer.ini, [Kernel] 섹션을 사용한다.
	// - 인자로 전체 경로를 주면 해당 파일을 사용한다(CFileDialog 로 선택한 경로).
	CString GetKernelSettingsPath() const;
	void LoadKernelSettings(LPCTSTR szPath = nullptr);
	void SaveKernelSettings(LPCTSTR szPath = nullptr);

public:
	void InitProcessedImage(const BYTE* pSrc, int width, int height, int channel);
	void CleanProcessedImage();
	void ResetProcessedImage(const BYTE* pSrc, int width, int height, int channels);

	// 부모가 소유하는 처리 대상 이미지(m_matProcessed) 의 참조를 전달받는다.
	void SetProcessedMatRef(cv::Mat* pMat) { m_refMatProcessed = pMat; }
	void SetImageBuffer(const BYTE& pSrc);
	void SetImageBuffer(const cv::Mat& mat);
	eViewTarget GetViewTarget() { return m_ViewTarget; };

private:
	SImageProcess sImageProcess = {};
	eViewTarget m_ViewTarget = eViewTarget::Origin;
	CRect m_rcView;

	BYTE* m_ImageBuffer = nullptr;
	cv::Mat* m_refMatProcessed = nullptr;

	// 커널 입력용 9개 EditBox (행 우선: [0..2]=1행, [3..5]=2행, [6..8]=3행)
	CEdit m_edKernel[9];

	// 커널 Save / Load 버튼 — CreateKernelUI() 에서 3x3 그리드 하단에 동적 생성
	CButton m_btnKernelSave;
	CButton m_btnKernelLoad;

	// ------------------------------------------------------------------
	// 부모 대화상자(IDC_LIST_LOG) 로깅 인터페이스
	// ------------------------------------------------------------------
	// 자식에서는 부모의 내부 구현을 알 필요가 없도록 WM_APPEND_LOG 메시지로
	// 전달한다. 문자열은 SendMessage 동기 호출 동안 유효해야 한다.
public:
	void LogToParent(LPCTSTR szMsg);
	afx_msg void OnBnClickedCheckKeepImage();
};

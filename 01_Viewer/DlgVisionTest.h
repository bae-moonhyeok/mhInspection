#pragma once
#include "afxdialogex.h"

#include "ImageViewEx.h"
#include "opencv2/core.hpp"

#define WM_CLOSE_VISION_TEST_DLG	(WM_USER + 100)
#define WM_UPDATE_VIEW				(WM_USER + 101)

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
	void ApplyConvolution3x3(cv::InputArray src, cv::OutputArray dst, cv::InputArray kernel);

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
};

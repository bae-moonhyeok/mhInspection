// DlgVisionTest.cpp: 구현 파일
//

#include "pch.h"
#include "01_Viewer.h"
#include "afxdialogex.h"
#include "DlgVisionTest.h"

#include "opencv2/imgproc.hpp"

#include "resource.h"

// DlgVisionTest 대화 상자

IMPLEMENT_DYNAMIC(DlgVisionTest, CDialogEx)

DlgVisionTest::DlgVisionTest(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_VISION_TEST, pParent)
{
	
}

DlgVisionTest::~DlgVisionTest()
{
}

void DlgVisionTest::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DlgVisionTest, CDialogEx)
	ON_BN_CLICKED(IDOK, &DlgVisionTest::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &DlgVisionTest::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BTN_CLOSE, &DlgVisionTest::OnBnClickedBtnClose)
	ON_BN_CLICKED(IDC_BTN_IMAGE_PROCESS, &DlgVisionTest::OnBnClickedBtnImageProcess)
	ON_COMMAND_RANGE(IDC_RADIO_ORIGIN, IDC_RADIO_RESULT, &DlgVisionTest::OnBnClickedRadioStatus)
	ON_WM_DESTROY()
END_MESSAGE_MAP()

// DlgVisionTest 메시지 처리기

void DlgVisionTest::OnBnClickedOk() { /*CDialogEx::OnOK();*/ }
void DlgVisionTest::OnBnClickedCancel() { CDialogEx::OnCancel(); }

void DlgVisionTest::OnBnClickedBtnClose()
{
	GetParent()->SendMessage(WM_CLOSE_VISION_TEST_DLG);
}

void DlgVisionTest::UpdateParameter()
{

}

cv::Mat DlgVisionTest::BuildKernelFromUI() const
{
	// 9개 EditBox 값을 읽어 3x3 double 커널 Mat 를 생성한다.
	cv::Mat kernel(3, 3, CV_64F);
	for (int i = 0; i < 9; ++i)
	{
		CString s;
		m_edKernel[i].GetWindowText(s);
		double v = _ttof(s);
		kernel.at<double>(i / 3, i % 3) = v;
	}
	return kernel;
}

void DlgVisionTest::ApplyConvolution3x3(cv::InputArray src, cv::OutputArray dst, cv::InputArray kernel)
{
	const int Width = src.cols();
	const int Height = src.rows();
	const int Channels = src.channels();
	const size_t Bytes = src.total();
	const int stride = Width * Channels;

	
}

void DlgVisionTest::OnBnClickedBtnImageProcess()
{
	// UI로부터 Parameter 갱신
	UpdateParameter();

	// 처리 대상 이미지가 유효한지 확인
	if (m_refMatProcessed == nullptr || m_refMatProcessed->empty())
	{
		AfxMessageBox(_T("처리할 이미지가 없습니다. 먼저 이미지를 불러오세요."), MB_ICONWARNING);
		return;
	}

	// UI 값으로부터 커널 생성 후 m_matProcessed 에 컨볼루션 적용
	cv::Mat kernel = BuildKernelFromUI();
	cv::Mat dst;
	
	cv::filter2D(*m_refMatProcessed, dst, -1, kernel, cv::Point(-1, -1), 0.0, cv::BORDER_DEFAULT);
	// TODO: ApplyConvolution3x3() 호출 (cv::filter2D 대체)


	*m_refMatProcessed = dst;

	// 부모에게 RESULT 뷰 갱신 요청 (현재 라디오 상태와 무관하게 결과로 전환)
	m_ViewTarget = eViewTarget::Result;
	UpdateViewer();
}

void DlgVisionTest::UpdateViewer()
{
	// Radio 체크 상태에 따른 출력
	PostMessage(WM_UPDATE_VIEW, (WPARAM)m_ViewTarget);
}

void DlgVisionTest::OnBnClickedRadioStatus(UINT ctrl_id)
{
	m_ViewTarget = static_cast<eViewTarget>(ctrl_id - IDC_RADIO_ORIGIN);
	UpdateViewer();
}

BOOL DlgVisionTest::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 3x3 커널 입력 UI 를 IDC_STATIC_KERNER3X3 영역 안에 생성한다.
	CreateKernelUI();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void DlgVisionTest::CreateKernelUI()
{
	// IDC_STATIC_KERNER3X3 의 클라이언트 사각형을 기준으로 3x3 EditBox 를 배치한다.
	CWnd* pAnchor = GetDlgItem(IDC_STATIC_KERNER3X3);
	if (pAnchor == nullptr) return;

	CRect rcAnchor;
	pAnchor->GetWindowRect(&rcAnchor);
	ScreenToClient(&rcAnchor);

	// 각 셀은 정사각형에 가깝도록 앵커 영역 내에서 중앙 배치
	const int cellSize = 80;     // 한 칸 크기(px)
	const int gap      = 6;      // 칸 사이 간격(px)
	const int gridW    = cellSize * 3 + gap * 2;
	const int gridH    = cellSize * 3 + gap * 2;

	const int x0 = rcAnchor.left + (rcAnchor.Width()  - gridW) / 2;
	const int y0 = rcAnchor.top  + (rcAnchor.Height() - gridH) / 2;

	// 기본값: 항등(identity) 커널 — 원본 그대로 통과
	const double defaults[9] = {
		0, 0, 0,
		0, 1, 0,
		0, 0, 0
	};

	CFont* pFont = GetFont();
	for (int i = 0; i < 9; ++i)
	{
		int row = i / 3;
		int col = i % 3;
		CRect rc(
			x0 + col * (cellSize + gap),
			y0 + row * (cellSize + gap),
			x0 + col * (cellSize + gap) + cellSize,
			y0 + row * (cellSize + gap) + cellSize);

		// ES_CENTER 로 가운데 정렬, 음수/소수 허용을 위해 ES_NUMBER 는 사용하지 않음
		m_edKernel[i].Create(
			WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_CENTER | ES_AUTOHSCROLL,
			rc, this, IDC_STATIC_KERNER3X3 + 1 + i);
		m_edKernel[i].SetFont(pFont);

		CString s;
		s.Format(_T("%g"), defaults[i]);
		m_edKernel[i].SetWindowText(s);
	}
}

void DlgVisionTest::OnDestroy()
{
	CDialogEx::OnDestroy();

	
}

void DlgVisionTest::InitProcessedImage(const BYTE* pSrc, int width, int height, int channels)
{

}

void DlgVisionTest::CleanProcessedImage()
{

}

void DlgVisionTest::ResetProcessedImage(const BYTE* pSrc, int width, int height, int channels)
{

}

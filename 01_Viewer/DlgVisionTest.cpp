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
	// 3x3 커널 수동 컨볼루션 구현 (cv::filter2D 대체).
	// - 경계 처리: BORDER_REPLICATE (가장자리 픽셀 값 복제)
	// - 결과는 src 의 depth/type 로 saturate_cast 후 반환
	// - src 지원 depth: CV_8U (최적), 그 외는 CV_64F 경유로 일반 처리
	cv::Mat matSrc = src.getMat();
	cv::Mat matK   = kernel.getMat();

	CV_Assert(matK.rows == 3 && matK.cols == 3);
	CV_Assert(!matSrc.empty());

	// 커널을 double 로 확보
	cv::Mat matKd;
	if (matK.type() == CV_64F) matKd = matK;
	else                       matK.convertTo(matKd, CV_64F);

	// 9개 커널 계수를 스택에 펼친다 — 내부 루프 최적화용
	const double k[9] = {
		matKd.at<double>(0, 0), matKd.at<double>(0, 1), matKd.at<double>(0, 2),
		matKd.at<double>(1, 0), matKd.at<double>(1, 1), matKd.at<double>(1, 2),
		matKd.at<double>(2, 0), matKd.at<double>(2, 1), matKd.at<double>(2, 2)
	};

	const int Height   = matSrc.rows;
	const int Width    = matSrc.cols;
	const int Channels = matSrc.channels();

	// 출력 Mat 를 src 와 동일한 타입으로 준비한다.
	dst.create(Height, Width, matSrc.type());
	cv::Mat matDst = dst.getMat();

	// CV_8U 전용 경로 — 실시간 미리보기에 충분한 속도 확보
	if (matSrc.depth() == CV_8U)
	{
		// 경계 1픽셀 복제 패딩
		cv::Mat padded;
		cv::copyMakeBorder(matSrc, padded, 1, 1, 1, 1, cv::BORDER_REPLICATE);

		for (int y = 0; y < Height; ++y)
		{
			const uchar* pRow0 = padded.ptr<uchar>(y);
			const uchar* pRow1 = padded.ptr<uchar>(y + 1);
			const uchar* pRow2 = padded.ptr<uchar>(y + 2);
			uchar*       pOut  = matDst.ptr<uchar>(y);

			for (int x = 0; x < Width; ++x)
			{
				for (int c = 0; c < Channels; ++c)
				{
					const int x0 = x * Channels + c;
					const int x1 = x0 + Channels;
					const int x2 = x1 + Channels;

					const double sum =
						pRow0[x0] * k[0] + pRow0[x1] * k[1] + pRow0[x2] * k[2] +
						pRow1[x0] * k[3] + pRow1[x1] * k[4] + pRow1[x2] * k[5] +
						pRow2[x0] * k[6] + pRow2[x1] * k[7] + pRow2[x2] * k[8];

					// 0~255 범위로 saturate (음수/오버플로 방지)
					pOut[x * Channels + c] = cv::saturate_cast<uchar>(sum);
				}
			}
		}
		return;
	}

	// 일반 경로: 64F 로 변환하여 계산 후 원래 타입으로 환원
	cv::Mat matSrcF, paddedF;
	matSrc.convertTo(matSrcF, CV_64F);
	cv::copyMakeBorder(matSrcF, paddedF, 1, 1, 1, 1, cv::BORDER_REPLICATE);

	cv::Mat matDstF(Height, Width, CV_MAKETYPE(CV_64F, Channels));

	for (int y = 0; y < Height; ++y)
	{
		for (int x = 0; x < Width; ++x)
		{
			for (int c = 0; c < Channels; ++c)
			{
				double sum = 0.0;
				for (int ky = 0; ky < 3; ++ky)
				{
					const double* pRow = paddedF.ptr<double>(y + ky);
					for (int kx = 0; kx < 3; ++kx)
					{
						sum += pRow[(x + kx) * Channels + c] * k[ky * 3 + kx];
					}
				}
				matDstF.ptr<double>(y)[x * Channels + c] = sum;
			}
		}
	}

	// 원래 타입으로 복귀 (convertTo 가 saturate 포함)
	matDstF.convertTo(matDst, matSrc.type());
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

	// cv::filter2D 대신 직접 구현한 3x3 컨볼루션 사용
	ApplyConvolution3x3(*m_refMatProcessed, dst, kernel);

	*m_refMatProcessed = dst;

	// 부모에게 RESULT 뷰 갱신 요청 (현재 라디오 상태와 무관하게 결과로 전환)
	m_ViewTarget = eViewTarget::Result;
	UpdateViewer();
}

void DlgVisionTest::UpdateViewer()
{
	// Radio 체크 상태에 따른 출력 — 메시지는 부모(CMy01ViewerDlg)가 처리하므로 부모로 전달한다.
	CWnd* pParent = GetParent();
	if (pParent != nullptr)
		pParent->PostMessage(WM_UPDATE_VIEW, (WPARAM)m_ViewTarget);
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

	// 라디오 버튼 초기 체크 상태: 원본(Origin)
	CheckRadioButton(IDC_RADIO_ORIGIN, IDC_RADIO_RESULT, IDC_RADIO_ORIGIN);
	m_ViewTarget = eViewTarget::Origin;

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

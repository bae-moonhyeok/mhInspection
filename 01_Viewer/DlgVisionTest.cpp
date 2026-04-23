// DlgVisionTest.cpp: 구현 파일
//

#include "pch.h"
#include "01_Viewer.h"
#include "afxdialogex.h"
#include "DlgVisionTest.h"

#include "opencv2/imgproc.hpp"

#include "resource.h"

// ==========================================================================
// 3x3 커널 관련(IDC_STATIC_KERNER3X3) 구현 방식 선택
// --------------------------------------------------------------------------
// 아래 4개 매크로 중 정확히 하나만 활성화하여 해당 구현만 컴파일한다.
// 나머지는 주석 처리 + #ifdef/#endif 가드로 비활성 상태를 유지한다.
// ==========================================================================
#define KERNEL_IMPL_CV_MAT              // [1] cv::Mat::at<T>() 기반 (OpenCV 이디엄)
//#define KERNEL_IMPL_CV_INPUT_ARRAY      // [2] cv::InputArray + Mat::ptr<T>() 기반
//#define KERNEL_IMPL_UCHAR               // [3] uchar* 원시 포인터 기반 (저수준)
//#define KERNEL_IMPL_UINT                // [4] UINT 누산기 기반 (정수 고정소수점)

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

// ======================================================================
// [1] cv::Mat 기반 구현 — cv::Mat::at<T>() 접근 방식 (OpenCV 이디엄)
// ======================================================================
#ifdef KERNEL_IMPL_CV_MAT
void DlgVisionTest::ApplyConvolution3x3(cv::InputArray src, cv::OutputArray dst, cv::InputArray kernel)
{
	// src/kernel 을 cv::Mat 로 획득하여 Mat::at<> 로만 접근한다.
	cv::Mat matSrc = src.getMat();
	cv::Mat matK   = kernel.getMat();
	CV_Assert(matK.rows == 3 && matK.cols == 3);
	CV_Assert(!matSrc.empty() && matSrc.type() == CV_8UC1);

	// 커널을 double 로 확보
	cv::Mat matKd;
	matK.convertTo(matKd, CV_64F);

	const int Height = matSrc.rows;
	const int Width  = matSrc.cols;

	// BORDER_REPLICATE 경계 확장
	cv::Mat matPad;
	cv::copyMakeBorder(matSrc, matPad, 1, 1, 1, 1, cv::BORDER_REPLICATE);

	dst.create(Height, Width, matSrc.type());
	cv::Mat matDst = dst.getMat();

	for (int y = 0; y < Height; ++y)
	{
		for (int x = 0; x < Width; ++x)
		{
			double sum = 0.0;
			for (int ky = 0; ky < 3; ++ky)
			{
				for (int kx = 0; kx < 3; ++kx)
				{
					// at<>() 는 경계 검사가 있어 다소 느리지만 가독성이 높다.
					const uchar  p = matPad.at<uchar>(y + ky, x + kx);
					const double k = matKd.at<double>(ky, kx);
					sum += static_cast<double>(p) * k;
				}
			}
			matDst.at<uchar>(y, x) = cv::saturate_cast<uchar>(sum);
		}
	}
}
#endif // KERNEL_IMPL_CV_MAT

// ======================================================================
// [2] cv::InputArray + Mat::ptr<T>() 기반 구현 — 행 포인터로 빠르게 접근
// ======================================================================
#ifdef KERNEL_IMPL_CV_INPUT_ARRAY
void DlgVisionTest::ApplyConvolution3x3(cv::InputArray src, cv::OutputArray dst, cv::InputArray kernel)
{
	// InputArray 로 추상화된 입력을 getMat() 로 바인딩하여 ptr<>() 로 순회한다.
	cv::Mat matSrc = src.getMat();
	cv::Mat matK   = kernel.getMat();
	CV_Assert(matK.rows == 3 && matK.cols == 3);
	CV_Assert(!matSrc.empty() && matSrc.type() == CV_8UC1);

	cv::Mat matKd;
	matK.convertTo(matKd, CV_64F);

	// 9개 커널 계수를 스택에 펼쳐 핫 루프 최적화
	const double k[9] = {
		matKd.at<double>(0,0), matKd.at<double>(0,1), matKd.at<double>(0,2),
		matKd.at<double>(1,0), matKd.at<double>(1,1), matKd.at<double>(1,2),
		matKd.at<double>(2,0), matKd.at<double>(2,1), matKd.at<double>(2,2)
	};

	const int Height = matSrc.rows;
	const int Width  = matSrc.cols;

	cv::Mat matPad;
	cv::copyMakeBorder(matSrc, matPad, 1, 1, 1, 1, cv::BORDER_REPLICATE);

	dst.create(Height, Width, matSrc.type());
	cv::Mat matDst = dst.getMat();

	for (int y = 0; y < Height; ++y)
	{
		const uchar* r0 = matPad.ptr<uchar>(y);
		const uchar* r1 = matPad.ptr<uchar>(y + 1);
		const uchar* r2 = matPad.ptr<uchar>(y + 2);
		uchar*       d  = matDst.ptr<uchar>(y);

		for (int x = 0; x < Width; ++x)
		{
			const double sum =
				r0[x]   * k[0] + r0[x+1] * k[1] + r0[x+2] * k[2] +
				r1[x]   * k[3] + r1[x+1] * k[4] + r1[x+2] * k[5] +
				r2[x]   * k[6] + r2[x+1] * k[7] + r2[x+2] * k[8];
			d[x] = cv::saturate_cast<uchar>(sum);
		}
	}
}
#endif // KERNEL_IMPL_CV_INPUT_ARRAY

// ======================================================================
// [3] uchar* 원시 포인터 기반 구현 — OpenCV API 호출 최소화
// ======================================================================
#ifdef KERNEL_IMPL_UCHAR
void DlgVisionTest::ApplyConvolution3x3(cv::InputArray src, cv::OutputArray dst, cv::InputArray kernel)
{
	// 입력 Mat 에서 .data, .step 만 꺼내 순수 C 스타일 포인터 연산으로 처리한다.
	cv::Mat matSrc = src.getMat();
	cv::Mat matK   = kernel.getMat();
	CV_Assert(matK.rows == 3 && matK.cols == 3);
	CV_Assert(!matSrc.empty() && matSrc.type() == CV_8UC1);

	cv::Mat matKd;
	matK.convertTo(matKd, CV_64F);
	const double k[9] = {
		matKd.at<double>(0,0), matKd.at<double>(0,1), matKd.at<double>(0,2),
		matKd.at<double>(1,0), matKd.at<double>(1,1), matKd.at<double>(1,2),
		matKd.at<double>(2,0), matKd.at<double>(2,1), matKd.at<double>(2,2)
	};

	const int Height = matSrc.rows;
	const int Width  = matSrc.cols;

	// 경계 확장도 수동으로 수행할 수 있으나 OpenCV 의 copyMakeBorder 를 재사용한다.
	cv::Mat matPad;
	cv::copyMakeBorder(matSrc, matPad, 1, 1, 1, 1, cv::BORDER_REPLICATE);

	dst.create(Height, Width, matSrc.type());
	cv::Mat matDst = dst.getMat();

	const uchar*  pSrc    = matPad.data;
	const size_t  srcStep = matPad.step;    // 바이트 단위 행 간격
	uchar*        pDst    = matDst.data;
	const size_t  dstStep = matDst.step;

	for (int y = 0; y < Height; ++y)
	{
		const uchar* r0 = pSrc + (y)     * srcStep;
		const uchar* r1 = pSrc + (y + 1) * srcStep;
		const uchar* r2 = pSrc + (y + 2) * srcStep;
		uchar*       dr = pDst + y       * dstStep;

		for (int x = 0; x < Width; ++x)
		{
			const double sum =
				r0[x]   * k[0] + r0[x+1] * k[1] + r0[x+2] * k[2] +
				r1[x]   * k[3] + r1[x+1] * k[4] + r1[x+2] * k[5] +
				r2[x]   * k[6] + r2[x+1] * k[7] + r2[x+2] * k[8];

			// saturate: 음수/255 초과를 직접 클램핑
			int v = static_cast<int>(sum + (sum >= 0 ? 0.5 : -0.5));
			if (v < 0)   v = 0;
			if (v > 255) v = 255;
			dr[x] = static_cast<uchar>(v);
		}
	}
}
#endif // KERNEL_IMPL_UCHAR

// ======================================================================
// [4] UINT 기반 구현 — 정수 고정소수점(8.8 스케일) 누산
// ======================================================================
#ifdef KERNEL_IMPL_UINT
void DlgVisionTest::ApplyConvolution3x3(cv::InputArray src, cv::OutputArray dst, cv::InputArray kernel)
{
	// 실수 계수를 256 배 스케일한 int 로 변환 → UINT/INT 누산기로 컨볼루션.
	// 부동소수점 없이 정수 연산만 사용 (임베디드/MCU 포팅 시 유용).
	cv::Mat matSrc = src.getMat();
	cv::Mat matK   = kernel.getMat();
	CV_Assert(matK.rows == 3 && matK.cols == 3);
	CV_Assert(!matSrc.empty() && matSrc.type() == CV_8UC1);

	cv::Mat matKd;
	matK.convertTo(matKd, CV_64F);

	// 커널을 8.8 고정소수점 정수(INT) 로 변환 — 음수 허용
	const int SHIFT = 8;                // 스케일 팩터: 2^8 = 256
	int kq[9];
	for (int i = 0; i < 9; ++i)
	{
		double v = matKd.at<double>(i / 3, i % 3) * (1 << SHIFT);
		kq[i] = static_cast<int>(v >= 0 ? v + 0.5 : v - 0.5);
	}

	const int Height = matSrc.rows;
	const int Width  = matSrc.cols;

	cv::Mat matPad;
	cv::copyMakeBorder(matSrc, matPad, 1, 1, 1, 1, cv::BORDER_REPLICATE);

	dst.create(Height, Width, matSrc.type());
	cv::Mat matDst = dst.getMat();

	for (int y = 0; y < Height; ++y)
	{
		const uchar* r0 = matPad.ptr<uchar>(y);
		const uchar* r1 = matPad.ptr<uchar>(y + 1);
		const uchar* r2 = matPad.ptr<uchar>(y + 2);
		uchar*       dr = matDst.ptr<uchar>(y);

		for (UINT x = 0; x < static_cast<UINT>(Width); ++x)
		{
			// INT 누산 (음수 커널 계수도 안전)
			int acc =
				static_cast<int>(r0[x])     * kq[0] +
				static_cast<int>(r0[x + 1]) * kq[1] +
				static_cast<int>(r0[x + 2]) * kq[2] +
				static_cast<int>(r1[x])     * kq[3] +
				static_cast<int>(r1[x + 1]) * kq[4] +
				static_cast<int>(r1[x + 2]) * kq[5] +
				static_cast<int>(r2[x])     * kq[6] +
				static_cast<int>(r2[x + 1]) * kq[7] +
				static_cast<int>(r2[x + 2]) * kq[8];

			// 8비트 스케일 복원 후 [0,255] 클램핑
			int v = acc >> SHIFT;
			if (v < 0)   v = 0;
			if (v > 255) v = 255;
			dr[x] = static_cast<uchar>(static_cast<UINT>(v) & 0xFFu);
		}
	}
}
#endif // KERNEL_IMPL_UINT

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

	// UI 값으로부터 커널 생성 후 파일로 저장 (영속화)
	cv::Mat kernel = BuildKernelFromUI();
	SaveKernelSettings();

	cv::Mat dst;
	// cv::filter2D 대신 직접 구현한 3x3 컨볼루션 사용
	ApplyConvolution3x3(*m_refMatProcessed, dst, kernel);

	*m_refMatProcessed = dst;

	// 영상처리 직후 결과(Result)로 뷰 전환 — 라디오 체크 상태도 함께 갱신한다.
	CheckRadioButton(IDC_RADIO_ORIGIN, IDC_RADIO_RESULT, IDC_RADIO_RESULT);
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
	}

	// EditBox 생성 후, 저장된 커널 값을 파일에서 읽어 복원한다.
	// (파일이 없거나 항목이 비어있으면 항등 커널이 기본값으로 채워진다.)
	LoadKernelSettings();
}

// ======================================================================
// 커널 설정 파일 입출력 — exe 폴더의 viewer.ini, [Kernel] 섹션, 키 K0..K8
// ======================================================================
CString DlgVisionTest::GetKernelSettingsPath() const
{
	// 부모(CMy01ViewerDlg)의 GetSettingsFilePath() 와 동일한 경로를 사용한다.
	TCHAR szPath[MAX_PATH] = { 0 };
	::GetModuleFileName(NULL, szPath, MAX_PATH);
	CString strPath(szPath);
	int nPos = strPath.ReverseFind(_T('\\'));
	if (nPos >= 0) strPath = strPath.Left(nPos + 1);
	strPath += _T("viewer.ini");
	return strPath;
}

void DlgVisionTest::LoadKernelSettings()
{
	CString strIni = GetKernelSettingsPath();
	LPCTSTR sec    = _T("Kernel");

	// 기본값: 항등(identity) 커널 — 원본 그대로 통과
	const double defaults[9] = {
		0, 0, 0,
		0, 1, 0,
		0, 0, 0
	};

	// 파일이 없으면 기본 커널을 UI 에 채워넣고 즉시 파일 생성
	if (::GetFileAttributes(strIni) == INVALID_FILE_ATTRIBUTES)
	{
		for (int i = 0; i < 9; ++i)
		{
			CString s;
			s.Format(_T("%g"), defaults[i]);
			if (m_edKernel[i].GetSafeHwnd() != NULL)
				m_edKernel[i].SetWindowText(s);
		}
		SaveKernelSettings();
		return;
	}

	// K0..K8 키에서 문자열을 읽어 EditBox 에 반영한다.
	// GetPrivateProfileString 은 문자열을 그대로 반환하므로 소수/음수/식(e.g. 1.5) 도 보존된다.
	for (int i = 0; i < 9; ++i)
	{
		CString key;
		key.Format(_T("K%d"), i);

		CString strDefault;
		strDefault.Format(_T("%g"), defaults[i]);

		TCHAR szBuf[64] = { 0 };
		::GetPrivateProfileString(
			sec, key, strDefault,
			szBuf, _countof(szBuf), strIni);

		if (m_edKernel[i].GetSafeHwnd() != NULL)
			m_edKernel[i].SetWindowText(szBuf);
	}
}

void DlgVisionTest::SaveKernelSettings()
{
	CString strIni = GetKernelSettingsPath();
	LPCTSTR sec    = _T("Kernel");

	// 현재 EditBox 에 입력된 문자열을 그대로 K0..K8 키로 저장한다.
	for (int i = 0; i < 9; ++i)
	{
		CString key, val;
		key.Format(_T("K%d"), i);

		if (m_edKernel[i].GetSafeHwnd() != NULL)
			m_edKernel[i].GetWindowText(val);
		else
			val = _T("0");

		::WritePrivateProfileString(sec, key, val, strIni);
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

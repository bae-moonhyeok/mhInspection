
// 01_ViewerDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "01_Viewer.h"
#include "01_ViewerDlg.h"
#include "afxdialogex.h"

#include "Constants.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMy01ViewerDlg 대화 상자

CMy01ViewerDlg::CMy01ViewerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MY01_VIEWER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_WndImageView = new lt::CImageViewEx;
}

void CMy01ViewerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_STATIC_VIEW, *m_WndImageView);
}

//bool CMy01ViewerDlg::CImageViewEx2ImageFile(const wchar_t* strFilePath, cv::Mat& matImage)
//{
//	if (matImage.empty()) return false;
//
//	// 파일 저장 대화상자 설정
//	TCHAR szFilter[] = _T("JPG 파일(*.jpg)|*.jpg|PNG 파일(*.png)|*.png|BMP 파일(*.bmp)|*.bmp|모든 파일(*.*)|*.*||");
//	CFileDialog dlg(FALSE, _T("jpg"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
//
//	if (dlg.DoModal() == IDOK)
//	{
//		CString strPath = dlg.GetPathName();
//
//		// CString을 std::string으로 변환 (OpenCV 대응)
//		CT2CA pszConvertedAnsiString(strPath);
//		std::string strPathStd(pszConvertedAnsiString);
//
//		if (cv::imwrite(strPathStd, matImage))
//		{
//			AfxMessageBox(_T("성공적으로 저장되었습니다."));
//		}
//		else
//		{
//			AfxMessageBox(_T("저장에 실패했습니다."));
//		}
//	}
//
//	return true;
//}
//
//bool CMy01ViewerDlg::CImageViewEx2ImageFile(UINT32* pData)
//{
//	if (pData == nullptr) return false;
//
//	CFileDialog dlg(FALSE, _T("png"), NULL, OFN_OVERWRITEPROMPT, _T("PNG 파일(*.png)|*.png||"));
//
//	if (dlg.DoModal() == IDOK)
//	{
//		// Raw 데이터를 Mat 객체로 생성 (복사하지 않고 주소만 참조)
//		// 8-bit, 1-channel 3-channel 4-channel (RGBA) 기준 예시
//	
//		CString pszPath(dlg.GetPathName());
//		if (m_WndImageView->SaveImage(pszPath))
//		{
//			AfxMessageBox(_T("저장 성공"));
//		}
//	}
//
//	return true;
//}
//
//bool CMy01ViewerDlg::CImageViewEx2ImageFile(const wchar_t* strFilePath, UINT32* pData)
//{
//	if (pData == nullptr) return false;
//
//	CFileDialog dlg(FALSE, _T("png"), NULL, OFN_OVERWRITEPROMPT, _T("PNG 파일(*.png)|*.png||"));
//
//	if (dlg.DoModal() == IDOK)
//	{
//		
//		// Raw 데이터를 Mat 객체로 생성 (복사하지 않고 주소만 참조)
//		// 8-bit, 1-channel 3-channel 4-channel (RGBA) 기준 예시
//		const int CV_TYPE = m_imageInfo.nBitPP == 32 ? CV_8UC4 :
//			m_imageInfo.nBitPP == 24 ? CV_8UC3 : CV_8UC1;
//		
//		cv::Mat matImage(image_cy, image_cx, CV_TYPE, pData);
//
//		CT2CA pszPath(dlg.GetPathName());
//		if (cv::imwrite((std::string)pszPath, matImage))
//		{
//			AfxMessageBox(_T("저장 성공"));
//		}
//	}
//
//	return true;
//}

BEGIN_MESSAGE_MAP(CMy01ViewerDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CMy01ViewerDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CMy01ViewerDlg::OnBnClickedCancel)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_SAVE, &CMy01ViewerDlg::OnBnClickedBtnSave)
	ON_BN_CLICKED(IDC_BTN_LOAD, &CMy01ViewerDlg::OnBnClickedBtnLoad)
END_MESSAGE_MAP()

// CMy01ViewerDlg 메시지 처리기

void CMy01ViewerDlg::UserInit()
{
	// 대화상자 크기를 조정합니다.
	::SetWindowPos(m_hWnd, NULL, 0, 0, monitor_cx, monitor_cy - taskbar_cy, SWP_NONE);

	CWnd* pWnd = GetDlgItem(IDC_BTN_LOAD);
	pWnd->SetWindowPos(NULL, margin_cx + view_cx + 5, caption_cy + 2, 48, 24, SWP_NONE);
	pWnd = GetDlgItem(IDC_BTN_SAVE);
	pWnd->SetWindowPos(NULL, margin_cx + view_cx + 5 + 48 + 5, caption_cy + 2, 48, 24, SWP_NONE);

	pWnd = GetDlgItem(IDC_STATIC_VIEW);
	pWnd->SetWindowPos(NULL, margin_cx, caption_cy + margin_cy, view_cx, view_cy, SWP_NONE);
	pWnd->GetWindowRect(&m_rtView);

	m_WndImageView->SetMinimumZoomRatio(100);
}

BOOL CMy01ViewerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	UserInit();

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CMy01ViewerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CMy01ViewerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMy01ViewerDlg::OnBnClickedOk() { /*CDialogEx::OnOK();*/ }
void CMy01ViewerDlg::OnBnClickedCancel() { CDialogEx::OnCancel(); }

void CMy01ViewerDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	delete m_WndImageView;
}

void CMy01ViewerDlg::OnBnClickedBtnSave()
{
	if (m_matCopy.empty())
	{
		AfxMessageBox(_T("먼저 이미지를 불러오세요."), MB_ICONWARNING);
		return;
	}

	TCHAR szFilter[] = _T("JPG 파일(*.jpg)|*.jpg|PNG 파일(*.png)|*.png|BMP 파일(*.bmp)|*.bmp|모든 파일(*.*)|*.*||");
	CFileDialog dlg(FALSE, _T("jpg"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, this);

	if (dlg.DoModal() != IDOK) return;

	CString strPath = dlg.GetPathName();
	CT2CA pszConvertedAnsiString(strPath);
	std::string strPathStd(pszConvertedAnsiString);

	if (cv::imwrite(strPathStd, m_matCopy))
		AfxMessageBox(_T("성공적으로 저장되었습니다."));
	else
		AfxMessageBox(_T("저장에 실패했습니다."));
}

//void CMy01ViewerDlg::ResizeImage(CDC* pDC, CImage& img)
//{
//	int iw = img.GetWidth(), ih = img.GetHeight();
//	double rx = (double)m_rtView.Width() / iw;
//	double ry = (double)m_rtView.Height() / ih;
//	double r = std::min(1.0, std::min(rx, ry));
//	int dw = (int)(iw * r), dh = (int)(ih * r);
//	int dx = m_rtView.left + (m_rtView.Width() - dw) / 2;
//	int dy = m_rtView.top + (m_rtView.Height() - dh) / 2;
//
//	pDC->SetStretchBltMode(HALFTONE);
//	::SetBrushOrgEx(pDC->GetSafeHdc(), 0, 0, NULL);
//	m_WndImageView->UpdateImageFromArray((PBYTE)img.GetBits(), rx, ry, m_imageInfo.nBitPP);
//}

void CMy01ViewerDlg::OnBnClickedBtnLoad()
{
	TCHAR szFilters[] = _T("Image Files (*.jpg;*.jpeg;*.png;*.bmp)|*.jpg;*.jpeg;*.png;*.bmp|All Files (*.*)|*.*||");
	CFileDialog fileDlg(TRUE, _T("jpg"), NULL, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilters, this);

	if (fileDlg.DoModal() != IDOK)
	{
		OutputDebugString(_T("사용자가 파일 선택을 취소했습니다.\n"));
		return;
	}

	CString strFilePath = fileDlg.GetPathName();

	if (GetFileAttributes(strFilePath) == INVALID_FILE_ATTRIBUTES)
	{
		AfxMessageBox(_T("선택한 파일 경로가 올바르지 않거나 파일이 존재하지 않습니다."), MB_ICONERROR);
		return;
	}

	CT2CA pszConvertedAnsiString(strFilePath);
	std::string strPathStd(pszConvertedAnsiString);

	cv::Mat matLoaded = cv::imread(strPathStd, cv::IMREAD_UNCHANGED);
	if (matLoaded.empty())
	{
		AfxMessageBox(_T("이미지를 로드하는 데 실패했습니다."), MB_ICONERROR);
		return;
	}

	// 불러온 직후 사본 이미지 생성 (저장 시 사용)
	m_matCopy = matLoaded.clone();

	// 1296*972 보다 큰 경우 이미지 크기 축소
	if (matLoaded.cols > view_cx || matLoaded.rows > view_cy)
	{
		double r = std::min((double)view_cx / matLoaded.cols, (double)view_cy / matLoaded.rows);
		cv::Mat matResized;
		cv::resize(matLoaded, matResized, cv::Size(), r, r, cv::INTER_AREA);
		matLoaded = matResized;
	}

	// 출력용: 컬러로 변환
	cv::Mat matDisp;
	if (matLoaded.channels() == 1)
		cv::cvtColor(matLoaded, matDisp, cv::COLOR_GRAY2BGR);
	else if (matLoaded.channels() == 4)
		cv::cvtColor(matLoaded, matDisp, cv::COLOR_BGRA2BGR);
	else
		matDisp = matLoaded;

	// CImageViewEx에 출력 (pitch 포함 bitmap array 사용)
	WORD wBpp = (WORD)(matDisp.channels() * 8);
	m_WndImageView->UpdateImageFromBitmapArray(
		matDisp.data, matDisp.cols, (LONG)matDisp.step, matDisp.rows, wBpp, TRUE);
	m_WndImageView->InvalidateDirect(FALSE);

	m_imageInfo = m_WndImageView->GetOverlayBitmapImageInfo_Color();
}


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

BEGIN_MESSAGE_MAP(CMy01ViewerDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CMy01ViewerDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CMy01ViewerDlg::OnBnClickedCancel)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_SAVE, &CMy01ViewerDlg::OnBnClickedBtnSave)
	ON_BN_CLICKED(IDC_BTN_LOAD, &CMy01ViewerDlg::OnBnClickedBtnLoad)
	ON_BN_CLICKED(IDC_BTN_RAW_APPLY, &CMy01ViewerDlg::OnBnClickedBtnRawApply)
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

	m_WndImageView->SetMinimumZoomRatio(100);

	LoadRawSettings();
	CreateRawSettingsUI();
	UpdateUIFromSettings();
}

CString CMy01ViewerDlg::GetSettingsFilePath() const
{
	TCHAR szPath[MAX_PATH] = { 0 };
	::GetModuleFileName(NULL, szPath, MAX_PATH);
	CString strPath(szPath);
	int nPos = strPath.ReverseFind(_T('\\'));
	if (nPos >= 0) strPath = strPath.Left(nPos + 1);
	strPath += _T("viewer.ini");
	return strPath;
}

void CMy01ViewerDlg::LoadRawSettings()
{
	CString strIni = GetSettingsFilePath();
	LPCTSTR sec = _T("Raw");

	if (::GetFileAttributes(strIni) == INVALID_FILE_ATTRIBUTES)
	{
		// 파일 없으면 기본값으로 생성
		m_rawSettings = RawImageSettings();
		SaveRawSettings();
		return;
	}

	m_rawSettings.nWidth         = ::GetPrivateProfileInt(sec, _T("Width"),        1280, strIni);
	m_rawSettings.nHeight        = ::GetPrivateProfileInt(sec, _T("Height"),        800, strIni);
	m_rawSettings.nBpp           = ::GetPrivateProfileInt(sec, _T("BPP"),            10, strIni);
	m_rawSettings.nCaptureFormat = ::GetPrivateProfileInt(sec, _T("CaptureFormat"),   0, strIni);
	m_rawSettings.nColorOrder    = ::GetPrivateProfileInt(sec, _T("ColorOrder"),      0, strIni);
}

void CMy01ViewerDlg::SaveRawSettings()
{
	CString strIni = GetSettingsFilePath();
	LPCTSTR sec = _T("Raw");
	CString v;
	v.Format(_T("%d"), m_rawSettings.nWidth);         ::WritePrivateProfileString(sec, _T("Width"),         v, strIni);
	v.Format(_T("%d"), m_rawSettings.nHeight);        ::WritePrivateProfileString(sec, _T("Height"),        v, strIni);
	v.Format(_T("%d"), m_rawSettings.nBpp);           ::WritePrivateProfileString(sec, _T("BPP"),           v, strIni);
	v.Format(_T("%d"), m_rawSettings.nCaptureFormat); ::WritePrivateProfileString(sec, _T("CaptureFormat"), v, strIni);
	v.Format(_T("%d"), m_rawSettings.nColorOrder);    ::WritePrivateProfileString(sec, _T("ColorOrder"),    v, strIni);
}

void CMy01ViewerDlg::CreateRawSettingsUI()
{
	const int x0 = margin_cx + view_cx + 5;
	const int y0 = caption_cy + 2 + 24 + 10;   // Load/Save 버튼 아래
	const int labelW = 90, editW = 90, rowH = 22, gap = 4;

	CRect rcGroup(x0, y0, x0 + labelW + editW + 10, y0 + rowH * 6 + 30);
	m_stRawGroup.Create(_T("RAW Image Settings"), WS_CHILD | WS_VISIBLE | SS_ETCHEDFRAME, rcGroup, this, IDC_STATIC_RAW_GROUP);

	int y = y0 + 10;

	// Width
	m_stRawWidth.Create(_T("Width:"), WS_CHILD | WS_VISIBLE | SS_LEFT, CRect(x0 + 6, y + 3, x0 + 6 + labelW, y + rowH), this, IDC_STATIC_RAW_WIDTH);
	m_edRawWidth.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_NUMBER | ES_AUTOHSCROLL,
		CRect(x0 + 6 + labelW, y, x0 + 6 + labelW + editW, y + rowH), this, IDC_EDIT_RAW_WIDTH);
	y += rowH + gap;

	// Height
	m_stRawHeight.Create(_T("Height:"), WS_CHILD | WS_VISIBLE | SS_LEFT, CRect(x0 + 6, y + 3, x0 + 6 + labelW, y + rowH), this, IDC_STATIC_RAW_HEIGHT);
	m_edRawHeight.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_NUMBER | ES_AUTOHSCROLL,
		CRect(x0 + 6 + labelW, y, x0 + 6 + labelW + editW, y + rowH), this, IDC_EDIT_RAW_HEIGHT);
	y += rowH + gap;

	// BPP
	m_stRawBpp.Create(_T("BPP:"), WS_CHILD | WS_VISIBLE | SS_LEFT, CRect(x0 + 6, y + 3, x0 + 6 + labelW, y + rowH), this, IDC_STATIC_RAW_BPP);
	m_edRawBpp.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_NUMBER | ES_AUTOHSCROLL,
		CRect(x0 + 6 + labelW, y, x0 + 6 + labelW + editW, y + rowH), this, IDC_EDIT_RAW_BPP);
	y += rowH + gap;

	// CaptureFormat
	m_stRawCapFmt.Create(_T("CaptureFormat:"), WS_CHILD | WS_VISIBLE | SS_LEFT, CRect(x0 + 6, y + 3, x0 + 6 + labelW, y + rowH), this, IDC_STATIC_RAW_CAPFMT);
	m_cmbRawCapFmt.Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_VSCROLL | CBS_DROPDOWNLIST,
		CRect(x0 + 6 + labelW, y, x0 + 6 + labelW + editW, y + rowH + 100), this, IDC_CMB_RAW_CAPFMT);
	m_cmbRawCapFmt.AddString(_T("Gray"));
	m_cmbRawCapFmt.AddString(_T("Bayer"));
	m_cmbRawCapFmt.AddString(_T("RGB"));
	m_cmbRawCapFmt.AddString(_T("YUV"));
	y += rowH + gap;

	// ColorOrder
	m_stRawColorOrder.Create(_T("ColorOrder:"), WS_CHILD | WS_VISIBLE | SS_LEFT, CRect(x0 + 6, y + 3, x0 + 6 + labelW, y + rowH), this, IDC_STATIC_RAW_COLORORDER);
	m_cmbRawColorOrder.Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_VSCROLL | CBS_DROPDOWNLIST,
		CRect(x0 + 6 + labelW, y, x0 + 6 + labelW + editW, y + rowH + 100), this, IDC_CMB_RAW_COLORORDER);
	m_cmbRawColorOrder.AddString(_T("None"));
	m_cmbRawColorOrder.AddString(_T("RGGB"));
	m_cmbRawColorOrder.AddString(_T("BGGR"));
	m_cmbRawColorOrder.AddString(_T("GRBG"));
	m_cmbRawColorOrder.AddString(_T("GBRG"));
	m_cmbRawColorOrder.AddString(_T("RGB"));
	m_cmbRawColorOrder.AddString(_T("BGR"));
	y += rowH + gap;

	// Apply 버튼
	m_btnRawApply.Create(_T("Apply && Save"), WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON,
		CRect(x0 + 6 + labelW, y + 4, x0 + 6 + labelW + editW, y + 4 + rowH), this, IDC_BTN_RAW_APPLY);

	// 글꼴 적용
	CFont* pFont = GetFont();
	m_stRawWidth.SetFont(pFont);       m_edRawWidth.SetFont(pFont);
	m_stRawHeight.SetFont(pFont);      m_edRawHeight.SetFont(pFont);
	m_stRawBpp.SetFont(pFont);         m_edRawBpp.SetFont(pFont);
	m_stRawCapFmt.SetFont(pFont);      m_cmbRawCapFmt.SetFont(pFont);
	m_stRawColorOrder.SetFont(pFont);  m_cmbRawColorOrder.SetFont(pFont);
	m_btnRawApply.SetFont(pFont);
	m_stRawGroup.SetFont(pFont);
}

void CMy01ViewerDlg::UpdateUIFromSettings()
{
	CString v;
	v.Format(_T("%d"), m_rawSettings.nWidth);  m_edRawWidth.SetWindowText(v);
	v.Format(_T("%d"), m_rawSettings.nHeight); m_edRawHeight.SetWindowText(v);
	v.Format(_T("%d"), m_rawSettings.nBpp);    m_edRawBpp.SetWindowText(v);
	m_cmbRawCapFmt.SetCurSel(m_rawSettings.nCaptureFormat);
	m_cmbRawColorOrder.SetCurSel(m_rawSettings.nColorOrder);
}

bool CMy01ViewerDlg::UpdateSettingsFromUI()
{
	CString sW, sH, sB;
	m_edRawWidth.GetWindowText(sW);
	m_edRawHeight.GetWindowText(sH);
	m_edRawBpp.GetWindowText(sB);

	int w = _ttoi(sW), h = _ttoi(sH), b = _ttoi(sB);
	if (w <= 0 || h <= 0 || b <= 0 || b > 32)
	{
		AfxMessageBox(_T("Width/Height/BPP 값이 올바르지 않습니다."), MB_ICONWARNING);
		return false;
	}

	m_rawSettings.nWidth         = w;
	m_rawSettings.nHeight        = h;
	m_rawSettings.nBpp           = b;
	m_rawSettings.nCaptureFormat = m_cmbRawCapFmt.GetCurSel();
	m_rawSettings.nColorOrder    = m_cmbRawColorOrder.GetCurSel();
	return true;
}

void CMy01ViewerDlg::OnBnClickedBtnRawApply()
{
	if (!UpdateSettingsFromUI()) return;
	SaveRawSettings();
	AfxMessageBox(_T("설정이 저장되었습니다."));
}

bool CMy01ViewerDlg::LoadRawFile(LPCTSTR szPath, cv::Mat& matOut)
{
	CFile file;
	CFileException ex;
	if (!file.Open(szPath, CFile::modeRead | CFile::typeBinary, &ex))
		return false;

	const int w = m_rawSettings.nWidth;
	const int h = m_rawSettings.nHeight;
	const int bpp = m_rawSettings.nBpp;
	const int bytesPerPx = (bpp <= 8) ? 1 : 2;
	const size_t total = (size_t)w * h * bytesPerPx;

	if ((ULONGLONG)file.GetLength() < total)
	{
		file.Close();
		AfxMessageBox(_T("RAW 파일 크기가 설정과 일치하지 않습니다."), MB_ICONERROR);
		return false;
	}

	std::vector<BYTE> buf(total);
	file.Read(buf.data(), (UINT)total);
	file.Close();

	const int type = (bytesPerPx == 1) ? CV_8UC1 : CV_16UC1;
	cv::Mat mat(h, w, type, buf.data());

	// 8-bit 로 정규화
	cv::Mat mat8;
	if (bytesPerPx == 2)
	{
		double scale = 255.0 / ((1 << bpp) - 1);
		mat.convertTo(mat8, CV_8UC1, scale);
	}
	else
	{
		mat8 = mat.clone();
	}

	// Bayer 디모자이크
	if (m_rawSettings.nCaptureFormat == 1)
	{
		int code = cv::COLOR_BayerBG2BGR;
		switch (m_rawSettings.nColorOrder)
		{
		case 1: code = cv::COLOR_BayerRG2BGR; break;  // RGGB
		case 2: code = cv::COLOR_BayerBG2BGR; break;  // BGGR
		case 3: code = cv::COLOR_BayerGR2BGR; break;  // GRBG
		case 4: code = cv::COLOR_BayerGB2BGR; break;  // GBRG
		default: code = cv::COLOR_BayerBG2BGR; break;
		}
		cv::cvtColor(mat8, matOut, code);
	}
	else
	{
		matOut = mat8.clone();
	}

	return true;
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

	TCHAR szFilter[] = _T("BMP 파일(*.bmp)|*.bmp|JPG 파일(*.jpg)|*.jpg|PNG 파일(*.png)|*.png|모든 파일(*.*)|*.*||");
	CTime time = CTime::GetCurrentTime();
	CString strPath = time.Format(_T("%m%d%H%M_"));
	CFileDialog dlg(FALSE, _T("bmp"), strPath, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, this);

	if (dlg.DoModal() != IDOK) return;

	strPath = dlg.GetPathName();
	CT2CA pszConvertedAnsiString(strPath);
	std::string strPathStd(pszConvertedAnsiString);

	if (cv::imwrite(strPathStd, m_matCopy))
		AfxMessageBox(_T("성공적으로 저장되었습니다."));
	else
		AfxMessageBox(_T("저장에 실패했습니다."));
}

void CMy01ViewerDlg::OnBnClickedBtnLoad()
{
	TCHAR szFilters[] = _T("Image Files (*.jpg;*.jpeg;*.png;*.bmp;*.raw)|*.jpg;*.jpeg;*.png;*.bmp;*.raw|RAW Files (*.raw)|*.raw|All Files (*.*)|*.*||");
	CFileDialog fileDlg(TRUE, _T("bmp"), NULL, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilters, this);

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

	CString strExt = fileDlg.GetFileExt();
	strExt.MakeLower();

	cv::Mat matLoaded;
	if (strExt == _T("raw"))
	{
		if (!LoadRawFile(strFilePath, matLoaded))
			return;
	}
	else
	{
		CT2CA pszConvertedAnsiString(strFilePath);
		std::string strPathStd(pszConvertedAnsiString);
		matLoaded = cv::imread(strPathStd, cv::IMREAD_UNCHANGED);
	}

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

	//m_imageInfo = m_WndImageView->GetOverlayBitmapImageInfo_Color();
}

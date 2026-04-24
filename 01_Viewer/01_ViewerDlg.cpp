
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
	DDX_Control(pDX, IDC_LIST_LOG, m_listLog);
	//DDX_Control(pDX, IDC_CHECK_KEEP_IMAGE, m_chkKeepImage);
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
	ON_BN_CLICKED(IDC_BTN_DLG, &CMy01ViewerDlg::OnBnClickedBtnDlg)
END_MESSAGE_MAP()

// CMy01ViewerDlg 메시지 처리기

void CMy01ViewerDlg::UserInit()
{
	// 대화상자 크기를 조정합니다.
	::SetWindowPos(m_hWnd, NULL, 0, 0, monitor_cx, monitor_cy - taskbar_cy, SWP_NONE);

	CWnd* pWnd = GetDlgItem(IDC_BTN_LOAD);
	pWnd->SetWindowPos(NULL, margin_cx + view_cx + 5, caption_cy + 2, 120, 80, SWP_NONE);
	pWnd = GetDlgItem(IDC_BTN_SAVE);
	pWnd->SetWindowPos(NULL, margin_cx + view_cx + 5 + 120 + 5, caption_cy + 2, 120, 80, SWP_NONE);

	pWnd = GetDlgItem(IDC_STATIC_VIEW);
	pWnd->SetWindowPos(NULL, margin_cx, caption_cy + margin_cy, view_cx, view_cy, SWP_NONE);

	pWnd = GetDlgItem(IDC_BTN_DLG);
	pWnd->SetWindowPos(NULL, monitor_cx - margin_cx - 120 - 14, -taskbar_cy + monitor_cy - margin_cy - 120, 120, 80, SWP_NONE);

	pWnd = GetDlgItem(IDC_LIST_LOG);
	pWnd->SetWindowPos(NULL, monitor_cx - margin_cx - 560 - 14, -taskbar_cy + monitor_cy - margin_cy - 360, 560, 80, SWP_NONE);

	// IDC_CHECK_KEEP_IMAGE: 자식 대화상자 이미지 자동 정리를 억제할지 여부
	// 위치는 IDC_BTN_DLG 버튼 바로 위. 기본값은 해제(unchecked).
	pWnd = GetDlgItem(IDC_CHECK_KEEP_IMAGE);
	if (pWnd != nullptr)
	{
		pWnd->SetWindowPos(NULL,
			monitor_cx - margin_cx - 120 - 14,
			-taskbar_cy + monitor_cy - margin_cy - 120 - 30,
			120, 24, SWP_NONE);
		pWnd->SetWindowText(_T("Keep Image"));
		m_chkKeepImage.SetCheck(BST_UNCHECKED);
	}

	m_WndImageView->SetMinimumZoomRatio(100);

	LoadRawSettings();
	CreateRawSettingsUI();
	UpdateUIFromSettings();

	AddLog(_T("Viewer ready."));
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
	m_rawSettings.nColorOrder    = ::GetPrivateProfileInt(sec, _T("ColorOrder"),      0, strIni);
}

void CMy01ViewerDlg::SaveRawSettings()
{
	CString strIni = GetSettingsFilePath();
	LPCTSTR sec = _T("Raw");
	CString v;
	v.Format(_T("%d"), m_rawSettings.nWidth);         ::WritePrivateProfileString(sec, _T("Width"),         v, strIni);
	v.Format(_T("%d"), m_rawSettings.nHeight);        ::WritePrivateProfileString(sec, _T("Height"),        v, strIni);
	v.Format(_T("%d"), m_rawSettings.nColorOrder);    ::WritePrivateProfileString(sec, _T("ColorOrder"),    v, strIni);
}

void CMy01ViewerDlg::CreateRawSettingsUI()
{
	const int x0 = margin_cx + view_cx + 5;
	const int y0 = caption_cy + 2 + 80 + 10;   // Load/Save 버튼 아래
	// 크기 1.5배 확대 (labelW 90→135, editW 90→135, rowH 22→33, gap 4→6)
	const int labelW = 135, editW = 135, rowH = 33, gap = 6;

	CRect rcGroup(x0, y0, x0 + labelW + editW + 15, y0 + rowH * 6 + 63);
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

	// ColorOrder
	m_stRawColorOrder.Create(_T("ColorOrder:"), WS_CHILD | WS_VISIBLE | SS_LEFT, CRect(x0 + 6, y + 3, x0 + 6 + labelW, y + rowH), this, IDC_STATIC_RAW_COLORORDER);
	m_cmbRawColorOrder.Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_VSCROLL | CBS_DROPDOWNLIST,
		CRect(x0 + 6 + labelW, y, x0 + 6 + labelW + editW, y + rowH + 100), this, IDC_CMB_RAW_COLORORDER);
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
	m_stRawColorOrder.SetFont(pFont);  m_cmbRawColorOrder.SetFont(pFont);
	m_btnRawApply.SetFont(pFont);
	m_stRawGroup.SetFont(pFont);
}

void CMy01ViewerDlg::UpdateUIFromSettings()
{
	CString v;
	v.Format(_T("%d"), m_rawSettings.nWidth);  m_edRawWidth.SetWindowText(v);
	v.Format(_T("%d"), m_rawSettings.nHeight); m_edRawHeight.SetWindowText(v);
	m_cmbRawColorOrder.SetCurSel(m_rawSettings.nColorOrder);
}

bool CMy01ViewerDlg::UpdateSettingsFromUI()
{
	CString sW, sH, sB;
	m_edRawWidth.GetWindowText(sW);
	m_edRawHeight.GetWindowText(sH);

	int w = _ttoi(sW), h = _ttoi(sH);
	if (w <= 0 || h <= 0)
	{
		AfxMessageBox(_T("Width/Height 값이 올바르지 않습니다."), MB_ICONWARNING);
		return false;
	}

	m_rawSettings.nWidth         = w;
	m_rawSettings.nHeight        = h;
	m_rawSettings.nColorOrder    = m_cmbRawColorOrder.GetCurSel();
	return true;
}

void CMy01ViewerDlg::OnBnClickedBtnRawApply()
{
	if (!UpdateSettingsFromUI()) return;
	SaveRawSettings();
	AfxMessageBox(_T("설정이 저장되었습니다."));

	CString log;
	log.Format(_T("[RAW] Settings saved (W=%d, H=%d, ColorOrder=%d)"),
		m_rawSettings.nWidth, m_rawSettings.nHeight, m_rawSettings.nColorOrder);
	AddLog(log);
}

bool CMy01ViewerDlg::LoadRawFile(LPCTSTR szPath, cv::Mat& matOut)
{
	CFile file;
	CFileException ex;
	if (!file.Open(szPath, CFile::modeRead | CFile::typeBinary, &ex))
		return false;

	const int w = m_rawSettings.nWidth;
	const int h = m_rawSettings.nHeight;
	const size_t total = (size_t)w * h;

	if ((ULONGLONG)file.GetLength() < total)
	{
		file.Close();
		AfxMessageBox(_T("RAW 파일 크기가 설정과 일치하지 않습니다."), MB_ICONERROR);
		return false;
	}

	std::vector<BYTE> buf(total);
	file.Read(buf.data(), (UINT)total);
	file.Close();

	const int type = CV_8UC1;
	cv::Mat mat(h, w, type, buf.data());

	matOut = mat.clone();

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

const cv::Mat& CMy01ViewerDlg::GetViewImage()
{
	// 자식 대화상자가 없으면 항상 원본(m_matCopy) 반환 — nullptr 역참조 방지
	if (m_DlgVisionTest != nullptr &&
		eViewTarget::Result == m_DlgVisionTest->GetViewTarget())
	{
		return m_matProcessed;
	}
	return m_matCopy;
}

void CMy01ViewerDlg::OnBnClickedBtnSave()
{
	const cv::Mat& refMat = GetViewImage();

	if (refMat.empty())
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

	if (cv::imwrite(strPathStd, refMat))
	{
		AfxMessageBox(_T("성공적으로 저장되었습니다."));
		CString log;
		log.Format(_T("[Save] %s"), (LPCTSTR)strPath);
		AddLog(log);
	}
	else
	{
		AfxMessageBox(_T("저장에 실패했습니다."));
		AddLog(_T("[Save] Failed."));
	}
}

void CMy01ViewerDlg::RefreshInpsectionDialogImage(size_t bytes)
{
	if (bytes != m_ProcessedImageSizeInBytes)
	{
		InitProcessedImage();
	}
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
		AddLog(_T("[Load] Failed."));
		return;
	}

	// --------------------------------------------------------------
	// 자식 대화상자 이미지 정리 — KEEP_IMAGE 해제 상태일 때만 BTN_LOAD 시점에 수행.
	//   체크 상태이면 기존 처리 이미지(m_matProcessed)를 유지한다.
	// --------------------------------------------------------------
	if (!IsKeepImage())
	{
		// 새 이미지로 교체되므로 기존 ImageBuffer 와 processed 를 비운다.
		delete[] m_ImageBuffer;
		m_ImageBuffer = nullptr;

		m_matProcessed.release();
		m_ProcessedImageSizeInBytes = 0;
		AddLog(_T("[Load] KEEP_IMAGE=OFF → processed image cleared"));
	}
	else
	{
		AddLog(_T("[Load] KEEP_IMAGE=ON → processed image preserved"));
	}

	// 불러온 직후 사본 이미지 생성 (저장 시 사용)
	m_matCopy = matLoaded.clone();

	// 로드된 이미지 정보로 RAW 설정(m_rawSettings) 및 UI 갱신
	m_rawSettings.nWidth  = matLoaded.cols;
	m_rawSettings.nHeight = matLoaded.rows;
	// ColorOrder: 채널 수에 따라 None(0)/RGB(1)/BGR(2) 추정 — OpenCV는 기본 BGR
	if (matLoaded.channels() == 1)      m_rawSettings.nColorOrder = 0; // None
	else if (matLoaded.channels() >= 3) m_rawSettings.nColorOrder = 2; // BGR
	UpdateUIFromSettings();

	// 1296*972 보다 큰 경우 이미지 크기 축소
	if (matLoaded.cols > view_cx || matLoaded.rows > view_cy)
	{
		double ratio = std::min((double)view_cx / matLoaded.cols, (double)view_cy / matLoaded.rows);
		cv::Mat matResized;
		cv::resize(matLoaded, matResized, cv::Size(), ratio, ratio, cv::INTER_AREA);
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
	//m_WndImageView->UpdateImageFromBitmapArray(
	//	matDisp.data, matDisp.cols, (LONG)matDisp.step, matDisp.rows, wBpp, TRUE);
	m_WndImageView->UpdateImageFromArray(
		matDisp.data, matDisp.cols, matDisp.rows, wBpp, FALSE);
	m_WndImageView->InvalidateDirect(FALSE);

	// 자식 대화상자가 생성된 경우 RESULT를 위한 처리
	if (m_DlgVisionTest)
		RefreshInpsectionDialogImage(matLoaded.total());

	// 로그: 파일 경로 + 해상도
	CString log;
	log.Format(_T("[Load] %s (%dx%d, ch=%d)"),
		(LPCTSTR)strFilePath, matLoaded.cols, matLoaded.rows, matLoaded.channels());
	AddLog(log);
}

void CMy01ViewerDlg::InitProcessedImage()
{
	m_matProcessed = m_matCopy.clone();
	if (m_matProcessed.channels() == 3)
		cv::cvtColor(m_matProcessed, m_matProcessed, cv::COLOR_BGR2GRAY);
	//if (m_matCopy.channels() == 3)
	//	cv::cvtColor(m_matCopy, m_matProcessed, cv::COLOR_RGB2GRAY);

	m_ProcessedImageSizeInBytes = m_matProcessed.rows * m_matProcessed.cols * m_matProcessed.elemSize1();
}

void CMy01ViewerDlg::OnBnClickedBtnDlg()
{
	if (m_DlgVisionTest == nullptr)
	{
		// 자식 대화상자 메모리를 할당하고 모달리스(modeless)로 생성한다.
		m_DlgVisionTest = new DlgVisionTest(this);
		if (!m_DlgVisionTest->Create(IDD_DLG_VISION_TEST, this))
		{
			// 생성 실패 시 정리
			delete m_DlgVisionTest;
			m_DlgVisionTest = nullptr;
			AfxMessageBox(_T("자식 대화상자를 생성할 수 없습니다."), MB_ICONERROR);
			return;
		}

		// 자식 대화상자에서 사용할 RESULT 이미지를 초기화한다.
		InitProcessedImage();

		// 자식 대화상자가 m_matProcessed 에 직접 접근할 수 있도록 참조를 넘긴다.
		m_DlgVisionTest->SetProcessedMatRef(&m_matProcessed);
	}

	// 이미 존재하거나 새로 생성된 경우 화면에 표시한다.
	m_DlgVisionTest->ShowWindow(SW_SHOW);
	m_DlgVisionTest->SetForegroundWindow();

	AddLog(_T("[Child] Vision test dialog opened"));
}

bool CMy01ViewerDlg::IsKeepImage() const
{
	// 체크박스 윈도우가 아직 생성되지 않았다면(초기화 전/리소스 누락) 기본값 false.
	if (m_chkKeepImage.GetSafeHwnd() == NULL) return false;
	return m_chkKeepImage.GetCheck() == BST_CHECKED;
}

void CMy01ViewerDlg::CleanUpInpsectionDialog()
{
	// --------------------------------------------------------------
	// 자식 대화상자 종료 경로
	// --------------------------------------------------------------
	//  - KEEP_IMAGE 체크 상태와 무관하게, 여기서는 '다이얼로그 윈도우' 만 파괴한다.
	//  - 처리 이미지(m_matProcessed, m_ImageBuffer)는 보존한다.
	//    (이전 정책: 종료 시 항상 정리 → 현재 정책: 종료 시 정리하지 않음)
	//  - 이미지 정리는 OnBnClickedBtnLoad() 시점에서 KEEP_IMAGE 해제 상태일 때만 수행.
	// --------------------------------------------------------------
	const bool bKeep = IsKeepImage();

	if (m_DlgVisionTest != nullptr)
	{
		m_DlgVisionTest->DestroyWindow();
		delete m_DlgVisionTest;
		m_DlgVisionTest = nullptr;
	}

	if (!bKeep)
	{
		// 보존 플래그가 꺼져있고, 자식 대화상자가 실제 정리 대상이었다면
		// 참조용 이미지 버퍼만 해제한다. (cv::Mat 자체는 다음 Load 때까지 보존)
		delete[] m_ImageBuffer;
		m_ImageBuffer = nullptr;
	}

	// 자식 대화상자 종료 후 메인 뷰(IDC_STATIC_VIEW)를 원본 이미지로 갱신한다.
	// — 직전에 Result 로 출력 중이던 내용을 초기화하고 Origin 상태로 복귀.
	if (!m_matCopy.empty())
	{
		WORD wBpp = (WORD)(m_matCopy.channels() * 8);
		m_WndImageView->UpdateImageFromArray(
			m_matCopy.data, m_matCopy.cols, m_matCopy.rows, wBpp, FALSE);
		m_WndImageView->InvalidateDirect(FALSE);
	}

	AddLog(bKeep
		? _T("[Child] Closed (KEEP_IMAGE=ON, processed kept)")
		: _T("[Child] Closed (KEEP_IMAGE=OFF)"));
}

// ======================================================================
// [설계 전용] IDC_BTN_CLONE_ORIGIN — 자식 대화상자 이미지 사본(m_matProcessed)
// 을 원본(m_matCopy) 으로부터 다시 클론하여 덮어쓰는 리셋 기능.
// 구현은 아직 없으며 아래 절차만 주석으로 남긴다 (요구사항: "설계하여 주석만 남길 것").
// ----------------------------------------------------------------------
// 설계 요약:
//   1) 전제: m_matCopy 가 비어있지 않을 것 (원본이 로드된 상태)
//   2) InitProcessedImage() 재호출 → m_matProcessed = m_matCopy.clone() + gray 변환
//   3) 자식 대화상자가 존재한다면 SetProcessedMatRef(&m_matProcessed) 재바인딩
//      (새 Mat 인스턴스로 교체되어 기존 포인터가 가리키는 데이터가 달라질 수 있으므로 재설정)
//   4) 현재 자식의 ViewTarget 이 Result 이면 즉시 재출력(WM_UPDATE_VIEW)
//   5) AddLog(_T("[Clone] m_matProcessed ← m_matCopy"))
//
// 동작 정책:
//   - IDC_CHECK_KEEP_IMAGE 상태와 무관하게 사용자가 명시적으로 요청한 시점이므로 항상 수행.
//   - 누적된 컨볼루션 결과가 폐기되므로, 필요 시 확인 다이얼로그(MessageBox Yes/No) 고려.
//
// 구현 예정 시그니처:
//   afx_msg void OnBnClickedBtnCloneOrigin();
//   ON_BN_CLICKED(IDC_BTN_CLONE_ORIGIN, &CMy01ViewerDlg::OnBnClickedBtnCloneOrigin)
// ======================================================================

LRESULT CMy01ViewerDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_CLOSE_VISION_TEST_DLG)
	{
		CleanUpInpsectionDialog();
		AddLog(TEXT("자식 대화 상자가 종료되었습니다."));
	}
	else if (message == WM_APPEND_LOG)
	{
		// 자식 대화상자(DlgVisionTest) 가 보낸 로그 요청.
		// LPARAM 은 LPCTSTR 문자열 포인터 — SendMessage 동기 경로라 포인터 유효성 보장.
		LPCTSTR sz = reinterpret_cast<LPCTSTR>(lParam);
		if (sz != nullptr) AddLog(sz);
	}
	else if (message == WM_UPDATE_VIEW)
	{
		// 자식 대화상자로부터 이미지 출력 요청 받음
		if (static_cast<eViewTarget>(wParam) == eViewTarget::Origin)
		{
			// 원본(m_matCopy) 출력
			WORD wBpp = (WORD)(m_matCopy.channels() * 8);
			m_WndImageView->UpdateImageFromArray(
				m_matCopy.data, m_matCopy.cols, m_matCopy.rows, wBpp, FALSE);
		}
		else
		{
			// 결과(m_matProcessed) 출력 — filter2D 결과 반영
			if (!m_matProcessed.empty())
			{
				WORD wBpp = (WORD)(m_matProcessed.channels() * 8);
				m_WndImageView->UpdateImageFromArray(
					m_matProcessed.data, m_matProcessed.cols, m_matProcessed.rows, wBpp, FALSE);
			}
		}
		m_WndImageView->InvalidateDirect(FALSE);
	}

	return CDialogEx::WindowProc(message, wParam, lParam);
}

void CMy01ViewerDlg::AddLog(const wchar_t* str)
{
/* 초 단위 */
	// 1. 현재 시간 객체 생성
	CTime now = CTime::GetCurrentTime();

	// 2. 원하는 포맷으로 문자열 생성
	// 결과 예: "현재 시간: 2026-04-23 16:21:39"
	m_strTime.Format(_T("[%s] %s"), 
		now.Format(_T("%Y-%m-%d %H:%M:%S")), str);
	m_listLog.InsertString(-1, m_strTime);

/*밀리초 단위*/
	// 1. 시스템 시간 구조체 선언 및 값 얻기
	SYSTEMTIME st;
	GetLocalTime(&st);

	// 2. CTime 객체로 변환 (날짜/시간 계산이 필요한 경우)
	CTime currentTime(st);

	// 3. 밀리초를 포함하여 포맷팅
	m_strTime.Format(_T("[%04d-%02d-%02d %02d:%02d:%02d.%03d] %s"),
		st.wYear, st.wMonth, st.wDay,
		st.wHour, st.wMinute, st.wSecond, st.wMilliseconds, // wMilliseconds가 ms 단위입니다.
		str);

	int index = m_listLog.InsertString(-1, m_strTime);
	m_listLog.SetCurSel(index);
}
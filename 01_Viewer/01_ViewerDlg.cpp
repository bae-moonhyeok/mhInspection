
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
	
}

void CMy01ViewerDlg::OnBnClickedBtnLoad()
{
	
}

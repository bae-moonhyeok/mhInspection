
// 01_ViewerDlg.h: 헤더 파일
//

#pragma once

#include "ImageViewEx.h"

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

private:
	lt::CImageViewEx* m_WndImageView = nullptr;
};

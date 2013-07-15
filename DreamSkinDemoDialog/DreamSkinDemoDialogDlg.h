
// DreamSkinDemoDialogDlg.h : header file
//

#pragma once
#include "afxwin.h"


// CDreamSkinDemoDialogDlg dialog
class CDreamSkinDemoDialogDlg : public CDialog
{
// Construction
public:
	CDreamSkinDemoDialogDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_DREAMSKINDEMODIALOG_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	void UpdateDreamSkinStatus();

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	LRESULT DefWindowProcBtnDemo(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	LRESULT DefWindowProcChkDemo(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	BOOL m_bSysMenuInclude;
	BOOL m_bEnableDreamSkin;
	BOOL m_bMaximizeBoxInclude;
	BOOL m_bMinimizeBoxInclude;
	BOOL m_bTitleBarInclude;
	BOOL m_bBorderInclude;
	BOOL m_bBorderSizable;
	BOOL m_bIconInclude;

	UINT m_nHookedWindowCount;
	CString m_strSkinPath;

	afx_msg void OnBnClickedSysmenuInclude();
	afx_msg void OnBnClickedDreamskinEnable();
	afx_msg void OnBnClickedMaximizeboxInclude();
	afx_msg void OnBnClickedMinimizeboxInclude();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedTitlebarInclude();
	afx_msg void OnBnClickedBorderInclude();
	afx_msg void OnBnClickedBorderSizable();
	afx_msg void OnBnClickedIconInclude();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedBtnLoadskin();
	
protected:
	WNDPROC	m_SkinProcBtnDemo;
	WNDPROC	m_SkinProcChk2StateDemo;
	WNDPROC	m_SkinProcChk3StateDemo;
	int     m_nBtnDemoStatus;
	int     m_nChk2StateDemoStatus;
	int     m_nChk3StateDemoStatus;
};

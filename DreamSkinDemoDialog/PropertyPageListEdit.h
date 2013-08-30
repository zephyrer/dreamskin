#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CPropertyPageListEdit dialog

class CPropertyPageListEdit : public CPropertyPage
{
	DECLARE_DYNAMIC(CPropertyPageListEdit)

public:
	CPropertyPageListEdit();
	virtual ~CPropertyPageListEdit();

// Dialog Data
	enum { IDD = IDD_DLG_LIST_EDIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	BOOL m_bEnableListBox;
	CString m_strResultListBox;
	CListBox m_lsDemoListBox;
	CListBox m_lsMultiColListBox;
	CString m_strResultListCtrl;
	CString m_strListBoxAdd;
	
	BOOL m_bEnableListCtrl;
	int m_nListCtrlView;
	CListCtrl m_lsDemoListCtrl;
	UINT     m_nListCtrlItemID;

	void UpdateListBoxWindows();

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedChkListboxEnable();
	afx_msg void OnLbnSelchangeListboxDemo();
	afx_msg void OnEnChangeEditListboxAdd();
	afx_msg void OnBnClickedBtnListboxAdd();
	afx_msg void OnBnClickedBtnListboxDel();
	afx_msg void OnLbnSelchangeListboxDemoMulticol();
	afx_msg void OnBnClickedChkListctrlEnable();
	afx_msg void OnCbnSelchangeCmbListctrlView();
	afx_msg void OnLvnItemchangedListctrlDemo(NMHDR *pNMHDR, LRESULT *pResult);
	BOOL m_bListCtrlFullRowSel;
	afx_msg void OnBnClickedListctrlFullrowsel();
};

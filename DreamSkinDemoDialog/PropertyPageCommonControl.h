#pragma once
#include "afxwin.h"


// CPropertyPageCommonControl dialog

class CPropertyPageCommonControl : public CPropertyPage
{
	DECLARE_DYNAMIC(CPropertyPageCommonControl)

public:
	CPropertyPageCommonControl();
	virtual ~CPropertyPageCommonControl();

// Dialog Data
	enum { IDD = IDD_DLG_COMMON_CONTROL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	BOOL m_bEnableAll;

	BOOL m_bEnableComboBox;
	CString m_strResultComboBox;
	CComboBox m_cmbDemoSimple;
	CComboBox m_cmbDemoDropdown;
	CComboBox m_cmbDemoDropList;

	virtual BOOL OnInitDialog();

protected:
	void    EnableComboBox(BOOL bEnable);
	void    EnableAllStatusUpdate();
public:
	afx_msg void OnBnClickedChkEnableCombobox();
	afx_msg void OnBnClickedChkEnableAll();
	afx_msg void OnBnClickedBtnClearCombobox();
	afx_msg void OnCbnSelchangeCmdDemoDroplist();
	afx_msg void OnCbnSelchangeCmdDemoDropdown();
	afx_msg void OnCbnEditchangeCmdDemoDropdown();
	afx_msg void OnCbnSelchangeCmbDemoSimple();
	afx_msg void OnCbnEditchangeCmbDemoSimple();
};

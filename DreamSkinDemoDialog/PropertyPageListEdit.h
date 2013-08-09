#pragma once
#include "afxwin.h"


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
	CString m_strResultListBox;
	CListBox m_lsDemoListBox;
	virtual BOOL OnInitDialog();
	BOOL m_bEnableListBox;
	afx_msg void OnBnClickedChkListboxEnable();
	afx_msg void OnLbnSelchangeListboxDemo();
};

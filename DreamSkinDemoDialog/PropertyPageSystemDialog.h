#pragma once


// CPropertyPageSystemDialog dialog

class CPropertyPageSystemDialog : public CPropertyPage
{
	DECLARE_DYNAMIC(CPropertyPageSystemDialog)

public:
	CPropertyPageSystemDialog();
	virtual ~CPropertyPageSystemDialog();

// Dialog Data
	enum { IDD = IDD_DLG_SYSTEM_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_strResultMessageBox;
	int m_nMessageBoxType;
	afx_msg void OnBnClickedBtnMessagebox();
};

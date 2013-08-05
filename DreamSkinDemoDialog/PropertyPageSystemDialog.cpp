// PropertyPageSystemDialog.cpp : implementation file
//

#include "stdafx.h"
#include "DreamSkinDemoDialog.h"
#include "PropertyPageSystemDialog.h"


// CPropertyPageSystemDialog dialog

IMPLEMENT_DYNAMIC(CPropertyPageSystemDialog, CPropertyPage)

CPropertyPageSystemDialog::CPropertyPageSystemDialog()
	: CPropertyPage(CPropertyPageSystemDialog::IDD)
	, m_strResultMessageBox(_T(""))
	, m_nMessageBoxType(0)
{
	m_psp.dwFlags &= ~PSP_HASHELP;
}

CPropertyPageSystemDialog::~CPropertyPageSystemDialog()
{
}

void CPropertyPageSystemDialog::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_RESULT_MESSAGEBOX, m_strResultMessageBox);
	DDX_CBIndex(pDX, IDC_CMB_MESSAGEBOX_TYPE, m_nMessageBoxType);
}


BEGIN_MESSAGE_MAP(CPropertyPageSystemDialog, CPropertyPage)
	ON_BN_CLICKED(IDC_BTN_MESSAGEBOX, &CPropertyPageSystemDialog::OnBnClickedBtnMessagebox)
END_MESSAGE_MAP()


// CPropertyPageSystemDialog message handlers

void CPropertyPageSystemDialog::OnBnClickedBtnMessagebox()
{
	UpdateData(TRUE);

	UINT nType;

	switch(m_nMessageBoxType)
	{
	case 1:
		nType = MB_OKCANCEL;
		break;
	case 2:
		nType = MB_YESNO;
		break;
	case 3:
		nType = MB_YESNOCANCEL;
		break;
	case 4:
		nType = MB_RETRYCANCEL;
		break;
	case 5:
		nType = MB_ABORTRETRYIGNORE;
		break;
	default:
		nType = 0;
		break;
	}

	int nResult = AfxMessageBox(IDS_MESSAGEBOX_DEMO_MESSAGE, nType);
	switch(nResult)
	{
	case IDOK:
		m_strResultMessageBox.Format(IDS_MESSAGEBOX_DEMO_RESULT, _T("OK"));
		break;
	case IDCANCEL:
		m_strResultMessageBox.Format(IDS_MESSAGEBOX_DEMO_RESULT, _T("Cancel"));
		break;
	case IDABORT:
		m_strResultMessageBox.Format(IDS_MESSAGEBOX_DEMO_RESULT, _T("Abort"));
		break;
	case IDIGNORE:
		m_strResultMessageBox.Format(IDS_MESSAGEBOX_DEMO_RESULT, _T("Ignore"));
		break;
	case IDNO:
		m_strResultMessageBox.Format(IDS_MESSAGEBOX_DEMO_RESULT, _T("No"));
		break;
	case IDRETRY:
		m_strResultMessageBox.Format(IDS_MESSAGEBOX_DEMO_RESULT, _T("Retry"));
		break;
	case IDYES:
		m_strResultMessageBox.Format(IDS_MESSAGEBOX_DEMO_RESULT, _T("Yes"));
		break;
	default:
		m_strResultMessageBox.Format(IDS_MESSAGEBOX_DEMO_RESULT, _T("Unknown"));
		break;
	}

	UpdateData(FALSE);
}

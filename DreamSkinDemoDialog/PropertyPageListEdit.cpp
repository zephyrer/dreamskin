// PropertyPageListEdit.cpp : implementation file
//

#include "stdafx.h"
#include "DreamSkinDemoDialog.h"
#include "PropertyPageListEdit.h"


// CPropertyPageListEdit dialog

IMPLEMENT_DYNAMIC(CPropertyPageListEdit, CPropertyPage)

CPropertyPageListEdit::CPropertyPageListEdit()
	: CPropertyPage(CPropertyPageListEdit::IDD)
	, m_strResultListBox(_T(""))
	, m_bEnableListBox(TRUE)
{

}

CPropertyPageListEdit::~CPropertyPageListEdit()
{
}

void CPropertyPageListEdit::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_RESULT_LISTBOX, m_strResultListBox);
	DDX_Control(pDX, IDC_LISTBOX_DEMO, m_lsDemoListBox);
	DDX_Check(pDX, IDC_CHK_LISTBOX_ENABLE, m_bEnableListBox);
}


BEGIN_MESSAGE_MAP(CPropertyPageListEdit, CPropertyPage)
	ON_BN_CLICKED(IDC_CHK_LISTBOX_ENABLE, &CPropertyPageListEdit::OnBnClickedChkListboxEnable)
	ON_LBN_SELCHANGE(IDC_LISTBOX_DEMO, &CPropertyPageListEdit::OnLbnSelchangeListboxDemo)
END_MESSAGE_MAP()


// CPropertyPageListEdit message handlers

BOOL CPropertyPageListEdit::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	CString strMessage;

	for (int i = 0; i < 100; i++)
	{
		strMessage.Format(_T("List Box Item %d"), i);
		m_lsDemoListBox.AddString((LPCTSTR)strMessage);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CPropertyPageListEdit::OnBnClickedChkListboxEnable()
{
	UpdateData(TRUE);

	m_lsDemoListBox.EnableWindow(m_bEnableListBox);

	UpdateData(FALSE);
}

void CPropertyPageListEdit::OnLbnSelchangeListboxDemo()
{
	UpdateData(TRUE);

	int nItems[11];
	int nLen = m_lsDemoListBox.GetSelItems(11, (int*)&nItems);

	m_strResultListBox = _T("");
	if (nLen <= 10)
	{
		for (int i = 0; i < nLen; i++)
		{
			CString strItem;
			m_lsDemoListBox.GetText(nItems[i], strItem);
			m_strResultListBox = m_strResultListBox + strItem + _T(";");
		}
	}
	else
	{
		for (int i = 0; i < 10; i++)
		{
			CString strItem;
			m_lsDemoListBox.GetText(i, strItem);
			m_strResultListBox = m_strResultListBox + strItem + _T(";");
		}
		m_strResultListBox = m_strResultListBox + _T("...");
	}

	UpdateData(FALSE);
}

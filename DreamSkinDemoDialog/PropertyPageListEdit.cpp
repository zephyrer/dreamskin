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
}


BEGIN_MESSAGE_MAP(CPropertyPageListEdit, CPropertyPage)
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

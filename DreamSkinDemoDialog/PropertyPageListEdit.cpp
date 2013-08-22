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
	, m_strListBoxAdd(_T(""))
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
	DDX_Text(pDX, IDC_EDIT_LISTBOX_ADD, m_strListBoxAdd);
	DDX_Control(pDX, IDC_LISTBOX_DEMO_MULTICOL, m_lsMultiColListBox);
}


BEGIN_MESSAGE_MAP(CPropertyPageListEdit, CPropertyPage)
	ON_BN_CLICKED(IDC_CHK_LISTBOX_ENABLE, &CPropertyPageListEdit::OnBnClickedChkListboxEnable)
	ON_LBN_SELCHANGE(IDC_LISTBOX_DEMO, &CPropertyPageListEdit::OnLbnSelchangeListboxDemo)
	ON_EN_CHANGE(IDC_EDIT_LISTBOX_ADD, &CPropertyPageListEdit::OnEnChangeEditListboxAdd)
	ON_BN_CLICKED(IDC_BTN_LISTBOX_ADD, &CPropertyPageListEdit::OnBnClickedBtnListboxAdd)
	ON_BN_CLICKED(IDC_BTN_LISTBOX_DEL, &CPropertyPageListEdit::OnBnClickedBtnListboxDel)
	ON_LBN_SELCHANGE(IDC_LISTBOX_DEMO_MULTICOL, &CPropertyPageListEdit::OnLbnSelchangeListboxDemoMulticol)
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
		m_lsMultiColListBox.AddString((LPCTSTR)strMessage);
	}

	UpdateListBoxWindows();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CPropertyPageListEdit::OnBnClickedChkListboxEnable()
{
	UpdateData(TRUE);

	m_lsDemoListBox.EnableWindow(m_bEnableListBox);
	m_lsMultiColListBox.EnableWindow(m_bEnableListBox);

	UpdateData(FALSE);
}

void CPropertyPageListEdit::OnLbnSelchangeListboxDemo()
{
	UpdateData(TRUE);

	m_strResultListBox = _T("");
	int nCount = 0;

	for (int i = 0; i < m_lsDemoListBox.GetCount(); i++)
	{
		if (m_lsDemoListBox.GetSel(i) > 0)
		{
			m_lsMultiColListBox.SetSel(i, 1);
			nCount++;
			if (nCount < 10)
			{
				CString strItem;
				m_lsDemoListBox.GetText(i, strItem);
				m_strResultListBox = m_strResultListBox + strItem + _T(";");
			}
			else if (nCount == 10)
			{
				m_strResultListBox = m_strResultListBox + _T("...");
			}
		}
		else
		{
			m_lsMultiColListBox.SetSel(i, 0);
		}
	}

	UpdateListBoxWindows();

	UpdateData(FALSE);
}

void CPropertyPageListEdit::OnEnChangeEditListboxAdd()
{
	UpdateData();

	UpdateListBoxWindows();
}

void CPropertyPageListEdit::UpdateListBoxWindows()
{
	if (m_strListBoxAdd.GetLength() > 0)
		GetDlgItem(IDC_BTN_LISTBOX_ADD)->EnableWindow(TRUE);
	else
		GetDlgItem(IDC_BTN_LISTBOX_ADD)->EnableWindow(FALSE);

	if (m_lsDemoListBox.GetSelCount() > 0)
		GetDlgItem(IDC_BTN_LISTBOX_DEL)->EnableWindow(TRUE);
	else
		GetDlgItem(IDC_BTN_LISTBOX_DEL)->EnableWindow(FALSE);
}

void CPropertyPageListEdit::OnBnClickedBtnListboxAdd()
{
	UpdateData();

	if (m_strListBoxAdd.GetLength() > 0)
	{
		m_lsDemoListBox.AddString((LPCTSTR)m_strListBoxAdd);
		m_lsMultiColListBox.AddString((LPCTSTR)m_strListBoxAdd);
	}
}

void CPropertyPageListEdit::OnBnClickedBtnListboxDel()
{
	if (m_lsDemoListBox.GetSelCount() > 0)
	{
		for (int i = m_lsDemoListBox.GetCount() - 1; i >= 0; i--)
		{
			if (m_lsDemoListBox.GetSel(i) > 0)
			{
				m_lsDemoListBox.DeleteString(i);
				m_lsMultiColListBox.DeleteString(i);
			}
		}

		OnLbnSelchangeListboxDemo();
	}

	UpdateListBoxWindows();
}
void CPropertyPageListEdit::OnLbnSelchangeListboxDemoMulticol()
{
	UpdateData(TRUE);

	m_strResultListBox = _T("");
	int nCount = 0;

	for (int i = 0; i < m_lsMultiColListBox.GetCount(); i++)
	{
		if (m_lsMultiColListBox.GetSel(i) > 0)
		{
			m_lsDemoListBox.SetSel(i, 1);
			nCount++;
			if (nCount < 10)
			{
				CString strItem;
				m_lsMultiColListBox.GetText(i, strItem);
				m_strResultListBox = m_strResultListBox + strItem + _T(";");
			}
			else if (nCount == 10)
			{
				m_strResultListBox = m_strResultListBox + _T("...");
			}
		}
		else
		{
			m_lsDemoListBox.SetSel(i, 0);
		}
	}

	UpdateListBoxWindows();

	UpdateData(FALSE);
}

// CtrlDemoPropertySheet.cpp : implementation file
//

#include "stdafx.h"
#include "DreamSkinDemoDialog.h"
#include "CtrlDemoPropertySheet.h"


// CCtrlDemoPropertySheet

IMPLEMENT_DYNAMIC(CCtrlDemoPropertySheet, CPropertySheet)

CCtrlDemoPropertySheet::CCtrlDemoPropertySheet(CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(IDS_PROPSHT_TITLE, pParentWnd, iSelectPage)
{
	AddPage(&m_PropertyPageCommonControl);
	AddPage(&m_PropertyPageListEdit);
	AddPage(&m_PropertyPageSystemDialog);
	
	m_psh.dwFlags |= PSH_NOAPPLYNOW;
	m_psh.dwFlags &= ~PSH_HASHELP;

}

CCtrlDemoPropertySheet::~CCtrlDemoPropertySheet()
{
}


BEGIN_MESSAGE_MAP(CCtrlDemoPropertySheet, CPropertySheet)
END_MESSAGE_MAP()


// CCtrlDemoPropertySheet message handlers

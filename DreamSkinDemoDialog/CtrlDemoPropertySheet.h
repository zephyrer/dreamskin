#pragma once

#include "PropertypageListEdit.h"
#include "PropertyPageSystemDialog.h"

// CCtrlDemoPropertySheet

class CCtrlDemoPropertySheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CCtrlDemoPropertySheet)

public:
	CCtrlDemoPropertySheet(CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CCtrlDemoPropertySheet();

public:
	CPropertyPageListEdit     m_PropertyPageListEdit;
	CPropertyPageSystemDialog m_PropertyPageSystemDialog;

protected:
	DECLARE_MESSAGE_MAP()
};



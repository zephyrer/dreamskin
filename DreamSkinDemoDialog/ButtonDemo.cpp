// ButtonDemo.cpp : implementation file
//

#include "stdafx.h"
#include "DreamSkinDemoDialog.h"
#include "ButtonDemo.h"


// CButtonDemo

IMPLEMENT_DYNAMIC(CButtonDemo, CButton)

CButtonDemo::CButtonDemo()
{

}

CButtonDemo::~CButtonDemo()
{
}


BEGIN_MESSAGE_MAP(CButtonDemo, CButton)
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()



// CButtonDemo message handlers



void CButtonDemo::OnMouseMove(UINT nFlags, CPoint point)
{
	SetWindowText(_T("Hover"));
	AfxMessageBox(_T("Test"));

	CButton::OnMouseMove(nFlags, point);
}

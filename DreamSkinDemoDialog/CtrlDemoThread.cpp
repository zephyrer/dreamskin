// CtrlDemoThread.cpp : implementation file
//

#include "stdafx.h"
#include "DreamSkinDemoDialog.h"
#include "CtrlDemoThread.h"

//DreamSkin Support
#include "../DreamSkin/DreamSkin.h"


// CCtrlDemoThread

IMPLEMENT_DYNCREATE(CCtrlDemoThread, CWinThread)

CCtrlDemoThread::CCtrlDemoThread()
{
}

CCtrlDemoThread::~CCtrlDemoThread()
{
}

BOOL CCtrlDemoThread::InitInstance()
{
	DreamSkinInit();
	AfxMessageBox(_T("test"));
	return FALSE;
}

int CCtrlDemoThread::ExitInstance()
{
	DreamSkinExit();
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CCtrlDemoThread, CWinThread)
END_MESSAGE_MAP()


// CCtrlDemoThread message handlers

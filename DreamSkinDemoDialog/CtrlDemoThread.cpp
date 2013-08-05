// CtrlDemoThread.cpp : implementation file
//

#include "stdafx.h"
#include "DreamSkinDemoDialog.h"
#include "CtrlDemoThread.h"

#include "CtrlDemoPropertySheet.h"
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
	if (theApp.EnableDreamSkin())
		DreamSkinInit();
	CCtrlDemoPropertySheet dlgCtrlDemo;
	dlgCtrlDemo.DoModal();
	return FALSE;
}

int CCtrlDemoThread::ExitInstance()
{
	if (theApp.EnableDreamSkin())
		DreamSkinExit();
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CCtrlDemoThread, CWinThread)
END_MESSAGE_MAP()


// CCtrlDemoThread message handlers

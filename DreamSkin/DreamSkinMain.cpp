//DreamSkinMain.cpp
#include <windows.h>
#include "HookWindowClassList.h"
#include "HookedWindowList.h"
#include "ImageHandleList.h"
#include "DreamSkinLoader.h"
#include "DreamSkinMain.h"
#include "DreamSkinWindow.h"
#include "DreamSkinDialog.h"
#include "DreamSkinButton.h"
#include "DreamSkinStatic.h"
#include "DreamSkinTab.h"
#include "DreamSkinEdit.h"

CDreamSkinMain theSkinMain;

/*
 * Windows Hook Procduce Routine
 * nCode	- hook code
 * wParam	- current-process flag
 * lParam	- message data
 */
LRESULT CALLBACK CDreamSkinMain::DreamSkinCallWndProc( int nCode, WPARAM wParam, LPARAM lParam )
{
	return theSkinMain.CallWndProc(nCode, wParam, lParam);
}

CDreamSkinMain::CDreamSkinMain()
{
	m_hOrgWndHook = NULL;
	m_pDefaultHookWindowClasses = new CHookWindowClassList();
	m_pHookedWindows = new CHookedWindowList();
	m_pImageHandleList = NULL;
}

CDreamSkinMain::~CDreamSkinMain()
{
	ExitInstance();
	delete m_pDefaultHookWindowClasses;
	delete m_pHookedWindows;
	if (m_pImageHandleList)
		delete m_pImageHandleList;
}

LRESULT CDreamSkinMain::CallWndProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	PCWPSTRUCT	pcs = (PCWPSTRUCT) lParam;
	HWND		hWnd = pcs->hwnd;

	LONG    lnHookedWindowProc = 0;

	//Replace window's message handle callback proc only when the window is valid
	if(hWnd != NULL && pcs->message != WM_DESTROY && pcs->message != WM_NCDESTROY)
	{
		//Get the class name that create the window
		//Note that we can only handle specified classes
		WCHAR	wstrClassName[MAX_CLASSNAME_LENGH + 1];
		memset(wstrClassName, 0, (MAX_CLASSNAME_LENGH + 1) * sizeof(WCHAR));

		GetClassNameW(hWnd, wstrClassName, MAX_CLASSNAME_LENGH);

		//Try to find the new window proc for this kind of window
		if(lnHookedWindowProc = m_pDefaultHookWindowClasses->GetWindowProcByName(wstrClassName))
		{   //We only replace the window message proc for windows that created by specified classes

			//Notice here we give a chance for other program can also modify the window proc
			//That has been hooked by our skin proc, so that if the window is already in the
			//hooked window list, we'll ignore it.
			if(m_pHookedWindows->GetHookedWindowPtr(hWnd) == NULL)
			{
				WNDPROC	OldWndProc;
				OldWndProc = (WNDPROC)GetWindowLong(hWnd, GWL_WNDPROC);

				if((LONG)OldWndProc != lnHookedWindowProc)
				{
					if(SetWindowLong(hWnd, GWL_WNDPROC, lnHookedWindowProc) != 0)
					{   //hooked succ, add this window in our hook window list
						AddHookedWindow(hWnd, OldWndProc);
					}
				}
				else
				{   //Note here may be something wrong, that this window has been hooked by our
					//skin, but not in our hooked window list, so we need to add this window in
					//our hooked window list
					AddHookedWindow(hWnd, OldWndProc);
				}
			}
		}
	}

	return CallNextHookEx(m_hOrgWndHook, nCode, wParam, lParam);
}

BOOL CDreamSkinMain::InitDefaultHookWindowClassList()
{
	BOOL bReturn = TRUE;

	bReturn = CDreamSkinDialog::InitialClass();
	if(!bReturn)
	{   //Init for dialog failed
		//TODO: Add error handle code
	}
	else
	{
		m_pDefaultHookWindowClasses->Add(DEFAULT_DIALOG_CLASSNAME_W, CDreamSkinDialog::DefWindowProc, (NEWINSTANCEPROC)CDreamSkinDialog::CreateInstance);
	}

	bReturn = CDreamSkinButton::InitialClass();
	if(!bReturn)
	{   //Init for dialog failed
		//TODO: Add error handle code
	}
	else
	{
		m_pDefaultHookWindowClasses->Add(DEFAULT_BUTTON_CLASSNAME_W, CDreamSkinButton::DefWindowProc, (NEWINSTANCEPROC)CDreamSkinButton::CreateInstance);
	}

	bReturn = CDreamSkinStatic::InitialClass();
	if(!bReturn)
	{   //Init for dialog failed
		//TODO: Add error handle code
	}
	else
	{
		m_pDefaultHookWindowClasses->Add(DEFAULT_STATIC_CLASSNAME_W, CDreamSkinStatic::DefWindowProc, (NEWINSTANCEPROC)CDreamSkinStatic::CreateInstance);
	}

	bReturn = CDreamSkinEdit::InitialClass();
	if(!bReturn)
	{   //Init for dialog failed
		//TODO: Add error handle code
	}
	else
	{
		m_pDefaultHookWindowClasses->Add(DEFAULT_EDIT_CLASSNAME_W, CDreamSkinEdit::DefWindowProc, (NEWINSTANCEPROC)CDreamSkinEdit::CreateInstance);
	}

	bReturn = CDreamSkinTab::InitialClass();
	if(!bReturn)
	{   //Init for dialog failed
		//TODO: Add error handle code
	}
	else
	{
		m_pDefaultHookWindowClasses->Add(DEFAULT_TAB_CLASSNAME_W, CDreamSkinTab::DefWindowProc, (NEWINSTANCEPROC)CDreamSkinTab::CreateInstance);
	}


	return TRUE;
}

BOOL CDreamSkinMain::InitInstance()
{
	BOOL bReturn = TRUE;
	
	bReturn = InitDefaultHookWindowClassList();
	if(!bReturn)
	{   //Fail to do the init
		//TODO: add error handle for this case
		return FALSE;
	}

	m_hOrgWndHook = SetWindowsHookEx(WH_CALLWNDPROC, (HOOKPROC)CDreamSkinMain::DreamSkinCallWndProc, GetModuleHandle(NULL), GetCurrentThreadId());

	if(m_hOrgWndHook != NULL)
	{
		return TRUE;
	}
	else
	{
		//TODO: Error Handle
		return FALSE;
	}

	//TODO: Enum all current window and send update to them
}

void CDreamSkinMain::ExitInstance()
{
	if (m_hOrgWndHook)
	{
		if(!UnhookWindowsHookEx(m_hOrgWndHook))
		{
			//TODO: Error Handle
		}

		m_hOrgWndHook = NULL;
	}

	m_pDefaultHookWindowClasses->Clear();
	m_pHookedWindows->Clear();

}

BOOL CDreamSkinMain::LoadSkin(const WCHAR *wstrSkinFilePath)
{
	BOOL bResult = TRUE;

	CImageHandleList *pImageHandleList = new CImageHandleList();
	CDreamSkinLoader theLoader(pImageHandleList);
	if (bResult = theLoader.Load(wstrSkinFilePath))
	{
		CDreamSkinButton::ApplySkin(&theLoader);
		CDreamSkinDialog::ApplySkin(&theLoader);
		CDreamSkinEdit::ApplySkin(&theLoader);
		CDreamSkinStatic::ApplySkin(&theLoader);
		CDreamSkinTab::ApplySkin(&theLoader);

		if (m_pImageHandleList)
		{
			delete m_pImageHandleList;
		}

		m_pImageHandleList = pImageHandleList;
		m_pHookedWindows->Reload();
	}

	return bResult;
}

BOOL CDreamSkinMain::AddHookedWindow(HWND hWnd, WNDPROC OrgWndProc)
{
	BOOL bResult = FALSE;

	//Get the class name that create the window
	//Note that we can only handle specified classes
	WCHAR	wstrClassName[MAX_CLASSNAME_LENGH + 1];
	memset(wstrClassName, 0, (MAX_CLASSNAME_LENGH<<1) + 1);

	GetClassNameW(hWnd, wstrClassName, MAX_CLASSNAME_LENGH);

	//Try to find the new window proc for this kind of window
	NEWINSTANCEPROC lnNewInstanceProc;
	if(lnNewInstanceProc = (NEWINSTANCEPROC)m_pDefaultHookWindowClasses->GetNewInstanceProcByName(wstrClassName))
	{   //We only replace the window message proc for windows that created by specified classes

		//Create new instance
		CDreamSkinWindow *pHookedWindow = (CDreamSkinWindow*)lnNewInstanceProc(hWnd, OrgWndProc);

		if (pHookedWindow)
			bResult = m_pHookedWindows->Add(hWnd, pHookedWindow);
	}

	return bResult;
}

CDreamSkinWindow* CDreamSkinMain::GetHookedWindow(HWND hWnd)
{
	return m_pHookedWindows->GetHookedWindowPtr(hWnd);
}

UINT CDreamSkinMain::GetHookedWindowCount()
{
	return m_pHookedWindows->Size();
}

void CDreamSkinMain::DelHookedWindow(HWND hWnd)
{
	m_pHookedWindows->Del(hWnd);
}
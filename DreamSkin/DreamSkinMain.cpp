//DreamSkinMain.cpp
#include <windows.h>
#include <Dbghelp.h>
#include <Tlhelp32.h>
#include "HookWindowClassList.h"
#include "HookedWindowList.h"
#include "HookedThreadList.h"
#include "ImageHandleList.h"
#include "DreamSkinLoader.h"
#include "DreamSkinMain.h"
#include "DreamSkinWindow.h"
#include "DreamSkinDialog.h"
#include "DreamSkinButton.h"
#include "DreamSkinStatic.h"
#include "DreamSkinTab.h"
#include "DreamSkinEdit.h"
#include "DreamSkinScrollBar.h"
#include "DreamSkinHeaderCtrl.h"
#include "DreamSkinListBox.h"
#include "DreamSkinListCtrl.h"
#include "DreamSkinComboBox.h"
#include "DreamSkinComboLBox.h"
#include "WinGdiEx.h"

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

BOOL CDreamSkinMain::HookAPILocal(APIHOOKITEM *pAPIHookItem)
{
	BOOL bResult = TRUE;

	if (pAPIHookItem->OrgAddr == 0)
	{
		HMODULE hModule = ::LoadLibraryW(pAPIHookItem->wstrModuleName);
		if (hModule)
		{
			pAPIHookItem->OrgAddr = (DWORD)::GetProcAddress(hModule, pAPIHookItem->strFunctionName);
			if (pAPIHookItem->OrgAddr)
			{
				//Hook import API table for already loaded modules
				char strModuleName[MAX_PATH];
				int nLen = ::WideCharToMultiByte(CP_ACP, 0, pAPIHookItem->wstrModuleName, -1, strModuleName, MAX_PATH, NULL, NULL);
				if (nLen > 0)
				{
					HANDLE hSnapshot= NULL;
					MODULEENTRY32 moudle;

					hSnapshot=CreateToolhelp32Snapshot(TH32CS_SNAPMODULE,GetCurrentProcessId());
					moudle.dwSize = sizeof(MODULEENTRY32);   
					Module32First(hSnapshot,&moudle);   
					do   
					{
						HookAPIByImportTable(pAPIHookItem->OrgAddr, pAPIHookItem->NewAddr, moudle.szModule, strModuleName);
					}   
					while(Module32Next(hSnapshot,&moudle) );   
					CloseHandle(hSnapshot);

					//Hook export API table for later loaded modules
					//TODO: This solution can't work on Windows 7, currently just remove it, we need some other solution later
					//HookAPIByExportTable(pAPIHookItem->OrgAddr, pAPIHookItem->NewAddr, hModule);
				}
				else
				{
					//convert the module name to multi-byte string failed
					bResult = FALSE;
				}
			}
			else
			{
				//find specified API failed
				bResult = FALSE;
			}

			//Don't call FreeLibrary if we already hook the export table
			::FreeLibrary(hModule);
		}
		else
		{
			//load specified module failed
			bResult = FALSE;
		}
	}

	return bResult;
}

void CDreamSkinMain::HookAPIByExportTable(DWORD OrgAddr,DWORD NewAddr, HMODULE hModule)
{
	PIMAGE_DOS_HEADER pImageDosHeader = (PIMAGE_DOS_HEADER)hModule;
	PIMAGE_NT_HEADERS pImageNtHeader = (PIMAGE_NT_HEADERS)((DWORD)hModule + pImageDosHeader->e_lfanew);
	PIMAGE_EXPORT_DIRECTORY pImageExportDirectory = (PIMAGE_EXPORT_DIRECTORY)((DWORD)hModule + pImageNtHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);
	DWORD *pAddressOfFunction = (DWORD*)(pImageExportDirectory->AddressOfFunctions + (DWORD)hModule);
	DWORD dwNumberOfNames = (DWORD)(pImageExportDirectory->NumberOfNames);
	for (int i = 0; i < (int)dwNumberOfNames; i++)  
	{   
		if((DWORD)((PBYTE)hModule + pAddressOfFunction[i]) == OrgAddr)   
		{   
			DWORD Addr = NewAddr - (DWORD)hModule;   
			DWORD dwOldProcted;   
			VirtualProtect(&(pAddressOfFunction[i]),sizeof(DWORD),PAGE_WRITECOPY,&dwOldProcted);   
			WriteProcessMemory(GetCurrentProcess(),&(pAddressOfFunction[i]),&Addr, sizeof(DWORD), NULL);   
			VirtualProtect(&(pAddressOfFunction[i]),sizeof(DWORD),dwOldProcted,0);   
			break;
		}   
	}
}

void CDreamSkinMain::HookAPIByImportTable(DWORD OrgAddr,DWORD NewAddr,const WCHAR *wstrLoadedModuleName, const char *strHookModuleName)
{ 
    PIMAGE_THUNK_DATA         pThunkData;
    DWORD *Addr2;   
    DWORD dwOLD;
	ULONG uSize;
    MEMORY_BASIC_INFORMATION  mbi;   
   
	HMODULE hMod = ::GetModuleHandleW(wstrLoadedModuleName);
	if (hMod)
	{
		PIMAGE_DOS_HEADER pImageDosHeader = (PIMAGE_DOS_HEADER)hMod;
		PIMAGE_NT_HEADERS pImageNtHeader = (PIMAGE_NT_HEADERS)((DWORD)hMod + pImageDosHeader->e_lfanew);
		PIMAGE_IMPORT_DESCRIPTOR pImportDescriptor = (PIMAGE_IMPORT_DESCRIPTOR)ImageDirectoryEntryToData(hMod,true,IMAGE_DIRECTORY_ENTRY_IMPORT,&uSize);   
		//PIMAGE_IMPORT_DESCRIPTOR pImportDescriptor = (PIMAGE_IMPORT_DESCRIPTOR)((DWORD)hMod + pImageNtHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);
		if (pImportDescriptor)
		{
			while(pImportDescriptor->Name)   
			{   
				char *szModName = (char *)((PBYTE)hMod+pImportDescriptor->Name) ;   
				if(_stricmp(szModName, strHookModuleName) == 0)   
				{   
					pThunkData = (PIMAGE_THUNK_DATA32)((PBYTE)hMod+pImportDescriptor->FirstThunk) ;   
					while(pThunkData->u1.Function)   
					{   
						Addr2 = (DWORD *)pThunkData->u1.Function ;   
						if((DWORD)Addr2 == OrgAddr)   
						{      
							VirtualQuery(&(pThunkData->u1.Function),&mbi,sizeof(mbi));   
							VirtualProtect(&(pThunkData->u1.Function),sizeof(DWORD),PAGE_READWRITE,&dwOLD);   
							WriteProcessMemory(GetCurrentProcess(),&(pThunkData->u1.Function),&NewAddr, sizeof(DWORD), NULL);   
							VirtualProtect(&(pThunkData->u1.Function),sizeof(DWORD),dwOLD,0);   
							break ;   
						}   
		   
						pThunkData++ ;   
					}   
		   
				}   
				pImportDescriptor++ ;   
			}
		}
	}
}

CDreamSkinMain::CDreamSkinMain()
{
	m_pHookedThreads = new CHookedThreadList();
	m_pDefaultHookWindowClasses = new CHookWindowClassList();
	m_pHookedWindows = new CHookedWindowList();
	m_pImageHandleList = NULL;

	m_HookSetScrollInfo.NewAddr = (DWORD)CDreamSkinScrollBar::SetScrollInfo;
	m_HookSetScrollInfo.OrgAddr = 0;
	m_HookSetScrollInfo.strFunctionName = "SetScrollInfo";
	m_HookSetScrollInfo.wstrModuleName = L"user32.dll";

	m_HookGetScrollInfo.NewAddr = (DWORD)CDreamSkinScrollBar::GetScrollInfo;
	m_HookGetScrollInfo.OrgAddr = 0;
	m_HookGetScrollInfo.strFunctionName = "GetScrollInfo";
	m_HookGetScrollInfo.wstrModuleName = L"user32.dll";

	m_HookSetScrollPos.NewAddr = (DWORD)CDreamSkinScrollBar::SetScrollPos;
	m_HookSetScrollPos.OrgAddr = 0;
	m_HookSetScrollPos.strFunctionName = "SetScrollPos";
	m_HookSetScrollPos.wstrModuleName = L"user32.dll";
}

CDreamSkinMain::~CDreamSkinMain()
{
	ExitInstance();
	delete m_pDefaultHookWindowClasses;
	delete m_pHookedWindows;
	delete m_pHookedThreads;
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

	HHOOK hOrgWndHook = m_pHookedThreads->GetOrgWndHook(GetCurrentThreadId());
	if (hOrgWndHook)
		return CallNextHookEx(hOrgWndHook, nCode, wParam, lParam);
	else
		return 0;
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

	bReturn = CDreamSkinListBox::InitialClass();
	if(!bReturn)
	{   //Init for dialog failed
		//TODO: Add error handle code
	}
	else
	{
		m_pDefaultHookWindowClasses->Add(DEFAULT_LISTBOX_CLASSNAME_W, CDreamSkinListBox::DefWindowProc, (NEWINSTANCEPROC)CDreamSkinListBox::CreateInstance);
	}

	bReturn = CDreamSkinListCtrl::InitialClass();
	if(!bReturn)
	{   //Init for dialog failed
		//TODO: Add error handle code
	}
	else
	{
		m_pDefaultHookWindowClasses->Add(DEFAULT_LISTCTRL_CLASSNAME_W, CDreamSkinListCtrl::DefWindowProc, (NEWINSTANCEPROC)CDreamSkinListCtrl::CreateInstance);
	}

	bReturn = CDreamSkinScrollBar::InitialClass();
	if(!bReturn)
	{   //Init for dialog failed
		//TODO: Add error handle code
	}
	else
	{
		m_pDefaultHookWindowClasses->Add(DEFAULT_SCROLLBAR_CLASSNAME_W, CDreamSkinScrollBar::DefWindowProc, (NEWINSTANCEPROC)CDreamSkinScrollBar::CreateInstance);
	}

	bReturn = CDreamSkinHeaderCtrl::InitialClass();
	if(!bReturn)
	{   //Init for dialog failed
		//TODO: Add error handle code
	}
	else
	{
		m_pDefaultHookWindowClasses->Add(DEFAULT_HEADERCTRL_CLASSNAME_W, CDreamSkinHeaderCtrl::DefWindowProc, (NEWINSTANCEPROC)CDreamSkinHeaderCtrl::CreateInstance);
	}

	bReturn = CDreamSkinComboBox::InitialClass();
	if(!bReturn)
	{   //Init for dialog failed
		//TODO: Add error handle code
	}
	else
	{
		m_pDefaultHookWindowClasses->Add(DEFAULT_COMBOBOX_CLASSNAME_W, CDreamSkinComboBox::DefWindowProc, (NEWINSTANCEPROC)CDreamSkinComboBox::CreateInstance);
	}

	bReturn = CDreamSkinComboLBox::InitialClass();
	if(!bReturn)
	{   //Init for dialog failed
		//TODO: Add error handle code
	}
	else
	{
		m_pDefaultHookWindowClasses->Add(DEFAULT_COMBOLBOX_CLASSNAME_W, CDreamSkinComboLBox::DefWindowProc, (NEWINSTANCEPROC)CDreamSkinComboLBox::CreateInstance);
	}


	return TRUE;
}

BOOL CDreamSkinMain::InitInstance()
{
	BOOL bReturn = TRUE;
	
	if (m_pHookedThreads->Size() == 0)
	{
		bReturn = InitDefaultHookWindowClassList();
		if(!bReturn)
		{   //Fail to do the init
			//TODO: add error handle for this case
			return FALSE;
		}
	}

	DWORD dwThreadID = ::GetCurrentThreadId();
	HHOOK hOrgWndHook = ::SetWindowsHookEx(WH_CALLWNDPROC, (HOOKPROC)CDreamSkinMain::DreamSkinCallWndProc, ::GetModuleHandle(NULL), dwThreadID);

	if(hOrgWndHook != NULL)
	{
		m_pHookedThreads->Add(dwThreadID, hOrgWndHook);
		bReturn = TRUE;
	}
	else
	{
		//TODO: Error Handle
		bReturn = FALSE;
	}

	if (bReturn)
		bReturn = HookAPILocal(&m_HookSetScrollInfo);

	if (bReturn)
		bReturn = HookAPILocal(&m_HookGetScrollInfo);

	if (bReturn)
		bReturn = HookAPILocal(&m_HookSetScrollPos);

	return bReturn;

	//TODO: Enum all current window and send update to them
}

void CDreamSkinMain::ExitInstance()
{
	m_pHookedThreads->Del(::GetCurrentThreadId());

	if (m_pHookedThreads->Size() == 0)
	{
		m_pDefaultHookWindowClasses->Clear();
		m_pHookedWindows->Clear();
	}

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
		CDreamSkinListBox::ApplySkin(&theLoader);
		CDreamSkinListCtrl::ApplySkin(&theLoader);
		CDreamSkinComboBox::ApplySkin(&theLoader);
		CDreamSkinScrollBar::ApplySkin(&theLoader);
		CDreamSkinHeaderCtrl::ApplySkin(&theLoader);

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

UINT CDreamSkinMain::GetHookedThreadCount()
{
	return m_pHookedThreads->Size();
}

void CDreamSkinMain::DelHookedWindow(HWND hWnd)
{
	m_pHookedWindows->Del(hWnd);
}
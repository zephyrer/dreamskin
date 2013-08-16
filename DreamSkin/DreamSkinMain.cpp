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
#include "DreamSkinListBox.h"
#include "WinGdiEx.h"

CDreamSkinMain theSkinMain;


typedef int (WINAPI *SETSCROLLINFO)(HWND hwnd, int fnBar, LPCSCROLLINFO lpsi, BOOL fRedraw);
typedef BOOL (WINAPI *GETSCROLLINFO)(HWND hwnd, int fnBar, LPSCROLLINFO lpsi);


BOOL bHooked = FALSE;
SETSCROLLINFO OldSetScrollInfo = NULL;
GETSCROLLINFO OldGetScrollInfo = NULL;

void DrawScrollBar(HDC hDC, LPSCROLLINFO lpsi, PSCROLLBARINFO psbi)
{
	RECT rcTop, rcBottom, rcClient, rcItem;
	rcTop.left = psbi->rcScrollBar.left;
	rcTop.top = psbi->rcScrollBar.top;
	rcTop.bottom = psbi->dxyLineButton;
	rcTop.right = psbi->rcScrollBar.right;

	rcBottom.left = psbi->rcScrollBar.left;
	rcBottom.top = psbi->rcScrollBar.bottom - psbi->dxyLineButton;
	rcBottom.bottom = psbi->rcScrollBar.bottom;
	rcBottom.right = psbi->rcScrollBar.right;

	::FillSolidRect(hDC, &rcTop, RGB(0, 255, 0));
	::FillSolidRect(hDC, &rcBottom, RGB(0, 255, 0));

	rcClient.left = psbi->rcScrollBar.left;
	rcClient.top  = psbi->dxyLineButton;
	rcClient.bottom = psbi->rcScrollBar.bottom - psbi->dxyLineButton;
	rcClient.right = psbi->rcScrollBar.right;
	::FillSolidRect(hDC, &rcClient, RGB(0, 0, 255));

	rcItem.left = psbi->rcScrollBar.left + 1;
	rcItem.right = psbi->rcScrollBar.right -1;
	rcItem.top = psbi->xyThumbTop;
	rcItem.bottom = psbi->xyThumbBottom;
	::FillSolidRect(hDC, &rcItem, RGB(0, 0, 128));

	::Draw3dRect(hDC, &psbi->rcScrollBar, RGB(255, 0, 0), RGB(255, 0, 0));
}

int WINAPI DreamSkinSetScrollInfo(HWND hWnd, int fnBar, LPCSCROLLINFO lpsi, BOOL fRedraw)
{
	int nResult = 0;
	if (OldSetScrollInfo)
		nResult = OldSetScrollInfo(hWnd, fnBar, lpsi, fRedraw);

	::SendMessage(hWnd, WM_NCPAINT, 1, 0);

	return nResult;
}

BOOL WINAPI DreamSkinGetScrollInfo(HWND hwnd, int fnBar, LPSCROLLINFO lpsi)
{
	BOOL bResult = TRUE;
	if (OldSetScrollInfo)
		bResult = OldGetScrollInfo(hwnd, fnBar, lpsi);

	HDC hWindowDC;
	hWindowDC = ::GetWindowDC(hwnd);
    // Paint into this DC

	RECT rcWindow;
	::GetWindowRect(hwnd, &rcWindow);

	DWORD dwStyle = ::GetWindowLong(hwnd, GWL_STYLE);
	SCROLLBARINFO ScrollBarInfo;

	ScrollBarInfo.cbSize = sizeof(SCROLLBARINFO);
	::GetScrollBarInfo(hwnd, OBJID_VSCROLL, &ScrollBarInfo);

	::OffsetRect(&ScrollBarInfo.rcScrollBar, 0 - rcWindow.left, 0 - rcWindow.top);
	DrawScrollBar(hWindowDC, lpsi, &ScrollBarInfo);

	::ReleaseDC(hwnd, hWindowDC);

	return bResult;
}

/*void ExportFind(DWORD FromAddr,DWORD ToAddr,const char *ModuleName)   
{   
    HANDLE hModule;   
    ULONG uSize ;   
    PIMAGE_EXPORT_DIRECTORY pExportDirectory;   
    DWORD FunctionNum;   
    DWORD *FunctionAddr;   
    DWORD dwOLD;   
    DWORD Addr;   
    MEMORY_BASIC_INFORMATION  mbi;   
   
    hModule=GetModuleHandle(ModuleName);   
    pExportDirectory=(PIMAGE_EXPORT_DIRECTORY)ImageDirectoryEntryToData(hModule,true,IMAGE_DIRECTORY_ENTRY_EXPORT,&uSize);   
    FunctionNum=pExportDirectory->NumberOfFunctions;   
    FunctionAddr=(DWORD *)((PBYTE)hModule+pExportDirectory->AddressOfFunctions);   
    for(int i=0;i<FunctionNum;i++)   
    {   
        if((DWORD)((PBYTE)hModule+FunctionAddr[i])==FromAddr)   
        {   
            Addr=ToAddr-(DWORD)hModule;   
            VirtualQuery(&(FunctionAddr[i]),&mbi,sizeof(mbi));   
            VirtualProtect(&(FunctionAddr[i]),sizeof(DWORD),PAGE_READWRITE,&dwOLD);   
            WriteProcessMemory(GetCurrentProcess(),&(FunctionAddr[i]),&Addr, sizeof(DWORD), NULL);   
            VirtualProtect(&(FunctionAddr[i]),sizeof(DWORD),dwOLD,0);   
        }   
    }   
   
} */

void IATFind(DWORD FromAddr,DWORD ToAddr,const WCHAR *module)   
{   
    PIMAGE_IMPORT_DESCRIPTOR  pImportDescriptor;    
    PIMAGE_THUNK_DATA         pThunkData;   
    ULONG uSize ;   
    DWORD *Addr2;   
    DWORD dwOLD;   
    MEMORY_BASIC_INFORMATION  mbi;   
   
    HMODULE hMod=GetModuleHandleW(module);  
	if (hMod)
	{
		pImportDescriptor=(PIMAGE_IMPORT_DESCRIPTOR)ImageDirectoryEntryToData(hMod,true,IMAGE_DIRECTORY_ENTRY_IMPORT,&uSize);   
		if(!pImportDescriptor)   
			return;   
	}
	else
	{
		DWORD dw = GetLastError();
		return ;
	}
   
    while(pImportDescriptor->Name)   
    {   
        char *szModName = (char *)((PBYTE)hMod+pImportDescriptor->Name) ;   
        if(_stricmp(szModName,"user32.dll")==0)   
        {   
            pThunkData = (PIMAGE_THUNK_DATA32)((PBYTE)hMod+pImportDescriptor->FirstThunk) ;   
            while(pThunkData->u1.Function)   
            {   
                Addr2 = (DWORD *)pThunkData->u1.Function ;   
                if((DWORD)Addr2==FromAddr)   
                {   
                    //file_str("ÕÒµ½µ¼Èë","F:\\temp.txt");    
                    VirtualQuery(&(pThunkData->u1.Function),&mbi,sizeof(mbi));   
                    VirtualProtect(&(pThunkData->u1.Function),sizeof(DWORD),PAGE_READWRITE,&dwOLD);   
                    WriteProcessMemory(GetCurrentProcess(),&(pThunkData->u1.Function),&ToAddr, sizeof(DWORD), NULL);   
                    VirtualProtect(&(pThunkData->u1.Function),sizeof(DWORD),dwOLD,0);   
                    break ;   
                }   
   
                pThunkData++ ;   
            }   
   
        }   
        pImportDescriptor++ ;   
    } 
}

BOOL HookAPILocal(WCHAR* wstrModuleName, char* strFuncName)
{ 
    HANDLE hSnapshot= NULL;   
    MODULEENTRY32 moudle;

	HMODULE hModule = LoadLibraryW(wstrModuleName);
	PIMAGE_DOS_HEADER pImageDosHeader = NULL;  
    PIMAGE_NT_HEADERS pImageNtHeader = NULL;  
    PIMAGE_EXPORT_DIRECTORY pImageExportDirectory = NULL;

	pImageDosHeader=(PIMAGE_DOS_HEADER)hModule;
    pImageNtHeader=(PIMAGE_NT_HEADERS)((DWORD)hModule+pImageDosHeader->e_lfanew);
    pImageExportDirectory=(PIMAGE_EXPORT_DIRECTORY)((DWORD)hModule+pImageNtHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);
    
    DWORD *pAddressOfFunction = (DWORD*)(pImageExportDirectory->AddressOfFunctions + (DWORD)hModule);
    DWORD *pAddressOfNames = (DWORD*)(pImageExportDirectory->AddressOfNames + (DWORD)hModule);
    DWORD dwNumberOfNames = (DWORD)(pImageExportDirectory->NumberOfNames);

	SETSCROLLINFO MySetScrollInfo = (SETSCROLLINFO)DreamSkinSetScrollInfo;
	SETSCROLLINFO OrgSetScrollInfo = (SETSCROLLINFO)GetProcAddress(hModule, "SetScrollInfo");

	if (OrgSetScrollInfo != MySetScrollInfo)
	{
		OldSetScrollInfo = OrgSetScrollInfo;

		hSnapshot=CreateToolhelp32Snapshot(TH32CS_SNAPMODULE,GetCurrentProcessId());   
		moudle.dwSize = sizeof(MODULEENTRY32);   
		Module32First(hSnapshot,&moudle);   
		do   
		{    
			IATFind((DWORD)OrgSetScrollInfo,(DWORD)MySetScrollInfo,moudle.szModule);   
		}   
		while(Module32Next(hSnapshot,&moudle) );   
		CloseHandle(hSnapshot);

		

		for (int i=0; i<(int)dwNumberOfNames; i++)  
		{   
			if((DWORD)((PBYTE)hModule+pAddressOfFunction[i])==(DWORD)OrgSetScrollInfo)   
			{   
				DWORD Addr=(DWORD)MySetScrollInfo-(DWORD)hModule;   
				DWORD dwOldProcted;
				//VirtualQuery(&(FunctionAddr[i]),&mbi,sizeof(mbi));   
				VirtualProtect(&(pAddressOfFunction[i]),sizeof(DWORD),PAGE_WRITECOPY,&dwOldProcted);   
				WriteProcessMemory(GetCurrentProcess(),&(pAddressOfFunction[i]),&Addr, sizeof(DWORD), NULL);   
				VirtualProtect(&(pAddressOfFunction[i]),sizeof(DWORD),dwOldProcted,0);   
				break;
			}   
		} 
	}

	/*GETSCROLLINFO MyGetScrollInfo = (GETSCROLLINFO)DreamSkinGetScrollInfo;
	GETSCROLLINFO OrgGetScrollInfo = (GETSCROLLINFO)GetProcAddress(hModule, "GetScrollInfo");

	if (OrgGetScrollInfo != MyGetScrollInfo)
	{
		OldGetScrollInfo = OrgGetScrollInfo;

		hSnapshot=CreateToolhelp32Snapshot(TH32CS_SNAPMODULE,GetCurrentProcessId());   
		moudle.dwSize = sizeof(MODULEENTRY32);   
		Module32First(hSnapshot,&moudle);   
		do   
		{    
			IATFind((DWORD)OrgGetScrollInfo,(DWORD)MyGetScrollInfo,moudle.szModule);   
		}   
		while(Module32Next(hSnapshot,&moudle) );   
		CloseHandle(hSnapshot);

		

		for (int i=0; i<(int)dwNumberOfNames; i++)  
		{   
			if((DWORD)((PBYTE)hModule+pAddressOfFunction[i])==(DWORD)OrgGetScrollInfo)   
			{   
				DWORD Addr=(DWORD)MyGetScrollInfo-(DWORD)hModule;   
				DWORD dwOldProcted;
				//VirtualQuery(&(FunctionAddr[i]),&mbi,sizeof(mbi));   
				VirtualProtect(&(pAddressOfFunction[i]),sizeof(DWORD),PAGE_WRITECOPY,&dwOldProcted);   
				WriteProcessMemory(GetCurrentProcess(),&(pAddressOfFunction[i]),&Addr, sizeof(DWORD), NULL);   
				VirtualProtect(&(pAddressOfFunction[i]),sizeof(DWORD),dwOldProcted,0);   
				break;
			}   
		} 
	}*/


	//SETSCROLLINFO OldFunc =  (SETSCROLLINFO)GetProcAddress(hModule, "SetScrollInfo");
	return TRUE;
}


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
	m_pHookedThreads = new CHookedThreadList();
	m_pDefaultHookWindowClasses = new CHookWindowClassList();
	m_pHookedWindows = new CHookedWindowList();
	m_pImageHandleList = NULL;
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
		bReturn = HookAPILocal(L"user32.dll", "SetScrollInfo");

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
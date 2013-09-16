//DreamSkinMain.h

#ifndef DREAMSKIN_MAIN_H
#define DREAMSKIN_MAIN_H

#define MAX_CLASSNAME_LENGH    1024

class CHookWindowClassList;
class CHookedWindowList;
class CDreamSkinWindow;
class CImageHandleList;
class CHookedThreadList;

typedef struct _tag_APIHOOKITEM
{
	WCHAR *wstrModuleName;         //Module path
	char  *strFunctionName;        //API name
	DWORD  OrgAddr;                //Original address
	DWORD  NewAddr;                //New address
}APIHOOKITEM;

class CDreamSkinMain
{
public:
	CDreamSkinMain();
	~CDreamSkinMain();

public:
	BOOL InitInstance();
	void ExitInstance();

	BOOL LoadSkin(const WCHAR* wstrSkinFilePath);

	BOOL AddHookedWindow(HWND hWnd, WNDPROC OrgWndProc);
	CDreamSkinWindow* GetHookedWindow(HWND hWnd);
	UINT GetHookedThreadCount();
	UINT GetHookedWindowCount();
	void DelHookedWindow(HWND hWnd);

protected:
	static LRESULT CALLBACK DreamSkinCallWndProc(int nCode, WPARAM wParam, LPARAM lParam);
	static BOOL HookAPILocal(APIHOOKITEM *pAPIHookItem);
	static void HookAPIByImportTable(DWORD OrgAddr,DWORD NewAddr, const WCHAR *wstrLoadedModuleName, const char *strHookModuleName);
	static void HookAPIByExportTable(DWORD OrgAddr,DWORD NewAddr, HMODULE hModule);


protected:
	LRESULT CallWndProc(int nCode, WPARAM wParam, LPARAM lParam);
	BOOL    InitDefaultHookWindowClassList();

	CHookWindowClassList *m_pDefaultHookWindowClasses;
	CHookedThreadList    *m_pHookedThreads;
	CHookedWindowList    *m_pHookedWindows;
	CImageHandleList     *m_pImageHandleList;

public:
	APIHOOKITEM           m_HookSetScrollInfo;
	APIHOOKITEM           m_HookGetScrollInfo;
	APIHOOKITEM           m_HookSetScrollPos;
};

extern CDreamSkinMain theSkinMain;
inline CDreamSkinMain* DreamSkinGetInstance() { return &theSkinMain; };


#endif // DREAMSKIN_MAIN_H
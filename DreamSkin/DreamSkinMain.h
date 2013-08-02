//DreamSkinMain.h

#ifndef DREAMSKIN_MAIN_H
#define DREAMSKIN_MAIN_H

#define MAX_CLASSNAME_LENGH    1024

class CHookWindowClassList;
class CHookedWindowList;
class CDreamSkinWindow;
class CImageHandleList;
class CHookedThreadList;

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

	LRESULT CallWndProc(int nCode, WPARAM wParam, LPARAM lParam);

	BOOL    InitDefaultHookWindowClassList();

protected:
	CHookWindowClassList *m_pDefaultHookWindowClasses;
	CHookedThreadList    *m_pHookedThreads;
	CHookedWindowList    *m_pHookedWindows;
	CImageHandleList     *m_pImageHandleList;
};

extern CDreamSkinMain theSkinMain;
inline CDreamSkinMain* DreamSkinGetInstance() { return &theSkinMain; };


#endif // DREAMSKIN_MAIN_H
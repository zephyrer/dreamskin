//DreamSkinMain.h

#ifndef DREAMSKIN_MAIN_H
#define DREAMSKIN_MAIN_H

#define MAX_CLASSNAME_LENGH    1024

class CHookWindowClassList;
class CHookedWindowList;
class CDreamSkinWindow;
class CImageHandleList;

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
	UINT GetHookedWindowCount();
	void DelHookedWindow(HWND hWnd);

protected:
	static LRESULT CALLBACK DreamSkinCallWndProc(int nCode, WPARAM wParam, LPARAM lParam);

	LRESULT CallWndProc(int nCode, WPARAM wParam, LPARAM lParam);

	BOOL    InitDefaultHookWindowClassList();

protected:
	HHOOK  m_hOrgWndHook;
	CHookWindowClassList *m_pDefaultHookWindowClasses;
	CHookedWindowList    *m_pHookedWindows;
	CImageHandleList     *m_pImageHandleList;
};

extern CDreamSkinMain theSkinMain;
inline CDreamSkinMain* DreamSkinGetInstance() { return &theSkinMain; };


#endif // DREAMSKIN_MAIN_H
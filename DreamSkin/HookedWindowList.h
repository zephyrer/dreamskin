//HookedWindowList.h
//A list for all windows that were hooked by our lib

#ifndef DREAMSKIN_HOOKEDWINDOWLIST_H
#define DREAMSKIN_HOOKEDWINDOWLIST_H

class CDreamSkinWindow;

class CHookedWindowList
{
public:
	CHookedWindowList();
	~CHookedWindowList();

public:
	//Add a window in the list
	BOOL  Add(HWND hWnd, CDreamSkinWindow *pHookedWindow);
	//Remove all windows from the list
	void  Clear();
	//Remove a window from the list
	void  Del(HWND hWnd);
	//Get the pointer to dream skin winow instance
	CDreamSkinWindow*  GetHookedWindowPtr(HWND hWnd);
	//Notify each window in the list to reload skin and refresh
	void  Reload();
	//Get the window count int the list
	UINT  Size(); 

private:
	void* m_pHookedWindowList;
};

#endif // DREAMSKIN_HOOKEDWINDOWLIST_H
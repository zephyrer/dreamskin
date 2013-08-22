//HookedWindowList.cpp

#include <windows.h>
#include <map>
using namespace std;

#include "DreamSkinMain.h"
#include "DreamSkinLoader.h"
#include "DreamSkinWindow.h"
#include "HookedWindowList.h"
#include "HookWindowClassList.h"

typedef map<HWND, CDreamSkinWindow*>   HOOKEDWINDOWPTRLIST;
typedef pair<HWND, CDreamSkinWindow*>  HOOKEDWINDOWHANDLEPTRPAIR;

CHookedWindowList::CHookedWindowList()
{
	m_pHookedWindowList = new HOOKEDWINDOWPTRLIST();
}

CHookedWindowList::~CHookedWindowList()
{
	delete m_pHookedWindowList;
}

BOOL CHookedWindowList::Add(HWND hWnd, CDreamSkinWindow *pHookedWindow)
{
	((HOOKEDWINDOWPTRLIST*)m_pHookedWindowList)->insert(HOOKEDWINDOWHANDLEPTRPAIR(hWnd, pHookedWindow));
	return TRUE;
}

void CHookedWindowList::Clear()
{
	CDreamSkinWindow *pHookedWindow = NULL;
	HOOKEDWINDOWPTRLIST::iterator iter;
	while (!((HOOKEDWINDOWPTRLIST*)m_pHookedWindowList)->empty())
	{
		iter = ((HOOKEDWINDOWPTRLIST*)m_pHookedWindowList)->begin();
		pHookedWindow = (CDreamSkinWindow*)(iter->second);
		((HOOKEDWINDOWPTRLIST*)m_pHookedWindowList)->erase(iter);
		pHookedWindow->Detach();
		pHookedWindow->UpdateWindow();
		delete pHookedWindow;
	}
}

void CHookedWindowList::Del(HWND hWnd)
{
	CDreamSkinWindow *pHookedWindow = GetHookedWindowPtr(hWnd);
	if (pHookedWindow)
	{
		((HOOKEDWINDOWPTRLIST*)m_pHookedWindowList)->erase(hWnd);
		delete pHookedWindow;
	}
}

CDreamSkinWindow* CHookedWindowList::GetHookedWindowPtr(HWND hWnd)
{
	CDreamSkinWindow *pHookedWindow = NULL;
	HOOKEDWINDOWPTRLIST::iterator iter; 
	if(hWnd)
	{
		iter = ((HOOKEDWINDOWPTRLIST*)m_pHookedWindowList)->find(hWnd);
		if(iter != ((HOOKEDWINDOWPTRLIST*)m_pHookedWindowList)->end())
		{
			pHookedWindow = (CDreamSkinWindow*)(iter->second);
		}
	}

	return pHookedWindow;
}

void CHookedWindowList::Reload()
{
	CDreamSkinWindow *pHookedWindow = NULL;
	HOOKEDWINDOWPTRLIST::iterator iter; 
	
	for(iter = ((HOOKEDWINDOWPTRLIST*)m_pHookedWindowList)->begin(); iter != ((HOOKEDWINDOWPTRLIST*)m_pHookedWindowList)->end(); iter++)
	{
		pHookedWindow = (CDreamSkinWindow*)(iter->second);
		pHookedWindow->Reload();
	}
}

UINT CHookedWindowList::Size()
{
	return ((HOOKEDWINDOWPTRLIST*)m_pHookedWindowList)->size();
}
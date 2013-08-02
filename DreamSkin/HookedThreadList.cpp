//HookedThreadList.cpp

#include <windows.h>
#include <map>
using namespace std;

#include "HookedThreadList.h"

typedef map<DWORD, HHOOK>   HOOKEDTHREADORGHOOKLIST;
typedef pair<DWORD, HHOOK>  HOOKEDTHREADIDORGHOOKPAIR;

CHookedThreadList::CHookedThreadList()
{
	m_pHookedThreadList = new HOOKEDTHREADORGHOOKLIST();
}

CHookedThreadList::~CHookedThreadList()
{
	delete m_pHookedThreadList;
}

BOOL CHookedThreadList::Add(DWORD dwThreadID, HHOOK hOrgWndHook)
{
	((HOOKEDTHREADORGHOOKLIST*)m_pHookedThreadList)->insert(HOOKEDTHREADIDORGHOOKPAIR(dwThreadID, hOrgWndHook));
	return TRUE;
}

void CHookedThreadList::Del(DWORD dwThreadID)
{
	HHOOK hOrgWndHook = GetOrgWndHook(dwThreadID);
	if (hOrgWndHook)
	{
		UnhookWindowsHookEx(hOrgWndHook);
		((HOOKEDTHREADORGHOOKLIST*)m_pHookedThreadList)->erase(dwThreadID);
	}
}

HHOOK CHookedThreadList::GetOrgWndHook(DWORD dwThreadID)
{
	HHOOK hOrgWndHook = NULL;
	HOOKEDTHREADORGHOOKLIST::iterator iter; 
	if(dwThreadID)
	{
		iter = ((HOOKEDTHREADORGHOOKLIST*)m_pHookedThreadList)->find(dwThreadID);
		if(iter != ((HOOKEDTHREADORGHOOKLIST*)m_pHookedThreadList)->end())
		{
			hOrgWndHook = (HHOOK)(iter->second);
		}
	}

	return hOrgWndHook;
}

UINT CHookedThreadList::Size()
{
	return ((HOOKEDTHREADORGHOOKLIST*)m_pHookedThreadList)->size();
}
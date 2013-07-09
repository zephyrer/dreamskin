//HookWindowClassList.h
#include <windows.h>
#include <string>
#include <map>
using namespace std;

#include "HookWindowClassList.h"

typedef struct _CLASSPROC
{
	WNDPROC WndProc;                            //windows message handler
	NEWINSTANCEPROC NewProc;                    //use to create new dreamskin related instance
}CLASSPROC;

typedef map<wstring, CLASSPROC> CLASSPROCLIST;
typedef pair<wstring, CLASSPROC> CLASSNAMEPROCPAIR;

CHookWindowClassList::CHookWindowClassList()
{
	m_pClassProcList = new CLASSPROCLIST();
}

CHookWindowClassList::~CHookWindowClassList()
{
	delete m_pClassProcList;
}

BOOL CHookWindowClassList::Add(WCHAR *wstrClassName, WNDPROC WndProc, NEWINSTANCEPROC NewProc)
{
	CLASSPROC ClassProc;
	ClassProc.WndProc = WndProc;
	ClassProc.NewProc = NewProc;
	((CLASSPROCLIST*)m_pClassProcList)->insert(CLASSNAMEPROCPAIR(wstring(wstrClassName), ClassProc));
	return TRUE;
}

void CHookWindowClassList::Clear()
{
	((CLASSPROCLIST*)m_pClassProcList)->clear();
}

LONG CHookWindowClassList::GetNewInstanceProcByName(WCHAR *wstrClassName)
{
	NEWINSTANCEPROC NewProcRet = NULL;
	CLASSPROC ClassProc;
	CLASSPROCLIST::iterator iter; 
	if(wstrClassName != NULL)
	{
		iter = ((CLASSPROCLIST*)m_pClassProcList)->find(wstring(wstrClassName));
		if(iter != ((CLASSPROCLIST*)m_pClassProcList)->end())
		{
			ClassProc = (CLASSPROC)(iter->second);
			NewProcRet = ClassProc.NewProc;
		}
	}

	return (LONG)NewProcRet;
}

LONG CHookWindowClassList::GetWindowProcByName(WCHAR *wstrClassName)
{
	WNDPROC WndProcRet = NULL;
	CLASSPROC ClassProc;
	CLASSPROCLIST::iterator iter; 
	if(wstrClassName != NULL)
	{
		iter = ((CLASSPROCLIST*)m_pClassProcList)->find(wstring(wstrClassName));
		if(iter != ((CLASSPROCLIST*)m_pClassProcList)->end())
		{
			ClassProc = (CLASSPROC)(iter->second);
			WndProcRet = ClassProc.WndProc;
		}
	}

	return (LONG)WndProcRet;
}




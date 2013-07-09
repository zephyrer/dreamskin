//HookWindowClassList.h
//A list for all windows classes that can be hooked by our lib

#ifndef DREAMSKIN_HOOKCLASSLIST_H
#define DREAMSKIN_HOOKCLASSLIST_H

//type definition for functions of create a new instance of skin windows
typedef void* (* NEWINSTANCEPROC)(HWND hWnd, WNDPROC OrgWndProc);

class CHookWindowClassList
{
public:
	CHookWindowClassList();
	~CHookWindowClassList();

public:
	//Add a class in the list
	BOOL Add(WCHAR *wstrClassName, WNDPROC WndProc, NEWINSTANCEPROC NewProc);
	//Clear the list
	void Clear();
	//Get the new instance procedure from the class name
	LONG GetNewInstanceProcByName(WCHAR *wstrClassName);
	//Get the window procdure from the class name
	LONG GetWindowProcByName(WCHAR *wstrClassName);
	

private:
	void* m_pClassProcList;
};


#endif // DREAMSKIN_HOOKCLASSLIST_H
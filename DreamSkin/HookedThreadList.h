//HookedThreadList.h
//A list for all thread that were hooked by our lib

#ifndef DREAMSKIN_HOOKEDTHREADLIST_H
#define DREAMSKIN_HOOKEDTHREADLIST_H

class CHookedThreadList
{
public:
	CHookedThreadList();
	~CHookedThreadList();

public:
	//Add a thread in the list
	BOOL  Add(DWORD dwThreadID, HHOOK hOrgWndHook);
	//Remove a thread from the list
	void  Del(DWORD dwThreadID);
	//Get the original hook to specified thread
	HHOOK  GetOrgWndHook(DWORD dwThreadID);
	//Get the thread count in the list
	UINT  Size(); 

private:
	void* m_pHookedThreadList;
};

#endif // DREAMSKIN_HOOKEDTHREADLIST_H
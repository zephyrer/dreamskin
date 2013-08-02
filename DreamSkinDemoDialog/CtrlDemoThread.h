#pragma once



// CCtrlDemoThread

class CCtrlDemoThread : public CWinThread
{
	DECLARE_DYNCREATE(CCtrlDemoThread)

protected:
	CCtrlDemoThread();           // protected constructor used by dynamic creation
	virtual ~CCtrlDemoThread();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()
};



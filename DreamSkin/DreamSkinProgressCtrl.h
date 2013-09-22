//DreamSkinProgressCtrl.h

#ifndef DREAMSKIN_PROGRESSCTRL_INCLUDE
#define DREAMSKIN_PROGRESSCTRL_INCLUDE

#define DEFAULT_PROGRESSCTRL_CLASSNAME_A ("msctls_progress32")
#define DEFAULT_PROGRESSCTRL_CLASSNAME_W (L"msctls_progress32")

class CDreamSkinProgressCtrl : public CDreamSkinWindow
{
//Contrustor and Destructor
public:
	CDreamSkinProgressCtrl(HWND hWnd, WNDPROC OrgWndProc);
	virtual ~CDreamSkinProgressCtrl();

	virtual void    Reload();

public:
	//Apply new skin
	static BOOL ApplySkin(CDreamSkinLoader *pLoader);
	//Create an instance of this class
	static CDreamSkinProgressCtrl * CreateInstance(HWND hWnd, WNDPROC OrgWndProc);
	//window procedure for dreamskin dialog, which provides basic skin functions
	static LRESULT WINAPI DefWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	//Initial the static member of the class
	static BOOL InitialClass();
	//Destroy the static member of the class
	static void DestroyClass();
	//Get the default skin for static
	static BOOL GetDefaultSkin(SKINPROGRESSCTRL *pSkinProgressCtrl);

public:
	static WNDPROC          s_DefaultWindowProc;      //Default static window proc
	static SKINPROGRESSCTRL s_SkinProgressCtrl;       //Progress Control Skin Settings

public:
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//process the WM_PAINT message
	virtual LRESULT OnPaint();

protected:
	//Draw the foreground of the progress control
	void    DrawHorzForeground(HDC hDC, RECT rcClient);
	void    DrawVertForeground(HDC hDC, RECT rcClient);

protected:
	SKINPROGRESSCTRL *m_pSkinProgressCtrl;            //Instance Related Progress Control Skin Settings
};

#endif // end of DREAMSKIN_PROGRESSCTRL_INCLUDE
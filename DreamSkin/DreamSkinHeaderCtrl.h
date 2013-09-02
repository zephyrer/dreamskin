//DreamSkinHeaderCtrl.h

#ifndef DREAMSKIN_HEADERCTRL_INCLUDE
#define DREAMSKIN_HEADERCTRL_INCLUDE

#define DEFAULT_HEADERCTRL_CLASSNAME_A ("SysHeader32")
#define DEFAULT_HEADERCTRL_CLASSNAME_W (L"SysHeader32")

class CDreamSkinHeaderCtrl : public CDreamSkinWindow
{
//Contrustor and Destructor
public:
	CDreamSkinHeaderCtrl(HWND hWnd, WNDPROC OrgWndProc);
	virtual ~CDreamSkinHeaderCtrl();

	virtual void    Reload();

public:
	//Apply new skin
	static BOOL ApplySkin(CDreamSkinLoader *pLoader);
	//Create an instance of this class
	static CDreamSkinHeaderCtrl * CreateInstance(HWND hWnd, WNDPROC OrgWndProc);
	//window procedure for dreamskin dialog, which provides basic skin functions
	static LRESULT WINAPI DefWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	//Initial the static member of the class
	static BOOL InitialClass();
	//Destroy the static member of the class
	static void DestroyClass();
	//Get the default skin for static
	static BOOL GetDefaultSkin(SKINHEADERCTRL *pSkinHeaderCtrl);

public:
	static WNDPROC          s_DefaultWindowProc;      //Default static window proc
	static SKINHEADERCTRL   s_SkinHeaderCtrl;         //Static Header Control Settings

public:
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//process the WM_LBUTTONDOWN message
	virtual LRESULT OnLButtonDown(UINT nFlags, POINTS point);
	//process the WM_LBUTTONDOWN message
	virtual LRESULT OnLButtonUp(UINT nFlags, POINTS point);
	//process the WM_MOUSELEAVE message
	virtual LRESULT OnMouseLeave();
	//process the WM_MOUSEMOVE message
	virtual LRESULT OnMouseMove(UINT nFlags, POINTS point);
	//process the WM_PAINT message
	virtual LRESULT OnPaint();

protected:
	//Get current button status
	int     GetCurrentStatus(DWORD dwStyle) const;

protected:
	SKINHEADERCTRL    *m_pSkinHeaderCtrl;             //Instance Related Header Control Skin Settings
	int                m_nCurHover;                   //Current hover item
	int                m_nCurPress;                   //Current press item
};



#endif // end of DREAMSKIN_HEADERCTRL_INCLUDE
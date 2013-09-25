//DreamSkinSpinCtrl.h

#ifndef DREAMSKIN_SPINCTRL_INCLUDE
#define DREAMSKIN_SPINCTRL_INCLUDE

#define DEFAULT_SPINCTRL_CLASSNAME_A ("msctls_updown32")
#define DEFAULT_SPINCTRL_CLASSNAME_W (L"msctls_updown32")

#define SPHT_NOWHERE          0
#define SPHT_ARROW_TOP        1
#define SPHT_ARROW_LEFT       1
#define SPHT_ARROW_BOTTOM     2
#define SPHT_ARROW_RIGHT      2

class CDreamSkinSpinCtrl : public CDreamSkinWindow
{
//Contrustor and Destructor
public:
	CDreamSkinSpinCtrl(HWND hWnd, WNDPROC OrgWndProc);
	virtual ~CDreamSkinSpinCtrl();

	virtual void    Reload();

public:
	//Apply new skin
	static BOOL ApplySkin(CDreamSkinLoader *pLoader);
	//Create an instance of this class
	static CDreamSkinSpinCtrl * CreateInstance(HWND hWnd, WNDPROC OrgWndProc);
	//window procedure for dreamskin dialog, which provides basic skin functions
	static LRESULT WINAPI DefWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	//Initial the static member of the class
	static BOOL InitialClass();
	//Destroy the static member of the class
	static void DestroyClass();
	//Get the default skin for static
	static BOOL GetDefaultSkin(SKINSPINCTRL *pSkinSpinCtrl);

public:
	static WNDPROC          s_DefaultWindowProc;      //Default static window proc
	static SKINSPINCTRL     s_SkinSpinCtrl;           //Spin Control Skin Settings

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
	void    DrawLeftButton(HDC hDC, RECT rcDraw, int nDrawStatus);
	void    DrawRightButton(HDC hDC, RECT rcDraw, int nDrawStatus);
	void    DrawTopButton(HDC hDC, RECT rcDraw, int nDrawStatus);
	void    DrawBottomButton(HDC hDC, RECT rcDraw, int nDrawStatus);

	int     SpinCtrlHitTest(RECT rcWindow, POINT point, DWORD dwStyle);
	int     SpinCtrlHitTest(RECT rcWindow, POINTS point, DWORD dwStyle);

protected:
	SKINSPINCTRL    *m_pSkinSpinCtrl;                 //Instance Related Spin Control Skin Settings
	int              m_nHover;                        //Mouse hover item
	int              m_nLButtonDown;                  //LButton down item
	BOOL             m_bIsTracking;                   //Whether is tracking spin ctrl now
};

#endif // end of DREAMSKIN_SPINCTRL_INCLUDE
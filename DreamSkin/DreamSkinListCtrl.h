//DreamSkinListCtrl.h

#ifndef DREAMSKIN_LISTCTRL_INCLUDE
#define DREAMSKIN_LISTCTRL_INCLUDE

#define DEFAULT_LISTCTRL_CLASSNAME_A ("SysListView32")
#define DEFAULT_LISTCTRL_CLASSNAME_W (L"SysListView32")

class CDreamSkinListCtrl : public CDreamSkinWindow
{
//Contrustor and Destructor
public:
	CDreamSkinListCtrl(HWND hWnd, WNDPROC OrgWndProc);
	virtual ~CDreamSkinListCtrl();

	virtual void    Reload();

public:
	//Apply new skin
	static BOOL ApplySkin(CDreamSkinLoader *pLoader);
	//Create an instance of this class
	static CDreamSkinListCtrl * CreateInstance(HWND hWnd, WNDPROC OrgWndProc);
	//window procedure for dreamskin dialog, which provides basic skin functions
	static LRESULT WINAPI DefWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	//Initial the static member of the class
	static BOOL InitialClass();
	//Destroy the static member of the class
	static void DestroyClass();
	//Get the default skin for static
	static BOOL GetDefaultSkin(SKINLISTCTRL *pSkinListCtrl);

public:
	static WNDPROC          s_DefaultWindowProc;      //Default static window proc
	static SKINLISTCTRL     s_SkinListCtrl;           //List Control Skin Settings

public:
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//process the WM_ERASEBKGND message
	virtual LRESULT OnEraseBkgnd(HDC hDC);
	//process the WM_MOUSELEAVE message
	virtual LRESULT OnMouseLeave();
	//process the WM_MOUSEMOVE message
	virtual LRESULT OnMouseMove(UINT nFlags, POINTS point);
	//process the WM_NCHITTEST message
	virtual LRESULT OnNcHitTest(POINTS point);
	//process the WM_NCLBUTTONDBLCLK message
	virtual LRESULT OnNcLButtonDbClick(UINT nHitTest, POINTS point);
	//process the WM_NCLBUTTONDOWN message
	virtual LRESULT OnNcLButtonDown(UINT nHitTest, POINTS point);
	//process the WM_NCMOUSELEAVE
	virtual LRESULT OnNcMouseLeave();
	//process the WM_NCMOUSEMOVE message
	virtual LRESULT OnNcMouseMove(UINT nHitTest, POINTS point);
	//process the WM_NCPAINT message
	virtual LRESULT OnNcPaint(HRGN hRGN);
	//process the SBM_GETSCROLLINFO message
	virtual LRESULT OnGetScrollInfo(int fnBar, LPSCROLLINFO lpsi);

protected:
	//Draw the background
	void    DrawBackground(HDC hDC, RECT rcClient);
	//Draw the border
	void    DrawBorder(HDC hDC, RECT rcWindow);

	//Get current button status
	int     GetCurrentStatus(DWORD dwStyle) const;

protected:
	SKINLISTCTRL     *m_pSkinListCtrl;                //Instance Related List Control Skin Settings
	UINT             m_nSBHover;                      //Current scroll bar hover item
	SCROLLBARTRACKINFO m_ScrollBarTrackInfo;          //Store the scrollbar track info
};

#endif // end of DREAMSKIN_LISTCTRL_INCLUDE
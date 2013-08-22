//DreamSkinListBox.h

#ifndef DREAMSKIN_LISTBOX_INCLUDE
#define DREAMSKIN_LISTBOX_INCLUDE

#define DEFAULT_LISTBOX_CLASSNAME_A ("ListBox")
#define DEFAULT_LISTBOX_CLASSNAME_W (L"ListBox")

class CDreamSkinListBox : public CDreamSkinWindow
{
//Contrustor and Destructor
public:
	CDreamSkinListBox(HWND hWnd, WNDPROC OrgWndProc);
	virtual ~CDreamSkinListBox();

	virtual void    Reload();

public:
	//Apply new skin
	static BOOL ApplySkin(CDreamSkinLoader *pLoader);
	//Create an instance of this class
	static CDreamSkinListBox * CreateInstance(HWND hWnd, WNDPROC OrgWndProc);
	//window procedure for dreamskin dialog, which provides basic skin functions
	static LRESULT WINAPI DefWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	//Initial the static member of the class
	static BOOL InitialClass();
	//Destroy the static member of the class
	static void DestroyClass();
	//Get the default skin for static
	static BOOL GetDefaultSkin(SKINLISTBOX *pSkinListBox);

	//
	static VOID CALLBACK OnTrackingScrollBarTimer(HWND hwnd, UINT uMsg, UINT_PTR idEvent,DWORD dwTime);

public:
	static WNDPROC          s_DefaultWindowProc;      //Default static window proc
	static SKINLISTBOX      s_SkinListBox;            //List Box Skin Settings

public:
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//process the WM_CREATE message
	virtual LRESULT OnCreate(LPCREATESTRUCT lpCreateStruct);
	//process the WM_DRAWITEM message
	virtual LRESULT OnDrawItem(UINT nCtrlID, LPDRAWITEMSTRUCT lpDrawItem);
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

protected:
	//Draw the background
	void    DrawBackground(HDC hDC, RECT rcClient);
	void    DrawBackground(HDC hDC, SKINBACKGROUND* pBackGround, RECT rcDraw);
	//Draw the border
	void    DrawBorder(HDC hDC, RECT rcWindow);
	void    DrawBorder(HDC hDC, SKINBORDER *pLBorder, SKINBORDER *pRBorder, SKINBORDER *pTBorder, SKINBORDER *pBBorder, RECT rcDraw);
	//Draw the title
	void    DrawTitle(HDC hDC, SKINTEXT *pText, RECT rcDraw, WCHAR *wstrTitle);
	//Draw one item
	void    DrawItem(HDC hDC, SKINITEM *pItem, RECT rcItem, WCHAR *wstrTitle);

	//Get current button status
	int     GetCurrentStatus(DWORD dwStyle) const;

protected:
	SKINLISTBOX     *m_pSkinListBox;                  //Instance Related List Box Skin Settings
	int              m_nOwnerDraw;                    //Whether the listbox is owner draw
	int              m_nCurHover;                     //Current hover item
	UINT             m_nSBLButtonDown;                //Store the last scroll bar left button down value to check if need to invoke any event
	UINT             m_nSBHover;                      //Current scroll bar hover item
};


#endif //end of DREAMSKIN_LISTBOX_INCLUDE
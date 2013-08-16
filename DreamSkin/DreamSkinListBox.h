//DreamSkinListBox.h

#ifndef DREAMSKIN_LISTBOX_INCLUDE
#define DREAMSKIN_LISTBOX_INCLUDE

#define DEFAULT_LISTBOX_CLASSNAME_A ("ListBox")
#define DEFAULT_LISTBOX_CLASSNAME_W (L"ListBox")

#define SBHT_NOWHERE          0
#define SBHT_ARROW_TOP        1
#define SBHT_ARROW_BOTTOM     2
#define SBHT_REGION_UP        3
#define SBHT_REGION_DOWN      4
#define SBHT_THUMB            5

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

protected:
	static void GetDefaultBackground(SKINBACKGROUND *pBackground, COLORREF clrBk);
	static void GetDefaultBorder(SKINBORDER *pBorder, COLORREF clrBk, int nWidth);
	static void GetDefaultIcon(SKINICON *pIcon, COLORREF clrDraw);
	static void GetDefaultItem(SKINITEM *pItem, COLORREF clrBk, COLORREF clrTxt);
	static void GetDefaultText(SKINTEXT *pText, COLORREF clrTxt);

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
	//process the WM_NCPAINT message
	virtual LRESULT OnNcPaint(HRGN hRGN);

protected:
	//Draw the background
	void    DrawBackground(HDC hDC, RECT rcClient);
	void    DrawBackground(HDC hDC, SKINBACKGROUND* pBackGround, RECT rcDraw);
	//Draw the border
	void    DrawBorder(HDC hDC, RECT rcWindow);
	void    DrawBorder(HDC hDC, SKINBORDER *pLBorder, SKINBORDER *pRBorder, SKINBORDER *pTBorder, SKINBORDER *pBBorder, RECT rcDraw);
	void    DrawScrollBar(HDC hDC, LPSCROLLINFO lpsi, PSCROLLBARINFO psbi);
	//Draw the title
	void    DrawTitle(HDC hDC, SKINTEXT *pText, RECT rcDraw, WCHAR *wstrTitle);
	//Draw one item
	void    DrawItem(HDC hDC, SKINITEM *pItem, RECT rcItem, WCHAR *wstrTitle);

	//Get the rect of item client area
	RECT    GetItemRectClient(SKINITEM *pItem, RECT rcItem) const;
	//Get current button status
	int     GetCurrentStatus(DWORD dwStyle) const;

	//Get the position of mouse point on scroll bar
	int     ScrollBarDragPos(int fnBar, LPSCROLLINFO lpsi, LPSCROLLBARINFO psbi, POINT point);
	int     ScrollBarHitTest(int fnBar, POINT point);
	int     ScrollBarHitTest(int fnBar, POINTS point);
	void    TrackScrollBar(int fnBar, int nSBHitTest);

protected:
	SKINLISTBOX     *m_pSkinListBox;                  //Instance Related List Box Skin Settings
	int              m_nOwnerDraw;                    //Whether the listbox is owner draw
	int              m_nCurHover;                     //Current hover item
	BOOL             m_bTrackingScorllBar;            //Whether is tracking scroll bar now
};


#endif //end of DREAMSKIN_LISTBOX_INCLUDE
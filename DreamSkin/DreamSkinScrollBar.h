//DreamSkinScrollBar.h

#ifndef DREAMSKIN_SCROLLBAR_INCLUDE
#define DREAMSKIN_SCROLLBAR_INCLUDE

#define DEFAULT_SCROLLBAR_CLASSNAME_A ("ScrollBar")
#define DEFAULT_SCROLLBAR_CLASSNAME_W (L"ScrollBar")

#define SBHT_NOWHERE          0
#define SBHT_ARROW_TOP        1
#define SBHT_ARROW_BOTTOM     2
#define SBHT_REGION_UP        3
#define SBHT_REGION_DOWN      4
#define SBHT_THUMB            5

typedef struct _tag_SCROLLBARTRACKINFO
{
	UINT nSBLButtonDown;
	BOOL bIsTracking;
	int  nTrackPos;
}SCROLLBARTRACKINFO;

class CDreamSkinScrollBar : public CDreamSkinWindow
{
//Contrustor and Destructor
public:
	CDreamSkinScrollBar(HWND hWnd, WNDPROC OrgWndProc);
	virtual ~CDreamSkinScrollBar();

	virtual void    Reload();

public:
	//Used to replace system SetScrollInfo API
	static int WINAPI SetScrollInfo(HWND hWnd, int fnBar, LPCSCROLLINFO lpsi, BOOL fRedraw);
	//Used to replace system SetScrollPos API
	static int WINAPI SetScrollPos(HWND hWnd, int nBar, int nPos, BOOL bRedraw);

	//Used to replace system GetScrollInfo API
	static BOOL WINAPI GetScrollInfo(HWND hWnd, int fnBar, LPSCROLLINFO lpsi);

	//Apply new skin
	static BOOL ApplySkin(CDreamSkinLoader *pLoader);
	//Create an instance of this class
	static CDreamSkinScrollBar * CreateInstance(HWND hWnd, WNDPROC OrgWndProc);
	//window procedure for dreamskin dialog, which provides basic skin functions
	static LRESULT WINAPI DefWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	//Initial the static member of the class
	static BOOL InitialClass();
	//Destroy the static member of the class
	static void DestroyClass();
	//Get the default skin for static
	static BOOL GetDefaultSkin(SKINSCROLLBAR *pSkinScrollBar);

	static void DrawWindowScrollBar(HWND hWnd, HDC hDC, UINT nSBLButtonDown, UINT nSBHover);
	//Get the position of mouse point on scroll bar
	static int  ScrollBarHitTest(HWND hWnd, int fnBar, POINTS point);
	static int  ScrollBarHitTest(HWND hWnd, int fnBar, POINT point);
	static int  ScrollBarHitTest(LPSCROLLBARINFO psbi, int fnBar, POINT point);

	static void TrackScrollBar(HWND hWnd, int fnBar, int nSBHitTest, SCROLLBARTRACKINFO *psbti);

public:
	static WNDPROC          s_DefaultWindowProc;      //Default static window proc
	static SKINSCROLLBAR    s_SkinScrollBar;          //ScrollBar Skin Settings

public:
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//process the SBM_GETSCROLLBARINFO
	virtual LRESULT OnGetScrollBarInfo(PSCROLLBARINFO psbi);
	//process the WM_HSCROLL message
	virtual LRESULT OnHScroll(UINT nSBCode, HWND hWndCtrl);
	//process the WM_LBUTTONDOWN message
	virtual LRESULT OnLButtonDown(UINT nFlags, POINTS point);
	//process the WM_MOUSELEAVE message
	virtual LRESULT OnMouseLeave();
	//process the WM_MOUSEMOVE message
	virtual LRESULT OnMouseMove(UINT nFlags, POINTS point);
	//process the WM_PAINT message
	virtual LRESULT OnPaint();
	//process the WM_VSCROLL message
	virtual LRESULT OnVScroll(UINT nSBCode, HWND hWndCtrl);

protected:
	static void DrawBackground(HDC hDC, RECT rcDraw, int nStatus);
	static void DrawBackground(HDC hDC, SKINBACKGROUND* pBackGround, RECT rcDraw);
	//Draw the border
	static void DrawBorder(HDC hDC, RECT rcWindow, int fnBar, int nStatus);
	static void DrawButton(HDC hDC, RECT rcDraw, int fnBar, int nType, int nStatus);
	static void DrawThumb(HDC hDC, RECT rcDraw, int fnBar, int nStatus);

	//Draw one item
	static void DrawItem(HDC hDC, SKINITEM *pItem, RECT rcItem, WCHAR *wstrTitle);
	
	static void DrawVertScrollBar(HWND hWnd, HDC hDC, UINT nSBLButtonDown, UINT nSBHover);
	static void DrawHorzScrollBar(HWND hWnd, HDC hDC, UINT nSBLButtonDown, UINT nSBHover);
	
	static int  ScrollBarDragPos(int fnBar, LPSCROLLINFO lpsi, LPSCROLLBARINFO psbi, POINT point, int nOffSet, UINT *pSBLButtonDown);

protected:
	void        DrawHorzScrollBar(HDC hDC, DWORD dwStyle);
	void        DrawVertScrollBar(HDC hDC, DWORD dwStyle);
	BOOL        GetScrollBarInfo(RECT rcWindow, int fnBar, LPSCROLLINFO lpsi, PSCROLLBARINFO psbi);

protected:
	SKINSCROLLBAR     *m_pSkinScrollBar;              //Instance Related ScrollBar Skin Settings
	UINT               m_nSBHover;                    //Current scroll bar hover item
	SCROLLBARTRACKINFO m_ScrollBarTrackInfo;          //Store the scrollbar track info
};

#endif // end of DREAMSKIN_SCROLLBAR_INCLUDE
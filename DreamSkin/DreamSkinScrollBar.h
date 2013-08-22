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

class CDreamSkinScrollBar : public CDreamSkinWindow
{
//Contrustor and Destructor
public:
	CDreamSkinScrollBar(HWND hWnd, WNDPROC OrgWndProc);
	virtual ~CDreamSkinScrollBar();

public:
	static WNDPROC          s_DefaultWindowProc;      //Default static window proc
	static SKINSCROLLBAR    s_SkinScrollBar;          //ScrollBar Skin Settings

public:
	//Used to replace system SetScrollInfo API
	static int WINAPI SetScrollInfo(HWND hWnd, int fnBar, LPCSCROLLINFO lpsi, BOOL fRedraw);

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

	static void TrackScrollBar(HWND hWnd, int fnBar, int nSBHitTest, UINT *pSBLButtonDown);

protected:
	static void DrawBackground(HDC hDC, RECT rcDraw, int nStatus);
	static void DrawBackground(HDC hDC, SKINBACKGROUND* pBackGround, RECT rcDraw);
	//Draw the border
	static void DrawBorder(HDC hDC, RECT rcWindow, int fnBar, int nStatus);
	static void DrawBorder(HDC hDC, SKINBORDER *pLBorder, SKINBORDER *pRBorder, SKINBORDER *pTBorder, SKINBORDER *pBBorder, RECT rcDraw);
	static void DrawButton(HDC hDC, RECT rcDraw, int fnBar, int nType, int nStatus);
	static void DrawThumb(HDC hDC, RECT rcDraw, int fnBar, int nStatus);

	//Draw one item
	static void DrawItem(HDC hDC, SKINITEM *pItem, RECT rcItem, WCHAR *wstrTitle);
	
	static void DrawVertScrollBar(HWND hWnd, HDC hDC, UINT nSBLButtonDown, UINT nSBHover);
	static void DrawHorzScrollBar(HWND hWnd, HDC hDC, UINT nSBLButtonDown, UINT nSBHover);
	
	static int  ScrollBarDragPos(int fnBar, LPSCROLLINFO lpsi, LPSCROLLBARINFO psbi, POINT point, UINT *pSBLButtonDown);

protected:
	SKINSCROLLBAR   *m_pSkinScrollBar;                //Instance Related ScrollBar Skin Settings
};

#endif // end of DREAMSKIN_SCROLLBAR_INCLUDE
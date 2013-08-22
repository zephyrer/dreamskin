//DreamSkinWindow.h

#ifndef DREAMSKIN_WINDOW_INCLUDE
#define DREAMSKIN_WINDOW_INCLUDE

//icon aligh left
#define DRAW_ITEM_ICON_HALIGN_LEFT            0x00000001
#define DRAW_ITEM_ICON_HALIGN_RIGHT           0x00000002
#define DRAW_ITEM_ICON_VALIGN_TOP             0x00000004
#define DRAW_ITEM_ICON_VALIGN_BOTTOM          0x00000008

class CDreamSkinWindow
{
public:
	//window procedure for dreamskin window, which provides basic skin functions
	static LRESULT WINAPI DefWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	//call original window procedure for specified window
	static LRESULT OrgWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	//Applies an offset to the RGB components of the specified color.
	static COLORREF OffsetColor(COLORREF clrOld, short shOffset);

	static void GetDefaultBackground(SKINBACKGROUND *pBackground, COLORREF clrBk);
	static void GetDefaultBorder(SKINBORDER *pBorder, COLORREF clrBk, int nWidth, int nDrawType = DRAWTYPE_FILLCOLOR);
	static void GetDefaultIcon(SKINICON *pIcon, COLORREF clrDraw);
	static void GetDefaultItem(SKINITEM *pItem, COLORREF clrBk, COLORREF clrTxt, COLORREF clrBd, int nWidthBd);
	static void GetDefaultSysButton(SKINSYSBUTTON *pSysButton, COLORREF clrBk, COLORREF clrText, COLORREF clrBd);
	static void GetDefaultText(SKINTEXT *pText, COLORREF clrTxt);

	//Get the rect of item client area
	static RECT GetItemRectClient(SKINITEM *pItem, RECT rcItem);
	//Get the rect of icon in specified item
	static RECT GetItemRectIcon(SKINITEM *pItem, RECT rcDraw, DWORD dwDrawOption);

	static void DrawIcon(HDC hDC, RECT rcDraw, SKINICON *pSkinIcon);

public:
	CDreamSkinWindow(HWND hWnd, WNDPROC OrgWndProc);
	virtual ~CDreamSkinWindow();

public:
	//virtual LRESULT CreateWindowRgn(HDC hDC);
	virtual void   Detach();
	
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	
	virtual void    Reload() {};

	virtual inline void UpdateWindow()
	{
		if (m_hWnd && ::IsWindow(m_hWnd))
		{
			::SetWindowPos(m_hWnd, 0, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED );

			::InvalidateRect(m_hWnd, NULL, TRUE);
			::UpdateWindow(m_hWnd);
		}
	}

protected:
	HWND               m_hWnd;
	WNDPROC            m_OrgWndProc;

	BOOL               m_bCreateRegion;
	BOOL               m_bAttached;
};

#endif // DREAMSKIN_WINDOW_INCLUDE
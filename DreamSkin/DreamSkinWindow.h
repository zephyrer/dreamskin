//DreamSkinWindow.h

#ifndef DREAMSKIN_WINDOW_INCLUDE
#define DREAMSKIN_WINDOW_INCLUDE

//typedef struct _RGNFROMBITMAPDATA
//{
//	HBITMAP  hBitmap;
//	COLORREF clrTansColor;
//	RECT     rcWndRect;
//}RGNFROMBITMAPDATA, *RGNFROMBITMAPDATA_PTR;
class CHookedWindowList;

class CDreamSkinWindow
{
public:
	//static HRGN CreateRgnBitmap(HDC hDC, void* pData);
	//static HRGN CreateRgnRect(HDC hDC, void* pData);

	//static void DrawImage(HDC hDC, RECT rcToDraw, HANDLE hImage, int nLBorderWidth, int nRBorderWidth, int nTBorderWidth, int nBBorderWidth, IMGDRAWMODE imgDrawMode);

	//window procedure for dreamskin dialog, which provides basic skin functions
	static LRESULT WINAPI DefWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	//Applies an offset to the RGB components of the specified color.
	static COLORREF OffsetColor(COLORREF clrOld, short shOffset);

	//static LRESULT SetDefaultWindowRgnType(RGNTYPE RgnType, RGNCREATEDATAW_PTR pRgnData);

public:
	//static WNDPROC m_sDefaultWindowProc;

	//static RGNTYPE       s_TypeRgn;       //Windows rgn create method

	//static CREATERGNPROC s_RgnProc;       //Used when the windows rgn create from customer function
	//static void *        s_pRgnData;      //Used when the windows rgn create from customer function

	//static HBITMAP       s_hBmpForRgn;    //Used when the windows rgn create from bitmap
	//static COLORREF      s_clrTrans;      //Used when the windows rgn create from bitmap

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

			RECT rcClient;
			::GetClientRect(m_hWnd, &rcClient);
			::InvalidateRect(m_hWnd, &rcClient, TRUE);
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
//DreamSkinWindow.cpp

#include <windows.h>

#include "DreamSkinMain.h"
#include "DreamSkinWindow.h"

//CREATERGNPROC CDreamSkinWindow::s_RgnProc  = NULL;
//void *        CDreamSkinWindow::s_pRgnData = NULL;
//RGNTYPE       CDreamSkinWindow::s_TypeRgn  = RGN_RECT;
//HBITMAP       CDreamSkinWindow::s_hBmpForRgn = NULL;
//COLORREF      CDreamSkinWindow::s_clrTrans = RGB(255, 255, 255);

LRESULT WINAPI CDreamSkinWindow::DefWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	CDreamSkinWindow *pHookedWindow = theSkinMain.GetHookedWindow(hWnd);
	if (message == WM_NCDESTROY)
	{
		WNDPROC OrgWndProc = pHookedWindow->m_OrgWndProc;
		theSkinMain.DelHookedWindow(hWnd);
		return CallWindowProc(OrgWndProc, hWnd, message, wParam, lParam);
	}
	else
	{
		if(pHookedWindow != NULL)
			return pHookedWindow->DefWindowProc(message, wParam, lParam);

		return 0;
	}
}

/***********************************************************************
 * This function applies an offset to the RGB components of a specified 
 * color.
 *
 * This function can be seen as an easy way to make a color darker or 
 * lighter than its default value.
 *
 * Parameters:
 *     clrOld[IN]
 *         The specified color to be offset.
 *
 *     shOffset[IN]
 *         A short value indicating the offset to apply to the color. 
 *         This value must be between -255 and 255.
 *
 * Return value:
 *     The new color after apply the offset
 **********************************************************************/
COLORREF CDreamSkinWindow::OffsetColor(COLORREF clrOld, short shOffset)
{
	COLORREF clrReturn;

	BYTE	byRed = 0;
	BYTE	byGreen = 0;
	BYTE	byBlue = 0;

	short	shOffsetR = shOffset;
	short	shOffsetG = shOffset;
	short	shOffsetB = shOffset;

	if(shOffset < -255)
		shOffset = -255;

	if(shOffset > 255)
		shOffset = 255;

	// Get RGB components of specified color
	byRed = GetRValue(clrOld);
	byGreen = GetGValue(clrOld);
	byBlue = GetBValue(clrOld);

	// Calculate max. allowed real offset
	if (shOffset > 0)
	{
		if (byRed + shOffset > 255)		shOffsetR = 255 - byRed;
		if (byGreen + shOffset > 255)	shOffsetG = 255 - byGreen;
		if (byBlue + shOffset > 255)	shOffsetB = 255 - byBlue;

		shOffset = min(min(shOffsetR, shOffsetG), shOffsetB);
	} // if
	else
	{
		if (byRed + shOffset < 0)		shOffsetR = -byRed;
		if (byGreen + shOffset < 0)		shOffsetG = -byGreen;
		if (byBlue + shOffset < 0)		shOffsetB = -byBlue;

		shOffset = max(max(shOffsetR, shOffsetG), shOffsetB);
	} // else

	// Set new color
	clrReturn = RGB(byRed + shOffset, byGreen + shOffset, byBlue + shOffset);

	return clrReturn;
}


CDreamSkinWindow::CDreamSkinWindow(HWND hWnd, WNDPROC OrgWndProc)
{
	m_hWnd = hWnd;
	m_OrgWndProc = OrgWndProc;
	m_bCreateRegion = TRUE;
	m_bAttached = TRUE;
}

CDreamSkinWindow::~CDreamSkinWindow()
{
	if (m_bAttached)
		Detach();
}

void CDreamSkinWindow::Detach()
{
	if (m_hWnd && ::IsWindow(m_hWnd))
	{
		if (m_bAttached)
		{
			m_bAttached = FALSE;
			SetWindowLong(m_hWnd, GWL_WNDPROC, (LONG)m_OrgWndProc);
		}
	}
}

LRESULT CDreamSkinWindow::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if(m_OrgWndProc != NULL && m_hWnd != NULL)
		return CallWindowProc(m_OrgWndProc, m_hWnd, message, wParam, lParam);
	else
		return 0;
}


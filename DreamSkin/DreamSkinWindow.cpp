//DreamSkinWindow.cpp

#include <windows.h>

#include "WinGdiEx.h"
#include "DreamSkinMain.h"
#include "DreamSkinLoader.h"
#include "DreamSkinWindow.h"

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

LRESULT CDreamSkinWindow::OrgWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	CDreamSkinWindow *pHookedWindow = theSkinMain.GetHookedWindow(hWnd);
	if (pHookedWindow != NULL)
	{
		WNDPROC OrgWndProc = pHookedWindow->m_OrgWndProc;

		if (message == WM_NCDESTROY)
			theSkinMain.DelHookedWindow(hWnd);

		return CallWindowProc(OrgWndProc, hWnd, message, wParam, lParam);
	}
	else
	{
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

void CDreamSkinWindow::GetDefaultBackground(SKINBACKGROUND *pBackground, COLORREF clrBk)
{
	pBackground->nDefault = 1;
	pBackground->nDrawType = DRAWTYPE_FILLCOLOR;
	pBackground->clrDraw.clrStart = clrBk;
	memset(&(pBackground->imgDraw), 0, sizeof(DRAWIMAGE));
}

void CDreamSkinWindow::GetDefaultBorder(SKINBORDER *pBorder, COLORREF clrBk, int nWidth, int nDrawType)
{
	pBorder->nDefault = 1;
	pBorder->nDrawType = nDrawType;
	pBorder->nWidth = nWidth;
	pBorder->nStart = 0;
	pBorder->nEnd = 0;
	pBorder->clrDraw.clrStart = clrBk;
	memset(&(pBorder->imgDraw), 0, sizeof(DRAWIMAGE));
}

void CDreamSkinWindow::GetDefaultIcon(SKINICON *pIcon, COLORREF clrDraw)
{
	pIcon->nDefault = 1;
	pIcon->nDrawType = DRAWTYPE_CUSTOMIZE;
	
	pIcon->nHeight = 13;
	pIcon->nWidth = 13;
	pIcon->nPadding = 2;

	pIcon->clrDraw.clrStart = clrDraw;
	memset(&(pIcon->imgDraw), 0, sizeof(DRAWIMAGE));
}

void CDreamSkinWindow::GetDefaultItem(SKINITEM *pItem, COLORREF clrBk, COLORREF clrTxt, COLORREF clrBd, int nWidthBd)
{
	pItem->nDefault = 1;
	GetDefaultBackground(&pItem->skinBk, clrBk);

	GetDefaultBorder(&pItem->skinLBorder, clrBd, nWidthBd);
	GetDefaultBorder(&pItem->skinRBorder, clrBd, nWidthBd);
	GetDefaultBorder(&pItem->skinTBorder, clrBd, nWidthBd);
	GetDefaultBorder(&pItem->skinBBorder, clrBd, nWidthBd);

	GetDefaultText(&pItem->skinTxt, clrTxt);
	GetDefaultIcon(&pItem->skinIcon, clrTxt);
}

void CDreamSkinWindow::GetDefaultSysButton(SKINSYSBUTTON *pSysButton, COLORREF clrBk, COLORREF clrText, COLORREF clrBd)
{
	pSysButton->nDefault = 1;
	pSysButton->nDrawType = DRAWTYPE_FILLCOLOR;
	pSysButton->clrBk.clrStart = clrBk;
	pSysButton->clrTxt = clrText;
	pSysButton->clrBd = clrBd;
	memset(&(pSysButton->imgDraw), 0, sizeof(DRAWIMAGE));
}

void CDreamSkinWindow::GetDefaultText(SKINTEXT *pText, COLORREF clrTxt)
{
	pText->nDefault = 1;
	pText->clrDraw = clrTxt;
	pText->bDrawShadow = FALSE;
	pText->clrShadow = RGB(255, 255, 255);

	pText->skinFont.nFontSize = 8;
	pText->skinFont.nBold = 0;
	wcscpy_s(pText->skinFont.wstrFontName, LF_FACESIZE, L"Microsoft Sans Serif");
}

RECT CDreamSkinWindow::GetItemRectClient(SKINITEM *pItem, RECT rcItem)
{
	RECT rcClient;

	rcClient = rcItem;
	rcClient.left = rcClient.left + pItem->skinLBorder.nWidth;
	rcClient.right = rcClient.right - pItem->skinRBorder.nWidth;
	rcClient.top = rcClient.top + pItem->skinTBorder.nWidth;
	rcClient.bottom = rcClient.bottom - pItem->skinBBorder.nWidth;

	return rcClient;
}

RECT CDreamSkinWindow::GetItemRectIcon(SKINITEM *pItem, RECT rcDraw, DWORD dwDrawOption)
{
	RECT rcIcon;
	int nHAlign, nVAlign;

	nHAlign = (dwDrawOption & DRAW_ITEM_ICON_HALIGN_LEFT) + (dwDrawOption & DRAW_ITEM_ICON_HALIGN_RIGHT);
	nVAlign = (dwDrawOption & DRAW_ITEM_ICON_VALIGN_TOP) + (dwDrawOption & DRAW_ITEM_ICON_VALIGN_BOTTOM);
	if (nHAlign == DRAW_ITEM_ICON_HALIGN_LEFT)
	{
		rcIcon.left = rcDraw.left;
		rcIcon.right = min(rcDraw.right, rcIcon.left + pItem->skinIcon.nWidth);
	}
	else if (nHAlign == DRAW_ITEM_ICON_HALIGN_RIGHT)
	{
		rcIcon.right = rcDraw.right;
		rcIcon.left = max(rcDraw.left, rcIcon.right - pItem->skinIcon.nWidth);
	}
	else
	{
		rcIcon.left = max(rcDraw.left, (rcDraw.right + rcDraw.left - pItem->skinIcon.nWidth) / 2);
		rcIcon.right = min(rcDraw.right, rcIcon.left + pItem->skinIcon.nWidth);
	}

	if (nVAlign == DRAW_ITEM_ICON_VALIGN_TOP)
	{
		rcIcon.top = rcDraw.top;
		rcIcon.bottom = min(rcDraw.bottom, rcIcon.top + pItem->skinIcon.nHeight);
	}
	else if (nVAlign == DRAW_ITEM_ICON_VALIGN_BOTTOM)
	{
		rcIcon.bottom = rcDraw.bottom;
		rcIcon.top = max(rcDraw.top, rcIcon.bottom - pItem->skinIcon.nHeight);
	}
	else
	{
		rcIcon.top = max(rcDraw.top, (rcDraw.bottom + rcDraw.top - pItem->skinIcon.nHeight) / 2);
		rcIcon.bottom = min(rcDraw.bottom, rcIcon.top + pItem->skinIcon.nHeight);
	}

	return rcIcon;
}

void CDreamSkinWindow::DrawIcon(HDC hDC, RECT rcDraw, SKINICON *pSkinIcon)
{
	if (pSkinIcon->nDrawType == DRAWTYPE_STRETCHBITMAP && pSkinIcon->imgDraw.hImage)
		::DrawStretchBitmap(hDC, rcDraw.left, rcDraw.top, rcDraw.right - rcDraw.left, rcDraw.bottom - rcDraw.top, pSkinIcon->imgDraw.hImage, pSkinIcon->imgDraw.x, pSkinIcon->imgDraw.y, pSkinIcon->imgDraw.nWidth, pSkinIcon->imgDraw.nHeight);
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


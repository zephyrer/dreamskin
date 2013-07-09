// WinGdiEx.cpp

#include <windows.h>

#include "WinGdiEx.h"

void FillSolidRect(HDC hDC, LPCRECT lpRect, COLORREF clr)
{
	SetBkColor(hDC, clr);
	ExtTextOut(hDC, 0, 0, ETO_OPAQUE, lpRect, NULL, 0, NULL);
}

void FillSolidRect(HDC hDC, int x, int y, int cx, int cy, COLORREF clr)
{
	RECT rect;
	rect.left = x;
	rect.top = y;
	rect.right = x + cx;
	rect.bottom = y + cy;
	SetBkColor(hDC, clr);
	ExtTextOut(hDC, 0, 0, ETO_OPAQUE, &rect, NULL, 0, NULL);
}

void DrawStretchBitmap(HDC hDC, LPRECT lpRectDst, HANDLE hBitmap, LPRECT lpRectSrc)
{
	DrawStretchBitmap(hDC, lpRectDst->left, lpRectDst->top, lpRectDst->right - lpRectDst->left, lpRectDst->bottom - lpRectDst->top, hBitmap, lpRectSrc->left, lpRectSrc->top, lpRectSrc->right - lpRectSrc->left, lpRectSrc->bottom - lpRectSrc->top);
}

void DrawStretchBitmap(HDC hDC, int nXDst, int nYDst, int nWidthDst, int nHeightDst, HANDLE hBitmap, int nXSrc, int nYSrc, int nWidthSrc, int nHeightSrc)
{
	HDC hdcImage = ::CreateCompatibleDC(hDC);
	HBITMAP hOldBmp = (HBITMAP)::SelectObject(hdcImage, hBitmap);
	StretchBlt(hDC, nXDst, nYDst, nWidthDst, nHeightDst, hdcImage, nXSrc, nYSrc, nWidthSrc, nHeightSrc, SRCCOPY);
	SelectObject(hdcImage, hOldBmp);
	DeleteDC(hdcImage);
}

void DrawStretchBitmapEx(HDC hDC, LPRECT lpRectDst, HANDLE hBitmap, LPRECT lpRectSrc, int nStart, int nEnd, int nOrientation)
{
	DrawStretchBitmapEx(hDC, lpRectDst->left, lpRectDst->top, lpRectDst->right - lpRectDst->left, lpRectDst->bottom - lpRectDst->top, hBitmap, lpRectSrc->left, lpRectSrc->top, lpRectSrc->right - lpRectSrc->left, lpRectSrc->bottom - lpRectSrc->top, nStart, nEnd, nOrientation);
}

void DrawStretchBitmapEx(HDC hDC, int nXDst, int nYDst, int nWidthDst, int nHeightDst, HANDLE hBitmap, int nXSrc, int nYSrc, int nWidthSrc, int nHeightSrc, int nStart, int nEnd, int nOrientation)
{
	HDC hdcImage = ::CreateCompatibleDC(hDC);
	HBITMAP hOldBmp = (HBITMAP)::SelectObject(hdcImage, hBitmap);

	if (nOrientation)
	{   //vertical
		if (nStart > 0)
			StretchBlt(hDC, nXDst, nYDst, nWidthDst, nStart, hdcImage, nXSrc, nYSrc, nWidthSrc, nStart, SRCCOPY);

		if (nHeightDst > nStart && nEnd > 0)
			StretchBlt(hDC, nXDst, nYDst + nHeightDst - nEnd, nWidthDst, nEnd, hdcImage, nXSrc, nYSrc + nHeightSrc - nEnd, nWidthSrc, nEnd, SRCCOPY);

		if (nHeightDst > (nStart + nEnd))
			StretchBlt(hDC, nXDst, nYDst + nStart, nWidthDst, nHeightDst - nStart - nEnd, hdcImage, nXSrc, nYSrc + nStart, nWidthSrc, nHeightSrc - nStart - nEnd, SRCCOPY);
	}
	else
	{   //horizontal
		if (nStart > 0)
			StretchBlt(hDC, nXDst, nYDst, nStart, nHeightDst, hdcImage, nXSrc, nYSrc, nStart, nHeightSrc, SRCCOPY);

		if (nWidthDst > nStart && nEnd > 0)
			StretchBlt(hDC, nXDst + nWidthDst - nEnd, nYDst, nEnd, nHeightDst, hdcImage, nXSrc + nWidthSrc - nEnd, nYSrc, nEnd, nHeightSrc, SRCCOPY);

		if (nWidthDst > (nStart + nEnd))
			StretchBlt(hDC, nXDst + nStart, nYDst, nWidthDst - nStart - nEnd, nHeightDst, hdcImage, nXSrc + nStart, nYSrc, nWidthSrc - nStart - nEnd, nHeightSrc, SRCCOPY);
	}

	SelectObject(hdcImage, hOldBmp);
	DeleteDC(hdcImage);
}



void Draw3dRect(HDC hDC, LPCRECT lpRect,	COLORREF clrTopLeft, COLORREF clrBottomRight)
{
	Draw3dRect(hDC, lpRect->left, lpRect->top, lpRect->right - lpRect->left,	lpRect->bottom - lpRect->top, clrTopLeft, clrBottomRight);
}

void Draw3dRect(HDC hDC, int x, int y, int cx, int cy, COLORREF clrTopLeft, COLORREF clrBottomRight)
{
	FillSolidRect(hDC, x, y, cx - 1, 1, clrTopLeft);
	FillSolidRect(hDC, x, y, 1, cy - 1, clrTopLeft);
	FillSolidRect(hDC, x + cx, y, -1, cy, clrBottomRight);
	FillSolidRect(hDC, x, y + cy, cx, -1, clrBottomRight);
}
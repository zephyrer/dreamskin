// WinGdiEx.h

#ifndef WIN_GDIEX_INCLUDE
#define WIN_GDIEX_INCLUDE

void FillSolidRect(HDC hDC, LPCRECT lpRect, COLORREF clr);
void FillSolidRect(HDC hDC, int x, int y, int cx, int cy, COLORREF clr);

void DrawStretchBitmap(HDC hDC, LPRECT lpRectDst, HANDLE hBitmap, LPRECT lpRectSrc);
void DrawStretchBitmap(HDC hDC, int nXDst, int nYDst, int nWidthDst, int nHeightDst, HANDLE hBitmap, int nXSrc, int nYSrc, int nWidthSrc, int nHeightSrc);

void DrawStretchBitmapEx(HDC hDC, LPRECT lpRectDst, HANDLE hBitmap, LPRECT lpRectSrc, int nStart, int nEnd, int nOrientation);
void DrawStretchBitmapEx(HDC hDC, int nXDst, int nYDst, int nWidthDst, int nHeightDst, HANDLE hBitmap, int nXSrc, int nYSrc, int nWidthSrc, int nHeightSrc, int nStart, int nEnd, int nOrientation);

void Draw3dRect(HDC hDC, LPCRECT lpRect,COLORREF clrTopLeft, COLORREF clrBottomRight);
void Draw3dRect(HDC hDC, int x, int y, int cx, int cy, COLORREF clrTopLeft, COLORREF clrBottomRight);


#endif // WIN_GDIEX_INCLUDE
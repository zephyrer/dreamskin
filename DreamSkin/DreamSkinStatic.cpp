//DreamSkinStatic.cpp
#include <windows.h>

#include "WinGdiEx.h"
#include "DreamSkinMain.h"
#include "DreamSkinLoader.h"
#include "DreamSkinWindow.h"
#include "DreamSkinStatic.h"

//static member declaration
WNDPROC         CDreamSkinStatic::s_DefaultWindowProc = NULL;
SKINSTATIC      CDreamSkinStatic::s_SkinStatic;

CDreamSkinStatic::CDreamSkinStatic(HWND hWnd, WNDPROC OrgWndProc)
	: CDreamSkinWindow(hWnd, OrgWndProc)
{
	m_pSkinStatic = &s_SkinStatic;
}

CDreamSkinStatic::~CDreamSkinStatic()
{
	if (m_pSkinStatic != &s_SkinStatic)
		delete m_pSkinStatic;
}

void CDreamSkinStatic::Reload()
{
	if (m_hWnd && ::IsWindow(m_hWnd))
	{
		RECT rcClient;
		::GetClientRect(m_hWnd, &rcClient);

		m_bCreateRegion = TRUE;
		::InvalidateRect(m_hWnd, &rcClient, TRUE);
	}
}

BOOL CDreamSkinStatic::ApplySkin(CDreamSkinLoader *pLoader)
{
	BOOL bResult = TRUE;

	InitialClass();

	SKINSTATIC SkinStatic;
	pLoader->GetSkinStatic(&SkinStatic);
	s_SkinStatic = SkinStatic;

	return bResult;
}

CDreamSkinStatic* CDreamSkinStatic::CreateInstance(HWND hWnd, WNDPROC OrgWndProc)
{
	CDreamSkinStatic *pSkinStatic = new CDreamSkinStatic(hWnd, OrgWndProc);
	return pSkinStatic;
}

LRESULT WINAPI CDreamSkinStatic::DefWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	CDreamSkinWindow *pHookedWindow = theSkinMain.GetHookedWindow(hWnd);
	if (message == WM_NCDESTROY)
	{
		return CDreamSkinWindow::DefWindowProc(hWnd, message, wParam, lParam);
	}
	else
	{
		
		if(pHookedWindow != NULL)
			return pHookedWindow->DefWindowProc(message, wParam, lParam);

		if (s_DefaultWindowProc)
			return CallWindowProc(s_DefaultWindowProc, hWnd, message, wParam, lParam);

		return 0;
	}
}

BOOL CDreamSkinStatic::InitialClass()
{
	//Get the default window proc for this class
	WNDCLASSW clsStaticDefault;
	if(!GetClassInfoW(GetModuleHandle(NULL), DEFAULT_STATIC_CLASSNAME_W, &clsStaticDefault))
	{
		//Get class info failed
		//TODO: add error handle code
		return FALSE;
	}
	s_DefaultWindowProc = clsStaticDefault.lpfnWndProc;

	GetDefaultSkin(&s_SkinStatic);

	return TRUE;
}

void CDreamSkinStatic::DestroyClass()
{

}

BOOL CDreamSkinStatic::GetDefaultSkin(SKINSTATIC *pSkinStatic)
{
	if (pSkinStatic)
	{
		GetDefaultBackground(&pSkinStatic->skinBkNormal, ::GetSysColor(COLOR_BTNFACE));
		GetDefaultText(&pSkinStatic->skinTxtNormal, ::GetSysColor(COLOR_BTNTEXT));
	}

	return TRUE;
}

LRESULT CDreamSkinStatic::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT nResult = ERROR_SUCCESS;

	switch(message)
	{
	case WM_ERASEBKGND:
		break;
	case WM_PAINT:
		nResult = OnPaint();
		break;
	case WM_SETTEXT:
		nResult = CDreamSkinWindow::DefWindowProc(message, wParam, lParam);
		UpdateWindow();
		break;
	default:
		nResult = CDreamSkinWindow::DefWindowProc(message, wParam, lParam);
	}

	return nResult;

}

LRESULT CDreamSkinStatic::OnPaint()
{
	PAINTSTRUCT ps;
	HDC hPaintDC = ::BeginPaint(m_hWnd, &ps);

	::SetBkMode(hPaintDC, TRANSPARENT);

	RECT rcClient;
	::GetClientRect(m_hWnd, &rcClient);

	DrawBackground(hPaintDC, rcClient);

	DrawTitle(hPaintDC, rcClient);

	::EndPaint(m_hWnd, &ps);

	return 0;
}

void CDreamSkinStatic::DrawBackground(HDC hDC, RECT rcClient)
{
	SKINBACKGROUND *pBackGround;

	pBackGround = &m_pSkinStatic->skinBkNormal;


	if (pBackGround->nDrawType == DRAWTYPE_STRETCHBITMAP && pBackGround->imgDraw.hImage)
		::DrawStretchBitmap(hDC, rcClient.left, rcClient.top, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, pBackGround->imgDraw.hImage, pBackGround->imgDraw.x, pBackGround->imgDraw.y, pBackGround->imgDraw.nWidth, pBackGround->imgDraw.nHeight);
	else
		::FillSolidRect(hDC, &rcClient, pBackGround->clrDraw.clrStart);
}

void CDreamSkinStatic::DrawTitle(HDC hDC, RECT rcClient)
{
	SKINTEXT *pText;
	HFONT hFnText, hFnOld = NULL;

	//Get the button title
	int nLen = ::GetWindowTextLengthW(m_hWnd);
	WCHAR *wstrTitle = new WCHAR[nLen + 1];
	::GetWindowTextW(m_hWnd, wstrTitle, nLen + 1);

	DWORD dwStyle = ::GetWindowLong(m_hWnd, GWL_STYLE);

	pText = &m_pSkinStatic->skinTxtNormal;

	if(wstrTitle[0] != 0)
	{
		//Create font
		LOGFONTW fnText;
		fnText.lfHeight = -MulDiv(pText->skinFont.nFontSize, GetDeviceCaps(hDC, LOGPIXELSY), 72);
		fnText.lfWeight = 0;
		fnText.lfEscapement = 0;
		fnText.lfOrientation = 0;
		if (pText->skinFont.nBold)
			fnText.lfWeight = FW_BOLD;
		else
			fnText.lfWeight = FW_NORMAL;
		fnText.lfItalic = 0;
		fnText.lfUnderline = 0;
		fnText.lfStrikeOut = 0;
		fnText.lfCharSet = DEFAULT_CHARSET;
		fnText.lfOutPrecision = OUT_DEFAULT_PRECIS;
		fnText.lfClipPrecision = OUT_DEFAULT_PRECIS; 
		fnText.lfQuality = DEFAULT_QUALITY;
		fnText.lfPitchAndFamily = DEFAULT_PITCH|FF_DONTCARE;
		wcscpy_s(fnText.lfFaceName, LF_FACESIZE, pText->skinFont.wstrFontName);

		hFnText = ::CreateFontIndirectW(&fnText);
		hFnOld = (HFONT)::SelectObject(hDC, hFnText);

		
		RECT rcTitle = rcClient;

		//To center text, get the real draw rect
		if ((dwStyle & 0x03) == SS_CENTER)
		{
			::DrawTextW(hDC, wstrTitle, nLen, &rcClient,  DT_WORDBREAK | DT_LEFT | DT_CALCRECT);
			::OffsetRect(&rcTitle, ((rcTitle.right - rcTitle.left) - (rcClient.right - rcClient.left)) / 2, ((rcTitle.bottom - rcTitle.top) - (rcClient.bottom - rcClient.top)) / 2);
		}
		else if ((dwStyle & 0x03) == SS_RIGHT)
		{
			::DrawTextW(hDC, wstrTitle, nLen, &rcClient,  DT_WORDBREAK | DT_LEFT | DT_CALCRECT);
			::OffsetRect(&rcTitle, (rcTitle.right - rcTitle.left) - (rcClient.right - rcClient.left), ((rcTitle.bottom - rcTitle.top) - (rcClient.bottom - rcClient.top)) / 2);
		}

		//we need to draw the shadow of text
		if(pText->bDrawShadow)
		{
			if ((dwStyle & 0x03) != SS_RIGHT)
				::OffsetRect(&rcTitle, 1, 1);
			::SetTextColor(hDC, pText->clrShadow);
			::DrawTextW(hDC, wstrTitle, nLen, &rcTitle,  DT_WORDBREAK | DT_LEFT);
			::OffsetRect(&rcTitle, -1, -1);
		}
		::SetTextColor(hDC, pText->clrDraw);
		::DrawTextW(hDC, wstrTitle, nLen, &rcTitle,  DT_WORDBREAK | DT_LEFT);

		::SelectObject(hDC, hFnOld);
		::DeleteObject(hFnText);
	}

	delete wstrTitle;
}
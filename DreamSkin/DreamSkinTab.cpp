//DreamSkinTab.cpp

#include <windows.h>
#include <commctrl.h>

#include "WinGdiEx.h"
#include "DreamSkinMain.h"
#include "DreamSkinLoader.h"
#include "DreamSkinWindow.h"
#include "DreamSkinTab.h"

//static member declaration
WNDPROC         CDreamSkinTab::s_DefaultWindowProc = NULL;
SKINTAB         CDreamSkinTab::s_SkinTab;

CDreamSkinTab::CDreamSkinTab(HWND hWnd, WNDPROC OrgWndProc)
	: CDreamSkinWindow(hWnd, OrgWndProc)
{
	m_pSkinTab = &s_SkinTab;
	m_nCurSel = 0;
	m_nCurHover = -1;
}

CDreamSkinTab::~CDreamSkinTab()
{
	if (m_pSkinTab != &s_SkinTab)
		delete m_pSkinTab;
}

void CDreamSkinTab::Reload()
{
	if (m_hWnd && ::IsWindow(m_hWnd))
	{
		RECT rcClient;
		::GetClientRect(m_hWnd, &rcClient);

		m_bCreateRegion = TRUE;
		::InvalidateRect(m_hWnd, &rcClient, TRUE);
	}
}

BOOL CDreamSkinTab::ApplySkin(CDreamSkinLoader *pLoader)
{
	BOOL bResult = TRUE;

	InitialClass();

	SKINTAB SkinTab;
	pLoader->GetSkinTab(&SkinTab);
	s_SkinTab = SkinTab;

	return bResult;
}

CDreamSkinTab* CDreamSkinTab::CreateInstance(HWND hWnd, WNDPROC OrgWndProc)
{
	CDreamSkinTab *pSkinTab = new CDreamSkinTab(hWnd, OrgWndProc);
	return pSkinTab;
}

LRESULT WINAPI CDreamSkinTab::DefWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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

BOOL CDreamSkinTab::InitialClass()
{
	//Get the default window proc for this class
	WNDCLASSW clsTabDefault;
	if(!GetClassInfoW(GetModuleHandle(NULL), DEFAULT_TAB_CLASSNAME_W, &clsTabDefault))
	{
		//Get class info failed
		//TODO: add error handle code
		return FALSE;
	}
	s_DefaultWindowProc = clsTabDefault.lpfnWndProc;

	GetDefaultSkin(&s_SkinTab);

	return TRUE;
}

void CDreamSkinTab::DestroyClass()
{

}

BOOL CDreamSkinTab::GetDefaultSkin(SKINTAB *pSkinTab)
{
	if (pSkinTab)
	{
		GetDefaultBackground(&pSkinTab->skinBkNormal, ::GetSysColor(COLOR_BTNFACE));

		GetDefaultBorder(&pSkinTab->skinLBorderNormal, ::GetSysColor(COLOR_GRAYTEXT), 1);
		GetDefaultBorder(&pSkinTab->skinRBorderNormal, ::GetSysColor(COLOR_GRAYTEXT), 1);
		GetDefaultBorder(&pSkinTab->skinTBorderNormal, ::GetSysColor(COLOR_GRAYTEXT), 1);
		GetDefaultBorder(&pSkinTab->skinBBorderNormal, ::GetSysColor(COLOR_GRAYTEXT), 1);

		GetDefaultButton(&pSkinTab->skinTabButton);

		pSkinTab->nButtonWidth = 21;
		pSkinTab->nButtonMargin = 0;
		pSkinTab->nButtonPadding = 5;
		pSkinTab->nDrawOrder = 0;
	}

	return TRUE;
}

void CDreamSkinTab::GetDefaultBackground(SKINBACKGROUND *pBackground, COLORREF clrBk)
{
	pBackground->nDefault = 1;
	pBackground->nDrawType = DRAWTYPE_FILLCOLOR;
	pBackground->clrDraw.clrStart = clrBk;
	memset(&(pBackground->imgDraw), 0, sizeof(DRAWIMAGE));
}

void CDreamSkinTab::GetDefaultBorder(SKINBORDER *pBorder, COLORREF clrBk, int nWidth, int nDrawType)
{
	pBorder->nDefault = 1;
	pBorder->nDrawType = nDrawType;
	pBorder->nWidth = nWidth;
	pBorder->nStart = 0;
	pBorder->nEnd = 0;
	pBorder->clrDraw.clrStart = clrBk;
	memset(&(pBorder->imgDraw), 0, sizeof(DRAWIMAGE));
}

void CDreamSkinTab::GetDefaultButton(SKINBUTTON *pSkinButton)
{
	pSkinButton->nRgnType = RGN_RECT;
	pSkinButton->clrTansparent = RGB(255, 255, 255);

	GetDefaultBackground(&pSkinButton->skinBkNormal, ::GetSysColor(COLOR_BTNFACE));
	GetDefaultBackground(&pSkinButton->skinBkDisable, ::GetSysColor(COLOR_BTNFACE));
	GetDefaultBackground(&pSkinButton->skinBkHover, ::GetSysColor(COLOR_BTNFACE));
	GetDefaultBackground(&pSkinButton->skinBkPress, ::GetSysColor(COLOR_BTNFACE));
	GetDefaultBackground(&pSkinButton->skinBkDefault, ::GetSysColor(COLOR_BTNFACE));

	GetDefaultText(&pSkinButton->skinTxtNormal, ::GetSysColor(COLOR_BTNTEXT));
	GetDefaultText(&pSkinButton->skinTxtDisable, ::GetSysColor(COLOR_GRAYTEXT));
	GetDefaultText(&pSkinButton->skinTxtHover, ::GetSysColor(COLOR_BTNTEXT));
	GetDefaultText(&pSkinButton->skinTxtPress, ::GetSysColor(COLOR_BTNTEXT));
	GetDefaultText(&pSkinButton->skinTxtDefault, ::GetSysColor(COLOR_BTNTEXT));

	GetDefaultBorder(&pSkinButton->skinLBorderNormal, ::GetSysColor(COLOR_GRAYTEXT), 1);
	GetDefaultBorder(&pSkinButton->skinRBorderNormal, ::GetSysColor(COLOR_GRAYTEXT), 1);
	GetDefaultBorder(&pSkinButton->skinTBorderNormal, ::GetSysColor(COLOR_GRAYTEXT), 1);
	GetDefaultBorder(&pSkinButton->skinBBorderNormal, ::GetSysColor(COLOR_GRAYTEXT), 1, DRAWTYPE_TRANSPARENT);

	GetDefaultBorder(&pSkinButton->skinLBorderDisable, ::GetSysColor(COLOR_GRAYTEXT), 1);
	GetDefaultBorder(&pSkinButton->skinRBorderDisable, ::GetSysColor(COLOR_GRAYTEXT), 1);
	GetDefaultBorder(&pSkinButton->skinTBorderDisable, ::GetSysColor(COLOR_GRAYTEXT), 1);
	GetDefaultBorder(&pSkinButton->skinBBorderDisable, ::GetSysColor(COLOR_GRAYTEXT), 1, DRAWTYPE_TRANSPARENT);

	GetDefaultBorder(&pSkinButton->skinLBorderHover, ::GetSysColor(COLOR_3DHILIGHT), 1);
	GetDefaultBorder(&pSkinButton->skinRBorderHover, ::GetSysColor(COLOR_3DDKSHADOW), 1);
	GetDefaultBorder(&pSkinButton->skinTBorderHover, ::GetSysColor(COLOR_3DHILIGHT), 1);
	GetDefaultBorder(&pSkinButton->skinBBorderHover, ::GetSysColor(COLOR_3DLIGHT), 1, DRAWTYPE_TRANSPARENT);

	GetDefaultBorder(&pSkinButton->skinLBorderPress, ::GetSysColor(COLOR_3DDKSHADOW), 1);
	GetDefaultBorder(&pSkinButton->skinRBorderPress, ::GetSysColor(COLOR_3DHILIGHT), 1);
	GetDefaultBorder(&pSkinButton->skinTBorderPress, ::GetSysColor(COLOR_3DDKSHADOW), 1);
	GetDefaultBorder(&pSkinButton->skinBBorderPress, ::GetSysColor(COLOR_BTNFACE), 1);

	GetDefaultBorder(&pSkinButton->skinLBorderDefault, ::GetSysColor(COLOR_BTNTEXT), 1);
	GetDefaultBorder(&pSkinButton->skinRBorderDefault, ::GetSysColor(COLOR_BTNTEXT), 1);
	GetDefaultBorder(&pSkinButton->skinTBorderDefault, ::GetSysColor(COLOR_BTNTEXT), 1);
	GetDefaultBorder(&pSkinButton->skinBBorderDefault, ::GetSysColor(COLOR_BTNTEXT), 1, DRAWTYPE_TRANSPARENT);
}

void CDreamSkinTab::GetDefaultText(SKINTEXT *pText, COLORREF clrTxt)
{
	pText->nDefault = 1;
	pText->clrDraw = clrTxt;
	pText->bDrawShadow = FALSE;
	pText->clrShadow = RGB(255, 255, 255);

	pText->skinFont.nFontSize = 8;
	pText->skinFont.nBold = 0;
	wcscpy_s(pText->skinFont.wstrFontName, LF_FACESIZE, L"Microsoft Sans Serif");
}

LRESULT CDreamSkinTab::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT nResult = ERROR_SUCCESS;

	switch(message)
	{
	case WM_ERASEBKGND:
		break;
	case WM_MOUSELEAVE:
		nResult = OnMouseLeave();
		break;
	case WM_MOUSEMOVE:
		nResult = OnMouseMove(wParam, MAKEPOINTS(lParam));
		break;
	case WM_PAINT:
		nResult = OnPaint();
		break;
	default:
		nResult = CDreamSkinWindow::DefWindowProc(message, wParam, lParam);
	}

	return nResult;

}

LRESULT CDreamSkinTab::OnMouseLeave()
{
	if(m_nCurHover >= 0)
	{
		m_nCurHover = -1;

		UpdateWindow();
	}

	return 0;
}

LRESULT CDreamSkinTab::OnMouseMove(UINT nHitTest, POINTS point)
{
	TRACKMOUSEEVENT EventTrack;
	EventTrack.cbSize = sizeof(TRACKMOUSEEVENT);
	EventTrack.dwFlags = TME_LEAVE;
	EventTrack.hwndTrack = m_hWnd;
	EventTrack.dwHoverTime = 0;

	TrackMouseEvent(&EventTrack);

	TCHITTESTINFO HitTestInfo;
	HitTestInfo.pt.x = point.x;
	HitTestInfo.pt.y = point.y;
	int nIndex = ::SendMessage(m_hWnd, TCM_HITTEST, 0, (LPARAM)&HitTestInfo);
	if(nIndex != m_nCurHover)
	{
		m_nCurHover = nIndex;
		UpdateWindow();
	}

	return 0;
}

LRESULT CDreamSkinTab::OnPaint()
{
	PAINTSTRUCT ps;
	HDC hPaintDC = ::BeginPaint(m_hWnd, &ps);

	::SetBkMode(hPaintDC, TRANSPARENT);

	RECT rcWindow, rcClient;
	::GetClientRect(m_hWnd, &rcWindow);
	rcClient = GetClientByWindow(rcWindow);

	DrawBackground(hPaintDC, rcWindow, rcClient);

	DrawBorder(hPaintDC, rcWindow, rcClient);

	DrawButtons(hPaintDC, rcWindow, rcClient);

	::EndPaint(m_hWnd, &ps);

	return 0;
}

void CDreamSkinTab::DrawBackground(HDC hDC, RECT rcWindow, RECT rcClient)
{
	HRGN hRgn = ::CreateRectRgn(rcWindow.left, rcWindow.top, rcWindow.right, rcWindow.bottom);
	HRGN hRgnTemp = ::CreateRectRgn(rcClient.left, rcClient.top, rcClient.right, rcClient.bottom);

	::CombineRgn(hRgn, hRgn, hRgnTemp, RGN_XOR);
	::DeleteObject(hRgnTemp);

	HBRUSH hBrush = ::CreateSolidBrush(m_pSkinTab->skinBkNormal.clrDraw.clrStart);
	::FillRgn(hDC, hRgn, hBrush);

	::DeleteObject(hBrush);
	::DeleteObject(hRgn);
}

void CDreamSkinTab::DrawBorder(HDC hDC, RECT rcWindow, RECT rcClient)
{
	RECT rcDraw;
	SKINBORDER *pLBorder, *pRBorder, *pTBorder, *pBBorder;
	
	pLBorder = &m_pSkinTab->skinLBorderNormal;
	pRBorder = &m_pSkinTab->skinRBorderNormal;
	pTBorder = &m_pSkinTab->skinTBorderNormal;
	pBBorder = &m_pSkinTab->skinBBorderNormal;


	//top border
	if (pTBorder->nWidth > 0)
	{
		rcDraw.left = rcWindow.left;
		rcDraw.top = rcClient.top - pTBorder->nWidth;
		rcDraw.right = rcWindow.right;
		rcDraw.bottom = rcClient.top;
		if (pTBorder->nDrawType == DRAWTYPE_STRETCHBITMAP && pTBorder->imgDraw.hImage)
			::DrawStretchBitmapEx(hDC, rcDraw.left, rcDraw.top, rcDraw.right - rcDraw.left, rcDraw.bottom - rcDraw.top, pTBorder->imgDraw.hImage, pTBorder->imgDraw.x, pTBorder->imgDraw.y, pTBorder->imgDraw.nWidth, pTBorder->imgDraw.nHeight, pTBorder->nStart, pTBorder->nEnd, 0);
		else
			::FillSolidRect(hDC, &rcDraw, pTBorder->clrDraw.clrStart);
	}

	//left border
	if (pLBorder->nWidth > 0)
	{
		rcDraw.left = rcWindow.left;
		rcDraw.top = rcClient.top;
		rcDraw.right = rcDraw.left + pLBorder->nWidth;
		rcDraw.bottom = rcClient.bottom;

		if (pLBorder->nDrawType == DRAWTYPE_STRETCHBITMAP && pLBorder->imgDraw.hImage)
			::DrawStretchBitmapEx(hDC, rcDraw.left, rcDraw.top, rcDraw.right - rcDraw.left, rcDraw.bottom - rcDraw.top, pLBorder->imgDraw.hImage, pLBorder->imgDraw.x, pLBorder->imgDraw.y, pLBorder->imgDraw.nWidth, pLBorder->imgDraw.nHeight, pLBorder->nStart, pLBorder->nEnd, 1);
		else
			::FillSolidRect(hDC, &rcDraw, pLBorder->clrDraw.clrStart);
	}

	//right border
	if (pRBorder->nWidth > 0)
	{
		rcDraw.left = rcWindow.right - pRBorder->nWidth;
		rcDraw.top = rcClient.top;
		rcDraw.right = rcWindow.right;
		rcDraw.bottom = rcClient.bottom;

		if (pRBorder->nDrawType == DRAWTYPE_STRETCHBITMAP && pRBorder->imgDraw.hImage)
			::DrawStretchBitmapEx(hDC, rcDraw.left, rcDraw.top, rcDraw.right - rcDraw.left, rcDraw.bottom - rcDraw.top, pRBorder->imgDraw.hImage, pRBorder->imgDraw.x, pRBorder->imgDraw.y, pRBorder->imgDraw.nWidth, pRBorder->imgDraw.nHeight, pRBorder->nStart, pRBorder->nEnd, 1);
		else
			::FillSolidRect(hDC, &rcDraw, pRBorder->clrDraw.clrStart);
	}

	//bottom border
	if (pBBorder->nWidth > 0)
	{
		rcDraw.left = rcWindow.left;
		rcDraw.top = rcClient.bottom;
		rcDraw.right = rcWindow.right;
		rcDraw.bottom = rcWindow.bottom;
		if (pBBorder->nDrawType == DRAWTYPE_STRETCHBITMAP && pBBorder->imgDraw.hImage)
			::DrawStretchBitmapEx(hDC, rcDraw.left, rcDraw.top, rcDraw.right - rcDraw.left, rcDraw.bottom - rcDraw.top, pBBorder->imgDraw.hImage, pBBorder->imgDraw.x, pBBorder->imgDraw.y, pBBorder->imgDraw.nWidth, pBBorder->imgDraw.nHeight, pBBorder->nStart, pBBorder->nEnd, 0);
		else
			::FillSolidRect(hDC, &rcDraw, pBBorder->clrDraw.clrStart);
	}
}

void CDreamSkinTab::DrawButton(HDC hDC, RECT rcItem, int nIndex)
{
	int nStatus = DRAWSTATUS_NORMAL;
	if (nIndex == m_nCurSel)
		nStatus = DRAWSTATUS_PRESS;
	else if (nIndex == m_nCurHover)
		nStatus = DRAWSTATUS_HOVER;

	DrawButtonBorder(hDC, rcItem, nStatus);

	RECT rcClient = GetButtonRectClient(rcItem, nStatus);
	DrawButtonBackground(hDC, rcClient, nStatus);

	DrawButtonTitle(hDC, rcClient, nIndex, nStatus);
}

void CDreamSkinTab::DrawButtonBackground(HDC hDC, RECT rcClient, int nStatus)
{
	SKINBACKGROUND *pBackGround;
	switch (nStatus)
	{
	case DRAWSTATUS_PRESS:
		pBackGround = &m_pSkinTab->skinTabButton.skinBkPress;
		break;
	case DRAWSTATUS_HOVER:
		pBackGround = &m_pSkinTab->skinTabButton.skinBkHover;
		break;
	default:
		pBackGround = &m_pSkinTab->skinTabButton.skinBkNormal;
		break;
	}

	if (pBackGround->nDrawType == DRAWTYPE_STRETCHBITMAP && pBackGround->imgDraw.hImage)
		::DrawStretchBitmap(hDC, rcClient.left, rcClient.top, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, pBackGround->imgDraw.hImage, pBackGround->imgDraw.x, pBackGround->imgDraw.y, pBackGround->imgDraw.nWidth, pBackGround->imgDraw.nHeight);
	else
		::FillSolidRect(hDC, &rcClient, pBackGround->clrDraw.clrStart);
}

void CDreamSkinTab::DrawButtonBorder(HDC hDC, RECT rcItem, int nStatus)
{
	RECT rcDraw;
	SKINBORDER *pLBorder, *pRBorder, *pTBorder, *pBBorder;
	switch (nStatus)
	{
	case DRAWSTATUS_PRESS:
		pLBorder = &m_pSkinTab->skinTabButton.skinLBorderPress;
		pRBorder = &m_pSkinTab->skinTabButton.skinRBorderPress;
		pTBorder = &m_pSkinTab->skinTabButton.skinTBorderPress;
		pBBorder = &m_pSkinTab->skinTabButton.skinBBorderPress;
		break;
	case DRAWSTATUS_HOVER:
		pLBorder = &m_pSkinTab->skinTabButton.skinLBorderHover;
		pRBorder = &m_pSkinTab->skinTabButton.skinRBorderHover;
		pTBorder = &m_pSkinTab->skinTabButton.skinTBorderHover;
		pBBorder = &m_pSkinTab->skinTabButton.skinBBorderHover;
		break;
	default:
		pLBorder = &m_pSkinTab->skinTabButton.skinLBorderNormal;
		pRBorder = &m_pSkinTab->skinTabButton.skinRBorderNormal;
		pTBorder = &m_pSkinTab->skinTabButton.skinTBorderNormal;
		pBBorder = &m_pSkinTab->skinTabButton.skinBBorderNormal;
		break;
	}

	//top border
	if (pTBorder->nWidth > 0)
	{
		rcDraw.left = rcItem.left;
		rcDraw.top = rcItem.top;
		rcDraw.right = rcItem.right;
		rcDraw.bottom = rcDraw.top + pTBorder->nWidth;
		if (pTBorder->nDrawType == DRAWTYPE_STRETCHBITMAP && pTBorder->imgDraw.hImage)
			::DrawStretchBitmapEx(hDC, rcDraw.left, rcDraw.top, rcDraw.right - rcDraw.left, rcDraw.bottom - rcDraw.top, pTBorder->imgDraw.hImage, pTBorder->imgDraw.x, pTBorder->imgDraw.y, pTBorder->imgDraw.nWidth, pTBorder->imgDraw.nHeight, pTBorder->nStart, pTBorder->nEnd, 0);
		else if (pTBorder->nDrawType == DRAWTYPE_TRANSPARENT)
			; //Do nothing
		else
			::FillSolidRect(hDC, &rcDraw, pTBorder->clrDraw.clrStart);
	}

	//left border
	if (pLBorder->nWidth > 0)
	{
		rcDraw.left = rcItem.left;
		rcDraw.top = rcItem.top + pTBorder->nWidth;
		rcDraw.right = rcDraw.left + pLBorder->nWidth;
		rcDraw.bottom = rcItem.bottom - pBBorder->nWidth;

		if (pLBorder->nDrawType == DRAWTYPE_STRETCHBITMAP && pLBorder->imgDraw.hImage)
			::DrawStretchBitmapEx(hDC, rcDraw.left, rcDraw.top, rcDraw.right - rcDraw.left, rcDraw.bottom - rcDraw.top, pLBorder->imgDraw.hImage, pLBorder->imgDraw.x, pLBorder->imgDraw.y, pLBorder->imgDraw.nWidth, pLBorder->imgDraw.nHeight, pLBorder->nStart, pLBorder->nEnd, 1);
		else if (pLBorder->nDrawType == DRAWTYPE_TRANSPARENT)
			; //Do nothing
		else
			::FillSolidRect(hDC, &rcDraw, pLBorder->clrDraw.clrStart);
	}

	//right border
	if (pRBorder->nWidth > 0)
	{
		rcDraw.left = rcItem.right - pRBorder->nWidth;
		rcDraw.top = rcItem.top + pTBorder->nWidth;
		rcDraw.right = rcItem.right;
		rcDraw.bottom = rcItem.bottom - pBBorder->nWidth;

		if (pRBorder->nDrawType == DRAWTYPE_STRETCHBITMAP && pRBorder->imgDraw.hImage)
			::DrawStretchBitmapEx(hDC, rcDraw.left, rcDraw.top, rcDraw.right - rcDraw.left, rcDraw.bottom - rcDraw.top, pRBorder->imgDraw.hImage, pRBorder->imgDraw.x, pRBorder->imgDraw.y, pRBorder->imgDraw.nWidth, pRBorder->imgDraw.nHeight, pRBorder->nStart, pRBorder->nEnd, 1);
		else if (pRBorder->nDrawType == DRAWTYPE_TRANSPARENT)
			; //Do nothing
		else
			::FillSolidRect(hDC, &rcDraw, pRBorder->clrDraw.clrStart);
	}

	//bottom border
	if (pBBorder->nWidth > 0)
	{
		rcDraw.left = rcItem.left;
		rcDraw.top = rcItem.bottom - pBBorder->nWidth;
		rcDraw.right = rcItem.right;
		rcDraw.bottom = rcItem.bottom;
		if (pBBorder->nDrawType == DRAWTYPE_STRETCHBITMAP && pBBorder->imgDraw.hImage)
			::DrawStretchBitmapEx(hDC, rcDraw.left, rcDraw.top, rcDraw.right - rcDraw.left, rcDraw.bottom - rcDraw.top, pBBorder->imgDraw.hImage, pBBorder->imgDraw.x, pBBorder->imgDraw.y, pBBorder->imgDraw.nWidth, pBBorder->imgDraw.nHeight, pBBorder->nStart, pBBorder->nEnd, 0);
		else if (pBBorder->nDrawType == DRAWTYPE_TRANSPARENT)
			; //Do nothing
		else
			::FillSolidRect(hDC, &rcDraw, pBBorder->clrDraw.clrStart);
	}
}

void CDreamSkinTab::DrawButtonTitle(HDC hDC, RECT rcClient, int nIndex, int nStatus)
{
	SKINTEXT *pText;
	HFONT hFnText, hFnOld = NULL;

	//Get the button title
	WCHAR wstrTitle[256];
	memset(wstrTitle, 0, 256 * sizeof(WCHAR));
	TCITEMW TabItem;
	TabItem.mask = TCIF_TEXT;
	TabItem.pszText = wstrTitle;
	TabItem.cchTextMax = 255;
	::SendMessageW(m_hWnd, TCM_GETITEM, nIndex, (LPARAM)&TabItem);

	switch (nStatus)
	{
	case DRAWSTATUS_PRESS:
		pText = &m_pSkinTab->skinTabButton.skinTxtPress;
		break;
	case DRAWSTATUS_HOVER:
		pText = &m_pSkinTab->skinTabButton.skinTxtHover;
		break;
	default:
		pText = &m_pSkinTab->skinTabButton.skinTxtNormal;
		break;
	}

	if(wstrTitle[0] != 0)
	{
		int nLen = wcslen(wstrTitle);

		//Create font
		LOGFONTW fnText;
		memset(&fnText, 0, sizeof(LOGFONTW));
		fnText.lfHeight = -MulDiv(pText->skinFont.nFontSize, GetDeviceCaps(hDC, LOGPIXELSY), 72);
		if (pText->skinFont.nBold)
			fnText.lfWeight = FW_BOLD;
		else
			fnText.lfWeight = FW_NORMAL;
		fnText.lfCharSet = DEFAULT_CHARSET;
		fnText.lfOutPrecision = OUT_DEFAULT_PRECIS;
		fnText.lfClipPrecision = OUT_DEFAULT_PRECIS; 
		fnText.lfQuality = DEFAULT_QUALITY;
		fnText.lfPitchAndFamily = DEFAULT_PITCH|FF_DONTCARE;
		wcscpy_s(fnText.lfFaceName, LF_FACESIZE, pText->skinFont.wstrFontName);

		hFnText = ::CreateFontIndirectW(&fnText);
		hFnOld = (HFONT)::SelectObject(hDC, hFnText);

		//To center text, get the real draw rect
		RECT rcTitle = rcClient;
		::DrawTextW(hDC, wstrTitle, nLen, &rcClient,  DT_WORDBREAK | DT_LEFT | DT_CALCRECT);
		::OffsetRect(&rcTitle, ((rcTitle.right - rcTitle.left) - (rcClient.right - rcClient.left)) / 2, ((rcTitle.bottom - rcTitle.top) - (rcClient.bottom - rcClient.top)) / 2);


		//we need to draw the shadow of text
		if(pText->bDrawShadow)
		{
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
}

void CDreamSkinTab::DrawButtons(HDC hDC, RECT rcWindow, RECT rcClient)
{
	int nItemCount = ::SendMessage(m_hWnd, TCM_GETITEMCOUNT, 0, 0);
	if (nItemCount > 0)
	{
		m_nCurSel = ::SendMessage(m_hWnd, TCM_GETCURSEL, 0, 0);
		int nIndex;
		for (int i = 0; i < nItemCount; i++)
		{
			if (m_pSkinTab->nDrawOrder)
			{
				nIndex = i;
			}
			else
			{
				nIndex = nItemCount - i - 1;
			}

			RECT rcItem;
			::SendMessage(m_hWnd, TCM_GETITEMRECT, nIndex, (LPARAM)&rcItem);
			if (!(rcItem.right < rcClient.left || rcItem.left > rcClient.right))
			{
				DrawButton(hDC, rcItem, nIndex);
			}
		}
	}
}

RECT CDreamSkinTab::GetClientByWindow(RECT rcWindow)
{
	SKINBORDER *pLBorder, *pRBorder, *pTBorder, *pBBorder;
	RECT rcClient;
	
	pLBorder = &m_pSkinTab->skinLBorderNormal;
	pRBorder = &m_pSkinTab->skinRBorderNormal;
	pTBorder = &m_pSkinTab->skinTBorderNormal;
	pBBorder = &m_pSkinTab->skinBBorderNormal;

	rcClient = rcWindow;
	rcClient.left += pLBorder->nWidth;
	rcClient.bottom -= pBBorder->nWidth;
	rcClient.right -= pRBorder->nWidth;

	m_nCurSel = ::SendMessage(m_hWnd, TCM_GETCURSEL, 0, 0);
	if (m_nCurSel >= 0)
	{
		RECT rcItem;
		::SendMessage(m_hWnd, TCM_GETITEMRECT, m_nCurSel, (LPARAM)&rcItem);
		rcClient.top = rcItem.bottom;
	}
	else
	{
		rcClient.top += m_pSkinTab->nButtonWidth;
	}

	return rcClient;
}

RECT CDreamSkinTab::GetButtonRectClient(RECT rcItem, int nStatus)
{
	RECT rcClient;
	SKINBORDER *pLBorder, *pRBorder, *pTBorder, *pBBorder;

	switch (nStatus)
	{
	case DRAWSTATUS_PRESS:
		pLBorder = &m_pSkinTab->skinTabButton.skinLBorderPress;
		pRBorder = &m_pSkinTab->skinTabButton.skinRBorderPress;
		pTBorder = &m_pSkinTab->skinTabButton.skinTBorderPress;
		pBBorder = &m_pSkinTab->skinTabButton.skinBBorderPress;
		break;
	case DRAWSTATUS_HOVER:
		pLBorder = &m_pSkinTab->skinTabButton.skinLBorderHover;
		pRBorder = &m_pSkinTab->skinTabButton.skinRBorderHover;
		pTBorder = &m_pSkinTab->skinTabButton.skinTBorderHover;
		pBBorder = &m_pSkinTab->skinTabButton.skinBBorderHover;
		break;
	default:
		pLBorder = &m_pSkinTab->skinTabButton.skinLBorderNormal;
		pRBorder = &m_pSkinTab->skinTabButton.skinRBorderNormal;
		pTBorder = &m_pSkinTab->skinTabButton.skinTBorderNormal;
		pBBorder = &m_pSkinTab->skinTabButton.skinBBorderNormal;
		break;
	}

	rcClient = rcItem;
	rcClient.left = rcClient.left + pLBorder->nWidth;
	rcClient.right = rcClient.right - pRBorder->nWidth;
	rcClient.top = rcClient.top + pTBorder->nWidth;
	rcClient.bottom = rcClient.bottom - pBBorder->nWidth;

	return rcClient;
}
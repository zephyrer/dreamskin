//DreamSkinButton.cpp
#include <windows.h>

#include "WinGdiEx.h"
#include "DreamSkinMain.h"
#include "DreamSkinLoader.h"
#include "DreamSkinWindow.h"
#include "DreamSkinButton.h"

//static member declaration
WNDPROC         CDreamSkinButton::s_DefaultWindowProc = NULL;
SKINBUTTON      CDreamSkinButton::s_SkinButton;

CDreamSkinButton::CDreamSkinButton(HWND hWnd, WNDPROC OrgWndProc)
	: CDreamSkinWindow(hWnd, OrgWndProc)
{
	Init();
}

CDreamSkinButton::~CDreamSkinButton()
{
	Destroy();
}

void CDreamSkinButton::Init()
{
	m_pSkinButton = &s_SkinButton;

	m_bMouseIn = FALSE;
	m_bLBtnDown = FALSE;
}

void CDreamSkinButton::Destroy()
{
	if (m_pSkinButton != &s_SkinButton)
	{
		delete m_pSkinButton;
		m_pSkinButton = &s_SkinButton;
	}
}

void CDreamSkinButton::Reload()
{
	if (m_hWnd && ::IsWindow(m_hWnd))
	{
		RECT rcClient;
		::GetClientRect(m_hWnd, &rcClient);

		m_bCreateRegion = TRUE;
		::InvalidateRect(m_hWnd, &rcClient, TRUE);
	}
}

BOOL CDreamSkinButton::ApplySkin(CDreamSkinLoader *pLoader)
{
	BOOL bResult = TRUE;

	InitialClass();

	SKINBUTTON SkinButton;
	pLoader->GetSkinButton(&SkinButton);
	s_SkinButton = SkinButton;

	return bResult;
}

CDreamSkinButton* CDreamSkinButton::CreateInstance(HWND hWnd, WNDPROC OrgWndProc)
{
	CDreamSkinButton *pSkinButton = new CDreamSkinButton(hWnd, OrgWndProc);
	return pSkinButton;
}

LRESULT WINAPI CDreamSkinButton::DefWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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

BOOL CDreamSkinButton::InitialClass()
{
	//Get the default window proc for this class
	WNDCLASSW clsButtonDefault;
	if(!GetClassInfoW(GetModuleHandle(NULL), DEFAULT_BUTTON_CLASSNAME_W, &clsButtonDefault))
	{
		//Get class info failed
		//TODO: add error handle code
		return FALSE;
	}
	s_DefaultWindowProc = clsButtonDefault.lpfnWndProc;

	GetDefaultSkin(&s_SkinButton);

	return TRUE;
}

void CDreamSkinButton::DestroyClass()
{

}

BOOL CDreamSkinButton::GetDefaultSkin(SKINBUTTON *pSkinButton)
{
	if (pSkinButton)
	{
		pSkinButton->nRgnType = RGN_RECT;
		pSkinButton->clrTansparent = RGB(255, 255, 255);

		GetDefaultBackground(&pSkinButton->skinBkNormal, ::GetSysColor(COLOR_BTNFACE));
		GetDefaultBackground(&pSkinButton->skinBkDisable, ::GetSysColor(COLOR_BTNFACE));
		GetDefaultBackground(&pSkinButton->skinBkHover, ::GetSysColor(COLOR_3DLIGHT));
		GetDefaultBackground(&pSkinButton->skinBkPress, ::GetSysColor(COLOR_3DLIGHT));
		GetDefaultBackground(&pSkinButton->skinBkDefault, ::GetSysColor(COLOR_BTNFACE));

		GetDefaultText(&pSkinButton->skinTxtNormal, ::GetSysColor(COLOR_BTNTEXT));
		GetDefaultText(&pSkinButton->skinTxtDisable, ::GetSysColor(COLOR_GRAYTEXT));
		GetDefaultText(&pSkinButton->skinTxtHover, ::GetSysColor(COLOR_BTNTEXT));
		GetDefaultText(&pSkinButton->skinTxtPress, ::GetSysColor(COLOR_BTNTEXT));
		GetDefaultText(&pSkinButton->skinTxtDefault, ::GetSysColor(COLOR_BTNTEXT));

		GetDefaultBorder(&pSkinButton->skinLBorderNormal, ::GetSysColor(COLOR_BTNTEXT), 1);
		GetDefaultBorder(&pSkinButton->skinRBorderNormal, ::GetSysColor(COLOR_BTNTEXT), 1);
		GetDefaultBorder(&pSkinButton->skinTBorderNormal, ::GetSysColor(COLOR_BTNTEXT), 1);
		GetDefaultBorder(&pSkinButton->skinBBorderNormal, ::GetSysColor(COLOR_BTNTEXT), 1);

		GetDefaultBorder(&pSkinButton->skinLBorderDisable, ::GetSysColor(COLOR_GRAYTEXT), 1);
		GetDefaultBorder(&pSkinButton->skinRBorderDisable, ::GetSysColor(COLOR_GRAYTEXT), 1);
		GetDefaultBorder(&pSkinButton->skinTBorderDisable, ::GetSysColor(COLOR_GRAYTEXT), 1);
		GetDefaultBorder(&pSkinButton->skinBBorderDisable, ::GetSysColor(COLOR_GRAYTEXT), 1);

		GetDefaultBorder(&pSkinButton->skinLBorderHover, ::GetSysColor(COLOR_3DHILIGHT), 1);
		GetDefaultBorder(&pSkinButton->skinRBorderHover, ::GetSysColor(COLOR_3DDKSHADOW), 1);
		GetDefaultBorder(&pSkinButton->skinTBorderHover, ::GetSysColor(COLOR_3DHILIGHT), 1);
		GetDefaultBorder(&pSkinButton->skinBBorderHover, ::GetSysColor(COLOR_3DDKSHADOW), 1);

		GetDefaultBorder(&pSkinButton->skinLBorderPress, ::GetSysColor(COLOR_3DDKSHADOW), 1);
		GetDefaultBorder(&pSkinButton->skinRBorderPress, ::GetSysColor(COLOR_3DHILIGHT), 1);
		GetDefaultBorder(&pSkinButton->skinTBorderPress, ::GetSysColor(COLOR_3DDKSHADOW), 1);
		GetDefaultBorder(&pSkinButton->skinBBorderPress, ::GetSysColor(COLOR_3DHILIGHT), 1);

		GetDefaultBorder(&pSkinButton->skinLBorderDefault, ::GetSysColor(COLOR_BTNTEXT), 2);
		GetDefaultBorder(&pSkinButton->skinRBorderDefault, ::GetSysColor(COLOR_BTNTEXT), 2);
		GetDefaultBorder(&pSkinButton->skinTBorderDefault, ::GetSysColor(COLOR_BTNTEXT), 2);
		GetDefaultBorder(&pSkinButton->skinBBorderDefault, ::GetSysColor(COLOR_BTNTEXT), 2);
	}

	return TRUE;
}

void CDreamSkinButton::GetDefaultBackground(SKINBACKGROUND *pBackground, COLORREF clrBk)
{
	pBackground->nDefault = 1;
	pBackground->nDrawType = DRAWTYPE_FILLCOLOR;
	pBackground->clrDraw.clrStart = clrBk;
	memset(&(pBackground->imgDraw), 0, sizeof(DRAWIMAGE));
}

void CDreamSkinButton::GetDefaultBorder(SKINBORDER *pBorder, COLORREF clrBk, int nWidth)
{
	pBorder->nDefault = 1;
	pBorder->nDrawType = DRAWTYPE_FILLCOLOR;
	pBorder->nWidth = nWidth;
	pBorder->nStart = 0;
	pBorder->nEnd = 0;
	pBorder->clrDraw.clrStart = clrBk;
	memset(&(pBorder->imgDraw), 0, sizeof(DRAWIMAGE));
}

void CDreamSkinButton::GetDefaultText(SKINTEXT *pText, COLORREF clrTxt)
{
	pText->nDefault = 1;
	pText->clrDraw = clrTxt;
	pText->bDrawShadow = FALSE;
	pText->clrShadow = RGB(255, 255, 255);

	pText->skinFont.nFontSize = 8;
	pText->skinFont.nBold = 0;
	wcscpy_s(pText->skinFont.wstrFontName, LF_FACESIZE, L"Microsoft Sans Serif");
}

LRESULT CDreamSkinButton::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT nResult = ERROR_SUCCESS;

	DWORD dwButtonStyle = ::GetWindowLong(m_hWnd, GWL_STYLE) & 0xff;
	if((dwButtonStyle == BS_PUSHBUTTON) || (dwButtonStyle == BS_DEFPUSHBUTTON))
	{
		switch(message)
		{
		case WM_ERASEBKGND:
			break;
		case WM_LBUTTONDOWN:
			nResult = OnLButtonDown(wParam, MAKEPOINTS(lParam));
			break;
		case WM_LBUTTONDBLCLK:
			nResult = ::SendMessageW(m_hWnd, WM_LBUTTONDOWN, wParam, lParam);
			break;
		case WM_LBUTTONUP:
			nResult = OnLButtonUp(wParam, MAKEPOINTS(lParam));
			break;
		case WM_MOUSELEAVE:
			nResult = OnMouseLeave();
			break;
		case WM_MOUSEMOVE:
			nResult = OnMouseMove(wParam, MAKEPOINTS(lParam));
			break;
		case WM_NCHITTEST:
			nResult = OnNcHitTest(MAKEPOINTS(lParam));
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
	}
	else
	{
		nResult = CallWindowProc(m_OrgWndProc, m_hWnd, message, wParam, lParam);
	}
	return nResult;

}

LRESULT CDreamSkinButton::OnLButtonDown(UINT nFlags, POINTS point)
{
	if(!m_bLBtnDown)
	{
		m_bLBtnDown = TRUE;
		m_bMouseIn  = TRUE;

		::SetFocus(m_hWnd);
		::SetCapture(m_hWnd);

		UpdateWindow();
	}

	return 0;
}

LRESULT CDreamSkinButton::OnLButtonUp(UINT nFlags, POINTS point)
{
	RECT rcClient;
	POINT ptTemp;
	HRGN hRGN = CreateRectRgn(0, 0, 0, 0);
	if (::GetWindowRgn(m_hWnd, hRGN) != ERROR)
	{
		if(::PtInRegion(hRGN, point.x, point.y))
		{
			if(m_bLBtnDown)
			{
				m_bLBtnDown = FALSE;
				::ReleaseCapture();
				UpdateWindow();
				::SendMessage(::GetParent(m_hWnd), WM_COMMAND, ::GetDlgCtrlID(m_hWnd), (LPARAM)m_hWnd);
			}
		}
		else
		{
			if (m_bLBtnDown)
			{
				m_bLBtnDown = FALSE;
				::ReleaseCapture();
				UpdateWindow();
			}
		}
	}
	else
	{
		ptTemp.x = point.x;
		ptTemp.y = point.y;
		::GetClientRect(m_hWnd, &rcClient);
		if (::PtInRect(&rcClient, ptTemp))
		{
			if(m_bLBtnDown)
			{
				m_bLBtnDown = FALSE;
				::ReleaseCapture();
				UpdateWindow();
				::SendMessage(::GetParent(m_hWnd), WM_COMMAND, ::GetDlgCtrlID(m_hWnd), (LPARAM)m_hWnd);
			}
		}
		else
		{
			if (m_bLBtnDown)
			{
				m_bLBtnDown = FALSE;
				::ReleaseCapture();
				UpdateWindow();
			}
		}
	}

	::DeleteObject(hRGN);

	return 0;
}

LRESULT CDreamSkinButton::OnMouseLeave()
{
	if(m_bMouseIn)
	{
		m_bMouseIn = FALSE;

		UpdateWindow();
	}

	return 0;
}

LRESULT CDreamSkinButton::OnMouseMove(UINT nHitTest, POINTS point)
{
	TRACKMOUSEEVENT EventTrack;
	EventTrack.cbSize = sizeof(TRACKMOUSEEVENT);
	EventTrack.dwFlags = TME_LEAVE;
	EventTrack.hwndTrack = m_hWnd;
	EventTrack.dwHoverTime = 0;

	TrackMouseEvent(&EventTrack);

	if(!m_bMouseIn)
	{
		m_bMouseIn = TRUE;
		UpdateWindow();
	}

	return 0;
}

LRESULT CDreamSkinButton::OnNcHitTest(POINTS point)
{
	RECT rcWindow;
	::GetWindowRect(m_hWnd, &rcWindow);
	
	POINT ptTemp;

	ptTemp.x = point.x;
	ptTemp.y = point.y;

	if (::PtInRect(&rcWindow, ptTemp))
		return HTCLIENT;
	else
		return HTNOWHERE;
}


/***********************************************************************
 * This function will draw the client area of the button. It is called 
 * every time the button client area needs to be painted. 
 *
 * This is a virtual function that can be rewritten in CDreamSkinButton-
 * derived classes to produce a whole range of button not available by 
 * default.
 *
 * Parameters:
 *     None.
 *
 * Return value:
 *     None.
 **********************************************************************/
LRESULT CDreamSkinButton::OnPaint()
{
	PAINTSTRUCT ps;
	HDC hPaintDC = ::BeginPaint(m_hWnd, &ps);

	::SetBkMode(hPaintDC, TRANSPARENT);

	RECT rcClient;
	::GetClientRect(m_hWnd, &rcClient);

	DrawBorder(hPaintDC, rcClient);

	rcClient = GetRectClient(rcClient);
	DrawBackground(hPaintDC, rcClient);

	DrawTitle(hPaintDC, rcClient);

	::EndPaint(m_hWnd, &ps);

	return 0;
}

void CDreamSkinButton::DrawBackground(HDC hDC, RECT rcClient)
{
	SKINBACKGROUND *pBackGround;
	int nStatus = GetCurrentStatus();
	switch (nStatus)
	{
	case DRAWSTATUS_DEFAULT:
		pBackGround = &m_pSkinButton->skinBkDefault;
		break;
	case DRAWSTATUS_PRESS:
		pBackGround = &m_pSkinButton->skinBkPress;
		break;
	case DRAWSTATUS_HOVER:
		pBackGround = &m_pSkinButton->skinBkHover;
		break;
	case DRAWSTATUS_DISABLE:
		pBackGround = &m_pSkinButton->skinBkDisable;
		break;
	default:
		pBackGround = &m_pSkinButton->skinBkNormal;
		break;
	}

	if (pBackGround->nDrawType == DRAWTYPE_STRETCHBITMAP && pBackGround->imgDraw.hImage)
		::DrawStretchBitmap(hDC, rcClient.left, rcClient.top, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, pBackGround->imgDraw.hImage, pBackGround->imgDraw.x, pBackGround->imgDraw.y, pBackGround->imgDraw.nWidth, pBackGround->imgDraw.nHeight);
	else
		::FillSolidRect(hDC, &rcClient, pBackGround->clrDraw.clrStart);
}

void CDreamSkinButton::DrawBorder(HDC hDC, RECT rcWindow)
{
	RECT rcDraw;
	SKINBORDER *pLBorder, *pRBorder, *pTBorder, *pBBorder;
	int nStatus = GetCurrentStatus();
	switch (nStatus)
	{
	case DRAWSTATUS_DEFAULT:
		pLBorder = &m_pSkinButton->skinLBorderDefault;
		pRBorder = &m_pSkinButton->skinRBorderDefault;
		pTBorder = &m_pSkinButton->skinTBorderDefault;
		pBBorder = &m_pSkinButton->skinBBorderDefault;
		break;
	case DRAWSTATUS_PRESS:
		pLBorder = &m_pSkinButton->skinLBorderPress;
		pRBorder = &m_pSkinButton->skinRBorderPress;
		pTBorder = &m_pSkinButton->skinTBorderPress;
		pBBorder = &m_pSkinButton->skinBBorderPress;
		break;
	case DRAWSTATUS_HOVER:
		pLBorder = &m_pSkinButton->skinLBorderHover;
		pRBorder = &m_pSkinButton->skinRBorderHover;
		pTBorder = &m_pSkinButton->skinTBorderHover;
		pBBorder = &m_pSkinButton->skinBBorderHover;
		break;
	case DRAWSTATUS_DISABLE:
		pLBorder = &m_pSkinButton->skinLBorderDisable;
		pRBorder = &m_pSkinButton->skinRBorderDisable;
		pTBorder = &m_pSkinButton->skinTBorderDisable;
		pBBorder = &m_pSkinButton->skinBBorderDisable;
		break;
	default:
		pLBorder = &m_pSkinButton->skinLBorderNormal;
		pRBorder = &m_pSkinButton->skinRBorderNormal;
		pTBorder = &m_pSkinButton->skinTBorderNormal;
		pBBorder = &m_pSkinButton->skinBBorderNormal;
		break;
	}

	//top border
	if (pTBorder->nWidth > 0)
	{
		rcDraw.left = rcWindow.left;
		rcDraw.top = rcWindow.top;
		rcDraw.right = rcWindow.right;
		rcDraw.bottom = rcDraw.top + pTBorder->nWidth;
		if (pTBorder->nDrawType == DRAWTYPE_STRETCHBITMAP && pTBorder->imgDraw.hImage)
			::DrawStretchBitmapEx(hDC, rcDraw.left, rcDraw.top, rcDraw.right - rcDraw.left, rcDraw.bottom - rcDraw.top, pTBorder->imgDraw.hImage, pTBorder->imgDraw.x, pTBorder->imgDraw.y, pTBorder->imgDraw.nWidth, pTBorder->imgDraw.nHeight, pTBorder->nStart, pTBorder->nEnd, 0);
		else
			::FillSolidRect(hDC, &rcDraw, pTBorder->clrDraw.clrStart);
	}

	//left border
	if (pLBorder->nWidth > 0)
	{
		rcDraw.left = rcWindow.left;
		rcDraw.top = rcWindow.top + pTBorder->nWidth;
		rcDraw.right = rcDraw.left + pLBorder->nWidth;
		rcDraw.bottom = rcWindow.bottom - rcWindow.top - pBBorder->nWidth;

		if (pLBorder->nDrawType == DRAWTYPE_STRETCHBITMAP && pLBorder->imgDraw.hImage)
			::DrawStretchBitmapEx(hDC, rcDraw.left, rcDraw.top, rcDraw.right - rcDraw.left, rcDraw.bottom - rcDraw.top, pLBorder->imgDraw.hImage, pLBorder->imgDraw.x, pLBorder->imgDraw.y, pLBorder->imgDraw.nWidth, pLBorder->imgDraw.nHeight, pLBorder->nStart, pLBorder->nEnd, 1);
		else
			::FillSolidRect(hDC, &rcDraw, pLBorder->clrDraw.clrStart);
	}

	//right border
	if (pRBorder->nWidth > 0)
	{
		rcDraw.left = rcWindow.right - pRBorder->nWidth;
		rcDraw.top = rcWindow.top + pTBorder->nWidth;
		rcDraw.right = rcWindow.right;
		rcDraw.bottom = rcWindow.bottom - rcWindow.top - pBBorder->nWidth;

		if (pRBorder->nDrawType == DRAWTYPE_STRETCHBITMAP && pRBorder->imgDraw.hImage)
			::DrawStretchBitmapEx(hDC, rcDraw.left, rcDraw.top, rcDraw.right - rcDraw.left, rcDraw.bottom - rcDraw.top, pRBorder->imgDraw.hImage, pRBorder->imgDraw.x, pRBorder->imgDraw.y, pRBorder->imgDraw.nWidth, pRBorder->imgDraw.nHeight, pRBorder->nStart, pRBorder->nEnd, 1);
		else
			::FillSolidRect(hDC, &rcDraw, pRBorder->clrDraw.clrStart);
	}

	//bottom border
	if (pBBorder->nWidth > 0)
	{
		rcDraw.left = rcWindow.left;
		rcDraw.top = rcWindow.bottom - pBBorder->nWidth;
		rcDraw.right = rcWindow.right;
		rcDraw.bottom = rcWindow.bottom;
		if (pBBorder->nDrawType == DRAWTYPE_STRETCHBITMAP && pBBorder->imgDraw.hImage)
			::DrawStretchBitmapEx(hDC, rcDraw.left, rcDraw.top, rcDraw.right - rcDraw.left, rcDraw.bottom - rcDraw.top, pBBorder->imgDraw.hImage, pBBorder->imgDraw.x, pBBorder->imgDraw.y, pBBorder->imgDraw.nWidth, pBBorder->imgDraw.nHeight, pBBorder->nStart, pBBorder->nEnd, 0);
		else
			::FillSolidRect(hDC, &rcDraw, pBBorder->clrDraw.clrStart);
	}
}

void CDreamSkinButton::DrawTitle(HDC hDC, RECT rcClient)
{
	SKINTEXT *pText;
	HFONT hFnText, hFnOld = NULL;

	//Get the button title
	int nLen = ::GetWindowTextLengthW(m_hWnd);
	WCHAR *wstrTitle = new WCHAR[nLen + 1];
	::GetWindowTextW(m_hWnd, wstrTitle, nLen + 1);

	int nStatus = GetCurrentStatus();
	switch (nStatus)
	{
	case DRAWSTATUS_DEFAULT:
		pText = &m_pSkinButton->skinTxtDefault;
		break;
	case DRAWSTATUS_PRESS:
		pText = &m_pSkinButton->skinTxtPress;
		break;
	case DRAWSTATUS_HOVER:
		pText = &m_pSkinButton->skinTxtHover;
		break;
	case DRAWSTATUS_DISABLE:
		pText = &m_pSkinButton->skinTxtDisable;
		break;
	default:
		pText = &m_pSkinButton->skinTxtNormal;
		break;
	}

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

	delete wstrTitle;
}

int CDreamSkinButton::GetCurrentStatus() const
{
	int nStatus;

	DWORD dwStyle = ::GetWindowLong(m_hWnd, GWL_STYLE);
	if(dwStyle & WS_DISABLED)
		nStatus = DRAWSTATUS_DISABLE;       //Disabled
	else if(m_bMouseIn)
		if(m_bLBtnDown)
			nStatus = DRAWSTATUS_PRESS;     //Pressed
		else
			nStatus = DRAWSTATUS_HOVER;     //Hover
	else
		if((dwStyle & 0xff) == BS_DEFPUSHBUTTON)
			nStatus = DRAWSTATUS_DEFAULT;   //Default
		else
			nStatus = DRAWSTATUS_NORMAL;    //Normal

	return nStatus;
}

RECT CDreamSkinButton::GetRectClient(RECT rcWindow)
{
	RECT rcClient;
	SKINBORDER *pLBorder, *pRBorder, *pTBorder, *pBBorder;

	int nStatus = GetCurrentStatus();
	switch (nStatus)
	{
	case DRAWSTATUS_DEFAULT:
		pLBorder = &m_pSkinButton->skinLBorderDefault;
		pRBorder = &m_pSkinButton->skinRBorderDefault;
		pTBorder = &m_pSkinButton->skinTBorderDefault;
		pBBorder = &m_pSkinButton->skinBBorderDefault;
		break;
	case DRAWSTATUS_PRESS:
		pLBorder = &m_pSkinButton->skinLBorderPress;
		pRBorder = &m_pSkinButton->skinRBorderPress;
		pTBorder = &m_pSkinButton->skinTBorderPress;
		pBBorder = &m_pSkinButton->skinBBorderPress;
		break;
	case DRAWSTATUS_HOVER:
		pLBorder = &m_pSkinButton->skinLBorderHover;
		pRBorder = &m_pSkinButton->skinRBorderHover;
		pTBorder = &m_pSkinButton->skinTBorderHover;
		pBBorder = &m_pSkinButton->skinBBorderHover;
		break;
	case DRAWSTATUS_DISABLE:
		pLBorder = &m_pSkinButton->skinLBorderDisable;
		pRBorder = &m_pSkinButton->skinRBorderDisable;
		pTBorder = &m_pSkinButton->skinTBorderDisable;
		pBBorder = &m_pSkinButton->skinBBorderDisable;
		break;
	default:
		pLBorder = &m_pSkinButton->skinLBorderNormal;
		pRBorder = &m_pSkinButton->skinRBorderNormal;
		pTBorder = &m_pSkinButton->skinTBorderNormal;
		pBBorder = &m_pSkinButton->skinBBorderNormal;
		break;
	}

	rcClient = rcWindow;
	rcClient.left = rcClient.left + pLBorder->nWidth;
	rcClient.right = rcClient.right - pRBorder->nWidth;
	rcClient.top = rcClient.top + pTBorder->nWidth;
	rcClient.bottom = rcClient.bottom - pBBorder->nWidth;

	return rcClient;
}
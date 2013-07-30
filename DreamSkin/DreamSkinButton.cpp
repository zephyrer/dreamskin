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
SKINCHECKBOX    CDreamSkinButton::s_SkinCheckBox;

CDreamSkinButton::CDreamSkinButton(HWND hWnd, WNDPROC OrgWndProc)
	: CDreamSkinWindow(hWnd, OrgWndProc)
{
	m_pSkinButton = &s_SkinButton;
	m_pSkinCheckBox = &s_SkinCheckBox;

	m_bMouseIn = FALSE;
	m_bLBtnDown = FALSE;
}

CDreamSkinButton::~CDreamSkinButton()
{
	if (m_pSkinButton != &s_SkinButton)
		delete m_pSkinButton;

	if (m_pSkinCheckBox != &s_SkinCheckBox)
		delete m_pSkinCheckBox;
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

	SKINCHECKBOX SkinCheckBox;
	pLoader->GetSkinCheckBox(&SkinCheckBox);
	s_SkinCheckBox = SkinCheckBox;

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

	GetDefaultButtonSkin(&s_SkinButton);
	GetDefaultCheckBoxSkin(&s_SkinCheckBox);

	return TRUE;
}

void CDreamSkinButton::DestroyClass()
{

}

BOOL CDreamSkinButton::GetDefaultButtonSkin(SKINBUTTON *pSkinButton)
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

BOOL CDreamSkinButton::GetDefaultCheckBoxSkin(SKINCHECKBOX *pSkinCheckBox)
{
	if (pSkinCheckBox)
	{
		pSkinCheckBox->nRgnType = RGN_RECT;
		pSkinCheckBox->clrTansparent = RGB(255, 255, 255);

		GetDefaultBackground(&pSkinCheckBox->skinBkNormalChecked, ::GetSysColor(COLOR_BTNFACE));
		GetDefaultBackground(&pSkinCheckBox->skinBkNormalPartChecked, ::GetSysColor(COLOR_BTNFACE));
		GetDefaultBackground(&pSkinCheckBox->skinBkNormalUnchecked, ::GetSysColor(COLOR_BTNFACE));

		GetDefaultBackground(&pSkinCheckBox->skinBkDisableChecked, ::GetSysColor(COLOR_BTNFACE));
		GetDefaultBackground(&pSkinCheckBox->skinBkDisablePartChecked, ::GetSysColor(COLOR_BTNFACE));
		GetDefaultBackground(&pSkinCheckBox->skinBkDisableUnchecked, ::GetSysColor(COLOR_BTNFACE));

		GetDefaultBackground(&pSkinCheckBox->skinBkHoverChecked, ::GetSysColor(COLOR_3DLIGHT));
		GetDefaultBackground(&pSkinCheckBox->skinBkHoverPartChecked, ::GetSysColor(COLOR_3DLIGHT));
		GetDefaultBackground(&pSkinCheckBox->skinBkHoverUnchecked, ::GetSysColor(COLOR_3DLIGHT));

		GetDefaultBackground(&pSkinCheckBox->skinBkPressChecked, ::GetSysColor(COLOR_3DLIGHT));
		GetDefaultBackground(&pSkinCheckBox->skinBkPressPartChecked, ::GetSysColor(COLOR_3DLIGHT));
		GetDefaultBackground(&pSkinCheckBox->skinBkPressUnchecked, ::GetSysColor(COLOR_3DLIGHT));

		GetDefaultText(&pSkinCheckBox->skinTxtNormalChecked, ::GetSysColor(COLOR_BTNTEXT));
		GetDefaultText(&pSkinCheckBox->skinTxtDisableChecked, ::GetSysColor(COLOR_GRAYTEXT));
		GetDefaultText(&pSkinCheckBox->skinTxtHoverChecked, ::GetSysColor(COLOR_BTNTEXT));
		GetDefaultText(&pSkinCheckBox->skinTxtPressChecked, ::GetSysColor(COLOR_BTNTEXT));

		GetDefaultText(&pSkinCheckBox->skinTxtNormalPartChecked, ::GetSysColor(COLOR_BTNTEXT));
		GetDefaultText(&pSkinCheckBox->skinTxtDisablePartChecked, ::GetSysColor(COLOR_GRAYTEXT));
		GetDefaultText(&pSkinCheckBox->skinTxtHoverPartChecked, ::GetSysColor(COLOR_BTNTEXT));
		GetDefaultText(&pSkinCheckBox->skinTxtPressPartChecked, ::GetSysColor(COLOR_BTNTEXT));

		GetDefaultText(&pSkinCheckBox->skinTxtNormalUnchecked, ::GetSysColor(COLOR_BTNTEXT));
		GetDefaultText(&pSkinCheckBox->skinTxtDisableUnchecked, ::GetSysColor(COLOR_GRAYTEXT));
		GetDefaultText(&pSkinCheckBox->skinTxtHoverUnchecked, ::GetSysColor(COLOR_BTNTEXT));
		GetDefaultText(&pSkinCheckBox->skinTxtPressUnchecked, ::GetSysColor(COLOR_BTNTEXT));

		GetDefaultIcon(&pSkinCheckBox->iconNormalChecked, ::GetSysColor(COLOR_BTNTEXT));
		GetDefaultIcon(&pSkinCheckBox->iconDisableChecked, ::GetSysColor(COLOR_GRAYTEXT));
		GetDefaultIcon(&pSkinCheckBox->iconHoverChecked, ::GetSysColor(COLOR_BTNTEXT));
		GetDefaultIcon(&pSkinCheckBox->iconPressChecked, ::GetSysColor(COLOR_BTNTEXT));

		GetDefaultIcon(&pSkinCheckBox->iconNormalPartChecked, ::GetSysColor(COLOR_BTNTEXT));
		GetDefaultIcon(&pSkinCheckBox->iconDisablePartChecked, ::GetSysColor(COLOR_GRAYTEXT));
		GetDefaultIcon(&pSkinCheckBox->iconHoverPartChecked, ::GetSysColor(COLOR_BTNTEXT));
		GetDefaultIcon(&pSkinCheckBox->iconPressPartChecked, ::GetSysColor(COLOR_BTNTEXT));

		GetDefaultIcon(&pSkinCheckBox->iconNormalUnchecked, ::GetSysColor(COLOR_BTNTEXT));
		GetDefaultIcon(&pSkinCheckBox->iconDisableUnchecked, ::GetSysColor(COLOR_GRAYTEXT));
		GetDefaultIcon(&pSkinCheckBox->iconHoverUnchecked, ::GetSysColor(COLOR_BTNTEXT));
		GetDefaultIcon(&pSkinCheckBox->iconPressUnchecked, ::GetSysColor(COLOR_BTNTEXT));
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

void CDreamSkinButton::GetDefaultIcon(SKINICON *pIcon, COLORREF clrDraw)
{
	pIcon->nDefault = 1;
	pIcon->nDrawType = DRAWTYPE_CUSTOMIZE;
	
	pIcon->nHeight = 13;
	pIcon->nWidth = 13;
	pIcon->nPadding = 2;

	pIcon->clrDraw.clrStart = clrDraw;
	memset(&(pIcon->imgDraw), 0, sizeof(DRAWIMAGE));
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

	DWORD dwButtonStyle = ::GetWindowLong(m_hWnd, GWL_STYLE) & 0x0f;

	if(dwButtonStyle == BS_PUSHBUTTON || dwButtonStyle == BS_DEFPUSHBUTTON)
		m_nBtnType = BUTTON_TYPE_PUSHBUTTON;
	else if (dwButtonStyle == BS_CHECKBOX || dwButtonStyle == BS_AUTOCHECKBOX || dwButtonStyle == BS_3STATE || dwButtonStyle == BS_AUTO3STATE)
		m_nBtnType = BUTTON_TYPE_CHECKBOX;
	else
		m_nBtnType = BUTTON_TYPE_UNSUPPORTED;

	if(m_nBtnType >= 0)
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
		case WM_NCPAINT:
			break;
		case WM_PAINT:
			nResult = OnPaint();
			break;
		//case BM_SETSTATE:
		//case BM_SETCHECK:
		case BM_SETSTYLE:
			nResult = OnSetButtonStyle(wParam & 0x0000FFFF);
			//OnPaint();
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
	DWORD dwButtonStyle = ::GetWindowLong(m_hWnd, GWL_STYLE) & 0x0f;
	int   nCheckStatus;
	BOOL  bBtnClick = FALSE;
	HRGN hRGN = CreateRectRgn(0, 0, 0, 0);
	if (::GetWindowRgn(m_hWnd, hRGN) != ERROR)
	{
		if(::PtInRegion(hRGN, point.x, point.y))
		{
			if(m_bLBtnDown)
			{
				m_bLBtnDown = FALSE;
				::ReleaseCapture();
				bBtnClick = TRUE;
			}
		}
		else
		{
			if (m_bLBtnDown)
			{
				m_bLBtnDown = FALSE;
				m_bMouseIn = FALSE;
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
				bBtnClick = TRUE;
			}
		}
		else
		{
			if (m_bLBtnDown || m_bMouseIn)
			{
				m_bLBtnDown = FALSE;
				m_bMouseIn = FALSE;
				::ReleaseCapture();
				UpdateWindow();
			}
		}
	}

	::DeleteObject(hRGN);

	if (bBtnClick)
	{
		switch (m_nBtnType)
		{
		case BUTTON_TYPE_CHECKBOX:
			nCheckStatus = ::SendMessage(m_hWnd, BM_GETCHECK, 0, 0);
			switch (dwButtonStyle)
			{
			case BS_AUTO3STATE:
				nCheckStatus = (nCheckStatus + 1) % 3;
				::SendMessage(m_hWnd, BM_SETCHECK, nCheckStatus, 0);
				break;
			case BS_AUTOCHECKBOX:
				nCheckStatus = (nCheckStatus + 1) % 2;
				::SendMessage(m_hWnd, BM_SETCHECK, nCheckStatus, 0);
				break;
			default:
				UpdateWindow();
				break;
			}
			::SendMessage(::GetParent(m_hWnd), WM_COMMAND, ::GetDlgCtrlID(m_hWnd), (LPARAM)m_hWnd);
			break;
		default:
			UpdateWindow();
			::SendMessage(::GetParent(m_hWnd), WM_COMMAND, ::GetDlgCtrlID(m_hWnd), (LPARAM)m_hWnd);
			break;
		}
	}

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
	if(!m_bMouseIn)
	{
		TRACKMOUSEEVENT EventTrack;
		EventTrack.cbSize = sizeof(TRACKMOUSEEVENT);
		EventTrack.dwFlags = TME_LEAVE;
		EventTrack.hwndTrack = m_hWnd;
		EventTrack.dwHoverTime = 0;

		TrackMouseEvent(&EventTrack);

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

	DWORD dwButtonStyle = ::GetWindowLong(m_hWnd, GWL_STYLE) & 0x0f;
	if (dwButtonStyle == BS_PUSHBUTTON || dwButtonStyle == BS_DEFPUSHBUTTON)
		DrawPushButton(hPaintDC, rcClient);
	else if (dwButtonStyle == BS_CHECKBOX || dwButtonStyle == BS_AUTOCHECKBOX || dwButtonStyle == BS_3STATE || dwButtonStyle == BS_AUTO3STATE)
		DrawCheckBox(hPaintDC, rcClient);

	::EndPaint(m_hWnd, &ps);

	return 0;
}

LRESULT CDreamSkinButton::OnSetButtonStyle(DWORD dwNewStyle)
{
	DWORD dwStyle = ::GetWindowLong(m_hWnd, GWL_STYLE) & 0xFFFF0000 | dwNewStyle;
	::SetWindowLong(m_hWnd, GWL_STYLE, dwStyle);

	return 0;
}

void CDreamSkinButton::DrawPushButton(HDC hDC, RECT rcWindow)
{
	DrawPushButtonBorder(hDC, rcWindow);

	RECT rcClient = GetPushButtonRectClient(rcWindow);
	DrawPushButtonBackground(hDC, rcClient);

	DrawPushButtonTitle(hDC, rcClient);
}

void CDreamSkinButton::DrawCheckBox(HDC hDC, RECT rcWindow)
{
	int nCheckStatus = ::SendMessage(m_hWnd, BM_GETCHECK, 0, 0);
	DrawCheckBoxBackground(hDC, rcWindow, nCheckStatus);

	DrawCheckBoxForeground(hDC, rcWindow, nCheckStatus);
}

void CDreamSkinButton::DrawPushButtonBackground(HDC hDC, RECT rcClient)
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

void CDreamSkinButton::DrawPushButtonBorder(HDC hDC, RECT rcWindow)
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

void CDreamSkinButton::DrawPushButtonTitle(HDC hDC, RECT rcClient)
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

	delete wstrTitle;
}

void CDreamSkinButton::DrawCheckBoxBackground(HDC hDC, RECT rcClient, int nCheckStatus)
{
	SKINBACKGROUND *pBackGround;
	int nStatus = GetCurrentStatus();
	switch (nStatus)
	{
	case DRAWSTATUS_PRESS:
		switch (nCheckStatus)
		{
		case BST_CHECKED:
			pBackGround = &m_pSkinCheckBox->skinBkPressChecked;
			break;
		case BST_INDETERMINATE:
			pBackGround = &m_pSkinCheckBox->skinBkPressPartChecked;
			break;
		default:
			pBackGround = &m_pSkinCheckBox->skinBkPressUnchecked;
			break;
		}
		break;
	case DRAWSTATUS_HOVER:
		switch (nCheckStatus)
		{
		case BST_CHECKED:
			pBackGround = &m_pSkinCheckBox->skinBkHoverChecked;
			break;
		case BST_INDETERMINATE:
			pBackGround = &m_pSkinCheckBox->skinBkHoverPartChecked;
			break;
		default:
			pBackGround = &m_pSkinCheckBox->skinBkHoverUnchecked;
			break;
		}
		break;
	case DRAWSTATUS_DISABLE:
		switch (nCheckStatus)
		{
		case BST_CHECKED:
			pBackGround = &m_pSkinCheckBox->skinBkDisableChecked;
			break;
		case BST_INDETERMINATE:
			pBackGround = &m_pSkinCheckBox->skinBkDisablePartChecked;
			break;
		default:
			pBackGround = &m_pSkinCheckBox->skinBkDisableUnchecked;
			break;
		}
		break;
	default:
		switch (nCheckStatus)
		{
		case BST_CHECKED:
			pBackGround = &m_pSkinCheckBox->skinBkNormalChecked;
			break;
		case BST_INDETERMINATE:
			pBackGround = &m_pSkinCheckBox->skinBkNormalPartChecked;
			break;
		default:
			pBackGround = &m_pSkinCheckBox->skinBkNormalUnchecked;
			break;
		}
		break;
	}

	if (pBackGround->nDrawType == DRAWTYPE_STRETCHBITMAP && pBackGround->imgDraw.hImage)
		::DrawStretchBitmap(hDC, rcClient.left, rcClient.top, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, pBackGround->imgDraw.hImage, pBackGround->imgDraw.x, pBackGround->imgDraw.y, pBackGround->imgDraw.nWidth, pBackGround->imgDraw.nHeight);
	else
		::FillSolidRect(hDC, &rcClient, pBackGround->clrDraw.clrStart);
}

void CDreamSkinButton::DrawCheckBoxForeground(HDC hDC, RECT rcClient, int nCheckStatus)
{
	SKINICON *pSkinIcon;
	SKINTEXT *pSkinText;
	int nStatus = GetCurrentStatus();
	switch (nStatus)
	{
	case DRAWSTATUS_PRESS:
		switch (nCheckStatus)
		{
		case BST_CHECKED:
			pSkinIcon = &m_pSkinCheckBox->iconPressChecked;
			pSkinText = &m_pSkinCheckBox->skinTxtPressChecked;
			break;
		case BST_INDETERMINATE:
			pSkinIcon = &m_pSkinCheckBox->iconPressPartChecked;
			pSkinText = &m_pSkinCheckBox->skinTxtPressPartChecked;
			break;
		default:
			pSkinIcon = &m_pSkinCheckBox->iconPressUnchecked;
			pSkinText = &m_pSkinCheckBox->skinTxtPressUnchecked;
			break;
		}
		break;
	case DRAWSTATUS_HOVER:
		switch (nCheckStatus)
		{
		case BST_CHECKED:
			pSkinIcon = &m_pSkinCheckBox->iconHoverChecked;
			pSkinText = &m_pSkinCheckBox->skinTxtHoverChecked;
			break;
		case BST_INDETERMINATE:
			pSkinIcon = &m_pSkinCheckBox->iconHoverPartChecked;
			pSkinText = &m_pSkinCheckBox->skinTxtHoverPartChecked;
			break;
		default:
			pSkinIcon = &m_pSkinCheckBox->iconHoverUnchecked;
			pSkinText = &m_pSkinCheckBox->skinTxtHoverUnchecked;
			break;
		}
		break;
	case DRAWSTATUS_DISABLE:
		switch (nCheckStatus)
		{
		case BST_CHECKED:
			pSkinIcon = &m_pSkinCheckBox->iconDisableChecked;
			pSkinText = &m_pSkinCheckBox->skinTxtDisableChecked;
			break;
		case BST_INDETERMINATE:
			pSkinIcon = &m_pSkinCheckBox->iconDisablePartChecked;
			pSkinText = &m_pSkinCheckBox->skinTxtDisablePartChecked;
			break;
		default:
			pSkinIcon = &m_pSkinCheckBox->iconDisableUnchecked;
			pSkinText = &m_pSkinCheckBox->skinTxtDisableUnchecked;
			break;
		}
		break;
	default:
		switch (nCheckStatus)
		{
		case BST_CHECKED:
			pSkinIcon = &m_pSkinCheckBox->iconNormalChecked;
			pSkinText = &m_pSkinCheckBox->skinTxtNormalChecked;
			break;
		case BST_INDETERMINATE:
			pSkinIcon = &m_pSkinCheckBox->iconNormalPartChecked;
			pSkinText = &m_pSkinCheckBox->skinTxtNormalPartChecked;
			break;
		default:
			pSkinIcon = &m_pSkinCheckBox->iconNormalUnchecked;
			pSkinText = &m_pSkinCheckBox->skinTxtNormalUnchecked;
			break;
		}
		break;
	}

	RECT rcDraw = GetCheckBoxRectIcon(rcClient, pSkinIcon);
	DrawCheckBoxIcon(hDC, rcDraw, pSkinIcon, nCheckStatus);

	rcDraw.left = rcDraw.right + rcDraw.left - rcClient.left;
	rcDraw.right = rcClient.right;
	rcDraw.top = rcClient.top;
	rcDraw.bottom = rcClient.bottom;
	DrawCheckBoxTitle(hDC, rcDraw, pSkinText);
}

void CDreamSkinButton::DrawCheckBoxIcon(HDC hDC, RECT rcDraw, SKINICON *pSkinIcon, int nCheckStatus)
{
	HPEN hDrawPen, hOldPen;

	if (pSkinIcon->nDrawType == DRAWTYPE_STRETCHBITMAP && pSkinIcon->imgDraw.hImage)
		::DrawStretchBitmap(hDC, rcDraw.left, rcDraw.top, rcDraw.right - rcDraw.left, rcDraw.bottom - rcDraw.top, pSkinIcon->imgDraw.hImage, pSkinIcon->imgDraw.x, pSkinIcon->imgDraw.y, pSkinIcon->imgDraw.nWidth, pSkinIcon->imgDraw.nHeight);
	else	
	{
		switch(nCheckStatus)
		{
		case BST_CHECKED:
			::Draw3dRect(hDC, &rcDraw, pSkinIcon->clrDraw.clrStart, pSkinIcon->clrDraw.clrStart);
			hDrawPen = ::CreatePen(PS_SOLID, 2, pSkinIcon->clrDraw.clrStart);
			hOldPen = (HPEN)::SelectObject(hDC, hDrawPen);
			::MoveToEx(hDC, rcDraw.left + (rcDraw.right - rcDraw.left) / 4, rcDraw.top + (rcDraw.bottom - rcDraw.top) / 2, NULL);
			::LineTo(hDC, rcDraw.left + (rcDraw.right - rcDraw.left) / 2, rcDraw.top + (rcDraw.bottom - rcDraw.top) * 3 / 4);
			::LineTo(hDC, rcDraw.left + (rcDraw.right - rcDraw.left) * 3 / 4, rcDraw.top + (rcDraw.bottom - rcDraw.top) / 4);
			::SelectObject(hDC, hOldPen);
			::DeleteObject(hDrawPen);
			break;
		case BST_INDETERMINATE:
			::Draw3dRect(hDC, &rcDraw, pSkinIcon->clrDraw.clrStart, pSkinIcon->clrDraw.clrStart);
			if ((rcDraw.right - rcDraw.left) > 4)
			{
				rcDraw.right -= 2;
				rcDraw.left += 2;
			}
			else if ((rcDraw.right - rcDraw.left) > 2)
			{
				rcDraw.right -= 1;
				rcDraw.left += 1;
			}
			if ((rcDraw.bottom - rcDraw.top) > 4)
			{
				rcDraw.bottom -= 2;
				rcDraw.top += 2;
			}
			else if ((rcDraw.bottom - rcDraw.bottom) > 2)
			{
				rcDraw.bottom -= 1;
				rcDraw.top += 1;
			}
			::FillSolidRect(hDC, &rcDraw, pSkinIcon->clrDraw.clrStart);
			break;
		default:
			::Draw3dRect(hDC, &rcDraw, pSkinIcon->clrDraw.clrStart, pSkinIcon->clrDraw.clrStart);
			break;
		}
	}
}

void CDreamSkinButton::DrawCheckBoxTitle(HDC hDC, RECT rcClient, SKINTEXT *pSkinText)
{
	HFONT hFnText, hFnOld = NULL;

	//Get the button title
	int nLen = ::GetWindowTextLengthW(m_hWnd);
	WCHAR *wstrTitle = new WCHAR[nLen + 1];
	::GetWindowTextW(m_hWnd, wstrTitle, nLen + 1);

	if(wstrTitle[0] != 0)
	{
		//Create font
		LOGFONTW fnText;
		fnText.lfHeight = -MulDiv(pSkinText->skinFont.nFontSize, GetDeviceCaps(hDC, LOGPIXELSY), 72);
		fnText.lfWeight = 0;
		fnText.lfEscapement = 0;
		fnText.lfOrientation = 0;
		if (pSkinText->skinFont.nBold)
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
		wcscpy_s(fnText.lfFaceName, LF_FACESIZE, pSkinText->skinFont.wstrFontName);

		hFnText = ::CreateFontIndirectW(&fnText);
		hFnOld = (HFONT)::SelectObject(hDC, hFnText);

		//we need to draw the shadow of text
		if(pSkinText->bDrawShadow)
		{
			::OffsetRect(&rcClient, 1, 1);
			::SetTextColor(hDC, pSkinText->clrShadow);
			::DrawTextW(hDC, wstrTitle, nLen, &rcClient,  DT_WORDBREAK | DT_LEFT);
			::OffsetRect(&rcClient, -1, -1);
		}
		::SetTextColor(hDC, pSkinText->clrDraw);
		::DrawTextW(hDC, wstrTitle, nLen, &rcClient,  DT_WORDBREAK | DT_LEFT);

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
		if((dwStyle & 0x0f) == BS_DEFPUSHBUTTON)
			nStatus = DRAWSTATUS_DEFAULT;   //Default
		else
			nStatus = DRAWSTATUS_NORMAL;    //Normal

	return nStatus;
}

RECT CDreamSkinButton::GetPushButtonRectClient(RECT rcWindow)
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

RECT CDreamSkinButton::GetCheckBoxRectIcon(RECT rcWindow, SKINICON *pSkinIcon)
{
	RECT rcDraw;
	if ((rcWindow.right - rcWindow.left) > (pSkinIcon->nWidth + 2 * pSkinIcon->nPadding))
		rcDraw.left = rcWindow.left + pSkinIcon->nPadding;
	else
		rcDraw.left = rcWindow.left + (rcWindow.right - rcWindow.left - pSkinIcon->nWidth) / 2;
	if ((rcWindow.bottom - rcWindow.top) > (pSkinIcon->nHeight + 2 * pSkinIcon->nPadding))
		rcDraw.top = rcWindow.top + pSkinIcon->nPadding;
	else
		rcDraw.top = rcWindow.top + (rcWindow.bottom - rcWindow.top - pSkinIcon->nHeight) / 2;
	rcDraw.right = rcDraw.left + pSkinIcon->nWidth;
	rcDraw.bottom = rcDraw.top + pSkinIcon->nHeight;

	return rcDraw;
}

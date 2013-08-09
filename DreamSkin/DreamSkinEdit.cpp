//DreamSkinEdit.cpp

#include <windows.h>

#include "WinGdiEx.h"
#include "DreamSkinMain.h"
#include "DreamSkinLoader.h"
#include "DreamSkinWindow.h"
#include "DreamSkinEdit.h"

//static member declaration
WNDPROC         CDreamSkinEdit::s_DefaultWindowProc = NULL;
SKINEDIT        CDreamSkinEdit::s_SkinEdit;

CDreamSkinEdit::CDreamSkinEdit(HWND hWnd, WNDPROC OrgWndProc)
	: CDreamSkinWindow(hWnd, OrgWndProc)
{
	m_pSkinEdit = &s_SkinEdit;

	m_hBkNormal = CreateBackgroundBrush(&m_pSkinEdit->skinBkNormal);
	m_hBkDisable = CreateBackgroundBrush(&m_pSkinEdit->skinBkDisable);
	m_hBkHover = CreateBackgroundBrush(&m_pSkinEdit->skinBkHover);
	m_hBkPress = CreateBackgroundBrush(&m_pSkinEdit->skinBkPress);

	m_hBkNormalReadOnly = CreateBackgroundBrush(&m_pSkinEdit->skinBkNormalReadOnly);
	m_hBkHoverReadOnly = CreateBackgroundBrush(&m_pSkinEdit->skinBkHoverReadOnly);
	m_hBkPressReadOnly = CreateBackgroundBrush(&m_pSkinEdit->skinBkPressReadOnly);

	m_bMouseIn = FALSE;
}

CDreamSkinEdit::~CDreamSkinEdit()
{
	if (m_pSkinEdit != &s_SkinEdit)
		delete m_pSkinEdit;

	::DeleteObject(m_hBkNormal);
	::DeleteObject(m_hBkDisable);
	::DeleteObject(m_hBkHover);
	::DeleteObject(m_hBkPress);

	::DeleteObject(m_hBkNormalReadOnly);
	::DeleteObject(m_hBkHoverReadOnly);
	::DeleteObject(m_hBkPressReadOnly);
}

void CDreamSkinEdit::Reload()
{
	if (m_pSkinEdit == &s_SkinEdit)
	{
		HBRUSH hTemp = m_hBkNormal;
		m_hBkNormal = CreateBackgroundBrush(&m_pSkinEdit->skinBkNormal);
		::DeleteObject(hTemp);

		hTemp = m_hBkDisable;
		m_hBkDisable = CreateBackgroundBrush(&m_pSkinEdit->skinBkDisable);
		::DeleteObject(hTemp);

		hTemp = m_hBkHover;
		m_hBkHover = CreateBackgroundBrush(&m_pSkinEdit->skinBkHover);
		::DeleteObject(hTemp);

		hTemp = m_hBkPress;
		m_hBkPress = CreateBackgroundBrush(&m_pSkinEdit->skinBkPress);
		::DeleteObject(hTemp);

		hTemp = m_hBkNormalReadOnly;
		m_hBkNormalReadOnly = CreateBackgroundBrush(&m_pSkinEdit->skinBkNormalReadOnly);
		::DeleteObject(hTemp);

		hTemp = m_hBkHoverReadOnly;
		m_hBkHoverReadOnly = CreateBackgroundBrush(&m_pSkinEdit->skinBkHoverReadOnly);
		::DeleteObject(hTemp);

		hTemp = m_hBkPressReadOnly;
		m_hBkPressReadOnly = CreateBackgroundBrush(&m_pSkinEdit->skinBkPressReadOnly);
		::DeleteObject(hTemp);
	}

	if (m_hWnd && ::IsWindow(m_hWnd))
	{
		RECT rcClient;
		::GetClientRect(m_hWnd, &rcClient);

		::SetWindowPos(m_hWnd, 0, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED );

		m_bCreateRegion = TRUE;
		::InvalidateRect(m_hWnd, &rcClient, TRUE);
		::UpdateWindow(m_hWnd);
	}
}

BOOL CDreamSkinEdit::ApplySkin(CDreamSkinLoader *pLoader)
{
	BOOL bResult = TRUE;

	InitialClass();

	SKINEDIT SkinEdit;
	pLoader->GetSkinEdit(&SkinEdit);
	s_SkinEdit = SkinEdit;

	return bResult;
}

CDreamSkinEdit* CDreamSkinEdit::CreateInstance(HWND hWnd, WNDPROC OrgWndProc)
{
	CDreamSkinEdit *pSkinEdit = new CDreamSkinEdit(hWnd, OrgWndProc);
	return pSkinEdit;
}

LRESULT WINAPI CDreamSkinEdit::DefWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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

BOOL CDreamSkinEdit::InitialClass()
{
	//Get the default window proc for this class
	WNDCLASSW clsEditDefault;
	if(!GetClassInfoW(GetModuleHandle(NULL), DEFAULT_EDIT_CLASSNAME_W, &clsEditDefault))
	{
		//Get class info failed
		//TODO: add error handle code
		return FALSE;
	}
	s_DefaultWindowProc = clsEditDefault.lpfnWndProc;

	GetDefaultSkin(&s_SkinEdit);

	return TRUE;
}

void CDreamSkinEdit::DestroyClass()
{

}

BOOL CDreamSkinEdit::GetDefaultSkin(SKINEDIT *pSkinEdit)
{
	if (pSkinEdit)
	{
		GetDefaultBackground(&pSkinEdit->skinBkNormal, RGB(255, 255, 255));
		GetDefaultBackground(&pSkinEdit->skinBkDisable, ::GetSysColor(COLOR_BTNFACE));
		GetDefaultBackground(&pSkinEdit->skinBkHover, RGB(255, 255, 255));
		GetDefaultBackground(&pSkinEdit->skinBkPress, RGB(255, 255, 255));

		GetDefaultBackground(&pSkinEdit->skinBkNormalReadOnly, RGB(255, 255, 255));
		GetDefaultBackground(&pSkinEdit->skinBkHoverReadOnly, RGB(255, 255, 255));
		GetDefaultBackground(&pSkinEdit->skinBkPressReadOnly, RGB(255, 255, 255));

		GetDefaultText(&pSkinEdit->skinTxtNormal, ::GetSysColor(COLOR_BTNTEXT));
		GetDefaultText(&pSkinEdit->skinTxtDisable, ::GetSysColor(COLOR_GRAYTEXT));
		GetDefaultText(&pSkinEdit->skinTxtHover, ::GetSysColor(COLOR_BTNTEXT));
		GetDefaultText(&pSkinEdit->skinTxtPress, ::GetSysColor(COLOR_BTNTEXT));

		GetDefaultText(&pSkinEdit->skinTxtNormalReadOnly, ::GetSysColor(COLOR_BTNTEXT));
		GetDefaultText(&pSkinEdit->skinTxtHoverReadOnly, ::GetSysColor(COLOR_BTNTEXT));
		GetDefaultText(&pSkinEdit->skinTxtPressReadOnly, ::GetSysColor(COLOR_BTNTEXT));

		GetDefaultBorder(&pSkinEdit->skinLBorderNormal, ::GetSysColor(COLOR_3DDKSHADOW), 1);
		GetDefaultBorder(&pSkinEdit->skinRBorderNormal, ::GetSysColor(COLOR_3DDKSHADOW), 1);
		GetDefaultBorder(&pSkinEdit->skinTBorderNormal, ::GetSysColor(COLOR_3DDKSHADOW), 1);
		GetDefaultBorder(&pSkinEdit->skinBBorderNormal, ::GetSysColor(COLOR_3DDKSHADOW), 1);

		GetDefaultBorder(&pSkinEdit->skinLBorderDisable, ::GetSysColor(COLOR_GRAYTEXT), 1);
		GetDefaultBorder(&pSkinEdit->skinRBorderDisable, ::GetSysColor(COLOR_GRAYTEXT), 1);
		GetDefaultBorder(&pSkinEdit->skinTBorderDisable, ::GetSysColor(COLOR_GRAYTEXT), 1);
		GetDefaultBorder(&pSkinEdit->skinBBorderDisable, ::GetSysColor(COLOR_GRAYTEXT), 1);

		GetDefaultBorder(&pSkinEdit->skinLBorderHover, ::GetSysColor(COLOR_3DDKSHADOW), 1);
		GetDefaultBorder(&pSkinEdit->skinRBorderHover, ::GetSysColor(COLOR_3DHILIGHT), 1);
		GetDefaultBorder(&pSkinEdit->skinTBorderHover, ::GetSysColor(COLOR_3DDKSHADOW), 1);
		GetDefaultBorder(&pSkinEdit->skinBBorderHover, ::GetSysColor(COLOR_3DHILIGHT), 1);

		GetDefaultBorder(&pSkinEdit->skinLBorderPress, ::GetSysColor(COLOR_3DDKSHADOW), 1);
		GetDefaultBorder(&pSkinEdit->skinRBorderPress, ::GetSysColor(COLOR_3DHILIGHT), 1);
		GetDefaultBorder(&pSkinEdit->skinTBorderPress, ::GetSysColor(COLOR_3DDKSHADOW), 1);
		GetDefaultBorder(&pSkinEdit->skinBBorderPress, ::GetSysColor(COLOR_3DHILIGHT), 1);

		GetDefaultBorder(&pSkinEdit->skinLBorderNormalReadOnly, ::GetSysColor(COLOR_3DDKSHADOW), 1);
		GetDefaultBorder(&pSkinEdit->skinRBorderNormalReadOnly, ::GetSysColor(COLOR_3DDKSHADOW), 1);
		GetDefaultBorder(&pSkinEdit->skinTBorderNormalReadOnly, ::GetSysColor(COLOR_3DDKSHADOW), 1);
		GetDefaultBorder(&pSkinEdit->skinBBorderNormalReadOnly, ::GetSysColor(COLOR_3DDKSHADOW), 1);

		GetDefaultBorder(&pSkinEdit->skinLBorderHoverReadOnly, ::GetSysColor(COLOR_3DDKSHADOW), 1);
		GetDefaultBorder(&pSkinEdit->skinRBorderHoverReadOnly, ::GetSysColor(COLOR_3DHILIGHT), 1);
		GetDefaultBorder(&pSkinEdit->skinTBorderHoverReadOnly, ::GetSysColor(COLOR_3DDKSHADOW), 1);
		GetDefaultBorder(&pSkinEdit->skinBBorderHoverReadOnly, ::GetSysColor(COLOR_3DHILIGHT), 1);

		GetDefaultBorder(&pSkinEdit->skinLBorderPressReadOnly, ::GetSysColor(COLOR_3DDKSHADOW), 1);
		GetDefaultBorder(&pSkinEdit->skinRBorderPressReadOnly, ::GetSysColor(COLOR_3DHILIGHT), 1);
		GetDefaultBorder(&pSkinEdit->skinTBorderPressReadOnly, ::GetSysColor(COLOR_3DDKSHADOW), 1);
		GetDefaultBorder(&pSkinEdit->skinBBorderPressReadOnly, ::GetSysColor(COLOR_3DHILIGHT), 1);
	}

	return TRUE;
}

void CDreamSkinEdit::GetDefaultBackground(SKINBACKGROUND *pBackground, COLORREF clrBk)
{
	pBackground->nDefault = 1;
	pBackground->nDrawType = DRAWTYPE_FILLCOLOR;
	pBackground->clrDraw.clrStart = clrBk;
	memset(&(pBackground->imgDraw), 0, sizeof(DRAWIMAGE));
}

void CDreamSkinEdit::GetDefaultBorder(SKINBORDER *pBorder, COLORREF clrBk, int nWidth)
{
	pBorder->nDefault = 1;
	pBorder->nDrawType = DRAWTYPE_FILLCOLOR;
	pBorder->nWidth = nWidth;
	pBorder->nStart = 0;
	pBorder->nEnd = 0;
	pBorder->clrDraw.clrStart = clrBk;
	memset(&(pBorder->imgDraw), 0, sizeof(DRAWIMAGE));
}

void CDreamSkinEdit::GetDefaultText(SKINTEXT *pText, COLORREF clrTxt)
{
	pText->nDefault = 1;
	pText->clrDraw = clrTxt;
	pText->bDrawShadow = FALSE;
	pText->clrShadow = RGB(255, 255, 255);

	pText->skinFont.nFontSize = 8;
	pText->skinFont.nBold = 0;
	wcscpy_s(pText->skinFont.wstrFontName, LF_FACESIZE, L"Microsoft Sans Serif");
}

HBRUSH  CDreamSkinEdit::CreateBackgroundBrush(SKINBACKGROUND *pSkinBackground)
{
	HBRUSH hBrush = ::CreateSolidBrush(pSkinBackground->clrDraw.clrStart);

	return hBrush;
}

LRESULT CDreamSkinEdit::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT nResult = ERROR_SUCCESS;

	switch(message)
	{
	case WM_CTLCOLOREDIT:
	case WM_CTLCOLORSTATIC:
		nResult = OnCtrlColor((HDC)wParam, (HWND)lParam);
		break;
	case WM_MOUSELEAVE:
		nResult = OnMouseLeave();
		break;
	case WM_MOUSEMOVE:
		nResult = OnMouseMove(wParam, MAKEPOINTS(lParam));
		break;
	case WM_NCCALCSIZE:
		nResult = OnNcCalcSize((BOOL)wParam, (NCCALCSIZE_PARAMS*)lParam);
		break;
	case WM_NCPAINT:
		nResult = OnNcPaint((HRGN)wParam);
		break;
	default:
		nResult = CDreamSkinWindow::DefWindowProc(message, wParam, lParam);
		break;
	}

	return nResult;

}

LRESULT CDreamSkinEdit::OnCtrlColor(HDC hDC, HWND hWndParent)
{
	HBRUSH hBrushBk;
	COLORREF clrText, clrBk;

	DWORD dwStyle = ::GetWindowLong(m_hWnd, GWL_STYLE);

	int nStatus = GetCurrentStatus(dwStyle);
	switch(nStatus)
	{
	case DRAWSTATUS_DISABLE:
		hBrushBk = m_hBkDisable;
		clrBk = m_pSkinEdit->skinBkDisable.clrDraw.clrStart;
		clrText = m_pSkinEdit->skinTxtDisable.clrDraw;
		break;
	case DRAWSTATUS_HOVER:
		if (dwStyle & ES_READONLY)
		{
			hBrushBk = m_hBkHoverReadOnly;
			clrBk = m_pSkinEdit->skinBkHoverReadOnly.clrDraw.clrStart;
			clrText = m_pSkinEdit->skinTxtHoverReadOnly.clrDraw;
		}
		else
		{
			hBrushBk = m_hBkHover;
			clrBk = m_pSkinEdit->skinBkHover.clrDraw.clrStart;
			clrText = m_pSkinEdit->skinTxtHover.clrDraw;
		}
		break;
	case DRAWSTATUS_PRESS:
		if (dwStyle & ES_READONLY)
		{
			hBrushBk = m_hBkPressReadOnly;
			clrBk = m_pSkinEdit->skinBkPressReadOnly.clrDraw.clrStart;
			clrText = m_pSkinEdit->skinTxtPressReadOnly.clrDraw;
		}
		else
		{
			hBrushBk = m_hBkPress;
			clrBk = m_pSkinEdit->skinBkPress.clrDraw.clrStart;
			clrText = m_pSkinEdit->skinTxtPress.clrDraw;
		}
		break;
	default:
		if (dwStyle & ES_READONLY)
		{
			hBrushBk = m_hBkNormalReadOnly;
			clrBk = m_pSkinEdit->skinBkNormalReadOnly.clrDraw.clrStart;
			clrText = m_pSkinEdit->skinTxtNormalReadOnly.clrDraw;
		}
		else
		{
			hBrushBk = m_hBkNormal;
			clrBk = m_pSkinEdit->skinBkNormal.clrDraw.clrStart;
			clrText = m_pSkinEdit->skinTxtNormal.clrDraw;
		}
		break;

	}

	//::SetBkMode(hDC, TRANSPARENT);
	::SetBkColor(hDC, clrBk);
	::SetTextColor(hDC, clrText);

	return (LRESULT)hBrushBk;
}

LRESULT CDreamSkinEdit::OnMouseLeave()
{
	if(m_bMouseIn)
	{
		m_bMouseIn = FALSE;

		UpdateWindow();
	}

	return 0;
}

LRESULT CDreamSkinEdit::OnMouseMove(UINT nFlags, POINTS point)
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

	return CDreamSkinWindow::DefWindowProc(WM_MOUSEMOVE, nFlags, MAKELPARAM(point.x, point.y));
}

LRESULT CDreamSkinEdit::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS *lpncsp)
{
	LRESULT nResult = 0;
	
	if(bCalcValidRects)
		lpncsp->rgrc[0] = GetRectClient(lpncsp->rgrc[0]);
	else
		*((RECT*)lpncsp) = GetRectClient(*((RECT*)lpncsp));

	 return nResult;
}

LRESULT CDreamSkinEdit::OnNcPaint(HRGN hRGN)
{
	HDC hWindowDC;
	hWindowDC = ::GetWindowDC(m_hWnd);
    // Paint into this DC

	RECT rcWindow;
	::GetWindowRect(m_hWnd, &rcWindow);
	::OffsetRect(&rcWindow, 0 - rcWindow.left, 0 - rcWindow.top);

	DWORD dwExStyle = ::GetWindowLong(m_hWnd, GWL_EXSTYLE);
	if (dwExStyle & WS_EX_CLIENTEDGE)
		DrawBorder(hWindowDC, rcWindow);

	::ReleaseDC(m_hWnd, hWindowDC);

	return 0;
}

void CDreamSkinEdit::DrawBorder(HDC hDC, RECT rcWindow)
{
	RECT rcDraw;
	SKINBORDER *pLBorder, *pRBorder, *pTBorder, *pBBorder;
	DWORD dwStyle = ::GetWindowLong(m_hWnd, GWL_STYLE);

	int nStatus = GetCurrentStatus(dwStyle);
	switch(nStatus)
	{
	case DRAWSTATUS_DISABLE:
		pLBorder = &m_pSkinEdit->skinLBorderDisable;
		pRBorder = &m_pSkinEdit->skinRBorderDisable;
		pTBorder = &m_pSkinEdit->skinTBorderDisable;
		pBBorder = &m_pSkinEdit->skinBBorderDisable;
		break;
	case DRAWSTATUS_HOVER:
		if (dwStyle & ES_READONLY)
		{
			pLBorder = &m_pSkinEdit->skinLBorderHoverReadOnly;
			pRBorder = &m_pSkinEdit->skinRBorderHoverReadOnly;
			pTBorder = &m_pSkinEdit->skinTBorderHoverReadOnly;
			pBBorder = &m_pSkinEdit->skinBBorderHoverReadOnly;
		}
		else
		{
			pLBorder = &m_pSkinEdit->skinLBorderHover;
			pRBorder = &m_pSkinEdit->skinRBorderHover;
			pTBorder = &m_pSkinEdit->skinTBorderHover;
			pBBorder = &m_pSkinEdit->skinBBorderHover;
		}
		break;
	case DRAWSTATUS_PRESS:
		if (dwStyle & ES_READONLY)
		{
			pLBorder = &m_pSkinEdit->skinLBorderPressReadOnly;
			pRBorder = &m_pSkinEdit->skinRBorderPressReadOnly;
			pTBorder = &m_pSkinEdit->skinTBorderPressReadOnly;
			pBBorder = &m_pSkinEdit->skinBBorderPressReadOnly;
		}
		else
		{
			pLBorder = &m_pSkinEdit->skinLBorderPress;
			pRBorder = &m_pSkinEdit->skinRBorderPress;
			pTBorder = &m_pSkinEdit->skinTBorderPress;
			pBBorder = &m_pSkinEdit->skinBBorderPress;
		}
		break;
	default:
		if (dwStyle & ES_READONLY)
		{
			pLBorder = &m_pSkinEdit->skinLBorderNormalReadOnly;
			pRBorder = &m_pSkinEdit->skinRBorderNormalReadOnly;
			pTBorder = &m_pSkinEdit->skinTBorderNormalReadOnly;
			pBBorder = &m_pSkinEdit->skinBBorderNormalReadOnly;
		}
		else
		{
			pLBorder = &m_pSkinEdit->skinLBorderNormal;
			pRBorder = &m_pSkinEdit->skinRBorderNormal;
			pTBorder = &m_pSkinEdit->skinTBorderNormal;
			pBBorder = &m_pSkinEdit->skinBBorderNormal;
		}
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

int CDreamSkinEdit::GetCurrentStatus(DWORD dwStyle) const
{
	int nStatus;

	if(dwStyle & WS_DISABLED)
		nStatus = DRAWSTATUS_DISABLE;       //Disabled
	else if (::GetCapture() == m_hWnd)
		nStatus = DRAWSTATUS_PRESS;         //Focus status
	else if(m_bMouseIn)
			nStatus = DRAWSTATUS_HOVER;     //Hover
	else
			nStatus = DRAWSTATUS_NORMAL;    //Normal

	return nStatus;
}

RECT CDreamSkinEdit::GetRectClient(RECT rcWindow)
{
	RECT rcClient;
	SKINBORDER *pLBorder, *pRBorder, *pTBorder, *pBBorder;

	DWORD dwExStyle = ::GetWindowLong(m_hWnd, GWL_EXSTYLE);

	rcClient = rcWindow;

	if (dwExStyle & WS_EX_CLIENTEDGE)
	{
		pLBorder = &m_pSkinEdit->skinLBorderNormal;
		pRBorder = &m_pSkinEdit->skinRBorderNormal;
		pTBorder = &m_pSkinEdit->skinTBorderNormal;
		pBBorder = &m_pSkinEdit->skinBBorderNormal;

		rcClient.left = rcClient.left + pLBorder->nWidth;
		rcClient.right = rcClient.right - pRBorder->nWidth;
		rcClient.top = rcClient.top + pTBorder->nWidth;
		rcClient.bottom = rcClient.bottom - pBBorder->nWidth;
	}

	return rcClient;
}
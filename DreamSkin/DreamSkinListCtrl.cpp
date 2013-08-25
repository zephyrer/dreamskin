//DreamSkinListCtrl.cpp
#include <windows.h>

#include "WinGdiEx.h"
#include "DreamSkinMain.h"
#include "DreamSkinLoader.h"
#include "DreamSkinWindow.h"
#include "DreamSkinScrollBar.h"
#include "DreamSkinListCtrl.h"

//static member declaration
WNDPROC         CDreamSkinListCtrl::s_DefaultWindowProc = NULL;
SKINLISTCTRL    CDreamSkinListCtrl::s_SkinListCtrl;

CDreamSkinListCtrl::CDreamSkinListCtrl(HWND hWnd, WNDPROC OrgWndProc)
	: CDreamSkinWindow(hWnd, OrgWndProc)
{
	m_pSkinListCtrl = &s_SkinListCtrl;

	m_ScrollBarTrackInfo.nSBLButtonDown = 0;
	m_ScrollBarTrackInfo.bIsTracking = FALSE;
	m_ScrollBarTrackInfo.nTrackPos = 0;
	m_nSBHover = 0;
}

CDreamSkinListCtrl::~CDreamSkinListCtrl()
{
	if (m_pSkinListCtrl != &s_SkinListCtrl)
		delete m_pSkinListCtrl;
}

void CDreamSkinListCtrl::Reload()
{
	if (m_hWnd && ::IsWindow(m_hWnd))
	{
		RECT rcClient;
		::GetClientRect(m_hWnd, &rcClient);

		m_bCreateRegion = TRUE;
		::InvalidateRect(m_hWnd, &rcClient, TRUE);
	}
}

BOOL CDreamSkinListCtrl::ApplySkin(CDreamSkinLoader *pLoader)
{
	BOOL bResult = TRUE;

	InitialClass();

	SKINLISTCTRL SkinListCtrl;
	pLoader->GetSkinListCtrl(&SkinListCtrl);
	s_SkinListCtrl = SkinListCtrl;

	return bResult;
}

CDreamSkinListCtrl* CDreamSkinListCtrl::CreateInstance(HWND hWnd, WNDPROC OrgWndProc)
{
	CDreamSkinListCtrl *pSkinListCtrl = new CDreamSkinListCtrl(hWnd, OrgWndProc);
	return pSkinListCtrl;
}

LRESULT WINAPI CDreamSkinListCtrl::DefWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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

BOOL CDreamSkinListCtrl::InitialClass()
{
	//Get the default window proc for this class
	WNDCLASSW clsListCtrlDefault;
	if(!GetClassInfoW(GetModuleHandle(NULL), DEFAULT_LISTCTRL_CLASSNAME_W, &clsListCtrlDefault))
	{
		//Get class info failed
		//TODO: add error handle code
		return FALSE;
	}
	s_DefaultWindowProc = clsListCtrlDefault.lpfnWndProc;

	GetDefaultSkin(&s_SkinListCtrl);

	return TRUE;
}

void CDreamSkinListCtrl::DestroyClass()
{

}

BOOL CDreamSkinListCtrl::GetDefaultSkin(SKINLISTCTRL *pSkinListCtrl)
{
	if (pSkinListCtrl)
	{
		GetDefaultBackground(&pSkinListCtrl->skinBkNormal, RGB(255, 255, 255));
		GetDefaultBackground(&pSkinListCtrl->skinBkDisable, RGB(236, 233, 216));           //COLOR_BTNFACE

		GetDefaultBorder(&pSkinListCtrl->skinLBorderNormal, RGB(113, 111, 100), 1);        //COLOR_3DDKSHADOW
		GetDefaultBorder(&pSkinListCtrl->skinRBorderNormal, RGB(255, 255, 255), 1);        //COLOR_3DHILIGHT
		GetDefaultBorder(&pSkinListCtrl->skinTBorderNormal, RGB(113, 111, 100), 1);
		GetDefaultBorder(&pSkinListCtrl->skinBBorderNormal, RGB(255, 255, 255), 1);

		GetDefaultBorder(&pSkinListCtrl->skinLBorderDisable, RGB(172, 168, 153), 1);       //COLOR_GRAYTEXT
		GetDefaultBorder(&pSkinListCtrl->skinRBorderDisable, RGB(172, 168, 153), 1);
		GetDefaultBorder(&pSkinListCtrl->skinTBorderDisable, RGB(172, 168, 153), 1);
		GetDefaultBorder(&pSkinListCtrl->skinBBorderDisable, RGB(172, 168, 153), 1);
	}

	return TRUE;
}

LRESULT CDreamSkinListCtrl::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT nResult = ERROR_SUCCESS;

	int nTemp;
	SCROLLINFO ScrollInfo;
	switch(message)
	{
	/*
	
	case WM_MEASUREITEM:
		nResult = CDreamSkinWindow::DefWindowProc(message, wParam, lParam);
		break;
	
	*/
	case WM_ERASEBKGND:
		nResult = OnEraseBkgnd((HDC)wParam);
		break;
	case WM_LBUTTONUP:
		nResult = CDreamSkinWindow::DefWindowProc(message, wParam, lParam);
		m_ScrollBarTrackInfo.nSBLButtonDown = 0;
		m_ScrollBarTrackInfo.bIsTracking = FALSE;
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
	case WM_NCLBUTTONDBLCLK:
		nResult = OnNcLButtonDbClick(wParam, MAKEPOINTS(lParam));
		break;
	case WM_NCLBUTTONDOWN:
		nResult = OnNcLButtonDown(wParam,  MAKEPOINTS(lParam));
		break;
	case WM_NCMOUSELEAVE:
		nResult = OnNcMouseLeave();
		break;
	case WM_NCMOUSEMOVE:
		nResult = OnNcMouseMove((UINT)wParam, MAKEPOINTS(lParam));
		break;
	case WM_NCPAINT:
		nResult = OnNcPaint((HRGN)wParam);
		break;
	case SBM_GETSCROLLINFO:
		nResult = OnGetScrollInfo(wParam, (LPSCROLLINFO)lParam);
		break;
	case WM_VSCROLL:
		if (LOWORD(wParam) == 5)
		{
			nTemp = HIWORD(wParam);
				ScrollInfo.cbSize = sizeof(SCROLLINFO);
				ScrollInfo.fMask = SIF_ALL;
				::GetScrollInfo(m_hWnd, SB_VERT, &ScrollInfo);
				nResult = CDreamSkinWindow::DefWindowProc(message, wParam, lParam);
				::GetScrollInfo(m_hWnd, SB_VERT, &ScrollInfo);
		}
		else
		{
			nResult = CDreamSkinWindow::DefWindowProc(message, wParam, lParam);
		}
		break;
	default:
		nResult = CDreamSkinWindow::DefWindowProc(message, wParam, lParam);
		break;
	}

	return nResult;

}

LRESULT CDreamSkinListCtrl::OnEraseBkgnd(HDC hDC)
{
	RECT rcClient;
	::GetClientRect(m_hWnd, &rcClient);

	DrawBackground(hDC, rcClient);

	return 0;
}

LRESULT CDreamSkinListCtrl::OnMouseLeave()
{
	/*if(m_nCurHover >= 0)
	{
		int nIndex = m_nCurHover;
		m_nCurHover = -1;

		RECT rcItem;
		::SendMessage(m_hWnd, LB_GETITEMRECT, nIndex, (LPARAM)&rcItem);
		::InvalidateRect(m_hWnd, &rcItem, FALSE);
	}*/

	if (m_nSBHover > 0)
	{
		POINT point;
		::GetCursorPos(&point);

		int nSBHover = 0;
		DWORD dwStyle = ::GetWindowLong(m_hWnd, GWL_STYLE);
		if (dwStyle & WS_VSCROLL)
		{
			nSBHover = CDreamSkinScrollBar::ScrollBarHitTest(m_hWnd, SB_VERT, point);
			if (nSBHover > 0)
				nSBHover = SB_VERT << 16 | nSBHover;
		}
		
		if (dwStyle & WS_HSCROLL && nSBHover == 0)
		{
			nSBHover = CDreamSkinScrollBar::ScrollBarHitTest(m_hWnd, SB_HORZ, point);
			if (nSBHover > 0)
				nSBHover = SB_HORZ << 16 | nSBHover;
		}

		if (nSBHover != m_nSBHover)
		{
			m_nSBHover = nSBHover;
			OnNcPaint(NULL);
		}
	}

	return 0;
}

LRESULT CDreamSkinListCtrl::OnMouseMove(UINT nFlags, POINTS point)
{
	TRACKMOUSEEVENT EventTrack;
	EventTrack.cbSize = sizeof(TRACKMOUSEEVENT);
	EventTrack.dwFlags = TME_LEAVE;
	EventTrack.hwndTrack = m_hWnd;
	EventTrack.dwHoverTime = 0;

	TrackMouseEvent(&EventTrack);

	int nSBHover = 0;
	DWORD dwStyle = ::GetWindowLong(m_hWnd, GWL_STYLE);
	if (dwStyle & WS_VSCROLL)
	{
		nSBHover = CDreamSkinScrollBar::ScrollBarHitTest(m_hWnd, SB_VERT, point);
		if (nSBHover > 0)
			nSBHover = SB_VERT << 16 | nSBHover;
	}
	
	if (dwStyle & WS_HSCROLL && nSBHover == 0)
	{
		nSBHover = CDreamSkinScrollBar::ScrollBarHitTest(m_hWnd, SB_HORZ, point);
		if (nSBHover > 0)
			nSBHover = SB_HORZ << 16 | nSBHover;
	}

	if (nSBHover != m_nSBHover)
	{
		m_nSBHover = nSBHover;
		OnNcPaint(NULL);
	}

	/*DWORD dwResult = (DWORD)::SendMessage(m_hWnd, LB_ITEMFROMPOINT, 0, MAKELPARAM(point.x, point.y));
	int nIndex = -1;
	if (!HIWORD(dwResult))
		nIndex = LOWORD(dwResult);

	if(nIndex != m_nCurHover)
	{
		int nTemp = m_nCurHover;
		m_nCurHover = nIndex;
		
		RECT rcItem;
		if (nIndex >= 0)
		{
			::SendMessage(m_hWnd, LB_GETITEMRECT, nIndex, (LPARAM)&rcItem);
			::InvalidateRect(m_hWnd, &rcItem, FALSE);
		}

		if (nTemp >= 0)
		{
			::SendMessage(m_hWnd, LB_GETITEMRECT, nTemp, (LPARAM)&rcItem);
			::InvalidateRect(m_hWnd, &rcItem, FALSE);
		}
	}*/

	return CDreamSkinWindow::DefWindowProc(WM_MOUSEMOVE, nFlags, MAKELPARAM(point.x, point.y));
}

LRESULT CDreamSkinListCtrl::OnNcHitTest(POINTS point)
{
	RECT rcWindow;
	::GetWindowRect(m_hWnd, &rcWindow);
	
	POINT ptTemp;

	ptTemp.x = point.x;
	ptTemp.y = point.y;

	DWORD dwStyle = ::GetWindowLong(m_hWnd, GWL_STYLE);

	if (::PtInRect(&rcWindow, ptTemp))
	{
		RECT rcClient = rcWindow;

		SKINBORDER *pLBorder, *pRBorder, *pTBorder, *pBBorder;
		DWORD dwExStyle = ::GetWindowLong(m_hWnd, GWL_EXSTYLE);
		if (dwExStyle & WS_EX_CLIENTEDGE)
		{
			int nStatus = GetCurrentStatus(dwStyle);
			switch(nStatus)
			{
			case DRAWSTATUS_DISABLE:
				pLBorder = &m_pSkinListCtrl->skinLBorderDisable;
				pRBorder = &m_pSkinListCtrl->skinRBorderDisable;
				pTBorder = &m_pSkinListCtrl->skinTBorderDisable;
				pBBorder = &m_pSkinListCtrl->skinBBorderDisable;
				break;
			default:
				pLBorder = &m_pSkinListCtrl->skinLBorderNormal;
				pRBorder = &m_pSkinListCtrl->skinRBorderNormal;
				pTBorder = &m_pSkinListCtrl->skinTBorderNormal;
				pBBorder = &m_pSkinListCtrl->skinBBorderNormal;
				break;
			}

			rcClient.left += pLBorder->nWidth;
			rcClient.top += pTBorder->nWidth;
			rcClient.right -= pRBorder->nWidth;
			rcClient.bottom -= pBBorder->nWidth;
		}

		SCROLLBARINFO ScrollBarInfo;
		if (dwStyle & WS_VSCROLL)
		{
			ScrollBarInfo.cbSize = sizeof(SCROLLBARINFO);
			::GetScrollBarInfo(m_hWnd, OBJID_VSCROLL, &ScrollBarInfo);
			rcClient.right = ScrollBarInfo.rcScrollBar.left;
			if (::PtInRect(&ScrollBarInfo.rcScrollBar, ptTemp))
				return HTVSCROLL;
		}

		if (dwStyle & WS_HSCROLL)
		{
			ScrollBarInfo.cbSize = sizeof(SCROLLBARINFO);
			::GetScrollBarInfo(m_hWnd, OBJID_HSCROLL, &ScrollBarInfo);
			rcClient.bottom = ScrollBarInfo.rcScrollBar.top;
			if (::PtInRect(&ScrollBarInfo.rcScrollBar, ptTemp))
				return HTHSCROLL;
		}

		if (::PtInRect(&rcClient, ptTemp))
			return HTCLIENT;
		else
			return HTBORDER;
	}
	else
	{
		return HTNOWHERE;
	}
}

LRESULT CDreamSkinListCtrl::OnNcLButtonDbClick(UINT nHitTest, POINTS point)
{
	LRESULT nResult = 0;

	if (nHitTest == HTVSCROLL || nHitTest == HTHSCROLL)
	{
		int fnBar = nHitTest == HTVSCROLL ? SB_VERT : SB_HORZ;
		switch(CDreamSkinScrollBar::ScrollBarHitTest(m_hWnd, fnBar, point))
		{
		case SBHT_THUMB:
			m_ScrollBarTrackInfo.nSBLButtonDown = fnBar << 16 | SBHT_THUMB;
			CDreamSkinScrollBar::TrackScrollBar(m_hWnd, fnBar, SBHT_THUMB, &m_ScrollBarTrackInfo);
			break;
		case SBHT_REGION_UP:
			m_ScrollBarTrackInfo.nSBLButtonDown = fnBar << 16 | SBHT_REGION_UP;
			CDreamSkinScrollBar::TrackScrollBar(m_hWnd, fnBar, SBHT_REGION_UP, &m_ScrollBarTrackInfo);
			break;
		case SBHT_REGION_DOWN:
			m_ScrollBarTrackInfo.nSBLButtonDown = fnBar << 16 | SBHT_REGION_DOWN;
			CDreamSkinScrollBar::TrackScrollBar(m_hWnd, fnBar, SBHT_REGION_DOWN, &m_ScrollBarTrackInfo);
			break;
		case SBHT_ARROW_TOP:
			m_ScrollBarTrackInfo.nSBLButtonDown = fnBar << 16 | SBHT_ARROW_TOP;
			CDreamSkinScrollBar::TrackScrollBar(m_hWnd, fnBar, SBHT_ARROW_TOP, &m_ScrollBarTrackInfo);
			break;
		case SBHT_ARROW_BOTTOM:
			m_ScrollBarTrackInfo.nSBLButtonDown = fnBar << 16 | SBHT_ARROW_BOTTOM;
			CDreamSkinScrollBar::TrackScrollBar(m_hWnd, fnBar, SBHT_ARROW_BOTTOM, &m_ScrollBarTrackInfo);
			break;
		default:
			m_ScrollBarTrackInfo.nSBLButtonDown = 0;
			nResult = CDreamSkinWindow::DefWindowProc(WM_NCLBUTTONDBLCLK, nHitTest, MAKELPARAM(point.x, point.y));
			OnNcPaint(NULL);
			break;
		}
	}
	else
	{
		m_ScrollBarTrackInfo.nSBLButtonDown = 0;
		nResult = CDreamSkinWindow::DefWindowProc(WM_NCLBUTTONDBLCLK, nHitTest, MAKELPARAM(point.x, point.y));
	}

	return nResult;
}

LRESULT CDreamSkinListCtrl::OnNcLButtonDown(UINT nHitTest, POINTS point)
{
	LRESULT nResult = 0;

	if (nHitTest == HTVSCROLL || nHitTest == HTHSCROLL)
	{
		int fnBar = nHitTest == HTVSCROLL ? SB_VERT : SB_HORZ;
		switch(CDreamSkinScrollBar::ScrollBarHitTest(m_hWnd, fnBar, point))
		{
		case SBHT_THUMB:
			m_ScrollBarTrackInfo.nSBLButtonDown = fnBar << 16 | SBHT_THUMB;
			CDreamSkinScrollBar::TrackScrollBar(m_hWnd, fnBar, SBHT_THUMB, &m_ScrollBarTrackInfo);
			break;
		case SBHT_REGION_UP:
			m_ScrollBarTrackInfo.nSBLButtonDown = fnBar << 16 | SBHT_REGION_UP;
			CDreamSkinScrollBar::TrackScrollBar(m_hWnd, fnBar, SBHT_REGION_UP, &m_ScrollBarTrackInfo);
			break;
		case SBHT_REGION_DOWN:
			m_ScrollBarTrackInfo.nSBLButtonDown = fnBar << 16 | SBHT_REGION_DOWN;
			CDreamSkinScrollBar::TrackScrollBar(m_hWnd, fnBar, SBHT_REGION_DOWN, &m_ScrollBarTrackInfo);
			break;
		case SBHT_ARROW_TOP:
			m_ScrollBarTrackInfo.nSBLButtonDown = fnBar << 16 | SBHT_ARROW_TOP;
			CDreamSkinScrollBar::TrackScrollBar(m_hWnd, fnBar, SBHT_ARROW_TOP, &m_ScrollBarTrackInfo);
			break;
		case SBHT_ARROW_BOTTOM:
			m_ScrollBarTrackInfo.nSBLButtonDown = fnBar << 16 | SBHT_ARROW_BOTTOM;
			CDreamSkinScrollBar::TrackScrollBar(m_hWnd, fnBar, SBHT_ARROW_BOTTOM, &m_ScrollBarTrackInfo);
			break;
		default:
			m_ScrollBarTrackInfo.nSBLButtonDown = 0;
			nResult = CDreamSkinWindow::DefWindowProc(WM_NCLBUTTONDOWN, nHitTest, MAKELPARAM(point.x, point.y));
			OnNcPaint(NULL);
			break;
		}
	}
	else
	{
		m_ScrollBarTrackInfo.nSBLButtonDown = 0;
		nResult = CDreamSkinWindow::DefWindowProc(WM_NCLBUTTONDOWN, nHitTest, MAKELPARAM(point.x, point.y));
	}

	return nResult;
}

LRESULT CDreamSkinListCtrl::OnNcMouseLeave()
{
	if (m_nSBHover != 0)
	{
		m_nSBHover = 0;
		OnNcPaint(NULL);
	}

	return 0;
}

LRESULT CDreamSkinListCtrl::OnNcMouseMove(UINT nHitTest, POINTS point)
{
	TRACKMOUSEEVENT EventTrack;
	EventTrack.cbSize = sizeof(TRACKMOUSEEVENT);
	EventTrack.dwFlags = TME_NONCLIENT | TME_LEAVE;
	EventTrack.hwndTrack = m_hWnd;
	EventTrack.dwHoverTime = 1;

	TrackMouseEvent(&EventTrack);

	int nSBHover = 0;
	DWORD dwStyle = ::GetWindowLong(m_hWnd, GWL_STYLE);
	if (dwStyle & WS_VSCROLL)
	{
		nSBHover = CDreamSkinScrollBar::ScrollBarHitTest(m_hWnd, SB_VERT, point);
		if (nSBHover > 0)
			nSBHover = SB_VERT << 16 | nSBHover;
	}
	
	if (dwStyle & WS_HSCROLL && nSBHover == 0)
	{
		nSBHover = CDreamSkinScrollBar::ScrollBarHitTest(m_hWnd, SB_HORZ, point);
		if (nSBHover > 0)
			nSBHover = SB_HORZ << 16 | nSBHover;
	}

	if (nSBHover != m_nSBHover)
	{
		m_nSBHover = nSBHover;
		OnNcPaint(NULL);
	}

	return 0; 
}

LRESULT CDreamSkinListCtrl::OnNcPaint(HRGN hRGN)
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

	DWORD dwStyle = ::GetWindowLong(m_hWnd, GWL_STYLE);
	if (dwStyle & WS_VSCROLL || dwStyle & WS_HSCROLL)
		CDreamSkinScrollBar::DrawWindowScrollBar(m_hWnd, hWindowDC, m_ScrollBarTrackInfo.nSBLButtonDown, m_nSBHover);

	::ReleaseDC(m_hWnd, hWindowDC);

	return 0;
}

LRESULT CDreamSkinListCtrl::OnGetScrollInfo(int fnBar, LPSCROLLINFO lpsi)
{
	BOOL bResult = FALSE;

	if (lpsi->fMask == SIF_TRACKPOS)
	{
		if (m_ScrollBarTrackInfo.bIsTracking && (m_ScrollBarTrackInfo.nSBLButtonDown >> 16) == fnBar)
		{
			lpsi->nTrackPos = m_ScrollBarTrackInfo.nTrackPos;
			bResult = TRUE;
		}
	}

	return bResult;
}

void CDreamSkinListCtrl::DrawBackground(HDC hDC, RECT rcClient)
{
	DWORD dwStyle = ::GetWindowLong(m_hWnd, GWL_STYLE);
	HBRUSH hBrush;

	int nStatus = GetCurrentStatus(dwStyle);
	switch(nStatus)
	{
	case DRAWSTATUS_DISABLE:
		hBrush = ::CreateSolidBrush(m_pSkinListCtrl->skinBkDisable.clrDraw.clrStart);
		break;
	default:
		hBrush = ::CreateSolidBrush(m_pSkinListCtrl->skinBkNormal.clrDraw.clrStart);
		break;
	}
	
	::FillRect(hDC, &rcClient, hBrush);
	::DeleteObject(hBrush);
}

void CDreamSkinListCtrl::DrawBorder(HDC hDC, RECT rcWindow)
{
	RECT rcDraw;
	SKINBORDER *pLBorder, *pRBorder, *pTBorder, *pBBorder;
	DWORD dwStyle = ::GetWindowLong(m_hWnd, GWL_STYLE);

	int nStatus = GetCurrentStatus(dwStyle);
	switch(nStatus)
	{
	case DRAWSTATUS_DISABLE:
		pLBorder = &m_pSkinListCtrl->skinLBorderDisable;
		pRBorder = &m_pSkinListCtrl->skinRBorderDisable;
		pTBorder = &m_pSkinListCtrl->skinTBorderDisable;
		pBBorder = &m_pSkinListCtrl->skinBBorderDisable;
		break;
	default:
		pLBorder = &m_pSkinListCtrl->skinLBorderNormal;
		pRBorder = &m_pSkinListCtrl->skinRBorderNormal;
		pTBorder = &m_pSkinListCtrl->skinTBorderNormal;
		pBBorder = &m_pSkinListCtrl->skinBBorderNormal;
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

int CDreamSkinListCtrl::GetCurrentStatus(DWORD dwStyle) const
{
	int nStatus;

	if(dwStyle & WS_DISABLED)
		nStatus = DRAWSTATUS_DISABLE;       //Disabled
	else
		nStatus = DRAWSTATUS_NORMAL;        //Normal

	return nStatus;
}
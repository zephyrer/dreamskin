//DreamSkinSpinCtrl.cpp
#include <windows.h>
#include <commctrl.h>

#include "WinGdiEx.h"
#include "DreamSkinMain.h"
#include "DreamSkinLoader.h"
#include "DreamSkinWindow.h"
#include "DreamSkinSpinCtrl.h"

//static member declaration
WNDPROC         CDreamSkinSpinCtrl::s_DefaultWindowProc = NULL;
SKINSPINCTRL    CDreamSkinSpinCtrl::s_SkinSpinCtrl;

CDreamSkinSpinCtrl::CDreamSkinSpinCtrl(HWND hWnd, WNDPROC OrgWndProc)
	: CDreamSkinWindow(hWnd, OrgWndProc)
{
	m_pSkinSpinCtrl = &s_SkinSpinCtrl;
	m_nHover = SPHT_NOWHERE;
	m_nLButtonDown = SPHT_NOWHERE;
	m_bIsTracking = FALSE;
}

CDreamSkinSpinCtrl::~CDreamSkinSpinCtrl()
{
	if (m_pSkinSpinCtrl != &s_SkinSpinCtrl)
		delete m_pSkinSpinCtrl;
}

void CDreamSkinSpinCtrl::Reload()
{
	if (m_hWnd && ::IsWindow(m_hWnd))
	{
		RECT rcClient;
		::GetClientRect(m_hWnd, &rcClient);

		m_bCreateRegion = TRUE;
		::InvalidateRect(m_hWnd, &rcClient, TRUE);
	}
}

BOOL CDreamSkinSpinCtrl::ApplySkin(CDreamSkinLoader *pLoader)
{
	BOOL bResult = TRUE;

	InitialClass();

	SKINSPINCTRL SkinSpinCtrl;
	pLoader->GetSkinSpinCtrl(&SkinSpinCtrl);
	s_SkinSpinCtrl = SkinSpinCtrl;

	return bResult;
}

CDreamSkinSpinCtrl* CDreamSkinSpinCtrl::CreateInstance(HWND hWnd, WNDPROC OrgWndProc)
{
	CDreamSkinSpinCtrl *pSkinSpinCtrl = new CDreamSkinSpinCtrl(hWnd, OrgWndProc);
	return pSkinSpinCtrl;
}

LRESULT WINAPI CDreamSkinSpinCtrl::DefWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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

BOOL CDreamSkinSpinCtrl::InitialClass()
{
	//Get the default window proc for this class
	WNDCLASSW clsSpinCtrlDefault;
	if(!GetClassInfoW(GetModuleHandle(NULL), DEFAULT_SPINCTRL_CLASSNAME_W, &clsSpinCtrlDefault))
	{
		//Get class info failed
		//TODO: add error handle code
		return FALSE;
	}
	s_DefaultWindowProc = clsSpinCtrlDefault.lpfnWndProc;

	GetDefaultSkin(&s_SkinSpinCtrl);

	return TRUE;
}

void CDreamSkinSpinCtrl::DestroyClass()
{

}

BOOL CDreamSkinSpinCtrl::GetDefaultSkin(SKINSPINCTRL *pSkinSpinCtrl)
{
	if (pSkinSpinCtrl)
	{
		GetDefaultSysButton(&pSkinSpinCtrl->skinBtnLeftNormal, RGB(236, 233, 216), RGB(0, 0, 0) , RGB(172, 168, 153));            //COLOR_BTNFACE
		GetDefaultSysButton(&pSkinSpinCtrl->skinBtnRightNormal, RGB(236, 233, 216), RGB(0, 0, 0), RGB(172, 168, 153));           //COLOR_BTNFACE
		GetDefaultSysButton(&pSkinSpinCtrl->skinBtnTopNormal, RGB(236, 233, 216), RGB(0, 0, 0), RGB(172, 168, 153));             //COLOR_BTNFACE
		GetDefaultSysButton(&pSkinSpinCtrl->skinBtnBottomNormal, RGB(236, 233, 216), RGB(0, 0, 0), RGB(172, 168, 153));          //COLOR_BTNFACE

		GetDefaultSysButton(&pSkinSpinCtrl->skinBtnLeftHover, RGB(241, 239, 226), RGB(255, 255, 255), RGB(172, 168, 153));             //COLOR_3DLIGHT
		GetDefaultSysButton(&pSkinSpinCtrl->skinBtnRightHover, RGB(241, 239, 226), RGB(255, 255, 255), RGB(172, 168, 153));
		GetDefaultSysButton(&pSkinSpinCtrl->skinBtnTopHover, RGB(241, 239, 226), RGB(255, 255, 255), RGB(172, 168, 153));
		GetDefaultSysButton(&pSkinSpinCtrl->skinBtnBottomHover, RGB(241, 239, 226), RGB(255, 255, 255), RGB(172, 168, 153));

		GetDefaultSysButton(&pSkinSpinCtrl->skinBtnLeftPress, RGB(241, 239, 226), RGB(0, 0, 0), RGB(172, 168, 153));
		GetDefaultSysButton(&pSkinSpinCtrl->skinBtnRightPress, RGB(241, 239, 226), RGB(0, 0, 0), RGB(172, 168, 153));
		GetDefaultSysButton(&pSkinSpinCtrl->skinBtnTopPress, RGB(241, 239, 226), RGB(0, 0, 0), RGB(172, 168, 153));
		GetDefaultSysButton(&pSkinSpinCtrl->skinBtnBottomPress, RGB(241, 239, 226), RGB(0, 0, 0), RGB(172, 168, 153));

		GetDefaultSysButton(&pSkinSpinCtrl->skinBtnLeftDisable, RGB(236, 233, 216), RGB(172, 168, 153), RGB(172, 168, 153));
		GetDefaultSysButton(&pSkinSpinCtrl->skinBtnRightDisable, RGB(236, 233, 216), RGB(172, 168, 153), RGB(172, 168, 153));
		GetDefaultSysButton(&pSkinSpinCtrl->skinBtnTopDisable, RGB(236, 233, 216), RGB(172, 168, 153), RGB(172, 168, 153));
		GetDefaultSysButton(&pSkinSpinCtrl->skinBtnBottomDisable, RGB(236, 233, 216), RGB(172, 168, 153), RGB(172, 168, 153));
	}

	return TRUE;
}

LRESULT CDreamSkinSpinCtrl::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT nResult = ERROR_SUCCESS;

	switch(message)
	{
	case WM_ERASEBKGND:
		break;
	case WM_LBUTTONDBLCLK:
		nResult = ::SendMessageW(m_hWnd, WM_LBUTTONDOWN, wParam, lParam);
		break;
	case WM_LBUTTONDOWN:
		nResult = OnLButtonDown(wParam, MAKEPOINTS(lParam));
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
	case WM_PAINT:
		nResult = OnPaint();
		break;
	default:
		nResult = CDreamSkinWindow::DefWindowProc(message, wParam, lParam);
	}

	return nResult;
}

LRESULT CDreamSkinSpinCtrl::OnLButtonDown(UINT nFlags, POINTS point)
{
	LRESULT nResult = 0;

	POINT pt;
	pt.x = point.x;
	pt.y = point.y;

	DWORD dwStyle = ::GetWindowLong(m_hWnd, GWL_STYLE);

	RECT rcClient;
	::GetClientRect(m_hWnd, &rcClient);

	m_nLButtonDown = SpinCtrlHitTest(rcClient, pt, dwStyle);
	::InvalidateRect(m_hWnd, &rcClient, FALSE);

	return CDreamSkinWindow::DefWindowProc(WM_LBUTTONDOWN, nFlags, MAKELPARAM(point.x, point.y));
}

LRESULT CDreamSkinSpinCtrl::OnLButtonUp(UINT nFlags, POINTS point)
{
	if (m_nLButtonDown != 0)
	{
		m_nLButtonDown = 0;
		::InvalidateRect(m_hWnd, NULL, FALSE);
	}

	return CDreamSkinWindow::DefWindowProc(WM_LBUTTONUP, nFlags, MAKELPARAM(point.x, point.y));
}


LRESULT CDreamSkinSpinCtrl::OnMouseLeave()
{
	POINT point;
	::GetCursorPos(&point);
	::ScreenToClient(m_hWnd, &point);

	DWORD dwStyle = ::GetWindowLong(m_hWnd, GWL_STYLE);

	RECT rcClient;
	::GetClientRect(m_hWnd, &rcClient);

	int nHover = SpinCtrlHitTest(rcClient, point, dwStyle);
	if (m_nHover != nHover)
	{
		m_nHover = nHover;
		::InvalidateRect(m_hWnd, &rcClient, FALSE);
	}

	return 0;
}

LRESULT CDreamSkinSpinCtrl::OnMouseMove(UINT nFlags, POINTS point)
{
	TRACKMOUSEEVENT EventTrack;
	EventTrack.cbSize = sizeof(TRACKMOUSEEVENT);
	EventTrack.dwFlags = TME_LEAVE;
	EventTrack.hwndTrack = m_hWnd;
	EventTrack.dwHoverTime = 0;

	TrackMouseEvent(&EventTrack);

	POINT pt;
	pt.x = point.x;
	pt.y = point.y;

	DWORD dwStyle = ::GetWindowLong(m_hWnd, GWL_STYLE);

	RECT rcClient;
	::GetClientRect(m_hWnd, &rcClient);

	int nHover = SpinCtrlHitTest(rcClient, pt, dwStyle);
	if (m_nHover != nHover)
	{
		m_nHover = nHover;
		::InvalidateRect(m_hWnd, &rcClient, FALSE);
	}

	return 0;
}

LRESULT CDreamSkinSpinCtrl::OnPaint()
{
	PAINTSTRUCT ps;
	HDC hPaintDC = ::BeginPaint(m_hWnd, &ps);

	::SetBkMode(hPaintDC, TRANSPARENT);

	RECT rcClient;
	::GetClientRect(m_hWnd, &rcClient);

	DWORD dwStyle = ::GetWindowLong(m_hWnd, GWL_STYLE);
	RECT rcDraw;
	int nStatus;
	if (dwStyle & UDS_HORZ)
	{
		rcDraw = rcClient;
		rcDraw.right = rcDraw.left + (rcClient.right - rcClient.left) / 2;
		if (dwStyle & WS_DISABLED)
			nStatus = DRAWSTATUS_DISABLE;
		else if (m_nHover == SPHT_ARROW_LEFT)
		{
			if (m_nLButtonDown == SPHT_ARROW_LEFT)
				nStatus = DRAWSTATUS_PRESS;
			else 
				nStatus = DRAWSTATUS_HOVER;
		}
		else
			nStatus = DRAWSTATUS_NORMAL;

		DrawLeftButton(hPaintDC, rcDraw, nStatus);

		::OffsetRect(&rcDraw, (rcClient.right - rcClient.left) / 2, 0);
		if (dwStyle & WS_DISABLED)
			nStatus = DRAWSTATUS_DISABLE;
		else if (m_nHover == SPHT_ARROW_RIGHT)
		{
			if (m_nLButtonDown == SPHT_ARROW_RIGHT)
				nStatus = DRAWSTATUS_PRESS;
			else 
				nStatus = DRAWSTATUS_HOVER;
		}
		else
			nStatus = DRAWSTATUS_NORMAL;

		DrawRightButton(hPaintDC, rcDraw, nStatus);
	}
	else
	{
		rcDraw = rcClient;
		rcDraw.bottom = rcDraw.top + (rcClient.bottom - rcClient.top) / 2;
		if (dwStyle & WS_DISABLED)
			nStatus = DRAWSTATUS_DISABLE;
		else if (m_nHover == SPHT_ARROW_LEFT)
		{
			if (m_nLButtonDown == SPHT_ARROW_LEFT)
				nStatus = DRAWSTATUS_PRESS;
			else 
				nStatus = DRAWSTATUS_HOVER;
		}
		else
			nStatus = DRAWSTATUS_NORMAL;

		DrawTopButton(hPaintDC, rcDraw, nStatus);

		::OffsetRect(&rcDraw, 0, (rcClient.bottom - rcClient.top) / 2);
		if (dwStyle & WS_DISABLED)
			nStatus = DRAWSTATUS_DISABLE;
		else if (m_nHover == SPHT_ARROW_RIGHT)
		{
			if (m_nLButtonDown == SPHT_ARROW_RIGHT)
				nStatus = DRAWSTATUS_PRESS;
			else 
				nStatus = DRAWSTATUS_HOVER;
		}
		else
			nStatus = DRAWSTATUS_NORMAL;

		DrawBottomButton(hPaintDC, rcDraw, nStatus);
	}

	::EndPaint(m_hWnd, &ps);

	return 0;
}

void CDreamSkinSpinCtrl::DrawLeftButton(HDC hDC, RECT rcDraw, int nDrawStatus)
{
	SKINSYSBUTTON *pSysButton;
	HPEN hDrawPen, hOldPen;

	switch(nDrawStatus)
	{
	case DRAWSTATUS_PRESS:
		pSysButton = &m_pSkinSpinCtrl->skinBtnLeftPress;
		break;
	case DRAWSTATUS_HOVER:
		pSysButton = &m_pSkinSpinCtrl->skinBtnLeftHover;
		break;
	case DRAWSTATUS_DISABLE:
		pSysButton = &m_pSkinSpinCtrl->skinBtnLeftDisable;
		break;
	default:
		pSysButton = &m_pSkinSpinCtrl->skinBtnLeftNormal;
		break;
	}

	if (pSysButton->nDrawType == DRAWTYPE_STRETCHBITMAP && pSysButton->imgDraw.hImage)
	{
		::DrawStretchBitmap(hDC, rcDraw.left, rcDraw.top, rcDraw.right - rcDraw.left, rcDraw.bottom - rcDraw.top, pSysButton->imgDraw.hImage, pSysButton->imgDraw.x, pSysButton->imgDraw.y, pSysButton->imgDraw.nWidth, pSysButton->imgDraw.nHeight);
	}
	else
	{
		::FillSolidRect(hDC, &rcDraw, pSysButton->clrBk.clrStart);

		POINT pt[3];
		int nWidth = rcDraw.right - rcDraw.left;
		int nHeight = rcDraw.bottom - rcDraw.top;
		hDrawPen = ::CreatePen(PS_SOLID, 1, pSysButton->clrTxt);
		hOldPen = (HPEN)::SelectObject(hDC, hDrawPen);
		int nTemp;

		pt[0].x = rcDraw.left + nWidth * 3 / 8;
		pt[0].y = rcDraw.top + nHeight / 2;
		nTemp = min(nHeight / 4, nWidth / 3);
		pt[1].x = pt[0].x + nTemp;
		pt[1].y = pt[0].y - nTemp;
		pt[2].x = pt[0].x + nTemp;
		pt[2].y = pt[0].y + nTemp;

		::MoveToEx(hDC, pt[0].x, pt[0].y, NULL);
		::LineTo(hDC, pt[1].x, pt[1].y);
		::MoveToEx(hDC, pt[0].x, pt[0].y, NULL);
		::LineTo(hDC, pt[2].x, pt[2].y);

		::Draw3dRect(hDC, &rcDraw, pSysButton->clrBd, pSysButton->clrBd);

		::SelectObject(hDC, hOldPen);
		::DeleteObject(hDrawPen);
	}
}

void CDreamSkinSpinCtrl::DrawRightButton(HDC hDC, RECT rcDraw, int nDrawStatus)
{
	SKINSYSBUTTON *pSysButton;
	HPEN hDrawPen, hOldPen;

	switch(nDrawStatus)
	{
	case DRAWSTATUS_PRESS:
		pSysButton = &m_pSkinSpinCtrl->skinBtnRightPress;
		break;
	case DRAWSTATUS_HOVER:
		pSysButton = &m_pSkinSpinCtrl->skinBtnRightHover;
		break;
	case DRAWSTATUS_DISABLE:
		pSysButton = &m_pSkinSpinCtrl->skinBtnRightDisable;
		break;
	default:
		pSysButton = &m_pSkinSpinCtrl->skinBtnRightNormal;
		break;
	}

	if (pSysButton->nDrawType == DRAWTYPE_STRETCHBITMAP && pSysButton->imgDraw.hImage)
	{
		::DrawStretchBitmap(hDC, rcDraw.left, rcDraw.top, rcDraw.right - rcDraw.left, rcDraw.bottom - rcDraw.top, pSysButton->imgDraw.hImage, pSysButton->imgDraw.x, pSysButton->imgDraw.y, pSysButton->imgDraw.nWidth, pSysButton->imgDraw.nHeight);
	}
	else
	{
		::FillSolidRect(hDC, &rcDraw, pSysButton->clrBk.clrStart);

		POINT pt[3];
		int nWidth = rcDraw.right - rcDraw.left;
		int nHeight = rcDraw.bottom - rcDraw.top;
		hDrawPen = ::CreatePen(PS_SOLID, 1, pSysButton->clrTxt);
		hOldPen = (HPEN)::SelectObject(hDC, hDrawPen);
		int nTemp;

		pt[0].x = rcDraw.right - nWidth * 3 / 8;
		pt[0].y = rcDraw.top + nHeight / 2;
		nTemp = min(nHeight / 4, nWidth / 3);
		pt[1].x = pt[0].x - nTemp;
		pt[1].y = pt[0].y - nTemp;
		pt[2].x = pt[0].x - nTemp;
		pt[2].y = pt[0].y + nTemp;

		::MoveToEx(hDC, pt[0].x, pt[0].y, NULL);
		::LineTo(hDC, pt[1].x, pt[1].y);
		::MoveToEx(hDC, pt[0].x, pt[0].y, NULL);
		::LineTo(hDC, pt[2].x, pt[2].y);

		::Draw3dRect(hDC, &rcDraw, pSysButton->clrBd, pSysButton->clrBd);

		::SelectObject(hDC, hOldPen);
		::DeleteObject(hDrawPen);
	}
}

void CDreamSkinSpinCtrl::DrawTopButton(HDC hDC, RECT rcDraw, int nDrawStatus)
{
	SKINSYSBUTTON *pSysButton;
	HPEN hDrawPen, hOldPen;

	switch(nDrawStatus)
	{
	case DRAWSTATUS_PRESS:
		pSysButton = &m_pSkinSpinCtrl->skinBtnTopPress;
		break;
	case DRAWSTATUS_HOVER:
		pSysButton = &m_pSkinSpinCtrl->skinBtnTopHover;
		break;
	case DRAWSTATUS_DISABLE:
		pSysButton = &m_pSkinSpinCtrl->skinBtnTopDisable;
		break;
	default:
		pSysButton = &m_pSkinSpinCtrl->skinBtnTopNormal;
		break;
	}

	if (pSysButton->nDrawType == DRAWTYPE_STRETCHBITMAP && pSysButton->imgDraw.hImage)
	{
		::DrawStretchBitmap(hDC, rcDraw.left, rcDraw.top, rcDraw.right - rcDraw.left, rcDraw.bottom - rcDraw.top, pSysButton->imgDraw.hImage, pSysButton->imgDraw.x, pSysButton->imgDraw.y, pSysButton->imgDraw.nWidth, pSysButton->imgDraw.nHeight);
	}
	else
	{
		::FillSolidRect(hDC, &rcDraw, pSysButton->clrBk.clrStart);

		POINT pt[3];
		int nWidth = rcDraw.right - rcDraw.left;
		int nHeight = rcDraw.bottom - rcDraw.top;
		hDrawPen = ::CreatePen(PS_SOLID, 1, pSysButton->clrTxt);
		hOldPen = (HPEN)::SelectObject(hDC, hDrawPen);
		int nTemp;

		pt[0].x = rcDraw.left + nWidth / 2;
		pt[0].y = rcDraw.top + nHeight * 3 / 8;
		nTemp = min(nHeight / 3, nWidth / 4);
		pt[1].x = pt[0].x - nTemp;
		pt[1].y = pt[0].y + nTemp;
		pt[2].x = pt[0].x + nTemp;
		pt[2].y = pt[0].y + nTemp;

		::MoveToEx(hDC, pt[0].x, pt[0].y, NULL);
		::LineTo(hDC, pt[1].x, pt[1].y);
		::MoveToEx(hDC, pt[0].x, pt[0].y, NULL);
		::LineTo(hDC, pt[2].x, pt[2].y);

		::Draw3dRect(hDC, &rcDraw, pSysButton->clrBd, pSysButton->clrBd);

		::SelectObject(hDC, hOldPen);
		::DeleteObject(hDrawPen);
	}
}

void CDreamSkinSpinCtrl::DrawBottomButton(HDC hDC, RECT rcDraw, int nDrawStatus)
{
	SKINSYSBUTTON *pSysButton;
	HPEN hDrawPen, hOldPen;

	switch(nDrawStatus)
	{
	case DRAWSTATUS_PRESS:
		pSysButton = &m_pSkinSpinCtrl->skinBtnBottomPress;
		break;
	case DRAWSTATUS_HOVER:
		pSysButton = &m_pSkinSpinCtrl->skinBtnBottomHover;
		break;
	case DRAWSTATUS_DISABLE:
		pSysButton = &m_pSkinSpinCtrl->skinBtnBottomDisable;
		break;
	default:
		pSysButton = &m_pSkinSpinCtrl->skinBtnBottomNormal;
		break;
	}

	if (pSysButton->nDrawType == DRAWTYPE_STRETCHBITMAP && pSysButton->imgDraw.hImage)
	{
		::DrawStretchBitmap(hDC, rcDraw.left, rcDraw.top, rcDraw.right - rcDraw.left, rcDraw.bottom - rcDraw.top, pSysButton->imgDraw.hImage, pSysButton->imgDraw.x, pSysButton->imgDraw.y, pSysButton->imgDraw.nWidth, pSysButton->imgDraw.nHeight);
	}
	else
	{
		::FillSolidRect(hDC, &rcDraw, pSysButton->clrBk.clrStart);

		POINT pt[3];
		int nWidth = rcDraw.right - rcDraw.left;
		int nHeight = rcDraw.bottom - rcDraw.top;
		hDrawPen = ::CreatePen(PS_SOLID, 1, pSysButton->clrTxt);
		hOldPen = (HPEN)::SelectObject(hDC, hDrawPen);
		int nTemp;

		pt[0].x = rcDraw.left + nWidth / 2;
		pt[0].y = rcDraw.bottom - nHeight * 3 / 8;
		nTemp = min(nHeight / 3, nWidth / 4);
		pt[1].x = pt[0].x - nTemp;
		pt[1].y = pt[0].y - nTemp;
		pt[2].x = pt[0].x + nTemp;
		pt[2].y = pt[0].y - nTemp;

		::MoveToEx(hDC, pt[0].x, pt[0].y, NULL);
		::LineTo(hDC, pt[1].x, pt[1].y);
		::MoveToEx(hDC, pt[0].x, pt[0].y, NULL);
		::LineTo(hDC, pt[2].x, pt[2].y);

		::Draw3dRect(hDC, &rcDraw, pSysButton->clrBd, pSysButton->clrBd);

		::SelectObject(hDC, hOldPen);
		::DeleteObject(hDrawPen);
	}
}

int CDreamSkinSpinCtrl::SpinCtrlHitTest(RECT rcWindow, POINTS point, DWORD dwStyle)
{
	POINT pt;
	pt.x = point.x;
	pt.y = point.y;

	return SpinCtrlHitTest(rcWindow, pt, dwStyle);
}

int CDreamSkinSpinCtrl::SpinCtrlHitTest(RECT rcWindow, POINT point, DWORD dwStyle)
{
	int nResult = SPHT_NOWHERE;

	if (::PtInRect(&rcWindow, point))
	{
		if (dwStyle & UDS_HORZ)
		{
			if (point.x < (rcWindow.right + rcWindow.left) / 2)
				nResult = SPHT_ARROW_LEFT;
			else
				nResult = SPHT_ARROW_RIGHT;
		}
		else
		{
			if (point.y < (rcWindow.bottom + rcWindow.top) / 2)
				nResult = SPHT_ARROW_TOP;
			else
				nResult = SPHT_ARROW_RIGHT;
		}
	}

	return nResult;
}
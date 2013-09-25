//DreamSkinSliderCtrl.cpp
#include <windows.h>
#include <commctrl.h>

#include "WinGdiEx.h"
#include "DreamSkinMain.h"
#include "DreamSkinLoader.h"
#include "DreamSkinWindow.h"
#include "DreamSkinSliderCtrl.h"

//static member declaration
WNDPROC         CDreamSkinSliderCtrl::s_DefaultWindowProc = NULL;
SKINSLIDERCTRL  CDreamSkinSliderCtrl::s_SkinSliderCtrl;

CDreamSkinSliderCtrl::CDreamSkinSliderCtrl(HWND hWnd, WNDPROC OrgWndProc)
	: CDreamSkinWindow(hWnd, OrgWndProc)
{
	m_pSkinSliderCtrl = &s_SkinSliderCtrl;
	m_nHover = SCHT_NOWHERE;

	m_SliderCtrlTrackInfo.nLButtonDown = SCHT_NOWHERE;
	m_SliderCtrlTrackInfo.bIsTracking = FALSE;
	m_SliderCtrlTrackInfo.nTrackPos = 0;
}

CDreamSkinSliderCtrl::~CDreamSkinSliderCtrl()
{
	if (m_pSkinSliderCtrl != &s_SkinSliderCtrl)
		delete m_pSkinSliderCtrl;
}

void CDreamSkinSliderCtrl::Reload()
{
	if (m_hWnd && ::IsWindow(m_hWnd))
	{
		RECT rcClient;
		::GetClientRect(m_hWnd, &rcClient);

		m_bCreateRegion = TRUE;
		::InvalidateRect(m_hWnd, &rcClient, TRUE);
	}
}

BOOL CDreamSkinSliderCtrl::ApplySkin(CDreamSkinLoader *pLoader)
{
	BOOL bResult = TRUE;

	InitialClass();

	SKINSLIDERCTRL SkinSliderCtrl;
	pLoader->GetSkinSliderCtrl(&SkinSliderCtrl);
	s_SkinSliderCtrl = SkinSliderCtrl;

	return bResult;
}

CDreamSkinSliderCtrl* CDreamSkinSliderCtrl::CreateInstance(HWND hWnd, WNDPROC OrgWndProc)
{
	CDreamSkinSliderCtrl *pSkinSliderCtrl = new CDreamSkinSliderCtrl(hWnd, OrgWndProc);
	return pSkinSliderCtrl;
}

LRESULT WINAPI CDreamSkinSliderCtrl::DefWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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

BOOL CDreamSkinSliderCtrl::InitialClass()
{
	//Get the default window proc for this class
	WNDCLASSW clsSliderCtrlDefault;
	if(!GetClassInfoW(GetModuleHandle(NULL), DEFAULT_SLIDERCTRL_CLASSNAME_W, &clsSliderCtrlDefault))
	{
		//Get class info failed
		//TODO: add error handle code
		return FALSE;
	}
	s_DefaultWindowProc = clsSliderCtrlDefault.lpfnWndProc;

	GetDefaultSkin(&s_SkinSliderCtrl);

	return TRUE;
}

void CDreamSkinSliderCtrl::DestroyClass()
{

}

BOOL CDreamSkinSliderCtrl::GetDefaultSkin(SKINSLIDERCTRL *pSkinSliderCtrl)
{
	if (pSkinSliderCtrl)
	{
		GetDefaultBackground(&pSkinSliderCtrl->skinHBkNormal, RGB(236, 233, 216));
		GetDefaultBackground(&pSkinSliderCtrl->skinHBkDisable, RGB(236, 233, 216));

		GetDefaultBackground(&pSkinSliderCtrl->skinVBkNormal, RGB(236, 233, 216));
		GetDefaultBackground(&pSkinSliderCtrl->skinVBkDisable, RGB(236, 233, 216));

		GetDefaultItem(&pSkinSliderCtrl->skinHSliderNormal, RGB(236, 233, 216), RGB(255, 255, 255), RGB(113, 111, 100), 1);
		GetDefaultItem(&pSkinSliderCtrl->skinHSliderDisable, RGB(236, 233, 216), RGB(255, 255, 255), RGB(113, 111, 100), 1);
		pSkinSliderCtrl->skinHSliderNormal.skinBBorder.clrDraw.clrStart = RGB(255, 255, 255);
		pSkinSliderCtrl->skinHSliderNormal.skinRBorder.clrDraw.clrStart = RGB(255, 255, 255);
		pSkinSliderCtrl->skinHSliderDisable.skinBBorder.clrDraw.clrStart = RGB(255, 255, 255);
		pSkinSliderCtrl->skinHSliderDisable.skinRBorder.clrDraw.clrStart = RGB(255, 255, 255);

		GetDefaultItem(&pSkinSliderCtrl->skinVSliderNormal, RGB(236, 233, 216), RGB(255, 255, 255), RGB(113, 111, 100), 1);
		GetDefaultItem(&pSkinSliderCtrl->skinVSliderDisable, RGB(236, 233, 216), RGB(255, 255, 255), RGB(113, 111, 100), 1);
		pSkinSliderCtrl->skinVSliderNormal.skinBBorder.clrDraw.clrStart = RGB(255, 255, 255);
		pSkinSliderCtrl->skinVSliderNormal.skinRBorder.clrDraw.clrStart = RGB(255, 255, 255);
		pSkinSliderCtrl->skinVSliderDisable.skinBBorder.clrDraw.clrStart = RGB(255, 255, 255);
		pSkinSliderCtrl->skinVSliderDisable.skinRBorder.clrDraw.clrStart = RGB(255, 255, 255);

		GetDefaultItem(&pSkinSliderCtrl->skinHThumbNormal, RGB(0, 0, 128), RGB(255, 255, 255), RGB(0, 0, 0), 0);
		GetDefaultItem(&pSkinSliderCtrl->skinHThumbDisable, RGB(113, 111, 100), RGB(255, 255, 255), RGB(0, 0, 0), 0);
		GetDefaultItem(&pSkinSliderCtrl->skinHThumbHover, RGB(0, 0, 255), RGB(255, 255, 255), RGB(0, 0, 0), 0);
		GetDefaultItem(&pSkinSliderCtrl->skinHThumbPress, RGB(113, 111, 100), RGB(255, 255, 255), RGB(0, 0, 0), 0);

		GetDefaultItem(&pSkinSliderCtrl->skinVThumbNormal, RGB(0, 0, 128), RGB(255, 255, 255), RGB(0, 0, 0), 0);
		GetDefaultItem(&pSkinSliderCtrl->skinVThumbDisable, RGB(113, 111, 100), RGB(255, 255, 255), RGB(0, 0, 0), 0);
		GetDefaultItem(&pSkinSliderCtrl->skinVThumbHover, RGB(0, 0, 255), RGB(255, 255, 255), RGB(0, 0, 0), 0);
		GetDefaultItem(&pSkinSliderCtrl->skinVThumbPress, RGB(113, 111, 100), RGB(255, 255, 255), RGB(0, 0, 0), 0);

		pSkinSliderCtrl->nHSliderWidth = 3;
		pSkinSliderCtrl->nVSliderWidth = 3;

		pSkinSliderCtrl->nHThumbWidth = 10;
		pSkinSliderCtrl->nVThumbWidth = 10;
	}

	return TRUE;
}

LRESULT CDreamSkinSliderCtrl::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT nResult = ERROR_SUCCESS;

	switch(message)
	{
	case WM_ERASEBKGND:
		break;
	case WM_HSCROLL:
		nResult = OnHScroll(wParam, (HWND)lParam);
		break;
	case WM_LBUTTONDBLCLK:
		nResult = ::SendMessageW(m_hWnd, WM_LBUTTONDOWN, wParam, lParam);
		break;
	case WM_LBUTTONDOWN:
		nResult = OnLButtonDown(wParam, MAKEPOINTS(lParam));
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
	case TBM_SETPOS:
		nResult = CDreamSkinWindow::DefWindowProc(message, wParam, lParam);
		if (wParam)
			::InvalidateRect(m_hWnd, NULL, FALSE);
		break;
	case WM_VSCROLL:
		nResult = OnVScroll(wParam, (HWND)lParam);
		break;
	default:
		nResult = CDreamSkinWindow::DefWindowProc(message, wParam, lParam);
	}

	return nResult;
}

LRESULT CDreamSkinSliderCtrl::OnHScroll(UINT nSBCode, HWND hWndCtrl)
{
	LRESULT lResult = 0;

	if (hWndCtrl == m_hWnd)
	{
		int nPos = ::SendMessage(m_hWnd, TBM_GETPOS, 0, 0);
		int nMin = ::SendMessage(m_hWnd, TBM_GETRANGEMIN, 0, 0);
		int nMax = ::SendMessage(m_hWnd, TBM_GETRANGEMAX, 0, 0);
		int nPage;

		switch(nSBCode & 0x0000FFFF)
		{
		case TB_LINEUP:
			if (nPos > nMin)
			{
				nPos--;
				::SendMessage(m_hWnd, TBM_SETPOS, 0, (LPARAM)nPos);
			}
			break;
		case TB_LINEDOWN:
			if (nPos < nMax)
			{
				nPos++;
				::SendMessage(m_hWnd, TBM_SETPOS, 0, (LPARAM)nPos);
			}
			break;
		case TB_PAGEUP:
			if (nPos > nMin)
			{
				nPage = ::SendMessage(m_hWnd, TBM_GETPAGESIZE, 0, 0);
				nPos -= nPage;
				if (nPos < nMin)
					nPos = nMin;
				::SendMessage(m_hWnd, TBM_SETPOS, 0, (LPARAM)nPos);
			}
			break;
		case TB_PAGEDOWN:
			if (nPos < nMax)
			{
				nPage = ::SendMessage(m_hWnd, TBM_GETPAGESIZE, 0, 0);
				nPos += nPage;
				if (nPos > nMax)
					nPos = nMax;
				::SendMessage(m_hWnd, TBM_SETPOS, 0, (LPARAM)nPos);
			}
			break;
		case SB_THUMBPOSITION:
			if (m_SliderCtrlTrackInfo.nTrackPos != nPos)
			{
				nPos = m_SliderCtrlTrackInfo.nTrackPos;
				if (nPos > nMax)
					nPos = nMax;
				else if (nPos < nMin)
					nPos = nMin;
				::SendMessage(m_hWnd, TBM_SETPOS, 0, (LPARAM)nPos);
			}
			nSBCode = TB_ENDTRACK;
			break;
		case TB_THUMBTRACK:
			if (m_SliderCtrlTrackInfo.nTrackPos != nPos)
			{
				nPos = m_SliderCtrlTrackInfo.nTrackPos;
				if (nPos > nMax)
					nPos = nMax;
				else if (nPos < nMin)
					nPos = nMin;
				::SendMessage(m_hWnd, TBM_SETPOS, 0, (LPARAM)nPos);
			}
			break;
		}

		HWND hWndParent = ::GetParent(hWndCtrl);
		if (hWndParent)
			lResult = ::SendMessage(hWndParent, WM_HSCROLL, nSBCode, (LPARAM)hWndCtrl);
		else
			lResult = CDreamSkinWindow::DefWindowProc(m_hWnd, WM_HSCROLL, nSBCode, (LPARAM)hWndCtrl);
	}

	return lResult;
}

LRESULT CDreamSkinSliderCtrl::OnLButtonDown(UINT nFlags, POINTS point)
{
	LRESULT nResult = 0;

	POINT pt;
	pt.x = point.x;
	pt.y = point.y;

	DWORD dwStyle = ::GetWindowLong(m_hWnd, GWL_STYLE);

	RECT rcClient;
	::GetClientRect(m_hWnd, &rcClient);

	switch(SliderCtrlHitTest(rcClient, pt, dwStyle))
	{
	case SCHT_THUMB:
		m_SliderCtrlTrackInfo.nLButtonDown = SCHT_THUMB;
		TrackSliderCtrl(dwStyle, SCHT_THUMB);
		break;
	case SCHT_REGION_UP:
		m_SliderCtrlTrackInfo.nLButtonDown = SCHT_REGION_UP;
		TrackSliderCtrl(dwStyle, SCHT_REGION_UP);
		break;
	case SCHT_REGION_DOWN:
		m_SliderCtrlTrackInfo.nLButtonDown = SCHT_REGION_DOWN;
		TrackSliderCtrl(dwStyle, SCHT_REGION_DOWN);
		break;
	default:
		m_SliderCtrlTrackInfo.nLButtonDown = 0;
		::InvalidateRect(m_hWnd, &rcClient, FALSE);
		break;
	}

	return nResult;
}

LRESULT CDreamSkinSliderCtrl::OnMouseLeave()
{
	POINT point;
	::GetCursorPos(&point);
	::ScreenToClient(m_hWnd, &point);

	DWORD dwStyle = ::GetWindowLong(m_hWnd, GWL_STYLE);

	RECT rcClient;
	::GetClientRect(m_hWnd, &rcClient);

	int nHover = SliderCtrlHitTest(rcClient, point, dwStyle);
	if (m_nHover != nHover)
	{
		m_nHover = nHover;
		::InvalidateRect(m_hWnd, &rcClient, FALSE);
	}

	return 0;
}

LRESULT CDreamSkinSliderCtrl::OnMouseMove(UINT nFlags, POINTS point)
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

	int nHover = SliderCtrlHitTest(rcClient, pt, dwStyle);
	if (m_nHover != nHover)
	{
		m_nHover = nHover;
		::InvalidateRect(m_hWnd, &rcClient, FALSE);
	}

	return 0;
}

LRESULT CDreamSkinSliderCtrl::OnPaint()
{
	PAINTSTRUCT ps;
	HDC hPaintDC = ::BeginPaint(m_hWnd, &ps);

	RECT rcClient;
	::GetClientRect(m_hWnd, &rcClient);

	DWORD dwStyle = ::GetWindowLong(m_hWnd, GWL_STYLE);
	if (dwStyle & TBS_VERT)
		DrawVertSliderCtrl(hPaintDC, rcClient, dwStyle);
	else
		DrawHorzSliderCtrl(hPaintDC, rcClient, dwStyle);

	::EndPaint(m_hWnd, &ps);

	return 0;
}

LRESULT CDreamSkinSliderCtrl::OnVScroll(UINT nSBCode, HWND hWndCtrl)
{
	LRESULT lResult = 0;

	if (hWndCtrl == m_hWnd)
	{
		int nPos = ::SendMessage(m_hWnd, TBM_GETPOS, 0, 0);
		int nMin = ::SendMessage(m_hWnd, TBM_GETRANGEMIN, 0, 0);
		int nMax = ::SendMessage(m_hWnd, TBM_GETRANGEMAX, 0, 0);
		int nPage;

		switch(nSBCode & 0x0000FFFF)
		{
		case TB_LINEUP:
			if (nPos > nMin)
			{
				nPos--;
				::SendMessage(m_hWnd, TBM_SETPOS, 0, (LPARAM)nPos);
			}
			break;
		case TB_LINEDOWN:
			if (nPos < nMax)
			{
				nPos++;
				::SendMessage(m_hWnd, TBM_SETPOS, 0, (LPARAM)nPos);
			}
			break;
		case TB_PAGEUP:
			if (nPos > nMin)
			{
				nPage = ::SendMessage(m_hWnd, TBM_GETPAGESIZE, 0, 0);
				nPos -= nPage;
				if (nPos < nMin)
					nPos = nMin;
				::SendMessage(m_hWnd, TBM_SETPOS, 0, (LPARAM)nPos);
			}
			break;
		case TB_PAGEDOWN:
			if (nPos < nMax)
			{
				nPage = ::SendMessage(m_hWnd, TBM_GETPAGESIZE, 0, 0);
				nPos += nPage;
				if (nPos > nMax)
					nPos = nMax;
				::SendMessage(m_hWnd, TBM_SETPOS, 0, (LPARAM)nPos);
			}
			break;
		case SB_THUMBPOSITION:
			if (m_SliderCtrlTrackInfo.nTrackPos != nPos)
			{
				nPos = m_SliderCtrlTrackInfo.nTrackPos;
				if (nPos > nMax)
					nPos = nMax;
				else if (nPos < nMin)
					nPos = nMin;
				::SendMessage(m_hWnd, TBM_SETPOS, 0, (LPARAM)nPos);
			}
			nSBCode = TB_ENDTRACK;
			break;
		case TB_THUMBTRACK:
			if (m_SliderCtrlTrackInfo.nTrackPos != nPos)
			{
				nPos = m_SliderCtrlTrackInfo.nTrackPos;
				if (nPos > nMax)
					nPos = nMax;
				else if (nPos < nMin)
					nPos = nMin;
				::SendMessage(m_hWnd, TBM_SETPOS, 0, (LPARAM)nPos);
			}
		}

		HWND hWndParent = ::GetParent(hWndCtrl);
		if (hWndParent)
			lResult = ::SendMessage(hWndParent, WM_VSCROLL, nSBCode, (LPARAM)hWndCtrl);
		else
			lResult = CDreamSkinWindow::DefWindowProc(m_hWnd, WM_VSCROLL, nSBCode, (LPARAM)hWndCtrl);
	}

	return lResult;
}

void CDreamSkinSliderCtrl::DrawHorzSliderCtrl(HDC hDC, RECT rcClient, DWORD dwStyle)
{
	RECT rcSlider, rcThumb;
	GetHorzThumbRect(rcClient, &rcSlider, &rcThumb);

	if (dwStyle & WS_DISABLED)
	{
		CDreamSkinWindow::DrawBackground(hDC, rcClient, &m_pSkinSliderCtrl->skinHBkDisable);
		if (rcSlider.right > rcSlider.left)
		{
			CDreamSkinWindow::DrawItem(hDC, &m_pSkinSliderCtrl->skinHSliderDisable, rcSlider, NULL);
			CDreamSkinWindow::DrawItem(hDC, &m_pSkinSliderCtrl->skinHThumbDisable, rcThumb, NULL);
		}
	}
	else
	{
		CDreamSkinWindow::DrawBackground(hDC, rcClient, &m_pSkinSliderCtrl->skinHBkNormal);
		if (rcSlider.right > rcSlider.left)
		{
			CDreamSkinWindow::DrawItem(hDC, &m_pSkinSliderCtrl->skinHSliderNormal, rcSlider, NULL);
			if (m_nHover == SCHT_THUMB)
			{
				if (m_SliderCtrlTrackInfo.bIsTracking && m_SliderCtrlTrackInfo.nLButtonDown == SCHT_THUMB)
					CDreamSkinWindow::DrawItem(hDC, &m_pSkinSliderCtrl->skinHThumbPress, rcThumb, NULL);
				else
					CDreamSkinWindow::DrawItem(hDC, &m_pSkinSliderCtrl->skinHThumbHover, rcThumb, NULL);
			}
			else
				CDreamSkinWindow::DrawItem(hDC, &m_pSkinSliderCtrl->skinHThumbNormal, rcThumb, NULL);
		}
	}
}

void CDreamSkinSliderCtrl::DrawVertSliderCtrl(HDC hDC, RECT rcClient, DWORD dwStyle)
{
	RECT rcSlider, rcThumb;
	GetVertThumbRect(rcClient, &rcSlider, &rcThumb);

	if (dwStyle & WS_DISABLED)
	{
		CDreamSkinWindow::DrawBackground(hDC, rcClient, &m_pSkinSliderCtrl->skinVBkDisable);
		if (rcSlider.bottom > rcSlider.top)
		{
			CDreamSkinWindow::DrawItem(hDC, &m_pSkinSliderCtrl->skinVSliderDisable, rcSlider, NULL);
			CDreamSkinWindow::DrawItem(hDC, &m_pSkinSliderCtrl->skinVThumbDisable, rcThumb, NULL);
		}
	}
	else
	{
		CDreamSkinWindow::DrawBackground(hDC, rcClient, &m_pSkinSliderCtrl->skinVBkNormal);
		if (rcSlider.bottom > rcSlider.top)
		{
			CDreamSkinWindow::DrawItem(hDC, &m_pSkinSliderCtrl->skinVSliderNormal, rcSlider, NULL);
			if (m_nHover == SCHT_THUMB)
			{
				if (m_SliderCtrlTrackInfo.bIsTracking && m_SliderCtrlTrackInfo.nLButtonDown == SCHT_THUMB)
					CDreamSkinWindow::DrawItem(hDC, &m_pSkinSliderCtrl->skinVThumbPress, rcThumb, NULL);
				else
					CDreamSkinWindow::DrawItem(hDC, &m_pSkinSliderCtrl->skinVThumbHover, rcThumb, NULL);
			}
			else
				CDreamSkinWindow::DrawItem(hDC, &m_pSkinSliderCtrl->skinVThumbNormal, rcThumb, NULL);
		}
	}
}

void CDreamSkinSliderCtrl::GetHorzThumbRect(RECT rcWindow, RECT *prcSlider, RECT *prcThumb)
{
	prcSlider->left = rcWindow.left + m_pSkinSliderCtrl->nHThumbWidth / 2;
	prcSlider->right = rcWindow.right - m_pSkinSliderCtrl->nHThumbWidth / 2;
	prcSlider->top = (rcWindow.bottom - rcWindow.top) / 2 - m_pSkinSliderCtrl->nHSliderWidth / 2;
	if (prcSlider->top < rcWindow.top)
		prcSlider->top = rcWindow.top;
	prcSlider->bottom = prcSlider->top + m_pSkinSliderCtrl->nHSliderWidth;
	if (prcSlider->bottom > rcWindow.bottom)
		prcSlider->bottom = rcWindow.bottom;

	int nMin = ::SendMessage(m_hWnd, TBM_GETRANGEMIN, 0, 0);
	int nMax = ::SendMessage(m_hWnd, TBM_GETRANGEMAX, 0, 0);
	int nPos = ::SendMessage(m_hWnd, TBM_GETPOS, 0, 0);
	if (nMax > nMin && prcSlider->right > prcSlider->left)
	{
		int nCenter = ((nPos - nMin) * prcSlider->right + (nMax - nPos) * prcSlider->left) / (nMax - nMin);
		prcThumb->left = nCenter - m_pSkinSliderCtrl->nHThumbWidth / 2;
		if (prcThumb->left < rcWindow.left)
			prcThumb->left = rcWindow.left;
		prcThumb->right = prcThumb->left + m_pSkinSliderCtrl->nHThumbWidth;
		prcThumb->top = rcWindow.top;
		prcThumb->bottom = rcWindow.bottom;
	}
	else
	{
		prcThumb->left = rcWindow.left;
		prcThumb->right = prcThumb->left + m_pSkinSliderCtrl->nHThumbWidth;
		prcThumb->top = rcWindow.top;
		prcThumb->bottom = rcWindow.bottom;
	}
}

void CDreamSkinSliderCtrl::GetVertThumbRect(RECT rcWindow, RECT *prcSlider, RECT *prcThumb)
{
	prcSlider->top = rcWindow.top + m_pSkinSliderCtrl->nVThumbWidth / 2;
	prcSlider->bottom = rcWindow.bottom - m_pSkinSliderCtrl->nVThumbWidth / 2;
	prcSlider->left = (rcWindow.right - rcWindow.left) / 2 - m_pSkinSliderCtrl->nVSliderWidth / 2;
	if (prcSlider->left < rcWindow.left)
		prcSlider->left = rcWindow.left;
	prcSlider->right = prcSlider->left + m_pSkinSliderCtrl->nVSliderWidth;
	if (prcSlider->right > rcWindow.right)
		prcSlider->right = rcWindow.right;

	int nMin = ::SendMessage(m_hWnd, TBM_GETRANGEMIN, 0, 0);
	int nMax = ::SendMessage(m_hWnd, TBM_GETRANGEMAX, 0, 0);
	int nPos = ::SendMessage(m_hWnd, TBM_GETPOS, 0, 0);
	if (nMax > nMin && prcSlider->bottom > prcSlider->top)
	{
		int nCenter = ((nPos - nMin) * prcSlider->bottom + (nMax - nPos) * prcSlider->top) / (nMax - nMin);
		prcThumb->top = nCenter - m_pSkinSliderCtrl->nVThumbWidth / 2;
		if (prcThumb->top < rcWindow.top)
			prcThumb->top = rcWindow.top;
		prcThumb->bottom = prcThumb->top + m_pSkinSliderCtrl->nVThumbWidth;
		prcThumb->left = rcWindow.left;
		prcThumb->right = rcWindow.right;
	}
	else
	{
		prcThumb->top = rcWindow.top;
		prcThumb->bottom = prcThumb->top + m_pSkinSliderCtrl->nVThumbWidth;
		prcThumb->left = rcWindow.left;
		prcThumb->right = rcWindow.right;
	}
}

int CDreamSkinSliderCtrl::SliderCtrlDragPos(RECT rcSlider, POINT point, int nOffset, DWORD dwStyle)
{
	int nResult;
	int nMin = ::SendMessage(m_hWnd, TBM_GETRANGEMIN, 0, 0);
	int nMax = ::SendMessage(m_hWnd, TBM_GETRANGEMAX, 0, 0);

	if (dwStyle & TBS_VERT)
	{
		point.y -= nOffset;
		if (point.y <= rcSlider.top)
			nResult = nMin;
		else if (point.y >= rcSlider.bottom)
			nResult = nMax;
		else
		{
			if (rcSlider.bottom > rcSlider.top)
				nResult = (nMax * (point.y - rcSlider.top) + (rcSlider.bottom - point.y) * nMin) / (rcSlider.bottom - rcSlider.top);
			else
				nResult = nMin;
		}
	}
	else
	{
		point.x -= nOffset;
		if (point.x <= rcSlider.left)
			nResult = nMin;
		else if (point.x >= rcSlider.right)
			nResult = nMax;
		else
		{
			if (rcSlider.right > rcSlider.left)
				nResult = (nMax * (point.x - rcSlider.left) + (rcSlider.right - point.x) * nMin) / (rcSlider.right - rcSlider.left);
			else
				nResult = nMin;
		}
	}

	return nResult;
}

int CDreamSkinSliderCtrl::SliderCtrlHitTest(RECT rcWindow, POINT point, DWORD dwStyle)
{
	RECT rcSlider, rcThumb;
	int nResult = SCHT_NOWHERE;
	if (::PtInRect(&rcWindow, point))
	{
		if (dwStyle & TBS_VERT)
		{
			GetVertThumbRect(rcWindow, &rcSlider, &rcThumb);
			if (::PtInRect(&rcThumb, point))
				nResult = SCHT_THUMB;
			else if (point.y < rcThumb.top)
				nResult = SCHT_REGION_UP;
			else
				nResult = SCHT_REGION_DOWN;
		}
		else
		{
			GetHorzThumbRect(rcWindow, &rcSlider, &rcThumb);
			if (::PtInRect(&rcThumb, point))
				nResult = SCHT_THUMB;
			else if (point.x < rcThumb.left)
				nResult = SCHT_REGION_UP;
			else
				nResult = SCHT_REGION_DOWN;
		}
	}

	return nResult;
}

void CDreamSkinSliderCtrl::TrackSliderCtrl(DWORD dwStyle, int nHitTest)
{
	int nPos = ::SendMessage(m_hWnd, TBM_GETPOS, 0, 0);

	UINT nScrollMsg;
	int nMsgCode;

	if (dwStyle & TBS_VERT)
		nScrollMsg = WM_VSCROLL;
	else
		nScrollMsg = WM_HSCROLL;

	switch(nHitTest)
	{
	case SCHT_REGION_DOWN:
		nMsgCode = TB_PAGEDOWN;
		break;
	case SCHT_REGION_UP:
		nMsgCode = TB_PAGEUP;
		break;
	default:
		nMsgCode = TB_THUMBTRACK;
		break;
	}

	MSG Msg;
	POINT point;

	m_SliderCtrlTrackInfo.bIsTracking = TRUE;
	m_SliderCtrlTrackInfo.nTrackPos = nPos;
	::SetCapture(m_hWnd);

	RECT rcClient;
	::GetClientRect(m_hWnd, &rcClient);

	if (nHitTest == SCHT_REGION_DOWN || nHitTest == SCHT_REGION_UP)
	{
		::SendMessage(m_hWnd, nScrollMsg, nMsgCode, (LPARAM)m_hWnd);
		::InvalidateRect(m_hWnd, &rcClient, FALSE);

		int nTimer1 = ::SetTimer(0, 0, 500, NULL);
		int nTimer2 = 0;
		int nNewHitTest;

		while (::GetCapture() == m_hWnd && m_SliderCtrlTrackInfo.bIsTracking)
		{
			if (!::GetMessage(&Msg, NULL, 0, 0))
				break;

			switch(Msg.message)
			{
			case WM_LBUTTONUP:
				if (nTimer2 > 0)
					KillTimer(0, nTimer2);
				m_SliderCtrlTrackInfo.bIsTracking = FALSE;
				if (m_SliderCtrlTrackInfo.nLButtonDown)
					m_SliderCtrlTrackInfo.nLButtonDown = 0;

				::SendMessage(m_hWnd, nScrollMsg, TB_ENDTRACK, (LPARAM)m_hWnd);
				::InvalidateRect(m_hWnd, &rcClient, FALSE);
				break;
			case WM_TIMER:
				if (Msg.wParam == nTimer1)
				{
					::KillTimer(0, nTimer1);

					::GetCursorPos(&point);
					::ScreenToClient(m_hWnd, &point);
					nNewHitTest = SliderCtrlHitTest(rcClient, point, dwStyle);
					if (nNewHitTest == nHitTest)
					{
						::SendMessage(m_hWnd, nScrollMsg, nMsgCode, (LPARAM)m_hWnd);
						::InvalidateRect(m_hWnd, &rcClient, FALSE);
					}
					nTimer2 = ::SetTimer(0, 0, 100, NULL);
				}
				else if (Msg.wParam == nTimer2)
				{
					::GetCursorPos(&point);
					::ScreenToClient(m_hWnd, &point);
					nNewHitTest = SliderCtrlHitTest(rcClient, point, dwStyle);
					if (nNewHitTest == nHitTest)
					{
						::SendMessage(m_hWnd, nScrollMsg, nMsgCode, (LPARAM)m_hWnd);
						::InvalidateRect(m_hWnd, &rcClient, FALSE);
					}
				}
				else
				{
					::DispatchMessage(&Msg);
				}
				break;
			default:
				::DispatchMessage(&Msg);
				break;
			}
		}
	}
	else
	{
		::InvalidateRect(m_hWnd, &rcClient, FALSE);

		::GetCursorPos(&point);
		::ScreenToClient(m_hWnd, &point);

		RECT rcSlider, rcThumb;
		int nOffSet;
		if (dwStyle & TBS_VERT)
		{
			GetVertThumbRect(rcClient, &rcSlider, &rcThumb);
			nOffSet = point.y - (rcThumb.bottom + rcThumb.top) / 2;
		}
		else
		{
			GetHorzThumbRect(rcClient, &rcSlider, &rcThumb);
			nOffSet = point.x - (rcThumb.right + rcThumb.left) / 2;
		}
			

		while (::GetCapture() == m_hWnd && m_SliderCtrlTrackInfo.bIsTracking)
		{
			if (!::GetMessage(&Msg, NULL, 0, 0))
				break;

			switch(Msg.message)
			{
			case WM_LBUTTONUP:
				m_SliderCtrlTrackInfo.bIsTracking = FALSE;
				::GetCursorPos(&point);
				::ScreenToClient(m_hWnd, &point);
				m_SliderCtrlTrackInfo.nTrackPos = SliderCtrlDragPos(rcSlider, point, nOffSet, dwStyle);
				if (m_SliderCtrlTrackInfo.nLButtonDown)
					m_SliderCtrlTrackInfo.nLButtonDown = 0;
				
				DefWindowProc(nScrollMsg, MAKEWPARAM(TB_THUMBPOSITION, (unsigned short)m_SliderCtrlTrackInfo.nTrackPos), (LPARAM)m_hWnd);
				::InvalidateRect(m_hWnd, &rcClient, FALSE);
				break;
			case WM_MOUSEMOVE:
				::GetCursorPos(&point);
				::ScreenToClient(m_hWnd, &point);
				m_SliderCtrlTrackInfo.nTrackPos = SliderCtrlDragPos(rcSlider, point, nOffSet, dwStyle);
				
				DefWindowProc(nScrollMsg, MAKEWPARAM(TB_THUMBTRACK, (unsigned short)m_SliderCtrlTrackInfo.nTrackPos), (LPARAM)m_hWnd);
				::InvalidateRect(m_hWnd, &rcClient, FALSE);
				break;
			default:
				DispatchMessage(&Msg);
				break;
			}
		}
	}

	if (m_SliderCtrlTrackInfo.nLButtonDown)
		m_SliderCtrlTrackInfo.nLButtonDown = 0;

	if (::IsWindow(m_hWnd) && ::GetCapture() == m_hWnd)
		::ReleaseCapture();
}
// DreamSkinComboLBox.cpp

#include <windows.h>

#include "WinGdiEx.h"
#include "DreamSkinMain.h"
#include "DreamSkinLoader.h"
#include "DreamSkinWindow.h"
#include "DreamSkinScrollBar.h"
#include "DreamSKinComboBox.h"
#include "DreamSkinComboLBox.h"

//static member declaration
WNDPROC         CDreamSkinComboLBox::s_DefaultWindowProc = NULL;

CDreamSkinComboLBox::CDreamSkinComboLBox(HWND hWnd, WNDPROC OrgWndProc)
	: CDreamSkinWindow(hWnd, OrgWndProc)
{
	m_pWndComboBox = NULL;
	m_nOwnerDraw = -1;

	m_ScrollBarTrackInfo.nSBLButtonDown = 0;
	m_ScrollBarTrackInfo.bIsTracking = FALSE;
	m_ScrollBarTrackInfo.nTrackPos = 0;
	m_nSBHover = -1;
}

CDreamSkinComboLBox::~CDreamSkinComboLBox()
{

}

void CDreamSkinComboLBox::Reload()
{
	if (m_hWnd && ::IsWindow(m_hWnd))
	{
		RECT rcClient;
		::GetClientRect(m_hWnd, &rcClient);

		m_bCreateRegion = TRUE;
		::InvalidateRect(m_hWnd, &rcClient, TRUE);
	}
}

BOOL CDreamSkinComboLBox::ApplySkin(CDreamSkinLoader *pLoader)
{
	BOOL bResult = TRUE;

	InitialClass();

	return bResult;
}

CDreamSkinComboLBox* CDreamSkinComboLBox::CreateInstance(HWND hWnd, WNDPROC OrgWndProc)
{
	CDreamSkinComboLBox *pSkinListBox = new CDreamSkinComboLBox(hWnd, OrgWndProc);
	return pSkinListBox;
}

LRESULT WINAPI CDreamSkinComboLBox::DefWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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

BOOL CDreamSkinComboLBox::InitialClass()
{
	//Get the default window proc for this class
	WNDCLASSW clsComboLBoxDefault;
	if(!GetClassInfoW(GetModuleHandle(NULL), DEFAULT_COMBOLBOX_CLASSNAME_W, &clsComboLBoxDefault))
	{
		//Get class info failed
		//TODO: add error handle code
		return FALSE;
	}
	s_DefaultWindowProc = clsComboLBoxDefault.lpfnWndProc;

	return TRUE;
}

void CDreamSkinComboLBox::DestroyClass()
{

}

LRESULT CDreamSkinComboLBox::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT nResult = ERROR_SUCCESS;

	switch(message)
	{
	case WM_CREATE:
		nResult = OnCreate((LPCREATESTRUCT)lParam);
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
	case WM_WINDOWPOSCHANGING:
	case WM_WINDOWPOSCHANGED:
		nResult = CDreamSkinWindow::DefWindowProc(message, wParam, lParam);
		OnNcPaint(NULL);
		break;
	default:
		nResult = CDreamSkinWindow::DefWindowProc(message, wParam, lParam);
	}

	return nResult;

}

LRESULT CDreamSkinComboLBox::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (lpCreateStruct->style & LBS_OWNERDRAWFIXED)
	{
		m_nOwnerDraw = 1;
	}
	else if (lpCreateStruct->style & LBS_OWNERDRAWVARIABLE)
	{
		m_nOwnerDraw = 2;
	}
	else
	{
		m_nOwnerDraw = 0;
		lpCreateStruct->style |= LBS_OWNERDRAWFIXED;
	}

	CDreamSkinWindow *pHookedWindow = theSkinMain.GetHookedWindow(lpCreateStruct->hwndParent);
	if (pHookedWindow)
		if (pHookedWindow->GetType() == DREAMSKIN_COMBOBOX)
			m_pWndComboBox = (CDreamSkinComboBox*)pHookedWindow;

	return CDreamSkinWindow::DefWindowProc(WM_CREATE, 0, (LPARAM)lpCreateStruct);
}

LRESULT CDreamSkinComboLBox::OnMouseLeave()
{
	if(m_pWndComboBox && m_pWndComboBox->m_nCurHover >= 0)
	{
		int nIndex = m_pWndComboBox->m_nCurHover;
		m_pWndComboBox->m_nCurHover = -1;

		RECT rcItem;
		::SendMessage(m_hWnd, LB_GETITEMRECT, nIndex, (LPARAM)&rcItem);
		::InvalidateRect(m_hWnd, &rcItem, FALSE);
	}

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

LRESULT CDreamSkinComboLBox::OnMouseMove(UINT nFlags, POINTS point)
{
	TRACKMOUSEEVENT EventTrack;
	EventTrack.cbSize = sizeof(TRACKMOUSEEVENT);
	EventTrack.dwFlags = TME_LEAVE;
	EventTrack.hwndTrack = m_hWnd;
	EventTrack.dwHoverTime = 0;

	TrackMouseEvent(&EventTrack);

	int nSBHover = 0;
	DWORD dwStyle = ::GetWindowLong(m_hWnd, GWL_STYLE);
	POINT ptScreen;
	ptScreen.x = point.x;
	ptScreen.y = point.y;
	::ClientToScreen(m_hWnd, &ptScreen);

	if (dwStyle & WS_VSCROLL)
	{
		nSBHover = CDreamSkinScrollBar::ScrollBarHitTest(m_hWnd, SB_VERT, ptScreen);
		if (nSBHover > 0)
			nSBHover = SB_VERT << 16 | nSBHover;
	}
	
	if (dwStyle & WS_HSCROLL && nSBHover == 0)
	{
		nSBHover = CDreamSkinScrollBar::ScrollBarHitTest(m_hWnd, SB_HORZ, ptScreen);
		if (nSBHover > 0)
			nSBHover = SB_HORZ << 16 | nSBHover;
	}

	if (nSBHover != m_nSBHover)
	{
		m_nSBHover = nSBHover;
		OnNcPaint(NULL);
	}

	if (m_pWndComboBox)
	{
		DWORD dwResult = (DWORD)::SendMessage(m_hWnd, LB_ITEMFROMPOINT, 0, MAKELPARAM(point.x, point.y));
		int nIndex = -1;
		if (!HIWORD(dwResult))
			nIndex = LOWORD(dwResult);

		if(nIndex != m_pWndComboBox->m_nCurHover)
		{
			int nTemp = m_pWndComboBox->m_nCurHover;
			m_pWndComboBox->m_nCurHover = nIndex;
			
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
		}
	}

	return CDreamSkinWindow::DefWindowProc(WM_MOUSEMOVE, nFlags, MAKELPARAM(point.x, point.y));
}

LRESULT CDreamSkinComboLBox::OnNcHitTest(POINTS point)
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

LRESULT CDreamSkinComboLBox::OnNcLButtonDbClick(UINT nHitTest, POINTS point)
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

LRESULT CDreamSkinComboLBox::OnNcLButtonDown(UINT nHitTest, POINTS point)
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

LRESULT CDreamSkinComboLBox::OnNcMouseLeave()
{
	if (m_nSBHover != 0)
	{
		m_nSBHover = 0;
		OnNcPaint(NULL);
	}

	return 0;
}

LRESULT CDreamSkinComboLBox::OnNcMouseMove(UINT nHitTest, POINTS point)
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

LRESULT CDreamSkinComboLBox::OnNcPaint(HRGN hRGN)
{
	if (m_pWndComboBox)
	{
		HDC hWindowDC;
		hWindowDC = ::GetWindowDC(m_hWnd);
		// Paint into this DC

		RECT rcWindow;
		::GetWindowRect(m_hWnd, &rcWindow);
		::OffsetRect(&rcWindow, 0 - rcWindow.left, 0 - rcWindow.top);

		DWORD dwExStyle = ::GetWindowLong(m_hWnd, GWL_EXSTYLE);
		DWORD dwStyle = ::GetWindowLong(m_hWnd, GWL_STYLE);

		if (dwExStyle & WS_EX_CLIENTEDGE || dwStyle & WS_BORDER)
			m_pWndComboBox->DrawLBBorder(hWindowDC, rcWindow);

		if (dwStyle & WS_VSCROLL || dwStyle & WS_HSCROLL)
			CDreamSkinScrollBar::DrawWindowScrollBar(m_hWnd, hWindowDC, m_ScrollBarTrackInfo.nSBLButtonDown, m_nSBHover);

		::ReleaseDC(m_hWnd, hWindowDC);

		return 0;
	}
	else
	{
		return CDreamSkinWindow::DefWindowProc(m_hWnd, WM_NCPAINT, (WPARAM)hRGN, 0);
	}
}
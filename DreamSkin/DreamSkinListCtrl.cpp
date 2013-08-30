//DreamSkinListCtrl.cpp
#include <windows.h>
#include <commctrl.h>

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
	m_nOwnerDraw = -1;
	m_nCurHover = -1;
	m_nCurHoverSub = -1;
	m_nViewType = -1;

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

		GetDefaultItem(&pSkinListCtrl->skinItemNormalSelected, RGB(0, 0, 128), RGB(255, 255, 255), RGB(0, 0, 0), 0);
		GetDefaultItem(&pSkinListCtrl->skinItemDisableSelected, RGB(128, 128, 128), RGB(255, 255, 255), RGB(0, 0, 0), 0);
		GetDefaultItem(&pSkinListCtrl->skinItemHoverSelected, RGB(0, 0, 128), RGB(255, 255, 255), RGB(0, 0, 0), 0);

		GetDefaultItem(&pSkinListCtrl->skinItemNormalUnselected, RGB(255, 255, 255), RGB(0, 0, 0), RGB(0, 0, 0), 0);
		GetDefaultItem(&pSkinListCtrl->skinItemDisableUnselected, RGB(236, 233, 216), RGB(0, 0, 0), RGB(0, 0, 0), 0);
		GetDefaultItem(&pSkinListCtrl->skinItemHoverUnselected, RGB(0, 0, 255), RGB(255, 255, 255), RGB(0, 0, 0), 0);
	}

	return TRUE;
}

LRESULT CDreamSkinListCtrl::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT nResult = ERROR_SUCCESS;

	switch(message)
	{
	case LVM_SETVIEW:
		nResult = CDreamSkinWindow::DefWindowProc(message, wParam, lParam);
		if (nResult > 0)
			m_nViewType = wParam;
		break;
	case WM_CTLCOLOREDIT:
		nResult = ::SendMessage((HWND)lParam, WM_CTLCOLOREDIT, wParam, (LPARAM)m_hWnd);
		break;
	case WM_CREATE:
		nResult = OnCreate((LPCREATESTRUCT)lParam);
		break;
	//case WM_DRAWITEM:
	//	nResult = OnDrawItem((UINT)wParam, (LPDRAWITEMSTRUCT)lParam, ::GetWindowLong(m_hWnd, GWL_STYLE) & LVS_TYPEMASK);
	//	break;
	case WM_ENABLE:
		nResult = CDreamSkinWindow::DefWindowProc(message, wParam, lParam);
		UpdateWindow();
		break;
	case WM_ERASEBKGND:
		nResult = OnEraseBkgnd((HDC)wParam);
		break;
	case WM_LBUTTONDOWN:
		nResult = OnLButtonDown(wParam, MAKEPOINTS(lParam));
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
	case WM_NOTIFY:
		nResult = OnNotify(wParam, (LPNMHDR)lParam);
		break;
	//case WM_PAINT:
	//	nResult = OnPaint();
	//	break;
	case SBM_GETSCROLLINFO:
		nResult = OnGetScrollInfo(wParam, (LPSCROLLINFO)lParam);
		break;
	default:
		nResult = CDreamSkinWindow::DefWindowProc(message, wParam, lParam);
		break;
	}

	return nResult;

}

LRESULT CDreamSkinListCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (lpCreateStruct->style & LVS_OWNERDRAWFIXED)
		m_nOwnerDraw = 1;
	else
		m_nOwnerDraw = 0;

	m_nViewType = GetViewTypeByStyle(lpCreateStruct->style);

	return CDreamSkinWindow::DefWindowProc(WM_CREATE, 0, (LPARAM)lpCreateStruct);
}

LRESULT CDreamSkinListCtrl::OnDrawItem(UINT nCtrlID, LPDRAWITEMSTRUCT lpDrawItem, int nViewType)
{
	DrawItem(lpDrawItem->hDC, lpDrawItem->itemID, -1, nViewType);

	return TRUE;
}

LRESULT CDreamSkinListCtrl::OnEraseBkgnd(HDC hDC)
{
	RECT rcClient;
	::GetClientRect(m_hWnd, &rcClient);

	DrawBackground(hDC, rcClient);

	return 0;
}

LRESULT CDreamSkinListCtrl::OnLButtonDown(UINT nFlags, POINTS point)
{
	LRESULT lResult = CDreamSkinWindow::DefWindowProc(WM_LBUTTONDOWN, nFlags, MAKELPARAM(point.x, point.y));

	if (m_nViewType == LV_VIEW_DETAILS)
	{
	}

	return lResult;
}

LRESULT CDreamSkinListCtrl::OnMouseLeave()
{
	if(m_nCurHover >= 0)
	{
		int nIndex = m_nCurHover;
		m_nCurHover = -1;
		m_nCurHoverSub = -1;

		RECT rcItem;
		rcItem.left = LVIR_BOUNDS;
		::SendMessage(m_hWnd, LVM_GETITEMRECT, nIndex, (LPARAM)&rcItem);
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

	LVHITTESTINFO HitTest;
	memset(&HitTest, 0, sizeof(LVHITTESTINFO));
	HitTest.pt.x = point.x;
	HitTest.pt.y = point.y;
	HitTest.flags = LVHT_ONITEM | LVHT_EX_ONCONTENTS;

	int nHoverItem, nHoverSubItem;
	//if (::SendMessage(m_hWnd, LVM_SUBITEMHITTEST, 0, (LPARAM)&HitTest) >= 0)
	if (::SendMessage(m_hWnd, LVM_HITTEST, 0, (LPARAM)&HitTest) >= 0)
	{
		nHoverItem = HitTest.iItem;
		nHoverSubItem = HitTest.iSubItem;
	}
	else
	{
		nHoverItem = -1;
		nHoverSubItem = -1;
	}

	if (nHoverItem != m_nCurHover || nHoverSubItem != m_nCurHoverSub)
	{
		int nTemp = m_nCurHover;

		m_nCurHover = nHoverItem;
		m_nCurHoverSub = nHoverSubItem;
		
		RECT rcItem;
		if (m_nCurHover >= 0)
		{
			rcItem.left = LVIR_BOUNDS;
			::SendMessage(m_hWnd, LVM_GETITEMRECT, m_nCurHover, (LPARAM)&rcItem);
			::InvalidateRect(m_hWnd, &rcItem, FALSE);
		}

		if (nTemp >= 0 && nTemp != m_nCurHover)
		{
			rcItem.left = LVIR_BOUNDS;
			::SendMessage(m_hWnd, LVM_GETITEMRECT, nTemp, (LPARAM)&rcItem);
			::InvalidateRect(m_hWnd, &rcItem, FALSE);
		}
	}

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

LRESULT CDreamSkinListCtrl::OnNotify(int idCtrl, LPNMHDR pnmh)
{
	LRESULT lResult = 0;
	LPNMLVCUSTOMDRAW lplvcd;
	DRAWITEMSTRUCT DrawItemStruct;

	if (pnmh->hwndFrom == m_hWnd)
	{
		switch(pnmh->code)
		{
		case NM_CUSTOMDRAW:
			lplvcd = (LPNMLVCUSTOMDRAW)pnmh;
			switch(lplvcd->nmcd.dwDrawStage)
			{
			case CDDS_PREPAINT:
				lResult = CDRF_NOTIFYITEMDRAW;
				break;
			case CDDS_ITEMPREPAINT:
				if (m_nViewType < 0)
					m_nViewType = GetViewTypeByStyle(::GetWindowLong(m_hWnd, GWL_STYLE));

				if (m_nViewType == LV_VIEW_DETAILS)
				{
					lResult = CDRF_SKIPDEFAULT;
					DrawItemStruct.CtlID = idCtrl;
					DrawItemStruct.CtlType = ODT_LISTVIEW;
					DrawItemStruct.hDC = lplvcd->nmcd.hdc;
					DrawItemStruct.hwndItem = pnmh->hwndFrom;
					DrawItemStruct.itemAction = ODA_DRAWENTIRE;
					DrawItemStruct.itemData = lplvcd->nmcd.lItemlParam;
					DrawItemStruct.itemID = lplvcd->nmcd.dwItemSpec;
					DrawItemStruct.itemState = lplvcd->nmcd.uItemState;
					DrawItemStruct.rcItem = lplvcd->nmcd.rc;
					OnDrawItem(idCtrl, &DrawItemStruct, ::GetWindowLong(m_hWnd, GWL_STYLE) & LVS_TYPEMASK);
				}
				else
				{
					lResult = CDreamSkinWindow::DefWindowProc(WM_NOTIFY, idCtrl, (LPARAM)pnmh);
				}
				break;
			default:
				lResult = CDreamSkinWindow::DefWindowProc(WM_NOTIFY, idCtrl, (LPARAM)pnmh);
				break;
			}
			break;
		default:
			lResult = CDreamSkinWindow::DefWindowProc(WM_NOTIFY, idCtrl, (LPARAM)pnmh);
			break;
		}
	}
	else
	{
		lResult = CDreamSkinWindow::DefWindowProc(WM_NOTIFY, idCtrl, (LPARAM)pnmh);
	}

	return lResult;
}

LRESULT CDreamSkinListCtrl::OnPaint()
{
	DWORD dwStyle = ::GetWindowLong(m_hWnd, GWL_STYLE);
	if ((dwStyle & LVS_TYPEMASK) != LVS_REPORT)
	{
		//TODO: we need to handle
		int aa = dwStyle & LVS_TYPEMASK;
		return CDreamSkinWindow::DefWindowProc(WM_PAINT, 0, 0);
	}
	else
	{
		return CDreamSkinWindow::DefWindowProc(WM_PAINT, 0, 0);
	}
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

void CDreamSkinListCtrl::DrawItem(HDC hDC, int nItem, int nSubItem, int nViewType)
{
	HWND hWndHeader = (HWND)::SendMessage(m_hWnd, LVM_GETHEADER, 0, 0);
	int nColNum;
	if (hWndHeader)
		nColNum = ::SendMessage(hWndHeader, HDM_GETITEMCOUNT, 0, 0);
	else
		nColNum = 0;

	DWORD dwStyle = ::GetWindowLong(m_hWnd, GWL_STYLE);
	int nStatus = GetCurrentStatus(dwStyle);

	DWORD dwExStyle = ::SendMessage(m_hWnd, LVM_GETEXTENDEDLISTVIEWSTYLE, 0, 0);

	SKINITEM *pItem;

	LVITEM lvItem;
	RECT rcDraw;
	WCHAR buf[256];
	for (int i = 0; i < nColNum; i++)
	{
		lvItem.iItem = nItem;
		lvItem.iSubItem = i;
		lvItem.mask = LVIF_IMAGE | LVIF_STATE | LVIF_TEXT;
		lvItem.stateMask = (UINT)-1;
		lvItem.pszText = buf;
		lvItem.cchTextMax = 256;
		::SendMessage(m_hWnd, LVM_GETITEM, 0, (LPARAM)&lvItem);

		if (lvItem.state & LVIS_SELECTED)
		{
			if (nStatus == DRAWSTATUS_DISABLE)
				pItem = &m_pSkinListCtrl->skinItemDisableSelected;
			else if ((lvItem.iItem == m_nCurHover) && (dwExStyle & LVS_EX_FULLROWSELECT || lvItem.iSubItem == m_nCurHoverSub))
				pItem = &m_pSkinListCtrl->skinItemHoverSelected;
			else
				pItem = &m_pSkinListCtrl->skinItemNormalSelected;
		}
		else
		{
			if (nStatus == DRAWSTATUS_DISABLE)
				pItem = &m_pSkinListCtrl->skinItemDisableUnselected;
			else if ((lvItem.iItem == m_nCurHover) && (dwExStyle & LVS_EX_FULLROWSELECT || lvItem.iSubItem == m_nCurHoverSub))
				pItem = &m_pSkinListCtrl->skinItemHoverUnselected;
			else
				pItem = &m_pSkinListCtrl->skinItemNormalUnselected;
		}

		rcDraw.top = i;
		rcDraw.left = LVIR_BOUNDS;
		::SendMessage(m_hWnd, LVM_GETSUBITEMRECT, nItem, (LPARAM)&rcDraw);

		CDreamSkinWindow::DrawItem(hDC, pItem, rcDraw, lvItem.pszText);
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

int CDreamSkinListCtrl::GetViewTypeByStyle(DWORD dwStyle) const
{
	int nResult;
	switch (dwStyle & LVS_TYPEMASK)
	{
	case LVS_ICON:
		nResult = LV_VIEW_ICON;
		break;
	case LVS_REPORT:
		nResult = LV_VIEW_DETAILS;
		break;
	case LVS_SMALLICON:
		nResult = LV_VIEW_SMALLICON;
		break;
	case LVS_LIST:
		nResult = LV_VIEW_LIST;
		break;
	default:
		nResult = -1;
		break;
	}

	return nResult;
}
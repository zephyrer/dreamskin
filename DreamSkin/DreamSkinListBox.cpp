//DreamSkinListBox.cpp
#include <windows.h>

#include "WinGdiEx.h"
#include "DreamSkinMain.h"
#include "DreamSkinLoader.h"
#include "DreamSkinWindow.h"
#include "DreamSkinListBox.h"
#include "DreamSkinScrollBar.h"

//static member declaration
WNDPROC         CDreamSkinListBox::s_DefaultWindowProc = NULL;
SKINLISTBOX     CDreamSkinListBox::s_SkinListBox;

CDreamSkinListBox::CDreamSkinListBox(HWND hWnd, WNDPROC OrgWndProc)
	: CDreamSkinWindow(hWnd, OrgWndProc)
{
	m_pSkinListBox = &s_SkinListBox;
	m_nOwnerDraw = -1;
	m_nCurHover = -1;

	m_nSBLButtonDown = 0;
	m_nSBHover = 0;
}

CDreamSkinListBox::~CDreamSkinListBox()
{
	if (m_pSkinListBox != &s_SkinListBox)
		delete m_pSkinListBox;
}

void CDreamSkinListBox::Reload()
{
	if (m_hWnd && ::IsWindow(m_hWnd))
	{
		RECT rcClient;
		::GetClientRect(m_hWnd, &rcClient);

		m_bCreateRegion = TRUE;
		::InvalidateRect(m_hWnd, &rcClient, TRUE);
	}
}

BOOL CDreamSkinListBox::ApplySkin(CDreamSkinLoader *pLoader)
{
	BOOL bResult = TRUE;

	InitialClass();

	SKINLISTBOX SkinListBox;
	pLoader->GetSkinListBox(&SkinListBox);
	s_SkinListBox = SkinListBox;

	return bResult;
}

CDreamSkinListBox* CDreamSkinListBox::CreateInstance(HWND hWnd, WNDPROC OrgWndProc)
{
	CDreamSkinListBox *pSkinListBox = new CDreamSkinListBox(hWnd, OrgWndProc);
	return pSkinListBox;
}

LRESULT WINAPI CDreamSkinListBox::DefWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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

BOOL CDreamSkinListBox::InitialClass()
{
	//Get the default window proc for this class
	WNDCLASSW clsListBoxDefault;
	if(!GetClassInfoW(GetModuleHandle(NULL), DEFAULT_LISTBOX_CLASSNAME_W, &clsListBoxDefault))
	{
		//Get class info failed
		//TODO: add error handle code
		return FALSE;
	}
	s_DefaultWindowProc = clsListBoxDefault.lpfnWndProc;

	GetDefaultSkin(&s_SkinListBox);

	return TRUE;
}

void CDreamSkinListBox::DestroyClass()
{

}

BOOL CDreamSkinListBox::GetDefaultSkin(SKINLISTBOX *pSkinListBox)
{
	if (pSkinListBox)
	{
		GetDefaultBackground(&pSkinListBox->skinBkNormal, RGB(255, 255, 255));
		GetDefaultBackground(&pSkinListBox->skinBkDisable, RGB(236, 233, 216));           //COLOR_BTNFACE

		GetDefaultBorder(&pSkinListBox->skinLBorderNormal, RGB(113, 111, 100), 1);        //COLOR_3DDKSHADOW
		GetDefaultBorder(&pSkinListBox->skinRBorderNormal, RGB(255, 255, 255), 1);        //COLOR_3DHILIGHT
		GetDefaultBorder(&pSkinListBox->skinTBorderNormal, RGB(113, 111, 100), 1);
		GetDefaultBorder(&pSkinListBox->skinBBorderNormal, RGB(255, 255, 255), 1);

		GetDefaultBorder(&pSkinListBox->skinLBorderDisable, RGB(172, 168, 153), 1);       //COLOR_GRAYTEXT
		GetDefaultBorder(&pSkinListBox->skinRBorderDisable, RGB(172, 168, 153), 1);
		GetDefaultBorder(&pSkinListBox->skinTBorderDisable, RGB(172, 168, 153), 1);
		GetDefaultBorder(&pSkinListBox->skinBBorderDisable, RGB(172, 168, 153), 1);

		GetDefaultItem(&pSkinListBox->skinItemNormalSelected, RGB(0, 0, 128), RGB(255, 255, 255), RGB(0, 0, 0), 0);
		GetDefaultItem(&pSkinListBox->skinItemDisableSelected, RGB(128, 128, 128), RGB(255, 255, 255), RGB(0, 0, 0), 0);
		GetDefaultItem(&pSkinListBox->skinItemHoverSelected, RGB(0, 0, 128), RGB(255, 255, 255), RGB(0, 0, 0), 0);

		GetDefaultItem(&pSkinListBox->skinItemNormalUnselected, RGB(255, 255, 255), RGB(0, 0, 0), RGB(0, 0, 0), 0);
		GetDefaultItem(&pSkinListBox->skinItemDisableUnselected, RGB(236, 233, 216), RGB(0, 0, 0), RGB(0, 0, 0), 0);
		GetDefaultItem(&pSkinListBox->skinItemHoverUnselected, RGB(0, 0, 255), RGB(255, 255, 255), RGB(0, 0, 0), 0);
	}

	return TRUE;
}

LRESULT CDreamSkinListBox::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT nResult = ERROR_SUCCESS;


	switch(message)
	{
	case WM_COMPAREITEM:
		nResult = CDreamSkinWindow::DefWindowProc(message, wParam, lParam);
		break;
	case WM_CREATE:
		nResult = OnCreate((LPCREATESTRUCT)lParam);
		break;
	case WM_DRAWITEM:
		nResult = OnDrawItem((UINT)wParam, (LPDRAWITEMSTRUCT)lParam);
		break;
	case WM_ENABLE:
		nResult = CDreamSkinWindow::DefWindowProc(message, wParam, lParam);
		UpdateWindow();
		break;
	case WM_ERASEBKGND:
		nResult = OnEraseBkgnd((HDC)wParam);
		break;
	case WM_LBUTTONUP:
		nResult = CDreamSkinWindow::DefWindowProc(message, wParam, lParam);
		m_nSBLButtonDown = 0;
		break;
	case WM_MEASUREITEM:
		nResult = CDreamSkinWindow::DefWindowProc(message, wParam, lParam);
		break;
	case WM_MOUSELEAVE:
		nResult = OnMouseLeave();
		OnNcPaint(NULL);
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
	//case WM_VSCROLL:
	//case WM_PRINTCLIENT:
	//	nResult = CDreamSkinWindow::DefWindowProc(message, wParam, lParam);
	//	OnNcPaint(NULL);
	//	break;
	default:
		nResult = CDreamSkinWindow::DefWindowProc(message, wParam, lParam);
	}

	return nResult;

}

LRESULT CDreamSkinListBox::OnCreate(LPCREATESTRUCT lpCreateStruct)
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
		lpCreateStruct->style |= LBS_HASSTRINGS;
	}

	return CDreamSkinWindow::DefWindowProc(WM_CREATE, 0, (LPARAM)lpCreateStruct);
}

LRESULT CDreamSkinListBox::OnDrawItem(UINT nCtrlID, LPDRAWITEMSTRUCT lpDrawItem)
{
	if (m_nOwnerDraw)
	{
		HWND hWndParent = ::GetParent(m_hWnd);
		if (hWndParent)
			return CDreamSkinWindow::OrgWindowProc(hWndParent, WM_DRAWITEM, (WPARAM)nCtrlID, (LPARAM)lpDrawItem);
		else
			return CDreamSkinWindow::DefWindowProc(WM_DRAWITEM, (WPARAM)nCtrlID, (LPARAM)lpDrawItem);
	}
	else
	{
		SKINITEM *pItem;
		if (lpDrawItem->itemState & ODS_SELECTED)
		{
			if (lpDrawItem->itemState & ODS_DISABLED)
				pItem = &m_pSkinListBox->skinItemDisableSelected;
			else if (lpDrawItem->itemID == m_nCurHover)
				pItem = &m_pSkinListBox->skinItemHoverSelected;
			else
				pItem = &m_pSkinListBox->skinItemNormalSelected;
		}
		else
		{
			if (lpDrawItem->itemState & ODS_DISABLED)
				pItem = &m_pSkinListBox->skinItemDisableUnselected;
			else if (lpDrawItem->itemID == m_nCurHover)
				pItem = &m_pSkinListBox->skinItemHoverUnselected;
			else
				pItem = &m_pSkinListBox->skinItemNormalUnselected;
		}

		//Get the button title
		int nLen = ::SendMessageW(m_hWnd, LB_GETTEXTLEN, lpDrawItem->itemID, 0);
		WCHAR *wstrTitle = NULL;
		if (nLen > 0)
		{
			wstrTitle = new WCHAR[nLen + 1];
			memset(wstrTitle, 0, sizeof(WCHAR) * (nLen + 1));
			::SendMessageW(m_hWnd, LB_GETTEXT, lpDrawItem->itemID, (LPARAM)wstrTitle);
		}

		DrawItem(lpDrawItem->hDC, pItem, lpDrawItem->rcItem, wstrTitle);

		if (nLen > 0)
			delete wstrTitle;

		return 1;
	}
}

LRESULT CDreamSkinListBox::OnEraseBkgnd(HDC hDC)
{
	RECT rcClient;
	::GetClientRect(m_hWnd, &rcClient);

	DrawBackground(hDC, rcClient);

	return 0;
}

LRESULT CDreamSkinListBox::OnMouseLeave()
{
	if(m_nCurHover >= 0)
	{
		int nIndex = m_nCurHover;
		m_nCurHover = -1;

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

LRESULT CDreamSkinListBox::OnMouseMove(UINT nFlags, POINTS point)
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

	DWORD dwResult = (DWORD)::SendMessage(m_hWnd, LB_ITEMFROMPOINT, 0, MAKELPARAM(point.x, point.y));
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
	}

	return CDreamSkinWindow::DefWindowProc(WM_MOUSEMOVE, nFlags, MAKELPARAM(point.x, point.y));
}

LRESULT CDreamSkinListBox::OnNcHitTest(POINTS point)
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
				pLBorder = &m_pSkinListBox->skinLBorderDisable;
				pRBorder = &m_pSkinListBox->skinRBorderDisable;
				pTBorder = &m_pSkinListBox->skinTBorderDisable;
				pBBorder = &m_pSkinListBox->skinBBorderDisable;
				break;
			default:
				pLBorder = &m_pSkinListBox->skinLBorderNormal;
				pRBorder = &m_pSkinListBox->skinRBorderNormal;
				pTBorder = &m_pSkinListBox->skinTBorderNormal;
				pBBorder = &m_pSkinListBox->skinBBorderNormal;
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

LRESULT CDreamSkinListBox::OnNcLButtonDbClick(UINT nHitTest, POINTS point)
{
	LRESULT nResult = 0;

	if (nHitTest == HTVSCROLL || nHitTest == HTHSCROLL)
	{
		int fnBar = nHitTest == HTVSCROLL ? SB_VERT : SB_HORZ;
		switch(CDreamSkinScrollBar::ScrollBarHitTest(m_hWnd, fnBar, point))
		{
		case SBHT_THUMB:
			m_nSBLButtonDown = fnBar << 16 | SBHT_THUMB;
			CDreamSkinScrollBar::TrackScrollBar(m_hWnd, fnBar, SBHT_THUMB, &m_nSBLButtonDown);
			break;
		case SBHT_REGION_UP:
			m_nSBLButtonDown = fnBar << 16 | SBHT_REGION_UP;
			CDreamSkinScrollBar::TrackScrollBar(m_hWnd, fnBar, SBHT_REGION_UP, &m_nSBLButtonDown);
			break;
		case SBHT_REGION_DOWN:
			m_nSBLButtonDown = fnBar << 16 | SBHT_REGION_DOWN;
			CDreamSkinScrollBar::TrackScrollBar(m_hWnd, fnBar, SBHT_REGION_DOWN, &m_nSBLButtonDown);
			break;
		case SBHT_ARROW_TOP:
			m_nSBLButtonDown = fnBar << 16 | SBHT_ARROW_TOP;
			CDreamSkinScrollBar::TrackScrollBar(m_hWnd, fnBar, SBHT_ARROW_TOP, &m_nSBLButtonDown);
			break;
		case SBHT_ARROW_BOTTOM:
			m_nSBLButtonDown = fnBar << 16 | SBHT_ARROW_BOTTOM;
			CDreamSkinScrollBar::TrackScrollBar(m_hWnd, fnBar, SBHT_ARROW_BOTTOM, &m_nSBLButtonDown);
			break;
		default:
			m_nSBLButtonDown = 0;
			nResult = CDreamSkinWindow::DefWindowProc(WM_NCLBUTTONDBLCLK, nHitTest, MAKELPARAM(point.x, point.y));
			OnNcPaint(NULL);
			break;
		}
	}
	else
	{
		m_nSBLButtonDown = 0;
		nResult = CDreamSkinWindow::DefWindowProc(WM_NCLBUTTONDBLCLK, nHitTest, MAKELPARAM(point.x, point.y));
	}

	return nResult;
}

LRESULT CDreamSkinListBox::OnNcLButtonDown(UINT nHitTest, POINTS point)
{
	LRESULT nResult = 0;

	if (nHitTest == HTVSCROLL || nHitTest == HTHSCROLL)
	{
		int fnBar = nHitTest == HTVSCROLL ? SB_VERT : SB_HORZ;
		switch(CDreamSkinScrollBar::ScrollBarHitTest(m_hWnd, fnBar, point))
		{
		case SBHT_THUMB:
			m_nSBLButtonDown = fnBar << 16 | SBHT_THUMB;
			CDreamSkinScrollBar::TrackScrollBar(m_hWnd, fnBar, SBHT_THUMB, &m_nSBLButtonDown);
			break;
		case SBHT_REGION_UP:
			m_nSBLButtonDown = fnBar << 16 | SBHT_REGION_UP;
			CDreamSkinScrollBar::TrackScrollBar(m_hWnd, fnBar, SBHT_REGION_UP, &m_nSBLButtonDown);
			break;
		case SBHT_REGION_DOWN:
			m_nSBLButtonDown = fnBar << 16 | SBHT_REGION_DOWN;
			CDreamSkinScrollBar::TrackScrollBar(m_hWnd, fnBar, SBHT_REGION_DOWN, &m_nSBLButtonDown);
			break;
		case SBHT_ARROW_TOP:
			m_nSBLButtonDown = fnBar << 16 | SBHT_ARROW_TOP;
			CDreamSkinScrollBar::TrackScrollBar(m_hWnd, fnBar, SBHT_ARROW_TOP, &m_nSBLButtonDown);
			break;
		case SBHT_ARROW_BOTTOM:
			m_nSBLButtonDown = fnBar << 16 | SBHT_ARROW_BOTTOM;
			CDreamSkinScrollBar::TrackScrollBar(m_hWnd, fnBar, SBHT_ARROW_BOTTOM, &m_nSBLButtonDown);
			break;
		default:
			m_nSBLButtonDown = 0;
			nResult = CDreamSkinWindow::DefWindowProc(WM_NCLBUTTONDOWN, nHitTest, MAKELPARAM(point.x, point.y));
			OnNcPaint(NULL);
			break;
		}
	}
	else
	{
		m_nSBLButtonDown = 0;
		nResult = CDreamSkinWindow::DefWindowProc(WM_NCLBUTTONDOWN, nHitTest, MAKELPARAM(point.x, point.y));
	}

	return nResult;
}

LRESULT CDreamSkinListBox::OnNcMouseLeave()
{
	if (m_nSBHover != 0)
	{
		m_nSBHover = 0;
		OnNcPaint(NULL);
	}

	return 0;
}

LRESULT CDreamSkinListBox::OnNcMouseMove(UINT nHitTest, POINTS point)
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

LRESULT CDreamSkinListBox::OnNcPaint(HRGN hRGN)
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
		CDreamSkinScrollBar::DrawWindowScrollBar(m_hWnd, hWindowDC, m_nSBLButtonDown, m_nSBHover);

	::ReleaseDC(m_hWnd, hWindowDC);

	return 0;
}

void CDreamSkinListBox::DrawBackground(HDC hDC, RECT rcClient)
{
	DWORD dwStyle = ::GetWindowLong(m_hWnd, GWL_STYLE);
	HBRUSH hBrush;

	int nStatus = GetCurrentStatus(dwStyle);
	switch(nStatus)
	{
	case DRAWSTATUS_DISABLE:
		hBrush = ::CreateSolidBrush(m_pSkinListBox->skinBkDisable.clrDraw.clrStart);
		break;
	default:
		hBrush = ::CreateSolidBrush(m_pSkinListBox->skinBkNormal.clrDraw.clrStart);
		break;
	}
	
	::FillRect(hDC, &rcClient, hBrush);
	::DeleteObject(hBrush);
}

void CDreamSkinListBox::DrawBackground(HDC hDC, SKINBACKGROUND* pBackGround, RECT rcDraw)
{
	if (pBackGround->nDrawType == DRAWTYPE_STRETCHBITMAP && pBackGround->imgDraw.hImage)
		::DrawStretchBitmap(hDC, rcDraw.left, rcDraw.top, rcDraw.right - rcDraw.left, rcDraw.bottom - rcDraw.top, pBackGround->imgDraw.hImage, pBackGround->imgDraw.x, pBackGround->imgDraw.y, pBackGround->imgDraw.nWidth, pBackGround->imgDraw.nHeight);
	else
		::FillSolidRect(hDC, &rcDraw, pBackGround->clrDraw.clrStart);
}

void CDreamSkinListBox::DrawBorder(HDC hDC, RECT rcWindow)
{
	RECT rcDraw;
	SKINBORDER *pLBorder, *pRBorder, *pTBorder, *pBBorder;
	DWORD dwStyle = ::GetWindowLong(m_hWnd, GWL_STYLE);

	int nStatus = GetCurrentStatus(dwStyle);
	switch(nStatus)
	{
	case DRAWSTATUS_DISABLE:
		pLBorder = &m_pSkinListBox->skinLBorderDisable;
		pRBorder = &m_pSkinListBox->skinRBorderDisable;
		pTBorder = &m_pSkinListBox->skinTBorderDisable;
		pBBorder = &m_pSkinListBox->skinBBorderDisable;
		break;
	default:
		pLBorder = &m_pSkinListBox->skinLBorderNormal;
		pRBorder = &m_pSkinListBox->skinRBorderNormal;
		pTBorder = &m_pSkinListBox->skinTBorderNormal;
		pBBorder = &m_pSkinListBox->skinBBorderNormal;
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

/*void CDreamSkinListBox::DrawBorder(HDC hDC, SKINBORDER *pLBorder, SKINBORDER *pRBorder, SKINBORDER *pTBorder, SKINBORDER *pBBorder, RECT rcDraw)
{
	RECT rcBorder;

	//top border
	if (pTBorder->nWidth > 0)
	{
		rcBorder.left = rcDraw.left;
		rcBorder.top = rcDraw.top;
		rcBorder.right = rcDraw.right;
		rcBorder.bottom = rcBorder.top + pTBorder->nWidth;
		if (pTBorder->nDrawType == DRAWTYPE_STRETCHBITMAP && pTBorder->imgDraw.hImage)
			::DrawStretchBitmapEx(hDC, rcBorder.left, rcBorder.top, rcBorder.right - rcBorder.left, rcBorder.bottom - rcBorder.top, pTBorder->imgDraw.hImage, pTBorder->imgDraw.x, pTBorder->imgDraw.y, pTBorder->imgDraw.nWidth, pTBorder->imgDraw.nHeight, pTBorder->nStart, pTBorder->nEnd, 0);
		else
			::FillSolidRect(hDC, &rcBorder, pTBorder->clrDraw.clrStart);
	}

	//left border
	if (pLBorder->nWidth > 0)
	{
		rcBorder.left = rcDraw.left;
		rcBorder.top = rcDraw.top + pTBorder->nWidth;
		rcBorder.right = rcBorder.left + pLBorder->nWidth;
		rcBorder.bottom = rcDraw.bottom - pBBorder->nWidth;

		if (pLBorder->nDrawType == DRAWTYPE_STRETCHBITMAP && pLBorder->imgDraw.hImage)
			::DrawStretchBitmapEx(hDC, rcBorder.left, rcBorder.top, rcBorder.right - rcBorder.left, rcBorder.bottom - rcBorder.top, pLBorder->imgDraw.hImage, pLBorder->imgDraw.x, pLBorder->imgDraw.y, pLBorder->imgDraw.nWidth, pLBorder->imgDraw.nHeight, pLBorder->nStart, pLBorder->nEnd, 1);
		else
			::FillSolidRect(hDC, &rcBorder, pLBorder->clrDraw.clrStart);
	}

	//right border
	if (pRBorder->nWidth > 0)
	{
		rcBorder.left = rcDraw.right - pRBorder->nWidth;
		rcBorder.top = rcDraw.top + pTBorder->nWidth;
		rcBorder.right = rcDraw.right;
		rcBorder.bottom = rcDraw.bottom - pBBorder->nWidth;

		if (pRBorder->nDrawType == DRAWTYPE_STRETCHBITMAP && pRBorder->imgDraw.hImage)
			::DrawStretchBitmapEx(hDC, rcBorder.left, rcBorder.top, rcBorder.right - rcBorder.left, rcBorder.bottom - rcBorder.top, pRBorder->imgDraw.hImage, pRBorder->imgDraw.x, pRBorder->imgDraw.y, pRBorder->imgDraw.nWidth, pRBorder->imgDraw.nHeight, pRBorder->nStart, pRBorder->nEnd, 1);
		else
			::FillSolidRect(hDC, &rcBorder, pRBorder->clrDraw.clrStart);
	}

	//bottom border
	if (pBBorder->nWidth > 0)
	{
		rcBorder.left = rcDraw.left;
		rcBorder.top = rcDraw.bottom - pBBorder->nWidth;
		rcBorder.right = rcDraw.right;
		rcBorder.bottom = rcDraw.bottom;
		if (pBBorder->nDrawType == DRAWTYPE_STRETCHBITMAP && pBBorder->imgDraw.hImage)
			::DrawStretchBitmapEx(hDC, rcBorder.left, rcBorder.top, rcBorder.right - rcBorder.left, rcBorder.bottom - rcBorder.top, pBBorder->imgDraw.hImage, pBBorder->imgDraw.x, pBBorder->imgDraw.y, pBBorder->imgDraw.nWidth, pBBorder->imgDraw.nHeight, pBBorder->nStart, pBBorder->nEnd, 0);
		else
			::FillSolidRect(hDC, &rcBorder, pBBorder->clrDraw.clrStart);
	}
}*/

void CDreamSkinListBox::DrawTitle(HDC hDC, SKINTEXT *pText, RECT rcDraw, WCHAR *wstrTitle)
{
	LOGFONTW fnText;
	HFONT hFnText, hFnOld = NULL;
	int nLen = wcslen(wstrTitle);

	if (nLen > 0)
	{
		//Create font
		
		memset(&fnText, 0, sizeof(LOGFONTW));
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

		//we need to draw the shadow of text
		if(pText->bDrawShadow)
		{
			::OffsetRect(&rcDraw, 1, 1);
			::SetTextColor(hDC, pText->clrShadow);
			::DrawTextW(hDC, wstrTitle, nLen, &rcDraw,  DT_WORDBREAK | DT_LEFT);
			::OffsetRect(&rcDraw, -1, -1);
		}
		::SetTextColor(hDC, pText->clrDraw);
		::DrawTextW(hDC, wstrTitle, nLen, &rcDraw,  DT_WORDBREAK | DT_LEFT);

		::SelectObject(hDC, hFnOld);
		::DeleteObject(hFnText);
	}
}

void CDreamSkinListBox::DrawItem(HDC hDC, SKINITEM *pItem, RECT rcItem, WCHAR *wstrTitle)
{
	RECT rcClient = GetItemRectClient(pItem, rcItem);
	DrawBackground(hDC, &pItem->skinBk, rcItem);

	CDreamSkinWindow::DrawBorder(hDC, &pItem->skinLBorder, &pItem->skinRBorder, &pItem->skinTBorder, &pItem->skinBBorder, rcItem);

	if (wstrTitle)
		DrawTitle(hDC, &pItem->skinTxt, rcClient, wstrTitle);
}

int CDreamSkinListBox::GetCurrentStatus(DWORD dwStyle) const
{
	int nStatus;

	if(dwStyle & WS_DISABLED)
		nStatus = DRAWSTATUS_DISABLE;       //Disabled
	else
		nStatus = DRAWSTATUS_NORMAL;        //Normal

	return nStatus;
}
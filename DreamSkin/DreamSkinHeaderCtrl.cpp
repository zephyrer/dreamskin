//DreamSkinHeaderCtrl.cpp
#include <windows.h>
#include <commctrl.h>

#include "WinGdiEx.h"
#include "DreamSkinMain.h"
#include "DreamSkinLoader.h"
#include "DreamSkinWindow.h"
#include "DreamSkinHeaderCtrl.h"

//static member declaration
WNDPROC         CDreamSkinHeaderCtrl::s_DefaultWindowProc = NULL;
SKINHEADERCTRL  CDreamSkinHeaderCtrl::s_SkinHeaderCtrl;

CDreamSkinHeaderCtrl::CDreamSkinHeaderCtrl(HWND hWnd, WNDPROC OrgWndProc)
	: CDreamSkinWindow(hWnd, OrgWndProc)
{
	m_pSkinHeaderCtrl = &s_SkinHeaderCtrl;
	m_nCurHover = -1;
	m_nCurPress = -1;
}

CDreamSkinHeaderCtrl::~CDreamSkinHeaderCtrl()
{
	if (m_pSkinHeaderCtrl != &s_SkinHeaderCtrl)
		delete m_pSkinHeaderCtrl;
}

void CDreamSkinHeaderCtrl::Reload()
{
	if (m_hWnd && ::IsWindow(m_hWnd))
	{
		RECT rcClient;
		::GetClientRect(m_hWnd, &rcClient);

		m_bCreateRegion = TRUE;
		::InvalidateRect(m_hWnd, &rcClient, TRUE);
	}
}

BOOL CDreamSkinHeaderCtrl::ApplySkin(CDreamSkinLoader *pLoader)
{
	BOOL bResult = TRUE;

	InitialClass();

	SKINHEADERCTRL SkinHeaderCtrl;
	pLoader->GetSkinHeaderCtrl(&SkinHeaderCtrl);
	s_SkinHeaderCtrl = SkinHeaderCtrl;

	return bResult;
}

CDreamSkinHeaderCtrl* CDreamSkinHeaderCtrl::CreateInstance(HWND hWnd, WNDPROC OrgWndProc)
{
	CDreamSkinHeaderCtrl *pSkinHeaderCtrl = new CDreamSkinHeaderCtrl(hWnd, OrgWndProc);
	return pSkinHeaderCtrl;
}

LRESULT WINAPI CDreamSkinHeaderCtrl::DefWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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

BOOL CDreamSkinHeaderCtrl::InitialClass()
{
	//Get the default window proc for this class
	WNDCLASSW clsHeaderCtrlDefault;
	if(!GetClassInfoW(GetModuleHandle(NULL), DEFAULT_HEADERCTRL_CLASSNAME_W, &clsHeaderCtrlDefault))
	{
		//Get class info failed
		//TODO: add error handle code
		return FALSE;
	}
	s_DefaultWindowProc = clsHeaderCtrlDefault.lpfnWndProc;

	GetDefaultSkin(&s_SkinHeaderCtrl);

	return TRUE;
}

void CDreamSkinHeaderCtrl::DestroyClass()
{

}

BOOL CDreamSkinHeaderCtrl::GetDefaultSkin(SKINHEADERCTRL *pSkinHeaderCtrl)
{
	if (pSkinHeaderCtrl)
	{
		GetDefaultItem(&pSkinHeaderCtrl->skinItemNormal, RGB(236, 233, 216), RGB(0, 0, 0), RGB(172, 168, 153), 1);
		GetDefaultItem(&pSkinHeaderCtrl->skinItemDisabled, RGB(236, 233, 216), RGB(0, 0, 0), RGB(172, 168, 153), 1);
		GetDefaultItem(&pSkinHeaderCtrl->skinItemHover, RGB(241, 239, 226), RGB(0, 0, 0), RGB(113, 111, 100), 1);
		GetDefaultItem(&pSkinHeaderCtrl->skinItemPress, RGB(113, 111, 100), RGB(255, 255, 255), RGB(113, 111, 100), 1);

		pSkinHeaderCtrl->skinItemNormal.skinTBorder.clrDraw.clrStart = RGB(236, 233, 216);
		pSkinHeaderCtrl->skinItemNormal.skinLBorder.clrDraw.clrStart = RGB(236, 233, 216);
		pSkinHeaderCtrl->skinItemDisabled.skinTBorder.clrDraw.clrStart = RGB(236, 233, 216);
		pSkinHeaderCtrl->skinItemDisabled.skinLBorder.clrDraw.clrStart = RGB(236, 233, 216);
		pSkinHeaderCtrl->skinItemHover.skinTBorder.clrDraw.clrStart = RGB(236, 233, 216);
		pSkinHeaderCtrl->skinItemHover.skinLBorder.clrDraw.clrStart = RGB(236, 233, 216);
	}

	return TRUE;
}

LRESULT CDreamSkinHeaderCtrl::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT nResult = ERROR_SUCCESS;

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
	case WM_PAINT:
		nResult = OnPaint();
		break;
	default:
		nResult = CDreamSkinWindow::DefWindowProc(message, wParam, lParam);
	}

	return nResult;

}

LRESULT CDreamSkinHeaderCtrl::OnLButtonDown(UINT nFlags, POINTS point)
{
	HDHITTESTINFO HitTestInfo;
	memset(&HitTestInfo, 0, sizeof(HDHITTESTINFO));
	HitTestInfo.pt.x = point.x;
	HitTestInfo.pt.y = point.y;
	int nIndex = (DWORD)::SendMessage(m_hWnd, HDM_HITTEST, 0, (LPARAM)&HitTestInfo);

	if(nIndex != m_nCurPress && !(HitTestInfo.flags & HHT_ONDIVIDER))
	{
		m_nCurPress = nIndex;
		
		RECT rcClient;
		::GetClientRect(m_hWnd, &rcClient);
		::InvalidateRect(m_hWnd, &rcClient, TRUE);
	}

	return CDreamSkinWindow::DefWindowProc(WM_LBUTTONDOWN, nFlags, MAKELPARAM(point.x, point.y));
}

LRESULT CDreamSkinHeaderCtrl::OnLButtonUp(UINT nFlags, POINTS point)
{
	if(m_nCurPress >= 0)
	{
		m_nCurPress = -1;

		RECT rcClient;
		::GetClientRect(m_hWnd, &rcClient);
		::InvalidateRect(m_hWnd, &rcClient, TRUE);
	}

	return CDreamSkinWindow::DefWindowProc(WM_LBUTTONUP, nFlags, MAKELPARAM(point.x, point.y));
}

LRESULT CDreamSkinHeaderCtrl::OnMouseLeave()
{
	if(m_nCurHover >= 0)
	{
		m_nCurHover = -1;

		RECT rcClient;
		::GetClientRect(m_hWnd, &rcClient);
		::InvalidateRect(m_hWnd, &rcClient, TRUE);
	}

	return 0;
}

LRESULT CDreamSkinHeaderCtrl::OnMouseMove(UINT nFlags, POINTS point)
{
	TRACKMOUSEEVENT EventTrack;
	EventTrack.cbSize = sizeof(TRACKMOUSEEVENT);
	EventTrack.dwFlags = TME_LEAVE;
	EventTrack.hwndTrack = m_hWnd;
	EventTrack.dwHoverTime = 0;

	TrackMouseEvent(&EventTrack);

	HDHITTESTINFO HitTestInfo;
	memset(&HitTestInfo, 0, sizeof(HDHITTESTINFO));
	HitTestInfo.pt.x = point.x;
	HitTestInfo.pt.y = point.y;
	int nIndex = (DWORD)::SendMessage(m_hWnd, HDM_HITTEST, 0, (LPARAM)&HitTestInfo);

	if(nIndex != m_nCurHover)
	{
		m_nCurHover = nIndex;
		
		RECT rcClient;
		::GetClientRect(m_hWnd, &rcClient);
		::InvalidateRect(m_hWnd, &rcClient, TRUE);
	}

	return CDreamSkinWindow::DefWindowProc(WM_MOUSEMOVE, nFlags, MAKELPARAM(point.x, point.y));
}

LRESULT CDreamSkinHeaderCtrl::OnPaint()
{
	PAINTSTRUCT ps;
	HDC hPaintDC = ::BeginPaint(m_hWnd, &ps);
	::SetBkMode(hPaintDC, TRANSPARENT);

	RECT rcClient, rcLast;
	::GetClientRect(m_hWnd, &rcClient);

	DWORD dwStyle = ::GetWindowLong(m_hWnd, GWL_STYLE);
	int nStatus = GetCurrentStatus(dwStyle);

	int nItemCount = ::SendMessage(m_hWnd, HDM_GETITEMCOUNT, 0, 0);

	HDITEM HdItem;
	WCHAR wstrItemText[256];
	RECT rcItem;
	SKINITEM *pItem;

	rcLast = rcClient;
	for (int i = 0; i < nItemCount; i++)
	{
		HdItem.mask = HDI_TEXT | HDI_FORMAT;
		HdItem.pszText = wstrItemText;
		HdItem.cchTextMax = 255;

		::SendMessage(m_hWnd, HDM_GETITEM, i, (LPARAM)&HdItem);
		::SendMessage(m_hWnd, HDM_GETITEMRECT, i, (LPARAM)&rcItem);

		if (nStatus == DRAWSTATUS_DISABLE)
			pItem = &m_pSkinHeaderCtrl->skinItemDisabled;
		else
			if (m_nCurHover == i)
				if (m_nCurPress == i)
					pItem = &m_pSkinHeaderCtrl->skinItemPress;
				else
					pItem = &m_pSkinHeaderCtrl->skinItemHover;
			else
				pItem = &m_pSkinHeaderCtrl->skinItemNormal;
		DrawItem(hPaintDC, pItem, rcItem, HdItem.pszText);

		if (rcItem.right > rcLast.left)
			rcLast.left = rcItem.right;
	}

	if (rcLast.left < rcLast.right)
	{
		if (nStatus == DRAWSTATUS_DISABLE)
			pItem = &m_pSkinHeaderCtrl->skinItemDisabled;
		else
			pItem = &m_pSkinHeaderCtrl->skinItemNormal;
		DrawItem(hPaintDC, pItem, rcLast, NULL);
	}

	::EndPaint(m_hWnd, &ps);

	return 0;
}

int CDreamSkinHeaderCtrl::GetCurrentStatus(DWORD dwStyle) const
{
	int nStatus;

	if(dwStyle & WS_DISABLED)
		nStatus = DRAWSTATUS_DISABLE;       //Disabled
	else
	{
		HWND hWndParent = ::GetParent(m_hWnd);
		if (hWndParent)
		{
			DWORD dwParentStyle = ::GetWindowLong(hWndParent, GWL_STYLE);
			if (dwParentStyle & WS_DISABLED)
				nStatus = DRAWSTATUS_DISABLE;       //Disabled
			else
				nStatus = DRAWSTATUS_NORMAL;        //Normal
		}
		else
			nStatus = DRAWSTATUS_NORMAL;        //Normal
	}

	return nStatus;
}
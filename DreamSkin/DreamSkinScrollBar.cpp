//DreamSkinScrollBar.cpp
#include <windows.h>

#include "WinGdiEx.h"
#include "DreamSkinMain.h"
#include "DreamSkinLoader.h"
#include "DreamSkinWindow.h"
#include "DreamSkinScrollBar.h"

#define DEFAULT_SCROLLBAR_BUTTON_XY           17

//static member declaration
WNDPROC         CDreamSkinScrollBar::s_DefaultWindowProc = NULL;
SKINSCROLLBAR   CDreamSkinScrollBar::s_SkinScrollBar;

typedef int (WINAPI *SETSCROLLINFO)(HWND hWnd, int fnBar, LPCSCROLLINFO lpsi, BOOL fRedraw);
typedef int (WINAPI *SETSCROLLPOS)(HWND hWnd, int nBar, int nPos, BOOL bRedraw);
typedef BOOL (WINAPI *GETSCROLLINFO)(HWND hWnd, int fnBar, LPSCROLLINFO lpsi);

int WINAPI CDreamSkinScrollBar::SetScrollInfo(HWND hWnd, int fnBar, LPCSCROLLINFO lpsi, BOOL fRedraw)
{
	int nResult = 0;
	if (theSkinMain.m_HookSetScrollInfo.OrgAddr)
	{
		//send this message only when we hooked this window
		CDreamSkinWindow *pSkinWindow = theSkinMain.GetHookedWindow(hWnd);
		if (pSkinWindow)
		{
			nResult = ((SETSCROLLINFO)theSkinMain.m_HookSetScrollInfo.OrgAddr)(hWnd, fnBar, lpsi, FALSE);
			if (fRedraw)
			{
				if (pSkinWindow->GetType() == DREAMSKIN_SCROLLBAR)
				{
					RECT rcClient;
					::GetClientRect(hWnd, &rcClient);
					::InvalidateRect(hWnd, &rcClient, FALSE);
				}
				else
				{
					::SendMessage(hWnd, WM_NCPAINT, 1, 0);
				}
			}
		}
		else
		{
			nResult = ((SETSCROLLINFO)theSkinMain.m_HookSetScrollInfo.OrgAddr)(hWnd, fnBar, lpsi, fRedraw);
		}
	}

	return nResult;
}

int WINAPI CDreamSkinScrollBar::SetScrollPos(HWND hWnd, int nBar, int nPos, BOOL bRedraw)
{
	int nResult = 0;
	if (theSkinMain.m_HookSetScrollPos.OrgAddr)
	{
		//send this message only when we hooked this window
		CDreamSkinWindow *pSkinWindow = theSkinMain.GetHookedWindow(hWnd);
		if (pSkinWindow)
		{
			nResult = ((SETSCROLLPOS)theSkinMain.m_HookSetScrollPos.OrgAddr)(hWnd, nBar, nPos, FALSE);
			if (bRedraw)
			{
				if (pSkinWindow->GetType() == DREAMSKIN_SCROLLBAR)
				{
					RECT rcClient;
					::GetClientRect(hWnd, &rcClient);
					::InvalidateRect(hWnd, &rcClient, FALSE);
				}
				else
				{
					::SendMessage(hWnd, WM_NCPAINT, 1, 0);
				}
			}
		}
		else
		{
			nResult = ((SETSCROLLPOS)theSkinMain.m_HookSetScrollPos.OrgAddr)(hWnd, nBar, nPos, bRedraw);
		}
	}

	return nResult;
}

BOOL WINAPI CDreamSkinScrollBar::GetScrollInfo(HWND hWnd, int fnBar, LPSCROLLINFO lpsi)
{
	int nResult = 0;
	if (theSkinMain.m_HookGetScrollInfo.OrgAddr)
	{
		nResult = ((GETSCROLLINFO)theSkinMain.m_HookGetScrollInfo.OrgAddr)(hWnd, fnBar, lpsi);
		
		//send this message only when we hooked this window
		if (theSkinMain.GetHookedWindow(hWnd))
		{
			if (lpsi->fMask & SIF_TRACKPOS)
			{
				SCROLLINFO ScrollInfo;
				ScrollInfo.cbSize = sizeof(SCROLLINFO);
				ScrollInfo.fMask = SIF_TRACKPOS;
				if (::SendMessage(hWnd, SBM_GETSCROLLINFO, (WPARAM)fnBar, (LPARAM)&ScrollInfo))
					lpsi->nTrackPos = ScrollInfo.nTrackPos;
			}
		}
	}

	return nResult;
}

CDreamSkinScrollBar::CDreamSkinScrollBar(HWND hWnd, WNDPROC OrgWndProc)
	: CDreamSkinWindow(hWnd, OrgWndProc)
{
	m_pSkinScrollBar = &s_SkinScrollBar;

	m_ScrollBarTrackInfo.nSBLButtonDown = 0;
	m_ScrollBarTrackInfo.bIsTracking = FALSE;
	m_ScrollBarTrackInfo.nTrackPos = 0;

	m_nSBHover = 0;
}

CDreamSkinScrollBar::~CDreamSkinScrollBar()
{
	if (m_pSkinScrollBar != &s_SkinScrollBar)
		delete m_pSkinScrollBar;
}

void CDreamSkinScrollBar::Reload()
{
	if (m_hWnd && ::IsWindow(m_hWnd))
	{
		RECT rcClient;
		::GetClientRect(m_hWnd, &rcClient);

		m_bCreateRegion = TRUE;
		::InvalidateRect(m_hWnd, &rcClient, TRUE);
	}
}

BOOL CDreamSkinScrollBar::ApplySkin(CDreamSkinLoader *pLoader)
{
	BOOL bResult = TRUE;

	InitialClass();

	SKINSCROLLBAR SkinScrollBar;
	pLoader->GetSkinScrollBar(&SkinScrollBar);
	s_SkinScrollBar = SkinScrollBar;

	return bResult;
}

CDreamSkinScrollBar* CDreamSkinScrollBar::CreateInstance(HWND hWnd, WNDPROC OrgWndProc)
{
	CDreamSkinScrollBar *pSkinScrollBar = new CDreamSkinScrollBar(hWnd, OrgWndProc);
	return pSkinScrollBar;
}

LRESULT WINAPI CDreamSkinScrollBar::DefWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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

BOOL CDreamSkinScrollBar::InitialClass()
{
	//Get the default window proc for this class
	WNDCLASSW clsScrollBarDefault;
	if(!GetClassInfoW(GetModuleHandle(NULL), DEFAULT_SCROLLBAR_CLASSNAME_W, &clsScrollBarDefault))
	{
		//Get class info failed
		//TODO: add error handle code
		return FALSE;
	}
	s_DefaultWindowProc = clsScrollBarDefault.lpfnWndProc;

	GetDefaultSkin(&s_SkinScrollBar);

	return TRUE;
}

void CDreamSkinScrollBar::DestroyClass()
{

}

BOOL CDreamSkinScrollBar::GetDefaultSkin(SKINSCROLLBAR *pSkinScrollBar)
{
	if (pSkinScrollBar)
	{
		GetDefaultBackground(&pSkinScrollBar->skinBkNormal, RGB(236, 233, 216));
		GetDefaultBackground(&pSkinScrollBar->skinBkDisable, RGB(236, 233, 216));

		GetDefaultBackground(&pSkinScrollBar->skinHBkNormal, RGB(255, 255, 255));
		GetDefaultBackground(&pSkinScrollBar->skinHBkDisable, RGB(236, 233, 216));

		GetDefaultBackground(&pSkinScrollBar->skinVBkNormal, RGB(255, 255, 255));
		GetDefaultBackground(&pSkinScrollBar->skinVBkDisable, RGB(236, 233, 216));

		GetDefaultSysButton(&pSkinScrollBar->skinBtnLeftNormal, RGB(236, 233, 216), RGB(0, 0, 0) , RGB(172, 168, 153));            //COLOR_BTNFACE
		GetDefaultSysButton(&pSkinScrollBar->skinBtnRightNormal, RGB(236, 233, 216), RGB(0, 0, 0), RGB(172, 168, 153));           //COLOR_BTNFACE
		GetDefaultSysButton(&pSkinScrollBar->skinBtnTopNormal, RGB(236, 233, 216), RGB(0, 0, 0), RGB(172, 168, 153));             //COLOR_BTNFACE
		GetDefaultSysButton(&pSkinScrollBar->skinBtnBottomNormal, RGB(236, 233, 216), RGB(0, 0, 0), RGB(172, 168, 153));          //COLOR_BTNFACE

		GetDefaultSysButton(&pSkinScrollBar->skinBtnLeftHover, RGB(241, 239, 226), RGB(0, 0, 0), RGB(172, 168, 153));             //COLOR_3DLIGHT
		GetDefaultSysButton(&pSkinScrollBar->skinBtnRightHover, RGB(241, 239, 226), RGB(0, 0, 0), RGB(172, 168, 153));
		GetDefaultSysButton(&pSkinScrollBar->skinBtnTopHover, RGB(241, 239, 226), RGB(0, 0, 0), RGB(172, 168, 153));
		GetDefaultSysButton(&pSkinScrollBar->skinBtnBottomHover, RGB(241, 239, 226), RGB(0, 0, 0), RGB(172, 168, 153));

		GetDefaultSysButton(&pSkinScrollBar->skinBtnLeftPress, RGB(241, 239, 226), RGB(0, 0, 0), RGB(172, 168, 153));
		GetDefaultSysButton(&pSkinScrollBar->skinBtnRightPress, RGB(241, 239, 226), RGB(0, 0, 0), RGB(172, 168, 153));
		GetDefaultSysButton(&pSkinScrollBar->skinBtnTopPress, RGB(241, 239, 226), RGB(0, 0, 0), RGB(172, 168, 153));
		GetDefaultSysButton(&pSkinScrollBar->skinBtnBottomPress, RGB(241, 239, 226), RGB(0, 0, 0), RGB(172, 168, 153));

		GetDefaultSysButton(&pSkinScrollBar->skinBtnLeftDisable, RGB(236, 233, 216), RGB(172, 168, 153), RGB(172, 168, 153));
		GetDefaultSysButton(&pSkinScrollBar->skinBtnRightDisable, RGB(236, 233, 216), RGB(172, 168, 153), RGB(172, 168, 153));
		GetDefaultSysButton(&pSkinScrollBar->skinBtnTopDisable, RGB(236, 233, 216), RGB(172, 168, 153), RGB(172, 168, 153));
		GetDefaultSysButton(&pSkinScrollBar->skinBtnBottomDisable, RGB(236, 233, 216), RGB(172, 168, 153), RGB(172, 168, 153));

		GetDefaultItem(&pSkinScrollBar->skinHItemNormal, RGB(0, 0, 128), RGB(255, 255, 255), RGB(0, 0, 0), 0);
		GetDefaultItem(&pSkinScrollBar->skinHItemDisabled, RGB(113, 111, 100), RGB(255, 255, 255), RGB(0, 0, 0), 0);
		GetDefaultItem(&pSkinScrollBar->skinHItemHover, RGB(0, 0, 255), RGB(255, 255, 255), RGB(0, 0, 0), 0);
		GetDefaultItem(&pSkinScrollBar->skinHItemPress, RGB(113, 111, 100), RGB(255, 255, 255), RGB(0, 0, 0), 0);

		GetDefaultItem(&pSkinScrollBar->skinVItemNormal, RGB(0, 0, 128), RGB(255, 255, 255), RGB(0, 0, 0), 0);
		GetDefaultItem(&pSkinScrollBar->skinVItemDisabled, RGB(113, 111, 100), RGB(255, 255, 255), RGB(0, 0, 0), 0);
		GetDefaultItem(&pSkinScrollBar->skinVItemHover, RGB(0, 0, 255), RGB(255, 255, 255), RGB(0, 0, 0), 0);
		GetDefaultItem(&pSkinScrollBar->skinVItemPress, RGB(113, 111, 100), RGB(255, 255, 255), RGB(0, 0, 0), 0);

		GetDefaultBorder(&pSkinScrollBar->skinHLBorderNormal, RGB(172, 168, 153), 0);        //COLOR_GRAYTEXT
		GetDefaultBorder(&pSkinScrollBar->skinHRBorderNormal, RGB(172, 168, 153), 0);        //COLOR_GRAYTEXT
		GetDefaultBorder(&pSkinScrollBar->skinHTBorderNormal, RGB(172, 168, 153), 1);
		GetDefaultBorder(&pSkinScrollBar->skinHBBorderNormal, RGB(172, 168, 153), 1);

		GetDefaultBorder(&pSkinScrollBar->skinVLBorderNormal, RGB(172, 168, 153), 1);        //COLOR_GRAYTEXT
		GetDefaultBorder(&pSkinScrollBar->skinVRBorderNormal, RGB(172, 168, 153), 1);        //COLOR_GRAYTEXT
		GetDefaultBorder(&pSkinScrollBar->skinVTBorderNormal, RGB(172, 168, 153), 0);
		GetDefaultBorder(&pSkinScrollBar->skinVBBorderNormal, RGB(172, 168, 153), 0);

		GetDefaultBorder(&pSkinScrollBar->skinHLBorderDisable, RGB(172, 168, 153), 0);       //COLOR_GRAYTEXT
		GetDefaultBorder(&pSkinScrollBar->skinHRBorderDisable, RGB(172, 168, 153), 0);
		GetDefaultBorder(&pSkinScrollBar->skinHTBorderDisable, RGB(172, 168, 153), 1);
		GetDefaultBorder(&pSkinScrollBar->skinHBBorderDisable, RGB(172, 168, 153), 1);

		GetDefaultBorder(&pSkinScrollBar->skinVLBorderDisable, RGB(172, 168, 153), 1);       //COLOR_GRAYTEXT
		GetDefaultBorder(&pSkinScrollBar->skinVRBorderDisable, RGB(172, 168, 153), 1);
		GetDefaultBorder(&pSkinScrollBar->skinVTBorderDisable, RGB(172, 168, 153), 0);
		GetDefaultBorder(&pSkinScrollBar->skinVBBorderDisable, RGB(172, 168, 153), 0);

		pSkinScrollBar->nBtnBottomIncludeBorder = 1;
		pSkinScrollBar->nBtnLeftIncludeBorder = 1;
		pSkinScrollBar->nBtnRightIncludeBorder = 1;
		pSkinScrollBar->nBtnTopIncludeBorder = 1;
		pSkinScrollBar->nHThumbIncludeBorder = 0;
		pSkinScrollBar->nVThumbIncludeBorder = 0;
	}

	return TRUE;
}

void CDreamSkinScrollBar::DrawWindowScrollBar(HWND hWnd, HDC hDC, UINT nSBLButtonDown, UINT nSBHover)
{
	DWORD dwStyle = ::GetWindowLong(hWnd, GWL_STYLE);

	if (dwStyle & WS_VSCROLL)
	{
		DrawVertScrollBar(hWnd, hDC, nSBLButtonDown, nSBHover);
	}

	if (dwStyle & WS_HSCROLL)
	{
		DrawHorzScrollBar(hWnd, hDC, nSBLButtonDown, nSBHover);
	}
}

int CDreamSkinScrollBar::ScrollBarHitTest(HWND hWnd, int fnBar, POINTS point)
{
	POINT pt;
	pt.x = point.x;
	pt.y = point.y;

	return ScrollBarHitTest(hWnd, fnBar, pt);
}

int CDreamSkinScrollBar::ScrollBarHitTest(HWND hWnd, int fnBar, POINT point)
{
	SCROLLBARINFO ScrollBarInfo;
	ScrollBarInfo.cbSize = sizeof(SCROLLBARINFO);
	
	if (fnBar == SB_HORZ)
	{
		::GetScrollBarInfo(hWnd, OBJID_HSCROLL, &ScrollBarInfo);
	}
	else if (fnBar == SB_VERT)
	{
		::GetScrollBarInfo(hWnd, OBJID_VSCROLL, &ScrollBarInfo);
	}
	else
	{
		::GetScrollBarInfo(hWnd, OBJID_CLIENT, &ScrollBarInfo);
		DWORD dwStyle = ::GetWindowLong(hWnd, GWL_STYLE);
		if ((dwStyle & 0x01) == SBS_HORZ)
			fnBar = SB_HORZ;
		else
			fnBar = SB_VERT;
	}

	return ScrollBarHitTest(&ScrollBarInfo, fnBar, point);
}

int CDreamSkinScrollBar::ScrollBarHitTest(LPSCROLLBARINFO psbi, int fnBar, POINT point)
{
	int nResult = SBHT_NOWHERE;

	if (fnBar == SB_HORZ)
	{
		if (::PtInRect(&psbi->rcScrollBar, point))
		{
			point.x = point.x - psbi->rcScrollBar.left;
			point.y = point.y - psbi->rcScrollBar.top;
			if (point.x < psbi->dxyLineButton)
				nResult = SBHT_ARROW_TOP;
			else if (point.x < psbi->xyThumbTop)
				nResult = SBHT_REGION_UP;
			else if (point.x < psbi->xyThumbBottom)
				nResult = SBHT_THUMB;
			else if (point.x < (psbi->rcScrollBar.right - psbi->rcScrollBar.left - psbi->dxyLineButton))
				nResult = SBHT_REGION_DOWN;
			else
				nResult = SBHT_ARROW_BOTTOM;
		}
	}
	else
	{
		if (::PtInRect(&psbi->rcScrollBar, point))
		{
			point.x = point.x - psbi->rcScrollBar.left;
			point.y = point.y - psbi->rcScrollBar.top;

			if (point.y < psbi->dxyLineButton)
				nResult = SBHT_ARROW_TOP;
			else if (point.y < psbi->xyThumbTop)
				nResult = SBHT_REGION_UP;
			else if (point.y < psbi->xyThumbBottom)
				nResult = SBHT_THUMB;
			else if (point.y < (psbi->rcScrollBar.bottom - psbi->rcScrollBar.top - psbi->dxyLineButton))
				nResult = SBHT_REGION_DOWN;
			else
				nResult = SBHT_ARROW_BOTTOM;
		}
	}

	return nResult;
}

void CDreamSkinScrollBar::TrackScrollBar(HWND hWnd, int fnBar, int nSBHitTest, SCROLLBARTRACKINFO *psbti)
{
	SCROLLINFO ScrollInfo;
	ScrollInfo.cbSize = sizeof(SCROLLINFO);
	ScrollInfo.fMask = SIF_ALL;
	::GetScrollInfo(hWnd, fnBar, &ScrollInfo);

	UINT nScrollMsg;
	int nMsgCode;

	int rnBar = fnBar;
	if (fnBar == SB_CTL)
	{
		DWORD dwStyle = ::GetWindowLong(hWnd, GWL_STYLE);
		if ((dwStyle & 0x01) == SBS_HORZ)
			fnBar = SB_HORZ;
		else
			fnBar = SB_VERT;
	}

	if (fnBar == SB_HORZ)
	{
		nScrollMsg = WM_HSCROLL;
		switch(nSBHitTest)
		{
		case SBHT_REGION_DOWN:
			nMsgCode = SB_PAGERIGHT;
			break;
		case SBHT_REGION_UP:
			nMsgCode = SB_PAGELEFT;
			break;
		case SBHT_ARROW_TOP:
			nMsgCode = SB_LINELEFT;
			break;
		case SBHT_ARROW_BOTTOM:
			nMsgCode = SB_LINERIGHT;
			break;
		default:
			nMsgCode = SB_THUMBTRACK;
			break;
		}
	}
	else
	{
		nScrollMsg = WM_VSCROLL;
		switch(nSBHitTest)
		{
		case SBHT_REGION_DOWN:
			nMsgCode = SB_PAGEDOWN;
			break;
		case SBHT_REGION_UP:
			nMsgCode = SB_PAGEUP;
			break;
		case SBHT_ARROW_TOP:
			nMsgCode = SB_LINEUP;
			break;
		case SBHT_ARROW_BOTTOM:
			nMsgCode = SB_LINEDOWN;
			break;
		default:
			nMsgCode = SB_THUMBTRACK;
			break;
		}
	}

	MSG Msg;
	POINT point;

	psbti->bIsTracking = TRUE;
	psbti->nTrackPos = ScrollInfo.nPos;
	::SetCapture(hWnd);

	if (nSBHitTest == SBHT_REGION_DOWN || nSBHitTest == SBHT_REGION_UP || nSBHitTest == SBHT_ARROW_TOP || nSBHitTest == SBHT_ARROW_BOTTOM)
	{
		RECT rcClient;
		::GetClientRect(hWnd, &rcClient);

		if (rnBar == SB_CTL)
		{
			::SendMessage(hWnd, nScrollMsg, nMsgCode, (LPARAM)hWnd);
			::InvalidateRect(hWnd, &rcClient, FALSE);
		}
		else
			::SendMessage(hWnd, nScrollMsg, nMsgCode, 0);

		int nTimer1 = ::SetTimer(0, 0, 500, NULL);
		int nTimer2 = 0;
		int nNewSBHitTest;

		while (::GetCapture() == hWnd && psbti->bIsTracking)
		{
			if (!::GetMessage(&Msg, NULL, 0, 0))
				break;

			switch(Msg.message)
			{
			case WM_LBUTTONUP:
				if (nTimer2 > 0)
					KillTimer(0, nTimer2);
				psbti->bIsTracking = FALSE;
				if (psbti->nSBLButtonDown)
					psbti->nSBLButtonDown = 0;
				if (rnBar == SB_CTL)
				{
					::SendMessage(hWnd, nScrollMsg, SB_ENDSCROLL, (LPARAM)hWnd);
					::InvalidateRect(hWnd, &rcClient, FALSE);
				}
				else
				{
					::SendMessage(hWnd, nScrollMsg, SB_ENDSCROLL, 0);
					::SendMessage(hWnd, WM_NCPAINT, 1, 0);
				}
				break;
			case WM_TIMER:
				if (Msg.wParam == nTimer1)
				{
					::KillTimer(0, nTimer1);

					::GetCursorPos(&point);
					nNewSBHitTest = ScrollBarHitTest(hWnd, rnBar, point);
					if (nNewSBHitTest == nSBHitTest)
						if (rnBar == SB_CTL)
						{
							::SendMessage(hWnd, nScrollMsg, nMsgCode, (LPARAM)hWnd);
							::InvalidateRect(hWnd, &rcClient, FALSE);
						}
						else
							::SendMessage(hWnd, nScrollMsg, nMsgCode, 0);
					nTimer2 = ::SetTimer(0, 0, 100, NULL);
				}
				else if (Msg.wParam == nTimer2)
				{
					::GetCursorPos(&point);
					nNewSBHitTest = ScrollBarHitTest(hWnd, rnBar, point);
					if (nNewSBHitTest == nSBHitTest)
						if (rnBar == SB_CTL)
						{
							::SendMessage(hWnd, nScrollMsg, nMsgCode, (LPARAM)hWnd);
							::InvalidateRect(hWnd, &rcClient, FALSE);
						}
						else
							::SendMessage(hWnd, nScrollMsg, nMsgCode, 0);
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
		::GetCursorPos(&point);

		RECT rcClient;
		::GetClientRect(hWnd, &rcClient);

		CDreamSkinWindow *pSkinWindow = NULL;

		if (rnBar == SB_CTL)
		{
			pSkinWindow = theSkinMain.GetHookedWindow(hWnd);
			if (pSkinWindow && pSkinWindow->GetType() != DREAMSKIN_SCROLLBAR)
				pSkinWindow = NULL;
			::InvalidateRect(hWnd, &rcClient, FALSE);
		}
		else
			::SendMessage(hWnd, WM_NCPAINT, 1, 0);

		SCROLLBARINFO ScrollBarInfo;
		ScrollBarInfo.cbSize = sizeof(SCROLLBARINFO);
		
		if (rnBar == SB_CTL)
		{
			::GetScrollBarInfo(hWnd, OBJID_CLIENT, &ScrollBarInfo);
		}
		else
		{
			if (fnBar == SB_HORZ)
			{
				::GetScrollBarInfo(hWnd, OBJID_HSCROLL, &ScrollBarInfo);
			}
			else
			{
				::GetScrollBarInfo(hWnd, OBJID_VSCROLL, &ScrollBarInfo);
			}
		}

		int nOffSet;
		if (fnBar == SB_HORZ)
			nOffSet = point.x - (ScrollBarInfo.rcScrollBar.left + ScrollBarInfo.xyThumbTop);
		else
			nOffSet = point.y - (ScrollBarInfo.rcScrollBar.top + ScrollBarInfo.xyThumbTop);

		while (::GetCapture() == hWnd && psbti->bIsTracking)
		{
			if (!::GetMessage(&Msg, NULL, 0, 0))
				break;

			switch(Msg.message)
			{
			case WM_LBUTTONUP:
				psbti->bIsTracking = FALSE;
				::GetCursorPos(&point);
				psbti->nTrackPos = ScrollBarDragPos(fnBar, &ScrollInfo, &ScrollBarInfo, point, nOffSet, &psbti->nSBLButtonDown);
				if (psbti->nSBLButtonDown)
					psbti->nSBLButtonDown = 0;
				
				if (rnBar == SB_CTL)
				{
					if (pSkinWindow)
						pSkinWindow->DefWindowProc(nScrollMsg, MAKEWPARAM(SB_THUMBPOSITION, (unsigned short)psbti->nTrackPos), (LPARAM)hWnd);
					else
						::SendMessage(hWnd, nScrollMsg, MAKEWPARAM(SB_THUMBPOSITION, (unsigned short)psbti->nTrackPos), (LPARAM)hWnd);
					::InvalidateRect(hWnd, &rcClient, FALSE);
				}
				else
				{
					::SendMessage(hWnd, nScrollMsg, MAKEWPARAM(SB_THUMBPOSITION, (unsigned short)psbti->nTrackPos), 0);
					::SendMessage(hWnd, WM_NCPAINT, 1, 0);
				}
				break;
			case WM_MOUSEMOVE:
				::GetCursorPos(&point);
				psbti->nTrackPos = ScrollBarDragPos(fnBar, &ScrollInfo, &ScrollBarInfo, point, nOffSet, &psbti->nSBLButtonDown);
				
				if (rnBar == SB_CTL)
				{
					if (pSkinWindow)
						pSkinWindow->DefWindowProc(nScrollMsg, MAKEWPARAM(SB_THUMBTRACK, (unsigned short)psbti->nTrackPos), (LPARAM)hWnd);
					else
						::SendMessage(hWnd, nScrollMsg, MAKEWPARAM(SB_THUMBTRACK, (unsigned short)psbti->nTrackPos), (LPARAM)hWnd);
					::InvalidateRect(hWnd, &rcClient, FALSE);
				}
				else
				{
					::SendMessage(hWnd, nScrollMsg, MAKEWPARAM(SB_THUMBTRACK, (unsigned short)psbti->nTrackPos), 0);
					::SendMessage(hWnd, WM_NCPAINT, 1, 0);
				}
				break;
			default:
				DispatchMessage(&Msg);
				break;
			}
		}
	}

	if (psbti->nSBLButtonDown)
		psbti->nSBLButtonDown = 0;

	if (::IsWindow(hWnd) && ::GetCapture() == hWnd)
		::ReleaseCapture();
}

void CDreamSkinScrollBar::DrawBackground(HDC hDC, SKINBACKGROUND* pBackground, RECT rcDraw)
{
	if (pBackground->nDrawType == DRAWTYPE_STRETCHBITMAP && pBackground->imgDraw.hImage)
		::DrawStretchBitmap(hDC, rcDraw.left, rcDraw.top, rcDraw.right - rcDraw.left, rcDraw.bottom - rcDraw.top, pBackground->imgDraw.hImage, pBackground->imgDraw.x, pBackground->imgDraw.y, pBackground->imgDraw.nWidth, pBackground->imgDraw.nHeight);
	else
		::FillSolidRect(hDC, &rcDraw, pBackground->clrDraw.clrStart);
}

void CDreamSkinScrollBar::DrawBorder(HDC hDC, RECT rcWindow, int fnBar, int nStatus)
{
	SKINBORDER *pLBorder, *pRBorder, *pTBorder, *pBBorder;
	if (fnBar == SB_HORZ)
	{
		if (nStatus == DRAWSTATUS_DISABLE)
		{
			pLBorder = &s_SkinScrollBar.skinHLBorderDisable;
			pRBorder = &s_SkinScrollBar.skinHRBorderDisable;
			pTBorder = &s_SkinScrollBar.skinHTBorderDisable;
			pBBorder = &s_SkinScrollBar.skinHBBorderDisable;
		}
		else
		{
			pLBorder = &s_SkinScrollBar.skinHLBorderNormal;
			pRBorder = &s_SkinScrollBar.skinHRBorderNormal;
			pTBorder = &s_SkinScrollBar.skinHTBorderNormal;
			pBBorder = &s_SkinScrollBar.skinHBBorderNormal;
		}
	}
	else
	{
		if (nStatus == DRAWSTATUS_DISABLE)
		{
			pLBorder = &s_SkinScrollBar.skinVLBorderDisable;
			pRBorder = &s_SkinScrollBar.skinVRBorderDisable;
			pTBorder = &s_SkinScrollBar.skinVTBorderDisable;
			pBBorder = &s_SkinScrollBar.skinVBBorderDisable;
		}
		else
		{
			pLBorder = &s_SkinScrollBar.skinVLBorderNormal;
			pRBorder = &s_SkinScrollBar.skinVRBorderNormal;
			pTBorder = &s_SkinScrollBar.skinVTBorderNormal;
			pBBorder = &s_SkinScrollBar.skinVBBorderNormal;
		}
	}

	CDreamSkinWindow::DrawBorder(hDC, pLBorder, pRBorder, pTBorder, pBBorder, rcWindow);
}

void CDreamSkinScrollBar::DrawButton(HDC hDC, RECT rcDraw, int fnBar, int nType, int nStatus)
{
	SKINSYSBUTTON *pSysButton;
	HBRUSH hDrawBrush;
	HRGN hRgn;
	HPEN hDrawPen, hOldPen;
	if (fnBar == SB_HORZ)
	{
		if (nType == SBHT_ARROW_TOP)
		{
			switch(nStatus)
			{
			case DRAWSTATUS_PRESS:
				pSysButton = &s_SkinScrollBar.skinBtnLeftPress;
				break;
			case DRAWSTATUS_HOVER:
				pSysButton = &s_SkinScrollBar.skinBtnLeftHover;
				break;
			case DRAWSTATUS_DISABLE:
				pSysButton = &s_SkinScrollBar.skinBtnLeftDisable;
				break;
			default:
				pSysButton = &s_SkinScrollBar.skinBtnLeftNormal;
				break;
			}
		}
		else
		{
			switch(nStatus)
			{
			case DRAWSTATUS_PRESS:
				pSysButton = &s_SkinScrollBar.skinBtnRightPress;
				break;
			case DRAWSTATUS_HOVER:
				pSysButton = &s_SkinScrollBar.skinBtnRightHover;
				break;
			case DRAWSTATUS_DISABLE:
				pSysButton = &s_SkinScrollBar.skinBtnRightDisable;
				break;
			default:
				pSysButton = &s_SkinScrollBar.skinBtnRightNormal;
				break;
			}
		}
	}
	else
	{
		if (nType == SBHT_ARROW_TOP)
		{
			switch(nStatus)
			{
			case DRAWSTATUS_PRESS:
				pSysButton = &s_SkinScrollBar.skinBtnTopPress;
				break;
			case DRAWSTATUS_HOVER:
				pSysButton = &s_SkinScrollBar.skinBtnTopHover;
				break;
			case DRAWSTATUS_DISABLE:
				pSysButton = &s_SkinScrollBar.skinBtnTopDisable;
				break;
			default:
				pSysButton = &s_SkinScrollBar.skinBtnTopNormal;
				break;
			}
		}
		else
		{
			switch(nStatus)
			{
			case DRAWSTATUS_PRESS:
				pSysButton = &s_SkinScrollBar.skinBtnBottomPress;
				break;
			case DRAWSTATUS_HOVER:
				pSysButton = &s_SkinScrollBar.skinBtnBottomHover;
				break;
			case DRAWSTATUS_DISABLE:
				pSysButton = &s_SkinScrollBar.skinBtnBottomDisable;
				break;
			default:
				pSysButton = &s_SkinScrollBar.skinBtnBottomNormal;
				break;
			}
		}
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
		hDrawBrush = ::CreateSolidBrush(pSysButton->clrTxt);
		hDrawPen = ::CreatePen(PS_SOLID, 1, pSysButton->clrTxt);
		hOldPen = (HPEN)::SelectObject(hDC, hDrawPen);
		int nTemp;

		if (fnBar == SB_HORZ)
		{
			if (nType == SBHT_ARROW_TOP) //draw lef button
			{
				pt[0].x = rcDraw.left + nWidth * 3 / 8;
				pt[0].y = rcDraw.top + nHeight / 2;
				nTemp = min(nHeight / 4, nWidth / 4);
				pt[1].x = pt[0].x + nTemp;
				pt[1].y = pt[0].y - nTemp;
				pt[2].x = pt[0].x + nTemp;
				pt[2].y = pt[0].y + nTemp;

			}//draw left button
			else //draw right button
			{
				pt[0].x = rcDraw.right - nWidth * 3 / 8;
				pt[0].y = rcDraw.top + nHeight / 2;
				nTemp = min(nHeight / 4, nWidth / 4);
				pt[1].x = pt[0].x - nTemp;
				pt[1].y = pt[0].y - nTemp;
				pt[2].x = pt[0].x - nTemp;
				pt[2].y = pt[0].y + nTemp;
			}//draw right button
		}
		else
		{
			if (nType == SBHT_ARROW_TOP) //draw top button
			{
				pt[0].x = rcDraw.left + nWidth / 2;
				pt[0].y = rcDraw.top + nHeight * 3 / 8;
				nTemp = min(nHeight / 4, nWidth / 4);
				pt[1].x = pt[0].x - nTemp;
				pt[1].y = pt[0].y + nTemp;
				pt[2].x = pt[0].x + nTemp;
				pt[2].y = pt[0].y + nTemp;
			}//draw top button
			else //draw bottom button
			{
				pt[0].x = rcDraw.left + nWidth / 2;
				pt[0].y = rcDraw.bottom - nHeight * 3 / 8;
				nTemp = min(nHeight / 4, nWidth / 4);
				pt[1].x = pt[0].x - nTemp;
				pt[1].y = pt[0].y - nTemp;
				pt[2].x = pt[0].x + nTemp;
				pt[2].y = pt[0].y - nTemp;
			}//draw bottom button
		}

		hRgn = ::CreatePolygonRgn(pt, 3, ALTERNATE);
		::FillRgn(hDC, hRgn, hDrawBrush);

		::MoveToEx(hDC, pt[0].x, pt[0].y, NULL);
		::LineTo(hDC, pt[1].x, pt[1].y);
		::LineTo(hDC, pt[2].x, pt[2].y);
		::LineTo(hDC, pt[0].x, pt[0].y);

		::Draw3dRect(hDC, &rcDraw, pSysButton->clrBd, pSysButton->clrBd);

		::SelectObject(hDC, hOldPen);
		::DeleteObject(hDrawPen);

		::DeleteObject(hRgn);
		::DeleteObject(hDrawBrush);
	}
}

void CDreamSkinScrollBar::DrawThumb(HDC hDC, RECT rcDraw, int fnBar, int nStatus)
{
	SKINITEM *pItem;
	if (fnBar == SB_HORZ)
	{
		switch(nStatus)
		{
		case DRAWSTATUS_PRESS:
			pItem = &s_SkinScrollBar.skinHItemPress;
			break;
		case DRAWSTATUS_HOVER:
			pItem = &s_SkinScrollBar.skinHItemHover;
			break;
		case DRAWSTATUS_DISABLE:
			pItem = &s_SkinScrollBar.skinHItemDisabled;
			break;
		default:
			pItem = &s_SkinScrollBar.skinHItemNormal;
			break;
		}
	}
	else
	{
		switch(nStatus)
		{
		case DRAWSTATUS_PRESS:
			pItem = &s_SkinScrollBar.skinVItemPress;
			break;
		case DRAWSTATUS_HOVER:
			pItem = &s_SkinScrollBar.skinVItemHover;
			break;
		case DRAWSTATUS_DISABLE:
			pItem = &s_SkinScrollBar.skinVItemDisabled;
			break;
		default:
			pItem = &s_SkinScrollBar.skinVItemNormal;
			break;
		}
	}
	
	DrawItem(hDC, pItem, rcDraw, NULL);
}

void CDreamSkinScrollBar::DrawItem(HDC hDC, SKINITEM *pItem, RECT rcItem, WCHAR *wstrTitle)
{
	RECT rcClient = GetItemRectClient(pItem, rcItem);
	DrawBackground(hDC, &pItem->skinBk, rcClient);

	CDreamSkinWindow::DrawBorder(hDC, &pItem->skinLBorder, &pItem->skinRBorder, &pItem->skinTBorder, &pItem->skinBBorder, rcItem);

	if (((rcClient.bottom - rcClient.top) >= pItem->skinIcon.nHeight) && ((rcClient.right - rcClient.left) >= pItem->skinIcon.nWidth))
	{
		RECT rcIcon = GetItemRectIcon(pItem, rcClient, 0);
		DrawIcon(hDC, rcIcon, &pItem->skinIcon);
	}
}

void CDreamSkinScrollBar::DrawVertScrollBar(HWND hWnd, HDC hDC, UINT nSBLButtonDown, UINT nSBHover)
{
	SCROLLINFO ScrollInfo;
	SCROLLBARINFO ScrollBarInfo;
	RECT rcTop, rcBottom, rcClient, rcItem;
	int nStatus;

	ScrollBarInfo.cbSize = sizeof(SCROLLBARINFO);
	::GetScrollBarInfo(hWnd, OBJID_VSCROLL, &ScrollBarInfo);
	if (ScrollBarInfo.rcScrollBar.right - ScrollBarInfo.rcScrollBar.left > 0)
	{
		ScrollInfo.cbSize = sizeof(SCROLLINFO);
		ScrollInfo.fMask = SIF_ALL;
		::GetScrollInfo(hWnd, SB_VERT, &ScrollInfo);

		RECT rcWindow;
		::GetWindowRect(hWnd, &rcWindow);
		::OffsetRect(&ScrollBarInfo.rcScrollBar, 0 - rcWindow.left, 0 - rcWindow.top);

		DWORD dwStyle = ::GetWindowLong(hWnd, GWL_STYLE);

		SKINBACKGROUND *pBackground;
		if(dwStyle & WS_DISABLED)
			pBackground = &s_SkinScrollBar.skinBkDisable;   //Disabled
		else
			pBackground = &s_SkinScrollBar.skinBkNormal;    //Normal
		DrawBackground(hDC, pBackground, ScrollBarInfo.rcScrollBar);

		if (ScrollInfo.nMax != ScrollInfo.nMin)
		{
			rcTop.left = ScrollBarInfo.rcScrollBar.left;
			rcTop.top = ScrollBarInfo.rcScrollBar.top;
			rcTop.bottom = ScrollBarInfo.rcScrollBar.top + ScrollBarInfo.dxyLineButton;
			rcTop.right = ScrollBarInfo.rcScrollBar.right;

			if (!s_SkinScrollBar.nBtnTopIncludeBorder)
			{
				rcTop.left += s_SkinScrollBar.skinVLBorderNormal.nWidth;
				rcTop.top += s_SkinScrollBar.skinVTBorderNormal.nWidth;
				rcTop.right -= s_SkinScrollBar.skinVRBorderNormal.nWidth;
			}

			rcBottom.left = ScrollBarInfo.rcScrollBar.left;
			rcBottom.top = ScrollBarInfo.rcScrollBar.bottom - ScrollBarInfo.dxyLineButton;
			rcBottom.bottom = ScrollBarInfo.rcScrollBar.bottom;
			rcBottom.right = ScrollBarInfo.rcScrollBar.right;

			if (!s_SkinScrollBar.nBtnBottomIncludeBorder)
			{
				rcBottom.left += s_SkinScrollBar.skinVLBorderNormal.nWidth;
				rcBottom.bottom -= s_SkinScrollBar.skinVBBorderNormal.nWidth;
				rcBottom.right -= s_SkinScrollBar.skinVRBorderNormal.nWidth;
			}

			rcClient.left = ScrollBarInfo.rcScrollBar.left;
			rcClient.top  = ScrollBarInfo.rcScrollBar.top + ScrollBarInfo.dxyLineButton;
			rcClient.bottom = ScrollBarInfo.rcScrollBar.bottom - ScrollBarInfo.dxyLineButton;
			rcClient.right = ScrollBarInfo.rcScrollBar.right;

			rcItem.left = ScrollBarInfo.rcScrollBar.left;
			rcItem.right = ScrollBarInfo.rcScrollBar.right;
			rcItem.top = ScrollBarInfo.xyThumbTop + ScrollBarInfo.rcScrollBar.top;
			rcItem.bottom = ScrollBarInfo.xyThumbBottom + ScrollBarInfo.rcScrollBar.top;
			if (!s_SkinScrollBar.nVThumbIncludeBorder)
			{
				rcItem.left += s_SkinScrollBar.skinVLBorderNormal.nWidth;
				rcItem.right -= s_SkinScrollBar.skinVRBorderNormal.nWidth;
			}
			

			//draw top button
			if(dwStyle & WS_DISABLED)
				nStatus = DRAWSTATUS_DISABLE;       //Disabled
			else if(((nSBHover >> 16) == SB_VERT) && ((nSBHover & 0x0000FFFF) == SBHT_ARROW_TOP))
				if(((nSBLButtonDown >> 16) == SB_VERT) && ((nSBLButtonDown & 0x0000FFFF) == SBHT_ARROW_TOP))
					nStatus = DRAWSTATUS_PRESS;     //Pressed
				else
					nStatus = DRAWSTATUS_HOVER;     //Hover
			else
				nStatus = DRAWSTATUS_NORMAL;    //Normal
			DrawButton(hDC, rcTop, SB_VERT, SBHT_ARROW_TOP, nStatus);

			//draw bottom button
			if(dwStyle & WS_DISABLED)
				nStatus = DRAWSTATUS_DISABLE;       //Disabled
			else if(((nSBHover >> 16) == SB_VERT) && ((nSBHover & 0x0000FFFF) == SBHT_ARROW_BOTTOM))
				if(((nSBLButtonDown >> 16) == SB_VERT) && ((nSBLButtonDown & 0x0000FFFF) == SBHT_ARROW_BOTTOM))
					nStatus = DRAWSTATUS_PRESS;     //Pressed
				else
					nStatus = DRAWSTATUS_HOVER;     //Hover
			else
				nStatus = DRAWSTATUS_NORMAL;    //Normal
			DrawButton(hDC, rcBottom, SB_VERT, SBHT_ARROW_BOTTOM, nStatus);

			//draw client area
			if(dwStyle & WS_DISABLED)
				pBackground = &s_SkinScrollBar.skinVBkDisable;   //Disabled
			else
				pBackground = &s_SkinScrollBar.skinVBkNormal;    //Normal
			DrawBackground(hDC, pBackground, rcClient);

			//draw thumb
			if ((rcClient.bottom - rcClient.top) > 5)
			{
				if(dwStyle & WS_DISABLED)
					nStatus = DRAWSTATUS_DISABLE;       //Disabled
				else if(((nSBHover >> 16) == SB_VERT) && ((nSBHover & 0x0000FFFF) == SBHT_THUMB))
					if(((nSBLButtonDown >> 16) == SB_VERT) && ((nSBLButtonDown & 0x0000FFFF) == SBHT_THUMB))
						nStatus = DRAWSTATUS_PRESS;     //Pressed
					else
						nStatus = DRAWSTATUS_HOVER;     //Hover
				else
					if(((nSBLButtonDown >> 16) == SB_VERT) && ((nSBLButtonDown & 0x0000FFFF) == SBHT_THUMB))
						nStatus = DRAWSTATUS_PRESS;     //Drag
					else
						nStatus = DRAWSTATUS_NORMAL;     //normal
				DrawThumb(hDC, rcItem, SB_VERT, nStatus);
			}

			//draw border
			if(dwStyle & WS_DISABLED)
				nStatus = DRAWSTATUS_DISABLE;   //Disabled
			else
				nStatus = DRAWSTATUS_NORMAL;    //Normal

			rcWindow = ScrollBarInfo.rcScrollBar;
			if (s_SkinScrollBar.nBtnTopIncludeBorder)
			{
				rcWindow.top = rcTop.bottom;
			}

			if (s_SkinScrollBar.nBtnBottomIncludeBorder)
			{
				rcWindow.bottom = rcBottom.top;
			}
			DrawBorder(hDC, rcWindow, SB_VERT, nStatus);
		}
	}
}

void CDreamSkinScrollBar::DrawHorzScrollBar(HWND hWnd, HDC hDC, UINT nSBLButtonDown, UINT nSBHover)
{
	SCROLLINFO ScrollInfo;
	SCROLLBARINFO ScrollBarInfo;
	RECT rcTop, rcBottom, rcClient, rcItem;
	int nStatus;

	ScrollBarInfo.cbSize = sizeof(SCROLLBARINFO);
	::GetScrollBarInfo(hWnd, OBJID_HSCROLL, &ScrollBarInfo);
	if (ScrollBarInfo.rcScrollBar.bottom - ScrollBarInfo.rcScrollBar.top > 0)
	{
		ScrollInfo.cbSize = sizeof(SCROLLINFO);
		ScrollInfo.fMask = SIF_ALL;
		::GetScrollInfo(hWnd, SB_HORZ, &ScrollInfo);

		RECT rcWindow;
		::GetWindowRect(hWnd, &rcWindow);
		::OffsetRect(&ScrollBarInfo.rcScrollBar, 0 - rcWindow.left, 0 - rcWindow.top);

		DWORD dwStyle = ::GetWindowLong(hWnd, GWL_STYLE);

		SKINBACKGROUND *pBackground;
		if(dwStyle & WS_DISABLED)
			pBackground = &s_SkinScrollBar.skinBkDisable;   //Disabled
		else
			pBackground = &s_SkinScrollBar.skinBkNormal;    //Normal
		DrawBackground(hDC, pBackground, ScrollBarInfo.rcScrollBar);

		if (ScrollInfo.nMax != ScrollInfo.nMin)
		{
			rcTop.left = ScrollBarInfo.rcScrollBar.left;
			rcTop.top = ScrollBarInfo.rcScrollBar.top;
			rcTop.bottom = ScrollBarInfo.rcScrollBar.bottom;
			rcTop.right = ScrollBarInfo.rcScrollBar.left + ScrollBarInfo.dxyLineButton;

			if (!s_SkinScrollBar.nBtnLeftIncludeBorder)
			{
				rcTop.left += s_SkinScrollBar.skinHLBorderNormal.nWidth;
				rcTop.top += s_SkinScrollBar.skinHTBorderNormal.nWidth;
				rcTop.bottom -= s_SkinScrollBar.skinHBBorderNormal.nWidth;
			}

			rcBottom.left = ScrollBarInfo.rcScrollBar.right - ScrollBarInfo.dxyLineButton;
			rcBottom.top = ScrollBarInfo.rcScrollBar.top;
			rcBottom.bottom = ScrollBarInfo.rcScrollBar.bottom;
			rcBottom.right = ScrollBarInfo.rcScrollBar.right;

			if (!s_SkinScrollBar.nBtnRightIncludeBorder)
			{
				rcBottom.top += s_SkinScrollBar.skinHTBorderNormal.nWidth;
				rcBottom.right -= s_SkinScrollBar.skinHRBorderNormal.nWidth;
				rcBottom.bottom -= s_SkinScrollBar.skinHBBorderNormal.nWidth;
			}

			rcClient.left = ScrollBarInfo.rcScrollBar.left + ScrollBarInfo.dxyLineButton;
			rcClient.top  = ScrollBarInfo.rcScrollBar.top;
			rcClient.bottom = ScrollBarInfo.rcScrollBar.bottom;
			rcClient.right = ScrollBarInfo.rcScrollBar.right - ScrollBarInfo.dxyLineButton;

			rcItem.left = ScrollBarInfo.rcScrollBar.left + ScrollBarInfo.xyThumbTop;
			rcItem.right = ScrollBarInfo.rcScrollBar.left + ScrollBarInfo.xyThumbBottom;
			rcItem.top = ScrollBarInfo.rcScrollBar.top;
			rcItem.bottom = ScrollBarInfo.rcScrollBar.bottom;
			if (!s_SkinScrollBar.nHThumbIncludeBorder)
			{
				rcItem.top += s_SkinScrollBar.skinHTBorderNormal.nWidth;
				rcItem.bottom -= s_SkinScrollBar.skinHBBorderNormal.nWidth;
			}

			//draw top button
			if(dwStyle & WS_DISABLED)
				nStatus = DRAWSTATUS_DISABLE;       //Disabled
			else if(((nSBHover >> 16) == SB_HORZ) && ((nSBHover & 0x0000FFFF) == SBHT_ARROW_TOP))
				if(((nSBLButtonDown >> 16) == SB_HORZ) && ((nSBLButtonDown & 0x0000FFFF) == SBHT_ARROW_TOP))
					nStatus = DRAWSTATUS_PRESS;     //Pressed
				else
					nStatus = DRAWSTATUS_HOVER;     //Hover
			else
				nStatus = DRAWSTATUS_NORMAL;    //Normal
			DrawButton(hDC, rcTop, SB_HORZ, SBHT_ARROW_TOP, nStatus);

			//draw bottom button
			if(dwStyle & WS_DISABLED)
				nStatus = DRAWSTATUS_DISABLE;       //Disabled
			else if(((nSBHover >> 16) == SB_HORZ) && ((nSBHover & 0x0000FFFF) == SBHT_ARROW_BOTTOM))
				if(((nSBLButtonDown >> 16) == SB_HORZ) && ((nSBLButtonDown & 0x0000FFFF) == SBHT_ARROW_BOTTOM))
					nStatus = DRAWSTATUS_PRESS;     //Pressed
				else
					nStatus = DRAWSTATUS_HOVER;     //Hover
			else
				nStatus = DRAWSTATUS_NORMAL;    //Normal
			DrawButton(hDC, rcBottom, SB_HORZ, SBHT_ARROW_BOTTOM, nStatus);

			//draw client area
			if(dwStyle & WS_DISABLED)
				pBackground = &s_SkinScrollBar.skinHBkDisable;   //Disabled
			else
				pBackground = &s_SkinScrollBar.skinHBkNormal;    //Normal
			DrawBackground(hDC, pBackground, rcClient);

			//draw thumb
			if ((rcClient.right - rcClient.left) > 5)
			{
				if(dwStyle & WS_DISABLED)
					nStatus = DRAWSTATUS_DISABLE;       //Disabled
				else if(((nSBHover >> 16) == SB_HORZ) && ((nSBHover & 0x0000FFFF) == SBHT_THUMB))
					if(((nSBLButtonDown >> 16) == SB_HORZ) && ((nSBLButtonDown & 0x0000FFFF) == SBHT_THUMB))
						nStatus = DRAWSTATUS_PRESS;     //Pressed
					else
						nStatus = DRAWSTATUS_HOVER;     //Hover
				else
					if(((nSBLButtonDown >> 16) == SB_HORZ) && ((nSBLButtonDown & 0x0000FFFF) == SBHT_THUMB))
						nStatus = DRAWSTATUS_PRESS;     //Drag
					else
						nStatus = DRAWSTATUS_NORMAL;     //normal
				DrawThumb(hDC, rcItem, SB_HORZ, nStatus);
			}

			//draw border
			if(dwStyle & WS_DISABLED)
				nStatus = DRAWSTATUS_DISABLE;   //Disabled
			else
				nStatus = DRAWSTATUS_NORMAL;    //Normal
			rcWindow = ScrollBarInfo.rcScrollBar;
			if (s_SkinScrollBar.nBtnLeftIncludeBorder)
			{
				rcWindow.left = rcTop.right;
			}

			if (s_SkinScrollBar.nBtnRightIncludeBorder)
			{
				rcWindow.right = rcBottom.left;
			}
			DrawBorder(hDC, rcWindow, SB_HORZ, nStatus);
		}
	}
}

int CDreamSkinScrollBar::ScrollBarDragPos(int fnBar, LPSCROLLINFO lpsi, LPSCROLLBARINFO psbi, POINT point, int nOffSet, UINT *pSBLButtonDown)
{
	BOOL bIsDrag = TRUE;
	int nPos;

	if (fnBar == SB_HORZ)
	{
		if (abs(point.y - (psbi->rcScrollBar.bottom + psbi->rcScrollBar.top) / 2) >= 150)
			bIsDrag = FALSE;
		else if ((psbi->rcScrollBar.left - point.x >= 10) || (point.x - psbi->rcScrollBar.right) >= 10)
			bIsDrag = FALSE;
	}
	else
	{
		if (abs(point.x - (psbi->rcScrollBar.right + psbi->rcScrollBar.left) / 2) >= 150)
			bIsDrag = FALSE;
		else if ((psbi->rcScrollBar.top - point.y >= 10) || (point.y - psbi->rcScrollBar.bottom) >= 10)
			bIsDrag = FALSE;
	}

	if (bIsDrag)
	{
		if (pSBLButtonDown)
			*pSBLButtonDown = ((*pSBLButtonDown) >> 16 << 16) | SBHT_THUMB;
		int nTop, nBottom;
		if (fnBar == SB_HORZ)
		{
			point.x -= nOffSet;
			nTop = psbi->rcScrollBar.left + psbi->dxyLineButton;
			nBottom = psbi->rcScrollBar.right - psbi->dxyLineButton;
			if (point.x <= nTop)
				nPos = lpsi->nMin;
			else if (point.x >= nBottom)
				nPos = lpsi->nMax;
			else
				nPos = (nBottom * lpsi->nMin - nTop * lpsi->nMax + lpsi->nMax * point.x - lpsi->nMin * point.x) / (nBottom - nTop);
		}
		else
		{
			point.y -= nOffSet;
			nTop = psbi->rcScrollBar.top + psbi->dxyLineButton;
			nBottom = psbi->rcScrollBar.bottom - psbi->dxyLineButton;
			if (point.y <= nTop)
				nPos = lpsi->nMin;
			else if (point.y >= nBottom)
				nPos = lpsi->nMax;
			else
				nPos = (nBottom * lpsi->nMin - nTop * lpsi->nMax + lpsi->nMax * point.y - lpsi->nMin * point.y) / (nBottom - nTop);
		}
	}
	else
	{
		if (pSBLButtonDown)
			*pSBLButtonDown = *pSBLButtonDown & 0xFFFF0000;
		nPos = lpsi->nPos;
	}

	return nPos;
}

LRESULT CDreamSkinScrollBar::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT nResult = ERROR_SUCCESS;

	switch(message)
	{
	case SBM_GETSCROLLBARINFO :
		nResult = OnGetScrollBarInfo((LPSCROLLBARINFO)lParam);
		break;
	case SBM_GETSCROLLINFO:
		nResult = OnGetScrollInfo(wParam, (LPSCROLLINFO)lParam);
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
	case WM_LBUTTONUP:
		m_ScrollBarTrackInfo.nSBLButtonDown = 0;
		m_ScrollBarTrackInfo.bIsTracking = FALSE;
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
	case WM_VSCROLL:
		nResult = OnVScroll(wParam, (HWND)lParam);
		break;
	default:
		nResult = CDreamSkinWindow::DefWindowProc(message, wParam, lParam);
		break;
	}

	return nResult;
}

LRESULT CDreamSkinScrollBar::OnGetScrollBarInfo(LPSCROLLBARINFO psbi)
{
	RECT rcWindow;
	::GetWindowRect(m_hWnd, &rcWindow);

	SCROLLINFO ScrollInfo;
	ScrollInfo.cbSize = sizeof(SCROLLINFO);
	ScrollInfo.fMask = SIF_ALL;
	::GetScrollInfo(m_hWnd, SB_CTL, &ScrollInfo);

	DWORD dwStyle = ::GetWindowLong(m_hWnd, GWL_STYLE);

	return GetScrollBarInfo(rcWindow, (dwStyle & 0x01) == SBS_HORZ ? SB_HORZ : SB_VERT, &ScrollInfo, psbi);
}

LRESULT CDreamSkinScrollBar::OnGetScrollInfo(int fnBar, LPSCROLLINFO lpsi)
{
	BOOL bResult = FALSE;

	if (lpsi->fMask == SIF_TRACKPOS)
	{
		if (m_ScrollBarTrackInfo.bIsTracking)
		{
			lpsi->nTrackPos = m_ScrollBarTrackInfo.nTrackPos;
			bResult = TRUE;
		}
	}
	else
	{
		bResult = CDreamSkinWindow::DefWindowProc(SBM_GETSCROLLINFO, fnBar, (LPARAM)lpsi);
	}

	return bResult;
}

LRESULT CDreamSkinScrollBar::OnHScroll(UINT nSBCode, HWND hWndCtrl)
{
	LRESULT lResult = 0;

	if (hWndCtrl == m_hWnd)
	{
		SCROLLINFO ScrollInfo;
		ScrollInfo.cbSize = sizeof(SCROLLINFO);
		ScrollInfo.fMask = SIF_ALL;
		::GetScrollInfo(m_hWnd, SB_CTL, &ScrollInfo);

		switch(nSBCode & 0x0000FFFF)
		{
		case SB_LINELEFT:
			if (ScrollInfo.nPos > ScrollInfo.nMin)
			{
				ScrollInfo.nPos -= 1;
				m_ScrollBarTrackInfo.nTrackPos = ScrollInfo.nPos;
				ScrollInfo.fMask = SIF_POS;
				::SetScrollInfo(m_hWnd, SB_CTL, &ScrollInfo, FALSE);
			}
			break;
		case SB_LINERIGHT:
			if (ScrollInfo.nPos < ScrollInfo.nMax)
			{
				ScrollInfo.nPos += 1;
				m_ScrollBarTrackInfo.nTrackPos = ScrollInfo.nPos;
				ScrollInfo.fMask = SIF_POS;
				::SetScrollInfo(m_hWnd, SB_CTL, &ScrollInfo, FALSE);
			}
			break;
		case SB_PAGELEFT:
			if (ScrollInfo.nPos > ScrollInfo.nMin)
			{
				ScrollInfo.nPos -= ScrollInfo.nPage;
				if (ScrollInfo.nPos < ScrollInfo.nMin)
					ScrollInfo.nPos = ScrollInfo.nMin;
				m_ScrollBarTrackInfo.nTrackPos = ScrollInfo.nPos;
				ScrollInfo.fMask = SIF_POS;
				::SetScrollInfo(m_hWnd, SB_CTL, &ScrollInfo, FALSE);
			}
			break;
		case SB_PAGERIGHT:
			if (ScrollInfo.nPos < ScrollInfo.nMax)
			{
				ScrollInfo.nPos += ScrollInfo.nPage;
				if (ScrollInfo.nPos > ScrollInfo.nMax)
					ScrollInfo.nPos = ScrollInfo.nMax;
				m_ScrollBarTrackInfo.nTrackPos = ScrollInfo.nPos;
				ScrollInfo.fMask = SIF_POS;
				::SetScrollInfo(m_hWnd, SB_CTL, &ScrollInfo, FALSE);
			}
			break;
		case SB_THUMBPOSITION:
			if (m_ScrollBarTrackInfo.nTrackPos != ScrollInfo.nPos)
			{
				ScrollInfo.nPos = m_ScrollBarTrackInfo.nTrackPos;
				if (ScrollInfo.nPos > ScrollInfo.nMax)
					ScrollInfo.nPos = ScrollInfo.nMax;
				if (ScrollInfo.nPos < ScrollInfo.nMin)
					ScrollInfo.nPos = ScrollInfo.nMin;
				ScrollInfo.fMask = SIF_POS;
				::SetScrollInfo(m_hWnd, SB_CTL, &ScrollInfo, FALSE);
			}
			nSBCode = SB_ENDSCROLL;
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

LRESULT CDreamSkinScrollBar::OnLButtonDown(UINT nFlags, POINTS point)
{
	LRESULT nResult = 0;

	POINT pt;
	pt.x = point.x;
	pt.y = point.y;

	int nSBHover = 0;
	DWORD dwStyle = ::GetWindowLong(m_hWnd, GWL_STYLE);

	RECT rcClient;
	::GetClientRect(m_hWnd, &rcClient);

	SCROLLINFO ScrollInfo;
	SCROLLBARINFO ScrollBarInfo;

	ScrollInfo.cbSize = sizeof(SCROLLINFO);
	ScrollInfo.fMask = SIF_ALL;
	::GetScrollInfo(m_hWnd, SB_CTL, &ScrollInfo);

	int fnBar = SB_HORZ;
	if ((dwStyle & 0x01) == SBS_VERT)
		fnBar = SB_VERT;

	GetScrollBarInfo(rcClient, fnBar, &ScrollInfo, &ScrollBarInfo);
	switch(ScrollBarHitTest(&ScrollBarInfo, fnBar, pt))
	{
	case SBHT_THUMB:
		m_ScrollBarTrackInfo.nSBLButtonDown = fnBar << 16 | SBHT_THUMB;
		TrackScrollBar(m_hWnd, SB_CTL, SBHT_THUMB, &m_ScrollBarTrackInfo);
		break;
	case SBHT_REGION_UP:
		m_ScrollBarTrackInfo.nSBLButtonDown = fnBar << 16 | SBHT_REGION_UP;
		TrackScrollBar(m_hWnd, SB_CTL, SBHT_REGION_UP, &m_ScrollBarTrackInfo);
		break;
	case SBHT_REGION_DOWN:
		m_ScrollBarTrackInfo.nSBLButtonDown = fnBar << 16 | SBHT_REGION_DOWN;
		TrackScrollBar(m_hWnd, SB_CTL, SBHT_REGION_DOWN, &m_ScrollBarTrackInfo);
		break;
	case SBHT_ARROW_TOP:
		m_ScrollBarTrackInfo.nSBLButtonDown = fnBar << 16 | SBHT_ARROW_TOP;
		TrackScrollBar(m_hWnd, SB_CTL, SBHT_ARROW_TOP, &m_ScrollBarTrackInfo);
		break;
	case SBHT_ARROW_BOTTOM:
		m_ScrollBarTrackInfo.nSBLButtonDown = fnBar << 16 | SBHT_ARROW_BOTTOM;
		TrackScrollBar(m_hWnd, SB_CTL, SBHT_ARROW_BOTTOM, &m_ScrollBarTrackInfo);
		break;
	default:
		m_ScrollBarTrackInfo.nSBLButtonDown = 0;
		::InvalidateRect(m_hWnd, &rcClient, FALSE);
		break;
	}

	return nResult;
}

LRESULT CDreamSkinScrollBar::OnMouseLeave()
{
	if (m_nSBHover > 0)
	{
		POINT point;
		::GetCursorPos(&point);

		int nSBHover = 0;
		DWORD dwStyle = ::GetWindowLong(m_hWnd, GWL_STYLE);

		RECT rcClient;
		::GetClientRect(m_hWnd, &rcClient);

		SCROLLINFO ScrollInfo;
		SCROLLBARINFO ScrollBarInfo;

		ScrollInfo.cbSize = sizeof(SCROLLINFO);
		ScrollInfo.fMask = SIF_ALL;
		::GetScrollInfo(m_hWnd, SB_CTL, &ScrollInfo);

		int fnBar = SB_HORZ;
		if ((dwStyle & 0x01) == SBS_VERT)
			fnBar = SB_VERT;

		GetScrollBarInfo(rcClient, fnBar, &ScrollInfo, &ScrollBarInfo);
		nSBHover = ScrollBarHitTest(&ScrollBarInfo, fnBar, point);
		if (nSBHover > 0)
			nSBHover = fnBar << 16 | nSBHover;

		if (nSBHover != m_nSBHover)
		{
			m_nSBHover = nSBHover;
			::InvalidateRect(m_hWnd, &rcClient, FALSE);
		}
	}

	return 0;
}

LRESULT CDreamSkinScrollBar::OnMouseMove(UINT nFlags, POINTS point)
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

	int nSBHover = 0;
	DWORD dwStyle = ::GetWindowLong(m_hWnd, GWL_STYLE);

	RECT rcClient;
	::GetClientRect(m_hWnd, &rcClient);

	SCROLLINFO ScrollInfo;
	SCROLLBARINFO ScrollBarInfo;

	ScrollInfo.cbSize = sizeof(SCROLLINFO);
	ScrollInfo.fMask = SIF_ALL;
	::GetScrollInfo(m_hWnd, SB_CTL, &ScrollInfo);

	int fnBar = SB_HORZ;
	if ((dwStyle & 0x01) == SBS_VERT)
		fnBar = SB_VERT;

	GetScrollBarInfo(rcClient, fnBar, &ScrollInfo, &ScrollBarInfo);
	nSBHover = ScrollBarHitTest(&ScrollBarInfo, fnBar, pt);
	if (nSBHover > 0)
		nSBHover = fnBar << 16 | nSBHover;

	if (nSBHover != m_nSBHover)
	{
		m_nSBHover = nSBHover;
		::InvalidateRect(m_hWnd, &rcClient, FALSE);
	}

	return 0;
}

LRESULT CDreamSkinScrollBar::OnPaint()
{
	PAINTSTRUCT ps;
	HDC hPaintDC = ::BeginPaint(m_hWnd, &ps);

	DWORD dwStyle = ::GetWindowLong(m_hWnd, GWL_STYLE);
	if ((dwStyle & 0x01) == SBS_HORZ)
	{
		DrawHorzScrollBar(hPaintDC, dwStyle);
	}
	else
	{
		DrawVertScrollBar(hPaintDC, dwStyle);
	}

	::EndPaint(m_hWnd, &ps);

	return 0;
}

LRESULT CDreamSkinScrollBar::OnVScroll(UINT nSBCode, HWND hWndCtrl)
{
	LRESULT lResult = 0;

	if (hWndCtrl == m_hWnd)
	{
		SCROLLINFO ScrollInfo;
		ScrollInfo.cbSize = sizeof(SCROLLINFO);
		ScrollInfo.fMask = SIF_ALL;
		::GetScrollInfo(m_hWnd, SB_CTL, &ScrollInfo);

		switch(nSBCode & 0x0000FFFF)
		{
		case SB_LINEUP:
			if (ScrollInfo.nPos > ScrollInfo.nMin)
			{
				ScrollInfo.nPos -= 1;
				m_ScrollBarTrackInfo.nTrackPos = ScrollInfo.nPos;
				ScrollInfo.fMask = SIF_POS;
				::SetScrollInfo(m_hWnd, SB_CTL, &ScrollInfo, TRUE);
			}
			break;
		case SB_LINEDOWN:
			if (ScrollInfo.nPos < ScrollInfo.nMax)
			{
				ScrollInfo.nPos += 1;
				m_ScrollBarTrackInfo.nTrackPos = ScrollInfo.nPos;
				ScrollInfo.fMask = SIF_POS;
				::SetScrollInfo(m_hWnd, SB_CTL, &ScrollInfo, TRUE);
			}
			break;
		case SB_PAGEUP:
			if (ScrollInfo.nPos > ScrollInfo.nMin)
			{
				ScrollInfo.nPos -= ScrollInfo.nPage;
				if (ScrollInfo.nPos < ScrollInfo.nMin)
					ScrollInfo.nPos = ScrollInfo.nMin;
				m_ScrollBarTrackInfo.nTrackPos = ScrollInfo.nPos;
				ScrollInfo.fMask = SIF_POS;
				::SetScrollInfo(m_hWnd, SB_CTL, &ScrollInfo, TRUE);
			}
			break;
		case SB_PAGEDOWN:
			if (ScrollInfo.nPos < ScrollInfo.nMax)
			{
				ScrollInfo.nPos += ScrollInfo.nPage;
				if (ScrollInfo.nPos > ScrollInfo.nMax)
					ScrollInfo.nPos = ScrollInfo.nMax;
				m_ScrollBarTrackInfo.nTrackPos = ScrollInfo.nPos;
				ScrollInfo.fMask = SIF_POS;
				::SetScrollInfo(m_hWnd, SB_CTL, &ScrollInfo, TRUE);
			}
			break;
		case SB_THUMBPOSITION:
			if (m_ScrollBarTrackInfo.nTrackPos != ScrollInfo.nPos)
			{
				ScrollInfo.nPos = m_ScrollBarTrackInfo.nTrackPos;
				if (ScrollInfo.nPos > ScrollInfo.nMax)
					ScrollInfo.nPos = ScrollInfo.nMax;
				if (ScrollInfo.nPos < ScrollInfo.nMin)
					ScrollInfo.nPos = ScrollInfo.nMin;
				ScrollInfo.fMask = SIF_POS;
				::SetScrollInfo(m_hWnd, SB_CTL, &ScrollInfo, FALSE);
			}
			nSBCode = SB_ENDSCROLL;
			break;
		}

		HWND hWndParent = ::GetParent(hWndCtrl);
		if (hWndParent)
			lResult = ::SendMessage(hWndParent, WM_VSCROLL, nSBCode, (LPARAM)hWndCtrl);
		else
			lResult = CDreamSkinWindow::DefWindowProc(m_hWnd, WM_VSCROLL, nSBCode, (LPARAM)hWndCtrl);
	}

	return lResult;
}

void CDreamSkinScrollBar::DrawHorzScrollBar(HDC hDC, DWORD dwStyle)
{
	SCROLLINFO ScrollInfo;
	SCROLLBARINFO ScrollBarInfo;
	RECT rcTop, rcBottom, rcClient, rcItem;
	int nStatus;

	ScrollInfo.cbSize = sizeof(SCROLLINFO);
	ScrollInfo.fMask = SIF_ALL;
	::GetScrollInfo(m_hWnd, SB_CTL, &ScrollInfo);

	RECT rcWindow;
	::GetClientRect(m_hWnd, &rcWindow);

	GetScrollBarInfo(rcWindow, SB_HORZ, &ScrollInfo, &ScrollBarInfo);
	if (ScrollBarInfo.rcScrollBar.bottom - ScrollBarInfo.rcScrollBar.top > 0)
	{
		SKINBACKGROUND *pBackground;
		if(dwStyle & WS_DISABLED)
			pBackground = &m_pSkinScrollBar->skinBkDisable;   //Disabled
		else
			pBackground = &m_pSkinScrollBar->skinHBkNormal;    //Normal
		DrawBackground(hDC, pBackground, rcWindow);

		if (ScrollInfo.nMax != ScrollInfo.nMin)
		{
			rcTop.left = ScrollBarInfo.rcScrollBar.left;
			rcTop.top = ScrollBarInfo.rcScrollBar.top;
			rcTop.bottom = ScrollBarInfo.rcScrollBar.bottom;
			rcTop.right = ScrollBarInfo.rcScrollBar.left + ScrollBarInfo.dxyLineButton;

			if (!m_pSkinScrollBar->nBtnLeftIncludeBorder)
			{
				rcTop.left += s_SkinScrollBar.skinHLBorderNormal.nWidth;
				rcTop.top += s_SkinScrollBar.skinHTBorderNormal.nWidth;
				rcTop.bottom -= s_SkinScrollBar.skinHBBorderNormal.nWidth;
			}

			rcBottom.left = ScrollBarInfo.rcScrollBar.right - ScrollBarInfo.dxyLineButton;
			rcBottom.top = ScrollBarInfo.rcScrollBar.top;
			rcBottom.bottom = ScrollBarInfo.rcScrollBar.bottom;
			rcBottom.right = ScrollBarInfo.rcScrollBar.right;

			if (!m_pSkinScrollBar->nBtnRightIncludeBorder)
			{
				rcBottom.top += s_SkinScrollBar.skinHTBorderNormal.nWidth;
				rcBottom.right -= s_SkinScrollBar.skinHRBorderNormal.nWidth;
				rcBottom.bottom -= s_SkinScrollBar.skinHBBorderNormal.nWidth;
			}

			rcClient.left = ScrollBarInfo.rcScrollBar.left + ScrollBarInfo.dxyLineButton;
			rcClient.top  = ScrollBarInfo.rcScrollBar.top;
			rcClient.bottom = ScrollBarInfo.rcScrollBar.bottom;
			rcClient.right = ScrollBarInfo.rcScrollBar.right - ScrollBarInfo.dxyLineButton;

			rcItem.left = ScrollBarInfo.rcScrollBar.left + ScrollBarInfo.xyThumbTop;
			rcItem.right = ScrollBarInfo.rcScrollBar.left + ScrollBarInfo.xyThumbBottom;
			rcItem.top = ScrollBarInfo.rcScrollBar.top;
			rcItem.bottom = ScrollBarInfo.rcScrollBar.bottom;
			if (!m_pSkinScrollBar->nHThumbIncludeBorder)
			{
				rcItem.top += s_SkinScrollBar.skinHTBorderNormal.nWidth;
				rcItem.bottom -= s_SkinScrollBar.skinHBBorderNormal.nWidth;
			}

			//draw top button
			if(dwStyle & WS_DISABLED)
				nStatus = DRAWSTATUS_DISABLE;       //Disabled
			else if(((m_nSBHover >> 16) == SB_HORZ) && ((m_nSBHover & 0x0000FFFF) == SBHT_ARROW_TOP))
				if(((m_ScrollBarTrackInfo.nSBLButtonDown >> 16) == SB_HORZ) && ((m_ScrollBarTrackInfo.nSBLButtonDown & 0x0000FFFF) == SBHT_ARROW_TOP))
					nStatus = DRAWSTATUS_PRESS;     //Pressed
				else
					nStatus = DRAWSTATUS_HOVER;     //Hover
			else
				nStatus = DRAWSTATUS_NORMAL;    //Normal
			DrawButton(hDC, rcTop, SB_HORZ, SBHT_ARROW_TOP, nStatus);

			//draw bottom button
			if(dwStyle & WS_DISABLED)
				nStatus = DRAWSTATUS_DISABLE;       //Disabled
			else if(((m_nSBHover >> 16) == SB_HORZ) && ((m_nSBHover & 0x0000FFFF) == SBHT_ARROW_BOTTOM))
				if(((m_ScrollBarTrackInfo.nSBLButtonDown >> 16) == SB_HORZ) && ((m_ScrollBarTrackInfo.nSBLButtonDown & 0x0000FFFF) == SBHT_ARROW_BOTTOM))
					nStatus = DRAWSTATUS_PRESS;     //Pressed
				else
					nStatus = DRAWSTATUS_HOVER;     //Hover
			else
				nStatus = DRAWSTATUS_NORMAL;    //Normal
			DrawButton(hDC, rcBottom, SB_HORZ, SBHT_ARROW_BOTTOM, nStatus);

			//draw client area
			if(dwStyle & WS_DISABLED)
				pBackground = &m_pSkinScrollBar->skinHBkDisable;   //Disabled
			else
				pBackground = &m_pSkinScrollBar->skinHBkNormal;    //Normal
			DrawBackground(hDC, pBackground, rcClient);

			//draw thumb
			if(dwStyle & WS_DISABLED)
				nStatus = DRAWSTATUS_DISABLE;       //Disabled
			else if(((m_nSBHover >> 16) == SB_HORZ) && ((m_nSBHover & 0x0000FFFF) == SBHT_THUMB))
				if(((m_ScrollBarTrackInfo.nSBLButtonDown >> 16) == SB_HORZ) && ((m_ScrollBarTrackInfo.nSBLButtonDown & 0x0000FFFF) == SBHT_THUMB))
					nStatus = DRAWSTATUS_PRESS;     //Pressed
				else
					nStatus = DRAWSTATUS_HOVER;     //Hover
			else
				if(((m_ScrollBarTrackInfo.nSBLButtonDown >> 16) == SB_HORZ) && ((m_ScrollBarTrackInfo.nSBLButtonDown & 0x0000FFFF) == SBHT_THUMB))
					nStatus = DRAWSTATUS_PRESS;     //Drag
				else
					nStatus = DRAWSTATUS_NORMAL;     //normal
			DrawThumb(hDC, rcItem, SB_HORZ, nStatus);

			//draw border
			if(dwStyle & WS_DISABLED)
				nStatus = DRAWSTATUS_DISABLE;   //Disabled
			else
				nStatus = DRAWSTATUS_NORMAL;    //Normal
			
			RECT rcWindow = ScrollBarInfo.rcScrollBar;
			if (m_pSkinScrollBar->nBtnLeftIncludeBorder)
			{
				rcWindow.left = rcTop.right;
			}

			if (m_pSkinScrollBar->nBtnRightIncludeBorder)
			{
				rcWindow.right = rcBottom.left;
			}
			DrawBorder(hDC, rcWindow, SB_HORZ, nStatus);
		}
	}
}

void CDreamSkinScrollBar::DrawVertScrollBar(HDC hDC, DWORD dwStyle)
{
	SCROLLINFO ScrollInfo;
	SCROLLBARINFO ScrollBarInfo;
	RECT rcTop, rcBottom, rcClient, rcItem;
	int nStatus;

	ScrollInfo.cbSize = sizeof(SCROLLINFO);
	ScrollInfo.fMask = SIF_ALL;
	::GetScrollInfo(m_hWnd, SB_CTL, &ScrollInfo);

	RECT rcWindow;
	::GetClientRect(m_hWnd, &rcWindow);

	GetScrollBarInfo(rcWindow, SB_VERT, &ScrollInfo, &ScrollBarInfo);
	if (ScrollBarInfo.rcScrollBar.right - ScrollBarInfo.rcScrollBar.left > 0)
	{
		SKINBACKGROUND *pBackground;
		if(dwStyle & WS_DISABLED)
			pBackground = &m_pSkinScrollBar->skinBkDisable;   //Disabled
		else
			pBackground = &m_pSkinScrollBar->skinHBkNormal;    //Normal
		DrawBackground(hDC, pBackground, rcWindow);

		if (ScrollInfo.nMax != ScrollInfo.nMin)
		{
			rcTop.left = ScrollBarInfo.rcScrollBar.left;
			rcTop.top = ScrollBarInfo.rcScrollBar.top;
			rcTop.bottom = ScrollBarInfo.rcScrollBar.top + ScrollBarInfo.dxyLineButton;;
			rcTop.right = ScrollBarInfo.rcScrollBar.right;

			if (!m_pSkinScrollBar->nBtnTopIncludeBorder)
			{
				rcTop.left += s_SkinScrollBar.skinVLBorderNormal.nWidth;
				rcTop.top += s_SkinScrollBar.skinVTBorderNormal.nWidth;
				rcTop.right -= s_SkinScrollBar.skinVRBorderNormal.nWidth;
			}

			rcBottom.left = ScrollBarInfo.rcScrollBar.left;
			rcBottom.top = ScrollBarInfo.rcScrollBar.bottom - ScrollBarInfo.dxyLineButton;
			rcBottom.bottom = ScrollBarInfo.rcScrollBar.bottom;
			rcBottom.right = ScrollBarInfo.rcScrollBar.right;

			if (!m_pSkinScrollBar->nBtnBottomIncludeBorder)
			{
				rcBottom.left += s_SkinScrollBar.skinVLBorderNormal.nWidth;
				rcBottom.right -= s_SkinScrollBar.skinVRBorderNormal.nWidth;
				rcBottom.bottom -= s_SkinScrollBar.skinVBBorderNormal.nWidth;
			}

			rcClient.left = ScrollBarInfo.rcScrollBar.left;
			rcClient.top  = ScrollBarInfo.rcScrollBar.top + ScrollBarInfo.dxyLineButton;
			rcClient.bottom = ScrollBarInfo.rcScrollBar.bottom - ScrollBarInfo.dxyLineButton;
			rcClient.right = ScrollBarInfo.rcScrollBar.right;

			rcItem.left = ScrollBarInfo.rcScrollBar.left;
			rcItem.right = ScrollBarInfo.rcScrollBar.right;
			rcItem.top = ScrollBarInfo.rcScrollBar.top + ScrollBarInfo.xyThumbTop;
			rcItem.bottom = ScrollBarInfo.rcScrollBar.top + ScrollBarInfo.xyThumbBottom;
			if (!m_pSkinScrollBar->nVThumbIncludeBorder)
			{
				rcItem.left += s_SkinScrollBar.skinVLBorderNormal.nWidth;
				rcItem.right -= s_SkinScrollBar.skinVRBorderNormal.nWidth;
			}

			//draw top button
			if(dwStyle & WS_DISABLED)
				nStatus = DRAWSTATUS_DISABLE;       //Disabled
			else if(((m_nSBHover >> 16) == SB_VERT) && ((m_nSBHover & 0x0000FFFF) == SBHT_ARROW_TOP))
				if(((m_ScrollBarTrackInfo.nSBLButtonDown >> 16) == SB_VERT) && ((m_ScrollBarTrackInfo.nSBLButtonDown & 0x0000FFFF) == SBHT_ARROW_TOP))
					nStatus = DRAWSTATUS_PRESS;     //Pressed
				else
					nStatus = DRAWSTATUS_HOVER;     //Hover
			else
				nStatus = DRAWSTATUS_NORMAL;    //Normal
			DrawButton(hDC, rcTop, SB_VERT, SBHT_ARROW_TOP, nStatus);

			//draw bottom button
			if(dwStyle & WS_DISABLED)
				nStatus = DRAWSTATUS_DISABLE;       //Disabled
			else if(((m_nSBHover >> 16) == SB_VERT) && ((m_nSBHover & 0x0000FFFF) == SBHT_ARROW_BOTTOM))
				if(((m_ScrollBarTrackInfo.nSBLButtonDown >> 16) == SB_VERT) && ((m_ScrollBarTrackInfo.nSBLButtonDown & 0x0000FFFF) == SBHT_ARROW_BOTTOM))
					nStatus = DRAWSTATUS_PRESS;     //Pressed
				else
					nStatus = DRAWSTATUS_HOVER;     //Hover
			else
				nStatus = DRAWSTATUS_NORMAL;    //Normal
			DrawButton(hDC, rcBottom, SB_VERT, SBHT_ARROW_BOTTOM, nStatus);

			//draw client area
			if(dwStyle & WS_DISABLED)
				pBackground = &m_pSkinScrollBar->skinVBkDisable;   //Disabled
			else
				pBackground = &m_pSkinScrollBar->skinVBkNormal;    //Normal
			DrawBackground(hDC, pBackground, rcClient);

			//draw thumb
			if(dwStyle & WS_DISABLED)
				nStatus = DRAWSTATUS_DISABLE;       //Disabled
			else if(((m_nSBHover >> 16) == SB_VERT) && ((m_nSBHover & 0x0000FFFF) == SBHT_THUMB))
				if(((m_ScrollBarTrackInfo.nSBLButtonDown >> 16) == SB_VERT) && ((m_ScrollBarTrackInfo.nSBLButtonDown & 0x0000FFFF) == SBHT_THUMB))
					nStatus = DRAWSTATUS_PRESS;     //Pressed
				else
					nStatus = DRAWSTATUS_HOVER;     //Hover
			else
				if(((m_ScrollBarTrackInfo.nSBLButtonDown >> 16) == SB_VERT) && ((m_ScrollBarTrackInfo.nSBLButtonDown & 0x0000FFFF) == SBHT_THUMB))
					nStatus = DRAWSTATUS_PRESS;     //Drag
				else
					nStatus = DRAWSTATUS_NORMAL;     //normal
			DrawThumb(hDC, rcItem, SB_VERT, nStatus);

			//draw border
			if(dwStyle & WS_DISABLED)
				nStatus = DRAWSTATUS_DISABLE;   //Disabled
			else
				nStatus = DRAWSTATUS_NORMAL;    //Normal
			
			RECT rcWindow = ScrollBarInfo.rcScrollBar;
			if (m_pSkinScrollBar->nBtnTopIncludeBorder)
			{
				rcWindow.top = rcTop.bottom;
			}

			if (m_pSkinScrollBar->nBtnBottomIncludeBorder)
			{
				rcWindow.bottom = rcBottom.top;
			}
			DrawBorder(hDC, rcWindow, SB_VERT, nStatus);
		}
	}
}

BOOL CDreamSkinScrollBar::GetScrollBarInfo(RECT rcWindow, int fnBar, LPSCROLLINFO lpsi, PSCROLLBARINFO psbi)
{
	int nClient, nThumb;

	psbi->rcScrollBar = rcWindow;
	
	RECT rcClient = rcWindow;
	::OffsetRect(&rcClient, 0 - rcWindow.left, 0 - rcWindow.top);
	if (lpsi->nMax == lpsi->nMin)
	{
		psbi->dxyLineButton = 0;
		psbi->xyThumbBottom = 0;
		psbi->xyThumbTop = 0;
	}
	else
	{
		if (fnBar == SB_HORZ)
		{
			if (m_pSkinScrollBar->skinBtnLeftNormal.nDrawType == DRAWTYPE_STRETCHBITMAP && m_pSkinScrollBar->skinBtnLeftNormal.imgDraw.hImage) 
				psbi->dxyLineButton = m_pSkinScrollBar->skinBtnLeftNormal.imgDraw.nWidth;
			else
				psbi->dxyLineButton = DEFAULT_SCROLLBAR_BUTTON_XY; //Hardcode value

			if ((rcClient.right - rcClient.left) <= (psbi->dxyLineButton * 2))
				psbi->dxyLineButton = (rcClient.right - rcClient.left) / 2;

			nClient = rcClient.right - rcClient.left - psbi->dxyLineButton * 2;

			nThumb = lpsi->nPage * nClient / (lpsi->nMax - lpsi->nMin);
			if (m_ScrollBarTrackInfo.bIsTracking && (lpsi->fMask & SIF_TRACKPOS))
			{
				if (m_ScrollBarTrackInfo.nTrackPos > lpsi->nMin)
					psbi->xyThumbTop = psbi->dxyLineButton + (m_ScrollBarTrackInfo.nTrackPos - lpsi->nMin) * nClient / (lpsi->nMax - lpsi->nMin);
				else
					psbi->xyThumbTop = psbi->dxyLineButton;
			}
			else
			{
				if (lpsi->nPos > lpsi->nMin)
					psbi->xyThumbTop = psbi->dxyLineButton + (lpsi->nPos - lpsi->nMin) * nClient / (lpsi->nMax - lpsi->nMin);
				else
					psbi->xyThumbTop = psbi->dxyLineButton;
			}
			if ((psbi->xyThumbTop + nThumb) > (psbi->dxyLineButton + nClient))
				psbi->xyThumbTop = psbi->dxyLineButton + nClient - nThumb;
			psbi->xyThumbBottom = psbi->xyThumbTop + nThumb;
			if (psbi->xyThumbTop < psbi->dxyLineButton)
				psbi->xyThumbTop = psbi->dxyLineButton;
		}
		else
		{
			if (m_pSkinScrollBar->skinBtnTopNormal.nDrawType == DRAWTYPE_STRETCHBITMAP && m_pSkinScrollBar->skinBtnTopNormal.imgDraw.hImage)
				psbi->dxyLineButton = m_pSkinScrollBar->skinBtnTopNormal.imgDraw.nHeight;
			else
				psbi->dxyLineButton = DEFAULT_SCROLLBAR_BUTTON_XY; //Hardcode value

			if ((rcClient.bottom - rcClient.top) <= (psbi->dxyLineButton * 2))
				psbi->dxyLineButton = (rcClient.bottom - rcClient.top) / 2;

			nClient = rcClient.bottom - rcClient.top - psbi->dxyLineButton * 2;

			nThumb = lpsi->nPage * nClient / (lpsi->nMax - lpsi->nMin);
			if (m_ScrollBarTrackInfo.bIsTracking && (lpsi->fMask & SIF_TRACKPOS))
			{
				if (m_ScrollBarTrackInfo.nTrackPos > lpsi->nMin)
					psbi->xyThumbTop = psbi->dxyLineButton + (m_ScrollBarTrackInfo.nTrackPos - lpsi->nMin) * nClient / (lpsi->nMax - lpsi->nMin);
				else
					psbi->xyThumbTop = psbi->dxyLineButton;
			}
			else
			{
				if (lpsi->nPos > lpsi->nMin)
					psbi->xyThumbTop = psbi->dxyLineButton + (lpsi->nPos - lpsi->nMin) * nClient / (lpsi->nMax - lpsi->nMin);
				else
					psbi->xyThumbTop = psbi->dxyLineButton;
			}
			if ((psbi->xyThumbTop + nThumb) > (psbi->dxyLineButton + nClient))
				psbi->xyThumbTop = psbi->dxyLineButton + nClient - nThumb;
			psbi->xyThumbBottom = psbi->xyThumbTop + nThumb;
			if (psbi->xyThumbTop < psbi->dxyLineButton)
				psbi->xyThumbTop = psbi->dxyLineButton;
		}
	}
	
	for (int i = 0; i <= CCHILDREN_SCROLLBAR; i++)
		psbi->rgstate[i] = 0;

	psbi->reserved = 0;

	return TRUE;
}
//DreamSkinScrollBar.cpp
#include <windows.h>

#include "WinGdiEx.h"
#include "DreamSkinMain.h"
#include "DreamSkinLoader.h"
#include "DreamSkinWindow.h"
#include "DreamSkinScrollBar.h"

//static member declaration
WNDPROC         CDreamSkinScrollBar::s_DefaultWindowProc = NULL;
SKINSCROLLBAR   CDreamSkinScrollBar::s_SkinScrollBar;

typedef int (WINAPI *SETSCROLLINFO)(HWND hwnd, int fnBar, LPCSCROLLINFO lpsi, BOOL fRedraw);

int WINAPI CDreamSkinScrollBar::SetScrollInfo(HWND hWnd, int fnBar, LPCSCROLLINFO lpsi, BOOL fRedraw)
{
	int nResult = 0;
	if (theSkinMain.m_HookSetScrollInfo.OrgAddr)
	{
		nResult = ((SETSCROLLINFO)theSkinMain.m_HookSetScrollInfo.OrgAddr)(hWnd, fnBar, lpsi, fRedraw);
		
		//send this message only when we hooked this window
		if (theSkinMain.GetHookedWindow(hWnd))
			::SendMessage(hWnd, WM_NCPAINT, 1, 0);
	}

	return nResult;
}

CDreamSkinScrollBar::CDreamSkinScrollBar(HWND hWnd, WNDPROC OrgWndProc)
	: CDreamSkinWindow(hWnd, OrgWndProc)
{
	m_pSkinScrollBar = &s_SkinScrollBar;
}

CDreamSkinScrollBar::~CDreamSkinScrollBar()
{
	if (m_pSkinScrollBar != &s_SkinScrollBar)
		delete m_pSkinScrollBar;
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
	int nResult = SBHT_NOWHERE;

	SCROLLBARINFO ScrollBarInfo;
	ScrollBarInfo.cbSize = sizeof(SCROLLBARINFO);
	
	if (fnBar == SB_HORZ)
	{
		::GetScrollBarInfo(hWnd, OBJID_HSCROLL, &ScrollBarInfo);
		if (::PtInRect(&ScrollBarInfo.rcScrollBar, point))
		{
			point.x = point.x - ScrollBarInfo.rcScrollBar.left;
			point.y = point.y - ScrollBarInfo.rcScrollBar.top;
			if (point.x < ScrollBarInfo.dxyLineButton)
				nResult = SBHT_ARROW_TOP;
			else if (point.x < ScrollBarInfo.xyThumbTop)
				nResult = SBHT_REGION_UP;
			else if (point.x < ScrollBarInfo.xyThumbBottom)
				nResult = SBHT_THUMB;
			else if (point.x < (ScrollBarInfo.rcScrollBar.right - ScrollBarInfo.rcScrollBar.left - ScrollBarInfo.dxyLineButton))
				nResult = SBHT_REGION_DOWN;
			else
				nResult = SBHT_ARROW_BOTTOM;
		}
	}
	else if (fnBar == SB_VERT)
	{
		::GetScrollBarInfo(hWnd, OBJID_VSCROLL, &ScrollBarInfo);
		if (::PtInRect(&ScrollBarInfo.rcScrollBar, point))
		{
			point.x = point.x - ScrollBarInfo.rcScrollBar.left;
			point.y = point.y - ScrollBarInfo.rcScrollBar.top;

			if (point.y < ScrollBarInfo.dxyLineButton)
				nResult = SBHT_ARROW_TOP;
			else if (point.y < ScrollBarInfo.xyThumbTop)
				nResult = SBHT_REGION_UP;
			else if (point.y < ScrollBarInfo.xyThumbBottom)
				nResult = SBHT_THUMB;
			else if (point.y < (ScrollBarInfo.rcScrollBar.bottom - ScrollBarInfo.rcScrollBar.top - ScrollBarInfo.dxyLineButton))
				nResult = SBHT_REGION_DOWN;
			else
				nResult = SBHT_ARROW_BOTTOM;
		}
	}

	return nResult;
}

void CDreamSkinScrollBar::TrackScrollBar(HWND hWnd, int fnBar, int nSBHitTest, UINT *pSBLButtonDown)
{
	SCROLLINFO ScrollInfo;
	ScrollInfo.cbSize = sizeof(SCROLLINFO);
	ScrollInfo.fMask = SIF_ALL;
	::GetScrollInfo(hWnd, fnBar, &ScrollInfo);

	UINT nScrollMsg;
	int nMsgCode;

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
	else if (fnBar == SB_VERT)
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

	BOOL bTrackingScorllBar = TRUE;
	::SetCapture(hWnd);

	if (nSBHitTest == SBHT_REGION_DOWN || nSBHitTest == SBHT_REGION_UP || nSBHitTest == SBHT_ARROW_TOP || nSBHitTest == SBHT_ARROW_BOTTOM)
	{
		::SendMessage(hWnd, nScrollMsg, nMsgCode, 0);

		int nTimer1 = ::SetTimer(0, 0, 500, NULL);
		int nTimer2 = 0;
		int nNewSBHitTest;

		while (::GetCapture() == hWnd && bTrackingScorllBar)
		{
			if (!::GetMessage(&Msg, NULL, 0, 0))
				break;

			switch(Msg.message)
			{
			case WM_LBUTTONUP:
				if (nTimer2 > 0)
					KillTimer(0, nTimer2);
				bTrackingScorllBar = FALSE;
				if (pSBLButtonDown)
					*pSBLButtonDown = 0;
				::SendMessage(hWnd, nScrollMsg, SB_ENDSCROLL, 0);
				::SendMessage(hWnd, WM_NCPAINT, 1, 0);
				break;
			case WM_TIMER:
				if (Msg.wParam == nTimer1)
				{
					::KillTimer(0, nTimer1);

					::GetCursorPos(&point);
					nNewSBHitTest = ScrollBarHitTest(hWnd, fnBar, point);
					if (nNewSBHitTest == nSBHitTest)
						::SendMessage(hWnd, nScrollMsg, nMsgCode, 0);
					nTimer2 = ::SetTimer(0, 0, 100, NULL);
				}
				else if (Msg.wParam == nTimer2)
				{
					::GetCursorPos(&point);
					nNewSBHitTest = ScrollBarHitTest(hWnd, fnBar, point);
					if (nNewSBHitTest == nSBHitTest)
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
		int nPos;
		::SendMessage(hWnd, WM_NCPAINT, 1, 0);

		SCROLLBARINFO ScrollBarInfo;
		ScrollBarInfo.cbSize = sizeof(SCROLLBARINFO);
		
		if (fnBar == SB_HORZ)
		{
			::GetScrollBarInfo(hWnd, OBJID_HSCROLL, &ScrollBarInfo);
		}
		else
		{
			::GetScrollBarInfo(hWnd, OBJID_VSCROLL, &ScrollBarInfo);
		}

		while (::GetCapture() == hWnd && bTrackingScorllBar)
		{
			if (!::GetMessage(&Msg, NULL, 0, 0))
				break;

			switch(Msg.message)
			{
			case WM_LBUTTONUP:
				bTrackingScorllBar = FALSE;
				::GetCursorPos(&point);
				nPos = ScrollBarDragPos(fnBar, &ScrollInfo, &ScrollBarInfo, point, pSBLButtonDown);
				if (pSBLButtonDown)
					*pSBLButtonDown = 0;
				::SendMessage(hWnd, nScrollMsg, MAKEWPARAM(SB_THUMBPOSITION, (unsigned short)nPos), 0);
				::SendMessage(hWnd, WM_NCPAINT, 1, 0);
				break;
			case WM_MOUSEMOVE:
				::GetCursorPos(&point);
				nPos = ScrollBarDragPos(fnBar, &ScrollInfo, &ScrollBarInfo, point, pSBLButtonDown);
				::SendMessage(hWnd, nScrollMsg, MAKEWPARAM(SB_THUMBTRACK, (unsigned short)nPos), 0);
				break;
			default:
				DispatchMessage(&Msg);
				break;
			}
		}
	}

	if (pSBLButtonDown)
		*pSBLButtonDown = 0;

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

	DrawBorder(hDC, pLBorder, pRBorder, pTBorder, pBBorder, rcWindow);
}

void CDreamSkinScrollBar::DrawBorder(HDC hDC, SKINBORDER *pLBorder, SKINBORDER *pRBorder, SKINBORDER *pTBorder, SKINBORDER *pBBorder, RECT rcDraw)
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

		if (fnBar == SB_HORZ)
		{
			if (nType == SBHT_ARROW_TOP) //draw lef button
			{
				pt[0].x = rcDraw.left + nWidth / 4;
				pt[0].y = rcDraw.top + nHeight / 2;
				pt[1].x = rcDraw.right - nWidth / 2;
				pt[1].y = rcDraw.top + nHeight / 4 - 1;
				pt[2].x = rcDraw.right - nWidth / 2;
				pt[2].y = rcDraw.bottom - nHeight / 4;
			}//draw left button
			else //draw right button
			{
				pt[0].x = rcDraw.right - nWidth / 4;
				pt[0].y = rcDraw.top + nHeight / 2;
				pt[1].x = rcDraw.left + nWidth / 2;
				pt[1].y = rcDraw.top + nHeight / 4 - 1;
				pt[2].x = rcDraw.left + nWidth / 2;
				pt[2].y = rcDraw.bottom - nHeight / 4;
			}//draw right button
		}
		else
		{
			if (nType == SBHT_ARROW_TOP) //draw top button
			{
				pt[0].x = rcDraw.left + nWidth / 2;
				pt[0].y = rcDraw.top + nHeight / 4;
				pt[1].x = rcDraw.right - nWidth / 4;
				pt[1].y = rcDraw.bottom - nHeight / 2;
				pt[2].x = rcDraw.left + nWidth / 4 - 1;
				pt[2].y = rcDraw.bottom - nHeight / 2;
			}//draw top button
			else //draw bottom button
			{
				pt[0].x = rcDraw.left + nWidth / 2;
				pt[0].y = rcDraw.bottom - nHeight / 4;
				pt[1].x = rcDraw.right - nWidth / 4;
				pt[1].y = rcDraw.top + nHeight / 2;
				pt[2].x = rcDraw.left + nWidth / 4 - 1;
				pt[2].y = rcDraw.top + nHeight / 2;
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

	DrawBorder(hDC, &pItem->skinLBorder, &pItem->skinRBorder, &pItem->skinTBorder, &pItem->skinBBorder, rcItem);

	if (((rcClient.bottom - rcClient.top) >= pItem->skinIcon.nHeight) && ((rcClient.right - rcClient.left) >= pItem->skinIcon.nWidth))
	{
		RECT rcIcon = GetItemRectIcon(pItem, rcClient, 0);
		DrawIcon(hDC, rcIcon, &pItem->skinIcon);
	}
	//if (wstrTitle)
	//	DrawTitle(hDC, &pItem->skinTxt, rcClient, wstrTitle);
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
		SKINBACKGROUND *pBackground;
		if(dwStyle & WS_DISABLED)
			pBackground = &s_SkinScrollBar.skinVBkDisable;   //Disabled
		else
			pBackground = &s_SkinScrollBar.skinVBkNormal;    //Normal
		DrawBackground(hDC, pBackground, rcClient);

		//draw thumb
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
		SKINBACKGROUND *pBackground;
		if(dwStyle & WS_DISABLED)
			pBackground = &s_SkinScrollBar.skinHBkDisable;   //Disabled
		else
			pBackground = &s_SkinScrollBar.skinHBkNormal;    //Normal
		DrawBackground(hDC, pBackground, rcClient);

		//draw thumb
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

int CDreamSkinScrollBar::ScrollBarDragPos(int fnBar, LPSCROLLINFO lpsi, LPSCROLLBARINFO psbi, POINT point, UINT *pSBLButtonDown)
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
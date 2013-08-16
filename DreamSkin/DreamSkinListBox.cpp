//DreamSkinListBox.cpp
#include <windows.h>

#include "WinGdiEx.h"
#include "DreamSkinMain.h"
#include "DreamSkinLoader.h"
#include "DreamSkinWindow.h"
#include "DreamSkinListBox.h"

//static member declaration
WNDPROC         CDreamSkinListBox::s_DefaultWindowProc = NULL;
SKINLISTBOX     CDreamSkinListBox::s_SkinListBox;

CDreamSkinListBox::CDreamSkinListBox(HWND hWnd, WNDPROC OrgWndProc)
	: CDreamSkinWindow(hWnd, OrgWndProc)
{
	m_pSkinListBox = &s_SkinListBox;
	m_nOwnerDraw = -1;
	m_nCurHover = -1;
	m_bTrackingScorllBar = FALSE;
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

		GetDefaultItem(&pSkinListBox->skinItemNormalSelected, RGB(0, 0, 128), RGB(255, 255, 255));
		GetDefaultItem(&pSkinListBox->skinItemDisableSelected, RGB(128, 128, 128), RGB(255, 255, 255));
		GetDefaultItem(&pSkinListBox->skinItemHoverSelected, RGB(0, 0, 128), RGB(255, 255, 255));

		GetDefaultItem(&pSkinListBox->skinItemNormalUnselected, RGB(255, 255, 255), RGB(0, 0, 0));
		GetDefaultItem(&pSkinListBox->skinItemDisableUnselected, RGB(236, 233, 216), RGB(0, 0, 0));
		GetDefaultItem(&pSkinListBox->skinItemHoverUnselected, RGB(0, 0, 255), RGB(255, 255, 255));
	}

	return TRUE;
}

VOID CALLBACK CDreamSkinListBox::OnTrackingScrollBarTimer(HWND hwnd, UINT uMsg, UINT_PTR idEvent,DWORD dwTime)
{
	KillTimer(hwnd, idEvent);
	PostThreadMessage(::GetCurrentThreadId(), WM_TIMER, idEvent, 0);
}

void CDreamSkinListBox::GetDefaultBackground(SKINBACKGROUND *pBackground, COLORREF clrBk)
{
	pBackground->nDefault = 1;
	pBackground->nDrawType = DRAWTYPE_FILLCOLOR;
	pBackground->clrDraw.clrStart = clrBk;
	memset(&(pBackground->imgDraw), 0, sizeof(DRAWIMAGE));
}

void CDreamSkinListBox::GetDefaultBorder(SKINBORDER *pBorder, COLORREF clrBk, int nWidth)
{
	pBorder->nDefault = 1;
	pBorder->nDrawType = DRAWTYPE_FILLCOLOR;
	pBorder->nWidth = nWidth;
	pBorder->nStart = 0;
	pBorder->nEnd = 0;
	pBorder->clrDraw.clrStart = clrBk;
	memset(&(pBorder->imgDraw), 0, sizeof(DRAWIMAGE));
}

void CDreamSkinListBox::GetDefaultIcon(SKINICON *pIcon, COLORREF clrDraw)
{
	pIcon->nDefault = 1;
	pIcon->nDrawType = DRAWTYPE_CUSTOMIZE;
	
	pIcon->nHeight = 13;
	pIcon->nWidth = 13;
	pIcon->nPadding = 2;

	pIcon->clrDraw.clrStart = clrDraw;
	memset(&(pIcon->imgDraw), 0, sizeof(DRAWIMAGE));
}

void CDreamSkinListBox::GetDefaultItem(SKINITEM *pItem, COLORREF clrBk, COLORREF clrTxt)
{
	pItem->nDefault = 1;
	GetDefaultBackground(&pItem->skinBk, clrBk);

	GetDefaultBorder(&pItem->skinLBorder, RGB(0, 0, 0), 0);
	GetDefaultBorder(&pItem->skinRBorder, RGB(0, 0, 0), 0);
	GetDefaultBorder(&pItem->skinTBorder, RGB(0, 0, 0), 0);
	GetDefaultBorder(&pItem->skinBBorder, RGB(0, 0, 0), 0);

	GetDefaultText(&pItem->skinTxt, clrTxt);
	GetDefaultIcon(&pItem->skinIcon, clrTxt);
}

void CDreamSkinListBox::GetDefaultText(SKINTEXT *pText, COLORREF clrTxt)
{
	pText->nDefault = 1;
	pText->clrDraw = clrTxt;
	pText->bDrawShadow = FALSE;
	pText->clrShadow = RGB(255, 255, 255);

	pText->skinFont.nFontSize = 8;
	pText->skinFont.nBold = 0;
	wcscpy_s(pText->skinFont.wstrFontName, LF_FACESIZE, L"Microsoft Sans Serif");
}

LRESULT CDreamSkinListBox::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT nResult = ERROR_SUCCESS;


	switch(message)
	{
	case WM_COMPAREITEM:
		//nResult = CDreamSkinWindow::DefWindowProc(message, wParam, lParam);
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
		m_bTrackingScorllBar = FALSE;
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
	case WM_NCLBUTTONDBLCLK:
		if (wParam == HTVSCROLL || wParam == HTHSCROLL)
		{
			switch(ScrollBarHitTest(wParam == HTVSCROLL ? SB_VERT : SB_HORZ, MAKEPOINTS(lParam)))
			{
			case SBHT_THUMB:
				TrackScrollBar(wParam == HTVSCROLL ? SB_VERT : SB_HORZ, SBHT_THUMB);
				break;
			case SBHT_REGION_UP:
				TrackScrollBar(wParam == HTVSCROLL ? SB_VERT : SB_HORZ, SBHT_REGION_UP);
				break;
			case SBHT_REGION_DOWN:
				TrackScrollBar(wParam == HTVSCROLL ? SB_VERT : SB_HORZ, SBHT_REGION_DOWN);
				break;
			case SBHT_ARROW_TOP:
				TrackScrollBar(wParam == HTVSCROLL ? SB_VERT : SB_HORZ, SBHT_ARROW_TOP);
				break;
			case SBHT_ARROW_BOTTOM:
				TrackScrollBar(wParam == HTVSCROLL ? SB_VERT : SB_HORZ, SBHT_ARROW_BOTTOM);
				break;
			default:
				nResult = CDreamSkinWindow::DefWindowProc(message, wParam, lParam);
				OnNcPaint(NULL);
				break;
			}
		}
		else
		{
			nResult = CDreamSkinWindow::DefWindowProc(message, wParam, lParam);
		}
		break;
	case WM_NCLBUTTONDOWN:
		if (wParam == HTVSCROLL || wParam == HTHSCROLL)
		{
			switch(ScrollBarHitTest(wParam == HTVSCROLL ? SB_VERT : SB_HORZ, MAKEPOINTS(lParam)))
			{
			case SBHT_THUMB:
				TrackScrollBar(wParam == HTVSCROLL ? SB_VERT : SB_HORZ, SBHT_THUMB);
				break;
			case SBHT_REGION_UP:
				TrackScrollBar(wParam == HTVSCROLL ? SB_VERT : SB_HORZ, SBHT_REGION_UP);
				break;
			case SBHT_REGION_DOWN:
				TrackScrollBar(wParam == HTVSCROLL ? SB_VERT : SB_HORZ, SBHT_REGION_DOWN);
				break;
			case SBHT_ARROW_TOP:
				TrackScrollBar(wParam == HTVSCROLL ? SB_VERT : SB_HORZ, SBHT_ARROW_TOP);
				break;
			case SBHT_ARROW_BOTTOM:
				TrackScrollBar(wParam == HTVSCROLL ? SB_VERT : SB_HORZ, SBHT_ARROW_BOTTOM);
				break;
			default:
				nResult = CDreamSkinWindow::DefWindowProc(message, wParam, lParam);
				OnNcPaint(NULL);
				break;
			}
		}
		else
		{
			nResult = CDreamSkinWindow::DefWindowProc(message, wParam, lParam);
		}
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

LRESULT CDreamSkinListBox::OnNcPaint(HRGN hRGN)
{
	HDC hWindowDC;
	hWindowDC = ::GetWindowDC(m_hWnd);
    // Paint into this DC

	RECT rcWindow;
	::GetWindowRect(m_hWnd, &rcWindow);

	DWORD dwStyle = ::GetWindowLong(m_hWnd, GWL_STYLE);
	SCROLLINFO ScrollInfo;
	SCROLLBARINFO ScrollBarInfo;
	if (dwStyle & WS_VSCROLL)
	{
		ScrollInfo.cbSize = sizeof(SCROLLINFO);
		ScrollInfo.fMask = SIF_ALL;
		::GetScrollInfo(m_hWnd, SB_VERT, &ScrollInfo);

		ScrollBarInfo.cbSize = sizeof(SCROLLBARINFO);
		::GetScrollBarInfo(m_hWnd, OBJID_VSCROLL, &ScrollBarInfo);

		::OffsetRect(&ScrollBarInfo.rcScrollBar, 0 - rcWindow.left, 0 - rcWindow.top);
		DrawScrollBar(hWindowDC, &ScrollInfo, &ScrollBarInfo);
	}

	::OffsetRect(&rcWindow, 0 - rcWindow.left, 0 - rcWindow.top);

	DWORD dwExStyle = ::GetWindowLong(m_hWnd, GWL_EXSTYLE);
	if (dwExStyle & WS_EX_CLIENTEDGE)
		DrawBorder(hWindowDC, rcWindow);

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

void CDreamSkinListBox::DrawBorder(HDC hDC, SKINBORDER *pLBorder, SKINBORDER *pRBorder, SKINBORDER *pTBorder, SKINBORDER *pBBorder, RECT rcDraw)
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

void CDreamSkinListBox::DrawScrollBar(HDC hDC, LPSCROLLINFO lpsi, PSCROLLBARINFO psbi)
{
	RECT rcTop, rcBottom, rcClient, rcItem;
	rcTop.left = psbi->rcScrollBar.left;
	rcTop.top = psbi->rcScrollBar.top;
	rcTop.bottom = psbi->dxyLineButton;
	rcTop.right = psbi->rcScrollBar.right;

	rcBottom.left = psbi->rcScrollBar.left;
	rcBottom.top = psbi->rcScrollBar.bottom - psbi->dxyLineButton;
	rcBottom.bottom = psbi->rcScrollBar.bottom;
	rcBottom.right = psbi->rcScrollBar.right;

	::FillSolidRect(hDC, &rcTop, RGB(0, 255, 0));
	::FillSolidRect(hDC, &rcBottom, RGB(0, 255, 0));

	rcClient.left = psbi->rcScrollBar.left;
	rcClient.top  = psbi->dxyLineButton;
	rcClient.bottom = psbi->rcScrollBar.bottom - psbi->dxyLineButton;
	rcClient.right = psbi->rcScrollBar.right;
	::FillSolidRect(hDC, &rcClient, RGB(0, 0, 255));

	rcItem.left = psbi->rcScrollBar.left + 1;
	rcItem.right = psbi->rcScrollBar.right -1;
	rcItem.top = psbi->xyThumbTop;
	rcItem.bottom = psbi->xyThumbBottom;
	::FillSolidRect(hDC, &rcItem, RGB(0, 0, 128));

	::Draw3dRect(hDC, &psbi->rcScrollBar, RGB(255, 0, 0), RGB(255, 0, 0));
}

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

	DrawBorder(hDC, &pItem->skinLBorder, &pItem->skinRBorder, &pItem->skinTBorder, &pItem->skinBBorder, rcItem);

	if (wstrTitle)
		DrawTitle(hDC, &pItem->skinTxt, rcClient, wstrTitle);
}

RECT CDreamSkinListBox::GetItemRectClient(SKINITEM *pItem, RECT rcItem) const
{
	RECT rcClient;

	rcClient = rcItem;
	rcClient.left = rcClient.left + pItem->skinLBorder.nWidth;
	rcClient.right = rcClient.right - pItem->skinRBorder.nWidth;
	rcClient.top = rcClient.top + pItem->skinTBorder.nWidth;
	rcClient.bottom = rcClient.bottom - pItem->skinBBorder.nWidth;

	return rcClient;
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

int CDreamSkinListBox::ScrollBarDragPos(int fnBar, LPSCROLLINFO lpsi, LPSCROLLBARINFO psbi, POINT point)
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
		nPos = lpsi->nPos;
	}

	return nPos;
}

int CDreamSkinListBox::ScrollBarHitTest(int fnBar, POINTS point)
{
	POINT pt;
	pt.x = point.x;
	pt.y = point.y;

	return ScrollBarHitTest(fnBar, pt);
}

int CDreamSkinListBox::ScrollBarHitTest(int fnBar, POINT point)
{
	int nResult = SBHT_NOWHERE;

	SCROLLBARINFO ScrollBarInfo;
	ScrollBarInfo.cbSize = sizeof(SCROLLBARINFO);
	
	if (fnBar == SB_HORZ)
	{
		::GetScrollBarInfo(m_hWnd, OBJID_HSCROLL, &ScrollBarInfo);
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
		::GetScrollBarInfo(m_hWnd, OBJID_VSCROLL, &ScrollBarInfo);
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

void CDreamSkinListBox::TrackScrollBar(int fnBar, int nSBHitTest)
{
	SCROLLINFO ScrollInfo;
	ScrollInfo.cbSize = sizeof(SCROLLINFO);
	ScrollInfo.fMask = SIF_ALL;
	::GetScrollInfo(m_hWnd, fnBar, &ScrollInfo);

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
	int nNewSBHitTest;

	m_bTrackingScorllBar = TRUE;
	::SetCapture(m_hWnd);

	if (nSBHitTest == SBHT_REGION_DOWN || nSBHitTest == SBHT_REGION_UP || nSBHitTest == SBHT_ARROW_TOP || nSBHitTest == SBHT_ARROW_BOTTOM)
	{
		::SendMessage(m_hWnd, nScrollMsg, nMsgCode, 0);
		int nTimer1 = ::SetTimer(0, 0, 500, NULL);
		int nTimer2 = 0;
		while (::GetCapture() == m_hWnd && m_bTrackingScorllBar)
		{
			if (!::GetMessage(&Msg, NULL, 0, 0))
				break;

			switch(Msg.message)
			{
			case WM_LBUTTONUP:
				if (nTimer2 > 0)
					KillTimer(0, nTimer2);
				m_bTrackingScorllBar = FALSE;
				::SendMessage(m_hWnd, nScrollMsg, SB_ENDSCROLL, 0);
				break;
			case WM_TIMER:
				if (Msg.wParam == nTimer1)
				{
					::KillTimer(0, nTimer1);

					::GetCursorPos(&point);
					nNewSBHitTest = ScrollBarHitTest(fnBar, point);
					if (nNewSBHitTest == nSBHitTest)
						::SendMessage(m_hWnd, nScrollMsg, nMsgCode, 0);
					nTimer2 = ::SetTimer(0, 0, 100, NULL);
				}
				else if (Msg.wParam == nTimer2)
				{
					::GetCursorPos(&point);
					nNewSBHitTest = ScrollBarHitTest(fnBar, point);
					if (nNewSBHitTest == nSBHitTest)
						::SendMessage(m_hWnd, nScrollMsg, nMsgCode, 0);
				}
				else
				{
					DispatchMessage(&Msg);
				}
				break;
			default:
				DispatchMessage(&Msg);
				break;
			}
		}
	}
	else
	{
		int nPos;

		SCROLLBARINFO ScrollBarInfo;
		ScrollBarInfo.cbSize = sizeof(SCROLLBARINFO);
		
		if (fnBar == SB_HORZ)
		{
			::GetScrollBarInfo(m_hWnd, OBJID_HSCROLL, &ScrollBarInfo);
		}
		else
		{
			::GetScrollBarInfo(m_hWnd, OBJID_VSCROLL, &ScrollBarInfo);
		}

		while (::GetCapture() == m_hWnd && m_bTrackingScorllBar)
		{
			if (!::GetMessage(&Msg, NULL, 0, 0))
				break;

			switch(Msg.message)
			{
			case WM_LBUTTONUP:
				m_bTrackingScorllBar = FALSE;
				nPos = ScrollBarDragPos(fnBar, &ScrollInfo, &ScrollBarInfo, point);
				::SendMessage(m_hWnd, nScrollMsg, MAKEWPARAM(SB_THUMBPOSITION, (unsigned short)nPos), 0);
				break;
			case WM_MOUSEMOVE:
				::GetCursorPos(&point);
				nPos = ScrollBarDragPos(fnBar, &ScrollInfo, &ScrollBarInfo, point);
				::SendMessage(m_hWnd, nScrollMsg, MAKEWPARAM(SB_THUMBTRACK, (unsigned short)nPos), 0);
				break;
			default:
				DispatchMessage(&Msg);
				break;
			}
		}
	}

	m_bTrackingScorllBar = FALSE;
	if (::IsWindow(m_hWnd) && ::GetCapture() == m_hWnd)
		::ReleaseCapture();
}
//DreamSkinDialog.cpp
#include <windows.h>

#include "WinGdiEx.h"
#include "DreamSkinMain.h"
#include "DreamSkinLoader.h"
#include "DreamSkinWindow.h"
#include "DreamSkinDialog.h"


//static member declaration
WNDPROC         CDreamSkinDialog::s_DefaultWindowProc = NULL;
SKINDIALOG      CDreamSkinDialog::s_SkinDialog;

CDreamSkinDialog::CDreamSkinDialog(HWND hWnd, WNDPROC OrgWndProc)
	: CDreamSkinWindow(hWnd, OrgWndProc)
{
	m_pSkinDialog = &s_SkinDialog;

	m_bActive = TRUE;
	m_nLastHitTest = 0;
	m_nNcLButtonDown = 0;
}

CDreamSkinDialog::~CDreamSkinDialog()
{
	if (m_pSkinDialog != &s_SkinDialog)
		delete m_pSkinDialog;
}

void CDreamSkinDialog::Reload()
{
	int nWidthGap, nHeightGap;
	if (m_hWnd && ::IsWindow(m_hWnd))
	{
		RECT rcWindow, rcClient;
		::GetWindowRect(m_hWnd, &rcWindow);
		::GetClientRect(m_hWnd, &rcClient);

		DWORD dwStyle = ::GetWindowLong(m_hWnd, GWL_STYLE);
		if((dwStyle & WS_BORDER) || (dwStyle & WS_THICKFRAME) || (dwStyle & DS_MODALFRAME))
		{
			nWidthGap = m_pSkinDialog->skinLBorderActive.nWidth + m_pSkinDialog->skinRBorderActive.nWidth - ((rcWindow.right - rcWindow.left) - (rcClient.right - rcClient.left));

			if((dwStyle & WS_BORDER) && (dwStyle & WS_DLGFRAME))  // WS_CAPTION
			{
				if (m_pSkinDialog->skinTitleBar.nIncBorder)
					nHeightGap = m_pSkinDialog->skinBBorderActive.nWidth + m_pSkinDialog->skinTitleBar.nWidth - ((rcWindow.bottom - rcWindow.top) - (rcClient.bottom - rcClient.top));
				else
					nHeightGap = m_pSkinDialog->skinTBorderActive.nWidth + m_pSkinDialog->skinBBorderActive.nWidth + m_pSkinDialog->skinTitleBar.nWidth - ((rcWindow.bottom - rcWindow.top) - (rcClient.bottom - rcClient.top));
			}
			else
			{
				nHeightGap = m_pSkinDialog->skinTBorderActive.nWidth + m_pSkinDialog->skinBBorderActive.nWidth - ((rcWindow.bottom - rcWindow.top) - (rcClient.bottom - rcClient.top));
			}
		}

		if(nWidthGap || nHeightGap)
		{
			if (m_bActive)
				::SetWindowPos(m_hWnd, 0, 0, 0, rcWindow.right - rcWindow.left + nWidthGap, rcWindow.bottom - rcWindow.top + nHeightGap, SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED);
			else
				::SetWindowPos(m_hWnd, 0, 0, 0, rcWindow.right - rcWindow.left + nWidthGap, rcWindow.bottom - rcWindow.top + nHeightGap, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED);
		}
		else
		{
			if (m_bActive)
				::SetWindowPos(m_hWnd, 0, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED );
			else
				::SetWindowPos(m_hWnd, 0, 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED );
		}

		m_bCreateRegion = TRUE;
		::InvalidateRect(m_hWnd, &rcClient, TRUE);
		::UpdateWindow(m_hWnd);
	}
}

BOOL CDreamSkinDialog::InitialClass()
{
	//Get the default window proc for this class
	WNDCLASSW clsDialogDefault;
	if(!GetClassInfoW(GetModuleHandle(NULL), DEFAULT_DIALOG_CLASSNAME_W, &clsDialogDefault))
	{
		//Get class info failed
		//TODO: add error handle code
		return FALSE;
	}
	s_DefaultWindowProc = clsDialogDefault.lpfnWndProc;

	GetDefaultSkin(&s_SkinDialog);

	return TRUE;
}

void CDreamSkinDialog::DestroyClass()
{

}

BOOL CDreamSkinDialog::GetDefaultSkin(SKINDIALOG *pSkinDialog)
{
	if (pSkinDialog)
	{
		pSkinDialog->nRgnType = RGN_RECT;
		pSkinDialog->clrTansparent = RGB(255, 255, 255);

		GetDefaultBackground(&(pSkinDialog->skinBkActive), ::GetSysColor(COLOR_BTNFACE));
		GetDefaultBackground(&(pSkinDialog->skinBkInactive), ::GetSysColor(COLOR_BTNFACE));

		GetDefaultBorder(&(pSkinDialog->skinLBorderActive), ::GetSysColor(COLOR_ACTIVEBORDER), ::GetSystemMetrics(SM_CXDLGFRAME));
		GetDefaultBorder(&(pSkinDialog->skinLBorderInactive), ::GetSysColor(COLOR_INACTIVEBORDER), ::GetSystemMetrics(SM_CXDLGFRAME));
		GetDefaultBorder(&(pSkinDialog->skinRBorderActive), ::GetSysColor(COLOR_ACTIVEBORDER), ::GetSystemMetrics(SM_CXDLGFRAME));
		GetDefaultBorder(&(pSkinDialog->skinRBorderInactive), ::GetSysColor(COLOR_INACTIVEBORDER), ::GetSystemMetrics(SM_CXDLGFRAME));
		GetDefaultBorder(&(pSkinDialog->skinTBorderActive), ::GetSysColor(COLOR_ACTIVEBORDER), ::GetSystemMetrics(SM_CYDLGFRAME));
		GetDefaultBorder(&(pSkinDialog->skinTBorderInactive), ::GetSysColor(COLOR_INACTIVEBORDER), ::GetSystemMetrics(SM_CYDLGFRAME));
		GetDefaultBorder(&(pSkinDialog->skinBBorderActive), ::GetSysColor(COLOR_ACTIVEBORDER), ::GetSystemMetrics(SM_CYDLGFRAME));
		GetDefaultBorder(&(pSkinDialog->skinBBorderInactive), ::GetSysColor(COLOR_INACTIVEBORDER), ::GetSystemMetrics(SM_CYDLGFRAME));
		
		GetDefaultTitleBar(&(pSkinDialog->skinTitleBar));

		pSkinDialog->hTitleFont = NULL;
		
		pSkinDialog->nTitleMargin = 4;
		pSkinDialog->nIconMargin = 4;

		pSkinDialog->nBtnPadding = 4;
	}

	return TRUE;
}

BOOL CDreamSkinDialog::ApplySkin(CDreamSkinLoader *pLoader)
{
	BOOL bResult = TRUE;

	InitialClass();

	SKINDIALOG SkinDialog;
	pLoader->GetSkinDialog(&SkinDialog);
	s_SkinDialog = SkinDialog;

	return bResult;
}

CDreamSkinDialog* CDreamSkinDialog::CreateInstance(HWND hWnd, WNDPROC OrgWndProc)
{
	CDreamSkinDialog *pSkinDialog = new CDreamSkinDialog(hWnd, OrgWndProc);
	return pSkinDialog;
}

LRESULT WINAPI CDreamSkinDialog::DefWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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

void CDreamSkinDialog::GetDefaultTitleBar(SKINTITLEBAR *pTitleBar)
{
	pTitleBar->nDefault = 1;
	pTitleBar->nIncBorder = 0;
	pTitleBar->nWidth = ::GetSystemMetrics(SM_CYSIZE);
	pTitleBar->nStart = 0;
	pTitleBar->nEnd = 0;
	pTitleBar->clrTxtActive = ::GetSysColor(COLOR_CAPTIONTEXT);
	pTitleBar->clrTxtInactive = ::GetSysColor(COLOR_INACTIVECAPTIONTEXT);
	GetDefaultBackground(&(pTitleBar->skinBkActive), ::GetSysColor(COLOR_ACTIVECAPTION));
	GetDefaultBackground(&(pTitleBar->skinBkInactive), ::GetSysColor(COLOR_INACTIVECAPTION));

	pTitleBar->nHeightClose = 16;
	pTitleBar->nWidthClose = 16;
	pTitleBar->nXMarginClose = 4;
	pTitleBar->nYMarginClose = 4;
	GetDefaultSysButton(&(pTitleBar->skinCloseNormal), ::GetSysColor(COLOR_ACTIVECAPTION) & 0xFF000000, ::GetSysColor(COLOR_INACTIVECAPTIONTEXT), ::GetSysColor(COLOR_INACTIVECAPTIONTEXT));
	GetDefaultSysButton(&(pTitleBar->skinCloseHover), ::GetSysColor(COLOR_ACTIVECAPTION) & 0xFF000000, ::GetSysColor(COLOR_HIGHLIGHTTEXT), ::GetSysColor(COLOR_HIGHLIGHTTEXT));
	GetDefaultSysButton(&(pTitleBar->skinClosePress), ::GetSysColor(COLOR_ACTIVECAPTION) & 0xFF000000, ::GetSysColor(COLOR_BTNSHADOW), ::GetSysColor(COLOR_BTNSHADOW));
	GetDefaultSysButton(&(pTitleBar->skinCloseDisable), ::GetSysColor(COLOR_INACTIVECAPTION) & 0xFF000000, ::GetSysColor(COLOR_INACTIVECAPTIONTEXT), ::GetSysColor(COLOR_INACTIVECAPTIONTEXT));

	pTitleBar->nHeightMax = 16;
	pTitleBar->nWidthMax = 16;
	pTitleBar->nXMarginMax = 4;
	pTitleBar->nYMarginMax = 4;
	GetDefaultSysButton(&(pTitleBar->skinMaxNormal), ::GetSysColor(COLOR_ACTIVECAPTION) & 0xFF000000, ::GetSysColor(COLOR_INACTIVECAPTIONTEXT), ::GetSysColor(COLOR_INACTIVECAPTIONTEXT));
	GetDefaultSysButton(&(pTitleBar->skinMaxHover), ::GetSysColor(COLOR_ACTIVECAPTION) & 0xFF000000, ::GetSysColor(COLOR_HIGHLIGHTTEXT), ::GetSysColor(COLOR_HIGHLIGHTTEXT));
	GetDefaultSysButton(&(pTitleBar->skinMaxPress), ::GetSysColor(COLOR_ACTIVECAPTION) & 0xFF000000, ::GetSysColor(COLOR_BTNSHADOW), ::GetSysColor(COLOR_BTNSHADOW));
	GetDefaultSysButton(&(pTitleBar->skinMaxDisable), ::GetSysColor(COLOR_INACTIVECAPTION) & 0xFF000000, ::GetSysColor(COLOR_INACTIVECAPTIONTEXT), ::GetSysColor(COLOR_INACTIVECAPTIONTEXT));
	GetDefaultSysButton(&(pTitleBar->skinRestoreNormal), ::GetSysColor(COLOR_ACTIVECAPTION) & 0xFF000000, ::GetSysColor(COLOR_INACTIVECAPTIONTEXT), ::GetSysColor(COLOR_INACTIVECAPTIONTEXT));
	GetDefaultSysButton(&(pTitleBar->skinRestoreHover), ::GetSysColor(COLOR_ACTIVECAPTION) & 0xFF000000, ::GetSysColor(COLOR_HIGHLIGHTTEXT), ::GetSysColor(COLOR_HIGHLIGHTTEXT));
	GetDefaultSysButton(&(pTitleBar->skinRestorePress), ::GetSysColor(COLOR_ACTIVECAPTION) & 0xFF000000, ::GetSysColor(COLOR_BTNSHADOW), ::GetSysColor(COLOR_BTNSHADOW));
	GetDefaultSysButton(&(pTitleBar->skinRestoreDisable), ::GetSysColor(COLOR_INACTIVECAPTION) & 0xFF000000, ::GetSysColor(COLOR_INACTIVECAPTIONTEXT), ::GetSysColor(COLOR_INACTIVECAPTIONTEXT));

	pTitleBar->nHeightMin = 16;
	pTitleBar->nWidthMin = 16;
	pTitleBar->nXMarginMin = 4;
	pTitleBar->nYMarginMin = 4;
	GetDefaultSysButton(&(pTitleBar->skinMinNormal), ::GetSysColor(COLOR_ACTIVECAPTION) & 0xFF000000, ::GetSysColor(COLOR_INACTIVECAPTIONTEXT), ::GetSysColor(COLOR_INACTIVECAPTIONTEXT));
	GetDefaultSysButton(&(pTitleBar->skinMinHover), ::GetSysColor(COLOR_ACTIVECAPTION) & 0xFF000000, ::GetSysColor(COLOR_HIGHLIGHTTEXT), ::GetSysColor(COLOR_HIGHLIGHTTEXT));
	GetDefaultSysButton(&(pTitleBar->skinMinPress), ::GetSysColor(COLOR_ACTIVECAPTION) & 0xFF000000, ::GetSysColor(COLOR_BTNSHADOW), ::GetSysColor(COLOR_BTNSHADOW));
	GetDefaultSysButton(&(pTitleBar->skinMinDisable), ::GetSysColor(COLOR_INACTIVECAPTION) & 0xFF000000, ::GetSysColor(COLOR_INACTIVECAPTIONTEXT), ::GetSysColor(COLOR_INACTIVECAPTIONTEXT));
}

LRESULT CDreamSkinDialog::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT nResult = ERROR_SUCCESS;
	//NCCALCSIZE_PARAMS *pCac;
	//CREATESTRUCT *pCreate;
	//int aa = 0;

	switch(message)
	{
	case WM_ACTIVATE:
		nResult = OnActive((UINT)wParam, (HANDLE)lParam);
		break;
	case WM_COMPAREITEM:
		break;
	case WM_CTLCOLOREDIT:
		nResult = ::SendMessage((HWND)lParam, WM_CTLCOLOREDIT, wParam, (LPARAM)m_hWnd);
		break;
	case WM_CTLCOLORSTATIC:
		nResult = ::SendMessage((HWND)lParam, WM_CTLCOLORSTATIC, wParam, (LPARAM)m_hWnd);
		break;
	case WM_CTLCOLORLISTBOX:
		nResult = ::SendMessage((HWND)lParam, WM_CTLCOLORLISTBOX, wParam, (LPARAM)m_hWnd);
		break;
	case WM_DRAWITEM:
		nResult = OnDrawItem((UINT)wParam, (LPDRAWITEMSTRUCT)lParam);
		break;
	case WM_MEASUREITEM:
		nResult = OnMesureItem((UINT)wParam, (LPMEASUREITEMSTRUCT)lParam);
		break;
	case WM_NCACTIVATE:
		nResult = OnNcActive((BOOL)wParam);
		break;
	case WM_CREATE:
		nResult = OnCreate((LPCREATESTRUCT)lParam);
		break;
	case WM_NCCALCSIZE:
		nResult = OnNcCalcSize((BOOL)wParam, (NCCALCSIZE_PARAMS*)lParam);
		break;
	case WM_NCHITTEST:
		nResult = OnNcHitTest(MAKEPOINTS(lParam));
		break;
	case WM_NCLBUTTONDOWN:
		nResult = OnNcLButtonDown(wParam, MAKEPOINTS(lParam));
		break;
	case WM_NCLBUTTONUP:
		nResult = OnNcLButtonUp(wParam, MAKEPOINTS(lParam));
		break;
	case WM_NCMOUSEMOVE:
		nResult = OnNcMouseMove((UINT)wParam, MAKEPOINTS(lParam));
		break;
	case WM_NCPAINT:
		nResult = OnNcPaint((HRGN)wParam);
		break;
	case WM_PAINT:
		nResult = OnPaint();
		break;
	case WM_SIZE:
		nResult = OnSize(wParam, LOWORD(lParam), HIWORD(lParam));
		break;
	case 174:
		break;
	//case WM_SIZING:
	//	nResult = 0;
	////	break;
	//case WM_SIZE:
	//	nResult = CDreamSkinWindow::DefWindowProc(message, wParam, lParam);
	//	break;
	/*case WM_ERASEBKGND:
		nResult =  OnEraseBkgnd((HDC)wParam);
		break;
	
	case WM_LBUTTONDOWN:
		point.x = LOWORD(lParam);
		point.y = HIWORD(lParam);
		nResult = OnLButtonDown(wParam, point);
		break;
	case WM_LBUTTONUP:
		point.x = LOWORD(lParam);
		point.y = HIWORD(lParam);
		nResult = OnLButtonUp(wParam, point);
		break;
	case WM_LBUTTONDBLCLK:
		::SendMessageW(m_hWnd, WM_LBUTTONDOWN, wParam, lParam);
		break;
	case WM_MOUSEMOVE:
		point.x = LOWORD(lParam);
		point.y = HIWORD(lParam);
		nResult = OnMouseMove(wParam, point);
		break;*/
	default:
		nResult = CDreamSkinWindow::DefWindowProc(message, wParam, lParam);
	}

	return nResult;
}

/***********************************************************************
 * This function will change the active/inactive state of the dialog 
 *
 * This is a virtual function that can be rewritten in CDreamSkinDialog-
 * derived classes to produce a whole range of dialog not available by 
 * default.
 *
 * Parameters:
 *     unState[IN]
 *         Specify whether the dialog is activated or deactivated.
 *     
 *     hWndOther[IN]
 *         Handle to the window being activated or deactivated, 
 *         depending on the value of the unState parameter.
 *
 * Return value:
 *     None.
 **********************************************************************/
LRESULT CDreamSkinDialog::OnActive(UINT unState, HANDLE hWndOther)
{
	if((unState == WA_ACTIVE) || (unState == WA_CLICKACTIVE))
	{
		m_bActive = TRUE;
	}
	else
	{
		m_bActive = FALSE;
	}

	UpdateWindow(TRUE);
	OnNcActive(m_bActive);
	return 0;
}

/***********************************************************************
 * This function will be called when a new window was created, it will
 * resize the window according the the specifed window border size.
 *
 * This is a virtual function that can be rewritten in CDreamSkinDialog-
 * derived classes to produce a whole range of dialog not available by 
 * default.
 *
 * Parameters:
 *     bActive[IN]
 *         Specify whether the dialog is activated or deactivated.
 *
 * Return value:
 *     None.
 **********************************************************************/
LRESULT CDreamSkinDialog::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	int nWidthGap = 0;
	int nHeightGap = 0;

	DWORD dwStyle = ::GetWindowLong(m_hWnd, GWL_STYLE);
	if (!(dwStyle & WS_CHILD))
	{
		if((dwStyle & WS_BORDER) || (dwStyle & WS_THICKFRAME) || (dwStyle & DS_MODALFRAME))
		{
			nWidthGap = m_pSkinDialog->skinLBorderActive.nWidth + m_pSkinDialog->skinRBorderActive.nWidth - 2 * ::GetSystemMetrics(SM_CXDLGFRAME);
			nHeightGap = m_pSkinDialog->skinTBorderActive.nWidth + m_pSkinDialog->skinBBorderActive.nWidth - 2 * ::GetSystemMetrics(SM_CYDLGFRAME);
		}

		if((dwStyle & WS_BORDER) && (dwStyle & WS_DLGFRAME))  // WS_CAPTION
		{
			if (m_pSkinDialog->skinTitleBar.nIncBorder)
				nHeightGap += m_pSkinDialog->skinTitleBar.nWidth - ::GetSystemMetrics(SM_CYSIZE) - m_pSkinDialog->skinTBorderActive.nWidth;
			else
				nHeightGap += m_pSkinDialog->skinTitleBar.nWidth - ::GetSystemMetrics(SM_CYSIZE);
		}

		if(nWidthGap < 0)
		nWidthGap = 0;

		if(nHeightGap < 0)
			nHeightGap = 0;

		if(nWidthGap > 0 || nHeightGap > 0)
		{
			lpCreateStruct->cx = lpCreateStruct->cx + nWidthGap;
			lpCreateStruct->cy = lpCreateStruct->cy + nHeightGap;
			if(::SetWindowPos(m_hWnd, 0, 0, 0, lpCreateStruct->cx, lpCreateStruct->cy, SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED))
				return 0;
			else
				return 1;
		}
	}

	return 0;
}

LRESULT CDreamSkinDialog::OnDrawItem(UINT nCtrlID, LPDRAWITEMSTRUCT lpDrawItem)
{
	switch (lpDrawItem->CtlType)
	{
	case ODT_LISTBOX:
		return CDreamSkinWindow::DefWindowProc(lpDrawItem->hwndItem, WM_DRAWITEM, (WPARAM)nCtrlID, (LPARAM)lpDrawItem); //::SendMessage(lpDrawItem->hwndItem, WM_DRAWITEM, (WPARAM)m_hWnd, (LPARAM)lpDrawItem);
		break;
	default:
		return CDreamSkinWindow::DefWindowProc(WM_DRAWITEM, (WPARAM)nCtrlID, (LPARAM)lpDrawItem);
		break;
	}
}

LRESULT CDreamSkinDialog::OnMesureItem(int nCtrlID, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	HWND hWndCtrl;
	LRESULT lResult = 0;
	switch (lpMeasureItemStruct->CtlType)
	{
	case ODT_LISTBOX:
		hWndCtrl = ::GetDlgItem(m_hWnd, lpMeasureItemStruct->CtlID);
		if (hWndCtrl)
			lResult = CDreamSkinWindow::DefWindowProc(hWndCtrl, WM_MEASUREITEM, (WPARAM)nCtrlID, (LPARAM)lpMeasureItemStruct);
		else
			lResult = CDreamSkinWindow::DefWindowProc(WM_MEASUREITEM, (WPARAM)nCtrlID, (LPARAM)lpMeasureItemStruct);
		break;
	default:
		lResult = CDreamSkinWindow::DefWindowProc(WM_MEASUREITEM, (WPARAM)nCtrlID, (LPARAM)lpMeasureItemStruct);
		break;
	}

	return lResult;
}

/***********************************************************************
 * This function will change the active/inactive state of the dialog 
 *
 * This is a virtual function that can be rewritten in CDreamSkinDialog-
 * derived classes to produce a whole range of dialog not available by 
 * default.
 *
 * Parameters:
 *     bActive[IN]
 *         Specify whether the dialog is activated or deactivated.
 *
 * Return value:
 *     None.
 **********************************************************************/
LRESULT CDreamSkinDialog::OnNcActive(BOOL bActive)
{
	m_bActive = bActive;
	if (!bActive)
	{
		m_nLastHitTest = HTNOWHERE;
	}
	UpdateWindow(FALSE);

	if(!bActive)
		return (LRESULT)TRUE;
	else
		return (LRESULT)FALSE;
}

LRESULT CDreamSkinDialog::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS *lpncsp)
{
	LRESULT nResult = 0;
	
	if(bCalcValidRects)
		lpncsp->rgrc[0] = GetRectClient(lpncsp->rgrc[0]);

	 return nResult;
}

LRESULT CDreamSkinDialog::OnNcHitTest(POINTS point)
{
	RECT rcWindow;
	::GetWindowRect(m_hWnd, &rcWindow);
	
	RECT rcTemp;
	POINT ptTemp;

	ptTemp.x = point.x;
	ptTemp.y = point.y;

	DWORD dwStyle = ::GetWindowLong(m_hWnd, GWL_STYLE);

	if (!(dwStyle & WS_CHILD))
	{
		//Has title bar
		if ((dwStyle & WS_BORDER) && (dwStyle & WS_DLGFRAME))      // WS_CAPTION
		{
			if (dwStyle & WS_SYSMENU)
			{
				//Check close button
				rcTemp = GetRectClose(rcWindow);
				::OffsetRect(&rcTemp, rcWindow.left, rcWindow.top);
				if(::PtInRect(&rcTemp, ptTemp))
					return HTCLOSE;

				//Check maximize button
				if (dwStyle & WS_MAXIMIZEBOX)
				{
					rcTemp = GetRectMax(rcWindow);
					::OffsetRect(&rcTemp, rcWindow.left, rcWindow.top);
					if(::PtInRect(&rcTemp, ptTemp))
						return HTMAXBUTTON;
				}

				//Check minimize button
				if (dwStyle & WS_MINIMIZEBOX)
				{
					rcTemp = GetRectMin(rcWindow, dwStyle & WS_MAXIMIZEBOX);
					::OffsetRect(&rcTemp, rcWindow.left, rcWindow.top);
					if(::PtInRect(&rcTemp, ptTemp))
						return HTMINBUTTON;
				}

				//Check icon
				if (::SendMessage(m_hWnd, WM_GETICON, ICON_SMALL, 0) || ::SendMessage(m_hWnd, WM_GETICON, ICON_BIG, 0))
				{
					rcTemp = GetRectIcon(rcWindow);
					::OffsetRect(&rcTemp, rcWindow.left, rcWindow.top);
					if(::PtInRect(&rcTemp, ptTemp))
						return HTSYSMENU;
				}
			}

			//Check the title bar
			rcTemp.left = rcWindow.left;
			rcTemp.right = rcWindow.right;
			rcTemp.top = rcWindow.top + m_pSkinDialog->skinTBorderActive.nWidth;
			rcTemp.bottom = rcTemp.top + m_pSkinDialog->skinTitleBar.nWidth;
			if(::PtInRect(&rcTemp, ptTemp))
				return HTCAPTION;
		}

		//Has border
		LRESULT result = PointOnBorder(ptTemp, rcWindow, dwStyle);
		if (result != HTNOWHERE)
		{
			if (dwStyle & WS_THICKFRAME)
				return result;
			else
				return HTBORDER;
		}
	}

	return HTCLIENT;
}

LRESULT CDreamSkinDialog::OnNcLButtonDown(DWORD dwHitTest, POINTS point)
{
	LRESULT nResult = ERROR_SUCCESS;
	switch(dwHitTest)
	{
	case HTCLOSE:
	case HTMAXBUTTON:
	case HTMINBUTTON:
		m_nNcLButtonDown = dwHitTest;
		UpdateWindow(FALSE);
		break;
	default:
		m_nNcLButtonDown = 0;
		nResult = CDreamSkinWindow::DefWindowProc(WM_NCLBUTTONDOWN, dwHitTest, MAKELPARAM(point.x, point.y));
	}

	return nResult;
}

LRESULT CDreamSkinDialog::OnNcLButtonUp(DWORD dwHitTest, POINTS point)
{
	LRESULT nResult = ERROR_SUCCESS;
	switch(dwHitTest)
	{
	case HTCLOSE:
		if (m_nNcLButtonDown == HTCLOSE)
		{
			m_nNcLButtonDown = 0;
			UpdateWindow(FALSE);
			nResult = ::SendMessage(m_hWnd, WM_SYSCOMMAND, SC_CLOSE, MAKELPARAM(point.x, point.y));
		}
		else
		{
			m_nNcLButtonDown = 0;
		}
		break;
	case HTMAXBUTTON:
		if (m_nNcLButtonDown == HTMAXBUTTON)
		{
			m_nNcLButtonDown = 0;
			if (::IsZoomed(m_hWnd))
				nResult = ::SendMessage(m_hWnd, WM_SYSCOMMAND, SC_RESTORE, MAKELPARAM(point.x, point.y));
			else
				nResult = ::SendMessage(m_hWnd, WM_SYSCOMMAND, SC_MAXIMIZE, MAKELPARAM(point.x, point.y));

			m_nLastHitTest = OnNcHitTest(point);
			UpdateWindow(FALSE);
		}
		else
		{
			m_nNcLButtonDown = 0;
		}
		break;
	case HTMINBUTTON:
		if (m_nNcLButtonDown == HTMINBUTTON)
		{
			m_nNcLButtonDown = 0;
			UpdateWindow(FALSE);
			nResult = ::SendMessage(m_hWnd, WM_SYSCOMMAND, SC_MINIMIZE, MAKELPARAM(point.x, point.y));
		}
		else
		{
			m_nNcLButtonDown = 0;
		}
		break;
	default:
		m_nNcLButtonDown = 0;
		nResult = CDreamSkinWindow::DefWindowProc(WM_NCLBUTTONUP, dwHitTest, MAKELPARAM(point.x, point.y));
	}

	return nResult;
}

LRESULT CDreamSkinDialog::OnNcMouseMove(UINT nHitTest, POINTS point)
{
	switch(nHitTest)
	{
	case HTCLOSE:
	case HTMAXBUTTON:
	case HTMINBUTTON:
		if(m_nLastHitTest != nHitTest)
		{
			m_nLastHitTest = nHitTest;
			UpdateWindow(FALSE);
		}
		break;
	case HTCAPTION:
		if(m_nLastHitTest != HTCAPTION)
		{
			m_nLastHitTest = HTCAPTION;
			UpdateWindow(FALSE);
		}
		break;
	default:
		return CDreamSkinWindow::DefWindowProc(WM_NCMOUSEMOVE, nHitTest, MAKELPARAM(point.x, point.y));
	}
	return 0; 
}

LRESULT CDreamSkinDialog::OnNcPaint(HRGN hRGN)
{
	HDC hWindowDC;
	hWindowDC = ::GetWindowDC(m_hWnd);
    // Paint into this DC

	if (m_bCreateRegion)
		CreateWindowRegion(hWindowDC);

	RECT rcWindow;
	::GetWindowRect(m_hWnd, &rcWindow);

	OnDrawBorder(hWindowDC, rcWindow);

	::ReleaseDC(m_hWnd, hWindowDC);

	return 0;
}

/***********************************************************************
 * This function will draw the client area of the dialog. It is called 
 * every time the dialog client area needs to be painted. 
 *
 * This is a virtual function that can be rewritten in CDreamSkinDialog-
 * derived classes to produce a whole range of dialog not available by 
 * default.
 *
 * Parameters:
 *     None.
 *
 * Return value:
 *     None.
 **********************************************************************/
LRESULT CDreamSkinDialog::OnPaint()
{
	PAINTSTRUCT ps;
	HDC hPaintDC = ::BeginPaint(m_hWnd, &ps);

	if (m_bCreateRegion)
		CreateWindowRegion(hPaintDC);

	::SetBkMode(hPaintDC, TRANSPARENT);

	RECT rcClient;
	::GetClientRect(m_hWnd, &rcClient);

	DrawBackground(hPaintDC, rcClient);

	::EndPaint(m_hWnd, &ps);

	return 0;
}

LRESULT CDreamSkinDialog::OnSize(UINT nType, WORD cx, WORD cy)
{
	m_bCreateRegion = TRUE;
	return CDreamSkinWindow::DefWindowProc(WM_SIZE, nType, MAKELPARAM(cx, cy));;
}

/***********************************************************************
 * This function will draw the client area's background of the dialog. 
 * It is called every time the dialog client area's background needs to 
 * be painted. 
 *
 * This is a virtual function that can be rewritten in CDreamSkinDialog-
 * derived classes to produce a whole range of dialog not available by 
 * default.
 *
 * Parameters:
 *     hDC[IN]
 *         Handle to the device context of the dialog.
 *
 *     rcClient[IN]
 *         The RECT struct that indicates the client bounds of the 
 *         dialog.
 *
 * Return value:
 *     None.
 **********************************************************************/
void CDreamSkinDialog::DrawBackground(HDC hDC, RECT rcClient)
{
	COLORREF clrBk;

	if(m_bActive)
	{
		clrBk = m_pSkinDialog->skinBkActive.clrDraw.clrStart;
	}
	else
	{
		clrBk = m_pSkinDialog->skinBkInactive.clrDraw.clrStart;
	}
	HBRUSH hBrush = ::CreateSolidBrush(clrBk);

	::FillRect(hDC, &rcClient, hBrush);

	::DeleteObject(hBrush);
}

/***********************************************************************
 * This function will draw the border the dialog. It is called every 
 * time the dialog border needs to be painted. 
 *
 * This is a virtual function that can be rewritten in CDreamSkinDialog-
 * derived classes to produce a whole range of dialog not available by 
 * default.
 *
 * Parameters:
 *     hDC[IN]
 *         Handle to the device context of the dialog.
 *
 *     rcWindow[IN]
 *         The RECT struct that indicates the the dimensions of the 
 *         bounding rectangle of the specified window. The dimensions 
 *         are given in screen coordinates that are relative to the 
 *         upper-left corner of the screen.
 *
 * Return value:
 *     None.
 **********************************************************************/
void CDreamSkinDialog::OnDrawBorder(HDC hDC, RECT rcWindow)
{
	SKINBORDER *pLBorder, *pRBorder, *pTBorder, *pBBorder;
	BOOL bTitleIncBorder;

	DWORD dwStyle = ::GetWindowLong(m_hWnd, GWL_STYLE);

	if (!(dwStyle & WS_CHILD))
	{
		if(m_bActive)
		{
			pLBorder = &(m_pSkinDialog->skinLBorderActive);
			pRBorder = &(m_pSkinDialog->skinRBorderActive);
			pTBorder = &(m_pSkinDialog->skinTBorderActive);
			pBBorder = &(m_pSkinDialog->skinBBorderActive);
			bTitleIncBorder = m_pSkinDialog->skinTitleBar.nIncBorder;
		}
		else
		{
			pLBorder = &(m_pSkinDialog->skinLBorderInactive);
			pRBorder = &(m_pSkinDialog->skinRBorderInactive);
			pTBorder = &(m_pSkinDialog->skinTBorderInactive);
			pBBorder = &(m_pSkinDialog->skinBBorderInactive);
			bTitleIncBorder = m_pSkinDialog->skinTitleBar.nIncBorder;
		}

		RECT rcDraw;

		//Draw title bar
		if((dwStyle & WS_BORDER) && (dwStyle & WS_DLGFRAME) && (m_pSkinDialog->skinTitleBar.nWidth > 0))
		{
			if (bTitleIncBorder)
			{
				rcDraw.left = 0;
				rcDraw.top = 0;
				rcDraw.bottom = rcDraw.top + m_pSkinDialog->skinTitleBar.nWidth;
				rcDraw.right = rcWindow.right - rcWindow.left;
			}
			else
			{
				rcDraw.left = m_pSkinDialog->skinLBorderActive.nWidth;
				rcDraw.top = m_pSkinDialog->skinTBorderActive.nWidth;
				rcDraw.bottom = rcDraw.top + m_pSkinDialog->skinTitleBar.nWidth;
				rcDraw.right = rcWindow.right - rcWindow.left - m_pSkinDialog->skinRBorderActive.nWidth;
			}
			
			DrawTitleBar(hDC, rcDraw);

			OnDrawSysButton(hDC, rcWindow);
		}
		else
		{
			bTitleIncBorder = FALSE;
		}

		//Draw top border
		if(pTBorder->nWidth > 0 && !bTitleIncBorder)
		{
			rcDraw.left = 0;
			rcDraw.top = 0;
			rcDraw.bottom = pTBorder->nWidth;
			rcDraw.right = rcWindow.right - rcWindow.left;

			if (pTBorder->nDrawType == DRAWTYPE_STRETCHBITMAP && pTBorder->imgDraw.hImage)
				::DrawStretchBitmapEx(hDC, rcDraw.left, rcDraw.top, rcDraw.right - rcDraw.left, rcDraw.bottom - rcDraw.top, pTBorder->imgDraw.hImage, pTBorder->imgDraw.x, pTBorder->imgDraw.y, pTBorder->imgDraw.nWidth, pTBorder->imgDraw.nHeight, pTBorder->nStart, pTBorder->nEnd, 0);
			else
				::FillSolidRect(hDC, &rcDraw, pTBorder->clrDraw.clrStart);
		}
			
		//Draw left border
		if(pLBorder->nWidth > 0)
		{
			rcDraw.left = 0;
			if (bTitleIncBorder)
				rcDraw.top = m_pSkinDialog->skinTitleBar.nWidth;
			else
				rcDraw.top = pTBorder->nWidth;
			rcDraw.bottom = rcWindow.bottom - rcWindow.top - pBBorder->nWidth;
			rcDraw.right = pLBorder->nWidth;

			if (pLBorder->nDrawType == DRAWTYPE_STRETCHBITMAP && pLBorder->imgDraw.hImage)
				::DrawStretchBitmapEx(hDC, rcDraw.left, rcDraw.top, rcDraw.right - rcDraw.left, rcDraw.bottom - rcDraw.top, pLBorder->imgDraw.hImage, pLBorder->imgDraw.x, pLBorder->imgDraw.y, pLBorder->imgDraw.nWidth, pLBorder->imgDraw.nHeight, pLBorder->nStart, pLBorder->nEnd, 1);
			else
				::FillSolidRect(hDC, &rcDraw, pLBorder->clrDraw.clrStart);
		}

		//Draw right border
		if(pRBorder->nWidth > 0)
		{
			rcDraw.left = rcWindow.right - rcWindow.left - pRBorder->nWidth;
			if (bTitleIncBorder)
				rcDraw.top = m_pSkinDialog->skinTitleBar.nWidth;
			else
				rcDraw.top = pTBorder->nWidth;
			rcDraw.bottom = rcWindow.bottom - rcWindow.top - pBBorder->nWidth;
			rcDraw.right = rcWindow.right - rcWindow.left;
			
			if (pRBorder->nDrawType == DRAWTYPE_STRETCHBITMAP && pRBorder->imgDraw.hImage)
				::DrawStretchBitmapEx(hDC, rcDraw.left, rcDraw.top, rcDraw.right - rcDraw.left, rcDraw.bottom - rcDraw.top, pRBorder->imgDraw.hImage, pRBorder->imgDraw.x, pRBorder->imgDraw.y, pRBorder->imgDraw.nWidth, pRBorder->imgDraw.nHeight, pRBorder->nStart, pRBorder->nEnd, 1);
			else
				::FillSolidRect(hDC, &rcDraw, pRBorder->clrDraw.clrStart);
		}

		//Draw bottom border
		if(pBBorder->nWidth > 0)
		{
			rcDraw.left = 0;
			rcDraw.top = rcWindow.bottom - rcWindow.top - pBBorder->nWidth;
			rcDraw.bottom = rcWindow.bottom - rcWindow.top;
			rcDraw.right = rcWindow.right - rcWindow.left;

			if (pBBorder->nDrawType == DRAWTYPE_STRETCHBITMAP && pBBorder->imgDraw.hImage)
				::DrawStretchBitmapEx(hDC, rcDraw.left, rcDraw.top, rcDraw.right - rcDraw.left, rcDraw.bottom - rcDraw.top, pBBorder->imgDraw.hImage, pBBorder->imgDraw.x, pBBorder->imgDraw.y, pBBorder->imgDraw.nWidth, pBBorder->imgDraw.nHeight, pBBorder->nStart, pBBorder->nEnd, 0);
			else
				::FillSolidRect(hDC, &rcDraw, pBBorder->clrDraw.clrStart);
		}
	}
}

void CDreamSkinDialog::OnDrawSysButton(HDC hDC, RECT rcWindow)
{
	RECT rcDraw;
	DWORD dwStyle = ::GetWindowLong(m_hWnd, GWL_STYLE);

	//Draw Sys Buttons
	if (dwStyle & WS_SYSMENU)
	{
		//Close Button
		rcDraw = GetRectClose(rcWindow);
		DrawCloseBtn(hDC, rcDraw);

		//Maximize Button
		if (dwStyle & WS_MAXIMIZEBOX)
		{
			rcDraw = GetRectMax(rcWindow);
			DrawMaxBtn(hDC, rcDraw);
		}

		//Minimize Button
		if (dwStyle & WS_MINIMIZEBOX)
		{
			rcDraw = GetRectMin(rcWindow, dwStyle & WS_MAXIMIZEBOX);
			DrawMinBtn(hDC, rcDraw);
		}
	}
}

void CDreamSkinDialog::CreateWindowRegion(HDC hDC)
{
	m_bCreateRegion = FALSE;

	HRGN hRgn = NULL, hRgnTemp;

	RECT rcWindow;
	::GetWindowRect(m_hWnd, &rcWindow);
	::OffsetRect(&rcWindow, 0 - rcWindow.left, 0 - rcWindow.top);

	long nWndHeight = rcWindow.bottom;
	long nWndWidth = rcWindow.right;

	DWORD dwStyle = ::GetWindowLong(m_hWnd, GWL_STYLE);
	
	if (m_pSkinDialog->nRgnType == RGN_BITMAP && !(dwStyle & WS_CHILD))
	{
		//Create compatible DC for the specified device
		HDC hMemDC = ::CreateCompatibleDC(hDC);
		HBITMAP hTempBitmap = ::CreateCompatibleBitmap(hDC, nWndWidth, nWndHeight);
		::SelectObject(hMemDC, hTempBitmap);

		OnDrawBorder(hMemDC, rcWindow);

		RECT rcClient = GetRectClient(rcWindow);
		
		//Create the initial region, the client region
		hRgn = ::CreateRectRgn(rcClient.left, rcClient.top, rcClient.right, rcClient.bottom);

		RECT rcTemp;

		//Top
		rcTemp.top = 0;
		rcTemp.left = 0;
		rcTemp.right = rcWindow.right;
		rcTemp.bottom = rcClient.top;
		hRgnTemp = GetRegionInRect(hMemDC, rcTemp, m_pSkinDialog->clrTansparent);
		::CombineRgn(hRgn, hRgn, hRgnTemp, RGN_OR);
		::DeleteObject(hRgnTemp);

		//Bottom
		rcTemp.top = rcClient.bottom;
		rcTemp.left = 0;
		rcTemp.right = rcWindow.right;
		rcTemp.bottom = rcWindow.bottom;
		hRgnTemp = GetRegionInRect(hMemDC, rcTemp, m_pSkinDialog->clrTansparent);
		::CombineRgn(hRgn, hRgn, hRgnTemp, RGN_OR);
		::DeleteObject(hRgnTemp);

		//left
		rcTemp.top = rcClient.top;
		rcTemp.left = 0;
		rcTemp.right = rcClient.left;
		rcTemp.bottom = rcClient.bottom;
		hRgnTemp = GetRegionInRect(hMemDC, rcTemp, m_pSkinDialog->clrTansparent);
		::CombineRgn(hRgn, hRgn, hRgnTemp, RGN_OR);
		::DeleteObject(hRgnTemp);

		//right
		rcTemp.top = rcClient.top;
		rcTemp.left = rcClient.right;
		rcTemp.right = rcWindow.right;
		rcTemp.bottom = rcClient.bottom;
		hRgnTemp = GetRegionInRect(hMemDC, rcTemp, m_pSkinDialog->clrTansparent);
		::CombineRgn(hRgn, hRgn, hRgnTemp, RGN_OR);
		::DeleteObject(hRgnTemp);

		::DeleteDC(hMemDC);
	}
	else
	{
		hRgn = ::CreateRectRgn(0, 0, nWndWidth, nWndHeight);
		::SetWindowRgn(m_hWnd, hRgn, TRUE);
		::DeleteObject(hRgn);
	}

	if (hRgn)
	{
		::SetWindowRgn(m_hWnd, hRgn, TRUE);
		::DeleteObject(hRgn);
	}
}

void CDreamSkinDialog::DrawCloseBtn(HDC hDC, RECT rcClose)
{
	SKINSYSBUTTON *pSysButton;
	HPEN hDrawPen, hOldPen;

	if(m_nLastHitTest == HTCLOSE)
	{
		if (m_nNcLButtonDown == HTCLOSE)
			pSysButton = &(m_pSkinDialog->skinTitleBar.skinClosePress);
		else
			pSysButton = &(m_pSkinDialog->skinTitleBar.skinCloseHover);
	}
	else
	{
		if (m_bActive)
			pSysButton = &(m_pSkinDialog->skinTitleBar.skinCloseNormal);
		else
			pSysButton = &(m_pSkinDialog->skinTitleBar.skinCloseDisable);
	}

	if (pSysButton->nDrawType == DRAWTYPE_STRETCHBITMAP && pSysButton->imgDraw.hImage)
	{
		::DrawStretchBitmap(hDC, rcClose.left, rcClose.top, rcClose.right - rcClose.left, rcClose.bottom - rcClose.top, pSysButton->imgDraw.hImage, pSysButton->imgDraw.x, pSysButton->imgDraw.y, pSysButton->imgDraw.nWidth, pSysButton->imgDraw.nHeight);
	}
	else
	{
		::Draw3dRect(hDC, &rcClose, pSysButton->clrTxt, pSysButton->clrTxt);
		hDrawPen = ::CreatePen(PS_SOLID, 2, pSysButton->clrTxt);
		hOldPen = (HPEN)::SelectObject(hDC, hDrawPen);
		::MoveToEx(hDC, rcClose.left + m_pSkinDialog->nBtnPadding, rcClose.top + m_pSkinDialog->nBtnPadding, NULL);
		::LineTo(hDC, rcClose.right - m_pSkinDialog->nBtnPadding, rcClose.bottom - m_pSkinDialog->nBtnPadding);
		::MoveToEx(hDC, rcClose.left + m_pSkinDialog->nBtnPadding, rcClose.bottom - m_pSkinDialog->nBtnPadding, NULL);
		::LineTo(hDC, rcClose.right - m_pSkinDialog->nBtnPadding, rcClose.top + m_pSkinDialog->nBtnPadding);
		::SelectObject(hDC, hOldPen);
		::DeleteObject(hDrawPen);
	}
}

void CDreamSkinDialog::DrawMaxBtn(HDC hDC, RECT rcMax)
{
	SKINSYSBUTTON *pSysButton;
	RECT rcDraw;
	HPEN hDrawPen, hOldPen;

	if (::IsZoomed(m_hWnd))
	{
		if(m_nLastHitTest == HTMAXBUTTON)
		{
			if (m_nNcLButtonDown == HTMAXBUTTON)
				pSysButton = &(m_pSkinDialog->skinTitleBar.skinRestorePress);
			else
				pSysButton = &(m_pSkinDialog->skinTitleBar.skinRestoreHover);
		}
		else
		{
			if (m_bActive)
				pSysButton = &(m_pSkinDialog->skinTitleBar.skinRestoreNormal);
			else
				pSysButton = &(m_pSkinDialog->skinTitleBar.skinRestoreDisable);
		}

		if (pSysButton->nDrawType == DRAWTYPE_STRETCHBITMAP && pSysButton->imgDraw.hImage)
		{
			::DrawStretchBitmap(hDC, rcMax.left, rcMax.top, rcMax.right - rcMax.left, rcMax.bottom - rcMax.top, pSysButton->imgDraw.hImage, pSysButton->imgDraw.x, pSysButton->imgDraw.y, pSysButton->imgDraw.nWidth, pSysButton->imgDraw.nHeight);
		}
		else
		{
			::Draw3dRect(hDC, &rcMax, pSysButton->clrTxt, pSysButton->clrTxt);

			rcDraw.bottom = rcMax.bottom - m_pSkinDialog->nBtnPadding;
			rcDraw.top = rcMax.top + (rcMax.bottom - rcMax.top - 2 * m_pSkinDialog->nBtnPadding) * 3 / 4;
			rcDraw.left = rcMax.left + m_pSkinDialog->nBtnPadding;
			rcDraw.right = rcDraw.left + (rcMax.right - rcMax.left - 2 * m_pSkinDialog->nBtnPadding) * 3 / 4;
			::Draw3dRect(hDC, &rcDraw, pSysButton->clrTxt, pSysButton->clrTxt);
			hDrawPen = ::CreatePen(PS_SOLID, 1, pSysButton->clrTxt);
			hOldPen = (HPEN)::SelectObject(hDC, hDrawPen);
			::MoveToEx(hDC, rcDraw.left, rcDraw.top + 1, NULL);
			::LineTo(hDC, rcDraw.right, rcDraw.top + 1);
			::SelectObject(hDC, hOldPen);
			::DeleteObject(hDrawPen);

			::OffsetRect(&rcDraw, (rcDraw.right - rcDraw.left) / 2, 0 - (rcDraw.bottom - rcDraw.top) / 2);
			FillSolidRect(hDC, rcDraw.left, rcDraw.top, rcDraw.right - rcDraw.left, 2, pSysButton->clrTxt);
			FillSolidRect(hDC, rcDraw.right, rcDraw.top, -1, rcDraw.bottom - rcDraw.top, pSysButton->clrTxt);
			FillSolidRect(hDC, rcDraw.right - (rcDraw.right - rcDraw.left) / 2, rcDraw.bottom, (rcDraw.right - rcDraw.left) / 2, 1, pSysButton->clrTxt);
		}
	}
	else
	{
		if(m_nLastHitTest == HTMAXBUTTON)
		{
			if (m_nNcLButtonDown == HTMAXBUTTON)
				pSysButton = &(m_pSkinDialog->skinTitleBar.skinMaxPress);
			else
				pSysButton = &(m_pSkinDialog->skinTitleBar.skinMaxHover);
		}
		else
		{
			if (m_bActive)
				pSysButton = &(m_pSkinDialog->skinTitleBar.skinMaxNormal);
			else
				pSysButton = &(m_pSkinDialog->skinTitleBar.skinMaxDisable);
		}

		if (pSysButton->nDrawType == DRAWTYPE_STRETCHBITMAP && pSysButton->imgDraw.hImage)
		{
			::DrawStretchBitmap(hDC, rcMax.left, rcMax.top, rcMax.right - rcMax.left, rcMax.bottom - rcMax.top, pSysButton->imgDraw.hImage, pSysButton->imgDraw.x, pSysButton->imgDraw.y, pSysButton->imgDraw.nWidth, pSysButton->imgDraw.nHeight);
		}
		else
		{
			::Draw3dRect(hDC, &rcMax, pSysButton->clrTxt, pSysButton->clrTxt);

			rcDraw.bottom = rcMax.bottom - m_pSkinDialog->nBtnPadding;
			rcDraw.top = rcMax.top + m_pSkinDialog->nBtnPadding;
			rcDraw.left = rcMax.left + m_pSkinDialog->nBtnPadding;
			rcDraw.right = rcMax.right - m_pSkinDialog->nBtnPadding;
			::Draw3dRect(hDC, &rcDraw, pSysButton->clrTxt, pSysButton->clrTxt);
			hDrawPen = ::CreatePen(PS_SOLID, 1, pSysButton->clrTxt);
			hOldPen = (HPEN)::SelectObject(hDC, hDrawPen);
			::MoveToEx(hDC, rcDraw.left, rcDraw.top + 1, NULL);
			::LineTo(hDC, rcDraw.right, rcDraw.top + 1);
			::SelectObject(hDC, hOldPen);
			::DeleteObject(hDrawPen);
		}
	}
}

void CDreamSkinDialog::DrawMinBtn(HDC hDC, RECT rcMin)
{
	SKINSYSBUTTON *pSysButton;
	HPEN hDrawPen, hOldPen;

	if(m_nLastHitTest == HTMINBUTTON)
	{
		if (m_nNcLButtonDown == HTMINBUTTON)
			pSysButton = &(m_pSkinDialog->skinTitleBar.skinMinPress);
		else
			pSysButton = &(m_pSkinDialog->skinTitleBar.skinMinHover);
	}
	else
	{
		if (m_bActive)
			pSysButton = &(m_pSkinDialog->skinTitleBar.skinMinNormal);
		else
			pSysButton = &(m_pSkinDialog->skinTitleBar.skinMinDisable);
	}

	if (pSysButton->nDrawType == DRAWTYPE_STRETCHBITMAP && pSysButton->imgDraw.hImage)
	{
		::DrawStretchBitmap(hDC, rcMin.left, rcMin.top, rcMin.right - rcMin.left, rcMin.bottom - rcMin.top, pSysButton->imgDraw.hImage, pSysButton->imgDraw.x, pSysButton->imgDraw.y, pSysButton->imgDraw.nWidth, pSysButton->imgDraw.nHeight);
	}
	else
	{
		::Draw3dRect(hDC, &rcMin, pSysButton->clrTxt, pSysButton->clrTxt);
		hDrawPen = ::CreatePen(PS_SOLID, 2, pSysButton->clrTxt);
		hOldPen = (HPEN)::SelectObject(hDC, hDrawPen);
		::MoveToEx(hDC, rcMin.left + m_pSkinDialog->nBtnPadding, rcMin.bottom - m_pSkinDialog->nBtnPadding, NULL);
		::LineTo(hDC, rcMin.right - m_pSkinDialog->nBtnPadding - 1, rcMin.bottom - m_pSkinDialog->nBtnPadding);
		::SelectObject(hDC, hOldPen);
		::DeleteObject(hDrawPen);
	}
}

void CDreamSkinDialog::DrawTitleBar(HDC hDC, RECT rcTitle)
{
	COLORREF clrTitleBar, clrText;
	int nDrawType;
	HICON hIcon;
	DRAWIMAGE *pImage;

	if (m_bActive)
	{
		nDrawType = m_pSkinDialog->skinTitleBar.skinBkActive.nDrawType;
		clrTitleBar = m_pSkinDialog->skinTitleBar.skinBkActive.clrDraw.clrStart;
		clrText = m_pSkinDialog->skinTitleBar.clrTxtActive;
		pImage = &(m_pSkinDialog->skinTitleBar.skinBkActive.imgDraw);
	}
	else
	{
		nDrawType = m_pSkinDialog->skinTitleBar.skinBkInactive.nDrawType;
		clrTitleBar = m_pSkinDialog->skinTitleBar.skinBkInactive.clrDraw.clrStart;
		clrText = m_pSkinDialog->skinTitleBar.clrTxtInactive;
		pImage = &(m_pSkinDialog->skinTitleBar.skinBkInactive.imgDraw);
	}

	if (nDrawType == DRAWTYPE_STRETCHBITMAP && pImage->hImage)
		::DrawStretchBitmapEx(hDC, rcTitle.left, rcTitle.top, rcTitle.right - rcTitle.left, rcTitle.bottom - rcTitle.top, pImage->hImage, pImage->x, pImage->y, pImage->nWidth, pImage->nHeight, m_pSkinDialog->skinTitleBar.nStart, m_pSkinDialog->skinTitleBar.nEnd, 0);
	else
		::FillSolidRect(hDC, &rcTitle, clrTitleBar);

	if ((hIcon = (HICON)::SendMessage(m_hWnd, WM_GETICON, ICON_SMALL, 0)) || (hIcon = (HICON)::SendMessage(m_hWnd, WM_GETICON, ICON_BIG, 0)))
	{
		::DrawIconEx(hDC, rcTitle.left + m_pSkinDialog->nIconMargin, rcTitle.top + (rcTitle.bottom - rcTitle.top - ::GetSystemMetrics(SM_CYSMICON)) / 2, hIcon, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0, NULL, DI_NORMAL);
		rcTitle.left += ::GetSystemMetrics(SM_CXSMICON) + m_pSkinDialog->nIconMargin;
	}

	//Get the button title
	int nLen = ::GetWindowTextLengthW(m_hWnd);
	WCHAR *wstrTitle = new WCHAR[nLen + 1];
	::GetWindowTextW(m_hWnd, wstrTitle, nLen + 1);

	//TODO: Use specified font if needed

	//To center text, get the real draw rect
	rcTitle.left += m_pSkinDialog->nTitleMargin;
	RECT rcText = rcTitle;
	::DrawTextW(hDC, wstrTitle, nLen, &rcTitle,  DT_WORDBREAK | DT_LEFT | DT_CALCRECT);
	::OffsetRect(&rcText, 0, ((rcText.bottom - rcText.top) - (rcTitle.bottom - rcTitle.top)) / 2);

	::SetTextColor(hDC, clrText);
	::SetBkMode(hDC, TRANSPARENT);
	::DrawTextW(hDC, wstrTitle, nLen, &rcText,  DT_WORDBREAK | DT_LEFT);

	delete wstrTitle;
}

RECT CDreamSkinDialog::GetRectClient(RECT rcWindow)
{
	RECT rcClient;
	SKINBORDER *pLBorder, *pRBorder, *pTBorder, *pBBorder;

	DWORD dwStyle = ::GetWindowLong(m_hWnd, GWL_STYLE);

	rcClient = rcWindow;

	if (!(dwStyle & WS_CHILD))
	{
		if(m_bActive)
		{
			pLBorder = &(m_pSkinDialog->skinLBorderActive);
			pRBorder = &(m_pSkinDialog->skinRBorderActive);
			pTBorder = &(m_pSkinDialog->skinTBorderActive);
			pBBorder = &(m_pSkinDialog->skinBBorderActive);
		}
		else
		{
			pLBorder = &(m_pSkinDialog->skinLBorderInactive);
			pRBorder = &(m_pSkinDialog->skinRBorderInactive);
			pTBorder = &(m_pSkinDialog->skinTBorderInactive);
			pBBorder = &(m_pSkinDialog->skinBBorderInactive);
		}

		
		if((dwStyle & WS_BORDER) || (dwStyle & WS_THICKFRAME) || (dwStyle & DS_MODALFRAME))
		{
			rcClient.left = rcClient.left + pLBorder->nWidth;
			rcClient.right = rcClient.right - pRBorder->nWidth;
			rcClient.top = rcClient.top + pTBorder->nWidth;
			rcClient.bottom = rcClient.bottom - pBBorder->nWidth;
		}

		if((dwStyle & WS_BORDER) && (dwStyle & WS_DLGFRAME))  // WS_CAPTION
		{
			if (m_pSkinDialog->skinTitleBar.nIncBorder)
				rcClient.top = rcClient.top + m_pSkinDialog->skinTitleBar.nWidth - pTBorder->nWidth;
			else
				rcClient.top = rcClient.top + m_pSkinDialog->skinTitleBar.nWidth;
		}
	}

	return rcClient;
}

RECT CDreamSkinDialog::GetRectClose(RECT rcWindow)
{
	RECT rcClose;
	SKINBORDER *pRBorder, *pTBorder;

	if(m_bActive)
	{
		pRBorder = &(m_pSkinDialog->skinRBorderActive);
		pTBorder = &(m_pSkinDialog->skinTBorderActive);
	}
	else
	{
		pRBorder = &(m_pSkinDialog->skinRBorderInactive);
		pTBorder = &(m_pSkinDialog->skinTBorderInactive);
	}

	
	if (m_pSkinDialog->skinTitleBar.nIncBorder)
	{
		rcClose.top = m_pSkinDialog->skinTitleBar.nYMarginClose;
		rcClose.right = rcWindow.right - rcWindow.left - m_pSkinDialog->skinTitleBar.nXMarginClose;
	}
	else
	{
		rcClose.top = pTBorder->nWidth + m_pSkinDialog->skinTitleBar.nYMarginClose;
		rcClose.right = rcWindow.right - rcWindow.left - m_pSkinDialog->skinTitleBar.nXMarginClose - pRBorder->nWidth;
	}
	rcClose.left = rcClose.right - m_pSkinDialog->skinTitleBar.nWidthClose;
	rcClose.bottom = rcClose.top + m_pSkinDialog->skinTitleBar.nHeightClose;

	return rcClose;
}

RECT CDreamSkinDialog::GetRectIcon(RECT rcWindow)
{
	RECT rcIcon;

	SKINBORDER *pLBorder, *pTBorder;

	if(m_bActive)
	{
		pLBorder = &(m_pSkinDialog->skinLBorderActive);
		pTBorder = &(m_pSkinDialog->skinTBorderActive);
	}
	else
	{
		pLBorder = &(m_pSkinDialog->skinLBorderInactive);
		pTBorder = &(m_pSkinDialog->skinTBorderInactive);
	}

	if (m_pSkinDialog->skinTitleBar.nIncBorder)
	{
		rcIcon.left = 0;
		rcIcon.top = (m_pSkinDialog->skinTitleBar.nWidth - ::GetSystemMetrics(SM_CYSMICON)) / 2;
	}
	else
	{
		rcIcon.left = pLBorder->nWidth;
		rcIcon.top = pTBorder->nWidth + (m_pSkinDialog->skinTitleBar.nWidth - ::GetSystemMetrics(SM_CYSMICON)) / 2;
	}
	rcIcon.right = rcIcon.left + ::GetSystemMetrics(SM_CXSMICON);
	rcIcon.bottom = rcIcon.top + ::GetSystemMetrics(SM_CYSMICON);

	return rcIcon;
}

RECT CDreamSkinDialog::GetRectMax(RECT rcWindow)
{
	RECT rcMax;

	SKINBORDER *pRBorder, *pTBorder;

	if(m_bActive)
	{
		pRBorder = &(m_pSkinDialog->skinRBorderActive);
		pTBorder = &(m_pSkinDialog->skinTBorderActive);
	}
	else
	{
		pRBorder = &(m_pSkinDialog->skinRBorderInactive);
		pTBorder = &(m_pSkinDialog->skinTBorderInactive);
	}

	if (m_pSkinDialog->skinTitleBar.nIncBorder)
	{
		rcMax.right = rcWindow.right - rcWindow.left - m_pSkinDialog->skinTitleBar.nXMarginClose - m_pSkinDialog->skinTitleBar.nWidthClose - m_pSkinDialog->skinTitleBar.nXMarginMax;
		rcMax.top = m_pSkinDialog->skinTitleBar.nYMarginMax;
	}
	else
	{
		rcMax.right = rcWindow.right - rcWindow.left - m_pSkinDialog->skinTitleBar.nXMarginClose - m_pSkinDialog->skinTitleBar.nWidthClose - m_pSkinDialog->skinTitleBar.nXMarginMax - pRBorder->nWidth;
		rcMax.top = pTBorder->nWidth + m_pSkinDialog->skinTitleBar.nYMarginMax;
	}
	

	rcMax.left = rcMax.right - m_pSkinDialog->skinTitleBar.nWidthMax;
	rcMax.bottom = rcMax.top + m_pSkinDialog->skinTitleBar.nHeightMax;

	return rcMax;
}

RECT CDreamSkinDialog::GetRectMin(RECT rcWindow, BOOL bMaxBox)
{
	RECT rcMin;

	SKINBORDER *pRBorder, *pTBorder;

	if(m_bActive)
	{
		pRBorder = &(m_pSkinDialog->skinRBorderActive);
		pTBorder = &(m_pSkinDialog->skinTBorderActive);
	}
	else
	{
		pRBorder = &(m_pSkinDialog->skinRBorderInactive);
		pTBorder = &(m_pSkinDialog->skinTBorderInactive);
	}

	if (m_pSkinDialog->skinTitleBar.nIncBorder)
	{
		rcMin.right = rcWindow.right - rcWindow.left - m_pSkinDialog->skinTitleBar.nXMarginClose - m_pSkinDialog->skinTitleBar.nWidthClose - m_pSkinDialog->skinTitleBar.nXMarginMin;
		rcMin.top = m_pSkinDialog->skinTitleBar.nYMarginMin;
	}
	else
	{
		rcMin.right = rcWindow.right - rcWindow.left - m_pSkinDialog->skinTitleBar.nXMarginClose - m_pSkinDialog->skinTitleBar.nWidthClose - m_pSkinDialog->skinTitleBar.nXMarginMin - pRBorder->nWidth;
		rcMin.top = pTBorder->nWidth + m_pSkinDialog->skinTitleBar.nYMarginMin;
	}

	if (bMaxBox)
		rcMin.right -= m_pSkinDialog->skinTitleBar.nWidthMax + m_pSkinDialog->skinTitleBar.nXMarginMax;

	rcMin.left = rcMin.right - m_pSkinDialog->skinTitleBar.nWidthMin;
	rcMin.bottom = rcMin.top + m_pSkinDialog->skinTitleBar.nHeightMin;

	return rcMin;
}

HRGN CDreamSkinDialog::GetRegionInRect(HDC hDC, RECT rcTarget, COLORREF clrTransparent)
{
	int nWidth = rcTarget.right - rcTarget.left;
	int nHeight = rcTarget.bottom - rcTarget.top;
	int x, y, start;
	HRGN hRgnTemp;

	HRGN hRgnRet = ::CreateRectRgn(rcTarget.left, rcTarget.top, rcTarget.right, rcTarget.bottom);
	if (nWidth >= nHeight)
	{
		for (y = rcTarget.top; y < rcTarget.bottom; y++)
		{
			x = rcTarget.left;
			do
			{
				while(x < rcTarget.right && ::GetPixel(hDC, x, y) != clrTransparent)
					x++;

				if (x >= rcTarget.right)
					break;

				start = x;

				while (x < rcTarget.right && ::GetPixel(hDC, x, y) == clrTransparent)
					x++;

				hRgnTemp = ::CreateRectRgn(start, y, x, y + 1);

				//merge to the result
				::CombineRgn(hRgnRet, hRgnRet, hRgnTemp, RGN_XOR);

				//del the temp region
				::DeleteObject(hRgnTemp);
			} while (x < rcTarget.right);
		}
	}
	else
	{
		for (x = rcTarget.left; x < rcTarget.right; x++)
		{
			y = rcTarget.top;
			do
			{
				while (y < rcTarget.bottom && ::GetPixel(hDC, x, y) != clrTransparent)
					y++;

				if (y >= rcTarget.bottom)
					break;

				start = y;

				while (y < rcTarget.bottom && ::GetPixel(hDC, x, y) == clrTransparent)
					y++;

				hRgnTemp = ::CreateRectRgn(x, start, x + 1, y);

				//merge to the result
				::CombineRgn(hRgnRet, hRgnRet, hRgnTemp, RGN_XOR);

				//del the temp region
				::DeleteObject(hRgnTemp);
			} while (y < rcTarget.bottom);
		}
	}

	return hRgnRet;
}

int CDreamSkinDialog::PointOnBorder(POINT point, RECT rcWindow, DWORD dwStyle)
{
	int nResult = HTNOWHERE;
	RECT rcTemp;
	BOOL bInLeft, bInRight, bInTop, bInBottom;

	SKINBORDER *pLBorder, *pRBorder, *pTBorder, *pBBorder;

	if(m_bActive)
	{
		pLBorder = &(m_pSkinDialog->skinLBorderActive);
		pRBorder = &(m_pSkinDialog->skinRBorderActive);
		pTBorder = &(m_pSkinDialog->skinTBorderActive);
		pBBorder = &(m_pSkinDialog->skinBBorderActive);
	}
	else
	{
		pLBorder = &(m_pSkinDialog->skinLBorderInactive);
		pRBorder = &(m_pSkinDialog->skinRBorderInactive);
		pTBorder = &(m_pSkinDialog->skinTBorderInactive);
		pBBorder = &(m_pSkinDialog->skinBBorderInactive);
	}
	
	if (dwStyle & (WS_BORDER | WS_THICKFRAME | DS_MODALFRAME))
	{
		bInLeft = bInRight = bInTop = bInBottom = FALSE;

		//Top border
		rcTemp.top = rcWindow.top;
		rcTemp.bottom = rcWindow.top + pTBorder->nWidth;
		rcTemp.left = rcWindow.left;
		rcTemp.right = rcWindow.right;
		if (::PtInRect(&rcTemp, point))
			bInTop = TRUE;

		//Left border
		rcTemp.top = rcWindow.top;
		rcTemp.bottom = rcWindow.bottom;
		rcTemp.left = rcWindow.left;
		rcTemp.right = rcWindow.left + pLBorder->nWidth;
		if (::PtInRect(&rcTemp, point))
			bInLeft = TRUE;

		//Rigth border
		rcTemp.top = rcWindow.top;
		rcTemp.bottom = rcWindow.bottom;
		rcTemp.right = rcWindow.right;
		rcTemp.left = rcWindow.right - pRBorder->nWidth;
		if (::PtInRect(&rcTemp, point))
			bInRight = TRUE;

		//Bottom border
		rcTemp.bottom = rcWindow.bottom;
		rcTemp.top = rcWindow.bottom - pBBorder->nWidth;
		rcTemp.left = rcWindow.left;
		rcTemp.right = rcWindow.right;
		if (::PtInRect(&rcTemp, point))
			bInBottom = TRUE;

		if (bInTop)
		{
			if (bInLeft)
				nResult = HTTOPLEFT;
			else if (bInRight)
				nResult = HTTOPRIGHT;
			else
				nResult = HTTOP;
		}
		else if (bInLeft)
		{
			if (bInBottom)
				nResult = HTBOTTOMLEFT;
			else
				nResult = HTLEFT;
		}
		else if (bInBottom)
		{
			if (bInRight)
				nResult = HTBOTTOMRIGHT;
			else
				nResult = HTBOTTOM;
		}
		else if (bInRight)
		{
			nResult = HTRIGHT;
		}
	}

	return nResult;
}
// DreamSkinComboBox.cpp
#include <windows.h>

#include "WinGdiEx.h"
#include "DreamSkinMain.h"
#include "DreamSkinLoader.h"
#include "DreamSkinWindow.h"
#include "DreamSkinScrollBar.h"
#include "DreamSkinComboBox.h"

//static member declaration
WNDPROC         CDreamSkinComboBox::s_DefaultWindowProc = NULL;
SKINCOMBOBOX    CDreamSkinComboBox::s_SkinComboBox;

CDreamSkinComboBox::CDreamSkinComboBox(HWND hWnd, WNDPROC OrgWndProc)
	: CDreamSkinWindow(hWnd, OrgWndProc)
{
	m_pSkinComboBox = &s_SkinComboBox;
	m_nOwnerDraw = -1;
	m_nCurHover = -1;
	m_bBtnHover = FALSE;
	m_bBtnPress = FALSE;
	m_bMouseIn = FALSE;

	m_hBkNormal = CreateBackgroundBrush(&m_pSkinComboBox->skinBkNormal);
	m_hBkDisable = CreateBackgroundBrush(&m_pSkinComboBox->skinBkDisable);
}

CDreamSkinComboBox::~CDreamSkinComboBox()
{
	if (m_pSkinComboBox != &s_SkinComboBox)
		delete m_pSkinComboBox;

	::DeleteObject(m_hBkNormal);
	::DeleteObject(m_hBkDisable);
}

void CDreamSkinComboBox::Reload()
{
	if (m_pSkinComboBox == &s_SkinComboBox)
	{
		HBRUSH hTemp = m_hBkNormal;
		m_hBkNormal = CreateBackgroundBrush(&m_pSkinComboBox->skinBkNormal);
		::DeleteObject(hTemp);

		hTemp = m_hBkDisable;
		m_hBkDisable = CreateBackgroundBrush(&m_pSkinComboBox->skinBkDisable);
		::DeleteObject(hTemp);
	}

	if (m_hWnd && ::IsWindow(m_hWnd))
	{
		RECT rcClient;
		::GetClientRect(m_hWnd, &rcClient);

		m_bCreateRegion = TRUE;
		::InvalidateRect(m_hWnd, &rcClient, TRUE);
	}
}

BOOL CDreamSkinComboBox::ApplySkin(CDreamSkinLoader *pLoader)
{
	BOOL bResult = TRUE;

	InitialClass();

	SKINCOMBOBOX SkinComboBox;
	pLoader->GetSkinComboBox(&SkinComboBox);
	s_SkinComboBox = SkinComboBox;

	return bResult;
}

CDreamSkinComboBox* CDreamSkinComboBox::CreateInstance(HWND hWnd, WNDPROC OrgWndProc)
{
	CDreamSkinComboBox *pSkinComboBox = new CDreamSkinComboBox(hWnd, OrgWndProc);
	return pSkinComboBox;
}

LRESULT WINAPI CDreamSkinComboBox::DefWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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

BOOL CDreamSkinComboBox::InitialClass()
{
	//Get the default window proc for this class
	WNDCLASSW clsComboBoxDefault;
	if(!GetClassInfoW(GetModuleHandle(NULL), DEFAULT_COMBOBOX_CLASSNAME_W, &clsComboBoxDefault))
	{
		//Get class info failed
		//TODO: add error handle code
		return FALSE;
	}
	s_DefaultWindowProc = clsComboBoxDefault.lpfnWndProc;

	GetDefaultSkin(&s_SkinComboBox);

	return TRUE;
}

void CDreamSkinComboBox::DestroyClass()
{

}

BOOL CDreamSkinComboBox::GetDefaultSkin(SKINCOMBOBOX *pSkinComboBox)
{
	if (pSkinComboBox)
	{
		GetDefaultBackground(&pSkinComboBox->skinBkNormal, RGB(255, 255, 255));
		GetDefaultBackground(&pSkinComboBox->skinBkDisable, ::GetSysColor(COLOR_BTNFACE));

		GetDefaultText(&pSkinComboBox->skinTxtNormal, ::GetSysColor(COLOR_BTNTEXT));
		GetDefaultText(&pSkinComboBox->skinTxtDisable, ::GetSysColor(COLOR_GRAYTEXT));

		GetDefaultBorder(&pSkinComboBox->skinLBorderNormal, RGB(113, 111, 100), 1);        //COLOR_3DDKSHADOW
		GetDefaultBorder(&pSkinComboBox->skinRBorderNormal, RGB(113, 111, 100), 1);        
		GetDefaultBorder(&pSkinComboBox->skinTBorderNormal, RGB(113, 111, 100), 1);
		GetDefaultBorder(&pSkinComboBox->skinBBorderNormal, RGB(113, 111, 100), 1);

		GetDefaultBorder(&pSkinComboBox->skinLBorderDisable, RGB(172, 168, 153), 1);       //COLOR_GRAYTEXT
		GetDefaultBorder(&pSkinComboBox->skinRBorderDisable, RGB(172, 168, 153), 1);
		GetDefaultBorder(&pSkinComboBox->skinTBorderDisable, RGB(172, 168, 153), 1);
		GetDefaultBorder(&pSkinComboBox->skinBBorderDisable, RGB(172, 168, 153), 1);

		GetDefaultBorder(&pSkinComboBox->skinLBorderHover, RGB(113, 111, 100), 1);
		GetDefaultBorder(&pSkinComboBox->skinRBorderHover, RGB(113, 111, 100), 1);
		GetDefaultBorder(&pSkinComboBox->skinTBorderHover, RGB(113, 111, 100), 1);
		GetDefaultBorder(&pSkinComboBox->skinBBorderHover, RGB(113, 111, 100), 1);

		GetDefaultBackground(&pSkinComboBox->skinListBox.skinBkNormal, RGB(255, 255, 255));
		GetDefaultBackground(&pSkinComboBox->skinListBox.skinBkDisable, RGB(236, 233, 216));           //COLOR_BTNFACE

		GetDefaultBorder(&pSkinComboBox->skinListBox.skinLBorderNormal, RGB(113, 111, 100), 1);        //COLOR_3DDKSHADOW
		GetDefaultBorder(&pSkinComboBox->skinListBox.skinRBorderNormal, RGB(113, 111, 100), 1);
		GetDefaultBorder(&pSkinComboBox->skinListBox.skinTBorderNormal, RGB(113, 111, 100), 1);
		GetDefaultBorder(&pSkinComboBox->skinListBox.skinBBorderNormal, RGB(113, 111, 100), 1);

		GetDefaultBorder(&pSkinComboBox->skinListBox.skinLBorderDisable, RGB(172, 168, 153), 1);       //COLOR_GRAYTEXT
		GetDefaultBorder(&pSkinComboBox->skinListBox.skinRBorderDisable, RGB(172, 168, 153), 1);
		GetDefaultBorder(&pSkinComboBox->skinListBox.skinTBorderDisable, RGB(172, 168, 153), 1);
		GetDefaultBorder(&pSkinComboBox->skinListBox.skinBBorderDisable, RGB(172, 168, 153), 1);

		GetDefaultSysButton(&pSkinComboBox->skinBtnNormal, RGB(236, 233, 216), RGB(0, 0, 0) , RGB(113, 111, 100));            //COLOR_BTNFACE
		GetDefaultSysButton(&pSkinComboBox->skinBtnHover, RGB(241, 239, 226), RGB(255, 255, 255), RGB(113, 111, 100));             //COLOR_3DLIGHT
		GetDefaultSysButton(&pSkinComboBox->skinBtnPress, RGB(241, 239, 226), RGB(128, 128, 128), RGB(113, 111, 100));
		GetDefaultSysButton(&pSkinComboBox->skinBtnDisable, RGB(236, 233, 216), RGB(172, 168, 153), RGB(172, 168, 153));

		pSkinComboBox->nBtnIncludeBorder = 1;

		GetDefaultItem(&pSkinComboBox->skinListBox.skinItemNormalSelected, RGB(0, 0, 128), RGB(255, 255, 255), RGB(0, 0, 0), 0);
		GetDefaultItem(&pSkinComboBox->skinListBox.skinItemDisableSelected, RGB(128, 128, 128), RGB(255, 255, 255), RGB(0, 0, 0), 0);
		GetDefaultItem(&pSkinComboBox->skinListBox.skinItemHoverSelected, RGB(0, 0, 128), RGB(255, 255, 255), RGB(0, 0, 0), 0);

		GetDefaultItem(&pSkinComboBox->skinListBox.skinItemNormalUnselected, RGB(255, 255, 255), RGB(0, 0, 0), RGB(0, 0, 0), 0);
		GetDefaultItem(&pSkinComboBox->skinListBox.skinItemDisableUnselected, RGB(236, 233, 216), RGB(0, 0, 0), RGB(0, 0, 0), 0);
		GetDefaultItem(&pSkinComboBox->skinListBox.skinItemHoverUnselected, RGB(0, 0, 255), RGB(255, 255, 255), RGB(0, 0, 0), 0);
	}

	return TRUE;
}

LRESULT CDreamSkinComboBox::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT nResult = ERROR_SUCCESS;

	switch(message)
	{
	case WM_CTLCOLOREDIT:
	case WM_CTLCOLORSTATIC:
		nResult = OnCtrlColor((HDC)wParam, (HWND)lParam);
		break;
	case WM_CREATE:
		nResult = OnCreate((LPCREATESTRUCT)lParam);
		break;
	case WM_DRAWITEM:
		nResult = OnDrawItem((UINT)wParam, (LPDRAWITEMSTRUCT)lParam);
		break;
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

LRESULT CDreamSkinComboBox::OnCtrlColor(HDC hDC, HWND hWndEdit)
{
	HBRUSH hBrushBk;
	COLORREF clrText, clrBk;

	DWORD dwStyle = ::GetWindowLong(m_hWnd, GWL_STYLE);

	int nStatus = GetCurrentStatus(dwStyle);
	switch(nStatus)
	{
	case DRAWSTATUS_DISABLE:
		hBrushBk = m_hBkDisable;
		clrBk = m_pSkinComboBox->skinBkDisable.clrDraw.clrStart;
		clrText = m_pSkinComboBox->skinTxtDisable.clrDraw;
		break;
	default:
		hBrushBk = m_hBkNormal;
		clrBk = m_pSkinComboBox->skinBkNormal.clrDraw.clrStart;
		clrText = m_pSkinComboBox->skinTxtNormal.clrDraw;
		break;

	}

	::SetBkMode(hDC, OPAQUE);
	::SetBkColor(hDC, clrBk);
	::SetTextColor(hDC, clrText);

	return (LRESULT)hBrushBk;
}

LRESULT CDreamSkinComboBox::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (lpCreateStruct->style & CBS_OWNERDRAWFIXED)
	{
		m_nOwnerDraw = 1;
	}
	else if (lpCreateStruct->style & CBS_OWNERDRAWVARIABLE)
	{
		m_nOwnerDraw = 2;
	}
	else
	{
		m_nOwnerDraw = 0;
	}

	return CDreamSkinWindow::DefWindowProc(WM_CREATE, 0, (LPARAM)lpCreateStruct);
}

LRESULT CDreamSkinComboBox::OnDrawItem(UINT nCtrlID, LPDRAWITEMSTRUCT lpDrawItem)
{
	if (m_nOwnerDraw)
	{
		return CDreamSkinWindow::DefWindowProc(WM_DRAWITEM, (WPARAM)nCtrlID, (LPARAM)lpDrawItem);
	}
	else
	{
		SKINITEM *pItem;
		if (lpDrawItem->itemState & ODS_SELECTED)
		{
			if (lpDrawItem->itemState & ODS_DISABLED)
				pItem = &m_pSkinComboBox->skinListBox.skinItemDisableSelected;
			else if (lpDrawItem->itemID == m_nCurHover)
				pItem = &m_pSkinComboBox->skinListBox.skinItemHoverSelected;
			else
				pItem = &m_pSkinComboBox->skinListBox.skinItemNormalSelected;
		}
		else
		{
			if (lpDrawItem->itemState & ODS_DISABLED)
				pItem = &m_pSkinComboBox->skinListBox.skinItemDisableUnselected;
			else if (lpDrawItem->itemID == m_nCurHover)
				pItem = &m_pSkinComboBox->skinListBox.skinItemHoverUnselected;
			else
				pItem = &m_pSkinComboBox->skinListBox.skinItemNormalUnselected;
		}

		//Get the button title
		int nLen = (int)::SendMessage(m_hWnd, CB_GETLBTEXTLEN, lpDrawItem->itemID, 0);
		WCHAR *wstrTitle = NULL;
		if (nLen > 0)
		{
			wstrTitle = new WCHAR[nLen + 1];
			memset(wstrTitle, 0, sizeof(WCHAR) * (nLen + 1));
			::SendMessageW(m_hWnd, CB_GETLBTEXT, lpDrawItem->itemID, (LPARAM)wstrTitle);
		}

		DrawItem(lpDrawItem->hDC, pItem, lpDrawItem->rcItem, wstrTitle);

		if (nLen > 0)
			delete wstrTitle;

		return 1;
	}
}

LRESULT CDreamSkinComboBox::OnLButtonDown(UINT nFlags, POINTS point)
{
	DWORD dwStyle = ::GetWindowLong(m_hWnd, GWL_STYLE);
	DWORD dwType = dwStyle & 0x03;

	POINT pt;
	pt.x = point.x;
	pt.y = point.y;

	RECT rcClient;
	::GetClientRect(m_hWnd, &rcClient);

	COMBOBOXINFO cmbInfo;
	cmbInfo.cbSize = sizeof(COMBOBOXINFO);
	BOOL bBtnDown = FALSE;
	if(::SendMessage(m_hWnd, CB_GETCOMBOBOXINFO, 0, (LPARAM)&cmbInfo))
	{
		if (dwType == CBS_DROPDOWN && ::PtInRect(&cmbInfo.rcButton, pt))
			bBtnDown = TRUE; 
		else if (dwType == CBS_DROPDOWNLIST && ::PtInRect(&rcClient, pt))
			bBtnDown = TRUE;

		if (bBtnDown != m_bBtnPress)
		{
			m_bBtnPress = bBtnDown;
			::InvalidateRect(m_hWnd, &rcClient, FALSE);
		}
	}

	return CDreamSkinWindow::DefWindowProc(WM_LBUTTONDOWN, nFlags, MAKELPARAM(point.x, point.y));
}

LRESULT CDreamSkinComboBox::OnLButtonUp(UINT nFlags, POINTS point)
{
	if (m_bBtnPress)
	{
		m_bBtnPress = FALSE;
		RECT rcClient;
		::GetClientRect(m_hWnd, &rcClient);
		::InvalidateRect(m_hWnd, &rcClient, FALSE);
	}

	return CDreamSkinWindow::DefWindowProc(WM_LBUTTONUP, nFlags, MAKELPARAM(point.x, point.y));
}

LRESULT CDreamSkinComboBox::OnMouseLeave()
{
	BOOL bRedraw = FALSE;
	if (m_bBtnHover)
	{
		m_bBtnHover = FALSE;
		bRedraw = TRUE;
	}

	if (m_bMouseIn)
	{
		COMBOBOXINFO cmbInfo;
		cmbInfo.cbSize = sizeof(COMBOBOXINFO);
		if(::SendMessage(m_hWnd, CB_GETCOMBOBOXINFO, 0, (LPARAM)&cmbInfo))
		{
			POINT pt;
			::GetCursorPos(&pt);

			RECT rcCombo;
			::GetWindowRect(m_hWnd, &rcCombo);

			DWORD dwStyle = ::GetWindowLong(m_hWnd, GWL_STYLE);
			DWORD dwType = dwStyle & 0x03;
			
			if (dwType == CBS_SIMPLE && cmbInfo.hwndList)
			{
				RECT rcList;
				::GetWindowRect(cmbInfo.hwndList, &rcList);
				rcCombo.bottom = rcList.top;
			}

			if (!::PtInRect(&rcCombo, pt))
			{
				m_bMouseIn = FALSE;
				bRedraw = TRUE;
			}
		}
		else
		{
			m_bMouseIn = FALSE;
			bRedraw = TRUE;
		}
	}

	if (bRedraw)
	{
		RECT rcClient;
		::GetClientRect(m_hWnd, &rcClient);
		::InvalidateRect(m_hWnd, &rcClient, FALSE);
	}

	return CDreamSkinWindow::DefWindowProc(WM_MOUSELEAVE, 0, 0);
}

LRESULT CDreamSkinComboBox::OnMouseMove(UINT nFlags, POINTS point)
{
	TRACKMOUSEEVENT EventTrack;
	EventTrack.cbSize = sizeof(TRACKMOUSEEVENT);
	EventTrack.dwFlags = TME_LEAVE;
	EventTrack.hwndTrack = m_hWnd;
	EventTrack.dwHoverTime = 0;

	TrackMouseEvent(&EventTrack);

	BOOL bRedraw = FALSE;

	DWORD dwStyle = ::GetWindowLong(m_hWnd, GWL_STYLE);
	DWORD dwType = dwStyle & 0x03;

	POINT pt;
	pt.x = point.x;
	pt.y = point.y;

	RECT rcClient;

	COMBOBOXINFO cmbInfo;
	cmbInfo.cbSize = sizeof(COMBOBOXINFO);
	
	if(::SendMessage(m_hWnd, CB_GETCOMBOBOXINFO, 0, (LPARAM)&cmbInfo))
	{
		if (dwType == CBS_SIMPLE && cmbInfo.hwndList)
		{
			::GetWindowRect(m_hWnd, &rcClient);
				
			if (dwType == CBS_SIMPLE && cmbInfo.hwndList)
			{
				RECT rcList;
				::GetWindowRect(cmbInfo.hwndList, &rcList);
				rcClient.bottom = rcList.top;
			}

			::OffsetRect(&rcClient, 0 - rcClient.left, 0 - rcClient.top);
		}
		else
		{
			::GetClientRect(m_hWnd, &rcClient);
		}

		BOOL bBtnHover = FALSE;
		if (::PtInRect(&rcClient, pt))
			bBtnHover = TRUE;

		if (bBtnHover != m_bMouseIn)
		{
			m_bMouseIn = bBtnHover;
			bRedraw = TRUE;
		}

		bBtnHover = FALSE;
		if (dwType == CBS_DROPDOWN && ::PtInRect(&cmbInfo.rcButton, pt))
			bBtnHover = TRUE; 
		else if (dwType == CBS_DROPDOWNLIST && ::PtInRect(&rcClient, pt))
			bBtnHover = TRUE;

		if (bBtnHover != m_bBtnHover)
		{
			m_bBtnHover = bBtnHover;
			bRedraw = TRUE;
		}

		if (bRedraw)
			::InvalidateRect(m_hWnd, &rcClient, FALSE);
	}

	return CDreamSkinWindow::DefWindowProc(WM_MOUSEMOVE, nFlags, MAKELPARAM(point.x, point.y));
}

LRESULT CDreamSkinComboBox::OnPaint()
{
	COMBOBOXINFO cmbInfo;
	cmbInfo.cbSize = sizeof(COMBOBOXINFO);
	if(::SendMessage(m_hWnd, CB_GETCOMBOBOXINFO, 0, (LPARAM)&cmbInfo))
	{
		RECT rcUpdate;
		if (::GetUpdateRect(m_hWnd, &rcUpdate, FALSE))
		{
			PAINTSTRUCT ps;
			HDC hPaintDC = ::BeginPaint(m_hWnd, &ps);

			::SetBkMode(hPaintDC, TRANSPARENT);

			RECT rcCombo;
			::GetWindowRect(m_hWnd, &rcCombo);

			DWORD dwStyle = ::GetWindowLong(m_hWnd, GWL_STYLE);
			DWORD dwType = dwStyle & 0x03;

			
			if (dwType == CBS_SIMPLE && cmbInfo.hwndList)
			{
				RECT rcList;
				::GetWindowRect(cmbInfo.hwndList, &rcList);
				rcCombo.bottom = rcList.top;
			}

			::OffsetRect(&rcCombo, 0 - rcCombo.left, 0 - rcCombo.top);

			DrawBackground(hPaintDC, rcCombo, dwStyle);

			if (dwType != CBS_SIMPLE)
			{
				RECT rcButton = cmbInfo.rcButton;
				int nMagicNum = 1;
				if (m_pSkinComboBox->nBtnIncludeBorder)
				{
					if (rcCombo.right - rcButton.right <= nMagicNum)
						rcButton.right = rcCombo.right;

					if (rcButton.top - rcCombo.top <= nMagicNum)
						rcButton.top = rcCombo.top;

					if (rcCombo.bottom - rcButton.bottom <= nMagicNum)
						rcButton.bottom = rcCombo.bottom;

					if (rcButton.left - rcCombo.left <= nMagicNum)
						rcButton.left = rcCombo.left;
				}

				if (m_bMouseIn && ! (m_bBtnHover || m_bBtnPress))
				{
					DrawButton(hPaintDC, rcButton, dwStyle);
					DrawBorder(hPaintDC, rcCombo);
				}
				else
				{
					DrawBorder(hPaintDC, rcCombo);
					DrawButton(hPaintDC, rcButton, dwStyle);
				}
			}
			else
			{
				DrawBorder(hPaintDC, rcCombo);
			}

			if (dwType == CBS_DROPDOWNLIST && m_hWnd == cmbInfo.hwndItem)
			{
				int nIndex = ::SendMessage(m_hWnd, CB_GETCURSEL, 0, 0);
				if (nIndex >= 0)
				{
					//Get the button title
					int nLen = (int)::SendMessage(m_hWnd, CB_GETLBTEXTLEN, nIndex, 0);
					WCHAR *wstrTitle = NULL;
					if (nLen > 0)
					{
						wstrTitle = new WCHAR[nLen + 1];
						memset(wstrTitle, 0, sizeof(WCHAR) * (nLen + 1));
						::SendMessageW(m_hWnd, CB_GETLBTEXT, nIndex, (LPARAM)wstrTitle);
					}

					SKINTEXT *pText;
					if (GetCurrentStatus(dwStyle) == DRAWSTATUS_DISABLE)
						pText = &m_pSkinComboBox->skinTxtDisable;
					else
						pText = &m_pSkinComboBox->skinTxtNormal;

					CDreamSkinWindow::DrawTitle(hPaintDC, pText, cmbInfo.rcItem, wstrTitle);
				}
			}

			::EndPaint(m_hWnd, &ps);
		}
	}

	return 0;
}

void CDreamSkinComboBox::DrawLBBorder(HDC hDC, RECT rcWindow)
{
	SKINBORDER *pLBorder, *pRBorder, *pTBorder, *pBBorder;
	DWORD dwStyle = ::GetWindowLong(m_hWnd, GWL_STYLE);

	int nStatus = GetCurrentStatus(dwStyle);
	switch(nStatus)
	{
	case DRAWSTATUS_DISABLE:
		pLBorder = &m_pSkinComboBox->skinListBox.skinLBorderDisable;
		pRBorder = &m_pSkinComboBox->skinListBox.skinRBorderDisable;
		pTBorder = &m_pSkinComboBox->skinListBox.skinTBorderDisable;
		pBBorder = &m_pSkinComboBox->skinListBox.skinBBorderDisable;
		break;
	default:
		pLBorder = &m_pSkinComboBox->skinListBox.skinLBorderNormal;
		pRBorder = &m_pSkinComboBox->skinListBox.skinRBorderNormal;
		pTBorder = &m_pSkinComboBox->skinListBox.skinTBorderNormal;
		pBBorder = &m_pSkinComboBox->skinListBox.skinBBorderNormal;
		break;

	}

	CDreamSkinWindow::DrawBorder(hDC, pLBorder, pRBorder, pTBorder, pBBorder, rcWindow);
}

HBRUSH  CDreamSkinComboBox::CreateBackgroundBrush(SKINBACKGROUND *pSkinBackground)
{
	HBRUSH hBrush = ::CreateSolidBrush(pSkinBackground->clrDraw.clrStart);

	return hBrush;
}

void CDreamSkinComboBox::DrawButton(HDC hDC, RECT rcDraw, SKINSYSBUTTON *pSysButton)
{
	HBRUSH hDrawBrush;
	HRGN hRgn;
	HPEN hDrawPen, hOldPen;

	if (pSysButton->nDrawType == DRAWTYPE_STRETCHBITMAP && pSysButton->imgDraw.hImage)
	{
		if (rcDraw.right - rcDraw.left != pSysButton->imgDraw.nWidth)
			rcDraw.left = rcDraw.right - pSysButton->imgDraw.nWidth;
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

		pt[0].x = rcDraw.left + nWidth / 2;
		pt[0].y = rcDraw.bottom - nHeight * 3 / 8;
		pt[1].x = pt[0].x - 1;
		pt[1].y = pt[0].y - 1;
		pt[2].x = pt[0].x + 1;
		pt[2].y = pt[0].y - 1;

		while ((pt[1].x > rcDraw.left + nWidth / 4) && (pt[2].x < rcDraw.right - nWidth / 4))
		{
			pt[1].x = pt[1].x - 1;
			pt[1].y = pt[1].y - 1;
			pt[2].x = pt[2].x + 1;
			pt[2].y = pt[2].y - 1;
		}

		hRgn = ::CreatePolygonRgn(pt, 3, ALTERNATE);
		::FillRgn(hDC, hRgn, hDrawBrush);

		::MoveToEx(hDC, pt[0].x, pt[0].y, NULL);
		::LineTo(hDC, pt[1].x, pt[1].y);
		::LineTo(hDC, pt[2].x, pt[2].y);
		::LineTo(hDC, pt[0].x, pt[0].y);

		if (pSysButton->bDrawBd)
			::Draw3dRect(hDC, &rcDraw, pSysButton->clrBd, pSysButton->clrBd);

		::SelectObject(hDC, hOldPen);
		::DeleteObject(hDrawPen);

		::DeleteObject(hRgn);
		::DeleteObject(hDrawBrush);
	}
}

void CDreamSkinComboBox::DrawBackground(HDC hDC, RECT rcClient, DWORD dwStyle)
{
	SKINBACKGROUND *pSkinBackground;

	int nStatus = GetCurrentStatus(dwStyle);
	switch(nStatus)
	{
	case DRAWSTATUS_DISABLE:
		pSkinBackground = &m_pSkinComboBox->skinBkDisable;
		break;
	default:
		pSkinBackground = &m_pSkinComboBox->skinBkNormal;
		break;

	}

	CDreamSkinWindow::DrawBackground(hDC, rcClient, pSkinBackground);
}

void CDreamSkinComboBox::DrawBorder(HDC hDC, RECT rcWindow)
{
	SKINBORDER *pLBorder, *pRBorder, *pTBorder, *pBBorder;
	DWORD dwStyle = ::GetWindowLong(m_hWnd, GWL_STYLE);

	int nStatus = GetCurrentStatus(dwStyle);
	switch(nStatus)
	{
	case DRAWSTATUS_DISABLE:
		pLBorder = &m_pSkinComboBox->skinLBorderDisable;
		pRBorder = &m_pSkinComboBox->skinRBorderDisable;
		pTBorder = &m_pSkinComboBox->skinTBorderDisable;
		pBBorder = &m_pSkinComboBox->skinBBorderDisable;
		break;
	default:
		if (m_bMouseIn)
		{
			pLBorder = &m_pSkinComboBox->skinLBorderHover;
			pRBorder = &m_pSkinComboBox->skinRBorderHover;
			pTBorder = &m_pSkinComboBox->skinTBorderHover;
			pBBorder = &m_pSkinComboBox->skinBBorderHover;
		}
		else
		{
			pLBorder = &m_pSkinComboBox->skinLBorderNormal;
			pRBorder = &m_pSkinComboBox->skinRBorderNormal;
			pTBorder = &m_pSkinComboBox->skinTBorderNormal;
			pBBorder = &m_pSkinComboBox->skinBBorderNormal;
		}
		break;

	}

	CDreamSkinWindow::DrawBorder(hDC, pLBorder, pRBorder, pTBorder, pBBorder, rcWindow);
}

void CDreamSkinComboBox::DrawButton(HDC hDC, RECT rcDraw, DWORD dwStyle)
{
	SKINSYSBUTTON *pSysButton;
	int nStatus = GetCurrentStatus(dwStyle);
	if (nStatus == DRAWSTATUS_DISABLE)
		pSysButton = &m_pSkinComboBox->skinBtnDisable;
	else
		if (m_bBtnHover)
			if (m_bBtnPress)
				pSysButton = &m_pSkinComboBox->skinBtnPress;
			else
				pSysButton = &m_pSkinComboBox->skinBtnHover;
		else
			pSysButton = &m_pSkinComboBox->skinBtnNormal;

	CDreamSkinComboBox::DrawButton(hDC, rcDraw, pSysButton);
}

int CDreamSkinComboBox::GetCurrentStatus(DWORD dwStyle) const
{
	int nStatus;

	if(dwStyle & WS_DISABLED)
		nStatus = DRAWSTATUS_DISABLE;       //Disabled
	else
		nStatus = DRAWSTATUS_NORMAL;        //Normal

	return nStatus;
}
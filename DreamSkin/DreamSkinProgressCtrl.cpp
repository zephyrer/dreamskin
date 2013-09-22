//DreamSkinProgressCtrl.cpp
#include <windows.h>
#include <commctrl.h>

#include "WinGdiEx.h"
#include "DreamSkinMain.h"
#include "DreamSkinLoader.h"
#include "DreamSkinWindow.h"
#include "DreamSkinProgressCtrl.h"

//static member declaration
WNDPROC          CDreamSkinProgressCtrl::s_DefaultWindowProc = NULL;
SKINPROGRESSCTRL CDreamSkinProgressCtrl::s_SkinProgressCtrl;

CDreamSkinProgressCtrl::CDreamSkinProgressCtrl(HWND hWnd, WNDPROC OrgWndProc)
	: CDreamSkinWindow(hWnd, OrgWndProc)
{
	m_pSkinProgressCtrl = &s_SkinProgressCtrl;
}

CDreamSkinProgressCtrl::~CDreamSkinProgressCtrl()
{
	if (m_pSkinProgressCtrl != &s_SkinProgressCtrl)
		delete m_pSkinProgressCtrl;
}

void CDreamSkinProgressCtrl::Reload()
{
	if (m_hWnd && ::IsWindow(m_hWnd))
	{
		RECT rcClient;
		::GetClientRect(m_hWnd, &rcClient);

		m_bCreateRegion = TRUE;
		::InvalidateRect(m_hWnd, &rcClient, TRUE);
	}
}

BOOL CDreamSkinProgressCtrl::ApplySkin(CDreamSkinLoader *pLoader)
{
	BOOL bResult = TRUE;

	InitialClass();

	SKINPROGRESSCTRL SkinProgressCtrl;
	pLoader->GetSkinProgressCtrl(&SkinProgressCtrl);
	s_SkinProgressCtrl = SkinProgressCtrl;

	return bResult;
}

CDreamSkinProgressCtrl* CDreamSkinProgressCtrl::CreateInstance(HWND hWnd, WNDPROC OrgWndProc)
{
	CDreamSkinProgressCtrl *pSkinProgressCtrl = new CDreamSkinProgressCtrl(hWnd, OrgWndProc);
	return pSkinProgressCtrl;
}

LRESULT WINAPI CDreamSkinProgressCtrl::DefWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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

BOOL CDreamSkinProgressCtrl::InitialClass()
{
	//Get the default window proc for this class
	WNDCLASSW clsProgressCtrlDefault;
	if(!GetClassInfoW(GetModuleHandle(NULL), DEFAULT_PROGRESSCTRL_CLASSNAME_W, &clsProgressCtrlDefault))
	{
		//Get class info failed
		//TODO: add error handle code
		return FALSE;
	}
	s_DefaultWindowProc = clsProgressCtrlDefault.lpfnWndProc;

	GetDefaultSkin(&s_SkinProgressCtrl);

	return TRUE;
}

void CDreamSkinProgressCtrl::DestroyClass()
{

}

BOOL CDreamSkinProgressCtrl::GetDefaultSkin(SKINPROGRESSCTRL *pSkinProgressCtrl)
{
	if (pSkinProgressCtrl)
	{
		GetDefaultBackground(&pSkinProgressCtrl->skinHBkNormal, RGB(255, 255, 255));
		GetDefaultBackground(&pSkinProgressCtrl->skinHFrNormal, RGB(0, 0, 255));
		GetDefaultBackground(&pSkinProgressCtrl->skinVBkNormal, RGB(255, 255, 255));
		GetDefaultBackground(&pSkinProgressCtrl->skinVFrNormal, RGB(0, 0, 255));

		GetDefaultBorder(&pSkinProgressCtrl->skinHLBorderNormal, RGB(113, 111, 100), 1);        //COLOR_3DDKSHADOW
		GetDefaultBorder(&pSkinProgressCtrl->skinHRBorderNormal, RGB(113, 111, 100), 1);
		GetDefaultBorder(&pSkinProgressCtrl->skinHTBorderNormal, RGB(113, 111, 100), 1);
		GetDefaultBorder(&pSkinProgressCtrl->skinHBBorderNormal, RGB(113, 111, 100), 1);

		GetDefaultBorder(&pSkinProgressCtrl->skinVLBorderNormal, RGB(113, 111, 100), 1);        //COLOR_3DDKSHADOW
		GetDefaultBorder(&pSkinProgressCtrl->skinVRBorderNormal, RGB(113, 111, 100), 1);
		GetDefaultBorder(&pSkinProgressCtrl->skinVTBorderNormal, RGB(113, 111, 100), 1);
		GetDefaultBorder(&pSkinProgressCtrl->skinVBBorderNormal, RGB(113, 111, 100), 1);

		pSkinProgressCtrl->nHBlockWidth = 10;
		pSkinProgressCtrl->nHBlockSpacing = 2;
		pSkinProgressCtrl->nVBlockWidth = 10;
		pSkinProgressCtrl->nVBlockSpacing = 2;
	}

	return TRUE;
}

LRESULT CDreamSkinProgressCtrl::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT nResult = ERROR_SUCCESS;

	switch(message)
	{
	case WM_ERASEBKGND:
		break;
	case WM_PAINT:
		nResult = OnPaint();
		break;
	case PBM_SETPOS:
		nResult = CDreamSkinWindow::DefWindowProc(message, wParam, lParam);
		::InvalidateRect(m_hWnd, NULL, FALSE);
		break;
	default:
		nResult = CDreamSkinWindow::DefWindowProc(message, wParam, lParam);
	}

	return nResult;
}

LRESULT CDreamSkinProgressCtrl::OnPaint()
{
	PAINTSTRUCT ps;
	HDC hPaintDC = ::BeginPaint(m_hWnd, &ps);

	RECT rcWindow;
	::GetClientRect(m_hWnd, &rcWindow);

	DWORD dwStyle = ::GetWindowLong(m_hWnd, GWL_STYLE);
	if (dwStyle & PBS_VERTICAL)
	{
		CDreamSkinWindow::DrawBackground(hPaintDC, rcWindow, &m_pSkinProgressCtrl->skinVBkNormal);

		DrawVertForeground(hPaintDC, rcWindow);

		CDreamSkinWindow::DrawBorder(hPaintDC, &m_pSkinProgressCtrl->skinVLBorderNormal, &m_pSkinProgressCtrl->skinVRBorderNormal, &m_pSkinProgressCtrl->skinVTBorderNormal, &m_pSkinProgressCtrl->skinVBBorderNormal, rcWindow);
	}
	else
	{
		CDreamSkinWindow::DrawBackground(hPaintDC, rcWindow, &m_pSkinProgressCtrl->skinHBkNormal);

		DrawHorzForeground(hPaintDC, rcWindow);

		CDreamSkinWindow::DrawBorder(hPaintDC, &m_pSkinProgressCtrl->skinHLBorderNormal, &m_pSkinProgressCtrl->skinHRBorderNormal, &m_pSkinProgressCtrl->skinHTBorderNormal, &m_pSkinProgressCtrl->skinHBBorderNormal, rcWindow);
	}

	::EndPaint(m_hWnd, &ps);

	return 0;
}

void CDreamSkinProgressCtrl::DrawHorzForeground(HDC hDC, RECT rcClient)
{
	int nPos;
	PBRANGE pbRang;
	RECT rcDraw = rcClient;

	::SendMessage(m_hWnd, PBM_GETRANGE, TRUE, (LPARAM)&pbRang);
	nPos = (int)::SendMessage(m_hWnd, PBM_GETPOS, 0, 0);

	if (nPos < pbRang.iLow)
		nPos = pbRang.iLow;
	else if (nPos > pbRang.iHigh)
		nPos = pbRang.iHigh;

	if (pbRang.iHigh > pbRang.iLow)
		rcDraw.right = ((nPos - pbRang.iLow) * rcClient.right + (pbRang.iHigh - nPos) * rcClient.left) / (pbRang.iHigh - pbRang.iLow);
	else
		rcDraw.right = rcDraw.left;

	CDreamSkinWindow::DrawBackground(hDC, rcDraw, &m_pSkinProgressCtrl->skinHFrNormal);
}

void CDreamSkinProgressCtrl::DrawVertForeground(HDC hDC, RECT rcClient)
{
	int nPos;
	PBRANGE pbRang;
	RECT rcDraw = rcClient;

	::SendMessage(m_hWnd, PBM_GETRANGE, TRUE, (LPARAM)&pbRang);
	nPos = (int)::SendMessage(m_hWnd, PBM_GETPOS, 0, 0);

	if (nPos < pbRang.iLow)
		nPos = pbRang.iLow;
	else if (nPos > pbRang.iHigh)
		nPos = pbRang.iHigh;

	if (pbRang.iHigh > pbRang.iLow)
		rcDraw.top = ((nPos - pbRang.iLow) * rcClient.top + (pbRang.iHigh - nPos) * rcClient.bottom) / (pbRang.iHigh - pbRang.iLow);
	else
		rcDraw.top = rcDraw.bottom;

	CDreamSkinWindow::DrawBackground(hDC, rcDraw, &m_pSkinProgressCtrl->skinVFrNormal);
}
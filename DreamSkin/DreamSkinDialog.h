//DreamSkinDialog.h

#ifndef DREAMSKIN_DIALOG_INCLUDE
#define DREAMSKIN_DIALOG_INCLUDE

#define DEFAULT_DIALOG_CLASSNAME_A ("#32770")
#define DEFAULT_DIALOG_CLASSNAME_W (L"#32770")

class CDreamSkinDialog : public CDreamSkinWindow
{
//Contrustor and Destructor
public:
	CDreamSkinDialog(HWND hWnd, WNDPROC OrgWndProc);
	virtual ~CDreamSkinDialog();

	virtual void    Reload();

public:
	//Apply new skin
	static BOOL ApplySkin(CDreamSkinLoader *pLoader);
	//Create an instance of this class
	static CDreamSkinDialog * CreateInstance(HWND hWnd, WNDPROC OrgWndProc);
	//window procedure for dreamskin dialog, which provides basic skin functions
	static LRESULT WINAPI DefWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	//Initial the static member of the class
	static BOOL InitialClass();
	//Destroy the static member of the class
	static void DestroyClass();
	//Get the default skin for dialog
	static BOOL GetDefaultSkin(SKINDIALOG *pSkinDialog);
	

public:
	static WNDPROC          s_DefaultWindowProc;      //Default dialog window proc
	static SKINDIALOG       s_SkinDialog;             //Dialog Skin Settings

protected:
	static void GetDefaultBackground(SKINBACKGROUND *pBackground, COLORREF clrBk);
	static void GetDefaultBorder(SKINBORDER *pBorder, COLORREF clrBk, int nWidth);
	static void GetDefaultSysButton(SKINSYSBUTTON *pSysButton, COLORREF clrBk, COLORREF clrText);
	static void GetDefaultTitleBar(SKINTITLEBAR *pTitleBar);

public:
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//process the WM_ACTIVATE message
	virtual LRESULT OnActive(UINT unState, HANDLE hWndOther);
	//process the WM_CREATE message
	virtual LRESULT OnCreate(LPCREATESTRUCT lpCreateStruct);
	//process the WM_DRAWITEM message
	virtual LRESULT OnDrawItem(UINT nCtrlID, LPDRAWITEMSTRUCT lpDrawItem);
	//process the WM_MEASUREITEM message
	virtual LRESULT OnMesureItem(int nCtrlID, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	//process the WM_NCACTIVE message
	virtual LRESULT OnNcActive(BOOL bActive);
	//proces the WM_NCCALCSIZE message
	virtual LRESULT OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp);
	//process the WM_NCHITTEST message
	virtual LRESULT OnNcHitTest(POINTS point);
	//process the WM_NCLBUTTONDOWN message
	virtual LRESULT OnNcLButtonDown(DWORD dwHitTest, POINTS point);
	//process the WM_NCLBUTTONUP message
	virtual LRESULT OnNcLButtonUp(DWORD dwHitTest, POINTS point);
	//process the WM_NCMOUSEMOVE message
	virtual LRESULT OnNcMouseMove(UINT nHitTest, POINTS point);
	//process the WM_NCPAINT message
	virtual LRESULT OnNcPaint(HRGN hRGN);
	//process the WM_PAINT message
	virtual LRESULT OnPaint();
	//process the WM_SIZE message
	virtual LRESULT OnSize(UINT nType, WORD cx, WORD cy);

	virtual inline void UpdateWindow()
	{
		if (m_hWnd && ::IsWindow(m_hWnd))
		{
			if (m_bActive)
				::SetWindowPos(m_hWnd, 0, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED );
			else
				::SetWindowPos(m_hWnd, 0, 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED );

			RECT rcClient;
			::GetClientRect(m_hWnd, &rcClient);
			::InvalidateRect(m_hWnd, &rcClient, TRUE);
			::UpdateWindow(m_hWnd);
		}
	}

	virtual inline void UpdateWindow(BOOL bClient)
	{
		if (m_hWnd && ::IsWindow(m_hWnd))
		{
			if(bClient)
			{
				RECT rcClient;
				::GetClientRect(m_hWnd, &rcClient);
				::InvalidateRect(m_hWnd, &rcClient, TRUE);
				::UpdateWindow(m_hWnd);
			}
			else
			{
				if (m_bActive)
					::SetWindowPos(m_hWnd, 0, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED );
				else
					::SetWindowPos(m_hWnd, 0, 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED );
			}
		}
	}

protected:
	//Create window region
	virtual void    CreateWindowRegion(HDC hDC);
	//Draw the background of the dialog
	virtual void    DrawBackground(HDC hDC, RECT rcClient);
	//Draw the close button
	virtual void    DrawCloseBtn(HDC hDC, RECT rcClose);
	//Draw the maximize/restore button
	virtual void    DrawMaxBtn(HDC hDC, RECT rcMax);
	//Draw the minimize button
	virtual void    DrawMinBtn(HDC hDC, RECT rcMin);
	//Draw title bar
	virtual void    DrawTitleBar(HDC hDC, RECT rcTitle);


	//Get rect of client
	virtual RECT    GetRectClient(RECT rcWindow);
	//Get rect of close button
	virtual RECT    GetRectClose(RECT rcWindow);
	//Get rect of maximize button
	virtual RECT    GetRectMax(RECT rcWindow);
	//Get rect of minimize button
	virtual RECT    GetRectMin(RECT rcWindow, BOOL bMaxBox);
	//Get rect of icon
	virtual RECT    GetRectIcon(RECT rcWindow);
	//Get region in rect
	virtual HRGN    GetRegionInRect(HDC hDC, RECT rcTarget, COLORREF clrTransparent);

	//Draw the border of the dialog
	virtual void    OnDrawBorder(HDC hDC, RECT rcWindow);

	//Draw the sys buttons of the dialog
	virtual void    OnDrawSysButton(HDC hDC, RECT rcWindow);

	//Check whether point in border
	virtual int     PointOnBorder(POINT point, RECT rcWindow, DWORD dwStyle);

protected:
	SKINDIALOG      *m_pSkinDialog;                   //Instance Related Dialog Skin Settings

	BOOL             m_bActive;                       //Indicate whether the dialog is in active status
	UINT             m_nLastHitTest;                  //Store the last NcHitTest value to check if the window need to be redraw
	UINT             m_nNcLButtonDown;                //Store the last NcLButtonDown value to check if need to invoke any event
};

#endif //end of DREAMSKIN_DIALOG_INCLUDE
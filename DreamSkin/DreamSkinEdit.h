//DreamSkinEdit.h

#ifndef DREAMSKIN_EDIT_INCLUDE
#define DREAMSKIN_EDIT_INCLUDE

#define DEFAULT_EDIT_CLASSNAME_A ("Edit")
#define DEFAULT_EDIT_CLASSNAME_W (L"Edit")

class CDreamSkinEdit : public CDreamSkinWindow
{
//Contrustor and Destructor
public:
	CDreamSkinEdit(HWND hWnd, WNDPROC OrgWndProc);
	virtual ~CDreamSkinEdit();

	virtual void    Reload();

public:
	//Apply new skin
	static BOOL ApplySkin(CDreamSkinLoader *pLoader);
	//Create an instance of this class
	static CDreamSkinEdit * CreateInstance(HWND hWnd, WNDPROC OrgWndProc);
	//window procedure for dreamskin dialog, which provides basic skin functions
	static LRESULT WINAPI DefWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	//Initial the static member of the class
	static BOOL InitialClass();
	//Destroy the static member of the class
	static void DestroyClass();
	//Get the default skin for static
	static BOOL GetDefaultSkin(SKINEDIT *pSkinEdit);

public:
	static WNDPROC          s_DefaultWindowProc;      //Default static window proc
	static SKINEDIT         s_SkinEdit;               //Edit Skin Settings

protected:
	//Create background brush
	static HBRUSH  CreateBackgroundBrush(SKINBACKGROUND *pSkinBackground);
	static void GetDefaultBackground(SKINBACKGROUND *pBackground, COLORREF clrBk);
	static void GetDefaultText(SKINTEXT *pText, COLORREF clrTxt);
	static void GetDefaultBorder(SKINBORDER *pBorder, COLORREF clrBk, int nWidth);

public:
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//process the WM_CTLCOLOREDIT
	virtual LRESULT OnCtrlColor(HDC hDC, HWND hWndParent);
	//process the WM_MOUSELEAVE message
	virtual LRESULT OnMouseLeave();
	//process the WM_MOUSEMOVE message
	virtual LRESULT OnMouseMove(UINT nHitTest, POINTS point);
	//process the WM_NCCALCSIZE message
	virtual LRESULT OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp);
	//process the WM_NCPAINT message
	virtual LRESULT OnNcPaint(HRGN hRGN);

protected:
	//Draw the border of the edit
	void    DrawBorder(HDC hDC, RECT rcWindow);

	//Get current button status
	int     GetCurrentStatus(DWORD dwStyle) const;
	//Get rect of client
	RECT    GetRectClient(RECT rcWindow);

	


protected:
	SKINEDIT        *m_pSkinEdit;                     //Instance Related Static Skin Settings

	HBRUSH           m_hBkNormal;                     //Brush to draw background in normal status
	HBRUSH           m_hBkDisable;                    //Brush to draw background in disable status
	HBRUSH           m_hBkHover;                      //Brush to draw background in hover status
	HBRUSH           m_hBkPress;                      //Brush to draw background in focus status

	HBRUSH           m_hBkNormalReadOnly;             //Brush to draw background in normal status for readonly edit
	HBRUSH           m_hBkHoverReadOnly;              //Brush to draw background in hover status for readonly edit
	HBRUSH           m_hBkPressReadOnly;              //Brush to draw background in focus status for readonly edit

	BOOL             m_bMouseIn;                      //Whether the mouse over the button
};

#endif // end of DREAMSKIN_EDIT_INCLUDE
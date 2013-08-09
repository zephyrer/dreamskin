//DreamSkinTab.h

#ifndef DREAMSKIN_TAB_INCLUDE
#define DREAMSKIN_TAB_INCLUDE

#define DEFAULT_TAB_CLASSNAME_A ("SysTabControl32")
#define DEFAULT_TAB_CLASSNAME_W (L"SysTabControl32")

class CDreamSkinTab : public CDreamSkinWindow
{
//Contrustor and Destructor
public:
	CDreamSkinTab(HWND hWnd, WNDPROC OrgWndProc);
	virtual ~CDreamSkinTab();

	virtual void    Reload();

public:
	//Apply new skin
	static BOOL ApplySkin(CDreamSkinLoader *pLoader);
	//Create an instance of this class
	static CDreamSkinTab * CreateInstance(HWND hWnd, WNDPROC OrgWndProc);
	//window procedure for dreamskin dialog, which provides basic skin functions
	static LRESULT WINAPI DefWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	//Initial the static member of the class
	static BOOL InitialClass();
	//Destroy the static member of the class
	static void DestroyClass();
	//Get the default skin for static
	static BOOL GetDefaultSkin(SKINTAB *pSkinTab);

public:
	static WNDPROC          s_DefaultWindowProc;      //Default static window proc
	static SKINTAB          s_SkinTab;                //Tab Skin Settings

protected:
	static void GetDefaultBackground(SKINBACKGROUND *pBackground, COLORREF clrBk);
	static void GetDefaultBorder(SKINBORDER *pBorder, COLORREF clrBk, int nWidth, int nDrawType = DRAWTYPE_FILLCOLOR);
	static void GetDefaultButton(SKINBUTTON *pSkinButton);
	static void GetDefaultText(SKINTEXT *pText, COLORREF clrTxt);

public:
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//process the WM_MOUSELEAVE message
	virtual LRESULT OnMouseLeave();
	//process the WM_MOUSEMOVE message
	virtual LRESULT OnMouseMove(UINT nFlags, POINTS point);
	//process the WM_PAINT message
	virtual LRESULT OnPaint();

protected:
	//Draw the background of the static
	void    DrawBackground(HDC hDC, RECT rcWindow, RECT rcClient);
	//Draw the border of the tab
	void    DrawBorder(HDC hDC, RECT rcWindow, RECT rcClient);
	//Draw one button
	void    DrawButton(HDC hDC, RECT rcItem, int nIndex);
	void    DrawButtonBackground(HDC hDC, RECT rcClient, int nStatus);
	void	DrawButtonBorder(HDC hDC, RECT rcItem, int nStatus);
	void	DrawButtonTitle(HDC hDC, RECT rcClient, int nIndex, int nStatus);
	//Draw all the buttons
	void    DrawButtons(HDC hDC, RECT rcWindow, RECT rcClient);
	//Draw the title of the static
	//virtual void    DrawTitle(HDC hDC, RECT rcClient);
	

	//Get the client rect by window rect
	RECT    GetClientByWindow(RECT rcWindow);
	//Get the window rect by client
	RECT    GetWindowByClient(RECT rcClient);
	//Get rect of button client
	RECT    GetButtonRectClient(RECT rcItem, int nStatus);

protected:
	SKINTAB         *m_pSkinTab;                      //Instance Related Tab Skin Settings
	int              m_nCurSel;                       //Current selected item
	int              m_nCurHover;                     //Current hover item
};

#endif // end of DREAMSKIN_TAB_INCLUDE
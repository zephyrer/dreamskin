//DreamSkinButton.h

#ifndef DREAMSKIN_BUTTON_INCLUDE
#define DREAMSKIN_BUTTON_INCLUDE

#define DEFAULT_BUTTON_CLASSNAME_A ("Button")
#define DEFAULT_BUTTON_CLASSNAME_W (L"Button")

class CDreamSkinButton : public CDreamSkinWindow
{
//Contrustor and Destructor
public:
	CDreamSkinButton(HWND hWnd, WNDPROC OrgWndProc);
	virtual ~CDreamSkinButton();

	virtual void    Init();

	virtual void    Destroy();

	virtual void    Reload();

public:
	//Apply new skin
	static BOOL ApplySkin(CDreamSkinLoader *pLoader);
	//Create an instance of this class
	static CDreamSkinButton * CreateInstance(HWND hWnd, WNDPROC OrgWndProc);
	//window procedure for dreamskin dialog, which provides basic skin functions
	static LRESULT WINAPI DefWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	//Initial the static member of the class
	static BOOL InitialClass();
	//Destroy the static member of the class
	static void DestroyClass();
	//Get the default skin for dialog
	static BOOL GetDefaultSkin(SKINBUTTON *pSkinButton);

public:
	static WNDPROC          s_DefaultWindowProc;      //Default button window proc
	static SKINBUTTON       s_SkinButton;             //Button Skin Settings

protected:
	static void GetDefaultBackground(SKINBACKGROUND *pBackground, COLORREF clrBk);
	static void GetDefaultBorder(SKINBORDER *pBorder, COLORREF clrBk, int nWidth);
	static void GetDefaultText(SKINTEXT *pText, COLORREF clrTxt);

public:
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//process the WM_LBUTTONDOWN message
	virtual LRESULT OnLButtonDown(UINT nFlags, POINTS point);
	//process the WM_LBUTTONDOWN message
	virtual LRESULT OnLButtonUp(UINT nFlags, POINTS point);
	//process the WM_MOUSELEAVE message
	virtual LRESULT OnMouseLeave();
	//process the WM_MOUSEMOVE message
	virtual LRESULT OnMouseMove(UINT nHitTest, POINTS point);
	//process the WM_NCHITTEST message
	virtual LRESULT OnNcHitTest(POINTS point);
	//process the WM_PAINT message
	virtual LRESULT OnPaint();

protected:
	//Draw the background of the button
	virtual void    DrawBackground(HDC hDC, RECT rcClient);
	//Draw the border of the button
	virtual void    DrawBorder(HDC hDC, RECT rcWindow);
	//Draw the title of the button
	virtual void    DrawTitle(HDC hDC, RECT rcClient);

	//Get current window status
	virtual int     GetCurrentStatus() const;

	//Get rect of client
	virtual RECT    GetRectClient(RECT rcWindow);
	

public:
	SKINBUTTON      *m_pSkinButton;                   //Instance Related Button Skin Settings

protected:
	BOOL             m_bMouseIn;                      //Whether the mouse over the button
	BOOL             m_bLBtnDown;                     //Whether the mouse left button down on the button
};

#endif // end of DREAMSKIN_BUTTON_INCLUDE
//DreamSkinButton.h

#ifndef DREAMSKIN_BUTTON_INCLUDE
#define DREAMSKIN_BUTTON_INCLUDE

#define DEFAULT_BUTTON_CLASSNAME_A ("Button")
#define DEFAULT_BUTTON_CLASSNAME_W (L"Button")

//button type
#define BUTTON_TYPE_UNSUPPORTED         -1
#define BUTTON_TYPE_PUSHBUTTON          0
#define BUTTON_TYPE_CHECKBOX            1

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
	//Get the default skin for button
	static BOOL GetDefaultButtonSkin(SKINBUTTON *pSkinButton);
	//Get the default skin for check box
	static BOOL GetDefaultCheckBoxSkin(SKINCHECKBOX *pSkinCheckBox);

public:
	static WNDPROC          s_DefaultWindowProc;      //Default button window proc
	static SKINBUTTON       s_SkinButton;             //Button Skin Settings
	static SKINCHECKBOX     s_SkinCheckBox;           //Check Box Skin Settings

protected:
	static void GetDefaultBackground(SKINBACKGROUND *pBackground, COLORREF clrBk);
	static void GetDefaultBorder(SKINBORDER *pBorder, COLORREF clrBk, int nWidth);
	static void GetDefaultText(SKINTEXT *pText, COLORREF clrTxt);
	static void GetDefaultIcon(SKINICON *pIcon, COLORREF clrDraw);

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
	//Draw push button
	void    DrawPushButton(HDC hDC, RECT rcWindow);
	//Draw check box
	void    DrawCheckBox(HDC hDC, RECT rcWindow);
	//Draw the background of the button
	void    DrawPushButtonBackground(HDC hDC, RECT rcClient);
	//Draw the border of the button
	void    DrawPushButtonBorder(HDC hDC, RECT rcWindow);
	//Draw the title of the button
	void    DrawPushButtonTitle(HDC hDC, RECT rcClient);
	//Draw the background of the checkbox
	void    DrawCheckBoxBackground(HDC hDC, RECT rcClient, int nCheckStatus);
	//Draw the foreground of the checkbox
	void    DrawCheckBoxForeground(HDC hDC, RECT rcClient, int nCheckStatus);
	//Draw the checkbox icon
	void    DrawCheckBoxIcon(HDC hDC, RECT rcDraw, SKINICON *pSkinIcon, int nCheckStatus);
	//Draw the title of the checkbox
	void    DrawCheckBoxTitle(HDC hDC, RECT rcClient, SKINTEXT *pSkinText);

	//Get current button status
	int     GetCurrentStatus() const;

	//Get rect of client
	RECT    GetPushButtonRectClient(RECT rcWindow);
	//Get rect of icon
	RECT    GetCheckBoxRectIcon(RECT rcWindow, SKINICON *pSkinIcon);
	

public:
	SKINBUTTON      *m_pSkinButton;                   //Instance Related Button Skin Settings
	SKINCHECKBOX    *m_pSkinCheckBox;                 //Instance Related CheckBox Skin Settings

protected:
	int              m_nBtnType;                      //The button type
	BOOL             m_bMouseIn;                      //Whether the mouse over the button
	BOOL             m_bLBtnDown;                     //Whether the mouse left button down on the button
};

#endif // end of DREAMSKIN_BUTTON_INCLUDE
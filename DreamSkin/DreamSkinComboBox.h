// DreamSkinComboBox.h


#ifndef DREAMSKIN_COMBOBOX_INCLUDE
#define DREAMSKIN_COMBOBOX_INCLUDE

#define DEFAULT_COMBOBOX_CLASSNAME_A ("ComboBox")
#define DEFAULT_COMBOBOX_CLASSNAME_W (L"ComboBox")

class CDreamSkinComboBox : public CDreamSkinWindow
{
//Contrustor and Destructor
public:
	CDreamSkinComboBox(HWND hWnd, WNDPROC OrgWndProc);
	virtual ~CDreamSkinComboBox();

	virtual void    Reload();

	virtual int     GetType() { return DREAMSKIN_COMBOBOX; };

public:
	//Apply new skin
	static BOOL ApplySkin(CDreamSkinLoader *pLoader);
	//Create an instance of this class
	static CDreamSkinComboBox * CreateInstance(HWND hWnd, WNDPROC OrgWndProc);
	//window procedure for dreamskin dialog, which provides basic skin functions
	static LRESULT WINAPI DefWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	//Initial the static member of the class
	static BOOL InitialClass();
	//Destroy the static member of the class
	static void DestroyClass();
	//Get the default skin for static
	static BOOL GetDefaultSkin(SKINCOMBOBOX *pSkinComboBox);

public:
	static WNDPROC          s_DefaultWindowProc;      //Default static window proc
	static SKINCOMBOBOX     s_SkinComboBox;           //Combo Box Skin Settings

public:
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//process the WM_CTLCOLOREDIT
	virtual LRESULT OnCtrlColor(HDC hDC, HWND hWndEdit);
	//process the WM_CREATE message
	virtual LRESULT OnCreate(LPCREATESTRUCT lpCreateStruct);
	//process the WM_DRAWITEM message
	virtual LRESULT OnDrawItem(UINT nCtrlID, LPDRAWITEMSTRUCT lpDrawItem);
	//process the WM_LBUTTONDOWN message
	virtual LRESULT OnLButtonDown(UINT nFlags, POINTS point);
	//process the WM_LBUTTONUP message
	virtual LRESULT OnLButtonUp(UINT nFlags, POINTS point);
	//process the WM_MOUSELEAVE message
	virtual LRESULT OnMouseLeave();
	//process the WM_MOUSEMOVE message
	virtual LRESULT OnMouseMove(UINT nFlags, POINTS point);
	//process the WM_PAINT message
	virtual LRESULT OnPaint();

	//Draw the listbox border
	void    DrawLBBorder(HDC hDC, RECT rcWindow);

public:
	int                m_nCurHover;                   //Current hover item
	int                m_nOwnerDraw;                  //Whether the listbox is owner draw

protected:
	//Create background brush
	static HBRUSH  CreateBackgroundBrush(SKINBACKGROUND *pSkinBackground);

	static void DrawButton(HDC hDC, RECT rcDraw, SKINSYSBUTTON *pSysButton);


	//Draw the background of the static
	void    DrawBackground(HDC hDC, RECT rcClient, DWORD dwStyle);
	//Draw the border
	void    DrawBorder(HDC hDC, RECT rcWindow);
	//Draw the drop down button
	void    DrawButton(HDC hDC, RECT rcDraw, DWORD dwStyle);

	//Get current button status
	int     GetCurrentStatus(DWORD dwStyle) const;

protected:
	SKINCOMBOBOX      *m_pSkinComboBox;               //Instance Related Combo Box Skin Settings
	BOOL               m_bBtnHover;                   //Mouse in dropdown button
	BOOL               m_bBtnPress;                   //Dropdown button was pressed
	BOOL               m_bMouseIn;                    //Whether the mouse over the combo box
	HBRUSH             m_hBkNormal;                   //Brush to draw background in normal status
	HBRUSH             m_hBkDisable;                  //Brush to draw background in disable status
};

#endif // end of DREAMSKIN_COMBOBOX_INCLUDE
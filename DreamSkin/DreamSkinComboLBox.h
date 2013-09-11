// DreamSkinComboLBox.h

#ifndef DREAMSKIN_COMBOLBOX_INCLUDE
#define DREAMSKIN_COMBOLBOX_INCLUDE

#define DEFAULT_COMBOLBOX_CLASSNAME_A ("ComboLBox")
#define DEFAULT_COMBOLBOX_CLASSNAME_W (L"ComboLBox")

class CDreamSkinComboLBox : public CDreamSkinWindow
{
//Contrustor and Destructor
public:
	CDreamSkinComboLBox(HWND hWnd, WNDPROC OrgWndProc);
	virtual ~CDreamSkinComboLBox();

	virtual void    Reload();

public:
	//Apply new skin
	static BOOL ApplySkin(CDreamSkinLoader *pLoader);
	//Create an instance of this class
	static CDreamSkinComboLBox * CreateInstance(HWND hWnd, WNDPROC OrgWndProc);
	//window procedure for dreamskin dialog, which provides basic skin functions
	static LRESULT WINAPI DefWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	//Initial the static member of the class
	static BOOL InitialClass();
	//Destroy the static member of the class
	static void DestroyClass();

public:
	static WNDPROC          s_DefaultWindowProc;      //Default static window proc

public:
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//process the WM_CREATE message
	virtual LRESULT OnCreate(LPCREATESTRUCT lpCreateStruct);
	//process the WM_MOUSELEAVE message
	virtual LRESULT OnMouseLeave();
	//process the WM_MOUSEMOVE message
	virtual LRESULT OnMouseMove(UINT nFlags, POINTS point);
	//process the WM_NCHITTEST message
	virtual LRESULT OnNcHitTest(POINTS point);
	//process the WM_NCLBUTTONDBLCLK message
	virtual LRESULT OnNcLButtonDbClick(UINT nHitTest, POINTS point);
	//process the WM_NCLBUTTONDOWN message
	virtual LRESULT OnNcLButtonDown(UINT nHitTest, POINTS point);
	//process the WM_NCMOUSELEAVE
	virtual LRESULT OnNcMouseLeave();
	//process the WM_NCMOUSEMOVE message
	virtual LRESULT OnNcMouseMove(UINT nHitTest, POINTS point);
	//process the WM_NCPAINT message
	virtual LRESULT OnNcPaint(HRGN hRGN);

protected:
	CDreamSkinComboBox *m_pWndComboBox;               //Dreamskin window for parent combo box;
	int                 m_nOwnerDraw;                 //Whether the listbox is owner draw
	UINT                m_nSBHover;                   //Current scroll bar hover item
	SCROLLBARTRACKINFO  m_ScrollBarTrackInfo;         //Store the scrollbar track info
};

#endif // end of DREAMSKIN_COMBOLBOX_INCLUDE
//DreamSkinSliderCtrl.h

#ifndef DREAMSKIN_SLIDERCTRL_INCLUDE
#define DREAMSKIN_SLIDERCTRL_INCLUDE

#define DEFAULT_SLIDERCTRL_CLASSNAME_A ("msctls_trackbar32")
#define DEFAULT_SLIDERCTRL_CLASSNAME_W (L"msctls_trackbar32")

#define SCHT_NOWHERE          0
#define SCHT_ARROW_TOP        1
#define SCHT_ARROW_BOTTOM     2
#define SCHT_REGION_UP        3
#define SCHT_REGION_DOWN      4
#define SCHT_THUMB            5

typedef struct _tag_SLIDERCTRLTRACKINFO
{
	UINT nLButtonDown;
	BOOL bIsTracking;
	int  nTrackPos;
}SLIDERCTRLTRACKINFO;

class CDreamSkinSliderCtrl : public CDreamSkinWindow
{
//Contrustor and Destructor
public:
	CDreamSkinSliderCtrl(HWND hWnd, WNDPROC OrgWndProc);
	virtual ~CDreamSkinSliderCtrl();

	virtual void    Reload();

public:
	//Apply new skin
	static BOOL ApplySkin(CDreamSkinLoader *pLoader);
	//Create an instance of this class
	static CDreamSkinSliderCtrl * CreateInstance(HWND hWnd, WNDPROC OrgWndProc);
	//window procedure for dreamskin dialog, which provides basic skin functions
	static LRESULT WINAPI DefWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	//Initial the static member of the class
	static BOOL InitialClass();
	//Destroy the static member of the class
	static void DestroyClass();
	//Get the default skin for static
	static BOOL GetDefaultSkin(SKINSLIDERCTRL *pSkinSliderCtrl);

	

public:
	static WNDPROC          s_DefaultWindowProc;      //Default static window proc
	static SKINSLIDERCTRL   s_SkinSliderCtrl;         //SliderCtrl Skin Settings

public:
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//process the WM_HSCROLL message
	virtual LRESULT OnHScroll(UINT nSBCode, HWND hWndCtrl);
	//process the WM_LBUTTONDOWN message
	virtual LRESULT OnLButtonDown(UINT nFlags, POINTS point);
	//process the WM_MOUSELEAVE message
	virtual LRESULT OnMouseLeave();
	//process the WM_MOUSEMOVE message
	virtual LRESULT OnMouseMove(UINT nFlags, POINTS point);
	//process the WM_PAINT message
	virtual LRESULT OnPaint();
	//process the WM_VSCROLL message
	virtual LRESULT OnVScroll(UINT nSBCode, HWND hWndCtrl);

protected:
	//Draw the foreground of the progress control
	void    DrawHorzSliderCtrl(HDC hDC, RECT rcClient, DWORD dwStyle);
	void    DrawVertSliderCtrl(HDC hDC, RECT rcClient, DWORD dwStyle);

	void    GetHorzThumbRect(RECT rcWindow, RECT *prcSlider, RECT *prcThumb);
	void    GetVertThumbRect(RECT rcWindow, RECT *prcSlider, RECT *prcThumb);

	int     SliderCtrlDragPos(RECT rcSlider, POINT point, int nOffset, DWORD dwStyle);
	int     SliderCtrlHitTest(RECT rcWindow, POINT point, DWORD dwStyle);
	void    TrackSliderCtrl(DWORD dwStyle, int nHitTest);

protected:
	SKINSLIDERCTRL     *m_pSkinSliderCtrl;            //Instance Related SliderCtrl Skin Settings
	int                 m_nHover;                     //Current slider control hover item
	SLIDERCTRLTRACKINFO m_SliderCtrlTrackInfo;        //Store the slider control track info
};

#endif // end of DREAMSKIN_SLIDERCTRL_INCLUDE
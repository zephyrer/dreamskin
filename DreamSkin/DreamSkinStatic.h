//DreamSkinStatic.h

#ifndef DREAMSKIN_STATIC_INCLUDE
#define DREAMSKIN_STATIC_INCLUDE

#define DEFAULT_STATIC_CLASSNAME_A ("Static")
#define DEFAULT_STATIC_CLASSNAME_W (L"Static")

class CDreamSkinStatic : public CDreamSkinWindow
{
//Contrustor and Destructor
public:
	CDreamSkinStatic(HWND hWnd, WNDPROC OrgWndProc);
	virtual ~CDreamSkinStatic();

	virtual void    Reload();

public:
	//Apply new skin
	static BOOL ApplySkin(CDreamSkinLoader *pLoader);
	//Create an instance of this class
	static CDreamSkinStatic * CreateInstance(HWND hWnd, WNDPROC OrgWndProc);
	//window procedure for dreamskin dialog, which provides basic skin functions
	static LRESULT WINAPI DefWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	//Initial the static member of the class
	static BOOL InitialClass();
	//Destroy the static member of the class
	static void DestroyClass();
	//Get the default skin for static
	static BOOL GetDefaultSkin(SKINSTATIC *pSkinStatic);

public:
	static WNDPROC          s_DefaultWindowProc;      //Default static window proc
	static SKINSTATIC       s_SkinStatic;             //Static Skin Settings

protected:
	static void GetDefaultBackground(SKINBACKGROUND *pBackground, COLORREF clrBk);
	static void GetDefaultText(SKINTEXT *pText, COLORREF clrTxt);

public:
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//process the WM_PAINT message
	virtual LRESULT OnPaint();

protected:
	//Draw the background of the static
	virtual void    DrawBackground(HDC hDC, RECT rcClient);
	//Draw the title of the static
	virtual void    DrawTitle(HDC hDC, RECT rcClient);

protected:
	SKINSTATIC      *m_pSkinStatic;                   //Instance Related Static Skin Settings
};

#endif // end of DREAMSKIN_STATIC_INCLUDE
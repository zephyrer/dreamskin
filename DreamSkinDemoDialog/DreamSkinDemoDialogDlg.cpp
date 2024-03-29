
// DreamSkinDemoDialogDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DreamSkinDemoDialog.h"
#include "DreamSkinDemoDialogDlg.h"
#include "CtrlDemoThread.h"

//DreamSkin Support
#include "../DreamSkin/DreamSkin.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CDreamSkinDemoDialogDlg dialog


LRESULT WINAPI DefWindowProcBtnDemo(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return ((CDreamSkinDemoDialogDlg*)(AfxGetApp()->GetMainWnd()))->DefWindowProcBtnDemo(hWnd, message, wParam, lParam);
}

LRESULT WINAPI DefWindowProcChkDemo(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return ((CDreamSkinDemoDialogDlg*)(AfxGetApp()->GetMainWnd()))->DefWindowProcChkDemo(hWnd, message, wParam, lParam);
}

LRESULT WINAPI DefWindowProcRadioDemo(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return ((CDreamSkinDemoDialogDlg*)(AfxGetApp()->GetMainWnd()))->DefWindowProcRadioDemo(hWnd, message, wParam, lParam);
}

LRESULT CDreamSkinDemoDialogDlg::DefWindowProcBtnDemo(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (::IsWindow(m_hWnd) && ::IsWindow(hWnd))
	{
		switch(message)
		{
		case WM_MOUSEMOVE:
			if (::GetCapture() == hWnd)
			{
				if (m_nBtnDemoStatus != 2)
				{
					m_nBtnDemoStatus = 2;
					::SetWindowText(hWnd, _T("Press"));
				}
			}
			else
			{
				if (m_nBtnDemoStatus != 1)
				{
					m_nBtnDemoStatus = 1;
					::SetWindowText(hWnd, _T("Hover"));
				}
			}
			break;
		case WM_MOUSELEAVE:
			if (m_nBtnDemoStatus != 0)
			{
				m_nBtnDemoStatus = 0;
				::SetWindowText(hWnd, _T("Normal"));
			}
			break;
		case WM_LBUTTONDOWN:
			if (m_nBtnDemoStatus != 2)
			{
				m_nBtnDemoStatus = 2;
				::SetWindowText(hWnd, _T("Press"));
			}
			break;
		}
	}

	return m_SkinProcBtnDemo(hWnd, message, wParam, lParam);
}

LRESULT CDreamSkinDemoDialogDlg::DefWindowProcChkDemo(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	TCHAR tstrText[64];
	int *pChkDemoStatus;
	int nStatus;
	WNDPROC SkinProcChkDemo;
	if (::IsWindow(m_hWnd) && ::IsWindow(hWnd))
	{
		if (hWnd == ::GetDlgItem(m_hWnd, IDC_CHK_DEMO_NORMAL))
		{
			pChkDemoStatus = &m_nChk2StateDemoStatus;
			_tcscpy_s(tstrText, 64, _T("2-States Checkbox - "));
			SkinProcChkDemo = m_SkinProcChk2StateDemo;
		}
		else
		{
			pChkDemoStatus = &m_nChk3StateDemoStatus;
			_tcscpy_s(tstrText, 64, _T("3-States Checkbox - "));
			SkinProcChkDemo = m_SkinProcChk3StateDemo;
		}

		switch(::SendMessage(hWnd, BM_GETCHECK, 0, 0))
		{
		case BST_CHECKED:
			nStatus = 1;
			_tcscat_s(tstrText, 64, _T("Checked "));
			break;
		case BST_INDETERMINATE:
			nStatus = 2;
			_tcscat_s(tstrText, 64, _T("Partially-Checked "));
			break;
		default:
			nStatus = 0;
			_tcscat_s(tstrText, 64, _T("Unchecked "));
			break;
		}

		switch(message)
		{
		case WM_MOUSEMOVE:
			if (::GetCapture() == hWnd)
			{
				nStatus = nStatus * 10 + 2;
				if (*pChkDemoStatus != nStatus)
				{
					*pChkDemoStatus = nStatus;
					_tcscat_s(tstrText, 64, _T("Press"));
					::SetWindowText(hWnd, tstrText);
				}
			}
			else
			{
				nStatus = nStatus * 10 + 1;
				if (*pChkDemoStatus != nStatus)
				{
					*pChkDemoStatus = nStatus;
					_tcscat_s(tstrText, 64, _T("Hover"));
					::SetWindowText(hWnd, tstrText);
				}
			}
			break;
		case WM_MOUSELEAVE:
			nStatus = nStatus * 10 + 0;
			if (*pChkDemoStatus != nStatus)
			{
				*pChkDemoStatus = nStatus;
				_tcscat_s(tstrText, 64, _T("Normal"));
				::SetWindowText(hWnd, tstrText);
			}
			break;
		case WM_LBUTTONDOWN:
			nStatus = nStatus * 10 + 2;
			if (*pChkDemoStatus != nStatus)
			{
				*pChkDemoStatus = nStatus;
				_tcscat_s(tstrText, 64, _T("Press"));
				::SetWindowText(hWnd, tstrText);
			}
			break;
		}
	}

	return SkinProcChkDemo(hWnd, message, wParam, lParam);
}

LRESULT CDreamSkinDemoDialogDlg::DefWindowProcRadioDemo(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	TCHAR tstrText[64];
	int *pRadioDemoStatus;
	int nStatus;
	WNDPROC SkinProcRadioDemo;
	if (::IsWindow(m_hWnd) && ::IsWindow(hWnd))
	{
		if (hWnd == ::GetDlgItem(m_hWnd, IDC_RADIO_NORMAL1))
		{
			pRadioDemoStatus = &m_nRadioDemo1Status;
			_tcscpy_s(tstrText, 64, _T("Radio1 - "));
			SkinProcRadioDemo = m_SkinProcRadio1Demo;
		}
		else
		{
			pRadioDemoStatus = &m_nRadioDemo2Status;
			_tcscpy_s(tstrText, 64, _T("Radio2 - "));
			SkinProcRadioDemo = m_SkinProcRadio2Demo;
		}

		switch(::SendMessage(hWnd, BM_GETCHECK, 0, 0))
		{
		case BST_CHECKED:
			nStatus = 1;
			_tcscat_s(tstrText, 64, _T("Selected "));
			break;
		default:
			nStatus = 0;
			_tcscat_s(tstrText, 64, _T("Unselected "));
			break;
		}

		switch(message)
		{
		case WM_MOUSEMOVE:
			if (::GetCapture() == hWnd)
			{
				nStatus = nStatus * 10 + 2;
				if (*pRadioDemoStatus != nStatus)
				{
					*pRadioDemoStatus = nStatus;
					_tcscat_s(tstrText, 64, _T("Press"));
					::SetWindowText(hWnd, tstrText);
				}
			}
			else
			{
				nStatus = nStatus * 10 + 1;
				if (*pRadioDemoStatus != nStatus)
				{
					*pRadioDemoStatus = nStatus;
					_tcscat_s(tstrText, 64, _T("Hover"));
					::SetWindowText(hWnd, tstrText);
				}
			}
			break;
		case WM_MOUSELEAVE:
			nStatus = nStatus * 10 + 0;
			if (*pRadioDemoStatus != nStatus)
			{
				*pRadioDemoStatus = nStatus;
				_tcscat_s(tstrText, 64, _T("Normal"));
				::SetWindowText(hWnd, tstrText);
			}
			break;
		case WM_LBUTTONDOWN:
			nStatus = nStatus * 10 + 2;
			if (*pRadioDemoStatus != nStatus)
			{
				*pRadioDemoStatus = nStatus;
				_tcscat_s(tstrText, 64, _T("Press"));
				::SetWindowText(hWnd, tstrText);
			}
			break;
		case BM_SETCHECK:
			nStatus = nStatus * 10 + (*pRadioDemoStatus % 10);
			if (*pRadioDemoStatus != nStatus)
			{
				*pRadioDemoStatus = nStatus;
				switch (nStatus % 10)
				{
				case 1:
					_tcscat_s(tstrText, 64, _T("Hover"));
					break;
				case 2:
					_tcscat_s(tstrText, 64, _T("Press"));
					break;
				default:
					_tcscat_s(tstrText, 64, _T("Normal"));
					break;
				}
				
				::SetWindowText(hWnd, tstrText);
			}
		}
	}

	return SkinProcRadioDemo(hWnd, message, wParam, lParam);
}

CDreamSkinDemoDialogDlg::CDreamSkinDemoDialogDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDreamSkinDemoDialogDlg::IDD, pParent)
	, m_bSysMenuInclude(TRUE)
	, m_bEnableDreamSkin(FALSE)
	, m_bMaximizeBoxInclude(TRUE)
	, m_bMinimizeBoxInclude(TRUE)
	, m_bTitleBarInclude(TRUE)
	, m_bBorderInclude(TRUE)
	, m_bBorderSizable(FALSE)
	, m_bIconInclude(TRUE)
	, m_nHookedWindowCount(0)
	, m_strSkinPath(_T(""))
	, m_strEditDemoNormal(_T("Normal Edit Control"))
	, m_strEditDemoReadOnly(_T("Read Only - Radio1 Selected"))
	, m_strEditDemoDisable(_T("Disabled Edit Control"))
	, m_nRadioDemo(0)
	, m_nRadioDisabled(0)
	, m_nHookedThreadCount(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_nBtnDemoStatus = 0;
	m_nChk2StateDemoStatus = 0;
	m_nChk3StateDemoStatus = 0;
	m_nRadioDemo1Status = 0;
	m_nRadioDemo2Status = 0;
}

void CDreamSkinDemoDialogDlg::UpdateDreamSkinStatus()
{
	BOOL bUpdate = FALSE;
	DREAMSKIN_STATUS status;
	DreamSkinStatus(&status);
	if (m_nHookedWindowCount != status.nHookedWindowCount)
	{
		m_nHookedWindowCount = status.nHookedWindowCount;
		bUpdate = TRUE;
	}

	if (m_nHookedThreadCount != status.nHookedThreadCount)
	{
		m_nHookedThreadCount = status.nHookedThreadCount;
		bUpdate = TRUE;
	}

	if (bUpdate)
		UpdateData(FALSE);
}

void CDreamSkinDemoDialogDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_SYSMENU_INCLUDE, m_bSysMenuInclude);
	DDX_Check(pDX, IDC_DREAMSKIN_ENABLE, m_bEnableDreamSkin);
	DDX_Check(pDX, IDC_MAXIMIZEBOX_INCLUDE, m_bMaximizeBoxInclude);
	DDX_Check(pDX, IDC_MINIMIZEBOX_INCLUDE, m_bMinimizeBoxInclude);
	DDX_Check(pDX, IDC_TITLEBAR_INCLUDE, m_bTitleBarInclude);
	DDX_Check(pDX, IDC_BORDER_INCLUDE, m_bBorderInclude);
	DDX_Check(pDX, IDC_BORDER_SIZABLE, m_bBorderSizable);
	DDX_Check(pDX, IDC_ICON_INCLUDE, m_bIconInclude);
	DDX_Text(pDX, IDC_EDIT_HOOKED_WINDOW_COUNT, m_nHookedWindowCount);
	DDX_Text(pDX, IDC_EDIT_HOOKED_THREAD_COUNT, m_nHookedThreadCount);
	DDX_Text(pDX, IDC_EDIT_SKIN_PATH, m_strSkinPath);
	DDX_Text(pDX, IDC_EDIT_DEMO_NORMAL, m_strEditDemoNormal);
	DDX_Text(pDX, IDC_EDIT_DEMO_READONLY, m_strEditDemoReadOnly);
	DDX_Text(pDX, IDC_EDIT_DEMO_DISABLE, m_strEditDemoDisable);
	DDX_Control(pDX, IDC_TAB_QUICK_DEMO, m_tabQuickDemo);
	DDX_Radio(pDX, IDC_RADIO_NORMAL1, m_nRadioDemo);
	DDX_Radio(pDX, IDC_RADIO_DISABLE_SELECTED, m_nRadioDisabled);
}

BEGIN_MESSAGE_MAP(CDreamSkinDemoDialogDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_SYSMENU_INCLUDE, &CDreamSkinDemoDialogDlg::OnBnClickedSysmenuInclude)
	ON_BN_CLICKED(IDC_DREAMSKIN_ENABLE, &CDreamSkinDemoDialogDlg::OnBnClickedDreamskinEnable)
	ON_BN_CLICKED(IDC_MAXIMIZEBOX_INCLUDE, &CDreamSkinDemoDialogDlg::OnBnClickedMaximizeboxInclude)
	ON_BN_CLICKED(IDC_MINIMIZEBOX_INCLUDE, &CDreamSkinDemoDialogDlg::OnBnClickedMinimizeboxInclude)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_TITLEBAR_INCLUDE, &CDreamSkinDemoDialogDlg::OnBnClickedTitlebarInclude)
	ON_BN_CLICKED(IDC_BORDER_INCLUDE, &CDreamSkinDemoDialogDlg::OnBnClickedBorderInclude)
	ON_BN_CLICKED(IDC_BORDER_SIZABLE, &CDreamSkinDemoDialogDlg::OnBnClickedBorderSizable)
	ON_BN_CLICKED(IDC_ICON_INCLUDE, &CDreamSkinDemoDialogDlg::OnBnClickedIconInclude)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_LOADSKIN, &CDreamSkinDemoDialogDlg::OnBnClickedBtnLoadskin)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_QUICK_DEMO, &CDreamSkinDemoDialogDlg::OnTcnSelchangeTabQuickDemo)
	ON_BN_CLICKED(IDC_RADIO_NORMAL1, &CDreamSkinDemoDialogDlg::OnBnClickedRadioNormal)
	ON_BN_CLICKED(IDC_RADIO_NORMAL2, &CDreamSkinDemoDialogDlg::OnBnClickedRadioNormal)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_NEWDIALOG, &CDreamSkinDemoDialogDlg::OnBnClickedBtnNewdialog)
END_MESSAGE_MAP()


// CDreamSkinDemoDialogDlg message handlers

BOOL CDreamSkinDemoDialogDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	m_tabQuickDemo.InsertItem(0, _T("Common Controls"));
	m_tabQuickDemo.InsertItem(1, _T("Dialog Settings"));
	ShowDialogSettings(FALSE);

	((CButton*)GetDlgItem(IDC_CHK_DEMO_DISABLE_CHECKED))->SetCheck(BST_CHECKED);
	((CButton*)GetDlgItem(IDC_CHK_DEMO_DISABLE_PARTCHECKED))->SetCheck(BST_INDETERMINATE);

	DREAMSKIN_STATUS status;
	DreamSkinStatus(&status);
	m_nHookedWindowCount = status.nHookedWindowCount;
	m_nHookedThreadCount = status.nHookedThreadCount;

	SetTimer(1, 1000, 0);
	

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDreamSkinDemoDialogDlg::ShowDialogSettings(BOOL bShow)
{
	int nCmdShow;
	if (bShow)
		nCmdShow = SW_SHOW;
	else
		nCmdShow = SW_HIDE;

	GetDlgItem(IDC_BORDER_INCLUDE)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_BORDER_SIZABLE)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_TITLEBAR_INCLUDE)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_SYSMENU_INCLUDE)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_MAXIMIZEBOX_INCLUDE)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_MINIMIZEBOX_INCLUDE)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_ICON_INCLUDE)->ShowWindow(nCmdShow);
}

void CDreamSkinDemoDialogDlg::ShowCommonControls(BOOL bShow)
{
	int nCmdShow;
	if (bShow)
		nCmdShow = SW_SHOW;
	else
		nCmdShow = SW_HIDE;

	GetDlgItem(IDC_STATIC_QUICK_DEMO)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_STATIC_BUTTONS)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_BTN_DEMO_NORMAL)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_BTN_DEMO_DISABLE)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_STATIC_CHECKBOXES)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_CHK_DEMO_NORMAL)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_CHK_DEMO_3STATE_NORMAL)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_CHK_DEMO_DISABLE_UNCHECKED)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_CHK_DEMO_DISABLE_CHECKED)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_CHK_DEMO_DISABLE_PARTCHECKED)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_STATIC_EDITS)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_EDIT_DEMO_NORMAL)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_EDIT_DEMO_READONLY)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_EDIT_DEMO_DISABLE)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_STATIC_RATIOS)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_RADIO_NORMAL1)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_RADIO_NORMAL2)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_RADIO_DISABLE_SELECTED)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_RADIO_DISABLE_UNSELECTED)->ShowWindow(nCmdShow);
}

void CDreamSkinDemoDialogDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CDreamSkinDemoDialogDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CDreamSkinDemoDialogDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CDreamSkinDemoDialogDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
}

void CDreamSkinDemoDialogDlg::OnBnClickedDreamskinEnable()
{
	UpdateData(TRUE);

	TCHAR tstrText[64];
	HWND hWnd;

	if (m_bEnableDreamSkin)
	{
		DreamSkinInit();

		GetDlgItem(IDC_BTN_DEMO_NORMAL)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_DEMO_NORMAL)->EnableWindow();

		GetDlgItem(IDC_CHK_DEMO_NORMAL)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHK_DEMO_NORMAL)->EnableWindow();

		GetDlgItem(IDC_CHK_DEMO_3STATE_NORMAL)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHK_DEMO_3STATE_NORMAL)->EnableWindow();

		GetDlgItem(IDC_RADIO_NORMAL1)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_NORMAL1)->EnableWindow();

		GetDlgItem(IDC_RADIO_NORMAL2)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_NORMAL2)->EnableWindow();

		m_SkinProcBtnDemo = (WNDPROC)::GetWindowLong(GetDlgItem(IDC_BTN_DEMO_NORMAL)->m_hWnd, GWL_WNDPROC);
		::SetWindowLong(GetDlgItem(IDC_BTN_DEMO_NORMAL)->m_hWnd, GWL_WNDPROC, (LONG)::DefWindowProcBtnDemo);

		hWnd = ::GetDlgItem(m_hWnd, IDC_CHK_DEMO_NORMAL);
		_tcscpy_s(tstrText, 64, _T("2-States Checkbox - "));
		switch(::SendMessage(hWnd, BM_GETCHECK, 0, 0))
		{
		case BST_CHECKED:
			m_nChk2StateDemoStatus = 10;
			_tcscat_s(tstrText, 64, _T("Checked Normal"));
			break;
		case BST_INDETERMINATE:
			m_nChk2StateDemoStatus = 20;
			_tcscat_s(tstrText, 64, _T("Partially-Checked Normal"));
			break;
		default:
			m_nChk2StateDemoStatus = 0;
			_tcscat_s(tstrText, 64, _T("Unchecked Normal"));
			break;
		}
		::SetWindowText(hWnd, tstrText);
		m_SkinProcChk2StateDemo = (WNDPROC)::GetWindowLong(hWnd, GWL_WNDPROC);
		::SetWindowLong(hWnd, GWL_WNDPROC, (LONG)::DefWindowProcChkDemo);
		

		hWnd = ::GetDlgItem(m_hWnd, IDC_CHK_DEMO_3STATE_NORMAL);
		_tcscpy_s(tstrText, 64, _T("3-States Checkbox - "));
		switch(::SendMessage(hWnd, BM_GETCHECK, 0, 0))
		{
		case BST_CHECKED:
			m_nChk3StateDemoStatus = 10;
			_tcscat_s(tstrText, 64, _T("Checked Normal"));
			break;
		case BST_INDETERMINATE:
			m_nChk3StateDemoStatus = 20;
			_tcscat_s(tstrText, 64, _T("Partially-Checked Normal"));
			break;
		default:
			m_nChk3StateDemoStatus = 0;
			_tcscat_s(tstrText, 64, _T("Unchecked Normal"));
			break;
		}
		::SetWindowText(hWnd, tstrText);
		m_SkinProcChk3StateDemo = (WNDPROC)::GetWindowLong(hWnd, GWL_WNDPROC);
		::SetWindowLong(hWnd, GWL_WNDPROC, (LONG)::DefWindowProcChkDemo);

		hWnd = ::GetDlgItem(m_hWnd, IDC_RADIO_NORMAL1);
		m_SkinProcRadio1Demo = (WNDPROC)::GetWindowLong(hWnd, GWL_WNDPROC);
		::SetWindowLong(hWnd, GWL_WNDPROC, (LONG)::DefWindowProcRadioDemo);

		hWnd = ::GetDlgItem(m_hWnd, IDC_RADIO_NORMAL2);
		m_SkinProcRadio2Demo = (WNDPROC)::GetWindowLong(hWnd, GWL_WNDPROC);
		::SetWindowLong(hWnd, GWL_WNDPROC, (LONG)::DefWindowProcRadioDemo);
	}
	else
	{
		hWnd = ::GetDlgItem(m_hWnd, IDC_CHK_DEMO_NORMAL);
		_tcscpy_s(tstrText, 64, _T("2-States Checkbox - "));
		switch(::SendMessage(hWnd, BM_GETCHECK, 0, 0))
		{
		case BST_CHECKED:
			m_nChk2StateDemoStatus = 10;
			_tcscat_s(tstrText, 64, _T("Checked Normal"));
			break;
		case BST_INDETERMINATE:
			m_nChk2StateDemoStatus = 20;
			_tcscat_s(tstrText, 64, _T("Partially-Checked Normal"));
			break;
		default:
			m_nChk2StateDemoStatus = 0;
			_tcscat_s(tstrText, 64, _T("Unchecked Normal"));
			break;
		}
		::SetWindowText(hWnd, tstrText);
		::SetWindowLong(hWnd, GWL_WNDPROC, (LONG)m_SkinProcChk2StateDemo);
		

		hWnd = ::GetDlgItem(m_hWnd, IDC_CHK_DEMO_3STATE_NORMAL);
		_tcscpy_s(tstrText, 64, _T("3-States Checkbox - "));
		switch(::SendMessage(hWnd, BM_GETCHECK, 0, 0))
		{
		case BST_CHECKED:
			m_nChk3StateDemoStatus = 10;
			_tcscat_s(tstrText, 64, _T("Checked Normal"));
			break;
		case BST_INDETERMINATE:
			m_nChk3StateDemoStatus = 20;
			_tcscat_s(tstrText, 64, _T("Partially-Checked Normal"));
			break;
		default:
			m_nChk3StateDemoStatus = 0;
			_tcscat_s(tstrText, 64, _T("Unchecked Normal"));
			break;
		}
		::SetWindowText(hWnd, tstrText);
		::SetWindowLong(hWnd, GWL_WNDPROC, (LONG)m_SkinProcChk3StateDemo);

		hWnd = ::GetDlgItem(m_hWnd, IDC_RADIO_NORMAL1);
		::SetWindowLong(hWnd, GWL_WNDPROC, (LONG)m_SkinProcRadio1Demo);

		hWnd = ::GetDlgItem(m_hWnd, IDC_RADIO_NORMAL2);
		::SetWindowLong(hWnd, GWL_WNDPROC, (LONG)m_SkinProcRadio2Demo);

		DreamSkinExit();
	}

	UpdateData(FALSE);
}


void CDreamSkinDemoDialogDlg::OnBnClickedSysmenuInclude()
{
	UpdateData(TRUE);

	DWORD dwStyle = ::GetWindowLong(m_hWnd, GWL_STYLE);
	BOOL bEnable = TRUE;
	if (m_bSysMenuInclude)
	{
		dwStyle |= WS_SYSMENU;
	}
	else
	{
		dwStyle &= ~WS_SYSMENU;
		bEnable = FALSE;
	}
	::SetWindowLong(m_hWnd, GWL_STYLE, dwStyle);
	::SetWindowPos(m_hWnd, 0, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);

	GetDlgItem(IDC_MAXIMIZEBOX_INCLUDE)->EnableWindow(bEnable);
	GetDlgItem(IDC_MINIMIZEBOX_INCLUDE)->EnableWindow(bEnable);

	UpdateData(FALSE);
}

void CDreamSkinDemoDialogDlg::OnBnClickedMaximizeboxInclude()
{
	UpdateData(TRUE);

	DWORD dwStyle = ::GetWindowLong(m_hWnd, GWL_STYLE);
	if (m_bMaximizeBoxInclude)
	{
		dwStyle |= WS_MAXIMIZEBOX;
	}
	else
	{
		dwStyle &= ~WS_MAXIMIZEBOX;
	}
	::SetWindowLong(m_hWnd, GWL_STYLE, dwStyle);
	::SetWindowPos(m_hWnd, 0, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);

	UpdateData(FALSE);
}

void CDreamSkinDemoDialogDlg::OnBnClickedMinimizeboxInclude()
{
	UpdateData(TRUE);

	DWORD dwStyle = ::GetWindowLong(m_hWnd, GWL_STYLE);
	if (m_bMinimizeBoxInclude)
	{
		dwStyle |= WS_MINIMIZEBOX;
	}
	else
	{
		dwStyle &= ~WS_MINIMIZEBOX;
	}
	::SetWindowLong(m_hWnd, GWL_STYLE, dwStyle);
	::SetWindowPos(m_hWnd, 0, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);

	UpdateData(FALSE);
}

void CDreamSkinDemoDialogDlg::OnBnClickedTitlebarInclude()
{
	UpdateData(TRUE);

	DWORD dwStyle = ::GetWindowLong(m_hWnd, GWL_STYLE);
	if (m_bTitleBarInclude)
	{
		dwStyle |= WS_CAPTION;
	}
	else
	{
		dwStyle &= ~WS_CAPTION;
	}
	::SetWindowLong(m_hWnd, GWL_STYLE, dwStyle);
	::SetWindowPos(m_hWnd, 0, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);

	Invalidate();
	UpdateWindow();

	UpdateData(FALSE);
}

void CDreamSkinDemoDialogDlg::OnBnClickedBorderInclude()
{
	UpdateData(TRUE);

	DWORD dwStyle = ::GetWindowLong(m_hWnd, GWL_STYLE);
	BOOL bEnable = TRUE;
	if (m_bBorderInclude)
	{
		dwStyle |= DS_MODALFRAME;
		dwStyle |= WS_BORDER;
		if (m_bBorderSizable)
			dwStyle |= WS_THICKFRAME;
	}
	else
	{
		dwStyle &= ~DS_MODALFRAME;
		dwStyle &= ~WS_BORDER;
		dwStyle &= ~WS_THICKFRAME;
		bEnable = FALSE;
	}
	::SetWindowLong(m_hWnd, GWL_STYLE, dwStyle);
	::SetWindowPos(m_hWnd, 0, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);

	GetDlgItem(IDC_TITLEBAR_INCLUDE)->EnableWindow(bEnable);
	GetDlgItem(IDC_BORDER_SIZABLE)->EnableWindow(bEnable);

	Invalidate();
	UpdateWindow();

	UpdateData(FALSE);
}

void CDreamSkinDemoDialogDlg::OnBnClickedBorderSizable()
{
	UpdateData(TRUE);

	DWORD dwStyle = ::GetWindowLong(m_hWnd, GWL_STYLE);
	BOOL bEnable = TRUE;
	if (m_bBorderSizable)
	{
		dwStyle |= WS_THICKFRAME;
	}
	else
	{
		dwStyle &= ~WS_THICKFRAME;
	}
	::SetWindowLong(m_hWnd, GWL_STYLE, dwStyle);
	::SetWindowPos(m_hWnd, 0, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);

	UpdateData(FALSE);
}

void CDreamSkinDemoDialogDlg::OnBnClickedIconInclude()
{
	UpdateData(TRUE);

	if (m_bIconInclude)
	{
		SetIcon(m_hIcon, TRUE);			// Set big icon
		SetIcon(m_hIcon, FALSE);		// Set small icon
	}
	else
	{
		SetIcon(NULL, TRUE);			// Set big icon
		SetIcon(NULL, FALSE);		// Set small icon
	}

	::SetWindowPos(m_hWnd, 0, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);

	UpdateData(FALSE);
}

void CDreamSkinDemoDialogDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	switch(nIDEvent)
	{
	case 1:
		UpdateDreamSkinStatus();
		break;
	default:
		CDialog::OnTimer(nIDEvent);
	}
}

void CDreamSkinDemoDialogDlg::OnBnClickedBtnLoadskin()
{
	UpdateData(TRUE);

	CString strSkinFileFilter;
	strSkinFileFilter.LoadString(IDS_SKIN_FILE_FILTER);

	CFileDialog OpenFileDialog(TRUE, _T("dsf"), m_strSkinPath, OFN_FILEMUSTEXIST, strSkinFileFilter);
	if(OpenFileDialog.DoModal() == IDOK)
	{
		if (DreamSkinLoad((LPCTSTR)(OpenFileDialog.GetPathName())))
		{
			m_strSkinPath = OpenFileDialog.GetPathName();
		}
	}

	UpdateData(FALSE);
}

void CDreamSkinDemoDialogDlg::OnTcnSelchangeTabQuickDemo(NMHDR *pNMHDR, LRESULT *pResult)
{
	int sel = m_tabQuickDemo.GetCurSel(); 

	switch(sel) 
	{
	case 1:
		ShowCommonControls(FALSE);
		ShowDialogSettings(TRUE);
		break;
	case 0:
		ShowDialogSettings(FALSE);
		ShowCommonControls(TRUE);
		break;
	}

	*pResult = 0;
}

void CDreamSkinDemoDialogDlg::OnBnClickedRadioNormal()
{
	UpdateData();

	if (m_nRadioDemo)
	{
		m_strEditDemoReadOnly = _T("Read Only - Radio2 Selected");
	}
	else
	{
		m_strEditDemoReadOnly = _T("Read Only - Radio1 Selected");
	}

	UpdateData(FALSE);
}

void CDreamSkinDemoDialogDlg::OnDestroy()
{
	CDialog::OnDestroy();
}

void CDreamSkinDemoDialogDlg::OnBnClickedBtnNewdialog()
{
	CWinThread* pThread = AfxBeginThread( RUNTIME_CLASS( CCtrlDemoThread ) );
	if ( !pThread )
		AfxMessageBox( _T( "Thread creation failed!" ), MB_YESNOCANCEL | MB_ICONWARNING ); 
}

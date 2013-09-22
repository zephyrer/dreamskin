// PropertyPageCommonControl.cpp : implementation file
//

#include "stdafx.h"
#include "DreamSkinDemoDialog.h"
#include "PropertyPageCommonControl.h"


// CPropertyPageCommonControl dialog

IMPLEMENT_DYNAMIC(CPropertyPageCommonControl, CPropertyPage)

CPropertyPageCommonControl::CPropertyPageCommonControl()
	: CPropertyPage(CPropertyPageCommonControl::IDD)
	, m_bEnableAll(TRUE)
	, m_bEnableComboBox(TRUE)
	, m_strResultComboBox(_T(""))
	, m_bEnableProgress(TRUE)
	, m_nSliderDemoHorz(0)
	, m_nSliderDemoVert(0)
	, m_nResultProgress(0)
{
	m_psp.dwFlags &= ~PSP_HASHELP;
	m_nProgressTimerID = 0;
	m_bPauseProgress = FALSE;
}

CPropertyPageCommonControl::~CPropertyPageCommonControl()
{

}

void CPropertyPageCommonControl::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CMB_DEMO_SIMPLE, m_cmbDemoSimple);
	DDX_Control(pDX, IDC_CMD_DEMO_DROPDOWN, m_cmbDemoDropdown);
	DDX_Control(pDX, IDC_CMD_DEMO_DROPLIST, m_cmbDemoDropList);
	DDX_Check(pDX, IDC_CHK_ENABLE_ALL, m_bEnableAll);
	DDX_Check(pDX, IDC_CHK_ENABLE_COMBOBOX, m_bEnableComboBox);
	DDX_Text(pDX, IDC_EDIT_RESULT_COMBOBOX, m_strResultComboBox);
	DDX_Check(pDX, IDC_CHK_ENABLE_PROGRESS, m_bEnableProgress);
	DDX_Slider(pDX, IDC_SLIDER_DEMO_HORZ, m_nSliderDemoHorz);
	DDV_MinMaxInt(pDX, m_nSliderDemoHorz, 0, 100);
	DDX_Slider(pDX, IDC_SLIDER_DEMO_VERT, m_nSliderDemoVert);
	DDV_MinMaxInt(pDX, m_nSliderDemoVert, 0, 100);
	DDX_Control(pDX, IDC_PROGRESS_DEMO_HORZ, m_prgDemoHorz);
	DDX_Control(pDX, IDC_PROGRESS_DEMO_VERT, m_prgDemoVert);
	DDX_Control(pDX, IDC_SCROLLBAR_DEMO_HORZ, m_sbDemoHorz);
	DDX_Control(pDX, IDC_SCROLLBAR_DEMO_VERT, m_sbDemoVert);
	DDX_Text(pDX, IDC_EDIT_RESULT_PROGRESS, m_nResultProgress);
	DDV_MinMaxInt(pDX, m_nResultProgress, 0, 100);
}


BEGIN_MESSAGE_MAP(CPropertyPageCommonControl, CPropertyPage)
	ON_BN_CLICKED(IDC_CHK_ENABLE_COMBOBOX, &CPropertyPageCommonControl::OnBnClickedChkEnableCombobox)
	ON_BN_CLICKED(IDC_CHK_ENABLE_ALL, &CPropertyPageCommonControl::OnBnClickedChkEnableAll)
	ON_BN_CLICKED(IDC_BTN_CLEAR_COMBOBOX, &CPropertyPageCommonControl::OnBnClickedBtnClearCombobox)
	ON_CBN_SELCHANGE(IDC_CMD_DEMO_DROPLIST, &CPropertyPageCommonControl::OnCbnSelchangeCmdDemoDroplist)
	ON_CBN_SELCHANGE(IDC_CMD_DEMO_DROPDOWN, &CPropertyPageCommonControl::OnCbnSelchangeCmdDemoDropdown)
	ON_CBN_EDITCHANGE(IDC_CMD_DEMO_DROPDOWN, &CPropertyPageCommonControl::OnCbnEditchangeCmdDemoDropdown)
	ON_CBN_SELCHANGE(IDC_CMB_DEMO_SIMPLE, &CPropertyPageCommonControl::OnCbnSelchangeCmbDemoSimple)
	ON_CBN_EDITCHANGE(IDC_CMB_DEMO_SIMPLE, &CPropertyPageCommonControl::OnCbnEditchangeCmbDemoSimple)
	ON_BN_CLICKED(IDC_CHK_ENABLE_PROGRESS, &CPropertyPageCommonControl::OnBnClickedChkEnableProgress)
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_BN_CLICKED(IDC_BTN_PROGRESS_START, &CPropertyPageCommonControl::OnBnClickedBtnProgressStart)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_PROGRESS_PAUSE, &CPropertyPageCommonControl::OnBnClickedBtnProgressPause)
END_MESSAGE_MAP()


// CPropertyPageCommonControl message handlers

BOOL CPropertyPageCommonControl::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	CString strMessage;
	for (int i = 0; i < 100; i++)
	{
		strMessage.Format(_T("Combo Box Item %d"), i);
		m_cmbDemoSimple.AddString((LPCTSTR)strMessage);
		m_cmbDemoDropdown.AddString((LPCTSTR)strMessage);
		m_cmbDemoDropList.AddString((LPCTSTR)strMessage);
	}

	SCROLLINFO ScrollInfo;
	ScrollInfo.cbSize = sizeof(SCROLLINFO);
	ScrollInfo.fMask = SIF_PAGE | SIF_POS | SIF_RANGE;
	ScrollInfo.nMax = 100;
	ScrollInfo.nMin = 0;
	ScrollInfo.nPage = 9;
	ScrollInfo.nPos = 0;
	m_sbDemoHorz.SetScrollInfo(&ScrollInfo);
	m_sbDemoVert.SetScrollInfo(&ScrollInfo);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CPropertyPageCommonControl::EnableComboBox(BOOL bEnable)
{
	GetDlgItem(IDC_CMD_DEMO_DROPLIST)->EnableWindow(bEnable);
	GetDlgItem(IDC_CMD_DEMO_DROPDOWN)->EnableWindow(bEnable);
	GetDlgItem(IDC_CMB_DEMO_SIMPLE)->EnableWindow(bEnable);
}

void CPropertyPageCommonControl::EnableProgress(BOOL bEnable)
{
	GetDlgItem(IDC_SCROLLBAR_DEMO_HORZ)->EnableWindow(bEnable);
	GetDlgItem(IDC_SCROLLBAR_DEMO_VERT)->EnableWindow(bEnable);
	GetDlgItem(IDC_PROGRESS_DEMO_HORZ)->EnableWindow(bEnable);
	GetDlgItem(IDC_PROGRESS_DEMO_VERT)->EnableWindow(bEnable);
	GetDlgItem(IDC_SLIDER_DEMO_HORZ)->EnableWindow(bEnable);
	GetDlgItem(IDC_SLIDER_DEMO_VERT)->EnableWindow(bEnable);
	GetDlgItem(IDC_SPIN_DEMO_HORZ)->EnableWindow(bEnable);
	GetDlgItem(IDC_SPIN_DEMO_VERT)->EnableWindow(bEnable);
}

void CPropertyPageCommonControl::EnableAllStatusUpdate()
{
	if (m_bEnableComboBox && m_bEnableProgress)
	{
		m_bEnableAll = 1;
	}
	else if (m_bEnableComboBox || m_bEnableProgress)
	{
		m_bEnableAll = 2;
	}
	else
	{
		m_bEnableAll = 0;
	}
}

void CPropertyPageCommonControl::ProgressPosUpdate()
{
	m_sbDemoHorz.SetScrollPos(m_nResultProgress);
	m_sbDemoVert.SetScrollPos(m_nResultProgress);
	m_prgDemoHorz.SetPos(m_nResultProgress);
	m_prgDemoVert.SetPos(m_nResultProgress);
	m_nSliderDemoHorz = m_nResultProgress;
	m_nSliderDemoVert = m_nResultProgress;
}

void CPropertyPageCommonControl::OnBnClickedChkEnableAll()
{
	UpdateData();

	m_bEnableAll = m_bEnableAll % 2;

	if (m_bEnableComboBox != m_bEnableAll)
	{
		m_bEnableComboBox = m_bEnableAll;
		EnableComboBox(m_bEnableComboBox);
	}

	if (m_bEnableProgress != m_bEnableAll)
	{
		m_bEnableProgress = m_bEnableAll;
		EnableProgress(m_bEnableProgress);
	}

	UpdateData(FALSE);
}

void CPropertyPageCommonControl::OnBnClickedChkEnableCombobox()
{
	UpdateData();

	EnableComboBox(m_bEnableComboBox);

	EnableAllStatusUpdate();

	UpdateData(FALSE);
}



void CPropertyPageCommonControl::OnBnClickedBtnClearCombobox()
{
	m_cmbDemoSimple.SetCurSel(-1);
	m_cmbDemoDropdown.SetCurSel(-1);
	m_cmbDemoDropList.SetCurSel(-1);

	m_strResultComboBox = _T("");

	UpdateData(FALSE);
}

void CPropertyPageCommonControl::OnCbnSelchangeCmdDemoDroplist()
{
	int nIndex = m_cmbDemoDropList.GetCurSel();
	m_cmbDemoDropdown.SetCurSel(nIndex);
	m_cmbDemoSimple.SetCurSel(nIndex);

	m_cmbDemoDropList.GetLBText(nIndex, m_strResultComboBox);

	UpdateData(FALSE);
}

void CPropertyPageCommonControl::OnCbnSelchangeCmdDemoDropdown()
{
	int nIndex = m_cmbDemoDropdown.GetCurSel();
	m_cmbDemoDropList.SetCurSel(nIndex);
	m_cmbDemoSimple.SetCurSel(nIndex);

	m_cmbDemoDropdown.GetLBText(nIndex, m_strResultComboBox);

	UpdateData(FALSE);
}

void CPropertyPageCommonControl::OnCbnEditchangeCmdDemoDropdown()
{
	m_cmbDemoDropdown.GetWindowText(m_strResultComboBox);
	m_cmbDemoDropList.SetCurSel(-1);
	m_cmbDemoSimple.SetCurSel(-1);
	m_strResultComboBox = _T("Customized Text: ") + m_strResultComboBox;

	UpdateData(FALSE);
}

void CPropertyPageCommonControl::OnCbnSelchangeCmbDemoSimple()
{
	int nIndex = m_cmbDemoSimple.GetCurSel();
	m_cmbDemoDropList.SetCurSel(nIndex);
	m_cmbDemoDropdown.SetCurSel(nIndex);

	m_cmbDemoSimple.GetLBText(nIndex, m_strResultComboBox);

	UpdateData(FALSE);
}

void CPropertyPageCommonControl::OnCbnEditchangeCmbDemoSimple()
{
	m_cmbDemoSimple.GetWindowText(m_strResultComboBox);
	m_cmbDemoDropList.SetCurSel(-1);
	m_cmbDemoDropdown.SetCurSel(-1);
	m_strResultComboBox = _T("Customized Text: ") + m_strResultComboBox;

	UpdateData(FALSE);
}

void CPropertyPageCommonControl::OnBnClickedChkEnableProgress()
{
	UpdateData();

	EnableProgress(m_bEnableProgress);

	EnableAllStatusUpdate();

	UpdateData(FALSE);
}

void CPropertyPageCommonControl::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	SCROLLINFO ScrollInfo;
	if (pScrollBar)
	{
		if (pScrollBar->m_hWnd == m_sbDemoHorz.m_hWnd)
		{
			switch(nSBCode)
			{
			case SB_LEFT:
				m_nResultProgress = 0;
				break;
			case SB_LINELEFT:
				if (m_nResultProgress > 0)
					m_nResultProgress--;
				break;
			case SB_LINERIGHT:
				if (m_nResultProgress < 100)
					m_nResultProgress++;
				break;
			case SB_PAGELEFT:
				pScrollBar->GetScrollInfo(&ScrollInfo, SIF_PAGE);
				m_nResultProgress -= ScrollInfo.nPage;
				if (m_nResultProgress < 0)
					m_nResultProgress = 0;
				break;
			case SB_PAGERIGHT:
				pScrollBar->GetScrollInfo(&ScrollInfo, SIF_PAGE);
				m_nResultProgress += ScrollInfo.nPage;
				if (m_nResultProgress > 100)
					m_nResultProgress = 100;
				break;
			case SB_RIGHT:
				m_nResultProgress = 100;
				break;
			case SB_THUMBTRACK:
				m_nResultProgress = nPos;
				if (m_nResultProgress > 100)
					m_nResultProgress = 100;
				else if (m_nResultProgress < 0)
					m_nResultProgress = 0;
				break;
			default:
				CPropertyPage::OnHScroll(nSBCode, nPos, pScrollBar);
				break;
			}

			ProgressPosUpdate();
			UpdateData(FALSE);
		}
		else if (pScrollBar->m_hWnd == GetDlgItem(IDC_SLIDER_DEMO_HORZ)->m_hWnd)
		{
			UpdateData();
			m_nResultProgress = m_nSliderDemoHorz;
			ProgressPosUpdate();
			UpdateData(FALSE);
		}
		else
		{
			CPropertyPage::OnHScroll(nSBCode, nPos, pScrollBar);
		}
	}
	else
	{
		CPropertyPage::OnHScroll(nSBCode, nPos, pScrollBar);
	}
}

void CPropertyPageCommonControl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	SCROLLINFO ScrollInfo;
	if (pScrollBar)
	{
		if (pScrollBar->m_hWnd == m_sbDemoVert.m_hWnd)
		{
			switch(nSBCode)
			{
			case SB_TOP:
				m_nResultProgress = 0;
				break;
			case SB_LINEUP:
				if (m_nResultProgress > 0)
					m_nResultProgress--;
				break;
			case SB_LINEDOWN:
				if (m_nResultProgress < 100)
					m_nResultProgress++;
				break;
			case SB_PAGEUP:
				pScrollBar->GetScrollInfo(&ScrollInfo, SIF_PAGE);
				m_nResultProgress -= ScrollInfo.nPage;
				if (m_nResultProgress < 0)
					m_nResultProgress = 0;
				break;
			case SB_PAGEDOWN:
				pScrollBar->GetScrollInfo(&ScrollInfo, SIF_PAGE);
				m_nResultProgress += ScrollInfo.nPage;
				if (m_nResultProgress > 100)
					m_nResultProgress = 100;
				break;
			case SB_BOTTOM:
				m_nResultProgress = 100;
				break;
			case SB_THUMBTRACK:
				m_nResultProgress = nPos;
				if (m_nResultProgress > 100)
					m_nResultProgress = 100;
				else if (m_nResultProgress < 0)
					m_nResultProgress = 0;
				break;
			default:
				CPropertyPage::OnHScroll(nSBCode, nPos, pScrollBar);
				break;
			}

			ProgressPosUpdate();
			UpdateData(FALSE);
		}
		else if (pScrollBar->m_hWnd == GetDlgItem(IDC_SLIDER_DEMO_VERT)->m_hWnd)
		{
			UpdateData();
			m_nResultProgress = m_nSliderDemoVert;
			ProgressPosUpdate();
			UpdateData(FALSE);
		}
		else
		{
			CPropertyPage::OnHScroll(nSBCode, nPos, pScrollBar);
		}
	}
	else
	{
		CPropertyPage::OnHScroll(nSBCode, nPos, pScrollBar);
	}
}
void CPropertyPageCommonControl::OnBnClickedBtnProgressStart()
{
	GetDlgItem(IDC_BTN_PROGRESS_START)->EnableWindow(FALSE);

	if (!m_nProgressTimerID)
		m_nProgressTimerID = SetTimer(1, 100, NULL);
	
	GetDlgItem(IDC_BTN_PROGRESS_PAUSE)->EnableWindow(TRUE);
}

void CPropertyPageCommonControl::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 1)
	{
		if (!m_bPauseProgress)
		{
			m_nResultProgress++;
			if (m_nResultProgress >= 100)
				m_nResultProgress = 0;

			ProgressPosUpdate();
			UpdateData(FALSE);
		}
	}
	else
	{
		CPropertyPage::OnTimer(nIDEvent);
	}
}

void CPropertyPageCommonControl::OnBnClickedBtnProgressPause()
{
	if (m_bPauseProgress)
	{
		m_bPauseProgress = FALSE;
		GetDlgItem(IDC_BTN_PROGRESS_PAUSE)->SetWindowText(_T("Pause"));
	}
	else
	{
		m_bPauseProgress = TRUE;
		GetDlgItem(IDC_BTN_PROGRESS_PAUSE)->SetWindowText(_T("Resume"));
	}
}

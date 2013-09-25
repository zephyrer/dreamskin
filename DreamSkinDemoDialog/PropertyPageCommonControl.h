#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CPropertyPageCommonControl dialog

class CPropertyPageCommonControl : public CPropertyPage
{
	DECLARE_DYNAMIC(CPropertyPageCommonControl)

public:
	CPropertyPageCommonControl();
	virtual ~CPropertyPageCommonControl();

// Dialog Data
	enum { IDD = IDD_DLG_COMMON_CONTROL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	BOOL m_bEnableAll;

	BOOL m_bEnableComboBox;
	CString m_strResultComboBox;
	CComboBox m_cmbDemoSimple;
	CComboBox m_cmbDemoDropdown;
	CComboBox m_cmbDemoDropList;

	BOOL m_bEnableProgress;
	int m_nResultProgress;
	CScrollBar m_sbDemoHorz;
	CScrollBar m_sbDemoVert;
	int m_nSliderDemoHorz;
	int m_nSliderDemoVert;
	CProgressCtrl m_prgDemoHorz;
	CProgressCtrl m_prgDemoVert;
	UINT_PTR   m_nProgressTimerID;
	BOOL       m_bPauseProgress;

	virtual BOOL OnInitDialog();

protected:
	void    EnableComboBox(BOOL bEnable);
	void    EnableProgress(BOOL bEnable);
	void    EnableAllStatusUpdate();
	void    ProgressPosUpdate();
public:
	afx_msg void OnBnClickedChkEnableCombobox();
	afx_msg void OnBnClickedChkEnableAll();
	afx_msg void OnBnClickedBtnClearCombobox();
	afx_msg void OnCbnSelchangeCmdDemoDroplist();
	afx_msg void OnCbnSelchangeCmdDemoDropdown();
	afx_msg void OnCbnEditchangeCmdDemoDropdown();
	afx_msg void OnCbnSelchangeCmbDemoSimple();
	afx_msg void OnCbnEditchangeCmbDemoSimple();
	
	afx_msg void OnBnClickedChkEnableProgress();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedBtnProgressStart();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedBtnProgressPause();
	afx_msg void OnDeltaposSpinDemoHorz(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinDemoVert(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtnSetProgress();
};

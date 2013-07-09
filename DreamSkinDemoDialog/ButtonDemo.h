#pragma once


// CButtonDemo

class CButtonDemo : public CButton
{
	DECLARE_DYNAMIC(CButtonDemo)

public:
	CButtonDemo();
	virtual ~CButtonDemo();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};



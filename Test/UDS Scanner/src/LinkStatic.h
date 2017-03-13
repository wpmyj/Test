#pragma once
#include "afxwin.h"
class CLinkStatic : public CStatic
{
public:
	CLinkStatic();
	~CLinkStatic();

	DECLARE_MESSAGE_MAP()
	//afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();

protected:
	//LPCTSTR url;
	CString m_strCaption;

public:
	void SetCaption(LPCTSTR _strCaption);
};


#pragma once

#include "opencv.hpp"
// CPage_About 对话框

class CPage_About : public CPropertyPage
{
	DECLARE_DYNAMIC(CPage_About)

public:
	CPage_About();
	virtual ~CPage_About();

// 对话框数据
	enum { IDD = IDD_PROPPAGE_ABOUT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnAbout_Static_Web();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);

//	virtual BOOL OnInitDialog();

public :
	//用于保存网址的屏幕坐标  
	CRect m_pRectLink;
};

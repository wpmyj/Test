#pragma once

#include "MFC_UI.h"
#include "opencv.hpp"
// CPage_About 对话框

class CPage_About : public CPropertyPage
{
	DECLARE_DYNAMIC(CPage_About)

public:
	CPage_About(MFC_UI *pUI);
	virtual ~CPage_About();

// 对话框数据
	enum { IDD = IDD_PROPPAGE_ABOUT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnSetActive();

	DECLARE_MESSAGE_MAP()

private:
	/**
	* @brief 在构造函数中初始化 
	* @see  CPage_Base(MFC_UI *pUI)
	*/
	MFC_UI  *m_pUI; 

public:
	afx_msg void OnAbout_Static_Web();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);

public :
	//用于保存网址的屏幕坐标  
	CRect m_pRectLink;
	
};

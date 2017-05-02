#pragma once

#include "Page_Custom.h"
#include "MFC_UI.h"
#include "ximage.h" 

class MFC_UI;
// CBase_Tab_Rotateshow 对话框

class CBase_Tab_Rotateshow : public CDialogEx, public CPage_Custom
{
	DECLARE_DYNAMIC(CBase_Tab_Rotateshow)

public:
	CBase_Tab_Rotateshow(MFC_UI *pUI);   // 标准构造函数
	virtual ~CBase_Tab_Rotateshow();

// 对话框数据
	enum { IDD = IDD_BASETAB_ROTATESHOW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

private:
	/**
	* @brief 在构造函数中初始化 
	* @see  CPage_Base(MFC_UI *pUI)
	*/
	MFC_UI  *m_pUI; 

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();

	CStatic m_picture;

public:
	CxImage m_image;
	void DrawImgOnCtrl(CxImage pImg, CWnd* wndDraw); //在picture control控件中根据picture control控件的大小动态显示图片

};

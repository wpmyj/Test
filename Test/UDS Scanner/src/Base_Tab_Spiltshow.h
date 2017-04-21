#pragma once

#include "Page_Custom.h"
#include "MFC_UI.h"

class MFC_UI;

// CBase_Tab_Spiltshow 对话框

class CBase_Tab_Spiltshow : public CDialogEx, public CPage_Custom
{
	DECLARE_DYNAMIC(CBase_Tab_Spiltshow)

public:
	CBase_Tab_Spiltshow(MFC_UI *pUI);   // 标准构造函数
	virtual ~CBase_Tab_Spiltshow();

// 对话框数据
	enum { IDD = IDD_BASETAB_SPILTSHOW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

private:
	/**
	* @brief 在构造函数中初始化 
	* @see  CPage_Base(MFC_UI *pUI)
	*/
	MFC_UI  *m_pUI; 
};

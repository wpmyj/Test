#pragma once
#include "MFC_UI.h"

class MFC_UI;

// CPage_Muiltstream 对话框

class CPage_Muiltstream : public CPropertyPage
{
	DECLARE_DYNAMIC(CPage_Muiltstream)

public:
	CPage_Muiltstream(MFC_UI *pUI);
	virtual ~CPage_Muiltstream();

// 对话框数据
	enum { IDD = IDD_PROPPAGE_MUILSTREAM };

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

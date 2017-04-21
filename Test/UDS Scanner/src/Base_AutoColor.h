#pragma once
#include "afxwin.h"

#include "Page_Custom.h"
#include "MFC_UI.h"

class MFC_UI;
// CBase_AutoColor 对话框

class CBase_AutoColor : public CPropertyPage, public CPage_Custom
{
	DECLARE_DYNAMIC(CBase_AutoColor)

public:
	CBase_AutoColor(MFC_UI *pUI);
	virtual ~CBase_AutoColor();

// 对话框数据
	enum { IDD = IDD_BASETAB_AUTOCOLOR };

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
	void UpdateControls(void);  ///< 更新控件状态
	void InitSliderCtrl();  ///< 初始化滑动条控件

public:
	CComboBox m_combo_resolution;
	
	CComboBox m_combo_compress;	
	CSliderCtrl m_slider_compressvalue;	
	CEdit m_edit_compressvalue;

	CComboBox m_combo_nocolor;
	CEdit m_edit_colorthres;
	CSliderCtrl m_slider_colorthres;

	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeTabautocolor_Combo_Compress();
	afx_msg void OnNMCustomdrawTabautocolor_Slider_Compressionvalue(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeTabautocolor_Edit_Compressvalue();
	afx_msg void OnCbnSelchangeTabautocolor_Combo_Resolution();
	
	afx_msg void OnCbnSelchangeTabautocolor_Combo_Nocolor();
	afx_msg void OnEnChangeTabautocolor_Edit_Colorthres();
	afx_msg void OnNMCustomdrawTabautocolor_Slider_Colorthres(NMHDR *pNMHDR, LRESULT *pResult);
};

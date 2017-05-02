#pragma once

#include "Page_Custom.h"
#include "MFC_UI.h"

class MFC_UI;
// CBase_Tab_Color 对话框

class CBase_Tab_Color : public CDialogEx, public CPage_Custom
{
	DECLARE_DYNAMIC(CBase_Tab_Color)

public:
	CBase_Tab_Color(MFC_UI *pUI); 
	virtual ~CBase_Tab_Color();

// 对话框数据
	enum { IDD = IDD_BASETAB_COLOR };

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
	afx_msg void OnNMCustomdrawTabcolor_Slider_Compressionvalue(NMHDR *pNMHDR, LRESULT *pResult);	
	afx_msg void OnEnChangeTabcolor_Edit_Compressvalue();	
	afx_msg void OnNMCustomdrawTabcolor_Slider_Contrast(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawTabcolor_Slider_Brightness(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeTabcolor_Edit_Contrast();
	afx_msg void OnEnChangeTabcolor_Edit_Brightness();
	afx_msg void OnCbnSelchangeTabcolor_Combo_Resolution();
//	afx_msg void OnTabColor_RadioBtn_Compress();
	afx_msg void OnCbnSelchangeTabcolor_Combo_Compressquality();

	CComboBox m_combo_resolution;
	CSliderCtrl m_slider_compressvalue;
	CEdit m_edit_compressvalue;
	CSliderCtrl m_slider_contrast;
	CEdit m_edit_contrast;	
	CSliderCtrl m_slider_brightness;
	CEdit m_edit_brightness;

//	int m_radio_compress;
	CComboBox m_combo_compressquality;

public:
	void UpdateControls(void);  ///< 更新控件状态
	void InitSliderCtrl();  ///< 初始化滑动条控件
	void SetCompressValue();  ///<设置压缩值滑动条是否可用	
	
};

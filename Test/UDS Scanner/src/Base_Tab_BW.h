#pragma once
#include "afxwin.h"

#include "Page_Custom.h"
#include "MFC_UI.h"

class MFC_UI;
// CBase_Tab_BW 对话框

class CBase_Tab_BW : public CPropertyPage, public CPage_Custom
{
	DECLARE_DYNAMIC(CBase_Tab_BW)

public:
	CBase_Tab_BW(MFC_UI *pUI);
	virtual ~CBase_Tab_BW();

// 对话框数据
	enum { IDD = IDD_BASETAB_BW };

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
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	afx_msg void OnCbnSelchangeTabbw_Combo_Resolution();
	
	afx_msg void OnNMCustomdrawTabbw_Slider_Brightness(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeTabbw_Edit_Brightness();
	afx_msg void OnNMCustomdrawTabbw_Slider_Threshold(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeTabbw_Edit_Threshold();
	afx_msg void OnCbnSelchangeTabbw_Combo_Binarization();
	afx_msg void OnEnChangeTabbw_Edit_NoiseNum();
	afx_msg void OnEnChangeTabbw_Edit_NoiseRange();
	afx_msg void OnCbnSelchangeTabBW_Combo_FilterColor();
	afx_msg void OnCbnSelchangeTabBW_Combo_FilterMode();
	afx_msg void OnNMCustomdrawTabBW_Slider_Filterlevel(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeTabBW_Edit_Filterlevel();	
	afx_msg void OnTabBW_RadioBtn_CompressG4();
	afx_msg void OnCbnSelchangeTabBW_Combo_Compressquality();
	afx_msg void OnNMCustomdrawTabbw_Slider_Compressionvalue(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeTabbw_Edit_Compressvalue();
	CComboBox m_combo_resolution;

	CSliderCtrl m_slider_brightness;
	CEdit m_edit_threshold;
	CEdit m_edit_brightness;
	CSliderCtrl m_slider_threshold;
	CComboBox m_combo_binarization;

	CEdit m_edit_noisenum;
	CEdit m_edit_noiserange;
	CScrollBar m_scroll_noisenum;
	CScrollBar m_scroll_noiserange;

	CComboBox m_combo_filtercolor;
	CComboBox m_combo_filtermode;
	CEdit m_edit_filterlevel;
	CSliderCtrl m_slider_filterlevel;

	CComboBox m_combo_compressquality;
	int m_radio_compress;

	CSliderCtrl m_slider_compressvalue;
	CEdit m_edit_compressvalue;

public:
	void UpdateControls(void);  ///< 更新控件状态
	void InitSliderCtrl();  ///< 初始化滑动条控件
	void SetBinarization(void);
	void SetScroll();
	void SetCompressValue();

};

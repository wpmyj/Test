#pragma once

#include "MFC_UI.h"
#include "Page_Custom.h"

class MFC_UI;

// CPage_Filter 对话框

class CPage_Filter : public CPropertyPage, public CPage_Custom
{
	DECLARE_DYNAMIC(CPage_Filter)

public:
	CPage_Filter(MFC_UI *pUI);
	virtual ~CPage_Filter();

// 对话框数据
	enum { IDD = IDD_PROPPAGE_FILTERCOLOR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	
private:
	/**
	* @brief 主界面
	* @see  MFC_UI::DisplayTWAINGUI()
	* @note 从外部接收值
	*/
	MFC_UI  *m_pUI; 

public:
	void SetCapValue();
	void UpdateControls(void);  ///< 更新控件状态
	void SetMultistream(void);
	void SetCapMulti(); //设置多流生效
	void InitFiltermap(void);
	void InitSliderCtrl();

private:
	afx_msg void OnFilter_Btn_Check_Frontcolor();
	afx_msg void OnFilter_Btn_Check_FrontGray();
	afx_msg void OnFilter_Btn_Check_FrontBw();
	afx_msg void OnFilter_Btn_Check_BackGray();
	afx_msg void OnFilter_Btn_Check_BackColor();
	afx_msg void OnFilter_Btn_Check_BackBw();

private:
	MAP_CAP m_filtermap;  ///<用于保存参数改变后的值

	CButton m_check_frontcolor;
	CButton m_check_frontgray;
	CButton m_check_backgray;
	CButton m_check_backcolor;
	CButton m_check_backbw;
	CButton m_check_frontbw;

	CButton m_btn_frontcolor;
	CButton m_btn_frontgray;
	CButton m_btn_frontbw;
	CButton m_btn_backgray;
	CButton m_btn_backcolor;
	CButton m_btn_backbw;

public:
	/** 父类指针*/
	CPage_Custom* m_pFilterPage;
	virtual BOOL OnInitDialog();
	virtual BOOL OnSetActive();
	CComboBox m_combo_filtercolor;
	CComboBox m_combo_filtermode;
	CEdit m_edit_filterlevel;
	CSliderCtrl m_slider_filterlevel;
	afx_msg void OnCbnSelchangeFilter_Combo_FilterColor();
	afx_msg void OnCbnSelchangeFilter_Combo_FilterMode();
	afx_msg void OnNMCustomdrawFilter_Slider_Filterlevel(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeFilter_Edit_Filterlevel();
};

#pragma once
#include "MFC_UI.h"
#include "afxwin.h"
#include "Page_Custom.h"

class MFC_UI;

// CPage_Advanced 对话框

class CPage_Advanced : public CPropertyPage, public CPage_Custom
{
	DECLARE_DYNAMIC(CPage_Advanced)

public:
	CPage_Advanced(MFC_UI *pUI);
	virtual ~CPage_Advanced();

// 对话框数据
	enum { IDD = IDD_PROPPAGE_ADVANCED };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnSetActive();

	DECLARE_MESSAGE_MAP()

private:
	/**
	* @brief 主界面
	* @see  MFC_UI::DisplayTWAINGUI()
	* @note 从外部接收值
	*/
	MFC_UI  *m_pUI; 

private:  
	CButton m_check_removepunch;
	CButton m_check_sharpen;
	CButton m_check_mirror;
	CButton m_check_removeback;
	CButton m_check_removedescreen;
	CButton m_check_removedenoise;
	CButton m_check_colorflip;

	CButton m_check_multifeeddetect;
	CButton m_check_mdvalue;

	MAP_CAP m_advancedmap;  ///<用于保存参数改变后的值

	TW_FRAME frame;

	//2.0版本
	CComboBox m_combo_standardsizes;
	CComboBox m_combo_uints;
	CScrollBar m_scroll_height;
	CScrollBar m_scroll_down;
	CScrollBar m_scroll_left;
	CScrollBar m_scroll_right;
	CScrollBar m_scroll_up;
	CScrollBar m_scroll_width;
	CEdit m_edit_width;
	CEdit m_edit_height;
	CEdit m_edit_left;
	CEdit m_edit_down;
	CEdit m_edit_right;
	CEdit m_edit_up;
	CEdit m_edit_xpos;
	CEdit m_edit_ypos;
	CScrollBar m_scroll_xpos;
	CScrollBar m_scroll_ypos;
	CComboBox m_combo_cutmethod;

	int m_resolution; ///<当前分辨率
	
	//英寸单位下，六者的最大取值，使用时需要根据单位转换
	int maxinches_xpos;
	int maxinches_ypos;
	int maxinches_updown;
	int maxinches_leftright;
	int maxinches_width;
	int maxinches_height;

	int m_radio_edgecolor;

public:
	void UpdateControls(void);  ///< 更新控件状态
	void SetCapValue(void); ///<设置参数

	void SetPaperSize(void);
	void SetXYPos(void); ///<设置XY偏移量是否可用。
	void SetScroll(); ///<根据edit的值更新滚动条的值;以及根据单位设定宽、高滚动条的范围;

private:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	afx_msg void OnAdvanced_Btn_Check_RemovePunch();
	afx_msg void OnAdvanced_Btn_Check_Sharpen();
	afx_msg void OnAdvanced_Btn_Check_Mirror();
	afx_msg void OnAdvanced_Btn_Check_RemoveBack();
	afx_msg void OnAdvanced_Btn_Check_RemoveDenoise();
	afx_msg void OnAdvanced_Btn_Check_RemoveDescreen();
	afx_msg void OnClicked_Check_Multifeeddetect();
	afx_msg void OnAdvanced_Btn_Check_Colorflip();
	afx_msg void OnClicked_Check_MdValue();
	afx_msg void OnCbnSelchangeAdvanced_Combo_Standardsizes();
	afx_msg void OnCbnSelchangeAdvanced_Combo_Cutmethod();

	//2.0版本
	afx_msg void OnCbnSelchangeAdvanced_Combo_Uints();	
	afx_msg void OnEnChangeAdvanced_Edit_EdgeXpos();
	afx_msg void OnEnChangeAdvanced_Edit_EdgeYpos();
	afx_msg void OnEnChangeAdvanced_Edit_EdgeUp();
	afx_msg void OnEnChangeAdvanced_Edit_EdgeLeft();
	afx_msg void OnEnChangeAdvanced_Edit_EdgeRight();
	afx_msg void OnEnChangeAdvanced_Edit_EdgeDown();

	afx_msg void OnAdvanced_RadioBtn_Edgecolor();

public:
	/** Base界面的父类指针*/
	CPage_Custom* m_pBasePage;
	
	
};

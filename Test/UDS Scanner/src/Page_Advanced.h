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
	MAP_CAP m_advancedmap;  ///<用于保存参数改变后的值
	TW_FRAME frame;

	CButton m_check_multifeeddetect;
	
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
	int m_radio_edgeorientation;
	int m_radio_cornercolor;
	CComboBox m_combo_edgecolor;
	CComboBox m_combo_mdvalue;
	CButton m_check_overlength;
	CButton m_check_codeidentity;
	CButton m_check_prefeed;
	CButton m_check_separatepage;
	CButton m_check_removepunch;
	CComboBox m_combo_codestandard;
	CSliderCtrl m_slider_waittime;
	CEdit m_edit_waittime;

	int m_resolution; ///<当前分辨率
	
	//英寸单位下，六者的最大取值，使用时需要根据单位转换
	int maxinches_xpos;
	int maxinches_ypos;
	int maxinches_updown;
	int maxinches_leftright;
	int maxinches_width;
	int maxinches_height;
	int maxinches_overlength;
	int maxinches_prefeed;

	CScrollBar m_scroll_overlength;
	CScrollBar m_scroll_prefeed;
	CEdit m_edit_overlength;
	CEdit m_edit_prefeed;

public:
	void UpdateControls(void);  ///< 更新控件状态
	void SetCapValue(void); ///<设置参数

	void SetPaperSize(void);
	void SetXYPos(void); ///<设置XY偏移量是否可用。
	void SetScroll(); ///<根据edit的值更新滚动条的值;以及根据单位设定宽、高滚动条的范围;
	void SetPrefeed(); ///<设置预先进纸编辑框是否可用
	void SetOverlength(); ///<设置超出预定长度是否可用

private:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	afx_msg void OnAdvanced_Btn_Check_RemovePunch();
	afx_msg void OnClicked_Check_Multifeeddetect();
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

	afx_msg void OnAdvanced_RadioBtn_Edgeorientation();
	afx_msg void OnAdvanced_RadioBtn_Cornercolor();
	afx_msg void OnCbnSelchangeAdvanced_Combo_Edgecolor();
	afx_msg void OnCbnSelchangeAdvanced_Combo_Findoverlay();
	afx_msg void OnAdvanced_Btn_Check_Overlength();
	afx_msg void OnAdvanced_Btn_Check_Prefeed();
	afx_msg void OnAdvanced_Btn_Check_Separatepage();
	afx_msg void OnAdvanced_Btn_Check_Codeidentity();
	afx_msg void OnCbnSelchangeAdvanced_ComboCodestand();
	afx_msg void OnNMCustomdrawBase_Slider_Waittime(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeBase_Edit_Waittime();
	afx_msg void OnEnChangeAdvanced_Edit_Overlength();	
	afx_msg void OnEnChangeAdvanced_Edit_Prefeed();

public:
	/** Base界面的父类指针*/
	CPage_Custom* m_pBasePage;
	
};

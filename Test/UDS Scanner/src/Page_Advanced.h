#pragma once
#include "MFC_UI.h"
#include "afxwin.h"
class MFC_UI;

// CPage_Advanced 对话框

class CPage_Advanced : public CPropertyPage
{
	DECLARE_DYNAMIC(CPage_Advanced)

public:
	//CPage_Advanced();
	CPage_Advanced(MFC_UI *pUI);
	virtual ~CPage_Advanced();

// 对话框数据
	enum { IDD = IDD_PROPPAGE_ADVANCED };

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
private:

	    
	/**
	* @brief 纸张大小: US Letter/US Legal/A4/A5  
	* @see CTWAINDS_FreeIMage.cpp Line 606
	*/
	CComboBox m_combo_standardsizes;  

	/**
	* @brief 纸张方向: 纵向/横向  
	* @see CTWAINDS_FreeIMage.cpp Line 615
	*/
	CComboBox m_combo_orientation; ///<纸张方向

	CComboBox m_combo_uints;  ///<　单位

	CComboBox m_combo_rotate;  ///<图像设置-图像旋转
	CComboBox m_combo_splitimage; ///<图像设置-图像分割

	CComboBox m_combo_resolution; ///< 多流输出-分辨率
	CComboBox m_combo_binarization;  ///<多流输出-二值化

	CEdit m_edit_custom_width;  ///< 纸张大小，自定义宽度
	CEdit m_edit_custom_height;  ///< 纸张大小，自定义高度

	CEdit m_edit_brightness; ///< 多流输出,亮度
	CEdit m_edit_contrast; ///< 多流输出，对比度
	CSliderCtrl m_slider_brightness;
	CSliderCtrl m_slider_contrast;

	CButton m_check_multistream;
	CButton m_check_backbw;
	CButton m_check_backcolor;
	CButton m_check_backgray;
	CButton m_check_frontbw;
	CButton m_check_frontcolor;
	CButton m_check_frontgray;

	typedef map<int, int> MAP_CAP;
	MAP_CAP m_advancedmap;  ///<用于保存参数改变后的值

private:
	void UpdateControls(void);  ///< 更新控件状态
	void InitSliderCtrl();  ///< 初始化滑动条控件
	//void SetControl(void); ///<设置参数
	void SetMultistream(void); ///<设置多流输出配套参数
	void SetColorGrayImage(void); ///<设置彩色和灰度，亮度、对比度可用
	void SetBWImage(void); ///<设置黑白图片时二值化可用

private:
	virtual BOOL OnInitDialog();
	//virtual void OnOK();

	afx_msg void OnCbnSelchangeAdvanced_Combo_Standardsizes();
	afx_msg void OnCbnSelchangeAdvanced_Combo_Orientation();
	afx_msg void OnCbnSelchangeAdvanced_Combo_Uints();
	
	afx_msg void OnCbnSelchangeAdvanced_Combo_Resolution();
	afx_msg void OnCbnSelchangeAdvanced_Combo_Rotate();

	afx_msg void OnAdvanced_Btn_Check_Multistream();
	
	afx_msg void OnNMCustomdrawAdvanced_Slider_Brightness(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawAdvanced_Slider_Contrast(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeAdvanced_Edit_Contrast();
	afx_msg void OnEnChangeAdvanced_Edit_Brightness();

	afx_msg void OnAdvanced_Btn_Check_FrontColor();
	afx_msg void OnAdvanced_Btn_Check_FrontGray();
	afx_msg void OnAdvanced_Btn_Check_FrontBW();
	afx_msg void OnAdvanced_Btn_Check_BackGray();
	afx_msg void OnAdvanced_Btn_Check_BackColor();
	afx_msg void OnAdvanced_Btn_Check_BackBW();
};

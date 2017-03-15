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
	//CPage_Advanced();
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

	CComboBox m_combo_rotate;  ///<图像设置-图像旋转
	CComboBox m_combo_splitimage; ///<图像设置-图像分割
	int m_radiobtn_spilt_vert;

	CEdit m_edit_gamma; ///< Gamma校正
	CEdit m_edit_removeblank; ///<去除空白页
	CEdit m_edit_spilt;

	CComboBox m_combo_cachemode; ///<缓存模式
//	CEdit m_edit_cachemode;
	CSliderCtrl m_slider_cachemode;

	CSliderCtrl m_slider_gamma;
	CSliderCtrl m_slider_removeblank;		
	
	CButton m_check_removeblank;
	CButton m_check_removepunch;
	CButton m_check_sharpen;
	CButton m_check_mirror;
	CButton m_check_removeback;
	CButton m_check_removedescreen;
	CButton m_check_removedenoise;
	CButton m_check_autocrop;
	CButton m_check_colorflip;

	CButton m_check_multifeeddetect;
	CButton m_check_mdvalue;

	MAP_CAP m_advancedmap;  ///<用于保存参数改变后的值

	TW_FRAME frame;
public:
	void UpdateControls(void);  ///< 更新控件状态
	void InitSliderCtrl();  ///< 初始化滑动条控件
	void SetCapValue(void); ///<设置参数
//void SetStandardsizes(void); ///<设置纸张大小选择“自定义”时，宽、高的连动
	void InitAdvancedmap(void); ///<初始化Map值，主要是为分割Map插入默认值
	void SetBlank(void); ///<判断并设置去除空白页checkBox的初始状态
	void SetSpiltimage(void); ///<设置拆分图像为自定义时，旁边的edit可用。
	void SetColorMode(int nIndex); ///根据缓存模式的选择，设置对应滑动条的范围以及static的值
	
private:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	
	afx_msg void OnCbnSelchangeAdvanced_Combo_Rotate();
	afx_msg void OnCbnSelchangeAdvanced_Combo_SpiltImage();
	
	afx_msg void OnNMCustomdrawAdvanced_Slider_Gamma(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawAdvanced_Slider_Removeblank(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeAdvanced_Edit_Gamma();
	afx_msg void OnEnChangeAdvanced_Edit_Removeblank();

	afx_msg void OnAdvanced_Btn_Check_RemoveBlank();
	afx_msg void OnAdvanced_Btn_Check_RemovePunch();
	afx_msg void OnAdvanced_Btn_Check_Sharpen();
	afx_msg void OnAdvanced_Btn_Check_Mirror();
	afx_msg void OnAdvanced_Btn_Check_RemoveBack();
	afx_msg void OnAdvanced_Btn_Check_RemoveDenoise();
	afx_msg void OnAdvanced_Btn_Check_RemoveDescreen();
	afx_msg void OnAdvanced_Btn_Check_AutoCrop();
	afx_msg void OnAdvanced_RadioBtn_Spilt();

	afx_msg void OnClicked_Check_Multifeeddetect();

	afx_msg void OnAdvanced_Btn_Check_Colorflip();

	afx_msg void OnCbnSelchangeAdvanced_Combo_Cachemode();
	afx_msg void OnNMCustomdrawAdvanced_Slider_Cachemode(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnClicked_Check_MdValue();

public:
	/** Base界面的父类指针*/
	CPage_Custom* m_pBasePage;

};

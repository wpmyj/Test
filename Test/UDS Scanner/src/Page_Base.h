#pragma once
#include "MFC_UI.h"
#include "afxwin.h"
#include "Dlg_Profile.h"
#include "Page_Custom.h"

class MFC_UI;
// CPage_Base 对话框

class CPage_Base : public CPropertyPage, public CPage_Custom
{
	DECLARE_DYNAMIC(CPage_Base)

public:
	///< 重载构造函数，添加形参MFC_UI *pUI
	CPage_Base(MFC_UI *pUI);
	virtual ~CPage_Base();

// 对话框数据
	enum { IDD = IDD_PROPPAGE_BASE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	virtual void OnOK();
	virtual void OnCancel();
	//virtual BOOL OnApply();

private:
	/**
	* @brief 在构造函数中初始化 
	* @see  CPage_Base(MFC_UI *pUI)
	*/
	MFC_UI  *m_pUI;  

private:
	void UpdateControls(void);  ///< 更新控件状态
	void InitSliderCtrl();  ///< 初始化滑动条控件
	void InitComboProfile();  ///< 初始化模版Combo
	void LoadProfile();  ///< 加载模版
	void NewBaseProfile(); ///<新建常用模板
	void SetLastProfile(); ///<遍历模板，设置模板中存在“上次使用模板”的情况
	void InitComboPixType(void); ///<初始化图像类型编辑框

	void SetCapValue(void);  ///<设置参数
	void SetDelete(void); ///<设置删除按钮是否可用

	/** 
	//* @brief 浏览并选择单个图片文件
	//* @param[out] strFilePath 图片绝对路径
	//* @return 成功返回true
	//*/
	//bool MyBrowseForSignalImage(PTCHAR strFilePath);

	/**
	* @brief 浏览并选择多个图片文件
	* @return 成功返回vector<string>类型值
	*/
	vector<string> MyBrowseForMultiImages();

	/**
	*  @brief  新建指定参数模板
	*  @param[in]  profilename 模板名
	*  @param[in]  pixeltype 图像类型 
	*  @param[in]  duplexenabled 单/双面 
	*  @param[in]  resolution 分辨率(默认200dpi)  
	*  @retval true 表示成功
	*  @retval false 表示失败  
	*/
	bool CreateNewProfile(std::string profilename, int pixeltype, 
		int duplexenabled, int resolution = 200);

private:
	CComboBox m_combo_source;   ///< 扫描方式:  ADF/Flatbed
	CComboBox m_combo_colormode;  ///< 图像类型:  黑白/灰度/彩色
	CComboBox m_combo_resolution;  ///< 分辨率:  50/100/200/.../600
	CComboBox m_combo_profile;  ///< 模版
	/**
	* @brief 单面/双面扫:  单面/双面  
	* @see CTWAINDS_FreeIMage.cpp Line 675
	*/
	CComboBox m_combo_scanside;

	CSliderCtrl m_slider_contrast;  ///< 对比度:  -100~+100
	CSliderCtrl m_slider_brightness;  ///< 亮度:  -100~+100
	CSliderCtrl m_slider_threshold;

	CEdit m_edit_contrast;  ///< 用于同步显示m_slider_contrast值
	CEdit m_edit_brightness;   ///< 用于同步显示m_slider_brightness值
	CEdit m_edit_threshold;

	CButton m_check_multifeeddetect; ///<"重张检测"
	CButton m_btn_chooseimage; ///<"选择图片按钮"

	MAP_CAP m_basemap;  ///<用于保存参数改变后的值

	//vector<string> m_vector_string_imagepath;  /**< 图片路劲 */

	//CDlg_Profile *m_pDlg;

	virtual BOOL OnInitDialog();
	
	afx_msg void OnNMCustomdrawBase_Slider_Contrast(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawBase_Slider_Brightness(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawBase_Slider_Threshold(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void OnEnChangeBase_Edit_Contrast();
	afx_msg void OnEnChangeBase_Edit_Brightness();
	afx_msg void OnEnChangeBase_Edit_Threshold();

	afx_msg void OnCbnSelchangeBase_Combo_Source();
	afx_msg void OnCbnSelchangeBase_Combo_Colormode();
	afx_msg void OnCbnSelchangeBase_Combo_Resolution();
	afx_msg void OnCbnSelchangeBase_Combo_Profile();
	afx_msg void OnCbnSelchangeBase_Combo_Scanside();

	afx_msg void OnBase_Btn_Newprofile();
	afx_msg void OnBase_Btn_Deleteprofile();

	afx_msg void OnClicked_Check_Multifeeddetect();

  /** 选择待扫图片 */
	afx_msg void OnBase_Btn_Chooseimage();
	/** 另存为模板 */
	afx_msg void OnBase_Btn_SaveAsprofile();
	/** 保存当前模板*/
//	afx_msg void OnBase_Btn_Saveprofile();

public:
	/** 父类指针*/
	CPage_Custom* m_pAdPage;

};

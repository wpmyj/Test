#pragma once

#include "MFC_UI.h"
#include "afxwin.h"
#include "Dlg_Profile.h"
#include "Page_Custom.h"
#include "Base_AutoColor.h"
#include "Base_Tab_Color.h"
#include "Base_Tab_Gray.h"
#include "Base_Tab_BW.h"

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
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	virtual BOOL OnSetActive();

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
	void InitComboPixType(void); ///<初始化图像类型编辑框
	void SetCapValue(void);  ///<设置参数
	void InitBasemap(void); ///<初始化Basemap，主要往里面增加CAP_DUPLEXENABLED的初始化
	void SetFlat(void); ///< 设置扫描模式为平板时，只能为单面。

	void SetMultistream(void); ///<设置多流输出配套参数


	//bool MyBrowseForSignalImage(PTCHAR strFilePath);
	/**
	* @brief 浏览并选择多个图片文件
	* @return 成功返回vector<string>类型值
	*/
	vector<string> MyBrowseForMultiImages();

	//2.0版本
	void SetSpiltimage();
	void SetBlank(void);
	void SetScroll();
	void SetDenoise(); //设置去噪声
	void SetCapMulti(); //设置多流生效
	void SetTabCtrl(); //设置图像类型选择时，Tab选中哪一页

private:
	CComboBox m_combo_colormode;  ///< 图像类型:  黑白/灰度/彩色

	int m_radiobtn_scanmode; ///< 扫描方式:  ADF自动进纸器/Flatbed平板 
	int m_radiobtn_duplex; ///<单双面：单面、双面、多流

	CButton m_check_frontcolor;
	CButton m_check_frontgray;
	CButton m_check_frontbw;
	CButton m_check_backbw;
	CButton m_check_backgray;
	CButton m_check_backcolor;

	CButton m_btn_frontcolor;
	CButton m_btn_frontgray;
	CButton m_btn_frontbw;
	CButton m_btn_backcolor;
	CButton m_btn_backgray;
	CButton m_btn_backbw;

	CComboBox m_combo_rotate;
	CComboBox m_combo_splitimage;
	CSliderCtrl m_slider_gamma;
	CEdit m_edit_gamma;
	CSliderCtrl m_slider_removeblank;
	CEdit m_edit_removeblank;
	CButton m_check_removeblank;

	MAP_CAP m_basemap;  ///<用于保存参数改变后的值

	TCHAR m_bmpFilePath[PATH_MAX]; ///<预览图保存路径

	CTabCtrl m_base_tab;
	CRect m_tabRect; // 标签Tab控件客户区的Rect

	CScrollBar m_scroll_noisenum;
	CScrollBar m_scroll_noiserange;
	CEdit m_edit_noisenum;
	CEdit m_edit_noiserange;

	afx_msg void OnCbnSelchangeBase_Combo_Colormode();

	afx_msg void OnBase_Btn_Newprofile();

	afx_msg void OnBase_RadioBtn_Scanmode_Auto();
	afx_msg void OnBase_RadioBtn_Scanmode_Flatbed();

	afx_msg void OnBase_RadioBtn_Duplex();

	afx_msg void OnBase_Btn_Check_FrontColor();
	afx_msg void OnBase_Btn_Check_FrontGray();
	afx_msg void OnBase_Btn_Check_FrontBw();
	afx_msg void OnBase_Btn_Check_BackColor();
	afx_msg void OnBase_Btn_Check_BackGray();
	afx_msg void OnBase_Btn_Check_BackBw();
//	afx_msg void OnPaint();

	afx_msg void OnTcnSelchangeBase_Tab(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnCbnSelchangeBase_Combo_Rotate();	
	afx_msg void OnCbnSelchangeBase_Combo_SpiltImage();	
	afx_msg void OnNMCustomdrawBase_Slider_Gamma(NMHDR *pNMHDR, LRESULT *pResult);	
	afx_msg void OnEnChangeBase_Edit_Gamma();	
	afx_msg void OnNMCustomdrawBase_Slider_Removeblank(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeBase_Edit_Removeblank();	
	afx_msg void OnBase_Btn_Check_RemoveBlank();

	afx_msg void OnBase_Btn_FrontColor();
	afx_msg void OnBase_Btn_Backcolor();
	afx_msg void OnBase_Btn_Frontgray();
	afx_msg void OnBase_Btn_Backgray();
	afx_msg void OnBase_Btn_Frontbw();
	afx_msg void OnBase_Btn_Backbw();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	afx_msg void OnEnChangeBase_Edit_NoiseNum();
	afx_msg void OnEnChangeBase_Edit_NoiseRange();
	
public:
	/** 父类指针*/
	CPage_Custom* m_pAdPage;	
	CPage_Custom* m_pPaperPage;	
	
	//Tab对话框类
	CBase_AutoColor *m_pTabAutoColor;
	CBase_Tab_Color *m_pTabColor;
	CBase_Tab_Gray *m_pTabGray;
	CBase_Tab_BW *m_pTabBW;
	
};

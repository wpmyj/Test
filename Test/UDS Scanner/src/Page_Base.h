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
	void SetColorGrayImage(void); ///<设置彩色和灰度，亮度、对比度可用
	void SetBWImage(void); ///<设置黑白图片时二值化可用
	int FindPaperSize(int index); ///<寻找index对应的纸张大小,返回index对应的纸张大小

	void PreView();  //预览按钮实际功能实现

	void SetBinarization(void); ///<设置二值化分别选择不同值时，该显示“去除斑点”还是“底色保留”

	//bool MyBrowseForSignalImage(PTCHAR strFilePath);
	/**
	* @brief 浏览并选择多个图片文件
	* @return 成功返回vector<string>类型值
	*/
	vector<string> MyBrowseForMultiImages();

	//DrawToHdc系列函数
	void DrawToHDC(HDC hDCDst, RECT* pDstRect, IplImage* img); //在指定dDCDst根据pDstRect绘图。
	void FillBitmapInfo(BITMAPINFO* bmi, int width, int height, int bpp, int origin); ///<为bmi写入信息头和调色板。
	void Show(IplImage* img, HDC dc, int x, int y, int w, int h, int from_x, int from_y);	 ///<使用DIB位图和颜色数据对与目标设备环境相关的设备在dc上的指定矩形中的像素进行设置
	CvRect RectToCvRect(RECT sr); ///<将Rect类型转为CvRect类型
	RECT NormalizeRect(RECT r); ///<标准化处理输入rect，防止左侧坐标大于右侧坐标，上侧坐标大于下侧坐标

private:
	CComboBox m_combo_colormode;  ///< 图像类型:  黑白/灰度/彩色
	CComboBox m_combo_resolution;  ///< 分辨率:  50/100/200/.../600
	CComboBox m_combo_binarization; ///<二值化：固定阈值。动态阈值、半色调

	CSliderCtrl m_slider_contrast;  ///< 对比度:  -100~+100
	CSliderCtrl m_slider_brightness;  ///< 亮度:  -100~+100
	CSliderCtrl m_slider_threshold; ///<阈值/去除斑点 

	CEdit m_edit_contrast;  ///< 用于同步显示m_slider_contrast值
	CEdit m_edit_brightness;   ///< 用于同步显示m_slider_brightness值
	CEdit m_edit_threshold; ///< 用于同步显示m_slider_threshold值

//	CButton m_btn_chooseimage;
	///<"选择图片按钮"

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

	CStatic m_base_picture;

	MAP_CAP m_basemap;  ///<用于保存参数改变后的值
	
	afx_msg void OnNMCustomdrawBase_Slider_Contrast(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawBase_Slider_Brightness(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawBase_Slider_Threshold(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void OnEnChangeBase_Edit_Contrast();
	afx_msg void OnEnChangeBase_Edit_Brightness();
	afx_msg void OnEnChangeBase_Edit_Threshold();

	afx_msg void OnCbnSelchangeBase_Combo_Colormode();
	afx_msg void OnCbnSelchangeBase_Combo_Resolution();
	afx_msg void OnCbnSelchangeBase_Combo_Binarization();

	afx_msg void OnBase_Btn_Newprofile();

  /** 选择待扫图片 */
//	afx_msg void OnBase_Btn_Chooseimage();

	afx_msg void OnBase_RadioBtn_Scanmode();
	afx_msg void OnBase_RadioBtn_Duplex();

	afx_msg void OnBase_Btn_Check_FrontColor();
	afx_msg void OnBase_Btn_Check_FrontGray();
	afx_msg void OnBase_Btn_Check_FrontBw();
	afx_msg void OnBase_Btn_Check_BackColor();
	afx_msg void OnBase_Btn_Check_BackGray();
	afx_msg void OnBase_Btn_Check_BackBw();

public:
	/** 父类指针*/
	CPage_Custom* m_pAdPage;	
	CPage_Custom* m_pPaperPage;	
};

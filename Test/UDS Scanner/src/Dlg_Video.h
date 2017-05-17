/**
* @file   Dlg_Video.h
* @brief 基于UDS_Video.ocx控件的摄像头界面.
* @author UDS
* @par   Copyright (c) UDS. All Rights Reserved.
* @date  02/24/2017
*/ 
#pragma once

#include "MFC_UI.h"
#include "CDUDS_Video.h"
#include "afxwin.h"
#include "PngButton.h"
//#include "LinkStatic.h"
//#include "CheckBoxEx.h"


#include "gdiplus.h" 
using namespace Gdiplus; 
#pragma comment(lib, "gdiplus.lib")

#include "ximage.h"  // CXImage
#include <vector>
#include "afxcmn.h"
#pragma comment(lib,"cximagel.lib")

#define THUMB_WIDTH  80
#define THUMB_HEIGHT 60

// CDlg_Video 对话框

/** INI文件中的Camera配置 */
struct INI_VIDEO
{
	//int  DpiIndex;            /**< DPI编号 */
	//int  PixelType;           /**< 颜色编号 */
	bool AutoCrop;            /**< 是否自动裁切 */ 
	bool NoStop;
	bool Running;
	//bool ShowInfo;            /**< 是否显示视频信息 */
	//bool playSound;           /**< 是否播放声音 */
	CString CameraName;       /**< 摄像头名称 */
};

class CDlg_Video : public CDialogEx
{
	DECLARE_DYNAMIC(CDlg_Video)

public:
	//CDlg_Video(CWnd* pParent = NULL);   // 标准构造函数
	CDlg_Video(MFC_UI *pUI,CWnd* pParent = NULL);   // 重载构造函数
	virtual ~CDlg_Video();

// 对话框数据
	enum { IDD = IDD_DLG_VIDEO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()


private:
	MFC_UI *m_pUI;

	int m_nRotateType;        ///< 选择方式：0-不旋转 1-右旋 2-180° 3-左旋
	int m_nDevIndex;          ///< 设备编号
	int m_nResIndex;          ///< 分辨率编号
	int m_nFileType;          ///< 保存图片类型：0-jpg 1-bmp 2-png 3-tif
	int m_nPixelType;         ///< 图像类型： 0-彩色 1-灰度 2-黑白
	int m_nFileCount;         ///< 已拍照片数量
	int m_nDPI;               ///< DPI
	//int m_nDPIIndex;          ///< DPI编号
public:
	bool m_bInitialend;       ///< 是否初始化成功
private:
	bool m_bIsDPI;            ///< 是否显示DPI（否则为分辨率）
	bool m_bAutoCrop;         ///< 是否自动裁切
	bool m_bNoStop;           ///< 是否拍照后关闭摄像头
	bool m_bRunning;          
	//bool m_bShowInfo;         ///< 是否显示视频信息
	//bool m_bPlaySound;        ///< 是否播放声音

	CString m_sCaptureName;   ///< 照片名称
	CString m_strTempPath;    ///< 照片零时存储路径
	CString m_sCameraName;    ///< 摄像头名称

public:
	CDUDS_Video m_ocx;
	CComboBox m_CombDev;
	CComboBox m_CombReso;
	CComboBox m_CombColor;
	//CComboBox m_combFiletype;

	CButton m_checkAutoExp;
	CButton m_CheckAC;
	//CButton m_CheckMSG;
	CButton m_checkManual;
	//CButton m_checkPlaySound;


	//CEdit m_editExp;
	//CEdit m_editPath;
	//CEdit m_editURL;

	CSliderCtrl m_sliderExp;
	//CRichEditCtrl m_richeditMSG;
	CPngButton m_button_rotleft;
	CPngButton m_button_rotright;
	CPngButton m_button_close;
	CPngButton m_button_ok;
	CPngButton m_button_capture;
	CPngButton m_button_selfcap;
	CPngButton m_button_DPI;

	CImage m_image_background;
	CStatic m_sCaptureCount;  ///< 显示已拍照照片数
	CImageList	m_imagelist;  ///< 缩略图对应ImageList控件
	CListCtrl   m_listctrl;   ///< 缩略图对应ListCtrl控件

	//CToolTipCtrl m_ToolTipCtrl;
	//int b;
	//std::vector<std::string> m_vector_thumbnail;
protected:
	/**
	*  @brief  初始化控件 
	*  @retval TRUE 表示成功
	*  @retval FALSE 表示失败  
	*/
	BOOL OcxInit();

	/**
	*  @brief  设置Cap的值
	*/
	void SetCapValue(void);

	/**
	*  @brief  切换到DPI或者分辨率，主要用于combo
	*  @param[in]  bisDPI 标记 
	*/
	void SwitchToDPIorRes(bool bisDPI);

	/**
	*  @brief  从INI文件中读取相机配置
	*/
	void ReadSetting();

	/**
	*  @brief  将相机配置写入INI文件
	*/
	void WriteSetting();

	/**
	*  @brief  播放声音
	*/
	void MyPlaySound(TCHAR* _szFileName);


	//load png from resource
	bool LoadImageFromResource(IN CImage* pImage, IN UINT nResID, IN LPCTSTR lpTyp);

	/**
	*  @brief  给控件添加png格式的背景图片
	*/
	void AddPngToControl();


	//HWND CreateToolTip(int toolID, HWND hDlg, PTSTR pszText);

	/**
	*  @brief  创建矩形区域ToolTip 
	*  @param[in]  hwndParent  父窗口句柄
	*  @param[out] pszText 提示内容  
	*/
	void CreateToolTipForRect(HWND hwndParent, PTSTR pszText);

	/**
	*  @brief  清空并删除文件夹
	*  @param[in]  szPath 待清空的文件夹路径
	*  @param[in]  deleteDir 是否删除文件夹
	*  @retval true 表示成功
	*  @retval false 表示失败  
	*/
	bool ClearAndDeleteDir(const TCHAR* szPath, bool deleteDir = false);

	/**
	*  @brief  更新CStatic控件文本内容并刷新
	*  @param[in]  nID 控件ID
	*  @param[in] strText 文本内容 
	*/
	void RefreshCStaticControl(int nID, LPCTSTR strText);

	/**
	*  @brief  保存缩略图，用于List控件显示。
	*  @param[in]  strName 图像名称 
	*/
	void SaveThumbNail(LPCTSTR strName);

	/**
	*  @brief  加载缩略图
	*/
	void LoadThumbNail();


	/**
	*  @brief  图像处理
	*/
	enum enum_image_handle  
	{
		IH_LEFT90,          /**< 左转90度 */ 
		IH_RIGHT90,         /**< 右转90度 */
		IH_FLIP,            /**< 水平翻转 */
		IH_MIRROR           /**< 垂直镜像 */
	};

	/**
	*  @brief  图像处理
	*  @param[in]  eMethod 处理方法
	*/
	void ImageHandle(enum_image_handle eMethod);	

	bool CheckFilePathExist(LPCSTR pPath);
public:
	void CaptureOne();

public:
	virtual BOOL OnInitDialog();

	afx_msg void OnClose();
	afx_msg void OnCbnSelchangeCombo_Dev();
	afx_msg void OnCbnSelchangeCombo_Reso();
	afx_msg void OnCbnSelchangeCombo_Color();
	//afx_msg void OnCbnSelchangeCombo_Filetype();
	afx_msg void OnBnClickedCheck_Autoexp();
	afx_msg void OnBnClickedCheck_Autocrop();
	afx_msg void OnBnClickedCheck_Manual();
	//afx_msg void OnBnClickedCheck_Showinfo();
	//afx_msg void OnBnClickedButton_Chose();
	//afx_msg void OnBnClickedButton_Upload();
	afx_msg void OnBnClickedButton_Capture();
	//afx_msg void OnClickedButton_Base64cap();
	afx_msg void OnClickedButton_Barcodecap();
	//afx_msg void OnClickedButton_Pdfcap();
	afx_msg void OnClickedButton_Sercap();
	afx_msg void OnBnClickedButton_Rotleft();
	afx_msg void OnBnClickedButton_Rotright();
	afx_msg void OnNMCustomdrawSlider_Exp(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnOk();
	afx_msg void OnBnClickedButton_Dpi();
	//afx_msg void OnBnClickedCheck_Playsound();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnVideo_Close();
	afx_msg void OnPaint();

	DECLARE_EVENTSINK_MAP()
	void GetBarcodeString_VideoUdsVideoctrl(LPCTSTR Barcode);
	void GetAutoCapFileName_VideoUdsVideoctrl(LPCTSTR fileName);
	void DeviceChanged_VideoUdsVideoctrl(LPCTSTR changeType, LPCTSTR deviceName);

	afx_msg void OnImageDelete();
	afx_msg void OnImageLeft90();
	afx_msg void OnImageRight90();
	afx_msg void OnImageFlipvertical();
	afx_msg void OnImageMirror();
	afx_msg void OnNMRClickVideoListThumbnail(NMHDR *pNMHDR, LRESULT *pResult);
};

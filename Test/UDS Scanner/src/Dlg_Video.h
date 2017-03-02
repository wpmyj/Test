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
//#include "e:\git_respositories\test\test(ocx)\uds scanner\cduds_video.h"
// CDlg_Video 对话框

/** INI文件中的Camera配置 */
struct INI_VIDEO
{
	//int  DpiIndex;            /**< DPI编号 */
	//int  PixelType;           /**< 颜色编号 */
	bool AutoCrop;            /**< 是否自动裁切 */ 
	bool ShowInfo;            /**< 是否显示视频信息 */
	bool playSound;           /**< 是否播放声音 */
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
	bool m_bInitialend;       ///< 是否初始化成功
	bool m_bIsDPI;            ///< 是否显示DPI（否则为分辨率）
	bool m_bAutoCrop;         ///< 是否自动裁切
	bool m_bShowInfo;         ///< 是否显示视频信息
	bool m_bPlaySound;        ///< 是否播放声音

	CString m_sCaptureName;   ///< 照片名称
	CStatic m_sCaptureCount;  ///< 显示已拍照照片数
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
	CButton m_CheckMSG;
	CButton m_checkManual;
	CButton m_checkPlaySound;
	CButton m_btnDPI;

	CEdit m_editExp;
	//CEdit m_editPath;
	//CEdit m_editURL;

	CSliderCtrl m_sliderExp;
	CRichEditCtrl m_richeditMSG;
	int b;
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
	afx_msg void OnBnClickedCheck_Showinfo();
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
	afx_msg void OnBnClickedCheck_Playsound();

	DECLARE_EVENTSINK_MAP()
	void GetBarcodeString_VideoUdsVideoctrl(LPCTSTR Barcode);
	void GetAutoCapFileName_VideoUdsVideoctrl(LPCTSTR fileName);
	void DeviceChanged_VideoUdsVideoctrl(LPCTSTR changeType, LPCTSTR deviceName);

};

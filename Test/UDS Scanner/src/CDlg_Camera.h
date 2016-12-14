#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "UDSCapture.h"
#include "MFC_UI.h"
/**
* @file   CDlg_Camera.h
* @brief 摄像头界面.
* @author UDS
* @par   Copyright (c) UDS All Rights Reserved.
* @date  11/28/2016
*/ 
// CDlg_Camera 对话框

/** INI文件中的Camera配置 */
struct INI_CAMERA
{
	CString Camera;
	CString CamFrameSize;
	CString CamTempPath;
	long CamExposure;
	long CamBrightness;
	long CamImageType;
	long CamDocSize;
	long CamDocWidth;
	long CamDocHeight;
	long CamOrientation;
	bool CamAutoClip ;
	bool CamAutoEnhance;
	bool CamAutoRotate;
};

class CDlg_Camera : public CDialogEx
{
	DECLARE_DYNAMIC(CDlg_Camera)

public:
	CDlg_Camera(MFC_UI *pUI,CWnd* pParent = NULL);   // 重载构造函数
	virtual ~CDlg_Camera();

// 对话框数据
	enum { IDD = IDD_DLG_CAMERA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_cbCameraList;         ///< 设备列表
	CComboBox m_cbDocList;            ///< 文档大小
	CComboBox m_cbSizeList;           ///< 像素（分辨率）
	CComboBox m_cbImageType;          ///< 图像类型

	CButton m_bPhoto;                 ///< 拍照
	CButton m_bStop;                  ///< 暂停
	CButton m_bHelp;                  ///< 帮助
	CButton m_cAutoClip;              ///< 智能拍照
	CButton m_cAutoEnhance;           ///< 文档增强
	CButton m_cAutoRotate;            ///< 文档旋转
	//CButton m_bDelete;      

	CStatic m_sPreviewWnd;            ///< 预览窗口
	CStatic m_sPhotoNo;               ///< 照片数

	CSliderCtrl m_slExposure;         ///< 曝光度
	CSliderCtrl m_slBrightness;       ///< 亮度

	INI_CAMERA m_ini;                 ///< 配置文件信息

	CListCtrl   m_listctrl;           ///< 缩略图对应ListCtrl控件
	CImageList	m_imagelist;          ///< 缩略图对应ImageList控件
private:
	MFC_UI *m_pUI;

	
protected:
	CUDSCapture m_Capture;
	bool m_bIsHelp;                   ///< 是否正在显示帮助信息
	void MapDocSize();
	virtual BOOL OnInitDialog();

	/**
	*  @brief  打开相机
	*/
	void StartCamera();

	/**
	*  @brief  设置Cap的值
	*/
	void SetCapValue(void);

	/**
	*  @brief  从INI文件中读取相机配置
	*/
	void ReadCameraSettingFromINI();

	/**
	*  @brief  将相机配置写入INI文件
	*/
	void WriteCameraSettingToINI();

	/**
	*  @brief  图片准备好消息响应函数
	*/
	LRESULT OnImageReady(WPARAM wParam, LPARAM lParam);

	/**
	*  @brief  图片保存消息响应函数
	*/
	LRESULT OnImageSaved(WPARAM wParam, LPARAM lParam);

	/**
	*  @brief  在指定路径下创建文件夹
	*  @param[in]  strPath 路径 
	*  @retval true 表示成功
	*  @retval false 表示失败  
	*/
	bool CreateDir(const CString& strPath);

	/**
	*  @brief  清空并删除文件夹
	*  @param[in]  szPath 待清空的文件夹路径
	*  @param[in]  deleteDir 是否删除文件夹
	*  @retval true 表示成功
	*  @retval false 表示失败  
	*/
	bool ClearAndDeleteDir(const TCHAR* szPath, bool deleteDir = false);

	/**
	*  @brief  加载缩略图
	*/
	void LoadThumbNail();

	/**
	*  @brief  通过文件名获取文件类型
	*  @param[in]  filename 
	*  @return 文件类型
	*/
	int GetTypeFromFileName(const CString filename);

	/**
	*  @brief  图像处理
	*/
	enum enum_image_handle  
	{
		left90,          /**< 左转90度 */ 
		right90,         /**< 右转90度 */
		flip,            /**< 水平翻转 */
		mirror           /**< 垂直镜像 */
	};

	/**
	*  @brief  图像处理
	*  @param[in]  eMethod 处理方法
	*/
	void ImageHandle(enum_image_handle eMethod);	


	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnButton_Photo();
	afx_msg void OnButton_Stop();
	afx_msg void OnButton_Camimage();
	afx_msg void OnButton_CameraSetting();
	afx_msg void OnButton_Adjust();
	afx_msg void OnButton_Help();

	afx_msg void OnCbnSelchangeCombo_Cameralist();
	afx_msg void OnCbnSelchangeCombo_Imagesize();
	afx_msg void OnCbnSelchangeCombo_Imagetype();
	afx_msg void OnCbnSelchangeCombo_Docsize();

	afx_msg void OnCheck_Autophoto();
	afx_msg void OnCheck_Autoenhance();
	afx_msg void OnCheck_Autorotate();

	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnOk();
	afx_msg void OnClose();
	afx_msg void OnCancel();
	afx_msg void OnImageDelete();
	afx_msg void OnNMRClickListThunmbnail(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void OnImageLeft90();
	afx_msg void OnImageRight90();
	afx_msg void OnImageFlipvertical();
	afx_msg void OnImageMirror();

};

#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "UDSCapture.h"
#include "MFC_UI.h"
/**
* @file   CDlg_Camera.h
* @brief ����ͷ����.
* @author UDS
* @par   Copyright (c) UDS All Rights Reserved.
* @date  11/28/2016
*/ 
// CDlg_Camera �Ի���

/** INI�ļ��е�Camera���� */
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
	CDlg_Camera(MFC_UI *pUI,CWnd* pParent = NULL);   // ���ع��캯��
	virtual ~CDlg_Camera();

// �Ի�������
	enum { IDD = IDD_DLG_CAMERA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_cbCameraList;
	CComboBox m_cbDocList;
	CComboBox m_cbSizeList;
	CComboBox m_cbImageType;

	CButton m_bPhoto;
	CButton m_bStop;
	CButton m_bDelete;
	CButton m_bHelp;

	CStatic m_sPreviewWnd;
	CStatic m_sPhotoNo;

	CButton m_cAutoClip;
	CButton m_cAutoEnhance;
	CButton m_cAutoRotate;

	CSliderCtrl m_slExposure;
	CSliderCtrl m_slBrightness;

	INI_CAMERA m_ini; ///< ���������ļ���Ϣ

	CListCtrl   m_listctrl;   ///< ����ͼ��ӦListCtrl�ؼ�
	CImageList	m_imagelist;  ///< ����ͼ��ӦImageList�ؼ�
private:
	MFC_UI *m_pUI;

	
protected:
	CUDSCapture m_Capture;
	bool m_bIsHelp;
	void MapDocSize();
	virtual BOOL OnInitDialog();

	/**
	*  @brief  �����
	*/
	void StartCamera();

	/**
	*  @brief  ����Cap��ֵ
	*/
	void SetCapValue(void);

	/**
	*  @brief  ��INI�ļ��ж�ȡ�������
	*/
	void ReadCameraSettingFromINI();

	/**
	*  @brief  ͼƬ׼������Ϣ��Ӧ����
	*/
	LRESULT OnImageReady(WPARAM wParam, LPARAM lParam);

	/**
	*  @brief  ͼƬ������Ϣ��Ӧ����
	*/
	LRESULT OnImageSaved(WPARAM wParam, LPARAM lParam);

	/**
	*  @brief  ��ָ��·���´����ļ���
	*  @param[in]  strPath ·�� 
	*  @retval true ��ʾ�ɹ�
	*  @retval false ��ʾʧ��  
	*/
	bool CreateDir(const CString& strPath);

	/**
	*  @brief  ��ղ�ɾ���ļ���
	*  @param[in]  szPath ����յ��ļ���·��
	*  @param[in]  deleteDir �Ƿ�ɾ���ļ�
	*  @retval true ��ʾ�ɹ�
	*  @retval false ��ʾʧ��  
	*/
	bool ClearAndDeleteDir(const TCHAR* szPath, bool deleteDir = false);

	/**
	*  @brief  ��������ͼ
	*/
	void LoadThumbNail();

	/**
	*  @brief  ͨ���ļ�����ȡ�ļ�����
	*  @param[in]  filename 
	*  @return �ļ�����
	*/
	int GetTypeFromFileName(const CString filename);

	/**
	*  @brief  ͼ����
	*/
	enum enum_image_handle  
	{
		left90,          /**< ��ת90�� */ 
		right90,         /**< ��ת90�� */
		flip,            /**< ˮƽ��ת */
		mirror           /**< ��ֱ���� */
	};

	/**
	*  @brief  ͼ����
	*  @param[in]  eMethod ������
	*/
	void ImageHandle(enum_image_handle eMethod);	


	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnButton_Photo();
	afx_msg void OnButton_Stop();
	afx_msg void OnButton_Camimage();
	afx_msg void OnButton_Delete();
	afx_msg void OnCbnSelchangeCombo_Cameralist();
	afx_msg void OnCbnSelchangeCombo_Imagesize();
	afx_msg void OnCbnSelchangeCombo_Imagetype();
	afx_msg void OnCbnSelchangeCombo_Docsize();
	afx_msg void OnCheck_Autophoto();
	afx_msg void OnCheck_Autoenhance();
	afx_msg void OnCheck_Autorotate();
	afx_msg void OnButton_Adjust();
	afx_msg void OnButton_Help();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnButton_Camvideo();
	afx_msg void OnOk();
	afx_msg void OnClose();
	afx_msg void OnCancel();
	afx_msg void OnImageDelete();
	afx_msg void OnNMRClickListThunmbnail(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnImageLeft90();
	afx_msg void OnImageRight90();
	afx_msg void OnImageFlipvertical();
	afx_msg void OnImageMirror();
};

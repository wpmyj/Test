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
	CString Camera;              /**< Ĭ������ͷ���� */
	CString CamFrameSize;        /**< ͼ��֡�ߴ� */
	CString SaveAsPath;         /**< ͼ��洢·�� */
	long CamExposure;            /**< �ع�� */
	long CamBrightness;          /**< ���� */
	long CamImageType;           /**< ͼ������.  0:��ɫ, 1:�Ҷ�*/
	long CamDocSize;             /**< �ĵ���С. 0:A3, 1:A4, 2:B5, 3:A5, 4:���֤, 5:�Զ��� */
	long CamDocWidth;            /**< �Զ����ֶ������� */
	long CamDocHeight;           /**< �Զ����ֶ�����߶� */
	long CamOrientation;         /**< Ĭ����ת�Ƕ�. 0-0, 1-90, 2-180, 3-270 */
	long threshold;              /**< ��ֵ. 0~255 */
	long DpiNumber;              /**< DPI��Ӧ��� */ 
	//long YDPI;                   /**< ��ֱ����ֱ��� */
	BYTE JpegQuality;            /**< JPEGͼƬ���� */
	bool CamAutoClip ;           /**< �Ƿ����ܲ�����ת����.  N:��, Y:�� */
	bool CamAutoEnhance;         /**< �Ƿ�����ĵ���ǿ.  N:��, Y:�� */
	bool CamAutoRotate;          /**< �Ƿ��Զ�˳ʱ��ת90��. N:��, Y:�� */
	bool UploadFileOnebyOne;     /**< �Ƿ�����һ�Ŵ�һ��. N:��, Y:��*/
	bool ShowThumbnail;          /**< �Ƿ���ʾ����ͼ. N:��, Y:�� */
	bool SaveAs;                 /**< �Ƿ����ͼƬ */
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
	CComboBox m_cbCameraList;         ///< �豸�б�
	CComboBox m_cbDocList;            ///< �ĵ���С
	CComboBox m_cbSizeList;           ///< ���أ��ֱ��ʣ�
	CComboBox m_cbImageType;          ///< ͼ������

	CButton m_bPhoto;                 ///< ����
	CButton m_bStop;                  ///< ��ͣ
	CButton m_bHelp;                  ///< ����
	CButton m_cAutoClip;              ///< ��������
	CButton m_cAutoEnhance;           ///< �ĵ���ǿ
	CButton m_cAutoRotate;            ///< �ĵ���ת
	//CButton m_bDelete;      

	CStatic m_sPreviewWnd;            ///< Ԥ������
	CStatic m_sPhotoNo;               ///< ��Ƭ��

	CSliderCtrl m_slExposure;         ///< �ع��
	CSliderCtrl m_slBrightness;       ///< ����

	INI_CAMERA m_ini;                 ///< �����ļ���Ϣ

	CListCtrl   m_listctrl;           ///< ����ͼ��ӦListCtrl�ؼ�
	CImageList	m_imagelist;          ///< ����ͼ��ӦImageList�ؼ�

	
private:
	MFC_UI *m_pUI;
	//MAP_CAP m_mapCap;                ///<���ڱ�������ı���ֵ
	
protected:
	CUDSCapture m_Capture;
	bool m_bIsHelp;                   ///< �Ƿ�������ʾ������Ϣ
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
	*  @brief  ���������д��INI�ļ�
	*/
	void WriteCameraSettingToINI();

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
	*  @param[in]  deleteDir �Ƿ�ɾ���ļ���
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

	/**
	*  @brief  �������ڴ�С��λ�õ�
	*/
	void AdjustWindow();

	/**
	*  @brief ���¿ؼ�״̬
	*/
	//void UpdateControls(void);  

	/**
	*  @brief ��ʼ���ؼ�
	*/
	void InitControls(void);  

	/**
	*  @brief  ��ȡ��ʱ����ͼƬ���ļ���·��
	*  @param[out]  pszPath ͼƬ·��
	*  @retval true ��ʾ�ɹ�
	*  @retval false ��ʾʧ��  
	*/
	bool GetTempSavePath(TCHAR* pszPath);

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

public:
	afx_msg void OnNMCustomdrawCamera_Slider_Threshold(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnCbnSelchange_Camera_ComboDpi();
};

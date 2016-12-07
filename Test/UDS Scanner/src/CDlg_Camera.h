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

private:
	MFC_UI *m_pUI;

	
protected:
	CUDSCapture m_Capture;
	bool m_bIsHelp;
	void MapDocSize();
	virtual BOOL OnInitDialog();
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
public:
	afx_msg void OnOk();
	//afx_msg void OnDestroy();
	afx_msg void OnClose();
	afx_msg void OCancel();
};

#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "UDSCapture.h"

/**
* @file   CDlg_Camera.h
* @brief 摄像头界面.
* @author UDS
* @par   Copyright (c) UDS All Rights Reserved.
* @date  11/28/2016
*/ 
// CDlg_Camera 对话框

class CDlg_Camera : public CDialogEx
{
	DECLARE_DYNAMIC(CDlg_Camera)

public:
	CDlg_Camera(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlg_Camera();

// 对话框数据
	enum { IDD = IDD_DLG_CAMERA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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

public:
	CUDSCapture m_Capture;
	bool m_bIsHelp;
	void MapDocSize();
	virtual BOOL OnInitDialog();
	void StartCamera();

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
};

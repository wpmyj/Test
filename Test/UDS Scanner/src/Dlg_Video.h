/**
* @file   Dlg_Video.h
* @brief ����UDS_Video.ocx�ؼ�������ͷ����.
* @author UDS
* @par   Copyright (c) UDS. All Rights Reserved.
* @date  02/24/2017
*/ 
#pragma once

#include "MFC_UI.h"
#include "CDUDS_Video.h"
#include "afxwin.h"
//#include "e:\git_respositories\test\test(ocx)\uds scanner\cduds_video.h"
// CDlg_Video �Ի���

/** INI�ļ��е�Camera���� */
struct INI_VIDEO
{
	//int  DpiIndex;            /**< DPI��� */
	//int  PixelType;           /**< ��ɫ��� */
	bool AutoCrop;            /**< �Ƿ��Զ����� */ 
	bool ShowInfo;            /**< �Ƿ���ʾ��Ƶ��Ϣ */
	bool playSound;           /**< �Ƿ񲥷����� */
	CString CameraName;       /**< ����ͷ���� */
};

class CDlg_Video : public CDialogEx
{
	DECLARE_DYNAMIC(CDlg_Video)

public:
	//CDlg_Video(CWnd* pParent = NULL);   // ��׼���캯��
	CDlg_Video(MFC_UI *pUI,CWnd* pParent = NULL);   // ���ع��캯��
	virtual ~CDlg_Video();

// �Ի�������
	enum { IDD = IDD_DLG_VIDEO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()


private:
	MFC_UI *m_pUI;

	int m_nRotateType;        ///< ѡ��ʽ��0-����ת 1-���� 2-180�� 3-����
	int m_nDevIndex;          ///< �豸���
	int m_nResIndex;          ///< �ֱ��ʱ��
	int m_nFileType;          ///< ����ͼƬ���ͣ�0-jpg 1-bmp 2-png 3-tif
	int m_nPixelType;         ///< ͼ�����ͣ� 0-��ɫ 1-�Ҷ� 2-�ڰ�
	int m_nFileCount;         ///< ������Ƭ����
	int m_nDPI;               ///< DPI
	//int m_nDPIIndex;          ///< DPI���
	bool m_bInitialend;       ///< �Ƿ��ʼ���ɹ�
	bool m_bIsDPI;            ///< �Ƿ���ʾDPI������Ϊ�ֱ��ʣ�
	bool m_bAutoCrop;         ///< �Ƿ��Զ�����
	bool m_bShowInfo;         ///< �Ƿ���ʾ��Ƶ��Ϣ
	bool m_bPlaySound;        ///< �Ƿ񲥷�����

	CString m_sCaptureName;   ///< ��Ƭ����
	CStatic m_sCaptureCount;  ///< ��ʾ��������Ƭ��
	CString m_strTempPath;    ///< ��Ƭ��ʱ�洢·��
	CString m_sCameraName;    ///< ����ͷ����

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
	*  @brief  ��ʼ���ؼ� 
	*  @retval TRUE ��ʾ�ɹ�
	*  @retval FALSE ��ʾʧ��  
	*/
	BOOL OcxInit();

	/**
	*  @brief  ����Cap��ֵ
	*/
	void SetCapValue(void);

	/**
	*  @brief  �л���DPI���߷ֱ��ʣ���Ҫ����combo
	*  @param[in]  bisDPI ��� 
	*/
	void SwitchToDPIorRes(bool bisDPI);

	/**
	*  @brief  ��INI�ļ��ж�ȡ�������
	*/
	void ReadSetting();

	/**
	*  @brief  ���������д��INI�ļ�
	*/
	void WriteSetting();

	/**
	*  @brief  ��������
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

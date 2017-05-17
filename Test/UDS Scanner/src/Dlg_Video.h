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

// CDlg_Video �Ի���

/** INI�ļ��е�Camera���� */
struct INI_VIDEO
{
	//int  DpiIndex;            /**< DPI��� */
	//int  PixelType;           /**< ��ɫ��� */
	bool AutoCrop;            /**< �Ƿ��Զ����� */ 
	bool NoStop;
	bool Running;
	//bool ShowInfo;            /**< �Ƿ���ʾ��Ƶ��Ϣ */
	//bool playSound;           /**< �Ƿ񲥷����� */
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
public:
	bool m_bInitialend;       ///< �Ƿ��ʼ���ɹ�
private:
	bool m_bIsDPI;            ///< �Ƿ���ʾDPI������Ϊ�ֱ��ʣ�
	bool m_bAutoCrop;         ///< �Ƿ��Զ�����
	bool m_bNoStop;           ///< �Ƿ����պ�ر�����ͷ
	bool m_bRunning;          
	//bool m_bShowInfo;         ///< �Ƿ���ʾ��Ƶ��Ϣ
	//bool m_bPlaySound;        ///< �Ƿ񲥷�����

	CString m_sCaptureName;   ///< ��Ƭ����
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
	CStatic m_sCaptureCount;  ///< ��ʾ��������Ƭ��
	CImageList	m_imagelist;  ///< ����ͼ��ӦImageList�ؼ�
	CListCtrl   m_listctrl;   ///< ����ͼ��ӦListCtrl�ؼ�

	//CToolTipCtrl m_ToolTipCtrl;
	//int b;
	//std::vector<std::string> m_vector_thumbnail;
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


	//load png from resource
	bool LoadImageFromResource(IN CImage* pImage, IN UINT nResID, IN LPCTSTR lpTyp);

	/**
	*  @brief  ���ؼ����png��ʽ�ı���ͼƬ
	*/
	void AddPngToControl();


	//HWND CreateToolTip(int toolID, HWND hDlg, PTSTR pszText);

	/**
	*  @brief  ������������ToolTip 
	*  @param[in]  hwndParent  �����ھ��
	*  @param[out] pszText ��ʾ����  
	*/
	void CreateToolTipForRect(HWND hwndParent, PTSTR pszText);

	/**
	*  @brief  ��ղ�ɾ���ļ���
	*  @param[in]  szPath ����յ��ļ���·��
	*  @param[in]  deleteDir �Ƿ�ɾ���ļ���
	*  @retval true ��ʾ�ɹ�
	*  @retval false ��ʾʧ��  
	*/
	bool ClearAndDeleteDir(const TCHAR* szPath, bool deleteDir = false);

	/**
	*  @brief  ����CStatic�ؼ��ı����ݲ�ˢ��
	*  @param[in]  nID �ؼ�ID
	*  @param[in] strText �ı����� 
	*/
	void RefreshCStaticControl(int nID, LPCTSTR strText);

	/**
	*  @brief  ��������ͼ������List�ؼ���ʾ��
	*  @param[in]  strName ͼ������ 
	*/
	void SaveThumbNail(LPCTSTR strName);

	/**
	*  @brief  ��������ͼ
	*/
	void LoadThumbNail();


	/**
	*  @brief  ͼ����
	*/
	enum enum_image_handle  
	{
		IH_LEFT90,          /**< ��ת90�� */ 
		IH_RIGHT90,         /**< ��ת90�� */
		IH_FLIP,            /**< ˮƽ��ת */
		IH_MIRROR           /**< ��ֱ���� */
	};

	/**
	*  @brief  ͼ����
	*  @param[in]  eMethod ������
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

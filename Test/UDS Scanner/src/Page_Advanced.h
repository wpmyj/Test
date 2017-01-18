#pragma once
#include "MFC_UI.h"
#include "afxwin.h"
#include "Page_Custom.h"

class MFC_UI;

// CPage_Advanced �Ի���

class CPage_Advanced : public CPropertyPage, public CPage_Custom
{
	DECLARE_DYNAMIC(CPage_Advanced)

public:
	//CPage_Advanced();
	CPage_Advanced(MFC_UI *pUI);
	virtual ~CPage_Advanced();

// �Ի�������
	enum { IDD = IDD_PROPPAGE_ADVANCED };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()


private:
	/**
	* @brief ������
	* @see  MFC_UI::DisplayTWAINGUI()
	* @note ���ⲿ����ֵ
	*/
	MFC_UI  *m_pUI; 
private:

	    
	/**
	* @brief ֽ�Ŵ�С: US Letter/US Legal/A4/A5  
	* @see CTWAINDS_FreeIMage.cpp Line 606
	*/
	CComboBox m_combo_standardsizes;  

	/**
	* @brief ֽ�ŷ���: ����/����  
	* @see CTWAINDS_FreeIMage.cpp Line 615
	*/
//	CComboBox m_combo_orientation;
///<ֽ�ŷ���

	CComboBox m_combo_uints;  ///<����λ

	CComboBox m_combo_rotate;  ///<ͼ������-ͼ����ת
	CComboBox m_combo_splitimage; ///<ͼ������-ͼ��ָ�

	CComboBox m_combo_resolution; ///< �������-�ֱ���
	CComboBox m_combo_binarization;  ///<�������-��ֵ��

	CEdit m_edit_custom_width;  ///< ֽ�Ŵ�С���Զ�����
	CEdit m_edit_custom_height;  ///< ֽ�Ŵ�С���Զ���߶�

	CEdit m_edit_brightness; ///< �������,����
	CEdit m_edit_contrast; ///< ����������Աȶ�
	CEdit m_edit_sensitive_threshold; ///< ���������ȥ���ߵ�
	CEdit m_edit_gamma; ///< GammaУ��
	CSliderCtrl m_slider_brightness;
	CSliderCtrl m_slider_contrast;
	CSliderCtrl m_slider_sensitive_threshold;
	CSliderCtrl m_slider_gamma;


	CButton m_check_multistream;
	CButton m_check_backbw;
	CButton m_check_backcolor;
	CButton m_check_backgray;
	CButton m_check_frontbw;
	CButton m_check_frontcolor;
	CButton m_check_frontgray;

	CButton m_check_removeblank;
	CButton m_check_removepunch;
	CButton m_check_sharpen;
//	CButton m_check_gamma;
	CButton m_check_mirror;
	CButton m_check_removeback;
	CButton m_check_removedescreen;
	CButton m_check_removedenoise;
	CButton m_check_autocrop;

	MAP_CAP m_advancedmap;  ///<���ڱ�������ı���ֵ

	int checknum; ///<��¼��������ĸ���

	TW_FRAME frame;

	//typedef enum _MULTISTREAM_ID
	//{
	//	FRONT_COLOR = IDC_CHECK_FRONTCOLOR,
	//	FRONT_GRAY,  
	//	FRONT_BW,    
	//	BACK_COLOR = FRONT_BW + 2, 
	//	BACK_GRAY,  
	//  BACK_BW,    
	//} MULTISTREAM_ID;

	//MULTISTREAM_ID m_enum_multiId;

	//int multiID[6];
public:
	void UpdateControls(void);  ///< ���¿ؼ�״̬
	void InitSliderCtrl();  ///< ��ʼ���������ؼ�
	void SetCapValue(void); ///<���ò���
	void SetMultistream(void); ///<���ö���������ײ���
	void SetColorGrayImage(void); ///<���ò�ɫ�ͻҶȣ����ȡ��Աȶȿ���
	void SetBWImage(void); ///<���úڰ�ͼƬʱ��ֵ������
	void SetBinarization(void); ///<���ö�ֵ���ֱ�ѡ��ֵͬʱ������ʾ��ȥ���ߵ㡱���ǡ���ɫ������
	void SetStandardsizes(void); ///<����ֽ�Ŵ�Сѡ���Զ��塱ʱ�����ߵ�����
	void InitAdvancedmap(void); ///<��ʼ��Mapֵ����Ҫ��Ϊ�ָ�Map����Ĭ��ֵ

	//void GetCheckNum(void); ///<��ȡ����������м���ѡ��
	void SetBlank(void); ///<�жϲ�����ȥ���հ�ҳcheckBox�ĳ�ʼ״̬

	int FindPaperSize(int index); ///<Ѱ��index��Ӧ��ֽ�Ŵ�С,����index��Ӧ��ֽ�Ŵ�С
	 
	//void SetMultiIDValue();  ///< ������multiID��ֵ
private:
	virtual BOOL OnInitDialog();

	afx_msg void OnCbnSelchangeAdvanced_Combo_Standardsizes();
	afx_msg void OnCbnSelchangeAdvanced_Combo_Uints();
	
	afx_msg void OnCbnSelchangeAdvanced_Combo_Resolution();
	afx_msg void OnCbnSelchangeAdvanced_Combo_Rotate();

	afx_msg void OnCbnSelchangeAdvanced_Combo_Binarization();
	afx_msg void OnCbnSelchangeAdvanced_Combo_SpiltImage();

	afx_msg void OnAdvanced_Btn_Check_Multistream();
	
	afx_msg void OnNMCustomdrawAdvanced_Slider_Brightness(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawAdvanced_Slider_Contrast(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawAdvanced_Slider_SensitiveThreshold(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawAdvanced_Slider_Gamma(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void OnEnChangeAdvanced_Edit_Contrast();
	afx_msg void OnEnChangeAdvanced_Edit_Brightness();
	afx_msg void OnEnChangeAdvanced_Edit_SensitiveThreshold();
	afx_msg void OnEnChangeAdvanced_Edit_Gamma();

	afx_msg void OnAdvanced_Btn_Check_FrontColor();
	afx_msg void OnAdvanced_Btn_Check_FrontGray();
	afx_msg void OnAdvanced_Btn_Check_FrontBW();
	afx_msg void OnAdvanced_Btn_Check_BackGray();
	afx_msg void OnAdvanced_Btn_Check_BackColor();
	afx_msg void OnAdvanced_Btn_Check_BackBW();

	afx_msg void OnAdvanced_Btn_Check_RemoveBlank();
	afx_msg void OnAdvanced_Btn_Check_RemovePunch();
	afx_msg void OnAdvanced_Btn_Check_Sharpen();
	afx_msg void OnAdvanced_Btn_Check_Mirror();
	afx_msg void OnAdvanced_Btn_Check_RemoveBack();
	afx_msg void OnAdvanced_Btn_Check_RemoveDenoise();
	afx_msg void OnAdvanced_Btn_Check_RemoveDescreen();
	afx_msg void OnAdvanced_Btn_Check_AutoCrop();

public:
	/** Base����ĸ���ָ��*/
	CPage_Custom* m_pBasePage;

	CSliderCtrl m_slider_removeblank;
	CEdit m_edit_removeblank;
	afx_msg void OnNMCustomdrawAdvanced_Slider_Removeblank(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeAdvanced_Edit_Removeblank();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//virtual void OnOK();
};

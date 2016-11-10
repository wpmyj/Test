#pragma once
#include "MFC_UI.h"
#include "afxwin.h"
class MFC_UI;

// CPage_Advanced �Ի���

class CPage_Advanced : public CPropertyPage
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
	CComboBox m_combo_orientation; ///<ֽ�ŷ���

	CComboBox m_combo_uints;  ///<����λ

	CComboBox m_combo_rotate;  ///<ͼ������-ͼ����ת
	CComboBox m_combo_splitimage; ///<ͼ������-ͼ��ָ�

	CComboBox m_combo_resolution; ///< �������-�ֱ���
	CComboBox m_combo_binarization;  ///<�������-��ֵ��

	CEdit m_edit_custom_width;  ///< ֽ�Ŵ�С���Զ�����
	CEdit m_edit_custom_height;  ///< ֽ�Ŵ�С���Զ���߶�

	CEdit m_edit_brightness; ///< �������,����
	CEdit m_edit_contrast; ///< ����������Աȶ�
	CSliderCtrl m_slider_brightness;
	CSliderCtrl m_slider_contrast;

	CButton m_check_multistream;
	CButton m_check_backbw;
	CButton m_check_backcolor;
	CButton m_check_backgray;
	CButton m_check_frontbw;
	CButton m_check_frontcolor;
	CButton m_check_frontgray;

	typedef map<int, int> MAP_CAP;
	MAP_CAP m_advancedmap;  ///<���ڱ�������ı���ֵ

private:
	void UpdateControls(void);  ///< ���¿ؼ�״̬
	void InitSliderCtrl();  ///< ��ʼ���������ؼ�
	//void SetControl(void); ///<���ò���
	void SetMultistream(void); ///<���ö���������ײ���
	void SetColorGrayImage(void); ///<���ò�ɫ�ͻҶȣ����ȡ��Աȶȿ���
	void SetBWImage(void); ///<���úڰ�ͼƬʱ��ֵ������

private:
	virtual BOOL OnInitDialog();
	//virtual void OnOK();

	afx_msg void OnCbnSelchangeAdvanced_Combo_Standardsizes();
	afx_msg void OnCbnSelchangeAdvanced_Combo_Orientation();
	afx_msg void OnCbnSelchangeAdvanced_Combo_Uints();
	
	afx_msg void OnCbnSelchangeAdvanced_Combo_Resolution();
	afx_msg void OnCbnSelchangeAdvanced_Combo_Rotate();

	afx_msg void OnAdvanced_Btn_Check_Multistream();
	
	afx_msg void OnNMCustomdrawAdvanced_Slider_Brightness(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawAdvanced_Slider_Contrast(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeAdvanced_Edit_Contrast();
	afx_msg void OnEnChangeAdvanced_Edit_Brightness();

	afx_msg void OnAdvanced_Btn_Check_FrontColor();
	afx_msg void OnAdvanced_Btn_Check_FrontGray();
	afx_msg void OnAdvanced_Btn_Check_FrontBW();
	afx_msg void OnAdvanced_Btn_Check_BackGray();
	afx_msg void OnAdvanced_Btn_Check_BackColor();
	afx_msg void OnAdvanced_Btn_Check_BackBW();
};

#pragma once
#include "MFC_UI.h"
#include "afxwin.h"
#include "Dlg_Profile.h"
#include "Page_Custom.h"

class MFC_UI;
// CPage_Base �Ի���

class CPage_Base : public CPropertyPage, public CPage_Custom
{
	DECLARE_DYNAMIC(CPage_Base)

public:
	///< ���ع��캯��������β�MFC_UI *pUI
	CPage_Base(MFC_UI *pUI);
	virtual ~CPage_Base();

// �Ի�������
	enum { IDD = IDD_PROPPAGE_BASE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private:
	virtual void OnOK();
	virtual void OnCancel();
	//virtual BOOL OnApply();

private:
	/**
	* @brief �ڹ��캯���г�ʼ�� 
	* @see  CPage_Base(MFC_UI *pUI)
	*/
	MFC_UI  *m_pUI;  

private:
	void UpdateControls(void);  ///< ���¿ؼ�״̬
	void InitSliderCtrl();  ///< ��ʼ���������ؼ�
	void InitComboProfile();  ///< ��ʼ��ģ��Combo
	void LoadProfile();  ///< ����ģ��
	void NewBaseProfile(); ///<�½�����ģ��
	void SetLastProfile(); ///<����ģ�壬����ģ���д��ڡ��ϴ�ʹ��ģ�塱�����
	void InitComboPixType(void); ///<��ʼ��ͼ�����ͱ༭��

	void SetCapValue(void);  ///<���ò���
	void SetDelete(void); ///<����ɾ����ť�Ƿ����

	/** 
	//* @brief �����ѡ�񵥸�ͼƬ�ļ�
	//* @param[out] strFilePath ͼƬ����·��
	//* @return �ɹ�����true
	//*/
	//bool MyBrowseForSignalImage(PTCHAR strFilePath);

	/**
	* @brief �����ѡ����ͼƬ�ļ�
	* @return �ɹ�����vector<string>����ֵ
	*/
	vector<string> MyBrowseForMultiImages();

	/**
	*  @brief  �½�ָ������ģ��
	*  @param[in]  profilename ģ����
	*  @param[in]  pixeltype ͼ������ 
	*  @param[in]  duplexenabled ��/˫�� 
	*  @param[in]  resolution �ֱ���(Ĭ��200dpi)  
	*  @retval true ��ʾ�ɹ�
	*  @retval false ��ʾʧ��  
	*/
	bool CreateNewProfile(std::string profilename, int pixeltype, 
		int duplexenabled, int resolution = 200);

private:
	CComboBox m_combo_source;   ///< ɨ�跽ʽ:  ADF/Flatbed
	CComboBox m_combo_colormode;  ///< ͼ������:  �ڰ�/�Ҷ�/��ɫ
	CComboBox m_combo_resolution;  ///< �ֱ���:  50/100/200/.../600
	CComboBox m_combo_profile;  ///< ģ��
	/**
	* @brief ����/˫��ɨ:  ����/˫��  
	* @see CTWAINDS_FreeIMage.cpp Line 675
	*/
	CComboBox m_combo_scanside;

	CSliderCtrl m_slider_contrast;  ///< �Աȶ�:  -100~+100
	CSliderCtrl m_slider_brightness;  ///< ����:  -100~+100
	CSliderCtrl m_slider_threshold;

	CEdit m_edit_contrast;  ///< ����ͬ����ʾm_slider_contrastֵ
	CEdit m_edit_brightness;   ///< ����ͬ����ʾm_slider_brightnessֵ
	CEdit m_edit_threshold;

	CButton m_check_multifeeddetect; ///<"���ż��"
	CButton m_btn_chooseimage; ///<"ѡ��ͼƬ��ť"

	MAP_CAP m_basemap;  ///<���ڱ�������ı���ֵ

	//vector<string> m_vector_string_imagepath;  /**< ͼƬ·�� */

	//CDlg_Profile *m_pDlg;

	virtual BOOL OnInitDialog();
	
	afx_msg void OnNMCustomdrawBase_Slider_Contrast(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawBase_Slider_Brightness(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawBase_Slider_Threshold(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void OnEnChangeBase_Edit_Contrast();
	afx_msg void OnEnChangeBase_Edit_Brightness();
	afx_msg void OnEnChangeBase_Edit_Threshold();

	afx_msg void OnCbnSelchangeBase_Combo_Source();
	afx_msg void OnCbnSelchangeBase_Combo_Colormode();
	afx_msg void OnCbnSelchangeBase_Combo_Resolution();
	afx_msg void OnCbnSelchangeBase_Combo_Profile();
	afx_msg void OnCbnSelchangeBase_Combo_Scanside();

	afx_msg void OnBase_Btn_Newprofile();
	afx_msg void OnBase_Btn_Deleteprofile();

	afx_msg void OnClicked_Check_Multifeeddetect();

  /** ѡ���ɨͼƬ */
	afx_msg void OnBase_Btn_Chooseimage();
	/** ���Ϊģ�� */
	afx_msg void OnBase_Btn_SaveAsprofile();
	/** ���浱ǰģ��*/
//	afx_msg void OnBase_Btn_Saveprofile();

public:
	/** ����ָ��*/
	CPage_Custom* m_pAdPage;

};

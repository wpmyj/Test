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
	virtual BOOL OnSetActive();

	DECLARE_MESSAGE_MAP()

private:
	/**
	* @brief ������
	* @see  MFC_UI::DisplayTWAINGUI()
	* @note ���ⲿ����ֵ
	*/
	MFC_UI  *m_pUI; 

private:  

	CComboBox m_combo_rotate;  ///<ͼ������-ͼ����ת
	CComboBox m_combo_splitimage; ///<ͼ������-ͼ��ָ�
	int m_radiobtn_spilt_vert;

	CEdit m_edit_gamma; ///< GammaУ��
	CEdit m_edit_removeblank; ///<ȥ���հ�ҳ
	CEdit m_edit_spilt;

	CComboBox m_combo_cachemode; ///<����ģʽ
//	CEdit m_edit_cachemode;
	CSliderCtrl m_slider_cachemode;

	CSliderCtrl m_slider_gamma;
	CSliderCtrl m_slider_removeblank;		
	
	CButton m_check_removeblank;
	CButton m_check_removepunch;
	CButton m_check_sharpen;
	CButton m_check_mirror;
	CButton m_check_removeback;
	CButton m_check_removedescreen;
	CButton m_check_removedenoise;
	CButton m_check_autocrop;
	CButton m_check_colorflip;

	CButton m_check_multifeeddetect;
	CButton m_check_mdvalue;

	MAP_CAP m_advancedmap;  ///<���ڱ�������ı���ֵ

	TW_FRAME frame;
public:
	void UpdateControls(void);  ///< ���¿ؼ�״̬
	void InitSliderCtrl();  ///< ��ʼ���������ؼ�
	void SetCapValue(void); ///<���ò���
//void SetStandardsizes(void); ///<����ֽ�Ŵ�Сѡ���Զ��塱ʱ�����ߵ�����
	void InitAdvancedmap(void); ///<��ʼ��Mapֵ����Ҫ��Ϊ�ָ�Map����Ĭ��ֵ
	void SetBlank(void); ///<�жϲ�����ȥ���հ�ҳcheckBox�ĳ�ʼ״̬
	void SetSpiltimage(void); ///<���ò��ͼ��Ϊ�Զ���ʱ���Աߵ�edit���á�
	void SetColorMode(int nIndex); ///���ݻ���ģʽ��ѡ�����ö�Ӧ�������ķ�Χ�Լ�static��ֵ
	
private:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	
	afx_msg void OnCbnSelchangeAdvanced_Combo_Rotate();
	afx_msg void OnCbnSelchangeAdvanced_Combo_SpiltImage();
	
	afx_msg void OnNMCustomdrawAdvanced_Slider_Gamma(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawAdvanced_Slider_Removeblank(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeAdvanced_Edit_Gamma();
	afx_msg void OnEnChangeAdvanced_Edit_Removeblank();

	afx_msg void OnAdvanced_Btn_Check_RemoveBlank();
	afx_msg void OnAdvanced_Btn_Check_RemovePunch();
	afx_msg void OnAdvanced_Btn_Check_Sharpen();
	afx_msg void OnAdvanced_Btn_Check_Mirror();
	afx_msg void OnAdvanced_Btn_Check_RemoveBack();
	afx_msg void OnAdvanced_Btn_Check_RemoveDenoise();
	afx_msg void OnAdvanced_Btn_Check_RemoveDescreen();
	afx_msg void OnAdvanced_Btn_Check_AutoCrop();
	afx_msg void OnAdvanced_RadioBtn_Spilt();

	afx_msg void OnClicked_Check_Multifeeddetect();

	afx_msg void OnAdvanced_Btn_Check_Colorflip();

	afx_msg void OnCbnSelchangeAdvanced_Combo_Cachemode();
	afx_msg void OnNMCustomdrawAdvanced_Slider_Cachemode(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnClicked_Check_MdValue();

public:
	/** Base����ĸ���ָ��*/
	CPage_Custom* m_pBasePage;

};

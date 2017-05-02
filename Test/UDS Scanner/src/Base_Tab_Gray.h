#pragma once

#include "Page_Custom.h"
#include "MFC_UI.h"

class MFC_UI;
// CBase_Tab_Gray �Ի���

class CBase_Tab_Gray : public CDialogEx, public CPage_Custom
{
	DECLARE_DYNAMIC(CBase_Tab_Gray)

public:
	CBase_Tab_Gray(MFC_UI *pUI); 
	virtual ~CBase_Tab_Gray();

// �Ի�������
	enum { IDD = IDD_BASETAB_GRAY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

private:
	/**
	* @brief �ڹ��캯���г�ʼ�� 
	* @see  CPage_Base(MFC_UI *pUI)
	*/
	MFC_UI  *m_pUI; 
public:
	virtual BOOL OnInitDialog();
	
	afx_msg void OnCbnSelchangeTabgray_Combo_Resolution();
	afx_msg void OnNMCustomdrawTabgray_Slider_Contrast(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawTabgray_Slider_Brightness(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeTabgray_Edit_Contrast();
	afx_msg void OnEnChangeTabgray_Edit_Brightness();
//	afx_msg void OnTabGray_RadioBtn_Compress();
	afx_msg void OnNMCustomdrawTabgray_Slider_Compressionvalue(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeTabgray_Edit_Compressvalue();

	afx_msg void OnCbnSelchangeTabGray_Combo_FilterColor();
	afx_msg void OnCbnSelchangeTabGray_Combo_FilterMode();
	afx_msg void OnNMCustomdrawTabGray_Slider_Filterlevel(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeTabGray_Edit_Filterlevel();
	afx_msg void OnCbnSelchangeTabgray_Combo_Compressquality();

	CComboBox m_combo_resolution;

	CSliderCtrl m_slider_brightness;
	CEdit m_edit_brightness;
	CSliderCtrl m_slider_contrast;
	CEdit m_edit_contrast;
	
//	int m_radio_compress;
	CSliderCtrl m_slider_compressvalue;
	CEdit m_edit_compressvalue;
	CComboBox m_combo_filtercolor;
	CComboBox m_combo_filtermode;
	CSliderCtrl m_slider_filterlevel;
	CEdit m_edit_filterlevel;

	CComboBox m_combo_compressquality;

public:
	void UpdateControls(void);  ///< ���¿ؼ�״̬
	void InitSliderCtrl();  ///< ��ʼ���������ؼ�
	void SetCompressValue();  ///<����ѹ��ֵ�������Ƿ����	
};

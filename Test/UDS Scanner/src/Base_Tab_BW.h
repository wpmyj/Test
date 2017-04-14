#pragma once
#include "afxwin.h"

#include "Page_Custom.h"
#include "MFC_UI.h"

class MFC_UI;
// CBase_Tab_BW �Ի���

class CBase_Tab_BW : public CPropertyPage, public CPage_Custom
{
	DECLARE_DYNAMIC(CBase_Tab_BW)

public:
	CBase_Tab_BW(MFC_UI *pUI);
	virtual ~CBase_Tab_BW();

// �Ի�������
	enum { IDD = IDD_BASETAB_BW };

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

	afx_msg void OnCbnSelchangeTabbw_Combo_Compress();
	afx_msg void OnNMCustomdrawTabbw_Slider_Compressionvalue(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeTabbw_Edit_Compressvalue();
	afx_msg void OnCbnSelchangeTabbw_Combo_Resolution();
	
	afx_msg void OnNMCustomdrawTabbw_Slider_Brightness(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeTabbw_Edit_Brightness();
	afx_msg void OnNMCustomdrawTabbw_Slider_Threshold(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeTabbw_Edit_Threshold();
	afx_msg void OnCbnSelchangeTabbw_Combo_Binarization();

	CComboBox m_combo_resolution;
	CComboBox m_combo_compress;
	CSliderCtrl m_slider_compressvalue;
	CEdit m_edit_compressvalue;

	CSliderCtrl m_slider_brightness;
	CEdit m_edit_threshold;
	CEdit m_edit_brightness;
	CSliderCtrl m_slider_threshold;
	CComboBox m_combo_binarization;

public:
	void UpdateControls(void);  ///< ���¿ؼ�״̬
	void InitSliderCtrl();  ///< ��ʼ���������ؼ�
	void SetBinarization(void);
	
};

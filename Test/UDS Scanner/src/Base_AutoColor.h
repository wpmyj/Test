#pragma once
#include "afxwin.h"

#include "Page_Custom.h"
#include "MFC_UI.h"
#include "Base_Tab_Color.h"
#include "Base_Tab_Gray.h"
#include "Base_Tab_BW.h"

class MFC_UI;
// CBase_AutoColor �Ի���

class CBase_AutoColor : public CDialogEx, public CPage_Custom
{
	DECLARE_DYNAMIC(CBase_AutoColor)

public:
	CBase_AutoColor(MFC_UI *pUI);
	virtual ~CBase_AutoColor();

// �Ի�������
	enum { IDD = IDD_BASETAB_AUTOCOLOR };

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
	void UpdateControls(void);  ///< ���¿ؼ�״̬
	void InitSliderCtrl();  ///< ��ʼ���������ؼ�
	void ShowOtherDialog();

private:
	CBase_Tab_Color m_dlg_color;
	CBase_Tab_Gray m_dlg_gray;
	CBase_Tab_BW m_dlg_bw;

public:
	CEdit m_edit_colorthres;
	CSliderCtrl m_slider_colorthres;
	CStatic m_staic_otherdialog;
	int m_radio_graybw;
	CButton m_radio_color;

	virtual BOOL OnInitDialog();
	afx_msg void OnEnChangeTabautocolor_Edit_Colorthres();
	afx_msg void OnNMCustomdrawTabautocolor_Slider_Colorthres(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnAuotoColor_RadioBtn_Nocolor_Gray();
	afx_msg void OnBase_Btn_Radio_color();
	afx_msg void OnAutocolor_Btn_Color();
	afx_msg void OnAutocolor_Btn_NocolorGray();
	afx_msg void OnAutocolor_Btn_NocolorBw();
};

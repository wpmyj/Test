#pragma once

#include "MFC_UI.h"
#include "afxwin.h"
#include "Page_Custom.h"

class MFC_UI;

// CPage_Set �Ի���

class CPage_Set : public CPropertyPage, public CPage_Custom
{
	DECLARE_DYNAMIC(CPage_Set)

public:
	CPage_Set(MFC_UI *pUI);
	virtual ~CPage_Set();

// �Ի�������
	enum { IDD = IDD_PROPPAGE_SET };

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

public:
	void UpdateControls(void);  ///< ���¿ؼ�״̬
	void InitSliderCtrl();  ///< ��ʼ���������ؼ�
	void SetCapValue(void); ///<���ò���
	void SetSavePower(void); ///<���ýڵ�ģʽ�������Ƿ����
	void SetOffTime(void); ///<���ùػ�ʱ�们�����Ƿ����

private:
	MAP_CAP m_setmap;  ///<���ڱ�������ı���ֵ


private:
	afx_msg void OnSet_Btn_Check_SavePower();
	afx_msg void OnSet_Btn_Check_OffTime();
	afx_msg void OnNMCustomdrawSet_Slider_SavePower(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSet_Slider_Offtime(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL OnInitDialog();

private:
	CButton m_check_savepower;
	CButton m_check_offtime;
	CSliderCtrl m_slider_offtime;
	CSliderCtrl m_slider_savepower;

};

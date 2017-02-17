#pragma once

#include "MFC_UI.h"
#include "Page_Custom.h"

class MFC_UI;
// CPage_Paper �Ի���

class CPage_Paper : public CPropertyPage, public CPage_Custom
{
	DECLARE_DYNAMIC(CPage_Paper)

public:
	CPage_Paper(MFC_UI *pUI);
	virtual ~CPage_Paper();

// �Ի�������
	enum { IDD = IDD_PROPPAGE_PAPER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

private:
	/**
	* @brief �ڹ��캯���г�ʼ�� 
	* @see  CPage_Base(MFC_UI *pUI)
	*/
	MFC_UI  *m_pUI; 

private:
	void SetCapValue(void);  ///<���ò���
	void UpdateControls(void);  ///< ���¿ؼ�״̬
	int FindPaperSize(int index); ///<Ѱ��index��Ӧ��ֽ�Ŵ�С,����index��Ӧ��ֽ�Ŵ�С
	int FindUnit(int index); ///<Ѱ��index��Ӧ�ĵ�λ,����index��Ӧ�ĵ�λ
	void SetScroll(void); ///<����edit��ֵ���¹�������ֵ;�Լ����ݵ�λ�趨���߹������ķ�Χ

private:
	MAP_CAP m_papermap;  ///<���ڱ�������ı���ֵ

	CComboBox m_combo_standardsizes; ///<ֽ�Ŵ�С: US Letter/US Legal/A4/A5
	CComboBox m_combo_uints; ///<��λ��Ӣ�ߣ����أ�����

	CEdit m_edit_width;
	CEdit m_edit_height;
	CScrollBar m_scroll_width;
	CScrollBar m_scroll_height;

	CEdit m_edit_xpos;
	CEdit m_edit_ypos;
	CScrollBar m_scroll_xpos;
	CScrollBar m_scroll_ypos;

	CEdit m_edit_up;
	CEdit m_edit_down;
	CEdit m_edit_left;
	CEdit m_edit_right;
	CScrollBar m_scroll_up;
	CScrollBar m_scroll_down;
	CScrollBar m_scroll_right;
	CScrollBar m_scroll_left;

private:
	virtual BOOL OnInitDialog();
	virtual BOOL OnSetActive();
	afx_msg void OnPaint();

private:
	afx_msg void OnCbnSelchangePaper_Combo_Standardsizes();
	afx_msg void OnCbnSelchangePaper_Combo_Uints();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	
};

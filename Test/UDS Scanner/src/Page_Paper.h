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
	void SetScroll(); ///<����edit��ֵ���¹�������ֵ;�Լ����ݵ�λ�趨���߹������ķ�Χ;
	void InitSliderCtrl();  ///< ��ʼ���������ؼ�
	void UpdatePicRectangle(int index, int unitindex, int xpos, int ypos); ///<����ѡ���ֽ�Ŷ�Ӧ��ţ�����ͼƬ�ؼ��ϵľ�������
	void SetPaperSize(void);
	float FindEditValue(int scrollpos); //���ݵ�λֵ����������ֵ��ת��

private:
	//int time;
	MAP_CAP m_papermap;  ///<���ڱ�������ı���ֵ

	CRectTracker m_rectTracker; ///<��Ƥ����
	CRect m_tRect; ///<���塢������Ƥ���ľ���

	CComboBox m_combo_standardsizes; ///<ֽ�Ŵ�С: US Letter/US Legal/A4/A5
	CComboBox m_combo_uints; ///<��λ��Ӣ�ߣ����أ�����
	
	/*int m_standarindex; ///<���ʼֽ�Ŵ�Сѡ���������ֵ
	int m_papervalue; ///<���ʼֽ�Ŵ�Сѡ�е�ֵ
	int m_unitindex; ///<���ʼ��λѡ���������ֵ
	int m_unitvalue; ///<���ʼ��λѡ�е�ֵ*/
	int m_resolution; ///<��ǰ�ֱ���

	CEdit m_edit_width;
	CEdit m_edit_height;
	CScrollBar m_scroll_width;
	CScrollBar m_scroll_height;

	//Ӣ�絥λ�£����ߵ����ȡֵ��ʹ��ʱ��Ҫ���ݵ�λת��
	int maxinches_xpos;
	int maxinches_ypos;
	int maxinches_updown;
	int maxinches_leftright;
	int maxinches_width;
	int maxinches_height;

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

	CComboBox m_combo_compress;
	CSliderCtrl m_slider_compressvalue;
	CEdit m_edit_compressvalue;

private:
	virtual BOOL OnInitDialog();
	virtual BOOL OnSetActive();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

private:
	afx_msg void OnCbnSelchangePaper_Combo_Standardsizes();
	afx_msg void OnCbnSelchangePaper_Combo_Uints();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	afx_msg void OnCbnSelchangePaper_Combo_Compress();
	afx_msg void OnEnChangeBase_Edit_Compressvalue();
	afx_msg void OnNMCustomdrawPaper_Slider_Compressionvalue(NMHDR *pNMHDR, LRESULT *pResult);
	
	afx_msg void OnEnChangeBase_Edit_EdgeRight();
	afx_msg void OnEnChangeBase_Edit_EdgeUp();
	afx_msg void OnEnChangeBase_Edit_EdgeLeft();
	afx_msg void OnEnChangeBase_Edit_EdgeDown();
	afx_msg void OnEnChangeBase_Edit_EdgeXpos();
	afx_msg void OnEnChangeBase_Edit_EdgeYpos();
};

#pragma once

#include "MFC_UI.h"
#include "afxwin.h"
#include "Dlg_Profile.h"
#include "Page_Custom.h"
#include "Base_Tab_Color.h"
#include "Base_Tab_Gray.h"
#include "Base_Tab_BW.h"
#include "Base_Tab_Preview.h"
#include "Base_Tab_Rotateshow.h"
#include "Base_Tab_Spiltshow.h"
#include "Base_AutoColor.h"


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
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	virtual BOOL OnSetActive();

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
	void SetCapValue(void);  ///<���ò���
	void InitBasemap(void); ///<��ʼ��Basemap����Ҫ����������CAP_DUPLEXENABLED�ĳ�ʼ��
	void SetFlat(void); ///< ����ɨ��ģʽΪƽ��ʱ��ֻ��Ϊ���档

	void SetMultistream(void); ///<���ö���������ײ���


	//bool MyBrowseForSignalImage(PTCHAR strFilePath);
	/**
	* @brief �����ѡ����ͼƬ�ļ�
	* @return �ɹ�����vector<string>����ֵ
	*/
	vector<string> MyBrowseForMultiImages();

	//2.0�汾
	void SetSpiltimage();
	void SetBlank(void);
	void SetCapMulti(); //���ö�����Ч
	void SetTabCtrl(); //����ͼ������ѡ��ʱ��Tabѡ����һҳ

	void SetAutoFrontColor();
	void SetAutoBackColor();
	void SetFrontColor();
	void SetFrontGray();
	void SetFrontBW();
	void SetBackColor();
	void SetBackGray();
	void SetBackBW();
	void PreView();
	int  GetCheckNum(); ///<�õ�����������checkboxѡ��

	void UpdateButton(int nIDCtrl, CString str);///<���°�ť�������ֵ���ɫ
	void SwitchCheckBtn(); ///<��ѡ��checkboxʱ��ѭ���ж�������������checkbox��״̬��

private:
	CComboBox m_combo_colormode;  ///< ͼ������:  �ڰ�/�Ҷ�/��ɫ

	int m_radiobtn_scanmode; ///< ɨ�跽ʽ:  ADF�Զ���ֽ��/Flatbedƽ�� 
	int m_radiobtn_duplex; ///<��˫�棺���桢˫�桢����

	CButton m_check_frontcolor;
	CButton m_check_frontgray;
	CButton m_check_frontbw;
	CButton m_check_backbw;
	CButton m_check_backgray;
	CButton m_check_backcolor;
	CButton m_check_autobackcolor;
	CButton m_check_autofrontcolor;

	CButton m_btn_frontcolor;
	CButton m_btn_frontgray;
	CButton m_btn_frontbw;
	CButton m_btn_backcolor;
	CButton m_btn_backgray;
	CButton m_btn_backbw;
	CButton m_btn_autofrontcolor;
	CButton m_btn_autobackcolor;
	CButton m_btn_rotateshow;
	CButton m_btn_spiltshow;

	CComboBox m_combo_rotate;
	CComboBox m_combo_splitimage;
	CSliderCtrl m_slider_gamma;
	CEdit m_edit_gamma;
	CSliderCtrl m_slider_removeblank;
	CEdit m_edit_removeblank;
	CButton m_check_removeblank;

	MAP_CAP m_basemap;  ///<���ڱ�������ı���ֵ

	TCHAR m_bmpFilePath[PATH_MAX]; ///<Ԥ��ͼ����·��

	CTabCtrl m_base_tab;
	CRect m_tabRect; // ��ǩTab�ؼ��ͻ�����Rect

	CMFCButton *m_mfcBtn;

	CComboBox m_combo_colorcorrect;

	afx_msg void OnCbnSelchangeBase_Combo_Colormode();

	afx_msg void OnBase_Btn_Newprofile();

	afx_msg void OnBase_RadioBtn_Scanmode_Auto();
	afx_msg void OnBase_RadioBtn_Scanmode_Flatbed();

	afx_msg void OnBase_RadioBtn_Duplex();

	afx_msg void OnBase_Btn_Check_FrontColor();
	afx_msg void OnBase_Btn_Check_FrontGray();
	afx_msg void OnBase_Btn_Check_FrontBw();
	afx_msg void OnBase_Btn_Check_BackColor();
	afx_msg void OnBase_Btn_Check_BackGray();
	afx_msg void OnBase_Btn_Check_BackBw();
	afx_msg void OnBase_Btn_Autofrontcolor();
	afx_msg void OnBase_Btn_Autobackcolor();
	afx_msg void OnBase_Btn_Check_Autofrontcolor();
	afx_msg void OnBase_Btn_Check_Autobackcolor();

	afx_msg void OnCbnSelchangeBase_Combo_Rotate();	
	afx_msg void OnCbnSelchangeBase_Combo_SpiltImage();	
	afx_msg void OnNMCustomdrawBase_Slider_Gamma(NMHDR *pNMHDR, LRESULT *pResult);	
	afx_msg void OnEnChangeBase_Edit_Gamma();	
	afx_msg void OnNMCustomdrawBase_Slider_Removeblank(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeBase_Edit_Removeblank();	
	afx_msg void OnBase_Btn_Check_RemoveBlank();

	afx_msg void OnBase_Btn_FrontColor();
	afx_msg void OnBase_Btn_Backcolor();
	afx_msg void OnBase_Btn_Frontgray();
	afx_msg void OnBase_Btn_Backgray();
	afx_msg void OnBase_Btn_Frontbw();
	afx_msg void OnBase_Btn_Backbw();

	afx_msg void OnBase_Btn_RotateShow();
	afx_msg void OnBase_Btn_SpiltShow();
	afx_msg void OnCbnSelchangeBase_Combo_Colorcorrect();

public:
	/** ����ָ��*/
	CPage_Custom* m_pAdPage;		
	
	//Tab�Ի�����
	CBase_Tab_Color *m_pTabColor;  //0
	CBase_Tab_Gray *m_pTabGray;  //1
	CBase_Tab_BW *m_pTabBW;  //2
	CBase_Tab_Preview *m_pTabPreview;  //3
	CBase_Tab_Rotateshow *m_pTabRotateshow;  //4
	CBase_Tab_Spiltshow *m_pTabSpiltshow;  //5
	CBase_AutoColor *m_pTabAutoColor;  //6

	CComboBox m_combo_joinimage;
	CComboBox m_combo_backprocess;
	CComboBox m_combo_backpromode;
	CEdit m_edit_backprostrenth;
	CSliderCtrl m_slider_backprostrenth;
	afx_msg void OnCbnSelchangeBase_Combo_Joinimg();
	afx_msg void OnCbnSelchangeBase_Combo_Backprocess();
	afx_msg void OnCbnSelchangeBase_Combo_BackproMode();
	afx_msg void OnNMCustomdrawBase_Slider_BackkproStrength(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeBase_Edit_BackkproStrength();
};

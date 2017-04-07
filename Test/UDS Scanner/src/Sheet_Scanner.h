#pragma once

#include "MFC_UI.h"
#include "Page_Profile.h"
#include "Page_Base.h"
#include "Page_Advanced.h"
#include "Page_About.h"
#include "Page_Paper.h"
#include "Page_Set.h"
#include "Page_Info.h"
#include "Page_Filter.h"
#include "Base_AutoColor.h"
#include "Base_Tab_BW.h"
#include "Base_Tab_Color.h"
#include "Base_Tab_Gray.h"

class MFC_UI;
// CSheet_Scanner
class CSheet_Scanner : public CPropertySheet
{
	DECLARE_DYNAMIC(CSheet_Scanner)

public:
	///< ���ع��캯��������β�MFC_UI *pUI
	CSheet_Scanner(MFC_UI *pUI, UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CSheet_Scanner(MFC_UI *pUI, LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CSheet_Scanner();

protected:
	DECLARE_MESSAGE_MAP()

private:
	MFC_UI *m_pUI;

public:
	CPage_Profile  *m_p_page_profile;
	CPage_Base     *m_p_page_base;
	CPage_Advanced *m_p_page_advanced;
	CPage_About    *m_p_page_about;
	CPage_Paper    *m_p_page_paper;
	CPage_Set      *m_p_page_set;
	CPage_Info     *m_p_page_info;
	CPage_Filter   *m_p_page_filter;

	CBase_AutoColor *m_p_baseTab_autocolor;
	CBase_Tab_BW    *m_p_baseTab_bw;
	CBase_Tab_Color *m_p_baseTab_color;
	CBase_Tab_Gray  *m_p_baseTab_gray;
	
	CButton m_btn_help; //����ҳ�ϡ���������ť
	CButton m_btn_preview; //����ҳ�ϡ�Ԥ������ť

public:
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();

	afx_msg void OnButtonHelp();  //add by zhu
	virtual afx_msg void OnButtonPreView();

	void SetPreViewStatus();
};



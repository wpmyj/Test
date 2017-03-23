#pragma once


#include "MFC_UI.h"
#include "Page_Profile.h"
#include "Page_Base.h"
#include "Page_Advanced.h"
#include "Page_About.h"
#include "Page_Paper.h"
#include "Page_Set.h"
#include "Page_Info.h"

//#include "Page_Muiltstream.h"
//#include "Page_ImageProcess.h"

class MFC_UI;
// CSheet_Scanner
class CSheet_Scanner : public CPropertySheet
{
	DECLARE_DYNAMIC(CSheet_Scanner)

public:
	///< 重载构造函数，添加形参MFC_UI *pUI
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
	//CPage_Muiltstream *m_p_page_muilstream;
	//CPage_ImageProcess *m_p_page_imageprocess;

	CButton m_btn_help; //属性页上“帮助”按钮
	CButton m_btn_preview; //属性页上“预览”按钮

public:
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();

	afx_msg void OnButtonHelp();  //add by zhu
	virtual afx_msg void OnButtonPreView();

	void SetPreViewStatus();
};



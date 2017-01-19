// Sheet_Scanner.cpp : 实现文件
//

#include "stdafx.h"
#include "UDS Scanner.h"
#include "Sheet_Scanner.h"

// CSheet_Scanner

IMPLEMENT_DYNAMIC(CSheet_Scanner, CPropertySheet)

CSheet_Scanner::CSheet_Scanner(MFC_UI* pUI, UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:m_pUI(pUI),CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	
	// 在属性表单中添加属性页,添加顺序就是显示的顺序
	m_p_page_base = new CPage_Base(pUI);
	AddPage(m_p_page_base);

	m_p_page_advanced = new CPage_Advanced(pUI);
	AddPage(m_p_page_advanced);

	//m_p_page_paper = new CPage_Paper(pUI);
	//AddPage(m_p_page_paper);

	//m_p_page_muilstream = new CPage_Muiltstream(pUI);
	//AddPage(m_p_page_muilstream);

	//m_p_page_imageprocess = new CPage_ImageProcess(pUI);
	//AddPage(m_p_page_imageprocess);

	AddPage(&m_page_about);

	m_p_page_base->m_pAdPage = m_p_page_advanced;
	m_p_page_advanced->m_pBasePage = m_p_page_base;
}

CSheet_Scanner::CSheet_Scanner(MFC_UI* pUI, LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:m_pUI(pUI),CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{

	// 在属性表单中添加属性页,添加顺序就是显示的顺序
	m_p_page_base = new CPage_Base(pUI);
	AddPage(m_p_page_base);

	m_p_page_advanced = new CPage_Advanced(pUI);
	AddPage(m_p_page_advanced);

	//m_p_page_paper = new CPage_Paper(pUI);
	//AddPage(m_p_page_paper);

	//m_p_page_muilstream = new CPage_Muiltstream(pUI);
	//AddPage(m_p_page_muilstream);

	//m_p_page_imageprocess = new CPage_ImageProcess(pUI);
	//AddPage(m_p_page_imageprocess);

	AddPage(&m_page_about);
	
	m_p_page_base->m_pAdPage = m_p_page_advanced;
	m_p_page_advanced->m_pBasePage = m_p_page_base;
}

CSheet_Scanner::~CSheet_Scanner()
{
	if (m_p_page_base)
	{
		delete m_p_page_base;
		m_p_page_base = NULL;
	}

	if (m_p_page_advanced)
	{
		delete m_p_page_advanced;
		m_p_page_advanced = NULL;
	}

	//if (m_p_page_paper)
	//{
	//	delete m_p_page_paper;
	//	m_p_page_paper = NULL;
	//}

	//if (m_p_page_muilstream)
	//{
	//	delete m_p_page_muilstream;
	//	m_p_page_muilstream = NULL;
	//}

	//if (m_p_page_imageprocess)
	//{
	//	delete m_p_page_imageprocess;
	//	m_p_page_imageprocess = NULL;
	//}

}


BEGIN_MESSAGE_MAP(CSheet_Scanner, CPropertySheet)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CSheet_Scanner 消息处理程序


void CSheet_Scanner::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	/*AfxMessageBox("Close");
	Sleep(3000);
	DestroyUI(m_pUI);
	CPropertySheet::OnClose();
	m_pUI->Cancel();*/
	//return;
}


void CSheet_Scanner::OnDestroy()
{
	CPropertySheet::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	// 窗口销毁前工作
	//m_pUI->Cancel();
	
}


BOOL CSheet_Scanner::OnInitDialog()
{
	BOOL bResult = CPropertySheet::OnInitDialog();

	// TODO:  在此添加您的专用代码
	SetActivePage(m_p_page_advanced);
	SetActivePage(m_p_page_base);
	if(m_pUI->m_bSetup)  // EnableDSUIOnly（只打开驱动界面），更改按钮名称为“保存”
	{
		GetDlgItem(IDOK)->SetWindowText("保存");
	}
	else
	{
		GetDlgItem(IDOK)->SetWindowText("扫描");
	}

	////暂时移除后三个界面
	//RemovePage(m_p_page_paper);
	//RemovePage(m_p_page_muilstream);
	//RemovePage(m_p_page_imageprocess);

	GetDlgItem(ID_APPLY_NOW)->ShowWindow(FALSE); //隐藏应用按钮 （标准按钮的ID是IDOK，IDCANCEL，IDHELP和ID_APPLY_NOW） 

	//GetDlgItem(ID_APPLY_NOW)->EnableWindow(TRUE);
	//GetDlgItem(ID_APPLY_NOW)->SetWindowText("帮助");//把“应用”改为“帮助”

	return bResult;
}

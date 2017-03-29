// Sheet_Scanner.cpp : 实现文件
//

#include "stdafx.h"
#include "UDS Scanner.h"
#include "Sheet_Scanner.h"

#define FILENAME_CHM_DS  TEXT("UDS Universal TWAIN DS使用手册.chm")
// CSheet_Scanner
extern HINSTANCE  g_hinstance;
extern void GetFilePath( char* szFileName, char* szFilePath);

IMPLEMENT_DYNAMIC(CSheet_Scanner, CPropertySheet)

CSheet_Scanner::CSheet_Scanner(MFC_UI* pUI, UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:m_pUI(pUI),CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	// 在属性表单中添加属性页,添加顺序就是显示的顺序
	m_p_page_profile = new CPage_Profile(pUI);
	AddPage(m_p_page_profile);

	m_p_page_base = new CPage_Base(pUI);
	AddPage(m_p_page_base);

	m_p_page_paper = new CPage_Paper(pUI);
	AddPage(m_p_page_paper);

	m_p_page_advanced = new CPage_Advanced(pUI);
	AddPage(m_p_page_advanced);

	m_p_page_set = new CPage_Set(pUI);
	AddPage(m_p_page_set);

	m_p_page_info = new CPage_Info(pUI);
	AddPage(m_p_page_info);

	//m_p_page_muilstream = new CPage_Muiltstream(pUI);
	//AddPage(m_p_page_muilstream);

	//m_p_page_imageprocess = new CPage_ImageProcess(pUI);
	//AddPage(m_p_page_imageprocess);
	m_p_page_about = new CPage_About(pUI);
	AddPage(m_p_page_about);

	m_p_page_base->m_pAdPage = m_p_page_advanced; //用于基本与高级之间参数同步
	m_p_page_base->m_pPaperPage = m_p_page_paper;

	m_p_page_advanced->m_pBasePage = m_p_page_base;
	
	m_p_page_profile->m_pBasePage = m_p_page_base;
	m_p_page_profile->m_pAdPage = m_p_page_advanced;
	m_p_page_profile->m_pPaperPage = m_p_page_paper;
	m_p_page_profile->m_pSetPage = m_p_page_set;
}

CSheet_Scanner::CSheet_Scanner(MFC_UI* pUI, LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:m_pUI(pUI),CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{

	// 在属性表单中添加属性页,添加顺序就是显示的顺序
	m_p_page_profile = new CPage_Profile(pUI);
	AddPage(m_p_page_profile);

	m_p_page_base = new CPage_Base(pUI);
	AddPage(m_p_page_base);

	m_p_page_paper = new CPage_Paper(pUI);
	AddPage(m_p_page_paper);

	m_p_page_advanced = new CPage_Advanced(pUI);
	AddPage(m_p_page_advanced);

	m_p_page_set = new CPage_Set(pUI);
	AddPage(m_p_page_set);

	m_p_page_info = new CPage_Info(pUI);
	AddPage(m_p_page_info);


	//m_p_page_muilstream = new CPage_Muiltstream(pUI);
	//AddPage(m_p_page_muilstream);

	//m_p_page_imageprocess = new CPage_ImageProcess(pUI);
	//AddPage(m_p_page_imageprocess);

	m_p_page_about = new CPage_About(pUI);
	AddPage(m_p_page_about);
	
	m_p_page_base->m_pAdPage = m_p_page_advanced;
	m_p_page_base->m_pPaperPage = m_p_page_paper;

	m_p_page_advanced->m_pBasePage = m_p_page_base;

	m_p_page_profile->m_pBasePage = m_p_page_base;
	m_p_page_profile->m_pAdPage = m_p_page_advanced;
	m_p_page_profile->m_pPaperPage = m_p_page_paper;
	m_p_page_profile->m_pSetPage = m_p_page_set;
}

CSheet_Scanner::~CSheet_Scanner()
{
	if (m_p_page_profile)
	{
		delete m_p_page_profile;
		m_p_page_profile = NULL;
	}

	if (m_p_page_base)
	{
		delete m_p_page_base;
		m_p_page_base = NULL;
	}

	if (m_p_page_paper)
	{
		delete m_p_page_paper;
		m_p_page_paper = NULL;
	}

	if (m_p_page_advanced)
	{
		delete m_p_page_advanced;
		m_p_page_advanced = NULL;
	}

	if (m_p_page_set)
	{
		delete m_p_page_set;
		m_p_page_set = NULL;
	}

	if (m_p_page_info)
	{
		delete m_p_page_info;
		m_p_page_info = NULL;
	}

	if (m_p_page_about)
	{
		delete m_p_page_about;
		m_p_page_about = NULL;
	}



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
	ON_BN_CLICKED(IDC_SHEET_BUTTON_HELP, &CSheet_Scanner::OnButtonHelp) //add by zhu
	ON_BN_CLICKED(IDC_SHEET_BUTTON_PREVIEW, &CSheet_Scanner::OnButtonPreView) 
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
	this->SetWindowText(MB_CAPTION);
	// TODO:  在此添加您的专用代码	
	SetActivePage(m_p_page_advanced);
	SetActivePage(m_p_page_base);
	SetActivePage(m_p_page_profile);

	if(m_pUI->m_bSetup)  // EnableDSUIOnly（只打开驱动界面），更改按钮名称为“保存”
	{
		GetDlgItem(IDOK)->SetWindowText("保存");
	}
	else
	{
		GetDlgItem(IDOK)->SetWindowText("扫描");
	}

	GetDlgItem(ID_APPLY_NOW)->ShowWindow(FALSE); //隐藏应用按钮 （标准按钮的ID是IDOK，IDCANCEL，IDHELP和ID_APPLY_NOW） 

	//GetDlgItem(ID_APPLY_NOW)->EnableWindow(TRUE);
	//GetDlgItem(ID_APPLY_NOW)->SetWindowText("帮助");//把“应用”改为“帮助”

	//在属性页增加帮助按钮
	CRect rect, tabrect, rect_cancel;
	int width, stepwidth;
	GetDlgItem(IDOK)->GetWindowRect(rect); //得到按钮的尺寸
	GetTabControl()->GetWindowRect(tabrect);  //新按钮的位置
	GetDlgItem(IDCANCEL)->GetWindowRect(rect_cancel); 
	ScreenToClient(rect); //Screen(屏幕坐标) 到 Client(客户区坐标)的转换
	ScreenToClient(tabrect);
	ScreenToClient(rect_cancel);

	width = rect.Width();
	stepwidth = rect_cancel.left - rect.right; //确定与取消按钮之间的距离
	
	rect.left = tabrect.left;
	rect.right = tabrect.left + width;
	m_btn_help.Create("帮助",BS_PUSHBUTTON|WS_CHILD|WS_VISIBLE|WS_TABSTOP, rect, this, IDC_SHEET_BUTTON_HELP);
	m_btn_help.SetFont(GetFont());
	
	rect.left = rect.right + stepwidth;
	rect.right = rect.left + width;
	m_btn_preview.Create("预览",BS_PUSHBUTTON|WS_CHILD|WS_VISIBLE|WS_TABSTOP, rect, this, IDC_SHEET_BUTTON_PREVIEW);
	m_btn_preview.SetFont(GetFont()); 
	
	SetPreViewStatus();

	return bResult;
}


void CSheet_Scanner::SetPreViewStatus()
{
	if(1 == GetTabControl()->GetCurFocus() || 2 == GetTabControl()->GetCurFocus()) //基本与纸张界面预览可用
	{
		m_btn_preview.EnableWindow(TRUE);
	}
	else
	{
		m_btn_preview.EnableWindow(FALSE);
	}
}


void CSheet_Scanner::OnButtonHelp()
{
	TCHAR szPath[MAX_PATH];
	memset(szPath, 0, sizeof(szPath));
	GetFilePath(FILENAME_CHM_DS, szPath);
	ShellExecute(NULL,TEXT("open"),szPath,NULL,NULL,SW_SHOWNORMAL);
}


void CSheet_Scanner::OnButtonPreView() 
{
	if(1 == GetTabControl()->GetCurFocus()) 
	{
		m_p_page_base->PreView();
	}
	else if(2 == GetTabControl()->GetCurFocus())
	{
		m_p_page_paper->PreView();
	}
	else{}
}

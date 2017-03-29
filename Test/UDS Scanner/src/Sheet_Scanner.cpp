// Sheet_Scanner.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "UDS Scanner.h"
#include "Sheet_Scanner.h"

#define FILENAME_CHM_DS  TEXT("UDS Universal TWAIN DSʹ���ֲ�.chm")
// CSheet_Scanner
extern HINSTANCE  g_hinstance;
extern void GetFilePath( char* szFileName, char* szFilePath);

IMPLEMENT_DYNAMIC(CSheet_Scanner, CPropertySheet)

CSheet_Scanner::CSheet_Scanner(MFC_UI* pUI, UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:m_pUI(pUI),CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	// �����Ա����������ҳ,���˳�������ʾ��˳��
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

	m_p_page_base->m_pAdPage = m_p_page_advanced; //���ڻ�����߼�֮�����ͬ��
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

	// �����Ա����������ҳ,���˳�������ʾ��˳��
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


// CSheet_Scanner ��Ϣ�������


void CSheet_Scanner::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
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

	// TODO: �ڴ˴������Ϣ����������
	// ��������ǰ����
	//m_pUI->Cancel();
	
}


BOOL CSheet_Scanner::OnInitDialog()
{
	BOOL bResult = CPropertySheet::OnInitDialog();
	this->SetWindowText(MB_CAPTION);
	// TODO:  �ڴ��������ר�ô���	
	SetActivePage(m_p_page_advanced);
	SetActivePage(m_p_page_base);
	SetActivePage(m_p_page_profile);

	if(m_pUI->m_bSetup)  // EnableDSUIOnly��ֻ���������棩�����İ�ť����Ϊ�����桱
	{
		GetDlgItem(IDOK)->SetWindowText("����");
	}
	else
	{
		GetDlgItem(IDOK)->SetWindowText("ɨ��");
	}

	GetDlgItem(ID_APPLY_NOW)->ShowWindow(FALSE); //����Ӧ�ð�ť ����׼��ť��ID��IDOK��IDCANCEL��IDHELP��ID_APPLY_NOW�� 

	//GetDlgItem(ID_APPLY_NOW)->EnableWindow(TRUE);
	//GetDlgItem(ID_APPLY_NOW)->SetWindowText("����");//�ѡ�Ӧ�á���Ϊ��������

	//������ҳ���Ӱ�����ť
	CRect rect, tabrect, rect_cancel;
	int width, stepwidth;
	GetDlgItem(IDOK)->GetWindowRect(rect); //�õ���ť�ĳߴ�
	GetTabControl()->GetWindowRect(tabrect);  //�°�ť��λ��
	GetDlgItem(IDCANCEL)->GetWindowRect(rect_cancel); 
	ScreenToClient(rect); //Screen(��Ļ����) �� Client(�ͻ�������)��ת��
	ScreenToClient(tabrect);
	ScreenToClient(rect_cancel);

	width = rect.Width();
	stepwidth = rect_cancel.left - rect.right; //ȷ����ȡ����ť֮��ľ���
	
	rect.left = tabrect.left;
	rect.right = tabrect.left + width;
	m_btn_help.Create("����",BS_PUSHBUTTON|WS_CHILD|WS_VISIBLE|WS_TABSTOP, rect, this, IDC_SHEET_BUTTON_HELP);
	m_btn_help.SetFont(GetFont());
	
	rect.left = rect.right + stepwidth;
	rect.right = rect.left + width;
	m_btn_preview.Create("Ԥ��",BS_PUSHBUTTON|WS_CHILD|WS_VISIBLE|WS_TABSTOP, rect, this, IDC_SHEET_BUTTON_PREVIEW);
	m_btn_preview.SetFont(GetFont()); 
	
	SetPreViewStatus();

	return bResult;
}


void CSheet_Scanner::SetPreViewStatus()
{
	if(1 == GetTabControl()->GetCurFocus() || 2 == GetTabControl()->GetCurFocus()) //������ֽ�Ž���Ԥ������
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

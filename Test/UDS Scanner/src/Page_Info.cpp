// Page_Info.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "UDS Scanner.h"
#include "Page_Info.h"
#include "afxdialogex.h"


// CPage_Info �Ի���

IMPLEMENT_DYNAMIC(CPage_Info, CPropertyPage)

CPage_Info::CPage_Info(MFC_UI *pUI)
	: m_pUI(pUI), CPropertyPage(CPage_Info::IDD)
{

}

CPage_Info::~CPage_Info()
{
}

void CPage_Info::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);	
	DDX_Control(pDX, IDC_INFO_LIST_INFORMATION, m_list_infomation);
}


BEGIN_MESSAGE_MAP(CPage_Info, CPropertyPage)
	ON_LBN_SELCHANGE(IDC_INFO_LIST_INFORMATION, &CPage_Info::OnLbnSelchangeInfo_List_Information)
	ON_BN_CLICKED(IDC_INFO_BTN_REPORT, &CPage_Info::OnInfo_Btn_Report)
	ON_BN_CLICKED(IDC_INFO_BUTTON_CLEARDICIDE, &CPage_Info::OnInfo_Btn_ClearDicideNum)
	ON_BN_CLICKED(IDC_INFO_BUTTON_CLEARFEED, &CPage_Info::OnInfo_Btn_ClearFeedNum)
END_MESSAGE_MAP()


// CPage_Info ��Ϣ�������


void CPage_Info::OnLbnSelchangeInfo_List_Information()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CPage_Info::OnInfo_Btn_Report()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

void CPage_Info::OnInfo_Btn_ClearDicideNum()
{

}

void CPage_Info::OnInfo_Btn_ClearFeedNum()
{

}

BOOL CPage_Info::OnInitDialog()
{
	__super::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	
	CRect rect(0,0,120,25);

	DWORD dwStyle = BS_PUSHBUTTON|WS_CHILD|WS_VISIBLE|WS_TABSTOP; 
	m_btn_cleardicidenum.Create("��ֽ�ּ�������", dwStyle, rect, this, IDC_INFO_BUTTON_CLEARDICIDE);
	m_btn_cleardicidenum.SetFont(GetFont());
	
	rect.left = rect.right + 10;
	rect.right = rect.left + 120;

	m_btn_clearfeednum.Create("��ֽ�ּ�������", dwStyle, rect, this, IDC_INFO_BUTTON_CLEARFEED);
	m_btn_clearfeednum.SetFont(GetFont());
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


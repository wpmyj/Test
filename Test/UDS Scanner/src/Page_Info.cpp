// Page_Info.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "UDS Scanner.h"
#include "Page_Info.h"
#include "afxdialogex.h"
#include "ComputerInfo.h"

#pragma comment(lib, "ComputerInfo.lib")
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
	//  DDX_Control(pDX, IDC_INFO_LIST_INFORMATION, m_list_infomation);
	DDX_Control(pDX, IDC_INFO_LIST_INFORMATION, m_list_infomation);
}


BEGIN_MESSAGE_MAP(CPage_Info, CPropertyPage)
//	ON_LBN_SELCHANGE(IDC_INFO_LIST_INFORMATION, &CPage_Info::OnLbnSelchangeInfo_List_Information)
	ON_BN_CLICKED(IDC_INFO_BTN_REPORT, &CPage_Info::OnInfo_Btn_Report)
	ON_BN_CLICKED(IDC_INFO_BUTTON_CLEARDICIDE, &CPage_Info::OnInfo_Btn_ClearDicideNum)
	ON_BN_CLICKED(IDC_INFO_BUTTON_CLEARFEED, &CPage_Info::OnInfo_Btn_ClearFeedNum)
END_MESSAGE_MAP()


// CPage_Info ��Ϣ�������


//void CPage_Info::OnLbnSelchangeInfo_List_Information()
//{
//	// TODO: �ڴ���ӿؼ�֪ͨ����������
//}


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
	
	GetComputerInfo();
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CPage_Info::GetComputerInfo()
{
	m_list_infomation.SetExtendedStyle( LVS_EX_FULLROWSELECT); // ��ʾѡ�����У�����ĳһ��
	//������չ���Ϊ��������ʽ������ѡȡ
	//m_list_infomation.DeleteAllItems(); // ���
	CRect rect;   
	m_list_infomation.GetClientRect(rect);

	m_list_infomation.InsertColumn(0, "", LVCFMT_LEFT, rect.Width()/3);
	m_list_infomation.InsertColumn(1, "", LVCFMT_LEFT, rect.Width() - rect.Width()/3);
	//m_list_infomation.SetRedraw(FALSE); // ��ֹ�ػ�

	STRING str1(TEXT("���봦������"));  // ��1��
	STRING str2(TEXT(""));            // ��2��
	// ��ȡ���봦������Ϣ
	GetCPUInfo(str2);
	int nIndex = 0;
	m_list_infomation.InsertItem(nIndex, str1.c_str());  // ��1��
	m_list_infomation.SetItemText(nIndex++, 1, str2.c_str()); // ��2��

	// ѭ����ȡӲ����Ϣ
	int nCount = 0;
	GetFixedDiskInfo(str2, nCount);
	int before, behind;  // �ֱ𱣴����ڵ�����';'��λ��
	before = behind = 0;
	STRING strSub(TEXT(""));
	for(int i = 0; i < nCount; i++)
	{
		str1 = TEXT("Ӳ�̣�");
		behind = str2.find(';', before); // ��ȡ';'��λ��,��ǰһ��';'���λ�ÿ�ʼ��
		strSub = str2.substr(before, behind-before); // ȡ����';'֮����ַ���
		before = behind + 1;
		m_list_infomation.InsertItem(nIndex, str1.c_str());
		m_list_infomation.SetItemText(nIndex++, 1, strSub.c_str()); // ��2��
	}

	//��ȡ�ڴ���Ϣ
	str1 = TEXT("�ڴ棺");
	GetMemoryInfo(str2);
	m_list_infomation.InsertItem(nIndex, str1.c_str());
	m_list_infomation.SetItemText(nIndex++, 1, str2.c_str()); // ��2��

	// ��ȡ����ϵͳ��Ϣ
	str1 = TEXT("����ϵͳ��");
	GetWindowsVersion(str2);
	m_list_infomation.InsertItem(nIndex, str1.c_str());
	m_list_infomation.SetItemText(nIndex++, 1, str2.c_str()); // ��2��

	// ��ȡ����ϵͳ����
	str1 = TEXT("����ϵͳ���ԣ�");
	GetUserDefaultLanguage(str2);
	m_list_infomation.InsertItem(nIndex, str1.c_str());
	m_list_infomation.SetItemText(nIndex++, 1, str2.c_str()); // ��2��

}


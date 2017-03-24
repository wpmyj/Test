// Page_Info.cpp : 实现文件
//

#include "stdafx.h"
#include "UDS Scanner.h"
#include "Page_Info.h"
#include "afxdialogex.h"
#include "ComputerInfo.h"

#pragma comment(lib, "ComputerInfo.lib")
// CPage_Info 对话框

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


// CPage_Info 消息处理程序


//void CPage_Info::OnLbnSelchangeInfo_List_Information()
//{
//	// TODO: 在此添加控件通知处理程序代码
//}


void CPage_Info::OnInfo_Btn_Report()
{
	// TODO: 在此添加控件通知处理程序代码
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

	// TODO:  在此添加额外的初始化
	
	CRect rect(0,0,120,25);

	DWORD dwStyle = BS_PUSHBUTTON|WS_CHILD|WS_VISIBLE|WS_TABSTOP; 
	m_btn_cleardicidenum.Create("分纸轮计数清零", dwStyle, rect, this, IDC_INFO_BUTTON_CLEARDICIDE);
	m_btn_cleardicidenum.SetFont(GetFont());
	
	rect.left = rect.right + 10;
	rect.right = rect.left + 120;

	m_btn_clearfeednum.Create("进纸轮计数清零", dwStyle, rect, this, IDC_INFO_BUTTON_CLEARFEED);
	m_btn_clearfeednum.SetFont(GetFont());
	
	GetComputerInfo();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CPage_Info::GetComputerInfo()
{
	m_list_infomation.SetExtendedStyle( LVS_EX_FULLROWSELECT); // 表示选中整行，而非某一列
	//设置扩展风格为：网格形式、整行选取
	//m_list_infomation.DeleteAllItems(); // 清空
	CRect rect;   
	m_list_infomation.GetClientRect(rect);

	m_list_infomation.InsertColumn(0, "", LVCFMT_LEFT, rect.Width()/3);
	m_list_infomation.InsertColumn(1, "", LVCFMT_LEFT, rect.Width() - rect.Width()/3);
	//m_list_infomation.SetRedraw(FALSE); // 防止重绘

	STRING str1(TEXT("中央处理器："));  // 第1列
	STRING str2(TEXT(""));            // 第2列
	// 获取中央处理器信息
	GetCPUInfo(str2);
	int nIndex = 0;
	m_list_infomation.InsertItem(nIndex, str1.c_str());  // 第1列
	m_list_infomation.SetItemText(nIndex++, 1, str2.c_str()); // 第2列

	// 循环获取硬盘信息
	int nCount = 0;
	GetFixedDiskInfo(str2, nCount);
	int before, behind;  // 分别保存相邻的两个';'的位置
	before = behind = 0;
	STRING strSub(TEXT(""));
	for(int i = 0; i < nCount; i++)
	{
		str1 = TEXT("硬盘：");
		behind = str2.find(';', before); // 获取';'的位置,从前一个';'后的位置开始找
		strSub = str2.substr(before, behind-before); // 取两个';'之间的字符串
		before = behind + 1;
		m_list_infomation.InsertItem(nIndex, str1.c_str());
		m_list_infomation.SetItemText(nIndex++, 1, strSub.c_str()); // 第2列
	}

	//获取内存信息
	str1 = TEXT("内存：");
	GetMemoryInfo(str2);
	m_list_infomation.InsertItem(nIndex, str1.c_str());
	m_list_infomation.SetItemText(nIndex++, 1, str2.c_str()); // 第2列

	// 获取操作系统信息
	str1 = TEXT("操作系统：");
	GetWindowsVersion(str2);
	m_list_infomation.InsertItem(nIndex, str1.c_str());
	m_list_infomation.SetItemText(nIndex++, 1, str2.c_str()); // 第2列

	// 获取操作系统语言
	str1 = TEXT("操作系统语言：");
	GetUserDefaultLanguage(str2);
	m_list_infomation.InsertItem(nIndex, str1.c_str());
	m_list_infomation.SetItemText(nIndex++, 1, str2.c_str()); // 第2列

}


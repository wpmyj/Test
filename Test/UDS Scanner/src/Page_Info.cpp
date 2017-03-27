// Page_Info.cpp : 实现文件
//

#include "stdafx.h"
#include "UDS Scanner.h"
#include "Page_Info.h"
#include "afxdialogex.h"
#include "ComputerInfo.h"
//#include <tchar.h>

#pragma comment(lib, "ComputerInfo.lib")

extern HWND g_hwndDLG;
extern int g_nDeviceNumber;
extern void GetFilePath( char* szFileName, char* szFilePath);
// CPage_Info 对话框

IMPLEMENT_DYNAMIC(CPage_Info, CPropertyPage)

CPage_Info::CPage_Info(MFC_UI *pUI)
	: m_pUI(pUI), CPropertyPage(CPage_Info::IDD)
	, m_hDLL(NULL)
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
	
	ShowComputerAndScannerInfo();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CPage_Info::ShowComputerAndScannerInfo()
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


	// 获取操作系统语言
	str1 = TEXT("应用程序名称：");
	{
		pTW_IDENTITY app = m_pUI->m_pDS->getApp();
		TCHAR appBuffer[1024];
		memset(appBuffer, 0, sizeof(appBuffer));
		_stprintf_s(appBuffer, "%s.%u.%u %s (%u.%u) %s", app->ProductName, 
			app->Version.MajorNum, app->Version.MinorNum, app->Manufacturer,						 
			app->ProtocolMajor, app->ProtocolMinor, app->Version.Info );
		str2 = appBuffer;
	}
	m_list_infomation.InsertItem(nIndex, str1.c_str());
	m_list_infomation.SetItemText(nIndex++, 1, str2.c_str()); // 第2列


	if (false == LoadDLL())
	{
		return;
	}

	NVMDATA data;
	ReadNVMData(&data);

	// 添加空行
	str1 = str2= TEXT("");
	m_list_infomation.InsertItem(nIndex, str1.c_str());
	m_list_infomation.SetItemText(nIndex++, 1, str2.c_str()); // 第2列

	TCHAR  buffer[MAX_PATH];
	// 获取扫描仪序列号
	str1 = TEXT("扫描仪序列号：");
	memset(buffer, 0, sizeof(buffer));
	_stprintf_s(buffer, "%s", data.SN);
	str2 = buffer;
	m_list_infomation.InsertItem(nIndex, str1.c_str());
	m_list_infomation.SetItemText(nIndex++, 1, str2.c_str()); // 第2列

	// 获取版本日期
	str1 = TEXT("版本日期：");
	memset(buffer, 0, sizeof(buffer));
	_stprintf_s(buffer, "%u/%u/%u", data.BornYear, data.BornMonth, data.BornDay);
	str2 = buffer;
	m_list_infomation.InsertItem(nIndex, str1.c_str());
	m_list_infomation.SetItemText(nIndex++, 1, str2.c_str()); // 第2列

	// 获取更新日期
	str1 = TEXT("更新日期：");
	memset(buffer, 0, sizeof(buffer));
	_stprintf_s(buffer, "%u/%u/%u", data.FirstScanYear, data.FirstScanMonth, data.FirstScanDay);
	str2 = buffer;
	m_list_infomation.InsertItem(nIndex, str1.c_str());
	m_list_infomation.SetItemText(nIndex++, 1, str2.c_str()); // 第2列

	// 获取耗材分纸轮计数
	str1 = TEXT("耗材分纸轮计数：");
	memset(buffer, 0, sizeof(buffer));
	_stprintf_s(buffer, "%lu", data.PadScanCount);
	str2 = buffer;
	m_list_infomation.InsertItem(nIndex, str1.c_str());
	m_list_infomation.SetItemText(nIndex++, 1, str2.c_str()); // 第2列

	// 获取耗材进纸轮计数
	str1 = TEXT("耗材进纸轮计数：");
	memset(buffer, 0, sizeof(buffer));
	_stprintf_s(buffer, "%lu", data.RollerCount);
	str2 = buffer;
	m_list_infomation.InsertItem(nIndex, str1.c_str());
	m_list_infomation.SetItemText(nIndex++, 1, str2.c_str()); // 第2列

	// 获取单面扫描次数
	str1 = TEXT("单面扫描次数：");
	memset(buffer, 0, sizeof(buffer));
	_stprintf_s(buffer, "%lu", data.ADFSingleModeScanCount);
	str2 = buffer;
	m_list_infomation.InsertItem(nIndex, str1.c_str());
	m_list_infomation.SetItemText(nIndex++, 1, str2.c_str()); // 第2列

	// 获取单面扫描次数
	str1 = TEXT("双面扫描次数：");
	memset(buffer, 0, sizeof(buffer));
	_stprintf_s(buffer, "%lu", data.ADFDuplexModeScanCount);
	str2 = buffer;
	m_list_infomation.InsertItem(nIndex, str1.c_str());
	m_list_infomation.SetItemText(nIndex++, 1, str2.c_str()); // 第2列

	// 获取重叠进纸次数
	str1 = TEXT("重叠进纸次数：");
	memset(buffer, 0, sizeof(buffer));
	_stprintf_s(buffer, "%lu", data.MultifeedCount);
	str2 = buffer;
	m_list_infomation.InsertItem(nIndex, str1.c_str());
	m_list_infomation.SetItemText(nIndex++, 1, str2.c_str()); // 第2列

	// 获取卡纸次数
	str1 = TEXT("卡纸次数：");
	memset(buffer, 0, sizeof(buffer));
	_stprintf_s(buffer, "%lu", data.JamCount);
	str2 = buffer;
	m_list_infomation.InsertItem(nIndex, str1.c_str());
	m_list_infomation.SetItemText(nIndex++, 1, str2.c_str()); // 第2列

	TerminateDriver();
	FreeLibrary(m_hDLL);
}

bool CPage_Info::LoadDLL()
{
	//::MessageBox(g_hwndDLG,TEXT("Into LoadDLL!"),MB_CAPTION,MB_OK);
	TCHAR DllPath[MAX_PATH];

	if (DEVICE_G6400 == g_nDeviceNumber)
	{
		GetFilePath(FILENAME_DLL_GL1, DllPath);
	} 
	else if(DEVICE_G6600 == g_nDeviceNumber)
	{
		GetFilePath(FILENAME_DLL_GL2, DllPath);
	}
	else
	{
		return false;
	}

	//::MessageBox(g_hwndDLG, DllPath, MB_CAPTION,MB_OK);

	m_hDLL = LoadLibrary(DllPath);
	if(m_hDLL == NULL)
	{
		::MessageBox(g_hwndDLG, TEXT("Load Dll Failed!"), MB_CAPTION, MB_OK);
		return false;
	}

	InitializeDriver = (InitializeDriverProc)GetProcAddress(m_hDLL, "InitializeDriver");
	if(InitializeDriver == NULL)
	{
		::MessageBox(g_hwndDLG, TEXT("Load InitializeDriver Failed!"), MB_CAPTION, MB_OK);
		return false;
	}

	InitializeScanner = (InitializeScannerProc)GetProcAddress(m_hDLL, "InitializeScanner");
	if(InitializeScanner == NULL)
	{
		::MessageBox(g_hwndDLG, TEXT("Load InitializeScanner Failed!"), MB_CAPTION, MB_OK);
		return false;
	}

	//GetScannerAbility = (GetScannerAbilityProc)GetProcAddress(m_hDLL, "GetScannerAbility");
	//if(GetScannerAbility == NULL)
	//{
	//	::MessageBox(g_hwndDLG, TEXT("Load GetScannerAbility Failed!"), MB_CAPTION, MB_OK);
	//	return false;
	//}

	//GetScannerAbilityEx = (GetScannerAbilityExProc)GetProcAddress(m_hDLL, "GetScannerAbilityEx");
	//if(GetScannerAbilityEx == NULL)
	//{
	//	::MessageBox(g_hwndDLG, TEXT("Load GetScannerAbilityEx Failed!"), MB_CAPTION, MB_OK);
	//	return false;
	//}

	//GetADFStatus = (GetADFStatusProc)GetProcAddress(m_hDLL, "GetADFStatus");
	//if(GetADFStatus == NULL)
	//{
	//	::MessageBox(g_hwndDLG, TEXT("Load GetADFStatus Failed!"), MB_CAPTION, MB_OK);
	//	return false;
	//}

	//StartScanJob = (StartScanJobProc)GetProcAddress(m_hDLL, "StartScanJob");
	//if(StartScanJob == NULL)
	//{
	//	::MessageBox(g_hwndDLG, TEXT("Load StartScanJob Failed!"), MB_CAPTION, MB_OK);
	//	return false;
	//}

	//SetScanParameter = (SetScanParameterProc)GetProcAddress(m_hDLL, "SetScanParameter");
	//if(SetScanParameter == NULL)
	//{
	//	::MessageBox(g_hwndDLG, TEXT("Load SetScanParameter Failed!"), MB_CAPTION, MB_OK);
	//	return false;
	//}

	//SetGammaTable = (SetGammaTableProc)GetProcAddress(m_hDLL, "SetGammaTable");
	//if(SetGammaTable == NULL)
	//{
	//	::MessageBox(g_hwndDLG, TEXT("Load SetGammaTable Failed!"), MB_CAPTION, MB_OK);
	//	return false;
	//}

	//StartScan = (StartScanProc)GetProcAddress(m_hDLL, "StartScan");
	//if(StartScan == NULL)
	//{
	//	::MessageBox(g_hwndDLG, TEXT("Load StartScan Failed!"), MB_CAPTION, MB_OK);
	//	return false;
	//}

	//ReadScan  = (ReadScanProc)GetProcAddress(m_hDLL, "ReadScan");
	//if(ReadScan  == NULL)
	//{
	//	::MessageBox(g_hwndDLG, TEXT("Load ReadScan Failed!"), MB_CAPTION, MB_OK);
	//	return false;
	//}

	//ReadScanEx = (ReadScanExProc)GetProcAddress(m_hDLL, "ReadScanEx");
	//if(ReadScanEx == NULL)
	//{
	//	::MessageBox(g_hwndDLG, TEXT("Load ReadScanEx Failed!"), MB_CAPTION, MB_OK);
	//	return false;
	//}

	//ReadScanEx2 = (ReadScanEx2Proc)GetProcAddress(m_hDLL, "ReadScanEx2");
	//if(ReadScanEx2 == NULL)
	//{
	//	::MessageBox(g_hwndDLG, TEXT("Load ReadScanEx2 Failed!"), MB_CAPTION, MB_OK);
	//	return false;
	//}

	//StopScan = (StopScanProc)GetProcAddress(m_hDLL, "StopScan");
	//if(StopScan == NULL)
	//{
	//	::MessageBox(g_hwndDLG, TEXT("Load StopScan Failed!"), MB_CAPTION, MB_OK);
	//	return false;
	//}

	//EndScanJob = (EndScanJobProc)GetProcAddress(m_hDLL, "EndScanJob");
	//if(EndScanJob == NULL)
	//{
	//	::MessageBox(g_hwndDLG, TEXT("Load EndScanJob Failed!"), MB_CAPTION, MB_OK);
	//	return false;
	//}

	//DoEjectPaperAfterScan = (DoEjectPaperAfterScanProc)GetProcAddress(m_hDLL, "DoEjectPaperAfterScan");
	//if(DoEjectPaperAfterScan == NULL)
	//{
	//	::MessageBox(g_hwndDLG, TEXT("Load DoEjectPaperAfterScan Failed!"), MB_CAPTION, MB_OK);
	//	return false;
	//}



	//GetLastStatusCode  = (GetLastStatusCodeProc)GetProcAddress(m_hDLL, "GetLastStatusCode");
	//if(GetLastStatusCode  == NULL)
	//{
	//	::MessageBox(g_hwndDLG, TEXT("Load GetLastStatusCode Failed!"), MB_CAPTION, MB_OK);
	//	return false;
	//}

	//GetDeviceList  = (GetDeviceListProc)GetProcAddress(m_hDLL, "GetDeviceList");
	//if(GetDeviceList  == NULL)
	//{
	//	::MessageBox(g_hwndDLG, TEXT("Load GetDeviceList Failed!"), MB_CAPTION, MB_OK);
	//	return false;
	//}

	//DestroyDeviceList  = (DestroyDeviceListProc)GetProcAddress(m_hDLL, "DestroyDeviceList");
	//if(DestroyDeviceList  == NULL)
	//{
	//	::MessageBox(g_hwndDLG, TEXT("Load DestroyDeviceList Failed!"), MB_CAPTION, MB_OK);
	//	return false;
	//}

	//SelectDevice  = (SelectDeviceProc)GetProcAddress(m_hDLL, "SelectDevice");
	//if(SelectDevice  == NULL)
	//{
	//	::MessageBox(g_hwndDLG, TEXT("Load SelectDevice Failed!"), MB_CAPTION, MB_OK);
	//	return false;
	//}

	//GeneralOperation = (GeneralOperationProc)GetProcAddress(m_hDLL, "GeneralOperation");
	//if(GeneralOperation  == NULL)
	//{
	//	::MessageBox(g_hwndDLG, TEXT("Load GeneralOperation Failed!"), MB_CAPTION, MB_OK);
	//	return false;
	//}

	//ResetScanner = (ResetScannerProc)GetProcAddress(m_hDLL, "ResetScanner");
	//if(ResetScanner  == NULL)
	//{
	//	::MessageBox(g_hwndDLG, TEXT("Load ResetScanner Failed!"), MB_CAPTION, MB_OK);
	//	return false;
	//}

	//LampOn = (LampOnProc)GetProcAddress(m_hDLL, "LampOn");
	//if(LampOn  == NULL)
	//{
	//	::MessageBox(g_hwndDLG, TEXT("Load LampOn Failed!"), MB_CAPTION, MB_OK);
	//	return false;
	//}

	//GetBackgroundRaster = (GetBackgroundRasterProc)GetProcAddress(m_hDLL, "GetBackgroundRaster");
	//if(GetBackgroundRaster  == NULL)
	//{
	//	::MessageBox(g_hwndDLG, TEXT("Load GetBackgroundRaster Failed!"), MB_CAPTION, MB_OK);
	//	return false;
	//}

	//TuneScanLength = (TuneScanLengthProc)GetProcAddress(m_hDLL, "TuneScanLength");
	//if(TuneScanLength == NULL)
	//{
	//	::MessageBox(g_hwndDLG, TEXT("Load TuneScanLength Failed!"), MB_CAPTION, MB_OK);
	//	return false;
	//}


	//SetBatchScanParameter = (SetBatchScanParameterProc)GetProcAddress(m_hDLL, "SetBatchScanParameter");
	//if(SetBatchScanParameter == NULL)
	//{
	//	::MessageBox(g_hwndDLG, TEXT("Load SetBatchScanParameter Failed!"), MB_CAPTION, MB_OK);
	//	return false;
	//}

	//SetSpeedMode = (SetSpeedModeProc)GetProcAddress(m_hDLL, "SetSpeedMode");
	//if(SetSpeedMode == NULL)
	//{
	//	::MessageBox(g_hwndDLG, TEXT("Load SetSpeedMode Failed!"), MB_CAPTION, MB_OK);
	//	return false;
	//}


	TerminateDriver  = (TerminateDriverProc)GetProcAddress(m_hDLL, "TerminateDriver");
	if(TerminateDriver  == NULL)
	{
		::MessageBox(g_hwndDLG, TEXT("Load TerminateDriver Failed!"), MB_CAPTION, MB_OK);
		return false;
	}

	ReadNVMData = (ReadNVMDataProc)GetProcAddress(m_hDLL, "ReadNVMData");
	if(ReadNVMData == NULL)
	{
		::MessageBox(g_hwndDLG, TEXT("Load ReadNVMData Failed!"), MB_CAPTION, MB_OK);
		return false;
	}

	if (FALSE == InitializeDriver())
	{
		::MessageBox(g_hwndDLG, TEXT("Run InitializeDriver Failed!"), MB_CAPTION, MB_OK);
		return false;
	}

	if(FALSE == InitializeScanner())
	{
		::MessageBox(g_hwndDLG, TEXT("Run InitializeScanner Failed!"), MB_CAPTION, MB_OK);
		return false;
	}

	return true;
}
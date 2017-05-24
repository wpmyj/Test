// Page_Info.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "UDS Scanner.h"
#include "Page_Info.h"
#include "afxdialogex.h"
#include "ComputerInfo.h"
#define  FILENAME_TXT_LOG  TEXT("log.txt")
//#include <tchar.h>

#pragma comment(lib, "ComputerInfo.lib")

extern HWND g_hwndDLG;
extern int g_nDeviceNumber;
extern void GetFilePath( char* szFileName, char* szFilePath);
// CPage_Info �Ի���

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
	DDX_Control(pDX, IDC_INFO_LIST_INFORMATION, m_list_infomation);
}


BEGIN_MESSAGE_MAP(CPage_Info, CPropertyPage)
	ON_BN_CLICKED(IDC_INFO_BTN_REPORT, &CPage_Info::OnInfo_Btn_Report)
	ON_BN_CLICKED(IDC_INFO_BUTTON_CLEARDICIDE, &CPage_Info::OnInfo_Btn_ClearDicideNum)
	ON_BN_CLICKED(IDC_INFO_BUTTON_CLEARFEED, &CPage_Info::OnInfo_Btn_ClearFeedNum)
END_MESSAGE_MAP()


// CPage_Info ��Ϣ�������


void CPage_Info::OnInfo_Btn_Report()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	TCHAR szPath[MAX_PATH];
	memset(szPath, 0, sizeof(szPath));
	GetFilePath(FILENAME_TXT_LOG, szPath);
	ShellExecute(NULL,TEXT("open"),szPath,NULL,NULL,SW_SHOWNORMAL);
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
	
	ShowComputerAndScannerInfo();


	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CPage_Info::ShowComputerAndScannerInfo()
{
	m_list_infomation.SetExtendedStyle( LVS_EX_FULLROWSELECT); // ��ʾѡ�����У�����ĳһ��
	//������չ���Ϊ��������ʽ������ѡȡ
	m_list_infomation.ShowScrollBar(SB_VERT,TRUE); // ֻ��ʾ��ֱ������

	CImageList m_image; 
	m_image.Create(1,20,ILC_COLOR32,1,0); 
	m_list_infomation.SetImageList(&m_image, LVSIL_SMALL);
	m_list_infomation.DeleteAllItems(); // ���
	CRect rect;   
	m_list_infomation.GetClientRect(rect);

	m_list_infomation.InsertColumn(0, "", LVCFMT_LEFT, rect.Width()/3);
	m_list_infomation.InsertColumn(1, "", LVCFMT_LEFT, rect.Width() - rect.Width()/3);
	//m_list_infomation.SetRedraw(FALSE); // ��ֹ�ػ�

	//////////////////////////////////////////////////////////////////////////
	// ��ȡ������Ϣ
	STRING str1(TEXT("���봦������"));  // ��1��
	STRING str2(TEXT(""));            // ��2��
	// ��ȡ���봦������Ϣ
	GetCPUInfo(str2);
	int nIndex = 0;
	m_list_infomation.InsertItem(nIndex, str1.c_str());  // ��1��
	m_list_infomation.SetItemText(nIndex++, 1, str2.c_str()); // ��2��
	
	FILE *pFile  = NULL;
	STRING strLog = TEXT("");

	//opens file
	TCHAR szPath[MAX_PATH];
	memset(szPath, 0, sizeof(szPath));
	GetFilePath(FILENAME_TXT_LOG, szPath);
	fopen_s(&pFile, szPath, "w"); // ��ֻд����ʽ���ļ�

	if (pFile)
	{
		strLog = str1 + str2 + TEXT("\n");
		fwrite(strLog.c_str(), strLog.size(), 1, pFile);
		fseek(pFile, 0, SEEK_END); 
	}
	else
	{
		TCHAR buffer[MAX_PATH];
		_stprintf_s(buffer, "�ļ�%s��ʧ�ܣ�", szPath);
		::MessageBox(g_hwndDLG,TEXT(buffer),MB_CAPTION,MB_OK);
	}

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

		if (pFile)
		{
			strLog = str1 + strSub + TEXT("\n");;
			fwrite(strLog.c_str(), strLog.size(), 1, pFile);
			fseek(pFile, 0, SEEK_END); 
		}
	}

	//��ȡ�ڴ���Ϣ
	str1 = TEXT("�ڴ棺");
	GetMemoryInfo(str2);
	m_list_infomation.InsertItem(nIndex, str1.c_str());
	m_list_infomation.SetItemText(nIndex++, 1, str2.c_str()); // ��2��

	if (pFile)
	{
		strLog = str1 + str2 + TEXT("\n");
		fwrite(strLog.c_str(), strLog.size(), 1, pFile);
		fseek(pFile, 0, SEEK_END); 
	}

	// ��ȡ����ϵͳ��Ϣ
	str1 = TEXT("����ϵͳ��");
	GetWindowsVersion(str2);
	m_list_infomation.InsertItem(nIndex, str1.c_str());
	m_list_infomation.SetItemText(nIndex++, 1, str2.c_str()); // ��2��

	if (pFile)
	{
		strLog = str1 + str2 + TEXT("\n");
		fwrite(strLog.c_str(), strLog.size(), 1, pFile);
		fseek(pFile, 0, SEEK_END); 
	}


	// ��ȡ����ϵͳ����
	str1 = TEXT("����ϵͳ���ԣ�");
	GetUserDefaultLanguage(str2);
	m_list_infomation.InsertItem(nIndex, str1.c_str());
	m_list_infomation.SetItemText(nIndex++, 1, str2.c_str()); // ��2��

	if (pFile)
	{
		strLog = str1 + str2 + TEXT("\n");
		fwrite(strLog.c_str(), strLog.size(), 1, pFile);
		fseek(pFile, 0, SEEK_END); 
	}

	// ��ȡ�豸����
	str1 = TEXT("�豸���ƣ�");
	switch(g_nDeviceNumber)
	{
	case DEVICE_OPENCV:
		str2 = TEXT("����ɨ����");
		break;
	case DEVICE_G6400:
		str2 = TEXT("G6400");
		break;
	case DEVICE_G6600:
		str2 = TEXT("G6600");
		break;
	default:
		break;
	}
	m_list_infomation.InsertItem(nIndex, str1.c_str());
	m_list_infomation.SetItemText(nIndex++, 1, str2.c_str()); // ��2��
	if (pFile)
	{
		strLog = str1 + str2 + TEXT("\n");
		fwrite(strLog.c_str(), strLog.size(), 1, pFile);
		fseek(pFile, 0, SEEK_END); 
	}

	// ��ȡӦ�ó�������
	str1 = TEXT("Ӧ�ó������ƣ�");
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
	m_list_infomation.SetItemText(nIndex++, 1, str2.c_str()); // ��2��

	if (pFile)
	{
		strLog = str1 + str2 + TEXT("\n");
		fwrite(strLog.c_str(), strLog.size(), 1, pFile);
		fseek(pFile, 0, SEEK_END); 
		fclose(pFile);
	}


	//////////////////////////////////////////////////////////////////////////
	// ��ȡɨ������Ϣ
	if (false == LoadDLL())
	{
		return;
	}

	NVMDATA data;
	ReadNVMData(&data);

	// ��ӿ���
	str1 = str2= TEXT("");
	m_list_infomation.InsertItem(nIndex, str1.c_str());
	m_list_infomation.SetItemText(nIndex++, 1, str2.c_str()); // ��2��

	fopen_s(&pFile, szPath, "a"); // ��׷�ӡ���ʽ���ļ�
	if (pFile)
	{
		strLog = str1 + str2 + TEXT("\n");
		fwrite(strLog.c_str(), strLog.size(), 1, pFile);
		fseek(pFile, 0, SEEK_END); 
	}
	else
	{
		TCHAR buffer[MAX_PATH];
		_stprintf_s(buffer, "�ļ�%s��ʧ��!", szPath);
		::MessageBox(g_hwndDLG,TEXT(buffer),MB_CAPTION,MB_OK);
	}

	TCHAR  buffer[MAX_PATH];
	// ��ȡɨ�������к�
	str1 = TEXT("ɨ�������кţ�");
	memset(buffer, 0, sizeof(buffer));
	_stprintf_s(buffer, "%s", data.SN);
	str2 = buffer;
	m_list_infomation.InsertItem(nIndex, str1.c_str());
	m_list_infomation.SetItemText(nIndex++, 1, str2.c_str()); // ��2��
	
	if (pFile)
	{
		strLog = str1 + str2 + TEXT("\n");
		fwrite(strLog.c_str(), strLog.size(), 1, pFile);
		fseek(pFile, 0, SEEK_END); 
	}

	// ��ȡ�汾����
	str1 = TEXT("�汾���ڣ�");
	memset(buffer, 0, sizeof(buffer));
	_stprintf_s(buffer, "%u/%u/%u", data.BornYear, data.BornMonth, data.BornDay);
	str2 = buffer;
	m_list_infomation.InsertItem(nIndex, str1.c_str());
	m_list_infomation.SetItemText(nIndex++, 1, str2.c_str()); // ��2��

	if (pFile)
	{
		strLog = str1 + str2 + TEXT("\n");
		fwrite(strLog.c_str(), strLog.size(), 1, pFile);
		fseek(pFile, 0, SEEK_END); 
	}

	// ��ȡ��������
	str1 = TEXT("�������ڣ�");
	memset(buffer, 0, sizeof(buffer));
	_stprintf_s(buffer, "%u/%u/%u", data.FirstScanYear, data.FirstScanMonth, data.FirstScanDay);
	str2 = buffer;
	m_list_infomation.InsertItem(nIndex, str1.c_str());
	m_list_infomation.SetItemText(nIndex++, 1, str2.c_str()); // ��2��

	if (pFile)
	{
		strLog = str1 + str2 + TEXT("\n");
		fwrite(strLog.c_str(), strLog.size(), 1, pFile);
		fseek(pFile, 0, SEEK_END); 
	}

	// ��ȡ�Ĳķ�ֽ�ּ���
	str1 = TEXT("�Ĳķ�ֽ�ּ�����");
	memset(buffer, 0, sizeof(buffer));
	_stprintf_s(buffer, "%lu", data.PadScanCount);
	str2 = buffer;
	m_list_infomation.InsertItem(nIndex, str1.c_str());
	m_list_infomation.SetItemText(nIndex++, 1, str2.c_str()); // ��2��

	if (pFile)
	{
		strLog = str1 + str2 + TEXT("\n");
		fwrite(strLog.c_str(), strLog.size(), 1, pFile);
		fseek(pFile, 0, SEEK_END); 
	}

	// ��ȡ�ĲĽ�ֽ�ּ���
	str1 = TEXT("�ĲĽ�ֽ�ּ�����");
	memset(buffer, 0, sizeof(buffer));
	_stprintf_s(buffer, "%lu", data.RollerCount);
	str2 = buffer;
	m_list_infomation.InsertItem(nIndex, str1.c_str());
	m_list_infomation.SetItemText(nIndex++, 1, str2.c_str()); // ��2��

	if (pFile)
	{
		strLog = str1 + str2 + TEXT("\n");
		fwrite(strLog.c_str(), strLog.size(), 1, pFile);
		fseek(pFile, 0, SEEK_END); 
	}

	// ��ȡ����ɨ�����
	str1 = TEXT("����ɨ�������");
	memset(buffer, 0, sizeof(buffer));
	_stprintf_s(buffer, "%lu", data.ADFSingleModeScanCount);
	str2 = buffer;
	m_list_infomation.InsertItem(nIndex, str1.c_str());
	m_list_infomation.SetItemText(nIndex++, 1, str2.c_str()); // ��2��

	if (pFile)
	{
		strLog = str1 + str2 + TEXT("\n");
		fwrite(strLog.c_str(), strLog.size(), 1, pFile);
		fseek(pFile, 0, SEEK_END); 
	} 

	// ��ȡ˫��ɨ�����
	str1 = TEXT("˫��ɨ�������");
	memset(buffer, 0, sizeof(buffer));
	_stprintf_s(buffer, "%lu", data.ADFDuplexModeScanCount);
	str2 = buffer;
	m_list_infomation.InsertItem(nIndex, str1.c_str());
	m_list_infomation.SetItemText(nIndex++, 1, str2.c_str()); // ��2��

	if (pFile)
	{
		strLog = str1 + str2 + TEXT("\n");
		fwrite(strLog.c_str(), strLog.size(), 1, pFile);
		fseek(pFile, 0, SEEK_END); 
	}

	// ��ȡ�ص���ֽ����
	str1 = TEXT("�ص���ֽ������");
	memset(buffer, 0, sizeof(buffer));
	_stprintf_s(buffer, "%lu", data.MultifeedCount);
	str2 = buffer;
	m_list_infomation.InsertItem(nIndex, str1.c_str());
	m_list_infomation.SetItemText(nIndex++, 1, str2.c_str()); // ��2��

	if (pFile)
	{
		strLog = str1 + str2 + TEXT("\n");
		fwrite(strLog.c_str(), strLog.size(), 1, pFile);
		fseek(pFile, 0, SEEK_END); 
	} 

	// ��ȡ��ֽ����
	str1 = TEXT("��ֽ������");
	memset(buffer, 0, sizeof(buffer));
	_stprintf_s(buffer, "%lu", data.JamCount);
	str2 = buffer;
	m_list_infomation.InsertItem(nIndex, str1.c_str());
	m_list_infomation.SetItemText(nIndex++, 1, str2.c_str()); // ��2��

	if (pFile)
	{
		strLog = str1 + str2 + TEXT("\n");
		fwrite(strLog.c_str(), strLog.size(), 1, pFile);
		fclose(pFile);
	}

	//TerminateDriver();
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

	m_hDLL = LoadLibrary(DllPath);
	if(m_hDLL == NULL)
	{
		::MessageBox(g_hwndDLG, TEXT("Load Dll Failed!"), MB_CAPTION, MB_OK);
		return false;
	}

	InitializeDriver = (InitializeDriverProc)GetProcAddress(m_hDLL, "InitializeDriver");
	if(InitializeDriver == NULL)
	{
		return false;
	}

	InitializeScanner = (InitializeScannerProc)GetProcAddress(m_hDLL, "InitializeScanner");
	if(InitializeScanner == NULL)
	{
		return false;
	}

	TerminateDriver  = (TerminateDriverProc)GetProcAddress(m_hDLL, "TerminateDriver");
	if(TerminateDriver  == NULL)
	{
		return false;
	}

	ReadNVMData = (ReadNVMDataProc)GetProcAddress(m_hDLL, "ReadNVMData");
	if(ReadNVMData == NULL)
	{
		return false;
	}

	if (FALSE == InitializeDriver())
	{
		return false;
	}

	if(FALSE == InitializeScanner())
	{
		return false;
	}

	return true;
}
BOOL CPage_Info::OnSetActive()
{
	// TODO: �ڴ����ר�ô����/����û���
	m_pUI->PreViewStatus();
	return __super::OnSetActive();
}
// Dlg_Device.cpp : 实现文件
//

#include "stdafx.h"
#include "UDS Scanner.h"
#include "Dlg_Device.h"
#include "afxdialogex.h"

#include "FindDevice.h"
#pragma comment(lib, "FindDevice.lib")
#include "public.h"

extern int g_nDeviceNumber;
extern HWND g_hwndDLG;
extern void GetFilePath( char* szFileName, char* szFilePath);
// CDlg_Device 对话框

IMPLEMENT_DYNAMIC(CDlg_Device, CDialogEx)

CDlg_Device::CDlg_Device(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlg_Device::IDD, pParent)
	, m_nNumber(0)
{

}

CDlg_Device::~CDlg_Device()
{
}

void CDlg_Device::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DEVICE_LIST, m_list_device);
	DDX_Control(pDX, IDC_DEVICE_CHECK_NOUI, m_check_noui);
}


BEGIN_MESSAGE_MAP(CDlg_Device, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlg_Device::OnBnClickedOk)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_DEVICE_LIST, &CDlg_Device::OnLvnItemchangedDeviceList)
	ON_BN_CLICKED(IDC_DEVICE_CHECK_NOUI, &CDlg_Device::OnBnClickedDeviceCheck_Noui)
END_MESSAGE_MAP()


// CDlg_Device 消息处理程序


void CDlg_Device::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码

	// Set the selection mark to the first item only if no other item is selected.
	if (m_list_device.GetSelectionMark() == -1)
	{
		m_list_device.SetSelectionMark(0);
	}

	int i = m_list_device.GetSelectionMark(); // 获得选中行的行标
	CString str = m_list_device.GetItemText(i,0); //i是行，j是列。
	if (str.Find("高拍仪") >= 0)
	{
		g_nDeviceNumber = DEVICE_CAMERA;
	}
	else if (str.Find("G6400") >= 0)
	{
		g_nDeviceNumber = DEVICE_G6400;
	}
	else if (str.Find("G6600") >= 0)
	{
		g_nDeviceNumber = DEVICE_G6600;
	}
	else
	{
		g_nDeviceNumber = DEVICE_OPENCV;
	}

	//保存设备编号
	CString strTemp;
	TCHAR szINIPath[MAX_PATH];  // INI文件路径
	GetFilePath(FILENAME_INI,szINIPath);
	strTemp.Format(TEXT("%d"), g_nDeviceNumber);
	WritePrivateProfileString(INI_APP_DEVICE,INI_KEY_DEVICENUMBER,strTemp,szINIPath);


	CDialogEx::OnOK();
}


BOOL CDlg_Device::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	m_list_device.SetExtendedStyle( LVS_EX_FULLROWSELECT); // 表示选中整行，而非某一列
	//设置扩展风格为：网格形式、整行选取
	m_list_device.DeleteAllItems(); // 清空
	CImageList m_image; 
	const unsigned int unHeight = 28;
	m_image.Create(1,unHeight,ILC_COLOR32,1,0); 
	m_list_device.SetImageList(&m_image, LVSIL_SMALL);

	CRect rect;   
	m_list_device.GetClientRect(rect);
	m_list_device.InsertColumn(0, "可用设备", LVCFMT_LEFT, rect.Width());

	//重绘false防止重绘闪烁
	m_list_device.SetRedraw(false);

	// 依次将连接设备添加进ListControl
	UINT index = 0;
	m_list_device.InsertItem(index++, TEXT("虚拟扫描仪"));
	if (TRUE == FindScanner(TEXT("0638"), TEXT("2c73"))) {
		m_list_device.InsertItem(index++, TEXT("扫描仪G6400"));
	}
	if (TRUE ==  FindScanner(TEXT("0638"), TEXT("2c74"))) {
		m_list_device.InsertItem(index++, TEXT("扫描仪G6600"));
	}
	if (0 != GetCameraCount()) {		
		m_list_device.InsertItem(index++, TEXT("高拍仪/拍摄仪"));		
	}

	// 循环判断配置文件中设备，并选中
	TCHAR szINIPath[MAX_PATH];  // INI文件路径
	GetFilePath(FILENAME_INI,szINIPath);
	g_nDeviceNumber = GetPrivateProfileInt(INI_APP_DEVICE,INI_KEY_DEVICENUMBER,1,szINIPath);

	for (int i=0; i< index; i++)
	{
		CString str = m_list_device.GetItemText(i,0); //i是行，j是列。
		if (str.Find("高拍仪") >= 0 && (DEVICE_CAMERA == g_nDeviceNumber)) {
			m_list_device.SetItemState(i, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
		}
		if (str.Find("G6400") >= 0 && (DEVICE_G6400 == g_nDeviceNumber)) {
			m_list_device.SetItemState(i, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
		}
		if (str.Find("G6600") >= 0 && (DEVICE_G6600 == g_nDeviceNumber)) {
			m_list_device.SetItemState(i, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
		}
		if (str.Find("虚拟扫描仪") >= 0 && (DEVICE_OPENCV == g_nDeviceNumber)) {
			m_list_device.SetItemState(i, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
		}
	}

	m_list_device.SetRedraw(true);  // 重绘
	m_list_device.Invalidate();

	m_check_noui.SetCheck(FALSE);
	OnBnClickedDeviceCheck_Noui(); // 必须调用
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlg_Device::SetNumber(int _number)
{
	m_nNumber = _number;
}

void CDlg_Device::OnLvnItemchangedDeviceList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	*pResult = 0;
}



void CDlg_Device::OnBnClickedDeviceCheck_Noui()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strTemp;
	TCHAR szINIPath[MAX_PATH];  // INI文件路径
	GetFilePath(FILENAME_INI,szINIPath);
	if (m_check_noui.GetCheck())
	{
		strTemp = TEXT("Y");
	} 
	else
	{
		strTemp = TEXT("N");
	}
	WritePrivateProfileString(INI_APP_DEVICE,INI_KEY_NOUI,strTemp,szINIPath);
}

// Dlg_Device.cpp : ʵ���ļ�
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
// CDlg_Device �Ի���

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


// CDlg_Device ��Ϣ�������


void CDlg_Device::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	// Set the selection mark to the first item only if no other item is selected.
	if (m_list_device.GetSelectionMark() == -1)
	{
		m_list_device.SetSelectionMark(0);
	}

	int i = m_list_device.GetSelectionMark(); // ���ѡ���е��б�
	CString str = m_list_device.GetItemText(i,0); //i���У�j���С�
	if (str.Find("������") >= 0)
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

	//�����豸���
	CString strTemp;
	TCHAR szINIPath[MAX_PATH];  // INI�ļ�·��
	GetFilePath(FILENAME_INI,szINIPath);
	strTemp.Format(TEXT("%d"), g_nDeviceNumber);
	WritePrivateProfileString(INI_APP_DEVICE,INI_KEY_DEVICENUMBER,strTemp,szINIPath);


	CDialogEx::OnOK();
}


BOOL CDlg_Device::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	m_list_device.SetExtendedStyle( LVS_EX_FULLROWSELECT); // ��ʾѡ�����У�����ĳһ��
	//������չ���Ϊ��������ʽ������ѡȡ
	m_list_device.DeleteAllItems(); // ���
	CImageList m_image; 
	const unsigned int unHeight = 28;
	m_image.Create(1,unHeight,ILC_COLOR32,1,0); 
	m_list_device.SetImageList(&m_image, LVSIL_SMALL);

	CRect rect;   
	m_list_device.GetClientRect(rect);
	m_list_device.InsertColumn(0, "�����豸", LVCFMT_LEFT, rect.Width());

	//�ػ�false��ֹ�ػ���˸
	m_list_device.SetRedraw(false);

	// ���ν������豸��ӽ�ListControl
	UINT index = 0;
	m_list_device.InsertItem(index++, TEXT("����ɨ����"));
	if (TRUE == FindScanner(TEXT("0638"), TEXT("2c73"))) {
		m_list_device.InsertItem(index++, TEXT("ɨ����G6400"));
	}
	if (TRUE ==  FindScanner(TEXT("0638"), TEXT("2c74"))) {
		m_list_device.InsertItem(index++, TEXT("ɨ����G6600"));
	}
	if (0 != GetCameraCount()) {		
		m_list_device.InsertItem(index++, TEXT("������/������"));		
	}

	// ѭ���ж������ļ����豸����ѡ��
	TCHAR szINIPath[MAX_PATH];  // INI�ļ�·��
	GetFilePath(FILENAME_INI,szINIPath);
	g_nDeviceNumber = GetPrivateProfileInt(INI_APP_DEVICE,INI_KEY_DEVICENUMBER,1,szINIPath);

	for (int i=0; i< index; i++)
	{
		CString str = m_list_device.GetItemText(i,0); //i���У�j���С�
		if (str.Find("������") >= 0 && (DEVICE_CAMERA == g_nDeviceNumber)) {
			m_list_device.SetItemState(i, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
		}
		if (str.Find("G6400") >= 0 && (DEVICE_G6400 == g_nDeviceNumber)) {
			m_list_device.SetItemState(i, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
		}
		if (str.Find("G6600") >= 0 && (DEVICE_G6600 == g_nDeviceNumber)) {
			m_list_device.SetItemState(i, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
		}
		if (str.Find("����ɨ����") >= 0 && (DEVICE_OPENCV == g_nDeviceNumber)) {
			m_list_device.SetItemState(i, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
		}
	}

	m_list_device.SetRedraw(true);  // �ػ�
	m_list_device.Invalidate();

	m_check_noui.SetCheck(FALSE);
	OnBnClickedDeviceCheck_Noui(); // �������
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CDlg_Device::SetNumber(int _number)
{
	m_nNumber = _number;
}

void CDlg_Device::OnLvnItemchangedDeviceList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	*pResult = 0;
}



void CDlg_Device::OnBnClickedDeviceCheck_Noui()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strTemp;
	TCHAR szINIPath[MAX_PATH];  // INI�ļ�·��
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

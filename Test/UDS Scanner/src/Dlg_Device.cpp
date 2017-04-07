// Dlg_Device.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "UDS Scanner.h"
#include "Dlg_Device.h"
#include "afxdialogex.h"

#include "FindDevice.h"
#pragma comment(lib, "FindDevice.lib")
#include "public.h"



#define  SKIN_BACK     _T("skin\\bk.png")
#define  SKIN_BUTTON   _T("skin\\button2.png")
#define  SKIN_CHECK    _T("skin\\check.png")

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
	DDX_Control(pDX, IDOK, m_btnOK);
}


BEGIN_MESSAGE_MAP(CDlg_Device, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlg_Device::OnBnClickedOk)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_DEVICE_LIST, &CDlg_Device::OnLvnItemchangedDeviceList)
	ON_BN_CLICKED(IDC_DEVICE_CHECK_NOUI, &CDlg_Device::OnBnClickedDeviceCheck_Noui)
	ON_WM_NCHITTEST()
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
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
	//SetClassLong(this->m_hWnd, GCL_STYLE, GetClassLong(this->m_hWnd, GCL_STYLE) | CS_DROPSHADOW);
	
	TCHAR path[MAX_PATH] = {0};
	GetFilePath(SKIN_BUTTON, path);
	m_btnOK.SetButtonImage(path, CXIMAGE_FORMAT_PNG);

	GetFilePath(SKIN_CHECK, path);
	m_check_noui.SetCheckImage(path, CXIMAGE_FORMAT_PNG);
	m_check_noui.SetToolTips(_T("ѡ�����Ժ�����ʾ���Ի���"));


	SetWindowPos(NULL, 0, 0, 256, 200, SWP_NOZORDER | SWP_NOMOVE);
	CRect rect_dlg, rect_static_noui, rect_check_noui, rect_list_device , rect_button_ok;

	GetWindowRect(rect_dlg);
	ScreenToClient(rect_dlg);

	GetDlgItem(IDC_DEVICE_LIST)->SetWindowPos(NULL, 10, 10, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
	GetDlgItem(IDC_DEVICE_LIST)->GetWindowRect(&rect_list_device);
	ScreenToClient(rect_list_device);

	GetDlgItem(IDC_DEVICE_STATIC_NOUI)->SetWindowPos(NULL, 10, rect_list_device.bottom + 10,
		0, 0, SWP_NOZORDER | SWP_NOSIZE);
	GetDlgItem(IDC_DEVICE_STATIC_NOUI)->GetWindowRect(&rect_static_noui);
	ScreenToClient(rect_static_noui);

	GetDlgItem(IDC_DEVICE_CHECK_NOUI)->SetWindowPos(NULL, rect_static_noui.right + 10,
		rect_static_noui.top, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
	GetDlgItem(IDC_DEVICE_CHECK_NOUI)->GetWindowRect(&rect_check_noui);
	ScreenToClient(rect_check_noui);

	GetDlgItem(IDOK)->GetWindowRect(&rect_button_ok);
	ScreenToClient(rect_button_ok);
	GetDlgItem(IDOK)->SetWindowPos(NULL, rect_dlg.right - rect_button_ok.Width() - 10,
		rect_check_noui.top - (rect_button_ok.Height() - rect_check_noui.Height()) / 2,
		0, 0, SWP_NOZORDER | SWP_NOSIZE);


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

	//m_check_noui.SetCheck(FALSE);
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
	if (m_check_noui.GetChecked())    
	{
		strTemp = TEXT("Y");
	} 
	else
	{
		strTemp = TEXT("N");
	}
	WritePrivateProfileString(INI_APP_DEVICE,INI_KEY_NOUI,strTemp,szINIPath);
}


LRESULT CDlg_Device::OnNcHitTest(CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	// ȡ��������ڵĴ�������
	UINT nHitTest = CDialog::OnNcHitTest(point);
	// �������ڴ��ڿͻ������򷵻ر��������Ÿ�Windows
	// ʹWindows������ڱ�����������д������ɵ����ƶ�����
	return (nHitTest == HTCLIENT) ? HTCAPTION : nHitTest;
	//return CDialogEx::OnNcHitTest(point);
}


void CDlg_Device::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
	TCHAR path[MAX_PATH] = {0};
  CImage imageBack;
	CRect rect;
	GetClientRect(&rect);	
	GetFilePath(SKIN_BACK, path);

	if (!imageBack.IsNull()) 
	{
		imageBack.Destroy();
	}
	imageBack.Load(path);
	if (!imageBack.IsNull())
	{
		imageBack.TransparentBlt(dc.m_hDC, 0, 0, rect.Width(), rect.Height(), 
			RGB(255, 255, 255));
	}

}


HBRUSH CDlg_Device::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	// TODO:  �ڴ˸��� DC ���κ�����
	CFont font1, font2/*, font3, font4, font5*/;
	font1.CreatePointFont(90, _T("΢���ź�"));
	font2.CreatePointFont(110, _T("΢���ź�"));
	//font3.CreatePointFont(100, _T("΢���ź�"));
	//font4.CreatePointFont(140, _T("΢���ź�"));
	//font5.CreatePointFont(90, _T("΢���ź�"));
	// TODO:  Change any attributes of the DC here

	switch (pWnd->GetDlgCtrlID())
	{
	case IDC_STATIC:
	case IDC_DEVICE_STATIC_NOUI:
	case IDOK:
		//case IDC_VIDEO_SLIDER_EXP:
		{
			//pDC->SetTextColor(RGB(255, 255, 255));
			//pDC->SetTextColor(RGB(44, 173, 219)); // ��ɫ
			//pDC->SetTextColor(RGB(129, 42, 142)); // ��ɫ
			pDC->SetTextColor(RGB(2, 232, 0));  // ����ɫ
			pDC->SelectObject(&font1);
			pDC->SetBkMode(TRANSPARENT);
			return (HBRUSH)GetStockObject(HOLLOW_BRUSH);
		}
		break;
	default: 
		break;

	}
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}

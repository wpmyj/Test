// CDlg_Camera.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "UDS Scanner.h"
#include "CDlg_Camera.h"
#include "afxdialogex.h"
#include "public.h"

// CDlg_Camera �Ի���

IMPLEMENT_DYNAMIC(CDlg_Camera, CDialogEx)

CDlg_Camera::CDlg_Camera(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlg_Camera::IDD, pParent)
{

}

CDlg_Camera::~CDlg_Camera()
{
}

void CDlg_Camera::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_CAMERALIST, m_cbCameraList);
	DDX_Control(pDX, IDC_COMBO_DOCSIZE, m_cbDocList);
	DDX_Control(pDX, IDC_COMBO_IMAGESIZE, m_cbSizeList);
	DDX_Control(pDX, IDC_COMBO_IMAGETYPE, m_cbImageType);
	DDX_Control(pDX, IDC_BUTTON_PHOTO, m_bPhoto);
	DDX_Control(pDX, IDC_BUTTON_STOP, m_bStop);
	DDX_Control(pDX, IDC_BUTTON_DELETE, m_bDelete);
	DDX_Control(pDX, IDC_BUTTON_HELP, m_bHelp);
	DDX_Control(pDX, IDC_STATIC_UI, m_sPreviewWnd);
	DDX_Control(pDX, IDC_STATIC_PHOTONO, m_sPhotoNo);
	DDX_Control(pDX, IDC_CHECK_AUTOPHOTO, m_cAutoClip);
	DDX_Control(pDX, IDC_CHECK_AUTOENHANCE, m_cAutoEnhance);
	DDX_Control(pDX, IDC_CHECK_AUTOROTATE, m_cAutoRotate);
	DDX_Control(pDX, IDC_SLIDER_EXPOSURE, m_slExposure);
	DDX_Control(pDX, IDC_SLIDER_BRIGHTNESS, m_slBrightness);
}


BEGIN_MESSAGE_MAP(CDlg_Camera, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_PHOTO, &CDlg_Camera::OnButton_Photo)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CDlg_Camera::OnButton_Stop)
	ON_BN_CLICKED(IDC_BUTTON_CAMIMAGE, &CDlg_Camera::OnButton_Camimage)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, &CDlg_Camera::OnButton_Delete)
	ON_CBN_SELCHANGE(IDC_COMBO_CAMERALIST, &CDlg_Camera::OnCbnSelchangeCombo_Cameralist)
	ON_CBN_SELCHANGE(IDC_COMBO_IMAGESIZE, &CDlg_Camera::OnCbnSelchangeCombo_Imagesize)
	ON_CBN_SELCHANGE(IDC_COMBO_IMAGETYPE, &CDlg_Camera::OnCbnSelchangeCombo_Imagetype)
	ON_CBN_SELCHANGE(IDC_COMBO_DOCSIZE, &CDlg_Camera::OnCbnSelchangeCombo_Docsize)
	ON_BN_CLICKED(IDC_CHECK_AUTOPHOTO, &CDlg_Camera::OnCheck_Autophoto)
	ON_BN_CLICKED(IDC_CHECK_AUTOENHANCE, &CDlg_Camera::OnCheck_Autoenhance)
	ON_BN_CLICKED(IDC_CHECK_AUTOROTATE, &CDlg_Camera::OnCheck_Autorotate)
	ON_BN_CLICKED(IDC_BUTTON_ADJUST, &CDlg_Camera::OnButton_Adjust)
	ON_BN_CLICKED(IDC_BUTTON_HELP, &CDlg_Camera::OnButton_Help)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_BUTTON_CAMVIDEO, &CDlg_Camera::OnButton_Camvideo)
END_MESSAGE_MAP()

void CDlg_Camera::MapDocSize()
{
	switch (m_Capture.m_Auto.docSize)
	{
	case 0:  // A3
		m_Capture.m_Auto.docWidth = -1;
		m_Capture.m_Auto.docHeight = -1;
		break;
	case 1:  // A4
		m_Capture.m_Auto.docWidth = 88;
		m_Capture.m_Auto.docHeight = 86;
		break;
	case 2:  // B5
		m_Capture.m_Auto.docWidth = 71;
		m_Capture.m_Auto.docHeight = 75;
		break;
	case 3:  // A5
		m_Capture.m_Auto.docWidth = 63;
		m_Capture.m_Auto.docHeight = 62;
		break;
	case 4:  // ����֤
		m_Capture.m_Auto.docWidth = 28;
		m_Capture.m_Auto.docHeight = 25;
		break;
	default:  // �Զ���
		//m_Capture.m_Auto.docWidth = m_pMainWnd->m_ini.CamDocWidth;  // �Զ����ֶ��������
		//m_Capture.m_Auto.docHeight = m_pMainWnd->m_ini.CamDocHeight;  // �Զ����ֶ�����߶�
		break;
	}
}


// CDlg_Camera ��Ϣ��������


BOOL CDlg_Camera::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ����Ӷ���ĳ�ʼ��

	// Mysher Camera License
	//theApp.CheckMysherLicense(theApp.m_OCXPath);  // ������������Ϣ�����theApp��
	//m_Capture.m_bActive = theApp.m_bCameraMysherActive;
	//if ( !m_pMainWnd->m_ini.LicenseCode.IsEmpty() )  // ��ע����(������)
	//{
	//	// ��ע����(������)��MyScan.INI�����������ֹ����ã��Ƚ�ע�����Ƿ�һ��
	//	if (theApp.GenerateLicenseCode(theApp.m_strCameraMysherSN) != m_pMainWnd->m_ini.LicenseCode)
	//		m_Capture.m_bActive = false;
	//	else
	//		m_Capture.m_bActive = true;
	//}

	// ��ʼ������ѡ��: ͼ�����͡��������㡢�Զ���ת���ĵ���ǿ
	m_cbImageType.AddString("��ɫ");  // Set m_cbImageType
	m_cbImageType.AddString("�Ҷ�");
	//m_cbImageType.AddString("�Զ�");
	//if (m_pMainWnd->m_ini.CamImageType == 1)  // �Ҷ�
	//	m_Capture.m_Auto.imageType = 1;
	//else //if (m_pMainWnd->m_ini.CamImageType == 2)  // ��ɫ
	//	m_Capture.m_Auto.imageType = 0;
	m_cbImageType.SetCurSel(m_Capture.m_Auto.imageType);

	//m_Capture.m_Auto.autoClip = m_pMainWnd->m_ini.CamAutoClip;
	if (m_Capture.m_Auto.autoClip == true)  // Set m_cAutoClip: ���ܲü�-��ת����
	{
		m_cAutoClip.SetCheck(BST_CHECKED);
		m_cbDocList.EnableWindow(FALSE);
	}
	else
	{
		m_cAutoClip.SetCheck(BST_UNCHECKED);
		m_cbDocList.EnableWindow(TRUE);
	}
	//m_Capture.m_Auto.autoRotate = m_pMainWnd->m_ini.CamAutoRotate;
	if (m_Capture.m_Auto.autoRotate == true)  // Set m_cAutoRotate: �Զ��ĵ���ת
		m_cAutoRotate.SetCheck(BST_CHECKED);
	else
		m_cAutoRotate.SetCheck(BST_UNCHECKED);
	//m_Capture.m_Auto.autoEnhance = m_pMainWnd->m_ini.CamAutoEnhance;
	if (m_Capture.m_Auto.autoEnhance == true)  // Set m_cAutoEnhance: �Զ��ĵ���ǿ
		m_cAutoEnhance.SetCheck(BST_CHECKED);
	else
		m_cAutoEnhance.SetCheck(BST_UNCHECKED);
	m_cbDocList.AddString("A3 420��297mm");  // Set m_cbDocList
	m_cbDocList.AddString("A4 297��210mm");
	m_cbDocList.AddString("B5 257��182mm");
	m_cbDocList.AddString("A5 210��148cm");
	m_cbDocList.AddString("����֤ 95��60mm");
	m_cbDocList.AddString("�Զ���");
	//m_Capture.m_Auto.docSize = m_pMainWnd->m_ini.CamDocSize;
	m_cbDocList.SetCurSel(m_Capture.m_Auto.docSize);
	MapDocSize();

	// ��ʼ������������������ء�Ԥ������
	//m_Capture.m_Auto.strCamrea = m_pMainWnd->m_ini.Camera1;  // Ĭ�������
	//m_Capture.m_Auto.strSize = m_pMainWnd->m_ini.CamFrameSize;  // Ĭ������
	//m_Capture.m_Auto.imageOrientation = m_pMainWnd->m_ini.CamOrientation;  // Ĭ��Ԥ������
	//m_Capture.m_Auto.fastPreview = m_pMainWnd->m_ini.CamFastPreview;
	//// ��ʼ���������������͡������ʽ��ͼ���ļ�·����ѹ����
	//m_Capture.m_strBarcodeType = m_pMainWnd->m_ini.BarcodeType;
	//m_Capture.m_strBarcodeFormat = m_pMainWnd->m_ini.BarcodeFormat;
	//m_Capture.m_nBarcodeLength = m_pMainWnd->m_ini.BarcodeLength;
	//m_Capture.m_bMultiBarcode = m_pMainWnd->m_ini.MultiBarcode;
	//m_Capture.m_bBarcodeRotate = m_pMainWnd->m_ini.BarcodeRotate;
	//m_Capture.m_strImagePath = m_pMainWnd->m_strTempPath;  // Set m_strImagePath
	//if (m_pMainWnd->m_ini.SaveHighQuality == true)
	//	m_Capture.m_nQuality = 80;  // Set Hight Quality
	//else
	//	m_Capture.m_nQuality = m_pMainWnd->m_ini.Quality;  // Set m_nQuality

	//m_Capture.m_nExposure = m_pMainWnd->m_ini.CamExposure;  // Set m_nExposure
	//m_Capture.m_nBrightness = m_pMainWnd->m_ini.CamBrightness;  // Set m_nBrightness

	m_Capture.m_hDlgWnd = this->GetSafeHwnd();
	m_Capture.m_hPreviewWnd = m_sPreviewWnd.GetSafeHwnd();
	if ( !m_Capture.CreateComInstance() ) {  // Initialize DirectShow
		MessageBox(TEXT("����Windows��֧�������!"), TEXT(MB_CAPTION));
	}

	m_bIsHelp = false;
	m_bStop.EnableWindow(FALSE);  // Set m_bStop
	m_bDelete.EnableWindow(FALSE);  // Set m_bDelete
	if ( m_Capture.GetDevicesList(&m_cbCameraList) > 0)  // Set m_cbCameraList
	{
		m_bPhoto.EnableWindow(TRUE);  // Set m_bPhoto
		m_Capture.m_bIsPreview = true;  // true: Preview
		StartCamera();
	}
	else
	{
		m_bPhoto.EnableWindow(FALSE);  // Set m_bPhoto
		MessageBox(TEXT("δ���������!"), TEXT(MB_CAPTION));
		EndDialog(IDCANCEL);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CDlg_Camera::StartCamera()
{
	m_Capture.StopCamera();  // Stop current camera and Start maybe a new one

	int devNo = m_cbCameraList.GetCurSel();
	int nCamCount = m_cbCameraList.GetCount();
	int sizeNo = m_cbSizeList.GetCurSel();
	int nSizeCount = m_cbSizeList.GetCount();
	if ( sizeNo != CB_ERR && nSizeCount > 0 )
		m_cbSizeList.GetLBText(sizeNo, m_Capture.m_Auto.strSize);  // Get current strSize
	if ( devNo != CB_ERR && nCamCount > 0 )
	{
		if ( m_Capture.SelectCamera(devNo, &m_cbSizeList) != true )  // Select Camera
			return;
		m_cbCameraList.GetLBText(devNo, m_Capture.m_Auto.strCamrea);  // Get current strCamrea

		int sizeNo = m_cbSizeList.GetCurSel();  // m_cbSizeList is Refilled by SelectCamera
		int nSizeCount = m_cbSizeList.GetCount();
		if ( sizeNo != CB_ERR && nSizeCount > 0 )
		{
			m_cbSizeList.GetLBText(sizeNo, m_Capture.m_Auto.strSize);  // Get current strSize
			m_Capture.SetupCamera();  // Setup Camera
			m_Capture.GetBrightness(&m_slBrightness);  // GetBrightness
			m_Capture.GetExposure(&m_slExposure);  // GetExposure
		}
	}
}


void CDlg_Camera::OnButton_Photo()
{
	// TODO: �ڴ����ӿؼ�֪ͨ�����������
	if (m_cAutoClip.GetCheck() == BST_CHECKED)  // Auto Clip
		m_Capture.m_Auto.autoClip = true;  // ��������
	else
		m_Capture.m_Auto.autoClip = false;  // �ֶ�����

	m_bPhoto.EnableWindow(FALSE);  // Set m_bPhoto
	if (m_Capture.m_Auto.autoClip == true)  // ��������
	{
		MessageBox(TEXT("��һ��: �Ƴ�������ϵ������ĵ�;\n�ڶ���: ����ȷ�����������Զ�ץ��״̬."), MB_CAPTION);
		m_Capture.m_bIsPreview = true;  // Ԥ��
		m_Capture.m_bIsAutoClipping = false;  // ����Զ�����״̬
		m_Capture.m_bCaptureBackground = true;  // ץ�ı���ͼ��
		m_bStop.EnableWindow(TRUE);  // Set m_bStop
	}
	else //if (m_Capture.m_Auto.autoClip == false)  // �ֶ�����
	{
		m_Capture.m_bIsPreview = false;  // ����ͼ���ļ�
		if (m_Capture.m_Auto.fastPreview == true)  // ����Ԥ��������ѡͼ��ߴ����������
			StartCamera();
		m_bStop.EnableWindow(FALSE);  // Set m_bStop
	}
}


void CDlg_Camera::OnButton_Stop()
{
	// TODO: �ڴ����ӿؼ�֪ͨ�����������
	m_Capture.m_bIsPreview = true;  // true: Preview
	m_Capture.m_bIsAutoClipping = false;  // ����Զ�����״̬
	if ( m_Capture.m_bIsAutoClipping && m_Capture.m_Auto.fastPreview )  // ����Ԥ���Զ�����״̬�����������
		StartCamera();
	m_bPhoto.EnableWindow(TRUE);  // Set m_bPhoto
	m_bStop.EnableWindow(FALSE);  // Set m_bStop
}


void CDlg_Camera::OnButton_Camimage()
{
	// TODO: �ڴ����ӿؼ�֪ͨ�����������
	m_Capture.ConfigCameraPin(this->m_hWnd);
}

void CDlg_Camera::OnButton_Adjust()
{
	// TODO: �ڴ����ӿؼ�֪ͨ�����������
	m_Capture.m_Auto.imageOrientation += 1;
	if (m_Capture.m_Auto.imageOrientation > 3)  // 0, 1-90, 2-180, 3-270
		m_Capture.m_Auto.imageOrientation = 0;
}

void CDlg_Camera::OnButton_Delete()
{
	// TODO: �ڴ����ӿؼ�֪ͨ�����������
	//if (m_Capture.m_nPhotoNo > 0)
	//{
	//	m_Capture.m_nPhotoNo -= 1;  m_Capture.m_strBarcode.Empty();
	//	CString str;
	//	str.Format("������ %d ��", m_Capture.m_nPhotoNo);
	//	m_sPhotoNo.SetWindowText(str);
	//	// ɾ���ļ�
	//	str = theApp.m_tempFileList.GetAt( theApp.m_tempFileList.GetSize()-1 );
	//	::DeleteFile(str);
	//	str.Replace("~Un", "~UnTh");
	//	::DeleteFile(str);
	//	// ɾ��theApp�б�����������
	//	theApp.m_tempFileList.RemoveAt( theApp.m_tempFileList.GetSize()-1 );
	//	theApp.m_tempBarcodeList.RemoveAt( theApp.m_tempBarcodeList.GetSize()-1 );
	//	theApp.m_nTempFileCount--;
	//}
	//if (m_Capture.m_nPhotoNo > 0)
	//	m_bDelete.EnableWindow(TRUE);
	//else
	//	m_bDelete.EnableWindow(FALSE);
}


void CDlg_Camera::OnCbnSelchangeCombo_Cameralist()
{
	// TODO: �ڴ����ӿؼ�֪ͨ�����������
	m_Capture.m_bIsPreview = true;  // Ԥ��
	m_Capture.m_bIsAutoClipping = false;  // ����Զ�����״̬
	m_Capture.m_nExposure = m_Capture.m_nBrightness = -10000;  // ����ع����
	StartCamera();
	m_bPhoto.EnableWindow(TRUE);  // Set m_bPhoto
	m_bStop.EnableWindow(FALSE);  // Set m_bStop
}


void CDlg_Camera::OnCbnSelchangeCombo_Imagesize()
{
	// TODO: �ڴ����ӿؼ�֪ͨ�����������
	m_Capture.m_bIsPreview = true;  // Ԥ��
	m_Capture.m_bIsAutoClipping = false;  // ����Զ�����״̬
	StartCamera();
	m_bPhoto.EnableWindow(TRUE);  // Set m_bPhoto
	m_bStop.EnableWindow(FALSE);  // Set m_bStop
}


void CDlg_Camera::OnCbnSelchangeCombo_Imagetype()
{
	// TODO: �ڴ����ӿؼ�֪ͨ�����������
	m_Capture.m_Auto.imageType = m_cbImageType.GetCurSel();
}


void CDlg_Camera::OnCbnSelchangeCombo_Docsize()
{
	// TODO: �ڴ����ӿؼ�֪ͨ�����������
	m_Capture.m_Auto.docSize = m_cbDocList.GetCurSel();
	MapDocSize();
}


void CDlg_Camera::OnCheck_Autophoto()
{
	// TODO: �ڴ����ӿؼ�֪ͨ�����������
	OnButton_Stop();

	if (m_cAutoClip.GetCheck() == BST_CHECKED)  // Auto photo
	{
		m_Capture.m_Auto.autoClip = true;
		m_cbDocList.EnableWindow(FALSE);
	}
	else
	{
		m_Capture.m_Auto.autoClip = false;
		m_cbDocList.EnableWindow(TRUE);
	}
}


void CDlg_Camera::OnCheck_Autoenhance()
{
	// TODO: �ڴ����ӿؼ�֪ͨ�����������
	if (m_cAutoEnhance.GetCheck() == BST_CHECKED)  // Auto document enhancement
		m_Capture.m_Auto.autoEnhance = true;
	else
		m_Capture.m_Auto.autoEnhance = false;
}


void CDlg_Camera::OnCheck_Autorotate()
{
	// TODO: �ڴ����ӿؼ�֪ͨ�����������
	if (m_cAutoRotate.GetCheck() == BST_CHECKED)  // Auto Rotate
		m_Capture.m_Auto.autoRotate = true;
	else
		m_Capture.m_Auto.autoRotate = false;
}


void CDlg_Camera::OnButton_Help()
{
	// TODO: �ڴ����ӿؼ�֪ͨ�����������
	if ( m_bIsHelp )  // ����״̬
	{
		m_bIsHelp = false;

		m_Capture.m_bIsPreview = true;  // true: Preview
		m_Capture.m_bIsAutoClipping = false;  // ����Զ�����״̬
		StartCamera();
		m_bPhoto.EnableWindow(TRUE);  // Set m_bPhoto
		m_bStop.EnableWindow(FALSE);  // Set m_bStop
		m_bHelp.SetWindowText("����");
	}
	else
	{
		m_bIsHelp = true;

		m_Capture.StopCamera();  // Stop current camera and Start maybe a new one
		m_Capture.m_bIsPreview = true;  // true: Preview
		m_Capture.m_bIsAutoClipping = false;  // ����Զ�����״̬
		m_bPhoto.EnableWindow(FALSE);  // Set m_bPhoto
		m_bStop.EnableWindow(FALSE);  // Set m_bStop
		m_bHelp.SetWindowText("����");
		CString strHelp;
		strHelp  = " ��׼�����̺�����棬���������ǡ��ɷ���һ�Ų����ĵ����������ȡ�\n";
		strHelp += "   �����ĵ�Ҫ��ѡ��ͼ�����ء��ĵ���ת����ǿ�����������\n\n";
		strHelp += " ���������ա����������ϵ��ĵ�����������ա�������������״̬��\n";
		strHelp += "   �����ĵ���ϵͳ���Զ���������ĵ����Զ����ա�������һ����\n";
		strHelp += "   ������һ�ţ�ϵͳ����������ա��������ͣ�������������ġ�\n\n";
		strHelp += " ���ֶ����ա����ĵ�����������Ϻ��ʵ�λ�ã���������ա���\n\n";
		strHelp += " ���������ա�ѡ�񡾱������ա���ȡ�����ա���\n\n";
		strHelp += " ������Ԥ��������Ԥ�����򡣡����ˡ�ɾ��ͼ��\n\n";
		/*if (m_Capture.m_bActive == false)
		{
		CString errMsg;
		if ( theApp.m_strCameraMysherSN.IsEmpty() )
		errMsg.Format(" ��������Ȩ��������δ��Ȩ, �����Լ�������ȫ�����ܣ�������ͼ��ˮӡ��\n\n");
		else
		errMsg.Format(" ��������Ȩ��������δ��Ȩ, ���кš�%s����\n   �����Լ�������ȫ�����ܣ�������ͼ��ˮӡ��\n\n", theApp.m_strCameraMysherSN);
		strHelp += errMsg;
		}*/
		strHelp += "\n                                     ��������ء���ť��������";
		//::MessageBox(this->m_hWnd,TEXT("SetWindowText!"),MB_CAPTION,MB_OK);
		m_sPreviewWnd.SetWindowText(strHelp);
	}
}


BOOL CDlg_Camera::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ�����ר�ô����/����û���
	if (pMsg->message >= WM_KEYFIRST && pMsg->message <= WM_KEYLAST)
	{
		if (pMsg->wParam == VK_RETURN)
			return TRUE;
		if (pMsg->wParam == VK_ESCAPE)  // ESC������������ -> ��ͣ
		{
			if (m_Capture.m_Auto.autoClip == true && m_Capture.m_bIsAutoClipping == true)  // ����������
				OnButton_Stop();
			return TRUE;
		}
		else if (pMsg->wParam == VK_SPACE)  // �ո�����ֶ����� -> ����
		{
			OnButton_Photo(); 
			return TRUE;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CDlg_Camera::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: �ڴ�������Ϣ������������/�����Ĭ��ֵ
	switch ( pScrollBar->GetDlgCtrlID() )
	{
	case IDC_SLIDER_EXPOSURE:
		m_Capture.SetExposure( &m_slExposure );
		break;
	case IDC_SLIDER_BRIGHTNESS:
		m_Capture.SetBrightness( &m_slBrightness );
		break;
	}

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CDlg_Camera::OnButton_Camvideo()
{
	// TODO: �ڴ����ӿؼ�֪ͨ�����������
	m_Capture.ConfigCameraPin(this->m_hWnd);
}
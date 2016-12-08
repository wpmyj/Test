// CDlg_Camera.cpp : 实现文件
//

#include "stdafx.h"
#include "UDS Scanner.h"
#include "CDlg_Camera.h"
#include "afxdialogex.h"
#include "public.h"

extern void GetFilePath( char* szFileName, char* szFilePath);
extern vector<CUST_IMAGEINFO> g_vecCust_ImageInfo;
// CDlg_Camera 对话框

IMPLEMENT_DYNAMIC(CDlg_Camera, CDialogEx)

CDlg_Camera::CDlg_Camera(MFC_UI* pUI, CWnd* pParent /*=NULL*/)
	:m_pUI(pUI), CDialogEx(CDlg_Camera::IDD, pParent)
{

}

CDlg_Camera::~CDlg_Camera()
{
	//::MessageBox(NULL,TEXT("~CDlg_Camera()"),MB_CAPTION,MB_OK);
	//m_Capture.StopCamera();
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
	ON_BN_CLICKED(IDOK, &CDlg_Camera::OnOk)
	ON_MESSAGE(WM_IMAGEREADY, OnImageReady)
	ON_MESSAGE(WM_IMAGESAVED, OnImageSaved)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDCANCEL, &CDlg_Camera::OCancel)
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
	case 4:  // 身份证
		m_Capture.m_Auto.docWidth = 28;
		m_Capture.m_Auto.docHeight = 25;
		break;
	default:  // 自定义
		m_Capture.m_Auto.docWidth = m_ini.CamDocWidth;  // 自定义手动拍摄宽度
		m_Capture.m_Auto.docHeight = m_ini.CamDocHeight;  // 自定义手动拍摄高度
		break;
	}
}


// CDlg_Camera 消息处理程序


BOOL CDlg_Camera::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CenterWindow();
	ReadCameraSettingFromINI();
	//MessageBox("after ReadCameraSettingFromINI");

	// Mysher Camera License
	//theApp.CheckMysherLicense(theApp.m_OCXPath);  // 麦哲速拍仪信息存放在theApp中
	//m_Capture.m_bActive = theApp.m_bCameraMysherActive;
	//if ( !m_pMainWnd->m_ini.LicenseCode.IsEmpty() )  // 有注册码(激活码)
	//{
	//	// 有注册码(激活码)：MyScan.INI配置项、或界面手工设置，比较注册码是否一致
	//	if (theApp.GenerateLicenseCode(theApp.m_strCameraMysherSN) != m_pMainWnd->m_ini.LicenseCode)
	//		m_Capture.m_bActive = false;
	//	else
	//		m_Capture.m_bActive = true;
	//}

	// 初始化界面选项: 图像类型、智能拍摄、自动旋转、文档增强
	m_cbImageType.InsertString(0, "彩色");  // Set m_cbImageType
	m_cbImageType.InsertString(1, "灰度");
	//m_cbImageType.AddString("自动");
	if (m_ini.CamImageType == 1) { // 灰度
		m_Capture.m_Auto.imageType = 1;
	}
	else {//if (m_pMainWnd->m_ini.CamImageType == 2)  // 彩色
		m_Capture.m_Auto.imageType = 0;
	}
	m_cbImageType.SetCurSel(m_Capture.m_Auto.imageType);

	m_Capture.m_Auto.autoClip = m_ini.CamAutoClip;
	if (m_Capture.m_Auto.autoClip == true)  // Set m_cAutoClip: 智能裁剪-旋转拍摄
	{
		m_cAutoClip.SetCheck(BST_CHECKED);
		m_cbDocList.EnableWindow(FALSE);
	}
	else
	{
		m_cAutoClip.SetCheck(BST_UNCHECKED);
		m_cbDocList.EnableWindow(TRUE);
	}
	m_Capture.m_Auto.autoRotate = m_ini.CamAutoRotate;
	if (m_Capture.m_Auto.autoRotate == true)  // Set m_cAutoRotate: 自动文档旋转
		m_cAutoRotate.SetCheck(BST_CHECKED);
	else
		m_cAutoRotate.SetCheck(BST_UNCHECKED);
	m_Capture.m_Auto.autoEnhance = m_ini.CamAutoEnhance;
	if (m_Capture.m_Auto.autoEnhance == true)  // Set m_cAutoEnhance: 自动文档增强
		m_cAutoEnhance.SetCheck(BST_CHECKED);
	else
		m_cAutoEnhance.SetCheck(BST_UNCHECKED);
	m_cbDocList.InsertString(0, "A3 420×297mm");  // Set m_cbDocList
	m_cbDocList.InsertString(1, "A4 297×210mm");
	m_cbDocList.InsertString(2, "B5 257×182mm");
	m_cbDocList.InsertString(3, "A5 210×148cm");
	m_cbDocList.InsertString(4, "身份证 95×60mm");
	m_cbDocList.InsertString(5, "自定义");
	m_Capture.m_Auto.docSize = m_ini.CamDocSize;
	m_cbDocList.SetCurSel(m_Capture.m_Auto.docSize);
	MapDocSize();

	// 初始化参数：摄像机、像素、预览方向
	m_Capture.m_Auto.strCamrea = m_ini.Camera;  // 默认摄像机
	m_Capture.m_Auto.strSize = m_ini.CamFrameSize;  // 默认像素
	m_Capture.m_Auto.imageOrientation = m_ini.CamOrientation;  // 默认预览方向
	//m_Capture.m_Auto.fastPreview = m_ini.CamFastPreview;
	// 初始化参数：条码类型、条码格式、图像文件路径、压缩比
	//m_Capture.m_strBarcodeType = m_ini.BarcodeType;
	//m_Capture.m_strBarcodeFormat = m_pMainWnd->m_ini.BarcodeFormat;
	//m_Capture.m_nBarcodeLength = m_pMainWnd->m_ini.BarcodeLength;
	//m_Capture.m_bMultiBarcode = m_pMainWnd->m_ini.MultiBarcode;
	//m_Capture.m_bBarcodeRotate = m_pMainWnd->m_ini.BarcodeRotate;
	
	//if (!PathIsDirectory(m_ini.CamTempPath)) // 判断输入路径是否合理
	//{	
	//	::MessageBox(NULL,TEXT("Path is not dir"),MB_CAPTION,MB_OK);
	//	
	//}

	CreateDir(m_ini.CamTempPath);
	m_Capture.m_strImagePath = m_ini.CamTempPath;
	//::MessageBox(NULL,TEXT(m_Capture.m_strImagePath),MB_CAPTION,MB_OK);

	//if (m_pMainWnd->m_ini.SaveHighQu.m_nQuality = 80;  // Set Hight Quality
	//else
	//	m_Capture.m_nQuality = m_pMainWnd->m_ini.Quality;  // Set m_nQuality

	m_Capture.m_nExposure = m_ini.CamExposure;  // Set m_nExposure
	m_Capture.m_nBrightness =m_ini.CamBrightness;  // Set m_nBrightness

	m_Capture.m_hDlgWnd = this->GetSafeHwnd();
	m_Capture.m_hPreviewWnd = m_sPreviewWnd.GetSafeHwnd();
	if ( !m_Capture.CreateComInstance() ) {  // Initialize DirectShow
		MessageBox(TEXT("您的Windows不支持摄像机!"), TEXT(MB_CAPTION));
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
		MessageBox(TEXT("未连接摄像机!"), TEXT(MB_CAPTION));
		EndDialog(IDCANCEL);
	}

	CString msg;
	msg.Format("已拍摄 %d张", m_Capture.m_nPhotoNo);
	m_sPhotoNo.SetWindowText(msg);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
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
	// TODO: 在此添加控件通知处理程序代码
	if (m_cAutoClip.GetCheck() == BST_CHECKED)  // Auto Clip
		m_Capture.m_Auto.autoClip = true;  // 智能拍摄
	else
		m_Capture.m_Auto.autoClip = false;  // 手动拍摄

	m_bPhoto.EnableWindow(FALSE);  // Set m_bPhoto
	if (m_Capture.m_Auto.autoClip == true)  // 智能拍摄
	{
		MessageBox(TEXT("第一步: 移除拍摄垫上的所有文档;\n第二步: 按【确定】键进入自动抓拍状态."), MB_CAPTION);
		m_Capture.m_bIsPreview = true;  // 预览
		m_Capture.m_bIsAutoClipping = false;  // 清除自动裁切状态
		m_Capture.m_bCaptureBackground = true;  // 抓拍背景图像
		m_bStop.EnableWindow(TRUE);  // Set m_bStop
	}
	else //if (m_Capture.m_Auto.autoClip == false)  // 手动拍摄
	{
		m_Capture.m_bIsPreview = false;  // 保存图像文件
		if (m_Capture.m_Auto.fastPreview == true)  // 快速预览：以所选图像尺寸重启摄像机
			StartCamera();
		m_bStop.EnableWindow(FALSE);  // Set m_bStop
	}
}


void CDlg_Camera::OnButton_Stop()
{
	// TODO: 在此添加控件通知处理程序代码
	m_Capture.m_bIsPreview = true;  // true: Preview
	m_Capture.m_bIsAutoClipping = false;  // 清除自动裁切状态
	if ( m_Capture.m_bIsAutoClipping && m_Capture.m_Auto.fastPreview )  // 快速预览自动裁切状态：重启摄像机
		StartCamera();
	m_bPhoto.EnableWindow(TRUE);  // Set m_bPhoto
	m_bStop.EnableWindow(FALSE);  // Set m_bStop
}


void CDlg_Camera::OnButton_Camimage()
{
	// TODO: 在此添加控件通知处理程序代码
	m_Capture.ConfigCameraPin(this->m_hWnd);
}

void CDlg_Camera::OnButton_Adjust()
{
	// TODO: 在此添加控件通知处理程序代码
	m_Capture.m_Auto.imageOrientation += 1;
	if (m_Capture.m_Auto.imageOrientation > 3)  // 0, 1-90, 2-180, 3-270
		m_Capture.m_Auto.imageOrientation = 0;
}

void CDlg_Camera::OnButton_Delete()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_Capture.m_nPhotoNo > 0)
	{
		m_Capture.m_nPhotoNo -= 1;  m_Capture.m_strBarcode.Empty();
		CString str;
		str.Format("已拍摄 %d 张", m_Capture.m_nPhotoNo);
		m_sPhotoNo.SetWindowText(str);
		// 删除文件
		//str = theApp.m_tempFileList.GetAt( theApp.m_tempFileList.GetSize()-1 );
		//::DeleteFile(str);
		//str.Replace("~Un", "~UnTh");
		//::DeleteFile(str);
		// 删除theApp中保存的相关数据
		//theApp.m_tempFileList.RemoveAt( theApp.m_tempFileList.GetSize()-1 );
		//theApp.m_tempBarcodeList.RemoveAt( theApp.m_tempBarcodeList.GetSize()-1 );
		//theApp.m_nTempFileCount--;
	}
	if (m_Capture.m_nPhotoNo > 0)
		m_bDelete.EnableWindow(TRUE);
	else
		m_bDelete.EnableWindow(FALSE);
}


void CDlg_Camera::OnCbnSelchangeCombo_Cameralist()
{
	// TODO: 在此添加控件通知处理程序代码
	m_Capture.m_bIsPreview = true;  // 预览
	m_Capture.m_bIsAutoClipping = false;  // 清除自动裁切状态
	m_Capture.m_nExposure = m_Capture.m_nBrightness = -10000;  // 清除曝光参数
	StartCamera();
	m_bPhoto.EnableWindow(TRUE);  // Set m_bPhoto
	m_bStop.EnableWindow(FALSE);  // Set m_bStop
}


void CDlg_Camera::OnCbnSelchangeCombo_Imagesize()
{
	// TODO: 在此添加控件通知处理程序代码
	m_Capture.m_bIsPreview = true;  // 预览
	m_Capture.m_bIsAutoClipping = false;  // 清除自动裁切状态
	StartCamera();
	m_bPhoto.EnableWindow(TRUE);  // Set m_bPhoto
	m_bStop.EnableWindow(FALSE);  // Set m_bStop
}


void CDlg_Camera::OnCbnSelchangeCombo_Imagetype()
{
	// TODO: 在此添加控件通知处理程序代码
	m_Capture.m_Auto.imageType = m_cbImageType.GetCurSel();
}


void CDlg_Camera::OnCbnSelchangeCombo_Docsize()
{
	// TODO: 在此添加控件通知处理程序代码
	m_Capture.m_Auto.docSize = m_cbDocList.GetCurSel();
	MapDocSize();
}


void CDlg_Camera::OnCheck_Autophoto()
{
	// TODO: 在此添加控件通知处理程序代码
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
	// TODO: 在此添加控件通知处理程序代码
	if (m_cAutoEnhance.GetCheck() == BST_CHECKED)  // Auto document enhancement
		m_Capture.m_Auto.autoEnhance = true;
	else
		m_Capture.m_Auto.autoEnhance = false;
}


void CDlg_Camera::OnCheck_Autorotate()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_cAutoRotate.GetCheck() == BST_CHECKED)  // Auto Rotate
		m_Capture.m_Auto.autoRotate = true;
	else
		m_Capture.m_Auto.autoRotate = false;
}


void CDlg_Camera::OnButton_Help()
{
	// TODO: 在此添加控件通知处理程序代码
	if ( m_bIsHelp )  // 帮助状态
	{
		m_bIsHelp = false;

		m_Capture.m_bIsPreview = true;  // true: Preview
		m_Capture.m_bIsAutoClipping = false;  // 清除自动裁切状态
		StartCamera();
		m_bPhoto.EnableWindow(TRUE);  // Set m_bPhoto
		m_bStop.EnableWindow(FALSE);  // Set m_bStop
		m_bHelp.SetWindowText("帮助");
	}
	else
	{
		m_bIsHelp = true;

		m_Capture.StopCamera();  // Stop current camera and Start maybe a new one
		m_Capture.m_bIsPreview = true;  // true: Preview
		m_Capture.m_bIsAutoClipping = false;  // 清除自动裁切状态
		m_bPhoto.EnableWindow(FALSE);  // Set m_bPhoto
		m_bStop.EnableWindow(FALSE);  // Set m_bStop
		m_bHelp.SetWindowText("返回");
		CString strHelp;
		strHelp  = " 【准备】铺好拍摄垫，连接速拍仪。可放上一张测试文档，调整亮度。\n";
		strHelp += "   根据文档要求，选择图像、像素、文档旋转或增强等拍摄参数。\n\n";
		strHelp += " 【智能拍照】清除拍摄垫上的文档，点击【拍照】进入智能连拍状态。\n";
		strHelp += "   放入文档，系统将自动检测放入的文档，自动拍照。拿走上一张再\n";
		strHelp += "   放入下一张，系统继续检测拍照。点击【暂停】结束智能连拍。\n\n";
		strHelp += " 【手动拍照】将文档放在拍摄垫上合适的位置，点击【拍照】。\n\n";
		strHelp += " 【结束拍照】选择【保存拍照】或【取消拍照】。\n\n";
		strHelp += " 【调整预览】调整预览方向。【回退】删除图像。\n\n";
		/*if (m_Capture.m_bActive == false)
		{
		CString errMsg;
		if ( theApp.m_strCameraMysherSN.IsEmpty() )
		errMsg.Format(" 【软件授权】速拍仪未授权, 您可以继续测试全部功能，但拍摄图像含水印。\n\n");
		else
		errMsg.Format(" 【软件授权】速拍仪未授权, 序列号【%s】。\n   您可以继续测试全部功能，但拍摄图像含水印。\n\n", theApp.m_strCameraMysherSN);
		strHelp += errMsg;
		}*/
		strHelp += "\n                                     点击【返回】按钮结束帮助";
		//::MessageBox(this->m_hWnd,TEXT("SetWindowText!"),MB_CAPTION,MB_OK);
		m_sPreviewWnd.SetWindowText(strHelp);
	}
}


BOOL CDlg_Camera::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message >= WM_KEYFIRST && pMsg->message <= WM_KEYLAST)
	{
		if (pMsg->wParam == VK_RETURN)
			return TRUE;
		if (pMsg->wParam == VK_ESCAPE)  // ESC键：智能拍摄 -> 暂停
		{
			if (m_Capture.m_Auto.autoClip == true && m_Capture.m_bIsAutoClipping == true)  // 智能拍摄中
				OnButton_Stop();
			return TRUE;
		}
		else if (pMsg->wParam == VK_SPACE)  // 空格键：手动拍摄 -> 拍照
		{
			OnButton_Photo(); 
			return TRUE;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CDlg_Camera::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
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
	// TODO: 在此添加控件通知处理程序代码
	m_Capture.ConfigCameraPin(this->m_hWnd);
}

void CDlg_Camera::SetCapValue(void)
{
	m_pUI->SetCapValueFloat(UDSCAP_DOCS_IN_ADF,m_Capture.m_nPhotoNo);  // 设置待传图片数量
	//m_pUI->SetCapValueInt(ICAP_XRESOLUTION, g_vecCust_ImageInfo[m_Capture.m_nPhotoNo].XResolution);
	//m_pUI->SetCapValueInt(ICAP_YRESOLUTION, g_vecCust_ImageInfo[m_Capture.m_nPhotoNo].YResolution);
}


void CDlg_Camera::OnOk()
{
	// TODO: 在此添加控件通知处理程序代码
	SetCapValue();
	if(m_pUI->m_bSetup)  // EnableDSOnly
	{
		m_pUI->Save();
	}
	else  
	{
		m_pUI->Scan();
	}
	//::MessageBox(NULL,TEXT("OnOK!"),MB_CAPTION,MB_OK);
	//OnClose();
	CDialogEx::OnOK();
}

void CDlg_Camera::ReadCameraSettingFromINI()
{
	char szINIPath[MAX_PATH];  // INI文件路径

	GetFilePath(FILENAME_INI,szINIPath);

	INI_CAMERA tempINI;

	//long
	tempINI.CamExposure    = (long)GetPrivateProfileInt(INI_APP_CAMERASETTING,INI_KEY_CAMEXPOSURE,0,szINIPath);
	tempINI.CamBrightness  = (long)GetPrivateProfileInt(INI_APP_CAMERASETTING,INI_KEY_CAMBRIGHTNESS,0,szINIPath);
	tempINI.CamImageType   = (long)GetPrivateProfileInt(INI_APP_CAMERASETTING,INI_KEY_CAMIMAGETYPE,0,szINIPath);
	tempINI.CamDocSize     = (long)GetPrivateProfileInt(INI_APP_CAMERASETTING,INI_KEY_CAMDOCSIZE,0,szINIPath);
	tempINI.CamDocWidth    = (long)GetPrivateProfileInt(INI_APP_CAMERASETTING,INI_KEY_CAMDOCWIDTH,0,szINIPath);
	tempINI.CamDocHeight   = (long)GetPrivateProfileInt(INI_APP_CAMERASETTING,INI_KEY_CAMDOCHEIGHT,0,szINIPath);
	tempINI.CamOrientation = (long)GetPrivateProfileInt(INI_APP_CAMERASETTING,INI_KEY_CAMORIENTATION,0,szINIPath);


  CString strTemp;
	int nMaxLength = 512;

	//CString
	GetPrivateProfileString(INI_APP_CAMERASETTING,INI_KEY_CAMERA,TEXT(""),strTemp.GetBufferSetLength(nMaxLength),nMaxLength,szINIPath);
	tempINI.Camera = strTemp;

	GetPrivateProfileString(INI_APP_CAMERASETTING,INI_KEY_CAMFRAMESIZE,TEXT(""),strTemp.GetBufferSetLength(nMaxLength),nMaxLength,szINIPath);
	tempINI.CamFrameSize = strTemp;

	GetPrivateProfileString(INI_APP_CAMERASETTING,INI_KEY_CAMTEMPPATH,TEXT(""),strTemp.GetBufferSetLength(nMaxLength),nMaxLength,szINIPath);
	tempINI.CamTempPath = strTemp;

	//bool
	GetPrivateProfileString(INI_APP_CAMERASETTING,INI_KEY_CAMAUTOCLIP,TEXT(""),strTemp.GetBufferSetLength(nMaxLength),nMaxLength,szINIPath);
	if (strTemp.Find(TEXT("Y")) >= 0) {
		tempINI.CamAutoClip = true;
	} 
	else {
		tempINI.CamAutoClip = false;
	}

	GetPrivateProfileString(INI_APP_CAMERASETTING,INI_KEY_CAMAUTOENHANCE,TEXT(""),strTemp.GetBufferSetLength(nMaxLength),nMaxLength,szINIPath);
	if (strTemp.Find(TEXT("Y")) >= 0) {
		tempINI.CamAutoEnhance = true;
	} 
	else {
		tempINI.CamAutoEnhance = false;
	}

	GetPrivateProfileString(INI_APP_CAMERASETTING,INI_KEY_CAMAUTOROTATE,TEXT(""),strTemp.GetBufferSetLength(nMaxLength),nMaxLength,szINIPath);
	if (strTemp.Find(TEXT("Y")) >= 0) {
		tempINI.CamAutoRotate = true;
	} 
	else {
		tempINI.CamAutoRotate = false;
	}

	strTemp.ReleaseBuffer(nMaxLength);
	m_ini = tempINI;
}

LRESULT CDlg_Camera::OnImageReady(WPARAM wParam, LPARAM lParam)
{
	m_Capture.m_bIsPreview = false;  // 转入保存状态
	if (m_Capture.m_Auto.fastPreview == true)
		StartCamera();

	return TRUE;
}

LRESULT CDlg_Camera::OnImageSaved(WPARAM wParam, LPARAM lParam)
{
	m_Capture.m_bIsPreview = true;  // 转入预览状态
	if (m_Capture.m_Auto.fastPreview == true)
		StartCamera();

	CString msg;
	if (m_Capture.m_strBarcode == "***")
		msg.Format("第 %d张", m_Capture.m_nPhotoNo);
	else
		msg.Format("第 %d张[%s]", m_Capture.m_nPhotoNo, m_Capture.m_strBarcode);
	m_sPhotoNo.SetWindowText(msg);
	if (m_Capture.m_nPhotoNo > 0)  // Set m_bDelete
		m_bDelete.EnableWindow(TRUE);
	else
		m_bDelete.EnableWindow(FALSE);
	if (m_Capture.m_Auto.autoClip == false)  // 手动拍摄
		m_bPhoto.EnableWindow(TRUE);  // Set m_bPhoto

	return TRUE;
}


void CDlg_Camera::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//::MessageBox(NULL,TEXT("OnClose()!"),MB_CAPTION,MB_OK);
	CString str;
	str.Format("确定退出吗？");
	if (AfxMessageBox(str, MB_YESNO) == IDNO) // 选择"否",则不退出
	{
		return ;
	}
	m_pUI->Cancel();
	CDialogEx::OnClose();
}


void CDlg_Camera::OCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	m_pUI->Cancel();
	CDialogEx::OnCancel();
}


bool CDlg_Camera::CreateDir(CString strFloderPath)
{
	if (!PathFileExists(strFloderPath))
	{
		//::MessageBox(NULL,TEXT("文件夹不存在!"),MB_CAPTION,MB_OK);
		if (!CreateDirectory(strFloderPath, NULL))
		{	
			return false;
		}
		return true;
	}
	
	return true;		
}


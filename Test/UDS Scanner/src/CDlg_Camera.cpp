// CDlg_Camera.cpp : 实现文件
//

#include "stdafx.h"
#include "UDS Scanner.h"
#include "CDlg_Camera.h"
#include "afxdialogex.h"
#include "public.h"

#include "ximage.h"  // CXImage
#include <vector>
#pragma comment(lib,"cximage.lib")

#define THUMB_WIDTH  80
#define THUMB_HEIGHT 100

extern void GetFilePath( char* szFileName, char* szFilePath);
extern std::vector<std::string> g_vector_imagepath;

//extern vector<CUST_IMAGEINFO> g_vecCust_ImageInfo;
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
	ClearAndDeleteDir(m_Capture.m_strTempPath); // 清空临时文件夹
	g_vector_imagepath.swap(std::vector<std::string>());  // 清除容器并最小化它的容量
	//g_vecCust_ImageInfo.swap( vector<CUST_IMAGEINFO>() );  // 清除容器并最小化它的容量

	m_Capture.m_nPhotoNo = 0;
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
	DDX_Control(pDX, IDC_BUTTON_HELP, m_bHelp);
	DDX_Control(pDX, IDC_STATIC_UI, m_sPreviewWnd);
	DDX_Control(pDX, IDC_STATIC_PHOTONO, m_sPhotoNo);
	DDX_Control(pDX, IDC_CHECK_AUTOPHOTO, m_cAutoClip);
	DDX_Control(pDX, IDC_CHECK_AUTOENHANCE, m_cAutoEnhance);
	DDX_Control(pDX, IDC_CHECK_AUTOROTATE, m_cAutoRotate);
	DDX_Control(pDX, IDC_SLIDER_EXPOSURE, m_slExposure);
	DDX_Control(pDX, IDC_SLIDER_BRIGHTNESS, m_slBrightness);
	DDX_Control(pDX, IDC_LIST_THUNMBNAIL, m_listctrl);
}


BEGIN_MESSAGE_MAP(CDlg_Camera, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_PHOTO, &CDlg_Camera::OnButton_Photo)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CDlg_Camera::OnButton_Stop)
	ON_BN_CLICKED(IDC_BUTTON_CAMIMAGE, &CDlg_Camera::OnButton_Camimage)
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
	ON_BN_CLICKED(IDOK, &CDlg_Camera::OnOk)
	ON_MESSAGE(WM_IMAGEREADY, OnImageReady)
	ON_MESSAGE(WM_IMAGESAVED, OnImageSaved)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDCANCEL, &CDlg_Camera::OnCancel)
	ON_COMMAND(ID_IMAGE_DELETE, &CDlg_Camera::OnImageDelete)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_THUNMBNAIL, &CDlg_Camera::OnNMRClickListThunmbnail)
	ON_COMMAND(ID_IMAGE_LEFT90, &CDlg_Camera::OnImageLeft90)
	ON_COMMAND(ID_IMAGE_RIGHT90, &CDlg_Camera::OnImageRight90)
	ON_COMMAND(ID_IMAGE_FLIPVERTICAL, &CDlg_Camera::OnImageFlipvertical)
	ON_COMMAND(ID_IMAGE_MIRROR, &CDlg_Camera::OnImageMirror)
	ON_BN_CLICKED(IDC_BUTTON_CAMERA_SETTING, &CDlg_Camera::OnButton_CameraSetting)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_CAMERA_SLIDER_THRESHOLD, &CDlg_Camera::OnNMCustomdrawCamera_Slider_Threshold)
	ON_CBN_SELCHANGE(IDC_CAMERA_COMBO_DPI, &CDlg_Camera::OnCbnSelchange_Camera_ComboDpi)
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
	ReadCameraSettingFromINI();
	AdjustWindow();
	InitControls();
 	//UpdateControls(); 
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
	m_cbImageType.InsertString(0, "黑白");  // Set m_cbImageType
	m_cbImageType.InsertString(1, "灰度");
	m_cbImageType.InsertString(2, "彩色");
	//m_cbImageType.AddString("自动");
	//if (m_ini.CamImageType == 1) { // 灰度
	//	m_Capture.m_Auto.imageType = 1;
	//}
	//else {//if (m_pMainWnd->m_ini.CamImageType == 0)  // 彩色
	//	m_Capture.m_Auto.imageType = 0;
	//}
	m_Capture.m_Auto.imageType = m_ini.CamImageType;
	m_cbImageType.SetCurSel(m_Capture.m_Auto.imageType);

	if ( 0 == m_Capture.m_Auto.imageType ) // B&W
	{
		((CSliderCtrl*)GetDlgItem(IDC_CAMERA_SLIDER_THRESHOLD))->EnableWindow(TRUE);
	}
	else
	{
		((CSliderCtrl*)GetDlgItem(IDC_CAMERA_SLIDER_THRESHOLD))->EnableWindow(FALSE);
	}

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
	m_cbDocList.InsertString(3, "A5 210×148mm");
	m_cbDocList.InsertString(4, "身份证 95×60mm");
	m_cbDocList.InsertString(5, "自定义");
	m_Capture.m_Auto.docSize = m_ini.CamDocSize;
	m_cbDocList.SetCurSel(m_Capture.m_Auto.docSize);
	MapDocSize();

	// 初始化参数：摄像机、像素、预览方向、DPI
	m_Capture.m_Auto.strCamrea = m_ini.Camera;  // 默认摄像机
	m_Capture.m_Auto.strSize = m_ini.CamFrameSize;  // 默认像素
	m_Capture.m_Auto.imageOrientation = m_ini.CamOrientation;  // 默认预览方向
	m_Capture.m_Auto.threshold = m_ini.threshold;  // 默认阈值
	((CSliderCtrl*)GetDlgItem(IDC_CAMERA_SLIDER_THRESHOLD))->SetPos(m_Capture.m_Auto.threshold);

	//m_Capture.m_Auto.XDPI = m_ini.XDPI;  // 默认XDPI
	//m_Capture.m_Auto.YDPI = m_ini.YDPI;  // 默认XDPI
	((CComboBox*)GetDlgItem(IDC_CAMERA_COMBO_DPI))->SetCurSel(m_ini.DpiNumber);
	CString strCBText;
	((CComboBox*)GetDlgItem(IDC_CAMERA_COMBO_DPI))->GetLBText(m_ini.DpiNumber, strCBText);  // 获取序号对应字符串
	int nval = _ttoi(strCBText);  // CString 转 int
	m_Capture.m_Auto.XDPI = m_Capture.m_Auto.YDPI = (long)nval;
	//m_Capture.m_Auto.fastPreview = m_ini.CamFastPreview;
	// 初始化参数：条码类型、条码格式、图像文件路径、压缩比
	//m_Capture.m_strBarcodeType = m_ini.BarcodeType;
	//m_Capture.m_strBarcodeFormat = m_pMainWnd->m_ini.BarcodeFormat;
	//m_Capture.m_nBarcodeLength = m_pMainWnd->m_ini.BarcodeLength;
	//m_Capture.m_bMultiBarcode = m_pMainWnd->m_ini.MultiBarcode;
	//m_Capture.m_bBarcodeRotate = m_pMainWnd->m_ini.BarcodeRotate;
	
	// 保存另存图片的文件夹路径
	if ( true == m_ini.SaveAs )
	{
		m_Capture.m_bSaveAs = m_ini.SaveAs;
		if ('\\' != m_ini.SaveAsPath.Right(1) )
		{
			m_ini.SaveAsPath.Format("%s%s", m_ini.SaveAsPath, "\\");
		}
		CreateDir(m_ini.SaveAsPath);
		m_Capture.m_strImagePath = m_ini.SaveAsPath;
	}

	// 保存临时保存图片的文件夹路径
	{
		TCHAR szPath[MAX_PATH];
		GetTempSavePath(szPath);
		m_Capture.m_strTempPath = szPath;
	}
	
	m_Capture.m_nQuality = m_ini.JpegQuality;  // Set m_nQuality
	m_Capture.m_nExposure = m_ini.CamExposure;  // Set m_nExposure
	m_Capture.m_nBrightness =m_ini.CamBrightness;  // Set m_nBrightness

	m_Capture.m_hDlgWnd = this->GetSafeHwnd();
	m_Capture.m_hPreviewWnd = m_sPreviewWnd.GetSafeHwnd();
	if ( !m_Capture.CreateComInstance() ) {  // Initialize DirectShow
		MessageBox(TEXT("您的Windows不支持摄像机!"), TEXT(MB_CAPTION));
	}

	m_bIsHelp = false;
	m_bStop.EnableWindow(FALSE);  // Set m_bStop
	//m_bDelete.EnableWindow(FALSE);  // Set m_bDelete
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

//格式
void CDlg_Camera::OnButton_Camimage()
{
	// TODO: 在此添加控件通知处理程序代码
	m_Capture.ConfigCameraPin(this->m_hWnd);
}

// 属性
void CDlg_Camera::OnButton_CameraSetting()
{
	// TODO: 在此添加控件通知处理程序代码
	m_Capture.ConfigCameraFilter(this->m_hWnd);
}


void CDlg_Camera::OnButton_Adjust()
{
	// TODO: 在此添加控件通知处理程序代码
	m_Capture.m_Auto.imageOrientation += 1;
	if (m_Capture.m_Auto.imageOrientation > 3)  // 0, 1-90, 2-180, 3-270
		m_Capture.m_Auto.imageOrientation = 0;
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
	if ( 0 == m_Capture.m_Auto.imageType ) // B&W
	{
		 ((CSliderCtrl*)GetDlgItem(IDC_CAMERA_SLIDER_THRESHOLD))->EnableWindow(TRUE);
	}
	else
	{
		((CSliderCtrl*)GetDlgItem(IDC_CAMERA_SLIDER_THRESHOLD))->EnableWindow(FALSE);
	}
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
		strHelp  = "\n\n 【准备】铺好拍摄垫，连接速拍仪。可放上一张测试文档，调整亮度。\n";
		strHelp += "  根据文档要求，选择图像、像素、文档旋转或增强等拍摄参数。\n\n";
		strHelp += " 【智能拍照】清除拍摄垫上的文档，点击【拍照】进入智能连拍状态。\n";
		strHelp += "  放入文档，系统将自动检测放入的文档，自动拍照。拿走上一张再\n";
		strHelp += "  放入下一张，系统继续检测拍照。点击【暂停】结束智能连拍。\n\n";
		strHelp += " 【手动拍照】将文档放在拍摄垫上合适的位置，点击【拍照】。\n\n";
		strHelp += " 【结束拍照】选择【确定】或【取消】。\n\n";
		strHelp += " 【预览】调整预览方向。\n\n";
		/*if (m_Capture.m_bActive == false)
		{
		CString errMsg;
		if ( theApp.m_strCameraMysherSN.IsEmpty() )
		errMsg.Format(" 【软件授权】速拍仪未授权, 您可以继续测试全部功能，但拍摄图像含水印。\n\n");
		else
		errMsg.Format(" 【软件授权】速拍仪未授权, 序列号【%s】。\n   您可以继续测试全部功能，但拍摄图像含水印。\n\n", theApp.m_strCameraMysherSN);
		strHelp += errMsg;
		}*/
		strHelp += "  点击【返回】按钮结束帮助。\n\n";
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


void CDlg_Camera::SetCapValue(void)
{
	m_pUI->SetCapValueFloat(UDSCAP_DOCS_IN_ADF, (float)m_Capture.m_nPhotoNo);  // 设置待传图片数量
	m_pUI->SetCapValueInt(ICAP_PIXELTYPE, (int)m_Capture.m_Auto.imageType);  // 设置图片类型

	//int pixeltype;
	//switch(m_Capture.m_Auto.imageType)  
	//{
	//case 1: // 灰度
	//	pixeltype = TWPT_GRAY;
	//	break;
	//case 0: // 彩色
	//	pixeltype = TWPT_RGB;
	//	break;
	//default:
	//	break;
	//}
	//m_pUI->SetCapValueInt(ICAP_PIXELTYPE, pixeltype);  // 设置图片类型
	
	//MAP_CAP::iterator iter; 
	//for(iter = m_mapCap.begin(); iter != m_mapCap.end(); iter++)
	//{
	//	switch(iter->first)
	//	{
	//	//case ICAP_CONTRAST:
	//	//case ICAP_BRIGHTNESS:
	//	case ICAP_THRESHOLD:
	//		{
	//			m_pUI->SetCapValueFloat(iter->first,iter->second);  // 设置阈值为当前滚动条值
	//			break;
	//		}
	//	//case ICAP_PIXELTYPE:
	//	case ICAP_XRESOLUTION:
	//	case ICAP_YRESOLUTION:
	//	//case CAP_FEEDERENABLED:
	//	//case UDSCAP_MULTIFEEDDETECT:
	//		{
	//			m_pUI->SetCapValueInt(iter->first,(int)iter->second); // 设置重张检测
	//			break;
	//		}	
	//	default:
	//		{
	//			break;
	//		}	
	//	}
	//}
}


void CDlg_Camera::ReadCameraSettingFromINI()
{
	char szINIPath[MAX_PATH];  // INI文件路径

	GetFilePath(FILENAME_INI,szINIPath);

	INI_CAMERA tempINI;

	//long
	tempINI.CamExposure    = (long)GetPrivateProfileInt(INI_APP_CAMERASETTING, INI_KEY_CAMEXPOSURE,    0,szINIPath);
	tempINI.CamBrightness  = (long)GetPrivateProfileInt(INI_APP_CAMERASETTING, INI_KEY_CAMBRIGHTNESS,  0,szINIPath);
	tempINI.CamImageType   = (long)GetPrivateProfileInt(INI_APP_CAMERASETTING, INI_KEY_CAMIMAGETYPE,   0,szINIPath);
	tempINI.CamDocSize     = (long)GetPrivateProfileInt(INI_APP_CAMERASETTING, INI_KEY_CAMDOCSIZE,     1,szINIPath);  // 默认文档大小 = A4
	tempINI.CamDocWidth    = (long)GetPrivateProfileInt(INI_APP_CAMERASETTING, INI_KEY_CAMDOCWIDTH,    0,szINIPath);
	tempINI.CamDocHeight   = (long)GetPrivateProfileInt(INI_APP_CAMERASETTING, INI_KEY_CAMDOCHEIGHT,   0,szINIPath);
	tempINI.CamOrientation = (long)GetPrivateProfileInt(INI_APP_CAMERASETTING, INI_KEY_CAMORIENTATION, 0,szINIPath);
	tempINI.JpegQuality    = (BYTE)GetPrivateProfileInt(INI_APP_CAMERASETTING, INI_KEY_JPEGQUALITY,   75,szINIPath);  // 默认JPEG图片质量 = 75
	tempINI.threshold      = (long)GetPrivateProfileInt(INI_APP_CAMERASETTING, INI_KEY_THRESHOLD,    128,szINIPath);  // 默认阈值 = 128
	tempINI.DpiNumber      = (long)GetPrivateProfileInt(INI_APP_CAMERASETTING, INI_KEY_DPINUMBER,      3,szINIPath);  // 默认图像DPI = 200(3)
	//tempINI.YDPI           = (long)GetPrivateProfileInt(INI_APP_CAMERASETTING, INI_KEY_YDPI,         200,szINIPath);  // 默认图像YDPI = 200

  CString strTemp;
	int nMaxLength = 512;

	//CString
	GetPrivateProfileString(INI_APP_CAMERASETTING,INI_KEY_CAMERA,TEXT(""),strTemp.GetBufferSetLength(nMaxLength),nMaxLength,szINIPath);
	tempINI.Camera = strTemp;

	GetPrivateProfileString(INI_APP_CAMERASETTING,INI_KEY_CAMFRAMESIZE,TEXT(""),strTemp.GetBufferSetLength(nMaxLength),nMaxLength,szINIPath);
	tempINI.CamFrameSize = strTemp;

	GetPrivateProfileString(INI_APP_CAMERASETTING,INI_KEY_CAMTEMPPATH,TEXT(""),strTemp.GetBufferSetLength(nMaxLength),nMaxLength,szINIPath);
	tempINI.SaveAsPath = strTemp;

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

	GetPrivateProfileString(INI_APP_CAMERASETTING,INI_KEY_UPLOADFILEONEBYONE,TEXT(""),strTemp.GetBufferSetLength(nMaxLength),nMaxLength,szINIPath);
	if (strTemp.Find(TEXT("Y")) >= 0) {
		tempINI.UploadFileOnebyOne = true;
	} 
	else {
		tempINI.UploadFileOnebyOne = false;
	}

	GetPrivateProfileString(INI_APP_CAMERASETTING,INI_KEY_SHOWTHUMBNAIL,TEXT(""),strTemp.GetBufferSetLength(nMaxLength),nMaxLength,szINIPath);
	if (strTemp.Find(TEXT("Y")) >= 0) {
		tempINI.ShowThumbnail = true;
	} 
	else {
		tempINI.ShowThumbnail = false;
	}

	GetPrivateProfileString(INI_APP_CAMERASETTING,INI_KEY_SAVEAS,TEXT(""),strTemp.GetBufferSetLength(nMaxLength),nMaxLength,szINIPath);
	if (strTemp.Find(TEXT("Y")) >= 0) {
		tempINI.SaveAs = true;
	} 
	else {
		tempINI.SaveAs = false;
	}


	strTemp.ReleaseBuffer(nMaxLength);
	m_ini = tempINI;
}


void CDlg_Camera::WriteCameraSettingToINI()
{
	char szINIPath[MAX_PATH];  // INI文件路径
	GetFilePath(FILENAME_INI,szINIPath);  // 获取INI文件路径

	//CString
	WritePrivateProfileString(INI_APP_CAMERASETTING,INI_KEY_CAMERA,m_Capture.m_Auto.strCamrea,szINIPath);
	WritePrivateProfileString(INI_APP_CAMERASETTING,INI_KEY_CAMFRAMESIZE,m_Capture.m_Auto.strSize,szINIPath);

	//long->CString
	CString strTemp;
	strTemp.Format("%ld",m_Capture.m_nExposure);
	WritePrivateProfileString(INI_APP_CAMERASETTING,INI_KEY_CAMEXPOSURE,strTemp,szINIPath);

	strTemp.Format("%ld",m_Capture.m_nBrightness);
	WritePrivateProfileString(INI_APP_CAMERASETTING,INI_KEY_CAMBRIGHTNESS,strTemp,szINIPath);

	strTemp.Format("%ld",m_Capture.m_Auto.imageType);
	WritePrivateProfileString(INI_APP_CAMERASETTING,INI_KEY_CAMIMAGETYPE,strTemp,szINIPath);

	strTemp.Format("%ld",m_Capture.m_Auto.docSize);
	WritePrivateProfileString(INI_APP_CAMERASETTING,INI_KEY_CAMDOCSIZE,strTemp,szINIPath);

	strTemp.Format("%ld",m_Capture.m_Auto.imageOrientation);
	WritePrivateProfileString(INI_APP_CAMERASETTING,INI_KEY_CAMORIENTATION,strTemp,szINIPath);

	strTemp.Format("%ld",m_Capture.m_Auto.threshold);
	WritePrivateProfileString(INI_APP_CAMERASETTING,INI_KEY_THRESHOLD,strTemp,szINIPath);

	strTemp.Format("%ld",m_ini.DpiNumber);
	WritePrivateProfileString(INI_APP_CAMERASETTING,INI_KEY_DPINUMBER,strTemp,szINIPath);

	//strTemp.Format("%ld",m_ini.DpiNumber);
	//WritePrivateProfileString(INI_APP_CAMERASETTING,INI_KEY_YDPI,strTemp,szINIPath);

	//bool->CString
	if (m_Capture.m_Auto.autoClip) {
		strTemp = "Y";
	} 
	else {
		strTemp = "N";
	}
	WritePrivateProfileString(INI_APP_CAMERASETTING,INI_KEY_CAMAUTOCLIP,strTemp,szINIPath);

	if (m_Capture.m_Auto.autoEnhance) {
		strTemp = "Y";
	} 
	else {
		strTemp = "N";
	}
	WritePrivateProfileString(INI_APP_CAMERASETTING,INI_KEY_CAMAUTOENHANCE,strTemp,szINIPath);

	if (m_Capture.m_Auto.autoRotate) {
		strTemp = "Y";
	} 
	else {
		strTemp = "N";
	}
	WritePrivateProfileString(INI_APP_CAMERASETTING,INI_KEY_CAMAUTOROTATE,strTemp,szINIPath);
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
	//if (m_Capture.m_strBarcode == "***")
		msg.Format("第 %d张", m_Capture.m_nPhotoNo);
	//else
		//msg.Format("第 %d张[%s]", m_Capture.m_nPhotoNo, m_Capture.m_strBarcode);
	m_sPhotoNo.SetWindowText(msg);
	//if (m_Capture.m_nPhotoNo > 0)  // Set m_bDelete
	//	m_bDelete.EnableWindow(TRUE);
	//else
	//	m_bDelete.EnableWindow(FALSE);
	if (m_Capture.m_Auto.autoClip == false)  // 手动拍摄
		m_bPhoto.EnableWindow(TRUE);  // Set m_bPhoto

	if (true == m_ini.ShowThumbnail) { // 显示缩略图
		LoadThumbNail();
	}

	if (true == m_ini.UploadFileOnebyOne)
	{
		WriteCameraSettingToINI(); 
		if (0 == m_Capture.m_nPhotoNo)  // 一张都没拍取消扫描
		{
			return FALSE;
		}

		SetCapValue();	
		if(m_pUI->m_bSetup)  // EnableDSOnly
		{
			m_pUI->Save();
		}
		else  
		{
			m_pUI->Scan();
		}
	}
	

	return TRUE;
}

void CDlg_Camera::OnOk()
{
	// TODO: 在此添加控件通知处理程序代码 

	WriteCameraSettingToINI(); 
	if (0 == m_Capture.m_nPhotoNo)  // 一张都没拍取消扫描
	{
		OnCancel();
		return;
	}

	SetCapValue();	
	if(m_pUI->m_bSetup)  // EnableDSOnly
	{
		m_pUI->Save();
	}
	else  
	{
		m_pUI->Scan();
	}

	CDialogEx::OnOK();
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


void CDlg_Camera::OnCancel()
{
	// TODO: 在此添加控件通知处理程序代码

	m_pUI->Cancel();
	CDialogEx::OnCancel();
}


bool CDlg_Camera::CreateDir(const CString& strPath)
{
	if (!PathFileExists(strPath))
	{
		//::MessageBox(NULL,TEXT("文件夹不存在!"),MB_CAPTION,MB_OK);
		if (!CreateDirectory(strPath, NULL))
		{	
			return false;
		}
		return true;
	}
	
	return true;		
}

bool CDlg_Camera::ClearAndDeleteDir(const TCHAR* szPath, bool deleteDir/*= false*/)
{
	if ((NULL == szPath))
	{
		return false;
	}

	/*检查输入目录是否是合法目录*/
	if (!PathIsDirectory(szPath))
	{
		return false;
	}

	/*创建源目录中查找文件的通配符*/
	CString strWildcard(szPath);
	if (strWildcard.Right(1) != _T('\\'))
	{
		strWildcard += _T("\\");
	}
	strWildcard += _T("*.*");

	CFileFind ff;  
	BOOL bFound;  
	bFound = ff.FindFile(strWildcard);  

	/*char buf[60];
	itoa(bFound,buf,10);
	::MessageBox(NULL,TEXT(buf),MB_CAPTION,MB_OK);*/


	while(bFound)  
	{  
		bFound = ff.FindNextFile();  
		CString sFilePath = ff.GetFilePath();  
		//::MessageBox(NULL,TEXT(sFilePath),MB_CAPTION,MB_OK);
		if(ff.IsDirectory())  
		{  
			if(!ff.IsDots())  
			{  
				ClearAndDeleteDir(sFilePath);  
			}  
		}  
		else  
		{  
			if(ff.IsReadOnly())  
			{  
				SetFileAttributes(sFilePath, FILE_ATTRIBUTE_NORMAL);  
			}  
			DeleteFile(sFilePath); 
			//::MessageBox(NULL,TEXT("DeleteFile!"),MB_CAPTION,MB_OK);
		}  
	}  
	ff.Close(); 

	//上面已经把文件夹下面的文件全部删除了，如果需要把文件夹也删除掉则加上一下代码 
	if (true == deleteDir)
	{
		SetFileAttributes(szPath, FILE_ATTRIBUTE_NORMAL);  //设置文件夹的属性  
		RemoveDirectory(szPath);  //删除文件夹  
	}
	
	return true;
}

void CDlg_Camera::LoadThumbNail()
{
	//::MessageBox(NULL,TEXT("LoadThumbNail()!"),MB_CAPTION,MB_OK);
	DWORD dwStyle;
	dwStyle = m_listctrl.GetExtendedStyle();
	dwStyle = dwStyle|LVS_SHOWSELALWAYS|LVS_ALIGNTOP|LVS_ICON|LVS_AUTOARRANGE;
	m_listctrl.SetExtendedStyle(dwStyle);
	m_imagelist.Create(THUMB_WIDTH,THUMB_HEIGHT,ILC_COLOR24,0,1);
	m_listctrl.SetImageList(&m_imagelist,LVSIL_NORMAL);

	//remove m_imagelist
	for(int i=0; i<m_imagelist.GetImageCount(); i++) {
		m_imagelist.Remove(i);
	}

	//remove all items from list view
	m_listctrl.DeleteAllItems();

	//set Number of image for m_imagelist
	//m_imagelist.SetImageCount(g_vecCust_ImageInfo.size());
	m_imagelist.SetImageCount(g_vector_imagepath.size());

	char path[MAX_PATH];
	std::vector<std::string>::iterator iter;
	//std::vector<CUST_IMAGEINFO>::iterator iter;

	//重绘false防止重绘闪烁
	m_listctrl.SetRedraw(false);
	int nIndex=0;

	TCHAR szItem[10];
	memset(szItem,0,sizeof(szItem));
	for(iter=g_vector_imagepath.begin();iter!=g_vector_imagepath.end();iter++,nIndex++)
	{
		//m_listctrl.InsertItem(nIndex,iter->imagePath.c_str(),nIndex);
		sprintf_s(szItem,"%d",nIndex+1);
		m_listctrl.InsertItem(nIndex,szItem,nIndex);
	}

	m_listctrl.SetRedraw(true);
	m_listctrl.Invalidate();

	//为背景和边框创建画刷
	HBRUSH hBrushBorder    =::CreateSolidBrush(RGB(220,220,220));
	HBRUSH hBrushBkground  =::CreateSolidBrush(RGB(255,255,255));

	//边框尺寸
	RECT rcBorder;
	rcBorder.left=rcBorder.top  =0;
	rcBorder.right              =THUMB_WIDTH;
	rcBorder.bottom             =THUMB_HEIGHT;

	const float fRatio=(float)THUMB_HEIGHT/THUMB_WIDTH; //const是为了防止只读变量在程序运行过程中“遭受”改变

	int XDest,YDest,nDestWidth,nDestHeight;
	nIndex=0;

	for(iter=g_vector_imagepath.begin();iter!=g_vector_imagepath.end();iter++,nIndex++)
	{ 
	  SSTRCPY(path, MAX_PATH, (*iter).c_str());
		int nImageType = GetTypeFromFileName(path); 

		if(nImageType == CXIMAGE_FORMAT_UNKNOWN)
			continue;
		CxImage image(path,nImageType);//把图像加载到image中

		if (CXIMAGE_FORMAT_TIF == nImageType)
		{
			image.Negative();  // 黑白图像需要黑白反转
		}
		//::MessageBox(NULL,TEXT(*iter),"*iter",MB_OK);
		//::MessageBox(NULL,TEXT(path),"LoadThumbNail",MB_OK);
		//image.Save("C:\\11.jpg",CXIMAGE_FORMAT_JPG);
		//CxImage image;
		//image.CreateFromHANDLE(*iter);//把图像加载到image中
		//image.Rotate180();  // CxIamge倒序存储图片数据
		if(image.IsValid()==false)
			continue;

		//计算矩形rect适应画板
		const float fImgRatio=(float)image.GetHeight()/image.GetWidth();
		if(fImgRatio>fRatio)
		{
			nDestWidth=THUMB_HEIGHT/fImgRatio;
			XDest=(THUMB_WIDTH-nDestWidth)/2;
			YDest=0;
			nDestHeight=THUMB_HEIGHT;
		}
		else
		{
			XDest=0;
			nDestWidth=THUMB_WIDTH;
			nDestHeight=THUMB_WIDTH*fImgRatio;
			YDest=(THUMB_HEIGHT-nDestHeight)/2;
		}

		//创建CClientDC
		CClientDC cdc(this);//创建一个与this指针关联的对象的设备上下文对象，this代表本类对象的指针
		HDC hDC=::CreateCompatibleDC(cdc.m_hDC);//创建一个与指定设备兼容的内存设备上下文环境
		HBITMAP bm=CreateCompatibleBitmap(cdc.m_hDC,THUMB_WIDTH,THUMB_HEIGHT);//创建与指定设备环境相兼容的位图
		HBITMAP pOldBitmapImage=(HBITMAP)SelectObject(hDC,bm);//选定bm到DC中

		//draw background
		::FillRect(hDC,&rcBorder,hBrushBkground);

		//draw image
		image.Stretch(hDC,XDest,YDest,nDestWidth,nDestHeight);

		//draw border
		::FrameRect(hDC,&rcBorder,hBrushBorder);

		SelectObject(hDC,pOldBitmapImage);

		//attach to bitmap and replace image in CImageList
		CBitmap bitmap;
		bitmap.Attach(bm);
		m_imagelist.Replace(nIndex,&bitmap,NULL);

		//redraw only a current item for removing flicker and fast speed
		m_listctrl.RedrawItems(nIndex,nIndex);

		//release used DC and Object
		DeleteDC(hDC);
		DeleteObject(bm);
	}
	DeleteObject(hBrushBorder);
	DeleteObject(hBrushBkground);
	m_listctrl.Invalidate();

}


int CDlg_Camera::GetTypeFromFileName(const CString filename)
{
	CString ext = filename.Right(filename.GetLength()-filename.ReverseFind('.')-1);

	int type = 0;
	if (ext == "bmp")     type = CXIMAGE_FORMAT_BMP;

#if CXIMAGE_SUPPORT_JPG    
	else if (ext=="jpg"||ext=="jpeg") type = CXIMAGE_FORMAT_JPG;    //3
#endif

#if CXIMAGE_SUPPORT_GIF                                       //如果满足#if后面的条件，就编译#if和#endif之间的程序
	else if (ext == "gif")    type = CXIMAGE_FORMAT_GIF;
#endif

#if CXIMAGE_SUPPORT_PNG
	else if (ext == "png")    type = CXIMAGE_FORMAT_PNG;
#endif

#if CXIMAGE_SUPPORT_MNG
	else if (ext=="mng"||ext=="jng") type = CXIMAGE_FORMAT_MNG;
#endif

#if CXIMAGE_SUPPORT_ICO
	else if (ext == "ico")    type = CXIMAGE_FORMAT_ICO;
#endif

#if CXIMAGE_SUPPORT_TIF
	else if (ext=="tiff"||ext=="tif") type = CXIMAGE_FORMAT_TIF;
#endif

#if CXIMAGE_SUPPORT_TGA
	else if (ext=="tga")    type = CXIMAGE_FORMAT_TGA;
#endif

#if CXIMAGE_SUPPORT_PCX
	else if (ext=="pcx")    type = CXIMAGE_FORMAT_PCX;
#endif

#if CXIMAGE_SUPPORT_WBMP
	else if (ext=="wbmp")    type = CXIMAGE_FORMAT_WBMP;
#endif

#if CXIMAGE_SUPPORT_WMF
	else if (ext=="wmf"||ext=="emf") type = CXIMAGE_FORMAT_WMF;
#endif

#if CXIMAGE_SUPPORT_J2K
	else if (ext=="j2k"||ext=="jp2") type = CXIMAGE_FORMAT_J2K;
#endif

#if CXIMAGE_SUPPORT_JBG
	else if (ext=="jbg")    type = CXIMAGE_FORMAT_JBG;
#endif

#if CXIMAGE_SUPPORT_JP2
	else if (ext=="jp2"||ext=="j2k") type = CXIMAGE_FORMAT_JP2;
#endif

#if CXIMAGE_SUPPORT_JPC
	else if (ext=="jpc"||ext=="j2c") type = CXIMAGE_FORMAT_JPC;
#endif

#if CXIMAGE_SUPPORT_PGX
	else if (ext=="pgx")    type = CXIMAGE_FORMAT_PGX;
#endif

#if CXIMAGE_SUPPORT_RAS
	else if (ext=="ras")    type = CXIMAGE_FORMAT_RAS;
#endif

#if CXIMAGE_SUPPORT_PNM
	else if (ext=="pnm"||ext=="pgm"||ext=="ppm") type = CXIMAGE_FORMAT_PNM;
#endif

	else type = CXIMAGE_FORMAT_UNKNOWN;

	return type;

}

void CDlg_Camera::OnImageDelete()
{
	// TODO: 在此添加命令处理程序代码

	int nSelectedIndex = m_listctrl.GetNextItem(-1, LVNI_SELECTED);
	if (nSelectedIndex == -1)
	{
		AfxMessageBox("未选中页码");
		return;
	}

	//int nCount = g_vecCust_ImageInfo.size();  // 获取图片数量
	int nCount = g_vector_imagepath.size();

	//////////////////////////////////////////////////////////////////////

	int nImageCount = m_imagelist.GetImageCount();

	LVITEM lvitem;
	for (int nIndex = 0; nIndex < nImageCount - 1; nIndex++)
	{
		if (nIndex < nSelectedIndex)
			continue;

		m_imagelist.Copy(nIndex, nIndex + 1);

		lvitem.iItem = nIndex;
		lvitem.mask = LVIF_IMAGE;
		m_listctrl.GetItem(&lvitem);
		lvitem.iImage = nIndex;
		m_listctrl.SetItem(&lvitem);

		CString text = m_listctrl.GetItemText(nIndex + 1, 0);
		m_listctrl.SetItemText(nIndex, 0, text);
	}
	m_imagelist.SetImageCount(nImageCount - 1);
	m_listctrl.DeleteItem(nImageCount - 1);

	////////////////////////////////////////////////////////////////////////////////

	int num = 0;
	//std::vector<CUST_IMAGEINFO>::iterator it;
	std::vector<std::string>::iterator it;
	for (it = g_vector_imagepath.begin(); it != g_vector_imagepath.end(); it++)
	{
		if (num < nSelectedIndex)
		{
			num++;
			continue;
		}

		if (num < nCount - 1)
			g_vector_imagepath[num] = g_vector_imagepath[num + 1];

		if (num == nCount - 1)
		{
			g_vector_imagepath.erase(it);
			if (m_Capture.m_nPhotoNo > 0)
			{
				m_Capture.m_nPhotoNo -= 1; 
				m_Capture.m_strBarcode.Empty();
				CString str;
				str.Format("已拍摄 %d 张", m_Capture.m_nPhotoNo);
				m_sPhotoNo.SetWindowText(str);
			}
			break;
		}

		num++;
	}

	if (nSelectedIndex > 0)
	{
		m_listctrl.SetItemState(-1, 0, LVIS_SELECTED);
		m_listctrl.SetItemState(nSelectedIndex - 1, LVIS_SELECTED, LVIS_SELECTED);											
	}
	else
	{
		m_listctrl.SetItemState(-1, 0, LVIS_SELECTED);
		m_listctrl.SetItemState(nSelectedIndex, LVIS_SELECTED, LVIS_SELECTED);
	}

	m_listctrl.Invalidate();
}


void CDlg_Camera::OnNMRClickListThunmbnail(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	CMenu menu, *pSubMenu; //定义下面要用到的cmenu对象 
	menu.LoadMenu(IDR_POPMENU_THUMBNAIL); //装载自定义的右键菜单 
	pSubMenu = menu.GetSubMenu(0); //获取第一个弹出菜单 
	CPoint oPoint; //定义一个用于确定光标位置的位置 
	GetCursorPos(&oPoint); //获取当前光标的位置 
	pSubMenu->TrackPopupMenu(TPM_LEFTALIGN, oPoint.x, oPoint.y, this); //在指定位置显示弹出菜单

	*pResult = 0;
}

void CDlg_Camera::ImageHandle(enum_image_handle eMethod)
{
	//::MessageBox(NULL,TEXT("ImageHandle()!"),MB_CAPTION,MB_OK);
	//获得当前选中图片的索引
	int nSelectedIndex = m_listctrl.GetNextItem(-1, LVNI_SELECTED);

	if (-1 == nSelectedIndex) {
		return;
	}
	
	TCHAR szTempPath[MAX_PATH]; 
	SSTRCPY(szTempPath, MAX_PATH, g_vector_imagepath[nSelectedIndex].c_str());

	int nImageType = GetTypeFromFileName(szTempPath); 

	if(nImageType == CXIMAGE_FORMAT_UNKNOWN) {
		return;
	}

	CxImage *pImage = new CxImage();
	pImage->Load(szTempPath,nImageType);//把图像加载到image中
	//::MessageBox(NULL,TEXT(szTempPath),MB_CAPTION,MB_OK);
	//CxImage image;
	//image.CreateFromHANDLE(*iter);//把图像加载到image中
	//image.Rotate180();  // CxIamge倒序存储图片数据
	if(pImage->IsValid()==false) {
		return;
	}

	switch (eMethod)
	{
	case left90: {
		pImage->RotateLeft();
		break;
		}	
	case right90: {
		pImage->RotateRight();
		break;
		}		
	case flip: {
		pImage->Flip();
		break;
		}	
	case mirror: {
		pImage->Mirror();
		break;
		}
	default: { 
		break;
		}		
	}

	//g_vector_imagepath[nSelectedIndex] = pImage->CopyToHandle();
	pImage->Save(szTempPath, nImageType);
	delete pImage;
	LoadThumbNail();
}


void CDlg_Camera::OnImageLeft90()
{
	// TODO: 在此添加命令处理程序代码
	ImageHandle(left90);
}


void CDlg_Camera::OnImageRight90()
{
	// TODO: 在此添加命令处理程序代码
	ImageHandle(right90);
}

// 垂直翻转
void CDlg_Camera::OnImageFlipvertical()
{
	// TODO: 在此添加命令处理程序代码
	ImageHandle(flip);
}


void CDlg_Camera::OnImageMirror()
{
	// TODO: 在此添加命令处理程序代码
	ImageHandle(mirror);
}

void CDlg_Camera::AdjustWindow()
{
	if (false == m_ini.ShowThumbnail)  // 不显示缩略图
	{
		GetDlgItem(IDC_LIST_THUNMBNAIL)->EnableWindow(FALSE);
		GetDlgItem(IDC_LIST_THUNMBNAIL)->ShowWindow(SW_HIDE);
		CRect recDlg1,recDlg2,recList;

		GetClientRect(&recDlg1);	
		ClientToScreen(&recDlg1);//转换为荧幕坐标   
		GetDlgItem(IDC_LIST_THUNMBNAIL)->GetClientRect(&recList);
		ClientToScreen(&recList);//转换为荧幕坐标   
		int listHeight = recList.Height();

		recDlg2.left = recDlg1.left;
		recDlg2.top = recDlg1.top;
		recDlg2.right = recDlg1.right;
		recDlg2.bottom = recDlg1.bottom - listHeight;  // 窗口新高度=窗口原高度-List控件高度

		MoveWindow(&recDlg2);  // 设置窗口新Rect
	}
	else
	{
		GetDlgItem(IDC_LIST_THUNMBNAIL)->EnableWindow(TRUE);
		GetDlgItem(IDC_LIST_THUNMBNAIL)->ShowWindow(SW_SHOW);
	}
	
	CenterWindow();  // 窗口居中
}

//void CDlg_Camera::UpdateControls(void)
//{
//	//float fMin,fMax,fStep;
//	//int nCapValue,nCapIndex;
//	//const FloatVector* lstCapValuesFlt;
//	//// 获取并设置阈值范围
//	//m_pUI->GetCapRangeFloat(ICAP_THRESHOLD, fMin, fMax, fStep);
//	//((CSliderCtrl*)GetDlgItem(IDC_CAMERA_SLIDER_THRESHOLD))->SetRange((int)fMin, (int)fMax);
//	//((CSliderCtrl*)GetDlgItem(IDC_CAMERA_SLIDER_THRESHOLD))->SetTicFreq((int)fStep);
//
//	//// 更新滑块位置
//	//nCapValue = (int)(m_pUI->GetCapValueFloat(ICAP_THRESHOLD));
//	//((CSliderCtrl*)GetDlgItem(IDC_CAMERA_SLIDER_THRESHOLD))->SetPos(nCapValue);
//	////m_Capture.m_Auto.threshold = nCapValue;
//
//	//// DPI
//	////m_pUI->SetCapValueFloat(ICAP_XRESOLUTION,0);
//	//((CComboBox*)GetDlgItem(IDC_CAMERA_COMBO_DPI))->ResetContent();
//	//nCapIndex = m_pUI->GetCurrentCapIndex(ICAP_XRESOLUTION);
//	//lstCapValuesFlt = m_pUI->GetValidCapFloat(ICAP_XRESOLUTION);
//
//	//for(unsigned int i=0; i<lstCapValuesFlt->size();i++)
//	//{
//	//	CString strTemp;
//	//	strTemp.Format("%d",(int)lstCapValuesFlt->at(i));
//	//	((CComboBox*)GetDlgItem(IDC_CAMERA_COMBO_DPI))->InsertString(i,strTemp);
//	//}
//	//((CComboBox*)GetDlgItem(IDC_CAMERA_COMBO_DPI))->SetCurSel(nCapIndex);
//	////OnCbnSelchange_Camera_ComboDpi();
//
//	//UpdateData(FALSE);  // 更新控件
//}






void CDlg_Camera::OnNMCustomdrawCamera_Slider_Threshold(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE); //接收数据
	int sldValue = ((CSliderCtrl*)GetDlgItem(IDC_CAMERA_SLIDER_THRESHOLD))->GetPos(); //获取滑块的当前位置
	
	m_Capture.m_Auto.threshold = sldValue;
	//m_mapCap[ICAP_THRESHOLD] = (float)sldValue;
	//CString str;
	//m_edit_threshold.SetWindowText(str); //在编辑框同步显示滚动条值
	//SetDlgItemText(IDC_BASE_EDIT_THRESHOLD, str);
	//UpdateControls();

	*pResult = 0;
}


void CDlg_Camera::OnCbnSelchange_Camera_ComboDpi()
{
	// TODO: 在此添加控件通知处理程序代码
	int nIndex =  ((CComboBox*)GetDlgItem(IDC_CAMERA_COMBO_DPI))->GetCurSel();
	CString strCBText; 
	((CComboBox*)GetDlgItem(IDC_CAMERA_COMBO_DPI))->GetLBText(nIndex, strCBText);
	int nval = _ttoi(strCBText);  // CString 转 int
	m_Capture.m_Auto.XDPI = (long)nval;
	m_Capture.m_Auto.YDPI = (long)nval;
	m_ini.DpiNumber = nIndex;

	//m_mapCap[ICAP_XRESOLUTION] = (float)nval;
	//m_mapCap[ICAP_YRESOLUTION] = (float)nval;
	//UpdateControls();
}

void CDlg_Camera::InitControls(void)
{
	// 设置阈值
	const int threshold_min = 0;
	const int threshold_max = 255;
	const int step = 1;
	((CSliderCtrl*)GetDlgItem(IDC_CAMERA_SLIDER_THRESHOLD))->SetRange(threshold_min, threshold_max);
	((CSliderCtrl*)GetDlgItem(IDC_CAMERA_SLIDER_THRESHOLD))->SetTicFreq(step);
	((CSliderCtrl*)GetDlgItem(IDC_CAMERA_SLIDER_THRESHOLD))->SetPos(128); // 默认阈值 = 128


	// DPI
	//TCHAR* szDPI[8] = {
	//	TEXT("50"),  TEXT("100"), TEXT("150"), TEXT("200"), 
	//	TEXT("300"), TEXT("400"), TEXT("500"), TEXT("600") };

	((CComboBox*)GetDlgItem(IDC_CAMERA_COMBO_DPI))->ResetContent();
	//for (unsigned int i = 0; i < sizeof(szDPI); i++ )
	//{
	//	((CComboBox*)GetDlgItem(IDC_CAMERA_COMBO_DPI))->InsertString(i, szDPI[i]);
	//}
	((CComboBox*)GetDlgItem(IDC_CAMERA_COMBO_DPI))->InsertString(0, TEXT("50"));
	((CComboBox*)GetDlgItem(IDC_CAMERA_COMBO_DPI))->InsertString(1, TEXT("100"));
	((CComboBox*)GetDlgItem(IDC_CAMERA_COMBO_DPI))->InsertString(2, TEXT("150"));
	((CComboBox*)GetDlgItem(IDC_CAMERA_COMBO_DPI))->InsertString(3, TEXT("200"));
	((CComboBox*)GetDlgItem(IDC_CAMERA_COMBO_DPI))->InsertString(4, TEXT("300"));
	((CComboBox*)GetDlgItem(IDC_CAMERA_COMBO_DPI))->InsertString(5, TEXT("400"));
	((CComboBox*)GetDlgItem(IDC_CAMERA_COMBO_DPI))->InsertString(6, TEXT("500"));
	((CComboBox*)GetDlgItem(IDC_CAMERA_COMBO_DPI))->InsertString(7, TEXT("600"));


	((CComboBox*)GetDlgItem(IDC_CAMERA_COMBO_DPI))->SetCurSel(3); // 默认DPI = 200

	UpdateData(FALSE);
}


bool CDlg_Camera::GetTempSavePath(TCHAR* pszPath)
{
	TCHAR szTempPath[MAX_PATH];
	memset(szTempPath, 0, MAX_PATH);
	GetTempPath(MAX_PATH, szTempPath);

	SSTRCAT(szTempPath, MAX_PATH, MB_CAPTION);
	SSTRCAT(szTempPath, MAX_PATH, TEXT("\\"));

	if (false == CreateDir(szTempPath))
	{
		MessageBox(TEXT("创建临时文件夹失败！"));
		return false;
	}

	SSTRCPY(pszPath, MAX_PATH, szTempPath);
	return true;
}




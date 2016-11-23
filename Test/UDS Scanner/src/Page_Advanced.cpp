// Page_Advanced.cpp : 实现文件
//

#include "stdafx.h"
#include "UDS Scanner.h"
#include "Page_Advanced.h"
#include "afxdialogex.h"


/** 滑动条最小值 */
#define SLIDER_MIN -100  
/** 滑动条最大值 */
#define SLIDER_MAX  100 

/** 去除斑点最小值 */
#define SLIDER_MIN_SENSITIVITY 1 
/** 去除斑点最大值 */
#define SLIDER_MAX_SENSITIVITY 30 

/** 底色保留最小值 */
#define SLIDER_MIN_THRESHOLD 1 
/** 底色保留最大值 */
#define SLIDER_MAX_THRESHOLD 255 

/** Gamma校正最小值 */
#define SLIDER_MIN_GAMMA 10 
/** Gamma校正最大值 */
#define SLIDER_MAX_GAMMA 255 

// CPage_Advanced 对话框

IMPLEMENT_DYNAMIC(CPage_Advanced, CPropertyPage)

	CPage_Advanced::CPage_Advanced(MFC_UI *pUI)
	: m_pUI(pUI), CPropertyPage(CPage_Advanced::IDD)
{

}

CPage_Advanced::~CPage_Advanced()
{
}

void CPage_Advanced::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ADVANCED_COMBO_STANDARDSIZES, m_combo_standardsizes);
	DDX_Control(pDX, IDC_ADVANCED_COMBO_ORIENTATION, m_combo_orientation);
	DDX_Control(pDX, IDC_ADVANCED_EDIT_CUSTOMWIDTH, m_edit_custom_width);
	DDX_Control(pDX, IDC_ADVANCED_EDIT_CUSTOMHEIGHT, m_edit_custom_height);
	DDX_Control(pDX, IDC_ADVANCED_COMBO_UINTS, m_combo_uints);
	DDX_Control(pDX, IDC_ADVANCED_COMBO_ROTATE, m_combo_rotate);
	DDX_Control(pDX, IDC_ADVANCED_COMBO_SPLITIMG, m_combo_splitimage);
	DDX_Control(pDX, IDC_ADVANCED_COMBO_RESOLUTION, m_combo_resolution);
	DDX_Control(pDX, IDC_ADVANCED_COMBO_BINARIZATION, m_combo_binarization);
	DDX_Control(pDX, IDC_ADVANCED_EDIT_BRIGHTNESS, m_edit_brightness);
	DDX_Control(pDX, IDC_ADVANCED_EDIT_CONTRAST, m_edit_contrast);
	DDX_Control(pDX, IDC_ADVANCED_EDIT_SENSITIVE_THRESHOLD, m_edit_sensitive_threshold);
	DDX_Control(pDX, IDC_ADVANCED_EDIT_SENSITIVE_GAMMA, m_edit_gamma);
	DDX_Control(pDX, IDC_CHECK_MULTISTREAM, m_check_multistream);
	DDX_Control(pDX, IDC_ADVANCED_SLIDER_BRIGHTNESS, m_slider_brightness);
	DDX_Control(pDX, IDC_ADVANCED_SLIDER_CONTRAST, m_slider_contrast);
	DDX_Control(pDX, IDC_ADVANCED_SLIDER_SENSITIVE_THRESHOLD, m_slider_sensitive_threshold);
	DDX_Control(pDX, IDC_ADVANCED_SLIDER_SENSITIVE_GAMMA, m_slider_gamma);
	DDX_Control(pDX, IDC_CHECK_BACKBW, m_check_backbw);
	DDX_Control(pDX, IDC_CHECK_BACKCOLOR, m_check_backcolor);
	DDX_Control(pDX, IDC_CHECK_BACKGRAY, m_check_backgray);
	DDX_Control(pDX, IDC_CHECK_FRONTBW, m_check_frontbw);
	DDX_Control(pDX, IDC_CHECK_FRONTCOLOR, m_check_frontcolor);
	DDX_Control(pDX, IDC_CHECK_FRONTGRAY, m_check_frontgray);
	DDX_Control(pDX, IDC_CHECK_REMOVEBLANK, m_check_removeblank);
	DDX_Control(pDX, IDC_CHECK_REMOVEPUNCH, m_check_removepunch);
	DDX_Control(pDX, IDC_CHECK_SHARPEN, m_check_sharpen);
	DDX_Control(pDX, IDC_CHECK_MIRROR, m_check_mirror);
	DDX_Control(pDX, IDC_CHECK_REMOVEBACK, m_check_removeback);
	DDX_Control(pDX, IDC_CHECK_REMOVEDESCREEN, m_check_removedescreen);
	DDX_Control(pDX, IDC_CHECK_REMOVEDEMOISE, m_check_removedenoise);
	DDX_Control(pDX, IDC_CHECK_AUTOCROP, m_check_autocrop);
}


BEGIN_MESSAGE_MAP(CPage_Advanced, CPropertyPage)
	ON_CBN_SELCHANGE(IDC_ADVANCED_COMBO_STANDARDSIZES, &CPage_Advanced::OnCbnSelchangeAdvanced_Combo_Standardsizes)
	ON_CBN_SELCHANGE(IDC_ADVANCED_COMBO_ORIENTATION, &CPage_Advanced::OnCbnSelchangeAdvanced_Combo_Orientation)
	ON_CBN_SELCHANGE(IDC_ADVANCED_COMBO_UINTS, &CPage_Advanced::OnCbnSelchangeAdvanced_Combo_Uints)
	ON_CBN_SELCHANGE(IDC_ADVANCED_COMBO_RESOLUTION, &CPage_Advanced::OnCbnSelchangeAdvanced_Combo_Resolution)
	ON_CBN_SELCHANGE(IDC_ADVANCED_COMBO_BINARIZATION, &CPage_Advanced::OnCbnSelchangeAdvanced_Combo_Binarization)
	ON_CBN_SELCHANGE(IDC_ADVANCED_COMBO_ROTATE, &CPage_Advanced::OnCbnSelchangeAdvanced_Combo_Rotate)
	ON_CBN_SELCHANGE(IDC_ADVANCED_COMBO_SPLITIMG, &CPage_Advanced::OnCbnSelchangeAdvanced_Combo_SpiltImage)
	ON_BN_CLICKED(IDC_CHECK_MULTISTREAM, &CPage_Advanced::OnAdvanced_Btn_Check_Multistream)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_ADVANCED_SLIDER_BRIGHTNESS, &CPage_Advanced::OnNMCustomdrawAdvanced_Slider_Brightness)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_ADVANCED_SLIDER_CONTRAST, &CPage_Advanced::OnNMCustomdrawAdvanced_Slider_Contrast)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_ADVANCED_SLIDER_SENSITIVE_THRESHOLD, &CPage_Advanced::OnNMCustomdrawAdvanced_Slider_SensitiveThreshold)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_ADVANCED_SLIDER_SENSITIVE_GAMMA, &CPage_Advanced::OnNMCustomdrawAdvanced_Slider_Gamma)
	ON_EN_CHANGE(IDC_ADVANCED_EDIT_CONTRAST, &CPage_Advanced::OnEnChangeAdvanced_Edit_Contrast)
	ON_EN_CHANGE(IDC_ADVANCED_EDIT_BRIGHTNESS, &CPage_Advanced::OnEnChangeAdvanced_Edit_Brightness)
	ON_EN_CHANGE(IDC_ADVANCED_EDIT_SENSITIVE_THRESHOLD, &CPage_Advanced::OnEnChangeAdvanced_Edit_SensitiveThreshold)
	ON_EN_CHANGE(IDC_ADVANCED_EDIT_SENSITIVE_GAMMA, &CPage_Advanced::OnEnChangeAdvanced_Edit_Gamma)
	ON_BN_CLICKED(IDC_CHECK_FRONTCOLOR, &CPage_Advanced::OnAdvanced_Btn_Check_FrontColor)
	ON_BN_CLICKED(IDC_CHECK_FRONTGRAY, &CPage_Advanced::OnAdvanced_Btn_Check_FrontGray)
	ON_BN_CLICKED(IDC_CHECK_FRONTBW, &CPage_Advanced::OnAdvanced_Btn_Check_FrontBW)
	ON_BN_CLICKED(IDC_CHECK_BACKGRAY, &CPage_Advanced::OnAdvanced_Btn_Check_BackGray)
	ON_BN_CLICKED(IDC_CHECK_BACKCOLOR, &CPage_Advanced::OnAdvanced_Btn_Check_BackColor)
	ON_BN_CLICKED(IDC_CHECK_BACKBW, &CPage_Advanced::OnAdvanced_Btn_Check_BackBW)
	ON_BN_CLICKED(IDC_CHECK_REMOVEBLANK, &CPage_Advanced::OnAdvanced_Btn_Check_RemoveBlank)
	ON_BN_CLICKED(IDC_CHECK_REMOVEPUNCH, &CPage_Advanced::OnAdvanced_Btn_Check_RemovePunch)
	ON_BN_CLICKED(IDC_CHECK_SHARPEN, &CPage_Advanced::OnAdvanced_Btn_Check_Sharpen)
	ON_BN_CLICKED(IDC_CHECK_MIRROR, &CPage_Advanced::OnAdvanced_Btn_Check_Mirror)
	ON_BN_CLICKED(IDC_CHECK_REMOVEBACK, &CPage_Advanced::OnAdvanced_Btn_Check_RemoveBack)
	ON_BN_CLICKED(IDC_CHECK_REMOVEDEMOISE, &CPage_Advanced::OnAdvanced_Btn_Check_RemoveDenoise)
	ON_BN_CLICKED(IDC_CHECK_REMOVEDESCREEN, &CPage_Advanced::OnAdvanced_Btn_Check_RemoveDescreen)
	ON_BN_CLICKED(IDC_CHECK_AUTOCROP, &CPage_Advanced::OnAdvanced_Btn_Check_AutoCrop)
END_MESSAGE_MAP()

//void CPage_Advanced::OnOK()
//{
//	// TODO: 在此添加专用代码和/或调用基类
//	//SetControl();
//
//	if(m_pUI->m_bSetup)  // EnableDSOnly
//	{
//		m_pUI->Save();
//	}
//	else  
//	{
//		m_pUI->Scan();
//	}
//
//	CPropertyPage::OnOK();
//}

//void CPage_Advanced::SetControl(void)
//{
//	MAP_CAP::iterator iter;
//
//	for(iter = m_advancedmap.begin(); iter != m_advancedmap.end(); iter++)
//	{
//		switch(iter->first)
//		{
//		case ICAP_ORIENTATION:
//			{
//				//AfxMessageBox("纸张方向");
//				m_pUI->SetCapValueInt(iter->first,iter->second);
//				break;
//			}	
//		case ICAP_SUPPORTEDSIZES:
//			{
//				//AfxMessageBox("纸张大小");
//				m_pUI->SetCapValueInt(iter->first,iter->second);
//				break;
//			}
//		case ICAP_UNITS:
//			{
//				//AfxMessageBox("纸张单位");
//				m_pUI->SetCapValueInt(iter->first,iter->second);
//				break;
//			}
//		case ICAP_XRESOLUTION:
//			{
//				//AfxMessageBox("分辨率");
//				m_pUI->SetCapValueInt(iter->first,iter->second);
//				break;
//			}
//		case ICAP_ROTATION:
//			{
//				//AfxMessageBox("图像旋转");
//				m_pUI->SetCapValueInt(iter->first,iter->second);
//				break;
//			}
//			//未完
//		}
//	}
//}

void CPage_Advanced::UpdateControls(void)
{
	int nCapIndex;
	const IntVector* lstCapValues;
	const FloatVector* lstCapValuesFlt;

  //纸张设置-纸张方向
	m_combo_orientation.ResetContent();  // 清空内容
	nCapIndex = m_pUI->GetCurrentCapIndex(ICAP_ORIENTATION);
	lstCapValues = m_pUI->GetValidCap(ICAP_ORIENTATION);
	for(unsigned int i=0; i<lstCapValues->size();i++)
	{
		switch(lstCapValues->at(i))
		{
		case TWOR_PORTRAIT:
			m_combo_orientation.InsertString(i,"纵向");
			break;
		case TWOR_LANDSCAPE:
			m_combo_orientation.InsertString(i,"横向");
			break;
		default:
			continue;
		}	
	}
	m_combo_orientation.SetCurSel(nCapIndex);  // 显示默认值

	//纸张设置-纸张大小
	m_combo_standardsizes.ResetContent();  // 清空内容
	nCapIndex = m_pUI->GetCurrentCapIndex(ICAP_SUPPORTEDSIZES);
	lstCapValues = m_pUI->GetValidCap(ICAP_SUPPORTEDSIZES);
	for(unsigned int i=0; i<lstCapValues->size();i++)
	{
		switch(lstCapValues->at(i))
		{
		case TWSS_NONE:
			m_combo_standardsizes.InsertString(i,"自定义");
			break;
		case TWSS_USLETTER:
			m_combo_standardsizes.InsertString(i,"US Letter (8.5\" x 11\")");  //216mm x 280mm
			break;
		case TWSS_USLEGAL:
			m_combo_standardsizes.InsertString(i,"US Legal (8.5\" x 14\")");  //216mm x 356mm
			break;
		case TWSS_PHOT64:
			m_combo_standardsizes.InsertString(i,"照片64 (6\" x 4\")");  //152mm x 102mm
			break;
		case TWSS_PHOT53:
			m_combo_standardsizes.InsertString(i,"照片53 (5\" x 3\")");  //127mm x 76mm
			break;
		case TWSS_A3:
			m_combo_standardsizes.InsertString(i,"ISO A3 (297mm x 420mm)"); //国际标准
			break;
		case TWSS_A4:
			m_combo_standardsizes.InsertString(i,"ISO A4 (210mm x 297mm)"); //国际标准
			break;
		case TWSS_A5:
			m_combo_standardsizes.InsertString(i,"ISO A5 (148mm x 210mm)");
			break;
		case TWSS_A6:
			m_combo_standardsizes.InsertString(i,"ISO A6 (105mm x 148mm)");
			break;
		case TWSS_A7:
			m_combo_standardsizes.InsertString(i,"ISO A7 (74mm x 105mm)");
			break;
		case TWSS_ISOB4:
			m_combo_standardsizes.InsertString(i,"ISO B4 (250mm x 353mm)");
			break;
		case TWSS_ISOB5:
			m_combo_standardsizes.InsertString(i,"ISO B5 (176mm x 250mm)");
			break;
		case TWSS_ISOB6:
			m_combo_standardsizes.InsertString(i,"ISO B6 (125mm x 176mm)");
			break;
		case TWSS_ISOB7:
			m_combo_standardsizes.InsertString(i,"ISO B7 (88mm x 125mm)");
			break;
		case TWSS_JISB4:
			m_combo_standardsizes.InsertString(i,"JIS B4 (257mm x 364mm)");//JIS日本标准
			break;
		case TWSS_JISB5:
			m_combo_standardsizes.InsertString(i,"JIS B5 (182mm x 257mm)");//JIS日本标准
			break;
		case TWSS_JISB6:
			m_combo_standardsizes.InsertString(i,"JIS B6 (128mm x 182mm)");
			break;
		case TWSS_JISB7:
			m_combo_standardsizes.InsertString(i,"JIS B7 (91mm x 128mm)");
			break;
		case UDSCAP_LONGDOCUMENT:
			m_combo_standardsizes.InsertString(i,"长纸模式");
			break;
		case TWSS_MAXSIZE:
			m_combo_standardsizes.InsertString(i,"最大扫描范围");
			break;
		default:
			continue;
		}
	}
	m_combo_standardsizes.SetCurSel(nCapIndex);  // 显示默认值

	if(TWSS_NONE == lstCapValues->at(nCapIndex))  ///<　纸张大小：自定义。
	{
		m_edit_custom_width.EnableWindow(TRUE);
		m_edit_custom_height.EnableWindow(TRUE);
	} 
	else
	{
		m_edit_custom_width.EnableWindow(FALSE);
		m_edit_custom_height.EnableWindow(FALSE);
		//GetDlgItem(IDC_ADVANCED_STATIC_ORIENTATION)->EnableWindow(TRUE);
	}

	// 自定义宽与高
	TW_FRAME frame;
	CString strTemp;

	frame = m_pUI->GetCurrentFrame();
	strTemp.Format("%0.2f",FIX32ToFloat(frame.Right));
	m_edit_custom_width.SetWindowText(strTemp);

	frame = m_pUI->GetCurrentFrame();
	strTemp.Format("%0.2f",FIX32ToFloat(frame.Bottom));
	m_edit_custom_height.SetWindowText(strTemp);

	//纸张设置-单位
	m_combo_uints.ResetContent();  // 清空内容
	nCapIndex = m_pUI->GetCurrentCapIndex(ICAP_UNITS);
	lstCapValues = m_pUI->GetValidCap(ICAP_UNITS);
	for(unsigned int i=0; i<lstCapValues->size();i++)
	{
		switch(lstCapValues->at(i))
		{
		case TWUN_INCHES:
			m_combo_uints.InsertString(i,"Inches"); //英寸
			break;
		case TWUN_PIXELS:
			m_combo_uints.InsertString(i,"Pixels"); //像素
			break;
		case TWUN_CENTIMETERS:
			m_combo_uints.InsertString(i,"Centimeters"); //厘米
			break;
		case TWUN_PICAS:
			m_combo_uints.InsertString(i,"Picas");
			break;
		case TWUN_POINTS:
			m_combo_uints.InsertString(i,"Points");
			break;
		case TWUN_TWIPS:
			m_combo_uints.InsertString(i,"Twips"); //缇
			break;
		default:
			continue;
		}
	}
	m_combo_uints.SetCurSel(nCapIndex);

	//多流输出-二值化
	m_combo_binarization.ResetContent(); //清空内容
	nCapIndex = m_pUI->GetCurrentCapIndex(UDSCAP_BINARIZATION);
	lstCapValues = m_pUI->GetValidCap(UDSCAP_BINARIZATION);
	for(unsigned int i=0; i<lstCapValues->size();i++)
	{
		switch(lstCapValues->at(i))
		{
		case TWBZ_DYNATHRESHOLD:
			m_combo_binarization.InsertString(i,"动态阈值");
			break;
		case TWBZ_FIXEDTHRESHOLD:
			m_combo_binarization.InsertString(i,"固定阈值"); 
			break;
		case TWBZ_HALFTONE1:
			m_combo_binarization.InsertString(i,"半色调1");
			break;
		case TWBZ_HALFTONE2:
			m_combo_binarization.InsertString(i,"半色调2");
			break;
		case TWBZ_HALFTONE3:
			m_combo_binarization.InsertString(i,"半色调3");
			break;
		case TWBZ_HALFTONE4:
			m_combo_binarization.InsertString(i,"半色调4");
			break;
		case TWBZ_HALFTONE5:
			m_combo_binarization.InsertString(i,"半色调5");
			break;
		case TWBZ_ERRORDIFF:
			m_combo_binarization.InsertString(i,"误差扩散");
			break;
		default:
			continue;
		}
	}
	m_combo_binarization.SetCurSel(nCapIndex);

	//多流输出-分辨率
	m_combo_resolution.ResetContent(); //清空内容
	nCapIndex = m_pUI->GetCurrentCapIndex(ICAP_XRESOLUTION);
	lstCapValuesFlt = m_pUI->GetValidCapFloat(ICAP_XRESOLUTION);  //float型的
	for(unsigned int i=0; i<lstCapValuesFlt->size();i++)
	{
		CString strTemp;
		strTemp.Format("%d",(int)lstCapValuesFlt->at(i));
		m_combo_resolution.InsertString(i,strTemp);
	}
	m_combo_resolution.SetCurSel(nCapIndex);


	//图像设置-图像旋转
	m_combo_rotate.ResetContent(); //清空内容
	nCapIndex = m_pUI->GetCurrentCapIndex(ICAP_ROTATION);
	lstCapValues = m_pUI->GetValidCap(ICAP_ROTATION);
	for(unsigned int i=0; i<lstCapValues->size();i++)
	{
		switch(lstCapValues->at(i))
		{
		case TWOR_ROT0:
			m_combo_rotate.InsertString(i,"不旋转图像");
			break;
		case TWOR_ROT90:
			m_combo_rotate.InsertString(i,"顺时针90度"); 
			break;
		case TWOR_ROT180:
			m_combo_rotate.InsertString(i,"顺时针180度");
			break;
		case TWOR_ROT270:
			m_combo_rotate.InsertString(i,"顺时针270度");
			break;
		default:
			continue;
		}
	}
	m_combo_rotate.SetCurSel(nCapIndex);

	//图像设置-图像分割
	m_combo_splitimage.ResetContent(); //清空内容
	nCapIndex = m_pUI->GetCurrentCapIndex(UDSCAP_SPLITIMAGE);
	lstCapValues = m_pUI->GetValidCap(UDSCAP_SPLITIMAGE);
	for(unsigned int i=0; i<lstCapValues->size();i++)
	{
		switch(lstCapValues->at(i))
		{
		case TWSI_NONE:
			m_combo_splitimage.InsertString(i,"不分割图像");
			break;
		case TWSI_HORIZONTAL:
			m_combo_splitimage.InsertString(i,"水平分割图像"); 
			break;
		case TWSI_VERTICAL:
			m_combo_splitimage.InsertString(i,"垂直分割图像");
			break;
		default:
			continue;
		}
	}
	m_combo_splitimage.SetCurSel(nCapIndex);
}


// CPage_Advanced 消息处理程序


BOOL CPage_Advanced::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  在此添加额外的初始化
	UpdateControls();
	InitSliderCtrl(); //初始化滑块

	//多流输出下的选项默认不使用
	m_check_multistream.SetCheck(FALSE);
	SetMultistream();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CPage_Advanced::SetMultistream(void)
{
	if(m_check_multistream.GetCheck()) //选中
	{
		GetDlgItem(IDC_CHECK_FRONTCOLOR)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_FRONTGRAY)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_FRONTBW)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_BACKCOLOR)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_BACKGRAY)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_BACKBW)->EnableWindow(TRUE);

		GetDlgItem(IDC_ADVANCED_COMBO_BINARIZATION)->EnableWindow(TRUE); //二值化
		GetDlgItem(IDC_ADVANCED_COMBO_RESOLUTION)->EnableWindow(TRUE); //分辨率
		GetDlgItem(IDC_ADVANCED_SLIDER_SENSITIVE_THRESHOLD)->EnableWindow(TRUE); //去除斑点
		GetDlgItem(IDC_ADVANCED_SLIDER_CONTRAST)->EnableWindow(TRUE); //对比度
		GetDlgItem(IDC_ADVANCED_SLIDER_BRIGHTNESS)->EnableWindow(TRUE); //亮度

		GetDlgItem(IDC_ADVANCED_EDIT_SENSITIVE_THRESHOLD)->EnableWindow(TRUE); //去除斑点Edit
		GetDlgItem(IDC_ADVANCED_EDIT_CONTRAST)->EnableWindow(TRUE); //对比度Edit
		GetDlgItem(IDC_ADVANCED_EDIT_BRIGHTNESS)->EnableWindow(TRUE); //亮度Edit

		SetColorGrayImage();
		SetBWImage();
	} 
	else
	{
		GetDlgItem(IDC_CHECK_FRONTCOLOR)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_FRONTGRAY)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_FRONTBW)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_BACKCOLOR)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_BACKGRAY)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_BACKBW)->EnableWindow(FALSE);

		GetDlgItem(IDC_ADVANCED_COMBO_BINARIZATION)->EnableWindow(FALSE); //二值化
		GetDlgItem(IDC_ADVANCED_COMBO_RESOLUTION)->EnableWindow(FALSE); //分辨率
		GetDlgItem(IDC_ADVANCED_SLIDER_SENSITIVE_THRESHOLD)->EnableWindow(FALSE); //去除斑点
		GetDlgItem(IDC_ADVANCED_SLIDER_BRIGHTNESS)->EnableWindow(FALSE); //亮度
		GetDlgItem(IDC_ADVANCED_SLIDER_CONTRAST)->EnableWindow(FALSE); //对比度

		GetDlgItem(IDC_ADVANCED_EDIT_SENSITIVE_THRESHOLD)->EnableWindow(FALSE); //去除斑点Edit
		GetDlgItem(IDC_ADVANCED_EDIT_BRIGHTNESS)->EnableWindow(FALSE); //亮度Edit
		GetDlgItem(IDC_ADVANCED_EDIT_CONTRAST)->EnableWindow(FALSE); //对比度Edit
	}
}


void CPage_Advanced::SetBinarization(void)
{
	if (m_check_frontbw.GetCheck() || m_check_backbw.GetCheck())
	{
		GetDlgItem(IDC_ADVANCED_SLIDER_SENSITIVE_THRESHOLD)->EnableWindow(TRUE); 
		GetDlgItem(IDC_ADVANCED_EDIT_SENSITIVE_THRESHOLD)->EnableWindow(TRUE); 

		CString strCBText;
		GetDlgItemText(IDC_ADVANCED_COMBO_BINARIZATION,strCBText);
		if (strCBText.Find("动态阈值") >= 0)
		{
			SetDlgItemText(IDC_ADVANCED_STATIC_SENSITIVITY_THRESHOLD, TEXT("去除斑点"));
			m_slider_sensitive_threshold.SetRange(SLIDER_MIN_SENSITIVITY,SLIDER_MAX_SENSITIVITY);
			m_slider_sensitive_threshold.SetTicFreq(1);
			m_slider_sensitive_threshold.SetPos(20);//设置为默认值20
		}
		else if(strCBText.Find("固定阈值") >= 0)
		{
			SetDlgItemText(IDC_ADVANCED_STATIC_SENSITIVITY_THRESHOLD, TEXT("底色保留"));

			m_slider_sensitive_threshold.SetRange(SLIDER_MIN_THRESHOLD,SLIDER_MAX_THRESHOLD);
			m_slider_sensitive_threshold.SetTicFreq(1);
			m_slider_sensitive_threshold.SetPos(128);//设置为默认值128
		}
		else if(strCBText.Find("半色调") >= 0 || strCBText.Find("误差扩散") >= 0)
		{
			GetDlgItem(IDC_ADVANCED_SLIDER_SENSITIVE_THRESHOLD)->EnableWindow(FALSE); 
			GetDlgItem(IDC_ADVANCED_EDIT_SENSITIVE_THRESHOLD)->EnableWindow(FALSE); 
		}
	}
	else
	{
		GetDlgItem(IDC_ADVANCED_SLIDER_SENSITIVE_THRESHOLD)->EnableWindow(FALSE); 
		GetDlgItem(IDC_ADVANCED_EDIT_SENSITIVE_THRESHOLD)->EnableWindow(FALSE); 
	}

	UpdateData(FALSE);
}


void CPage_Advanced::OnCbnSelchangeAdvanced_Combo_Standardsizes()
{
	// TODO: 在此添加控件通知处理程序代码
	int nIndex = m_combo_standardsizes.GetCurSel();
	CString strCBText; 
	m_combo_standardsizes.GetLBText( nIndex, strCBText);
	int nval;
	//int nval = _ttoi(strCBText);  // CString 转 int
	if (strCBText.Find("自定义") >= 0)
	{
	nval = TWSS_NONE;
	}
	else if (strCBText.Find("US Letter") >= 0)
	{
		nval = TWSS_USLETTER;
	}
	else if (strCBText.Find("照片64") >= 0)
	{
		nval = TWSS_PHOT64;
	}
	else if (strCBText.Find("照片53") >= 0)
	{
		nval = TWSS_PHOT53;
	}
	else if (strCBText.Find("US Legal") >= 0)
	{
		nval = TWSS_USLEGAL;
	}
	else if (strCBText.Find("ISO A3") >= 0)
	{
		nval = TWSS_A3;
	}
	else if (strCBText.Find("ISO A4") >= 0)
	{
		nval = TWSS_A4;
	}
	else if (strCBText.Find("ISO A5") >= 0)
	{
		nval = TWSS_A5;
	}
	else if (strCBText.Find("ISO A6") >= 0)
	{
		nval = TWSS_A6;
	}
	else if (strCBText.Find("ISO A7") >= 0)
	{
		nval = TWSS_A7;
	}
	else if (strCBText.Find("ISO B4") >= 0)
	{
		nval = TWSS_ISOB4;
	}
	else if (strCBText.Find("ISO B5") >= 0)
	{
		nval = TWSS_ISOB5;
	}
	else if (strCBText.Find("ISO B6") >= 0)
	{
		nval = TWSS_ISOB6;
	}
	else if (strCBText.Find("ISO B7") >= 0)
	{
		nval = TWSS_ISOB7;
	}
	else if (strCBText.Find("JIS B4") >= 0)
	{
		nval = TWSS_JISB4;
	}
	else if (strCBText.Find("JIS B5") >= 0)
	{
		nval = TWSS_JISB5;
	}
	else if (strCBText.Find("JIS B6") >= 0)
	{
		nval = TWSS_JISB6;
	}
	else if (strCBText.Find("JIS B7") >= 0)
	{
		nval = TWSS_JISB7;
	}
	else if (strCBText.Find("长纸模式") >= 0)
	{
		nval = UDSCAP_LONGDOCUMENT;
	}
	else if (strCBText.Find("最大扫描") >= 0)
	{
		nval = TWSS_MAXSIZE;
	}
	else
	{
		return;
	}
	m_pUI->SetCapValueInt(ICAP_SUPPORTEDSIZES,nval); 
	//m_advancedmap.insert(map<int, int> :: value_type(ICAP_SUPPORTEDSIZES, nval));
	UpdateControls(); 
}


void CPage_Advanced::OnCbnSelchangeAdvanced_Combo_Orientation()
{
	// TODO: 在此添加控件通知处理程序代码
	int nIndex = m_combo_orientation.GetCurSel();
	CString strCBText; 
	m_combo_orientation.GetLBText( nIndex, strCBText);
	/*int nval = _ttoi(strCBText);  ///< CString 转 int*/
	int nval;
	if (strCBText.Find("纵向") >= 0)
	{
		nval = TWOR_PORTRAIT;
	}
	else
	{
		nval = TWOR_LANDSCAPE;
	}
	m_pUI->SetCapValueInt(ICAP_ORIENTATION,nval); 
	//m_advancedmap.insert(map<int, int> :: value_type(ICAP_ORIENTATION, nval));
	UpdateControls(); 
}


void CPage_Advanced::OnCbnSelchangeAdvanced_Combo_Uints()
{
	// TODO: 在此添加控件通知处理程序代码
	int nIndex = m_combo_uints.GetCurSel();
	CString strCBText; 
	m_combo_uints.GetLBText( nIndex, strCBText);
	int nval;
	//int nval = _ttoi(strCBText);  ///< CString 转 int
	if (strCBText.Find("Inches") >= 0)
	{
		nval = TWUN_INCHES;
	}
	else if (strCBText.Find("Pixels") >= 0)
	{
		nval = TWUN_PIXELS;
	}
	else if (strCBText.Find("Centimeters") >= 0)
	{
		nval = TWUN_CENTIMETERS;
	}
	else if (strCBText.Find("Picas") >= 0)
	{
		nval = TWUN_PICAS;
	}
	else if (strCBText.Find("Points") >= 0)
	{
		nval = TWUN_POINTS;
	}
	else if (strCBText.Find("Twips") >= 0)
	{
		nval = TWUN_TWIPS;
	}
	else
	{
		return;
	}
	m_pUI->SetCapValueInt(ICAP_UNITS,nval); 
	//m_advancedmap.insert(map<int, int> :: value_type(ICAP_UNITS, nval));
	UpdateControls(); 
}


//void CPage_Advanced::OnEnChangeAdvanced_Edit_Customwidth()
//{
//	// TODO:  如果该控件是 RICHEDIT 控件，它将不
//	// 发送此通知，除非重写 CPropertyPage::OnInitDialog()
//	// 函数并调用 CRichEditCtrl().SetEventMask()，
//	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
//
//	// todo:  在此添加控件通知处理程序代码
//	CString strWidth,strHeight;
//	m_edit_custom_width.GetWindowText(strWidth);
//	m_edit_custom_height.GetWindowText(strHeight);
//	float fW = (float)_ttof(strWidth);  // CString -> float
//	float fH = (float)_ttof(strHeight); 
//	m_pUI->SetCurrentFrame(0.0f,0.0f,fH,fW);
////	UpdateControls(); 
//}
//
//
//void CPage_Advanced::OnEnChangeAdvanced_Edit_Customheight()
//{
//	// TODO:  如果该控件是 RICHEDIT 控件，它将不
//	// 发送此通知，除非重写 CPropertyPage::OnInitDialog()
//	// 函数并调用 CRichEditCtrl().SetEventMask()，
//	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
//
//	// TODO:  在此添加控件通知处理程序代码
//	CString strWidth,strHeight;
//	m_edit_custom_width.GetWindowText(strWidth);
//	m_edit_custom_height.GetWindowText(strHeight);
//	float fW = (float)_ttof(strWidth);  // CString -> float
//	float fH = (float)_ttof(strHeight); 
//	m_pUI->SetCurrentFrame(0.0f,0.0f,fH,fW);
//}



void CPage_Advanced::OnCbnSelchangeAdvanced_Combo_Resolution()
{
	// TODO: 在此添加控件通知处理程序代码
	int nIndex = m_combo_resolution.GetCurSel();
	CString strCBText; 
	m_combo_resolution.GetLBText(nIndex, strCBText);
	int nval = _ttoi(strCBText);  // CString 转 int
	//CString str;
	//str.Format("%d",nval);
	//AfxMessageBox(str);
	//m_advancedmap.insert(map<int, int> :: value_type(ICAP_XRESOLUTION, nval));
	//m_advancedmap.insert(map<int, int> :: value_type(ICAP_YRESOLUTION, nval)); 
	m_pUI->SetCapValueInt(ICAP_XRESOLUTION,nval); 
	m_pUI->SetCapValueInt(ICAP_YRESOLUTION,nval);
	UpdateControls();
	/*// 设置应用按钮为可用状态
	SetModified(TRUE);*/
	return;
}


void CPage_Advanced::OnCbnSelchangeAdvanced_Combo_Rotate()
{
	// TODO: 在此添加控件通知处理程序代码
	int nIndex = m_combo_rotate.GetCurSel();
	CString strCBText; 
	m_combo_rotate.GetLBText( nIndex, strCBText);
	int nval;
	if (strCBText.Find("不旋转图像") >= 0)
	{
		nval = TWOR_ROT0;
	}
	else if(strCBText.Find("顺时针90度") >= 0)
	{
		nval = TWOR_ROT90; 
	}
	else if(strCBText.Find("顺时针180度") >= 0)
	{
		nval = TWOR_ROT180; 
	}
	else if(strCBText.Find("顺时针270度") >= 0)
	{
		nval = TWOR_ROT270; 
	}
	else
	{}
	//m_advancedmap.insert(map<int, int> :: value_type(ICAP_ROTATION, nval));
	m_pUI->SetCapValueInt(ICAP_ROTATION,nval);
	UpdateControls();
}


void CPage_Advanced::OnCbnSelchangeAdvanced_Combo_Binarization()
{
	// TODO: 在此添加控件通知处理程序代码
	int nIndex = m_combo_binarization.GetCurSel();
	CString strCBText; 
	m_combo_binarization.GetLBText( nIndex, strCBText);
	int nval;
	if (strCBText.Find("动态阈值") >= 0)
	{
		nval = TWBZ_DYNATHRESHOLD;
	}
	else if(strCBText.Find("固定阈值") >= 0)
	{
		nval = TWBZ_FIXEDTHRESHOLD; 
	}
	else if(strCBText.Find("半色调1") >= 0)
	{
		nval = TWBZ_HALFTONE1; 
	}
	else if(strCBText.Find("半色调2") >= 0)
	{
		nval = TWBZ_HALFTONE2; 
	}
	else if(strCBText.Find("半色调3") >= 0)
	{
		nval = TWBZ_HALFTONE3; 
	}
	else if(strCBText.Find("半色调4") >= 0)
	{
		nval = TWBZ_HALFTONE4; 
	}
	else if(strCBText.Find("半色调5") >= 0)
	{
		nval = TWBZ_HALFTONE5; 
	}
	else if(strCBText.Find("误差扩散") >= 0)
	{
		nval = TWBZ_ERRORDIFF; 
	}
	else
	{}
	m_pUI->SetCapValueInt(UDSCAP_BINARIZATION,nval); 

	SetBinarization();
	UpdateControls();
}


void CPage_Advanced::OnCbnSelchangeAdvanced_Combo_SpiltImage()
{
	// TODO: 在此添加控件通知处理程序代码
	int nIndex = m_combo_splitimage.GetCurSel();
	CString strCBText; 
	m_combo_splitimage.GetLBText( nIndex, strCBText);
	int nval;
	if (strCBText.Find("不分割图像") >= 0)
	{
		nval = TWSI_NONE;
	}
	else if(strCBText.Find("水平分割图像") >= 0)
	{
		nval = TWSI_HORIZONTAL; 
	}
	else if(strCBText.Find("垂直分割图像") >= 0)
	{
		nval = TWSI_VERTICAL; 
	}
	else
	{}
	m_pUI->SetCapValueInt(UDSCAP_SPLITIMAGE,nval); 
	UpdateControls();
}



void CPage_Advanced::OnAdvanced_Btn_Check_Multistream()
{
	// TODO: 在此添加控件通知处理程序代码
	SetMultistream();
}

void CPage_Advanced::InitSliderCtrl()
{
	m_slider_contrast.SetRange(SLIDER_MIN,SLIDER_MAX);
	m_slider_contrast.SetTicFreq(1);  // 设置滑动条刻度的频度为1个单位，很重要，若不加这句滑块初始位置不变
	m_slider_contrast.SetPos(0);

	m_slider_brightness.SetRange(SLIDER_MIN,SLIDER_MAX);
	m_slider_brightness.SetTicFreq(1);
	m_slider_brightness.SetPos(0);//设置为中间

	m_slider_gamma.SetRange(SLIDER_MIN_GAMMA,SLIDER_MAX_GAMMA);
	m_slider_gamma.SetTicFreq(1);
	m_slider_gamma.SetPos(100);  //默认设置位置为100，不为10

	UpdateData(FALSE);  // 更新控件
}

//Gamma图像校正
void CPage_Advanced::OnNMCustomdrawAdvanced_Slider_Gamma(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);  // 接收数据
	CString str;
	int sldValue = m_slider_gamma.GetPos();  // 获取滑块当前位置
	m_pUI->SetCapValueFloat(ICAP_GAMMA,(float)sldValue);  

	str.Format("%d", sldValue);
	m_edit_gamma.SetWindowText(str);  // 在编辑框同步显示滚动条值
	UpdateData(FALSE);  // 更新控件
	UpdateControls();
	/*// 设置应用按钮为可用状态
	SetModified(TRUE);*/
	*pResult = 0;
}


void CPage_Advanced::OnNMCustomdrawAdvanced_Slider_SensitiveThreshold(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);  // 接收数据
	CString str;
	int sldValue = m_slider_sensitive_threshold.GetPos();  // 获取滑块当前位置
	m_pUI->SetCapValueFloat(UDSCAP_SENSITIVETHRESHOLD,(float)sldValue);  

	str.Format("%d", sldValue);
	m_edit_sensitive_threshold.SetWindowText(str);  // 在编辑框同步显示滚动条值
	UpdateData(FALSE);  // 更新控件
	UpdateControls();
	/*// 设置应用按钮为可用状态
	SetModified(TRUE);*/
	*pResult = 0;
}


void CPage_Advanced::OnNMCustomdrawAdvanced_Slider_Brightness(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);  // 接收数据
	CString str;
	int sldValue = m_slider_brightness.GetPos();  // 获取滑块当前位置
	//m_basemap.insert(map<int, int> :: value_type(ICAP_BRIGHTNESS, sldValue));
	m_pUI->SetCapValueFloat(ICAP_BRIGHTNESS,(float)sldValue);  // 设置亮度为当前滚动条值

	str.Format("%d", sldValue);
	m_edit_brightness.SetWindowText(str);  // 在编辑框同步显示滚动条值
	UpdateData(FALSE);  // 更新控件
	UpdateControls();
	/*// 设置应用按钮为可用状态
	SetModified(TRUE);*/
	*pResult = 0;
}


void CPage_Advanced::OnNMCustomdrawAdvanced_Slider_Contrast(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);  // 接收数据
	CString str;
	int sldValue = m_slider_contrast.GetPos();  // 获取滑块当前位置
	//m_basemap.insert(map<int, int> :: value_type(ICAP_CONTRAST, sldValue));
	m_pUI->SetCapValueFloat(ICAP_CONTRAST,(float)sldValue);  // 设置对比度为当前滚动条值

	str.Format("%d", sldValue);
	m_edit_contrast.SetWindowText(str);  // 在编辑框同步显示滚动条值
	UpdateData(FALSE);  // 更新控件
	UpdateControls();
	/*// 设置应用按钮为可用状态
	SetModified(TRUE);*/
	*pResult = 0;
}


void CPage_Advanced::OnEnChangeAdvanced_Edit_Contrast()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CPropertyPage::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);  // 接收数据
	CString str;
	m_edit_contrast.GetWindowText(str);
	int nval = _ttoi(str);
	m_slider_contrast.SetPos(nval);
//	m_basemap.insert(map<int, int> :: value_type(ICAP_CONTRAST, nval));
	m_pUI->SetCapValueFloat(ICAP_CONTRAST,(float)nval);  // 设置对比度为当前滚动条值

	m_edit_contrast.SetSel(str.GetLength(), str.GetLength(),TRUE);  // 设置编辑框控件范围
	UpdateData(FALSE);  // 更新控件
	UpdateControls();
	/*// 设置应用按钮为可用状态
	SetModified(TRUE);*/
}



void CPage_Advanced::OnEnChangeAdvanced_Edit_SensitiveThreshold()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CPropertyPage::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);  // 接收数据
	CString str;
	m_edit_sensitive_threshold.GetWindowText(str);
	int nval = _ttoi(str);
	m_slider_sensitive_threshold.SetPos(nval);

	m_pUI->SetCapValueFloat(UDSCAP_SENSITIVETHRESHOLD,(float)nval);  

	m_edit_sensitive_threshold.SetSel(str.GetLength(), str.GetLength(),TRUE);  // 设置编辑框控件范围
	UpdateData(FALSE);  // 更新控件
	UpdateControls();

	/*// 设置应用按钮为可用状态
	SetModified(TRUE);*/
}


void CPage_Advanced::OnEnChangeAdvanced_Edit_Gamma()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CPropertyPage::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);  // 接收数据
	CString str;
	m_edit_gamma.GetWindowText(str);
	int nval = _ttoi(str);
	m_slider_gamma.SetPos(nval);
	//m_basemap.insert(map<int, int> :: value_type(ICAP_BRIGHTNESS, nval));
	m_pUI->SetCapValueFloat(ICAP_GAMMA,(float)nval);  // 设置对比度为当前滚动条值

	m_edit_gamma.SetSel(str.GetLength(), str.GetLength(),TRUE);  // 设置编辑框控件范围
	UpdateData(FALSE);  // 更新控件
	UpdateControls();

	/*// 设置应用按钮为可用状态
	SetModified(TRUE);*/
}


void CPage_Advanced::OnEnChangeAdvanced_Edit_Brightness()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CPropertyPage::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);  // 接收数据
	CString str;
	m_edit_brightness.GetWindowText(str);
	int nval = _ttoi(str);
	m_slider_brightness.SetPos(nval);
	//m_basemap.insert(map<int, int> :: value_type(ICAP_BRIGHTNESS, nval));
	m_pUI->SetCapValueFloat(ICAP_BRIGHTNESS,(float)nval);  // 设置对比度为当前滚动条值

	m_edit_brightness.SetSel(str.GetLength(), str.GetLength(),TRUE);  // 设置编辑框控件范围
	UpdateData(FALSE);  // 更新控件
	UpdateControls();

	/*// 设置应用按钮为可用状态
	SetModified(TRUE);*/
}


void CPage_Advanced::SetColorGrayImage(void)
{
	if (m_check_frontgray.GetCheck() || m_check_frontcolor.GetCheck()
			|| m_check_backcolor.GetCheck() || m_check_backgray.GetCheck())
	{
		GetDlgItem(IDC_ADVANCED_SLIDER_BRIGHTNESS)->EnableWindow(TRUE);
		GetDlgItem(IDC_ADVANCED_SLIDER_CONTRAST)->EnableWindow(TRUE);
		GetDlgItem(IDC_ADVANCED_EDIT_BRIGHTNESS)->EnableWindow(TRUE);
		GetDlgItem(IDC_ADVANCED_EDIT_CONTRAST)->EnableWindow(TRUE);
	} 
	else
	{
		GetDlgItem(IDC_ADVANCED_SLIDER_BRIGHTNESS)->EnableWindow(FALSE);
		GetDlgItem(IDC_ADVANCED_SLIDER_CONTRAST)->EnableWindow(FALSE);
		GetDlgItem(IDC_ADVANCED_EDIT_BRIGHTNESS)->EnableWindow(FALSE);
		GetDlgItem(IDC_ADVANCED_EDIT_CONTRAST)->EnableWindow(FALSE);
	}
}


void CPage_Advanced::SetBWImage(void)
{
	if (m_check_frontbw.GetCheck() || m_check_backbw.GetCheck())
	{
		GetDlgItem(IDC_ADVANCED_COMBO_BINARIZATION)->EnableWindow(TRUE);
	} 
	else
	{
		GetDlgItem(IDC_ADVANCED_COMBO_BINARIZATION)->EnableWindow(FALSE);
	}
	SetBinarization(); //设置“去除斑点”还是“底色保留”
}

void CPage_Advanced::OnAdvanced_Btn_Check_FrontColor()
{
	// TODO: 在此添加控件通知处理程序代码
	SetColorGrayImage();
}


void CPage_Advanced::OnAdvanced_Btn_Check_FrontGray()
{
	// TODO: 在此添加控件通知处理程序代码
	SetColorGrayImage();
}


void CPage_Advanced::OnAdvanced_Btn_Check_FrontBW()
{
	// TODO: 在此添加控件通知处理程序代码
	SetBWImage();
}


void CPage_Advanced::OnAdvanced_Btn_Check_BackGray()
{
	// TODO: 在此添加控件通知处理程序代码
	SetColorGrayImage();
}


void CPage_Advanced::OnAdvanced_Btn_Check_BackColor()
{
	// TODO: 在此添加控件通知处理程序代码
	SetColorGrayImage();
}


void CPage_Advanced::OnAdvanced_Btn_Check_BackBW()
{
	// TODO: 在此添加控件通知处理程序代码
	SetBWImage();
}

//去除空白页
void CPage_Advanced::OnAdvanced_Btn_Check_RemoveBlank()
{
	// TODO: 在此添加控件通知处理程序代码
	int nval;
	if (m_check_removeblank.GetCheck())
	{
		nval = TWBP_AUTO;
	} 
	else
	{
		nval = TWBP_DISABLE;
	}

	m_pUI->SetCapValueFloat(ICAP_AUTODISCARDBLANKPAGES,(float)nval); 
//	m_pUI->SetCapValueInt(ICAP_AUTODISCARDBLANKPAGES,nval); 
	//CString str;
	//str.Format("空白页%d",ms);
	//AfxMessageBox(str);
}


//去除穿孔
void CPage_Advanced::OnAdvanced_Btn_Check_RemovePunch()
{
	// TODO: 在此添加控件通知处理程序代码
	int nval;
	if (m_check_removepunch.GetCheck())
	{
		//AfxMessageBox("去除穿孔选中");
		nval = TWRP_AUTO;
	} 
	else
	{
		//AfxMessageBox("去除穿孔未选中");
		nval = TWRP_DISABLE;
	}
	//m_pUI->SetCapValueFloat(UDSCAP_PUNCHHOLEREMOVEL,(float)nval); 
	m_pUI->SetCapValueInt(UDSCAP_PUNCHHOLEREMOVEL,nval); 

	/*CString str;
	str.Format("%d",nval);
	AfxMessageBox(str);*/
}


//锐化图像
void CPage_Advanced::OnAdvanced_Btn_Check_Sharpen()
{
	// TODO: 在此添加控件通知处理程序代码
	int nval;
	if (m_check_sharpen.GetCheck())
	{
		//AfxMessageBox("锐化图像选中");
		nval = TWSP_AUTO;
	} 
	else
	{
		//AfxMessageBox("锐化图像未选中");
		nval = TWSP_DISABLE;
	}
	//m_pUI->SetCapValueFloat(UDSCAP_SHARPEN,(float)nval); 
	m_pUI->SetCapValueInt(UDSCAP_SHARPEN,nval); 
}


//图像镜像处理
void CPage_Advanced::OnAdvanced_Btn_Check_Mirror()
{
	// TODO: 在此添加控件通知处理程序代码
	int nval;
	if (m_check_mirror.GetCheck())
	{
		nval = TWMR_AUTO;
	} 
	else
	{
		nval = TWMR_DISABLE;
	}
	//m_pUI->SetCapValueFloat(UDSCAP_MIRROR,(float)nval); 
	m_pUI->SetCapValueInt(UDSCAP_MIRROR,nval); 
}


//去除背景
void CPage_Advanced::OnAdvanced_Btn_Check_RemoveBack()
{
	// TODO: 在此添加控件通知处理程序代码
	int nval;
	if (m_check_removeback.GetCheck())
	{
		nval = TWRB_AUTO;
	} 
	else
	{
		nval = TWRB_DISABLE;
	}
	//m_pUI->SetCapValueFloat(UDSCAP_REMOVEBACKGROUND,(float)nval); 
	m_pUI->SetCapValueInt(UDSCAP_REMOVEBACKGROUND,nval); 
}


//去除网纹
void CPage_Advanced::OnAdvanced_Btn_Check_RemoveDescreen()
{
	// TODO: 在此添加控件通知处理程序代码
	int nval;
	if (m_check_removedescreen.GetCheck())
	{
		nval = TWDS_AUTO;
	} 
	else
	{
		nval = TWDS_DISABLE;
	}
	//m_pUI->SetCapValueFloat(UDSCAP_DESCREEN,(float)nval); 
	m_pUI->SetCapValueInt(UDSCAP_DESCREEN,nval); 
}


//去除噪声
void CPage_Advanced::OnAdvanced_Btn_Check_RemoveDenoise()
{
	// TODO: 在此添加控件通知处理程序代码
	int nval;
	if (m_check_removedenoise.GetCheck())
	{
		nval = TWDN_AUTO;
	} 
	else
	{
		nval = TWDN_DISABLE;
	}
	//m_pUI->SetCapValueFloat(UDSCAP_DENOISE,(float)nval); 
	m_pUI->SetCapValueInt(UDSCAP_DENOISE,nval);
}


//自动裁切及校正
void CPage_Advanced::OnAdvanced_Btn_Check_AutoCrop()
{
	// TODO: 在此添加控件通知处理程序代码
	int nval;
	if (m_check_autocrop.GetCheck())
	{
		nval = TWAC_AUTO;
	} 
	else
	{
		nval = TWAC_DISABLE;
	}
	//m_pUI->SetCapValueFloat(UDSCAP_AUTOCROP,(float)nval); 
	m_pUI->SetCapValueInt(UDSCAP_AUTOCROP,nval);
}





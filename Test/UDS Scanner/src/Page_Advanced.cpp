// Page_Advanced.cpp : 实现文件
//

#include "stdafx.h"
#include "UDS Scanner.h"
#include "Page_Advanced.h"
#include "afxdialogex.h"

/**********************************************************
*  高4位 ： 7    6   5    4      低四位 ： 3    2   1    0
* （背面） 保留 黑白 灰度 彩色    （正面）  保留 黑白 灰度 彩色
**********************************************************/
BYTE g_MuiltStream = 0x00;

bool muiltstream; //高级界面多流不选中

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
	DDX_Control(pDX, IDC_ADVANCED_SLIDER_REMOVEBLANK, m_slider_removeblank);
	DDX_Control(pDX, IDC_ADVANCED_EDIT_REMOVEBLANK, m_edit_removeblank);
}


BEGIN_MESSAGE_MAP(CPage_Advanced, CPropertyPage)
	ON_CBN_SELCHANGE(IDC_ADVANCED_COMBO_STANDARDSIZES, &CPage_Advanced::OnCbnSelchangeAdvanced_Combo_Standardsizes)
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
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_ADVANCED_SLIDER_REMOVEBLANK, &CPage_Advanced::OnNMCustomdrawAdvanced_Slider_Removeblank)
	ON_EN_CHANGE(IDC_ADVANCED_EDIT_REMOVEBLANK, &CPage_Advanced::OnEnChangeAdvanced_Edit_Removeblank)
END_MESSAGE_MAP()


void CPage_Advanced::SetCapValue(void)
{
	MAP_CAP::iterator iter;

	for(iter = m_advancedmap.begin(); iter != m_advancedmap.end(); iter++)
	{
		switch(iter->first)
		{
		case ICAP_SUPPORTEDSIZES:  //纸张大小
		case ICAP_UNITS:  //单位
		case ICAP_XRESOLUTION:  //X分辨率
		case ICAP_YRESOLUTION:  //Y分辨率
		case ICAP_ROTATION:  //旋转
		case UDSCAP_DENOISE: //去噪声
		case UDSCAP_DESCREEN: //去网纹
		case UDSCAP_REMOVEBACKGROUND: //去背景
		case UDSCAP_SHARPEN: //图像锐化
		case UDSCAP_REMOVEBLANK: //去除空白页的checkbox
		case UDSCAP_MIRROR: //镜像处理
		case UDSCAP_BINARIZATION: //二值化
			{
				m_pUI->SetCapValueInt(iter->first,(int)(iter->second));
				break;
			}	

		case ICAP_FRAMES: //框架
			{
				m_pUI->SetCurrentFrame(0,0,iter->second,iter->second);
				break;
			}	

		case UDSCAP_PUNCHHOLEREMOVEL: //去除穿孔
			{
				if(m_check_removepunch.GetCheck()) //去穿孔可用时
				{
					m_pUI->SetCapValueInt(iter->first,(int)(iter->second));
				}
				else
				{
					m_pUI->SetCapValueInt(iter->first,FALSE);
				}
				break;
			}

		case UDSCAP_AUTOCROP: //自动裁切与校正
			{
				if(m_check_autocrop.GetCheck()) //自动裁切与校正可用  防止客户在选中该项后，又点了多流（此时该项本应不可用），所以在不可用时要设置为FALSE
				{
					m_pUI->SetCapValueInt(iter->first,(int)(iter->second));
				}
				else
				{
					m_pUI->SetCapValueInt(iter->first,FALSE);
				}
				break;
			}

		case UDSCAP_MULTISTREAM: //多流输出
			{
				if(m_check_multistream.GetCheck())
				{
					if(m_check_frontbw.GetCheck() || m_check_backbw.GetCheck())
					{
						m_pUI->SetCapValueInt(ICAP_PIXELTYPE,TWPT_BW);
					}
					if(m_check_frontgray.GetCheck() || m_check_backgray.GetCheck())
					{
						m_pUI->SetCapValueInt(ICAP_PIXELTYPE,TWPT_GRAY);
					}
					if(m_check_frontcolor.GetCheck() || m_check_backcolor.GetCheck())
					{
						m_pUI->SetCapValueInt(ICAP_PIXELTYPE,TWPT_RGB);
					}

					GetCheckNum();
					m_pUI->SetCapValueInt(UDSCAP_DOCS_IN_ADF,checknum); //设置扫描张数为checknum	

					m_pUI->SetCapValueInt(iter->first,(int)(iter->second));
				}	
				break;
			}	

		case UDSCAP_FRONTCOLOR: //彩色正面
		case UDSCAP_FRONTGRAY: //灰度正面
		case UDSCAP_FRONTBW: //黑白正面
		case UDSCAP_BACKCOLOR: //彩色背面
		case UDSCAP_BACKGRAY: //灰度背面
		case UDSCAP_BACKBW: //黑白背面
			{
				if(((CButton*)GetDlgItem(IDC_CHECK_MULTISTREAM))->GetCheck())
				{
					m_pUI->SetCapValueInt(iter->first,(int)(iter->second));
				}
				else
				{
					m_pUI->SetCapValueInt(iter->first,FALSE);
				}
				break;
			}	

		case UDSCAP_SPLITIMAGE: //图像分割
			{
				m_pUI->SetCapValueInt(iter->first,(int)(iter->second)); //设置参数生效
				if((int)(iter->second) != TWSI_NONE)
				{
					m_pUI->SetCapValueInt(UDSCAP_DOCS_IN_ADF,2); //设置扫描张数为2
				}
				else if((int)(iter->second) == TWSI_NONE && m_pBasePage->scanside == 0) //不拆分&&单面
				{
					m_pUI->SetCapValueInt(UDSCAP_DOCS_IN_ADF,1); //不拆分时又设回1
				}
				break;
			}

		case ICAP_AUTODISCARDBLANKPAGES: //去除空白页
			{
				if(m_slider_removeblank.IsWindowEnabled())
				{
					m_pUI->SetCapValueFloat(iter->first,iter->second);
				}	
				break;
			}

		case ICAP_GAMMA: //Gamma校正
			{
				/*
				char buf[10];
				itoa(iter->second, buf, 10);
				::MessageBox(NULL,TEXT(buf),"ICAP_GAMMA",MB_OK);*/

				m_pUI->SetCapValueFloat(iter->first,iter->second);
				break;
			}

		case UDSCAP_SENSITIVETHRESHOLD_REMOVESPOTS: //去除斑点
			{
				if(m_slider_sensitive_threshold.IsWindowEnabled())
				{
					m_pUI->SetCapValueFloat(iter->first,iter->second);
				}	
				break;
			}

		case ICAP_THRESHOLD:
			{
				if(m_slider_sensitive_threshold.IsWindowEnabled())
				{
					m_pUI->SetCapValueFloat(iter->first,iter->second);  //底色保留，实际是二值化的阈值
				}
				break;
			}

		case ICAP_BRIGHTNESS: //亮度
			{
				if(m_slider_brightness.IsWindowEnabled())
				{
					m_pUI->SetCapValueFloat(iter->first,iter->second);
				}
				break;
			}
		case ICAP_CONTRAST: //对比度
			{
				if(m_slider_contrast.IsWindowEnabled())
				{
					m_pUI->SetCapValueFloat(iter->first,iter->second);
				}
				break;
			}

		default:
			{
				break;
			}	
			//未完
		}
	}
}

void CPage_Advanced::UpdateControls(void)
{
	int nCapIndex;
	const IntVector* lstCapValues;
	const FloatVector* lstCapValuesFlt;
	int nCapValue;
	CString strText;

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

	// 自定义宽与高
	TW_FRAME frameTemp;
	CString strTemp;

	frameTemp = m_pUI->GetCurrentFrame();
	strTemp.Format("%0.2f",FIX32ToFloat(frameTemp.Right)); //宽
	SetDlgItemText(IDC_ADVANCED_EDIT_CUSTOMWIDTH, strTemp);

	frameTemp = m_pUI->GetCurrentFrame();
	strTemp.Format("%0.2f",FIX32ToFloat(frameTemp.Bottom)); //高
	SetDlgItemText(IDC_ADVANCED_EDIT_CUSTOMHEIGHT, strTemp);

	SetStandardsizes();

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
	lstCapValuesFlt = m_pUI->GetValidCapFloat(ICAP_ROTATION);
	for(unsigned int i=0; i<lstCapValuesFlt->size();i++)
	{
		switch((int)lstCapValuesFlt->at(i))
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
			m_combo_splitimage.InsertString(i,"不拆分"); //不分割图像
			break;
		case TWSI_HORIZONTAL:
			m_combo_splitimage.InsertString(i,"上下");  //水平分割图像
			break;
		case TWSI_VERTICAL:
			m_combo_splitimage.InsertString(i,"左右"); //垂直分割图像
			break;
		default:
			continue;
		}
	}
	m_combo_splitimage.SetCurSel(nCapIndex);

	//多流输出-对比度
	//nCapValue = (int)(m_pUI->GetCapValueFloat(ICAP_CONTRAST)); 
	nCapValue = m_pBasePage->contrast;
	m_slider_contrast.SetPos(nCapValue);
	strText.Format("%d",nCapValue);
	SetDlgItemText(IDC_ADVANCED_EDIT_CONTRAST, strText);

	//多流输出-亮度
	//nCapValue = (int)(m_pUI->GetCapValueFloat(ICAP_BRIGHTNESS)); 
	nCapValue = m_pBasePage->brightness;
	m_slider_brightness.SetPos(nCapValue);
	strText.Format("%d",nCapValue);
	//m_edit_brightness.SetWindowText(strText);
	SetDlgItemText(IDC_ADVANCED_EDIT_BRIGHTNESS, strText);
	
	CString str;
	GetDlgItemText(IDC_ADVANCED_STATIC_SENSITIVITY_THRESHOLD,str);
	if(str.Find("去除斑点") >= 0)
	{
		//多流输出-去除斑点 
		nCapValue = (int)(m_pUI->GetCapValueFloat(UDSCAP_SENSITIVETHRESHOLD_REMOVESPOTS)); 
		m_slider_sensitive_threshold.SetPos(nCapValue);
		strText.Format("%d",nCapValue);
		SetDlgItemText(IDC_ADVANCED_EDIT_SENSITIVE_THRESHOLD, strText);
	}
	else if(str.Find("底色保留") >= 0)
	{
		//多流输出-底色保留  与二值化阈值是同样的意义
		//nCapValue = (int)(m_pUI->GetCapValueFloat(UDSCAP_SENSITIVETHRESHOLD_COLORRETENT)); 
		nCapValue = (int)(m_pUI->GetCapValueFloat(ICAP_THRESHOLD));
		m_slider_sensitive_threshold.SetPos(nCapValue);
		strText.Format("%d",nCapValue);
		SetDlgItemText(IDC_ADVANCED_EDIT_SENSITIVE_THRESHOLD, strText);
	}
	else{}//必须保留
	
	//Gamma校正 
	nCapValue = (int)(m_pUI->GetCapValueFloat(ICAP_GAMMA)); //GetCapValueFloat能否得到CTWAINContainerFix32类型
	m_slider_gamma.SetPos(nCapValue);
	float valueTemp = ((float)nCapValue)/100;
	strText.Format("%.2f", valueTemp);
	SetDlgItemText(IDC_ADVANCED_EDIT_SENSITIVE_GAMMA, strText);

	//去除空白页checkbox
	nCapValue = (int)(m_pUI->GetCapValueBool(UDSCAP_REMOVEBLANK));
	m_check_removeblank.SetCheck(nCapValue);
	//去除空白页 -1自动;-2不可用:改为滑动条 
	nCapValue = (int)(m_pUI->GetCapValueFloat(ICAP_AUTODISCARDBLANKPAGES)); 
	m_slider_removeblank.SetPos(nCapValue);
	strText.Format("%d",nCapValue);
	SetDlgItemText(IDC_ADVANCED_EDIT_REMOVEBLANK, strText);
	
	//去除穿孔等
	nCapValue = (int)(m_pUI->GetCapValueBool(UDSCAP_PUNCHHOLEREMOVEL));
	m_check_removepunch.SetCheck(nCapValue);

	nCapValue = (int)(m_pUI->GetCapValueBool(UDSCAP_SHARPEN));
	m_check_sharpen.SetCheck(nCapValue);

	nCapValue = (int)(m_pUI->GetCapValueBool(UDSCAP_MIRROR));
	m_check_mirror.SetCheck(nCapValue);

	nCapValue = (int)(m_pUI->GetCapValueBool(UDSCAP_REMOVEBACKGROUND));
	m_check_removeback.SetCheck(nCapValue);

	nCapValue = (int)(m_pUI->GetCapValueBool(UDSCAP_DESCREEN));
	m_check_removedescreen.SetCheck(nCapValue);

	nCapValue = (int)(m_pUI->GetCapValueBool(UDSCAP_DENOISE));
	m_check_removedenoise.SetCheck(nCapValue); 

	nCapValue = (int)(m_pUI->GetCapValueBool(UDSCAP_AUTOCROP));
	m_check_autocrop.SetCheck(nCapValue);

	//多流输出：默认不使用
	nCapValue = (int)(m_pUI->GetCapValueBool(UDSCAP_MULTISTREAM));
	m_check_multistream.SetCheck(nCapValue);
	//彩色正面
	nCapValue = (int)(m_pUI->GetCapValueBool(UDSCAP_FRONTCOLOR));
	m_check_frontcolor.SetCheck(nCapValue);
	//灰度正面
	nCapValue = (int)(m_pUI->GetCapValueBool(UDSCAP_FRONTGRAY));
	m_check_frontgray.SetCheck(nCapValue);
	//黑白正面
	nCapValue = (int)(m_pUI->GetCapValueBool(UDSCAP_FRONTBW));
	m_check_frontbw.SetCheck(nCapValue);
	//彩色背面
	nCapValue = (int)(m_pUI->GetCapValueBool(UDSCAP_BACKCOLOR));
	m_check_backcolor.SetCheck(nCapValue);
	//灰度背面
	nCapValue = (int)(m_pUI->GetCapValueBool(UDSCAP_BACKGRAY));
	m_check_backgray.SetCheck(nCapValue);
	//黑白背面
	nCapValue = (int)(m_pUI->GetCapValueBool(UDSCAP_BACKBW));
	m_check_backbw.SetCheck(nCapValue);
}


// CPage_Advanced 消息处理程序
BOOL CPage_Advanced::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  在此添加额外的初始化
	InitAdvancedmap(); //初始化Map

	InitSliderCtrl(); //初始化滑块 要放在UpdateControls之前，否则设置滑块的步长无效
	UpdateControls();

	SetMultistream();
	SetBlank();

	GetCheckNum();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CPage_Advanced::SetBlank(void)
{
	if(m_check_removeblank.GetCheck())
	{
		GetDlgItem(IDC_ADVANCED_SLIDER_REMOVEBLANK)->EnableWindow(TRUE);
		GetDlgItem(IDC_ADVANCED_EDIT_REMOVEBLANK)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_ADVANCED_SLIDER_REMOVEBLANK)->EnableWindow(FALSE);
		GetDlgItem(IDC_ADVANCED_EDIT_REMOVEBLANK)->EnableWindow(FALSE);
	}
}

void CPage_Advanced::InitAdvancedmap(void)
{
	m_advancedmap.erase(m_advancedmap.begin(),m_advancedmap.end());//清空

	int nCapIndex;
	nCapIndex = m_pUI->GetCurrentCapIndex(UDSCAP_SPLITIMAGE);
	m_advancedmap[UDSCAP_SPLITIMAGE] = (float)nCapIndex; //初始化时只为map插入“分割Cap”的值，特例

}


void CPage_Advanced::GetCheckNum(void)
{
	checknum = 0;
	if(m_check_frontcolor.GetCheck())
	{
		g_MuiltStream = g_MuiltStream|0x01; //正面彩色
		checknum++;
		/*char buf[10] = {0};
		_stprintf_s(buf, 10, _T("%02x"), g_MuiltStream);
		::MessageBox(NULL,TEXT(buf),"0x01",MB_OK);*/
	}
	else
	{
		g_MuiltStream = g_MuiltStream&0xFE;
	}

	if(m_check_frontgray.GetCheck())
	{ 
		g_MuiltStream = g_MuiltStream|0x02;  //正面灰度
		checknum++;
	}
	else
	{
		g_MuiltStream = g_MuiltStream&0xFD;
	}

	if(m_check_frontbw.GetCheck())
	{
		g_MuiltStream = g_MuiltStream|0x04;  //正面背白
		checknum++;
	}
	else
	{
		g_MuiltStream = g_MuiltStream&0xFB;
	}

	if(m_check_backcolor.GetCheck())
	{
		g_MuiltStream = g_MuiltStream|0x10;  //背面彩色
		checknum++;
	}
	else
	{
		g_MuiltStream = g_MuiltStream&0xEF;
	}

	if(m_check_backgray.GetCheck())
	{
		g_MuiltStream = g_MuiltStream|0x20;  //背面灰度
		checknum++;
	}
	else
	{
		g_MuiltStream = g_MuiltStream&0xDF;
	}

	if(m_check_backbw.GetCheck())
	{
		g_MuiltStream = g_MuiltStream|0x40;  //背面黑白
		checknum++;
	}
	else
	{
		g_MuiltStream = g_MuiltStream&0xBF;
	}
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

		GetDlgItem(IDC_ADVANCED_COMBO_SPLITIMG)->EnableWindow(FALSE); //图像拆分不可用
		
		((CButton*)GetDlgItem(IDC_CHECK_AUTOCROP))->SetCheck(FALSE); //自动裁切与校正不选中
		((CButton*)GetDlgItem(IDC_CHECK_REMOVEPUNCH))->SetCheck(FALSE); //去除穿孔不选中
		m_pUI->SetCapValueInt(UDSCAP_PUNCHHOLEREMOVEL,TWRP_DISABLE);
		m_pUI->SetCapValueInt(UDSCAP_AUTOCROP,TWAC_DISABLE);

		GetDlgItem(IDC_CHECK_AUTOCROP)->EnableWindow(FALSE); //自动裁切与校正不可用
		GetDlgItem(IDC_CHECK_REMOVEPUNCH)->EnableWindow(FALSE);//去除穿孔不可用

		muiltstream = true;
		m_pBasePage->BaseStatus();

		//m_pUI->SetCapValueInt(UDSCAP_MULTISTREAM,nval); 
		//UpdateControls();
		//m_advancedmap.insert(map<int, float> :: value_type(UDSCAP_MULTISTREAM, 1.0f));
		m_advancedmap[UDSCAP_MULTISTREAM] = 1.0f;

		SetColorGrayImage();
		SetBWImage();
	} 
	else 
	{
		//多流输出未选中时，六个选项也均不要选中
		/*m_check_frontcolor.SetCheck(FALSE);
		m_check_frontgray.SetCheck(FALSE);
		m_check_frontbw.SetCheck(FALSE);
		m_check_backcolor.SetCheck(FALSE);
		m_check_backgray.SetCheck(FALSE);
		m_check_backbw.SetCheck(FALSE);*/

		((CButton*)GetDlgItem(IDC_CHECK_FRONTCOLOR))->SetCheck(FALSE); 
		((CButton*)GetDlgItem(IDC_CHECK_FRONTGRAY))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_CHECK_FRONTBW))->SetCheck(FALSE); 
		((CButton*)GetDlgItem(IDC_CHECK_BACKCOLOR))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_CHECK_BACKGRAY))->SetCheck(FALSE); 
		((CButton*)GetDlgItem(IDC_CHECK_BACKBW))->SetCheck(FALSE);

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

		GetDlgItem(IDC_ADVANCED_COMBO_SPLITIMG)->EnableWindow(TRUE); //图像拆分可用
		GetDlgItem(IDC_CHECK_AUTOCROP)->EnableWindow(TRUE); //自动裁切与校正可用
		GetDlgItem(IDC_CHECK_REMOVEPUNCH)->EnableWindow(TRUE); //去除穿孔可用

		muiltstream = false;
		m_pBasePage->BaseStatus();

		m_pUI->SetCapValueInt(UDSCAP_MULTISTREAM,FALSE);
		m_pUI->SetCapValueInt(UDSCAP_FRONTCOLOR,FALSE);
		m_pUI->SetCapValueInt(UDSCAP_FRONTGRAY,FALSE);
		m_pUI->SetCapValueInt(UDSCAP_FRONTBW,FALSE);
		m_pUI->SetCapValueInt(UDSCAP_BACKCOLOR,FALSE);
		m_pUI->SetCapValueInt(UDSCAP_BACKGRAY,FALSE);
		m_pUI->SetCapValueInt(UDSCAP_BACKBW,FALSE);

		//m_advancedmap.insert(map<int, float> :: value_type(UDSCAP_MULTISTREAM, 0.0));
		m_advancedmap[UDSCAP_MULTISTREAM] = 0.0f;
	}
}


void CPage_Advanced::SetBinarization(void)
{
	int nCapValue;
	float fMin,fMax,fStep;
	
	if (m_check_frontbw.GetCheck() || m_check_backbw.GetCheck())
	{
		GetDlgItem(IDC_ADVANCED_SLIDER_SENSITIVE_THRESHOLD)->EnableWindow(TRUE); 
		GetDlgItem(IDC_ADVANCED_EDIT_SENSITIVE_THRESHOLD)->EnableWindow(TRUE); 

		CString strCBText;
		GetDlgItemText(IDC_ADVANCED_COMBO_BINARIZATION,strCBText);
		if (strCBText.Find("动态阈值") >= 0)
		{
			SetDlgItemText(IDC_ADVANCED_STATIC_SENSITIVITY_THRESHOLD, TEXT("去除斑点:"));
			m_pUI->GetCapRangeFloat(UDSCAP_SENSITIVETHRESHOLD_REMOVESPOTS, fMin, fMax, fStep);
			m_slider_sensitive_threshold.SetRange((int)fMin, (int)fMax);
			m_slider_sensitive_threshold.SetTicFreq((int)fStep); 

			nCapValue = (int)(m_pUI->GetCapValueFloat(UDSCAP_SENSITIVETHRESHOLD_REMOVESPOTS));
			m_slider_sensitive_threshold.SetPos(nCapValue);

			//设置此时亮度不可用
			GetDlgItem(IDC_ADVANCED_SLIDER_BRIGHTNESS)->EnableWindow(FALSE); 
			GetDlgItem(IDC_ADVANCED_EDIT_BRIGHTNESS)->EnableWindow(FALSE);
		} 
		else if(strCBText.Find("固定阈值") >= 0)
		{
			SetDlgItemText(IDC_ADVANCED_STATIC_SENSITIVITY_THRESHOLD, TEXT("底色保留:"));
			//m_pUI->GetCapRangeFloat(UDSCAP_SENSITIVETHRESHOLD_COLORRETENT, fMin, fMax, fStep);功能等同阈值
			m_pUI->GetCapRangeFloat(ICAP_THRESHOLD, fMin, fMax, fStep);
			m_slider_sensitive_threshold.SetRange((int)fMin, (int)fMax);
			m_slider_sensitive_threshold.SetTicFreq((int)fStep); 

			//nCapValue = (int)(m_pUI->GetCapValueFloat(UDSCAP_SENSITIVETHRESHOLD_COLORRETENT));
			nCapValue = (int)(m_pUI->GetCapValueFloat(ICAP_THRESHOLD));
			m_slider_sensitive_threshold.SetPos(nCapValue);

			//设置此时亮度可用
			GetDlgItem(IDC_ADVANCED_SLIDER_BRIGHTNESS)->EnableWindow(TRUE); 
			GetDlgItem(IDC_ADVANCED_EDIT_BRIGHTNESS)->EnableWindow(TRUE);
		}
		else if(strCBText.Find("半色调") >= 0 || strCBText.Find("误差扩散") >= 0)
		{
			GetDlgItem(IDC_ADVANCED_SLIDER_SENSITIVE_THRESHOLD)->EnableWindow(FALSE); 
			GetDlgItem(IDC_ADVANCED_EDIT_SENSITIVE_THRESHOLD)->EnableWindow(FALSE); 

			//设置此时亮度可用
			GetDlgItem(IDC_ADVANCED_SLIDER_BRIGHTNESS)->EnableWindow(TRUE); 
			GetDlgItem(IDC_ADVANCED_EDIT_BRIGHTNESS)->EnableWindow(TRUE);
		}
	}
	else
	{
		GetDlgItem(IDC_ADVANCED_SLIDER_SENSITIVE_THRESHOLD)->EnableWindow(FALSE); 
		GetDlgItem(IDC_ADVANCED_EDIT_SENSITIVE_THRESHOLD)->EnableWindow(FALSE); 
	}

	UpdateData(FALSE);
}


void CPage_Advanced::SetStandardsizes(void)
{
	int nIndex = m_combo_standardsizes.GetCurSel();
	CString strCBText; 
	m_combo_standardsizes.GetLBText( nIndex, strCBText);
	if (strCBText.Find("自定义") >= 0)
	{
		m_edit_custom_width.EnableWindow(TRUE);
		m_edit_custom_height.EnableWindow(TRUE);
		m_combo_uints.EnableWindow(TRUE);
	} 
	else
	{
		m_edit_custom_width.EnableWindow(FALSE);
		m_edit_custom_height.EnableWindow(FALSE);
		m_combo_uints.EnableWindow(FALSE); //单位此时也不可用
	}
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
	//m_pUI->SetCapValueInt(ICAP_SUPPORTEDSIZES,nval); 
	//m_advancedmap.insert(map<int, double> :: value_type(ICAP_SUPPORTEDSIZES, (double)nval));
	m_advancedmap[ICAP_SUPPORTEDSIZES] = (float)nval;

	//UpdateControls(); 
	m_combo_standardsizes.SetCurSel(nIndex);
	SetStandardsizes();
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
	//m_pUI->SetCapValueInt(ICAP_UNITS,nval); 
	//m_advancedmap.insert(map<int, float> :: value_type(ICAP_UNITS, (float)nval));
	m_advancedmap[ICAP_UNITS] = (float)nval;

	//UpdateControls(); 
	m_combo_uints.SetCurSel(nIndex);
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
	//m_advancedmap.insert(map<int, float> :: value_type(ICAP_XRESOLUTION, (float)nval));
	//m_advancedmap.insert(map<int, float> :: value_type(ICAP_YRESOLUTION, (float)nval)); 
	//m_pUI->SetCapValueInt(ICAP_XRESOLUTION,nval); 
	//m_pUI->SetCapValueInt(ICAP_YRESOLUTION,nval);
	m_advancedmap[ICAP_XRESOLUTION] = (float)nval;
	m_advancedmap[ICAP_YRESOLUTION] = (float)nval;

	//UpdateControls();
	m_combo_resolution.SetCurSel(nIndex);
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
	//m_advancedmap.insert(map<int, float> :: value_type(ICAP_ROTATION, (float)nval));
	//m_pUI->SetCapValueInt(ICAP_ROTATION,nval);
	m_advancedmap[ICAP_ROTATION] = (float)nval;

	//UpdateControls();
	m_combo_rotate.SetCurSel(nIndex);
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
	//m_advancedmap.insert(map<int, float> :: value_type(UDSCAP_BINARIZATION, (float)nval));
	//m_pUI->SetCapValueInt(UDSCAP_BINARIZATION,nval); 
	m_advancedmap[UDSCAP_BINARIZATION] = (float)nval;

	SetBinarization();
	//UpdateControls();
	m_combo_binarization.SetCurSel(nIndex);
}


void CPage_Advanced::OnCbnSelchangeAdvanced_Combo_SpiltImage()
{
	// TODO: 在此添加控件通知处理程序代码
	int nIndex = m_combo_splitimage.GetCurSel();
	CString strCBText; 
	m_combo_splitimage.GetLBText( nIndex, strCBText);
	int nval;
	if (strCBText.Find("不拆分") >= 0) //不分割图像
	{
		nval = TWSI_NONE;
	}
	else if(strCBText.Find("上下") >= 0) //水平分割图像
	{
		nval = TWSI_HORIZONTAL; 
	}
	else if(strCBText.Find("左右") >= 0) //垂直分割图像
	{
		nval = TWSI_VERTICAL; 
	}
	else
	{}
	//m_pUI->SetCapValueInt(UDSCAP_SPLITIMAGE,nval); 
	//UpdateControls();
	//m_advancedmap.insert(map<int, float> :: value_type(UDSCAP_SPLITIMAGE, (float)nval));
	m_advancedmap[UDSCAP_SPLITIMAGE] = (float)nval;

	m_combo_splitimage.SetCurSel(nIndex);
}



void CPage_Advanced::OnAdvanced_Btn_Check_Multistream()
{
	// TODO: 在此添加控件通知处理程序代码
	SetMultistream();
}

void CPage_Advanced::InitSliderCtrl()
{
	
	float fMin,fMax,fStep;
	m_pUI->GetCapRangeFloat(ICAP_CONTRAST, fMin, fMax, fStep);
	m_slider_contrast.SetRange((int)fMin, (int)fMax);
	m_slider_contrast.SetTicFreq((int)fStep);  // 设置滑动条刻度的频度为1个单位，很重要，若不加这句滑块初始位置不变

	m_pUI->GetCapRangeFloat(ICAP_BRIGHTNESS, fMin, fMax, fStep);
	m_slider_brightness.SetRange((int)fMin, (int)fMax);
	m_slider_brightness.SetTicFreq((int)fStep);

	m_pUI->GetCapRangeFloat(ICAP_GAMMA, fMin, fMax, fStep);
	m_slider_gamma.SetRange((int)fMin, (int)fMax);
	m_slider_gamma.SetTicFreq((int)fStep); //步长

	//去除空白页
	m_pUI->GetCapRangeFloat(ICAP_AUTODISCARDBLANKPAGES, fMin, fMax, fStep);
	m_slider_removeblank.SetRange((int)fMin, (int)fMax);
	m_slider_removeblank.SetTicFreq((int)fStep); //步长

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
	str.Format("%d",sldValue);

	//m_pUI->SetCapValueFloat(ICAP_GAMMA,(float)sldValue);  
	//m_advancedmap.insert(map<int, float> :: value_type(ICAP_GAMMA, (float)sldValue));
	//m_advancedmap[ICAP_GAMMA] = (float)sldValue;
	float Value = ((int)sldValue*10)/10.00f;
	m_advancedmap[ICAP_GAMMA] = Value;
	/*
	CString stra;
	stra.Format("%.2f",Value);
	::MessageBox(NULL,TEXT(stra),"Value",MB_OK);*/

	float valueTemp = ((float)sldValue)/100;
	str.Format("%.2f", valueTemp); //小数点后只要2位
	SetDlgItemText(IDC_ADVANCED_EDIT_SENSITIVE_GAMMA, str);// 在编辑框同步显示滚动条值
	UpdateData(FALSE);  // 更新控件
	//UpdateControls();

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
	//m_pUI->SetCapValueFloat(UDSCAP_SENSITIVETHRESHOLD_REMOVESPOTS,(float)sldValue);  
	//m_advancedmap.insert(map<int, float> :: value_type(UDSCAP_SENSITIVETHRESHOLD_REMOVESPOTS, (float)sldValue));
	//m_advancedmap[UDSCAP_SENSITIVETHRESHOLD_REMOVESPOTS] = (float)sldValue;
	m_advancedmap[ICAP_THRESHOLD] = (float)sldValue;

	str.Format("%d", sldValue);
	//m_edit_sensitive_threshold.SetWindowText(str);  // 在编辑框同步显示滚动条值
	SetDlgItemText(IDC_ADVANCED_EDIT_SENSITIVE_THRESHOLD, str);
	// 在编辑框同步显示滚动条值
	UpdateData(FALSE);  // 更新控件
	//UpdateControls();
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
	//m_advancedmap.insert(map<int, float> :: value_type(ICAP_BRIGHTNESS, (float)sldValue));
	//m_pUI->SetCapValueFloat(ICAP_BRIGHTNESS,(float)sldValue);  // 设置亮度为当前滚动条值
	m_advancedmap[ICAP_BRIGHTNESS] = (float)sldValue;

	str.Format("%d", sldValue);
	//m_edit_brightness.SetWindowText(str);  // 在编辑框同步显示滚动条值
	SetDlgItemText(IDC_ADVANCED_EDIT_BRIGHTNESS, str);
	UpdateData(FALSE);  // 更新控件
	//UpdateControls();
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
	//m_advancedmap.insert(map<int, float> :: value_type(ICAP_CONTRAST, (float)sldValue));
	//m_pUI->SetCapValueFloat(ICAP_CONTRAST,(float)sldValue);  // 设置对比度为当前滚动条值
	m_advancedmap[ICAP_CONTRAST] = (float)sldValue;

	str.Format("%d", sldValue);
	//m_edit_contrast.SetWindowText(str);  // 在编辑框同步显示滚动条值
	SetDlgItemText(IDC_ADVANCED_EDIT_CONTRAST, str);
	UpdateData(FALSE);  // 更新控件
	//UpdateControls();
	/*// 设置应用按钮为可用状态
	SetModified(TRUE);*/
	*pResult = 0;
}


void CPage_Advanced::OnNMCustomdrawAdvanced_Slider_Removeblank(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);  // 接收数据
	CString str;
	int sldValue = m_slider_removeblank.GetPos();  // 获取滑块当前位置
	m_advancedmap[ICAP_AUTODISCARDBLANKPAGES] = (float)sldValue;

	str.Format("%d", sldValue);
	SetDlgItemText(IDC_ADVANCED_EDIT_REMOVEBLANK, str);
	UpdateData(FALSE);  // 更新控件

	*pResult = 0;
}


void CPage_Advanced::OnEnChangeAdvanced_Edit_Removeblank()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 __super::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	UpdateData(TRUE);  // 接收数据
	CString str;
	m_edit_removeblank.GetWindowText(str);
	int nval = _ttoi(str);
	m_slider_removeblank.SetPos(nval);
	m_advancedmap[ICAP_AUTODISCARDBLANKPAGES] = (float)nval;
	m_edit_removeblank.SetSel(str.GetLength(), str.GetLength(),TRUE);  // 设置编辑框控件范围
	UpdateData(FALSE);  // 更新控件
	// TODO:  在此添加控件通知处理程序代码
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
	//m_advancedmap.insert(map<int, float> :: value_type(ICAP_CONTRAST, (float)nval));	
	//m_pUI->SetCapValueFloat(ICAP_CONTRAST,(float)nval);  // 设置对比度为当前滚动条值
	m_advancedmap[ICAP_CONTRAST] = (float)nval;

	m_edit_contrast.SetSel(str.GetLength(), str.GetLength(),TRUE);  // 设置编辑框控件范围
	UpdateData(FALSE);  // 更新控件
	//UpdateControls();
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
	//m_advancedmap.insert(map<int, float> :: value_type(UDSCAP_SENSITIVETHRESHOLD_REMOVESPOTS, (float)nval));	
	//m_pUI->SetCapValueFloat(UDSCAP_SENSITIVETHRESHOLD_REMOVESPOTS,(float)nval);  
	//m_advancedmap[UDSCAP_SENSITIVETHRESHOLD_REMOVESPOTS] = (float)nval;
	m_advancedmap[ICAP_THRESHOLD] = (float)nval;

	m_edit_sensitive_threshold.SetSel(str.GetLength(), str.GetLength(),TRUE);  // 设置编辑框控件范围
	UpdateData(FALSE);  // 更新控件
	//UpdateControls();

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

	//int nval = _ttoi(str);
	float fval = (float)(_ttof(str));
	int nval = (int)(100*fval);
	m_slider_gamma.SetPos(nval);
	//m_advancedmap.insert(map<int, float> :: value_type(ICAP_GAMMA, (float)nval));
	//m_pUI->SetCapValueFloat(ICAP_GAMMA,(float)nval);  // 设置对比度为当前滚动条值
	m_advancedmap[ICAP_GAMMA] = float(nval); //map存的是放大100倍的值

	/*
	CString strTemp;
	strTemp.Format("%.2f",(float)nval);
	::MessageBox(NULL,TEXT(strTemp),"edit",MB_OK);*/

	m_edit_gamma.SetSel(str.GetLength(), str.GetLength(),TRUE);  // 设置编辑框控件范围
	UpdateData(FALSE);  // 更新控件
	//UpdateControls();

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
	//m_advancedmap.insert(map<int, float> :: value_type(ICAP_BRIGHTNESS, (float)nval));
	//m_pUI->SetCapValueFloat(ICAP_BRIGHTNESS,(float)nval);  // 设置对比度为当前滚动条值
	m_advancedmap[ICAP_BRIGHTNESS] = (float)nval;

	m_edit_brightness.SetSel(str.GetLength(), str.GetLength(),TRUE);  // 设置编辑框控件范围
	UpdateData(FALSE);  // 更新控件
	//UpdateControls();

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
		//图像增强系列不可用
		GetDlgItem(IDC_CHECK_REMOVEDEMOISE)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_REMOVEDESCREEN)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_SHARPEN)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_REMOVEBACK)->EnableWindow(FALSE);
	} 
	else
	{
		GetDlgItem(IDC_ADVANCED_COMBO_BINARIZATION)->EnableWindow(FALSE);
		//图像增强系列可用
		GetDlgItem(IDC_CHECK_REMOVEDEMOISE)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_REMOVEDESCREEN)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_SHARPEN)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_REMOVEBACK)->EnableWindow(TRUE);
	}
	SetBinarization(); //设置“去除斑点”还是“底色保留”
}

//彩色正面
void CPage_Advanced::OnAdvanced_Btn_Check_FrontColor()
{
	// TODO: 在此添加控件通知处理程序代码
	SetColorGrayImage();

	int nval;
	if (m_check_frontcolor.GetCheck())
	{
		nval = TWFC_AUTO;
	} 
	else
	{
		nval = TWFC_DISABLE;
	}
	m_advancedmap[UDSCAP_FRONTCOLOR] = (float)nval;
	GetCheckNum();
}

//灰度正面
void CPage_Advanced::OnAdvanced_Btn_Check_FrontGray()
{
	// TODO: 在此添加控件通知处理程序代码
	SetColorGrayImage();

	int nval;
	if (m_check_frontgray.GetCheck())
	{
		nval = TWFG_AUTO;
	} 
	else
	{
		nval = TWFG_DISABLE;
	}
	m_advancedmap[UDSCAP_FRONTGRAY] = (float)nval;
	GetCheckNum();
}

//黑白正面
void CPage_Advanced::OnAdvanced_Btn_Check_FrontBW()
{
	// TODO: 在此添加控件通知处理程序代码
	SetBWImage();

	int nval;
	if (m_check_frontbw.GetCheck())
	{
		nval = TWFB_AUTO;
	} 
	else
	{
		nval = TWFB_DISABLE;
	}
	m_advancedmap[UDSCAP_FRONTBW] = (float)nval;

	GetCheckNum();
}

//彩色背面
void CPage_Advanced::OnAdvanced_Btn_Check_BackColor()
{
	// TODO: 在此添加控件通知处理程序代码
	SetColorGrayImage();

	int nval;
	if (m_check_backcolor.GetCheck())
	{
		nval = TWBC_AUTO;
	} 
	else
	{
		nval = TWBC_DISABLE;
	}
	m_advancedmap[UDSCAP_BACKCOLOR] = (float)nval;
	GetCheckNum();
}

//灰度背面
void CPage_Advanced::OnAdvanced_Btn_Check_BackGray()
{
	// TODO: 在此添加控件通知处理程序代码
	SetColorGrayImage();

	int nval;
	if (m_check_backgray.GetCheck())
	{
		nval = TWBG_AUTO;
	} 
	else
	{
		nval = TWBG_DISABLE;
	}
	m_advancedmap[UDSCAP_BACKGRAY] = (float)nval;
	GetCheckNum();
}

//黑白背面
void CPage_Advanced::OnAdvanced_Btn_Check_BackBW()
{
	// TODO: 在此添加控件通知处理程序代码
	SetBWImage();

	int nval;
	if (m_check_backbw.GetCheck())
	{
		nval = TWBB_AUTO;
	} 
	else
	{
		nval = TWBB_DISABLE;
	}
	m_advancedmap[UDSCAP_BACKBW] = (float)nval;
	GetCheckNum();
}

//去除空白页
void CPage_Advanced::OnAdvanced_Btn_Check_RemoveBlank()
{
	// TODO: 在此添加控件通知处理程序代码
	int nval;
	if (m_check_removeblank.GetCheck())
	{
		nval = TWRA_AUTO;
	} 
	else
	{
		nval = TWRA_DISABLE;
	}
	m_advancedmap[UDSCAP_REMOVEBLANK] = (float)nval;

	SetBlank();
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
	//m_pUI->SetCapValueInt(UDSCAP_PUNCHHOLEREMOVEL,nval); 
	//m_advancedmap.insert(map<int, float> :: value_type(UDSCAP_PUNCHHOLEREMOVEL, (float)nval));
	m_advancedmap[UDSCAP_PUNCHHOLEREMOVEL] = (float)nval;

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
	//m_pUI->SetCapValueInt(UDSCAP_SHARPEN,nval); 
	//m_advancedmap.insert(map<int, float> :: value_type(UDSCAP_SHARPEN, (float)nval));
	m_advancedmap[UDSCAP_SHARPEN] = (float)nval;
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
	//m_pUI->SetCapValueInt(UDSCAP_MIRROR,nval); 
	//m_advancedmap.insert(map<int, float> :: value_type(UDSCAP_MIRROR, (float)nval));
	m_advancedmap[UDSCAP_MIRROR] = (float)nval;
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
	//m_pUI->SetCapValueInt(UDSCAP_REMOVEBACKGROUND,nval); 
	//m_advancedmap.insert(map<int, float> :: value_type(UDSCAP_REMOVEBACKGROUND, (float)nval));
	m_advancedmap[UDSCAP_REMOVEBACKGROUND] = (float)nval;
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
	//m_pUI->SetCapValueInt(UDSCAP_DESCREEN,nval); 
	//m_advancedmap.insert(map<int, float> :: value_type(UDSCAP_DESCREEN, (float)nval));
	m_advancedmap[UDSCAP_DESCREEN] = (float)nval;
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
	//m_pUI->SetCapValueInt(UDSCAP_DENOISE,nval);
	//m_advancedmap.insert(map<int, float> :: value_type(UDSCAP_DENOISE, (float)nval));
	m_advancedmap[UDSCAP_DENOISE] = (float)nval;
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
	//m_pUI->SetCapValueInt(UDSCAP_AUTOCROP,nval);
	//m_advancedmap.insert(map<int, float> :: value_type(UDSCAP_AUTOCROP, (float)nval));
	m_advancedmap[UDSCAP_AUTOCROP] = (float)nval;
}


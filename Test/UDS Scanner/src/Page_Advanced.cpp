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

// CPage_Advanced 对话框

IMPLEMENT_DYNAMIC(CPage_Advanced, CPropertyPage)

CPage_Advanced::CPage_Advanced(MFC_UI *pUI)
	: m_pUI(pUI), CPropertyPage(CPage_Advanced::IDD)
{
	m_radiobtn_spilt_vert = 0;
}

CPage_Advanced::~CPage_Advanced()
{
	m_advancedmap.swap(map<int, float>());  // 清空并释放内存
}

void CPage_Advanced::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ADVANCED_COMBO_ROTATE, m_combo_rotate);
	DDX_Control(pDX, IDC_ADVANCED_COMBO_SPLITIMG, m_combo_splitimage);
	DDX_Control(pDX, IDC_ADVANCED_EDIT_SENSITIVE_GAMMA, m_edit_gamma);
	DDX_Control(pDX, IDC_ADVANCED_SLIDER_SENSITIVE_GAMMA, m_slider_gamma);
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
	DDX_Control(pDX, IDC_CHECK_MULTIFEEDDETECT, m_check_multifeeddetect);
	DDX_Control(pDX, IDC_ADVANCED_EDIT_SPILT, m_edit_spilt);
	DDX_Radio(pDX, IDC_ADVANCED_RADIO_VERTICAL, m_radiobtn_spilt_vert);
	DDX_Control(pDX, IDC_ADVANCED_CHECK_COLORFLIP, m_check_colorflip);
	DDX_Control(pDX, IDC_ADVANCED_COMBO_CACHEMODE, m_combo_cachemode);
	DDX_Control(pDX, IDC_ADVANCED_EDIT_CACHEMODE, m_edit_cachemode);
	DDX_Control(pDX, IDC_ADVANCED_SLIDER_CACHEMODE, m_slider_cachemode);
}


BEGIN_MESSAGE_MAP(CPage_Advanced, CPropertyPage)
	ON_CBN_SELCHANGE(IDC_ADVANCED_COMBO_ROTATE, &CPage_Advanced::OnCbnSelchangeAdvanced_Combo_Rotate)
	ON_CBN_SELCHANGE(IDC_ADVANCED_COMBO_SPLITIMG, &CPage_Advanced::OnCbnSelchangeAdvanced_Combo_SpiltImage)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_ADVANCED_SLIDER_SENSITIVE_GAMMA, &CPage_Advanced::OnNMCustomdrawAdvanced_Slider_Gamma)
	ON_EN_CHANGE(IDC_ADVANCED_EDIT_SENSITIVE_GAMMA, &CPage_Advanced::OnEnChangeAdvanced_Edit_Gamma)
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
	ON_BN_CLICKED(IDC_CHECK_MULTIFEEDDETECT, &CPage_Advanced::OnClicked_Check_Multifeeddetect)
	ON_BN_CLICKED(IDC_ADVANCED_RADIO_VERTICAL, &CPage_Advanced::OnAdvanced_RadioBtn_Spilt)
	ON_BN_CLICKED(IDC_ADVANCED_CHECK_COLORFLIP, &CPage_Advanced::OnAdvanced_Btn_Check_Colorflip)
	ON_CBN_SELCHANGE(IDC_ADVANCED_COMBO_CACHEMODE, &CPage_Advanced::OnCbnSelchangeAdvanced_Combo_Cachemode)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_ADVANCED_SLIDER_CACHEMODE, &CPage_Advanced::OnNMCustomdrawAdvanced_Slider_Cachemode)
	ON_EN_CHANGE(IDC_ADVANCED_EDIT_CACHEMODE, &CPage_Advanced::OnEnChangeAdvanced_Edit_Cachemode)
END_MESSAGE_MAP()


void CPage_Advanced::SetCapValue(void)
{
	MAP_CAP::iterator iter;
	if (true == m_advancedmap.empty())
	{
		return;
	}
	for(iter = m_advancedmap.begin(); iter != m_advancedmap.end(); iter++)
	{
		switch(iter->first)
		{
		case ICAP_ROTATION:  //旋转
		case UDSCAP_DENOISE: //去噪声
		case UDSCAP_DESCREEN: //去网纹
		case UDSCAP_REMOVEBACKGROUND: //去背景
		case UDSCAP_SHARPEN: //图像锐化
		case UDSCAP_REMOVEBLANK: //去除空白页的checkbox
		case UDSCAP_MIRROR: //镜像处理
		case UDSCAP_MULTIFEEDDETECT: //重张检测
		case UDSCAP_COLORFLIP: //色彩翻转
		case UDSCAP_CACHEMODE: //缓存模式
			{
				m_pUI->SetCapValueInt(iter->first,(int)(iter->second));
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
			
		case UDSCAP_SPLITIMAGE: //图像分割
			{
				m_pUI->SetCapValueInt(iter->first,(int)(iter->second)); //设置参数生效

				if(m_combo_splitimage.IsWindowEnabled())
				{
					if((int)(iter->second) == TWSI_HORIZONTAL || (int)(iter->second) == TWSI_VERTICAL)
					{
						m_pUI->SetCapValueInt(UDSCAP_DOCS_IN_ADF, 2); //设置扫描张数为2
					}
					else if((int)(iter->second) == TWSI_NONE && m_pBasePage->scanside == 0) //不拆分&&单面
					{
						m_pUI->SetCapValueInt(UDSCAP_DOCS_IN_ADF, 1); //不拆分时又设回1
					}	
					/*else if((int)(iter->second) == TWSI_DEFINED)
					{
						if(m_edit_spilt.IsWindowEnabled())
						{
							CString str;
							m_edit_spilt.GetWindowText(str);
							int num = _ttoi(str);
							m_pUI->SetCapValueInt(UDSCAP_DOCS_IN_ADF, num); 
						}
						else{}
					}*/
					else{}
				}
				else
				{
					m_pUI->SetCapValueInt(iter->first, TWSI_NONE); //设置参数生效
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
				m_pUI->SetCapValueFloat(iter->first,iter->second);
				break;
			}
		
		case UDSCAP_CACHEMODE_AUTO: //缓存模式--自动
		case UDSCAP_CACHEMODE_PAPERNUM:
		case UDSCAP_CACHEMODE_MEMORYSIZE:
			{
				m_pUI->SetCapValueFloat(iter->first,iter->second);
				break;
			}

		default:
			{
				break;
			}	

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
	int nval;

	//重张检测：默认使用
	nCapValue = (int)(m_pUI->GetCapValueBool(UDSCAP_MULTIFEEDDETECT));
	m_check_multifeeddetect.SetCheck(nCapValue);

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
	nval = (int)lstCapValuesFlt->at(nCapIndex);
	m_advancedmap[ICAP_ROTATION] = (float)nval;

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
		case TWSI_DEFINED:
			m_combo_splitimage.InsertString(i,"自定义"); //垂直分割图像
			break;
		default:
			continue;
		}
	}
	m_combo_splitimage.SetCurSel(nCapIndex);
	nval = (int)lstCapValuesFlt->at(nCapIndex);
	m_advancedmap[UDSCAP_SPLITIMAGE] = (float)nval;//不能只更新容器，还要更新CAP
	
	//Gamma校正 
	nCapValue = (int)(m_pUI->GetCapValueFloat(ICAP_GAMMA)); //GetCapValueFloat能否得到CTWAINContainerFix32类型
	m_slider_gamma.SetPos(nCapValue);
	float valueTemp = ((float)nCapValue)/100;
	strText.Format("%.2f", valueTemp);
	SetDlgItemText(IDC_ADVANCED_EDIT_SENSITIVE_GAMMA, strText);
	m_advancedmap[ICAP_GAMMA] = float(nCapValue);

	//去除空白页checkbox
	nCapValue = (int)(m_pUI->GetCapValueBool(UDSCAP_REMOVEBLANK));
	m_check_removeblank.SetCheck(nCapValue);
	m_advancedmap[UDSCAP_REMOVEBLANK] = (float)nCapValue;
	//去除空白页 -1自动;-2不可用:改为滑动条 
	nCapValue = (int)(m_pUI->GetCapValueFloat(ICAP_AUTODISCARDBLANKPAGES)); 
	m_slider_removeblank.SetPos(nCapValue);
	strText.Format("%d",nCapValue);
	SetDlgItemText(IDC_ADVANCED_EDIT_REMOVEBLANK, strText);
	m_advancedmap[ICAP_AUTODISCARDBLANKPAGES] = float(nCapValue);
	
	//去除穿孔等
	nCapValue = (int)(m_pUI->GetCapValueBool(UDSCAP_PUNCHHOLEREMOVEL));
	m_check_removepunch.SetCheck(nCapValue);
	m_advancedmap[UDSCAP_PUNCHHOLEREMOVEL] = (float)nCapValue;

	nCapValue = (int)(m_pUI->GetCapValueBool(UDSCAP_SHARPEN));
	m_check_sharpen.SetCheck(nCapValue);
	m_advancedmap[UDSCAP_SHARPEN] = (float)nCapValue;

	nCapValue = (int)(m_pUI->GetCapValueBool(UDSCAP_MIRROR));
	m_check_mirror.SetCheck(nCapValue);
	m_advancedmap[UDSCAP_MIRROR] = (float)nCapValue;

	nCapValue = (int)(m_pUI->GetCapValueBool(UDSCAP_REMOVEBACKGROUND));
	m_check_removeback.SetCheck(nCapValue);
	m_advancedmap[UDSCAP_REMOVEBACKGROUND] = (float)nCapValue;

	nCapValue = (int)(m_pUI->GetCapValueBool(UDSCAP_DESCREEN));
	m_check_removedescreen.SetCheck(nCapValue);
	m_advancedmap[UDSCAP_DESCREEN] = (float)nCapValue;

	nCapValue = (int)(m_pUI->GetCapValueBool(UDSCAP_DENOISE));
	m_check_removedenoise.SetCheck(nCapValue); 
	m_advancedmap[UDSCAP_DENOISE] = (float)nCapValue;

	nCapValue = (int)(m_pUI->GetCapValueBool(UDSCAP_AUTOCROP));
	m_check_autocrop.SetCheck(nCapValue);
	m_advancedmap[UDSCAP_AUTOCROP] = (float)nCapValue;

	nCapValue = (int)(m_pUI->GetCapValueBool(UDSCAP_COLORFLIP));
	m_check_colorflip.SetCheck(nCapValue);
	m_advancedmap[UDSCAP_COLORFLIP] = (float)nCapValue;

	//缓存模式
	m_combo_cachemode.ResetContent(); //清空内容
	nCapIndex = m_pUI->GetCurrentCapIndex(UDSCAP_CACHEMODE);
	SetColorMode(nCapIndex); //设置滑动条的范围与值
	lstCapValues = m_pUI->GetValidCap(UDSCAP_CACHEMODE);	
	for(unsigned int i=0; i<lstCapValues->size();i++)
	{
		switch(lstCapValues->at(i))
		{
		case TWCM_NONE:
			m_combo_cachemode.InsertString(i,"自动"); //自动
			break;
		case TWCM_PAPERNUM:
			m_combo_cachemode.InsertString(i,"纸张数量");  //纸张数量		
			break;
		case TWCM_MEMORYSIZE:
			m_combo_cachemode.InsertString(i,"内存大小"); //内存大小
			break;
		default:
			continue;
		}
	}
	m_combo_cachemode.SetCurSel(nCapIndex);
	nval = lstCapValues->at(nCapIndex);
	m_advancedmap[UDSCAP_CACHEMODE] = (float)nval;//不能只更新容器，还要更新CAP
 
}

void CPage_Advanced::SetColorMode(int nIndex)
{
	float fMin,fMax,fStep;
	int value;
	CString str;
	switch(nIndex)
	{
	case 0:
		SetDlgItemText(IDC_ADVANCED_SLIDERSTATIC_CACHEMODE, TEXT("图像张数:"));
		SetDlgItemText(IDC_ADVANCED_STATIC_CACHEMODEUNIT, TEXT("页"));
		
		m_pUI->GetCapRangeFloat(UDSCAP_CACHEMODE_AUTO, fMin, fMax, fStep);
		//设置滑动条范围
		m_slider_cachemode.SetRange((int)fMin, (int)fMax); //设置范围
		m_slider_cachemode.SetTicFreq((int)fStep); //步长

		value = (int)(m_pUI->GetCapValueFloat(UDSCAP_CACHEMODE_AUTO));	
		m_slider_cachemode.SetPos(value);	
		if(value == 0)
		{
			str = "无限";
		}
		else
		{
			str.Format("%d",value);
		}
		SetDlgItemText(IDC_ADVANCED_EDIT_CACHEMODE, str);

		break;
	case 1:
		SetDlgItemText(IDC_ADVANCED_SLIDERSTATIC_CACHEMODE, TEXT("纸张数量:"));
		SetDlgItemText(IDC_ADVANCED_STATIC_CACHEMODEUNIT, TEXT("页"));

		m_pUI->GetCapRangeFloat(UDSCAP_CACHEMODE_PAPERNUM, fMin, fMax, fStep);
		//设置滑动条范围
		m_slider_cachemode.SetRange((int)fMin, (int)fMax); //设置范围
		m_slider_cachemode.SetTicFreq((int)fStep); //步长

		value = (int)(m_pUI->GetCapValueFloat(UDSCAP_CACHEMODE_PAPERNUM));	
		m_slider_cachemode.SetPos(value);
		str.Format("%d",value);
		SetDlgItemText(IDC_ADVANCED_EDIT_CACHEMODE, str);

		break;
	case 2:
		SetDlgItemText(IDC_ADVANCED_SLIDERSTATIC_CACHEMODE, TEXT("内存大小:"));
		SetDlgItemText(IDC_ADVANCED_STATIC_CACHEMODEUNIT, TEXT("MB"));

		m_pUI->GetCapRangeFloat(UDSCAP_CACHEMODE_MEMORYSIZE, fMin, fMax, fStep);
		//设置滑动条范围
		m_slider_cachemode.SetRange((int)fMin, (int)fMax); //设置范围
		m_slider_cachemode.SetTicFreq((int)fStep); //步长

		value = (int)(m_pUI->GetCapValueFloat(UDSCAP_CACHEMODE_MEMORYSIZE));
		m_slider_cachemode.SetPos(value);	
		if(value == 0)
		{
			str = "默认";
		}
		else
		{
			str.Format("%d",value);
		}
		SetDlgItemText(IDC_ADVANCED_EDIT_CACHEMODE, str);

		break;
	}
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
	SetSpiltimage();

	//暂时隐藏多份拆分
	GetDlgItem(IDC_ADVANCED_RADIO_VERTICAL)->ShowWindow(FALSE);
	GetDlgItem(IDC_ADVANCED_RADIO_HORIZONTAL)->ShowWindow(FALSE);
	GetDlgItem(IDC_ADVANCED_EDIT_SPILT)->ShowWindow(FALSE);

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
	
	m_advancedmap[ICAP_ROTATION] = (float)nval;
	m_combo_rotate.SetCurSel(nIndex);
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
	else if(strCBText.Find("自定义") >= 0) //垂直分割图像
	{
		nval = TWSI_DEFINED; 
	}
	else
	{}
	m_advancedmap[UDSCAP_SPLITIMAGE] = (float)nval;
	m_combo_splitimage.SetCurSel(nIndex);
	SetSpiltimage();
}


void CPage_Advanced::InitSliderCtrl()
{
	float fMin,fMax,fStep;
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

	float Value = ((int)sldValue*10)/10.00f;
	m_advancedmap[ICAP_GAMMA] = Value;

	float valueTemp = ((float)sldValue)/100;
	str.Format("%.2f", valueTemp); //小数点后只要2位
	SetDlgItemText(IDC_ADVANCED_EDIT_SENSITIVE_GAMMA, str);// 在编辑框同步显示滚动条值
	UpdateData(FALSE);  // 更新控件

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

	float fval = (float)(_ttof(str));
	int nval = (int)(100*fval);
	m_slider_gamma.SetPos(nval);
	m_advancedmap[ICAP_GAMMA] = float(nval); //map存的是放大100倍的值

	m_edit_gamma.SetSel(str.GetLength(), str.GetLength(),TRUE);  // 设置编辑框控件范围
	UpdateData(FALSE);  // 更新控件
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
		nval = TWRP_AUTO;
	} 
	else
	{
		nval = TWRP_DISABLE;
	}
	m_advancedmap[UDSCAP_PUNCHHOLEREMOVEL] = (float)nval;
}


//锐化图像
void CPage_Advanced::OnAdvanced_Btn_Check_Sharpen()
{
	// TODO: 在此添加控件通知处理程序代码
	int nval;
	if (m_check_sharpen.GetCheck())
	{
		nval = TWSP_AUTO;
	} 
	else
	{
		nval = TWSP_DISABLE;
	} 
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
	m_advancedmap[UDSCAP_AUTOCROP] = (float)nval;
}

void CPage_Advanced::OnAdvanced_Btn_Check_Colorflip()
{
	// TODO: 在此添加控件通知处理程序代码
	int nval;
	if (m_check_colorflip.GetCheck())
	{
		nval = TWCF_AUTO;
	} 
	else
	{
		nval = TWCF_DISABLE;
	}
	m_advancedmap[UDSCAP_COLORFLIP] = (float)nval;
}


//PreTranslateMessage返回TRUE时，不会把消息发送给窗口函数处理
BOOL CPage_Advanced::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	CEdit* pEdit1 = (CEdit*)GetDlgItem(IDC_ADVANCED_EDIT_BRIGHTNESS);  
	CEdit* pEdit2 = (CEdit*)GetDlgItem(IDC_ADVANCED_EDIT_CONTRAST);  

	CString str1, str2;   
	GetDlgItemText(IDC_ADVANCED_EDIT_BRIGHTNESS, str1); // 获取edit中文本  
	GetDlgItemText(IDC_ADVANCED_EDIT_CONTRAST, str2);

	if( (GetFocus() == pEdit1 ||GetFocus() == pEdit2) && (pMsg->message == WM_CHAR))  
	{  
		//允许输入数字//和小数点“.”
		if((pMsg->wParam >= '0' && pMsg->wParam <= '9'))   
		{  
			return 0;  
		} 
		else if(pMsg->wParam == '.')
		{
			return 1; //不准输入小数点
		}
		// 保证负号'-'只能出现一次,并且只能出现在第一个字符
		else if (pMsg->wParam == '-') //亮度、对比度只能输入负号与数字
		{
			if(str1.IsEmpty() || str2.IsEmpty())
			{
				return 0; //第一位时才能输入
			}
			else 
			{
				return 1;
			}
		}
		//接受Backspace和delete键 
		else if(pMsg->wParam == 0x08 || pMsg->wParam == 0x2E)  
		{  
			return 0;  
		}  
		else
		{ 
			return 1; 
		}
	}  

	//需要输入小数点的Edit只允许一个小数点
	CEdit* pEdit3 = (CEdit*)GetDlgItem(IDC_ADVANCED_EDIT_SENSITIVE_GAMMA); 
	CString str3;   
	GetDlgItemText(IDC_ADVANCED_EDIT_SENSITIVE_GAMMA, str3); // 获取edit中文本  
	if( (GetFocus() == pEdit3) && (pMsg->message == WM_CHAR))  
	{  
		//允许输入数字和小数点“.”
		if(pMsg->wParam == '.')
		{
			//输入框只允许输入一个小数点
			int nPos3 = 0; 
			nPos3 = str3.Find('.'); // 查找.的位置 
	
			if(nPos3 >= 0)  //必须分开写，或||操作的话总会满足
			{  
				return 1;   //如果存在,返回,即不再允许输入
			}	
			return 0;
		}
		//接受Backspace和delete键 
		else if(pMsg->wParam == 0x08 || pMsg->wParam == 0x2E)  
		{  
			return 0;  
		}  
		else if((pMsg->wParam >= '0' && pMsg->wParam <= '9'))   
		{  
			return 0;  
		} 
		else
		{ 
			return 1; 
		}
	}

	//必须分开写，或||操作的话总会满足
	CEdit* pEdit4 = (CEdit*)GetDlgItem(IDC_ADVANCED_EDIT_CUSTOMWIDTH); 
	CString str4;     
	GetDlgItemText(IDC_ADVANCED_EDIT_CUSTOMWIDTH, str4);
	if( (GetFocus() == pEdit4) && (pMsg->message == WM_CHAR))  
	{  
		//允许输入数字和小数点“.”
		if(pMsg->wParam == '.')
		{
			//输入框只允许输入一个小数点
			int nPos4 = 0; 
			nPos4 = str4.Find('.');
			if(nPos4 >= 0)  
			{  
				return 1;   //如果存在,返回,即不再允许输入
			}	
			return 0;
		}
		//接受Backspace和delete键 
		else if(pMsg->wParam == 0x08 || pMsg->wParam == 0x2E)  
		{  
			return 0;  
		}  
		else if((pMsg->wParam >= '0' && pMsg->wParam <= '9'))   
		{  
			return 0;  
		} 
		else
		{ 
			return 1; 
		}
	}

	CEdit* pEdit5 = (CEdit*)GetDlgItem(IDC_ADVANCED_EDIT_CUSTOMHEIGHT);
	CString str5;   
	GetDlgItemText(IDC_ADVANCED_EDIT_CUSTOMHEIGHT, str5);
	if( (GetFocus() == pEdit5) && (pMsg->message == WM_CHAR))  
	{  
		//允许输入数字和小数点“.”
		if(pMsg->wParam == '.')
		{
			//输入框只允许输入一个小数点
			int nPos5 = 0; 
			nPos5 = str5.Find('.');
			if(nPos5 >= 0)
			{
				return 1;
			}
			return 0;
		}
		//接受Backspace和delete键 
		else if(pMsg->wParam == 0x08 || pMsg->wParam == 0x2E)  
		{  
			return 0;  
		}  
		else if((pMsg->wParam >= '0' && pMsg->wParam <= '9'))   
		{  
			return 0;  
		} 
		else
		{ 
			return 1; 
		}
	}

	return __super::PreTranslateMessage(pMsg);
}

void CPage_Advanced::OnClicked_Check_Multifeeddetect()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_check_multifeeddetect.GetCheck()) //点中
	{
		m_advancedmap[UDSCAP_MULTIFEEDDETECT] = TRUE;
	} 
	else
	{
		m_advancedmap[UDSCAP_MULTIFEEDDETECT] = FALSE;
	}
}


BOOL CPage_Advanced::OnSetActive()
{
	// TODO: 在此添加专用代码和/或调用基类
	m_pUI->PreViewStatus();
	return __super::OnSetActive();
}


void CPage_Advanced::SetSpiltimage()
{
	CString strCBText;
	GetDlgItemText(IDC_ADVANCED_COMBO_SPLITIMG, strCBText);

	CString str;
	str.Format("%d",2);

	if(strCBText.Find("不拆分") >= 0)
	{
		str.Format("%d",1);
		SetDlgItemText(IDC_ADVANCED_EDIT_SPILT, str);

		m_edit_spilt.EnableWindow(FALSE);
		GetDlgItem(IDC_ADVANCED_RADIO_HORIZONTAL)->EnableWindow(FALSE);
		GetDlgItem(IDC_ADVANCED_RADIO_VERTICAL)->EnableWindow(FALSE);
	}
	else if(strCBText.Find("上下") >= 0 || strCBText.Find("左右") >= 0) //水平、垂直分割图像
	{
		SetDlgItemText(IDC_ADVANCED_EDIT_SPILT, str);

		m_edit_spilt.EnableWindow(FALSE);
		GetDlgItem(IDC_ADVANCED_RADIO_HORIZONTAL)->EnableWindow(FALSE);
		GetDlgItem(IDC_ADVANCED_RADIO_VERTICAL)->EnableWindow(FALSE);
	}
	else if(strCBText.Find("自定义") >= 0)
	{
		SetDlgItemText(IDC_ADVANCED_EDIT_SPILT, str);
		m_edit_spilt.EnableWindow(TRUE);
		GetDlgItem(IDC_ADVANCED_RADIO_HORIZONTAL)->EnableWindow(TRUE);
		GetDlgItem(IDC_ADVANCED_RADIO_VERTICAL)->EnableWindow(TRUE);
	}
	else{}
}


void CPage_Advanced::OnAdvanced_RadioBtn_Spilt()
{
	// TODO: 在此添加控件通知处理程序代码
	//switch(m_radiobtn_spilt_hori)
	//	//{
	//	//case 0:
	//	//	//m_advancedmap[UDSCAP_SPLITIMAGE_VH] = (float)m_radiobtn_spilt_hori;
	//	//	break;
	//	//case 1:
	//	//	break;
	//	//}
}


void CPage_Advanced::OnCbnSelchangeAdvanced_Combo_Cachemode()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	int nIndex = m_combo_cachemode.GetCurSel();
	CString strCBText; 
	m_combo_cachemode.GetLBText( nIndex, strCBText);
	int nval;
	if (strCBText.Find("自动") >= 0)
	{
		nval = TWCM_NONE;
	}
	else if(strCBText.Find("纸张数量") >= 0)
	{
		nval = TWCM_PAPERNUM; 
	}
	else if(strCBText.Find("内存大小") >= 0)
	{
		nval = TWCM_MEMORYSIZE; 
	}
	else
	{}

	m_advancedmap[UDSCAP_CACHEMODE] = (float)nval;
	m_combo_cachemode.SetCurSel(nIndex);
	SetColorMode(nIndex);
	UpdateData(FALSE);
}


void CPage_Advanced::OnNMCustomdrawAdvanced_Slider_Cachemode(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);  // 接收数据
	CString str;
	int sldValue = m_slider_cachemode.GetPos();  // 获取滑块当前位置
	int nIndex = m_combo_cachemode.GetCurSel();
	switch(nIndex)
	{
	case 0:
		{
			m_advancedmap[UDSCAP_CACHEMODE_AUTO] = (float)sldValue;

			if(sldValue == 0){
				str = "无限";
			}
			else{
				str.Format("%d",sldValue);
			}
			SetDlgItemText(IDC_ADVANCED_EDIT_CACHEMODE, str);
		}		
		break;
	case 1:
		{
			m_advancedmap[UDSCAP_CACHEMODE_PAPERNUM] = (float)sldValue;

			m_slider_cachemode.SetPos(sldValue);
			str.Format("%d",sldValue);
			SetDlgItemText(IDC_ADVANCED_EDIT_CACHEMODE, str);	
		}		
		break;
	case 2:
		{
			m_advancedmap[UDSCAP_CACHEMODE_MEMORYSIZE] = (float)sldValue;

			if(sldValue == 0){
				str = "默认";
			}
			else{
				str.Format("%d",sldValue);
			}
			SetDlgItemText(IDC_ADVANCED_EDIT_CACHEMODE, str);
		}	
		break;
	}
	UpdateData(FALSE);  // 更新控件

	*pResult = 0;
}


void CPage_Advanced::OnEnChangeAdvanced_Edit_Cachemode()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 __super::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);  // 接收数据
	CString str;
	m_edit_cachemode.GetWindowText(str);
	int nval = _ttoi(str);
	m_slider_cachemode.SetPos(nval);

	int nIndex = m_combo_cachemode.GetCurSel();
	switch(nIndex)
	{
	case 0:
		if(nval > 100)
		{
			nval = 100;
		}
		else if(nval < 0)
		{
			nval = 0;
		}
		else{}
		m_advancedmap[UDSCAP_CACHEMODE_AUTO] = (float)nval;	
		break;
	case 1:
		if(nval > 25)
		{
			nval = 25;
		}
		else if(nval < 0)
		{
			nval = 0;
		}
		else{}
		m_advancedmap[UDSCAP_CACHEMODE_PAPERNUM] = (float)nval;		
		break;
	case 2:
		if(nval > 1024)
		{
			nval = 1024;
		}
		else if(nval < 0)
		{
			nval = 0;
		}
		else{}
		m_advancedmap[UDSCAP_CACHEMODE_MEMORYSIZE] = (float)nval;
		break;
	}
	m_edit_cachemode.SetSel(str.GetLength(), str.GetLength(),TRUE);  // 设置edit光标位置
	UpdateData(FALSE);  // 更新控件
}

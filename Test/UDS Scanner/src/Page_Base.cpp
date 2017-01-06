// Page_Base.cpp : 实现文件
//

#include "stdafx.h"
#include "UDS Scanner.h"
#include "Page_Base.h"
#include "afxdialogex.h"
#include "Dlg_Profile.h"

#include "CTWAINDS_UDS.h"

// CPage_Base 对话框
extern HWND g_hwndDLG;
extern bool muiltstream; //高级界面多流不选中

IMPLEMENT_DYNAMIC(CPage_Base, CPropertyPage)

CPage_Base::CPage_Base(MFC_UI *pUI)
	: m_pUI(pUI),CPropertyPage(CPage_Base::IDD)
{

}


CPage_Base::~CPage_Base()
{
	/*if (m_pDlg)
	{
		delete m_pDlg;
		m_pDlg = NULL;
	}
	*/
}

void CPage_Base::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BASE_COMBO_SOURCE, m_combo_source);
	DDX_Control(pDX, IDC_BASE_COMBO_COLORMODE, m_combo_colormode);
	DDX_Control(pDX, IDC_BASE_COMBO_RESOLUTION, m_combo_resolution);
	DDX_Control(pDX, IDC_BASE_COMBO_SCANSIDE, m_combo_scanside);
	DDX_Control(pDX, IDC_BASE_SLIDER_CONTRAST, m_slider_contrast);
	DDX_Control(pDX, IDC_BASE_SLIDER_BRIGHTNESS, m_slider_brightness);
	DDX_Control(pDX, IDC_BASE_SLIDER_THRESHOLD, m_slider_threshold);
	DDX_Control(pDX, IDC_CHECK_MULTIFEEDDETECT, m_check_multifeeddetect);
	DDX_Control(pDX, IDC_BASE_EDIT_BRIGHTNESS, m_edit_brightness);
	DDX_Control(pDX, IDC_BASE_EDIT_CONTRAST, m_edit_contrast);
	DDX_Control(pDX, IDC_BASE_EDIT_THRESHOLD, m_edit_threshold);
	DDX_Control(pDX, IDC_BASE_COMBO_PROFILE, m_combo_profile);
	DDX_Control(pDX, IDC_BASE_BTN_CHOOSEIMAGE, m_btn_chooseimage);
}


BEGIN_MESSAGE_MAP(CPage_Base, CPropertyPage)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_BASE_SLIDER_CONTRAST, &CPage_Base::OnNMCustomdrawBase_Slider_Contrast)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_BASE_SLIDER_BRIGHTNESS, &CPage_Base::OnNMCustomdrawBase_Slider_Brightness)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_BASE_SLIDER_THRESHOLD, &CPage_Base::OnNMCustomdrawBase_Slider_Threshold)
	ON_EN_CHANGE(IDC_BASE_EDIT_CONTRAST, &CPage_Base::OnEnChangeBase_Edit_Contrast)
	ON_EN_CHANGE(IDC_BASE_EDIT_BRIGHTNESS, &CPage_Base::OnEnChangeBase_Edit_Brightness)
	ON_EN_CHANGE(IDC_BASE_EDIT_THRESHOLD, &CPage_Base::OnEnChangeBase_Edit_Threshold)
	ON_CBN_SELCHANGE(IDC_BASE_COMBO_SOURCE, &CPage_Base::OnCbnSelchangeBase_Combo_Source)
	ON_CBN_SELCHANGE(IDC_BASE_COMBO_COLORMODE, &CPage_Base::OnCbnSelchangeBase_Combo_Colormode)
	ON_CBN_SELCHANGE(IDC_BASE_COMBO_RESOLUTION, &CPage_Base::OnCbnSelchangeBase_Combo_Resolution)
	ON_CBN_SELCHANGE(IDC_BASE_COMBO_PROFILE, &CPage_Base::OnCbnSelchangeBase_Combo_Profile)
	ON_CBN_SELCHANGE(IDC_BASE_COMBO_SCANSIDE, &CPage_Base::OnCbnSelchangeBase_Combo_Scanside)
	ON_BN_CLICKED(IDC_BASE_BTN_DELETEPROFILE, &CPage_Base::OnBase_Btn_Deleteprofile)
	ON_BN_CLICKED(IDC_BASE_BTN_CHOOSEIMAGE, &CPage_Base::OnBase_Btn_Chooseimage)	
	ON_BN_CLICKED(IDC_CHECK_MULTIFEEDDETECT, &CPage_Base::OnClicked_Check_Multifeeddetect)
	ON_BN_CLICKED(IDC_BASE_BTN_SAVEASPROFILE, &CPage_Base::OnBase_Btn_SaveAsprofile)
//	ON_BN_CLICKED(IDC_BASE_BTN_SAVEPROFILE, &CPage_Base::OnBase_Btn_Saveprofile)
//	ON_BN_CLICKED(IDC_BUTTON1, &CPage_Base::OnBnClickedButton1)
//ON_BN_CLICKED(IDC_BUTTON1, &CPage_Base::OnClickedButton1)
ON_BN_CLICKED(IDC_BASE_BTN_HELP, &CPage_Base::OnBase_Btn_Help)
END_MESSAGE_MAP()


// CPage_Base 消息处理程序


void CPage_Base::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	SetCapValue(); //点击确定后才设置	
	m_pAdPage->SetCapValue(); //先设置高级界面，再设置基本界面,否则基本的“双面”设置后，高级的“分割”又设置为1了
	
	m_pUI->TW_SaveProfileToFile("上次使用模板");

	if(m_pUI->m_bSetup)  // EnableDSOnly
	{
		m_pUI->Save();
	}
	else  
	{
		m_pUI->Scan();
	}

	CPropertyPage::OnOK();
}

void CPage_Base::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	bool status = false;
	unsigned int unIndex = 1;
	lstString strFileNames;
	m_pUI->TW_GetAllProfiles(strFileNames);

	lstString::iterator iter = strFileNames.begin();
	for(;iter!=strFileNames.end(); iter++)
	{
		CString strTemp(iter->c_str());		

		if(strTemp.Find("上次使用模板") >=0 ) 
		{
			m_combo_profile.SetCurSel(unIndex);
			LoadProfile();
			status = true;	
		}
		unIndex ++;
	}

	if(!status) //没找见上次使用模板
	{
		m_pUI->ResetAllCaps();
	}

	m_pUI->Cancel();

	__super::OnCancel();
	//CPropertyPage::OnCancel();
}


//BOOL CPage_Base::OnApply() //点击“确定”与“应用”按钮都调用
//{
//	// TODO: 在此添加专用代码和/或调用基类
//	AfxMessageBox("OnApply");
//
//	ShellExecute(NULL,"open","C:\\Windows\\twain_32\\UDS General TWAIN DS\\UDS General TWAIN DS.CHM",NULL,NULL,SW_SHOWNORMAL);
//	
//	return CPropertyPage::OnApply();
//}


void CPage_Base::SetCapValue(void)
{
	MAP_CAP::iterator iter; 
	for(iter = m_basemap.begin(); iter != m_basemap.end(); iter++)
	{
		switch(iter->first)
		{
		case ICAP_CONTRAST:
		case ICAP_BRIGHTNESS:
		case ICAP_THRESHOLD:
			{
				m_pUI->SetCapValueFloat(iter->first,iter->second);  // 设置阈值为当前滚动条值
				break;
			}

		case ICAP_PIXELTYPE:
			{
				if(GetDlgItem(IDC_BASE_COMBO_COLORMODE)->IsWindowEnabled())//图像类型可用时才设置
				{
					m_pUI->SetCapValueInt(iter->first,(int)iter->second);
				}	

				break;
			}

		case CAP_FEEDERENABLED:
		case ICAP_XRESOLUTION:
		case ICAP_YRESOLUTION:
		case UDSCAP_MULTIFEEDDETECT:
			{
				m_pUI->SetCapValueInt(iter->first,(int)iter->second); // 设置重张检测
				break;
			}	

		case CAP_DUPLEXENABLED:
			{
				if(GetDlgItem(IDC_BASE_COMBO_SCANSIDE)->IsWindowEnabled())//单双面可用时才设置
				{
					m_pUI->SetCapValueInt(iter->first,(int)iter->second); 
					if(1 == ((int)iter->second)) //双面，单面该值为0
					{
						//::MessageBox(g_hwndDLG,"双面","m_nSourceHeight",MB_OK);
						m_pUI->SetCapValueInt(UDSCAP_DOCS_IN_ADF, 2);
					}	
					else
					{
						m_pUI->SetCapValueInt(UDSCAP_DOCS_IN_ADF, 1);
					}
				}		
				break;
			}

		default:
			{
				break;
			}	
		}
	}
}

void CPage_Base::UpdateControls(void)
{
	int nCapIndex;
	const IntVector* lstCapValues;
	const FloatVector* lstCapValuesFlt;
	int nCapValue;
	CString strText;

	// 扫描方式	
	m_combo_source.ResetContent();
	nCapIndex = m_pUI->GetCurrentCapIndex(CAP_FEEDERENABLED);
	lstCapValues = m_pUI->GetValidCap(CAP_FEEDERENABLED);
	if(lstCapValues && lstCapValues->size()>0)
	{
		m_combo_source.InsertString(0,lstCapValues->at(0)!=0?"自动送纸器":"平板");
		if(lstCapValues->size()>1)
		{
			m_combo_source.InsertString(1,lstCapValues->at(1)!=0?"自动送纸器":"平板");
		}	
	}
	m_combo_source.SetCurSel(nCapIndex);

  // 图像类型 
	m_combo_colormode.ResetContent();
	nCapIndex = m_pUI->GetCurrentCapIndex(ICAP_PIXELTYPE);
	lstCapValues = m_pUI->GetValidCap(ICAP_PIXELTYPE);
	for(unsigned int i=0; i<lstCapValues->size();i++)
	{
		switch(lstCapValues->at(i))
		{
		case TWPT_BW:
			m_combo_colormode.InsertString(i,"黑白");
			break;
		case TWPT_GRAY:
			m_combo_colormode.InsertString(i,"灰度");
			break;
		case TWPT_RGB:
			m_combo_colormode.InsertString(i,"彩色");
			break;
		default:
			continue;
		}
	}
	m_combo_colormode.SetCurSel(nCapIndex);

	
	// 分辨率
	//m_pUI->SetCapValueFloat(ICAP_XRESOLUTION,0);
	m_combo_resolution.ResetContent();
	nCapIndex = m_pUI->GetCurrentCapIndex(ICAP_XRESOLUTION);
	lstCapValuesFlt = m_pUI->GetValidCapFloat(ICAP_XRESOLUTION);

	for(unsigned int i=0; i<lstCapValuesFlt->size();i++)
	{
		CString strTemp;
		strTemp.Format("%d",(int)lstCapValuesFlt->at(i));
		m_combo_resolution.InsertString(i,strTemp);
	}
	m_combo_resolution.SetCurSel(nCapIndex);

	
	// 单面/双面扫
	// @see CTWAINDS_FreeIMage.cpp Line 675
	m_combo_scanside.ResetContent();  // 清空内容
	nCapIndex = m_pUI->GetCurrentCapIndex(CAP_DUPLEXENABLED); 
	lstCapValues = m_pUI->GetValidCap(CAP_DUPLEXENABLED);
	if(lstCapValues && lstCapValues->size()>0)
	{
		m_combo_scanside.InsertString(0,lstCapValues->at(0)!=0?"双面":"单面"); //插入顺序是 0-单面，1-双面
		if(lstCapValues->size()>1)
		{
			m_combo_scanside.InsertString(1,lstCapValues->at(1)!=0?"双面":"单面");
		}
		m_combo_scanside.SetCurSel(nCapIndex);  // 显示默认值
		m_basemap[CAP_DUPLEXENABLED] = (float)nCapIndex; 
	}

	// 对比度 
	nCapValue = (int)(m_pUI->GetCapValueFloat(ICAP_CONTRAST)); 
	m_slider_contrast.SetPos(nCapValue);
	strText.Format("%d",nCapValue);
	//m_edit_contrast.SetWindowText(strText);
	SetDlgItemText(IDC_BASE_EDIT_CONTRAST,strText);

	// 亮度 
	nCapValue = (int)(m_pUI->GetCapValueFloat(ICAP_BRIGHTNESS));
	m_slider_brightness.SetPos(nCapValue);
	strText.Format("%d",nCapValue);
	//m_edit_brightness.SetWindowText(strText);
	SetDlgItemText(IDC_BASE_EDIT_BRIGHTNESS,strText);

	// 阈值 
	nCapValue = (int)(m_pUI->GetCapValueFloat(ICAP_THRESHOLD));
	m_slider_threshold.SetPos(nCapValue);
	strText.Format("%d",nCapValue);
	//m_edit_threshold.SetWindowText(strText);
	SetDlgItemText(IDC_BASE_EDIT_THRESHOLD,strText);

	//重张检测：默认使用
	nCapValue = (int)(m_pUI->GetCapValueBool(UDSCAP_MULTIFEEDDETECT));
	m_check_multifeeddetect.SetCheck(nCapValue);

	muiltstream = m_pUI->GetCapValueBool(UDSCAP_MULTISTREAM);

	InitComboPixType(); //初始化图像类型下拉框值对应的亮度等值是否可用

	UpdateData(FALSE);
}


void CPage_Base::InitBasemap(void)
{
	m_basemap.erase(m_basemap.begin(),m_basemap.end());//清空

	int nCapIndex;
	nCapIndex = m_pUI->GetCurrentCapIndex(CAP_DUPLEXENABLED);
	scanside = nCapIndex; //初始化scanside，防止用户未点击下拉框改变单双面直接扫描时，scanside默认为0，高级界面仍会设置裁切
	m_basemap[CAP_DUPLEXENABLED] = (float)nCapIndex; //初始化时只为map插入“单双面”的值，特例
}


BOOL CPage_Base::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  在此添加额外的初始化	
	InitBasemap();

	InitSliderCtrl();
	UpdateControls();

	InitComboProfile();

	m_pAdPage->InitAdvancedmap(); //初始化高级界面的Map

	m_btn_chooseimage.ShowWindow(FALSE); //选择图片按钮暂时不启用
	GetDlgItem(IDC_BASE_BTN_SAVEPROFILE)->ShowWindow(FALSE); //保存当前模板暂时不启用

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CPage_Base::BaseStatus(void)
{
	if(muiltstream)
	{
		//AfxMessageBox("不图像类型可用");
		GetDlgItem(IDC_BASE_COMBO_COLORMODE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BASE_COMBO_SCANSIDE)->EnableWindow(FALSE);
	}
	else
	{
		//AfxMessageBox("多流不可用，图像类型可用");
		GetDlgItem(IDC_BASE_COMBO_COLORMODE)->EnableWindow(TRUE);
		GetDlgItem(IDC_BASE_COMBO_SCANSIDE)->EnableWindow(TRUE);
	}
	UpdateData(FALSE);
}

void CPage_Base::InitSliderCtrl()
{
	float fMin,fMax,fStep;
	m_pUI->GetCapRangeFloat(ICAP_CONTRAST, fMin, fMax, fStep);
	m_slider_contrast.SetRange((int)fMin, (int)fMax);
	m_slider_contrast.SetTicFreq((int)fStep);  // 设置滑动条刻度的频度为1个单位，很重要，若不加这句滑块初始位置不变

	m_pUI->GetCapRangeFloat(ICAP_BRIGHTNESS, fMin, fMax, fStep);
	m_slider_brightness.SetRange((int)fMin, (int)fMax);
	m_slider_brightness.SetTicFreq((int)fStep);

	m_pUI->GetCapRangeFloat(ICAP_THRESHOLD, fMin, fMax, fStep);
	m_slider_threshold.SetRange((int)fMin, (int)fMax);
	m_slider_threshold.SetTicFreq((int)fStep);

	UpdateData(FALSE);  // 更新控件
}


////////////////////////////////////////////////////////////////////////////////
// Slider控件内容变动
void CPage_Base::OnNMCustomdrawBase_Slider_Contrast(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);  // 接收数据
	CString str;
	int sldValue = m_slider_contrast.GetPos();  // 获取滑块当前位置
	//m_basemap.insert(map<int, float> :: value_type(ICAP_CONTRAST, (float)sldValue)); //以这种形式插入，键值一旦存在，就会插入失败
	//m_pUI->SetCapValueFloat(ICAP_CONTRAST,(float)sldValue);  // 设置对比度为当前滚动条值
	m_basemap[ICAP_CONTRAST] = float(sldValue);

	str.Format("%d", sldValue);
	//m_edit_contrast.SetWindowText(str);  // 在编辑框同步显示滚动条值
	SetDlgItemText(IDC_BASE_EDIT_CONTRAST, str);
	//UpdateControls();

	contrast = sldValue;
	m_pAdPage->UpdateControls();

	UpdateData(FALSE);  // 更新控件

	/*// 设置应用按钮为可用状态
	SetModified(TRUE);*/
	*pResult = 0;
}


void CPage_Base::OnNMCustomdrawBase_Slider_Brightness(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);  // 接收数据
	CString str;
	int sldValue = m_slider_brightness.GetPos();  // 获取滑块当前位置
	//m_basemap.insert(map<int, float> :: value_type(ICAP_BRIGHTNESS, float(sldValue)));
	m_basemap[ICAP_BRIGHTNESS] = float(sldValue);
	//m_pUI->SetCapValueFloat(ICAP_BRIGHTNESS,(float)sldValue);  // 设置亮度为当前滚动条值

	str.Format("%d", sldValue);
	//m_edit_brightness.SetWindowText(str);  // 在编辑框同步显示滚动条值
	SetDlgItemText(IDC_BASE_EDIT_BRIGHTNESS,str);
	//UpdateControls();

	brightness = sldValue;
	m_pAdPage->UpdateControls();

	UpdateData(FALSE);  // 更新控件

	/*// 设置应用按钮为可用状态
	SetModified(TRUE);*/
	*pResult = 0;
}


void CPage_Base::OnNMCustomdrawBase_Slider_Threshold(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码	
	UpdateData(TRUE); //接收数据
	CString str;
	int sldValue = m_slider_threshold.GetPos(); //获取滑块的当前位置
	
	/*
	if(0 == sldValue)
	{
		//m_basemap.insert(map<int, float> :: value_type(ICAP_THRESHOLD, 128.0f));
		//m_pUI->SetCapValueFloat(ICAP_THRESHOLD,128.0);  //虚拟默认128.G6400默认230
		m_basemap[ICAP_THRESHOLD] = 128.0f;
	}
	else
	{
		//m_basemap.insert(map<int, float> :: value_type(ICAP_THRESHOLD, (float)sldValue));
		//m_pUI->SetCapValueFloat(ICAP_THRESHOLD,(float)sldValue);  // 
		m_basemap[ICAP_THRESHOLD] = (float)sldValue;
	}*/

	m_basemap[ICAP_THRESHOLD] = (float)sldValue;

	str.Format("%d", sldValue);
	//m_edit_threshold.SetWindowText(str); //在编辑框同步显示滚动条值
	SetDlgItemText(IDC_BASE_EDIT_THRESHOLD, str);
	//UpdateControls();
	UpdateData(FALSE); //更新控件。

	/*// 设置应用按钮为可用状态
	SetModified(TRUE);*/

	*pResult = 0;
}


////////////////////////////////////////////////////////////////////////////////
// Edit控件内容变动
void CPage_Base::OnEnChangeBase_Edit_Contrast()
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
	//m_basemap.insert(map<int, float> :: value_type(ICAP_CONTRAST, (float)nval));
	//m_pUI->SetCapValueFloat(ICAP_CONTRAST,(float)nval);  // 设置对比度为当前滚动条值
	m_basemap[ICAP_CONTRAST] = (float)nval;

	m_edit_contrast.SetSel(str.GetLength(), str.GetLength(),TRUE);  // 设置编辑框控件范围
	//UpdateControls();

	contrast = nval;
	m_pAdPage->UpdateControls(); //更新一次高级界面，同步对比度参数

	UpdateData(FALSE);  // 更新控件

	/*// 设置应用按钮为可用状态
	SetModified(TRUE);*/
}


void CPage_Base::OnEnChangeBase_Edit_Brightness()
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
	//m_basemap.insert(map<int, float> :: value_type(ICAP_BRIGHTNESS, (float)nval));
	//m_pUI->SetCapValueFloat(ICAP_BRIGHTNESS,(float)nval);  // 设置对比度为当前滚动条值
	m_basemap[ICAP_BRIGHTNESS] = (float)nval;
	m_edit_brightness.SetSel(str.GetLength(), str.GetLength(),TRUE);  // 设置编辑框控件范围

	//UpdateControls();
	brightness = nval;
	m_pAdPage->UpdateControls(); //更新一次高级界面，同步对比度参数

	UpdateData(FALSE);  // 更新控件

	/*// 设置应用按钮为可用状态
	SetModified(TRUE);*/
}


void CPage_Base::OnEnChangeBase_Edit_Threshold()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CPropertyPage::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	UpdateData(TRUE); //接收数据
	CString str;
	m_edit_threshold.GetWindowText(str);
	int nval = _ttoi(str);
	m_slider_threshold.SetPos(nval); //_ttoi把CString类型转换为int
	//m_basemap.insert(map<int, int> :: value_type(ICAP_THRESHOLD, nval));
	if(0 == nval)
	{
		//m_basemap.insert(map<int, float> :: value_type(ICAP_THRESHOLD, 128.0f));
		//m_pUI->SetCapValueFloat(ICAP_THRESHOLD,128.0);  // 设置阈值为当前滚动条值
		m_basemap[ICAP_THRESHOLD] = 128.0f;
	}
	else
	{
		//m_basemap.insert(map<int, float> :: value_type(ICAP_THRESHOLD, (float)nval));
		//m_pUI->SetCapValueFloat(ICAP_THRESHOLD,(float)nval);  // 设置阈值为当前滚动条值
		m_basemap[ICAP_THRESHOLD] = (float)nval;
	}

	m_edit_threshold.SetSel(str.GetLength(), str.GetLength(), TRUE); //设置编辑框控件范围
	//UpdateControls();
	UpdateData(FALSE); //更新控件

	/*// 设置应用按钮为可用状态
	SetModified(TRUE);*/
	// TODO:  在此添加控件通知处理程序代码
}


////////////////////////////////////////////////////////////////////////////////
// Combobox控件内容变动
void CPage_Base::OnCbnSelchangeBase_Combo_Source()
{
	// TODO: 在此添加控件通知处理程序代码
	int nIndex = m_combo_source.GetCurSel();  // 当前combo序号
	/*CString str;
	str.Format("%d",nIndex);
	AfxMessageBox(str);//*/
	CString strCBText; 
	m_combo_source.GetLBText( nIndex, strCBText);  // 获取当前选项内容
	int nval;
	if(strCBText.Find("平板") >= 0)
	{
		nval = 0;
	} 
	else
	{
		nval = 1; 
	}
	//m_basemap.insert(map<int, float> :: value_type(CAP_FEEDERENABLED, (float)nval));
	//m_pUI->SetCapValueInt(CAP_FEEDERENABLED,nval);  // 设置对应参数
	m_basemap[CAP_FEEDERENABLED] = (float)nval;
	//UpdateControls();
	m_combo_source.SetCurSel(nIndex);

	/*// 设置应用按钮为可用状态
	SetModified(TRUE);*/
	return;
}


void CPage_Base::InitComboPixType(void)
{
	int nIndex = m_combo_colormode.GetCurSel();

	if(0 == nIndex) //黑白
	{
		GetDlgItem(IDC_BASE_SLIDER_CONTRAST)->EnableWindow(FALSE);
		GetDlgItem(IDC_BASE_EDIT_CONTRAST)->EnableWindow(FALSE);
		GetDlgItem(IDC_BASE_SLIDER_BRIGHTNESS)->EnableWindow(FALSE);
		GetDlgItem(IDC_BASE_EDIT_BRIGHTNESS)->EnableWindow(FALSE);	

		GetDlgItem(IDC_BASE_SLIDER_THRESHOLD)->EnableWindow(TRUE);	
		GetDlgItem(IDC_BASE_EDIT_THRESHOLD)->EnableWindow(TRUE);		
	} 
	else
	{
		GetDlgItem(IDC_BASE_SLIDER_CONTRAST)->EnableWindow(TRUE);
		GetDlgItem(IDC_BASE_EDIT_CONTRAST)->EnableWindow(TRUE);
		GetDlgItem(IDC_BASE_SLIDER_BRIGHTNESS)->EnableWindow(TRUE);
		GetDlgItem(IDC_BASE_EDIT_BRIGHTNESS)->EnableWindow(TRUE);

		GetDlgItem(IDC_BASE_SLIDER_THRESHOLD)->EnableWindow(FALSE);	
		GetDlgItem(IDC_BASE_EDIT_THRESHOLD)->EnableWindow(FALSE);
	}

	BaseStatus(); //获取base界面图像模式,传给高级界面。
}

void CPage_Base::OnCbnSelchangeBase_Combo_Colormode()
{
	// TODO: 在此添加控件通知处理程序代码
	int nIndex = m_combo_colormode.GetCurSel();

	// 直接根据当前序号nIndex设置图像类型
	//m_basemap.insert(map<int, float> :: value_type(ICAP_PIXELTYPE, (float)nIndex)); 
	//m_pUI->SetCapValueInt(ICAP_PIXELTYPE,nIndex); 
	m_basemap[ICAP_PIXELTYPE] = (float)nIndex;

	m_combo_colormode.SetCurSel(nIndex);
	//UpdateControls();
	InitComboPixType(); //zhu 判断亮度等是否可用
	//m_pAdPage->SetMultistream(); 

	/*// 设置应用按钮为可用状态
	SetModified(TRUE);*/
	//return;
}


void CPage_Base::OnCbnSelchangeBase_Combo_Resolution()
{
	// TODO: 在此添加控件通知处理程序代码
	int nIndex = m_combo_resolution.GetCurSel();
	CString strCBText; 
	m_combo_resolution.GetLBText(nIndex, strCBText);
	int nval = _ttoi(strCBText);  // CString 转 int
	//CString str;
	//str.Format("%d",nval);
	//AfxMessageBox(str);
	//m_basemap.insert(map<int, float> :: value_type(ICAP_XRESOLUTION, (float)nval));
	//m_basemap.insert(map<int, float> :: value_type(ICAP_YRESOLUTION, (float)nval)); 
	//m_pUI->SetCapValueInt(ICAP_XRESOLUTION,nval); 
	//m_pUI->SetCapValueInt(ICAP_YRESOLUTION,nval);
	m_basemap[ICAP_XRESOLUTION] = (float)nval;
	m_basemap[ICAP_YRESOLUTION] = (float)nval;
	//UpdateControls();
	m_combo_resolution.SetCurSel(nIndex);

	/*// 设置应用按钮为可用状态
	SetModified(TRUE);*/
	return;
}

////////////////////////////////////////////////////////////////////////////////
// 双面/单面扫 
void CPage_Base::OnCbnSelchangeBase_Combo_Scanside()
{
	// TODO: 在此添加控件通知处理程序代码
	int nIndex = m_combo_scanside.GetCurSel();
	CString strCBText; 
	m_combo_scanside.GetLBText( nIndex, strCBText);
	int nval;
	if (strCBText.Find("单面") >= 0)
	{
		nval = 0;
		scanside = 0;
	} 
	else
	{
		nval = 1;
		scanside = 1;
	}

	//m_basemap.insert(map<int, float> :: value_type(CAP_DUPLEXENABLED, (float)nval));
	//m_pUI->SetCapValueInt(CAP_DUPLEXENABLED,nval?1:0);
	m_basemap[CAP_DUPLEXENABLED] = (float)nval;
	//UpdateControls();
	m_combo_scanside.SetCurSel(nIndex);
}


void CPage_Base::OnClicked_Check_Multifeeddetect()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_check_multifeeddetect.GetCheck()) //点中
	{
		//m_basemap.insert(map<int, float> :: value_type(UDSCAP_MULTIFEEDDETECT, 1.0f));
		//m_pUI->SetCapValueInt(UDSCAP_MULTIFEEDDETECT,true);	
		m_basemap[UDSCAP_MULTIFEEDDETECT] = TRUE;
	} 
	else
	{
		//m_basemap.insert(map<int, float> :: value_type(UDSCAP_MULTIFEEDDETECT, 0.0f));
		//m_pUI->SetCapValueInt(UDSCAP_MULTIFEEDDETECT,false);
		m_basemap[UDSCAP_MULTIFEEDDETECT] = FALSE;
	}
}


////////////////////////////////////////////////////////////////////////////////
void CPage_Base::OnBase_Btn_Chooseimage()
{
	// TODO: 在此添加控件通知处理程序代码

	//// 单张图片
	//CHAR szFilePath[MAX_PATH] = {0};
	//if (true == MyBrowseForSignalImage(szFilePath))
	//{
	//	m_pUI->m_pDS->SetScannerImagePath_Signal(szFilePath);
	//	//m_pUI->SetCapValueInt(UDSCAP_DOCS_IN_ADF,2);  
	//}
	

	// 多张图片
	vector<string> vector_string_imagepath;
	vector_string_imagepath = MyBrowseForMultiImages();

	unsigned int nCount = vector_string_imagepath.size();

	m_pUI->SetCapValueInt(UDSCAP_DOCS_IN_ADF,nCount);	
	m_pUI->m_pDS->SetScannerImagePath_Multi(vector_string_imagepath); // 传递多张图片路径

}

////////////////////////////////////////////////////////////////////////////////
//bool CPage_Base::MyBrowseForSignalImage(PTCHAR strFilePath)
//{
//	OPENFILENAME ofn = { 0 };
//	TCHAR szFilename[MAX_PATH] = { 0 };  // 用于接收文件名  
//
//	ofn.lStructSize = sizeof(OPENFILENAME); // 结构体大小  
//	ofn.hwndOwner = NULL;  // 拥有着窗口句柄，为NULL表示对话框是非模态的，实际应用中一般都要有这个句柄  
//	ofn.lpstrFilter = TEXT("所有文件\0*.*\0\0");  // 设置过滤  
//	ofn.nFilterIndex = 1;  // 过滤器索引  
//	ofn.lpstrFile = szFilename;  // 接收返回的文件名，注意第一个字符需要为NULL  
//	ofn.nMaxFile = sizeof(szFilename);  // 缓冲区长度  
//	ofn.lpstrInitialDir = NULL;  // 初始目录为默认  
//	ofn.lpstrTitle = TEXT("请选择一个图片文件");  // 使用系统默认标题留空即可  
//	ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;  // 文件、目录必须存在，
//	if (GetOpenFileName(&ofn))  
//	{  
//		SSTRCPY(strFilePath,sizeof(szFilename),szFilename);
//		//::MessageBox(NULL, strFilePath, "选择的图片文件", 0);  
//		return true;
//	}  
//	else
//	{  
//		::MessageBox(NULL, TEXT("请选择一个图片文件"), NULL, MB_ICONERROR);  
//		return false;
//	}  
//
//}



vector<string> CPage_Base::MyBrowseForMultiImages()
{
	vector<string> vectorFileName;
	OPENFILENAME ofn;
	TCHAR szOpenFileNames[80*MAX_PATH];
	TCHAR szPath[MAX_PATH];
  //TCHAR szFileName[1*MAX_PATH];
	TCHAR* p;
	int nLen = 0;

	//vector<string> vectorOpenFileNames;
	//vector<string> vectorFileName;

	ZeroMemory( &ofn, sizeof(ofn) );
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_EXPLORER | 
		OFN_ALLOWMULTISELECT ;  // 文件、目录必须存在，使用Explorer风格用户界面，列表框允许多选 
	ofn.lStructSize = sizeof(ofn);
	ofn.lpstrFile = szOpenFileNames;
	ofn.nMaxFile = sizeof(szOpenFileNames);
	ofn.lpstrFile[0] = '\0';
	ofn.lpstrFilter = TEXT("所有文件\0*.*\0\0");
	ofn.lpstrTitle = TEXT("请选择图片文件");  // 使用系统默认标题留空即可  
	if( GetOpenFileName( &ofn ) )
	{  
		/* 
		* 把第一个文件名前的复制到szPath,即:
		* 如果只选了一个文件,就复制到最后一个'/'
		* 如果选了多个文件,就复制到第一个NULL字符
		*/
		lstrcpyn(szPath, szOpenFileNames, ofn.nFileOffset );
		/*
		* 当只选了一个文件时,下面这个NULL字符是必需的.
		* 这里不区别对待选了一个和多个文件的情况
		*/
		szPath[ ofn.nFileOffset ] = '\0';
		nLen = lstrlen(szPath);

		/* 如果选了多个文件,则必须加上'\\' */
		if( szPath[nLen-1] != '\\' )  
		{
			lstrcat(szPath, TEXT("\\"));
		}

		p = szOpenFileNames + ofn.nFileOffset; // 把指针移到第一个文件

		/*ZeroMemory(szFileName, sizeof(szFileName));
		while( *p )
		{   
			lstrcat(szFileName, szPath);  ///< 给文件名加上路径  
			lstrcat(szFileName, p);    ///< 加上文件名  
			lstrcat(szFileName, TEXT("\n")); ///< 换行   
			p += lstrlen(p) +1;     ///< 移至下一个文件
		}
		::MessageBox(NULL, szFileName, TEXT("MultiSelect"), MB_OK);*/

		while (*p)
		{
			TCHAR szTemp[MAX_PATH] = {0};
			lstrcat(szTemp, szPath);  // 给文件名加上路径  
			lstrcat(szTemp, p);    // 加上文件名  
			vectorFileName.push_back(szTemp);
			p += lstrlen(p) +1;    // 移至下一个文件
		}

		/*vector<string>::iterator it = vectorFileName.begin();

		for (it; it!=vectorFileName.end(); it++)
		{
		::MessageBox(NULL, (*it).c_str(),TEXT("MultiSelect"), MB_OK);
		}*/
		return vectorFileName;
		//return true;
	}
	return vectorFileName;
	//return false;
}


////////////////////////////////////////////////////////////////////////////////
// 新建模板,即另存为模板 
void CPage_Base::OnBase_Btn_SaveAsprofile()
{
	// TODO: 在此添加控件通知处理程序代码
	//CDlg_Profile *pDlg = new CDlg_Profile;
	//dlg.DoModal();
	//CWnd *pMainWnd = CWnd::FromHandle(g_hwndDLG);
	//m_pDlg = new CDlg_Profile;
	//int nRes = m_pDlg->Create(CDlg_Profile::IDD);
	//m_pDlg->ShowWindow(SW_SHOW);

	CDlg_Profile dlg;  // 新建模版对话框
	dlg.DoModal();

	CString strExistName;

	if (TRUE == dlg.m_bOk)  // 确定新建模版
	{
		strExistName = dlg.GetProfileName();  // 获取对话框中保存的模版名
	}
	else
	{
		return;
	}

	// 判断新建模板名是否已存在
	CString strCombo;  
	int nLength;   
	for (int i=0;i < m_combo_profile.GetCount();i++)
	{        
		nLength = m_combo_profile.GetLBTextLen( i );  // 获取Combobox内容长度
		m_combo_profile.GetLBText( i, strCombo.GetBuffer(nLength));
		if (strCombo == strExistName)
		{
			if (AfxMessageBox("模版已存在，您想要重新创建吗？",MB_OKCANCEL) == IDCANCEL)  
			{
				return;  // 取消新建同名模版
			}
		}
		strCombo.ReleaseBuffer();      
	}

	//保存修改的CapValue
	SetCapValue();

	CString strName = strExistName;
	string strProfile = strName.GetBuffer();  // CString->string
	strName.ReleaseBuffer();

	if(m_pUI->TW_SaveProfileToFile(strProfile))
	{		
		m_combo_profile.AddString(strName);
		m_combo_profile.SetCurSel(m_combo_profile.GetCount()-1);
	}
	
	UpdateData(FALSE);
}


////////////////////////////////////////////////////////////////////////////////
// 删除模板 
void CPage_Base::OnBase_Btn_Deleteprofile()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);  // 接收数据
	int nIndex = m_combo_profile.GetCurSel();
	CString strProfile; 
	m_combo_profile.GetLBText( nIndex, strProfile);

	if(m_pUI->TW_DeleteProfile(strProfile.GetBuffer()))
	{
		m_combo_profile.DeleteString(nIndex);
	}
	int nCount = m_combo_profile.GetCount();
	m_combo_profile.SetCurSel(0);  // 切换到默认模板
	LoadProfile();
}

void CPage_Base::InitComboProfile()
{
	m_combo_profile.ResetContent();
	m_combo_profile.InsertString(0,"默认模板");
  m_combo_profile.SetCurSel(0); //设置为默认模板
	
	NewBaseProfile();
	SetLastProfile();
}

//遍历模板，设置模板中存在“上次使用模板”的情况
void CPage_Base::SetLastProfile(void)
{
	lstString strFileNames;
	m_pUI->TW_GetAllProfiles(strFileNames);

	unsigned int unIndex = 1;
	lstString::iterator iter = strFileNames.begin();
	for(;iter!=strFileNames.end(); iter++)
	{
		CString strTemp(iter->c_str());		
		m_combo_profile.InsertString(unIndex, strTemp);

		if(strTemp.Find("上次使用") >=0 ) {
			m_combo_profile.SetCurSel(unIndex);
			LoadProfile();
		}
		unIndex ++;
	}
}
////////////////////////////////////////////////////////////////////////////////
//新建模板 
void CPage_Base::NewBaseProfile()
{
	string strProfileName;
	
	//新建“彩色,单面,200dpi”模板
	strProfileName = "UDS--彩色,单面,200dpi";
	if(false == CreateNewProfile(strProfileName, 2, 0)) {
		return;
	}

	//新建“彩色,双面,200dpi”模板
	strProfileName = "UDS--彩色,双面,200dpi";
	if(false == CreateNewProfile(strProfileName, 2, 1)) {
		return;
	}

	//新建“黑白,单面,200dpi”模板
	strProfileName = "UDS--黑白,单面,200dpi";
	if(false == CreateNewProfile(strProfileName, 0, 0)) {
		return;
	}

	//新建“黑白,双面,200dpi”模板
	strProfileName = "UDS--黑白,双面,200dpi";
	if(false == CreateNewProfile(strProfileName, 0, 1)) {
		return;
	}

	//新建“灰度,单面,200dpi”模板
	strProfileName = "UDS--灰度,单面,200dpi";
	if(false == CreateNewProfile(strProfileName, 1, 0)) {
		return;
	}

	//新建“灰度,双面,200dpi”模板
	strProfileName = "UDS--灰度,双面,200dpi";
	if(false == CreateNewProfile(strProfileName, 1, 1)) {
		return;
	}

	//新建“彩色,单面,300dpi”模板
	strProfileName = "UDS--彩色,单面,300dpi";
	if(false == CreateNewProfile(strProfileName, 2, 0, 300)) {
		return;
	}

	//新建“彩色,双面,300dpi”模板
	strProfileName = "UDS--彩色,双面,300dpi";
	if(false == CreateNewProfile(strProfileName, 2, 1, 300)) {
		return;
	}
}


////////////////////////////////////////////////////////////////////////////////
void CPage_Base::LoadProfile()
{
	UpdateData(TRUE);  // 接收数据
	int nIndex = m_combo_profile.GetCurSel();
	if(0 == nIndex)  // 默认模板，重置驱动参数
	{
		bool resetstatus = m_pUI->ResetAllCaps();
		if(resetstatus)
		{
			//::MessageBox(g_hwndDLG,"Reset成功","m_nSourceHeight",MB_OK);
		}
		else
		{
			//::MessageBox(g_hwndDLG,"Reset失败","m_nSourceHeight",MB_OK);
		}
	}
	else  // 其它模板
	{	
		CString strProfile; 
		m_combo_profile.GetLBText( nIndex, strProfile);
		m_pUI->TW_LoadProfileFromFile(strProfile.GetBuffer()); //会m_pDS->SetGustomDSData
	}
	
	UpdateControls();
	m_pAdPage->UpdateControls();//高级设置界面参数也更新(有分辨率共同存在)

}

////////////////////////////////////////////////////////////////////////////////
// 扫描参数同步为模板值 
void CPage_Base::OnCbnSelchangeBase_Combo_Profile()
{
	// TODO: 在此添加控件通知处理程序代码
	LoadProfile();	
	SetDelete();
}


void CPage_Base::SetDelete(void)
{
	int nIndex = m_combo_profile.GetCurSel();
	CString strCBText; 
	m_combo_profile.GetLBText( nIndex, strCBText);
	if (strCBText.Find("UDS") >= 0)
	{
		GetDlgItem(IDC_BASE_BTN_DELETEPROFILE)->EnableWindow(FALSE);
	} 
	else
	{
		GetDlgItem(IDC_BASE_BTN_DELETEPROFILE)->EnableWindow(TRUE);
	}
}

//void CPage_Base::OnBase_Btn_Saveprofile()
//{
//	// TODO: 在此添加控件通知处理程序代码
//	SetCapValue();
//	m_pAdPage->SetCapValue();
//
//	int nIndex = m_combo_profile.GetCurSel();
//	CString strCBText; 
//	m_combo_profile.GetLBText( nIndex, strCBText);
//
//	if (strCBText.Find("默认模板") >= 0 || strCBText.Find("UDS") >= 0)//为默认模板或给定模板时，保存为上次使用模板
//	{
//		m_pUI->TW_SaveProfileToFile("上次使用模板"); 	
//		//SetLastProfile(); //SetLastProfile会再遍历一次，插入所有
//		
//		lstString strFileNames;
//		m_pUI->TW_GetAllProfiles(strFileNames);
//
//		unsigned int unIndex = 1;
//		lstString::iterator iter = strFileNames.begin();
//		for(;iter!=strFileNames.end(); iter++)
//		{
//			CString strTemp(iter->c_str());		
//
//			if(strTemp.Find("上次使用") >=0 ) 
//			{
//				m_combo_profile.InsertString(unIndex, strTemp); //与SetLastProfile不同，只插入上次使用模板
//				m_combo_profile.SetCurSel(unIndex);
//				LoadProfile();
//				
//			/*	int nIndexTemp = m_combo_profile.GetCurSel();
//				CString strCBTextTemp; 
//				m_combo_profile.GetLBText( nIndexTemp, strCBTextTemp);
//
//				m_combo_profile.InsertString(unIndex, strTemp); //与SetLastProfile不同，只插入上次使用模板*/
//			}
//			unIndex ++;
//		}
//	}
//	else //其他用户新建的模板
//	{
//		string strProfile;
//		strProfile = strCBText;
//		m_pUI->TW_SaveProfileToFile(strProfile); 
//		LoadProfile();
//	}
//}

bool CPage_Base::CreateNewProfile(std::string profilename, int pixeltype, 
	int duplexenabled, int resolution /*= 200*/)
{
	if(false == m_pUI->SetCapValueInt(ICAP_PIXELTYPE,pixeltype)) { 
		return false;
	}
	
	if(false == m_pUI->SetCapValueInt(CAP_DUPLEXENABLED,duplexenabled)) {
		return false;
	}

	if(false == m_pUI->SetCapValueInt(ICAP_XRESOLUTION,resolution)) { 
		return false;
	}

	if(false == m_pUI->SetCapValueInt(ICAP_YRESOLUTION,resolution)) {
		return false;
	}

	if(false == m_pUI->TW_SaveProfileToFile(profilename)) {
		return false;
	}

	return true;
}


void CPage_Base::OnBase_Btn_Help()
{
	// TODO: 在此添加控件通知处理程序代码
	ShellExecute(NULL,"open","C:\\Windows\\twain_32\\UDS General TWAIN DS\\UDS General TWAIN DS使用手册.chm",NULL,NULL,SW_SHOWNORMAL);
}

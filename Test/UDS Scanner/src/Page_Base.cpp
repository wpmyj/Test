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
extern HINSTANCE  g_hinstance;
extern int g_nDeviceNumber; 

IMPLEMENT_DYNAMIC(CPage_Base, CPropertyPage)

CPage_Base::CPage_Base(MFC_UI *pUI)
	: m_pUI(pUI), CPropertyPage(CPage_Base::IDD)
{
	m_pTabAutoColor = new CBase_AutoColor(pUI);
	m_pTabColor = new CBase_Tab_Color(pUI);
	m_pTabGray = new CBase_Tab_Gray(pUI);
	m_pTabBW = new CBase_Tab_BW(pUI);
}


CPage_Base::~CPage_Base()
{
	m_basemap.swap(map<int, float>());
	remove(m_bmpFilePath); //删除临时预览图片

	if (m_pTabAutoColor)
	{
		delete m_pTabAutoColor;
		m_pTabAutoColor = NULL;
	}
	if (m_pTabColor)
	{
		delete m_pTabColor;
		m_pTabColor = NULL;
	}
	if (m_pTabGray)
	{
		delete m_pTabGray;
		m_pTabGray = NULL;
	}
	if (m_pTabBW)
	{
		delete m_pTabBW;
		m_pTabBW = NULL;
	}

}

void CPage_Base::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BASE_COMBO_COLORMODE, m_combo_colormode);
	DDX_Radio(pDX, IDC_BASE_RADIO_DUPLEX_DAN, m_radiobtn_duplex);
	DDX_Control(pDX, IDC_CHECK_FRONTCOLOR, m_check_frontcolor);
	DDX_Control(pDX, IDC_CHECK_FRONTGRAY, m_check_frontgray);
	DDX_Control(pDX, IDC_CHECK_FRONTBW, m_check_frontbw);
	DDX_Control(pDX, IDC_CHECK_BACKBW, m_check_backbw);
	DDX_Control(pDX, IDC_CHECK_BACKGRAY, m_check_backgray);
	DDX_Control(pDX, IDC_CHECK_BACKCOLOR, m_check_backcolor);
	DDX_Control(pDX, IDC_BASE_BUTTON_FRONTCOLOR, m_btn_frontcolor);
	DDX_Control(pDX, IDC_BASE_BUTTON_FRONTGRAY, m_btn_frontgray);
	DDX_Control(pDX, IDC_BASE_BUTTON_FRONTBW, m_btn_frontbw);
	DDX_Control(pDX, IDC_BASE_BUTTON_BACKCOLOR, m_btn_backcolor);
	DDX_Control(pDX, IDC_BASE_BUTTON_BACKGRAY, m_btn_backgray);
	DDX_Control(pDX, IDC_BASE_BUTTON_BACKBW, m_btn_backbw);
	DDX_Radio(pDX, IDC_BASE_RADIO_SCANMODE_AUTO, m_radiobtn_scanmode);
	DDX_Control(pDX, IDC_BASE_TAB, m_base_tab);
	DDX_Control(pDX, IDC_BASE_COMBO_ROTATE, m_combo_rotate);
	DDX_Control(pDX, IDC_BASE_COMBO_SPLITIMG, m_combo_splitimage);
	DDX_Control(pDX, IDC_BASE_SLIDER_GAMMA, m_slider_gamma);
	DDX_Control(pDX, IDC_BASE_EDIT_GAMMA, m_edit_gamma);
	DDX_Control(pDX, IDC_BASE_SLIDER_REMOVEBLANK, m_slider_removeblank);
	DDX_Control(pDX, IDC_BASE_EDIT_REMOVEBLANK, m_edit_removeblank);
	DDX_Control(pDX, IDC_BASE_CHECK_REMOVEBLANK, m_check_removeblank);
	DDX_Control(pDX, IDC_BASE_SCROLLBAR_NOISENUM, m_scroll_noisenum);
	DDX_Control(pDX, IDC_BASE_SCROLLBAR_NOISERANGE, m_scroll_noiserange);
	DDX_Control(pDX, IDC_BASE_EDIT_NOISENUM, m_edit_noisenum);
	DDX_Control(pDX, IDC_BASE_EDIT_NOISERANGE, m_edit_noiserange);
}


BEGIN_MESSAGE_MAP(CPage_Base, CPropertyPage)
	ON_CBN_SELCHANGE(IDC_BASE_COMBO_COLORMODE, &CPage_Base::OnCbnSelchangeBase_Combo_Colormode)
	ON_BN_CLICKED(IDC_BASE_RADIO_DUPLEX_DAN, &CPage_Base::OnBase_RadioBtn_Duplex)
	ON_BN_CLICKED(IDC_BASE_RADIO_DUPLEX_SHUANG, &CPage_Base::OnBase_RadioBtn_Duplex)
	ON_BN_CLICKED(IDC_BASE_RADIO_DUPLEX_MUILTSTREAM, &CPage_Base::OnBase_RadioBtn_Duplex)
	ON_BN_CLICKED(IDC_CHECK_FRONTCOLOR, &CPage_Base::OnBase_Btn_Check_FrontColor)
	ON_BN_CLICKED(IDC_CHECK_FRONTGRAY, &CPage_Base::OnBase_Btn_Check_FrontGray)
	ON_BN_CLICKED(IDC_CHECK_FRONTBW, &CPage_Base::OnBase_Btn_Check_FrontBw)
	ON_BN_CLICKED(IDC_CHECK_BACKCOLOR, &CPage_Base::OnBase_Btn_Check_BackColor)
	ON_BN_CLICKED(IDC_CHECK_BACKGRAY, &CPage_Base::OnBase_Btn_Check_BackGray)
	ON_BN_CLICKED(IDC_CHECK_BACKBW, &CPage_Base::OnBase_Btn_Check_BackBw)
	ON_BN_CLICKED(IDC_BASE_RADIO_SCANMODE_AUTO, &CPage_Base::OnBase_RadioBtn_Scanmode_Auto)
	ON_BN_CLICKED(IDC_BASE_RADIO_SCANMODE_Flatbed, &CPage_Base::OnBase_RadioBtn_Scanmode_Flatbed)
//	ON_WM_PAINT()
	ON_NOTIFY(TCN_SELCHANGE, IDC_BASE_TAB, &CPage_Base::OnTcnSelchangeBase_Tab)
	ON_CBN_SELCHANGE(IDC_BASE_COMBO_ROTATE, &CPage_Base::OnCbnSelchangeBase_Combo_Rotate)
	ON_CBN_SELCHANGE(IDC_BASE_COMBO_SPLITIMG, &CPage_Base::OnCbnSelchangeBase_Combo_SpiltImage)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_BASE_SLIDER_GAMMA, &CPage_Base::OnNMCustomdrawBase_Slider_Gamma)
	ON_EN_CHANGE(IDC_BASE_EDIT_GAMMA, &CPage_Base::OnEnChangeBase_Edit_Gamma)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_BASE_SLIDER_REMOVEBLANK, &CPage_Base::OnNMCustomdrawBase_Slider_Removeblank)
	ON_EN_CHANGE(IDC_BASE_EDIT_REMOVEBLANK, &CPage_Base::OnEnChangeBase_Edit_Removeblank)
	ON_BN_CLICKED(IDC_BASE_CHECK_REMOVEBLANK, &CPage_Base::OnBase_Btn_Check_RemoveBlank)
	ON_BN_CLICKED(IDC_BASE_BUTTON_FRONTCOLOR, &CPage_Base::OnBase_Btn_FrontColor)
	ON_BN_CLICKED(IDC_BASE_BUTTON_BACKCOLOR, &CPage_Base::OnBase_Btn_Backcolor)
	ON_BN_CLICKED(IDC_BASE_BUTTON_FRONTGRAY, &CPage_Base::OnBase_Btn_Frontgray)
	ON_BN_CLICKED(IDC_BASE_BUTTON_BACKGRAY, &CPage_Base::OnBase_Btn_Backgray)
	ON_BN_CLICKED(IDC_BASE_BUTTON_FRONTBW, &CPage_Base::OnBase_Btn_Frontbw)
	ON_BN_CLICKED(IDC_BASE_BUTTON_BACKBW, &CPage_Base::OnBase_Btn_Backbw)
	ON_WM_VSCROLL()
	ON_EN_CHANGE(IDC_BASE_EDIT_NOISENUM, &CPage_Base::OnEnChangeBase_Edit_NoiseNum)
	ON_EN_CHANGE(IDC_BASE_EDIT_NOISERANGE, &CPage_Base::OnEnChangeBase_Edit_NoiseRange)
END_MESSAGE_MAP()


// CPage_Base 消息处理程序

void CPage_Base::SetCapValue(void)
{
	MAP_CAP::iterator iter; 
	for(iter = m_basemap.begin(); iter != m_basemap.end(); iter++)
	{
		switch(iter->first)
		{
		case UDSCAP_MULTISTREAM: //多流输出
			{
				m_pUI->SetCapValueInt(iter->first,(int)(iter->second));

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
				break;
			}	
	
		case UDSCAP_MULTISTREAM_VALUE: // 多流输出选项值
			{
				SetCapMulti();
				break;
			}

		default:
			{
				break;
			}	
		}
	}
}


void CPage_Base::SetCapMulti()
{
	BYTE temp = (BYTE)m_pUI->GetCapValueFloat(UDSCAP_MULTISTREAM_VALUE);
	unsigned int doc_count = 0;

	//若多流未选中多流 选项值设为0，,然后退出
	if( 0 == m_radiobtn_duplex) {
		m_pUI->SetCapValueFloat(UDSCAP_MULTISTREAM_VALUE, 0.0f); // 保存多流选项值
	}
	else
	{
		/************************************************************
		* 判断多流chebox各选项的状态;
		* 若选中则‘&’上对应bit为1的值;
		* 若未选中则'|'上对应bit为0的值.
		************************************************************/
		// 正面彩色
		if (m_check_frontcolor.GetCheck()) {  
			temp |= 0x01;
			doc_count++;
		} 
		else {
			temp &= (0xFF-0x01);
		}
		// 正面彩色
		if (m_check_frontgray.GetCheck()) {
			temp |= 0x02;
			doc_count++;
		} 
		else {
			temp &= (0xFF-0x02);
		}
		// 正面黑白
		if (m_check_frontbw.GetCheck()) {
			temp |= 0x04;
			doc_count++;
		} 
		else {
			temp &= (0xFF-0x04);
		}
		// 背面彩色
		if (m_check_backcolor.GetCheck()) {
			temp |= 0x10;
			doc_count++;
		} 
		else {
			temp &= (0xFF-0x10);
		}
		// 背面灰度
		if (m_check_backgray.GetCheck()) {
			temp |= 0x20;
			doc_count++;
		} 
		else {
			temp &= (0xFF-0x20);
		}
		// 背面黑白
		if (m_check_backbw.GetCheck()) {
			temp |= 0x40;
			doc_count++;
		} 
		else {
			temp &= (0xFF-0x40);
		}

		m_pUI->SetCapValueFloat(UDSCAP_MULTISTREAM_VALUE, (float)temp); // 保存多流选项值
		m_pUI->SetCapValueInt(UDSCAP_DOCS_IN_ADF,doc_count); // 设置纸张数
	}
}

void CPage_Base::UpdateControls(void)
{
	int nCapIndex;
	const IntVector* lstCapValues;
	const FloatVector* lstCapValuesFlt;
	int nCapValue;
	CString strText;
	float fCapValue;

	//去除噪声-噪声数目
	fCapValue = m_pUI->GetCapValueFloat(UDSCAP_NOISENUM);
	strText.Format("%d",(int)fCapValue);
	SetDlgItemText(IDC_BASE_EDIT_NOISENUM,strText);

	//去除噪声-噪声范围
	fCapValue = m_pUI->GetCapValueFloat(UDSCAP_NOISERANGE);
	strText.Format("%d",(int)fCapValue);
	SetDlgItemText(IDC_BASE_EDIT_NOISERANGE,strText);

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
	InitComboPixType();
	
	//2.0版本
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
		case TWSI_DEFINED:
			m_combo_splitimage.InsertString(i,"自定义"); //垂直分割图像
			break;
		default:
			continue;
		}
	}
	m_combo_splitimage.SetCurSel(nCapIndex);

	//Gamma校正 
	nCapValue = (int)(m_pUI->GetCapValueFloat(ICAP_GAMMA)); //GetCapValueFloat能否得到CTWAINContainerFix32类型	
	m_slider_gamma.SetPos(nCapValue);
	float valueTemp = ((float)nCapValue)/100;
	strText.Format("%.2f", valueTemp);
	SetDlgItemText(IDC_BASE_EDIT_GAMMA, strText);

	//去除空白页checkbox
	nCapValue = (int)(m_pUI->GetCapValueBool(UDSCAP_REMOVEBLANK));
	m_check_removeblank.SetCheck(nCapValue);
	//去除空白页 -1自动;-2不可用:改为滑动条 
	nCapValue = (int)(m_pUI->GetCapValueFloat(ICAP_AUTODISCARDBLANKPAGES)); 
	m_slider_removeblank.SetPos(nCapValue);
	strText.Format("%d",nCapValue);
	SetDlgItemText(IDC_BASE_EDIT_REMOVEBLANK, strText);
	SetBlank();

	//int型radio更新要放在最后，否则SetDlgItemText刷新时会又重新设置。
	// 扫描方式	
	nCapIndex = m_pUI->GetCurrentCapIndex(CAP_FEEDERENABLED);
	if(0 == nCapIndex) //平板
	{
		m_radiobtn_scanmode = 1;
	}
	else //1为自动进纸器
	{
		m_radiobtn_scanmode = 0;
	}
	SetFlat();

	//多流输出：默认不使用
	nCapValue = (int)(m_pUI->GetCapValueBool(UDSCAP_MULTISTREAM));
	if(nCapValue == 1) //多流选中
	{
		m_radiobtn_duplex = 2;
		// 获取多流选项值并更新控件状态
		BYTE value = (BYTE)m_pUI->GetCapValueFloat(UDSCAP_MULTISTREAM_VALUE);
		// 循环判断每bit的值，并更新对应控件的状态
		for (unsigned int i = 0; i < 7; i++)
		{
			switch(i)
			{
			case 0:
				{
					if ( 0x01 == (value & 0x01) ) {
						m_check_frontcolor.SetCheck(TRUE);
					}
					else {
						m_check_frontcolor.SetCheck(FALSE);
					}
				}
				break;
			case 1:
				{
					if ( 0x01 == (value & 0x01) ) {
						m_check_frontgray.SetCheck(TRUE);
					}
					else {
						m_check_frontgray.SetCheck(FALSE);
					}
				}
			case 2:
				{
					if ( 0x01 == (value & 0x01) ) {
						m_check_frontbw.SetCheck(TRUE);
					}
					else {
						m_check_frontbw.SetCheck(FALSE);
					}
				}
				break;
			case 4:
				{
					if ( 0x01 == (value & 0x01) ) {
						m_check_backcolor.SetCheck(TRUE);
					}
					else {
						m_check_backcolor.SetCheck(FALSE);
					}
				}
				break;
			case 5:
				{
					if ( 0x01 == (value & 0x01) ) {
						m_check_backgray.SetCheck(TRUE);
					}
					else {
						m_check_backgray.SetCheck(FALSE);
					}
				}
				break;
			case 6:
				{
					if ( 0x01 == (value & 0x01) ) {
						m_check_backbw.SetCheck(TRUE);
					}
					else {
						m_check_backbw.SetCheck(FALSE);
					}
				}
				break;
			default:
				break;
			}
			value = value >> 1; // 始终比较最低位
		} // for end
	}//if end
	else //单双面选中
	{
		// 单面/双面扫
		nCapIndex = m_pUI->GetCurrentCapIndex(CAP_DUPLEXENABLED);
		m_radiobtn_duplex = nCapIndex; //0为单面，1为双面
	}
	SetMultistream();
	SetDenoise();
}


void CPage_Base::InitBasemap(void)
{
	m_basemap.erase(m_basemap.begin(),m_basemap.end());//清空

	int nCapIndex = m_pUI->GetCurrentCapIndex(CAP_DUPLEXENABLED);
	m_basemap[CAP_DUPLEXENABLED] = (float)nCapIndex; //初始化时只为map插入“单双面”的值，特例

	int nCapValue = (int)(m_pUI->GetCapValueBool(UDSCAP_MULTISTREAM));
	m_basemap[UDSCAP_MULTISTREAM_VALUE] = (float)nCapValue;

	float value = m_pUI->GetCapValueFloat(UDSCAP_MULTISTREAM_VALUE);
	m_basemap[UDSCAP_MULTISTREAM_VALUE] = value; // 初始化时添加UDSCAP_MULTISTREAM_VALUE，保证SetCapValue()会更新该Cap的值
}


BOOL CPage_Base::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	// TODO:  在此添加额外的初始化	
	InitBasemap();
	InitSliderCtrl();
	UpdateControls();
	
	if(m_radiobtn_scanmode == 0)
	{
		GetDlgItem(IDC_BASE_RADIO_DUPLEX_SHUANG)->EnableWindow(TRUE); 
		GetDlgItem(IDC_BASE_RADIO_DUPLEX_MUILTSTREAM)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_BASE_RADIO_DUPLEX_SHUANG)->EnableWindow(FALSE); 
		GetDlgItem(IDC_BASE_RADIO_DUPLEX_MUILTSTREAM)->EnableWindow(FALSE);
	}
	
	InitComboPixType(); //初始化图像类型下拉框值对应的亮度等值是否可用,需在SetBinarization后
	
	if(g_nDeviceNumber != 1)//不为虚拟扫描仪，禁用多流
	{
		GetDlgItem(IDC_BASE_RADIO_DUPLEX_MUILTSTREAM)->EnableWindow(FALSE); //禁用多流
	}

	GetDlgItem(IDC_BASE_RADIO_SCANMODE_Flatbed)->ShowWindow(FALSE); //暂时隐藏平板。
	 
	//2.0版本界面修改
	//新增TAB控件  
	//m_base_tab.InsertItem(0, _T("自动彩色"));  //插入第一个标签     
	m_base_tab.InsertItem(0, _T("彩色"));  
	m_base_tab.InsertItem(1, _T("灰度"));     
	m_base_tab.InsertItem(2, _T("黑白"));
	//m_pTabAutoColor->Create(IDD_BASETAB_AUTOCOLOR, &m_base_tab); //创建第一个标签页     
	m_pTabColor->Create(IDD_BASETAB_COLOR, &m_base_tab); 
	m_pTabGray->Create(IDD_BASETAB_GRAY, &m_base_tab);    
	m_pTabBW->Create(IDD_BASETAB_BW, &m_base_tab);    

	m_base_tab.GetClientRect(&m_tabRect);    // 获取标签控件客户区Rect     
	// 调整tabRect，使其覆盖范围适合放置标签页     
	m_tabRect.left += 1;                    
	m_tabRect.right -= 1;     
	m_tabRect.top += 25;     
	m_tabRect.bottom -= 1;   
	//根据调整好的tabRect放置主子对话框，并设置为显示 
	//m_pTabAutoColor->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_SHOWWINDOW);     
	//根据调整好的tabRect放置其他子对话框，并设置为隐藏     
	m_pTabColor->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_SHOWWINDOW);  
	m_pTabGray->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_HIDEWINDOW);  
	m_pTabBW->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_HIDEWINDOW);  
	SetTabCtrl();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CPage_Base::InitSliderCtrl()
{
	float fMin,fMax,fStep;
	m_pUI->GetCapRangeFloat(ICAP_GAMMA, fMin, fMax, fStep);
	m_slider_gamma.SetRange((int)fMin, (int)fMax);
	m_slider_gamma.SetTicFreq((int)fStep); //步长

	m_pUI->GetCapRangeFloat(ICAP_AUTODISCARDBLANKPAGES, fMin, fMax, fStep);
	m_slider_removeblank.SetRange((int)fMin, (int)fMax);
	m_slider_removeblank.SetTicFreq((int)fStep); //步长

	UpdateData(FALSE);  // 更新控件，刷新当前对话框
}


void CPage_Base::SetFlat(void)
{
	if(1 == m_radiobtn_scanmode)//为1表示平板
	{
		m_basemap[CAP_DUPLEXENABLED] = 0.0f;
		m_radiobtn_duplex = 0; //平板时，只能是单面
	}	
	else //为0表示自动进纸器选中
	{
		m_radiobtn_scanmode = 0;
	}
	SetMultistream();
}


void CPage_Base::InitComboPixType(void)
{
	int nIndex = m_combo_colormode.GetCurSel();
	
	if(0 == nIndex) //黑白
	{
		m_edit_noisenum.EnableWindow(TRUE); 
		m_scroll_noisenum.EnableWindow(TRUE); 
		m_edit_noiserange.EnableWindow(TRUE); 
		m_scroll_noiserange.EnableWindow(TRUE); 
	} 
	else
	{
		m_edit_noisenum.EnableWindow(FALSE); 
		m_scroll_noisenum.EnableWindow(FALSE); 
		m_edit_noiserange.EnableWindow(FALSE); 
		m_scroll_noiserange.EnableWindow(FALSE); 
	}
}

void CPage_Base::OnCbnSelchangeBase_Combo_Colormode()
{
	// TODO: 在此添加控件通知处理程序代码
	int nIndex = m_combo_colormode.GetCurSel();

	if(GetDlgItem(IDC_BASE_COMBO_COLORMODE)->IsWindowEnabled())//图像类型可用时才设置
	{
		m_pUI->SetCapValueInt(ICAP_PIXELTYPE,nIndex);
	}	

	m_combo_colormode.SetCurSel(nIndex);

	InitComboPixType(); //zhu 判断亮度等是否可用
	SetTabCtrl();
}


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


BOOL CPage_Base::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	CEdit* pEdit1 = (CEdit*)GetDlgItem(IDC_BASE_EDIT_REMOVEBLANK);  
	CString str1;   
	GetDlgItemText(IDC_BASE_EDIT_REMOVEBLANK, str1); // 获取edit中文本  

	if( (GetFocus() == pEdit1) && (pMsg->message == WM_CHAR))  
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
	CEdit* pEdit2 = (CEdit*)GetDlgItem(IDC_BASE_EDIT_GAMMA); 
	CString str2;   
	GetDlgItemText(IDC_BASE_EDIT_GAMMA, str2); // 获取edit中文本  
	if( (GetFocus() == pEdit2) && (pMsg->message == WM_CHAR))  
	{  
		//允许输入数字和小数点“.”
		if(pMsg->wParam == '.')
		{
			//输入框只允许输入一个小数点
			int nPos3 = 0; 
			nPos3 = str2.Find('.'); // 查找.的位置 

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

	/*
	//获取控件窗口指针  
	CEdit* pEdit1 = (CEdit*)GetDlgItem(IDC_BASE_EDIT_BRIGHTNESS);  
	CEdit* pEdit2 = (CEdit*)GetDlgItem(IDC_BASE_EDIT_CONTRAST);  
	CEdit* pEdit3 = (CEdit*)GetDlgItem(IDC_BASE_EDIT_THRESHOLD);
	
	CString str1, str2, str3;   
	GetDlgItemText(IDC_BASE_EDIT_BRIGHTNESS, str1); // 获取edit中文本  
	GetDlgItemText(IDC_BASE_EDIT_CONTRAST, str2);
	GetDlgItemText(IDC_BASE_EDIT_THRESHOLD, str3);

	if( (GetFocus() == pEdit1 || GetFocus() == pEdit2 || GetFocus() == pEdit3) 
				&& (pMsg->message == WM_CHAR))  
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
		//接受Backspace和delete键 
		else if(pMsg->wParam == 0x08 || pMsg->wParam == 0x2E)  
		{
			  // 设置编辑框控件范围
			return 0;  
		}  
		else
		{
			return 1;
		}
	}

	//对比度、亮度能输入负号
	if( (GetFocus() == pEdit1 || GetFocus() == pEdit2) 
		&& (pMsg->message == WM_CHAR))
	{
		//保证负号'-'只能出现一次,并且只能出现在第一个字符
		if(pMsg->wParam == '-') //亮度、对比度只能输入负号与数字
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
	}
	*/
	return __super::PreTranslateMessage(pMsg);
}

void CPage_Base::OnBase_RadioBtn_Duplex()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE); //将radio的状态值更新给关联的变量
	switch(m_radiobtn_duplex)
	{
	case 0:
	case 1:
		m_pUI->SetCapValueInt(CAP_DUPLEXENABLED,m_radiobtn_duplex); 
		if(1 == m_radiobtn_duplex) //双面，单面该值为0
		{
			m_pUI->SetCapValueInt(UDSCAP_DOCS_IN_ADF, 2);
		}	
		else
		{
			m_pUI->SetCapValueInt(UDSCAP_DOCS_IN_ADF, 1);
		}

		m_basemap[UDSCAP_MULTISTREAM] = 0.0f;
		m_pUI->SetCapValueInt(UDSCAP_MULTISTREAM,FALSE);		

		break;
	case 2:
		m_basemap[UDSCAP_MULTISTREAM] = 1.0f;
		m_pUI->SetCapValueInt(UDSCAP_MULTISTREAM, TRUE);//直接设置有效
		
		m_check_frontcolor.SetCheck(TRUE);
		m_base_tab.SetCurSel(0);
		break;
	}
	SetFlat();//内含SetMultiStream();
	SetDenoise();
	SetCapMulti();
	UpdateData(FALSE);
}


void CPage_Base::SetMultistream(void)
{
	if(m_radiobtn_duplex == 2) //多流选中
	{
		m_check_frontcolor.EnableWindow(TRUE);
		m_check_frontgray.EnableWindow(TRUE);
		m_check_frontbw.EnableWindow(TRUE);
		m_check_backcolor.EnableWindow(TRUE);
		m_check_backgray.EnableWindow(TRUE);
		m_check_backbw.EnableWindow(TRUE);
		
		m_btn_frontcolor.EnableWindow(TRUE);
		m_btn_frontgray.EnableWindow(TRUE);
		m_btn_frontbw.EnableWindow(TRUE);
		m_btn_backcolor.EnableWindow(TRUE);
		m_btn_backgray.EnableWindow(TRUE);
		m_btn_backbw.EnableWindow(TRUE);

		m_combo_colormode.EnableWindow(FALSE);
		m_combo_splitimage.EnableWindow(FALSE);	
	} 
	else 
	{
		//多流输出未选中时，六个选项也均不要选中
		m_check_frontcolor.SetCheck(FALSE);
		m_check_frontgray.SetCheck(FALSE);
		m_check_frontbw.SetCheck(FALSE);
		m_check_backcolor.SetCheck(FALSE);
		m_check_backgray.SetCheck(FALSE);
		m_check_backbw.SetCheck(FALSE);

		m_check_frontcolor.EnableWindow(FALSE);
		m_check_frontgray.EnableWindow(FALSE);
		m_check_frontbw.EnableWindow(FALSE);
		m_check_backcolor.EnableWindow(FALSE);
		m_check_backgray.EnableWindow(FALSE);
		m_check_backbw.EnableWindow(FALSE);

		m_btn_frontcolor.EnableWindow(FALSE);
		m_btn_frontgray.EnableWindow(FALSE);
		m_btn_frontbw.EnableWindow(FALSE);
		m_btn_backcolor.EnableWindow(FALSE);
		m_btn_backgray.EnableWindow(FALSE);
		m_btn_backbw.EnableWindow(FALSE);

		m_combo_colormode.EnableWindow(TRUE);	
		m_combo_splitimage.EnableWindow(TRUE);
	}
}


void CPage_Base::OnBase_Btn_Check_FrontColor()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_check_frontcolor.GetCheck())
	{
		m_btn_frontcolor.SetFocus();
		m_btn_frontcolor.SetState(TRUE);
		m_btn_frontcolor.SetButtonStyle(BS_DEFPUSHBUTTON);
	}
	else
	{
		m_btn_frontcolor.SetState(FALSE);
	}
	SetDenoise();
	SetCapMulti();
	m_base_tab.SetCurSel(0);
}


void CPage_Base::OnBase_Btn_Check_FrontGray()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_check_frontgray.GetCheck())
	{
		m_btn_frontgray.SetFocus();
		m_btn_frontgray.SetState(TRUE);
		m_btn_frontgray.SetButtonStyle(BS_DEFPUSHBUTTON);
	}
	else
	{
		m_btn_frontgray.SetState(FALSE);
	}
	SetDenoise();
	SetCapMulti();
	m_base_tab.SetCurSel(1);
}


void CPage_Base::OnBase_Btn_Check_FrontBw()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_check_frontbw.GetCheck())
	{
		m_btn_frontbw.SetFocus();
		m_btn_frontbw.SetState(TRUE);
		m_btn_frontbw.SetButtonStyle(BS_DEFPUSHBUTTON);
	}
	else
	{
		m_btn_frontbw.SetState(FALSE);
	}
	SetDenoise();
	SetCapMulti();
	m_base_tab.SetCurSel(2);
}


void CPage_Base::OnBase_Btn_Check_BackColor()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_check_backcolor.GetCheck())
	{
		m_btn_backcolor.SetFocus();
		m_btn_backcolor.SetState(TRUE);
		m_btn_backcolor.SetButtonStyle(BS_DEFPUSHBUTTON);
	}
	else
	{
		m_btn_backcolor.SetState(FALSE);
	}
	SetDenoise();
	SetCapMulti();
	m_base_tab.SetCurSel(0);
}


void CPage_Base::OnBase_Btn_Check_BackGray()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_check_backgray.GetCheck())
	{
		m_btn_backgray.SetFocus();
		m_btn_backgray.SetState(TRUE);
		m_btn_backgray.SetButtonStyle(BS_DEFPUSHBUTTON);
	}
	else
	{
		m_btn_backgray.SetState(FALSE);
	}
	SetDenoise();
	SetCapMulti();
	m_base_tab.SetCurSel(1);
}


void CPage_Base::OnBase_Btn_Check_BackBw()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_check_backbw.GetCheck())
	{
		m_btn_backbw.SetFocus();
		m_btn_backbw.SetState(TRUE);
		m_btn_backbw.SetButtonStyle(BS_DEFPUSHBUTTON);
	}
	else
	{
		m_btn_backbw.SetState(FALSE);
	}
	SetDenoise();
	SetCapMulti();
	m_base_tab.SetCurSel(2);
}

BOOL CPage_Base::OnSetActive()
{
	// TODO: 在此添加专用代码和/或调用基类
	m_pUI->PreViewStatus();
	UpdateControls();
	
	Invalidate();

	SetSpiltimage();
	return __super::OnSetActive();
}


void CPage_Base::OnBase_RadioBtn_Scanmode_Auto()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE); //将radio的状态值更新给关联的变量
	
	m_radiobtn_scanmode = 0;
	int index = 1;	
	SetFlat();
	
	GetDlgItem(IDC_BASE_RADIO_DUPLEX_SHUANG)->EnableWindow(TRUE); 
	GetDlgItem(IDC_BASE_RADIO_DUPLEX_MUILTSTREAM)->EnableWindow(TRUE);
	
	m_basemap[CAP_FEEDERENABLED] = (float)index;
	UpdateData(FALSE);
}


void CPage_Base::OnBase_RadioBtn_Scanmode_Flatbed()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE); //将radio的状态值更新给关联的变量

	m_radiobtn_scanmode = 1;
	int index = 0;	
	SetFlat();

	GetDlgItem(IDC_BASE_RADIO_DUPLEX_SHUANG)->EnableWindow(FALSE); 
	GetDlgItem(IDC_BASE_RADIO_DUPLEX_MUILTSTREAM)->EnableWindow(FALSE);

	//m_basemap[CAP_FEEDERENABLED] = (float)index;
	if(m_radiobtn_scanmode==0)//自动进纸器
	{
		m_pUI->SetCapValueInt(CAP_FEEDERENABLED,TRUE); 
	}
	else //平板
	{
		m_pUI->SetCapValueInt(CAP_FEEDERENABLED,FALSE); 
	}

	UpdateData(FALSE);
}


void CPage_Base::OnTcnSelchangeBase_Tab(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	switch(m_base_tab.GetCurSel())     
	{       
	/*case 0:     
		m_pTabAutoColor->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_SHOWWINDOW);     
		m_pTabColor->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_HIDEWINDOW);  
		m_pTabGray->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_HIDEWINDOW);  
		m_pTabBW->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_HIDEWINDOW);  
		break;    */ 
	case 0:     
		//m_pTabAutoColor->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_HIDEWINDOW);     
		m_pTabColor->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_SHOWWINDOW);  
		m_pTabGray->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_HIDEWINDOW);  
		m_pTabBW->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_HIDEWINDOW);  
		break;
	case 1:
		//m_pTabAutoColor->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_HIDEWINDOW);     
		m_pTabColor->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_HIDEWINDOW);  
		m_pTabGray->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_SHOWWINDOW);  
		m_pTabBW->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_HIDEWINDOW); 
		break;
	case 2:
		//m_pTabAutoColor->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_HIDEWINDOW);     
		m_pTabColor->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_HIDEWINDOW);  
		m_pTabGray->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_HIDEWINDOW);  
		m_pTabBW->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_SHOWWINDOW); 
	default:     
		break;     
	}     

	*pResult = 0;
}


void CPage_Base::OnCbnSelchangeBase_Combo_Rotate()
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

	m_pUI->SetCapValueInt(ICAP_ROTATION,nval); 
	m_combo_rotate.SetCurSel(nIndex);
}


void CPage_Base::OnCbnSelchangeBase_Combo_SpiltImage()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
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
	if(m_combo_splitimage.IsWindowEnabled())
	{
		m_pUI->SetCapValueInt(UDSCAP_SPLITIMAGE,nval); //设置参数生效

		if(nval == TWSI_HORIZONTAL || nval == TWSI_VERTICAL)
		{
			m_pUI->SetCapValueInt(UDSCAP_DOCS_IN_ADF, 2); //设置扫描张数为2
		}
		else if(nval == TWSI_NONE && m_radiobtn_duplex == 0) //不拆分&&单面
		{
				m_pUI->SetCapValueInt(UDSCAP_DOCS_IN_ADF, 1); //不拆分时又设回1
		}	
		/*else if(nval == TWSI_DEFINED)
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
		m_pUI->SetCapValueInt(UDSCAP_SPLITIMAGE, TWSI_NONE); //设置参数生效
	}

	m_combo_splitimage.SetCurSel(nIndex);
	SetSpiltimage();
	UpdateData(FALSE);
}

void CPage_Base::SetSpiltimage()
{
	CString strCBText;
	GetDlgItemText(IDC_BASE_COMBO_SPLITIMG, strCBText);

	CString str;
	str.Format("%d",2);

	if(strCBText.Find("不拆分") >= 0)
	{
		//str.Format("%d",1);
		//SetDlgItemText(IDC_ADVANCED_EDIT_SPILT, str);
		//m_edit_spilt.EnableWindow(FALSE);

		//GetDlgItem(IDC_ADVANCED_RADIO_HORIZONTAL)->EnableWindow(FALSE);
		//GetDlgItem(IDC_ADVANCED_RADIO_VERTICAL)->EnableWindow(FALSE);
	}
	else if(strCBText.Find("上下") >= 0 || strCBText.Find("左右") >= 0) //水平、垂直分割图像
	{
		//SetDlgItemText(IDC_ADVANCED_EDIT_SPILT, str);
		//m_edit_spilt.EnableWindow(FALSE);

		//GetDlgItem(IDC_ADVANCED_RADIO_HORIZONTAL)->EnableWindow(FALSE);
		//GetDlgItem(IDC_ADVANCED_RADIO_VERTICAL)->EnableWindow(FALSE);
	}
	else if(strCBText.Find("自定义") >= 0)
	{
		//SetDlgItemText(IDC_ADVANCED_EDIT_SPILT, str);
		//m_edit_spilt.EnableWindow(TRUE);

		//GetDlgItem(IDC_ADVANCED_RADIO_HORIZONTAL)->EnableWindow(TRUE);
		//GetDlgItem(IDC_ADVANCED_RADIO_VERTICAL)->EnableWindow(TRUE);
	}
	else{}
}

void CPage_Base::OnNMCustomdrawBase_Slider_Gamma(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);  // 接收数据
	CString str;
	int sldValue = m_slider_gamma.GetPos();  // 获取滑块当前位置
	str.Format("%d",sldValue);

	float value = ((int)sldValue*10)/10.00f;
	m_pUI->SetCapValueFloat(ICAP_GAMMA,value);

	float valueTemp = ((float)sldValue)/100;
	str.Format("%.2f", valueTemp); //小数点后只要2位
	SetDlgItemText(IDC_BASE_EDIT_GAMMA, str);// 在编辑框同步显示滚动条值
	UpdateData(FALSE);  // 更新控件

	*pResult = 0;
}


void CPage_Base::OnEnChangeBase_Edit_Gamma()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 __super::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);  // 接收数据
	CString str;
	m_edit_gamma.GetWindowText(str);

	float fval = (float)(_ttof(str));
	int nval = (int)(100*fval);
	m_slider_gamma.SetPos(nval);
	m_pUI->SetCapValueFloat(ICAP_GAMMA, float(nval));

	m_edit_gamma.SetSel(str.GetLength(), str.GetLength(),TRUE);  // 设置编辑框控件范围
	UpdateData(FALSE);  // 更新控件
}


void CPage_Base::OnNMCustomdrawBase_Slider_Removeblank(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);  // 接收数据
	CString str;
	int sldValue = m_slider_removeblank.GetPos();  // 获取滑块当前位置

	if(m_slider_removeblank.IsWindowEnabled())
	{
		m_pUI->SetCapValueFloat(ICAP_AUTODISCARDBLANKPAGES,(float)sldValue);
	}	

	str.Format("%d", sldValue);
	SetDlgItemText(IDC_BASE_EDIT_REMOVEBLANK, str);
	UpdateData(FALSE);  // 更新控件

	*pResult = 0;
}


void CPage_Base::OnEnChangeBase_Edit_Removeblank()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 __super::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);  // 接收数据
	CString str;
	m_edit_removeblank.GetWindowText(str);
	int nval = _ttoi(str);
	m_slider_removeblank.SetPos(nval);

	if(m_slider_removeblank.IsWindowEnabled())
	{
		m_pUI->SetCapValueFloat(ICAP_AUTODISCARDBLANKPAGES,(float)nval);
	}	

	m_edit_removeblank.SetSel(str.GetLength(), str.GetLength(),TRUE);  // 设置编辑框控件范围
	UpdateData(FALSE);  // 更新控件
}


void CPage_Base::OnBase_Btn_Check_RemoveBlank()
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
	m_pUI->SetCapValueInt(UDSCAP_REMOVEBLANK,nval);
	SetBlank();
}

void CPage_Base::SetBlank(void)
{
	if(m_check_removeblank.GetCheck())
	{
		m_slider_removeblank.EnableWindow(TRUE);
		m_edit_removeblank.EnableWindow(TRUE);
	}
	else
	{
		m_slider_removeblank.EnableWindow(FALSE);
		m_edit_removeblank.EnableWindow(FALSE);
	}
}


void CPage_Base::OnBase_Btn_FrontColor()
{
	// TODO: 在此添加控件通知处理程序代码
	SetDenoise();
	m_base_tab.SetCurSel(0);
	//m_pTabAutoColor->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_HIDEWINDOW);     
	m_pTabColor->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_SHOWWINDOW);  
	m_pTabGray->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_HIDEWINDOW);  
	m_pTabBW->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_HIDEWINDOW);  
}


void CPage_Base::OnBase_Btn_Backcolor()
{
	// TODO: 在此添加控件通知处理程序代码
	SetDenoise();
	m_base_tab.SetCurSel(0);
	//m_pTabAutoColor->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_HIDEWINDOW);     
	m_pTabColor->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_SHOWWINDOW);  
	m_pTabGray->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_HIDEWINDOW);  
	m_pTabBW->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_HIDEWINDOW);  
}


void CPage_Base::OnBase_Btn_Frontgray()
{
	// TODO: 在此添加控件通知处理程序代码
	SetDenoise();
	m_base_tab.SetCurSel(1);
	//m_pTabAutoColor->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_HIDEWINDOW);     
	m_pTabColor->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_HIDEWINDOW);  
	m_pTabGray->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_SHOWWINDOW);  
	m_pTabBW->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_HIDEWINDOW); 
}


void CPage_Base::OnBase_Btn_Backgray()
{
	// TODO: 在此添加控件通知处理程序代码
	SetDenoise();
	m_base_tab.SetCurSel(1);
	//m_pTabAutoColor->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_HIDEWINDOW);     
	m_pTabColor->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_HIDEWINDOW);  
	m_pTabGray->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_SHOWWINDOW);  
	m_pTabBW->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_HIDEWINDOW); 
}


void CPage_Base::OnBase_Btn_Frontbw()
{
	// TODO: 在此添加控件通知处理程序代码
	SetDenoise();
	m_base_tab.SetCurSel(2);
	//m_pTabAutoColor->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_HIDEWINDOW);     
	m_pTabColor->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_HIDEWINDOW);  
	m_pTabGray->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_HIDEWINDOW);  
	m_pTabBW->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_SHOWWINDOW); 
}


void CPage_Base::OnBase_Btn_Backbw()
{
	// TODO: 在此添加控件通知处理程序代码
	SetDenoise();
	m_base_tab.SetCurSel(2);
	//m_pTabAutoColor->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_HIDEWINDOW);     
	m_pTabColor->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_HIDEWINDOW);  
	m_pTabGray->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_HIDEWINDOW);  
	m_pTabBW->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_SHOWWINDOW); 
}


void CPage_Base::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	int scrollpos;
	CString str;
	SetScroll();
	switch(pScrollBar->GetDlgCtrlID())
	{
		//去除噪声数目               
	case IDC_BASE_SCROLLBAR_NOISENUM:
		{
			scrollpos = m_scroll_noisenum.GetScrollPos();
			switch(nSBCode)
			{
			case SB_LINEUP: //如果向上滚动一列，则pos加1
				scrollpos += 1;
				break;
			case SB_LINEDOWN: //如果向下滚动一列，则pos减1
				scrollpos -= 1; 
				break;
			case SB_TOP: //最顶端
				scrollpos = 3600;
				break;
			case SB_BOTTOM:
				scrollpos = 0;
				break;
			}
			// 设置滚动块位置  
			m_scroll_noisenum.SetScrollPos(scrollpos);
			str.Format("%d", scrollpos);
			SetDlgItemText(IDC_BASE_EDIT_NOISENUM, str); 
		}
		break;
	//去除噪声-噪声范围
	case IDC_BASE_SCROLLBAR_NOISERANGE:
		{
			scrollpos = m_scroll_noiserange.GetScrollPos();
			switch(nSBCode)
			{
			case SB_LINEUP: //如果向上滚动一列，则pos加1
				scrollpos += 1;
				break;
			case SB_LINEDOWN: //如果向下滚动一列，则pos减1
				scrollpos -= 1; 
				break;
			case SB_TOP: //最顶端
				scrollpos = 30;
				break;
			case SB_BOTTOM:
				scrollpos = 1;
				break;
			}
			// 设置滚动块位置  
			m_scroll_noiserange.SetScrollPos(scrollpos);
			str.Format("%d", scrollpos);
			SetDlgItemText(IDC_BASE_EDIT_NOISERANGE, str); 
		}
	}
	__super::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CPage_Base::SetScroll()
{
	CString str;
	int nval;
	float fMin,fMax,fStep;
	
	m_pUI->GetCapRangeFloat(UDSCAP_NOISENUM, fMin, fMax, fStep);
	m_scroll_noisenum.SetScrollRange(fMin, fMax); 
	m_edit_noisenum.GetWindowText(str); 
	nval = _ttof(str);
	m_scroll_noisenum.SetScrollPos(nval); 

	m_pUI->GetCapRangeFloat(UDSCAP_NOISERANGE, fMin, fMax, fStep);
	m_scroll_noiserange.SetScrollRange((int)fMin, (int)fMax); 
	m_edit_noiserange.GetWindowText(str); 
	nval = _ttof(str);
	m_scroll_noiserange.SetScrollPos(nval); 
}

void CPage_Base::SetDenoise()
{
	if(m_check_frontbw.GetCheck() || m_check_backbw.GetCheck()
	|| m_btn_frontbw.GetCheck() || m_btn_backbw.GetCheck())
	{
		m_edit_noisenum.EnableWindow(TRUE); 
		m_scroll_noisenum.EnableWindow(TRUE); 
		m_edit_noiserange.EnableWindow(TRUE); 
		m_scroll_noiserange.EnableWindow(TRUE); 
	}
	else
	{
		m_edit_noisenum.EnableWindow(FALSE); 
		m_scroll_noisenum.EnableWindow(FALSE); 
		m_edit_noiserange.EnableWindow(FALSE); 
		m_scroll_noiserange.EnableWindow(FALSE); 
	}
}

void CPage_Base::OnEnChangeBase_Edit_NoiseNum()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 __super::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);  // 接收数据
	SetScroll();

	CString str;
	m_edit_noisenum.GetWindowText(str);
	float fval = _ttof(str);
	m_scroll_noisenum.SetScrollPos((int)fval);

	m_pUI->SetCapValueFloat(UDSCAP_NOISENUM, fval); 
	m_edit_noisenum.SetSel(str.GetLength(), str.GetLength(),TRUE);  // 设置编辑框控件范围

	UpdateData(FALSE);  // 更新控件
}


void CPage_Base::OnEnChangeBase_Edit_NoiseRange()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 __super::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);  // 接收数据
	SetScroll();

	CString str;
	m_edit_noiserange.GetWindowText(str);
	float fval = _ttof(str);
	m_scroll_noiserange.SetScrollPos((int)fval);

	m_pUI->SetCapValueFloat(UDSCAP_NOISERANGE, fval); 
	m_edit_noiserange.SetSel(str.GetLength(), str.GetLength(),TRUE);  // 设置编辑框控件范围

	UpdateData(FALSE);  // 更新控件
}

void CPage_Base::SetTabCtrl()
{
	int nIndex = m_combo_colormode.GetCurSel();
	switch(nIndex)
	{
	case 0:
		m_base_tab.SetCurSel(2);
		break;
	case 1:
		m_base_tab.SetCurSel(1);
		break;
	case 2:
		m_base_tab.SetCurSel(0);
		break;
	}	
}
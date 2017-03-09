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

IMPLEMENT_DYNAMIC(CPage_Base, CPropertyPage)

CPage_Base::CPage_Base(MFC_UI *pUI)
	: m_pUI(pUI),CPropertyPage(CPage_Base::IDD)
{
}


CPage_Base::~CPage_Base()
{
}

void CPage_Base::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BASE_COMBO_COLORMODE, m_combo_colormode);
	DDX_Control(pDX, IDC_BASE_COMBO_RESOLUTION, m_combo_resolution);
	DDX_Control(pDX, IDC_BASE_SLIDER_CONTRAST, m_slider_contrast);
	DDX_Control(pDX, IDC_BASE_SLIDER_BRIGHTNESS, m_slider_brightness);
	DDX_Control(pDX, IDC_BASE_SLIDER_THRESHOLD, m_slider_threshold);
	DDX_Control(pDX, IDC_BASE_EDIT_BRIGHTNESS, m_edit_brightness);
	DDX_Control(pDX, IDC_BASE_EDIT_CONTRAST, m_edit_contrast);
	DDX_Control(pDX, IDC_BASE_EDIT_THRESHOLD, m_edit_threshold);
	//  DDX_Control(pDX, IDC_BASE_BTN_CHOOSEIMAGE, m_btn_chooseimage);
	DDX_Radio(pDX, IDC_BASE_RADIO_DUPLEX_DAN, m_radiobtn_duplex);
	DDX_Control(pDX, IDC_CHECK_FRONTCOLOR, m_check_frontcolor);
	DDX_Control(pDX, IDC_CHECK_FRONTGRAY, m_check_frontgray);
	DDX_Control(pDX, IDC_CHECK_FRONTBW, m_check_frontbw);
	DDX_Control(pDX, IDC_CHECK_BACKBW, m_check_backbw);
	DDX_Control(pDX, IDC_CHECK_BACKGRAY, m_check_backgray);
	DDX_Control(pDX, IDC_CHECK_BACKCOLOR, m_check_backcolor);
	DDX_Control(pDX, IDC_BASE_COMBO_BINARIZATION, m_combo_binarization);
	DDX_Control(pDX, IDC_BASE_BUTTON_FRONTCOLOR, m_btn_frontcolor);
	DDX_Control(pDX, IDC_BASE_BUTTON_FRONTGRAY, m_btn_frontgray);
	DDX_Control(pDX, IDC_BASE_BUTTON_FRONTBW, m_btn_frontbw);
	DDX_Control(pDX, IDC_BASE_BUTTON_BACKCOLOR, m_btn_backcolor);
	DDX_Control(pDX, IDC_BASE_BUTTON_BACKGRAY, m_btn_backgray);
	DDX_Control(pDX, IDC_BASE_BUTTON_BACKBW, m_btn_backbw);
	DDX_Control(pDX, IDC_BASE_PREPICTURE, m_base_picture);
	DDX_Radio(pDX, IDC_BASE_RADIO_SCANMODE_AUTO, m_radiobtn_scanmode);
//	DDX_Control(pDX, IDC_BASE_RADIO_SCANMODE_AUTO, m_radiobtn_scanmode_auto);
//	DDX_Control(pDX, IDC_BASE_RADIO_SCANMODE_Flatbed, m_radiobtn_scanmode_flat);
}


BEGIN_MESSAGE_MAP(CPage_Base, CPropertyPage)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_BASE_SLIDER_CONTRAST, &CPage_Base::OnNMCustomdrawBase_Slider_Contrast)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_BASE_SLIDER_BRIGHTNESS, &CPage_Base::OnNMCustomdrawBase_Slider_Brightness)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_BASE_SLIDER_THRESHOLD, &CPage_Base::OnNMCustomdrawBase_Slider_Threshold)
	ON_EN_CHANGE(IDC_BASE_EDIT_CONTRAST, &CPage_Base::OnEnChangeBase_Edit_Contrast)
	ON_EN_CHANGE(IDC_BASE_EDIT_BRIGHTNESS, &CPage_Base::OnEnChangeBase_Edit_Brightness)
	ON_EN_CHANGE(IDC_BASE_EDIT_THRESHOLD, &CPage_Base::OnEnChangeBase_Edit_Threshold)
	ON_CBN_SELCHANGE(IDC_BASE_COMBO_COLORMODE, &CPage_Base::OnCbnSelchangeBase_Combo_Colormode)
	ON_CBN_SELCHANGE(IDC_BASE_COMBO_RESOLUTION, &CPage_Base::OnCbnSelchangeBase_Combo_Resolution)
//	ON_BN_CLICKED(IDC_BASE_BTN_CHOOSEIMAGE, &CPage_Base::OnBase_Btn_Chooseimage)
	ON_BN_CLICKED(IDC_BASE_RADIO_DUPLEX_DAN, &CPage_Base::OnBase_RadioBtn_Duplex)
	ON_BN_CLICKED(IDC_BASE_RADIO_DUPLEX_SHUANG, &CPage_Base::OnBase_RadioBtn_Duplex)
	ON_BN_CLICKED(IDC_BASE_RADIO_DUPLEX_MUILTSTREAM, &CPage_Base::OnBase_RadioBtn_Duplex)
	ON_BN_CLICKED(IDC_CHECK_FRONTCOLOR, &CPage_Base::OnBase_Btn_Check_FrontColor)
	ON_BN_CLICKED(IDC_CHECK_FRONTGRAY, &CPage_Base::OnBase_Btn_Check_FrontGray)
	ON_BN_CLICKED(IDC_CHECK_FRONTBW, &CPage_Base::OnBase_Btn_Check_FrontBw)
	ON_BN_CLICKED(IDC_CHECK_BACKCOLOR, &CPage_Base::OnBase_Btn_Check_BackColor)
	ON_BN_CLICKED(IDC_CHECK_BACKGRAY, &CPage_Base::OnBase_Btn_Check_BackGray)
	ON_BN_CLICKED(IDC_CHECK_BACKBW, &CPage_Base::OnBase_Btn_Check_BackBw)
	ON_CBN_SELCHANGE(IDC_BASE_COMBO_BINARIZATION, &CPage_Base::OnCbnSelchangeBase_Combo_Binarization)
	ON_BN_CLICKED(IDC_BASE_RADIO_SCANMODE_AUTO, &CPage_Base::OnBase_RadioBtn_Scanmode_Auto)
	ON_BN_CLICKED(IDC_BASE_RADIO_SCANMODE_Flatbed, &CPage_Base::OnBase_RadioBtn_Scanmode_Flatbed)
END_MESSAGE_MAP()


// CPage_Base 消息处理程序

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

		case UDSCAP_BINARIZATION: //二值化
		case ICAP_XRESOLUTION:
		case ICAP_YRESOLUTION:
			{
				m_pUI->SetCapValueInt(iter->first,(int)iter->second); 
				break;
			}	

		case CAP_FEEDERENABLED:
			{
				if(m_radiobtn_scanmode==0)//自动进纸器
				{
					m_pUI->SetCapValueInt(iter->first,TRUE); 
				}
				else //平板
				{
					m_pUI->SetCapValueInt(iter->first,FALSE); 
				}
				break;
			}

		case UDSCAP_REMOVESPOTS: //去除斑点
			{
				if(m_slider_threshold.IsWindowEnabled())
				{
					m_pUI->SetCapValueFloat(iter->first,iter->second);
				}	
				break;
			}

		case CAP_DUPLEXENABLED:
			{
				m_pUI->SetCapValueInt(iter->first,(int)iter->second); 

				if(1 == ((int)iter->second)) //双面，单面该值为0
				{
					m_pUI->SetCapValueInt(UDSCAP_DOCS_IN_ADF, 2);
				}	
				else
				{
					m_pUI->SetCapValueInt(UDSCAP_DOCS_IN_ADF, 1);
				}
				break;
			}

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
				BYTE temp = (BYTE)m_pUI->GetCapValueFloat(UDSCAP_MULTISTREAM_VALUE);
				unsigned int doc_count = 0;

				//若多流未选中多流 选项值设为0，,然后退出
				if( 0 == m_radiobtn_duplex) {
					m_pUI->SetCapValueFloat(iter->first, 0.0f); // 保存多流选项值
					break;
				}

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

				m_pUI->SetCapValueFloat(iter->first, (float)temp); // 保存多流选项值
				m_pUI->SetCapValueInt(UDSCAP_DOCS_IN_ADF,doc_count); // 设置纸张数

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
	int nval;

	CString str;
	GetDlgItemText(IDC_BASE_STATIC_THRESHOLD,str);
	if(str.Find("去除斑点") >= 0)
	{
		//多流输出-去除斑点 
		nCapValue = (int)(m_pUI->GetCapValueFloat(UDSCAP_REMOVESPOTS)); 
		m_slider_threshold.SetPos(nCapValue);
		strText.Format("%d",nCapValue);
		SetDlgItemText(IDC_BASE_EDIT_THRESHOLD, strText);
		m_basemap[UDSCAP_REMOVESPOTS] = (float)nCapIndex;
	}
	else if(str.Find("底色保留") >= 0)
	{
		//多流输出-底色保留  与二值化阈值是同样的意义 
		nCapValue = (int)(m_pUI->GetCapValueFloat(ICAP_THRESHOLD));
		m_slider_threshold.SetPos(nCapValue);
		strText.Format("%d",nCapValue);
		SetDlgItemText(IDC_ADVANCED_EDIT_SENSITIVE_THRESHOLD, strText);
		m_basemap[ICAP_THRESHOLD] = (float)nCapIndex;
	}
	else{}//必须保留

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
	m_basemap[CAP_FEEDERENABLED] = (float)m_radiobtn_scanmode;

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
	m_basemap[ICAP_PIXELTYPE] = (float)nCapIndex;
	InitComboPixType();

	//二值化
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
	m_basemap[UDSCAP_BINARIZATION] = (float)nCapIndex;
	SetBinarization();

	// 分辨率
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
	nval = (int)lstCapValuesFlt->at(nCapIndex);
	m_basemap[ICAP_XRESOLUTION] = (float)nval;
	m_basemap[ICAP_YRESOLUTION] = (float)nval;

	// 对比度 
	nCapValue = (int)(m_pUI->GetCapValueFloat(ICAP_CONTRAST)); 
	m_slider_contrast.SetPos(nCapValue);
	strText.Format("%d",nCapValue);
	SetDlgItemText(IDC_BASE_EDIT_CONTRAST,strText);
	m_basemap[ICAP_CONTRAST] = float(nCapValue);

	// 亮度 
	nCapValue = (int)(m_pUI->GetCapValueFloat(ICAP_BRIGHTNESS));
	m_slider_brightness.SetPos(nCapValue);
	strText.Format("%d",nCapValue);
	SetDlgItemText(IDC_BASE_EDIT_BRIGHTNESS,strText);
	m_basemap[ICAP_BRIGHTNESS] = float(nCapValue);

	// 阈值 
	nCapValue = (int)(m_pUI->GetCapValueFloat(ICAP_THRESHOLD));
	m_slider_threshold.SetPos(nCapValue);
	strText.Format("%d",nCapValue);
	SetDlgItemText(IDC_BASE_EDIT_THRESHOLD,strText);
	m_basemap[ICAP_THRESHOLD] = float(nCapValue);

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
		m_basemap[CAP_DUPLEXENABLED] = (float)nCapIndex;
	}
	scanside = m_radiobtn_duplex; //初始化scanside，防止用户未点击下拉框改变单双面直接扫描时，scanside默认为0，高级界面仍会设置裁切
	SetMultistream();
}


void CPage_Base::InitBasemap(void)
{
	m_basemap.erase(m_basemap.begin(),m_basemap.end());//清空

	int nCapIndex;
	nCapIndex = m_pUI->GetCurrentCapIndex(CAP_DUPLEXENABLED);
	m_basemap[CAP_DUPLEXENABLED] = (float)nCapIndex; //初始化时只为map插入“单双面”的值，特例

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
	
	SetMultistream();
	SetFlat();
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
	
	SetBinarization();
	InitComboPixType(); //初始化图像类型下拉框值对应的亮度等值是否可用,需在SetBinarization后

	m_pAdPage->InitAdvancedmap(); //初始化高级界面的Map
	
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CPage_Base::PreView()
{
	UpdateData(TRUE);

	SetCapValue();
	m_pAdPage->SetCapValue();
	m_pPaperPage->SetCapValue();
	m_pUI->TW_SaveProfileToFile("上次使用模板");

	BYTE *data = NULL; //图像数据
	data = m_pUI->PreView();  

	if(data != NULL)
	{
		// 保存图片
		char bmpFilePath[PATH_MAX];;	
		GetModuleFileName(g_hinstance, bmpFilePath, PATH_MAX);
		// strip filename from path
		size_t x = strlen(bmpFilePath);
		while(x > 0)
		{
			if(PATH_SEPERATOR == bmpFilePath[x-1])
			{
				bmpFilePath[x-1] = 0;
				break;
			}
			--x;
		}

		SSTRCPY(bmpFilePath, sizeof(bmpFilePath), bmpFilePath);
		strcat(bmpFilePath,  "\\");
		strcat(bmpFilePath, "preview.bmp");

		CFile file;
		try
		{
			if(file.Open(bmpFilePath, CFile::modeWrite | CFile::modeCreate))
			{
				//写入文件
				file.Write((LPSTR)&(m_pUI->m_bmpFileHeader), sizeof(BITMAPFILEHEADER)); // 写文件头
				file.Write((LPSTR)&(m_pUI->m_bmpInfoHeader), sizeof(BITMAPINFOHEADER)); // 写信息头
				if (m_pUI->m_nBpp < 16)
				{			
					DWORD dwColors = 0;
					if (true == m_pUI->GetColorsUsed(m_pUI->m_nBpp, dwColors))
					{
						file.Write((LPSTR)(m_pUI->m_bmpLpRGB),sizeof(RGBQUAD) * dwColors); // 写调色板
					}	

					if (m_pUI->m_bmpLpRGB)
					{
						delete []m_pUI->m_bmpLpRGB;
						m_pUI->m_bmpLpRGB = NULL;
					}
				}
				file.Write(data, m_pUI->m_nDIBSize); // 写数据
				file.Close();
				if (data)
				{
					delete []data;
					data = NULL;
				}	
			}
		}
		catch (...) 
		{
			AfxMessageBox("SaveDIB2Bmp Error!");
		}

		//显示图片
		IplImage* img = cvLoadImage((CT2CA)bmpFilePath, 1);
		CWnd *pWnd = GetDlgItem(IDC_BASE_PREPICTURE); 
		CDC* pDC = pWnd->GetDC();
		HDC hDC = pDC->GetSafeHdc();
		CRect rect;
		pWnd->GetClientRect(&rect);
		SetRect(rect, rect.left, rect.top, rect.right, rect.bottom);
		
		//调整长宽比例因子，使图像显示不失真
		CRect newRect;
		int width = img->width;
		int height = img->height;

		if(width <= rect.Width() && height <= rect.Height())//小图片，不缩放
		{
			newRect = CRect(rect.TopLeft(), CSize(width,height));
		}
		else
		{
			float xScale = (float)rect.Width() / (float)width;
			float yScale = (float)rect.Height() / (float)height;
			float scale = xScale>=yScale?yScale:xScale; 
			newRect = CRect(rect.TopLeft(), CSize((int)width*scale, (int)height*scale));
		}

		DrawToHDC(hDC, &newRect, img);
		ReleaseDC(pDC);
	}
}

//DrawToHdc系列函数
RECT CPage_Base::NormalizeRect(RECT r)  
{  
	int t;  

	if( r.left > r.right )  
	{  
		t = r.left;  
		r.left = r.right;  
		r.right = t;  
	}  

	if( r.top > r.bottom )  
	{  
		t = r.top;  
		r.top = r.bottom;  
		r.bottom = t;  
	}  

	return r;  
}  
CvRect CPage_Base::RectToCvRect(RECT sr)  
{  
	sr = NormalizeRect( sr );  
	return cvRect( sr.left, sr.top, sr.right - sr.left, sr.bottom - sr.top );  
} 
void  CPage_Base::FillBitmapInfo(BITMAPINFO* bmi, int width, int height, int bpp, int origin)  
{  
	assert( bmi && width >= 0 && height >= 0 && (bpp == 8 || bpp == 24 || bpp == 32));  

	BITMAPINFOHEADER* bmih = &(bmi->bmiHeader);  

	memset(bmih, 0, sizeof(*bmih));  
	bmih->biSize = sizeof(BITMAPINFOHEADER);  
	bmih->biWidth = width;  
	bmih->biHeight = origin ? abs(height) : -abs(height);  
	bmih->biPlanes = 1;  
	bmih->biBitCount = (unsigned short)bpp;  
	bmih->biCompression = BI_RGB;  

	if(bpp == 8)  
	{  
		RGBQUAD* palette = bmi->bmiColors;  
		int i;  
		for( i = 0; i < 256; i++ )  
		{  
			palette[i].rgbBlue = palette[i].rgbGreen = palette[i].rgbRed = (BYTE)i;  
			palette[i].rgbReserved = 0;  
		}  
	}  
}  
void  CPage_Base::Show(IplImage* img, HDC dc, int x, int y, int w, int h, int from_x, int from_y)  
{  
	if( img && img->depth == IPL_DEPTH_8U )  
	{  
		uchar buffer[sizeof(BITMAPINFOHEADER) + 1024];  
		BITMAPINFO* bmi = (BITMAPINFO*)buffer;  
		int bmp_w = img->width, bmp_h = img->height;  

		int bpp = img ? (img->depth & 255)*img->nChannels : 0;
		FillBitmapInfo(bmi, bmp_w, bmp_h, bpp, img->origin);  

		from_x = MIN(MAX( from_x, 0 ), bmp_w - 1);  
		from_y = MIN(MAX( from_y, 0 ), bmp_h - 1);  

		int sw = MAX(MIN( bmp_w - from_x, w ), 0);  
		int sh = MAX(MIN( bmp_h - from_y, h ), 0);  

		SetDIBitsToDevice(  
			dc, x, y, sw, sh, from_x, from_y, from_y, sh,  
			img->imageData + from_y*img->widthStep,  
			bmi, DIB_RGB_COLORS);  
	}  
}  
void  CPage_Base::DrawToHDC(HDC hDCDst, RECT* pDstRect, IplImage* img )  
{  
	if(pDstRect && img && img->depth == IPL_DEPTH_8U && img->imageData )  
	{  
		uchar buffer[sizeof(BITMAPINFOHEADER) + 1024];  
		BITMAPINFO* bmi = (BITMAPINFO*)buffer;  
		int bmp_w = img->width, bmp_h = img->height;  

		CvRect roi = cvGetImageROI(img);
		CvRect dst = RectToCvRect(*pDstRect);  

		if( roi.width == dst.width && roi.height == dst.height )  
		{  
			Show(img, hDCDst, dst.x, dst.y, dst.width, dst.height, roi.x, roi.y);  
			return;  
		}  

		if(roi.width > dst.width)  
		{  
			SetStretchBltMode(hDCDst, // handle to device context  
				HALFTONE );  
		}  
		else  
		{  
			SetStretchBltMode(hDCDst, // handle to device context  
				COLORONCOLOR );  
		}  

		int bpp = img ? (img->depth & 255)*img->nChannels : 0;
		FillBitmapInfo(bmi, bmp_w, bmp_h, bpp, img->origin);  
		::StretchDIBits(
			hDCDst,  
			dst.x, dst.y, dst.width, dst.height,  
			roi.x, roi.y, roi.width, roi.height,  
			img->imageData, bmi, DIB_RGB_COLORS, SRCCOPY);  
	}  
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

	UpdateData(FALSE);  // 更新控件，刷新当前对话框
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
	m_basemap[ICAP_CONTRAST] = float(sldValue);

	str.Format("%d", sldValue);
	SetDlgItemText(IDC_BASE_EDIT_CONTRAST, str);

	//contrast = sldValue;
	//m_pAdPage->UpdateControls();

	UpdateData(FALSE);  // 更新控件

	*pResult = 0;
}


void CPage_Base::OnNMCustomdrawBase_Slider_Brightness(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);  // 接收数据
	CString str;
	int sldValue = m_slider_brightness.GetPos();  // 获取滑块当前位置
	m_basemap[ICAP_BRIGHTNESS] = float(sldValue);

	str.Format("%d", sldValue);
	SetDlgItemText(IDC_BASE_EDIT_BRIGHTNESS,str);

	//brightness = sldValue;
	//m_pAdPage->UpdateControls();

	UpdateData(FALSE);  // 更新控件

	*pResult = 0;
}


void CPage_Base::OnNMCustomdrawBase_Slider_Threshold(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码	
	UpdateData(TRUE); //接收数据
	CString str;
	int sldValue = m_slider_threshold.GetPos(); //获取滑块的当前位置
	m_basemap[ICAP_THRESHOLD] = (float)sldValue;

	str.Format("%d", sldValue);
	SetDlgItemText(IDC_BASE_EDIT_THRESHOLD, str);
	UpdateData(FALSE); //更新控件。

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
	m_basemap[ICAP_CONTRAST] = (float)nval;

	m_edit_contrast.SetSel(str.GetLength(), str.GetLength(),TRUE);  // 设置编辑框控件范围
	
	//contrast = nval;
	//m_pAdPage->UpdateControls(); //更新一次高级界面，同步对比度参数

	UpdateData(FALSE);  // 更新控件
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
	m_basemap[ICAP_BRIGHTNESS] = (float)nval;
	m_edit_brightness.SetSel(str.GetLength(), str.GetLength(),TRUE);  // 设置编辑框控件范围

	//brightness = nval;
	//m_pAdPage->UpdateControls(); //更新一次高级界面，同步对比度参数

	UpdateData(FALSE);  // 更新控件
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
	m_basemap[ICAP_THRESHOLD] = (float)nval;

	m_edit_threshold.SetSel(str.GetLength(), str.GetLength(), TRUE); //设置编辑框控件范围

	UpdateData(FALSE); //更新控件
}

void CPage_Base::SetFlat(void)
{
	if(1 == m_radiobtn_scanmode)//为1表示平板
	{
		m_basemap[CAP_DUPLEXENABLED] = 0.0f;
		m_radiobtn_duplex = 0; //平板时，只能是单面
	}	
	else //为0表示自动进纸器选中
	{}
	SetMultistream();
}


void CPage_Base::InitComboPixType(void)
{
	int nIndex = m_combo_colormode.GetCurSel();

	if(0 == nIndex) //黑白
	{
		m_slider_contrast.EnableWindow(FALSE);  //由指针改为变量，否则初始化就闪退
		m_edit_contrast.EnableWindow(FALSE);
		m_slider_brightness.EnableWindow(FALSE);
		m_edit_brightness.EnableWindow(FALSE);

		m_slider_threshold.EnableWindow(TRUE);
		m_edit_threshold.EnableWindow(TRUE);
		m_combo_binarization.EnableWindow(TRUE);
	} 
	else
	{
		m_slider_contrast.EnableWindow(TRUE);  
		m_edit_contrast.EnableWindow(TRUE);
		m_slider_brightness.EnableWindow(TRUE);
		m_edit_brightness.EnableWindow(TRUE);

		m_slider_threshold.EnableWindow(FALSE);
		m_edit_threshold.EnableWindow(FALSE);
		m_combo_binarization.EnableWindow(FALSE);
	}

	BaseStatus(); //获取base界面图像模式,传给高级界面。
}

void CPage_Base::OnCbnSelchangeBase_Combo_Colormode()
{
	// TODO: 在此添加控件通知处理程序代码
	int nIndex = m_combo_colormode.GetCurSel();
	m_basemap[ICAP_PIXELTYPE] = (float)nIndex;
	m_combo_colormode.SetCurSel(nIndex);

	InitComboPixType(); //zhu 判断亮度等是否可用
	return;
}


void CPage_Base::OnCbnSelchangeBase_Combo_Resolution()
{
	// TODO: 在此添加控件通知处理程序代码
	int nIndex = m_combo_resolution.GetCurSel();
	CString strCBText; 
	m_combo_resolution.GetLBText(nIndex, strCBText);
	int nval = _ttoi(strCBText);  // CString 转 int

	m_basemap[ICAP_XRESOLUTION] = (float)nval;
	m_basemap[ICAP_YRESOLUTION] = (float)nval;
	
	m_combo_resolution.SetCurSel(nIndex);
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
	//获取控件窗口指针  
	CEdit* pEdit1 = (CEdit*)GetDlgItem(IDC_BASE_EDIT_BRIGHTNESS);  
	CEdit* pEdit2 = (CEdit*)GetDlgItem(IDC_BASE_EDIT_CONTRAST);  

	CString str1, str2;   
	GetDlgItemText(IDC_BASE_EDIT_BRIGHTNESS, str1); // 获取edit中文本  
	GetDlgItemText(IDC_BASE_EDIT_CONTRAST, str2);

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
		//保证负号'-'只能出现一次,并且只能出现在第一个字符
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

	return __super::PreTranslateMessage(pMsg);
}


//void CPage_Base::OnBase_RadioBtn_Scanmode()
//{
//	// TODO: 在此添加控件通知处理程序代码
//	UpdateData(TRUE); //将radio的状态值更新给关联的变量
//	int index;
//	SetFlat();
//	if(1 == m_radiobtn_scanmode)
//	{
//		index = 0;
//		GetDlgItem(IDC_BASE_RADIO_DUPLEX_SHUANG)->EnableWindow(FALSE); 
//		GetDlgItem(IDC_BASE_RADIO_DUPLEX_MUILTSTREAM)->EnableWindow(FALSE);
//	}
//	else //为0表示自动进纸器选中
//	{
//		index = 1;
//		GetDlgItem(IDC_BASE_RADIO_DUPLEX_SHUANG)->EnableWindow(TRUE); 
//		GetDlgItem(IDC_BASE_RADIO_DUPLEX_MUILTSTREAM)->EnableWindow(TRUE);
//	}
//
//	m_basemap[CAP_FEEDERENABLED] = (float)index;
//	UpdateData(FALSE);
//}


void CPage_Base::OnBase_RadioBtn_Duplex()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE); //将radio的状态值更新给关联的变量
	scanside = m_radiobtn_duplex; 
	switch(m_radiobtn_duplex)
	{
	case 0:
	case 1:
		m_basemap[CAP_DUPLEXENABLED] = (float)m_radiobtn_duplex;

		m_pUI->SetCapValueInt(UDSCAP_MULTISTREAM,FALSE);
		m_basemap[UDSCAP_MULTISTREAM] = 0.0f;

		m_slider_brightness.EnableWindow(TRUE); //亮度
		m_slider_contrast.EnableWindow(TRUE); //对比度
		m_edit_brightness.EnableWindow(TRUE); //亮度Edit
		m_edit_contrast.EnableWindow(TRUE); //对比度Edit

		break;
	case 2:
		m_basemap[UDSCAP_MULTISTREAM] = 1.0f;
		break;
	}
	SetFlat();//内含SetMultiStream();

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
		
		m_combo_colormode.EnableWindow(FALSE);

		SetColorGrayImage();
		SetBWImage();
		//GetDlgItem(IDC_ADVANCED_COMBO_SPLITIMG)->EnableWindow(FALSE); //图像拆分不可用

		//((CButton*)GetDlgItem(IDC_CHECK_AUTOCROP))->SetCheck(FALSE); //自动裁切与校正不选中
		//((CButton*)GetDlgItem(IDC_CHECK_REMOVEPUNCH))->SetCheck(FALSE); //去除穿孔不选中

		//m_pUI->SetCapValueInt(UDSCAP_PUNCHHOLEREMOVEL,TWRP_DISABLE);
		//m_pUI->SetCapValueInt(UDSCAP_AUTOCROP,TWAC_DISABLE);
		//GetDlgItem(IDC_CHECK_AUTOCROP)->EnableWindow(FALSE); //自动裁切与校正不可用
		//GetDlgItem(IDC_CHECK_REMOVEPUNCH)->EnableWindow(FALSE);//去除穿孔不可用
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

		m_combo_colormode.EnableWindow(TRUE);	
		//GetDlgItem(IDC_ADVANCED_COMBO_SPLITIMG)->EnableWindow(TRUE); //图像拆分可用
		//GetDlgItem(IDC_CHECK_AUTOCROP)->EnableWindow(TRUE); //自动裁切与校正可用
		//GetDlgItem(IDC_CHECK_REMOVEPUNCH)->EnableWindow(TRUE); //去除穿孔可用			
	}
}


void CPage_Base::SetColorGrayImage(void)
{
	if (m_check_frontgray.GetCheck() || m_check_frontcolor.GetCheck()
		|| m_check_backcolor.GetCheck() || m_check_backgray.GetCheck())
	{
		m_slider_brightness.EnableWindow(TRUE); 
		m_slider_contrast.EnableWindow(TRUE); 
		m_edit_brightness.EnableWindow(TRUE); 
		m_edit_contrast.EnableWindow(TRUE); 
	} 
	else
	{
		m_slider_brightness.EnableWindow(FALSE); 
		m_slider_contrast.EnableWindow(FALSE); 
		m_edit_brightness.EnableWindow(FALSE); 
		m_edit_contrast.EnableWindow(FALSE);
	}
}


void CPage_Base::SetBWImage(void)
{
	if(m_check_frontbw.GetCheck() || m_check_backbw.GetCheck())
	{
		m_slider_threshold.EnableWindow(TRUE); 
		m_edit_threshold.EnableWindow(TRUE); 

		//图像增强系列不可用
		/*GetDlgItem(IDC_CHECK_REMOVEDEMOISE)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_REMOVEDESCREEN)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_SHARPEN)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_REMOVEBACK)->EnableWindow(FALSE);*/
	} 
	else
	{
		m_slider_threshold.EnableWindow(FALSE); 
		m_edit_threshold.EnableWindow(FALSE); 
		//图像增强系列可用
		/*GetDlgItem(IDC_CHECK_REMOVEDEMOISE)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_REMOVEDESCREEN)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_SHARPEN)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_REMOVEBACK)->EnableWindow(TRUE);*/
	}
	SetBinarization(); //设置“去除斑点”还是“阈值”
}


void CPage_Base::OnBase_Btn_Check_FrontColor()
{
	// TODO: 在此添加控件通知处理程序代码
	SetColorGrayImage();
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
}


void CPage_Base::OnBase_Btn_Check_FrontGray()
{
	// TODO: 在此添加控件通知处理程序代码
	SetColorGrayImage();
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
}


void CPage_Base::OnBase_Btn_Check_FrontBw()
{
	// TODO: 在此添加控件通知处理程序代码
	SetBWImage();
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
}


void CPage_Base::OnBase_Btn_Check_BackColor()
{
	// TODO: 在此添加控件通知处理程序代码
	SetColorGrayImage();
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
}


void CPage_Base::OnBase_Btn_Check_BackGray()
{
	// TODO: 在此添加控件通知处理程序代码
	SetColorGrayImage();
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
}


void CPage_Base::OnBase_Btn_Check_BackBw()
{
	// TODO: 在此添加控件通知处理程序代码
	SetBWImage();
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
}


BOOL CPage_Base::OnSetActive()
{
	// TODO: 在此添加专用代码和/或调用基类
	m_pUI->PreViewStatus();
	return __super::OnSetActive();
}


void CPage_Base::OnCbnSelchangeBase_Combo_Binarization()
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
	m_basemap[UDSCAP_BINARIZATION] = (float)nval;

	SetBinarization();
	m_combo_binarization.SetCurSel(nIndex);
}

void CPage_Base::SetBinarization(void)
{
	int nCapValue;
	float fMin,fMax,fStep;
	
	if(m_radiobtn_duplex == 0 || m_radiobtn_duplex == 1 || m_check_frontbw.GetCheck() || m_check_backbw.GetCheck())
	{
		m_slider_threshold.EnableWindow(TRUE);
		m_edit_threshold.EnableWindow(TRUE); 

		CString strCBText;
		GetDlgItemText(IDC_BASE_COMBO_BINARIZATION,strCBText);
		if (strCBText.Find("动态阈值") >= 0)
		{
			SetDlgItemText(IDC_BASE_STATIC_THRESHOLD, TEXT("去除斑点:"));
			m_pUI->GetCapRangeFloat(UDSCAP_REMOVESPOTS, fMin, fMax, fStep);
			m_slider_threshold.SetRange((int)fMin, (int)fMax);
			m_slider_threshold.SetTicFreq((int)fStep); 

			nCapValue = (int)(m_pUI->GetCapValueFloat(UDSCAP_REMOVESPOTS));
			m_slider_threshold.SetPos(nCapValue);

			//设置此时亮度不可用
			m_slider_brightness.EnableWindow(FALSE);
			m_edit_brightness.EnableWindow(FALSE); 

		} 
		else if(strCBText.Find("固定阈值") >= 0)
		{
			SetDlgItemText(IDC_BASE_STATIC_THRESHOLD, TEXT("阈值:"));
			m_pUI->GetCapRangeFloat(ICAP_THRESHOLD, fMin, fMax, fStep);
			m_slider_threshold.SetRange((int)fMin, (int)fMax);
			m_slider_threshold.SetTicFreq((int)fStep); 

			nCapValue = (int)(m_pUI->GetCapValueFloat(ICAP_THRESHOLD));
			m_slider_threshold.SetPos(nCapValue);

			//设置此时亮度可用
			m_slider_brightness.EnableWindow(TRUE);
			m_edit_brightness.EnableWindow(TRUE);
		}
		else if(strCBText.Find("半色调") >= 0 || strCBText.Find("误差扩散") >= 0)
		{
			m_slider_threshold.EnableWindow(FALSE);
			m_edit_threshold.EnableWindow(FALSE);

			//设置此时亮度可用
			m_slider_brightness.EnableWindow(TRUE);
			m_edit_brightness.EnableWindow(TRUE);
		}
	}
	else
	{
		m_slider_threshold.EnableWindow(FALSE);
		m_edit_threshold.EnableWindow(FALSE);
	} 
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

	m_basemap[CAP_FEEDERENABLED] = (float)index;
	UpdateData(FALSE);
}

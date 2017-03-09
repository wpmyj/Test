// Page_Base.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "UDS Scanner.h"
#include "Page_Base.h"
#include "afxdialogex.h"
#include "Dlg_Profile.h"

#include "CTWAINDS_UDS.h"

// CPage_Base �Ի���
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


// CPage_Base ��Ϣ�������

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
				m_pUI->SetCapValueFloat(iter->first,iter->second);  // ������ֵΪ��ǰ������ֵ
				break;
			}

		case ICAP_PIXELTYPE:
			{
				if(GetDlgItem(IDC_BASE_COMBO_COLORMODE)->IsWindowEnabled())//ͼ�����Ϳ���ʱ������
				{
					m_pUI->SetCapValueInt(iter->first,(int)iter->second);
				}	

				break;
			}

		case UDSCAP_BINARIZATION: //��ֵ��
		case ICAP_XRESOLUTION:
		case ICAP_YRESOLUTION:
			{
				m_pUI->SetCapValueInt(iter->first,(int)iter->second); 
				break;
			}	

		case CAP_FEEDERENABLED:
			{
				if(m_radiobtn_scanmode==0)//�Զ���ֽ��
				{
					m_pUI->SetCapValueInt(iter->first,TRUE); 
				}
				else //ƽ��
				{
					m_pUI->SetCapValueInt(iter->first,FALSE); 
				}
				break;
			}

		case UDSCAP_REMOVESPOTS: //ȥ���ߵ�
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

				if(1 == ((int)iter->second)) //˫�棬�����ֵΪ0
				{
					m_pUI->SetCapValueInt(UDSCAP_DOCS_IN_ADF, 2);
				}	
				else
				{
					m_pUI->SetCapValueInt(UDSCAP_DOCS_IN_ADF, 1);
				}
				break;
			}

		case UDSCAP_MULTISTREAM: //�������
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
	
		case UDSCAP_MULTISTREAM_VALUE: // �������ѡ��ֵ
			{
				BYTE temp = (BYTE)m_pUI->GetCapValueFloat(UDSCAP_MULTISTREAM_VALUE);
				unsigned int doc_count = 0;

				//������δѡ�ж��� ѡ��ֵ��Ϊ0��,Ȼ���˳�
				if( 0 == m_radiobtn_duplex) {
					m_pUI->SetCapValueFloat(iter->first, 0.0f); // �������ѡ��ֵ
					break;
				}

				/************************************************************
				* �ж϶���chebox��ѡ���״̬;
				* ��ѡ����&���϶�ӦbitΪ1��ֵ;
				* ��δѡ����'|'�϶�ӦbitΪ0��ֵ.
				************************************************************/
				// �����ɫ
				if (m_check_frontcolor.GetCheck()) {  
					temp |= 0x01;
					doc_count++;
				} 
				else {
					temp &= (0xFF-0x01);
				}
				// �����ɫ
				if (m_check_frontgray.GetCheck()) {
					temp |= 0x02;
					doc_count++;
				} 
				else {
					temp &= (0xFF-0x02);
				}
				// ����ڰ�
				if (m_check_frontbw.GetCheck()) {
					temp |= 0x04;
					doc_count++;
				} 
				else {
					temp &= (0xFF-0x04);
				}
				// �����ɫ
				if (m_check_backcolor.GetCheck()) {
					temp |= 0x10;
					doc_count++;
				} 
				else {
					temp &= (0xFF-0x10);
				}
				// ����Ҷ�
				if (m_check_backgray.GetCheck()) {
					temp |= 0x20;
					doc_count++;
				} 
				else {
					temp &= (0xFF-0x20);
				}
				// ����ڰ�
				if (m_check_backbw.GetCheck()) {
					temp |= 0x40;
					doc_count++;
				} 
				else {
					temp &= (0xFF-0x40);
				}

				m_pUI->SetCapValueFloat(iter->first, (float)temp); // �������ѡ��ֵ
				m_pUI->SetCapValueInt(UDSCAP_DOCS_IN_ADF,doc_count); // ����ֽ����

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
	if(str.Find("ȥ���ߵ�") >= 0)
	{
		//�������-ȥ���ߵ� 
		nCapValue = (int)(m_pUI->GetCapValueFloat(UDSCAP_REMOVESPOTS)); 
		m_slider_threshold.SetPos(nCapValue);
		strText.Format("%d",nCapValue);
		SetDlgItemText(IDC_BASE_EDIT_THRESHOLD, strText);
		m_basemap[UDSCAP_REMOVESPOTS] = (float)nCapIndex;
	}
	else if(str.Find("��ɫ����") >= 0)
	{
		//�������-��ɫ����  ���ֵ����ֵ��ͬ�������� 
		nCapValue = (int)(m_pUI->GetCapValueFloat(ICAP_THRESHOLD));
		m_slider_threshold.SetPos(nCapValue);
		strText.Format("%d",nCapValue);
		SetDlgItemText(IDC_ADVANCED_EDIT_SENSITIVE_THRESHOLD, strText);
		m_basemap[ICAP_THRESHOLD] = (float)nCapIndex;
	}
	else{}//���뱣��

	// ɨ�跽ʽ	
	nCapIndex = m_pUI->GetCurrentCapIndex(CAP_FEEDERENABLED);
	if(0 == nCapIndex) //ƽ��
	{
		m_radiobtn_scanmode = 1;
	}
	else //1Ϊ�Զ���ֽ��
	{
		m_radiobtn_scanmode = 0;
	}
	SetFlat();
	m_basemap[CAP_FEEDERENABLED] = (float)m_radiobtn_scanmode;

  // ͼ������ 
	m_combo_colormode.ResetContent();
	nCapIndex = m_pUI->GetCurrentCapIndex(ICAP_PIXELTYPE);
	lstCapValues = m_pUI->GetValidCap(ICAP_PIXELTYPE);
	for(unsigned int i=0; i<lstCapValues->size();i++)
	{
		switch(lstCapValues->at(i))
		{
		case TWPT_BW:
			m_combo_colormode.InsertString(i,"�ڰ�");
			break;
		case TWPT_GRAY:
			m_combo_colormode.InsertString(i,"�Ҷ�");
			break;
		case TWPT_RGB:
			m_combo_colormode.InsertString(i,"��ɫ");
			break;
		default:
			continue;
		}
	}
	m_combo_colormode.SetCurSel(nCapIndex);
	m_basemap[ICAP_PIXELTYPE] = (float)nCapIndex;
	InitComboPixType();

	//��ֵ��
	m_combo_binarization.ResetContent(); //�������
	nCapIndex = m_pUI->GetCurrentCapIndex(UDSCAP_BINARIZATION);
	lstCapValues = m_pUI->GetValidCap(UDSCAP_BINARIZATION);
	for(unsigned int i=0; i<lstCapValues->size();i++)
	{
		switch(lstCapValues->at(i))
		{
		case TWBZ_DYNATHRESHOLD:
			m_combo_binarization.InsertString(i,"��̬��ֵ");
			break;
		case TWBZ_FIXEDTHRESHOLD:
			m_combo_binarization.InsertString(i,"�̶���ֵ"); 
			break;
		case TWBZ_HALFTONE1:
			m_combo_binarization.InsertString(i,"��ɫ��1");
			break;
		case TWBZ_HALFTONE2:
			m_combo_binarization.InsertString(i,"��ɫ��2");
			break;
		case TWBZ_HALFTONE3:
			m_combo_binarization.InsertString(i,"��ɫ��3");
			break;
		case TWBZ_HALFTONE4:
			m_combo_binarization.InsertString(i,"��ɫ��4");
			break;
		case TWBZ_HALFTONE5:
			m_combo_binarization.InsertString(i,"��ɫ��5");
			break;
		case TWBZ_ERRORDIFF:
			m_combo_binarization.InsertString(i,"�����ɢ");
			break;
		default:
			continue;
		}
	}
	m_combo_binarization.SetCurSel(nCapIndex);
	m_basemap[UDSCAP_BINARIZATION] = (float)nCapIndex;
	SetBinarization();

	// �ֱ���
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

	// �Աȶ� 
	nCapValue = (int)(m_pUI->GetCapValueFloat(ICAP_CONTRAST)); 
	m_slider_contrast.SetPos(nCapValue);
	strText.Format("%d",nCapValue);
	SetDlgItemText(IDC_BASE_EDIT_CONTRAST,strText);
	m_basemap[ICAP_CONTRAST] = float(nCapValue);

	// ���� 
	nCapValue = (int)(m_pUI->GetCapValueFloat(ICAP_BRIGHTNESS));
	m_slider_brightness.SetPos(nCapValue);
	strText.Format("%d",nCapValue);
	SetDlgItemText(IDC_BASE_EDIT_BRIGHTNESS,strText);
	m_basemap[ICAP_BRIGHTNESS] = float(nCapValue);

	// ��ֵ 
	nCapValue = (int)(m_pUI->GetCapValueFloat(ICAP_THRESHOLD));
	m_slider_threshold.SetPos(nCapValue);
	strText.Format("%d",nCapValue);
	SetDlgItemText(IDC_BASE_EDIT_THRESHOLD,strText);
	m_basemap[ICAP_THRESHOLD] = float(nCapValue);

	//���������Ĭ�ϲ�ʹ��
	nCapValue = (int)(m_pUI->GetCapValueBool(UDSCAP_MULTISTREAM));
	if(nCapValue == 1) //����ѡ��
	{
		m_radiobtn_duplex = 2;
		// ��ȡ����ѡ��ֵ�����¿ؼ�״̬
		BYTE value = (BYTE)m_pUI->GetCapValueFloat(UDSCAP_MULTISTREAM_VALUE);
		// ѭ���ж�ÿbit��ֵ�������¶�Ӧ�ؼ���״̬
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
			value = value >> 1; // ʼ�ձȽ����λ
		} // for end
	}//if end
	else //��˫��ѡ��
	{
		// ����/˫��ɨ
		nCapIndex = m_pUI->GetCurrentCapIndex(CAP_DUPLEXENABLED);
		m_radiobtn_duplex = nCapIndex; //0Ϊ���棬1Ϊ˫��
		m_basemap[CAP_DUPLEXENABLED] = (float)nCapIndex;
	}
	scanside = m_radiobtn_duplex; //��ʼ��scanside����ֹ�û�δ���������ı䵥˫��ֱ��ɨ��ʱ��scansideĬ��Ϊ0���߼������Ի����ò���
	SetMultistream();
}


void CPage_Base::InitBasemap(void)
{
	m_basemap.erase(m_basemap.begin(),m_basemap.end());//���

	int nCapIndex;
	nCapIndex = m_pUI->GetCurrentCapIndex(CAP_DUPLEXENABLED);
	m_basemap[CAP_DUPLEXENABLED] = (float)nCapIndex; //��ʼ��ʱֻΪmap���롰��˫�桱��ֵ������

	float value = m_pUI->GetCapValueFloat(UDSCAP_MULTISTREAM_VALUE);
	m_basemap[UDSCAP_MULTISTREAM_VALUE] = value; // ��ʼ��ʱ���UDSCAP_MULTISTREAM_VALUE����֤SetCapValue()����¸�Cap��ֵ
}


BOOL CPage_Base::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��	
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
	InitComboPixType(); //��ʼ��ͼ������������ֵ��Ӧ�����ȵ�ֵ�Ƿ����,����SetBinarization��

	m_pAdPage->InitAdvancedmap(); //��ʼ���߼������Map
	
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CPage_Base::PreView()
{
	UpdateData(TRUE);

	SetCapValue();
	m_pAdPage->SetCapValue();
	m_pPaperPage->SetCapValue();
	m_pUI->TW_SaveProfileToFile("�ϴ�ʹ��ģ��");

	BYTE *data = NULL; //ͼ������
	data = m_pUI->PreView();  

	if(data != NULL)
	{
		// ����ͼƬ
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
				//д���ļ�
				file.Write((LPSTR)&(m_pUI->m_bmpFileHeader), sizeof(BITMAPFILEHEADER)); // д�ļ�ͷ
				file.Write((LPSTR)&(m_pUI->m_bmpInfoHeader), sizeof(BITMAPINFOHEADER)); // д��Ϣͷ
				if (m_pUI->m_nBpp < 16)
				{			
					DWORD dwColors = 0;
					if (true == m_pUI->GetColorsUsed(m_pUI->m_nBpp, dwColors))
					{
						file.Write((LPSTR)(m_pUI->m_bmpLpRGB),sizeof(RGBQUAD) * dwColors); // д��ɫ��
					}	

					if (m_pUI->m_bmpLpRGB)
					{
						delete []m_pUI->m_bmpLpRGB;
						m_pUI->m_bmpLpRGB = NULL;
					}
				}
				file.Write(data, m_pUI->m_nDIBSize); // д����
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

		//��ʾͼƬ
		IplImage* img = cvLoadImage((CT2CA)bmpFilePath, 1);
		CWnd *pWnd = GetDlgItem(IDC_BASE_PREPICTURE); 
		CDC* pDC = pWnd->GetDC();
		HDC hDC = pDC->GetSafeHdc();
		CRect rect;
		pWnd->GetClientRect(&rect);
		SetRect(rect, rect.left, rect.top, rect.right, rect.bottom);
		
		//��������������ӣ�ʹͼ����ʾ��ʧ��
		CRect newRect;
		int width = img->width;
		int height = img->height;

		if(width <= rect.Width() && height <= rect.Height())//СͼƬ��������
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

//DrawToHdcϵ�к���
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
	m_slider_contrast.SetTicFreq((int)fStep);  // ���û������̶ȵ�Ƶ��Ϊ1����λ������Ҫ����������们���ʼλ�ò���

	m_pUI->GetCapRangeFloat(ICAP_BRIGHTNESS, fMin, fMax, fStep);
	m_slider_brightness.SetRange((int)fMin, (int)fMax);
	m_slider_brightness.SetTicFreq((int)fStep);

	m_pUI->GetCapRangeFloat(ICAP_THRESHOLD, fMin, fMax, fStep);
	m_slider_threshold.SetRange((int)fMin, (int)fMax);
	m_slider_threshold.SetTicFreq((int)fStep);

	UpdateData(FALSE);  // ���¿ؼ���ˢ�µ�ǰ�Ի���
}


////////////////////////////////////////////////////////////////////////////////
// Slider�ؼ����ݱ䶯
void CPage_Base::OnNMCustomdrawBase_Slider_Contrast(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);  // ��������
	CString str;
	int sldValue = m_slider_contrast.GetPos();  // ��ȡ���鵱ǰλ��
	m_basemap[ICAP_CONTRAST] = float(sldValue);

	str.Format("%d", sldValue);
	SetDlgItemText(IDC_BASE_EDIT_CONTRAST, str);

	//contrast = sldValue;
	//m_pAdPage->UpdateControls();

	UpdateData(FALSE);  // ���¿ؼ�

	*pResult = 0;
}


void CPage_Base::OnNMCustomdrawBase_Slider_Brightness(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);  // ��������
	CString str;
	int sldValue = m_slider_brightness.GetPos();  // ��ȡ���鵱ǰλ��
	m_basemap[ICAP_BRIGHTNESS] = float(sldValue);

	str.Format("%d", sldValue);
	SetDlgItemText(IDC_BASE_EDIT_BRIGHTNESS,str);

	//brightness = sldValue;
	//m_pAdPage->UpdateControls();

	UpdateData(FALSE);  // ���¿ؼ�

	*pResult = 0;
}


void CPage_Base::OnNMCustomdrawBase_Slider_Threshold(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������	
	UpdateData(TRUE); //��������
	CString str;
	int sldValue = m_slider_threshold.GetPos(); //��ȡ����ĵ�ǰλ��
	m_basemap[ICAP_THRESHOLD] = (float)sldValue;

	str.Format("%d", sldValue);
	SetDlgItemText(IDC_BASE_EDIT_THRESHOLD, str);
	UpdateData(FALSE); //���¿ؼ���

	*pResult = 0;
}


////////////////////////////////////////////////////////////////////////////////
// Edit�ؼ����ݱ䶯
void CPage_Base::OnEnChangeBase_Edit_Contrast()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CPropertyPage::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);  // ��������
	CString str;
	m_edit_contrast.GetWindowText(str);
	int nval = _ttoi(str);
	m_slider_contrast.SetPos(nval);
	m_basemap[ICAP_CONTRAST] = (float)nval;

	m_edit_contrast.SetSel(str.GetLength(), str.GetLength(),TRUE);  // ���ñ༭��ؼ���Χ
	
	//contrast = nval;
	//m_pAdPage->UpdateControls(); //����һ�θ߼����棬ͬ���ԱȶȲ���

	UpdateData(FALSE);  // ���¿ؼ�
}


void CPage_Base::OnEnChangeBase_Edit_Brightness()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CPropertyPage::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);  // ��������
	CString str;
	m_edit_brightness.GetWindowText(str);
	int nval = _ttoi(str);
	m_slider_brightness.SetPos(nval);
	m_basemap[ICAP_BRIGHTNESS] = (float)nval;
	m_edit_brightness.SetSel(str.GetLength(), str.GetLength(),TRUE);  // ���ñ༭��ؼ���Χ

	//brightness = nval;
	//m_pAdPage->UpdateControls(); //����һ�θ߼����棬ͬ���ԱȶȲ���

	UpdateData(FALSE);  // ���¿ؼ�
}


void CPage_Base::OnEnChangeBase_Edit_Threshold()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CPropertyPage::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
	UpdateData(TRUE); //��������
	CString str;
	m_edit_threshold.GetWindowText(str);
	int nval = _ttoi(str);
	m_slider_threshold.SetPos(nval); //_ttoi��CString����ת��Ϊint
	m_basemap[ICAP_THRESHOLD] = (float)nval;

	m_edit_threshold.SetSel(str.GetLength(), str.GetLength(), TRUE); //���ñ༭��ؼ���Χ

	UpdateData(FALSE); //���¿ؼ�
}

void CPage_Base::SetFlat(void)
{
	if(1 == m_radiobtn_scanmode)//Ϊ1��ʾƽ��
	{
		m_basemap[CAP_DUPLEXENABLED] = 0.0f;
		m_radiobtn_duplex = 0; //ƽ��ʱ��ֻ���ǵ���
	}	
	else //Ϊ0��ʾ�Զ���ֽ��ѡ��
	{}
	SetMultistream();
}


void CPage_Base::InitComboPixType(void)
{
	int nIndex = m_combo_colormode.GetCurSel();

	if(0 == nIndex) //�ڰ�
	{
		m_slider_contrast.EnableWindow(FALSE);  //��ָ���Ϊ�����������ʼ��������
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

	BaseStatus(); //��ȡbase����ͼ��ģʽ,�����߼����档
}

void CPage_Base::OnCbnSelchangeBase_Combo_Colormode()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nIndex = m_combo_colormode.GetCurSel();
	m_basemap[ICAP_PIXELTYPE] = (float)nIndex;
	m_combo_colormode.SetCurSel(nIndex);

	InitComboPixType(); //zhu �ж����ȵ��Ƿ����
	return;
}


void CPage_Base::OnCbnSelchangeBase_Combo_Resolution()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nIndex = m_combo_resolution.GetCurSel();
	CString strCBText; 
	m_combo_resolution.GetLBText(nIndex, strCBText);
	int nval = _ttoi(strCBText);  // CString ת int

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
		OFN_ALLOWMULTISELECT ;  // �ļ���Ŀ¼������ڣ�ʹ��Explorer����û����棬�б�������ѡ 
	ofn.lStructSize = sizeof(ofn);
	ofn.lpstrFile = szOpenFileNames;
	ofn.nMaxFile = sizeof(szOpenFileNames);
	ofn.lpstrFile[0] = '\0';
	ofn.lpstrFilter = TEXT("�����ļ�\0*.*\0\0");
	ofn.lpstrTitle = TEXT("��ѡ��ͼƬ�ļ�");  // ʹ��ϵͳĬ�ϱ������ռ���  
	if( GetOpenFileName( &ofn ) )
	{  
		/* 
		* �ѵ�һ���ļ���ǰ�ĸ��Ƶ�szPath,��:
		* ���ֻѡ��һ���ļ�,�͸��Ƶ����һ��'/'
		* ���ѡ�˶���ļ�,�͸��Ƶ���һ��NULL�ַ�
		*/
		lstrcpyn(szPath, szOpenFileNames, ofn.nFileOffset );
		/*
		* ��ֻѡ��һ���ļ�ʱ,�������NULL�ַ��Ǳ����.
		* ���ﲻ����Դ�ѡ��һ���Ͷ���ļ������
		*/
		szPath[ ofn.nFileOffset ] = '\0';
		nLen = lstrlen(szPath);

		/* ���ѡ�˶���ļ�,��������'\\' */
		if( szPath[nLen-1] != '\\' )  
		{
			lstrcat(szPath, TEXT("\\"));
		}

		p = szOpenFileNames + ofn.nFileOffset; // ��ָ���Ƶ���һ���ļ�

		/*ZeroMemory(szFileName, sizeof(szFileName));
		while( *p )
		{   
			lstrcat(szFileName, szPath);  ///< ���ļ�������·��  
			lstrcat(szFileName, p);    ///< �����ļ���  
			lstrcat(szFileName, TEXT("\n")); ///< ����   
			p += lstrlen(p) +1;     ///< ������һ���ļ�
		}
		::MessageBox(NULL, szFileName, TEXT("MultiSelect"), MB_OK);*/

		while (*p)
		{
			TCHAR szTemp[MAX_PATH] = {0};
			lstrcat(szTemp, szPath);  // ���ļ�������·��  
			lstrcat(szTemp, p);    // �����ļ���  
			vectorFileName.push_back(szTemp);
			p += lstrlen(p) +1;    // ������һ���ļ�
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
	// TODO: �ڴ����ר�ô����/����û���
	//��ȡ�ؼ�����ָ��  
	CEdit* pEdit1 = (CEdit*)GetDlgItem(IDC_BASE_EDIT_BRIGHTNESS);  
	CEdit* pEdit2 = (CEdit*)GetDlgItem(IDC_BASE_EDIT_CONTRAST);  

	CString str1, str2;   
	GetDlgItemText(IDC_BASE_EDIT_BRIGHTNESS, str1); // ��ȡedit���ı�  
	GetDlgItemText(IDC_BASE_EDIT_CONTRAST, str2);

	if( (GetFocus() == pEdit1 ||GetFocus() == pEdit2) && (pMsg->message == WM_CHAR))  
	{  
		//������������//��С���㡰.��
		if((pMsg->wParam >= '0' && pMsg->wParam <= '9'))   
		{  
			return 0;  
		} 
		else if(pMsg->wParam == '.')
		{
			return 1; //��׼����С����
		}
		//��֤����'-'ֻ�ܳ���һ��,����ֻ�ܳ����ڵ�һ���ַ�
		else if (pMsg->wParam == '-') //���ȡ��Աȶ�ֻ�����븺��������
		{
			if(str1.IsEmpty() || str2.IsEmpty())
			{
				return 0; //��һλʱ��������
			}
			else 
			{
				return 1;
			}
		}
		//����Backspace��delete�� 
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
//	// TODO: �ڴ���ӿؼ�֪ͨ����������
//	UpdateData(TRUE); //��radio��״ֵ̬���¸������ı���
//	int index;
//	SetFlat();
//	if(1 == m_radiobtn_scanmode)
//	{
//		index = 0;
//		GetDlgItem(IDC_BASE_RADIO_DUPLEX_SHUANG)->EnableWindow(FALSE); 
//		GetDlgItem(IDC_BASE_RADIO_DUPLEX_MUILTSTREAM)->EnableWindow(FALSE);
//	}
//	else //Ϊ0��ʾ�Զ���ֽ��ѡ��
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE); //��radio��״ֵ̬���¸������ı���
	scanside = m_radiobtn_duplex; 
	switch(m_radiobtn_duplex)
	{
	case 0:
	case 1:
		m_basemap[CAP_DUPLEXENABLED] = (float)m_radiobtn_duplex;

		m_pUI->SetCapValueInt(UDSCAP_MULTISTREAM,FALSE);
		m_basemap[UDSCAP_MULTISTREAM] = 0.0f;

		m_slider_brightness.EnableWindow(TRUE); //����
		m_slider_contrast.EnableWindow(TRUE); //�Աȶ�
		m_edit_brightness.EnableWindow(TRUE); //����Edit
		m_edit_contrast.EnableWindow(TRUE); //�Աȶ�Edit

		break;
	case 2:
		m_basemap[UDSCAP_MULTISTREAM] = 1.0f;
		break;
	}
	SetFlat();//�ں�SetMultiStream();

	UpdateData(FALSE);
}


void CPage_Base::SetMultistream(void)
{
	if(m_radiobtn_duplex == 2) //����ѡ��
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
		//GetDlgItem(IDC_ADVANCED_COMBO_SPLITIMG)->EnableWindow(FALSE); //ͼ���ֲ�����

		//((CButton*)GetDlgItem(IDC_CHECK_AUTOCROP))->SetCheck(FALSE); //�Զ�������У����ѡ��
		//((CButton*)GetDlgItem(IDC_CHECK_REMOVEPUNCH))->SetCheck(FALSE); //ȥ�����ײ�ѡ��

		//m_pUI->SetCapValueInt(UDSCAP_PUNCHHOLEREMOVEL,TWRP_DISABLE);
		//m_pUI->SetCapValueInt(UDSCAP_AUTOCROP,TWAC_DISABLE);
		//GetDlgItem(IDC_CHECK_AUTOCROP)->EnableWindow(FALSE); //�Զ�������У��������
		//GetDlgItem(IDC_CHECK_REMOVEPUNCH)->EnableWindow(FALSE);//ȥ�����ײ�����
	} 
	else 
	{
		//�������δѡ��ʱ������ѡ��Ҳ����Ҫѡ��
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
		//GetDlgItem(IDC_ADVANCED_COMBO_SPLITIMG)->EnableWindow(TRUE); //ͼ���ֿ���
		//GetDlgItem(IDC_CHECK_AUTOCROP)->EnableWindow(TRUE); //�Զ�������У������
		//GetDlgItem(IDC_CHECK_REMOVEPUNCH)->EnableWindow(TRUE); //ȥ�����׿���			
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

		//ͼ����ǿϵ�в�����
		/*GetDlgItem(IDC_CHECK_REMOVEDEMOISE)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_REMOVEDESCREEN)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_SHARPEN)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_REMOVEBACK)->EnableWindow(FALSE);*/
	} 
	else
	{
		m_slider_threshold.EnableWindow(FALSE); 
		m_edit_threshold.EnableWindow(FALSE); 
		//ͼ����ǿϵ�п���
		/*GetDlgItem(IDC_CHECK_REMOVEDEMOISE)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_REMOVEDESCREEN)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_SHARPEN)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_REMOVEBACK)->EnableWindow(TRUE);*/
	}
	SetBinarization(); //���á�ȥ���ߵ㡱���ǡ���ֵ��
}


void CPage_Base::OnBase_Btn_Check_FrontColor()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ����ר�ô����/����û���
	m_pUI->PreViewStatus();
	return __super::OnSetActive();
}


void CPage_Base::OnCbnSelchangeBase_Combo_Binarization()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nIndex = m_combo_binarization.GetCurSel();
	CString strCBText; 
	m_combo_binarization.GetLBText( nIndex, strCBText);
	int nval;
	if (strCBText.Find("��̬��ֵ") >= 0)
	{
		nval = TWBZ_DYNATHRESHOLD;
	}
	else if(strCBText.Find("�̶���ֵ") >= 0)
	{
		nval = TWBZ_FIXEDTHRESHOLD; 
	}
	else if(strCBText.Find("��ɫ��1") >= 0)
	{
		nval = TWBZ_HALFTONE1; 
	}
	else if(strCBText.Find("��ɫ��2") >= 0)
	{
		nval = TWBZ_HALFTONE2; 
	}
	else if(strCBText.Find("��ɫ��3") >= 0)
	{
		nval = TWBZ_HALFTONE3; 
	}
	else if(strCBText.Find("��ɫ��4") >= 0)
	{
		nval = TWBZ_HALFTONE4; 
	}
	else if(strCBText.Find("��ɫ��5") >= 0)
	{
		nval = TWBZ_HALFTONE5; 
	}
	else if(strCBText.Find("�����ɢ") >= 0)
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
		if (strCBText.Find("��̬��ֵ") >= 0)
		{
			SetDlgItemText(IDC_BASE_STATIC_THRESHOLD, TEXT("ȥ���ߵ�:"));
			m_pUI->GetCapRangeFloat(UDSCAP_REMOVESPOTS, fMin, fMax, fStep);
			m_slider_threshold.SetRange((int)fMin, (int)fMax);
			m_slider_threshold.SetTicFreq((int)fStep); 

			nCapValue = (int)(m_pUI->GetCapValueFloat(UDSCAP_REMOVESPOTS));
			m_slider_threshold.SetPos(nCapValue);

			//���ô�ʱ���Ȳ�����
			m_slider_brightness.EnableWindow(FALSE);
			m_edit_brightness.EnableWindow(FALSE); 

		} 
		else if(strCBText.Find("�̶���ֵ") >= 0)
		{
			SetDlgItemText(IDC_BASE_STATIC_THRESHOLD, TEXT("��ֵ:"));
			m_pUI->GetCapRangeFloat(ICAP_THRESHOLD, fMin, fMax, fStep);
			m_slider_threshold.SetRange((int)fMin, (int)fMax);
			m_slider_threshold.SetTicFreq((int)fStep); 

			nCapValue = (int)(m_pUI->GetCapValueFloat(ICAP_THRESHOLD));
			m_slider_threshold.SetPos(nCapValue);

			//���ô�ʱ���ȿ���
			m_slider_brightness.EnableWindow(TRUE);
			m_edit_brightness.EnableWindow(TRUE);
		}
		else if(strCBText.Find("��ɫ��") >= 0 || strCBText.Find("�����ɢ") >= 0)
		{
			m_slider_threshold.EnableWindow(FALSE);
			m_edit_threshold.EnableWindow(FALSE);

			//���ô�ʱ���ȿ���
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE); //��radio��״ֵ̬���¸������ı���
	
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE); //��radio��״ֵ̬���¸������ı���

	m_radiobtn_scanmode = 1;
	int index = 0;	
	SetFlat();

	GetDlgItem(IDC_BASE_RADIO_DUPLEX_SHUANG)->EnableWindow(FALSE); 
	GetDlgItem(IDC_BASE_RADIO_DUPLEX_MUILTSTREAM)->EnableWindow(FALSE);

	m_basemap[CAP_FEEDERENABLED] = (float)index;
	UpdateData(FALSE);
}

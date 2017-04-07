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
	remove(m_bmpFilePath); //ɾ����ʱԤ��ͼƬ

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


// CPage_Base ��Ϣ�������

void CPage_Base::SetCapValue(void)
{
	MAP_CAP::iterator iter; 
	for(iter = m_basemap.begin(); iter != m_basemap.end(); iter++)
	{
		switch(iter->first)
		{
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

	//������δѡ�ж��� ѡ��ֵ��Ϊ0��,Ȼ���˳�
	if( 0 == m_radiobtn_duplex) {
		m_pUI->SetCapValueFloat(UDSCAP_MULTISTREAM_VALUE, 0.0f); // �������ѡ��ֵ
	}
	else
	{
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

		m_pUI->SetCapValueFloat(UDSCAP_MULTISTREAM_VALUE, (float)temp); // �������ѡ��ֵ
		m_pUI->SetCapValueInt(UDSCAP_DOCS_IN_ADF,doc_count); // ����ֽ����
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

	//ȥ������-������Ŀ
	fCapValue = m_pUI->GetCapValueFloat(UDSCAP_NOISENUM);
	strText.Format("%d",(int)fCapValue);
	SetDlgItemText(IDC_BASE_EDIT_NOISENUM,strText);

	//ȥ������-������Χ
	fCapValue = m_pUI->GetCapValueFloat(UDSCAP_NOISERANGE);
	strText.Format("%d",(int)fCapValue);
	SetDlgItemText(IDC_BASE_EDIT_NOISERANGE,strText);

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
	InitComboPixType();
	
	//2.0�汾
	//ͼ������-ͼ����ת
	m_combo_rotate.ResetContent(); //�������
	nCapIndex = m_pUI->GetCurrentCapIndex(ICAP_ROTATION);
	lstCapValuesFlt = m_pUI->GetValidCapFloat(ICAP_ROTATION);
	for(unsigned int i=0; i<lstCapValuesFlt->size();i++)
	{
		switch((int)lstCapValuesFlt->at(i))
		{
		case TWOR_ROT0:
			m_combo_rotate.InsertString(i,"����תͼ��");
			break;
		case TWOR_ROT90:
			m_combo_rotate.InsertString(i,"˳ʱ��90��"); 
			break;
		case TWOR_ROT180:
			m_combo_rotate.InsertString(i,"˳ʱ��180��");
			break;
		case TWOR_ROT270:
			m_combo_rotate.InsertString(i,"˳ʱ��270��");
			break;
		default:
			continue;
		}
	}
	m_combo_rotate.SetCurSel(nCapIndex);

	//ͼ������-ͼ��ָ�
	m_combo_splitimage.ResetContent(); //�������
	nCapIndex = m_pUI->GetCurrentCapIndex(UDSCAP_SPLITIMAGE);
	lstCapValues = m_pUI->GetValidCap(UDSCAP_SPLITIMAGE);
	for(unsigned int i=0; i<lstCapValues->size();i++)
	{
		switch(lstCapValues->at(i))
		{
		case TWSI_NONE:
			m_combo_splitimage.InsertString(i,"�����"); //���ָ�ͼ��
			break;
		case TWSI_HORIZONTAL:
			m_combo_splitimage.InsertString(i,"����");  //ˮƽ�ָ�ͼ��
			break;
		case TWSI_VERTICAL:
			m_combo_splitimage.InsertString(i,"����"); //��ֱ�ָ�ͼ��
			break;
		case TWSI_DEFINED:
			m_combo_splitimage.InsertString(i,"�Զ���"); //��ֱ�ָ�ͼ��
			break;
		default:
			continue;
		}
	}
	m_combo_splitimage.SetCurSel(nCapIndex);

	//GammaУ�� 
	nCapValue = (int)(m_pUI->GetCapValueFloat(ICAP_GAMMA)); //GetCapValueFloat�ܷ�õ�CTWAINContainerFix32����	
	m_slider_gamma.SetPos(nCapValue);
	float valueTemp = ((float)nCapValue)/100;
	strText.Format("%.2f", valueTemp);
	SetDlgItemText(IDC_BASE_EDIT_GAMMA, strText);

	//ȥ���հ�ҳcheckbox
	nCapValue = (int)(m_pUI->GetCapValueBool(UDSCAP_REMOVEBLANK));
	m_check_removeblank.SetCheck(nCapValue);
	//ȥ���հ�ҳ -1�Զ�;-2������:��Ϊ������ 
	nCapValue = (int)(m_pUI->GetCapValueFloat(ICAP_AUTODISCARDBLANKPAGES)); 
	m_slider_removeblank.SetPos(nCapValue);
	strText.Format("%d",nCapValue);
	SetDlgItemText(IDC_BASE_EDIT_REMOVEBLANK, strText);
	SetBlank();

	//int��radio����Ҫ������󣬷���SetDlgItemTextˢ��ʱ�����������á�
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
	}
	SetMultistream();
	SetDenoise();
}


void CPage_Base::InitBasemap(void)
{
	m_basemap.erase(m_basemap.begin(),m_basemap.end());//���

	int nCapIndex = m_pUI->GetCurrentCapIndex(CAP_DUPLEXENABLED);
	m_basemap[CAP_DUPLEXENABLED] = (float)nCapIndex; //��ʼ��ʱֻΪmap���롰��˫�桱��ֵ������

	int nCapValue = (int)(m_pUI->GetCapValueBool(UDSCAP_MULTISTREAM));
	m_basemap[UDSCAP_MULTISTREAM_VALUE] = (float)nCapValue;

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
	
	InitComboPixType(); //��ʼ��ͼ������������ֵ��Ӧ�����ȵ�ֵ�Ƿ����,����SetBinarization��
	
	if(g_nDeviceNumber != 1)//��Ϊ����ɨ���ǣ����ö���
	{
		GetDlgItem(IDC_BASE_RADIO_DUPLEX_MUILTSTREAM)->EnableWindow(FALSE); //���ö���
	}

	GetDlgItem(IDC_BASE_RADIO_SCANMODE_Flatbed)->ShowWindow(FALSE); //��ʱ����ƽ�塣
	 
	//2.0�汾�����޸�
	//����TAB�ؼ�  
	//m_base_tab.InsertItem(0, _T("�Զ���ɫ"));  //�����һ����ǩ     
	m_base_tab.InsertItem(0, _T("��ɫ"));  
	m_base_tab.InsertItem(1, _T("�Ҷ�"));     
	m_base_tab.InsertItem(2, _T("�ڰ�"));
	//m_pTabAutoColor->Create(IDD_BASETAB_AUTOCOLOR, &m_base_tab); //������һ����ǩҳ     
	m_pTabColor->Create(IDD_BASETAB_COLOR, &m_base_tab); 
	m_pTabGray->Create(IDD_BASETAB_GRAY, &m_base_tab);    
	m_pTabBW->Create(IDD_BASETAB_BW, &m_base_tab);    

	m_base_tab.GetClientRect(&m_tabRect);    // ��ȡ��ǩ�ؼ��ͻ���Rect     
	// ����tabRect��ʹ�串�Ƿ�Χ�ʺϷ��ñ�ǩҳ     
	m_tabRect.left += 1;                    
	m_tabRect.right -= 1;     
	m_tabRect.top += 25;     
	m_tabRect.bottom -= 1;   
	//���ݵ����õ�tabRect�������ӶԻ��򣬲�����Ϊ��ʾ 
	//m_pTabAutoColor->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_SHOWWINDOW);     
	//���ݵ����õ�tabRect���������ӶԻ��򣬲�����Ϊ����     
	m_pTabColor->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_SHOWWINDOW);  
	m_pTabGray->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_HIDEWINDOW);  
	m_pTabBW->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_HIDEWINDOW);  
	SetTabCtrl();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CPage_Base::InitSliderCtrl()
{
	float fMin,fMax,fStep;
	m_pUI->GetCapRangeFloat(ICAP_GAMMA, fMin, fMax, fStep);
	m_slider_gamma.SetRange((int)fMin, (int)fMax);
	m_slider_gamma.SetTicFreq((int)fStep); //����

	m_pUI->GetCapRangeFloat(ICAP_AUTODISCARDBLANKPAGES, fMin, fMax, fStep);
	m_slider_removeblank.SetRange((int)fMin, (int)fMax);
	m_slider_removeblank.SetTicFreq((int)fStep); //����

	UpdateData(FALSE);  // ���¿ؼ���ˢ�µ�ǰ�Ի���
}


void CPage_Base::SetFlat(void)
{
	if(1 == m_radiobtn_scanmode)//Ϊ1��ʾƽ��
	{
		m_basemap[CAP_DUPLEXENABLED] = 0.0f;
		m_radiobtn_duplex = 0; //ƽ��ʱ��ֻ���ǵ���
	}	
	else //Ϊ0��ʾ�Զ���ֽ��ѡ��
	{
		m_radiobtn_scanmode = 0;
	}
	SetMultistream();
}


void CPage_Base::InitComboPixType(void)
{
	int nIndex = m_combo_colormode.GetCurSel();
	
	if(0 == nIndex) //�ڰ�
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nIndex = m_combo_colormode.GetCurSel();

	if(GetDlgItem(IDC_BASE_COMBO_COLORMODE)->IsWindowEnabled())//ͼ�����Ϳ���ʱ������
	{
		m_pUI->SetCapValueInt(ICAP_PIXELTYPE,nIndex);
	}	

	m_combo_colormode.SetCurSel(nIndex);

	InitComboPixType(); //zhu �ж����ȵ��Ƿ����
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
	CEdit* pEdit1 = (CEdit*)GetDlgItem(IDC_BASE_EDIT_REMOVEBLANK);  
	CString str1;   
	GetDlgItemText(IDC_BASE_EDIT_REMOVEBLANK, str1); // ��ȡedit���ı�  

	if( (GetFocus() == pEdit1) && (pMsg->message == WM_CHAR))  
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

	//��Ҫ����С�����Editֻ����һ��С����
	CEdit* pEdit2 = (CEdit*)GetDlgItem(IDC_BASE_EDIT_GAMMA); 
	CString str2;   
	GetDlgItemText(IDC_BASE_EDIT_GAMMA, str2); // ��ȡedit���ı�  
	if( (GetFocus() == pEdit2) && (pMsg->message == WM_CHAR))  
	{  
		//�����������ֺ�С���㡰.��
		if(pMsg->wParam == '.')
		{
			//�����ֻ��������һ��С����
			int nPos3 = 0; 
			nPos3 = str2.Find('.'); // ����.��λ�� 

			if(nPos3 >= 0)  //����ֿ�д����||�����Ļ��ܻ�����
			{  
				return 1;   //�������,����,��������������
			}	
			return 0;
		}
		//����Backspace��delete�� 
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
	//��ȡ�ؼ�����ָ��  
	CEdit* pEdit1 = (CEdit*)GetDlgItem(IDC_BASE_EDIT_BRIGHTNESS);  
	CEdit* pEdit2 = (CEdit*)GetDlgItem(IDC_BASE_EDIT_CONTRAST);  
	CEdit* pEdit3 = (CEdit*)GetDlgItem(IDC_BASE_EDIT_THRESHOLD);
	
	CString str1, str2, str3;   
	GetDlgItemText(IDC_BASE_EDIT_BRIGHTNESS, str1); // ��ȡedit���ı�  
	GetDlgItemText(IDC_BASE_EDIT_CONTRAST, str2);
	GetDlgItemText(IDC_BASE_EDIT_THRESHOLD, str3);

	if( (GetFocus() == pEdit1 || GetFocus() == pEdit2 || GetFocus() == pEdit3) 
				&& (pMsg->message == WM_CHAR))  
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
		//����Backspace��delete�� 
		else if(pMsg->wParam == 0x08 || pMsg->wParam == 0x2E)  
		{
			  // ���ñ༭��ؼ���Χ
			return 0;  
		}  
		else
		{
			return 1;
		}
	}

	//�Աȶȡ����������븺��
	if( (GetFocus() == pEdit1 || GetFocus() == pEdit2) 
		&& (pMsg->message == WM_CHAR))
	{
		//��֤����'-'ֻ�ܳ���һ��,����ֻ�ܳ����ڵ�һ���ַ�
		if(pMsg->wParam == '-') //���ȡ��Աȶ�ֻ�����븺��������
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
	}
	*/
	return __super::PreTranslateMessage(pMsg);
}

void CPage_Base::OnBase_RadioBtn_Duplex()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE); //��radio��״ֵ̬���¸������ı���
	switch(m_radiobtn_duplex)
	{
	case 0:
	case 1:
		m_pUI->SetCapValueInt(CAP_DUPLEXENABLED,m_radiobtn_duplex); 
		if(1 == m_radiobtn_duplex) //˫�棬�����ֵΪ0
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
		m_pUI->SetCapValueInt(UDSCAP_MULTISTREAM, TRUE);//ֱ��������Ч
		
		m_check_frontcolor.SetCheck(TRUE);
		m_base_tab.SetCurSel(0);
		break;
	}
	SetFlat();//�ں�SetMultiStream();
	SetDenoise();
	SetCapMulti();
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ����ר�ô����/����û���
	m_pUI->PreViewStatus();
	UpdateControls();
	
	Invalidate();

	SetSpiltimage();
	return __super::OnSetActive();
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

	//m_basemap[CAP_FEEDERENABLED] = (float)index;
	if(m_radiobtn_scanmode==0)//�Զ���ֽ��
	{
		m_pUI->SetCapValueInt(CAP_FEEDERENABLED,TRUE); 
	}
	else //ƽ��
	{
		m_pUI->SetCapValueInt(CAP_FEEDERENABLED,FALSE); 
	}

	UpdateData(FALSE);
}


void CPage_Base::OnTcnSelchangeBase_Tab(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nIndex = m_combo_rotate.GetCurSel();
	CString strCBText; 
	m_combo_rotate.GetLBText( nIndex, strCBText);
	int nval;
	if (strCBText.Find("����תͼ��") >= 0)
	{
		nval = TWOR_ROT0;
	}
	else if(strCBText.Find("˳ʱ��90��") >= 0)
	{
		nval = TWOR_ROT90; 
	}
	else if(strCBText.Find("˳ʱ��180��") >= 0)
	{
		nval = TWOR_ROT180; 
	}
	else if(strCBText.Find("˳ʱ��270��") >= 0)
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	int nIndex = m_combo_splitimage.GetCurSel();
	CString strCBText; 
	m_combo_splitimage.GetLBText( nIndex, strCBText);
	int nval;
	if (strCBText.Find("�����") >= 0) //���ָ�ͼ��
	{
		nval = TWSI_NONE;	
	}
	else if(strCBText.Find("����") >= 0) //ˮƽ�ָ�ͼ��
	{
		nval = TWSI_HORIZONTAL; 
	}
	else if(strCBText.Find("����") >= 0) //��ֱ�ָ�ͼ��
	{
		nval = TWSI_VERTICAL; 
	}
	else if(strCBText.Find("�Զ���") >= 0) //��ֱ�ָ�ͼ��
	{
		nval = TWSI_DEFINED; 
	}
	else
	{}
	if(m_combo_splitimage.IsWindowEnabled())
	{
		m_pUI->SetCapValueInt(UDSCAP_SPLITIMAGE,nval); //���ò�����Ч

		if(nval == TWSI_HORIZONTAL || nval == TWSI_VERTICAL)
		{
			m_pUI->SetCapValueInt(UDSCAP_DOCS_IN_ADF, 2); //����ɨ������Ϊ2
		}
		else if(nval == TWSI_NONE && m_radiobtn_duplex == 0) //�����&&����
		{
				m_pUI->SetCapValueInt(UDSCAP_DOCS_IN_ADF, 1); //�����ʱ�����1
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
		m_pUI->SetCapValueInt(UDSCAP_SPLITIMAGE, TWSI_NONE); //���ò�����Ч
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

	if(strCBText.Find("�����") >= 0)
	{
		//str.Format("%d",1);
		//SetDlgItemText(IDC_ADVANCED_EDIT_SPILT, str);
		//m_edit_spilt.EnableWindow(FALSE);

		//GetDlgItem(IDC_ADVANCED_RADIO_HORIZONTAL)->EnableWindow(FALSE);
		//GetDlgItem(IDC_ADVANCED_RADIO_VERTICAL)->EnableWindow(FALSE);
	}
	else if(strCBText.Find("����") >= 0 || strCBText.Find("����") >= 0) //ˮƽ����ֱ�ָ�ͼ��
	{
		//SetDlgItemText(IDC_ADVANCED_EDIT_SPILT, str);
		//m_edit_spilt.EnableWindow(FALSE);

		//GetDlgItem(IDC_ADVANCED_RADIO_HORIZONTAL)->EnableWindow(FALSE);
		//GetDlgItem(IDC_ADVANCED_RADIO_VERTICAL)->EnableWindow(FALSE);
	}
	else if(strCBText.Find("�Զ���") >= 0)
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);  // ��������
	CString str;
	int sldValue = m_slider_gamma.GetPos();  // ��ȡ���鵱ǰλ��
	str.Format("%d",sldValue);

	float value = ((int)sldValue*10)/10.00f;
	m_pUI->SetCapValueFloat(ICAP_GAMMA,value);

	float valueTemp = ((float)sldValue)/100;
	str.Format("%.2f", valueTemp); //С�����ֻҪ2λ
	SetDlgItemText(IDC_BASE_EDIT_GAMMA, str);// �ڱ༭��ͬ����ʾ������ֵ
	UpdateData(FALSE);  // ���¿ؼ�

	*pResult = 0;
}


void CPage_Base::OnEnChangeBase_Edit_Gamma()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д __super::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);  // ��������
	CString str;
	m_edit_gamma.GetWindowText(str);

	float fval = (float)(_ttof(str));
	int nval = (int)(100*fval);
	m_slider_gamma.SetPos(nval);
	m_pUI->SetCapValueFloat(ICAP_GAMMA, float(nval));

	m_edit_gamma.SetSel(str.GetLength(), str.GetLength(),TRUE);  // ���ñ༭��ؼ���Χ
	UpdateData(FALSE);  // ���¿ؼ�
}


void CPage_Base::OnNMCustomdrawBase_Slider_Removeblank(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);  // ��������
	CString str;
	int sldValue = m_slider_removeblank.GetPos();  // ��ȡ���鵱ǰλ��

	if(m_slider_removeblank.IsWindowEnabled())
	{
		m_pUI->SetCapValueFloat(ICAP_AUTODISCARDBLANKPAGES,(float)sldValue);
	}	

	str.Format("%d", sldValue);
	SetDlgItemText(IDC_BASE_EDIT_REMOVEBLANK, str);
	UpdateData(FALSE);  // ���¿ؼ�

	*pResult = 0;
}


void CPage_Base::OnEnChangeBase_Edit_Removeblank()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д __super::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);  // ��������
	CString str;
	m_edit_removeblank.GetWindowText(str);
	int nval = _ttoi(str);
	m_slider_removeblank.SetPos(nval);

	if(m_slider_removeblank.IsWindowEnabled())
	{
		m_pUI->SetCapValueFloat(ICAP_AUTODISCARDBLANKPAGES,(float)nval);
	}	

	m_edit_removeblank.SetSel(str.GetLength(), str.GetLength(),TRUE);  // ���ñ༭��ؼ���Χ
	UpdateData(FALSE);  // ���¿ؼ�
}


void CPage_Base::OnBase_Btn_Check_RemoveBlank()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SetDenoise();
	m_base_tab.SetCurSel(0);
	//m_pTabAutoColor->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_HIDEWINDOW);     
	m_pTabColor->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_SHOWWINDOW);  
	m_pTabGray->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_HIDEWINDOW);  
	m_pTabBW->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_HIDEWINDOW);  
}


void CPage_Base::OnBase_Btn_Backcolor()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SetDenoise();
	m_base_tab.SetCurSel(0);
	//m_pTabAutoColor->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_HIDEWINDOW);     
	m_pTabColor->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_SHOWWINDOW);  
	m_pTabGray->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_HIDEWINDOW);  
	m_pTabBW->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_HIDEWINDOW);  
}


void CPage_Base::OnBase_Btn_Frontgray()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SetDenoise();
	m_base_tab.SetCurSel(1);
	//m_pTabAutoColor->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_HIDEWINDOW);     
	m_pTabColor->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_HIDEWINDOW);  
	m_pTabGray->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_SHOWWINDOW);  
	m_pTabBW->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_HIDEWINDOW); 
}


void CPage_Base::OnBase_Btn_Backgray()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SetDenoise();
	m_base_tab.SetCurSel(1);
	//m_pTabAutoColor->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_HIDEWINDOW);     
	m_pTabColor->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_HIDEWINDOW);  
	m_pTabGray->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_SHOWWINDOW);  
	m_pTabBW->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_HIDEWINDOW); 
}


void CPage_Base::OnBase_Btn_Frontbw()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SetDenoise();
	m_base_tab.SetCurSel(2);
	//m_pTabAutoColor->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_HIDEWINDOW);     
	m_pTabColor->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_HIDEWINDOW);  
	m_pTabGray->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_HIDEWINDOW);  
	m_pTabBW->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_SHOWWINDOW); 
}


void CPage_Base::OnBase_Btn_Backbw()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SetDenoise();
	m_base_tab.SetCurSel(2);
	//m_pTabAutoColor->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_HIDEWINDOW);     
	m_pTabColor->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_HIDEWINDOW);  
	m_pTabGray->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_HIDEWINDOW);  
	m_pTabBW->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_SHOWWINDOW); 
}


void CPage_Base::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	int scrollpos;
	CString str;
	SetScroll();
	switch(pScrollBar->GetDlgCtrlID())
	{
		//ȥ��������Ŀ               
	case IDC_BASE_SCROLLBAR_NOISENUM:
		{
			scrollpos = m_scroll_noisenum.GetScrollPos();
			switch(nSBCode)
			{
			case SB_LINEUP: //������Ϲ���һ�У���pos��1
				scrollpos += 1;
				break;
			case SB_LINEDOWN: //������¹���һ�У���pos��1
				scrollpos -= 1; 
				break;
			case SB_TOP: //���
				scrollpos = 3600;
				break;
			case SB_BOTTOM:
				scrollpos = 0;
				break;
			}
			// ���ù�����λ��  
			m_scroll_noisenum.SetScrollPos(scrollpos);
			str.Format("%d", scrollpos);
			SetDlgItemText(IDC_BASE_EDIT_NOISENUM, str); 
		}
		break;
	//ȥ������-������Χ
	case IDC_BASE_SCROLLBAR_NOISERANGE:
		{
			scrollpos = m_scroll_noiserange.GetScrollPos();
			switch(nSBCode)
			{
			case SB_LINEUP: //������Ϲ���һ�У���pos��1
				scrollpos += 1;
				break;
			case SB_LINEDOWN: //������¹���һ�У���pos��1
				scrollpos -= 1; 
				break;
			case SB_TOP: //���
				scrollpos = 30;
				break;
			case SB_BOTTOM:
				scrollpos = 1;
				break;
			}
			// ���ù�����λ��  
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
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д __super::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);  // ��������
	SetScroll();

	CString str;
	m_edit_noisenum.GetWindowText(str);
	float fval = _ttof(str);
	m_scroll_noisenum.SetScrollPos((int)fval);

	m_pUI->SetCapValueFloat(UDSCAP_NOISENUM, fval); 
	m_edit_noisenum.SetSel(str.GetLength(), str.GetLength(),TRUE);  // ���ñ༭��ؼ���Χ

	UpdateData(FALSE);  // ���¿ؼ�
}


void CPage_Base::OnEnChangeBase_Edit_NoiseRange()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д __super::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);  // ��������
	SetScroll();

	CString str;
	m_edit_noiserange.GetWindowText(str);
	float fval = _ttof(str);
	m_scroll_noiserange.SetScrollPos((int)fval);

	m_pUI->SetCapValueFloat(UDSCAP_NOISERANGE, fval); 
	m_edit_noiserange.SetSel(str.GetLength(), str.GetLength(),TRUE);  // ���ñ༭��ؼ���Χ

	UpdateData(FALSE);  // ���¿ؼ�
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
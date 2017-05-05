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
	m_pTabPreview = new CBase_Tab_Preview(pUI);
	m_pTabRotateshow = new CBase_Tab_Rotateshow(pUI);
	m_pTabSpiltshow = new CBase_Tab_Spiltshow(pUI);
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

	if(m_pTabPreview)
	{
		delete m_pTabPreview;
		m_pTabPreview = NULL;
	}

	if(m_pTabRotateshow)
	{
		delete m_pTabRotateshow;
		m_pTabRotateshow = NULL;
	}

	if(m_pTabSpiltshow)
	{
		delete m_pTabSpiltshow;
		m_pTabSpiltshow = NULL;
	}

	if(m_mfcBtn)
	{
		delete m_mfcBtn;
		m_mfcBtn = NULL;
	}
}

void CPage_Base::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
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
	DDX_Control(pDX, IDC_BASE_TAB, m_base_tab);
	DDX_Control(pDX, IDC_BASE_COMBO_ROTATE, m_combo_rotate);
	DDX_Control(pDX, IDC_BASE_COMBO_SPLITIMG, m_combo_splitimage);
	DDX_Control(pDX, IDC_BASE_SLIDER_REMOVEBLANK, m_slider_removeblank);
	DDX_Control(pDX, IDC_BASE_EDIT_REMOVEBLANK, m_edit_removeblank);
	DDX_Control(pDX, IDC_BASE_CHECK_REMOVEBLANK, m_check_removeblank);
	DDX_Control(pDX, IDC_BASE_BUTTON_ROTATE_SHOW, m_btn_rotateshow);
	DDX_Control(pDX, IDC_BASE_BUTTON_SPILT_SHOW, m_btn_spiltshow);
	DDX_Control(pDX, IDC_BASE_BUTTON_AUTOFRONTCOLOR, m_btn_autofrontcolor);
	DDX_Control(pDX, IDC_BASE_BUTTON_AUTOBACKCOLOR, m_btn_autobackcolor);
	DDX_Control(pDX, IDC_CHECK_AUTOBACKCOLOR, m_check_autobackcolor);
	DDX_Control(pDX, IDC_CHECK_AUTOFRONTCOLOR, m_check_autofrontcolor);
	DDX_Control(pDX, IDC_BASE_COMBO_JOINIMG, m_combo_joinimage);
	DDX_Control(pDX, IDC_BASE_COMBO_BACKPROCESS, m_combo_backprocess);
	DDX_Control(pDX, IDC_BASE_COMBO_BACKKPRO_MODE, m_combo_backpromode);
	DDX_Control(pDX, IDC_BASE_EDIT_BACKKPRO_STRENGTH, m_edit_backprostrenth);
	DDX_Control(pDX, IDC_BASE_SLIDER_BACKKPRO_STRENGTH, m_slider_backprostrenth);
	DDX_Radio(pDX, IDC_BASE_RADIO_ROTATEMODE_FAST, m_radiobtn_rotatemode);
	DDX_Radio(pDX, IDC_BASE_RADIO_BACKROTATE_SAME, m_radiobtn_backimgrotate);
	DDX_Radio(pDX, IDC_BASE_RADIO_COLORFLIP_POSITIVE, m_radiobtn_colorflip);
	DDX_Control(pDX, IDC_BASE_CHECK_HOZIMIRROR, m_check_mirror);
}


BEGIN_MESSAGE_MAP(CPage_Base, CPropertyPage)
	ON_BN_CLICKED(IDC_CHECK_FRONTCOLOR, &CPage_Base::OnBase_Btn_Check_FrontColor)
	ON_BN_CLICKED(IDC_CHECK_FRONTGRAY, &CPage_Base::OnBase_Btn_Check_FrontGray)
	ON_BN_CLICKED(IDC_CHECK_FRONTBW, &CPage_Base::OnBase_Btn_Check_FrontBw)
	ON_BN_CLICKED(IDC_CHECK_BACKCOLOR, &CPage_Base::OnBase_Btn_Check_BackColor)
	ON_BN_CLICKED(IDC_CHECK_BACKGRAY, &CPage_Base::OnBase_Btn_Check_BackGray)
	ON_BN_CLICKED(IDC_CHECK_BACKBW, &CPage_Base::OnBase_Btn_Check_BackBw)
	ON_CBN_SELCHANGE(IDC_BASE_COMBO_ROTATE, &CPage_Base::OnCbnSelchangeBase_Combo_Rotate)
	ON_CBN_SELCHANGE(IDC_BASE_COMBO_SPLITIMG, &CPage_Base::OnCbnSelchangeBase_Combo_SpiltImage)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_BASE_SLIDER_REMOVEBLANK, &CPage_Base::OnNMCustomdrawBase_Slider_Removeblank)
	ON_EN_CHANGE(IDC_BASE_EDIT_REMOVEBLANK, &CPage_Base::OnEnChangeBase_Edit_Removeblank)
	ON_BN_CLICKED(IDC_BASE_CHECK_REMOVEBLANK, &CPage_Base::OnBase_Btn_Check_RemoveBlank)
	ON_BN_CLICKED(IDC_BASE_BUTTON_FRONTCOLOR, &CPage_Base::OnBase_Btn_FrontColor)
	ON_BN_CLICKED(IDC_BASE_BUTTON_BACKCOLOR, &CPage_Base::OnBase_Btn_Backcolor)
	ON_BN_CLICKED(IDC_BASE_BUTTON_FRONTGRAY, &CPage_Base::OnBase_Btn_Frontgray)
	ON_BN_CLICKED(IDC_BASE_BUTTON_BACKGRAY, &CPage_Base::OnBase_Btn_Backgray)
	ON_BN_CLICKED(IDC_BASE_BUTTON_FRONTBW, &CPage_Base::OnBase_Btn_Frontbw)
	ON_BN_CLICKED(IDC_BASE_BUTTON_BACKBW, &CPage_Base::OnBase_Btn_Backbw)
	ON_BN_CLICKED(IDC_BASE_BUTTON_ROTATE_SHOW, &CPage_Base::OnBase_Btn_RotateShow)
	ON_BN_CLICKED(IDC_BASE_BUTTON_SPILT_SHOW, &CPage_Base::OnBase_Btn_SpiltShow)
	ON_BN_CLICKED(IDC_BASE_BUTTON_AUTOFRONTCOLOR, &CPage_Base::OnBase_Btn_Autofrontcolor)
	ON_BN_CLICKED(IDC_BASE_BUTTON_AUTOBACKCOLOR, &CPage_Base::OnBase_Btn_Autobackcolor)
	ON_BN_CLICKED(IDC_CHECK_AUTOFRONTCOLOR, &CPage_Base::OnBase_Btn_Check_Autofrontcolor)
	ON_BN_CLICKED(IDC_CHECK_AUTOBACKCOLOR, &CPage_Base::OnBase_Btn_Check_Autobackcolor)
	ON_CBN_SELCHANGE(IDC_BASE_COMBO_JOINIMG, &CPage_Base::OnCbnSelchangeBase_Combo_Joinimg)
	ON_CBN_SELCHANGE(IDC_BASE_COMBO_BACKPROCESS, &CPage_Base::OnCbnSelchangeBase_Combo_Backprocess)
	ON_CBN_SELCHANGE(IDC_BASE_COMBO_BACKPRO_MODE, &CPage_Base::OnCbnSelchangeBase_Combo_BackproMode)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_BASE_SLIDER_BACKKPRO_STRENGTH, &CPage_Base::OnNMCustomdrawBase_Slider_BackkproStrength)
	ON_EN_CHANGE(IDC_BASE_EDIT_BACKKPRO_STRENGTH, &CPage_Base::OnEnChangeBase_Edit_BackkproStrength)
	ON_BN_CLICKED(IDC_BASE_RADIO_ROTATEMODE_FAST, &CPage_Base::OnBase_RadioBtn_Rotatemode_Fast)
	ON_BN_CLICKED(IDC_BASE_RADIO_ROTATEMODE_ALL, &CPage_Base::OnBase_RadioBtn_Rotatemode_Fast)
	ON_BN_CLICKED(IDC_BASE_RADIO_ROTATEMODE_SLOW, &CPage_Base::OnBase_RadioBtn_Rotatemode_Fast)
	ON_BN_CLICKED(IDC_BASE_RADIO_BACKROTATE_SAME, &CPage_Base::OnBase_RadioBtn_Backrotate_Same)
	ON_BN_CLICKED(IDC_BASE_RADIO_BACKROTATE_DIFF, &CPage_Base::OnBase_RadioBtn_Backrotate_Same)
	ON_BN_CLICKED(IDC_BASE_RADIO_COLORFLIP_POSITIVE, &CPage_Base::OnBase_RadioBtn_Colorflip_Positive)
	ON_BN_CLICKED(IDC_BASE_RADIO_COLORFLIP_NEGATIVE, &CPage_Base::OnBase_RadioBtn_Colorflip_Positive)
	ON_BN_CLICKED(IDC_BASE_CHECK_HOZIMIRROR, &CPage_Base::OnBase_Btn_Check_Hozimirror)
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
				m_pUI->SetCapValueInt(iter->first,TRUE);
				
				break;
			}	
		
		case UDSCAP_MULTISTREAM_VALUE: // �������ѡ��ֵ
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

void CPage_Base::UpdateControls(void)
{
	UpdateData(TRUE);  
	int nCapIndex;
	const IntVector* lstCapValues;
	const FloatVector* lstCapValuesFlt;
	int nCapValue;
	CString strText;
	float fCapValue;
	
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
		case TWOR_DEFINED:
			m_combo_rotate.InsertString(i,"�Զ���ת");
			break;
		default:
			continue;
		}
	}
	m_combo_rotate.SetCurSel(nCapIndex);
	SetRotate();

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

	//ȥ���հ�ҳcheckbox
	nCapValue = (int)(m_pUI->GetCapValueBool(UDSCAP_REMOVEBLANK));
	m_check_removeblank.SetCheck(nCapValue);
	//ȥ���հ�ҳ -1�Զ�;-2������:��Ϊ������ 
	nCapValue = (int)(m_pUI->GetCapValueFloat(ICAP_AUTODISCARDBLANKPAGES)); 
	m_slider_removeblank.SetPos(nCapValue);
	strText.Format("%d",nCapValue);
	SetDlgItemText(IDC_BASE_EDIT_REMOVEBLANK, strText);
	SetBlank();

	//�½�������
	//ͼ��ƴ��
	m_combo_joinimage.ResetContent();
	nCapIndex = m_pUI->GetCurrentCapIndex(UDSCAP_JOINIMAGE);
	lstCapValues = m_pUI->GetValidCap(UDSCAP_JOINIMAGE);
	for(unsigned int i=0; i<lstCapValues->size();i++)
	{
		switch(lstCapValues->at(i))
		{
		case TWJI_LEFTRIGHT:
			m_combo_joinimage.InsertString(i,"����ƴ��");
			break;
		case TWJI_UPDOWN:
			m_combo_joinimage.InsertString(i,"����ƴ��");
			break;
		case TWJI_UPDOWNBACKTURN:
			m_combo_joinimage.InsertString(i,"����ƴ�ӣ����浹ת��");
			break;
		default:
			continue;
		}
	}
	m_combo_joinimage.SetCurSel(nCapIndex);
	
	//��������
	m_combo_backprocess.ResetContent();
	nCapIndex = m_pUI->GetCurrentCapIndex(UDSCAP_BACKPROCESS);
	lstCapValues = m_pUI->GetValidCap(UDSCAP_BACKPROCESS);
	for(unsigned int i=0; i<lstCapValues->size();i++)
	{
		switch(lstCapValues->at(i))
		{
		case TWBS_NONE:
			m_combo_backprocess.InsertString(i,"��");
			break;
		case TWBS_SMOOTH:
			m_combo_backprocess.InsertString(i,"ƽ��");
			break;
		case TWBS_FILTER:
			m_combo_backprocess.InsertString(i,"�˳�");
			break;
		default:
			continue;
		}
	}
	m_combo_backprocess.SetCurSel(nCapIndex);
	SetBackProcess();
	
	//��������ǿ��
	nCapValue = (int)(m_pUI->GetCapValueFloat(UDSCAP_BACKPROSTRENTH)); 
	m_slider_backprostrenth.SetPos(nCapValue);
	strText.Format("%d",nCapValue);
	SetDlgItemText(IDC_BASE_EDIT_BACKKPRO_STRENGTH, strText);

	//��תģʽ
	nCapIndex = m_pUI->GetCurrentCapIndex(UDSCAP_ROTATEMODE);
	m_radiobtn_rotatemode = nCapIndex; 

	//����ͼ����ת
	nCapIndex = m_pUI->GetCurrentCapIndex(UDSCAP_BACKIMGROTATE);
	m_radiobtn_backimgrotate = nCapIndex; 

	//ɫ�ʷ�ת
	nCapIndex = m_pUI->GetCurrentCapIndex(UDSCAP_COLORFLIP);
	m_radiobtn_colorflip = nCapIndex; 

	nCapValue = (int)(m_pUI->GetCapValueBool(UDSCAP_MIRROR));
	m_check_mirror.SetCheck(nCapValue);
	//end����

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
					SetFrontColor();
					UpdateButton(IDC_BASE_BUTTON_FRONTCOLOR,"��ɫ");
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
					SetFrontGray();
					UpdateButton(IDC_BASE_BUTTON_FRONTGRAY,"�Ҷ�");
				}
				else {
					m_check_frontgray.SetCheck(FALSE);
				}
			}
		case 2:
			{
				if ( 0x01 == (value & 0x01) ) {
					m_check_frontbw.SetCheck(TRUE);
					SetFrontBW();
					UpdateButton(IDC_BASE_BUTTON_FRONTBW,"�ڰ�");
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
					SetBackColor();
					UpdateButton(IDC_BASE_BUTTON_BACKCOLOR,"��ɫ");
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
					SetBackGray();
					UpdateButton(IDC_BASE_BUTTON_BACKGRAY,"�Ҷ�");
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
					SetBackBW();
					UpdateButton(IDC_BASE_BUTTON_BACKBW,"�ڰ�");
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
	SetMultistream();

	m_pTabColor->UpdateControls(); 
	m_pTabGray->UpdateControls();    
	m_pTabBW->UpdateControls(); 
	m_pTabAutoColor->UpdateControls();

	UpdateData(FALSE);
}


void CPage_Base::InitBasemap(void)
{
	m_basemap.erase(m_basemap.begin(),m_basemap.end());//���

	int nCapValue = (int)(m_pUI->GetCapValueBool(UDSCAP_MULTISTREAM));
	m_basemap[UDSCAP_MULTISTREAM] = (float)nCapValue;

	float value = m_pUI->GetCapValueFloat(UDSCAP_MULTISTREAM_VALUE);
	m_basemap[UDSCAP_MULTISTREAM_VALUE] = value; // ��ʼ��ʱ���UDSCAP_MULTISTREAM_VALUE����֤SetCapValue()����¸�Cap��ֵ
}


BOOL CPage_Base::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��	
	//2.0�汾�����޸�
	//����TAB�ؼ�    
	m_pTabColor->Create(IDD_BASETAB_COLOR, &m_base_tab); 
	m_pTabGray->Create(IDD_BASETAB_GRAY, &m_base_tab);    
	m_pTabBW->Create(IDD_BASETAB_BW, &m_base_tab);    
	m_pTabPreview->Create(IDD_BASETAB_PREVIEW, &m_base_tab);
	m_pTabRotateshow->Create(IDD_BASETAB_ROTATESHOW, &m_base_tab);
	m_pTabSpiltshow->Create(IDD_BASETAB_SPILTSHOW, &m_base_tab);
	m_pTabAutoColor->Create(IDD_BASETAB_AUTOCOLOR, &m_base_tab);   

	m_base_tab.GetClientRect(&m_tabRect);    // ��ȡ��ǩ�ؼ��ͻ���Rect     
	// ����tabRect��ʹ�串�Ƿ�Χ�ʺϷ��ñ�ǩҳ     
	m_tabRect.left += 1;                    
	m_tabRect.right -= 1;     
	m_tabRect.top += 25;     
	m_tabRect.bottom -= 1;   

	InitBasemap();
	InitSliderCtrl();
	UpdateControls();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CPage_Base::InitSliderCtrl()
{
	float fMin,fMax,fStep;
	m_pUI->GetCapRangeFloat(ICAP_AUTODISCARDBLANKPAGES, fMin, fMax, fStep);
	m_slider_removeblank.SetRange((int)fMin, (int)fMax);
	m_slider_removeblank.SetTicFreq((int)fStep); //����

	m_pUI->GetCapRangeFloat(UDSCAP_BACKPROSTRENTH, fMin, fMax, fStep);
	m_slider_backprostrenth.SetRange((int)fMin, (int)fMax);
	m_slider_backprostrenth.SetTicFreq((int)fStep); //����

	UpdateData(FALSE);  // ���¿ؼ���ˢ�µ�ǰ�Ի���
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

	return __super::PreTranslateMessage(pMsg);
}


void CPage_Base::SetMultistream(void)
{
	m_combo_splitimage.EnableWindow(FALSE);	
	if(m_check_autobackcolor.GetCheck() || m_check_autofrontcolor.GetCheck())
	{
		m_check_backbw.SetCheck(FALSE);
		m_check_backbw.EnableWindow(FALSE);
		m_btn_backbw.EnableWindow(FALSE);
		m_check_backgray.SetCheck(FALSE);
		m_check_backgray.EnableWindow(FALSE);
		m_btn_backgray.EnableWindow(FALSE);
		m_check_backcolor.SetCheck(FALSE);
		m_check_backcolor.EnableWindow(FALSE);
		m_btn_backcolor.EnableWindow(FALSE);

		m_check_frontbw.SetCheck(FALSE);
		m_check_frontbw.EnableWindow(FALSE);
		m_btn_frontbw.EnableWindow(FALSE);
		m_check_frontgray.SetCheck(FALSE);
		m_check_frontgray.EnableWindow(FALSE);
		m_btn_frontgray.EnableWindow(FALSE);
		m_check_frontcolor.SetCheck(FALSE);
		m_check_frontcolor.EnableWindow(FALSE);
		m_btn_frontcolor.EnableWindow(FALSE);
	}
	else
	{
		m_check_frontcolor.SetCheck(TRUE);
		m_check_backbw.EnableWindow(TRUE);
		m_btn_backbw.EnableWindow(TRUE);
		m_check_backgray.EnableWindow(TRUE);
		m_btn_backgray.EnableWindow(TRUE);
		m_check_backcolor.EnableWindow(TRUE);
		m_btn_backcolor.EnableWindow(TRUE);
		m_check_frontbw.EnableWindow(TRUE);
		m_btn_frontbw.EnableWindow(TRUE);
		m_check_frontgray.EnableWindow(TRUE);
		m_btn_frontgray.EnableWindow(TRUE);
		m_check_frontcolor.EnableWindow(TRUE);
		m_btn_frontcolor.EnableWindow(TRUE);
	}
}


void CPage_Base::UpdateButton(int nIDCtrl, CString str)
{
	m_mfcBtn = new CMFCButton; //MFCButton
	CRect rect;
	GetDlgItem(nIDCtrl)->GetWindowRect(rect);
	ScreenToClient(rect); //Screen(��Ļ����) �� Client(�ͻ�������)��ת��
	m_mfcBtn->Create(_T(str),WS_VISIBLE,rect,this,nIDCtrl);
	m_mfcBtn->SetFaceColor(RGB(153, 217, 234)); //  ���ı�����ɫ
	//m_mfcBtn->SetTextColor(RGB(255, 255, 255)); //  ����������ɫ
	m_mfcBtn->SetTextHotColor(RGB(63, 72, 204)); //  ���ĸ�����ɫ

}

void CPage_Base::SwitchCheckBtn()
{
	if(m_check_backbw.GetCheck())
	{
		SetBackBW();
		UpdateButton(IDC_BASE_BUTTON_BACKBW,"�ڰ�");
	}
	else if(m_check_frontbw.GetCheck())
	{
		SetFrontBW();
		UpdateButton(IDC_BASE_BUTTON_FRONTBW,"�ڰ�");
	}
	else if(m_check_backgray.GetCheck())
	{
		SetBackGray();
		UpdateButton(IDC_BASE_BUTTON_BACKGRAY,"�Ҷ�");
	}
	else if(m_check_frontgray.GetCheck())
	{	
		SetFrontGray();
		UpdateButton(IDC_BASE_BUTTON_FRONTGRAY,"�Ҷ�");
	}
	else if(m_check_backcolor.GetCheck())
	{	
		SetBackColor();
		UpdateButton(IDC_BASE_BUTTON_BACKCOLOR,"��ɫ");
	}
	else if(m_check_frontcolor.GetCheck())
	{	
		SetFrontColor();
		UpdateButton(IDC_BASE_BUTTON_FRONTCOLOR,"��ɫ");
	}	
	else if(m_check_autobackcolor.GetCheck())
	{
		SetAutoBackColor();	
		UpdateButton(IDC_BASE_BUTTON_AUTOBACKCOLOR,"�Զ���ɫ");
	}	
	else if(m_check_autofrontcolor.GetCheck())
	{
		SetAutoFrontColor();	
		UpdateButton(IDC_BASE_BUTTON_AUTOFRONTCOLOR,"�Զ���ɫ");
	}	
	else{}
}

void CPage_Base::OnBase_Btn_Check_Autofrontcolor()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	if(m_check_frontgray.GetCheck())
	{
		m_btn_autofrontcolor.SetFocus();
		m_btn_autofrontcolor.SetState(TRUE);
		m_btn_autofrontcolor.SetButtonStyle(BS_DEFPUSHBUTTON);

		//SetCapMulti();
		SetAutoFrontColor();
	}
	else
	{
		m_btn_autofrontcolor.SetState(FALSE);
		SwitchCheckBtn();
	}
	SetMultistream();
	UpdateData(FALSE);
}

void CPage_Base::OnBase_Btn_Check_Autobackcolor()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	if(m_btn_autobackcolor.GetCheck())
	{
		m_btn_autobackcolor.SetFocus();
		m_btn_autobackcolor.SetState(TRUE);
		m_btn_autobackcolor.SetButtonStyle(BS_DEFPUSHBUTTON);

		//SetCapMulti();
		SetAutoBackColor();
	}
	else
	{
		m_btn_autobackcolor.SetState(FALSE);
		SwitchCheckBtn();
	}
	SetMultistream();
	UpdateData(FALSE);
}

void CPage_Base::OnBase_Btn_Check_FrontColor()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	int num=GetCheckNum();
	if(num<1)
	{
		m_check_frontcolor.SetCheck(TRUE);
	}

	if(m_check_frontcolor.GetCheck())
	{
		m_btn_frontcolor.SetFocus();
		m_btn_frontcolor.SetState(TRUE);
		m_btn_frontcolor.SetButtonStyle(BS_DEFPUSHBUTTON);

		//UpdateButton(IDC_BASE_BUTTON_FRONTCOLOR,"��ɫ����");
		SetCapMulti();
		SetFrontColor();
	}
	else
	{
		m_btn_frontcolor.SetState(FALSE);
		SwitchCheckBtn();
	}
	UpdateData(FALSE);
}

void CPage_Base::OnBase_Btn_Check_FrontGray()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	int num=GetCheckNum();
	if(num<1)
	{
		m_check_frontgray.SetCheck(TRUE);
	}

	if(m_check_frontgray.GetCheck())
	{
		m_btn_frontgray.SetFocus();
		m_btn_frontgray.SetState(TRUE);
		m_btn_frontgray.SetButtonStyle(BS_DEFPUSHBUTTON);
		
		SetCapMulti();
		SetFrontGray();
		//UpdateButton(IDC_BASE_BUTTON_FRONTGRAY,"�Ҷ�����");
	}
	else
	{
		m_btn_frontgray.SetState(FALSE);
		SwitchCheckBtn();
	}	
	UpdateData(FALSE);
}


void CPage_Base::OnBase_Btn_Check_FrontBw()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	int num=GetCheckNum();
	if(num<1)
	{
		m_check_frontbw.SetCheck(TRUE);
	}

	if(m_check_frontbw.GetCheck())
	{
		m_btn_frontbw.SetFocus();
		m_btn_frontbw.SetState(TRUE);
		m_btn_frontbw.SetButtonStyle(BS_DEFPUSHBUTTON);

		//UpdateButton(IDC_BASE_BUTTON_FRONTBW,"�ڰ�����");
		SetCapMulti();
		SetFrontBW();	
	}
	else
	{
		m_btn_frontbw.SetState(FALSE);
		SwitchCheckBtn();
	}
	UpdateData(FALSE);
}


void CPage_Base::OnBase_Btn_Check_BackColor()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	int num=GetCheckNum();
	if(num<1)
	{
		m_check_backcolor.SetCheck(TRUE);
	}

	if(m_check_backcolor.GetCheck())
	{
		m_btn_backcolor.SetFocus();
		m_btn_backcolor.SetState(TRUE);
		m_btn_backcolor.SetButtonStyle(BS_DEFPUSHBUTTON);

		//UpdateButton(IDC_BASE_BUTTON_BACKCOLOR,"��ɫ����");
		SetCapMulti();
		SetBackColor();
	}
	else
	{
		m_btn_backcolor.SetState(FALSE);
		SwitchCheckBtn();
	}
	UpdateData(FALSE);
}


void CPage_Base::OnBase_Btn_Check_BackGray()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	int num=GetCheckNum();
	if(num<1)
	{
		m_check_backgray.SetCheck(TRUE);
	}

	if(m_check_backgray.GetCheck())
	{
		m_btn_backgray.SetFocus();
		m_btn_backgray.SetState(TRUE);
		m_btn_backgray.SetButtonStyle(BS_DEFPUSHBUTTON);

		//UpdateButton(IDC_BASE_BUTTON_BACKGRAY,"�Ҷȱ���");
		SetCapMulti();
		SetBackGray();
	}
	else
	{
		m_btn_backgray.SetState(FALSE);
		SwitchCheckBtn();
	}
	UpdateData(FALSE);
}


void CPage_Base::OnBase_Btn_Check_BackBw()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	int num=GetCheckNum();
	if(num<1)
	{
		m_check_backbw.SetCheck(TRUE);
	}

	if(m_check_backbw.GetCheck())
	{
		m_btn_backbw.SetFocus();
		m_btn_backbw.SetState(TRUE);
		m_btn_backbw.SetButtonStyle(BS_DEFPUSHBUTTON);

		//UpdateButton(IDC_BASE_BUTTON_BACKBW,"�ڰױ���");
		SetCapMulti();
		SetBackBW();
	}
	else
	{
		m_btn_backbw.SetState(FALSE);
		SwitchCheckBtn();
	}
	UpdateData(FALSE);
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
	else if(strCBText.Find("�Զ���ת") >= 0)
	{
		nval = TWOR_DEFINED;
	}
	else
	{}

	m_pUI->SetCapValueInt(ICAP_ROTATION,nval); 
	m_combo_rotate.SetCurSel(nIndex);
	SetRotate();
}

void CPage_Base::SetRotate()
{
	int nIndex = m_combo_rotate.GetCurSel();
	if(nIndex == 4)//�Զ���
	{
		GetDlgItem(IDC_BASE_RADIO_ROTATEMODE_FAST)->EnableWindow(TRUE); 
		GetDlgItem(IDC_BASE_RADIO_ROTATEMODE_ALL)->EnableWindow(TRUE);
		GetDlgItem(IDC_BASE_RADIO_ROTATEMODE_SLOW)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_BASE_RADIO_ROTATEMODE_FAST)->EnableWindow(FALSE); 
		GetDlgItem(IDC_BASE_RADIO_ROTATEMODE_ALL)->EnableWindow(FALSE);
		GetDlgItem(IDC_BASE_RADIO_ROTATEMODE_SLOW)->EnableWindow(FALSE);
	}
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
		else if(nval == TWSI_NONE)// && m_radiobtn_duplex == 0) //�����&&����
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

void CPage_Base::SetAutoFrontColor()
{
	//if(m_check_autofrontcolor.GetCheck())
	{
		m_base_tab.DeleteAllItems();
		m_base_tab.InsertItem(6, _T("�����Զ���ɫ"));
		m_base_tab.SetCurSel(6);
		m_pTabColor->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_HIDEWINDOW);  
		m_pTabGray->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_HIDEWINDOW);  
		m_pTabBW->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_HIDEWINDOW); 
		m_pTabPreview->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_HIDEWINDOW);
		m_pTabRotateshow->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_HIDEWINDOW);
		m_pTabSpiltshow->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_HIDEWINDOW);
		m_pTabAutoColor->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_SHOWWINDOW);
		m_pTabAutoColor->basebutton = 0;
		m_pTabAutoColor->UpdateControls();
	}	
}
void CPage_Base::SetAutoBackColor()
{
	//if(m_check_autobackcolor.GetCheck())
	{
		m_base_tab.DeleteAllItems();
		m_base_tab.InsertItem(6, _T("�����Զ���ɫ"));
		m_base_tab.SetCurSel(6);
		m_pTabColor->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_HIDEWINDOW);  
		m_pTabGray->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_HIDEWINDOW);  
		m_pTabBW->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_HIDEWINDOW); 
		m_pTabPreview->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_HIDEWINDOW);
		m_pTabRotateshow->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_HIDEWINDOW);
		m_pTabSpiltshow->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_HIDEWINDOW);
		m_pTabAutoColor->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_SHOWWINDOW);
		m_pTabAutoColor->basebutton = 0;
		m_pTabAutoColor->UpdateControls();
	}	
}
void CPage_Base::SetFrontColor()
{
	//if(m_check_frontcolor.GetCheck())
	{
		//UpdateButton(IDC_BASE_BUTTON_FRONTCOLOR,"��ɫ����");
		m_base_tab.DeleteAllItems();
		m_base_tab.InsertItem(0, _T("��ɫ����"));
		m_base_tab.SetCurSel(0);
		m_pTabColor->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_SHOWWINDOW);  
		m_pTabGray->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_HIDEWINDOW);  
		m_pTabBW->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_HIDEWINDOW); 
		m_pTabPreview->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_HIDEWINDOW);
		m_pTabRotateshow->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_HIDEWINDOW);
		m_pTabSpiltshow->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_HIDEWINDOW);
		m_pTabAutoColor->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_HIDEWINDOW);
		m_pTabColor->basebutton = 0;
		m_pTabColor->UpdateControls();
	}	
}
void CPage_Base::SetFrontGray()
{
	//if(m_check_frontgray.GetCheck())
	{
		//UpdateButton(IDC_BASE_BUTTON_FRONTGRAY,"�Ҷ�����");
		m_base_tab.DeleteAllItems();
		m_base_tab.InsertItem(1, _T("�Ҷ�����"));
		m_base_tab.SetCurSel(1);
		m_pTabColor->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_HIDEWINDOW);  
		m_pTabGray->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_SHOWWINDOW);  
		m_pTabBW->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_HIDEWINDOW); 
		m_pTabPreview->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_HIDEWINDOW);
		m_pTabRotateshow->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_HIDEWINDOW);
		m_pTabSpiltshow->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_HIDEWINDOW);
		m_pTabAutoColor->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_HIDEWINDOW);
		m_pTabGray->basebutton = 0;
		m_pTabGray->UpdateControls();
	}
	/*else
	{
		delete m_mfcBtn;
	}*/
}
void CPage_Base::SetFrontBW()
{
	//if(m_check_frontbw.GetCheck())
	{
		//UpdateButton(IDC_BASE_BUTTON_FRONTBW,"�ڰ�����");
		m_base_tab.DeleteAllItems();
		m_base_tab.InsertItem(2, _T("�ڰ�����"));
		m_base_tab.SetCurSel(2);
		m_pTabColor->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_HIDEWINDOW);  
		m_pTabGray->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_HIDEWINDOW);  
		m_pTabBW->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_SHOWWINDOW); 
		m_pTabPreview->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_HIDEWINDOW);
		m_pTabRotateshow->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_HIDEWINDOW);
		m_pTabSpiltshow->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_HIDEWINDOW);
		m_pTabAutoColor->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_HIDEWINDOW);
		m_pTabBW->basebutton = 0;
		m_pTabBW->UpdateControls();
	}
}
void CPage_Base::SetBackColor()
{
	//if(m_check_backcolor.GetCheck())
	{
		//UpdateButton(IDC_BASE_BUTTON_BACKCOLOR,"��ɫ����");
		m_base_tab.DeleteAllItems();
		m_base_tab.InsertItem(0, _T("��ɫ����"));
		m_base_tab.SetCurSel(0);
		m_pTabColor->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_SHOWWINDOW);  
		m_pTabGray->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_HIDEWINDOW);  
		m_pTabBW->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_HIDEWINDOW);  
		m_pTabPreview->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_HIDEWINDOW);
		m_pTabRotateshow->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_HIDEWINDOW);
		m_pTabSpiltshow->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_HIDEWINDOW);
		m_pTabAutoColor->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_HIDEWINDOW);
		m_pTabColor->basebutton = 1;
		m_pTabColor->UpdateControls();
	}
}

void CPage_Base::SetBackGray()
{
	//if(m_check_backgray.GetCheck())
	{
		//UpdateButton(IDC_BASE_BUTTON_BACKGRAY,"�Ҷȱ���");
		m_base_tab.DeleteAllItems();
		m_base_tab.InsertItem(1, _T("�Ҷȱ���"));
		m_base_tab.SetCurSel(1);
		m_pTabColor->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_HIDEWINDOW);  
		m_pTabGray->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_SHOWWINDOW);  
		m_pTabBW->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_HIDEWINDOW); 
		m_pTabPreview->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_HIDEWINDOW);
		m_pTabRotateshow->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_HIDEWINDOW);
		m_pTabSpiltshow->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_HIDEWINDOW);
		m_pTabAutoColor->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_HIDEWINDOW);
		m_pTabGray->basebutton = 1;
		m_pTabGray->UpdateControls();
	}
}
void CPage_Base::SetBackBW()
{
	//if(m_check_frontbw.GetCheck())
	{
		//UpdateButton(IDC_BASE_BUTTON_BACKBW,"�ڰױ���");
		m_base_tab.DeleteAllItems();
		m_base_tab.InsertItem(2, _T("�ڰױ���"));
		m_base_tab.SetCurSel(2);
		m_pTabColor->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_HIDEWINDOW);  
		m_pTabGray->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_HIDEWINDOW);  
		m_pTabBW->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_SHOWWINDOW);
		m_pTabPreview->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_HIDEWINDOW);
		m_pTabRotateshow->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_HIDEWINDOW);
		m_pTabSpiltshow->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_HIDEWINDOW);
		m_pTabAutoColor->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_HIDEWINDOW);
		m_pTabBW->basebutton = 1;
		m_pTabBW->UpdateControls();
	}
}

void CPage_Base::OnBase_Btn_Autofrontcolor()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SetAutoFrontColor();
}


void CPage_Base::OnBase_Btn_Autobackcolor()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SetAutoBackColor();
}

void CPage_Base::OnBase_Btn_FrontColor()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������	
	UpdateData(TRUE);
	if(m_check_frontcolor.GetCheck())
	{
		SetFrontColor();
		UpdateButton(IDC_BASE_BUTTON_FRONTCOLOR,"��ɫ");
	}
	UpdateData(FALSE);
}


void CPage_Base::OnBase_Btn_Backcolor()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	if(m_check_backcolor.GetCheck())
	{
		SetBackColor();
		UpdateButton(IDC_BASE_BUTTON_BACKCOLOR,"��ɫ");
	}
	UpdateData(FALSE);
}


void CPage_Base::OnBase_Btn_Frontgray()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	if(m_check_frontgray.GetCheck())
	{
		SetFrontGray();
		UpdateButton(IDC_BASE_BUTTON_FRONTGRAY,"�Ҷ�");
	}
	UpdateData(FALSE);
}


void CPage_Base::OnBase_Btn_Backgray()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	if(m_check_backgray.GetCheck())
	{
		SetBackGray();
		UpdateButton(IDC_BASE_BUTTON_BACKGRAY,"�Ҷ�");
	}
	UpdateData(FALSE);
}


void CPage_Base::OnBase_Btn_Frontbw()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	if(m_check_frontbw.GetCheck())
	{
		SetFrontBW();
		UpdateButton(IDC_BASE_BUTTON_FRONTBW,"�ڰ�");
	}
	UpdateData(FALSE);
}


void CPage_Base::OnBase_Btn_Backbw()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	if(m_check_frontbw.GetCheck())
	{
		SetBackBW();
		UpdateButton(IDC_BASE_BUTTON_BACKBW,"�ڰ�");
	}
	UpdateData(FALSE);
}

void CPage_Base::PreView()
{
	m_base_tab.DeleteAllItems();
	m_base_tab.InsertItem(3, _T("Ԥ��"));
	m_base_tab.SetCurSel(3);
	m_pTabColor->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_HIDEWINDOW);  
	m_pTabGray->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_HIDEWINDOW);  
	m_pTabBW->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_HIDEWINDOW); 
	m_pTabPreview->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_SHOWWINDOW);
	m_pTabRotateshow->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_HIDEWINDOW);
	m_pTabSpiltshow->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_HIDEWINDOW);
	m_pTabAutoColor->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_HIDEWINDOW);
	m_pTabPreview->PreView();
}

void CPage_Base::OnBase_Btn_RotateShow()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_base_tab.DeleteAllItems();
	m_base_tab.InsertItem(4, _T("��ת��ʾ"));
	m_base_tab.SetCurSel(4);
	m_pTabColor->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_HIDEWINDOW);  
	m_pTabGray->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_HIDEWINDOW);  
	m_pTabBW->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_HIDEWINDOW); 
	m_pTabPreview->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_HIDEWINDOW);
	m_pTabRotateshow->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_SHOWWINDOW);
	m_pTabSpiltshow->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_HIDEWINDOW);
	m_pTabAutoColor->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_HIDEWINDOW);
}


void CPage_Base::OnBase_Btn_SpiltShow()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_base_tab.DeleteAllItems();
	m_base_tab.InsertItem(5, _T("�����ʾ"));
	m_base_tab.SetCurSel(5);
	m_pTabColor->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_HIDEWINDOW);  
	m_pTabGray->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_HIDEWINDOW);  
	m_pTabBW->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_HIDEWINDOW); 
	m_pTabPreview->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_HIDEWINDOW);
	m_pTabRotateshow->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_HIDEWINDOW);
	m_pTabSpiltshow->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_SHOWWINDOW);
	m_pTabAutoColor->SetWindowPos(NULL, m_tabRect.left, m_tabRect.top, m_tabRect.Width(), m_tabRect.Height(), SWP_HIDEWINDOW);
}

int CPage_Base::GetCheckNum()
{
	int num=0;
	if(m_check_frontcolor.GetCheck())
	{
		num++;
	}
	else if(m_check_frontgray.GetCheck())
	{
		num++;
	}
	else if(m_check_frontbw.GetCheck())
	{
		num++;
	}
	else if(m_check_backcolor.GetCheck())
	{
		num++;
	}
	else if(m_check_backgray.GetCheck())
	{
		num++;
	}
	else if(m_check_backbw.GetCheck())
	{
		num++;
	}
	else{}
	
	return num;
}


void CPage_Base::OnCbnSelchangeBase_Combo_Joinimg()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nIndex = m_combo_joinimage.GetCurSel();
	CString strCBText; 
	m_combo_joinimage.GetLBText( nIndex, strCBText);
	int nval;
	if (strCBText.Find("����ƴ��") >= 0)
	{
		nval = TWJI_LEFTRIGHT;
	}
	else if(strCBText.Find("����ƴ��") >= 0)
	{
		nval = TWJI_UPDOWN; 
	}
	else if(strCBText.Find("����ƴ�ӣ����浹ת��") >= 0)
	{
		nval = TWJI_UPDOWNBACKTURN; 
	}
	else
	{}

	m_pUI->SetCapValueInt(UDSCAP_JOINIMAGE,nval); 
	m_combo_joinimage.SetCurSel(nIndex);
}


void CPage_Base::OnCbnSelchangeBase_Combo_Backprocess()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nIndex = m_combo_backprocess.GetCurSel();
	CString strCBText; 
	m_combo_backprocess.GetLBText( nIndex, strCBText);
	int nval;
	if (strCBText.Find("��") >= 0)
	{
		nval = TWBS_NONE;
	}
	else if(strCBText.Find("ƽ��") >= 0)
	{
		nval = TWBS_SMOOTH; 
	}
	else if(strCBText.Find("�˳�") >= 0)
	{
		nval = TWBS_FILTER; 
	}
	else
	{}

	m_pUI->SetCapValueInt(UDSCAP_BACKPROCESS,nval); 
	m_combo_backprocess.SetCurSel(nIndex);	

	SetBackProcess();
}


void CPage_Base::OnCbnSelchangeBase_Combo_BackproMode()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������	
	int proindex = m_combo_backprocess.GetCurSel();
	
	int modeindex = m_combo_backpromode.GetCurSel();
	CString strCBText; 
	m_combo_backpromode.GetLBText( modeindex, strCBText);
	int nval;

	if(proindex == 1) //ƽ��
	{
		if (strCBText.Find("�Զ�") >= 0)
		{
			nval = TWBM_AUTO;
		}
		else if(strCBText.Find("����") >= 0)
		{
			nval = TWBI_SCREEN; 
		}
		else
		{}
		m_combo_backpromode.SetCurSel(modeindex);
		m_pUI->SetCapValueInt(UDSCAP_BACKPROMODE,nval); 
	}
	else if(proindex == 2) //�˳�
	{
		if (strCBText.Find("�Զ�") >= 0)
		{
			nval = TWBF_AUTO;
		}
		else if(strCBText.Find("��ɫ") >= 0)
		{
			nval = TWBF_WHITE; 
		}
		else
		{} 
		m_combo_backpromode.SetCurSel(modeindex);
		m_pUI->SetCapValueInt(UDSCAP_BACKPROFILLCOLOR,nval); 
	}
	else{}
}


void CPage_Base::OnNMCustomdrawBase_Slider_BackkproStrength(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);  // ��������s=
	CString str;
	int sldValue = m_slider_backprostrenth.GetPos();  // ��ȡ���鵱ǰλ��
	
	if(m_slider_backprostrenth.IsWindowEnabled())
	{
		m_pUI->SetCapValueFloat(UDSCAP_BACKPROSTRENTH,(float)sldValue);
	}	

	str.Format("%d", sldValue);
	SetDlgItemText(IDC_BASE_EDIT_BACKKPRO_STRENGTH, str);
	UpdateData(FALSE);  // ���¿ؼ�
	*pResult = 0;
}


void CPage_Base::OnEnChangeBase_Edit_BackkproStrength()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д __super::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);  // ��������
	CString str;
	m_edit_backprostrenth.GetWindowText(str);
	int nval = _ttoi(str);
	m_slider_backprostrenth.SetPos(nval);

	if(m_slider_backprostrenth.IsWindowEnabled())
	{
		m_pUI->SetCapValueFloat(UDSCAP_BACKPROSTRENTH,(float)nval);
	}	

	m_edit_backprostrenth.SetSel(str.GetLength(), str.GetLength(),TRUE);  // ���ñ༭��ؼ���Χ
	UpdateData(FALSE);  // ���¿ؼ�
}


void CPage_Base::OnBase_RadioBtn_Rotatemode_Fast()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE); //��radio��״ֵ̬���¸������ı���
	int nIndex = m_combo_rotate.GetCurSel();
	if(nIndex == 4)//�Զ���ת
	{
		m_pUI->SetCapValueInt(UDSCAP_ROTATEMODE,m_radiobtn_rotatemode);
	}
	else
	{
		m_pUI->SetCapValueInt(UDSCAP_ROTATEMODE,TWRM_FAST);
	}
	UpdateData(FALSE);
}


void CPage_Base::OnBase_RadioBtn_Backrotate_Same()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE); //��radio��״ֵ̬���¸������ı���
	m_pUI->SetCapValueInt(UDSCAP_BACKIMGROTATE,m_radiobtn_backimgrotate);	
	UpdateData(FALSE);
}


void CPage_Base::OnBase_RadioBtn_Colorflip_Positive()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE); //��radio��״ֵ̬���¸������ı���
	m_pUI->SetCapValueInt(UDSCAP_COLORFLIP,m_radiobtn_colorflip);	
	UpdateData(FALSE);
}


void CPage_Base::OnBase_Btn_Check_Hozimirror()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nval;
	if (m_check_mirror.GetCheck())
	{
		nval = TWMR_AUTO;
	} 
	else
	{
		nval = TWMR_DISABLE;
	}
	m_pUI->SetCapValueInt(UDSCAP_MIRROR,nval);
}

void CPage_Base::SetBackProcess()
{
	int nIndex = m_combo_backprocess.GetCurSel();
	CString strCBText; 
	m_combo_backprocess.GetLBText( nIndex, strCBText);

	int nCapValue;
	int nCapIndex;
	float fMin,fMax,fStep;
	CString strText;
	const IntVector* lstCapValues;

	if (strCBText.Find("��") >= 0)
	{
		GetDlgItem(IDC_BASE_SLIDER_BACKKPRO_STRENGTH)->EnableWindow(FALSE); 
		GetDlgItem(IDC_BASE_EDIT_BACKKPRO_STRENGTH)->EnableWindow(FALSE); 
		GetDlgItem(IDC_BASE_COMBO_BACKPRO_MODE)->EnableWindow(FALSE);

		//��������ģʽ
		m_combo_backpromode.ResetContent();
		nCapIndex = m_pUI->GetCurrentCapIndex(UDSCAP_BACKPROMODE);
		lstCapValues = m_pUI->GetValidCap(UDSCAP_BACKPROMODE);
		for(unsigned int i=0; i<lstCapValues->size();i++)
		{
			switch(lstCapValues->at(i))
			{
			case TWBM_AUTO:
				m_combo_backpromode.InsertString(i,"�Զ�");
				break;
			case TWBI_SCREEN:
				m_combo_backpromode.InsertString(i,"����");
				break;
			default:
				continue;
			}
		}
		m_combo_backpromode.SetCurSel(nCapIndex);

		SetDlgItemText(IDC_BASE_STATIC_BACKPRO_MODE, TEXT("ģʽ:"));
	}
	else if(strCBText.Find("ƽ��") >= 0)
	{
		GetDlgItem(IDC_BASE_SLIDER_BACKKPRO_STRENGTH)->EnableWindow(TRUE); 
		GetDlgItem(IDC_BASE_EDIT_BACKKPRO_STRENGTH)->EnableWindow(TRUE); 
		GetDlgItem(IDC_BASE_COMBO_BACKPRO_MODE)->EnableWindow(TRUE);

		//��������ģʽ
		m_combo_backpromode.ResetContent();
		nCapIndex = m_pUI->GetCurrentCapIndex(UDSCAP_BACKPROMODE);
		lstCapValues = m_pUI->GetValidCap(UDSCAP_BACKPROMODE);
		for(unsigned int i=0; i<lstCapValues->size();i++)
		{
			switch(lstCapValues->at(i))
			{
			case TWBM_AUTO:
				m_combo_backpromode.InsertString(i,"�Զ�");
				break;
			case TWBI_SCREEN:
				m_combo_backpromode.InsertString(i,"����");
				break;
			default:
				continue;
			}
		}
		m_combo_backpromode.SetCurSel(nCapIndex);
		
		SetDlgItemText(IDC_BASE_STATIC_BACKPRO_MODE, TEXT("ģʽ:"));
	}
	else if(strCBText.Find("�˳�") >= 0)
	{
		GetDlgItem(IDC_BASE_SLIDER_BACKKPRO_STRENGTH)->EnableWindow(TRUE); 
		GetDlgItem(IDC_BASE_EDIT_BACKKPRO_STRENGTH)->EnableWindow(TRUE); 
		GetDlgItem(IDC_BASE_COMBO_BACKPRO_MODE)->EnableWindow(TRUE);

		//�����������ɫ
		m_combo_backpromode.ResetContent();
		nCapIndex = m_pUI->GetCurrentCapIndex(UDSCAP_BACKPROFILLCOLOR);
		lstCapValues = m_pUI->GetValidCap(UDSCAP_BACKPROFILLCOLOR);
		for(unsigned int i=0; i<lstCapValues->size();i++)
		{
			switch(lstCapValues->at(i))
			{
			case TWBF_AUTO:
				m_combo_backpromode.InsertString(i,"�Զ�");
				break;
			case TWBF_WHITE:
				m_combo_backpromode.InsertString(i,"��ɫ");
				break;
			default:
				continue;
			}
		}
		m_combo_backpromode.SetCurSel(nCapIndex);

		SetDlgItemText(IDC_BASE_STATIC_BACKPRO_MODE, TEXT("���ɫ:"));
	}
	else
	{}
}
// Page_Base.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "UDS Scanner.h"
#include "Page_Base.h"
#include "afxdialogex.h"
#include "Dlg_Profile.h"

#include "CTWAINDS_UDS.h"

/** ��������Сֵ */
#define SLIDER_MIN -100  
/** ���������ֵ */
#define SLIDER_MAX  100 

/** ��ֵ��������Сֵ */
#define SLIDER_MIN_THRESHOLD 0
/** ��ֵ���������ֵ */
#define SLIDER_MAX_THRESHOLD 255

#define UNUSED_VAR(x) (void)x;

// CPage_Base �Ի���
extern HWND g_hwndDLG;

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
	ON_BN_CLICKED(IDC_BASE_BTN_SAVEPROFILE, &CPage_Base::OnBase_Btn_Saveprofile)
	ON_BN_CLICKED(IDC_BUTTON1, &CPage_Base::OnBnClickedButton1)
END_MESSAGE_MAP()


// CPage_Base ��Ϣ�������


void CPage_Base::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���
	if(m_pUI->m_bSetup)  // EnableDSOnly
	{
		m_pUI->Save();
	}
	else  
	{
		//::MessageBox(g_hwndDLG,"OnOK",MB_CAPTION,MB_OK);
		SetCapValue(); //���ȷ���������	
		m_pAdPage->SetCapValue();
		m_pUI->Scan();
	//m_pUI->Save(); //ɨ���ҲҪ������Ĳ�������ɨ�裬�ٱ��棻���ȱ����ֱ���˳�
	}

	CPropertyPage::OnOK();
}


void CPage_Base::OnCancel()
{
	// TODO: �ڴ����ר�ô����/����û���
	m_pUI->Cancel();
	CPropertyPage::OnCancel();
}

//BOOL CPage_Base::OnApply()  //�����ȷ�����롰Ӧ�á���ť������
//{
//	// TODO: �ڴ����ר�ô����/����û���
//	AfxMessageBox("OnApply");
//	ChangeControls();
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
			{
				m_pUI->SetCapValueFloat(iter->first,iter->second);  // ���öԱȶ�Ϊ��ǰ������ֵ
				break;
			}		
		case ICAP_BRIGHTNESS:
			{
				m_pUI->SetCapValueFloat(iter->first,iter->second);  // ��������Ϊ��ǰ������ֵ
				break;
			}	
		case ICAP_THRESHOLD:
			{
				m_pUI->SetCapValueFloat(iter->first,iter->second);  // ������ֵΪ��ǰ������ֵ
				break;
			}	
		case CAP_FEEDERENABLED:
			{
				m_pUI->SetCapValueInt(iter->first,(int)iter->second); // ����ɨ��ģʽ
				break;
			}		
		case ICAP_PIXELTYPE:
			{
				m_pUI->SetCapValueInt(iter->first,(int)iter->second);  // ������������
				break;
			}	
		case ICAP_XRESOLUTION:
			{
				m_pUI->SetCapValueInt(iter->first,(int)iter->second);  // ����X�ֱ���
				break;
			}		
		case ICAP_YRESOLUTION:
			{
				m_pUI->SetCapValueInt(iter->first,(int)iter->second);  // ����Y�ֱ���
				break;
			}		
		case CAP_DUPLEXENABLED:
			{
				m_pUI->SetCapValueInt(iter->first,(int)iter->second); // ���õ�˫��
				break;
			}			
		case UDSCAP_MULTIFEEDDETECT:
			{
				m_pUI->SetCapValueInt(iter->first,(int)iter->second); // �������ż��
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
	
	// ɨ�跽ʽ	
	m_combo_source.ResetContent();
	nCapIndex = m_pUI->GetCurrentCapIndex(CAP_FEEDERENABLED);
	lstCapValues = m_pUI->GetValidCap(CAP_FEEDERENABLED);
	if(lstCapValues && lstCapValues->size()>0)
	{
		m_combo_source.InsertString(0,lstCapValues->at(0)!=0?"�Զ���ֽ��":"ƽ��");
		if(lstCapValues->size()>1)
		{
			m_combo_source.InsertString(1,lstCapValues->at(1)!=0?"�Զ���ֽ��":"ƽ��");
		}	
	}
	m_combo_source.SetCurSel(nCapIndex);


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

	
	// �ֱ���
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

	
	// ����/˫��ɨ
	// @see CTWAINDS_FreeIMage.cpp Line 675
	m_combo_scanside.ResetContent();  // �������
	nCapIndex = m_pUI->GetCurrentCapIndex(CAP_DUPLEXENABLED); //1
	lstCapValues = m_pUI->GetValidCap(CAP_DUPLEXENABLED);
	if(lstCapValues && lstCapValues->size()>0)
	{
		m_combo_scanside.InsertString(0,lstCapValues->at(0)!=0?"˫��":"����"); //����˳���� 0-���棬1-˫��
		if(lstCapValues->size()>1)
		{
			m_combo_scanside.InsertString(1,lstCapValues->at(1)!=0?"˫��":"����");
		}
		m_combo_scanside.SetCurSel(nCapIndex);  // ��ʾĬ��ֵ
	}

	UpdateData(FALSE);
}


BOOL CPage_Base::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	UpdateControls();
	InitSliderCtrl();
	InitComboProfile();
	InitComboPixType(); //��ʼ��ͼ������������ֵ��Ӧ�����ȵ�ֵ�Ƿ����

	m_check_multifeeddetect.SetCheck(TRUE); //Ĭ������ѡ�����ż��
	m_btn_chooseimage.ShowWindow(FALSE); //ѡ��ͼƬ��ť��ʱ������

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CPage_Base::InitSliderCtrl()
{
	m_slider_contrast.SetRange(SLIDER_MIN,SLIDER_MAX);
	m_slider_contrast.SetTicFreq(1);  // ���û������̶ȵ�Ƶ��Ϊ1����λ������Ҫ����������们���ʼλ�ò���
	m_slider_contrast.SetPos(0);

	m_slider_brightness.SetRange(SLIDER_MIN,SLIDER_MAX);
	m_slider_brightness.SetTicFreq(1);
	m_slider_brightness.SetPos(0);//����Ϊ�м�

	m_slider_threshold.SetRange(SLIDER_MIN_THRESHOLD,SLIDER_MAX_THRESHOLD);
	m_slider_threshold.SetTicFreq(1);
	m_slider_threshold.SetPos(128); //����λ��ΪĬ��ֵ128

	UpdateData(FALSE);  // ���¿ؼ�
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
	m_basemap.insert(map<int, float> :: value_type(ICAP_CONTRAST, (float)sldValue));
	//m_pUI->SetCapValueFloat(ICAP_CONTRAST,(float)sldValue);  // ���öԱȶ�Ϊ��ǰ������ֵ

	str.Format("%d", sldValue);
	m_edit_contrast.SetWindowText(str);  // �ڱ༭��ͬ����ʾ������ֵ
	//UpdateControls();
	UpdateData(FALSE);  // ���¿ؼ�

	/*// ����Ӧ�ð�ťΪ����״̬
	SetModified(TRUE);*/
	*pResult = 0;
}


void CPage_Base::OnNMCustomdrawBase_Slider_Brightness(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);  // ��������
	CString str;
	int sldValue = m_slider_brightness.GetPos();  // ��ȡ���鵱ǰλ��
	m_basemap.insert(map<int, float> :: value_type(ICAP_BRIGHTNESS, (float)sldValue));
	//m_pUI->SetCapValueFloat(ICAP_BRIGHTNESS,(float)sldValue);  // ��������Ϊ��ǰ������ֵ

	str.Format("%d", sldValue);
	m_edit_brightness.SetWindowText(str);  // �ڱ༭��ͬ����ʾ������ֵ
	//UpdateControls();
	UpdateData(FALSE);  // ���¿ؼ�

	/*// ����Ӧ�ð�ťΪ����״̬
	SetModified(TRUE);*/
	*pResult = 0;
}


void CPage_Base::OnNMCustomdrawBase_Slider_Threshold(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������	
	UpdateData(TRUE); //��������
	CString str;
	int sldValue = m_slider_threshold.GetPos(); //��ȡ����ĵ�ǰλ��
	
	if(0 == sldValue)
	{
		m_basemap.insert(map<int, float> :: value_type(ICAP_THRESHOLD, 128.0));
		//m_pUI->SetCapValueFloat(ICAP_THRESHOLD,128.0);  //����Ĭ��128.G6400Ĭ��230
	}
	else
	{
		m_basemap.insert(map<int, float> :: value_type(ICAP_THRESHOLD, (float)sldValue));
	//	m_pUI->SetCapValueFloat(ICAP_THRESHOLD,(float)sldValue);  // 
	}

	str.Format("%d", sldValue);
	m_edit_threshold.SetWindowText(str); //�ڱ༭��ͬ����ʾ������ֵ
	//UpdateControls();
	UpdateData(FALSE); //���¿ؼ���

	/*// ����Ӧ�ð�ťΪ����״̬
	SetModified(TRUE);*/

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
	m_basemap.insert(map<int, float> :: value_type(ICAP_CONTRAST, (float)nval));
	//m_pUI->SetCapValueFloat(ICAP_CONTRAST,(float)nval);  // ���öԱȶ�Ϊ��ǰ������ֵ

	m_edit_contrast.SetSel(str.GetLength(), str.GetLength(),TRUE);  // ���ñ༭��ؼ���Χ
	//UpdateControls();
	UpdateData(FALSE);  // ���¿ؼ�

	/*// ����Ӧ�ð�ťΪ����״̬
	SetModified(TRUE);*/
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
	m_basemap.insert(map<int, float> :: value_type(ICAP_BRIGHTNESS, (float)nval));
	//m_pUI->SetCapValueFloat(ICAP_BRIGHTNESS,(float)nval);  // ���öԱȶ�Ϊ��ǰ������ֵ

	m_edit_brightness.SetSel(str.GetLength(), str.GetLength(),TRUE);  // ���ñ༭��ؼ���Χ
	//UpdateControls();
	UpdateData(FALSE);  // ���¿ؼ�

	/*// ����Ӧ�ð�ťΪ����״̬
	SetModified(TRUE);*/
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
	//m_basemap.insert(map<int, int> :: value_type(ICAP_THRESHOLD, nval));
	if(0 == nval)
	{
		m_basemap.insert(map<int, float> :: value_type(ICAP_THRESHOLD, 128.0));
		//m_pUI->SetCapValueFloat(ICAP_THRESHOLD,128.0);  // ������ֵΪ��ǰ������ֵ
	}
	else
	{
		m_basemap.insert(map<int, float> :: value_type(ICAP_THRESHOLD, (float)nval));
		//m_pUI->SetCapValueFloat(ICAP_THRESHOLD,(float)nval);  // ������ֵΪ��ǰ������ֵ
	}

	m_edit_threshold.SetSel(str.GetLength(), str.GetLength(), TRUE); //���ñ༭��ؼ���Χ
	//UpdateControls();
	UpdateData(FALSE); //���¿ؼ�

	/*// ����Ӧ�ð�ťΪ����״̬
	SetModified(TRUE);*/
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


////////////////////////////////////////////////////////////////////////////////
// Combobox�ؼ����ݱ䶯
void CPage_Base::OnCbnSelchangeBase_Combo_Source()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nIndex = m_combo_source.GetCurSel();  // ��ǰcombo���
	/*CString str;
	str.Format("%d",nIndex);
	AfxMessageBox(str);//*/
	CString strCBText; 
	m_combo_source.GetLBText( nIndex, strCBText);  // ��ȡ��ǰѡ������
	int nval;
	if(strCBText.Find("ƽ��") >= 0)
	{
		nval = 0;
	} 
	else
	{
		nval = 1; 
	}
	m_basemap.insert(map<int, float> :: value_type(CAP_FEEDERENABLED, (float)nval));
	//m_pUI->SetCapValueInt(CAP_FEEDERENABLED,nval);  // ���ö�Ӧ����
	//UpdateControls();
	m_combo_source.SetCurSel(nIndex);

	/*// ����Ӧ�ð�ťΪ����״̬
	SetModified(TRUE);*/
	return;
}


void CPage_Base::InitComboPixType(void)
{
	int nIndex = m_combo_colormode.GetCurSel();

	if(0 == nIndex) //�ڰ�
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
}

void CPage_Base::OnCbnSelchangeBase_Combo_Colormode()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nIndex = m_combo_colormode.GetCurSel();
	// ֱ�Ӹ��ݵ�ǰ���nIndex����ͼ������
	m_basemap.insert(map<int, float> :: value_type(ICAP_PIXELTYPE, (float)nIndex)); 
	//m_pUI->SetCapValueInt(ICAP_PIXELTYPE,nIndex);  
	m_combo_colormode.SetCurSel(nIndex);
	//UpdateControls();
	InitComboPixType(); //zhu �ж����ȵ��Ƿ����

	/*// ����Ӧ�ð�ťΪ����״̬
	SetModified(TRUE);*/
	//return;
}


void CPage_Base::OnCbnSelchangeBase_Combo_Resolution()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nIndex = m_combo_resolution.GetCurSel();
	CString strCBText; 
	m_combo_resolution.GetLBText(nIndex, strCBText);
	int nval = _ttoi(strCBText);  // CString ת int
	//CString str;
	//str.Format("%d",nval);
	//AfxMessageBox(str);
	m_basemap.insert(map<int, float> :: value_type(ICAP_XRESOLUTION, (float)nval));
	m_basemap.insert(map<int, float> :: value_type(ICAP_YRESOLUTION, (float)nval)); 
	//m_pUI->SetCapValueInt(ICAP_XRESOLUTION,nval); 
	//m_pUI->SetCapValueInt(ICAP_YRESOLUTION,nval);
	//UpdateControls();
	m_combo_resolution.SetCurSel(nIndex);

	/*// ����Ӧ�ð�ťΪ����״̬
	SetModified(TRUE);*/
	return;
}

////////////////////////////////////////////////////////////////////////////////
// ˫��/����ɨ 
void CPage_Base::OnCbnSelchangeBase_Combo_Scanside()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nIndex = m_combo_scanside.GetCurSel();
	CString strCBText; 
	m_combo_scanside.GetLBText( nIndex, strCBText);
	int nval;
	m_combo_scanside.GetLBText( nIndex, strCBText);
	if (strCBText.Find("����") >= 0)
	{
		nval = 0;
	} 
	else
	{
		nval = 1;
	}

	m_basemap.insert(map<int, float> :: value_type(CAP_DUPLEXENABLED, (float)nval));
	//m_pUI->SetCapValueInt(CAP_DUPLEXENABLED,nval?1:0);
	//UpdateControls();
	m_combo_scanside.SetCurSel(nIndex);
}


void CPage_Base::OnClicked_Check_Multifeeddetect()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_check_multifeeddetect.GetCheck()) //����
	{
		m_basemap.insert(map<int, float> :: value_type(CAP_DUPLEXENABLED, 1.0));
	//m_pUI->SetCapValueInt(UDSCAP_MULTIFEEDDETECT,true);	
	} 
	else
	{
		m_basemap.insert(map<int, float> :: value_type(CAP_DUPLEXENABLED, 0.0));
		//m_pUI->SetCapValueInt(UDSCAP_MULTIFEEDDETECT,false);	
	}
}


////////////////////////////////////////////////////////////////////////////////
void CPage_Base::OnBase_Btn_Chooseimage()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	//// ����ͼƬ
	//CHAR szFilePath[MAX_PATH] = {0};
	//if (true == MyBrowseForSignalImage(szFilePath))
	//{
	//	m_pUI->m_pDS->SetScannerImagePath_Signal(szFilePath);
	//	//m_pUI->SetCapValueInt(UDSCAP_DOCS_IN_ADF,2);  
	//}
	

	// ����ͼƬ
	vector<string> vector_string_imagepath;
	vector_string_imagepath = MyBrowseForMultiImages();

	unsigned int nCount = vector_string_imagepath.size();

	m_pUI->SetCapValueInt(UDSCAP_DOCS_IN_ADF,nCount);	
	m_pUI->m_pDS->SetScannerImagePath_Multi(vector_string_imagepath); // ���ݶ���ͼƬ·��

}

////////////////////////////////////////////////////////////////////////////////
//bool CPage_Base::MyBrowseForSignalImage(PTCHAR strFilePath)
//{
//	OPENFILENAME ofn = { 0 };
//	TCHAR szFilename[MAX_PATH] = { 0 };  // ���ڽ����ļ���  
//
//	ofn.lStructSize = sizeof(OPENFILENAME); // �ṹ���С  
//	ofn.hwndOwner = NULL;  // ӵ���Ŵ��ھ����ΪNULL��ʾ�Ի����Ƿ�ģ̬�ģ�ʵ��Ӧ����һ�㶼Ҫ��������  
//	ofn.lpstrFilter = TEXT("�����ļ�\0*.*\0\0");  // ���ù���  
//	ofn.nFilterIndex = 1;  // ����������  
//	ofn.lpstrFile = szFilename;  // ���շ��ص��ļ�����ע���һ���ַ���ҪΪNULL  
//	ofn.nMaxFile = sizeof(szFilename);  // ����������  
//	ofn.lpstrInitialDir = NULL;  // ��ʼĿ¼ΪĬ��  
//	ofn.lpstrTitle = TEXT("��ѡ��һ��ͼƬ�ļ�");  // ʹ��ϵͳĬ�ϱ������ռ���  
//	ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;  // �ļ���Ŀ¼������ڣ�
//	if (GetOpenFileName(&ofn))  
//	{  
//		SSTRCPY(strFilePath,sizeof(szFilename),szFilename);
//		//::MessageBox(NULL, strFilePath, "ѡ���ͼƬ�ļ�", 0);  
//		return true;
//	}  
//	else
//	{  
//		::MessageBox(NULL, TEXT("��ѡ��һ��ͼƬ�ļ�"), NULL, MB_ICONERROR);  
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


////////////////////////////////////////////////////////////////////////////////
// �½�ģ��,�����Ϊģ�� 
void CPage_Base::OnBase_Btn_SaveAsprofile()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//CDlg_Profile *pDlg = new CDlg_Profile;
	//dlg.DoModal();
	//CWnd *pMainWnd = CWnd::FromHandle(g_hwndDLG);
	//m_pDlg = new CDlg_Profile;
	//int nRes = m_pDlg->Create(CDlg_Profile::IDD);
	//m_pDlg->ShowWindow(SW_SHOW);

	CDlg_Profile dlg;  // �½�ģ��Ի���
	dlg.DoModal();

	CString strExistName;

	if (TRUE == dlg.m_bOk)  // ȷ���½�ģ��
	{
		strExistName = dlg.GetProfileName();  // ��ȡ�Ի����б����ģ����
	}
	else
	{
		return;
	}

	// �ж��½�ģ�����Ƿ��Ѵ���
	CString strCombo;  
	int nLength;   
	for (int i=0;i < m_combo_profile.GetCount();i++)
	{        
		nLength = m_combo_profile.GetLBTextLen( i );  // ��ȡCombobox���ݳ���
		m_combo_profile.GetLBText( i, strCombo.GetBuffer(nLength));
		if (strCombo == strExistName)
		{
			if (AfxMessageBox("ģ���Ѵ��ڣ�����Ҫ���´�����",MB_OKCANCEL) == IDCANCEL)  
			{
				return;  // ȡ���½�ͬ��ģ��
			}

		}
		strCombo.ReleaseBuffer();      
	}

	//�����޸ĵ�CapValue
	SetCapValue();
	//m_pAdPage->SetCapValue(); //���ø߼�����


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
// ɾ��ģ�� 
void CPage_Base::OnBase_Btn_Deleteprofile()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);  // ��������
	int nIndex = m_combo_profile.GetCurSel();
	CString strProfile; 
	m_combo_profile.GetLBText( nIndex, strProfile);

	if(m_pUI->TW_DeleteProfile(strProfile.GetBuffer()))
	{
		m_combo_profile.DeleteString(nIndex);
	}
	int nCount = m_combo_profile.GetCount();
	m_combo_profile.SetCurSel(nCount-1);  // �л������һ��
	LoadProfile();
}

void CPage_Base::InitComboProfile()
{
	m_combo_profile.ResetContent();
	m_combo_profile.InsertString(0,"Ĭ��ģ��");
//m_combo_profile.SetCurSel(0); //����ΪĬ��ģ��

	NewBaseProfile();
	
	// �����Ѵ��ڵ�ģ��
	lstString strFileNames;
	m_pUI->TW_GetAllProfiles(strFileNames);

	lstString::iterator iter = strFileNames.begin();
	for(;iter!=strFileNames.end(); iter++)
	{
		CString strTemp(iter->c_str());
		m_combo_profile.AddString(strTemp);
	}

	m_combo_profile.SetCurSel(1); //����Ϊ����ǰģ�塱
}

////////////////////////////////////////////////////////////////////////////////
//�½�ģ�� 
void CPage_Base::NewBaseProfile()
{
	//�½�����ǰģ�塱����Ĭ��ģ�������ͬ������
	CString strName = "��ǰģ��";
	string strProfile = strName.GetBuffer();  // CString->string
	//strName.ReleaseBuffer();
	m_pUI->TW_SaveProfileToFile(strProfile);

	//�½����ڰ�,����,200dpi��ģ��
	m_pUI->SetCapValueInt(ICAP_PIXELTYPE,0);  
	m_pUI->SetCapValueInt(CAP_DUPLEXENABLED,0); 
	//m_pUI->SetCapValueInt(ICAP_XRESOLUTION,200);  // ����X�ֱ���
	//m_pUI->SetCapValueInt(ICAP_YRESOLUTION,200);  // ����X�ֱ���
	strName = "�ڰ�,����,200dpi";
	strProfile = strName.GetBuffer();  // CString->string
	m_pUI->TW_SaveProfileToFile(strProfile);

	//�½����ڰ�,˫��,200dpi��ģ��
	m_pUI->SetCapValueInt(ICAP_PIXELTYPE,0);  
	m_pUI->SetCapValueInt(CAP_DUPLEXENABLED,1);
	strName = "�ڰ�,˫��,200dpi";
	strProfile = strName.GetBuffer();  // CString->string
	m_pUI->TW_SaveProfileToFile(strProfile);

	//�½����Ҷ�,����,200dpi��ģ��
	m_pUI->SetCapValueInt(ICAP_PIXELTYPE,1);  
	m_pUI->SetCapValueInt(CAP_DUPLEXENABLED,0); 
	strName = "�Ҷ�,����,200dpi";
	strProfile = strName.GetBuffer();  // CString->string
	m_pUI->TW_SaveProfileToFile(strProfile);

	//�½����Ҷ�,˫��,200dpi��ģ��
	m_pUI->SetCapValueInt(ICAP_PIXELTYPE,1);  
	m_pUI->SetCapValueInt(CAP_DUPLEXENABLED,1); 
	strName = "�Ҷ�,˫��,200dpi";
	strProfile = strName.GetBuffer();  // CString->string
	m_pUI->TW_SaveProfileToFile(strProfile);

	//�½�����ɫ,����,200dpi��ģ��
	m_pUI->SetCapValueInt(ICAP_PIXELTYPE,2);  
	m_pUI->SetCapValueInt(CAP_DUPLEXENABLED,0); 
	strName = "��ɫ,����,200dpi";
	strProfile = strName.GetBuffer();  // CString->string
	m_pUI->TW_SaveProfileToFile(strProfile);

	//�½�����ɫ,˫��,200dpi��ģ��
	m_pUI->SetCapValueInt(ICAP_PIXELTYPE,2); 
	m_pUI->SetCapValueInt(CAP_DUPLEXENABLED,1); 
	strName = "��ɫ,˫��,200dpi";
	strProfile = strName.GetBuffer();  // CString->string
	m_pUI->TW_SaveProfileToFile(strProfile);
}


////////////////////////////////////////////////////////////////////////////////
void CPage_Base::LoadProfile()
{
	UpdateData(TRUE);  // ��������
	int nIndex = m_combo_profile.GetCurSel();
	if(0 == nIndex)  // Ĭ��ģ�壬������������
	{
		m_pUI->ResetAllCaps();
	}
	else  // ����ģ��
	{	
		CString strProfile; 
		m_combo_profile.GetLBText( nIndex, strProfile);
	//	AfxMessageBox(strProfile);
		m_pUI->TW_LoadProfileFromFile(strProfile.GetBuffer());
	}
	UpdateControls();
	//m_pAdPage->UpdateControl(); //�߼����ý������Ҳ����
}

////////////////////////////////////////////////////////////////////////////////
// ɨ�����ͬ��Ϊģ��ֵ 
void CPage_Base::OnCbnSelchangeBase_Combo_Profile()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	LoadProfile();
}


void CPage_Base::OnBase_Btn_Saveprofile()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//SetCapValue();
	//m_pUI->Save();
	int nIndex = m_combo_profile.GetCurSel();
	string strProfile;
	m_pUI->TW_SaveProfileToFile(strProfile);
}

// ������
void CPage_Base::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnOK();  // ֱ�ӵ���OnOK
}

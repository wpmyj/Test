// Page_Advanced.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "UDS Scanner.h"
#include "Page_Advanced.h"
#include "afxdialogex.h"

/**********************************************************
*  ��4λ �� 7    6   5    4      ����λ �� 3    2   1    0
* �����棩 ���� �ڰ� �Ҷ� ��ɫ    �����棩  ���� �ڰ� �Ҷ� ��ɫ
**********************************************************/

// CPage_Advanced �Ի���

IMPLEMENT_DYNAMIC(CPage_Advanced, CPropertyPage)

CPage_Advanced::CPage_Advanced(MFC_UI *pUI)
	: m_pUI(pUI), CPropertyPage(CPage_Advanced::IDD)
{
	m_radiobtn_spilt_vert = 0;
}

CPage_Advanced::~CPage_Advanced()
{
	m_advancedmap.swap(map<int, float>());  // ��ղ��ͷ��ڴ�
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
		case ICAP_ROTATION:  //��ת
		case UDSCAP_DENOISE: //ȥ����
		case UDSCAP_DESCREEN: //ȥ����
		case UDSCAP_REMOVEBACKGROUND: //ȥ����
		case UDSCAP_SHARPEN: //ͼ����
		case UDSCAP_REMOVEBLANK: //ȥ���հ�ҳ��checkbox
		case UDSCAP_MIRROR: //������
		case UDSCAP_MULTIFEEDDETECT: //���ż��
		case UDSCAP_COLORFLIP: //ɫ�ʷ�ת
		case UDSCAP_CACHEMODE: //����ģʽ
			{
				m_pUI->SetCapValueInt(iter->first,(int)(iter->second));
				break;
			}	
		case UDSCAP_PUNCHHOLEREMOVEL: //ȥ������
			{
				if(m_check_removepunch.GetCheck()) //ȥ���׿���ʱ
				{
					m_pUI->SetCapValueInt(iter->first,(int)(iter->second));
				}
				else
				{
					m_pUI->SetCapValueInt(iter->first,FALSE);
				}
				break;
			}
		case UDSCAP_AUTOCROP: //�Զ�������У��
			{
				if(m_check_autocrop.GetCheck()) //�Զ�������У������  ��ֹ�ͻ���ѡ�и�����ֵ��˶�������ʱ���Ӧ�����ã��������ڲ�����ʱҪ����ΪFALSE
				{
					m_pUI->SetCapValueInt(iter->first,(int)(iter->second));
				}
				else
				{
					m_pUI->SetCapValueInt(iter->first,FALSE);
				}
				break;
			}
			
		case UDSCAP_SPLITIMAGE: //ͼ��ָ�
			{
				m_pUI->SetCapValueInt(iter->first,(int)(iter->second)); //���ò�����Ч

				if(m_combo_splitimage.IsWindowEnabled())
				{
					if((int)(iter->second) == TWSI_HORIZONTAL || (int)(iter->second) == TWSI_VERTICAL)
					{
						m_pUI->SetCapValueInt(UDSCAP_DOCS_IN_ADF, 2); //����ɨ������Ϊ2
					}
					else if((int)(iter->second) == TWSI_NONE && m_pBasePage->scanside == 0) //�����&&����
					{
						m_pUI->SetCapValueInt(UDSCAP_DOCS_IN_ADF, 1); //�����ʱ�����1
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
					m_pUI->SetCapValueInt(iter->first, TWSI_NONE); //���ò�����Ч
				}
				break;
			}
		case ICAP_AUTODISCARDBLANKPAGES: //ȥ���հ�ҳ
			{
				if(m_slider_removeblank.IsWindowEnabled())
				{
					m_pUI->SetCapValueFloat(iter->first,iter->second);
				}	
				break;
			}
		case ICAP_GAMMA: //GammaУ��
			{			
				m_pUI->SetCapValueFloat(iter->first,iter->second);
				break;
			}
		
		case UDSCAP_CACHEMODE_AUTO: //����ģʽ--�Զ�
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

	//���ż�⣺Ĭ��ʹ��
	nCapValue = (int)(m_pUI->GetCapValueBool(UDSCAP_MULTIFEEDDETECT));
	m_check_multifeeddetect.SetCheck(nCapValue);

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
	nval = (int)lstCapValuesFlt->at(nCapIndex);
	m_advancedmap[ICAP_ROTATION] = (float)nval;

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
	nval = (int)lstCapValuesFlt->at(nCapIndex);
	m_advancedmap[UDSCAP_SPLITIMAGE] = (float)nval;//����ֻ������������Ҫ����CAP
	
	//GammaУ�� 
	nCapValue = (int)(m_pUI->GetCapValueFloat(ICAP_GAMMA)); //GetCapValueFloat�ܷ�õ�CTWAINContainerFix32����
	m_slider_gamma.SetPos(nCapValue);
	float valueTemp = ((float)nCapValue)/100;
	strText.Format("%.2f", valueTemp);
	SetDlgItemText(IDC_ADVANCED_EDIT_SENSITIVE_GAMMA, strText);
	m_advancedmap[ICAP_GAMMA] = float(nCapValue);

	//ȥ���հ�ҳcheckbox
	nCapValue = (int)(m_pUI->GetCapValueBool(UDSCAP_REMOVEBLANK));
	m_check_removeblank.SetCheck(nCapValue);
	m_advancedmap[UDSCAP_REMOVEBLANK] = (float)nCapValue;
	//ȥ���հ�ҳ -1�Զ�;-2������:��Ϊ������ 
	nCapValue = (int)(m_pUI->GetCapValueFloat(ICAP_AUTODISCARDBLANKPAGES)); 
	m_slider_removeblank.SetPos(nCapValue);
	strText.Format("%d",nCapValue);
	SetDlgItemText(IDC_ADVANCED_EDIT_REMOVEBLANK, strText);
	m_advancedmap[ICAP_AUTODISCARDBLANKPAGES] = float(nCapValue);
	
	//ȥ�����׵�
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

	//����ģʽ
	m_combo_cachemode.ResetContent(); //�������
	nCapIndex = m_pUI->GetCurrentCapIndex(UDSCAP_CACHEMODE);
	SetColorMode(nCapIndex); //���û������ķ�Χ��ֵ
	lstCapValues = m_pUI->GetValidCap(UDSCAP_CACHEMODE);	
	for(unsigned int i=0; i<lstCapValues->size();i++)
	{
		switch(lstCapValues->at(i))
		{
		case TWCM_NONE:
			m_combo_cachemode.InsertString(i,"�Զ�"); //�Զ�
			break;
		case TWCM_PAPERNUM:
			m_combo_cachemode.InsertString(i,"ֽ������");  //ֽ������		
			break;
		case TWCM_MEMORYSIZE:
			m_combo_cachemode.InsertString(i,"�ڴ��С"); //�ڴ��С
			break;
		default:
			continue;
		}
	}
	m_combo_cachemode.SetCurSel(nCapIndex);
	nval = lstCapValues->at(nCapIndex);
	m_advancedmap[UDSCAP_CACHEMODE] = (float)nval;//����ֻ������������Ҫ����CAP
 
}

void CPage_Advanced::SetColorMode(int nIndex)
{
	float fMin,fMax,fStep;
	int value;
	CString str;
	switch(nIndex)
	{
	case 0:
		SetDlgItemText(IDC_ADVANCED_SLIDERSTATIC_CACHEMODE, TEXT("ͼ������:"));
		SetDlgItemText(IDC_ADVANCED_STATIC_CACHEMODEUNIT, TEXT("ҳ"));
		
		m_pUI->GetCapRangeFloat(UDSCAP_CACHEMODE_AUTO, fMin, fMax, fStep);
		//���û�������Χ
		m_slider_cachemode.SetRange((int)fMin, (int)fMax); //���÷�Χ
		m_slider_cachemode.SetTicFreq((int)fStep); //����

		value = (int)(m_pUI->GetCapValueFloat(UDSCAP_CACHEMODE_AUTO));	
		m_slider_cachemode.SetPos(value);	
		if(value == 0)
		{
			str = "����";
		}
		else
		{
			str.Format("%d",value);
		}
		SetDlgItemText(IDC_ADVANCED_EDIT_CACHEMODE, str);

		break;
	case 1:
		SetDlgItemText(IDC_ADVANCED_SLIDERSTATIC_CACHEMODE, TEXT("ֽ������:"));
		SetDlgItemText(IDC_ADVANCED_STATIC_CACHEMODEUNIT, TEXT("ҳ"));

		m_pUI->GetCapRangeFloat(UDSCAP_CACHEMODE_PAPERNUM, fMin, fMax, fStep);
		//���û�������Χ
		m_slider_cachemode.SetRange((int)fMin, (int)fMax); //���÷�Χ
		m_slider_cachemode.SetTicFreq((int)fStep); //����

		value = (int)(m_pUI->GetCapValueFloat(UDSCAP_CACHEMODE_PAPERNUM));	
		m_slider_cachemode.SetPos(value);
		str.Format("%d",value);
		SetDlgItemText(IDC_ADVANCED_EDIT_CACHEMODE, str);

		break;
	case 2:
		SetDlgItemText(IDC_ADVANCED_SLIDERSTATIC_CACHEMODE, TEXT("�ڴ��С:"));
		SetDlgItemText(IDC_ADVANCED_STATIC_CACHEMODEUNIT, TEXT("MB"));

		m_pUI->GetCapRangeFloat(UDSCAP_CACHEMODE_MEMORYSIZE, fMin, fMax, fStep);
		//���û�������Χ
		m_slider_cachemode.SetRange((int)fMin, (int)fMax); //���÷�Χ
		m_slider_cachemode.SetTicFreq((int)fStep); //����

		value = (int)(m_pUI->GetCapValueFloat(UDSCAP_CACHEMODE_MEMORYSIZE));
		m_slider_cachemode.SetPos(value);	
		if(value == 0)
		{
			str = "Ĭ��";
		}
		else
		{
			str.Format("%d",value);
		}
		SetDlgItemText(IDC_ADVANCED_EDIT_CACHEMODE, str);

		break;
	}
}

// CPage_Advanced ��Ϣ�������
BOOL CPage_Advanced::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	InitAdvancedmap(); //��ʼ��Map
	InitSliderCtrl(); //��ʼ������ Ҫ����UpdateControls֮ǰ���������û���Ĳ�����Ч
	UpdateControls();
	SetMultistream();
	SetBlank();
	SetSpiltimage();

	//��ʱ���ض�ݲ��
	GetDlgItem(IDC_ADVANCED_RADIO_VERTICAL)->ShowWindow(FALSE);
	GetDlgItem(IDC_ADVANCED_RADIO_HORIZONTAL)->ShowWindow(FALSE);
	GetDlgItem(IDC_ADVANCED_EDIT_SPILT)->ShowWindow(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
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
	m_advancedmap.erase(m_advancedmap.begin(),m_advancedmap.end());//���
	int nCapIndex;
	nCapIndex = m_pUI->GetCurrentCapIndex(UDSCAP_SPLITIMAGE);
	m_advancedmap[UDSCAP_SPLITIMAGE] = (float)nCapIndex; //��ʼ��ʱֻΪmap���롰�ָ�Cap����ֵ������
}


//void CPage_Advanced::OnEnChangeAdvanced_Edit_Customwidth()
//{
//	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
//	// ���ʹ�֪ͨ��������д CPropertyPage::OnInitDialog()
//	// ���������� CRichEditCtrl().SetEventMask()��
//	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
//
//	// todo:  �ڴ���ӿؼ�֪ͨ����������
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
//	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
//	// ���ʹ�֪ͨ��������д CPropertyPage::OnInitDialog()
//	// ���������� CRichEditCtrl().SetEventMask()��
//	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
//
//	// TODO:  �ڴ���ӿؼ�֪ͨ����������
//	CString strWidth,strHeight;
//	m_edit_custom_width.GetWindowText(strWidth);
//	m_edit_custom_height.GetWindowText(strHeight);
//	float fW = (float)_ttof(strWidth);  // CString -> float
//	float fH = (float)_ttof(strHeight); 
//	m_pUI->SetCurrentFrame(0.0f,0.0f,fH,fW);
//}


void CPage_Advanced::OnCbnSelchangeAdvanced_Combo_Rotate()
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
	
	m_advancedmap[ICAP_ROTATION] = (float)nval;
	m_combo_rotate.SetCurSel(nIndex);
}


void CPage_Advanced::OnCbnSelchangeAdvanced_Combo_SpiltImage()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	m_advancedmap[UDSCAP_SPLITIMAGE] = (float)nval;
	m_combo_splitimage.SetCurSel(nIndex);
	SetSpiltimage();
}


void CPage_Advanced::InitSliderCtrl()
{
	float fMin,fMax,fStep;
	m_pUI->GetCapRangeFloat(ICAP_GAMMA, fMin, fMax, fStep);
	m_slider_gamma.SetRange((int)fMin, (int)fMax);
	m_slider_gamma.SetTicFreq((int)fStep); //����

	//ȥ���հ�ҳ
	m_pUI->GetCapRangeFloat(ICAP_AUTODISCARDBLANKPAGES, fMin, fMax, fStep);
	m_slider_removeblank.SetRange((int)fMin, (int)fMax);
	m_slider_removeblank.SetTicFreq((int)fStep); //����

	UpdateData(FALSE);  // ���¿ؼ�
}

//Gammaͼ��У��
void CPage_Advanced::OnNMCustomdrawAdvanced_Slider_Gamma(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);  // ��������
	CString str;
	int sldValue = m_slider_gamma.GetPos();  // ��ȡ���鵱ǰλ��
	str.Format("%d",sldValue);

	float Value = ((int)sldValue*10)/10.00f;
	m_advancedmap[ICAP_GAMMA] = Value;

	float valueTemp = ((float)sldValue)/100;
	str.Format("%.2f", valueTemp); //С�����ֻҪ2λ
	SetDlgItemText(IDC_ADVANCED_EDIT_SENSITIVE_GAMMA, str);// �ڱ༭��ͬ����ʾ������ֵ
	UpdateData(FALSE);  // ���¿ؼ�

	*pResult = 0;
}


void CPage_Advanced::OnNMCustomdrawAdvanced_Slider_Removeblank(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);  // ��������
	CString str;
	int sldValue = m_slider_removeblank.GetPos();  // ��ȡ���鵱ǰλ��
	m_advancedmap[ICAP_AUTODISCARDBLANKPAGES] = (float)sldValue;

	str.Format("%d", sldValue);
	SetDlgItemText(IDC_ADVANCED_EDIT_REMOVEBLANK, str);
	UpdateData(FALSE);  // ���¿ؼ�

	*pResult = 0;
}


void CPage_Advanced::OnEnChangeAdvanced_Edit_Removeblank()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д __super::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
	UpdateData(TRUE);  // ��������
	CString str;
	m_edit_removeblank.GetWindowText(str);
	int nval = _ttoi(str);
	m_slider_removeblank.SetPos(nval);
	m_advancedmap[ICAP_AUTODISCARDBLANKPAGES] = (float)nval;
	m_edit_removeblank.SetSel(str.GetLength(), str.GetLength(),TRUE);  // ���ñ༭��ؼ���Χ
	UpdateData(FALSE);  // ���¿ؼ�
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CPage_Advanced::OnEnChangeAdvanced_Edit_Gamma()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CPropertyPage::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);  // ��������
	CString str;
	m_edit_gamma.GetWindowText(str);

	float fval = (float)(_ttof(str));
	int nval = (int)(100*fval);
	m_slider_gamma.SetPos(nval);
	m_advancedmap[ICAP_GAMMA] = float(nval); //map����ǷŴ�100����ֵ

	m_edit_gamma.SetSel(str.GetLength(), str.GetLength(),TRUE);  // ���ñ༭��ؼ���Χ
	UpdateData(FALSE);  // ���¿ؼ�
}


//ȥ���հ�ҳ
void CPage_Advanced::OnAdvanced_Btn_Check_RemoveBlank()
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
	m_advancedmap[UDSCAP_REMOVEBLANK] = (float)nval;

	SetBlank();
}


//ȥ������
void CPage_Advanced::OnAdvanced_Btn_Check_RemovePunch()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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


//��ͼ��
void CPage_Advanced::OnAdvanced_Btn_Check_Sharpen()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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


//ͼ������
void CPage_Advanced::OnAdvanced_Btn_Check_Mirror()
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
	m_advancedmap[UDSCAP_MIRROR] = (float)nval;
}


//ȥ������
void CPage_Advanced::OnAdvanced_Btn_Check_RemoveBack()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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


//ȥ������
void CPage_Advanced::OnAdvanced_Btn_Check_RemoveDescreen()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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


//ȥ������
void CPage_Advanced::OnAdvanced_Btn_Check_RemoveDenoise()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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


//�Զ����м�У��
void CPage_Advanced::OnAdvanced_Btn_Check_AutoCrop()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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


//PreTranslateMessage����TRUEʱ���������Ϣ���͸����ں�������
BOOL CPage_Advanced::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	CEdit* pEdit1 = (CEdit*)GetDlgItem(IDC_ADVANCED_EDIT_BRIGHTNESS);  
	CEdit* pEdit2 = (CEdit*)GetDlgItem(IDC_ADVANCED_EDIT_CONTRAST);  

	CString str1, str2;   
	GetDlgItemText(IDC_ADVANCED_EDIT_BRIGHTNESS, str1); // ��ȡedit���ı�  
	GetDlgItemText(IDC_ADVANCED_EDIT_CONTRAST, str2);

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
		// ��֤����'-'ֻ�ܳ���һ��,����ֻ�ܳ����ڵ�һ���ַ�
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

	//��Ҫ����С�����Editֻ����һ��С����
	CEdit* pEdit3 = (CEdit*)GetDlgItem(IDC_ADVANCED_EDIT_SENSITIVE_GAMMA); 
	CString str3;   
	GetDlgItemText(IDC_ADVANCED_EDIT_SENSITIVE_GAMMA, str3); // ��ȡedit���ı�  
	if( (GetFocus() == pEdit3) && (pMsg->message == WM_CHAR))  
	{  
		//�����������ֺ�С���㡰.��
		if(pMsg->wParam == '.')
		{
			//�����ֻ��������һ��С����
			int nPos3 = 0; 
			nPos3 = str3.Find('.'); // ����.��λ�� 
	
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

	//����ֿ�д����||�����Ļ��ܻ�����
	CEdit* pEdit4 = (CEdit*)GetDlgItem(IDC_ADVANCED_EDIT_CUSTOMWIDTH); 
	CString str4;     
	GetDlgItemText(IDC_ADVANCED_EDIT_CUSTOMWIDTH, str4);
	if( (GetFocus() == pEdit4) && (pMsg->message == WM_CHAR))  
	{  
		//�����������ֺ�С���㡰.��
		if(pMsg->wParam == '.')
		{
			//�����ֻ��������һ��С����
			int nPos4 = 0; 
			nPos4 = str4.Find('.');
			if(nPos4 >= 0)  
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

	CEdit* pEdit5 = (CEdit*)GetDlgItem(IDC_ADVANCED_EDIT_CUSTOMHEIGHT);
	CString str5;   
	GetDlgItemText(IDC_ADVANCED_EDIT_CUSTOMHEIGHT, str5);
	if( (GetFocus() == pEdit5) && (pMsg->message == WM_CHAR))  
	{  
		//�����������ֺ�С���㡰.��
		if(pMsg->wParam == '.')
		{
			//�����ֻ��������һ��С����
			int nPos5 = 0; 
			nPos5 = str5.Find('.');
			if(nPos5 >= 0)
			{
				return 1;
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

void CPage_Advanced::OnClicked_Check_Multifeeddetect()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_check_multifeeddetect.GetCheck()) //����
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
	// TODO: �ڴ����ר�ô����/����û���
	m_pUI->PreViewStatus();
	return __super::OnSetActive();
}


void CPage_Advanced::SetSpiltimage()
{
	CString strCBText;
	GetDlgItemText(IDC_ADVANCED_COMBO_SPLITIMG, strCBText);

	CString str;
	str.Format("%d",2);

	if(strCBText.Find("�����") >= 0)
	{
		str.Format("%d",1);
		SetDlgItemText(IDC_ADVANCED_EDIT_SPILT, str);

		m_edit_spilt.EnableWindow(FALSE);
		GetDlgItem(IDC_ADVANCED_RADIO_HORIZONTAL)->EnableWindow(FALSE);
		GetDlgItem(IDC_ADVANCED_RADIO_VERTICAL)->EnableWindow(FALSE);
	}
	else if(strCBText.Find("����") >= 0 || strCBText.Find("����") >= 0) //ˮƽ����ֱ�ָ�ͼ��
	{
		SetDlgItemText(IDC_ADVANCED_EDIT_SPILT, str);

		m_edit_spilt.EnableWindow(FALSE);
		GetDlgItem(IDC_ADVANCED_RADIO_HORIZONTAL)->EnableWindow(FALSE);
		GetDlgItem(IDC_ADVANCED_RADIO_VERTICAL)->EnableWindow(FALSE);
	}
	else if(strCBText.Find("�Զ���") >= 0)
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	int nIndex = m_combo_cachemode.GetCurSel();
	CString strCBText; 
	m_combo_cachemode.GetLBText( nIndex, strCBText);
	int nval;
	if (strCBText.Find("�Զ�") >= 0)
	{
		nval = TWCM_NONE;
	}
	else if(strCBText.Find("ֽ������") >= 0)
	{
		nval = TWCM_PAPERNUM; 
	}
	else if(strCBText.Find("�ڴ��С") >= 0)
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);  // ��������
	CString str;
	int sldValue = m_slider_cachemode.GetPos();  // ��ȡ���鵱ǰλ��
	int nIndex = m_combo_cachemode.GetCurSel();
	switch(nIndex)
	{
	case 0:
		{
			m_advancedmap[UDSCAP_CACHEMODE_AUTO] = (float)sldValue;

			if(sldValue == 0){
				str = "����";
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
				str = "Ĭ��";
			}
			else{
				str.Format("%d",sldValue);
			}
			SetDlgItemText(IDC_ADVANCED_EDIT_CACHEMODE, str);
		}	
		break;
	}
	UpdateData(FALSE);  // ���¿ؼ�

	*pResult = 0;
}


void CPage_Advanced::OnEnChangeAdvanced_Edit_Cachemode()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д __super::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);  // ��������
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
	m_edit_cachemode.SetSel(str.GetLength(), str.GetLength(),TRUE);  // ����edit���λ��
	UpdateData(FALSE);  // ���¿ؼ�
}

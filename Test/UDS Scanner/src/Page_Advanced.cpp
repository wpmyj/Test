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
BYTE g_MuiltStream = 0x00;

bool muiltstream; //�߼����������ѡ��

// CPage_Advanced �Ի���

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
		case ICAP_SUPPORTEDSIZES:  //ֽ�Ŵ�С
		case ICAP_UNITS:  //��λ
		case ICAP_XRESOLUTION:  //X�ֱ���
		case ICAP_YRESOLUTION:  //Y�ֱ���
		case ICAP_ROTATION:  //��ת
		case UDSCAP_DENOISE: //ȥ����
		case UDSCAP_DESCREEN: //ȥ����
		case UDSCAP_REMOVEBACKGROUND: //ȥ����
		case UDSCAP_SHARPEN: //ͼ����
		case UDSCAP_REMOVEBLANK: //ȥ���հ�ҳ��checkbox
		case UDSCAP_MIRROR: //������
		case UDSCAP_BINARIZATION: //��ֵ��
			{
				m_pUI->SetCapValueInt(iter->first,(int)(iter->second));
				break;
			}	

		case ICAP_FRAMES: //���
			{
				m_pUI->SetCurrentFrame(0,0,iter->second,iter->second);
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

		case UDSCAP_MULTISTREAM: //�������
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
					m_pUI->SetCapValueInt(UDSCAP_DOCS_IN_ADF,checknum); //����ɨ������Ϊchecknum	

					m_pUI->SetCapValueInt(iter->first,(int)(iter->second));
				}	
				break;
			}	

		case UDSCAP_FRONTCOLOR: //��ɫ����
		case UDSCAP_FRONTGRAY: //�Ҷ�����
		case UDSCAP_FRONTBW: //�ڰ�����
		case UDSCAP_BACKCOLOR: //��ɫ����
		case UDSCAP_BACKGRAY: //�Ҷȱ���
		case UDSCAP_BACKBW: //�ڰױ���
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

		case UDSCAP_SPLITIMAGE: //ͼ��ָ�
			{
				m_pUI->SetCapValueInt(iter->first,(int)(iter->second)); //���ò�����Ч
				if((int)(iter->second) != TWSI_NONE)
				{
					m_pUI->SetCapValueInt(UDSCAP_DOCS_IN_ADF,2); //����ɨ������Ϊ2
				}
				else if((int)(iter->second) == TWSI_NONE && m_pBasePage->scanside == 0) //�����&&����
				{
					m_pUI->SetCapValueInt(UDSCAP_DOCS_IN_ADF,1); //�����ʱ�����1
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
				/*
				char buf[10];
				itoa(iter->second, buf, 10);
				::MessageBox(NULL,TEXT(buf),"ICAP_GAMMA",MB_OK);*/

				m_pUI->SetCapValueFloat(iter->first,iter->second);
				break;
			}

		case UDSCAP_SENSITIVETHRESHOLD_REMOVESPOTS: //ȥ���ߵ�
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
					m_pUI->SetCapValueFloat(iter->first,iter->second);  //��ɫ������ʵ���Ƕ�ֵ������ֵ
				}
				break;
			}

		case ICAP_BRIGHTNESS: //����
			{
				if(m_slider_brightness.IsWindowEnabled())
				{
					m_pUI->SetCapValueFloat(iter->first,iter->second);
				}
				break;
			}
		case ICAP_CONTRAST: //�Աȶ�
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
			//δ��
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

	//ֽ������-ֽ�Ŵ�С
	m_combo_standardsizes.ResetContent();  // �������
	nCapIndex = m_pUI->GetCurrentCapIndex(ICAP_SUPPORTEDSIZES);
	lstCapValues = m_pUI->GetValidCap(ICAP_SUPPORTEDSIZES);
	for(unsigned int i=0; i<lstCapValues->size();i++)
	{
		switch(lstCapValues->at(i))
		{
		case TWSS_NONE:
			m_combo_standardsizes.InsertString(i,"�Զ���");
			break;
		case TWSS_USLETTER:
			m_combo_standardsizes.InsertString(i,"US Letter (8.5\" x 11\")");  //216mm x 280mm
			break;
		case TWSS_USLEGAL:
			m_combo_standardsizes.InsertString(i,"US Legal (8.5\" x 14\")");  //216mm x 356mm
			break;
		case TWSS_PHOT64:
			m_combo_standardsizes.InsertString(i,"��Ƭ64 (6\" x 4\")");  //152mm x 102mm
			break;
		case TWSS_PHOT53:
			m_combo_standardsizes.InsertString(i,"��Ƭ53 (5\" x 3\")");  //127mm x 76mm
			break;
		case TWSS_A3:
			m_combo_standardsizes.InsertString(i,"ISO A3 (297mm x 420mm)"); //���ʱ�׼
			break;
		case TWSS_A4:
			m_combo_standardsizes.InsertString(i,"ISO A4 (210mm x 297mm)"); //���ʱ�׼
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
			m_combo_standardsizes.InsertString(i,"JIS B4 (257mm x 364mm)");//JIS�ձ���׼
			break;
		case TWSS_JISB5:
			m_combo_standardsizes.InsertString(i,"JIS B5 (182mm x 257mm)");//JIS�ձ���׼
			break;
		case TWSS_JISB6:
			m_combo_standardsizes.InsertString(i,"JIS B6 (128mm x 182mm)");
			break;
		case TWSS_JISB7:
			m_combo_standardsizes.InsertString(i,"JIS B7 (91mm x 128mm)");
			break;
		case UDSCAP_LONGDOCUMENT:
			m_combo_standardsizes.InsertString(i,"��ֽģʽ");
			break;
		case TWSS_MAXSIZE:
			m_combo_standardsizes.InsertString(i,"���ɨ�跶Χ");
			break;
		default:
			continue;
		}
	}
	m_combo_standardsizes.SetCurSel(nCapIndex);  // ��ʾĬ��ֵ

	// �Զ�������
	TW_FRAME frameTemp;
	CString strTemp;

	frameTemp = m_pUI->GetCurrentFrame();
	strTemp.Format("%0.2f",FIX32ToFloat(frameTemp.Right)); //��
	SetDlgItemText(IDC_ADVANCED_EDIT_CUSTOMWIDTH, strTemp);

	frameTemp = m_pUI->GetCurrentFrame();
	strTemp.Format("%0.2f",FIX32ToFloat(frameTemp.Bottom)); //��
	SetDlgItemText(IDC_ADVANCED_EDIT_CUSTOMHEIGHT, strTemp);

	SetStandardsizes();

	//ֽ������-��λ
	m_combo_uints.ResetContent();  // �������
	nCapIndex = m_pUI->GetCurrentCapIndex(ICAP_UNITS);
	lstCapValues = m_pUI->GetValidCap(ICAP_UNITS);
	for(unsigned int i=0; i<lstCapValues->size();i++)
	{
		switch(lstCapValues->at(i))
		{
		case TWUN_INCHES:
			m_combo_uints.InsertString(i,"Inches"); //Ӣ��
			break;
		case TWUN_PIXELS:
			m_combo_uints.InsertString(i,"Pixels"); //����
			break;
		case TWUN_CENTIMETERS:
			m_combo_uints.InsertString(i,"Centimeters"); //����
			break;
		case TWUN_PICAS:
			m_combo_uints.InsertString(i,"Picas");
			break;
		case TWUN_POINTS:
			m_combo_uints.InsertString(i,"Points");
			break;
		case TWUN_TWIPS:
			m_combo_uints.InsertString(i,"Twips"); //�
			break;
		default:
			continue;
		}
	}
	m_combo_uints.SetCurSel(nCapIndex);

	//�������-��ֵ��
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

	//�������-�ֱ���
	m_combo_resolution.ResetContent(); //�������
	nCapIndex = m_pUI->GetCurrentCapIndex(ICAP_XRESOLUTION);
	lstCapValuesFlt = m_pUI->GetValidCapFloat(ICAP_XRESOLUTION);  //float�͵�
	for(unsigned int i=0; i<lstCapValuesFlt->size();i++)
	{
		CString strTemp;
		strTemp.Format("%d",(int)lstCapValuesFlt->at(i));
		m_combo_resolution.InsertString(i,strTemp);
	}
	m_combo_resolution.SetCurSel(nCapIndex);

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
		default:
			continue;
		}
	}
	m_combo_splitimage.SetCurSel(nCapIndex);

	//�������-�Աȶ�
	//nCapValue = (int)(m_pUI->GetCapValueFloat(ICAP_CONTRAST)); 
	nCapValue = m_pBasePage->contrast;
	m_slider_contrast.SetPos(nCapValue);
	strText.Format("%d",nCapValue);
	SetDlgItemText(IDC_ADVANCED_EDIT_CONTRAST, strText);

	//�������-����
	//nCapValue = (int)(m_pUI->GetCapValueFloat(ICAP_BRIGHTNESS)); 
	nCapValue = m_pBasePage->brightness;
	m_slider_brightness.SetPos(nCapValue);
	strText.Format("%d",nCapValue);
	//m_edit_brightness.SetWindowText(strText);
	SetDlgItemText(IDC_ADVANCED_EDIT_BRIGHTNESS, strText);
	
	CString str;
	GetDlgItemText(IDC_ADVANCED_STATIC_SENSITIVITY_THRESHOLD,str);
	if(str.Find("ȥ���ߵ�") >= 0)
	{
		//�������-ȥ���ߵ� 
		nCapValue = (int)(m_pUI->GetCapValueFloat(UDSCAP_SENSITIVETHRESHOLD_REMOVESPOTS)); 
		m_slider_sensitive_threshold.SetPos(nCapValue);
		strText.Format("%d",nCapValue);
		SetDlgItemText(IDC_ADVANCED_EDIT_SENSITIVE_THRESHOLD, strText);
	}
	else if(str.Find("��ɫ����") >= 0)
	{
		//�������-��ɫ����  ���ֵ����ֵ��ͬ��������
		//nCapValue = (int)(m_pUI->GetCapValueFloat(UDSCAP_SENSITIVETHRESHOLD_COLORRETENT)); 
		nCapValue = (int)(m_pUI->GetCapValueFloat(ICAP_THRESHOLD));
		m_slider_sensitive_threshold.SetPos(nCapValue);
		strText.Format("%d",nCapValue);
		SetDlgItemText(IDC_ADVANCED_EDIT_SENSITIVE_THRESHOLD, strText);
	}
	else{}//���뱣��
	
	//GammaУ�� 
	nCapValue = (int)(m_pUI->GetCapValueFloat(ICAP_GAMMA)); //GetCapValueFloat�ܷ�õ�CTWAINContainerFix32����
	m_slider_gamma.SetPos(nCapValue);
	float valueTemp = ((float)nCapValue)/100;
	strText.Format("%.2f", valueTemp);
	SetDlgItemText(IDC_ADVANCED_EDIT_SENSITIVE_GAMMA, strText);

	//ȥ���հ�ҳcheckbox
	nCapValue = (int)(m_pUI->GetCapValueBool(UDSCAP_REMOVEBLANK));
	m_check_removeblank.SetCheck(nCapValue);
	//ȥ���հ�ҳ -1�Զ�;-2������:��Ϊ������ 
	nCapValue = (int)(m_pUI->GetCapValueFloat(ICAP_AUTODISCARDBLANKPAGES)); 
	m_slider_removeblank.SetPos(nCapValue);
	strText.Format("%d",nCapValue);
	SetDlgItemText(IDC_ADVANCED_EDIT_REMOVEBLANK, strText);
	
	//ȥ�����׵�
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

	//���������Ĭ�ϲ�ʹ��
	nCapValue = (int)(m_pUI->GetCapValueBool(UDSCAP_MULTISTREAM));
	m_check_multistream.SetCheck(nCapValue);
	//��ɫ����
	nCapValue = (int)(m_pUI->GetCapValueBool(UDSCAP_FRONTCOLOR));
	m_check_frontcolor.SetCheck(nCapValue);
	//�Ҷ�����
	nCapValue = (int)(m_pUI->GetCapValueBool(UDSCAP_FRONTGRAY));
	m_check_frontgray.SetCheck(nCapValue);
	//�ڰ�����
	nCapValue = (int)(m_pUI->GetCapValueBool(UDSCAP_FRONTBW));
	m_check_frontbw.SetCheck(nCapValue);
	//��ɫ����
	nCapValue = (int)(m_pUI->GetCapValueBool(UDSCAP_BACKCOLOR));
	m_check_backcolor.SetCheck(nCapValue);
	//�Ҷȱ���
	nCapValue = (int)(m_pUI->GetCapValueBool(UDSCAP_BACKGRAY));
	m_check_backgray.SetCheck(nCapValue);
	//�ڰױ���
	nCapValue = (int)(m_pUI->GetCapValueBool(UDSCAP_BACKBW));
	m_check_backbw.SetCheck(nCapValue);
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

	GetCheckNum();

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


void CPage_Advanced::GetCheckNum(void)
{
	checknum = 0;
	if(m_check_frontcolor.GetCheck())
	{
		g_MuiltStream = g_MuiltStream|0x01; //�����ɫ
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
		g_MuiltStream = g_MuiltStream|0x02;  //����Ҷ�
		checknum++;
	}
	else
	{
		g_MuiltStream = g_MuiltStream&0xFD;
	}

	if(m_check_frontbw.GetCheck())
	{
		g_MuiltStream = g_MuiltStream|0x04;  //���汳��
		checknum++;
	}
	else
	{
		g_MuiltStream = g_MuiltStream&0xFB;
	}

	if(m_check_backcolor.GetCheck())
	{
		g_MuiltStream = g_MuiltStream|0x10;  //�����ɫ
		checknum++;
	}
	else
	{
		g_MuiltStream = g_MuiltStream&0xEF;
	}

	if(m_check_backgray.GetCheck())
	{
		g_MuiltStream = g_MuiltStream|0x20;  //����Ҷ�
		checknum++;
	}
	else
	{
		g_MuiltStream = g_MuiltStream&0xDF;
	}

	if(m_check_backbw.GetCheck())
	{
		g_MuiltStream = g_MuiltStream|0x40;  //����ڰ�
		checknum++;
	}
	else
	{
		g_MuiltStream = g_MuiltStream&0xBF;
	}
}

void CPage_Advanced::SetMultistream(void)
{
	if(m_check_multistream.GetCheck()) //ѡ��
	{
		GetDlgItem(IDC_CHECK_FRONTCOLOR)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_FRONTGRAY)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_FRONTBW)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_BACKCOLOR)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_BACKGRAY)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_BACKBW)->EnableWindow(TRUE);

		GetDlgItem(IDC_ADVANCED_COMBO_BINARIZATION)->EnableWindow(TRUE); //��ֵ��
		GetDlgItem(IDC_ADVANCED_COMBO_RESOLUTION)->EnableWindow(TRUE); //�ֱ���
		GetDlgItem(IDC_ADVANCED_SLIDER_SENSITIVE_THRESHOLD)->EnableWindow(TRUE); //ȥ���ߵ�
		GetDlgItem(IDC_ADVANCED_SLIDER_CONTRAST)->EnableWindow(TRUE); //�Աȶ�
		GetDlgItem(IDC_ADVANCED_SLIDER_BRIGHTNESS)->EnableWindow(TRUE); //����

		GetDlgItem(IDC_ADVANCED_EDIT_SENSITIVE_THRESHOLD)->EnableWindow(TRUE); //ȥ���ߵ�Edit
		GetDlgItem(IDC_ADVANCED_EDIT_CONTRAST)->EnableWindow(TRUE); //�Աȶ�Edit
		GetDlgItem(IDC_ADVANCED_EDIT_BRIGHTNESS)->EnableWindow(TRUE); //����Edit

		GetDlgItem(IDC_ADVANCED_COMBO_SPLITIMG)->EnableWindow(FALSE); //ͼ���ֲ�����
		
		((CButton*)GetDlgItem(IDC_CHECK_AUTOCROP))->SetCheck(FALSE); //�Զ�������У����ѡ��
		((CButton*)GetDlgItem(IDC_CHECK_REMOVEPUNCH))->SetCheck(FALSE); //ȥ�����ײ�ѡ��
		m_pUI->SetCapValueInt(UDSCAP_PUNCHHOLEREMOVEL,TWRP_DISABLE);
		m_pUI->SetCapValueInt(UDSCAP_AUTOCROP,TWAC_DISABLE);

		GetDlgItem(IDC_CHECK_AUTOCROP)->EnableWindow(FALSE); //�Զ�������У��������
		GetDlgItem(IDC_CHECK_REMOVEPUNCH)->EnableWindow(FALSE);//ȥ�����ײ�����

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
		//�������δѡ��ʱ������ѡ��Ҳ����Ҫѡ��
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

		GetDlgItem(IDC_ADVANCED_COMBO_BINARIZATION)->EnableWindow(FALSE); //��ֵ��
		GetDlgItem(IDC_ADVANCED_COMBO_RESOLUTION)->EnableWindow(FALSE); //�ֱ���
		GetDlgItem(IDC_ADVANCED_SLIDER_SENSITIVE_THRESHOLD)->EnableWindow(FALSE); //ȥ���ߵ�
		GetDlgItem(IDC_ADVANCED_SLIDER_BRIGHTNESS)->EnableWindow(FALSE); //����
		GetDlgItem(IDC_ADVANCED_SLIDER_CONTRAST)->EnableWindow(FALSE); //�Աȶ�

		GetDlgItem(IDC_ADVANCED_EDIT_SENSITIVE_THRESHOLD)->EnableWindow(FALSE); //ȥ���ߵ�Edit
		GetDlgItem(IDC_ADVANCED_EDIT_BRIGHTNESS)->EnableWindow(FALSE); //����Edit
		GetDlgItem(IDC_ADVANCED_EDIT_CONTRAST)->EnableWindow(FALSE); //�Աȶ�Edit

		GetDlgItem(IDC_ADVANCED_COMBO_SPLITIMG)->EnableWindow(TRUE); //ͼ���ֿ���
		GetDlgItem(IDC_CHECK_AUTOCROP)->EnableWindow(TRUE); //�Զ�������У������
		GetDlgItem(IDC_CHECK_REMOVEPUNCH)->EnableWindow(TRUE); //ȥ�����׿���

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
		if (strCBText.Find("��̬��ֵ") >= 0)
		{
			SetDlgItemText(IDC_ADVANCED_STATIC_SENSITIVITY_THRESHOLD, TEXT("ȥ���ߵ�:"));
			m_pUI->GetCapRangeFloat(UDSCAP_SENSITIVETHRESHOLD_REMOVESPOTS, fMin, fMax, fStep);
			m_slider_sensitive_threshold.SetRange((int)fMin, (int)fMax);
			m_slider_sensitive_threshold.SetTicFreq((int)fStep); 

			nCapValue = (int)(m_pUI->GetCapValueFloat(UDSCAP_SENSITIVETHRESHOLD_REMOVESPOTS));
			m_slider_sensitive_threshold.SetPos(nCapValue);

			//���ô�ʱ���Ȳ�����
			GetDlgItem(IDC_ADVANCED_SLIDER_BRIGHTNESS)->EnableWindow(FALSE); 
			GetDlgItem(IDC_ADVANCED_EDIT_BRIGHTNESS)->EnableWindow(FALSE);
		} 
		else if(strCBText.Find("�̶���ֵ") >= 0)
		{
			SetDlgItemText(IDC_ADVANCED_STATIC_SENSITIVITY_THRESHOLD, TEXT("��ɫ����:"));
			//m_pUI->GetCapRangeFloat(UDSCAP_SENSITIVETHRESHOLD_COLORRETENT, fMin, fMax, fStep);���ܵ�ͬ��ֵ
			m_pUI->GetCapRangeFloat(ICAP_THRESHOLD, fMin, fMax, fStep);
			m_slider_sensitive_threshold.SetRange((int)fMin, (int)fMax);
			m_slider_sensitive_threshold.SetTicFreq((int)fStep); 

			//nCapValue = (int)(m_pUI->GetCapValueFloat(UDSCAP_SENSITIVETHRESHOLD_COLORRETENT));
			nCapValue = (int)(m_pUI->GetCapValueFloat(ICAP_THRESHOLD));
			m_slider_sensitive_threshold.SetPos(nCapValue);

			//���ô�ʱ���ȿ���
			GetDlgItem(IDC_ADVANCED_SLIDER_BRIGHTNESS)->EnableWindow(TRUE); 
			GetDlgItem(IDC_ADVANCED_EDIT_BRIGHTNESS)->EnableWindow(TRUE);
		}
		else if(strCBText.Find("��ɫ��") >= 0 || strCBText.Find("�����ɢ") >= 0)
		{
			GetDlgItem(IDC_ADVANCED_SLIDER_SENSITIVE_THRESHOLD)->EnableWindow(FALSE); 
			GetDlgItem(IDC_ADVANCED_EDIT_SENSITIVE_THRESHOLD)->EnableWindow(FALSE); 

			//���ô�ʱ���ȿ���
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
	if (strCBText.Find("�Զ���") >= 0)
	{
		m_edit_custom_width.EnableWindow(TRUE);
		m_edit_custom_height.EnableWindow(TRUE);
		m_combo_uints.EnableWindow(TRUE);
	} 
	else
	{
		m_edit_custom_width.EnableWindow(FALSE);
		m_edit_custom_height.EnableWindow(FALSE);
		m_combo_uints.EnableWindow(FALSE); //��λ��ʱҲ������
	}
}


void CPage_Advanced::OnCbnSelchangeAdvanced_Combo_Standardsizes()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nIndex = m_combo_standardsizes.GetCurSel();
	CString strCBText; 
	m_combo_standardsizes.GetLBText( nIndex, strCBText);
	int nval;
	//int nval = _ttoi(strCBText);  // CString ת int
	if (strCBText.Find("�Զ���") >= 0)
	{
		nval = TWSS_NONE;
	}
	else if (strCBText.Find("US Letter") >= 0)
	{
		nval = TWSS_USLETTER;
	}
	else if (strCBText.Find("��Ƭ64") >= 0)
	{
		nval = TWSS_PHOT64;
	}
	else if (strCBText.Find("��Ƭ53") >= 0)
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
	else if (strCBText.Find("��ֽģʽ") >= 0)
	{
		nval = UDSCAP_LONGDOCUMENT;
	}
	else if (strCBText.Find("���ɨ��") >= 0)
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nIndex = m_combo_uints.GetCurSel();
	CString strCBText; 
	m_combo_uints.GetLBText( nIndex, strCBText);
	int nval;
	//int nval = _ttoi(strCBText);  ///< CString ת int
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



void CPage_Advanced::OnCbnSelchangeAdvanced_Combo_Resolution()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nIndex = m_combo_resolution.GetCurSel();
	CString strCBText; 
	m_combo_resolution.GetLBText(nIndex, strCBText);
	int nval = _ttoi(strCBText);  // CString ת int
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
	/*// ����Ӧ�ð�ťΪ����״̬
	SetModified(TRUE);*/
	return;
}


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
	//m_advancedmap.insert(map<int, float> :: value_type(ICAP_ROTATION, (float)nval));
	//m_pUI->SetCapValueInt(ICAP_ROTATION,nval);
	m_advancedmap[ICAP_ROTATION] = (float)nval;

	//UpdateControls();
	m_combo_rotate.SetCurSel(nIndex);
}


void CPage_Advanced::OnCbnSelchangeAdvanced_Combo_Binarization()
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
	//m_advancedmap.insert(map<int, float> :: value_type(UDSCAP_BINARIZATION, (float)nval));
	//m_pUI->SetCapValueInt(UDSCAP_BINARIZATION,nval); 
	m_advancedmap[UDSCAP_BINARIZATION] = (float)nval;

	SetBinarization();
	//UpdateControls();
	m_combo_binarization.SetCurSel(nIndex);
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SetMultistream();
}

void CPage_Advanced::InitSliderCtrl()
{
	
	float fMin,fMax,fStep;
	m_pUI->GetCapRangeFloat(ICAP_CONTRAST, fMin, fMax, fStep);
	m_slider_contrast.SetRange((int)fMin, (int)fMax);
	m_slider_contrast.SetTicFreq((int)fStep);  // ���û������̶ȵ�Ƶ��Ϊ1����λ������Ҫ����������们���ʼλ�ò���

	m_pUI->GetCapRangeFloat(ICAP_BRIGHTNESS, fMin, fMax, fStep);
	m_slider_brightness.SetRange((int)fMin, (int)fMax);
	m_slider_brightness.SetTicFreq((int)fStep);

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
	str.Format("%.2f", valueTemp); //С�����ֻҪ2λ
	SetDlgItemText(IDC_ADVANCED_EDIT_SENSITIVE_GAMMA, str);// �ڱ༭��ͬ����ʾ������ֵ
	UpdateData(FALSE);  // ���¿ؼ�
	//UpdateControls();

	/*// ����Ӧ�ð�ťΪ����״̬
	SetModified(TRUE);*/
	*pResult = 0;
}


void CPage_Advanced::OnNMCustomdrawAdvanced_Slider_SensitiveThreshold(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);  // ��������
	CString str;
	int sldValue = m_slider_sensitive_threshold.GetPos();  // ��ȡ���鵱ǰλ��
	//m_pUI->SetCapValueFloat(UDSCAP_SENSITIVETHRESHOLD_REMOVESPOTS,(float)sldValue);  
	//m_advancedmap.insert(map<int, float> :: value_type(UDSCAP_SENSITIVETHRESHOLD_REMOVESPOTS, (float)sldValue));
	//m_advancedmap[UDSCAP_SENSITIVETHRESHOLD_REMOVESPOTS] = (float)sldValue;
	m_advancedmap[ICAP_THRESHOLD] = (float)sldValue;

	str.Format("%d", sldValue);
	//m_edit_sensitive_threshold.SetWindowText(str);  // �ڱ༭��ͬ����ʾ������ֵ
	SetDlgItemText(IDC_ADVANCED_EDIT_SENSITIVE_THRESHOLD, str);
	// �ڱ༭��ͬ����ʾ������ֵ
	UpdateData(FALSE);  // ���¿ؼ�
	//UpdateControls();
	/*// ����Ӧ�ð�ťΪ����״̬
	SetModified(TRUE);*/
	*pResult = 0;
}


void CPage_Advanced::OnNMCustomdrawAdvanced_Slider_Brightness(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);  // ��������
	CString str;
	int sldValue = m_slider_brightness.GetPos();  // ��ȡ���鵱ǰλ��
	//m_advancedmap.insert(map<int, float> :: value_type(ICAP_BRIGHTNESS, (float)sldValue));
	//m_pUI->SetCapValueFloat(ICAP_BRIGHTNESS,(float)sldValue);  // ��������Ϊ��ǰ������ֵ
	m_advancedmap[ICAP_BRIGHTNESS] = (float)sldValue;

	str.Format("%d", sldValue);
	//m_edit_brightness.SetWindowText(str);  // �ڱ༭��ͬ����ʾ������ֵ
	SetDlgItemText(IDC_ADVANCED_EDIT_BRIGHTNESS, str);
	UpdateData(FALSE);  // ���¿ؼ�
	//UpdateControls();
	/*// ����Ӧ�ð�ťΪ����״̬
	SetModified(TRUE);*/
	*pResult = 0;
}


void CPage_Advanced::OnNMCustomdrawAdvanced_Slider_Contrast(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);  // ��������
	CString str;
	int sldValue = m_slider_contrast.GetPos();  // ��ȡ���鵱ǰλ��
	//m_advancedmap.insert(map<int, float> :: value_type(ICAP_CONTRAST, (float)sldValue));
	//m_pUI->SetCapValueFloat(ICAP_CONTRAST,(float)sldValue);  // ���öԱȶ�Ϊ��ǰ������ֵ
	m_advancedmap[ICAP_CONTRAST] = (float)sldValue;

	str.Format("%d", sldValue);
	//m_edit_contrast.SetWindowText(str);  // �ڱ༭��ͬ����ʾ������ֵ
	SetDlgItemText(IDC_ADVANCED_EDIT_CONTRAST, str);
	UpdateData(FALSE);  // ���¿ؼ�
	//UpdateControls();
	/*// ����Ӧ�ð�ťΪ����״̬
	SetModified(TRUE);*/
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

void CPage_Advanced::OnEnChangeAdvanced_Edit_Contrast()
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
	//m_advancedmap.insert(map<int, float> :: value_type(ICAP_CONTRAST, (float)nval));	
	//m_pUI->SetCapValueFloat(ICAP_CONTRAST,(float)nval);  // ���öԱȶ�Ϊ��ǰ������ֵ
	m_advancedmap[ICAP_CONTRAST] = (float)nval;

	m_edit_contrast.SetSel(str.GetLength(), str.GetLength(),TRUE);  // ���ñ༭��ؼ���Χ
	UpdateData(FALSE);  // ���¿ؼ�
	//UpdateControls();
	/*// ����Ӧ�ð�ťΪ����״̬
	SetModified(TRUE);*/
}



void CPage_Advanced::OnEnChangeAdvanced_Edit_SensitiveThreshold()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CPropertyPage::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);  // ��������
	CString str;
	m_edit_sensitive_threshold.GetWindowText(str);
	int nval = _ttoi(str);
	m_slider_sensitive_threshold.SetPos(nval);
	//m_advancedmap.insert(map<int, float> :: value_type(UDSCAP_SENSITIVETHRESHOLD_REMOVESPOTS, (float)nval));	
	//m_pUI->SetCapValueFloat(UDSCAP_SENSITIVETHRESHOLD_REMOVESPOTS,(float)nval);  
	//m_advancedmap[UDSCAP_SENSITIVETHRESHOLD_REMOVESPOTS] = (float)nval;
	m_advancedmap[ICAP_THRESHOLD] = (float)nval;

	m_edit_sensitive_threshold.SetSel(str.GetLength(), str.GetLength(),TRUE);  // ���ñ༭��ؼ���Χ
	UpdateData(FALSE);  // ���¿ؼ�
	//UpdateControls();

	/*// ����Ӧ�ð�ťΪ����״̬
	SetModified(TRUE);*/
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

	//int nval = _ttoi(str);
	float fval = (float)(_ttof(str));
	int nval = (int)(100*fval);
	m_slider_gamma.SetPos(nval);
	//m_advancedmap.insert(map<int, float> :: value_type(ICAP_GAMMA, (float)nval));
	//m_pUI->SetCapValueFloat(ICAP_GAMMA,(float)nval);  // ���öԱȶ�Ϊ��ǰ������ֵ
	m_advancedmap[ICAP_GAMMA] = float(nval); //map����ǷŴ�100����ֵ

	/*
	CString strTemp;
	strTemp.Format("%.2f",(float)nval);
	::MessageBox(NULL,TEXT(strTemp),"edit",MB_OK);*/

	m_edit_gamma.SetSel(str.GetLength(), str.GetLength(),TRUE);  // ���ñ༭��ؼ���Χ
	UpdateData(FALSE);  // ���¿ؼ�
	//UpdateControls();

	/*// ����Ӧ�ð�ťΪ����״̬
	SetModified(TRUE);*/
}


void CPage_Advanced::OnEnChangeAdvanced_Edit_Brightness()
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
	//m_advancedmap.insert(map<int, float> :: value_type(ICAP_BRIGHTNESS, (float)nval));
	//m_pUI->SetCapValueFloat(ICAP_BRIGHTNESS,(float)nval);  // ���öԱȶ�Ϊ��ǰ������ֵ
	m_advancedmap[ICAP_BRIGHTNESS] = (float)nval;

	m_edit_brightness.SetSel(str.GetLength(), str.GetLength(),TRUE);  // ���ñ༭��ؼ���Χ
	UpdateData(FALSE);  // ���¿ؼ�
	//UpdateControls();

	/*// ����Ӧ�ð�ťΪ����״̬
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
		//ͼ����ǿϵ�в�����
		GetDlgItem(IDC_CHECK_REMOVEDEMOISE)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_REMOVEDESCREEN)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_SHARPEN)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_REMOVEBACK)->EnableWindow(FALSE);
	} 
	else
	{
		GetDlgItem(IDC_ADVANCED_COMBO_BINARIZATION)->EnableWindow(FALSE);
		//ͼ����ǿϵ�п���
		GetDlgItem(IDC_CHECK_REMOVEDEMOISE)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_REMOVEDESCREEN)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_SHARPEN)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_REMOVEBACK)->EnableWindow(TRUE);
	}
	SetBinarization(); //���á�ȥ���ߵ㡱���ǡ���ɫ������
}

//��ɫ����
void CPage_Advanced::OnAdvanced_Btn_Check_FrontColor()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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

//�Ҷ�����
void CPage_Advanced::OnAdvanced_Btn_Check_FrontGray()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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

//�ڰ�����
void CPage_Advanced::OnAdvanced_Btn_Check_FrontBW()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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

//��ɫ����
void CPage_Advanced::OnAdvanced_Btn_Check_BackColor()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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

//�Ҷȱ���
void CPage_Advanced::OnAdvanced_Btn_Check_BackGray()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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

//�ڰױ���
void CPage_Advanced::OnAdvanced_Btn_Check_BackBW()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
		//AfxMessageBox("ȥ������ѡ��");
		nval = TWRP_AUTO;
	} 
	else
	{
		//AfxMessageBox("ȥ������δѡ��");
		nval = TWRP_DISABLE;
	}
	//m_pUI->SetCapValueInt(UDSCAP_PUNCHHOLEREMOVEL,nval); 
	//m_advancedmap.insert(map<int, float> :: value_type(UDSCAP_PUNCHHOLEREMOVEL, (float)nval));
	m_advancedmap[UDSCAP_PUNCHHOLEREMOVEL] = (float)nval;

	/*CString str;
	str.Format("%d",nval);
	AfxMessageBox(str);*/
}


//��ͼ��
void CPage_Advanced::OnAdvanced_Btn_Check_Sharpen()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nval;
	if (m_check_sharpen.GetCheck())
	{
		//AfxMessageBox("��ͼ��ѡ��");
		nval = TWSP_AUTO;
	} 
	else
	{
		//AfxMessageBox("��ͼ��δѡ��");
		nval = TWSP_DISABLE;
	} 
	//m_pUI->SetCapValueInt(UDSCAP_SHARPEN,nval); 
	//m_advancedmap.insert(map<int, float> :: value_type(UDSCAP_SHARPEN, (float)nval));
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
	//m_pUI->SetCapValueInt(UDSCAP_MIRROR,nval); 
	//m_advancedmap.insert(map<int, float> :: value_type(UDSCAP_MIRROR, (float)nval));
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
	//m_pUI->SetCapValueInt(UDSCAP_REMOVEBACKGROUND,nval); 
	//m_advancedmap.insert(map<int, float> :: value_type(UDSCAP_REMOVEBACKGROUND, (float)nval));
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
	//m_pUI->SetCapValueInt(UDSCAP_DESCREEN,nval); 
	//m_advancedmap.insert(map<int, float> :: value_type(UDSCAP_DESCREEN, (float)nval));
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
	//m_pUI->SetCapValueInt(UDSCAP_DENOISE,nval);
	//m_advancedmap.insert(map<int, float> :: value_type(UDSCAP_DENOISE, (float)nval));
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
	//m_pUI->SetCapValueInt(UDSCAP_AUTOCROP,nval);
	//m_advancedmap.insert(map<int, float> :: value_type(UDSCAP_AUTOCROP, (float)nval));
	m_advancedmap[UDSCAP_AUTOCROP] = (float)nval;
}


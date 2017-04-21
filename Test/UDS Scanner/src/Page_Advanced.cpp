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
}

CPage_Advanced::~CPage_Advanced()
{
	m_advancedmap.swap(map<int, float>());  // ��ղ��ͷ��ڴ�
}

void CPage_Advanced::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_REMOVEPUNCH, m_check_removepunch);
	DDX_Control(pDX, IDC_CHECK_SHARPEN, m_check_sharpen);
	DDX_Control(pDX, IDC_CHECK_MIRROR, m_check_mirror);
	DDX_Control(pDX, IDC_CHECK_REMOVEBACK, m_check_removeback);
	DDX_Control(pDX, IDC_CHECK_REMOVEDESCREEN, m_check_removedescreen);
	DDX_Control(pDX, IDC_CHECK_REMOVEDEMOISE, m_check_removedenoise);
	DDX_Control(pDX, IDC_CHECK_MULTIFEEDDETECT, m_check_multifeeddetect);
	DDX_Control(pDX, IDC_ADVANCED_CHECK_COLORFLIP, m_check_colorflip);
	DDX_Control(pDX, IDC_CHECK_MULTIFEEDDETECT_VALUE, m_check_mdvalue);
	DDX_Control(pDX, IDC_ADVANCED_COMBO_STANDARDSIZES, m_combo_standardsizes);
	DDX_Control(pDX, IDC_ADVANCED_COMBO_UINT, m_combo_uints);
	DDX_Control(pDX, IDC_ADVANCED_SCROLLBAR_HEIGHT, m_scroll_height);
	DDX_Control(pDX, IDC_ADVANCED_SCROLLBAR_DOWN, m_scroll_down);
	DDX_Control(pDX, IDC_ADVANCED_SCROLLBAR_LEFT, m_scroll_left);
	DDX_Control(pDX, IDC_ADVANCED_SCROLLBAR_RIGHT, m_scroll_right);
	DDX_Control(pDX, IDC_ADVANCED_SCROLLBAR_UP, m_scroll_up);
	DDX_Control(pDX, IDC_ADVANCED_SCROLLBAR_WIDTH, m_scroll_width);
	DDX_Control(pDX, IDC_ADVANCED_EDIT_WIDTH, m_edit_width);
	DDX_Control(pDX, IDC_ADVANCED_EDIT_HEIGHT, m_edit_height);
	DDX_Control(pDX, IDC_ADVANCED_EDIT_LEFT, m_edit_left);
	DDX_Control(pDX, IDC_ADVANCED_EDIT_DOWN, m_edit_down);
	DDX_Control(pDX, IDC_ADVANCED_EDIT_RIGHT, m_edit_right);
	DDX_Control(pDX, IDC_ADVANCED_EDIT_UP, m_edit_up);
	DDX_Control(pDX, IDC_ADVANCED_EDIT_XPOS, m_edit_xpos);
	DDX_Control(pDX, IDC_ADVANCED_EDIT_YPOS, m_edit_ypos);
	DDX_Control(pDX, IDC_ADVANCED_SCROLLBAR_XPOS, m_scroll_xpos);
	DDX_Control(pDX, IDC_ADVANCED_SCROLLBAR_YPOS, m_scroll_ypos);
	DDX_Control(pDX, IDC_ADVANCED_COMBO_CUTMETHOD, m_combo_cutmethod);
	DDX_Radio(pDX, IDC_ADVANCED_RADIO_EDGECOLOR_WHITE, m_radio_edgecolor);
}


BEGIN_MESSAGE_MAP(CPage_Advanced, CPropertyPage)
	ON_BN_CLICKED(IDC_CHECK_REMOVEPUNCH, &CPage_Advanced::OnAdvanced_Btn_Check_RemovePunch)
	ON_BN_CLICKED(IDC_CHECK_SHARPEN, &CPage_Advanced::OnAdvanced_Btn_Check_Sharpen)
	ON_BN_CLICKED(IDC_CHECK_MIRROR, &CPage_Advanced::OnAdvanced_Btn_Check_Mirror)
	ON_BN_CLICKED(IDC_CHECK_REMOVEBACK, &CPage_Advanced::OnAdvanced_Btn_Check_RemoveBack)
	ON_BN_CLICKED(IDC_CHECK_REMOVEDEMOISE, &CPage_Advanced::OnAdvanced_Btn_Check_RemoveDenoise)
	ON_BN_CLICKED(IDC_CHECK_REMOVEDESCREEN, &CPage_Advanced::OnAdvanced_Btn_Check_RemoveDescreen)
	ON_BN_CLICKED(IDC_CHECK_MULTIFEEDDETECT, &CPage_Advanced::OnClicked_Check_Multifeeddetect)
	ON_BN_CLICKED(IDC_ADVANCED_CHECK_COLORFLIP, &CPage_Advanced::OnAdvanced_Btn_Check_Colorflip)
	ON_BN_CLICKED(IDC_CHECK_MULTIFEEDDETECT_VALUE, &CPage_Advanced::OnClicked_Check_MdValue)
	ON_CBN_SELCHANGE(IDC_ADVANCED_COMBO_STANDARDSIZES, &CPage_Advanced::OnCbnSelchangeAdvanced_Combo_Standardsizes)
	ON_CBN_SELCHANGE(IDC_ADVANCED_COMBO_UINT, &CPage_Advanced::OnCbnSelchangeAdvanced_Combo_Uints)
	ON_WM_VSCROLL()
	ON_EN_CHANGE(IDC_ADVANCED_EDIT_XPOS, &CPage_Advanced::OnEnChangeAdvanced_Edit_EdgeXpos)
	ON_EN_CHANGE(IDC_ADVANCED_EDIT_YPOS, &CPage_Advanced::OnEnChangeAdvanced_Edit_EdgeYpos)
	ON_EN_CHANGE(IDC_ADVANCED_EDIT_UP, &CPage_Advanced::OnEnChangeAdvanced_Edit_EdgeUp)
	ON_EN_CHANGE(IDC_ADVANCED_EDIT_LEFT, &CPage_Advanced::OnEnChangeAdvanced_Edit_EdgeLeft)
	ON_EN_CHANGE(IDC_ADVANCED_EDIT_RIGHT, &CPage_Advanced::OnEnChangeAdvanced_Edit_EdgeRight)
	ON_EN_CHANGE(IDC_ADVANCED_EDIT_DOWN, &CPage_Advanced::OnEnChangeAdvanced_Edit_EdgeDown)
	ON_CBN_SELCHANGE(IDC_ADVANCED_COMBO_CUTMETHOD, &CPage_Advanced::OnCbnSelchangeAdvanced_Combo_Cutmethod)
	ON_BN_CLICKED(IDC_ADVANCED_RADIO_EDGECOLOR_WHITE, &CPage_Advanced::OnAdvanced_RadioBtn_Edgecolor)
	ON_BN_CLICKED(IDC_ADVANCED_RADIO_EDGECOLOR_BLACK, &CPage_Advanced::OnAdvanced_RadioBtn_Edgecolor)
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
		case ICAP_SUPPORTEDSIZES:  //ֽ�Ŵ�С //����˴����ã������ڻص�Ĭ��ģ��ʱ��Ĭ��ģ���ж�Ӧ����USletter��С
			{
				m_pUI->SetCapValueInt(iter->first,iter->second); 
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
	float fCapValue;
	
	// ͼ����з�ʽ 
	m_combo_cutmethod.ResetContent();
	nCapIndex = m_pUI->GetCurrentCapIndex(UDSCAP_CUTMETHOD);
	lstCapValues = m_pUI->GetValidCap(UDSCAP_CUTMETHOD);
	for(unsigned int i=0; i<lstCapValues->size();i++)
	{
		switch(lstCapValues->at(i))
		{
		case TWCT_NONE:
			m_combo_cutmethod.InsertString(i,"������");
			break;
		case TWCT_AUTO:
			m_combo_cutmethod.InsertString(i,"�Զ��������ƫ");
			break;
		case TWCT_SPECIFY:
			m_combo_cutmethod.InsertString(i,"ָ�����ȡ����");
			break;
		default:
			continue;
		}
	}
	m_combo_cutmethod.SetCurSel(nCapIndex);

	//���ż�⣺Ĭ��ʹ��
	nCapValue = (int)(m_pUI->GetCapValueBool(UDSCAP_MULTIFEEDDETECT));
	m_check_multifeeddetect.SetCheck(nCapValue);
	if(nCapValue == 1)
	{
		m_check_mdvalue.EnableWindow(TRUE);
	}
	else
	{
		m_check_mdvalue.EnableWindow(FALSE);
	}
	//���ż�����ֵ
	nCapValue = (int)(m_pUI->GetCapValueBool(UDSCAP_MULTIFEEDDETECT_VALUE));
	m_check_mdvalue.SetCheck(nCapValue);
	
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

	nCapValue = (int)(m_pUI->GetCapValueBool(UDSCAP_COLORFLIP));
	m_check_colorflip.SetCheck(nCapValue);

	//V2.0�汾
	//ֽ������-��λ
	m_combo_uints.ResetContent();  // �������
	nCapIndex = m_pUI->GetCurrentCapIndex(ICAP_UNITS);
	lstCapValues = m_pUI->GetValidCap(ICAP_UNITS);
	for(unsigned int i=0; i<lstCapValues->size();i++)
	{
		switch(lstCapValues->at(i))
		{
		case TWUN_INCHES:
			m_combo_uints.InsertString(i,"Ӣ��Inches"); //Ӣ��
			break;
		case TWUN_PIXELS:
			m_combo_uints.InsertString(i,"����Pixels"); //����
			break;
		case TWUN_CENTIMETERS:
			m_combo_uints.InsertString(i,"����Centimeters"); //����
			break;
		default:
			continue;
		}
	}
	m_combo_uints.SetCurSel(nCapIndex);
	nval = FindUnit(nCapIndex);

	SetScroll();
	// �Զ�������
	TW_FRAME frame;	
	int nIndex = m_combo_uints.GetCurSel();
	//��λΪinches��centiʱedit��ʾ��λС��������ֱ����ʾ����
	switch(FindUnit(nIndex)) //��ǰ����ĵ�λ
	{
	case TWUN_INCHES:
		{
			frame = m_pUI->GetCurrentFrame();
			strText.Format("%0.2f",FIX32ToFloat(frame.Right));
			SetDlgItemText(IDC_ADVANCED_EDIT_WIDTH, strText);

			frame = m_pUI->GetCurrentFrame();
			strText.Format("%0.2f",FIX32ToFloat(frame.Bottom));
			SetDlgItemText(IDC_ADVANCED_EDIT_HEIGHT, strText);

			//��Ե���� ��������
			fCapValue = m_pUI->GetCapValueFloat(UDSCAP_EDGE_UP);
			if(fCapValue > (float)maxinches_updown)
			{
				fCapValue = (float)maxinches_updown;
			}
			else if(fCapValue < 0.00)
			{
				fCapValue = 0.00;
			}
			else{}	
			strText.Format("%0.2f",fCapValue);
			SetDlgItemText(IDC_ADVANCED_EDIT_UP,strText);

			fCapValue = m_pUI->GetCapValueFloat(UDSCAP_EDGE_DOWN);
			if(fCapValue > (float)maxinches_updown)
			{
				fCapValue = (float)maxinches_updown;
			}
			else if(fCapValue < 0.00)
			{
				fCapValue = 0.00;
			}
			else{}	
			strText.Format("%0.2f",fCapValue);
			SetDlgItemText(IDC_ADVANCED_EDIT_DOWN,strText);

			fCapValue = m_pUI->GetCapValueFloat(UDSCAP_EDGE_LEFT);
			if(fCapValue > (float)maxinches_leftright)
			{
				fCapValue = (float)maxinches_leftright;
			}
			else if(fCapValue < 0.00)
			{
				fCapValue = 0.00;
			}
			else{}	
			strText.Format("%0.2f",fCapValue);
			SetDlgItemText(IDC_ADVANCED_EDIT_LEFT,strText);

			fCapValue = m_pUI->GetCapValueFloat(UDSCAP_EDGE_RIGHT);
			if(fCapValue > (float)maxinches_leftright)
			{
				fCapValue = (float)maxinches_leftright;
			}
			else if(fCapValue < 0.00)
			{
				fCapValue = 0.00;
			}
			else{}	
			strText.Format("%0.2f",fCapValue);
			SetDlgItemText(IDC_ADVANCED_EDIT_RIGHT,strText);

			//XYƫ����
			fCapValue = m_pUI->GetCapValueFloat(UDSCAP_XPOS);
			if(fCapValue > (float)maxinches_xpos)
			{
				fCapValue = (float)maxinches_xpos;
			}
			else if(fCapValue < 0.00)
			{
				fCapValue = 0.00;
			}
			else{}		
			strText.Format("%0.2f",fCapValue);
			SetDlgItemText(IDC_ADVANCED_EDIT_XPOS,strText);

			fCapValue = m_pUI->GetCapValueFloat(UDSCAP_YPOS);
			if(fCapValue > (float)maxinches_ypos)
			{
				fCapValue = (float)maxinches_ypos;
			}
			else if(fCapValue < 0.00)
			{
				fCapValue = 0.00;
			}
			else{}		
			strText.Format("%0.2f",fCapValue);
			SetDlgItemText(IDC_ADVANCED_EDIT_YPOS,strText);

			break;
		}
	case TWUN_CENTIMETERS:
		{
			frame = m_pUI->GetCurrentFrame();
			strText.Format("%0.2f",FIX32ToFloat(frame.Right));
			SetDlgItemText(IDC_ADVANCED_EDIT_WIDTH, strText);

			frame = m_pUI->GetCurrentFrame();
			strText.Format("%0.2f",FIX32ToFloat(frame.Bottom));
			SetDlgItemText(IDC_ADVANCED_EDIT_HEIGHT, strText);

			//��Ե���� ��������
			float fvalue;
			fCapValue = m_pUI->GetCapValueFloat(UDSCAP_EDGE_UP);
			fvalue = ConvertUnits(fCapValue, TWUN_INCHES, TWUN_CENTIMETERS, m_resolution);
			if(fvalue > (float)maxinches_updown)
			{
				fvalue = (float)maxinches_updown;
			}
			else if(fvalue < 0.00)
			{
				fvalue = 0.00;
			}
			else{}	
			strText.Format("%0.2f",fvalue);
			SetDlgItemText(IDC_ADVANCED_EDIT_UP,strText);

			fCapValue = m_pUI->GetCapValueFloat(UDSCAP_EDGE_DOWN);
			fvalue = ConvertUnits(fCapValue, TWUN_INCHES, TWUN_CENTIMETERS, m_resolution);
			if(fvalue > (float)maxinches_updown)
			{
				fvalue = (float)maxinches_updown;
			}
			else if(fvalue < 0.00)
			{
				fvalue = 0.00;
			}
			else{}	
			strText.Format("%0.2f",fvalue);
			SetDlgItemText(IDC_ADVANCED_EDIT_DOWN,strText);

			fCapValue = m_pUI->GetCapValueFloat(UDSCAP_EDGE_LEFT);
			fvalue = ConvertUnits(fCapValue, TWUN_INCHES, TWUN_CENTIMETERS, m_resolution);
			if(fvalue > (float)maxinches_leftright)
			{
				fvalue = (float)maxinches_leftright;
			}
			else if(fvalue < 0.00)
			{
				fvalue = 0.00;
			}
			else{}	
			strText.Format("%0.2f",fvalue);
			SetDlgItemText(IDC_ADVANCED_EDIT_LEFT,strText);

			fCapValue = m_pUI->GetCapValueFloat(UDSCAP_EDGE_RIGHT);
			fvalue = ConvertUnits(fCapValue, TWUN_INCHES, TWUN_CENTIMETERS, m_resolution);
			if(fvalue > (float)maxinches_leftright)
			{
				fvalue = (float)maxinches_leftright;
			}
			else if(fvalue < 0.00)
			{
				fvalue = 0.00;
			}
			else{}	
			strText.Format("%0.2f",fvalue);
			SetDlgItemText(IDC_ADVANCED_EDIT_RIGHT,strText);

			//XYƫ����
			fCapValue = m_pUI->GetCapValueFloat(UDSCAP_XPOS);
			fvalue = ConvertUnits(fCapValue, TWUN_INCHES, TWUN_CENTIMETERS, m_resolution);
			if(fvalue > (float)maxinches_xpos)
			{
				fvalue = (float)maxinches_xpos;
			}
			else if(fvalue < 0.00)
			{
				fvalue = 0.00;
			}
			else{}	
			strText.Format("%0.2f",fvalue);
			SetDlgItemText(IDC_ADVANCED_EDIT_XPOS,strText);

			fCapValue = m_pUI->GetCapValueFloat(UDSCAP_YPOS);
			fvalue = ConvertUnits(fCapValue, TWUN_INCHES, TWUN_CENTIMETERS, m_resolution);
			if(fvalue > (float)maxinches_ypos)
			{
				fvalue = (float)maxinches_ypos;
			}
			else if(fvalue < 0.00)
			{
				fvalue = 0.00;
			}
			else{}	
			strText.Format("%0.2f",fvalue);
			SetDlgItemText(IDC_ADVANCED_EDIT_YPOS,strText);

			break;
		}
	case TWUN_PIXELS:
		{
			frame = m_pUI->GetCurrentFrame();
			strText.Format("%d",(int)FIX32ToFloat(frame.Right));
			SetDlgItemText(IDC_ADVANCED_EDIT_WIDTH, strText);

			frame = m_pUI->GetCurrentFrame();
			strText.Format("%d",(int)FIX32ToFloat(frame.Bottom));
			SetDlgItemText(IDC_ADVANCED_EDIT_HEIGHT, strText);

			//��Ե���� ��������
			int nvalue;
			fCapValue = m_pUI->GetCapValueFloat(UDSCAP_EDGE_UP);
			nvalue = ConvertUnits(fCapValue, TWUN_INCHES, TWUN_PIXELS, m_resolution);
			if(nvalue > maxinches_updown)
			{
				nvalue = maxinches_updown;
			}
			else if(nvalue < 0)
			{
				nvalue = 0;
			}
			else{}	
			strText.Format("%d",nvalue);
			SetDlgItemText(IDC_ADVANCED_EDIT_UP,strText);

			fCapValue = m_pUI->GetCapValueFloat(UDSCAP_EDGE_DOWN);
			nvalue = ConvertUnits(fCapValue, TWUN_INCHES, TWUN_PIXELS, m_resolution);
			if(nvalue > maxinches_updown)
			{
				nvalue = maxinches_updown;
			}
			else if(nvalue < 0)
			{
				nvalue = 0;
			}
			else{}	
			strText.Format("%d",nvalue);
			SetDlgItemText(IDC_ADVANCED_EDIT_DOWN,strText);

			fCapValue = m_pUI->GetCapValueFloat(UDSCAP_EDGE_LEFT);
			nvalue = ConvertUnits(fCapValue, TWUN_INCHES, TWUN_PIXELS, m_resolution);
			if(nvalue > maxinches_leftright)
			{
				nvalue = maxinches_leftright;
			}
			else if(nvalue < 0)
			{
				nvalue = 0;
			}
			else{}	
			strText.Format("%d",nvalue);
			SetDlgItemText(IDC_ADVANCED_EDIT_LEFT,strText);

			fCapValue = m_pUI->GetCapValueFloat(UDSCAP_EDGE_RIGHT);
			nvalue = ConvertUnits(fCapValue, TWUN_INCHES, TWUN_PIXELS, m_resolution);
			if(nvalue > maxinches_leftright)
			{
				nvalue = maxinches_leftright;
			}
			else if(nvalue < 0)
			{
				nvalue = 0;
			}
			else{}	
			strText.Format("%d",nvalue);
			SetDlgItemText(IDC_ADVANCED_EDIT_RIGHT,strText);

			fCapValue = m_pUI->GetCapValueFloat(UDSCAP_XPOS);
			nvalue = ConvertUnits(fCapValue, TWUN_INCHES, TWUN_PIXELS, m_resolution);
			if(nvalue > maxinches_xpos)
			{
				nvalue = maxinches_xpos;
			}
			else if(nvalue < 0)
			{
				nvalue = 0;
			}
			else{}	
			strText.Format("%d",nvalue);
			SetDlgItemText(IDC_ADVANCED_EDIT_XPOS,strText);

			fCapValue = m_pUI->GetCapValueFloat(UDSCAP_YPOS);
			nvalue = ConvertUnits(fCapValue, TWUN_INCHES, TWUN_PIXELS, m_resolution);
			if(nvalue > maxinches_ypos)
			{
				nvalue = maxinches_ypos;
			}
			else if(nvalue < 0)
			{
				nvalue = 0;
			}
			else{}	
			strText.Format("%d",nvalue);
			SetDlgItemText(IDC_ADVANCED_EDIT_YPOS,strText);

			break;
		}
	}

	//ֽ������-ֽ�Ŵ�С
	m_combo_standardsizes.ResetContent();  // �������
	nCapIndex = m_pUI->GetCurrentCapIndex(ICAP_SUPPORTEDSIZES);
	lstCapValues = m_pUI->GetValidCap(ICAP_SUPPORTEDSIZES);
	for(unsigned int i=0; i<lstCapValues->size();i++)
	{
		switch(lstCapValues->at(i))
		{
		case TWSS_NONE:
			m_combo_standardsizes.InsertString(i,"�Զ�");
			break;
		case TWSS_USLETTER:
			m_combo_standardsizes.InsertString(i,"US Letter (8.5\" x 11\")");  //216mm x 280mm
			break;
		case TWSS_USLEGAL:
			m_combo_standardsizes.InsertString(i,"US Legal (8.5\" x 14\")");  //216mm x 356mm
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
		case TWSS_ISOB5:
			m_combo_standardsizes.InsertString(i,"ISO B5 (176mm x 250mm)");
			break;
		case TWSS_ISOB6:
			m_combo_standardsizes.InsertString(i,"ISO B6 (125mm x 176mm)");
			break;
		case TWSS_ISOB7:
			m_combo_standardsizes.InsertString(i,"ISO B7 (88mm x 125mm)");
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
	m_combo_standardsizes.SetCurSel(nCapIndex);  //��ʾĬ��ֵ
	nval = FindPaperSize(nCapIndex);
	m_advancedmap[ICAP_SUPPORTEDSIZES] = (float)nval;//����ֻ������������Ҫ����CAP;�˴�������
	SetPaperSize();

	//�����ɫ
	nCapIndex = m_pUI->GetCurrentCapIndex(UDSCAP_EDGE_COLOR);
	m_radio_edgecolor = nCapIndex;

}


// CPage_Advanced ��Ϣ�������
BOOL CPage_Advanced::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	UpdateControls();
	SetMultistream();
	
	const FloatVector* lstCapValuesFlt = m_pUI->GetValidCapFloat(ICAP_XRESOLUTION);
	int nCapIndex = m_pUI->GetCurrentCapIndex(ICAP_XRESOLUTION);
	m_resolution = (int)lstCapValuesFlt->at(nCapIndex); //x�ֱ���

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
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
	m_pUI->SetCapValueInt(UDSCAP_PUNCHHOLEREMOVEL,nval);
}


//��ͼ��
void CPage_Advanced::OnAdvanced_Btn_Check_Sharpen()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nval;
	if (m_check_sharpen.GetCheck())
	{
		nval = TWSN_AUTO;
	} 
	else
	{
		nval = TWSN_DISABLE;
	} 
	m_pUI->SetCapValueInt(UDSCAP_SHARPEN,nval);
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
	m_pUI->SetCapValueInt(UDSCAP_MIRROR,nval);
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
	m_pUI->SetCapValueInt(UDSCAP_REMOVEBACKGROUND,nval);
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
	m_pUI->SetCapValueInt(UDSCAP_DESCREEN,nval);
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
	m_pUI->SetCapValueInt(UDSCAP_DENOISE,nval);
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
	m_pUI->SetCapValueInt(UDSCAP_COLORFLIP,nval);
}


//PreTranslateMessage����TRUEʱ���������Ϣ���͸����ں�������
BOOL CPage_Advanced::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	CEdit* pEdit[10];
	pEdit[0] = (CEdit*)GetDlgItem(IDC_ADVANCED_EDIT_WIDTH);   
	pEdit[1] = (CEdit*)GetDlgItem(IDC_ADVANCED_EDIT_HEIGHT);  
	pEdit[2] = (CEdit*)GetDlgItem(IDC_ADVANCED_EDIT_XPOS);
	pEdit[3] = (CEdit*)GetDlgItem(IDC_ADVANCED_EDIT_YPOS);  
	pEdit[4] = (CEdit*)GetDlgItem(IDC_ADVANCED_EDIT_UP);  
	pEdit[5] = (CEdit*)GetDlgItem(IDC_ADVANCED_EDIT_DOWN);
	pEdit[6] = (CEdit*)GetDlgItem(IDC_ADVANCED_EDIT_LEFT);  
	pEdit[7] = (CEdit*)GetDlgItem(IDC_ADVANCED_EDIT_RIGHT); 

	CString str[10];   
	GetDlgItemText(IDC_ADVANCED_EDIT_WIDTH, str[0]); // ��ȡedit���ı�  
	GetDlgItemText(IDC_ADVANCED_EDIT_HEIGHT, str[1]);
	GetDlgItemText(IDC_ADVANCED_EDIT_XPOS, str[2]);
	GetDlgItemText(IDC_ADVANCED_EDIT_YPOS, str[3]);  
	GetDlgItemText(IDC_ADVANCED_EDIT_UP, str[4]);
	GetDlgItemText(IDC_ADVANCED_EDIT_DOWN, str[5]);
	GetDlgItemText(IDC_ADVANCED_EDIT_LEFT, str[6]); 
	GetDlgItemText(IDC_ADVANCED_EDIT_RIGHT, str[7]);

	int nPos[8] = {0}; 
	for(int i = 0; i < 8; i++)
	{
		nPos[i] = str[i].Find('.'); // ����.��λ�� 
	}

	if(GetFocus() == pEdit[0] && (pMsg->message == WM_CHAR))
	{
		if(pMsg->wParam == '.')
		{
			//�����ֻ��������һ��С����
			if(nPos[0] >= 0)  //����ֿ�д����||�����Ļ��ܻ�����
			{  
				return 1;   //�������,����,��������������
			}	
			return 0;
		}
		//������������
		else if((pMsg->wParam >= '0' && pMsg->wParam <= '9'))   
		{  
			return 0;  
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

	if(GetFocus() == pEdit[1] && (pMsg->message == WM_CHAR))
	{
		if(pMsg->wParam == '.')
		{
			//�����ֻ��������һ��С����
			if(nPos[1] >= 0)  //����ֿ�д����||�����Ļ��ܻ�����
			{  
				return 1;   //�������,����,��������������
			}	
			return 0;
		}
		//������������
		else if((pMsg->wParam >= '0' && pMsg->wParam <= '9'))   
		{  
			return 0;  
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

	if(GetFocus() == pEdit[2] && (pMsg->message == WM_CHAR))
	{
		if(pMsg->wParam == '.')
		{
			//�����ֻ��������һ��С����
			if(nPos[2] >= 0)  //����ֿ�д����||�����Ļ��ܻ�����
			{  
				return 1;   //�������,����,��������������
			}	
			return 0;
		}
		//������������
		else if((pMsg->wParam >= '0' && pMsg->wParam <= '9'))   
		{  
			return 0;  
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

	if(GetFocus() == pEdit[3] && (pMsg->message == WM_CHAR))
	{
		if(pMsg->wParam == '.')
		{
			//�����ֻ��������һ��С����
			if(nPos[3] >= 0)  //����ֿ�д����||�����Ļ��ܻ�����
			{  
				return 1;   //�������,����,��������������
			}	
			return 0;
		}
		//������������
		else if((pMsg->wParam >= '0' && pMsg->wParam <= '9'))   
		{  
			return 0;  
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

	if(GetFocus() == pEdit[4] && (pMsg->message == WM_CHAR))
	{
		if(pMsg->wParam == '.')
		{
			//�����ֻ��������һ��С����
			if(nPos[4] >= 0)  //����ֿ�д����||�����Ļ��ܻ�����
			{  
				return 1;   //�������,����,��������������
			}	
			return 0;
		}
		//������������
		else if((pMsg->wParam >= '0' && pMsg->wParam <= '9'))   
		{  
			return 0;  
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

	if(GetFocus() == pEdit[5] && (pMsg->message == WM_CHAR))
	{
		if(pMsg->wParam == '.')
		{
			//�����ֻ��������һ��С����
			if(nPos[5] >= 0)  //����ֿ�д����||�����Ļ��ܻ�����
			{  
				return 1;   //�������,����,��������������
			}	
			return 0;
		}
		//������������
		else if((pMsg->wParam >= '0' && pMsg->wParam <= '9'))   
		{  
			return 0;  
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

	if(GetFocus() == pEdit[6] && (pMsg->message == WM_CHAR))
	{
		if(pMsg->wParam == '.')
		{
			//�����ֻ��������һ��С����
			if(nPos[6] >= 0)  //����ֿ�д����||�����Ļ��ܻ�����
			{  
				return 1;   //�������,����,��������������
			}	
			return 0;
		}
		//������������
		else if((pMsg->wParam >= '0' && pMsg->wParam <= '9'))   
		{  
			return 0;  
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

	if(GetFocus() == pEdit[7] && (pMsg->message == WM_CHAR))
	{
		if(pMsg->wParam == '.')
		{
			//�����ֻ��������һ��С����
			if(nPos[7] >= 0)  //����ֿ�д����||�����Ļ��ܻ�����
			{  
				return 1;   //�������,����,��������������
			}	
			return 0;
		}
		//������������
		else if((pMsg->wParam >= '0' && pMsg->wParam <= '9'))   
		{  
			return 0;  
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

	return __super::PreTranslateMessage(pMsg);
}

void CPage_Advanced::OnClicked_Check_Multifeeddetect()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_check_multifeeddetect.GetCheck()) //����
	{
		m_pUI->SetCapValueInt(UDSCAP_MULTIFEEDDETECT,TRUE);
		m_check_mdvalue.EnableWindow(TRUE);
	} 
	else
	{
		m_pUI->SetCapValueInt(UDSCAP_MULTIFEEDDETECT,FALSE);
		m_check_mdvalue.SetCheck(FALSE);
		m_pUI->SetCapValueInt(UDSCAP_MULTIFEEDDETECT_VALUE,FALSE);
		m_check_mdvalue.EnableWindow(FALSE);
	}
}


void CPage_Advanced::OnClicked_Check_MdValue()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(m_check_mdvalue.GetCheck()) //����
	{
		m_pUI->SetCapValueInt(UDSCAP_MULTIFEEDDETECT_VALUE,TRUE);
	} 
	else
	{
		m_pUI->SetCapValueInt(UDSCAP_MULTIFEEDDETECT_VALUE,FALSE);
	}
}


BOOL CPage_Advanced::OnSetActive()
{
	// TODO: �ڴ����ר�ô����/����û���
	m_pUI->PreViewStatus();
	UpdateControls();
	return __super::OnSetActive();
}


void CPage_Advanced::OnCbnSelchangeAdvanced_Combo_Standardsizes()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nIndex = m_combo_standardsizes.GetCurSel();
	int nval = FindPaperSize(nIndex);
	m_combo_standardsizes.SetCurSel(nIndex);
	SetPaperSize();
	m_pUI->SetCapValueInt(ICAP_SUPPORTEDSIZES, nval); //�ܹ�ֱ����Ӧ
	UpdateControls(); //���¿���
}

void CPage_Advanced::SetPaperSize(void)
{
	SetXYPos();

	int nIndex = m_combo_standardsizes.GetCurSel(); 
	int nval = FindPaperSize(nIndex);
	int nIndex_unit = m_combo_uints.GetCurSel();
	int nval_unit = FindUnit(nIndex_unit);
	CString str;
	float value = 118.00;
	if(nval == UDSCAP_LONGDOCUMENT)
	{
		m_scroll_height.ShowWindow(FALSE);

		//switch���ó�ֽ���ֵ
		switch(nval_unit)
		{
		case TWUN_INCHES:
			str.Format("%0.2f",value);
			m_edit_height.SetWindowText(str);
			break;
		case TWUN_CENTIMETERS:		
			value = ConvertUnits(value, TWUN_INCHES, TWUN_CENTIMETERS, m_resolution);	
			str.Format("%0.2f",value);
			m_edit_height.SetWindowText(str);
			break;
		case TWUN_PIXELS:
			value = ConvertUnits(value, TWUN_INCHES, TWUN_PIXELS, m_resolution);	
			str.Format("%d",(int)value);
			m_edit_height.SetWindowText(str);
			break;
		}
		
	}
	else
	{
		m_scroll_height.ShowWindow(TRUE);
	}

	/*if(nval == TWSS_NONE)  //ֽ�Ŵ�С���Զ���
	{
		m_edit_width.EnableWindow(TRUE);
		m_edit_height.EnableWindow(TRUE);
		m_scroll_width.EnableWindow(TRUE);
		m_scroll_height.EnableWindow(TRUE);
	} 
	else*/
	{
		m_edit_width.EnableWindow(FALSE);
		m_edit_height.EnableWindow(FALSE);
		m_scroll_width.EnableWindow(FALSE);
		m_scroll_height.EnableWindow(FALSE);
	}
}

void CPage_Advanced::SetXYPos()
{
	int index = m_combo_standardsizes.GetCurSel();	
	switch(index)
	{
		//�Զ���US Legal
	case 0:
	case 2:
		{
			m_edit_xpos.EnableWindow(FALSE);
			m_edit_ypos.EnableWindow(FALSE);
			m_scroll_xpos.EnableWindow(FALSE);
			m_scroll_ypos.EnableWindow(FALSE);
			break;
		}

		//US Letter
	case 1:
		{
			m_edit_xpos.EnableWindow(FALSE);
			m_edit_ypos.EnableWindow(TRUE);
			m_scroll_xpos.EnableWindow(FALSE);
			m_scroll_ypos.EnableWindow(TRUE);
			break;
		}

		//����
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
	case 10:
	case 11:
	case 12:
	case 13:
	case 14:
		{
			m_edit_xpos.EnableWindow(TRUE);
			m_edit_ypos.EnableWindow(TRUE);
			m_scroll_xpos.EnableWindow(TRUE);
			m_scroll_ypos.EnableWindow(TRUE);
			break;
		}
	}
}

void CPage_Advanced::OnCbnSelchangeAdvanced_Combo_Uints()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nIndex = m_combo_uints.GetCurSel();
	int nval = FindUnit(nIndex);	
	m_combo_uints.SetCurSel(nIndex);
	m_pUI->SetCapValueInt(ICAP_UNITS, nval); //�����ܹ�ֱ����Ӧ
	
	UpdateControls();
}

void CPage_Advanced::SetScroll()
{
	// ���ô�ֱ�������Ĺ�����Χ	
	//���
	int nIndex = m_combo_uints.GetCurSel();
	CString str;
	int nval;
	float tempinche;

	switch(FindUnit(nIndex))
	{
	case TWUN_INCHES:	
		{
			//��
			maxinches_width = 850;
			m_scroll_width.SetScrollRange(0, maxinches_width); 
			m_edit_width.GetWindowText(str); //8.27
			nval = _ttof(str) * 100;
			m_scroll_width.SetScrollPos(nval); //������pos�Ǳ༭��ֵ��100��

			//XPOS��Χ
			maxinches_xpos = maxinches_width - nval;
			m_scroll_xpos.SetScrollRange(0, maxinches_xpos); //0.23
			//Xƫ����
			m_edit_xpos.GetWindowText(str);
			nval = _ttof(str) * 100;
			m_scroll_xpos.SetScrollPos(nval); 

			//��
			maxinches_height = 1400;
			m_scroll_height.SetScrollRange(0, maxinches_height);
			m_edit_height.GetWindowText(str);
			nval = _ttof(str) * 100;
			m_scroll_height.SetScrollPos(nval);

			//YPOS��Χ
			maxinches_ypos = maxinches_height - nval;
			m_scroll_ypos.SetScrollRange(0, maxinches_ypos);
			m_edit_ypos.GetWindowText(str);
			nval = _ttof(str) * 100;
			m_scroll_ypos.SetScrollPos(nval); 

			//��Ե���� ��������
			maxinches_updown = 50;
			m_scroll_up.SetScrollRange(0, maxinches_updown);
			m_edit_up.GetWindowText(str);
			nval = _ttof(str) * 100;
			m_scroll_up.SetScrollPos(nval); 

			m_scroll_down.SetScrollRange(0, maxinches_updown);
			m_edit_down.GetWindowText(str);
			nval = _ttof(str) * 100;
			m_scroll_down.SetScrollPos(nval); 

			maxinches_leftright = 19;
			m_scroll_left.SetScrollRange(0, maxinches_leftright);
			m_edit_left.GetWindowText(str);
			nval = _ttof(str) * 100;
			m_scroll_left.SetScrollPos(nval);

			m_scroll_right.SetScrollRange(0, maxinches_leftright);
			m_edit_right.GetWindowText(str);
			nval = _ttof(str) * 100;
			m_scroll_right.SetScrollPos(nval); 

			break;
		}
	case TWUN_CENTIMETERS:
		{
			//��
			maxinches_width = (int)ConvertUnits(850.00, TWUN_INCHES, TWUN_CENTIMETERS, m_resolution);
			m_scroll_width.SetScrollRange(0, maxinches_width); 
			m_edit_width.GetWindowText(str); //8.27
			nval = _ttof(str) * 100;
			m_scroll_width.SetScrollPos(nval); //������pos�Ǳ༭��ֵ��100��

			//��ǰ��ȡstrΪ���׵�λ,��ת��ΪӢ��
			tempinche = ConvertUnits(_ttof(str), TWUN_CENTIMETERS, TWUN_INCHES, m_resolution);
			//XPOS��Χ
			maxinches_xpos = (int)ConvertUnits(100.00*(8.50-tempinche), TWUN_INCHES, TWUN_CENTIMETERS, m_resolution);
			m_scroll_xpos.SetScrollRange(0, maxinches_xpos); 

			//X��Yƫ����
			m_edit_xpos.GetWindowText(str);
			nval = _ttof(str) * 100;
			m_scroll_xpos.SetScrollPos(nval); 

			//��
			maxinches_height = (int)ConvertUnits(1400.00, TWUN_INCHES, TWUN_CENTIMETERS, m_resolution);
			m_scroll_height.SetScrollRange(0, maxinches_height);
			m_edit_height.GetWindowText(str);
			nval = _ttof(str) * 100;
			m_scroll_height.SetScrollPos(nval);

			//��ǰ��ȡstrΪ���׵�λ,��ת��ΪӢ��
			tempinche = ConvertUnits(_ttof(str), TWUN_CENTIMETERS, TWUN_INCHES, m_resolution);
			//YPOS��Χ
			maxinches_ypos = (int)ConvertUnits(100.00*(14.00-tempinche), TWUN_INCHES, TWUN_CENTIMETERS, m_resolution);
			m_scroll_ypos.SetScrollRange(0, maxinches_ypos);
			m_edit_ypos.GetWindowText(str);
			nval = _ttof(str) * 100;
			m_scroll_ypos.SetScrollPos(nval); 

			//��Ե���� ��������
			maxinches_updown = (int)ConvertUnits(50.00, TWUN_INCHES, TWUN_CENTIMETERS, m_resolution);
			m_scroll_up.SetScrollRange(0, maxinches_updown);
			m_edit_up.GetWindowText(str);
			nval = _ttof(str) * 100;
			m_scroll_up.SetScrollPos(nval);

			m_scroll_down.SetScrollRange(0, maxinches_updown);
			m_edit_down.GetWindowText(str);
			nval = _ttof(str) * 100;
			m_scroll_down.SetScrollPos(nval); 

			maxinches_leftright = (int)ConvertUnits(19.00, TWUN_INCHES, TWUN_CENTIMETERS, m_resolution);
			m_scroll_left.SetScrollRange(0, maxinches_leftright);
			m_edit_left.GetWindowText(str);
			nval = _ttof(str) * 100;
			m_scroll_left.SetScrollPos(nval);

			m_scroll_right.SetScrollRange(0, maxinches_leftright);
			m_edit_right.GetWindowText(str);
			nval = _ttof(str) * 100;
			m_scroll_right.SetScrollPos(nval); 

			break;
		}
	case TWUN_PIXELS:
		{
			//��
			maxinches_width = (int)ConvertUnits(850.00, TWUN_INCHES, TWUN_PIXELS, m_resolution);
			m_scroll_width.SetScrollRange(0, maxinches_width); 
			m_edit_width.GetWindowText(str); 
			nval = _ttof(str);
			m_scroll_width.SetScrollPos(nval); 

			//��ǰ��ȡstrΪ���ص�λ,��ת��ΪӢ��
			tempinche = ConvertUnits(_ttof(str), TWUN_PIXELS, TWUN_INCHES, m_resolution);
			//XPOS��Χ
			maxinches_xpos = (int)ConvertUnits(100.00*(8.50-tempinche), TWUN_INCHES, TWUN_PIXELS, m_resolution);
			m_scroll_xpos.SetScrollRange(0, maxinches_xpos); 
			//X��Yƫ����
			m_edit_xpos.GetWindowText(str);
			nval = _ttof(str);
			m_scroll_xpos.SetScrollPos(nval); 

			//��
			maxinches_height = (int)ConvertUnits(1400.00, TWUN_INCHES, TWUN_PIXELS, m_resolution);
			m_scroll_height.SetScrollRange(0, maxinches_height);
			m_edit_height.GetWindowText(str);
			nval = _ttof(str);
			m_scroll_height.SetScrollPos(nval);

			//��ǰ��ȡstrΪ���׵�λ,��ת��ΪӢ��
			tempinche = ConvertUnits(_ttof(str), TWUN_PIXELS, TWUN_INCHES, m_resolution);
			//YPOS��Χ
			maxinches_ypos = (int)ConvertUnits(100.00*(14.00-tempinche), TWUN_INCHES, TWUN_PIXELS, m_resolution);
			m_scroll_ypos.SetScrollRange(0, maxinches_ypos);
			m_edit_ypos.GetWindowText(str);
			nval = _ttof(str);
			m_scroll_ypos.SetScrollPos(nval); 

			//��Ե���� ��������
			maxinches_updown = (int)ConvertUnits(50.00, TWUN_INCHES, TWUN_PIXELS, m_resolution);
			m_scroll_up.SetScrollRange(0, maxinches_updown);
			m_edit_up.GetWindowText(str);
			nval = _ttof(str);
			m_scroll_up.SetScrollPos(nval); 

			m_scroll_down.SetScrollRange(0, maxinches_updown);
			m_edit_down.GetWindowText(str);
			nval = _ttof(str);
			m_scroll_down.SetScrollPos(nval); 

			maxinches_leftright = (int)ConvertUnits(19.00, TWUN_INCHES, TWUN_PIXELS, m_resolution);
			m_scroll_left.SetScrollRange(0, maxinches_leftright);
			m_edit_left.GetWindowText(str);
			nval = _ttof(str);
			m_scroll_left.SetScrollPos(nval);

			m_scroll_right.SetScrollRange(0, maxinches_leftright);
			m_edit_right.GetWindowText(str);
			nval = _ttof(str);
			m_scroll_right.SetScrollPos(nval); 

			break;
		}		
	}//switch	
}

void CPage_Advanced::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	int scrollpos;
	CString str;
	float editvalue;
	int nIndex = m_combo_uints.GetCurSel();
	int nval = FindUnit(nIndex);

	SetScroll(); //��ʼ����Χ

	switch(pScrollBar->GetDlgCtrlID())
	{
		//��
	case IDC_ADVANCED_SCROLLBAR_WIDTH:
		scrollpos = m_scroll_width.GetScrollPos(); //��ȡ��ֱ��������ǰλ��
		switch(nSBCode)
		{
		case SB_LINEUP: //������Ϲ���һ�У���pos��1
			scrollpos += 1;
			break;
		case SB_LINEDOWN: //������¹���һ�У���pos��1
			scrollpos -= 1; 
			break;
		case SB_TOP: //���
			scrollpos = maxinches_width;
			break;
		case SB_BOTTOM:
			scrollpos = 0;
			break;

		}
		// ���ù�����λ��  
		m_scroll_width.SetScrollPos(scrollpos);
		switch(nval)
		{
		case TWUN_INCHES:
		case TWUN_CENTIMETERS:
			{
				editvalue = (float)scrollpos/100.00;
				str.Format("%0.2f", editvalue);
				break;
			}
		case TWUN_PIXELS:
			{
				editvalue = (float)scrollpos;
				str.Format("%d", scrollpos);
				break;
			}	
		}
		SetDlgItemText(IDC_ADVANCED_EDIT_WIDTH, str); //����edit��ֵ
		break;

		//��
	case IDC_ADVANCED_SCROLLBAR_HEIGHT:
		//��
		scrollpos = m_scroll_height.GetScrollPos(); 
		switch(nSBCode)
		{
		case SB_LINEUP: 
			scrollpos += 1;
			break;
		case SB_LINEDOWN: 
			scrollpos -= 1; 
			break;
		case SB_TOP: //���
			scrollpos = maxinches_height;
			break;
		case SB_BOTTOM:
			scrollpos = 0;
			break;
		}
		// ���ù�����λ��  
		m_scroll_height.SetScrollPos(scrollpos);
		switch(nval)
		{
		case TWUN_INCHES:
		case TWUN_CENTIMETERS:
			{
				editvalue = (float)scrollpos/100.00;
				str.Format("%0.2f", editvalue);
				break;
			}
		case TWUN_PIXELS:
			{
				editvalue = (float)scrollpos;
				str.Format("%d", scrollpos);
				break;
			}	
		}
		SetDlgItemText(IDC_ADVANCED_EDIT_HEIGHT, str); //����edit��ֵ
		break;

		//��Ե���� ��������
	case IDC_ADVANCED_SCROLLBAR_UP:
		scrollpos = m_scroll_up.GetScrollPos(); 
		switch(nSBCode)
		{
		case SB_LINEUP: 
			scrollpos += 1;
			break;
		case SB_LINEDOWN: 
			scrollpos -= 1; 
			break;	
		case SB_TOP: //���
			scrollpos = maxinches_updown;
			break;
		case SB_BOTTOM:
			scrollpos = 0;
			break;
		}
		m_scroll_up.SetScrollPos(scrollpos);
		switch(nval)
		{
		case TWUN_INCHES:
		case TWUN_CENTIMETERS:
			{
				editvalue = (float)scrollpos/100.00;
				str.Format("%0.2f", editvalue);
				break;
			}
		case TWUN_PIXELS:
			{
				editvalue = (float)scrollpos;
				str.Format("%d", scrollpos);
				break;
			}	
		}	
		SetDlgItemText(IDC_ADVANCED_EDIT_UP, str); //����edit��ֵ
		break;
	case IDC_ADVANCED_SCROLLBAR_DOWN:
		scrollpos = m_scroll_down.GetScrollPos(); 
		switch(nSBCode)
		{
		case SB_LINEUP: 
			scrollpos += 1;
			break;
		case SB_LINEDOWN: 
			scrollpos -= 1; 
			break; 
		case SB_TOP: //���
			scrollpos = maxinches_updown;
			break;
		case SB_BOTTOM:
			scrollpos = 0;
			break;
		}
		m_scroll_down.SetScrollPos(scrollpos);
		switch(nval)
		{
		case TWUN_INCHES:
		case TWUN_CENTIMETERS:
			{
				editvalue = (float)scrollpos/100.00;
				str.Format("%0.2f", editvalue);
				break;
			}
		case TWUN_PIXELS:
			{
				editvalue = (float)scrollpos;
				str.Format("%d", scrollpos);
				break;
			}	
		}	
		SetDlgItemText(IDC_ADVANCED_EDIT_DOWN, str); //����edit��ֵ
		break;

	case IDC_ADVANCED_SCROLLBAR_LEFT:
		scrollpos = m_scroll_left.GetScrollPos(); 
		switch(nSBCode)
		{
		case SB_LINEUP: 
			scrollpos += 1;
			break;
		case SB_LINEDOWN: 
			scrollpos -= 1; 
			break;
		case SB_TOP: //���
			scrollpos = maxinches_leftright;
			break;
		case SB_BOTTOM:
			scrollpos = 0;
			break;
		}
		m_scroll_left.SetScrollPos(scrollpos);
		switch(nval)
		{
		case TWUN_INCHES:
		case TWUN_CENTIMETERS:
			{
				editvalue = (float)scrollpos/100.00;
				str.Format("%0.2f", editvalue);
				break;
			}
		case TWUN_PIXELS:
			{
				editvalue = (float)scrollpos;
				str.Format("%d", scrollpos);
				break;
			}	
		}
		SetDlgItemText(IDC_ADVANCED_EDIT_LEFT, str); //����edit��ֵ
		break;
	case IDC_ADVANCED_SCROLLBAR_RIGHT:
		scrollpos = m_scroll_right.GetScrollPos(); 
		switch(nSBCode)
		{
		case SB_LINEUP: 
			scrollpos += 1;
			break;
		case SB_LINEDOWN: 
			scrollpos -= 1; 
			break;
		case SB_TOP: //���
			scrollpos = maxinches_leftright;
			break;
		case SB_BOTTOM:
			scrollpos = 0;
			break;
		}
		m_scroll_right.SetScrollPos(scrollpos);
		switch(nval)
		{
		case TWUN_INCHES:
		case TWUN_CENTIMETERS:
			{
				editvalue = (float)scrollpos/100.00;
				str.Format("%0.2f", editvalue);
				break;
			}
		case TWUN_PIXELS:
			{
				editvalue = (float)scrollpos;
				str.Format("%d", scrollpos);
				break;
			}	
		}	
		SetDlgItemText(IDC_ADVANCED_EDIT_RIGHT, str); //����edit��ֵ
		break;

		//Xƫ����
	case IDC_ADVANCED_SCROLLBAR_XPOS:
		scrollpos = m_scroll_xpos.GetScrollPos(); 
		switch(nSBCode)
		{
		case SB_LINEUP: 
			scrollpos += 1;
			break;
		case SB_LINEDOWN: 
			scrollpos -= 1; 
			break;
		case SB_TOP: //���
			scrollpos = maxinches_xpos;
			break;
		case SB_BOTTOM:
			scrollpos = 0;
			break;
		}
		m_scroll_xpos.SetScrollPos(scrollpos);
		switch(nval)
		{
		case TWUN_INCHES:
		case TWUN_CENTIMETERS:
			{
				editvalue = (float)scrollpos/100.00;
				str.Format("%0.2f", editvalue);
				break;
			}
		case TWUN_PIXELS:
			{
				editvalue = (float)scrollpos;
				str.Format("%d", scrollpos);
				break;
			}	
		}	
		SetDlgItemText(IDC_ADVANCED_EDIT_XPOS, str); //����edit��ֵ
		break;
	case IDC_ADVANCED_SCROLLBAR_YPOS:
		scrollpos = m_scroll_ypos.GetScrollPos(); 
		switch(nSBCode)
		{
		case SB_LINEUP: 
			scrollpos += 1;
			break;
		case SB_LINEDOWN: 
			scrollpos -= 1; 
			break;
		case SB_TOP: //���
			scrollpos = maxinches_ypos;
			break;
		case SB_BOTTOM:
			scrollpos = 0;
			break;
		}
		m_scroll_ypos.SetScrollPos(scrollpos);
		switch(nval)
		{
		case TWUN_INCHES:
		case TWUN_CENTIMETERS:
			{
				editvalue = (float)scrollpos/100.00;
				str.Format("%0.2f", editvalue);
				break;
			}
		case TWUN_PIXELS:
			{
				editvalue = (float)scrollpos;
				str.Format("%d", scrollpos);
				break;
			}	
		}	
		SetDlgItemText(IDC_ADVANCED_EDIT_YPOS, str); //����edit��ֵ

		break;

	default:
		break;
	}

	__super::OnVScroll(nSBCode, nPos, pScrollBar);
}


void CPage_Advanced::OnEnChangeAdvanced_Edit_EdgeXpos()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д __super::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);  // ��������

	SetScroll();

	CString str;
	m_edit_xpos.GetWindowText(str);
	float fval = _ttof(str);

	int nIndex = m_combo_uints.GetCurSel();
	int editvalue;
	int nval = FindUnit(nIndex);
	switch(nval)
	{
	case TWUN_INCHES:
	case TWUN_CENTIMETERS:
		{		
			editvalue = (int)(fval * 100.00);  //��Ҫ����100.		
			break;
		}
	case TWUN_PIXELS:
		{
			editvalue = (int)fval; 
			break;
		}
	}

	if(editvalue > maxinches_xpos)
	{
		editvalue = maxinches_xpos;
	}
	else if(editvalue < 0)
	{
		editvalue = 0;
	}
	else{}
	m_scroll_xpos.SetScrollPos(editvalue);

	switch(nval)
	{
	case TWUN_INCHES:
	case TWUN_CENTIMETERS:
		{		
			fval = (float)(editvalue/100.00);
			str.Format("%0.2f",fval);	
			break;
		}
	case TWUN_PIXELS:
		{
			fval = (float)editvalue;
			str.Format("%0.2f",(float)editvalue); 
			break;
		}
	}

	m_pUI->SetCapValueFloat(UDSCAP_XPOS, fval); 

	m_edit_xpos.SetSel(str.GetLength(), str.GetLength(),TRUE);  // ���ñ༭��ؼ���Χ

	UpdateData(FALSE);  // ���¿ؼ�
}


void CPage_Advanced::OnEnChangeAdvanced_Edit_EdgeYpos()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д __super::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);  // ��������
	SetScroll();

	CString str;
	m_edit_ypos.GetWindowText(str);
	float fval = _ttof(str);

	int nIndex = m_combo_uints.GetCurSel();
	int nval = FindUnit(nIndex);
	float editvalue;
	switch(nval)
	{
	case TWUN_INCHES:
	case TWUN_CENTIMETERS:
		{		
			editvalue = fval * 100;  //��Ҫ����100.		
			break;
		}
	case TWUN_PIXELS:
		{
			editvalue = fval; 
			break;
		}
	}

	if(editvalue > maxinches_ypos)
	{
		editvalue = maxinches_ypos;
	}
	else if(editvalue < 0)
	{
		editvalue = 0;
	}
	else{}
	m_scroll_ypos.SetScrollPos(editvalue);

	switch(nval)
	{
	case TWUN_INCHES:
	case TWUN_CENTIMETERS:
		{		
			fval = (float)(editvalue/100.00);
			str.Format("%0.2f",fval);	
			break;
		}
	case TWUN_PIXELS:
		{
			fval = (float)editvalue;
			str.Format("%0.2f",(float)editvalue); 
			break;
		}
	}

	m_pUI->SetCapValueFloat(UDSCAP_YPOS, fval); 

	m_edit_ypos.SetSel(str.GetLength(), str.GetLength(),TRUE);  // ���ñ༭��ؼ���Χ

	UpdateData(FALSE);  // ���¿ؼ�
}


void CPage_Advanced::OnEnChangeAdvanced_Edit_EdgeUp()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д __super::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);  // ��������
	SetScroll();

	CString str;
	m_edit_up.GetWindowText(str);
	float fval = _ttof(str);

	int nIndex = m_combo_uints.GetCurSel();
	int nval = FindUnit(nIndex);
	float editvalue;
	switch(nval)
	{
	case TWUN_INCHES:
	case TWUN_CENTIMETERS:
		{		
			editvalue = fval * 100;  //��Ҫ����100.		
			break;
		}
	case TWUN_PIXELS:
		{
			editvalue = fval; 
			break;
		}
	}
	if(editvalue > maxinches_updown)
	{
		editvalue = maxinches_updown;
	}
	else if(editvalue < 0)
	{
		editvalue = 0;
	}
	else{}
	m_scroll_up.SetScrollPos(editvalue);

	switch(nval)
	{
	case TWUN_INCHES:
	case TWUN_CENTIMETERS:
		{		
			fval = (float)(editvalue/100.00);
			str.Format("%0.2f",fval);	
			break;
		}
	case TWUN_PIXELS:
		{
			fval = (float)editvalue;
			str.Format("%0.2f",(float)editvalue); 
			break;
		}
	}
	m_pUI->SetCapValueFloat(UDSCAP_EDGE_UP, fval);

	m_edit_up.SetSel(str.GetLength(), str.GetLength(),TRUE);  // ���ñ༭��ؼ���Χ

	UpdateData(FALSE);  // ���¿ؼ�
}


void CPage_Advanced::OnEnChangeAdvanced_Edit_EdgeLeft()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д __super::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);  // ��������
	SetScroll();

	CString str;
	m_edit_left.GetWindowText(str);
	float fval = _ttof(str);

	int nIndex = m_combo_uints.GetCurSel();
	int nval = FindUnit(nIndex);
	float editvalue;
	switch(nval)
	{
	case TWUN_INCHES:
	case TWUN_CENTIMETERS:
		{		
			editvalue = fval * 100;  //��Ҫ����100.		
			break;
		}
	case TWUN_PIXELS:
		{
			editvalue = fval; 
			break;
		}
	}

	if(editvalue > maxinches_leftright)
	{
		editvalue = maxinches_leftright;
	}
	else if(editvalue < 0)
	{
		editvalue = 0;
	}
	else{}
	m_scroll_left.SetScrollPos(editvalue);

	switch(nval)
	{
	case TWUN_INCHES:
	case TWUN_CENTIMETERS:
		{		
			fval = (float)(editvalue/100.00);
			str.Format("%0.2f",fval);	
			break;
		}
	case TWUN_PIXELS:
		{
			fval = (float)editvalue;
			str.Format("%0.2f",(float)editvalue); 
			break;
		}
	}
	m_pUI->SetCapValueFloat(UDSCAP_EDGE_LEFT, fval); 

	m_edit_left.SetSel(str.GetLength(), str.GetLength(),TRUE);  // ���ñ༭��ؼ���Χ

	UpdateData(FALSE);  // ���¿ؼ�
}


void CPage_Advanced::OnEnChangeAdvanced_Edit_EdgeRight()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д __super::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);  // ��������
	SetScroll();

	CString str;
	m_edit_right.GetWindowText(str);
	float fval = _ttof(str);

	int nIndex = m_combo_uints.GetCurSel();
	int nval = FindUnit(nIndex);
	float editvalue;
	switch(nval)
	{
	case TWUN_INCHES:
	case TWUN_CENTIMETERS:
		{		
			editvalue = fval * 100;  //��Ҫ����100.		
			break;
		}
	case TWUN_PIXELS:
		{
			editvalue = fval; 
			break;
		}
	}
	if(editvalue > maxinches_leftright)
	{
		editvalue = maxinches_leftright;
	}
	else if(editvalue < 0)
	{
		editvalue = 0;
	}
	else{}
	m_scroll_right.SetScrollPos(editvalue);

	switch(nval)
	{
	case TWUN_INCHES:
	case TWUN_CENTIMETERS:
		{		
			fval = (float)(editvalue/100.00);
			str.Format("%0.2f",fval);	
			break;
		}
	case TWUN_PIXELS:
		{
			fval = (float)editvalue;
			str.Format("%0.2f",(float)editvalue); 
			break;
		}
	}
	m_pUI->SetCapValueFloat(UDSCAP_EDGE_RIGHT, fval);

	m_edit_right.SetSel(str.GetLength(), str.GetLength(),TRUE);  // ���ñ༭��ؼ���Χ

	UpdateData(FALSE);  // ���¿ؼ�
}


void CPage_Advanced::OnEnChangeAdvanced_Edit_EdgeDown()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д __super::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);  // ��������
	SetScroll();

	CString str;
	m_edit_down.GetWindowText(str);
	float fval = _ttof(str);

	int nIndex = m_combo_uints.GetCurSel();
	int nval = FindUnit(nIndex);
	float editvalue;
	switch(nval)
	{
	case TWUN_INCHES:
	case TWUN_CENTIMETERS:
		{		
			editvalue = fval * 100;  //��Ҫ����100
			break;
		}
	case TWUN_PIXELS:
		{
			editvalue = fval; 
			break;
		}
	}

	if(editvalue > maxinches_updown)
	{
		editvalue = maxinches_updown;
	}
	else if(editvalue < 0)
	{
		editvalue = 0;
	}
	else{}
	m_scroll_down.SetScrollPos(editvalue);

	switch(nval)
	{
	case TWUN_INCHES:
	case TWUN_CENTIMETERS:
		{		
			fval = (float)(editvalue/100.00);
			str.Format("%0.2f",fval);	
			break;
		}
	case TWUN_PIXELS:
		{
			fval = (float)editvalue;
			str.Format("%0.2f",(float)editvalue); 
			break;
		}
	}
	m_pUI->SetCapValueFloat(UDSCAP_EDGE_DOWN, fval); 

	m_edit_down.SetSel(str.GetLength(), str.GetLength(),TRUE);  // ���ñ༭��ؼ���Χ

	UpdateData(FALSE);  // ���¿ؼ�
}


void CPage_Advanced::OnCbnSelchangeAdvanced_Combo_Cutmethod()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nIndex = m_combo_cutmethod.GetCurSel();
	CString strCBText; 
	m_combo_cutmethod.GetLBText( nIndex, strCBText);
	int nval;
	if (strCBText.Find("������") >= 0)
	{
		nval = TWCT_NONE;
	}
	else if(strCBText.Find("�Զ��������ƫ") >= 0)
	{
		nval = TWCT_AUTO; 
	}
	else if(strCBText.Find("ָ�����ȡ����") >= 0)
	{
		nval = TWCT_SPECIFY; 
	}
	else
	{}

	m_pUI->SetCapValueInt(UDSCAP_CUTMETHOD,nval); 
	m_combo_cutmethod.SetCurSel(nIndex);

	if(nIndex == 1) //�Զ�������У��
	{
		nval = TWAC_AUTO;
	} 
	else
	{
		nval = TWAC_DISABLE;
	}
	m_pUI->SetCapValueInt(UDSCAP_AUTOCROP,nval);

}

void CPage_Advanced::OnAdvanced_RadioBtn_Edgecolor()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	switch(m_radio_edgecolor)
	{
	case 0:
		m_pUI->SetCapValueInt(UDSCAP_EDGE_COLOR,TWEC_WHITE);
		break;
	case 1:
		m_pUI->SetCapValueInt(UDSCAP_EDGE_COLOR,TWEC_BLACK);
		break;
	}
	UpdateData(FALSE);
}

// Page_Filter.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "UDS Scanner.h"
#include "Page_Filter.h"
#include "afxdialogex.h"


// CPage_Filter �Ի���

IMPLEMENT_DYNAMIC(CPage_Filter, CPropertyPage)

CPage_Filter::CPage_Filter(MFC_UI *pUI)
	: m_pUI(pUI), CPropertyPage(CPage_Filter::IDD)
{

}

CPage_Filter::~CPage_Filter()
{
	m_filtermap.swap(map<int, float>());  // ��ղ��ͷ��ڴ�
}

void CPage_Filter::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FILTERCOLOR_CHECK_FRONTCOLOR, m_check_frontcolor);
	DDX_Control(pDX, IDC_FILTERCOLOR_CHECK_FRONTGRAY, m_check_frontgray);
	DDX_Control(pDX, IDC_FILTERCOLOR_CHECK_BACKGRAY, m_check_backgray);
	DDX_Control(pDX, IDC_FILTERCOLOR_CHECK_BACKCOLOR, m_check_backcolor);
	DDX_Control(pDX, IDC_FILTERCOLOR_CHECK_BACKBW, m_check_backbw);
	DDX_Control(pDX, IDC_FILTERCOLOR_CHECK_FRONTBW, m_check_frontbw);
	DDX_Control(pDX, IDC_FILTERCOLOR_BUTTON_FRONTCOLOR, m_btn_frontcolor);
	DDX_Control(pDX, IDC_FILTERCOLOR_BUTTON_FRONTGRAY, m_btn_frontgray);
	DDX_Control(pDX, IDC_FILTERCOLOR_BUTTON_FRONTBW, m_btn_frontbw);
	DDX_Control(pDX, IDC_FILTERCOLOR_BUTTON_BACKGRAY, m_btn_backgray);
	DDX_Control(pDX, IDC_FILTERCOLOR_BUTTON_BACKCOLOR, m_btn_backcolor);
	DDX_Control(pDX, IDC_FILTERCOLOR_BUTTON_BACKBW, m_btn_backbw);
	DDX_Control(pDX, IDC_FILTERCOLOR_COMBO_COLOR, m_combo_filtercolor);
	DDX_Control(pDX, IDC_FILTERCOLOR_COMBO_FILTERMODE, m_combo_filtermode);
	DDX_Control(pDX, IDC_FILTERCOLOR_EDIT_FILTERLEVEL, m_edit_filterlevel);
	DDX_Control(pDX, IDC_FILTERCOLOR_SLIDER_FILTERLEVEL, m_slider_filterlevel);
}


BEGIN_MESSAGE_MAP(CPage_Filter, CPropertyPage)
	ON_BN_CLICKED(IDC_FILTERCOLOR_CHECK_FRONTCOLOR, &CPage_Filter::OnFilter_Btn_Check_Frontcolor)
	ON_BN_CLICKED(IDC_FILTERCOLOR_CHECK_FRONTGRAY, &CPage_Filter::OnFilter_Btn_Check_FrontGray)
	ON_BN_CLICKED(IDC_FILTERCOLOR_CHECK_FRONTBW, &CPage_Filter::OnFilter_Btn_Check_FrontBw)
	ON_BN_CLICKED(IDC_FILTERCOLOR_CHECK_BACKGRAY, &CPage_Filter::OnFilter_Btn_Check_BackGray)
	ON_BN_CLICKED(IDC_FILTERCOLOR_CHECK_BACKCOLOR, &CPage_Filter::OnFilter_Btn_Check_BackColor)
	ON_BN_CLICKED(IDC_FILTERCOLOR_CHECK_BACKBW, &CPage_Filter::OnFilter_Btn_Check_BackBw)
	ON_CBN_SELCHANGE(IDC_FILTERCOLOR_COMBO_COLOR, &CPage_Filter::OnCbnSelchangeFilter_Combo_FilterColor)
	ON_CBN_SELCHANGE(IDC_FILTERCOLOR_COMBO_FILTERMODE, &CPage_Filter::OnCbnSelchangeFilter_Combo_FilterMode)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_FILTERCOLOR_SLIDER_FILTERLEVEL, &CPage_Filter::OnNMCustomdrawFilter_Slider_Filterlevel)
	ON_EN_CHANGE(IDC_FILTERCOLOR_EDIT_FILTERLEVEL, &CPage_Filter::OnEnChangeFilter_Edit_Filterlevel)
END_MESSAGE_MAP()


// CPage_Filter ��Ϣ�������
void CPage_Filter::SetCapValue(void)
{
	MAP_CAP::iterator iter; 
	for(iter = m_filtermap.begin(); iter != m_filtermap.end(); iter++)
	{
		switch(iter->first)
		{
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

void CPage_Filter::UpdateControls(void)
{
	int nCapIndex;
	const IntVector* lstCapValues;
	float fCapValue;
	int nCapValue;
	CString strText;

	//�˳���ɫ
	m_combo_filtercolor.ResetContent(); 
	nCapIndex = m_pUI->GetCurrentCapIndex(UDSCAP_FILTERCOLOR);
	lstCapValues = m_pUI->GetValidCap(UDSCAP_FILTERCOLOR);
	for(unsigned int i=0; i<lstCapValues->size();i++)
	{
		switch(lstCapValues->at(i))
		{
		case TWFL_NONE:
			m_combo_filtercolor.InsertString(i,"���˳�"); 
			break;
		case TWFL_RED:
			m_combo_filtercolor.InsertString(i,"��ɫ"); 
			break;
		case TWFL_GREEN:
			m_combo_filtercolor.InsertString(i,"��ɫ"); 
			break;
		case TWFL_BLUE:
			m_combo_filtercolor.InsertString(i,"��ɫ"); 
			break;
		default:
			continue;
		}
	}
	m_combo_filtercolor.SetCurSel(nCapIndex);

	//�˳�ģʽ
	m_combo_filtermode.ResetContent(); 
	nCapIndex = m_pUI->GetCurrentCapIndex(UDSCAP_FILTERMODE);
	lstCapValues = m_pUI->GetValidCap(UDSCAP_FILTERMODE);
	for(unsigned int i=0; i<lstCapValues->size();i++)
	{
		switch(lstCapValues->at(i))
		{
		case TWFL_NONE:
			m_combo_filtermode.InsertString(i,"�Զ�"); 
			break;
		case TWFL_RED:
			m_combo_filtermode.InsertString(i,"�Զ���"); 
			break;
		default:
			continue;
		}
	}
	m_combo_filtermode.SetCurSel(nCapIndex);

	//�˳��̶�
	nCapValue = (int)m_pUI->GetCapValueFloat(UDSCAP_FILTERLEVEL);
	m_slider_filterlevel.SetPos(nCapValue);
	strText.Format("%d",nCapValue);
	SetDlgItemText(IDC_FILTERCOLOR_EDIT_FILTERLEVEL,strText);

	//���������Ĭ�ϲ�ʹ��
	nCapValue = (int)(m_pUI->GetCapValueBool(UDSCAP_MULTISTREAM));
	if(nCapValue == 1) //����ѡ��
	{
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
	SetMultistream();

}

void CPage_Filter::SetMultistream(void)
{
	int nCapValue = (int)(m_pUI->GetCapValueBool(UDSCAP_MULTISTREAM));

	if(nCapValue == 1) //����ѡ��
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
	}
}

void CPage_Filter::SetCapMulti()
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

void CPage_Filter::InitFiltermap(void)
{
	m_filtermap.erase(m_filtermap.begin(),m_filtermap.end());//���

	float value = m_pUI->GetCapValueFloat(UDSCAP_MULTISTREAM_VALUE);
	m_filtermap[UDSCAP_MULTISTREAM_VALUE] = value; // ��ʼ��ʱ���UDSCAP_MULTISTREAM_VALUE����֤SetCapValue()����¸�Cap��ֵ
}


void CPage_Filter::OnFilter_Btn_Check_Frontcolor()
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
	SetCapMulti();
}


void CPage_Filter::OnFilter_Btn_Check_FrontGray()
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
	SetCapMulti();
}


void CPage_Filter::OnFilter_Btn_Check_FrontBw()
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
	SetCapMulti();
}


void CPage_Filter::OnFilter_Btn_Check_BackGray()
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
	SetCapMulti();
}


void CPage_Filter::OnFilter_Btn_Check_BackColor()
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
	SetCapMulti();
}


void CPage_Filter::OnFilter_Btn_Check_BackBw()
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
	SetCapMulti();
}


BOOL CPage_Filter::OnInitDialog()
{
	__super::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//2.0�汾����
	InitSliderCtrl();
	UpdateControls();
	InitFiltermap();
	

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CPage_Filter::InitSliderCtrl()
{
	float fMin,fMax,fStep;
	m_pUI->GetCapRangeFloat(UDSCAP_FILTERLEVEL, fMin, fMax, fStep);
	m_slider_filterlevel.SetRange((int)fMin, (int)fMax);
	m_slider_filterlevel.SetTicFreq((int)fStep); //����

	UpdateData(FALSE);  // ���¿ؼ���ˢ�µ�ǰ�Ի���
}

BOOL CPage_Filter::OnSetActive()
{
	// TODO: �ڴ����ר�ô����/����û���
	m_pUI->PreViewStatus();
	UpdateControls();
	return __super::OnSetActive();
}


void CPage_Filter::OnCbnSelchangeFilter_Combo_FilterColor()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nIndex = m_combo_filtercolor.GetCurSel();
	CString strCBText; 
	m_combo_filtercolor.GetLBText( nIndex, strCBText);
	int nval;
	if (strCBText.Find("���˳�") >= 0)
	{
		nval = TWFL_NONE;
	}
	else if(strCBText.Find("��ɫ") >= 0)
	{
		nval = TWFL_RED; 
	}
	else if(strCBText.Find("��ɫ") >= 0)
	{
		nval = TWFL_GREEN; 
	}
	else if(strCBText.Find("��ɫ") >= 0)
	{
		nval = TWFL_BLUE; 
	}
	else if(strCBText.Find("�Զ�") >= 0)
	{
		nval = TWFL_AUTO; 
	}
	else
	{}

	m_pUI->SetCapValueInt(UDSCAP_FILTERCOLOR,nval); 
	m_combo_filtercolor.SetCurSel(nIndex);
}


void CPage_Filter::OnCbnSelchangeFilter_Combo_FilterMode()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nIndex = m_combo_filtermode.GetCurSel();
	CString strCBText; 
	m_combo_filtermode.GetLBText( nIndex, strCBText);
	int nval;
	if (strCBText.Find("�Զ�") >= 0)
	{
		nval = TWFM_AUTO;
	}
	else if(strCBText.Find("�Զ���") >= 0)
	{
		nval = TWFM_DEFINED; 
	}
	else
	{}

	m_pUI->SetCapValueInt(UDSCAP_FILTERMODE,nval); 
	m_combo_filtermode.SetCurSel(nIndex);
}


void CPage_Filter::OnNMCustomdrawFilter_Slider_Filterlevel(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);  // ��������
	CString str;
	int sldValue = m_slider_filterlevel.GetPos();  // ��ȡ���鵱ǰλ��
	str.Format("%d",sldValue);
	SetDlgItemText(IDC_FILTERCOLOR_EDIT_FILTERLEVEL, str);// �ڱ༭��ͬ����ʾ������ֵ
	UpdateData(FALSE);  // ���¿ؼ�

	*pResult = 0;
}


void CPage_Filter::OnEnChangeFilter_Edit_Filterlevel()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д __super::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);  // ��������
	CString str;
	m_edit_filterlevel.GetWindowText(str);
	int nval = _ttoi(str);
	m_slider_filterlevel.SetPos(nval);
	m_pUI->SetCapValueFloat(UDSCAP_FILTERLEVEL,(float)nval);
	
	m_edit_filterlevel.SetSel(str.GetLength(), str.GetLength(),TRUE);  // ���ñ༭��ؼ���Χ
	UpdateData(FALSE);  // ���¿ؼ�
}

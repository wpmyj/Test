// Page_Set.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "UDS Scanner.h"
#include "Page_Set.h"
#include "afxdialogex.h"


// CPage_Set �Ի���

IMPLEMENT_DYNAMIC(CPage_Set, CPropertyPage)

CPage_Set::CPage_Set(MFC_UI *pUI)
	: m_pUI(pUI), CPropertyPage(CPage_Set::IDD)
{

}

CPage_Set::~CPage_Set()
{
	m_setmap.swap(map<int, float>());  // ��ղ��ͷ��ڴ�
}

void CPage_Set::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SET_CHECK_SAVEPOWER, m_check_savepower);
	DDX_Control(pDX, IDC_SET_CHECK_OFFTIME, m_check_offtime);
	DDX_Control(pDX, IDC_SET_SLIDER_OFFTIME, m_slider_offtime);
	DDX_Control(pDX, IDC_SET_SLIDER_SAVEPOWER, m_slider_savepower);
	//  DDX_Control(pDX, IDC_SET_EDIT_SAVEPOWER, m_edit_savepower);
	//  DDX_Control(pDX, IDC_SET_EDIT_OFFTIME, m_edit_offtime);
}


BEGIN_MESSAGE_MAP(CPage_Set, CPropertyPage)
	ON_BN_CLICKED(IDC_SET_CHECK_SAVEPOWER, &CPage_Set::OnSet_Btn_Check_SavePower)
	ON_BN_CLICKED(IDC_SET_CHECK_OFFTIME, &CPage_Set::OnSet_Btn_Check_OffTime)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SET_SLIDER_SAVEPOWER, &CPage_Set::OnNMCustomdrawSet_Slider_SavePower)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SET_SLIDER_OFFTIME, &CPage_Set::OnNMCustomdrawSet_Slider_Offtime)
//	ON_WM_HSCROLL()
//ON_EN_CHANGE(IDC_SET_EDIT_SAVEPOWER, &CPage_Set::OnEnChangeSet_Edit_Savepower)
//ON_EN_CHANGE(IDC_SET_EDIT_OFFTIME, &CPage_Set::OnEnChangeSet_Edit_Offtime)
END_MESSAGE_MAP()


// CPage_Set ��Ϣ�������
void CPage_Set::SetCapValue(void)
{
	MAP_CAP::iterator iter;
	if (true == m_setmap.empty())
	{
		return;
	}
	for(iter = m_setmap.begin(); iter != m_setmap.end(); iter++)
	{
		switch(iter->first)
		{
		case UDSCAP_SAVEPOWER: //�ڵ�ģʽ
		case UDSCAP_OFFTIME: //�ػ�ʱ��
			{
				m_pUI->SetCapValueInt(iter->first,(int)(iter->second));
				break;
			}	

		case UDSCAP_SAVEPOWER_VALUE: //�ڵ�ģʽֵ
		case UDSCAP_OFFTIME_VALUE: //�ػ�ʱ��ֵ
			{			
				m_pUI->SetCapValueFloat(iter->first,iter->second);
				break;
			}
		}
	}
}


void CPage_Set::UpdateControls(void)
{
	int nCapValue;
	int nval;
	CString strText;

	//�ڵ�ģʽ
	nCapValue = (int)(m_pUI->GetCapValueBool(UDSCAP_SAVEPOWER));
	m_check_savepower.SetCheck(nCapValue);
	SetSavePower();
	//�ڵ�ģʽֵ
	nCapValue = (int)(m_pUI->GetCapValueFloat(UDSCAP_SAVEPOWER_VALUE));
	m_slider_savepower.SetPos(nCapValue);
	SetDlgItemText(IDC_SET_STATIC_SAVEPOWER, strText);
	m_setmap[UDSCAP_SAVEPOWER_VALUE] = float(nCapValue);

	//�ػ�ʱ��
	nCapValue = (int)(m_pUI->GetCapValueBool(UDSCAP_OFFTIME));
	m_check_offtime.SetCheck(nCapValue);
	SetOffTime();
	//�ػ�ʱ��ֵ
	nCapValue = (int)(m_pUI->GetCapValueFloat(UDSCAP_OFFTIME_VALUE));
	m_slider_offtime.SetPos(nCapValue);
	SetDlgItemText(IDC_SET_STATIC_OFFTIME, strText);
	m_setmap[UDSCAP_OFFTIME_VALUE] = float(nCapValue);

}


BOOL CPage_Set::OnInitDialog()
{
	__super::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	InitSliderCtrl(); //��ʼ������ Ҫ����UpdateControls֮ǰ���������û���Ĳ�����Ч
	UpdateControls();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CPage_Set::InitSliderCtrl()
{
	float fMin,fMax,fStep;
	m_pUI->GetCapRangeFloat(UDSCAP_SAVEPOWER_VALUE, fMin, fMax, fStep);
	m_slider_savepower.SetRange((int)fMin, (int)fMax, TRUE);
	m_slider_savepower.SetTicFreq((int)fStep); //����

	m_pUI->GetCapRangeFloat(UDSCAP_OFFTIME_VALUE, fMin, fMax, fStep);
	m_slider_offtime.SetRange((int)fMin, (int)fMax, TRUE);
	m_slider_offtime.SetTicFreq((int)fStep); //����

	UpdateData(FALSE);  // ���¿ؼ�
}

//�ڵ�ģʽ
void CPage_Set::OnSet_Btn_Check_SavePower()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nval;
	if (m_check_savepower.GetCheck())
	{
		nval = TWSP_AUTO;
	} 
	else
	{
		nval = TWSP_DISABLE;
	}
	m_setmap[UDSCAP_SAVEPOWER] = (float)nval;
	SetSavePower();
}

//�ػ�ʱ��
void CPage_Set::OnSet_Btn_Check_OffTime()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nval;
	if (m_check_offtime.GetCheck())
	{
		nval = TWOT_AUTO;
	} 
	else
	{
		nval = TWOT_DISABLE;
	}
	m_setmap[UDSCAP_OFFTIME] = (float)nval;
	SetOffTime();
}


void CPage_Set::OnNMCustomdrawSet_Slider_SavePower(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);  // ��������
	CString str;
	int sldValue = m_slider_savepower.GetPos();  // ��ȡ���鵱ǰλ��
	m_setmap[UDSCAP_SAVEPOWER_VALUE] = (float)sldValue;

	str.Format("%d��", sldValue);
	SetDlgItemText(IDC_SET_STATIC_SAVEPOWER, str);
	UpdateData(FALSE);  // ���¿ؼ�

	*pResult = 0;
}


void CPage_Set::OnNMCustomdrawSet_Slider_Offtime(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);  // ��������
	CString str;
	int sldValue = m_slider_offtime.GetPos();  // ��ȡ���鵱ǰλ��
	
	int sldValue_temp = m_slider_savepower.GetPos();  
	if(sldValue <= sldValue_temp)//�ػ�ʱ�������ڽڵ�ģʽ���õ�ʱ��
	{
		sldValue = sldValue_temp;
		//m_slider_offtime.SetPos(sldValue);
	}
	
	m_setmap[UDSCAP_OFFTIME_VALUE] = (float)sldValue;
	
	str.Format("%d��", sldValue);
	SetDlgItemText(IDC_SET_STATIC_OFFTIME, str);
	UpdateData(FALSE);  // ���¿ؼ�
	
	*pResult = 0;
}


void CPage_Set::SetSavePower(void)
{
	if(m_check_savepower.GetCheck())
	{
		m_slider_savepower.EnableWindow(TRUE);
	}
	else
	{
		m_slider_savepower.EnableWindow(FALSE);
	}
}


void CPage_Set::SetOffTime(void)
{
	if(m_check_offtime.GetCheck())
	{
		m_slider_offtime.EnableWindow(TRUE);
	}
	else
	{
		m_slider_offtime.EnableWindow(FALSE);
	}
}


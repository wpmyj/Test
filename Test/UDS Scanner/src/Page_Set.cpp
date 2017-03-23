// Page_Set.cpp : 实现文件
//

#include "stdafx.h"
#include "UDS Scanner.h"
#include "Page_Set.h"
#include "afxdialogex.h"


// CPage_Set 对话框

IMPLEMENT_DYNAMIC(CPage_Set, CPropertyPage)

CPage_Set::CPage_Set(MFC_UI *pUI)
	: m_pUI(pUI), CPropertyPage(CPage_Set::IDD)
{

}

CPage_Set::~CPage_Set()
{
	m_setmap.swap(map<int, float>());  // 清空并释放内存
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


// CPage_Set 消息处理程序
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
		case UDSCAP_SAVEPOWER: //节电模式
		case UDSCAP_OFFTIME: //关机时间
			{
				m_pUI->SetCapValueInt(iter->first,(int)(iter->second));
				break;
			}	

		case UDSCAP_SAVEPOWER_VALUE: //节电模式值
		case UDSCAP_OFFTIME_VALUE: //关机时间值
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

	//节电模式
	nCapValue = (int)(m_pUI->GetCapValueBool(UDSCAP_SAVEPOWER));
	m_check_savepower.SetCheck(nCapValue);
	SetSavePower();
	//节电模式值
	nCapValue = (int)(m_pUI->GetCapValueFloat(UDSCAP_SAVEPOWER_VALUE));
	m_slider_savepower.SetPos(nCapValue);
	SetDlgItemText(IDC_SET_STATIC_SAVEPOWER, strText);
	m_setmap[UDSCAP_SAVEPOWER_VALUE] = float(nCapValue);

	//关机时间
	nCapValue = (int)(m_pUI->GetCapValueBool(UDSCAP_OFFTIME));
	m_check_offtime.SetCheck(nCapValue);
	SetOffTime();
	//关机时间值
	nCapValue = (int)(m_pUI->GetCapValueFloat(UDSCAP_OFFTIME_VALUE));
	m_slider_offtime.SetPos(nCapValue);
	SetDlgItemText(IDC_SET_STATIC_OFFTIME, strText);
	m_setmap[UDSCAP_OFFTIME_VALUE] = float(nCapValue);

}


BOOL CPage_Set::OnInitDialog()
{
	__super::OnInitDialog();

	// TODO:  在此添加额外的初始化
	InitSliderCtrl(); //初始化滑块 要放在UpdateControls之前，否则设置滑块的步长无效
	UpdateControls();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CPage_Set::InitSliderCtrl()
{
	float fMin,fMax,fStep;
	m_pUI->GetCapRangeFloat(UDSCAP_SAVEPOWER_VALUE, fMin, fMax, fStep);
	m_slider_savepower.SetRange((int)fMin, (int)fMax, TRUE);
	m_slider_savepower.SetTicFreq((int)fStep); //步长

	m_pUI->GetCapRangeFloat(UDSCAP_OFFTIME_VALUE, fMin, fMax, fStep);
	m_slider_offtime.SetRange((int)fMin, (int)fMax, TRUE);
	m_slider_offtime.SetTicFreq((int)fStep); //步长

	UpdateData(FALSE);  // 更新控件
}

//节电模式
void CPage_Set::OnSet_Btn_Check_SavePower()
{
	// TODO: 在此添加控件通知处理程序代码
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

//关机时间
void CPage_Set::OnSet_Btn_Check_OffTime()
{
	// TODO: 在此添加控件通知处理程序代码
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
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);  // 接收数据
	CString str;
	int sldValue = m_slider_savepower.GetPos();  // 获取滑块当前位置
	m_setmap[UDSCAP_SAVEPOWER_VALUE] = (float)sldValue;

	str.Format("%d分", sldValue);
	SetDlgItemText(IDC_SET_STATIC_SAVEPOWER, str);
	UpdateData(FALSE);  // 更新控件

	*pResult = 0;
}


void CPage_Set::OnNMCustomdrawSet_Slider_Offtime(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);  // 接收数据
	CString str;
	int sldValue = m_slider_offtime.GetPos();  // 获取滑块当前位置
	
	int sldValue_temp = m_slider_savepower.GetPos();  
	if(sldValue <= sldValue_temp)//关机时间必须大于节电模式设置的时间
	{
		sldValue = sldValue_temp;
		//m_slider_offtime.SetPos(sldValue);
	}
	
	m_setmap[UDSCAP_OFFTIME_VALUE] = (float)sldValue;
	
	str.Format("%d分", sldValue);
	SetDlgItemText(IDC_SET_STATIC_OFFTIME, str);
	UpdateData(FALSE);  // 更新控件
	
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


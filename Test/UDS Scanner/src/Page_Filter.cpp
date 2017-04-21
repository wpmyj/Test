// Page_Filter.cpp : 实现文件
//

#include "stdafx.h"
#include "UDS Scanner.h"
#include "Page_Filter.h"
#include "afxdialogex.h"


// CPage_Filter 对话框

IMPLEMENT_DYNAMIC(CPage_Filter, CPropertyPage)

CPage_Filter::CPage_Filter(MFC_UI *pUI)
	: m_pUI(pUI), CPropertyPage(CPage_Filter::IDD)
{

}

CPage_Filter::~CPage_Filter()
{
	m_filtermap.swap(map<int, float>());  // 清空并释放内存
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


// CPage_Filter 消息处理程序
void CPage_Filter::SetCapValue(void)
{
	MAP_CAP::iterator iter; 
	for(iter = m_filtermap.begin(); iter != m_filtermap.end(); iter++)
	{
		switch(iter->first)
		{
		case UDSCAP_MULTISTREAM_VALUE: // 多流输出选项值
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

	//多流输出：默认不使用
	nCapValue = (int)(m_pUI->GetCapValueBool(UDSCAP_MULTISTREAM));
	if(nCapValue == 1) //多流选中
	{
		// 获取多流选项值并更新控件状态
		BYTE value = (BYTE)m_pUI->GetCapValueFloat(UDSCAP_MULTISTREAM_VALUE);
		// 循环判断每bit的值，并更新对应控件的状态
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
			value = value >> 1; // 始终比较最低位
		} // for end
	}//if end
	SetMultistream();

}

void CPage_Filter::SetMultistream(void)
{
	int nCapValue = (int)(m_pUI->GetCapValueBool(UDSCAP_MULTISTREAM));

	if(nCapValue == 1) //多流选中
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
		//多流输出未选中时，六个选项也均不要选中
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
	* 判断多流chebox各选项的状态;
	* 若选中则‘&’上对应bit为1的值;
	* 若未选中则'|'上对应bit为0的值.
	************************************************************/
	// 正面彩色
	if (m_check_frontcolor.GetCheck()) {  
		temp |= 0x01;
		doc_count++;
	} 
	else {
		temp &= (0xFF-0x01);
	}
	// 正面彩色
	if (m_check_frontgray.GetCheck()) {
		temp |= 0x02;
		doc_count++;
	} 
	else {
		temp &= (0xFF-0x02);
	}
	// 正面黑白
	if (m_check_frontbw.GetCheck()) {
		temp |= 0x04;
		doc_count++;
	} 
	else {
		temp &= (0xFF-0x04);
	}
	// 背面彩色
	if (m_check_backcolor.GetCheck()) {
		temp |= 0x10;
		doc_count++;
	} 
	else {
		temp &= (0xFF-0x10);
	}
	// 背面灰度
	if (m_check_backgray.GetCheck()) {
		temp |= 0x20;
		doc_count++;
	} 
	else {
		temp &= (0xFF-0x20);
	}
	// 背面黑白
	if (m_check_backbw.GetCheck()) {
		temp |= 0x40;
		doc_count++;
	} 
	else {
		temp &= (0xFF-0x40);
	}

	m_pUI->SetCapValueFloat(UDSCAP_MULTISTREAM_VALUE, (float)temp); // 保存多流选项值
	m_pUI->SetCapValueInt(UDSCAP_DOCS_IN_ADF,doc_count); // 设置纸张数

}

void CPage_Filter::InitFiltermap(void)
{
	m_filtermap.erase(m_filtermap.begin(),m_filtermap.end());//清空

	float value = m_pUI->GetCapValueFloat(UDSCAP_MULTISTREAM_VALUE);
	m_filtermap[UDSCAP_MULTISTREAM_VALUE] = value; // 初始化时添加UDSCAP_MULTISTREAM_VALUE，保证SetCapValue()会更新该Cap的值
}


void CPage_Filter::OnFilter_Btn_Check_Frontcolor()
{
	// TODO: 在此添加控件通知处理程序代码
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
	// TODO: 在此添加控件通知处理程序代码
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
	// TODO: 在此添加控件通知处理程序代码
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
	// TODO: 在此添加控件通知处理程序代码
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
	// TODO: 在此添加控件通知处理程序代码
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
	// TODO: 在此添加控件通知处理程序代码
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

	// TODO:  在此添加额外的初始化
	//2.0版本界面
	InitSliderCtrl();
	UpdateControls();
	InitFiltermap();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CPage_Filter::InitSliderCtrl()
{
	float fMin,fMax,fStep;
	m_pUI->GetCapRangeFloat(UDSCAP_FILTERLEVEL, fMin, fMax, fStep);
	m_slider_filterlevel.SetRange((int)fMin, (int)fMax);
	m_slider_filterlevel.SetTicFreq((int)fStep); //步长

	UpdateData(FALSE);  // 更新控件，刷新当前对话框
}

BOOL CPage_Filter::OnSetActive()
{
	// TODO: 在此添加专用代码和/或调用基类
	m_pUI->PreViewStatus();
	UpdateControls();
	return __super::OnSetActive();
}


void CPage_Filter::OnCbnSelchangeFilter_Combo_FilterColor()
{
	// TODO: 在此添加控件通知处理程序代码
	
}


void CPage_Filter::OnCbnSelchangeFilter_Combo_FilterMode()
{
	// TODO: 在此添加控件通知处理程序代码
	
}


void CPage_Filter::OnNMCustomdrawFilter_Slider_Filterlevel(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码


	*pResult = 0;
}


void CPage_Filter::OnEnChangeFilter_Edit_Filterlevel()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 __super::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码

}

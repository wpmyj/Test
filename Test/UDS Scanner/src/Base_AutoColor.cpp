// Base_AutoColor.cpp : 实现文件
//

#include "stdafx.h"
#include "UDS Scanner.h"
#include "Base_AutoColor.h"
#include "afxdialogex.h"


// CBase_AutoColor 对话框

IMPLEMENT_DYNAMIC(CBase_AutoColor, CPropertyPage)

CBase_AutoColor::CBase_AutoColor(MFC_UI *pUI)
	:m_pUI(pUI),CPropertyPage(CBase_AutoColor::IDD)
{
}

CBase_AutoColor::~CBase_AutoColor()
{
}

void CBase_AutoColor::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TABAUTOCOLOR_COMBO_RESOLUTION, m_combo_resolution);
	DDX_Control(pDX, IDC_TABAUTOCOLOR_COMBO_COMPRESS, m_combo_compress);
	DDX_Control(pDX, IDC_TABAUTOCOLOR_SLIDER_COMPRESSION, m_slider_compressvalue);
	DDX_Control(pDX, IDC_TABAUTOCOLOR_EDIT_COMPRESSVALUE, m_edit_compressvalue);
	DDX_Control(pDX, IDC_TABAUTOCOLOR_COMBO_NOCOLOR, m_combo_nocolor);
	DDX_Control(pDX, IDC_TABAUTOCOLOR_EDIT_COLORTHRES, m_edit_colorthres);
	DDX_Control(pDX, IDC_TABAUTOCOLOR_SLIDER_COLORTHRES, m_slider_colorthres);
}


BEGIN_MESSAGE_MAP(CBase_AutoColor, CPropertyPage)
	ON_CBN_SELCHANGE(IDC_TABAUTOCOLOR_COMBO_COMPRESS, &CBase_AutoColor::OnCbnSelchangeTabautocolor_Combo_Compress)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_TABAUTOCOLOR_SLIDER_COMPRESSION, &CBase_AutoColor::OnNMCustomdrawTabautocolor_Slider_Compressionvalue)
	ON_EN_CHANGE(IDC_TABAUTOCOLOR_EDIT_COMPRESSVALUE, &CBase_AutoColor::OnEnChangeTabautocolor_Edit_Compressvalue)
	ON_CBN_SELCHANGE(IDC_TABAUTOCOLOR_COMBO_RESOLUTION, &CBase_AutoColor::OnCbnSelchangeTabautocolor_Combo_Resolution)
	ON_CBN_SELCHANGE(IDC_TABAUTOCOLOR_COMBO_NOCOLOR, &CBase_AutoColor::OnCbnSelchangeTabautocolor_Combo_Nocolor)
	ON_EN_CHANGE(IDC_TABAUTOCOLOR_EDIT_COLORTHRES, &CBase_AutoColor::OnEnChangeTabautocolor_Edit_Colorthres)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_TABAUTOCOLOR_SLIDER_COLORTHRES, &CBase_AutoColor::OnNMCustomdrawTabautocolor_Slider_Colorthres)
END_MESSAGE_MAP()


// CBase_AutoColor 消息处理程序

void CBase_AutoColor::UpdateControls(void)
{
	int nCapIndex;
	const IntVector* lstCapValues;
	const FloatVector* lstCapValuesFlt;
	float fCapValue;
	int nCapValue;
	CString strText;

	//非彩色时扫描
	m_combo_nocolor.ResetContent();  // 清空内容
	nCapIndex = m_pUI->GetCurrentCapIndex(UDSCAP_NOCOLOR);
	lstCapValues = m_pUI->GetValidCap(UDSCAP_NOCOLOR);
	for(unsigned int i=0; i<lstCapValues->size();i++)
	{
		switch(lstCapValues->at(i))
		{
		case TWNC_GRAY:
			m_combo_nocolor.InsertString(i,"灰阶"); 
			break;
		case TWNC_BLACK:
			m_combo_nocolor.InsertString(i,"黑白"); 
			break;
		default:
			continue;
		}
	}
	m_combo_nocolor.SetCurSel(nCapIndex);

	// 色彩阈值 
	nCapValue = (int)(m_pUI->GetCapValueFloat(UDSCAP_COLORTHRESHOLD)); 
	m_slider_colorthres.SetPos(nCapValue);
	strText.Format("%d",nCapValue);
	SetDlgItemText(IDC_TABAUTOCOLOR_EDIT_COLORTHRES,strText);

	//压缩
	m_combo_compress.ResetContent();  // 清空内容
	nCapIndex = m_pUI->GetCurrentCapIndex(ICAP_COMPRESSION);
	lstCapValues = m_pUI->GetValidCap(ICAP_COMPRESSION);
	for(unsigned int i=0; i<lstCapValues->size();i++)
	{
		switch(lstCapValues->at(i))
		{
		case TWCP_NONE:
			m_combo_compress.InsertString(i,"自动"); //英寸
			break;
		case TWCP_JPEG:
			m_combo_compress.InsertString(i,"JPEG"); //像素
			break;
		case TWCP_GROUP4:
			m_combo_compress.InsertString(i,"G4"); //厘米
			break;
		default:
			continue;
		}
	}
	m_combo_compress.SetCurSel(nCapIndex);

	// 压缩比 
	nCapValue = (int)(m_pUI->GetCapValueFloat(UDSCAP_COMPRESSVALUE)); 
	m_slider_compressvalue.SetPos(nCapValue);
	strText.Format("%d",nCapValue);
	SetDlgItemText(IDC_TABAUTOCOLOR_EDIT_COMPRESSVALUE,strText);

	// 分辨率
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

}


BOOL CBase_AutoColor::OnInitDialog()
{
	__super::OnInitDialog();

	// TODO:  在此添加额外的初始化	
	InitSliderCtrl();
	UpdateControls();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CBase_AutoColor::InitSliderCtrl()
{
	float fMin,fMax,fStep;
	m_pUI->GetCapRangeFloat(UDSCAP_COMPRESSVALUE, fMin, fMax, fStep);
	m_slider_compressvalue.SetRange((int)fMin, (int)fMax);
	m_slider_compressvalue.SetTicFreq((int)fStep);

	m_pUI->GetCapRangeFloat(UDSCAP_COLORTHRESHOLD, fMin, fMax, fStep);
	m_slider_colorthres.SetRange((int)fMin, (int)fMax);
	m_slider_colorthres.SetTicFreq((int)fStep);
}


void CBase_AutoColor::OnCbnSelchangeTabautocolor_Combo_Compress()
{
	// TODO: 在此添加控件通知处理程序代码
	int nIndex = m_combo_compress.GetCurSel();
	CString strCBText; 
	m_combo_compress.GetLBText( nIndex, strCBText);
	int nval;
	if (strCBText.Find("自动") >= 0)
	{
		nval = TWCP_NONE;
	}
	else if(strCBText.Find("JPEG") >= 0)
	{
		nval = TWCP_JPEG; 
	}
	else if(strCBText.Find("G4") >= 0)
	{
		nval = TWCP_GROUP4; 
	} 
	else
	{}
	m_pUI->SetCapValueInt(ICAP_COMPRESSION, nval); 
	m_combo_compress.SetCurSel(nIndex);
}


void CBase_AutoColor::OnNMCustomdrawTabautocolor_Slider_Compressionvalue(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);  // 接收数据
	CString str;
	int sldValue = m_slider_compressvalue.GetPos();  // 获取滑块当前位置
	m_pUI->SetCapValueInt(UDSCAP_COMPRESSVALUE, sldValue); 

	str.Format("%d", sldValue);
	SetDlgItemText(IDC_TABAUTOCOLOR_EDIT_COMPRESSVALUE, str);

	*pResult = 0;
}


void CBase_AutoColor::OnEnChangeTabautocolor_Edit_Compressvalue()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 __super::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);  // 接收数据
	CString str;
	m_edit_compressvalue.GetWindowText(str);
	int nval = _ttoi(str);
	m_slider_compressvalue.SetPos(nval);

	m_pUI->SetCapValueFloat(UDSCAP_COMPRESSVALUE, (float)nval);

	m_edit_compressvalue.SetSel(str.GetLength(), str.GetLength(),TRUE);  // 设置编辑框控件范围

	UpdateData(FALSE);  // 更新控件
}


void CBase_AutoColor::OnCbnSelchangeTabautocolor_Combo_Resolution()
{
	// TODO: 在此添加控件通知处理程序代码
	int nIndex = m_combo_resolution.GetCurSel();
	CString strCBText; 
	m_combo_resolution.GetLBText(nIndex, strCBText);
	int nval = _ttoi(strCBText);  // CString 转 int

	m_pUI->SetCapValueInt(ICAP_XRESOLUTION, nval); 
	m_pUI->SetCapValueInt(ICAP_YRESOLUTION, nval); 
	m_combo_resolution.SetCurSel(nIndex);
}

void CBase_AutoColor::OnCbnSelchangeTabautocolor_Combo_Nocolor()
{
	// TODO: 在此添加控件通知处理程序代码
	int nIndex = m_combo_nocolor.GetCurSel();
	CString strCBText; 
	m_combo_nocolor.GetLBText( nIndex, strCBText);
	int nval;
	if (strCBText.Find("灰阶") >= 0)
	{
		nval = TWNC_GRAY;
	}
	else if(strCBText.Find("黑白") >= 0)
	{
		nval = TWNC_BLACK; 
	}
	else
	{}
	m_pUI->SetCapValueInt(UDSCAP_NOCOLOR, nval); 
	m_combo_nocolor.SetCurSel(nIndex);
}


void CBase_AutoColor::OnEnChangeTabautocolor_Edit_Colorthres()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 __super::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);  // 接收数据
	CString str;
	m_edit_colorthres.GetWindowText(str);
	int nval = _ttoi(str);
	m_slider_colorthres.SetPos(nval);

	m_pUI->SetCapValueFloat(UDSCAP_COLORTHRESHOLD, (float)nval);

	m_edit_colorthres.SetSel(str.GetLength(), str.GetLength(),TRUE);  // 设置编辑框控件范围

	UpdateData(FALSE);  // 更新控件
}


void CBase_AutoColor::OnNMCustomdrawTabautocolor_Slider_Colorthres(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);  // 接收数据
	CString str;
	int sldValue = m_slider_colorthres.GetPos();  // 获取滑块当前位置
	m_pUI->SetCapValueInt(UDSCAP_COLORTHRESHOLD, sldValue); 

	str.Format("%d", sldValue);
	SetDlgItemText(IDC_TABAUTOCOLOR_EDIT_COLORTHRES, str);

	*pResult = 0;
}

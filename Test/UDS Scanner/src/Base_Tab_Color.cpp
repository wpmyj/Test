// Base_Tab_Color.cpp : 实现文件
//

#include "stdafx.h"
#include "UDS Scanner.h"
#include "Base_Tab_Color.h"
#include "afxdialogex.h"

extern int basebright;
// CBase_Tab_Color 对话框

IMPLEMENT_DYNAMIC(CBase_Tab_Color, CDialogEx)

CBase_Tab_Color::CBase_Tab_Color(MFC_UI *pUI)
	: m_pUI(pUI),CDialogEx(CBase_Tab_Color::IDD)
{

}

CBase_Tab_Color::~CBase_Tab_Color()
{
}

void CBase_Tab_Color::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TABCOLOR_COMBO_COMPRESS, m_combo_compress);
	DDX_Control(pDX, IDC_TABCOLOR_SLIDER_COMPRESSION, m_slider_compressvalue);
	DDX_Control(pDX, IDC_TABCOLOR_EDIT_COMPRESSVALUE, m_edit_compressvalue);
	DDX_Control(pDX, IDC_TABCOLOR_COMBO_RESOLUTION, m_combo_resolution);
	DDX_Control(pDX, IDC_TABCOLOR_SLIDER_CONTRAST, m_slider_contrast);
	DDX_Control(pDX, IDC_TABCOLOR_EDIT_CONTRAST, m_edit_contrast);
	DDX_Control(pDX, IDC_TABCOLOR_EDIT_BRIGHTNESS, m_edit_brightness);
	DDX_Control(pDX, IDC_TABCOLOR_SLIDER_BRIGHTNESS, m_slider_brightness);
}


BEGIN_MESSAGE_MAP(CBase_Tab_Color, CDialogEx)
	ON_CBN_SELCHANGE(IDC_TABCOLOR_COMBO_COMPRESS, &CBase_Tab_Color::OnCbnSelchangeTabcolor_Combo_Compress)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_TABCOLOR_SLIDER_COMPRESSION, &CBase_Tab_Color::OnNMCustomdrawTabcolor_Slider_Compressionvalue)
	ON_EN_CHANGE(IDC_TABCOLOR_EDIT_COMPRESSVALUE, &CBase_Tab_Color::OnEnChangeTabcolor_Edit_Compressvalue)
	ON_CBN_SELCHANGE(IDC_TABCOLOR_COMBO_RESOLUTION, &CBase_Tab_Color::OnCbnSelchangeTabcolor_Combo_Resolution)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_TABCOLOR_SLIDER_CONTRAST, &CBase_Tab_Color::OnNMCustomdrawTabcolor_Slider_Contrast)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_TABCOLOR_SLIDER_BRIGHTNESS, &CBase_Tab_Color::OnNMCustomdrawTabcolor_Slider_Brightness)
	ON_EN_CHANGE(IDC_TABCOLOR_EDIT_CONTRAST, &CBase_Tab_Color::OnEnChangeTabcolor_Edit_Contrast)
	ON_EN_CHANGE(IDC_TABCOLOR_EDIT_BRIGHTNESS, &CBase_Tab_Color::OnEnChangeTabcolor_Edit_Brightness)
END_MESSAGE_MAP()


// CBase_Tab_Color 消息处理程序


void CBase_Tab_Color::UpdateControls(void)
{
	int nCapIndex;
	const IntVector* lstCapValues;
	const FloatVector* lstCapValuesFlt;
	float fCapValue;
	int nCapValue;
	CString strText;

	//多流输出：默认不使用
	int MultiCapValue = (int)(m_pUI->GetCapValueBool(UDSCAP_MULTISTREAM));

	//压缩
	m_combo_compress.ResetContent();  // 清空内容
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
	if(MultiCapValue == 0) //多流未选中
	{
		nCapIndex = m_pUI->GetCurrentCapIndex(ICAP_COMPRESSION);
	}
	else
	{
		switch(basebutton)
		{
		case 0: //正面
			nCapIndex = m_pUI->GetCurrentCapIndex(UDSCAP_COMPRESSIONFC);
			break;
		case 1: //背面
			nCapIndex = m_pUI->GetCurrentCapIndex(UDSCAP_COMPRESSIONBC);
			break;
		}
	}	
	m_combo_compress.SetCurSel(nCapIndex);

	// 压缩比 
	if(MultiCapValue == 0) //多流未选中
	{
		nCapValue = (int)(m_pUI->GetCapValueFloat(UDSCAP_COMPRESSVALUE)); 
	}
	else
	{
		switch(basebutton)
		{
		case 0: //正面
			nCapValue = (int)(m_pUI->GetCapValueFloat(UDSCAP_COMPRESSIONVALUEFC)); 
			break;
		case 1: //背面
			nCapValue = (int)(m_pUI->GetCapValueFloat(UDSCAP_COMPRESSIONVALUEBC));
			break;
		}
	}	
	m_slider_compressvalue.SetPos(nCapValue);
	strText.Format("%d",nCapValue);
	SetDlgItemText(IDC_TABCOLOR_EDIT_COMPRESSVALUE,strText);

	// 分辨率
	m_combo_resolution.ResetContent();	
	lstCapValuesFlt = m_pUI->GetValidCapFloat(ICAP_XRESOLUTION);
	for(unsigned int i=0; i<lstCapValuesFlt->size();i++)
	{
		CString strTemp;
		strTemp.Format("%d",(int)lstCapValuesFlt->at(i));
		m_combo_resolution.InsertString(i,strTemp);
	}
	if(MultiCapValue == 0) //多流未选中
	{
		nCapIndex = m_pUI->GetCurrentCapIndex(ICAP_XRESOLUTION);
	}
	else
	{
		switch(basebutton)
		{
		case 0: //正面
			nCapIndex = m_pUI->GetCurrentCapIndex(UDSCAP_RESOLUTIONFC);
			break;
		case 1: //背面
			nCapIndex = m_pUI->GetCurrentCapIndex(UDSCAP_RESOLUTIONBC);
			break;
		}
	}
	m_combo_resolution.SetCurSel(nCapIndex);
	
	//亮度
	if(MultiCapValue == 0) //多流未选中
	{
		// 亮度 
		nCapValue = (int)(m_pUI->GetCapValueFloat(ICAP_BRIGHTNESS));
	}
	else
	{
		switch(basebutton)
		{
		case 0: //正面
			nCapValue = (int)(m_pUI->GetCapValueFloat(UDSCAP_BRIGHTNESSFC));
			break;
		case 1: //背面
			nCapValue = (int)(m_pUI->GetCapValueFloat(UDSCAP_BRIGHTNESSBC));
			break;
		}
	}
	m_slider_brightness.SetPos(nCapValue);
	strText.Format("%d",nCapValue);
	SetDlgItemText(IDC_TABCOLOR_EDIT_BRIGHTNESS,strText);

	// 对比度 
	if(MultiCapValue == 0) //多流未选中
	{
		nCapValue = (int)(m_pUI->GetCapValueFloat(ICAP_CONTRAST)); 
	}
	else
	{
		switch(basebutton)
		{
		case 0: //正面
			nCapValue = (int)(m_pUI->GetCapValueFloat(UDSCAP_CONTRASTFC));
			break;
		case 1: //背面
			nCapValue = (int)(m_pUI->GetCapValueFloat(UDSCAP_CONTRASTBC));
			break;
		}
	}
	m_slider_contrast.SetPos(nCapValue);
	strText.Format("%d",nCapValue);
	SetDlgItemText(IDC_TABCOLOR_EDIT_CONTRAST,strText);

}


BOOL CBase_Tab_Color::OnInitDialog()
{
	__super::OnInitDialog();

	// TODO:  在此添加额外的初始化
	InitSliderCtrl();
	UpdateControls();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CBase_Tab_Color::InitSliderCtrl()
{
	float fMin,fMax,fStep;
	m_pUI->GetCapRangeFloat(UDSCAP_COMPRESSVALUE, fMin, fMax, fStep);
	m_slider_compressvalue.SetRange((int)fMin, (int)fMax);
	m_slider_compressvalue.SetTicFreq((int)fStep);

	m_pUI->GetCapRangeFloat(ICAP_BRIGHTNESS, fMin, fMax, fStep);
	m_slider_brightness.SetRange((int)fMin, (int)fMax);
	m_slider_brightness.SetTicFreq((int)fStep);

	m_pUI->GetCapRangeFloat(ICAP_CONTRAST, fMin, fMax, fStep);
	m_slider_contrast.SetRange((int)fMin, (int)fMax);
	m_slider_contrast.SetTicFreq((int)fStep);  // 设置滑动条刻度的频度为1个单位，很重要，若不加这句滑块初始位置不变

}


void CBase_Tab_Color::OnCbnSelchangeTabcolor_Combo_Compress()
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

	int MultiCapValue = (int)(m_pUI->GetCapValueBool(UDSCAP_MULTISTREAM));
	if(MultiCapValue == 0) //多流未选中
	{ 
		m_pUI->SetCapValueInt(ICAP_COMPRESSION, nval); 
	}
	else
	{
		switch(basebutton)
		{
		case 0: //正面
			m_pUI->SetCapValueInt(UDSCAP_COMPRESSIONFC, nval); 
			break;
		case 1: //背面
			m_pUI->SetCapValueInt(UDSCAP_COMPRESSIONBC, nval); 
			break;
		}
	}
	m_combo_compress.SetCurSel(nIndex);
}


void CBase_Tab_Color::OnNMCustomdrawTabcolor_Slider_Compressionvalue(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);  // 接收数据
	CString str;
	int sldValue = m_slider_compressvalue.GetPos();  // 获取滑块当前位置
	
	int nCapValue = (int)(m_pUI->GetCapValueBool(UDSCAP_MULTISTREAM));
	if(nCapValue == 0) //多流未选中
	{
		m_pUI->SetCapValueInt(UDSCAP_COMPRESSVALUE, sldValue); 
	}
	else
	{
		switch(basebutton)
		{
		case 0:
			m_pUI->SetCapValueInt(UDSCAP_COMPRESSIONVALUEFC, sldValue);
			break;
		case 1:
			m_pUI->SetCapValueInt(UDSCAP_COMPRESSIONVALUEBC, sldValue);
			break;
		}
	}

	str.Format("%d", sldValue);
	SetDlgItemText(IDC_TABCOLOR_EDIT_COMPRESSVALUE, str);

	*pResult = 0;
}


void CBase_Tab_Color::OnEnChangeTabcolor_Edit_Compressvalue()
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

	int nCapValue = (int)(m_pUI->GetCapValueBool(UDSCAP_MULTISTREAM));
	if(nCapValue == 0) //多流未选中
	{
		m_pUI->SetCapValueFloat(UDSCAP_COMPRESSVALUE, (float)nval);
	}
	else
	{
		switch(basebutton)
		{
		case 0:
			m_pUI->SetCapValueFloat(UDSCAP_COMPRESSIONVALUEFC, (float)nval);
			break;
		case 1:
			m_pUI->SetCapValueFloat(UDSCAP_COMPRESSIONVALUEBC, (float)nval);
			break;
		}
	}

	m_edit_compressvalue.SetSel(str.GetLength(), str.GetLength(),TRUE);  // 设置编辑框控件范围

	UpdateData(FALSE);  // 更新控件
}


void CBase_Tab_Color::OnCbnSelchangeTabcolor_Combo_Resolution()
{
	// TODO: 在此添加控件通知处理程序代码
	int nIndex = m_combo_resolution.GetCurSel();
	CString strCBText; 
	m_combo_resolution.GetLBText(nIndex, strCBText);
	int nval = _ttoi(strCBText);  // CString 转 int

	int MultiCapValue = (int)(m_pUI->GetCapValueBool(UDSCAP_MULTISTREAM));
	if(MultiCapValue == 0) //多流未选中
	{
		m_pUI->SetCapValueInt(ICAP_XRESOLUTION, nval); 
		m_pUI->SetCapValueInt(ICAP_YRESOLUTION, nval); 
	}
	else
	{
		switch(basebutton)
		{
		case 0: //正面
			m_pUI->SetCapValueInt(UDSCAP_RESOLUTIONFC, nval); 
			break;
		case 1: //背面
			m_pUI->SetCapValueInt(UDSCAP_RESOLUTIONBC, nval); 
			break;
		}
	}
	m_combo_resolution.SetCurSel(nIndex);
}


void CBase_Tab_Color::OnNMCustomdrawTabcolor_Slider_Brightness(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);  // 接收数据
	CString str;
	int sldValue = m_slider_brightness.GetPos();  // 获取滑块当前位置

	int nCapValue = (int)(m_pUI->GetCapValueBool(UDSCAP_MULTISTREAM));
	if(nCapValue == 0) //多流未选中
	{
		m_pUI->SetCapValueFloat(ICAP_BRIGHTNESS, (float)sldValue);
	}
	else
	{
		switch(basebutton)
		{
		case 0:
			m_pUI->SetCapValueFloat(UDSCAP_BRIGHTNESSFC, (float)sldValue);
			break;
		case 1:
			m_pUI->SetCapValueFloat(UDSCAP_BRIGHTNESSBC, (float)sldValue);
			break;
		}
	}
	str.Format("%d", sldValue);
	SetDlgItemText(IDC_TABCOLOR_EDIT_BRIGHTNESS,str);

	UpdateData(FALSE);  // 更新控件
	*pResult = 0;
}


void CBase_Tab_Color::OnNMCustomdrawTabcolor_Slider_Contrast(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);  // 接收数据
	CString str;
	int sldValue = m_slider_contrast.GetPos();  // 获取滑块当前位置

	int nCapValue = (int)(m_pUI->GetCapValueBool(UDSCAP_MULTISTREAM));
	if(nCapValue == 0) //多流未选中
	{
		m_pUI->SetCapValueFloat(ICAP_CONTRAST, (float)sldValue); 
	}
	else
	{
		switch(basebutton)
		{
		case 0:
			m_pUI->SetCapValueFloat(UDSCAP_CONTRASTFC, (float)sldValue);
			break;
		case 1:
			m_pUI->SetCapValueFloat(UDSCAP_CONTRASTBC, (float)sldValue);
			break;
		}
	}
	
	str.Format("%d", sldValue);
	SetDlgItemText(IDC_TABCOLOR_EDIT_CONTRAST, str);

	UpdateData(FALSE);  // 更新控件

	*pResult = 0;
}


void CBase_Tab_Color::OnEnChangeTabcolor_Edit_Brightness()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 __super::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);  // 接收数据
	CString str;
	m_edit_brightness.GetWindowText(str);
	int nval = _ttoi(str);
	m_slider_brightness.SetPos(nval);

	int nCapValue = (int)(m_pUI->GetCapValueBool(UDSCAP_MULTISTREAM));
	if(nCapValue == 0) //多流未选中
	{
		m_pUI->SetCapValueFloat(ICAP_BRIGHTNESS, (float)nval);
	}
	else
	{
		switch(basebutton)
		{
		case 0:
			m_pUI->SetCapValueFloat(UDSCAP_BRIGHTNESSFC, (float)nval);
			break;
		case 1:
			m_pUI->SetCapValueFloat(UDSCAP_BRIGHTNESSBC, (float)nval);
			break;
		}
	}
	
	m_edit_brightness.SetSel(str.GetLength(), str.GetLength(),TRUE);  // 设置编辑框控件范围

	UpdateData(FALSE);  // 更新控件
}


void CBase_Tab_Color::OnEnChangeTabcolor_Edit_Contrast()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 __super::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);  // 接收数据
	CString str;
	m_edit_contrast.GetWindowText(str);
	int nval = _ttoi(str);
	m_slider_contrast.SetPos(nval);

	int nCapValue = (int)(m_pUI->GetCapValueBool(UDSCAP_MULTISTREAM));
	if(nCapValue == 0) //多流未选中
	{
		m_pUI->SetCapValueFloat(ICAP_CONTRAST, (float)nval); 
	}
	else
	{
		switch(basebutton)
		{
		case 0:
			m_pUI->SetCapValueFloat(UDSCAP_CONTRASTFC, (float)nval);
			break;
		case 1:
			m_pUI->SetCapValueFloat(UDSCAP_CONTRASTBC, (float)nval);
			break;
		}
	}

	m_edit_contrast.SetSel(str.GetLength(), str.GetLength(),TRUE);  // 设置编辑框控件范围

	UpdateData(FALSE);  // 更新控件
}




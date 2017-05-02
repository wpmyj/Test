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
	DDX_Control(pDX, IDC_TABCOLOR_SLIDER_COMPRESSION, m_slider_compressvalue);
	DDX_Control(pDX, IDC_TABCOLOR_EDIT_COMPRESSVALUE, m_edit_compressvalue);
	DDX_Control(pDX, IDC_TABCOLOR_SLIDER_CONTRAST, m_slider_contrast);
	DDX_Control(pDX, IDC_TABCOLOR_EDIT_CONTRAST, m_edit_contrast);
	DDX_Control(pDX, IDC_TABCOLOR_EDIT_BRIGHTNESS, m_edit_brightness);
	DDX_Control(pDX, IDC_TABCOLOR_SLIDER_BRIGHTNESS, m_slider_brightness);
	DDX_Control(pDX, IDC_TABCOLOR_COMBO_RESOLUTION, m_combo_resolution);
	//  DDX_Radio(pDX, IDC_TABCOLOR_RADIO_COMPRESS_AUTO, m_radio_compress);
	DDX_Control(pDX, IDC_TABCOLOR_COMBO_COMPRESSQUALITY, m_combo_compressquality);
}


BEGIN_MESSAGE_MAP(CBase_Tab_Color, CDialogEx)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_TABCOLOR_SLIDER_COMPRESSION, &CBase_Tab_Color::OnNMCustomdrawTabcolor_Slider_Compressionvalue)
	ON_EN_CHANGE(IDC_TABCOLOR_EDIT_COMPRESSVALUE, &CBase_Tab_Color::OnEnChangeTabcolor_Edit_Compressvalue)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_TABCOLOR_SLIDER_CONTRAST, &CBase_Tab_Color::OnNMCustomdrawTabcolor_Slider_Contrast)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_TABCOLOR_SLIDER_BRIGHTNESS, &CBase_Tab_Color::OnNMCustomdrawTabcolor_Slider_Brightness)
	ON_EN_CHANGE(IDC_TABCOLOR_EDIT_CONTRAST, &CBase_Tab_Color::OnEnChangeTabcolor_Edit_Contrast)
	ON_EN_CHANGE(IDC_TABCOLOR_EDIT_BRIGHTNESS, &CBase_Tab_Color::OnEnChangeTabcolor_Edit_Brightness)
//	ON_BN_CLICKED(IDC_TABCOLOR_RADIO_COMPRESS_AUTO, &CBase_Tab_Color::OnTabColor_RadioBtn_Compress)
//	ON_BN_CLICKED(IDC_TABCOLOR_RADIO_COMPRESS_JPEG, &CBase_Tab_Color::OnTabColor_RadioBtn_Compress)
	ON_CBN_SELCHANGE(IDC_TABCOLOR_COMBO_RESOLUTION, &CBase_Tab_Color::OnCbnSelchangeTabcolor_Combo_Resolution)
	ON_CBN_SELCHANGE(IDC_TABCOLOR_COMBO_COMPRESSQUALITY, &CBase_Tab_Color::OnCbnSelchangeTabcolor_Combo_Compressquality)
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

	// 压缩质量 
	m_combo_compressquality.ResetContent();
	lstCapValues = m_pUI->GetValidCap(UDSCAP_COMPRESSQUALITY);
	for(unsigned int i=0; i<lstCapValues->size();i++)
	{
		switch(lstCapValues->at(i))
		{
		case TWCQ_BEST:
			m_combo_compressquality.InsertString(i,"最佳");
			break;
		case TWCQ_FINE:
			m_combo_compressquality.InsertString(i,"良好");
			break;
		case TWCQ_JUST:
			m_combo_compressquality.InsertString(i,"一般");
			break;
		case TWCQ_DEFINED:
			m_combo_compressquality.InsertString(i,"自定义");
			break;
		default:
			continue;
		}
	}
	if(MultiCapValue == 0) //多流未选中
	{
		nCapIndex = m_pUI->GetCurrentCapIndex(UDSCAP_COMPRESSQUALITY);
	}
	else
	{
		switch(basebutton)
		{
		case 0: //正面
			nCapIndex = m_pUI->GetCurrentCapIndex(UDSCAP_COMPRESSQUALITYFC);
			break;
		case 1: //背面
			nCapIndex = m_pUI->GetCurrentCapIndex(UDSCAP_COMPRESSQUALITYBC);
			break;
		}
	}
	m_combo_compressquality.SetCurSel(nCapIndex);
	SetCompressValue();

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
	//if(nCapIndex==2)
	//{
	//	m_radio_compress = nCapIndex - 1; //0为自动、1为JPEG,2为G4
	//}	
	//else
	//{
	//	m_radio_compress = nCapIndex; //0为自动、1为JPEG，2为G4
	//}
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


//void CBase_Tab_Color::OnCbnSelchangeTabcolor_Combo_Compress()
//{
//	// TODO: 在此添加控件通知处理程序代码
//	int nIndex = m_combo_compress.GetCurSel();
//	CString strCBText; 
//	m_combo_compress.GetLBText( nIndex, strCBText);
//	int nval;
//	if (strCBText.Find("自动") >= 0)
//	{
//		nval = TWCP_NONE;
//	}
//	else if(strCBText.Find("JPEG") >= 0)
//	{
//		nval = TWCP_JPEG; 
//	}
//	else if(strCBText.Find("G4") >= 0)
//	{
//		nval = TWCP_GROUP4; 
//	} 
//	else
//	{}
//
//	int MultiCapValue = (int)(m_pUI->GetCapValueBool(UDSCAP_MULTISTREAM));
//	if(MultiCapValue == 0) //多流未选中
//	{ 
//		m_pUI->SetCapValueInt(ICAP_COMPRESSION, nval); 
//	}
//	else
//	{
//		switch(basebutton)
//		{
//		case 0: //正面
//			m_pUI->SetCapValueInt(UDSCAP_COMPRESSIONFC, nval); 
//			break;
//		case 1: //背面
//			m_pUI->SetCapValueInt(UDSCAP_COMPRESSIONBC, nval); 
//			break;
//		}
//	}
//	m_combo_compress.SetCurSel(nIndex);
//}


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


//void CBase_Tab_Color::OnTabColor_RadioBtn_Compress()
//{
//	// TODO: 在此添加控件通知处理程序代码
//	UpdateData(TRUE); //将radio的状态值更新给关联的变量
//	int MultiCapValue = (int)(m_pUI->GetCapValueBool(UDSCAP_MULTISTREAM));
//	switch(m_radio_compress)
//	{
//	case 0:
//		if(MultiCapValue == 0) //多流未选中
//		{
//			m_pUI->SetCapValueInt(ICAP_COMPRESSION,TWCP_NONE); 
//		}
//		else
//		{
//			switch(basebutton)
//			{
//			case 0: //正面
//				m_pUI->SetCapValueInt(UDSCAP_COMPRESSIONFC,TWCP_NONE); 
//				break;
//			case 1: //背面
//				m_pUI->SetCapValueInt(UDSCAP_COMPRESSIONBC,TWCP_NONE); 
//				break;
//			}
//		}
//
//		break;
//	case 1:
//		if(MultiCapValue == 0) //多流未选中
//		{
//			m_pUI->SetCapValueInt(ICAP_COMPRESSION,TWCP_JPEG); 
//		}
//		else
//		{
//			switch(basebutton)
//			{
//			case 0: //正面
//				m_pUI->SetCapValueInt(UDSCAP_COMPRESSIONFC,TWCP_JPEG); 
//				break;
//			case 1: //背面
//				m_pUI->SetCapValueInt(UDSCAP_COMPRESSIONBC,TWCP_JPEG); 
//				break;
//			}
//		}
//		break;
//	}
//	UpdateData(FALSE);
//}


void CBase_Tab_Color::OnCbnSelchangeTabcolor_Combo_Compressquality()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	int nIndex = m_combo_compressquality.GetCurSel();
	CString strCBText; 
	m_combo_compressquality.GetLBText( nIndex, strCBText);
	int nval;
	if (strCBText.Find("最佳") >= 0) 
	{
		nval = TWCQ_BEST;	
	}
	else if(strCBText.Find("良好") >= 0) 
	{
		nval = TWCQ_FINE; 
	}
	else if(strCBText.Find("一般") >= 0) 
	{
		nval = TWCQ_JUST; 
	}
	else if(strCBText.Find("自定义") >= 0) 
	{
		nval = TWCQ_DEFINED; 
	}
	else
	{}

	int MultiCapValue = (int)(m_pUI->GetCapValueBool(UDSCAP_MULTISTREAM));
	if(MultiCapValue == 0) //多流未选中
	{
		m_pUI->SetCapValueInt(UDSCAP_COMPRESSQUALITY,nval); //设置参数生效	
	}
	else
	{
		switch(basebutton)
		{
		case 0: //正面
			m_pUI->SetCapValueInt(UDSCAP_COMPRESSQUALITYFC, nval); 
			break;
		case 1: //背面
			m_pUI->SetCapValueInt(UDSCAP_COMPRESSQUALITYBC, nval); 
			break;
		}
	}
	m_combo_compressquality.SetCurSel(nIndex);
	SetCompressValue();
}

void CBase_Tab_Color::SetCompressValue()
{
	int nIndex = m_combo_compressquality.GetCurSel();
	if(nIndex == 3)
	{
		m_slider_compressvalue.EnableWindow(TRUE);
		m_edit_compressvalue.EnableWindow(TRUE);
	}
	else
	{
		m_slider_compressvalue.EnableWindow(FALSE);
		m_edit_compressvalue.EnableWindow(FALSE);
	}
}
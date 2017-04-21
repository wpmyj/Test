// Base_Tab_Gray.cpp : 实现文件
//

#include "stdafx.h"
#include "UDS Scanner.h"
#include "Base_Tab_Gray.h"
#include "afxdialogex.h"


// CBase_Tab_Gray 对话框

IMPLEMENT_DYNAMIC(CBase_Tab_Gray, CDialogEx)

CBase_Tab_Gray::CBase_Tab_Gray(MFC_UI *pUI)
	: m_pUI(pUI),CDialogEx(CBase_Tab_Gray::IDD)
{
}

CBase_Tab_Gray::~CBase_Tab_Gray()
{
}

void CBase_Tab_Gray::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TABGRAY_COMBO_RESOLUTION, m_combo_resolution);
	DDX_Control(pDX, IDC_TABGRAY_SLIDER_CONTRAST, m_slider_contrast);
	DDX_Control(pDX, IDC_TABGRAY_SLIDER_BRIGHTNESS, m_slider_brightness);
	DDX_Control(pDX, IDC_TABGRAY_EDIT_CONTRAST, m_edit_contrast);
	DDX_Control(pDX, IDC_TABGRAY_EDIT_BRIGHTNESS, m_edit_brightness);
	DDX_Radio(pDX, IDC_TABGRAY_RADIO_COMPRESS_AUTO, m_radio_compress);
	DDX_Control(pDX, IDC_TABGRAY_SLIDER_COMPRESSION, m_slider_compressvalue);
	DDX_Control(pDX, IDC_TABGRAY_EDIT_COMPRESSVALUE, m_edit_compressvalue);
	DDX_Control(pDX, IDC_TABGRAY_COMBO_COLOR, m_combo_filtercolor);
	DDX_Control(pDX, IDC_TABGRAY_COMBO_FILTERMODE, m_combo_filtermode);
	DDX_Control(pDX, IDC_TABGRAY_SLIDER_FILTERLEVEL, m_slider_filterlevel);
	DDX_Control(pDX, IDC_TABGRAY_EDIT_FILTERLEVEL, m_edit_filterlevel);
	DDX_Control(pDX, IDC_TABGRAY_COMBO_COMPRESSQUALITY, m_combo_compressquality);
}


BEGIN_MESSAGE_MAP(CBase_Tab_Gray, CDialogEx)
	ON_CBN_SELCHANGE(IDC_TABGRAY_COMBO_RESOLUTION, &CBase_Tab_Gray::OnCbnSelchangeTabgray_Combo_Resolution)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_TABGRAY_SLIDER_CONTRAST, &CBase_Tab_Gray::OnNMCustomdrawTabgray_Slider_Contrast)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_TABGRAY_SLIDER_BRIGHTNESS, &CBase_Tab_Gray::OnNMCustomdrawTabgray_Slider_Brightness)
	ON_EN_CHANGE(IDC_TABGRAY_EDIT_CONTRAST, &CBase_Tab_Gray::OnEnChangeTabgray_Edit_Contrast)
	ON_EN_CHANGE(IDC_TABGRAY_EDIT_BRIGHTNESS, &CBase_Tab_Gray::OnEnChangeTabgray_Edit_Brightness)
	ON_BN_CLICKED(IDC_TABGRAY_RADIO_COMPRESS_AUTO, &CBase_Tab_Gray::OnTabGray_RadioBtn_Compress)
	ON_BN_CLICKED(IDC_TABGRAY_RADIO_COMPRESS_JPEG, &CBase_Tab_Gray::OnTabGray_RadioBtn_Compress)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_TABGRAY_SLIDER_COMPRESSION, &CBase_Tab_Gray::OnNMCustomdrawTabgray_Slider_Compressionvalue)
	ON_EN_CHANGE(IDC_TABGRAY_EDIT_COMPRESSVALUE, &CBase_Tab_Gray::OnEnChangeTabgray_Edit_Compressvalue)
	ON_CBN_SELCHANGE(IDC_TABGRAY_COMBO_COLOR, &CBase_Tab_Gray::OnCbnSelchangeTabGray_Combo_FilterColor)
	ON_CBN_SELCHANGE(IDC_TABGRAY_COMBO_FILTERMODE, &CBase_Tab_Gray::OnCbnSelchangeTabGray_Combo_FilterMode)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_TABGRAY_SLIDER_FILTERLEVEL, &CBase_Tab_Gray::OnNMCustomdrawTabGray_Slider_Filterlevel)
	ON_EN_CHANGE(IDC_TABGRAY_EDIT_FILTERLEVEL, &CBase_Tab_Gray::OnEnChangeTabGray_Edit_Filterlevel)
	ON_CBN_SELCHANGE(IDC_TABGRAY_COMBO_COMPRESSQUALITY, &CBase_Tab_Gray::OnCbnSelchangeTabgray_Combo_Compressquality)
END_MESSAGE_MAP()


// CBase_Tab_Gray 消息处理程序


void CBase_Tab_Gray::UpdateControls(void)
{
	UpdateData(TRUE);
	int nCapIndex;
	const IntVector* lstCapValues;
	const FloatVector* lstCapValuesFlt;
	float fCapValue;
	int nCapValue;
	CString strText;
	int nval;
	//多流输出：默认不使用
	int MultiCapValue = (int)(m_pUI->GetCapValueBool(UDSCAP_MULTISTREAM));\
	
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
			nCapIndex = m_pUI->GetCurrentCapIndex(UDSCAP_RESOLUTIONFG);
			break;
		case 1: //背面
			nCapIndex = m_pUI->GetCurrentCapIndex(UDSCAP_RESOLUTIONBG);
			break;
		}
	}
	m_combo_resolution.SetCurSel(nCapIndex);
	nval = (int)lstCapValuesFlt->at(nCapIndex);

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
			nCapValue = (int)(m_pUI->GetCapValueFloat(UDSCAP_BRIGHTNESSFG));
			break;
		case 1: //背面
			nCapValue = (int)(m_pUI->GetCapValueFloat(UDSCAP_BRIGHTNESSBG));
			break;
		}
	}
	m_slider_brightness.SetPos(nCapValue);
	strText.Format("%d",nCapValue);
	SetDlgItemText(IDC_TABGRAY_EDIT_BRIGHTNESS,strText);

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
			nCapValue = (int)(m_pUI->GetCapValueFloat(UDSCAP_CONTRASTFG));
			break;
		case 1: //背面
			nCapValue = (int)(m_pUI->GetCapValueFloat(UDSCAP_CONTRASTBG));
			break;
		}
	}
	m_slider_contrast.SetPos(nCapValue);
	strText.Format("%d",nCapValue);
	SetDlgItemText(IDC_TABGRAY_EDIT_CONTRAST,strText);

	//滤除颜色
	m_combo_filtercolor.ResetContent(); 
	nCapIndex = m_pUI->GetCurrentCapIndex(UDSCAP_FILTERCOLOR);
	lstCapValues = m_pUI->GetValidCap(UDSCAP_FILTERCOLOR);
	for(unsigned int i=0; i<lstCapValues->size();i++)
	{
		switch(lstCapValues->at(i))
		{
		case TWFL_NONE:
			m_combo_filtercolor.InsertString(i,"不滤除"); 
			break;
		case TWFL_RED:
			m_combo_filtercolor.InsertString(i,"红色"); 
			break;
		case TWFL_GREEN:
			m_combo_filtercolor.InsertString(i,"绿色"); 
			break;
		case TWFL_BLUE:
			m_combo_filtercolor.InsertString(i,"蓝色"); 
			break;
		default:
			continue;
		}
	}
	m_combo_filtercolor.SetCurSel(nCapIndex);

	//滤除模式
	m_combo_filtermode.ResetContent(); 
	nCapIndex = m_pUI->GetCurrentCapIndex(UDSCAP_FILTERMODE);
	lstCapValues = m_pUI->GetValidCap(UDSCAP_FILTERMODE);
	for(unsigned int i=0; i<lstCapValues->size();i++)
	{
		switch(lstCapValues->at(i))
		{
		case TWFL_NONE:
			m_combo_filtermode.InsertString(i,"自动"); 
			break;
		case TWFL_RED:
			m_combo_filtermode.InsertString(i,"自定义"); 
			break;
		default:
			continue;
		}
	}
	m_combo_filtermode.SetCurSel(nCapIndex);

	//滤除程度
	nCapValue = (int)m_pUI->GetCapValueFloat(UDSCAP_FILTERLEVEL);
	m_slider_filterlevel.SetPos(nCapValue);
	strText.Format("%d",nCapValue);
	SetDlgItemText(IDC_TABGRAY_EDIT_FILTERLEVEL,strText);

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
			nCapIndex = m_pUI->GetCurrentCapIndex(UDSCAP_COMPRESSQUALITYFG);
			break;
		case 1: //背面
			nCapIndex = m_pUI->GetCurrentCapIndex(UDSCAP_COMPRESSQUALITYBG);
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
			nCapValue = (int)(m_pUI->GetCapValueFloat(UDSCAP_COMPRESSIONVALUEFG)); 
			break;
		case 1: //背面
			nCapValue = (int)(m_pUI->GetCapValueFloat(UDSCAP_COMPRESSIONVALUEBG));
			break;
		}
	}
	m_slider_compressvalue.SetPos(nCapValue);
	strText.Format("%d",nCapValue);
	SetDlgItemText(IDC_TABGRAY_EDIT_COMPRESSVALUE,strText);

	//压缩
	if(MultiCapValue == 0) //多流未选中
	{
		nCapIndex = m_pUI->GetCurrentCapIndex(ICAP_COMPRESSION);
	}
	else
	{
		switch(basebutton)
		{
		case 0: //正面
			nCapIndex = m_pUI->GetCurrentCapIndex(UDSCAP_COMPRESSIONFG);	
			break;
		case 1: //背面
			nCapIndex = m_pUI->GetCurrentCapIndex(UDSCAP_COMPRESSIONBG);
			break;
		}
	}	
	if(nCapIndex==2)
	{
		m_radio_compress = nCapIndex - 1; //0为自动、1为JPEG,2为G4
	}	
	else
	{
		m_radio_compress = nCapIndex; //0为自动、1为JPEG，2为G4
	}
	UpdateData(FALSE);
}


BOOL CBase_Tab_Gray::OnInitDialog()
{
	__super::OnInitDialog();

	// TODO:  在此添加额外的初始化
	InitSliderCtrl();
	UpdateControls();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CBase_Tab_Gray::InitSliderCtrl()
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
	
	m_pUI->GetCapRangeFloat(UDSCAP_FILTERLEVEL, fMin, fMax, fStep);
	m_slider_filterlevel.SetRange((int)fMin, (int)fMax);
	m_slider_filterlevel.SetTicFreq((int)fStep); //步长
}


//void CBase_Tab_Gray::OnCbnSelchangeTabgray_Combo_Compress()
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
//			m_pUI->SetCapValueInt(UDSCAP_COMPRESSIONFG, nval); 
//			break;
//		case 1: //背面
//			m_pUI->SetCapValueInt(UDSCAP_COMPRESSIONBG, nval); 
//			break;
//		}
//	}
//
//	m_combo_compress.SetCurSel(nIndex);
//}


void CBase_Tab_Gray::OnNMCustomdrawTabgray_Slider_Compressionvalue(NMHDR *pNMHDR, LRESULT *pResult)
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
			m_pUI->SetCapValueInt(UDSCAP_COMPRESSIONVALUEFG, sldValue);
			break;
		case 1:
			m_pUI->SetCapValueInt(UDSCAP_COMPRESSIONVALUEBG, sldValue);
			break;
		}
	}

	str.Format("%d", sldValue);
	SetDlgItemText(IDC_TABGRAY_EDIT_COMPRESSVALUE, str);

	*pResult = 0;
}


void CBase_Tab_Gray::OnEnChangeTabgray_Edit_Compressvalue()
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
			m_pUI->SetCapValueFloat(UDSCAP_COMPRESSIONVALUEFG, (float)nval);
			break;
		case 1:
			m_pUI->SetCapValueFloat(UDSCAP_COMPRESSIONVALUEBG, (float)nval);
			break;
		}
	}

	m_edit_compressvalue.SetSel(str.GetLength(), str.GetLength(),TRUE);  // 设置编辑框控件范围

	UpdateData(FALSE);  // 更新控件
}


void CBase_Tab_Gray::OnCbnSelchangeTabgray_Combo_Resolution()
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
			m_pUI->SetCapValueInt(UDSCAP_RESOLUTIONFG, nval); 
			break;
		case 1: //背面
			m_pUI->SetCapValueInt(UDSCAP_RESOLUTIONBG, nval); 
			break;
		}
	}
	m_combo_resolution.SetCurSel(nIndex);
}


void CBase_Tab_Gray::OnNMCustomdrawTabgray_Slider_Brightness(NMHDR *pNMHDR, LRESULT *pResult)
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
			m_pUI->SetCapValueFloat(UDSCAP_BRIGHTNESSFG, (float)sldValue);
			break;
		case 1:
			m_pUI->SetCapValueFloat(UDSCAP_BRIGHTNESSBG, (float)sldValue);
			break;
		}
	}
	str.Format("%d", sldValue);
	SetDlgItemText(IDC_TABGRAY_EDIT_BRIGHTNESS,str);

	UpdateData(FALSE);  // 更新控件
	*pResult = 0;
}


void CBase_Tab_Gray::OnNMCustomdrawTabgray_Slider_Contrast(NMHDR *pNMHDR, LRESULT *pResult)
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
			m_pUI->SetCapValueFloat(UDSCAP_CONTRASTFG, (float)sldValue);
			break;
		case 1:
			m_pUI->SetCapValueFloat(UDSCAP_CONTRASTBG, (float)sldValue);
			break;
		}
	}

	str.Format("%d", sldValue);
	SetDlgItemText(IDC_TABGRAY_EDIT_CONTRAST, str);

	UpdateData(FALSE);  // 更新控件
	*pResult = 0;
}


void CBase_Tab_Gray::OnEnChangeTabgray_Edit_Brightness()
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
			m_pUI->SetCapValueFloat(UDSCAP_BRIGHTNESSFG, (float)nval);
			break;
		case 1:
			m_pUI->SetCapValueFloat(UDSCAP_BRIGHTNESSBG, (float)nval);
			break;
		}
	}
	
	m_edit_brightness.SetSel(str.GetLength(), str.GetLength(),TRUE);  // 设置编辑框控件范围

	UpdateData(FALSE);  // 更新控件
}


void CBase_Tab_Gray::OnEnChangeTabgray_Edit_Contrast()
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
			m_pUI->SetCapValueFloat(UDSCAP_CONTRASTFG, (float)nval);
			break;
		case 1:
			m_pUI->SetCapValueFloat(UDSCAP_CONTRASTBG, (float)nval);
			break;
		}
	}

	m_edit_contrast.SetSel(str.GetLength(), str.GetLength(),TRUE);  // 设置编辑框控件范围

	UpdateData(FALSE);  // 更新控件
}


void CBase_Tab_Gray::OnTabGray_RadioBtn_Compress()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE); //将radio的状态值更新给关联的变量
	int MultiCapValue = (int)(m_pUI->GetCapValueBool(UDSCAP_MULTISTREAM));
	switch(m_radio_compress)
	{
	case 0:
		if(MultiCapValue == 0) //多流未选中
		{
			m_pUI->SetCapValueInt(ICAP_COMPRESSION,TWCP_NONE); 
		}
		else
		{
			switch(basebutton)
			{
			case 0: //正面
				m_pUI->SetCapValueInt(UDSCAP_COMPRESSIONFG,TWCP_NONE); 
				break;
			case 1: //背面
				m_pUI->SetCapValueInt(UDSCAP_COMPRESSIONBG,TWCP_NONE); 
				break;
			}
		}

		break;
	case 1:
		if(MultiCapValue == 0) //多流未选中
		{
			m_pUI->SetCapValueInt(ICAP_COMPRESSION,TWCP_JPEG); 
		}
		else
		{
			switch(basebutton)
			{
			case 0: //正面
				m_pUI->SetCapValueInt(UDSCAP_COMPRESSIONFG,TWCP_JPEG); 
				break;
			case 1: //背面
				m_pUI->SetCapValueInt(UDSCAP_COMPRESSIONBG,TWCP_JPEG); 
				break;
			}
		}
		break;
	}
	UpdateData(FALSE);
}


void CBase_Tab_Gray::OnCbnSelchangeTabGray_Combo_FilterColor()
{
	// TODO: 在此添加控件通知处理程序代码
	int nIndex = m_combo_filtercolor.GetCurSel();
	CString strCBText; 
	m_combo_filtercolor.GetLBText( nIndex, strCBText);
	int nval;
	if (strCBText.Find("不滤除") >= 0)
	{
		nval = TWFL_NONE;
	}
	else if(strCBText.Find("红色") >= 0)
	{
		nval = TWFL_RED; 
	}
	else if(strCBText.Find("绿色") >= 0)
	{
		nval = TWFL_GREEN; 
	}
	else if(strCBText.Find("蓝色") >= 0)
	{
		nval = TWFL_BLUE; 
	}
	else if(strCBText.Find("自动") >= 0)
	{
		nval = TWFL_AUTO; 
	}
	else
	{}

	m_pUI->SetCapValueInt(UDSCAP_FILTERCOLOR,nval); 
	m_combo_filtercolor.SetCurSel(nIndex);
}


void CBase_Tab_Gray::OnCbnSelchangeTabGray_Combo_FilterMode()
{
	// TODO: 在此添加控件通知处理程序代码
	int nIndex = m_combo_filtermode.GetCurSel();
	CString strCBText; 
	m_combo_filtermode.GetLBText( nIndex, strCBText);
	int nval;
	if (strCBText.Find("自动") >= 0)
	{
		nval = TWFM_AUTO;
	}
	else if(strCBText.Find("自定义") >= 0)
	{
		nval = TWFM_DEFINED; 
	}
	else
	{}

	m_pUI->SetCapValueInt(UDSCAP_FILTERMODE,nval); 
	m_combo_filtermode.SetCurSel(nIndex);
}


void CBase_Tab_Gray::OnNMCustomdrawTabGray_Slider_Filterlevel(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);  // 接收数据
	CString str;
	int sldValue = m_slider_filterlevel.GetPos();  // 获取滑块当前位置
	str.Format("%d",sldValue);
	SetDlgItemText(IDC_TABGRAY_EDIT_FILTERLEVEL, str);// 在编辑框同步显示滚动条值
	UpdateData(FALSE);  // 更新控件
	*pResult = 0;
}


void CBase_Tab_Gray::OnEnChangeTabGray_Edit_Filterlevel()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 __super::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);  // 接收数据
	CString str;
	m_edit_filterlevel.GetWindowText(str);
	int nval = _ttoi(str);
	m_slider_filterlevel.SetPos(nval);
	m_pUI->SetCapValueFloat(UDSCAP_FILTERLEVEL,(float)nval);

	m_edit_filterlevel.SetSel(str.GetLength(), str.GetLength(),TRUE);  // 设置编辑框控件范围
	UpdateData(FALSE);  // 更新控件
}


void CBase_Tab_Gray::OnCbnSelchangeTabgray_Combo_Compressquality()
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
			m_pUI->SetCapValueInt(UDSCAP_COMPRESSQUALITYFG, nval); 
			break;
		case 1: //背面
			m_pUI->SetCapValueInt(UDSCAP_COMPRESSQUALITYBG, nval); 
			break;
		}
	}
	m_combo_compressquality.SetCurSel(nIndex);
	SetCompressValue();
}

void CBase_Tab_Gray::SetCompressValue()
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
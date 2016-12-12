// Page_Paper.cpp : 实现文件
//

#include "stdafx.h"
#include "UDS Scanner.h"
#include "Page_Paper.h"
#include "afxdialogex.h"


// CPage_Paper 对话框

IMPLEMENT_DYNAMIC(CPage_Paper, CPropertyPage)

CPage_Paper::CPage_Paper(MFC_UI *pUI)
	: m_pUI(pUI),CPropertyPage(CPage_Paper::IDD)
{

}

CPage_Paper::~CPage_Paper()
{
}

void CPage_Paper::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PAPER_COMBO_ORIENTATION, m_combo_orientation);
	DDX_Control(pDX, IDC_PAPER_COMBO_STANDARDSIZES, m_combo_standardsizes);
	DDX_Control(pDX, IDC_PAPER_COMBO_UINTS, m_combo_uints);
	DDX_Control(pDX, IDC_PAPER_EDIT_WIDTH, m_edit_width);
	DDX_Control(pDX, IDC_PAPER_EDIT_HEIGHT, m_edit_height);
	DDX_Control(pDX, IDC_PAPER_EDIT_DOWN, m_edit_down);
	DDX_Control(pDX, IDC_PAPER_EDIT_LEFT, m_edit_left);
	DDX_Control(pDX, IDC_PAPER_EDIT_RIGHT, m_edit_right);
	DDX_Control(pDX, IDC_PAPER_EDIT_UP, m_edit_up);
	DDX_Control(pDX, IDC_PAPER_EDIT_XPOS, m_edit_xpos);
	DDX_Control(pDX, IDC_PAPER_EDIT_YPOS, m_edit_ypos);
	DDX_Control(pDX, IDC_PAPER_SCROLLBAR_XPOS, m_scroll_xpos);
	DDX_Control(pDX, IDC_PAPER_SCROLLBAR_YPOS, m_scroll_ypos);
	DDX_Control(pDX, IDC_PAPER_SCROLLBAR_WIDTH, m_scroll_width);
	DDX_Control(pDX, IDC_PAPER_SCROLLBAR_UP, m_scroll_up);
	DDX_Control(pDX, IDC_PAPER_SCROLLBAR_RIGHT, m_scroll_right);
	DDX_Control(pDX, IDC_PAPER_SCROLLBAR_LEFT, m_scroll_left);
	DDX_Control(pDX, IDC_PAPER_SCROLLBAR_HEIGHT, m_scroll_height);
	DDX_Control(pDX, IDC_PAPER_SCROLLBAR_DOWN, m_scroll_down);
}


BEGIN_MESSAGE_MAP(CPage_Paper, CPropertyPage)
	ON_CBN_SELCHANGE(IDC_PAPER_COMBO_ORIENTATION, &CPage_Paper::OnCbnSelchangePaper_Combo_Orientation)
	ON_CBN_SELCHANGE(IDC_PAPER_COMBO_STANDARDSIZES, &CPage_Paper::OnCbnSelchangePaper_Combo_Standardsizes)
	ON_CBN_SELCHANGE(IDC_PAPER_COMBO_UINTS, &CPage_Paper::OnCbnSelchangePaper_Combo_Uints)
	ON_WM_VSCROLL()
END_MESSAGE_MAP()


// CPage_Paper 消息处理程序

void CPage_Paper::UpdateControls(void)
{
	int nCapIndex;
	const IntVector* lstCapValues;
	//const FloatVector* lstCapValuesFlt; //暂时未用到

  //纸张设置-纸张方向
	m_combo_orientation.ResetContent();  // 清空内容
	nCapIndex = m_pUI->GetCurrentCapIndex(ICAP_ORIENTATION);
	lstCapValues = m_pUI->GetValidCap(ICAP_ORIENTATION);
	for(unsigned int i=0; i<lstCapValues->size();i++)
	{
		switch(lstCapValues->at(i))
		{
		case TWOR_PORTRAIT:
			m_combo_orientation.InsertString(i,"纵向");
			break;
		case TWOR_LANDSCAPE:
			m_combo_orientation.InsertString(i,"横向");
			break;
		default:
			continue;
		}	
	}
	m_combo_orientation.SetCurSel(nCapIndex);  // 显示默认值

	//纸张设置-纸张大小
	m_combo_standardsizes.ResetContent();  // 清空内容
	nCapIndex = m_pUI->GetCurrentCapIndex(ICAP_SUPPORTEDSIZES);
	lstCapValues = m_pUI->GetValidCap(ICAP_SUPPORTEDSIZES);
	for(unsigned int i=0; i<lstCapValues->size();i++)
	{
		switch(lstCapValues->at(i))
		{
		case TWSS_NONE:
			m_combo_standardsizes.InsertString(i,"自定义");
			break;
		case TWSS_USLETTER:
			m_combo_standardsizes.InsertString(i,"US Letter (8.5\" x 11\")");  //216mm x 280mm
			break;
		case TWSS_USLEGAL:
			m_combo_standardsizes.InsertString(i,"US Legal (8.5\" x 14\")");  //216mm x 356mm
			break;
		case TWSS_PHOT64:
			m_combo_standardsizes.InsertString(i,"照片64 (6\" x 4\")");  //152mm x 102mm
			break;
		case TWSS_PHOT53:
			m_combo_standardsizes.InsertString(i,"照片53 (5\" x 3\")");  //127mm x 76mm
			break;
		case TWSS_A3:
			m_combo_standardsizes.InsertString(i,"ISO A3 (297mm x 420mm)"); //国际标准
			break;
		case TWSS_A4:
			m_combo_standardsizes.InsertString(i,"ISO A4 (210mm x 297mm)"); //国际标准
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
			m_combo_standardsizes.InsertString(i,"JIS B4 (257mm x 364mm)");//JIS日本标准
			break;
		case TWSS_JISB5:
			m_combo_standardsizes.InsertString(i,"JIS B5 (182mm x 257mm)");//JIS日本标准
			break;
		case TWSS_JISB6:
			m_combo_standardsizes.InsertString(i,"JIS B6 (128mm x 182mm)");
			break;
		case TWSS_JISB7:
			m_combo_standardsizes.InsertString(i,"JIS B7 (91mm x 128mm)");
			break;
		case UDSCAP_LONGDOCUMENT:
			m_combo_standardsizes.InsertString(i,"长纸模式");
			break;
		case TWSS_MAXSIZE:
			m_combo_standardsizes.InsertString(i,"最大扫描范围");
			break;
		default:
			continue;
		}
	}
	m_combo_standardsizes.SetCurSel(nCapIndex);  // 显示默认值

	
	if(TWSS_NONE == lstCapValues->at(nCapIndex))  ///<　纸张大小：自定义。
	{
		m_edit_width.EnableWindow(TRUE);
		m_edit_height.EnableWindow(TRUE);
	//	GetDlgItem(IDC_ADVANCED_STATIC_ORIENTATION)->EnableWindow(FALSE);
	} 
	else
	{
		m_edit_width.EnableWindow(FALSE);
		m_edit_height.EnableWindow(FALSE);
	}

	// 自定义宽与高
	TW_FRAME frame;
	CString strTemp;

	frame = m_pUI->GetCurrentFrame();
	strTemp.Format("%0.2f",FIX32ToFloat(frame.Right));
	m_edit_width.SetWindowText(strTemp);

	frame = m_pUI->GetCurrentFrame();
	strTemp.Format("%0.2f",FIX32ToFloat(frame.Bottom));
	m_edit_height.SetWindowText(strTemp);

	//纸张设置-单位
	m_combo_uints.ResetContent();  // 清空内容
	nCapIndex = m_pUI->GetCurrentCapIndex(ICAP_UNITS);
	lstCapValues = m_pUI->GetValidCap(ICAP_UNITS);
	for(unsigned int i=0; i<lstCapValues->size();i++)
	{
		switch(lstCapValues->at(i))
		{
		case TWUN_INCHES:
			m_combo_uints.InsertString(i,"Inches"); //英寸
			break;
		case TWUN_PIXELS:
			m_combo_uints.InsertString(i,"Pixels"); //像素
			break;
		case TWUN_CENTIMETERS:
			m_combo_uints.InsertString(i,"Centimeters"); //厘米
			break;
		case TWUN_PICAS:
			m_combo_uints.InsertString(i,"Picas");
			break;
		case TWUN_POINTS:
			m_combo_uints.InsertString(i,"Points");
			break;
		case TWUN_TWIPS:
			m_combo_uints.InsertString(i,"Twips"); //缇
			break;
		default:
			continue;
		}
	}
	m_combo_uints.SetCurSel(nCapIndex);
}


BOOL CPage_Paper::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  在此添加额外的初始化
	UpdateControls();

	SetScroll();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CPage_Paper::SetScroll(void)
{
	// 设置垂直滚动条的滚动范围为1到100   
	m_scroll_width.SetScrollRange(1, 100);
	m_scroll_height.SetScrollRange(1, 100);
	// 设置垂直滚动条的初始位置为20   
	//m_scroll_width.SetScrollPos(20);   
	// 在对应编辑框中显示20   
	//SetDlgItemInt(IDC_PAPER_EDIT_WIDTH, 20);
}

void CPage_Paper::OnCbnSelchangePaper_Combo_Orientation()
{
	// TODO: 在此添加控件通知处理程序代码
	int nIndex = m_combo_orientation.GetCurSel();
	CString strCBText; 
	m_combo_orientation.GetLBText( nIndex, strCBText);
	//*int nval = _ttoi(strCBText);  ///< CString 转 int*/
	int nval;
	if (strCBText.Find("纵向") >= 0)
	{
		nval = TWOR_PORTRAIT;
	}
	else
	{
		nval = TWOR_LANDSCAPE;
	}
	m_pUI->SetCapValueInt(ICAP_ORIENTATION,nval); 
	UpdateControls();
}


void CPage_Paper::OnCbnSelchangePaper_Combo_Standardsizes()
{
	// TODO: 在此添加控件通知处理程序代码
	int nIndex = m_combo_standardsizes.GetCurSel();
	CString strCBText; 
	m_combo_standardsizes.GetLBText( nIndex, strCBText);
	int nval;
	//int nval = _ttoi(strCBText);  // CString 转 int
	if (strCBText.Find("自定义") >= 0)
	{
		nval = TWSS_NONE;
	}
	else if (strCBText.Find("US Letter") >= 0)
	{
		nval = TWSS_USLETTER;
	}
	else if (strCBText.Find("US Legal") >= 0)
	{
		nval = TWSS_USLEGAL;
	}
	else if (strCBText.Find("照片64") >= 0)
	{
		nval = TWSS_PHOT64;
	}
	else if (strCBText.Find("照片53") >= 0)
	{
		nval = TWSS_PHOT53;
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
	else if (strCBText.Find("长纸模式") >= 0)
	{
		nval = UDSCAP_LONGDOCUMENT;
	}
	else if (strCBText.Find("最大扫描") >= 0)
	{
		nval = TWSS_MAXSIZE;
	}
	else
	{
		return;
	}
	m_pUI->SetCapValueInt(ICAP_SUPPORTEDSIZES,nval); 
	UpdateControls();
}


void CPage_Paper::OnCbnSelchangePaper_Combo_Uints()
{
	// TODO: 在此添加控件通知处理程序代码
	int nIndex = m_combo_uints.GetCurSel();
	CString strCBText; 
	m_combo_uints.GetLBText( nIndex, strCBText);
	int nval;
	//int nval = _ttoi(strCBText);  ///< CString 转 int
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
	m_pUI->SetCapValueInt(ICAP_UNITS,nval); 
	UpdateControls();
}


void CPage_Paper::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	// 遗留问题，响应此消息，只能使一个滚动条改变
	//宽
	int widthpos = m_scroll_width.GetScrollPos(); //获取垂直滚动条当前位置
	switch(nSBCode)
	{
	case SB_LINEDOWN: //如果向下滚动一列，则pos加1
		widthpos += 1; 
		break;
	case SB_LINEUP: //如果向上滚动一列，则pos减1
		widthpos -= 1;
		break;
	case SB_THUMBPOSITION: //如果拖动滚动块滚动到指定位置，则pos赋值为nPos的值
		widthpos = nPos;
		break;
	case SB_TOP: //最顶端
		widthpos = 1;
		break;
	case SB_BOTTOM:
		widthpos = 100;
		break;
	default:   
		SetDlgItemInt(IDC_PAPER_EDIT_WIDTH, widthpos);  
		return;   
	}
	// 设置滚动块位置  
	m_scroll_width.SetScrollPos(widthpos);

	//高
	//int heightpos = m_scroll_height.GetScrollPos(); //获取垂直滚动条当前位置
	//switch(nSBCode)
	//{
	//case SB_LINEDOWN: //如果向下滚动一列，则pos加1
	//	heightpos += 1; 
	//	break;
	//case SB_LINEUP: //如果向上滚动一列，则pos减1
	//	heightpos -= 1;
	//	break;
	//case SB_THUMBPOSITION: //如果拖动滚动块滚动到指定位置，则pos赋值为nPos的值
	//	heightpos = nPos;
	//	break;
	//case SB_TOP: //最顶端
	//	heightpos = 1;
	//	break;
	//case SB_BOTTOM:
	//	heightpos = 100;
	//	break;
	//default:   
	//	SetDlgItemInt(IDC_PAPER_EDIT_HEIGHT, heightpos);  
	//	return;   
	//}
	//// 设置滚动块位置  
	//m_scroll_height.SetScrollPos(heightpos);

	CPropertyPage::OnVScroll(nSBCode, nPos, pScrollBar);
}

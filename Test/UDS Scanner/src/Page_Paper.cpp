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
	DDX_Control(pDX, IDC_PAPER_COMBO_COMPRESS, m_combo_compress);
	DDX_Control(pDX, IDC_PAPER_SLIDER_COMPRESSION, m_slider_compressvalue);
	DDX_Control(pDX, IDC_PAPER_EDIT_COMPRESSVALUE, m_edit_compressvalue);
}


BEGIN_MESSAGE_MAP(CPage_Paper, CPropertyPage)
	ON_CBN_SELCHANGE(IDC_PAPER_COMBO_STANDARDSIZES, &CPage_Paper::OnCbnSelchangePaper_Combo_Standardsizes)
	ON_CBN_SELCHANGE(IDC_PAPER_COMBO_UINTS, &CPage_Paper::OnCbnSelchangePaper_Combo_Uints)
	ON_WM_VSCROLL()
	ON_WM_PAINT()
	ON_EN_CHANGE(IDC_PAPER_EDIT_COMPRESSVALUE, &CPage_Paper::OnEnChangeBase_Edit_Compressvalue)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_PAPER_SLIDER_COMPRESSION, &CPage_Paper::OnNMCustomdrawPaper_Slider_Compressionvalue)
	ON_CBN_SELCHANGE(IDC_PAPER_COMBO_COMPRESS, &CPage_Paper::OnCbnSelchangePaper_Combo_Compress)
END_MESSAGE_MAP()


// CPage_Paper 消息处理程序
void CPage_Paper::SetCapValue(void)
{
	MAP_CAP::iterator iter; 
	for(iter = m_papermap.begin(); iter != m_papermap.end(); iter++)
	{
		switch(iter->first)
		{
		case ICAP_SUPPORTEDSIZES:  //纸张大小
		case ICAP_UNITS:  //单位
		case ICAP_COMPRESSION: //压缩格式
			{
				m_pUI->SetCapValueInt(iter->first,(int)iter->second); 
				break;
			}	

		case UDSCAP_EDGE_UP: // 边缘扩充
		case UDSCAP_EDGE_DOWN:
		case UDSCAP_EDGE_LEFT:
		case UDSCAP_EDGE_RIGHT:
		case UDSCAP_XPOS: //X偏移量
		case UDSCAP_YPOS:
			{
				m_pUI->SetCapValueFloat(iter->first, iter->second); 
				break;
			}

		case UDSCAP_COMPRESSVALUE: //压缩比
			{
				if(m_slider_compressvalue.IsWindowEnabled())
				{
					m_pUI->SetCapValueFloat(iter->first,iter->second);
				}	
				break;
			}
		}
	}
}


void CPage_Paper::UpdateControls(void)
{
	int nCapIndex;
	const IntVector* lstCapValues;
	float fCapValue;
	int nCapValue;
	CString strText;

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
	int nval = FindPaperSize(nCapIndex);
	m_papermap[ICAP_SUPPORTEDSIZES] = (float)nval;//不能只更新容器，还要更新CAP

	//SetStandardsizes();
	if(TWSS_NONE == lstCapValues->at(nCapIndex))  ///<　纸张大小：自定义。
	{
		m_edit_width.EnableWindow(TRUE);
		m_edit_height.EnableWindow(TRUE);
		m_scroll_width.EnableWindow(TRUE);
		m_scroll_height.EnableWindow(TRUE);
	} 
	else
	{
		m_edit_width.EnableWindow(FALSE);
		m_edit_height.EnableWindow(FALSE);
		m_scroll_width.EnableWindow(FALSE);
		m_scroll_height.EnableWindow(FALSE);
	}

	//纸张设置-单位
	m_combo_uints.ResetContent();  // 清空内容
	nCapIndex = m_pUI->GetCurrentCapIndex(ICAP_UNITS);
	lstCapValues = m_pUI->GetValidCap(ICAP_UNITS);
	for(unsigned int i=0; i<lstCapValues->size();i++)
	{
		switch(lstCapValues->at(i))
		{
		case TWUN_INCHES:
			m_combo_uints.InsertString(i,"英寸Inches"); //英寸
			break;
		case TWUN_PIXELS:
			m_combo_uints.InsertString(i,"像素Pixels"); //像素
			break;
		case TWUN_CENTIMETERS:
			m_combo_uints.InsertString(i,"厘米Centimeters"); //厘米
			break;
		default:
			continue;
		}
	}
	m_combo_uints.SetCurSel(nCapIndex);

	// 自定义宽与高
	TW_FRAME frame;
	CString strTemp;

	int nIndex = m_combo_uints.GetCurSel();
	//单位为inches、centi时edit显示两位小数，其他直接显示整数
	switch(FindUnit(nIndex))
	{
	case TWUN_INCHES:
	case TWUN_CENTIMETERS:
		{
			frame = m_pUI->GetCurrentFrame();
			strTemp.Format("%0.2f",FIX32ToFloat(frame.Right));
			m_edit_width.SetWindowText(strTemp);

			frame = m_pUI->GetCurrentFrame();
			strTemp.Format("%0.2f",FIX32ToFloat(frame.Bottom));
			m_edit_height.SetWindowText(strTemp);
			break;
		}
	case TWUN_PIXELS:
		{
			frame = m_pUI->GetCurrentFrame();
			strTemp.Format("%d",(int)FIX32ToFloat(frame.Right));
			m_edit_width.SetWindowText(strTemp);

			frame = m_pUI->GetCurrentFrame();
			strTemp.Format("%d",(int)FIX32ToFloat(frame.Bottom));
			m_edit_height.SetWindowText(strTemp);
			break;
		}
	}

	//边缘扩充 上下左右
	fCapValue = m_pUI->GetCapValueFloat(UDSCAP_EDGE_UP);
	strText.Format("%0.2f",fCapValue);
	SetDlgItemText(IDC_PAPER_EDIT_UP,strText);

	fCapValue = m_pUI->GetCapValueFloat(UDSCAP_EDGE_DOWN);
	strText.Format("%0.2f",fCapValue);
	SetDlgItemText(IDC_PAPER_EDIT_DOWN,strText);

	fCapValue = m_pUI->GetCapValueFloat(UDSCAP_EDGE_LEFT);
	strText.Format("%0.2f",fCapValue);
	SetDlgItemText(IDC_PAPER_EDIT_LEFT,strText);

	fCapValue = m_pUI->GetCapValueFloat(UDSCAP_EDGE_RIGHT);
	strText.Format("%0.2f",fCapValue);
	SetDlgItemText(IDC_PAPER_EDIT_RIGHT,strText);

	//X偏移量
	fCapValue = m_pUI->GetCapValueFloat(UDSCAP_XPOS);
	strText.Format("%0.2f",fCapValue);
	SetDlgItemText(IDC_PAPER_EDIT_XPOS,strText);

	fCapValue = m_pUI->GetCapValueFloat(UDSCAP_YPOS);
	strText.Format("%0.2f",fCapValue);
	SetDlgItemText(IDC_PAPER_EDIT_YPOS,strText);

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
	SetDlgItemText(IDC_PAPER_EDIT_COMPRESSVALUE,strText);
}


BOOL CPage_Paper::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  在此添加额外的初始化
	UpdateControls();
	SetScroll();
	InitSliderCtrl();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CPage_Paper::InitSliderCtrl()
{
	float fMin,fMax,fStep;
	m_pUI->GetCapRangeFloat(UDSCAP_COMPRESSVALUE, fMin, fMax, fStep);
	m_slider_compressvalue.SetRange((int)fMin, (int)fMax);
	m_slider_compressvalue.SetTicFreq((int)fStep);
}


void CPage_Paper::SetScroll(void)
{
	// 设置垂直滚动条的滚动范围
	//宽高
	int nIndex = m_combo_uints.GetCurSel();
	CString str;
	int nval;
	switch(FindUnit(nIndex))
	{
	case TWUN_INCHES:
	case TWUN_CENTIMETERS:
		{
			m_scroll_width.SetScrollRange(0, 5000); 
			m_scroll_height.SetScrollRange(0, 5000);
			m_scroll_up.SetScrollRange(0, 5000);
			m_scroll_down.SetScrollRange(0, 5000);
			m_scroll_left.SetScrollRange(0, 5000);
			m_scroll_right.SetScrollRange(0, 5000);
			m_scroll_xpos.SetScrollRange(0, 5000);
			m_scroll_ypos.SetScrollRange(0, 5000);

			//宽
			m_edit_width.GetWindowText(str);
			nval = _ttof(str) * 100;
			m_scroll_width.SetScrollPos(nval); //滑动条pos是编辑框值的100倍
			//高
			m_edit_height.GetWindowText(str);
			nval = _ttof(str) * 100;
			m_scroll_height.SetScrollPos(nval); 
			//边缘扩充 上下左右
			m_edit_up.GetWindowText(str);
			nval = _ttof(str) * 100;
			m_scroll_up.SetScrollPos(nval); 		
			m_edit_down.GetWindowText(str);
			nval = _ttof(str) * 100;
			m_scroll_down.SetScrollPos(nval); 
			m_edit_left.GetWindowText(str);
			nval = _ttof(str) * 100;
			m_scroll_left.SetScrollPos(nval); 
			m_edit_right.GetWindowText(str);
			nval = _ttof(str) * 100;
			m_scroll_right.SetScrollPos(nval); 
			//X、Y偏移量
			m_edit_xpos.GetWindowText(str);
			nval = _ttof(str) * 100;
			m_scroll_xpos.SetScrollPos(nval); 
			m_edit_ypos.GetWindowText(str);
			nval = _ttof(str) * 100;
			m_scroll_ypos.SetScrollPos(nval); 

			break;
		}
		
	case TWUN_PIXELS:
		{
			m_scroll_width.SetScrollRange(0, 25000); 
			m_scroll_height.SetScrollRange(0, 25000);
			m_scroll_up.SetScrollRange(0, 25000);
			m_scroll_down.SetScrollRange(0, 25000);
			m_scroll_left.SetScrollRange(0, 25000);
			m_scroll_right.SetScrollRange(0, 25000);
			m_scroll_xpos.SetScrollRange(0, 25000);
			m_scroll_ypos.SetScrollRange(0, 25000);

			//宽
			m_edit_width.GetWindowText(str);
			nval = _ttof(str);
			m_scroll_width.SetScrollPos(nval); //滑动条pos就是编辑框的值
			//高
			m_edit_height.GetWindowText(str);
			nval = _ttof(str);
			m_scroll_height.SetScrollPos(nval); 
			//边缘扩充 上下左右
			m_edit_up.GetWindowText(str);
			nval = _ttof(str);
			m_scroll_up.SetScrollPos(nval); 		
			m_edit_down.GetWindowText(str);
			nval = _ttof(str);
			m_scroll_down.SetScrollPos(nval); 
			m_edit_left.GetWindowText(str);
			nval = _ttof(str);
			m_scroll_left.SetScrollPos(nval); 
			m_edit_right.GetWindowText(str);
			nval = _ttof(str);
			m_scroll_right.SetScrollPos(nval); 
			//X、Y偏移量
			m_edit_xpos.GetWindowText(str);
			nval = _ttof(str);
			m_scroll_xpos.SetScrollPos(nval); 
			m_edit_ypos.GetWindowText(str);
			nval = _ttof(str);
			m_scroll_ypos.SetScrollPos(nval); 

			break;
		}		
	}//switch

	
}


void CPage_Paper::OnCbnSelchangePaper_Combo_Standardsizes()
{
	// TODO: 在此添加控件通知处理程序代码
	int nIndex = m_combo_standardsizes.GetCurSel();
	int nval = FindPaperSize(nIndex);
	m_papermap[ICAP_SUPPORTEDSIZES] = (float)nval;
	m_combo_standardsizes.SetCurSel(nIndex);
}


void CPage_Paper::OnCbnSelchangePaper_Combo_Uints()
{
	// TODO: 在此添加控件通知处理程序代码
	int nIndex = m_combo_uints.GetCurSel();
	int nval = FindUnit(nIndex);
	
	m_papermap[ICAP_UNITS] = (float)nval;
	m_combo_uints.SetCurSel(nIndex);
	//m_pUI->SetCapValueInt(ICAP_UNITS,nval); //能够直接响应
	//UpdateControls();
}


int CPage_Paper::FindUnit(int index)
{
	CString strCBText; 
	m_combo_uints.GetLBText(index, strCBText);
	int nval;
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
	else
	{
		nval = TWUN_INCHES; //默认A4
	}
	return nval;
}


void CPage_Paper::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	int scrollpos;
	CString str;
	float editvalue;
	int nIndex = m_combo_uints.GetCurSel();

	SetScroll();
	switch(pScrollBar->GetDlgCtrlID())
	{
		//宽
	case IDC_PAPER_SCROLLBAR_WIDTH:
		scrollpos = m_scroll_width.GetScrollPos(); //获取垂直滚动条当前位置
		switch(nSBCode)
		{
		case SB_LINEUP: //如果向上滚动一列，则pos加1
			scrollpos += 1;
			break;
		case SB_LINEDOWN: //如果向下滚动一列，则pos减1
			scrollpos -= 1; 
			break;	
		}
		// 设置滚动块位置  
		m_scroll_width.SetScrollPos(scrollpos);

		switch(FindUnit(nIndex))
		{
		case TWUN_INCHES:
		case TWUN_CENTIMETERS:
			{
				editvalue = (float)scrollpos/100;  //需要除以100.
				str.Format("%0.2f", editvalue);
				break;
			}
		case TWUN_PIXELS:
			{
				str.Format("%d", scrollpos);
				break;
			}	
		}
		SetDlgItemText(IDC_PAPER_EDIT_WIDTH, str); //设置edit的值
		break;
	
		//高
	case IDC_PAPER_SCROLLBAR_HEIGHT:
		//高
		scrollpos = m_scroll_height.GetScrollPos(); 
		switch(nSBCode)
		{
		case SB_LINEUP: 
			scrollpos += 1;
			break;
		case SB_LINEDOWN: 
			scrollpos -= 1; 
			break;
		}
		// 设置滚动块位置  
		m_scroll_height.SetScrollPos(scrollpos);

		switch(FindUnit(nIndex))
		{
		case TWUN_INCHES:
		case TWUN_CENTIMETERS:
			{
				editvalue = (float)scrollpos/100;  
				str.Format("%0.2f", editvalue);
				break;
			}
		case TWUN_PIXELS:
			{
				str.Format("%d", scrollpos);
				break;
			}	
		}
		SetDlgItemText(IDC_PAPER_EDIT_HEIGHT, str); 
		break;

	//边缘扩充 上下左右
	case IDC_PAPER_SCROLLBAR_UP:
		scrollpos = m_scroll_up.GetScrollPos(); 
		switch(nSBCode)
		{
		case SB_LINEUP: 
			scrollpos += 1;
			break;
		case SB_LINEDOWN: 
			scrollpos -= 1; 
			break;	 
		}
		m_scroll_up.SetScrollPos(scrollpos);
		switch(FindUnit(nIndex))
		{
		case TWUN_INCHES:
		case TWUN_CENTIMETERS:
			{
				editvalue = (float)scrollpos/100;  //需要除以100.
				str.Format("%0.2f", editvalue);
				m_papermap[UDSCAP_EDGE_UP] = editvalue;
				break;
			}
		case TWUN_PIXELS:
			{
				str.Format("%d", scrollpos);
				m_papermap[UDSCAP_EDGE_UP] = (float)scrollpos;
				break;
			}	
		}		
		SetDlgItemText(IDC_PAPER_EDIT_UP, str); //设置edit的值
		break;
	case IDC_PAPER_SCROLLBAR_DOWN:
		scrollpos = m_scroll_down.GetScrollPos(); 
		switch(nSBCode)
		{
		case SB_LINEUP: 
			scrollpos += 1;
			break;
		case SB_LINEDOWN: 
			scrollpos -= 1; 
			break; 
		}
		m_scroll_down.SetScrollPos(scrollpos);
		switch(FindUnit(nIndex))
		{
		case TWUN_INCHES:
		case TWUN_CENTIMETERS:
			{
				editvalue = (float)scrollpos/100;  //需要除以100.
				str.Format("%0.2f", editvalue);
				m_papermap[UDSCAP_EDGE_DOWN] = editvalue;
				break;
			}
		case TWUN_PIXELS:
			{
				str.Format("%d", scrollpos);
				m_papermap[UDSCAP_EDGE_DOWN] = (float)scrollpos;
				break;
			}	
		}	
		SetDlgItemText(IDC_PAPER_EDIT_DOWN, str); //设置edit的值
		break;

	case IDC_PAPER_SCROLLBAR_LEFT:
		scrollpos = m_scroll_left.GetScrollPos(); 
		switch(nSBCode)
		{
		case SB_LINEUP: 
			scrollpos += 1;
			break;
		case SB_LINEDOWN: 
			scrollpos -= 1; 
			break;
		}
		m_scroll_left.SetScrollPos(scrollpos);
		switch(FindUnit(nIndex))
		{
		case TWUN_INCHES:
		case TWUN_CENTIMETERS:
			{
				editvalue = (float)scrollpos/100;  //需要除以100.
				str.Format("%0.2f", editvalue);
				m_papermap[UDSCAP_EDGE_LEFT] = editvalue;
				break;
			}
		case TWUN_PIXELS:
			{
				str.Format("%d", scrollpos);
				m_papermap[UDSCAP_EDGE_LEFT] = (float)scrollpos;
				break;
			}	
		}
		SetDlgItemText(IDC_PAPER_EDIT_LEFT, str); //设置edit的值
		break;
	case IDC_PAPER_SCROLLBAR_RIGHT:
		scrollpos = m_scroll_right.GetScrollPos(); 
		switch(nSBCode)
		{
		case SB_LINEUP: 
			scrollpos += 1;
			break;
		case SB_LINEDOWN: 
			scrollpos -= 1; 
			break;
		}
		m_scroll_right.SetScrollPos(scrollpos);
		switch(FindUnit(nIndex))
		{
		case TWUN_INCHES:
		case TWUN_CENTIMETERS:
			{
				editvalue = (float)scrollpos/100;  //需要除以100.
				str.Format("%0.2f", editvalue);
				m_papermap[UDSCAP_EDGE_RIGHT] = editvalue;
				break;
			}
		case TWUN_PIXELS:
			{
				str.Format("%d", scrollpos);
				m_papermap[UDSCAP_EDGE_RIGHT] = (float)scrollpos;
				break;
			}	
		}	
		SetDlgItemText(IDC_PAPER_EDIT_RIGHT, str); //设置edit的值
		break;

	//X偏移量
	case IDC_PAPER_SCROLLBAR_XPOS:
		scrollpos = m_scroll_xpos.GetScrollPos(); 
		switch(nSBCode)
		{
		case SB_LINEUP: 
			scrollpos += 1;
			break;
		case SB_LINEDOWN: 
			scrollpos -= 1; 
			break;
		}
		m_scroll_xpos.SetScrollPos(scrollpos);
		switch(FindUnit(nIndex))
		{
		case TWUN_INCHES:
		case TWUN_CENTIMETERS:
			{
				editvalue = (float)scrollpos/100;  //需要除以100.
				str.Format("%0.2f", editvalue);
				m_papermap[UDSCAP_XPOS] = editvalue;
				break;
			}
		case TWUN_PIXELS:
			{
				str.Format("%d", scrollpos);
				m_papermap[UDSCAP_XPOS] = (float)scrollpos;
				break;
			}	
		}		
		SetDlgItemText(IDC_PAPER_EDIT_XPOS, str); //设置edit的值
		break;
	case IDC_PAPER_SCROLLBAR_YPOS:
		scrollpos = m_scroll_ypos.GetScrollPos(); 
		switch(nSBCode)
		{
		case SB_LINEUP: 
			scrollpos += 1;
			break;
		case SB_LINEDOWN: 
			scrollpos -= 1; 
			break;
		}
		m_scroll_ypos.SetScrollPos(scrollpos);
		switch(FindUnit(nIndex))
		{
		case TWUN_INCHES:
		case TWUN_CENTIMETERS:
			{
				editvalue = (float)scrollpos/100;  //需要除以100.
				str.Format("%0.2f", editvalue);
				m_papermap[UDSCAP_YPOS] = editvalue;
				break;
			}
		case TWUN_PIXELS:
			{
				str.Format("%d", scrollpos);
				m_papermap[UDSCAP_YPOS] = (float)scrollpos;
				break;
			}	
		}		
		SetDlgItemText(IDC_PAPER_EDIT_YPOS, str); //设置edit的值
		break;

	default:
		break;
	}

	CPropertyPage::OnVScroll(nSBCode, nPos, pScrollBar);
}


int CPage_Paper::FindPaperSize(int index)
{
	CString strCBText; 
	m_combo_standardsizes.GetLBText(index, strCBText);
	int nval;
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
		nval = TWSS_A4; //默认A4
	}
	return nval;
}


BOOL CPage_Paper::OnSetActive()
{
	// TODO: 在此添加专用代码和/或调用基类
	m_pUI->PreViewStatus();
	return CPropertyPage::OnSetActive();
}


void CPage_Paper::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 __super::OnPaint()
	//获得控件位置
	CRect rect;
	GetDlgItem(IDC_PAPER_PREPICTURE)->GetWindowRect(rect);
	ScreenToClient(rect);

	//指定标尺宽度
	int width = rect.Width(); //14英寸
	//指定起点横坐标偏移
	CPoint beginpoint;
	beginpoint.x = rect.left;
	beginpoint.y = rect.top-2; //画在图片上方

	//画刻度线
	int degree;
	dc.SetTextAlign(TA_CENTER | TA_BOTTOM);//将刻度线数字标注在刻度线的上方
	dc.SetBkMode(TRANSPARENT);//消除白色背景
	//画大刻度
	for(degree = 0; degree <= 900; degree += 100)
	{
		dc.MoveTo(beginpoint.x + degree*width/900, beginpoint.y-width/9/3);
		dc.LineTo(beginpoint.x + degree*width/900, beginpoint.y);
		CString str;
		str.Format(_T("%d"), (degree/100));
		dc.TextOut(beginpoint.x + degree*width/900, beginpoint.y-width/9/3, str);
	}
	//画小刻度
	for(degree = 20; degree <= 900; degree += 20)  
	{  
		dc.MoveTo(beginpoint.x + degree*width/900, beginpoint.y - width/9/5);  
		dc.LineTo(beginpoint.x + degree*width/900, beginpoint.y);  
	}  

	//画左侧标尺
	//指定标尺长度
	int height = rect.Height(); //14英寸
	//指定起点横坐标偏移
	beginpoint.x = rect.left-2;//画在图片左侧
	beginpoint.y = rect.top; 

	//画刻度线
	dc.SetTextAlign(TA_CENTER | TA_LEFT);//将刻度线数字标注在刻度线的左侧
	dc.SetBkMode(TRANSPARENT);//消除白色背景
	//画大刻度
	for(degree = 0; degree <= 1400; degree += 100)
	{
		dc.MoveTo(beginpoint.x - height/14/3, beginpoint.y + degree*height/1400);
		dc.LineTo(beginpoint.x, beginpoint.y + degree*height/1400);
		CString str;
		str.Format(_T("%d"), (degree/100));
		dc.TextOut(beginpoint.x - height/14/3-10, beginpoint.y + degree*height/1400-7, str); //使数字与线居中
	}
	//画小刻度
	for(degree = 20; degree <= 1400; degree += 20)  
	{  
		dc.MoveTo(beginpoint.x - height/14/5, beginpoint.y + degree*height/1400);  
		dc.LineTo(beginpoint.x, beginpoint.y + degree*height/1400);  
	}  
}


void CPage_Paper::OnEnChangeBase_Edit_Compressvalue()
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
	m_papermap[UDSCAP_COMPRESSVALUE] = (float)nval;

	m_edit_compressvalue.SetSel(str.GetLength(), str.GetLength(),TRUE);  // 设置编辑框控件范围

	UpdateData(FALSE);  // 更新控件
}


void CPage_Paper::OnNMCustomdrawPaper_Slider_Compressionvalue(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	//TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);  // 接收数据
	CString str;
	int sldValue = m_slider_compressvalue.GetPos();  // 获取滑块当前位置
	m_papermap[UDSCAP_COMPRESSVALUE] = float(sldValue);
	
	str.Format("%d", sldValue);
	SetDlgItemText(IDC_PAPER_EDIT_COMPRESSVALUE, str);

	*pResult = 0;
}


void CPage_Paper::OnCbnSelchangePaper_Combo_Compress()
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
	m_papermap[ICAP_COMPRESSION] = nval;

	m_combo_compress.SetCurSel(nIndex);
}

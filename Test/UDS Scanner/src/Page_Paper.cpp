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
	ON_WM_LBUTTONDOWN()
//	ON_EN_CHANGE(IDC_PAPER_EDIT_UP, &CPage_Paper::OnEnChangeBase_Edit_Edgeup)
	ON_EN_CHANGE(IDC_PAPER_EDIT_RIGHT, &CPage_Paper::OnEnChangeBase_Edit_EdgeRight)
	ON_EN_CHANGE(IDC_PAPER_EDIT_UP, &CPage_Paper::OnEnChangeBase_Edit_EdgeUp)
	ON_EN_CHANGE(IDC_PAPER_EDIT_LEFT, &CPage_Paper::OnEnChangeBase_Edit_EdgeLeft)
	ON_EN_CHANGE(IDC_PAPER_EDIT_DOWN, &CPage_Paper::OnEnChangeBase_Edit_EdgeDown)
	ON_EN_CHANGE(IDC_PAPER_EDIT_XPOS, &CPage_Paper::OnEnChangeBase_Edit_EdgeXpos)
	ON_EN_CHANGE(IDC_PAPER_EDIT_YPOS, &CPage_Paper::OnEnChangeBase_Edit_EdgeYpos)
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
	int nval;

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
	nval = FindPaperSize(nCapIndex);
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
	nval = FindUnit(nCapIndex);
	m_papermap[ICAP_UNITS] = (float)nval;

	// 自定义宽与高
	TW_FRAME frame;	
	int nIndex = m_combo_uints.GetCurSel();
	int org = FindUnit(nIndex); //当前界面的单位
	/*
	int old = FindUnit(m_unitindex); //界面最开始的单位
	nCapIndex = m_pUI->GetCurrentCapIndex(ICAP_XRESOLUTION);
	const FloatVector* lstCapValuesFlt = m_pUI->GetValidCapFloat(ICAP_XRESOLUTION);
	int reso = (int)lstCapValuesFlt->at(nCapIndex); //x分辨率
	SetScroll(); */
	//单位为inches、centi时edit显示两位小数，其他直接显示整数
	switch(org)
	{
	case TWUN_INCHES:
	case TWUN_CENTIMETERS:
		{
			frame = m_pUI->GetCurrentFrame();
			strText.Format("%0.2f",FIX32ToFloat(frame.Right));
			m_edit_width.SetWindowText(strText);

			frame = m_pUI->GetCurrentFrame();
			strText.Format("%0.2f",FIX32ToFloat(frame.Bottom));
			m_edit_height.SetWindowText(strText);
			/*
			//边缘扩充 上下左右
			int nvalue;
			fCapValue = m_pUI->GetCapValueFloat(UDSCAP_EDGE_UP);
			nvalue = ConvertUnits(fCapValue, old, org, reso);
			strText.Format("%0.2f",(float)nvalue);
			//AfxMessageBox(strText);
			SetDlgItemText(IDC_PAPER_EDIT_UP,strText);

			fCapValue = m_pUI->GetCapValueFloat(UDSCAP_EDGE_DOWN);
			nvalue = ConvertUnits(fCapValue, old, org, reso);
			strText.Format("%0.2f",(float)nvalue);
			SetDlgItemText(IDC_PAPER_EDIT_DOWN,strText);

			fCapValue = m_pUI->GetCapValueFloat(UDSCAP_EDGE_LEFT);
			nvalue = ConvertUnits(fCapValue, old, org, reso);
			strText.Format("%0.2f",(float)nvalue);
			SetDlgItemText(IDC_PAPER_EDIT_LEFT,strText);

			fCapValue = m_pUI->GetCapValueFloat(UDSCAP_EDGE_RIGHT);
			nvalue = ConvertUnits(fCapValue, old, org, reso);
			strText.Format("%0.2f",(float)nvalue);
			SetDlgItemText(IDC_PAPER_EDIT_RIGHT,strText);
			
			//XY偏移量
			fCapValue = m_pUI->GetCapValueFloat(UDSCAP_XPOS);
			nvalue = ConvertUnits(fCapValue, old, org, reso);
			strText.Format("%0.2f",(float)nvalue);
			SetDlgItemText(IDC_PAPER_EDIT_XPOS,strText);

			fCapValue = m_pUI->GetCapValueFloat(UDSCAP_YPOS);
			nvalue = ConvertUnits(fCapValue, old, org, reso);
			strText.Format("%0.2f",(float)nvalue);
			SetDlgItemText(IDC_PAPER_EDIT_YPOS,strText);
			*/
			break;
		}
	case TWUN_PIXELS:
		{
			frame = m_pUI->GetCurrentFrame();
			strText.Format("%d",(int)FIX32ToFloat(frame.Right));
			m_edit_width.SetWindowText(strText);

			frame = m_pUI->GetCurrentFrame();
			strText.Format("%d",(int)FIX32ToFloat(frame.Bottom));
			m_edit_height.SetWindowText(strText);

			/*
			//边缘扩充 上下左右
			int nvalue;
			fCapValue = m_pUI->GetCapValueFloat(UDSCAP_EDGE_UP);
			nvalue = ConvertUnits(fCapValue, old, org, reso);
			strText.Format("%d",nvalue);
			SetDlgItemText(IDC_PAPER_EDIT_UP,strText);

			fCapValue = m_pUI->GetCapValueFloat(UDSCAP_EDGE_DOWN);
			nvalue = ConvertUnits(fCapValue, old, org, reso);
			strText.Format("%d",nvalue);
			SetDlgItemText(IDC_PAPER_EDIT_DOWN,strText);

			fCapValue = m_pUI->GetCapValueFloat(UDSCAP_EDGE_LEFT);
			nvalue = ConvertUnits(fCapValue, old, org, reso);
			strText.Format("%d",nvalue);
			SetDlgItemText(IDC_PAPER_EDIT_LEFT,strText);

			fCapValue = m_pUI->GetCapValueFloat(UDSCAP_EDGE_RIGHT);
			nvalue = ConvertUnits(fCapValue, old, org, reso);
			strText.Format("%d",nvalue);
			SetDlgItemText(IDC_PAPER_EDIT_RIGHT,strText);

			fCapValue = m_pUI->GetCapValueFloat(UDSCAP_XPOS);
			nvalue = ConvertUnits(fCapValue, old, org, reso);
			strText.Format("%d",nvalue);
			SetDlgItemText(IDC_PAPER_EDIT_XPOS,strText);

			fCapValue = m_pUI->GetCapValueFloat(UDSCAP_YPOS);
			nvalue = ConvertUnits(fCapValue, old, org, reso);
			strText.Format("%d",nvalue);
			SetDlgItemText(IDC_PAPER_EDIT_YPOS,strText);
			*/
			break;
		}
	}
		
	//边缘扩充 上下左右
	fCapValue = m_pUI->GetCapValueFloat(UDSCAP_EDGE_UP);
	strText.Format("%0.2f",fCapValue);
	SetDlgItemText(IDC_PAPER_EDIT_UP,strText);
	m_papermap[IDC_PAPER_EDIT_UP] = fCapValue;

	fCapValue = m_pUI->GetCapValueFloat(UDSCAP_EDGE_DOWN);
	strText.Format("%0.2f",fCapValue);
	SetDlgItemText(IDC_PAPER_EDIT_DOWN,strText);
	m_papermap[UDSCAP_EDGE_DOWN] = fCapValue;

	fCapValue = m_pUI->GetCapValueFloat(UDSCAP_EDGE_LEFT);
	strText.Format("%0.2f",fCapValue);
	SetDlgItemText(IDC_PAPER_EDIT_LEFT,strText);
	m_papermap[UDSCAP_EDGE_LEFT] = fCapValue;

	fCapValue = m_pUI->GetCapValueFloat(UDSCAP_EDGE_RIGHT);
	strText.Format("%0.2f",fCapValue);
	SetDlgItemText(IDC_PAPER_EDIT_RIGHT,strText);
	m_papermap[UDSCAP_EDGE_RIGHT] = fCapValue;

	//X偏移量
	fCapValue = m_pUI->GetCapValueFloat(UDSCAP_XPOS);
	strText.Format("%0.2f",fCapValue);
	SetDlgItemText(IDC_PAPER_EDIT_XPOS,strText);
	m_papermap[UDSCAP_XPOS] = fCapValue;

	fCapValue = m_pUI->GetCapValueFloat(UDSCAP_YPOS);
	strText.Format("%0.2f",fCapValue);
	SetDlgItemText(IDC_PAPER_EDIT_YPOS,strText);
	m_papermap[UDSCAP_YPOS] = fCapValue;

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
	//m_papermap[ICAP_COMPRESSION] = fCapValue;

	// 压缩比 
	nCapValue = (int)(m_pUI->GetCapValueFloat(UDSCAP_COMPRESSVALUE)); 
	m_slider_compressvalue.SetPos(nCapValue);
	strText.Format("%d",nCapValue);
	SetDlgItemText(IDC_PAPER_EDIT_COMPRESSVALUE,strText);
	//m_papermap[UDSCAP_COMPRESSVALUE] = fCapValue;
}


BOOL CPage_Paper::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  在此添加额外的初始化
	UpdateControls();
	SetScroll();
	InitSliderCtrl();

	//橡皮筋类初始化
	m_rectTracker.m_nStyle = CRectTracker::solidLine;//设置RectTracker样式,实线CRectTracker::resizeOutside|
	m_rectTracker.m_nHandleSize = 5; //控制柄的像素大小

	m_standarindex = m_combo_standardsizes.GetCurSel();
	int nval = FindPaperSize(m_standarindex);
	m_unitindex = m_combo_uints.GetCurSel();
	int unitnval = FindUnit(m_unitindex);	 
	UpdatePicRectangle(nval, unitnval); //初始画矩形

	GetDlgItem(IDC_PAPER_COMBO_COMPRESS)->ShowWindow(FALSE); //暂时隐藏
	GetDlgItem(IDC_PAPER_SLIDER_COMPRESSION)->ShowWindow(FALSE); //暂时隐藏
	GetDlgItem(IDC_PAPER_EDIT_COMPRESSVALUE)->ShowWindow(FALSE); //暂时隐藏
	
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

	m_pUI->SetCapValueInt(ICAP_SUPPORTEDSIZES, nval); //能够直接响应
	UpdateControls(); //更新宽、高

	int unitIndex = m_combo_uints.GetCurSel();
	int unitnval = FindUnit(unitIndex);	//寻找最开始的单位，设置回去
	
	UpdatePicRectangle(nval, unitnval);

	nval = FindPaperSize(m_standarindex); //寻找最开始的纸张大小，设置回去
	m_pUI->SetCapValueInt(ICAP_SUPPORTEDSIZES, nval);
	unitnval = FindUnit(m_unitindex); //寻找最开始的单位，设置回去
	m_pUI->SetCapValueInt(ICAP_UNITS, unitnval);
}


void CPage_Paper::UpdatePicRectangle(int index, int unitindex)
{
	InvalidateRect(NULL);
	UpdateWindow();

	//设置当前Combo对应单位、纸张大小生效
	m_pUI->SetCapValueInt(ICAP_UNITS, unitindex);
	m_pUI->SetCapValueInt(ICAP_SUPPORTEDSIZES, index);	

	//纸张大小尺寸：单位不同，值不同
  TW_FRAME frame;
	frame = m_pUI->GetCurrentFrame();
	float right = FIX32ToFloat(frame.Right); //A4: 8.27英寸; 1653.40像素; 21.00厘米
	float bottom = FIX32ToFloat(frame.Bottom);

	/*CString str;
	str.Format("%0.2f", right);
	AfxMessageBox(str);
	str.Format("%0.2f", bottom);
	AfxMessageBox(str);*/

	//图片控件的宽、高
	CRect picrect;
	GetDlgItem(IDC_PAPER_PREPICTURE)->GetWindowRect(picrect);
	float width = (float)picrect.Width(); //257
	float height = (float)picrect.Height(); //392

	CRect endrect;
	float widthscale;
	float heightscale;
	//根据英寸的值，设置比例，最终得到需要画出区域的大小
	if(unitindex == TWUN_INCHES)//英寸
	{
		widthscale = 100.00f * width / 900.00f;//width/900=x/(right*100)算x
		heightscale = 100.00f * height / 1400.00f;
	}
	else if(unitindex == TWUN_PIXELS)//像素
	{
		int nCapIndex = m_pUI->GetCurrentCapIndex(ICAP_XRESOLUTION);
		const FloatVector* lstCapValuesFlt = m_pUI->GetValidCapFloat(ICAP_XRESOLUTION);
		int reso = (int)lstCapValuesFlt->at(nCapIndex); //x分辨率
		float fscale = (float)reso/200.00; //200dpi对应1800 不同分辨率下，横坐标的最大值

		widthscale = width / (1800.00f * fscale);//width/1800=x/right算x
		heightscale = height / (2800.00f * fscale);
	}
	else if(unitindex == TWUN_CENTIMETERS)//厘米
	{
		widthscale = width / 27.00f;//width/27=x/right算x
		heightscale = height / 42.00f;
	}
	else{}
	endrect.right = (int)(right * widthscale);
	endrect.bottom = (int)(bottom * heightscale);
	
	//画图
	CDC *pDC = GetDlgItem(IDC_PAPER_PREPICTURE)->GetDC();
	CPen pen(PS_SOLID, 1, RGB(255,0,0));  
	CPen *pOldPen = pDC->SelectObject(&pen);  
	CBrush *pOldBr = (CBrush *)pDC->SelectStockObject(NULL_BRUSH); 
	pDC->Rectangle(0, 0, endrect.right, endrect.bottom);
	pDC->SelectObject(pOldBr);  
	pDC->SelectObject(pOldPen);

	Invalidate(FALSE);
}


void CPage_Paper::OnCbnSelchangePaper_Combo_Uints()
{
	// TODO: 在此添加控件通知处理程序代码
	int nIndex = m_combo_uints.GetCurSel();
	int nval = FindUnit(nIndex);	
	m_papermap[ICAP_UNITS] = (float)nval;
	m_combo_uints.SetCurSel(nIndex);

	m_pUI->SetCapValueInt(ICAP_UNITS,nval); //界面能够直接响应
	UpdateControls();

	Invalidate(); //刷新标尺
	nval = FindPaperSize(m_unitindex);
	m_pUI->SetCapValueInt(ICAP_UNITS, nval); //设置回去
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
				editvalue = (float)scrollpos;
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
				break;
			}
		case TWUN_PIXELS:
			{
				editvalue = (float)scrollpos; 
				str.Format("%d", scrollpos);
				break;
			}	
		}	
		m_papermap[UDSCAP_EDGE_UP] = editvalue;
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
				break;
			}
		case TWUN_PIXELS:
			{
				editvalue = (float)scrollpos;
				str.Format("%d", scrollpos);
				break;
			}	
		}	
		m_papermap[UDSCAP_EDGE_DOWN] = editvalue; 
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
				break;
			}
		case TWUN_PIXELS:
			{
				editvalue = (float)scrollpos;
				str.Format("%d", scrollpos);
				break;
			}	
		}
		m_papermap[UDSCAP_EDGE_LEFT] = editvalue; 
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
				break;
			}
		case TWUN_PIXELS:
			{
				editvalue = (float)scrollpos;
				str.Format("%d", scrollpos);
				break;
			}	
		}	
		m_papermap[UDSCAP_EDGE_RIGHT] = editvalue;
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
				break;
			}
		case TWUN_PIXELS:
			{
				editvalue = scrollpos;
				str.Format("%d", scrollpos);
				break;
			}	
		}	
		m_papermap[UDSCAP_XPOS] = editvalue; //存入Map
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
				break;
			}
		case TWUN_PIXELS:
			{
				editvalue = (float)scrollpos;
				str.Format("%d", scrollpos);
				break;
			}	
		}	
		m_papermap[UDSCAP_YPOS] = editvalue; //存入Map
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
	m_rectTracker.Draw(&dc); //橡皮筋类画好矩形框

	//画标尺
	//获得控件位置
	CRect rect;
	GetDlgItem(IDC_PAPER_PREPICTURE)->GetWindowRect(rect);
	ScreenToClient(rect);

	CPoint beginpoint;	//指定起点横坐标偏移
	int width = rect.Width();; //指定标尺宽度
	int height = rect.Height();; //指定标尺长度
	//画刻度线
	int degree;
	dc.SetTextAlign(TA_CENTER | TA_BOTTOM);//将刻度线数字标注在刻度线的上方
	dc.SetBkMode(TRANSPARENT);//消除白色背景

	int nIndex = m_combo_uints.GetCurSel();
	int nval = FindUnit(nIndex);
	if(nval == TWUN_INCHES)//英寸
	{
		beginpoint.x = rect.left;
		beginpoint.y = rect.top-2; //画在图片上方
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
	else if(nval == TWUN_PIXELS)//像素
	{
		int nCapIndex = m_pUI->GetCurrentCapIndex(ICAP_XRESOLUTION);
		const FloatVector* lstCapValuesFlt = m_pUI->GetValidCapFloat(ICAP_XRESOLUTION);
		int reso = (int)lstCapValuesFlt->at(nCapIndex); //x分辨率
		float fscale = (float)reso/200.00; //200dpi对应1800
		int xmax = (int)(1800.00 * fscale);//不同分辨率下，横坐标的最大值
		int ymax = (int)(2800.00 * fscale);//不同分辨率下，纵坐标的最大值
		/*CString str;
		str.Format("%d",xreso);
		AfxMessageBox(str);
		str.Format("%d",yreso);
		AfxMessageBox(str);
*/
		int xpos = 0;
		beginpoint.x = rect.left;
		beginpoint.y = rect.top-2; //画在图片上方

		int xMax = xmax * 100;
		int yMax = ymax * 100;
		int step = (int)(fscale * 20000.00f);
		//画大刻度
		for(degree = 0; degree <= xMax; degree += step)
		{
			dc.MoveTo(beginpoint.x + degree*width/xMax, beginpoint.y-width/9/3);
			dc.LineTo(beginpoint.x + degree*width/xMax, beginpoint.y);
			CString str;
			str.Format(_T("%d"), (degree/100));
			
			if(degree <= (int)(40000 * fscale))
			{
				dc.TextOut(beginpoint.x + degree*width/xMax - 3, beginpoint.y-width/9/3, str);
			}
			else
			{
				xpos += 1;
				dc.TextOut(beginpoint.x + degree*width/xMax + xpos - 2, beginpoint.y-width/9/3, str);
			}
		}
		//画小刻度
		for(degree = step/5; degree <= xMax; degree += step/5)  
		{  
			dc.MoveTo(beginpoint.x + degree*width/xMax, beginpoint.y - width/9/5);  
			dc.LineTo(beginpoint.x + degree*width/xMax, beginpoint.y);  
		}  

		//画左侧标尺
		//指定起点横坐标偏移
		beginpoint.x = rect.left-2;//画在图片左侧
		beginpoint.y = rect.top; 

		//画刻度线
		dc.SetTextAlign(TA_CENTER | TA_LEFT);//将刻度线数字标注在刻度线的左侧
		dc.SetBkMode(TRANSPARENT);//消除白色背景
		//画大刻度
		for(degree = 0; degree <= yMax; degree += step)
		{
			dc.MoveTo(beginpoint.x - height/14/3, beginpoint.y + degree*height/yMax);
			dc.LineTo(beginpoint.x, beginpoint.y + degree*height/yMax);
			CString str;
			str.Format(_T("%d"), (degree/100));
			dc.TextOut(beginpoint.x - height/14/3-17, beginpoint.y + degree*height/yMax-7, str); //使数字与线居中
		}
		//画小刻度
		for(degree = step/5; degree <= yMax; degree += step/5)  
		{  
			dc.MoveTo(beginpoint.x - height/14/5, beginpoint.y + degree*height/yMax);  
			dc.LineTo(beginpoint.x, beginpoint.y + degree*height/yMax);  
		} 
	}
	else if(nval == TWUN_CENTIMETERS)//厘米
	{
		beginpoint.x = rect.left;
		beginpoint.y = rect.top-2; //画在图片上方
		//画大刻度
		for(degree = 0; degree <= 2700; degree += 300)
		{
			dc.MoveTo(beginpoint.x + degree*width/2700, beginpoint.y-width/9/3);
			dc.LineTo(beginpoint.x + degree*width/2700, beginpoint.y);
			CString str;
			str.Format(_T("%d"), (degree/100));
			dc.TextOut(beginpoint.x + degree*width/2700, beginpoint.y-width/9/3, str);
		}
		//画小刻度
		for(degree = 60; degree <= 2700; degree += 60)  
		{  
			dc.MoveTo(beginpoint.x + degree*width/2700, beginpoint.y - width/9/5);  
			dc.LineTo(beginpoint.x + degree*width/2700, beginpoint.y);  
		}  

		//画左侧标尺
		//指定起点横坐标偏移
		beginpoint.x = rect.left-2;//画在图片左侧
		beginpoint.y = rect.top; 
		//画刻度线
		dc.SetTextAlign(TA_CENTER | TA_LEFT);//将刻度线数字标注在刻度线的左侧
		dc.SetBkMode(TRANSPARENT);//消除白色背景

		//画大刻度
		for(degree = 0; degree <= 4200; degree += 300)
		{
			dc.MoveTo(beginpoint.x - height/14/3, beginpoint.y + degree*height/4200);
			dc.LineTo(beginpoint.x, beginpoint.y + degree*height/4200);
			CString str;
			str.Format(_T("%d"), (degree/100));
			dc.TextOut(beginpoint.x - height/14/3-10, beginpoint.y + degree*height/4200-7, str); //使数字与线居中
		}
		//画小刻度
		for(degree = 60; degree <= 4200; degree += 60)  
		{  
			dc.MoveTo(beginpoint.x - height/14/5, beginpoint.y + degree*height/4200);  
			dc.LineTo(beginpoint.x, beginpoint.y + degree*height/4200);  
		} 
	}
	else{}
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


void CPage_Paper::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CRect rect;
	GetDlgItem(IDC_PAPER_PREPICTURE)->GetWindowRect(&rect);//获取窗体中控件的区域
	ScreenToClient(&rect); //转换为相对区域CRect

	//判断是否在图片框内，不处理不在图片框内的点击
	if (point.x < rect.left || point.x > rect.right || point.y < rect.top || point.y > rect.bottom)
		return;

	if(point.x > rect.left && point.y > rect.top)
	{
		if(m_rectTracker.HitTest(point) < 0) //如果未集中矩形选择框，则重新画选择框
		{
			m_rectTracker.TrackRubberBand(this, point, TRUE); //TRUE表示可以往任意方向画虚线框，若为FALSE，只能往右下画虚线框
			m_rectTracker.m_rect.NormalizeRect(); //正规化矩形
			m_tRect = m_rectTracker.m_rect;   //得到画好的橡皮筋框
			m_rectTracker.m_rect.SetRect(m_tRect.left,m_tRect.top,m_tRect.right,m_tRect.bottom); //画出调整好的矩形框
		}
		else //如果击中矩形选择框
		{
			m_rectTracker.SetCursor(this, nFlags); //改变鼠标的形状
			
			m_rectTracker.Track(this, point, TRUE);
			m_rectTracker.m_rect.NormalizeRect(); //正规化矩形

			m_tRect = m_rectTracker.m_rect; //得到画好的橡皮筋框

			//调整矩形框的位置
			if(m_tRect.top < rect.top)
			{//超出图片框顶部的位置
				m_tRect.bottom = rect.top - m_tRect.top + m_tRect.bottom;
				m_tRect.top = rect.top;
			}
			if(m_tRect.bottom > rect.bottom)
			{//超出底部的位置
				m_tRect.top = rect.bottom - m_tRect.bottom + m_tRect.top;
				m_tRect.bottom = rect.bottom;
			}
			if (m_tRect.right > rect.right)
			{//超出右边
				m_tRect.left = rect.right - m_tRect.right + m_tRect.left;
				m_tRect.right = rect.right;
			}
			if (m_tRect.left < rect.left)
			{//超出左边
				m_tRect.right= rect.left - m_tRect.left + m_tRect.right;
				m_tRect.left = rect.left;
			}
			m_rectTracker.m_rect.SetRect(m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
		}
		Invalidate(); //刷新窗口区域，是的CRectTracker对象重绘到窗口上 

		/*CPaintDC dc(this);
		CPen pen;
		pen.CreatePen(PS_SOLID,1,RGB(255,0,0));
		Draw(&dc,&pen);*/
	}
	__super::OnLButtonDown(nFlags, point);
}


void CPage_Paper::OnEnChangeBase_Edit_EdgeRight()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 __super::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);  // 接收数据
	CString str;
	m_edit_right.GetWindowText(str);
	float nval = _ttof(str);

	int nIndex = m_combo_uints.GetCurSel();
	float editvalue;
	switch(FindUnit(nIndex))
	{
	case TWUN_INCHES:
	case TWUN_CENTIMETERS:
		{		
			editvalue = nval * 100;  //需要除以100.		
			break;
		}
	case TWUN_PIXELS:
		{
			editvalue = nval; 
			break;
		}
	}
	m_scroll_right.SetScrollPos(editvalue);
	m_papermap[UDSCAP_EDGE_RIGHT] = nval;

	m_edit_right.SetSel(str.GetLength(), str.GetLength(),TRUE);  // 设置编辑框控件范围

	UpdateData(FALSE);  // 更新控件
}


void CPage_Paper::OnEnChangeBase_Edit_EdgeUp()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 __super::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);  // 接收数据
	CString str;
	m_edit_up.GetWindowText(str);
	float nval = _ttof(str);

	int nIndex = m_combo_uints.GetCurSel();
	float editvalue;
	switch(FindUnit(nIndex))
	{
	case TWUN_INCHES:
	case TWUN_CENTIMETERS:
		{		
			editvalue = nval * 100;  //需要除以100.		
			break;
		}
	case TWUN_PIXELS:
		{
			editvalue = nval; 
			break;
		}
	}
	m_scroll_up.SetScrollPos(editvalue);
	m_papermap[UDSCAP_EDGE_UP] = nval;

	m_edit_up.SetSel(str.GetLength(), str.GetLength(),TRUE);  // 设置编辑框控件范围

	UpdateData(FALSE);  // 更新控件
}


void CPage_Paper::OnEnChangeBase_Edit_EdgeLeft()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 __super::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);  // 接收数据
	CString str;
	m_edit_left.GetWindowText(str);
	float nval = _ttof(str);

	int nIndex = m_combo_uints.GetCurSel();
	float editvalue;
	switch(FindUnit(nIndex))
	{
	case TWUN_INCHES:
	case TWUN_CENTIMETERS:
		{		
			editvalue = nval * 100;  //需要除以100.		
			break;
		}
	case TWUN_PIXELS:
		{
			editvalue = nval; 
			break;
		}
	}
	m_scroll_left.SetScrollPos(editvalue);
	m_papermap[UDSCAP_EDGE_LEFT] = nval;

	m_edit_left.SetSel(str.GetLength(), str.GetLength(),TRUE);  // 设置编辑框控件范围

	UpdateData(FALSE);  // 更新控件
}


void CPage_Paper::OnEnChangeBase_Edit_EdgeDown()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 __super::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);  // 接收数据
	CString str;
	m_edit_down.GetWindowText(str);
	float nval = _ttof(str);

	int nIndex = m_combo_uints.GetCurSel();
	float editvalue;
	switch(FindUnit(nIndex))
	{
	case TWUN_INCHES:
	case TWUN_CENTIMETERS:
		{		
			editvalue = nval * 100;  //需要除以100.		
			break;
		}
	case TWUN_PIXELS:
		{
			editvalue = nval; 
			break;
		}
	}
	m_scroll_down.SetScrollPos(editvalue);
	m_papermap[UDSCAP_EDGE_DOWN] = nval;

	m_edit_down.SetSel(str.GetLength(), str.GetLength(),TRUE);  // 设置编辑框控件范围

	UpdateData(FALSE);  // 更新控件
}


void CPage_Paper::OnEnChangeBase_Edit_EdgeXpos()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 __super::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);  // 接收数据
	CString str;
	m_edit_xpos.GetWindowText(str);
	float nval = _ttof(str);

	int nIndex = m_combo_uints.GetCurSel();
	float editvalue;
	switch(FindUnit(nIndex))
	{
	case TWUN_INCHES:
	case TWUN_CENTIMETERS:
		{		
			editvalue = nval * 100;  //需要除以100.		
			break;
		}
	case TWUN_PIXELS:
		{
			editvalue = nval; 
			break;
		}
	}
	m_scroll_xpos.SetScrollPos(editvalue);
	m_papermap[UDSCAP_XPOS] = nval;

	m_edit_xpos.SetSel(str.GetLength(), str.GetLength(),TRUE);  // 设置编辑框控件范围

	UpdateData(FALSE);  // 更新控件
}


void CPage_Paper::OnEnChangeBase_Edit_EdgeYpos()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 __super::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);  // 接收数据
	CString str;
	m_edit_ypos.GetWindowText(str);
	float nval = _ttof(str);

	int nIndex = m_combo_uints.GetCurSel();
	float editvalue;
	switch(FindUnit(nIndex))
	{
	case TWUN_INCHES:
	case TWUN_CENTIMETERS:
		{		
			editvalue = nval * 100;  //需要除以100.		
			break;
		}
	case TWUN_PIXELS:
		{
			editvalue = nval; 
			break;
		}
	}
	m_scroll_ypos.SetScrollPos(editvalue);
	m_papermap[UDSCAP_YPOS] = nval;

	m_edit_ypos.SetSel(str.GetLength(), str.GetLength(),TRUE);  // 设置编辑框控件范围

	UpdateData(FALSE);  // 更新控件
}

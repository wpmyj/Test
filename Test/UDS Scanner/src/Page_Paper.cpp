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
	m_papermap.swap(map<int, float>());  // 清空并释放内存
}

void CPage_Paper::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_PAPERPRE_COMBO_UINT, m_combo_uints);
	DDX_Control(pDX, IDC_PAPER_CHECK_FRONTCOLOR, m_check_frontcolor);
	DDX_Control(pDX, IDC_PAPER_CHECK_FRONTBW, m_check_frontbw);
	DDX_Control(pDX, IDC_PAPER_CHECK_FRONTGRAY, m_check_frontgray);
	DDX_Control(pDX, IDC_PAPER_CHECK_BACKGRAY, m_check_backgray);
	DDX_Control(pDX, IDC_PAPER_CHECK_BACKCOLOR, m_check_backcolor);
	DDX_Control(pDX, IDC_PAPER_CHECK_BACKBW, m_check_backbw);
	DDX_Control(pDX, IDC_PAPER_BUTTON_FRONTCOLOR, m_btn_frontcolor);
	DDX_Control(pDX, IDC_PAPER_BUTTON_FRONTBW, m_btn_frontbw);
	DDX_Control(pDX, IDC_PAPER_BUTTON_FRONTGRAY, m_btn_frontgray);
	DDX_Control(pDX, IDC_PAPER_BUTTON_BACKGRAY, m_btn_backgray);
	DDX_Control(pDX, IDC_PAPER_BUTTON_BACKCOLOR, m_btn_backcolor);
	DDX_Control(pDX, IDC_PAPER_BUTTON_BACKBW, m_btn_backbw);
}


BEGIN_MESSAGE_MAP(CPage_Paper, CPropertyPage)
	ON_WM_VSCROLL()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_PAPER_CHECK_FRONTCOLOR, &CPage_Paper::OnPaper_Btn_Check_FrontColor)
	ON_BN_CLICKED(IDC_PAPER_CHECK_FRONTGRAY, &CPage_Paper::OnPaper_Btn_Check_FrontGray)
	ON_BN_CLICKED(IDC_PAPER_CHECK_FRONTBW, &CPage_Paper::OnPaper_Btn_Check_FrontBw)
	ON_BN_CLICKED(IDC_PAPER_CHECK_BACKBW, &CPage_Paper::OnPaper_Btn_Check_BackBw)
	ON_BN_CLICKED(IDC_PAPER_CHECK_BACKCOLOR, &CPage_Paper::OnPaper_Btn_Check_BackColor)
	ON_BN_CLICKED(IDC_PAPER_CHECK_BACKGRAY, &CPage_Paper::OnPaper_Btn_Check_BackGray)
	ON_CBN_SELCHANGE(IDC_PAPERPRE_COMBO_UINT, &CPage_Paper::OnCbnSelchangePaper_Combo_Uints)
END_MESSAGE_MAP()


// CPage_Paper 消息处理程序

void CPage_Paper::SetCapValue(void)
{
	MAP_CAP::iterator iter; 
	for(iter = m_papermap.begin(); iter != m_papermap.end(); iter++)
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


void CPage_Paper::InitPapermap(void)
{
	m_papermap.erase(m_papermap.begin(),m_papermap.end());//清空

	float value = m_pUI->GetCapValueFloat(UDSCAP_MULTISTREAM_VALUE);
	m_papermap[UDSCAP_MULTISTREAM_VALUE] = value; // 初始化时添加UDSCAP_MULTISTREAM_VALUE，保证SetCapValue()会更新该Cap的值
}


void CPage_Paper::SetCapMulti()
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

void CPage_Paper::UpdateControls(void)
{
	int nCapIndex;
	const IntVector* lstCapValues;
	float fCapValue;
	int nCapValue;
	CString strText;
	int nval;

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

void CPage_Paper::SetMultistream(void)
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

BOOL CPage_Paper::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	UpdateControls();

	//2.0版本界面
	InitPapermap();
	
	//橡皮筋类初始化
	m_rectTracker.m_nStyle = CRectTracker::solidLine;//设置RectTracker样式,实线CRectTracker::resizeOutside|
	m_rectTracker.m_nHandleSize = 5; //控制柄的像素大小

	//初始化纸张单位，并未m_endrect赋值
	int paperindex = m_pUI->GetCurrentCapIndex(ICAP_SUPPORTEDSIZES);
	int papervalue = FindPaperSize(paperindex);
	int unitindex  = m_pUI->GetCurrentCapIndex(ICAP_UNITS);
	int unitvalue = FindUnit(unitindex);
	UpdatePicRectangle(papervalue, unitvalue, 0, 0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CPage_Paper::UpdatePicRectangle(int index, int unitindex, int xpos, int ypos)
{
	InvalidateRect(NULL);
	UpdateWindow();

	//纸张大小尺寸：单位不同，值不同
  TW_FRAME frame;
	frame = m_pUI->GetCurrentFrame();
	float right = FIX32ToFloat(frame.Right); //A4: 8.27英寸; 1653.40像素; 21.00厘米
	float bottom = FIX32ToFloat(frame.Bottom);

	//图片控件的宽、高
	CRect picrect;
	GetDlgItem(IDC_PAPER_PREPICTURE)->GetWindowRect(picrect);
	float width = (float)picrect.Width(); //257
	float height = (float)picrect.Height(); //392

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
	m_endrect.right = (int)(right * widthscale);
	m_endrect.bottom = (int)(bottom * heightscale);
	
	//画图
	CDC *pDC = GetDlgItem(IDC_PAPER_PREPICTURE)->GetDC();
	CPen pen(PS_SOLID, 1, RGB(255,0,0));  
	CPen *pOldPen = pDC->SelectObject(&pen);  
	CBrush *pOldBr = (CBrush *)pDC->SelectStockObject(NULL_BRUSH); 
	pDC->Rectangle(xpos, ypos, m_endrect.right, m_endrect.bottom);
	pDC->SelectObject(pOldBr);  
	pDC->SelectObject(pOldPen);
}

void CPage_Paper::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CPropertyPage::OnVScroll(nSBCode, nPos, pScrollBar);
}


BOOL CPage_Paper::OnSetActive()
{
	// TODO: 在此添加专用代码和/或调用基类
	m_pUI->PreViewStatus();
	UpdateControls();
	return CPropertyPage::OnSetActive();
} 


void CPage_Paper::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 __super::OnPaint()
	DrawImage(); //画预览图
	
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

void CPage_Paper::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	/*
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
		*/
		/*CPaintDC dc(this);
		CPen pen;
		pen.CreatePen(PS_SOLID,1,RGB(255,0,0));
		Draw(&dc,&pen);*/
	//}
	__super::OnLButtonDown(nFlags, point);
}


BOOL CPage_Paper::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	//获取控件窗口指针  
	

	return __super::PreTranslateMessage(pMsg);
}


void CPage_Paper::PreView()
{
	UpdateData(TRUE);

	SetCapValue();
	m_pUI->SetCapValueInt(UDSCAP_DOCS_IN_ADF, 1);//预览时只扫描一张
	m_pUI->TW_SaveProfileToFile("上次使用模板");

	BYTE *data = NULL; //图像数据
	data = m_pUI->PreView();  

	if(data != NULL)
	{
		// 保存图片
		GetBmpFilePath();//为m_bmpFilePath赋值

		CFile file;
		try
		{
			if(file.Open(m_bmpFilePath, CFile::modeWrite | CFile::modeCreate))
			{
				//写入文件
				file.Write((LPSTR)&(m_pUI->m_bmpFileHeader), sizeof(BITMAPFILEHEADER)); // 写文件头
				file.Write((LPSTR)&(m_pUI->m_bmpInfoHeader), sizeof(BITMAPINFOHEADER)); // 写信息头
				if (m_pUI->m_nBpp < 16)
				{			
					DWORD dwColors = 0;
					if (true == m_pUI->GetColorsUsed(m_pUI->m_nBpp, dwColors))
					{
						file.Write((LPSTR)(m_pUI->m_bmpLpRGB),sizeof(RGBQUAD) * dwColors); // 写调色板
					}	

					if (m_pUI->m_bmpLpRGB)
					{
						delete []m_pUI->m_bmpLpRGB;
						m_pUI->m_bmpLpRGB = NULL;
					}
				}
				file.Write(data, m_pUI->m_nDIBSize); // 写数据
				file.Close();
				if (data)
				{
					delete []data;
					data = NULL;
				}	
			}
		}
		catch (...) 
		{
			AfxMessageBox("SaveDIB2Bmp Error!");
		}	

		Invalidate(); //直接刷新，OnPaint中实现DrawImage();
	}

	UpdateData(FALSE);
}

void CPage_Paper::DrawImage(void) 
{
	UpdateData(TRUE);
	//显示图片
	CWnd *pWnd = GetDlgItem(IDC_PAPER_PREPICTURE); 
	CDC* pDC = pWnd->GetDC();
	HDC hDC = pDC->GetSafeHdc();
	
	IplImage* img = cvLoadImage((CT2CA)m_bmpFilePath, 1);
	if(img != NULL)
	{
		//调整长宽比例因子，使图像显示不失真
		CRect newRect;
		int width = img->width;
		int height = img->height;

		if(width <= m_endrect.Width() && height <= m_endrect.Height()) //小图片，不缩放
		{
			newRect = CRect(m_endrect.TopLeft(), CSize(width,height));
		}
		else
		{
			float xScale = (float)m_endrect.Width() / (float)width;
			float yScale = (float)m_endrect.Height() / (float)height;
			float scale = xScale>=yScale?yScale:xScale; 
			newRect = CRect(m_endrect.TopLeft(), CSize((int)width*scale, (int)height*scale));
		}

		DrawToHDC(hDC, &newRect, img);
		ReleaseDC(pDC);
		cvReleaseImage(&img);

		UpdateData(FALSE);
	}
}

void CPage_Paper::GetBmpFilePath()
{
	if(GetTempSavePath(m_bmpFilePath))
	{
		strcat(m_bmpFilePath, "preview.bmp");
	}
}

bool CPage_Paper::GetTempSavePath(TCHAR* pszPath)
{
	TCHAR szTempPath[MAX_PATH];
	memset(szTempPath, 0, MAX_PATH);
	GetTempPath(MAX_PATH, szTempPath);

	SSTRCAT(szTempPath, MAX_PATH, MB_CAPTION);
	SSTRCAT(szTempPath, MAX_PATH, TEXT("\\"));

	if(false == CreateDir(szTempPath))
	{
		MessageBox(TEXT("创建临时文件夹失败！"));
		return false;
	}

	SSTRCPY(pszPath, MAX_PATH, szTempPath);
	return true;
}

bool CPage_Paper::CreateDir(const CString& strPath)
{
	if (!PathFileExists(strPath))
	{
		if (!CreateDirectory(strPath, NULL))
		{	
			return false;
		}
		return true;
	}

	return true;		
}

//DrawToHdc系列函数
RECT CPage_Paper::NormalizeRect(RECT r)  
{  
	int t;  

	if( r.left > r.right )  
	{  
		t = r.left;  
		r.left = r.right;  
		r.right = t;  
	}  

	if( r.top > r.bottom )  
	{  
		t = r.top;  
		r.top = r.bottom;  
		r.bottom = t;  
	}  

	return r;  
}  
CvRect CPage_Paper::RectToCvRect(RECT sr)  
{  
	sr = NormalizeRect( sr );  
	return cvRect( sr.left, sr.top, sr.right - sr.left, sr.bottom - sr.top );  
} 
void  CPage_Paper::FillBitmapInfo(BITMAPINFO* bmi, int width, int height, int bpp, int origin)  
{  
	assert( bmi && width >= 0 && height >= 0 && (bpp == 8 || bpp == 24 || bpp == 32));  

	BITMAPINFOHEADER* bmih = &(bmi->bmiHeader);  

	memset(bmih, 0, sizeof(*bmih));  
	bmih->biSize = sizeof(BITMAPINFOHEADER);  
	bmih->biWidth = width;  
	bmih->biHeight = origin ? abs(height) : -abs(height);  
	bmih->biPlanes = 1;  
	bmih->biBitCount = (unsigned short)bpp;  
	bmih->biCompression = BI_RGB;  

	if(bpp == 8)  
	{  
		RGBQUAD* palette = bmi->bmiColors;  
		int i;  
		for( i = 0; i < 256; i++ )  
		{  
			palette[i].rgbBlue = palette[i].rgbGreen = palette[i].rgbRed = (BYTE)i;  
			palette[i].rgbReserved = 0;  
		}  
	}  
}  
void  CPage_Paper::Show(IplImage* img, HDC dc, int x, int y, int w, int h, int from_x, int from_y)  
{  
	if( img && img->depth == IPL_DEPTH_8U )  
	{  
		uchar buffer[sizeof(BITMAPINFOHEADER) + 1024];  
		BITMAPINFO* bmi = (BITMAPINFO*)buffer;  
		int bmp_w = img->width, bmp_h = img->height;  

		int bpp = img ? (img->depth & 255)*img->nChannels : 0;
		FillBitmapInfo(bmi, bmp_w, bmp_h, bpp, img->origin);  

		from_x = MIN(MAX( from_x, 0 ), bmp_w - 1);  
		from_y = MIN(MAX( from_y, 0 ), bmp_h - 1);  

		int sw = MAX(MIN( bmp_w - from_x, w ), 0);  
		int sh = MAX(MIN( bmp_h - from_y, h ), 0);  

		SetDIBitsToDevice(  
			dc, x, y, sw, sh, from_x, from_y, from_y, sh,  
			img->imageData + from_y*img->widthStep,  
			bmi, DIB_RGB_COLORS);  
	}  
}  
void  CPage_Paper::DrawToHDC(HDC hDCDst, RECT* pDstRect, IplImage* img )  
{  
	if(pDstRect && img && img->depth == IPL_DEPTH_8U && img->imageData )  
	{  
		uchar buffer[sizeof(BITMAPINFOHEADER) + 1024];  
		BITMAPINFO* bmi = (BITMAPINFO*)buffer;  
		int bmp_w = img->width, bmp_h = img->height;  

		CvRect roi = cvGetImageROI(img);
		CvRect dst = RectToCvRect(*pDstRect);  

		if( roi.width == dst.width && roi.height == dst.height )  
		{  
			Show(img, hDCDst, dst.x, dst.y, dst.width, dst.height, roi.x, roi.y);  
			return;  
		}  

		if(roi.width > dst.width)  
		{  
			SetStretchBltMode(hDCDst, // handle to device context  
				HALFTONE );  
		}  
		else  
		{  
			SetStretchBltMode(hDCDst, // handle to device context  
				COLORONCOLOR );  
		}  

		int bpp = img ? (img->depth & 255)*img->nChannels : 0;
		FillBitmapInfo(bmi, bmp_w, bmp_h, bpp, img->origin);  
		::StretchDIBits(
			hDCDst,  
			dst.x, dst.y, dst.width, dst.height,  
			roi.x, roi.y, roi.width, roi.height,  
			img->imageData, bmi, DIB_RGB_COLORS, SRCCOPY);  

	}  
}  


void CPage_Paper::OnPaper_Btn_Check_FrontColor()
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


void CPage_Paper::OnPaper_Btn_Check_FrontGray()
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


void CPage_Paper::OnPaper_Btn_Check_FrontBw()
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


void CPage_Paper::OnPaper_Btn_Check_BackBw()
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


void CPage_Paper::OnPaper_Btn_Check_BackColor()
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


void CPage_Paper::OnPaper_Btn_Check_BackGray()
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


void CPage_Paper::OnCbnSelchangePaper_Combo_Uints()
{
	// TODO: 在此添加控件通知处理程序代码
	int nIndex = m_combo_uints.GetCurSel();
	int nval = FindUnit(nIndex);	
	m_combo_uints.SetCurSel(nIndex);
	m_pUI->SetCapValueInt(ICAP_UNITS, nval); //界面能够直接响应

	UpdateControls();

	Invalidate(); //先刷新标尺
	//int paperIndex = m_pUI->GetCurrentCapIndex(ICAP_SUPPORTEDSIZES);
	//int paperval = FindPaperSize(paperIndex);
	//UpdatePicRectangle(paperval, nval, 0, 0);
}

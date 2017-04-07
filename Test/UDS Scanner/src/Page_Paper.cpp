// Page_Paper.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "UDS Scanner.h"
#include "Page_Paper.h"
#include "afxdialogex.h"

// CPage_Paper �Ի���

IMPLEMENT_DYNAMIC(CPage_Paper, CPropertyPage)

CPage_Paper::CPage_Paper(MFC_UI *pUI)
	: m_pUI(pUI),CPropertyPage(CPage_Paper::IDD)
{
}

CPage_Paper::~CPage_Paper()
{
	m_papermap.swap(map<int, float>());  // ��ղ��ͷ��ڴ�
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


// CPage_Paper ��Ϣ�������

void CPage_Paper::SetCapValue(void)
{
	MAP_CAP::iterator iter; 
	for(iter = m_papermap.begin(); iter != m_papermap.end(); iter++)
	{
		switch(iter->first)
		{
		case UDSCAP_MULTISTREAM_VALUE: // �������ѡ��ֵ
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
	m_papermap.erase(m_papermap.begin(),m_papermap.end());//���

	float value = m_pUI->GetCapValueFloat(UDSCAP_MULTISTREAM_VALUE);
	m_papermap[UDSCAP_MULTISTREAM_VALUE] = value; // ��ʼ��ʱ���UDSCAP_MULTISTREAM_VALUE����֤SetCapValue()����¸�Cap��ֵ
}


void CPage_Paper::SetCapMulti()
{
	BYTE temp = (BYTE)m_pUI->GetCapValueFloat(UDSCAP_MULTISTREAM_VALUE);
	unsigned int doc_count = 0;

	/************************************************************
	* �ж϶���chebox��ѡ���״̬;
	* ��ѡ����&���϶�ӦbitΪ1��ֵ;
	* ��δѡ����'|'�϶�ӦbitΪ0��ֵ.
	************************************************************/
	// �����ɫ
	if (m_check_frontcolor.GetCheck()) {  
		temp |= 0x01;
		doc_count++;
	} 
	else {
		temp &= (0xFF-0x01);
	}
	// �����ɫ
	if (m_check_frontgray.GetCheck()) {
		temp |= 0x02;
		doc_count++;
	} 
	else {
		temp &= (0xFF-0x02);
	}
	// ����ڰ�
	if (m_check_frontbw.GetCheck()) {
		temp |= 0x04;
		doc_count++;
	} 
	else {
		temp &= (0xFF-0x04);
	}
	// �����ɫ
	if (m_check_backcolor.GetCheck()) {
		temp |= 0x10;
		doc_count++;
	} 
	else {
		temp &= (0xFF-0x10);
	}
	// ����Ҷ�
	if (m_check_backgray.GetCheck()) {
		temp |= 0x20;
		doc_count++;
	} 
	else {
		temp &= (0xFF-0x20);
	}
	// ����ڰ�
	if (m_check_backbw.GetCheck()) {
		temp |= 0x40;
		doc_count++;
	} 
	else {
		temp &= (0xFF-0x40);
	}

	m_pUI->SetCapValueFloat(UDSCAP_MULTISTREAM_VALUE, (float)temp); // �������ѡ��ֵ
	m_pUI->SetCapValueInt(UDSCAP_DOCS_IN_ADF,doc_count); // ����ֽ����

}

void CPage_Paper::UpdateControls(void)
{
	int nCapIndex;
	const IntVector* lstCapValues;
	float fCapValue;
	int nCapValue;
	CString strText;
	int nval;

	//ֽ������-��λ
	m_combo_uints.ResetContent();  // �������
	nCapIndex = m_pUI->GetCurrentCapIndex(ICAP_UNITS);
	lstCapValues = m_pUI->GetValidCap(ICAP_UNITS);
	for(unsigned int i=0; i<lstCapValues->size();i++)
	{
		switch(lstCapValues->at(i))
		{
		case TWUN_INCHES:
			m_combo_uints.InsertString(i,"Ӣ��Inches"); //Ӣ��
			break;
		case TWUN_PIXELS:
			m_combo_uints.InsertString(i,"����Pixels"); //����
			break;
		case TWUN_CENTIMETERS:
			m_combo_uints.InsertString(i,"����Centimeters"); //����
			break;
		default:
			continue;
		}
	}
	m_combo_uints.SetCurSel(nCapIndex);
	nval = FindUnit(nCapIndex);

	//���������Ĭ�ϲ�ʹ��
	nCapValue = (int)(m_pUI->GetCapValueBool(UDSCAP_MULTISTREAM));
	if(nCapValue == 1) //����ѡ��
	{
		// ��ȡ����ѡ��ֵ�����¿ؼ�״̬
		BYTE value = (BYTE)m_pUI->GetCapValueFloat(UDSCAP_MULTISTREAM_VALUE);
		// ѭ���ж�ÿbit��ֵ�������¶�Ӧ�ؼ���״̬
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
			value = value >> 1; // ʼ�ձȽ����λ
		} // for end
	}//if end
	SetMultistream();
	
}

void CPage_Paper::SetMultistream(void)
{
	int nCapValue = (int)(m_pUI->GetCapValueBool(UDSCAP_MULTISTREAM));

	if(nCapValue == 1) //����ѡ��
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
		//�������δѡ��ʱ������ѡ��Ҳ����Ҫѡ��
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

	//2.0�汾����
	InitPapermap();
	
	//��Ƥ�����ʼ��
	m_rectTracker.m_nStyle = CRectTracker::solidLine;//����RectTracker��ʽ,ʵ��CRectTracker::resizeOutside|
	m_rectTracker.m_nHandleSize = 5; //���Ʊ������ش�С

	//��ʼ��ֽ�ŵ�λ����δm_endrect��ֵ
	int paperindex = m_pUI->GetCurrentCapIndex(ICAP_SUPPORTEDSIZES);
	int papervalue = FindPaperSize(paperindex);
	int unitindex  = m_pUI->GetCurrentCapIndex(ICAP_UNITS);
	int unitvalue = FindUnit(unitindex);
	UpdatePicRectangle(papervalue, unitvalue, 0, 0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CPage_Paper::UpdatePicRectangle(int index, int unitindex, int xpos, int ypos)
{
	InvalidateRect(NULL);
	UpdateWindow();

	//ֽ�Ŵ�С�ߴ磺��λ��ͬ��ֵ��ͬ
  TW_FRAME frame;
	frame = m_pUI->GetCurrentFrame();
	float right = FIX32ToFloat(frame.Right); //A4: 8.27Ӣ��; 1653.40����; 21.00����
	float bottom = FIX32ToFloat(frame.Bottom);

	//ͼƬ�ؼ��Ŀ���
	CRect picrect;
	GetDlgItem(IDC_PAPER_PREPICTURE)->GetWindowRect(picrect);
	float width = (float)picrect.Width(); //257
	float height = (float)picrect.Height(); //392

	float widthscale;
	float heightscale;
	//����Ӣ���ֵ�����ñ��������յõ���Ҫ��������Ĵ�С
	if(unitindex == TWUN_INCHES)//Ӣ��
	{
		widthscale = 100.00f * width / 900.00f;//width/900=x/(right*100)��x
		heightscale = 100.00f * height / 1400.00f;
	}
	else if(unitindex == TWUN_PIXELS)//����
	{
		int nCapIndex = m_pUI->GetCurrentCapIndex(ICAP_XRESOLUTION);
		const FloatVector* lstCapValuesFlt = m_pUI->GetValidCapFloat(ICAP_XRESOLUTION);
		int reso = (int)lstCapValuesFlt->at(nCapIndex); //x�ֱ���
		float fscale = (float)reso/200.00; //200dpi��Ӧ1800 ��ͬ�ֱ����£�����������ֵ

		widthscale = width / (1800.00f * fscale);//width/1800=x/right��x
		heightscale = height / (2800.00f * fscale);
	}
	else if(unitindex == TWUN_CENTIMETERS)//����
	{
		widthscale = width / 27.00f;//width/27=x/right��x
		heightscale = height / 42.00f;
	}
	else{}
	m_endrect.right = (int)(right * widthscale);
	m_endrect.bottom = (int)(bottom * heightscale);
	
	//��ͼ
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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CPropertyPage::OnVScroll(nSBCode, nPos, pScrollBar);
}


BOOL CPage_Paper::OnSetActive()
{
	// TODO: �ڴ����ר�ô����/����û���
	m_pUI->PreViewStatus();
	UpdateControls();
	return CPropertyPage::OnSetActive();
} 


void CPage_Paper::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� __super::OnPaint()
	DrawImage(); //��Ԥ��ͼ
	
	m_rectTracker.Draw(&dc); //��Ƥ���໭�þ��ο�

	//�����
	//��ÿؼ�λ��
	CRect rect;
	GetDlgItem(IDC_PAPER_PREPICTURE)->GetWindowRect(rect);
	ScreenToClient(rect);

	CPoint beginpoint;	//ָ����������ƫ��
	int width = rect.Width();; //ָ����߿��
	int height = rect.Height();; //ָ����߳���
	//���̶���
	int degree;
	dc.SetTextAlign(TA_CENTER | TA_BOTTOM);//���̶������ֱ�ע�ڿ̶��ߵ��Ϸ�
	dc.SetBkMode(TRANSPARENT);//������ɫ����

	int nIndex = m_combo_uints.GetCurSel();
	int nval = FindUnit(nIndex);
	if(nval == TWUN_INCHES)//Ӣ��
	{
		beginpoint.x = rect.left;
		beginpoint.y = rect.top-2; //����ͼƬ�Ϸ�
		//����̶�
		for(degree = 0; degree <= 900; degree += 100)
		{
			dc.MoveTo(beginpoint.x + degree*width/900, beginpoint.y-width/9/3);
			dc.LineTo(beginpoint.x + degree*width/900, beginpoint.y);
			CString str;
			str.Format(_T("%d"), (degree/100));
			dc.TextOut(beginpoint.x + degree*width/900, beginpoint.y-width/9/3, str);
		}
		//��С�̶�
		for(degree = 20; degree <= 900; degree += 20)  
		{  
			dc.MoveTo(beginpoint.x + degree*width/900, beginpoint.y - width/9/5);  
			dc.LineTo(beginpoint.x + degree*width/900, beginpoint.y);  
		}  

		//�������
		//ָ����������ƫ��
		beginpoint.x = rect.left-2;//����ͼƬ���
		beginpoint.y = rect.top; 
		//���̶���
		dc.SetTextAlign(TA_CENTER | TA_LEFT);//���̶������ֱ�ע�ڿ̶��ߵ����
		dc.SetBkMode(TRANSPARENT);//������ɫ����
		//����̶�
		for(degree = 0; degree <= 1400; degree += 100)
		{
			dc.MoveTo(beginpoint.x - height/14/3, beginpoint.y + degree*height/1400);
			dc.LineTo(beginpoint.x, beginpoint.y + degree*height/1400);
			CString str;
			str.Format(_T("%d"), (degree/100));
			dc.TextOut(beginpoint.x - height/14/3-10, beginpoint.y + degree*height/1400-7, str); //ʹ�������߾���
		}
		//��С�̶�
		for(degree = 20; degree <= 1400; degree += 20)  
		{  
			dc.MoveTo(beginpoint.x - height/14/5, beginpoint.y + degree*height/1400);  
			dc.LineTo(beginpoint.x, beginpoint.y + degree*height/1400);  
		} 
	}
	else if(nval == TWUN_PIXELS)//����
	{
		int nCapIndex = m_pUI->GetCurrentCapIndex(ICAP_XRESOLUTION);
		const FloatVector* lstCapValuesFlt = m_pUI->GetValidCapFloat(ICAP_XRESOLUTION);
		int reso = (int)lstCapValuesFlt->at(nCapIndex); //x�ֱ���
		float fscale = (float)reso/200.00; //200dpi��Ӧ1800
		int xmax = (int)(1800.00 * fscale);//��ͬ�ֱ����£�����������ֵ
		int ymax = (int)(2800.00 * fscale);//��ͬ�ֱ����£�����������ֵ
	
		int xpos = 0;
		beginpoint.x = rect.left;
		beginpoint.y = rect.top-2; //����ͼƬ�Ϸ�

		int xMax = xmax * 100;
		int yMax = ymax * 100;
		int step = (int)(fscale * 20000.00f);
		//����̶�
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
		//��С�̶�
		for(degree = step/5; degree <= xMax; degree += step/5)  
		{  
			dc.MoveTo(beginpoint.x + degree*width/xMax, beginpoint.y - width/9/5);  
			dc.LineTo(beginpoint.x + degree*width/xMax, beginpoint.y);  
		}  

		//�������
		//ָ����������ƫ��
		beginpoint.x = rect.left-2;//����ͼƬ���
		beginpoint.y = rect.top; 

		//���̶���
		dc.SetTextAlign(TA_CENTER | TA_LEFT);//���̶������ֱ�ע�ڿ̶��ߵ����
		dc.SetBkMode(TRANSPARENT);//������ɫ����
		//����̶�
		for(degree = 0; degree <= yMax; degree += step)
		{
			dc.MoveTo(beginpoint.x - height/14/3, beginpoint.y + degree*height/yMax);
			dc.LineTo(beginpoint.x, beginpoint.y + degree*height/yMax);
			CString str;
			str.Format(_T("%d"), (degree/100));
			dc.TextOut(beginpoint.x - height/14/3-17, beginpoint.y + degree*height/yMax-7, str); //ʹ�������߾���
		}
		//��С�̶�
		for(degree = step/5; degree <= yMax; degree += step/5)  
		{  
			dc.MoveTo(beginpoint.x - height/14/5, beginpoint.y + degree*height/yMax);  
			dc.LineTo(beginpoint.x, beginpoint.y + degree*height/yMax);  
		} 
	}
	else if(nval == TWUN_CENTIMETERS)//����
	{
		beginpoint.x = rect.left;
		beginpoint.y = rect.top-2; //����ͼƬ�Ϸ�
		//����̶�
		for(degree = 0; degree <= 2700; degree += 300)
		{
			dc.MoveTo(beginpoint.x + degree*width/2700, beginpoint.y-width/9/3);
			dc.LineTo(beginpoint.x + degree*width/2700, beginpoint.y);
			CString str;
			str.Format(_T("%d"), (degree/100));
			dc.TextOut(beginpoint.x + degree*width/2700, beginpoint.y-width/9/3, str);
		}
		//��С�̶�
		for(degree = 60; degree <= 2700; degree += 60)  
		{  
			dc.MoveTo(beginpoint.x + degree*width/2700, beginpoint.y - width/9/5);  
			dc.LineTo(beginpoint.x + degree*width/2700, beginpoint.y);  
		}  

		//�������
		//ָ����������ƫ��
		beginpoint.x = rect.left-2;//����ͼƬ���
		beginpoint.y = rect.top; 
		//���̶���
		dc.SetTextAlign(TA_CENTER | TA_LEFT);//���̶������ֱ�ע�ڿ̶��ߵ����
		dc.SetBkMode(TRANSPARENT);//������ɫ����

		//����̶�
		for(degree = 0; degree <= 4200; degree += 300)
		{
			dc.MoveTo(beginpoint.x - height/14/3, beginpoint.y + degree*height/4200);
			dc.LineTo(beginpoint.x, beginpoint.y + degree*height/4200);
			CString str;
			str.Format(_T("%d"), (degree/100));
			dc.TextOut(beginpoint.x - height/14/3-10, beginpoint.y + degree*height/4200-7, str); //ʹ�������߾���
		}
		//��С�̶�
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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	/*
	CRect rect;
	GetDlgItem(IDC_PAPER_PREPICTURE)->GetWindowRect(&rect);//��ȡ�����пؼ�������
	ScreenToClient(&rect); //ת��Ϊ�������CRect

	//�ж��Ƿ���ͼƬ���ڣ���������ͼƬ���ڵĵ��
	if (point.x < rect.left || point.x > rect.right || point.y < rect.top || point.y > rect.bottom)
		return;

	if(point.x > rect.left && point.y > rect.top)
	{
		if(m_rectTracker.HitTest(point) < 0) //���δ���о���ѡ��������»�ѡ���
		{
			m_rectTracker.TrackRubberBand(this, point, TRUE); //TRUE��ʾ���������ⷽ�����߿���ΪFALSE��ֻ�������»����߿�
			m_rectTracker.m_rect.NormalizeRect(); //���滯����
			m_tRect = m_rectTracker.m_rect;   //�õ����õ���Ƥ���
			m_rectTracker.m_rect.SetRect(m_tRect.left,m_tRect.top,m_tRect.right,m_tRect.bottom); //���������õľ��ο�
		}
		else //������о���ѡ���
		{
			m_rectTracker.SetCursor(this, nFlags); //�ı�������״
			
			m_rectTracker.Track(this, point, TRUE);
			m_rectTracker.m_rect.NormalizeRect(); //���滯����

			m_tRect = m_rectTracker.m_rect; //�õ����õ���Ƥ���

			//�������ο��λ��
			if(m_tRect.top < rect.top)
			{//����ͼƬ�򶥲���λ��
				m_tRect.bottom = rect.top - m_tRect.top + m_tRect.bottom;
				m_tRect.top = rect.top;
			}
			if(m_tRect.bottom > rect.bottom)
			{//�����ײ���λ��
				m_tRect.top = rect.bottom - m_tRect.bottom + m_tRect.top;
				m_tRect.bottom = rect.bottom;
			}
			if (m_tRect.right > rect.right)
			{//�����ұ�
				m_tRect.left = rect.right - m_tRect.right + m_tRect.left;
				m_tRect.right = rect.right;
			}
			if (m_tRect.left < rect.left)
			{//�������
				m_tRect.right= rect.left - m_tRect.left + m_tRect.right;
				m_tRect.left = rect.left;
			}
			m_rectTracker.m_rect.SetRect(m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
		}
		Invalidate(); //ˢ�´��������ǵ�CRectTracker�����ػ浽������ 
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
	// TODO: �ڴ����ר�ô����/����û���
	//��ȡ�ؼ�����ָ��  
	

	return __super::PreTranslateMessage(pMsg);
}


void CPage_Paper::PreView()
{
	UpdateData(TRUE);

	SetCapValue();
	m_pUI->SetCapValueInt(UDSCAP_DOCS_IN_ADF, 1);//Ԥ��ʱֻɨ��һ��
	m_pUI->TW_SaveProfileToFile("�ϴ�ʹ��ģ��");

	BYTE *data = NULL; //ͼ������
	data = m_pUI->PreView();  

	if(data != NULL)
	{
		// ����ͼƬ
		GetBmpFilePath();//Ϊm_bmpFilePath��ֵ

		CFile file;
		try
		{
			if(file.Open(m_bmpFilePath, CFile::modeWrite | CFile::modeCreate))
			{
				//д���ļ�
				file.Write((LPSTR)&(m_pUI->m_bmpFileHeader), sizeof(BITMAPFILEHEADER)); // д�ļ�ͷ
				file.Write((LPSTR)&(m_pUI->m_bmpInfoHeader), sizeof(BITMAPINFOHEADER)); // д��Ϣͷ
				if (m_pUI->m_nBpp < 16)
				{			
					DWORD dwColors = 0;
					if (true == m_pUI->GetColorsUsed(m_pUI->m_nBpp, dwColors))
					{
						file.Write((LPSTR)(m_pUI->m_bmpLpRGB),sizeof(RGBQUAD) * dwColors); // д��ɫ��
					}	

					if (m_pUI->m_bmpLpRGB)
					{
						delete []m_pUI->m_bmpLpRGB;
						m_pUI->m_bmpLpRGB = NULL;
					}
				}
				file.Write(data, m_pUI->m_nDIBSize); // д����
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

		Invalidate(); //ֱ��ˢ�£�OnPaint��ʵ��DrawImage();
	}

	UpdateData(FALSE);
}

void CPage_Paper::DrawImage(void) 
{
	UpdateData(TRUE);
	//��ʾͼƬ
	CWnd *pWnd = GetDlgItem(IDC_PAPER_PREPICTURE); 
	CDC* pDC = pWnd->GetDC();
	HDC hDC = pDC->GetSafeHdc();
	
	IplImage* img = cvLoadImage((CT2CA)m_bmpFilePath, 1);
	if(img != NULL)
	{
		//��������������ӣ�ʹͼ����ʾ��ʧ��
		CRect newRect;
		int width = img->width;
		int height = img->height;

		if(width <= m_endrect.Width() && height <= m_endrect.Height()) //СͼƬ��������
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
		MessageBox(TEXT("������ʱ�ļ���ʧ�ܣ�"));
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

//DrawToHdcϵ�к���
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nIndex = m_combo_uints.GetCurSel();
	int nval = FindUnit(nIndex);	
	m_combo_uints.SetCurSel(nIndex);
	m_pUI->SetCapValueInt(ICAP_UNITS, nval); //�����ܹ�ֱ����Ӧ

	UpdateControls();

	Invalidate(); //��ˢ�±��
	//int paperIndex = m_pUI->GetCurrentCapIndex(ICAP_SUPPORTEDSIZES);
	//int paperval = FindPaperSize(paperIndex);
	//UpdatePicRectangle(paperval, nval, 0, 0);
}

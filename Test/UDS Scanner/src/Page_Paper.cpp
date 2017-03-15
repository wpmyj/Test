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
	ON_EN_CHANGE(IDC_PAPER_EDIT_RIGHT, &CPage_Paper::OnEnChangeBase_Edit_EdgeRight)
	ON_EN_CHANGE(IDC_PAPER_EDIT_UP, &CPage_Paper::OnEnChangeBase_Edit_EdgeUp)
	ON_EN_CHANGE(IDC_PAPER_EDIT_LEFT, &CPage_Paper::OnEnChangeBase_Edit_EdgeLeft)
	ON_EN_CHANGE(IDC_PAPER_EDIT_DOWN, &CPage_Paper::OnEnChangeBase_Edit_EdgeDown)
	ON_EN_CHANGE(IDC_PAPER_EDIT_XPOS, &CPage_Paper::OnEnChangeBase_Edit_EdgeXpos)
	ON_EN_CHANGE(IDC_PAPER_EDIT_YPOS, &CPage_Paper::OnEnChangeBase_Edit_EdgeYpos)
END_MESSAGE_MAP()


// CPage_Paper ��Ϣ�������
void CPage_Paper::SetCapValue(void)
{
	MAP_CAP::iterator iter; 
	for(iter = m_papermap.begin(); iter != m_papermap.end(); iter++)
	{
		switch(iter->first)
		{	
		case ICAP_UNITS:  //��λ
		case ICAP_SUPPORTEDSIZES:  //ֽ�Ŵ�С	
		case ICAP_COMPRESSION: //ѹ����ʽ
			{
				m_pUI->SetCapValueInt(iter->first,iter->second); 
				break;
			}	

		case ICAP_PHYSICALWIDTH: //���
		case ICAP_PHYSICALHEIGHT:
		case UDSCAP_EDGE_UP: // ��Ե����
		case UDSCAP_EDGE_DOWN:
		case UDSCAP_EDGE_LEFT:
		case UDSCAP_EDGE_RIGHT:
		case UDSCAP_XPOS: //Xƫ����
		case UDSCAP_YPOS:
			{
				m_pUI->SetCapValueFloat(iter->first, iter->second); 
				break;
			}

		case UDSCAP_COMPRESSVALUE: //ѹ����
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
	m_papermap[ICAP_UNITS] = (float)nval;
	
	// �Զ�������
	TW_FRAME frame;	
	int nIndex = m_combo_uints.GetCurSel();
	//��λΪinches��centiʱedit��ʾ��λС��������ֱ����ʾ����
	switch(FindUnit(nIndex)) //��ǰ����ĵ�λ
	{
	case TWUN_INCHES:
		{
			frame = m_pUI->GetCurrentFrame();
			strText.Format("%0.2f",FIX32ToFloat(frame.Right));
			SetDlgItemText(IDC_PAPER_EDIT_WIDTH, strText);

			frame = m_pUI->GetCurrentFrame();
			strText.Format("%0.2f",FIX32ToFloat(frame.Bottom));
			SetDlgItemText(IDC_PAPER_EDIT_HEIGHT, strText);

			//��Ե���� ��������
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

			//XYƫ����
			fCapValue = m_pUI->GetCapValueFloat(UDSCAP_XPOS);
			strText.Format("%0.2f",fCapValue);
			SetDlgItemText(IDC_PAPER_EDIT_XPOS,strText);

			fCapValue = m_pUI->GetCapValueFloat(UDSCAP_YPOS);
			strText.Format("%0.2f",fCapValue);
			SetDlgItemText(IDC_PAPER_EDIT_YPOS,strText);
		
			break;
		}
	case TWUN_CENTIMETERS:
		{
			frame = m_pUI->GetCurrentFrame();
			strText.Format("%0.2f",FIX32ToFloat(frame.Right));
			SetDlgItemText(IDC_PAPER_EDIT_WIDTH, strText);

			frame = m_pUI->GetCurrentFrame();
			strText.Format("%0.2f",FIX32ToFloat(frame.Bottom));
			SetDlgItemText(IDC_PAPER_EDIT_HEIGHT, strText);

			//��Ե���� ��������
			float fvalue;
			fCapValue = m_pUI->GetCapValueFloat(UDSCAP_EDGE_UP);
			fvalue = ConvertUnits(fCapValue, TWUN_INCHES, TWUN_CENTIMETERS, m_resolution);
			strText.Format("%0.2f",fvalue);
			SetDlgItemText(IDC_PAPER_EDIT_UP,strText);

			fCapValue = m_pUI->GetCapValueFloat(UDSCAP_EDGE_DOWN);
			fvalue = ConvertUnits(fCapValue, TWUN_INCHES, TWUN_CENTIMETERS, m_resolution);
			strText.Format("%0.2f",fvalue);
			SetDlgItemText(IDC_PAPER_EDIT_DOWN,strText);

			fCapValue = m_pUI->GetCapValueFloat(UDSCAP_EDGE_LEFT);
			fvalue = ConvertUnits(fCapValue, TWUN_INCHES, TWUN_CENTIMETERS, m_resolution);
			strText.Format("%0.2f",fvalue);
			SetDlgItemText(IDC_PAPER_EDIT_LEFT,strText);

			fCapValue = m_pUI->GetCapValueFloat(UDSCAP_EDGE_RIGHT);
			fvalue = ConvertUnits(fCapValue, TWUN_INCHES, TWUN_CENTIMETERS, m_resolution);
			strText.Format("%0.2f",fvalue);
			SetDlgItemText(IDC_PAPER_EDIT_RIGHT,strText);

			//XYƫ����
			fCapValue = m_pUI->GetCapValueFloat(UDSCAP_XPOS);
			fvalue = ConvertUnits(fCapValue, TWUN_INCHES, TWUN_CENTIMETERS, m_resolution);
			strText.Format("%0.2f",fvalue);
			SetDlgItemText(IDC_PAPER_EDIT_XPOS,strText);

			fCapValue = m_pUI->GetCapValueFloat(UDSCAP_YPOS);
			fvalue = ConvertUnits(fCapValue, TWUN_INCHES, TWUN_CENTIMETERS, m_resolution);
			strText.Format("%0.2f",fvalue);
			SetDlgItemText(IDC_PAPER_EDIT_YPOS,strText);

			break;
		}
	case TWUN_PIXELS:
		{
			frame = m_pUI->GetCurrentFrame();
			strText.Format("%d",(int)FIX32ToFloat(frame.Right));
			SetDlgItemText(IDC_PAPER_EDIT_WIDTH, strText);

			frame = m_pUI->GetCurrentFrame();
			strText.Format("%d",(int)FIX32ToFloat(frame.Bottom));
			SetDlgItemText(IDC_PAPER_EDIT_HEIGHT, strText);

			//��Ե���� ��������
			int nvalue;
			fCapValue = m_pUI->GetCapValueFloat(UDSCAP_EDGE_UP);
			nvalue = ConvertUnits(fCapValue, TWUN_INCHES, TWUN_PIXELS, m_resolution);
			strText.Format("%d",nvalue);
			SetDlgItemText(IDC_PAPER_EDIT_UP,strText);

			fCapValue = m_pUI->GetCapValueFloat(UDSCAP_EDGE_DOWN);
			nvalue = ConvertUnits(fCapValue, TWUN_INCHES, TWUN_PIXELS, m_resolution);
			strText.Format("%d",nvalue);
			SetDlgItemText(IDC_PAPER_EDIT_DOWN,strText);

			fCapValue = m_pUI->GetCapValueFloat(UDSCAP_EDGE_LEFT);
			nvalue = ConvertUnits(fCapValue, TWUN_INCHES, TWUN_PIXELS, m_resolution);
			strText.Format("%d",nvalue);
			SetDlgItemText(IDC_PAPER_EDIT_LEFT,strText);

			fCapValue = m_pUI->GetCapValueFloat(UDSCAP_EDGE_RIGHT);
			nvalue = ConvertUnits(fCapValue, TWUN_INCHES, TWUN_PIXELS, m_resolution);
			strText.Format("%d",nvalue);
			SetDlgItemText(IDC_PAPER_EDIT_RIGHT,strText);

			fCapValue = m_pUI->GetCapValueFloat(UDSCAP_XPOS);
			nvalue = ConvertUnits(fCapValue, TWUN_INCHES, TWUN_PIXELS, m_resolution);
			strText.Format("%d",nvalue);
			SetDlgItemText(IDC_PAPER_EDIT_XPOS,strText);

			fCapValue = m_pUI->GetCapValueFloat(UDSCAP_YPOS);
			nvalue = ConvertUnits(fCapValue, TWUN_INCHES, TWUN_PIXELS, m_resolution);
			strText.Format("%d",nvalue);
			SetDlgItemText(IDC_PAPER_EDIT_YPOS,strText);

			break;
		}
	}

	//ֽ������-ֽ�Ŵ�С
	m_combo_standardsizes.ResetContent();  // �������
	nCapIndex = m_pUI->GetCurrentCapIndex(ICAP_SUPPORTEDSIZES);
	lstCapValues = m_pUI->GetValidCap(ICAP_SUPPORTEDSIZES);
	for(unsigned int i=0; i<lstCapValues->size();i++)
	{
		switch(lstCapValues->at(i))
		{
		case TWSS_NONE:
			m_combo_standardsizes.InsertString(i,"�Զ�");
			break;
		case TWSS_USLETTER:
			m_combo_standardsizes.InsertString(i,"US Letter (8.5\" x 11\")");  //216mm x 280mm
			break;
		case TWSS_USLEGAL:
			m_combo_standardsizes.InsertString(i,"US Legal (8.5\" x 14\")");  //216mm x 356mm
			break;
		case TWSS_A4:
			m_combo_standardsizes.InsertString(i,"ISO A4 (210mm x 297mm)"); //���ʱ�׼
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
		case TWSS_ISOB5:
			m_combo_standardsizes.InsertString(i,"ISO B5 (176mm x 250mm)");
			break;
		case TWSS_ISOB6:
			m_combo_standardsizes.InsertString(i,"ISO B6 (125mm x 176mm)");
			break;
		case TWSS_ISOB7:
			m_combo_standardsizes.InsertString(i,"ISO B7 (88mm x 125mm)");
			break;
		case TWSS_JISB5:
			m_combo_standardsizes.InsertString(i,"JIS B5 (182mm x 257mm)");//JIS�ձ���׼
			break;
		case TWSS_JISB6:
			m_combo_standardsizes.InsertString(i,"JIS B6 (128mm x 182mm)");
			break;
		case TWSS_JISB7:
			m_combo_standardsizes.InsertString(i,"JIS B7 (91mm x 128mm)");
			break;
		case UDSCAP_LONGDOCUMENT:
			m_combo_standardsizes.InsertString(i,"��ֽģʽ");
			break;
		case TWSS_MAXSIZE:
			m_combo_standardsizes.InsertString(i,"���ɨ�跶Χ");
			break;
		default:
			continue;
		}
	}
	m_combo_standardsizes.SetCurSel(nCapIndex);  //��ʾĬ��ֵ
	nval = FindPaperSize(nCapIndex);
	m_papermap[ICAP_SUPPORTEDSIZES] = (float)nval;//����ֻ������������Ҫ����CAP
	SetPaperSize();
	
	//ѹ��
	m_combo_compress.ResetContent();  // �������
	nCapIndex = m_pUI->GetCurrentCapIndex(ICAP_COMPRESSION);
	lstCapValues = m_pUI->GetValidCap(ICAP_COMPRESSION);
	for(unsigned int i=0; i<lstCapValues->size();i++)
	{
		switch(lstCapValues->at(i))
		{
		case TWCP_NONE:
			m_combo_compress.InsertString(i,"�Զ�"); //Ӣ��
			break;
		case TWCP_JPEG:
			m_combo_compress.InsertString(i,"JPEG"); //����
			break;
		case TWCP_GROUP4:
			m_combo_compress.InsertString(i,"G4"); //����
			break;
		default:
			continue;
		}
	}
	m_combo_compress.SetCurSel(nCapIndex);
	nval = (int)lstCapValues->at(nCapIndex);
	m_papermap[ICAP_COMPRESSION] = (float)nval;

	// ѹ���� 
	nCapValue = (int)(m_pUI->GetCapValueFloat(UDSCAP_COMPRESSVALUE)); 
	m_slider_compressvalue.SetPos(nCapValue);
	strText.Format("%d",nCapValue);
	SetDlgItemText(IDC_PAPER_EDIT_COMPRESSVALUE,strText);
	m_papermap[UDSCAP_COMPRESSVALUE] = (float)nCapValue;
}

void CPage_Paper::SetPaperSize(void)
{
	SetXYPos();

	int nIndex = m_combo_standardsizes.GetCurSel(); 
	int nval = FindPaperSize(nIndex);
	int nIndex_unit = m_combo_uints.GetCurSel();
	int nval_unit = FindUnit(nIndex_unit);
	CString str;
	float value = 118.00;
	if(nval == UDSCAP_LONGDOCUMENT)
	{
		m_scroll_height.ShowWindow(FALSE);

		//switch���ó�ֽ���ֵ
		switch(nval_unit)
		{
		case TWUN_INCHES:
			str.Format("%0.2f",value);
			m_edit_height.SetWindowText(str);
			break;
		case TWUN_CENTIMETERS:		
			value = ConvertUnits(value, TWUN_INCHES, TWUN_CENTIMETERS, m_resolution);	
			str.Format("%0.2f",value);
			m_edit_height.SetWindowText(str);
			break;
		case TWUN_PIXELS:
			value = ConvertUnits(value, TWUN_INCHES, TWUN_PIXELS, m_resolution);	
			str.Format("%d",(int)value);
			m_edit_height.SetWindowText(str);
			break;
		}
		
	}
	else
	{
		m_scroll_height.ShowWindow(TRUE);
	}

	/*if(nval == TWSS_NONE)  //ֽ�Ŵ�С���Զ���
	{
		m_edit_width.EnableWindow(TRUE);
		m_edit_height.EnableWindow(TRUE);
		m_scroll_width.EnableWindow(TRUE);
		m_scroll_height.EnableWindow(TRUE);
	} 
	else*/
	{
		m_edit_width.EnableWindow(FALSE);
		m_edit_height.EnableWindow(FALSE);
		m_scroll_width.EnableWindow(FALSE);
		m_scroll_height.EnableWindow(FALSE);
	}
}

BOOL CPage_Paper::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	UpdateControls();
	InitSliderCtrl();

	//��Ƥ�����ʼ��
	m_rectTracker.m_nStyle = CRectTracker::solidLine;//����RectTracker��ʽ,ʵ��CRectTracker::resizeOutside|
	m_rectTracker.m_nHandleSize = 5; //���Ʊ������ش�С

	const FloatVector* lstCapValuesFlt = m_pUI->GetValidCapFloat(ICAP_XRESOLUTION);
	int nCapIndex = m_pUI->GetCurrentCapIndex(ICAP_XRESOLUTION);
	m_resolution = (int)lstCapValuesFlt->at(nCapIndex); //x�ֱ���

	GetDlgItem(IDC_PAPER_COMBO_COMPRESS)->ShowWindow(FALSE); //��ʱ����
	GetDlgItem(IDC_PAPER_SLIDER_COMPRESSION)->ShowWindow(FALSE); //��ʱ����
	GetDlgItem(IDC_PAPER_EDIT_COMPRESSVALUE)->ShowWindow(FALSE); //��ʱ����
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CPage_Paper::InitSliderCtrl()
{
	float fMin,fMax,fStep;
	m_pUI->GetCapRangeFloat(UDSCAP_COMPRESSVALUE, fMin, fMax, fStep);
	m_slider_compressvalue.SetRange((int)fMin, (int)fMax);
	m_slider_compressvalue.SetTicFreq((int)fStep);
}


void CPage_Paper::SetScroll()
{
	// ���ô�ֱ�������Ĺ�����Χ	
	//���
	int nIndex = m_combo_uints.GetCurSel();
	CString str;
	int nval;
	float tempinche;

	switch(FindUnit(nIndex))
	{
	case TWUN_INCHES:	
		{
			//��
			maxinches_width = 850;
			m_scroll_width.SetScrollRange(0, maxinches_width); 
			m_edit_width.GetWindowText(str); //8.27
			nval = _ttof(str) * 100;
			m_scroll_width.SetScrollPos(nval); //������pos�Ǳ༭��ֵ��100��

			//XPOS��Χ
			maxinches_xpos = maxinches_width - nval;
			m_scroll_xpos.SetScrollRange(0, maxinches_xpos); //0.23
			//Xƫ����
			m_edit_xpos.GetWindowText(str);
			nval = _ttof(str) * 100;
			m_scroll_xpos.SetScrollPos(nval); 

			//��
			maxinches_height = 1400;
			m_scroll_height.SetScrollRange(0, maxinches_height);
			m_edit_height.GetWindowText(str);
			nval = _ttof(str) * 100;
			m_scroll_height.SetScrollPos(nval);

			//YPOS��Χ
			maxinches_ypos = maxinches_height - nval;
			m_scroll_ypos.SetScrollRange(0, maxinches_ypos);
			m_edit_ypos.GetWindowText(str);
			nval = _ttof(str) * 100;
			m_scroll_ypos.SetScrollPos(nval); 

			//��Ե���� ��������
			maxinches_updown = 50;
			m_scroll_up.SetScrollRange(0, maxinches_updown);
			m_edit_up.GetWindowText(str);
			nval = _ttof(str) * 100;
			m_scroll_up.SetScrollPos(nval); 

			m_scroll_down.SetScrollRange(0, maxinches_updown);
			m_edit_down.GetWindowText(str);
			nval = _ttof(str) * 100;
			m_scroll_down.SetScrollPos(nval); 

			maxinches_leftright = 19;
			m_scroll_left.SetScrollRange(0, maxinches_leftright);
			m_edit_left.GetWindowText(str);
			nval = _ttof(str) * 100;
			m_scroll_left.SetScrollPos(nval);

			m_scroll_right.SetScrollRange(0, maxinches_leftright);
			m_edit_right.GetWindowText(str);
			nval = _ttof(str) * 100;
			m_scroll_right.SetScrollPos(nval); 

			break;
		}
	case TWUN_CENTIMETERS:
		{
			//��
			maxinches_width = (int)ConvertUnits(850.00, TWUN_INCHES, TWUN_CENTIMETERS, m_resolution);
			m_scroll_width.SetScrollRange(0, maxinches_width); 
			m_edit_width.GetWindowText(str); //8.27
			nval = _ttof(str) * 100;
			m_scroll_width.SetScrollPos(nval); //������pos�Ǳ༭��ֵ��100��

			//��ǰ��ȡstrΪ���׵�λ,��ת��ΪӢ��
			tempinche = ConvertUnits(_ttof(str), TWUN_CENTIMETERS, TWUN_INCHES, m_resolution);
			//XPOS��Χ
			maxinches_xpos = (int)ConvertUnits(100.00*(8.50-tempinche), TWUN_INCHES, TWUN_CENTIMETERS, m_resolution);
			m_scroll_xpos.SetScrollRange(0, maxinches_xpos); 
			
			//X��Yƫ����
			m_edit_xpos.GetWindowText(str);
			nval = _ttof(str) * 100;
			m_scroll_xpos.SetScrollPos(nval); 

			//��
			maxinches_height = (int)ConvertUnits(1400.00, TWUN_INCHES, TWUN_CENTIMETERS, m_resolution);
			m_scroll_height.SetScrollRange(0, maxinches_height);
			m_edit_height.GetWindowText(str);
			nval = _ttof(str) * 100;
			m_scroll_height.SetScrollPos(nval);

			//��ǰ��ȡstrΪ���׵�λ,��ת��ΪӢ��
			tempinche = ConvertUnits(_ttof(str), TWUN_CENTIMETERS, TWUN_INCHES, m_resolution);
			//YPOS��Χ
			maxinches_ypos = (int)ConvertUnits(100.00*(14.00-tempinche), TWUN_INCHES, TWUN_CENTIMETERS, m_resolution);
			m_scroll_ypos.SetScrollRange(0, maxinches_ypos);
			m_edit_ypos.GetWindowText(str);
			nval = _ttof(str) * 100;
			m_scroll_ypos.SetScrollPos(nval); 

			//��Ե���� ��������
			maxinches_updown = (int)ConvertUnits(50.00, TWUN_INCHES, TWUN_CENTIMETERS, m_resolution);
			m_scroll_up.SetScrollRange(0, maxinches_updown);
			m_edit_up.GetWindowText(str);
			nval = _ttof(str) * 100;
			m_scroll_up.SetScrollPos(nval);

			m_scroll_down.SetScrollRange(0, maxinches_updown);
			m_edit_down.GetWindowText(str);
			nval = _ttof(str) * 100;
			m_scroll_down.SetScrollPos(nval); 

			maxinches_leftright = (int)ConvertUnits(19.00, TWUN_INCHES, TWUN_CENTIMETERS, m_resolution);
			m_scroll_left.SetScrollRange(0, maxinches_leftright);
			m_edit_left.GetWindowText(str);
			nval = _ttof(str) * 100;
			m_scroll_left.SetScrollPos(nval);

			m_scroll_right.SetScrollRange(0, maxinches_leftright);
			m_edit_right.GetWindowText(str);
			nval = _ttof(str) * 100;
			m_scroll_right.SetScrollPos(nval); 

			break;
		}
	case TWUN_PIXELS:
		{
			//��
			maxinches_width = (int)ConvertUnits(850.00, TWUN_INCHES, TWUN_PIXELS, m_resolution);
			m_scroll_width.SetScrollRange(0, maxinches_width); 
			m_edit_width.GetWindowText(str); 
			nval = _ttof(str);
			m_scroll_width.SetScrollPos(nval); 
			
			//��ǰ��ȡstrΪ���ص�λ,��ת��ΪӢ��
			tempinche = ConvertUnits(_ttof(str), TWUN_PIXELS, TWUN_INCHES, m_resolution);
			//XPOS��Χ
			maxinches_xpos = (int)ConvertUnits(100.00*(8.50-tempinche), TWUN_INCHES, TWUN_PIXELS, m_resolution);
			m_scroll_xpos.SetScrollRange(0, maxinches_xpos); 
			//X��Yƫ����
			m_edit_xpos.GetWindowText(str);
			nval = _ttof(str);
			m_scroll_xpos.SetScrollPos(nval); 
			
			//��
			maxinches_height = (int)ConvertUnits(1400.00, TWUN_INCHES, TWUN_PIXELS, m_resolution);
			m_scroll_height.SetScrollRange(0, maxinches_height);
			m_edit_height.GetWindowText(str);
			nval = _ttof(str);
			m_scroll_height.SetScrollPos(nval);

			//��ǰ��ȡstrΪ���׵�λ,��ת��ΪӢ��
			tempinche = ConvertUnits(_ttof(str), TWUN_PIXELS, TWUN_INCHES, m_resolution);
			//YPOS��Χ
			maxinches_ypos = (int)ConvertUnits(100.00*(14.00-tempinche), TWUN_INCHES, TWUN_PIXELS, m_resolution);
			m_scroll_ypos.SetScrollRange(0, maxinches_ypos);
			m_edit_ypos.GetWindowText(str);
			nval = _ttof(str);
			m_scroll_ypos.SetScrollPos(nval); 

			//��Ե���� ��������
			maxinches_updown = (int)ConvertUnits(50.00, TWUN_INCHES, TWUN_PIXELS, m_resolution);
			m_scroll_up.SetScrollRange(0, maxinches_updown);
			m_edit_up.GetWindowText(str);
			nval = _ttof(str);
			m_scroll_up.SetScrollPos(nval); 

			m_scroll_down.SetScrollRange(0, maxinches_updown);
			m_edit_down.GetWindowText(str);
			nval = _ttof(str);
			m_scroll_down.SetScrollPos(nval); 

			maxinches_leftright = (int)ConvertUnits(19.00, TWUN_INCHES, TWUN_PIXELS, m_resolution);
			m_scroll_left.SetScrollRange(0, maxinches_leftright);
			m_edit_left.GetWindowText(str);
			nval = _ttof(str);
			m_scroll_left.SetScrollPos(nval);

			m_scroll_right.SetScrollRange(0, maxinches_leftright);
			m_edit_right.GetWindowText(str);
			nval = _ttof(str);
			m_scroll_right.SetScrollPos(nval); 
		
			break;
		}		
	}//switch	
}


void CPage_Paper::OnCbnSelchangePaper_Combo_Standardsizes()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nIndex = m_combo_standardsizes.GetCurSel();
	int nval = FindPaperSize(nIndex);
	m_papermap[ICAP_SUPPORTEDSIZES] = nval;
	m_combo_standardsizes.SetCurSel(nIndex);
	SetPaperSize();
	m_pUI->SetCapValueInt(ICAP_SUPPORTEDSIZES, nval); //�ܹ�ֱ����Ӧ
	UpdateControls(); //���¿���

	int unitIndex = m_combo_uints.GetCurSel(); //��ǰ��λ
	int unitnval = FindUnit(unitIndex);
	
	UpdatePicRectangle(nval, unitnval, 0, 0);
}

void CPage_Paper::SetXYPos()
{
	int index = m_combo_standardsizes.GetCurSel();	
	switch(index)
	{
		//�Զ���US Legal
	case 0:
	case 2:
		{
			m_edit_xpos.EnableWindow(FALSE);
			m_edit_ypos.EnableWindow(FALSE);
			m_scroll_xpos.EnableWindow(FALSE);
			m_scroll_ypos.EnableWindow(FALSE);
			break;
		}

		//US Letter
	case 1:
		{
			m_edit_xpos.EnableWindow(FALSE);
			m_edit_ypos.EnableWindow(TRUE);
			m_scroll_xpos.EnableWindow(FALSE);
			m_scroll_ypos.EnableWindow(TRUE);
			break;
		}

		//����
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
	case 10:
	case 11:
	case 12:
	case 13:
	case 14:
		{
			m_edit_xpos.EnableWindow(TRUE);
			m_edit_ypos.EnableWindow(TRUE);
			m_scroll_xpos.EnableWindow(TRUE);
			m_scroll_ypos.EnableWindow(TRUE);
			break;
		}
	}
}

void CPage_Paper::UpdatePicRectangle(int index, int unitindex, int xpos, int ypos)
{
	InvalidateRect(NULL);
	UpdateWindow();

	//���õ�ǰCombo��Ӧ��λ��ֽ�Ŵ�С��Ч
	//m_pUI->SetCapValueInt(ICAP_UNITS, unitindex);
	//m_pUI->SetCapValueInt(ICAP_SUPPORTEDSIZES, index);	

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

	CRect endrect;
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
	endrect.right = (int)(right * widthscale);
	endrect.bottom = (int)(bottom * heightscale);
	
	//��ͼ
	CDC *pDC = GetDlgItem(IDC_PAPER_PREPICTURE)->GetDC();
	CPen pen(PS_SOLID, 1, RGB(255,0,0));  
	CPen *pOldPen = pDC->SelectObject(&pen);  
	CBrush *pOldBr = (CBrush *)pDC->SelectStockObject(NULL_BRUSH); 
	pDC->Rectangle(xpos, ypos, endrect.right, endrect.bottom);
	pDC->SelectObject(pOldBr);  
	pDC->SelectObject(pOldPen);

	Invalidate(FALSE);
}


void CPage_Paper::OnCbnSelchangePaper_Combo_Uints()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nIndex = m_combo_uints.GetCurSel();
	int nval = FindUnit(nIndex);	
	m_papermap[ICAP_UNITS] = nval;
	m_combo_uints.SetCurSel(nIndex);

	m_pUI->SetCapValueInt(ICAP_UNITS,nval); //�����ܹ�ֱ����Ӧ
	UpdateControls();

	Invalidate(); //ˢ�±��
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
		nval = TWUN_INCHES; //Ĭ��A4
	}
	return nval;
}

void CPage_Paper::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	int scrollpos;
	CString str;
	float editvalue;
	int nIndex = m_combo_uints.GetCurSel();
	int nval = FindUnit(nIndex);

	SetScroll(); //��ʼ����Χ

	switch(pScrollBar->GetDlgCtrlID())
	{
		//��
	case IDC_PAPER_SCROLLBAR_WIDTH:
		scrollpos = m_scroll_width.GetScrollPos(); //��ȡ��ֱ��������ǰλ��
		switch(nSBCode)
		{
		case SB_LINEUP: //������Ϲ���һ�У���pos��1
			scrollpos += 1;
			break;
		case SB_LINEDOWN: //������¹���һ�У���pos��1
			scrollpos -= 1; 
			break;
		case SB_TOP: //���
			scrollpos = maxinches_height;
			break;
		case SB_BOTTOM:
			scrollpos = 0;
			break;
		}
		// ���ù�����λ��  
		m_scroll_width.SetScrollPos(scrollpos);
		switch(nval)
		{
		case TWUN_INCHES:
		case TWUN_CENTIMETERS:
			{
				editvalue = (float)scrollpos/100.00;
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
		m_papermap[ICAP_PHYSICALWIDTH] = editvalue;
		SetDlgItemText(IDC_PAPER_EDIT_WIDTH, str); //����edit��ֵ
		break;
	
		//��
	case IDC_PAPER_SCROLLBAR_HEIGHT:
		//��
		scrollpos = m_scroll_height.GetScrollPos(); 
		switch(nSBCode)
		{
		case SB_LINEUP: 
			scrollpos += 1;
			break;
		case SB_LINEDOWN: 
			scrollpos -= 1; 
			break;
		case SB_TOP: //���
			scrollpos = maxinches_height;
			break;
		case SB_BOTTOM:
			scrollpos = 0;
			break;
		}
		// ���ù�����λ��  
		m_scroll_height.SetScrollPos(scrollpos);
		switch(nval)
		{
		case TWUN_INCHES:
		case TWUN_CENTIMETERS:
			{
				editvalue = (float)scrollpos/100.00;
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
		m_papermap[ICAP_PHYSICALHEIGHT] = editvalue;
		SetDlgItemText(IDC_PAPER_EDIT_HEIGHT, str); //����edit��ֵ
		break;

	//��Ե���� ��������
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
		case SB_TOP: //���
			scrollpos = maxinches_updown;
			break;
		case SB_BOTTOM:
			scrollpos = 0;
			break;
		}
		m_scroll_up.SetScrollPos(scrollpos);
		switch(nval)
		{
		case TWUN_INCHES:
		case TWUN_CENTIMETERS:
			{
				editvalue = (float)scrollpos/100.00;
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
		SetDlgItemText(IDC_PAPER_EDIT_UP, str); //����edit��ֵ
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
		case SB_TOP: //���
			scrollpos = maxinches_updown;
			break;
		case SB_BOTTOM:
			scrollpos = 0;
			break;
		}
		m_scroll_down.SetScrollPos(scrollpos);
		switch(nval)
		{
		case TWUN_INCHES:
		case TWUN_CENTIMETERS:
			{
				editvalue = (float)scrollpos/100.00;
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
		SetDlgItemText(IDC_PAPER_EDIT_DOWN, str); //����edit��ֵ
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
		case SB_TOP: //���
			scrollpos = maxinches_leftright;
			break;
		case SB_BOTTOM:
			scrollpos = 0;
			break;
		}
		m_scroll_left.SetScrollPos(scrollpos);
		switch(nval)
		{
		case TWUN_INCHES:
		case TWUN_CENTIMETERS:
			{
				editvalue = (float)scrollpos/100.00;
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
		SetDlgItemText(IDC_PAPER_EDIT_LEFT, str); //����edit��ֵ
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
		case SB_TOP: //���
			scrollpos = maxinches_leftright;
			break;
		case SB_BOTTOM:
			scrollpos = 0;
			break;
		}
		m_scroll_right.SetScrollPos(scrollpos);
		switch(nval)
		{
		case TWUN_INCHES:
		case TWUN_CENTIMETERS:
			{
				editvalue = (float)scrollpos/100.00;
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
		SetDlgItemText(IDC_PAPER_EDIT_RIGHT, str); //����edit��ֵ
		break;

	//Xƫ����
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
		case SB_TOP: //���
			scrollpos = maxinches_xpos;
			break;
		case SB_BOTTOM:
			scrollpos = 0;
			break;
		}
		m_scroll_xpos.SetScrollPos(scrollpos);
		switch(nval)
		{
		case TWUN_INCHES:
		case TWUN_CENTIMETERS:
			{
				editvalue = (float)scrollpos/100.00;
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
		m_papermap[UDSCAP_XPOS] = editvalue; //����Map
		SetDlgItemText(IDC_PAPER_EDIT_XPOS, str); //����edit��ֵ

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
		case SB_TOP: //���
			scrollpos = maxinches_ypos;
			break;
		case SB_BOTTOM:
			scrollpos = 0;
			break;
		}
		m_scroll_ypos.SetScrollPos(scrollpos);
		switch(nval)
		{
		case TWUN_INCHES:
		case TWUN_CENTIMETERS:
			{
				editvalue = (float)scrollpos/100.00;
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
		m_papermap[UDSCAP_YPOS] = editvalue; //����Map
		SetDlgItemText(IDC_PAPER_EDIT_YPOS, str); //����edit��ֵ

		break;

	default:
		break;
	}

	CPropertyPage::OnVScroll(nSBCode, nPos, pScrollBar);
}


int CPage_Paper::FindPaperSize(int index)
{
	int nval;
	
	switch(index)
	{
	case 0:
		nval = TWSS_NONE;
		break;
	case 1:
		nval = TWSS_USLETTER;
		break;
	case 2:
		nval = TWSS_USLEGAL;
		break;
	case 3:
		nval = TWSS_A4;
		break;
	case 4:
		nval = TWSS_A5;
		break;
	case 5:
		nval = TWSS_A6;
		break;
	case 6:
		nval = TWSS_A7;
		break;
	case 7:
		nval = TWSS_ISOB5;
		break;
	case 8:
		nval = TWSS_ISOB6;
		break;
	case 9:
		nval = TWSS_ISOB7;
		break;
	case 10:
		nval = TWSS_JISB5;
		break;
	case 11:
		nval = TWSS_JISB6;
		break;
	case 12:
		nval = TWSS_JISB7;
		break;
	case 13:
		nval = UDSCAP_LONGDOCUMENT;
		break;
	case 14:
		nval = TWSS_MAXSIZE;
		break;
	}
	return nval;
}


BOOL CPage_Paper::OnSetActive()
{
	// TODO: �ڴ����ר�ô����/����û���
	m_pUI->PreViewStatus();
	return CPropertyPage::OnSetActive();
} 


void CPage_Paper::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� __super::OnPaint()
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
		/*CString str;
		str.Format("%d",xreso);
		AfxMessageBox(str);
		str.Format("%d",yreso);
		AfxMessageBox(str);
*/
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


void CPage_Paper::OnEnChangeBase_Edit_Compressvalue()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д __super::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);  // ��������
	CString str;
	m_edit_compressvalue.GetWindowText(str);
	int nval = _ttoi(str);
	m_slider_compressvalue.SetPos(nval);
	
	m_papermap[UDSCAP_COMPRESSVALUE] = (float)nval;
	m_pUI->SetCapValueFloat(UDSCAP_COMPRESSVALUE, (float)nval);

	m_edit_compressvalue.SetSel(str.GetLength(), str.GetLength(),TRUE);  // ���ñ༭��ؼ���Χ

	UpdateData(FALSE);  // ���¿ؼ�
}


void CPage_Paper::OnNMCustomdrawPaper_Slider_Compressionvalue(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	//TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);  // ��������
	CString str;
	int sldValue = m_slider_compressvalue.GetPos();  // ��ȡ���鵱ǰλ��
	m_papermap[UDSCAP_COMPRESSVALUE] = sldValue;
	m_pUI->SetCapValueInt(UDSCAP_COMPRESSVALUE, sldValue); 
	
	str.Format("%d", sldValue);
	SetDlgItemText(IDC_PAPER_EDIT_COMPRESSVALUE, str);

	*pResult = 0;
}


void CPage_Paper::OnCbnSelchangePaper_Combo_Compress()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nIndex = m_combo_compress.GetCurSel();
	CString strCBText; 
	m_combo_compress.GetLBText( nIndex, strCBText);
	int nval;
	if (strCBText.Find("�Զ�") >= 0)
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
	m_pUI->SetCapValueInt(ICAP_COMPRESSION, nval); 
	m_combo_compress.SetCurSel(nIndex);
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


void CPage_Paper::OnEnChangeBase_Edit_EdgeRight()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д __super::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);  // ��������
	CString str;
	m_edit_right.GetWindowText(str);
	float fval = _ttof(str);

	int nIndex = m_combo_uints.GetCurSel();
	float editvalue;
	switch(FindUnit(nIndex))
	{
	case TWUN_INCHES:
	case TWUN_CENTIMETERS:
		{		
			editvalue = fval * 100;  //��Ҫ����100.		
			break;
		}
	case TWUN_PIXELS:
		{
			editvalue = fval; 
			break;
		}
	}
	if(editvalue > maxinches_leftright)
	{
		editvalue = maxinches_leftright;
	}
	else if(editvalue < 0)
	{
		editvalue = 0;
	}
	else{}

	m_scroll_right.SetScrollPos(editvalue);
	m_papermap[UDSCAP_EDGE_RIGHT] = fval;

	m_edit_right.SetSel(str.GetLength(), str.GetLength(),TRUE);  // ���ñ༭��ؼ���Χ

	UpdateData(FALSE);  // ���¿ؼ�
}


void CPage_Paper::OnEnChangeBase_Edit_EdgeUp()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д __super::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);  // ��������
	CString str;
	m_edit_up.GetWindowText(str);
	float fval = _ttof(str);

	int nIndex = m_combo_uints.GetCurSel();
	float editvalue;
	switch(FindUnit(nIndex))
	{
	case TWUN_INCHES:
	case TWUN_CENTIMETERS:
		{		
			editvalue = fval * 100;  //��Ҫ����100.		
			break;
		}
	case TWUN_PIXELS:
		{
			editvalue = fval; 
			break;
		}
	}
	if(editvalue > maxinches_updown)
	{
		editvalue = maxinches_updown;
	}
	else if(editvalue < 0)
	{
		editvalue = 0;
	}
	else{}

	m_scroll_up.SetScrollPos(editvalue);
	m_papermap[UDSCAP_EDGE_UP] = fval;

	m_edit_up.SetSel(str.GetLength(), str.GetLength(),TRUE);  // ���ñ༭��ؼ���Χ

	UpdateData(FALSE);  // ���¿ؼ�
}


void CPage_Paper::OnEnChangeBase_Edit_EdgeLeft()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д __super::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);  // ��������
	CString str;
	m_edit_left.GetWindowText(str);
	float fval = _ttof(str);

	int nIndex = m_combo_uints.GetCurSel();
	float editvalue;
	switch(FindUnit(nIndex))
	{
	case TWUN_INCHES:
	case TWUN_CENTIMETERS:
		{		
			editvalue = fval * 100;  //��Ҫ����100.		
			break;
		}
	case TWUN_PIXELS:
		{
			editvalue = fval; 
			break;
		}
	}

	if(editvalue > maxinches_leftright)
	{
		editvalue = maxinches_leftright;
	}
	else if(editvalue < 0)
	{
		editvalue = 0;
	}
	else{}

	m_scroll_left.SetScrollPos(editvalue);
	m_papermap[UDSCAP_EDGE_LEFT] = fval;

	m_edit_left.SetSel(str.GetLength(), str.GetLength(),TRUE);  // ���ñ༭��ؼ���Χ

	UpdateData(FALSE);  // ���¿ؼ�
}


void CPage_Paper::OnEnChangeBase_Edit_EdgeDown()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д __super::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);  // ��������
	CString str;
	m_edit_down.GetWindowText(str);
	float fval = _ttof(str);

	int nIndex = m_combo_uints.GetCurSel();
	float editvalue;
	switch(FindUnit(nIndex))
	{
	case TWUN_INCHES:
	case TWUN_CENTIMETERS:
		{		
			editvalue = fval * 100;  //��Ҫ����100
			break;
		}
	case TWUN_PIXELS:
		{
			editvalue = fval; 
			break;
		}
	}

	if(editvalue > maxinches_updown)
	{
		editvalue = maxinches_updown;
	}
	else if(editvalue < 0)
	{
		editvalue = 0;
	}
	else{}

	m_scroll_down.SetScrollPos(editvalue);
	m_papermap[UDSCAP_EDGE_DOWN] = fval;

	m_edit_down.SetSel(str.GetLength(), str.GetLength(),TRUE);  // ���ñ༭��ؼ���Χ

	UpdateData(FALSE);  // ���¿ؼ�
}


void CPage_Paper::OnEnChangeBase_Edit_EdgeXpos()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д __super::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);  // ��������
	CString str;
	m_edit_xpos.GetWindowText(str);
	float fval = _ttof(str);
	
	int nIndex = m_combo_uints.GetCurSel();
	float editvalue;
	switch(FindUnit(nIndex))
	{
	case TWUN_INCHES:
	case TWUN_CENTIMETERS:
		{		
			editvalue = fval * 100;  //��Ҫ����100.		
			break;
		}
	case TWUN_PIXELS:
		{
			editvalue = fval; 
			break;
		}
	}

	if(editvalue > maxinches_xpos)
	{
		editvalue = maxinches_xpos;
	}
	else if(editvalue < 0)
	{
		editvalue = 0;
	}
	else{}

	m_scroll_xpos.SetScrollPos(editvalue);
	m_papermap[UDSCAP_XPOS] = fval;

	m_edit_xpos.SetSel(str.GetLength(), str.GetLength(),TRUE);  // ���ñ༭��ؼ���Χ

	UpdateData(FALSE);  // ���¿ؼ�
}


void CPage_Paper::OnEnChangeBase_Edit_EdgeYpos()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д __super::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);  // ��������
	CString str;
	m_edit_ypos.GetWindowText(str);
	float fval = _ttof(str);
	
	int nIndex = m_combo_uints.GetCurSel();
	float editvalue;
	switch(FindUnit(nIndex))
	{
	case TWUN_INCHES:
	case TWUN_CENTIMETERS:
		{		
			editvalue = fval * 100;  //��Ҫ����100.		
			break;
		}
	case TWUN_PIXELS:
		{
			editvalue = fval; 
			break;
		}
	}

	if(editvalue > maxinches_ypos)
	{
		editvalue = maxinches_ypos;
	}
	else if(editvalue < 0)
	{
		editvalue = 0;
	}
	else{}

	m_scroll_ypos.SetScrollPos(editvalue);
	m_papermap[UDSCAP_YPOS] = fval;

	m_edit_ypos.SetSel(str.GetLength(), str.GetLength(),TRUE);  // ���ñ༭��ؼ���Χ

	UpdateData(FALSE);  // ���¿ؼ�
}

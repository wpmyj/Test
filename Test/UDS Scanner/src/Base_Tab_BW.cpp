// Base_Tab_BW.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "UDS Scanner.h"
#include "Base_Tab_BW.h"
#include "afxdialogex.h"


// CBase_Tab_BW �Ի���

IMPLEMENT_DYNAMIC(CBase_Tab_BW, CPropertyPage)

CBase_Tab_BW::CBase_Tab_BW(MFC_UI *pUI)
	: m_pUI(pUI),CPropertyPage(CBase_Tab_BW::IDD)
{
}

CBase_Tab_BW::~CBase_Tab_BW()
{
}

void CBase_Tab_BW::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TABBW_COMBO_RESOLUTION, m_combo_resolution);
	DDX_Control(pDX, IDC_TABBW_SLIDER_BRIGHTNESS, m_slider_brightness);
	DDX_Control(pDX, IDC_TABBW_EDIT_THRESHOLD, m_edit_threshold);
	DDX_Control(pDX, IDC_TABBW_EDIT_BRIGHTNESS, m_edit_brightness);
	DDX_Control(pDX, IDC_TABBW_SLIDER_THRESHOLD, m_slider_threshold);
	DDX_Control(pDX, IDC_TABBW_COMBO_BINARIZATION, m_combo_binarization);
	DDX_Control(pDX, IDC_TABBW_EDIT_NOISENUM, m_edit_noisenum);
	DDX_Control(pDX, IDC_TABBW_EDIT_NOISERANGE, m_edit_noiserange);
	DDX_Control(pDX, IDC_TABBW_SCROLLBAR_NOISENUM, m_scroll_noisenum);
	DDX_Control(pDX, IDC_TABBW_SCROLLBAR_NOISERANGE, m_scroll_noiserange);
	DDX_Radio(pDX, IDC_TABBW_RADIO_COMPRESS_AUTO, m_radio_compress);
	DDX_Control(pDX, IDC_TABBW_COMBO_COLOR, m_combo_filtercolor);
	DDX_Control(pDX, IDC_TABBW_COMBO_FILTERMODE, m_combo_filtermode);
	DDX_Control(pDX, IDC_TABBW_EDIT_FILTERLEVEL, m_edit_filterlevel);
	DDX_Control(pDX, IDC_TABBW_SLIDER_FILTERLEVEL, m_slider_filterlevel);
	DDX_Control(pDX, IDC_TABGRAY_COMBO_COMPRESSQUALITY, m_combo_compressquality);
}


BEGIN_MESSAGE_MAP(CBase_Tab_BW, CPropertyPage)
	ON_CBN_SELCHANGE(IDC_TABBW_COMBO_RESOLUTION, &CBase_Tab_BW::OnCbnSelchangeTabbw_Combo_Resolution)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_TABBW_SLIDER_BRIGHTNESS, &CBase_Tab_BW::OnNMCustomdrawTabbw_Slider_Brightness)
	ON_EN_CHANGE(IDC_TABBW_EDIT_BRIGHTNESS, &CBase_Tab_BW::OnEnChangeTabbw_Edit_Brightness)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_TABBW_SLIDER_THRESHOLD, &CBase_Tab_BW::OnNMCustomdrawTabbw_Slider_Threshold)
	ON_EN_CHANGE(IDC_TABBW_EDIT_THRESHOLD, &CBase_Tab_BW::OnEnChangeTabbw_Edit_Threshold)
	ON_CBN_SELCHANGE(IDC_TABBW_COMBO_BINARIZATION, &CBase_Tab_BW::OnCbnSelchangeTabbw_Combo_Binarization)
	ON_EN_CHANGE(IDC_TABBW_EDIT_NOISENUM, &CBase_Tab_BW::OnEnChangeTabbw_Edit_NoiseNum)
	ON_EN_CHANGE(IDC_TABBW_EDIT_NOISERANGE, &CBase_Tab_BW::OnEnChangeTabbw_Edit_NoiseRange)
	ON_WM_VSCROLL()
	ON_BN_CLICKED(IDC_TABBW_RADIO_COMPRESS_AUTO, &CBase_Tab_BW::OnTabBW_RadioBtn_Compress)
	ON_BN_CLICKED(IDC_TABBW_RADIO_COMPRESS_G4, &CBase_Tab_BW::OnTabBW_RadioBtn_Compress)
	ON_CBN_SELCHANGE(IDC_TABBW_COMBO_COLOR, &CBase_Tab_BW::OnCbnSelchangeTabBW_Combo_FilterColor)
	ON_CBN_SELCHANGE(IDC_TABBW_COMBO_FILTERMODE, &CBase_Tab_BW::OnCbnSelchangeTabBW_Combo_FilterMode)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_TABBW_SLIDER_FILTERLEVEL, &CBase_Tab_BW::OnNMCustomdrawTabBW_Slider_Filterlevel)
	ON_EN_CHANGE(IDC_TABBW_EDIT_FILTERLEVEL, &CBase_Tab_BW::OnEnChangeTabBW_Edit_Filterlevel)
	ON_CBN_SELCHANGE(IDC_TABGRAY_COMBO_COMPRESSQUALITY, &CBase_Tab_BW::OnCbnSelchangeTabBW_Combo_Compressquality)
END_MESSAGE_MAP()


// CBase_Tab_BW ��Ϣ�������

void CBase_Tab_BW::UpdateControls(void)
{
	UpdateData(TRUE);
	int nCapIndex;
	const IntVector* lstCapValues;
	const FloatVector* lstCapValuesFlt;
	float fCapValue;
	int nCapValue;
	CString strText;
	int nval;

	//���������Ĭ�ϲ�ʹ��
	int MultiCapValue = (int)(m_pUI->GetCapValueBool(UDSCAP_MULTISTREAM));

	//ȥ������-������Ŀ
	fCapValue = m_pUI->GetCapValueFloat(UDSCAP_NOISENUM);
	strText.Format("%d",(int)fCapValue);
	SetDlgItemText(IDC_TABBW_EDIT_NOISENUM,strText);

	//ȥ������-������Χ
	fCapValue = m_pUI->GetCapValueFloat(UDSCAP_NOISERANGE);
	strText.Format("%d",(int)fCapValue);
	SetDlgItemText(IDC_TABBW_EDIT_NOISERANGE,strText);

	// �ֱ���
	m_combo_resolution.ResetContent();
	lstCapValuesFlt = m_pUI->GetValidCapFloat(ICAP_XRESOLUTION);
	for(unsigned int i=0; i<lstCapValuesFlt->size();i++)
	{
		CString strTemp;
		strTemp.Format("%d",(int)lstCapValuesFlt->at(i));
		m_combo_resolution.InsertString(i,strTemp);
	}
	if(MultiCapValue == 0) //����δѡ��
	{
		nCapIndex = m_pUI->GetCurrentCapIndex(ICAP_XRESOLUTION);
	}
	else
	{
		switch(basebutton)
		{
		case 0: //����
			nCapIndex = m_pUI->GetCurrentCapIndex(UDSCAP_RESOLUTIONFB);
			break;
		case 1: //����
			nCapIndex = m_pUI->GetCurrentCapIndex(UDSCAP_RESOLUTIONBB);
			break;
		}
	}
	m_combo_resolution.SetCurSel(nCapIndex);
	nval = (int)lstCapValuesFlt->at(nCapIndex);

	if(MultiCapValue == 0) //����δѡ��
	{
		// ���� 
		nCapValue = (int)(m_pUI->GetCapValueFloat(ICAP_BRIGHTNESS));
	}
	else
	{
		switch(basebutton)
		{
		case 0: //����
			nCapValue = (int)(m_pUI->GetCapValueFloat(UDSCAP_BRIGHTNESSFB));
			break;
		case 1: //����
			nCapValue = (int)(m_pUI->GetCapValueFloat(UDSCAP_BRIGHTNESSBB));
			break;
		}
	}
	m_slider_brightness.SetPos(nCapValue);
	strText.Format("%d",nCapValue);
	SetDlgItemText(IDC_TABBW_EDIT_BRIGHTNESS,strText);

	//��ֵ��
	m_combo_binarization.ResetContent(); //�������
	lstCapValues = m_pUI->GetValidCap(UDSCAP_BINARIZATION);
	for(unsigned int i=0; i<lstCapValues->size();i++)
	{
		switch(lstCapValues->at(i))
		{
		case TWBZ_DYNATHRESHOLD:
			m_combo_binarization.InsertString(i,"��̬��ֵ");
			break;
		case TWBZ_FIXEDTHRESHOLD:
			m_combo_binarization.InsertString(i,"�̶���ֵ"); 
			break;
		case TWBZ_HALFTONE1:
			m_combo_binarization.InsertString(i,"��ɫ��1");
			break;
		case TWBZ_HALFTONE2:
			m_combo_binarization.InsertString(i,"��ɫ��2");
			break;
		case TWBZ_HALFTONE3:
			m_combo_binarization.InsertString(i,"��ɫ��3");
			break;
		case TWBZ_HALFTONE4:
			m_combo_binarization.InsertString(i,"��ɫ��4");
			break;
		case TWBZ_HALFTONE5:
			m_combo_binarization.InsertString(i,"��ɫ��5");
			break;
		case TWBZ_ERRORDIFF:
			m_combo_binarization.InsertString(i,"�����ɢ");
			break;
		default:
			continue;
		}
	}
	if(MultiCapValue == 0) //����δѡ��
	{
		nCapIndex = m_pUI->GetCurrentCapIndex(UDSCAP_BINARIZATION);
	}
	else
	{
		switch(basebutton)
		{
		case 0: //����
			nCapIndex = m_pUI->GetCurrentCapIndex(UDSCAP_BINARIZATIONFB);
			break;
		case 1: //����
			nCapIndex = m_pUI->GetCurrentCapIndex(UDSCAP_BINARIZATIONBB);
			break;
		}
	}
	m_combo_binarization.SetCurSel(nCapIndex);
	SetBinarization();

	//��ֵ
	CString str;
	GetDlgItemText(IDC_BASETABBW_STATIC_THRESHOLD,str);
	if(str.Find("ȥ���ߵ�") >= 0)
	{
		if(MultiCapValue == 0) //����δѡ��
		{
			//�������-ȥ���ߵ� 
			nCapValue = (int)(m_pUI->GetCapValueFloat(UDSCAP_REMOVESPOTS)); 
		}
		else
		{
			switch(basebutton)
			{
			case 0: //����
				nCapValue = (int)(m_pUI->GetCapValueFloat(UDSCAP_REMOVESPOTSFB)); 
				break;
			case 1: //����
				nCapValue = (int)(m_pUI->GetCapValueFloat(UDSCAP_REMOVESPOTSBB)); 
				break;
			}
		}
	}
	else if(str.Find("��ֵ") >= 0)
	{
		//�������-��ɫ����  ���ֵ����ֵ��ͬ�������� 
		if(MultiCapValue == 0) //����δѡ��
		{
			//�������-ȥ���ߵ� 
			nCapValue = (int)(m_pUI->GetCapValueFloat(ICAP_THRESHOLD));
		}
		else
		{
			switch(basebutton)
			{
			case 0: //����
				nCapValue = (int)(m_pUI->GetCapValueFloat(UDSCAP_THRESHOLDFB)); 
				break;
			case 1: //����
				nCapValue = (int)(m_pUI->GetCapValueFloat(UDSCAP_THRESHOLDBB)); 
				break;
			}
		}
	}
	else{}//���뱣��
	m_slider_threshold.SetPos(nCapValue);
	strText.Format("%d",nCapValue);
	SetDlgItemText(IDC_TABBW_EDIT_THRESHOLD, strText);

	//�˳���ɫ
	m_combo_filtercolor.ResetContent(); 
	nCapIndex = m_pUI->GetCurrentCapIndex(UDSCAP_FILTERCOLOR);
	lstCapValues = m_pUI->GetValidCap(UDSCAP_FILTERCOLOR);
	for(unsigned int i=0; i<lstCapValues->size();i++)
	{
		switch(lstCapValues->at(i))
		{
		case TWFL_NONE:
			m_combo_filtercolor.InsertString(i,"���˳�"); 
			break;
		case TWFL_RED:
			m_combo_filtercolor.InsertString(i,"��ɫ"); 
			break;
		case TWFL_GREEN:
			m_combo_filtercolor.InsertString(i,"��ɫ"); 
			break;
		case TWFL_BLUE:
			m_combo_filtercolor.InsertString(i,"��ɫ"); 
			break;
		default:
			continue;
		}
	}
	m_combo_filtercolor.SetCurSel(nCapIndex);

	//�˳�ģʽ
	m_combo_filtermode.ResetContent(); 
	nCapIndex = m_pUI->GetCurrentCapIndex(UDSCAP_FILTERMODE);
	lstCapValues = m_pUI->GetValidCap(UDSCAP_FILTERMODE);
	for(unsigned int i=0; i<lstCapValues->size();i++)
	{
		switch(lstCapValues->at(i))
		{
		case TWFL_NONE:
			m_combo_filtermode.InsertString(i,"�Զ�"); 
			break;
		case TWFL_RED:
			m_combo_filtermode.InsertString(i,"�Զ���"); 
			break;
		default:
			continue;
		}
	}
	m_combo_filtermode.SetCurSel(nCapIndex);

	//�˳��̶�
	nCapValue = (int)m_pUI->GetCapValueFloat(UDSCAP_FILTERLEVEL);
	m_slider_filterlevel.SetPos(nCapValue);
	strText.Format("%d",nCapValue);
	SetDlgItemText(IDC_TABBW_EDIT_FILTERLEVEL,strText);

	// ѹ������ 
	m_combo_compressquality.ResetContent();
	lstCapValues = m_pUI->GetValidCap(UDSCAP_COMPRESSQUALITY);
	for(unsigned int i=0; i<lstCapValues->size();i++)
	{
		switch(lstCapValues->at(i))
		{
		case TWCQ_BEST:
			m_combo_compressquality.InsertString(i,"���");
			break;
		case TWCQ_FINE:
			m_combo_compressquality.InsertString(i,"����");
			break;
		case TWCQ_JUST:
			m_combo_compressquality.InsertString(i,"һ��");
			break;
		case TWCQ_DEFINED:
			m_combo_compressquality.InsertString(i,"�Զ���");
			break;
		default:
			continue;
		}
	}
	if(MultiCapValue == 0) //����δѡ��
	{
		nCapIndex = m_pUI->GetCurrentCapIndex(UDSCAP_COMPRESSQUALITY);
	}
	else
	{
		switch(basebutton)
		{
		case 0: //����
			nCapIndex = m_pUI->GetCurrentCapIndex(UDSCAP_COMPRESSQUALITYFB);
			break;
		case 1: //����
			nCapIndex = m_pUI->GetCurrentCapIndex(UDSCAP_COMPRESSQUALITYBB);
			break;
		}
	}
	m_combo_compressquality.SetCurSel(nCapIndex);

	//ѹ��
	if(MultiCapValue == 0) //����δѡ��
	{
		nCapIndex = m_pUI->GetCurrentCapIndex(ICAP_COMPRESSION);
	}
	else
	{
		switch(basebutton)
		{
		case 0: //����
			nCapIndex = m_pUI->GetCurrentCapIndex(UDSCAP_COMPRESSIONFB);	
			break;
		case 1: //����
			nCapIndex = m_pUI->GetCurrentCapIndex(UDSCAP_COMPRESSIONBB);
			break;
		}
	}	
	if(nCapIndex==2)
	{
		m_radio_compress = nCapIndex - 1; //0Ϊ�Զ���1ΪJPEG,2ΪG4
	}	
	else
	{
		m_radio_compress = nCapIndex; //0Ϊ�Զ���1ΪJPEG��2ΪG4
	}
	UpdateData(FALSE);
}


BOOL CBase_Tab_BW::OnInitDialog()
{
	__super::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��	
	InitSliderCtrl();
	UpdateControls();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CBase_Tab_BW::InitSliderCtrl()
{
	float fMin,fMax,fStep;

	m_pUI->GetCapRangeFloat(ICAP_BRIGHTNESS, fMin, fMax, fStep);
	m_slider_brightness.SetRange((int)fMin, (int)fMax);
	m_slider_brightness.SetTicFreq((int)fStep);

	m_pUI->GetCapRangeFloat(ICAP_THRESHOLD, fMin, fMax, fStep);
	m_slider_threshold.SetTicFreq((int)fStep); 

	m_pUI->GetCapRangeFloat(UDSCAP_FILTERLEVEL, fMin, fMax, fStep);
	m_slider_filterlevel.SetRange((int)fMin, (int)fMax);
	m_slider_filterlevel.SetTicFreq((int)fStep); //����
}

void CBase_Tab_BW::OnCbnSelchangeTabbw_Combo_Resolution()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nIndex = m_combo_resolution.GetCurSel();
	CString strCBText; 
	m_combo_resolution.GetLBText(nIndex, strCBText);
	int nval = _ttoi(strCBText);  // CString ת int
	
	int MultiCapValue = (int)(m_pUI->GetCapValueBool(UDSCAP_MULTISTREAM));
	if(MultiCapValue == 0) //����δѡ��
	{
		m_pUI->SetCapValueInt(ICAP_XRESOLUTION, nval); 
		m_pUI->SetCapValueInt(ICAP_YRESOLUTION, nval); 
	}
	else
	{
		switch(basebutton)
		{
		case 0: //����
			m_pUI->SetCapValueInt(UDSCAP_RESOLUTIONFB, nval); 
			break;
		case 1: //����
			m_pUI->SetCapValueInt(UDSCAP_RESOLUTIONBB, nval); 
			break;
		}
	}

	m_combo_resolution.SetCurSel(nIndex);
}


void CBase_Tab_BW::OnNMCustomdrawTabbw_Slider_Brightness(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);  // ��������
	CString str;
	int sldValue = m_slider_brightness.GetPos();  // ��ȡ���鵱ǰλ��
	
	int nCapValue = (int)(m_pUI->GetCapValueBool(UDSCAP_MULTISTREAM));
	if(nCapValue == 0) //����δѡ��
	{
		m_pUI->SetCapValueFloat(ICAP_BRIGHTNESS, (float)sldValue);
	}
	else
	{
		switch(basebutton)
		{
		case 0:
			m_pUI->SetCapValueFloat(UDSCAP_BRIGHTNESSFB, (float)sldValue);
			break;
		case 1:
			m_pUI->SetCapValueFloat(UDSCAP_BRIGHTNESSBB, (float)sldValue);
			break;
		}
	}
	str.Format("%d", sldValue);
	SetDlgItemText(IDC_TABBW_EDIT_BRIGHTNESS,str);

	UpdateData(FALSE);  // ���¿ؼ�
	*pResult = 0;
}


void CBase_Tab_BW::OnEnChangeTabbw_Edit_Brightness()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д __super::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);  // ��������
	CString str;
	m_edit_brightness.GetWindowText(str);
	int nval = _ttoi(str);
	m_slider_brightness.SetPos(nval);
	
	int nCapValue = (int)(m_pUI->GetCapValueBool(UDSCAP_MULTISTREAM));
	if(nCapValue == 0) //����δѡ��
	{
		m_pUI->SetCapValueFloat(ICAP_BRIGHTNESS, (float)nval);
	}
	else
	{
		switch(basebutton)
		{
		case 0:
			m_pUI->SetCapValueFloat(UDSCAP_BRIGHTNESSFB, (float)nval);
			break;
		case 1:
			m_pUI->SetCapValueFloat(UDSCAP_BRIGHTNESSBB, (float)nval);
			break;
		}
	}

	m_edit_brightness.SetSel(str.GetLength(), str.GetLength(),TRUE);  // ���ñ༭��ؼ���Χ

	UpdateData(FALSE);  // ���¿ؼ�
}


void CBase_Tab_BW::OnNMCustomdrawTabbw_Slider_Threshold(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE); //��������
	CString str;
	int sldValue = m_slider_threshold.GetPos(); //��ȡ����ĵ�ǰλ��

	int MultiCapValue = (int)(m_pUI->GetCapValueBool(UDSCAP_MULTISTREAM));
	CString strStatic;
	GetDlgItemText(IDC_BASETABBW_STATIC_THRESHOLD,strStatic); 
	if(strStatic.Find("ȥ���ߵ�") >= 0)
	{
		if(MultiCapValue == 0) //����δѡ��
		{
			m_pUI->SetCapValueFloat(UDSCAP_REMOVESPOTS, (float)sldValue);
		}
		else
		{
			switch(basebutton)
			{
			case 0: //����
				m_pUI->SetCapValueFloat(UDSCAP_REMOVESPOTSFB, (float)sldValue); 
				break;
			case 1: //���� 
				m_pUI->SetCapValueFloat(UDSCAP_REMOVESPOTSBB, (float)sldValue); 
				break;
			}
		}
	}
	else if(strStatic.Find("��ֵ") >= 0)
	{
		//�������-��ɫ����  ���ֵ����ֵ��ͬ�������� 
		if(MultiCapValue == 0) //����δѡ��
		{
			m_pUI->SetCapValueFloat(ICAP_THRESHOLD, (float)sldValue);
		}
		else
		{
			switch(basebutton)
			{
			case 0: //���� 
				m_pUI->SetCapValueFloat(UDSCAP_THRESHOLDFB, (float)sldValue);
				break;
			case 1: //����
				m_pUI->SetCapValueFloat(UDSCAP_THRESHOLDBB, (float)sldValue);
				break;
			}
		}
	}
	else{}//���뱣��

	str.Format("%d", sldValue);
	SetDlgItemText(IDC_TABBW_EDIT_THRESHOLD, str);
	UpdateData(FALSE); //���¿ؼ���

	*pResult = 0;
}


void CBase_Tab_BW::OnEnChangeTabbw_Edit_Threshold()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д __super::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE); //��������
	CString str;
	m_edit_threshold.GetWindowText(str);
	int nval = _ttoi(str);
	m_slider_threshold.SetPos(nval); //_ttoi��CString����ת��Ϊint

	int MultiCapValue = (int)(m_pUI->GetCapValueBool(UDSCAP_MULTISTREAM));
	CString strStatic;
	GetDlgItemText(IDC_BASETABBW_STATIC_THRESHOLD,strStatic);
	if(strStatic.Find("ȥ���ߵ�") >= 0)
	{
		if(MultiCapValue == 0) //����δѡ��
		{
			m_pUI->SetCapValueFloat(UDSCAP_REMOVESPOTS,(float)nval);  // ������ֵΪ��ǰ������ֵ
		}
		else
		{
			switch(basebutton)
			{
			case 0: //����
				m_pUI->SetCapValueFloat(UDSCAP_REMOVESPOTSFB,(float)nval);
				break;
			case 1: //���� 
				m_pUI->SetCapValueFloat(UDSCAP_REMOVESPOTSBB,(float)nval);
				break;
			}
		}
	}
	else if(strStatic.Find("��ֵ") >= 0)
	{
		//�������-��ɫ����  ���ֵ����ֵ��ͬ�������� 
		if(MultiCapValue == 0) //����δѡ��
		{
			m_pUI->SetCapValueFloat(ICAP_THRESHOLD,(float)nval);
		}
		else
		{
			switch(basebutton)
			{
			case 0: //���� 
				m_pUI->SetCapValueFloat(UDSCAP_THRESHOLDFB,(float)nval);
				break;
			case 1: //����
				m_pUI->SetCapValueFloat(UDSCAP_THRESHOLDBB,(float)nval);
				break;
			}
		}
	}
	else{}//���뱣��

	m_edit_threshold.SetSel(str.GetLength(), str.GetLength(), TRUE); //���ñ༭��ؼ���Χ

	UpdateData(FALSE); //���¿ؼ�
}


void CBase_Tab_BW::OnCbnSelchangeTabbw_Combo_Binarization() 
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE); //��������
	int nIndex = m_combo_binarization.GetCurSel();
	CString strCBText; 
	m_combo_binarization.GetLBText( nIndex, strCBText);
	int nval;
	if (strCBText.Find("��̬��ֵ") >= 0)
	{
		nval = TWBZ_DYNATHRESHOLD;
	}
	else if(strCBText.Find("�̶���ֵ") >= 0)
	{
		nval = TWBZ_FIXEDTHRESHOLD; 
	}
	else if(strCBText.Find("��ɫ��1") >= 0)
	{
		nval = TWBZ_HALFTONE1; 
	}
	else if(strCBText.Find("��ɫ��2") >= 0)
	{
		nval = TWBZ_HALFTONE2; 
	}
	else if(strCBText.Find("��ɫ��3") >= 0)
	{
		nval = TWBZ_HALFTONE3; 
	}
	else if(strCBText.Find("��ɫ��4") >= 0)
	{
		nval = TWBZ_HALFTONE4; 
	}
	else if(strCBText.Find("��ɫ��5") >= 0)
	{
		nval = TWBZ_HALFTONE5; 
	}
	else if(strCBText.Find("�����ɢ") >= 0)
	{
		nval = TWBZ_ERRORDIFF; 
	}
	else
	{}
	
	int MultiCapValue = (int)(m_pUI->GetCapValueBool(UDSCAP_MULTISTREAM));
	if(MultiCapValue == 0) //����δѡ��
	{ 
		m_pUI->SetCapValueInt(UDSCAP_BINARIZATION, nval); 
	}
	else
	{
		switch(basebutton)
		{
		case 0: //����
			m_pUI->SetCapValueInt(UDSCAP_BINARIZATIONFB, nval); 
			break;
		case 1: //����
			m_pUI->SetCapValueInt(UDSCAP_BINARIZATIONBB, nval); 
			break;
		}
	}

	SetBinarization();
	m_combo_binarization.SetCurSel(nIndex);
	UpdateData(FALSE); //���¿ؼ�
}


void CBase_Tab_BW::SetBinarization(void)
{
	int nCapValue;
	float fMin,fMax,fStep;

	int MultiCapValue = (int)(m_pUI->GetCapValueBool(UDSCAP_MULTISTREAM));

	//if(m_radiobtn_duplex == 0 || m_radiobtn_duplex == 1 || m_check_frontbw.GetCheck() || m_check_backbw.GetCheck())
	{
	//	m_slider_threshold.EnableWindow(TRUE);
	//	m_edit_threshold.EnableWindow(TRUE); 

		CString strCBText;
		GetDlgItemText(IDC_TABBW_COMBO_BINARIZATION,strCBText);
		if (strCBText.Find("��̬��ֵ") >= 0)
		{
			//���ô�ʱ���Ȳ�����
			m_slider_brightness.EnableWindow(FALSE);
			m_edit_brightness.EnableWindow(FALSE); 

			m_slider_threshold.EnableWindow(TRUE);
			m_edit_threshold.EnableWindow(TRUE);

			SetDlgItemText(IDC_BASETABBW_STATIC_THRESHOLD, TEXT("ȥ���ߵ�:"));
			m_pUI->GetCapRangeFloat(UDSCAP_REMOVESPOTS, fMin, fMax, fStep);
			m_slider_threshold.SetRange((int)fMin, (int)fMax);

			if(MultiCapValue == 0) //����δѡ��
			{ 
				nCapValue = (int)(m_pUI->GetCapValueFloat(UDSCAP_REMOVESPOTS)); 
			}
			else
			{
				switch(basebutton)
				{
				case 0: //����
					nCapValue = (int)(m_pUI->GetCapValueFloat(UDSCAP_REMOVESPOTSFB)); 
					break;
				case 1: //����
					nCapValue = (int)(m_pUI->GetCapValueFloat(UDSCAP_REMOVESPOTSBB)); 
					break;
				}
			}
			m_slider_threshold.SetPos(nCapValue);
		} 
		else if(strCBText.Find("�̶���ֵ") >= 0)
		{
			m_slider_threshold.EnableWindow(TRUE);
			m_edit_threshold.EnableWindow(TRUE);
			//���ô�ʱ���ȿ���
			m_slider_brightness.EnableWindow(TRUE);
			m_edit_brightness.EnableWindow(TRUE);
			
			SetDlgItemText(IDC_BASETABBW_STATIC_THRESHOLD, TEXT("��ֵ:"));
			m_pUI->GetCapRangeFloat(ICAP_THRESHOLD, fMin, fMax, fStep);
			m_slider_threshold.SetRange((int)fMin, (int)fMax);

			if(MultiCapValue == 0) //����δѡ��
			{  
				nCapValue = (int)(m_pUI->GetCapValueFloat(ICAP_THRESHOLD));
			}
			else
			{
				switch(basebutton)
				{
				case 0: //����
					nCapValue = (int)(m_pUI->GetCapValueFloat(UDSCAP_THRESHOLDFB)); 
					break;
				case 1: //����
					nCapValue = (int)(m_pUI->GetCapValueFloat(UDSCAP_THRESHOLDBB)); 
					break;
				}
			}
			m_slider_threshold.SetPos(nCapValue);
		}
		else if(strCBText.Find("��ɫ��") >= 0 || strCBText.Find("�����ɢ") >= 0)
		{
			m_slider_threshold.EnableWindow(FALSE);
			m_edit_threshold.EnableWindow(FALSE);

			//���ô�ʱ���ȿ���
			m_slider_brightness.EnableWindow(TRUE);
			m_edit_brightness.EnableWindow(TRUE);
		}
	}
	//else
	{
		//m_slider_threshold.EnableWindow(FALSE);
		//m_edit_threshold.EnableWindow(FALSE);
	}
}


void CBase_Tab_BW::OnEnChangeTabbw_Edit_NoiseNum()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д __super::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);  // ��������
	SetScroll();

	CString str;
	m_edit_noisenum.GetWindowText(str);
	float fval = _ttof(str);
	m_scroll_noisenum.SetScrollPos((int)fval);

	m_pUI->SetCapValueFloat(UDSCAP_NOISENUM, fval); 
	m_edit_noisenum.SetSel(str.GetLength(), str.GetLength(),TRUE);  // ���ñ༭��ؼ���Χ

	UpdateData(FALSE);  // ���¿ؼ�
}


void CBase_Tab_BW::OnEnChangeTabbw_Edit_NoiseRange()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д __super::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);  // ��������
	SetScroll();

	CString str;
	m_edit_noiserange.GetWindowText(str);
	float fval = _ttof(str);
	m_scroll_noiserange.SetScrollPos((int)fval);

	m_pUI->SetCapValueFloat(UDSCAP_NOISERANGE, fval); 
	m_edit_noiserange.SetSel(str.GetLength(), str.GetLength(),TRUE);  // ���ñ༭��ؼ���Χ

	UpdateData(FALSE);  // ���¿ؼ�
}

void CBase_Tab_BW::SetScroll()
{
	CString str;
	int nval;
	float fMin,fMax,fStep;

	m_pUI->GetCapRangeFloat(UDSCAP_NOISENUM, fMin, fMax, fStep);
	m_scroll_noisenum.SetScrollRange(fMin, fMax); 
	m_edit_noisenum.GetWindowText(str); 
	nval = _ttof(str);
	m_scroll_noisenum.SetScrollPos(nval); 

	m_pUI->GetCapRangeFloat(UDSCAP_NOISERANGE, fMin, fMax, fStep);
	m_scroll_noiserange.SetScrollRange((int)fMin, (int)fMax); 
	m_edit_noiserange.GetWindowText(str); 
	nval = _ttof(str);
	m_scroll_noiserange.SetScrollPos(nval); 
}



void CBase_Tab_BW::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	int scrollpos;
	CString str;
	SetScroll();
	switch(pScrollBar->GetDlgCtrlID())
	{
		//ȥ��������Ŀ               
	case IDC_TABBW_SCROLLBAR_NOISENUM:
		{
			scrollpos = m_scroll_noisenum.GetScrollPos();
			switch(nSBCode)
			{
			case SB_LINEUP: //������Ϲ���һ�У���pos��1
				scrollpos += 1;
				break;
			case SB_LINEDOWN: //������¹���һ�У���pos��1
				scrollpos -= 1; 
				break;
			case SB_TOP: //���
				scrollpos = 3600;
				break;
			case SB_BOTTOM:
				scrollpos = 0;
				break;
			}
			// ���ù�����λ��  
			m_scroll_noisenum.SetScrollPos(scrollpos);
			str.Format("%d", scrollpos);
			SetDlgItemText(IDC_TABBW_EDIT_NOISENUM, str); 
		}
		break;
		//ȥ������-������Χ
	case IDC_TABBW_SCROLLBAR_NOISERANGE:
		{
			scrollpos = m_scroll_noiserange.GetScrollPos();
			switch(nSBCode)
			{
			case SB_LINEUP: //������Ϲ���һ�У���pos��1
				scrollpos += 1;
				break;
			case SB_LINEDOWN: //������¹���һ�У���pos��1
				scrollpos -= 1; 
				break;
			case SB_TOP: //���
				scrollpos = 30;
				break;
			case SB_BOTTOM:
				scrollpos = 1;
				break;
			}
			// ���ù�����λ��  
			m_scroll_noiserange.SetScrollPos(scrollpos);
			str.Format("%d", scrollpos);
			SetDlgItemText(IDC_TABBW_EDIT_NOISERANGE, str); 
		}
	}
	__super::OnVScroll(nSBCode, nPos, pScrollBar);
}


void CBase_Tab_BW::OnTabBW_RadioBtn_Compress()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE); //��radio��״ֵ̬���¸������ı���
	int MultiCapValue = (int)(m_pUI->GetCapValueBool(UDSCAP_MULTISTREAM));
	switch(m_radio_compress)
	{
	case 0:
		if(MultiCapValue == 0) //����δѡ��
		{
			m_pUI->SetCapValueInt(ICAP_COMPRESSION,TWCP_NONE); 
		}
		else
		{
			switch(basebutton)
			{
			case 0: //����
				m_pUI->SetCapValueInt(UDSCAP_COMPRESSIONFB,TWCP_NONE); 
				break;
			case 1: //����
				m_pUI->SetCapValueInt(UDSCAP_COMPRESSIONBB,TWCP_NONE); 
				break;
			}
		}
		
		break;
	case 1:
		if(MultiCapValue == 0) //����δѡ��
		{
			m_pUI->SetCapValueInt(ICAP_COMPRESSION,TWCP_GROUP4); 
		}
		else
		{
			switch(basebutton)
			{
			case 0: //����
				m_pUI->SetCapValueInt(UDSCAP_COMPRESSIONFB,TWCP_GROUP4); 
				break;
			case 1: //����
				m_pUI->SetCapValueInt(UDSCAP_COMPRESSIONBB,TWCP_GROUP4); 
				break;
			}
		}
		break;
	}
	UpdateData(FALSE);
}

void CBase_Tab_BW::OnCbnSelchangeTabBW_Combo_FilterColor()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nIndex = m_combo_filtercolor.GetCurSel();
	CString strCBText; 
	m_combo_filtercolor.GetLBText( nIndex, strCBText);
	int nval;
	if (strCBText.Find("���˳�") >= 0)
	{
		nval = TWFL_NONE;
	}
	else if(strCBText.Find("��ɫ") >= 0)
	{
		nval = TWFL_RED; 
	}
	else if(strCBText.Find("��ɫ") >= 0)
	{
		nval = TWFL_GREEN; 
	}
	else if(strCBText.Find("��ɫ") >= 0)
	{
		nval = TWFL_BLUE; 
	}
	else if(strCBText.Find("�Զ�") >= 0)
	{
		nval = TWFL_AUTO; 
	}
	else
	{}

	m_pUI->SetCapValueInt(UDSCAP_FILTERCOLOR,nval); 
	m_combo_filtercolor.SetCurSel(nIndex);
}


void CBase_Tab_BW::OnCbnSelchangeTabBW_Combo_FilterMode()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nIndex = m_combo_filtermode.GetCurSel();
	CString strCBText; 
	m_combo_filtermode.GetLBText( nIndex, strCBText);
	int nval;
	if (strCBText.Find("�Զ�") >= 0)
	{
		nval = TWFM_AUTO;
	}
	else if(strCBText.Find("�Զ���") >= 0)
	{
		nval = TWFM_DEFINED; 
	}
	else
	{}

	m_pUI->SetCapValueInt(UDSCAP_FILTERMODE,nval); 
	m_combo_filtermode.SetCurSel(nIndex);
}


void CBase_Tab_BW::OnNMCustomdrawTabBW_Slider_Filterlevel(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);  // ��������
	CString str;
	int sldValue = m_slider_filterlevel.GetPos();  // ��ȡ���鵱ǰλ��
	str.Format("%d",sldValue);
	SetDlgItemText(IDC_TABBW_EDIT_FILTERLEVEL, str);// �ڱ༭��ͬ����ʾ������ֵ
	UpdateData(FALSE);  // ���¿ؼ�

	*pResult = 0;
}


void CBase_Tab_BW::OnEnChangeTabBW_Edit_Filterlevel()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д __super::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);  // ��������
	CString str;
	m_edit_filterlevel.GetWindowText(str);
	int nval = _ttoi(str);
	m_slider_filterlevel.SetPos(nval);
	m_pUI->SetCapValueFloat(UDSCAP_FILTERLEVEL,(float)nval);

	m_edit_filterlevel.SetSel(str.GetLength(), str.GetLength(),TRUE);  // ���ñ༭��ؼ���Χ
	UpdateData(FALSE);  // ���¿ؼ�
}


void CBase_Tab_BW::OnCbnSelchangeTabBW_Combo_Compressquality()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	int nIndex = m_combo_compressquality.GetCurSel();
	CString strCBText; 
	m_combo_compressquality.GetLBText( nIndex, strCBText);
	int nval;
	if (strCBText.Find("���") >= 0) 
	{
		nval = TWCQ_BEST;	
	}
	else if(strCBText.Find("����") >= 0) 
	{
		nval = TWCQ_FINE; 
	}
	else if(strCBText.Find("һ��") >= 0) 
	{
		nval = TWCQ_JUST; 
	}
	else if(strCBText.Find("�Զ���") >= 0) 
	{
		nval = TWCQ_DEFINED; 
	}
	else
	{}

	int MultiCapValue = (int)(m_pUI->GetCapValueBool(UDSCAP_MULTISTREAM));
	if(MultiCapValue == 0) //����δѡ��
	{
		m_pUI->SetCapValueInt(UDSCAP_COMPRESSQUALITY,nval); //���ò�����Ч	
	}
	else
	{
		switch(basebutton)
		{
		case 0: //����
			m_pUI->SetCapValueInt(UDSCAP_COMPRESSQUALITYFB, nval); 
			break;
		case 1: //����
			m_pUI->SetCapValueInt(UDSCAP_COMPRESSQUALITYBB, nval); 
			break;
		}
	}
	m_combo_compressquality.SetCurSel(nIndex);
}

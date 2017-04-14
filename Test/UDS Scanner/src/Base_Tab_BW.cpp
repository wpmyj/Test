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
	DDX_Control(pDX, IDC_TABBW_COMBO_COMPRESS, m_combo_compress);
	DDX_Control(pDX, IDC_TABBW_SLIDER_COMPRESSION, m_slider_compressvalue);
	DDX_Control(pDX, IDC_TABBW_EDIT_COMPRESSVALUE, m_edit_compressvalue);
	DDX_Control(pDX, IDC_TABBW_COMBO_RESOLUTION, m_combo_resolution);
	DDX_Control(pDX, IDC_TABBW_SLIDER_BRIGHTNESS, m_slider_brightness);
	DDX_Control(pDX, IDC_TABBW_EDIT_THRESHOLD, m_edit_threshold);
	DDX_Control(pDX, IDC_TABBW_EDIT_BRIGHTNESS, m_edit_brightness);
	DDX_Control(pDX, IDC_TABBW_SLIDER_THRESHOLD, m_slider_threshold);
	DDX_Control(pDX, IDC_TABBW_COMBO_BINARIZATION, m_combo_binarization);
}


BEGIN_MESSAGE_MAP(CBase_Tab_BW, CPropertyPage)
	ON_CBN_SELCHANGE(IDC_TABBW_COMBO_COMPRESS, &CBase_Tab_BW::OnCbnSelchangeTabbw_Combo_Compress)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_TABBW_SLIDER_COMPRESSION, &CBase_Tab_BW::OnNMCustomdrawTabbw_Slider_Compressionvalue)
	ON_EN_CHANGE(IDC_TABBW_EDIT_COMPRESSVALUE, &CBase_Tab_BW::OnEnChangeTabbw_Edit_Compressvalue)
	ON_CBN_SELCHANGE(IDC_TABBW_COMBO_RESOLUTION, &CBase_Tab_BW::OnCbnSelchangeTabbw_Combo_Resolution)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_TABBW_SLIDER_BRIGHTNESS, &CBase_Tab_BW::OnNMCustomdrawTabbw_Slider_Brightness)
	ON_EN_CHANGE(IDC_TABBW_EDIT_BRIGHTNESS, &CBase_Tab_BW::OnEnChangeTabbw_Edit_Brightness)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_TABBW_SLIDER_THRESHOLD, &CBase_Tab_BW::OnNMCustomdrawTabbw_Slider_Threshold)
	ON_EN_CHANGE(IDC_TABBW_EDIT_THRESHOLD, &CBase_Tab_BW::OnEnChangeTabbw_Edit_Threshold)
	ON_CBN_SELCHANGE(IDC_TABBW_COMBO_BINARIZATION, &CBase_Tab_BW::OnCbnSelchangeTabbw_Combo_Binarization)
END_MESSAGE_MAP()


// CBase_Tab_BW ��Ϣ�������

void CBase_Tab_BW::UpdateControls(void)
{
	int nCapIndex;
	const IntVector* lstCapValues;
	const FloatVector* lstCapValuesFlt;
	float fCapValue;
	int nCapValue;
	CString strText;
	int nval;

	//���������Ĭ�ϲ�ʹ��
	int MultiCapValue = (int)(m_pUI->GetCapValueBool(UDSCAP_MULTISTREAM));

	//ѹ��
	m_combo_compress.ResetContent();  // �������
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
	m_combo_compress.SetCurSel(nCapIndex);

	// ѹ���� 
	if(MultiCapValue == 0) //����δѡ��
	{
		nCapValue = (int)(m_pUI->GetCapValueFloat(UDSCAP_COMPRESSVALUE)); 
	}
	else
	{
		switch(basebutton)
		{
		case 0: //����
			nCapValue = (int)(m_pUI->GetCapValueFloat(UDSCAP_COMPRESSIONVALUEFB)); 
			break;
		case 1: //����
			nCapValue = (int)(m_pUI->GetCapValueFloat(UDSCAP_COMPRESSIONVALUEBB));
			break;
		}
	}	
	m_slider_compressvalue.SetPos(nCapValue);
	strText.Format("%d",nCapValue);
	SetDlgItemText(IDC_TABBW_EDIT_COMPRESSVALUE,strText);

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
	m_pUI->GetCapRangeFloat(UDSCAP_COMPRESSVALUE, fMin, fMax, fStep);
	m_slider_compressvalue.SetRange((int)fMin, (int)fMax);
	m_slider_compressvalue.SetTicFreq((int)fStep);

	m_pUI->GetCapRangeFloat(ICAP_BRIGHTNESS, fMin, fMax, fStep);
	m_slider_brightness.SetRange((int)fMin, (int)fMax);
	m_slider_brightness.SetTicFreq((int)fStep);

	m_pUI->GetCapRangeFloat(ICAP_THRESHOLD, fMin, fMax, fStep);
	m_slider_threshold.SetTicFreq((int)fStep); 
}


void CBase_Tab_BW::OnCbnSelchangeTabbw_Combo_Compress()
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

	int MultiCapValue = (int)(m_pUI->GetCapValueBool(UDSCAP_MULTISTREAM));
	if(MultiCapValue == 0) //����δѡ��
	{ 
		m_pUI->SetCapValueInt(ICAP_COMPRESSION, nval); 
	}
	else
	{
		switch(basebutton)
		{
		case 0: //����
			m_pUI->SetCapValueInt(UDSCAP_COMPRESSIONFB, nval); 
			break;
		case 1: //����
			m_pUI->SetCapValueInt(UDSCAP_COMPRESSIONBB, nval); 
			break;
		}
	}
	m_combo_compress.SetCurSel(nIndex);
}


void CBase_Tab_BW::OnNMCustomdrawTabbw_Slider_Compressionvalue(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);  // ��������
	CString str;
	int sldValue = m_slider_compressvalue.GetPos();  // ��ȡ���鵱ǰλ��

	int nCapValue = (int)(m_pUI->GetCapValueBool(UDSCAP_MULTISTREAM));
	if(nCapValue == 0) //����δѡ��
	{
		m_pUI->SetCapValueInt(UDSCAP_COMPRESSVALUE, sldValue); 
	}
	else
	{
		switch(basebutton)
		{
		case 0:
			m_pUI->SetCapValueInt(UDSCAP_COMPRESSIONVALUEFB, sldValue);
			break;
		case 1:
			m_pUI->SetCapValueInt(UDSCAP_COMPRESSIONVALUEBB, sldValue);
			break;
		}
	}

	str.Format("%d", sldValue);
	SetDlgItemText(IDC_TABBW_EDIT_COMPRESSVALUE, str);

	*pResult = 0;
}


void CBase_Tab_BW::OnEnChangeTabbw_Edit_Compressvalue()
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

	int nCapValue = (int)(m_pUI->GetCapValueBool(UDSCAP_MULTISTREAM));
	if(nCapValue == 0) //����δѡ��
	{
		m_pUI->SetCapValueFloat(UDSCAP_COMPRESSVALUE, (float)nval);
	}
	else
	{
		switch(basebutton)
		{
		case 0:
			m_pUI->SetCapValueFloat(UDSCAP_COMPRESSIONVALUEFB, (float)nval);
			break;
		case 1:
			m_pUI->SetCapValueFloat(UDSCAP_COMPRESSIONVALUEBB, (float)nval);
			break;
		}
	}

	m_edit_compressvalue.SetSel(str.GetLength(), str.GetLength(),TRUE);  // ���ñ༭��ؼ���Χ

	UpdateData(FALSE);  // ���¿ؼ�
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

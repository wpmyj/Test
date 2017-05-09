// Base_Tab_Color.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "UDS Scanner.h"
#include "Base_Tab_Color.h"
#include "afxdialogex.h"

extern int basebright;
// CBase_Tab_Color �Ի���

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
	DDX_Control(pDX, IDC_TABCOLOR_COMBO_COMPRESSQUALITY, m_combo_compressquality);
	DDX_Control(pDX, IDC_TABCOLOR_COMBO_COLOROPTI, m_combo_colorcorrect);
	DDX_Control(pDX, IDC_BASETAB_COLOR_CHECK_NATIVESAVE, m_check_nativesave);
}


BEGIN_MESSAGE_MAP(CBase_Tab_Color, CDialogEx)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_TABCOLOR_SLIDER_COMPRESSION, &CBase_Tab_Color::OnNMCustomdrawTabcolor_Slider_Compressionvalue)
	ON_EN_CHANGE(IDC_TABCOLOR_EDIT_COMPRESSVALUE, &CBase_Tab_Color::OnEnChangeTabcolor_Edit_Compressvalue)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_TABCOLOR_SLIDER_CONTRAST, &CBase_Tab_Color::OnNMCustomdrawTabcolor_Slider_Contrast)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_TABCOLOR_SLIDER_BRIGHTNESS, &CBase_Tab_Color::OnNMCustomdrawTabcolor_Slider_Brightness)
	ON_EN_CHANGE(IDC_TABCOLOR_EDIT_CONTRAST, &CBase_Tab_Color::OnEnChangeTabcolor_Edit_Contrast)
	ON_EN_CHANGE(IDC_TABCOLOR_EDIT_BRIGHTNESS, &CBase_Tab_Color::OnEnChangeTabcolor_Edit_Brightness)
	ON_CBN_SELCHANGE(IDC_TABCOLOR_COMBO_RESOLUTION, &CBase_Tab_Color::OnCbnSelchangeTabcolor_Combo_Resolution)
	ON_CBN_SELCHANGE(IDC_TABCOLOR_COMBO_COMPRESSQUALITY, &CBase_Tab_Color::OnCbnSelchangeTabcolor_Combo_Compressquality)
	ON_CBN_SELCHANGE(IDC_TABCOLOR_COMBO_COLOROPTI, &CBase_Tab_Color::OnCbnSelchangeBase_ComboColoropti)
	ON_BN_CLICKED(IDC_BASETAB_COLOR_CHECK_NATIVESAVE, &CBase_Tab_Color::OnBaseTab_Color_Btn_Check_Nativesave)
END_MESSAGE_MAP()


// CBase_Tab_Color ��Ϣ�������


void CBase_Tab_Color::UpdateControls(void)
{
	int nCapIndex;
	const IntVector* lstCapValues;
	const FloatVector* lstCapValuesFlt;
	float fCapValue;
	int nCapValue;
	CString strText;

	//���������Ĭ�ϲ�ʹ��
	int MultiCapValue = (int)(m_pUI->GetCapValueBool(UDSCAP_MULTISTREAM));

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
			nCapIndex = m_pUI->GetCurrentCapIndex(UDSCAP_RESOLUTIONFC);
			break;
		case 1: //����
			nCapIndex = m_pUI->GetCurrentCapIndex(UDSCAP_RESOLUTIONBC);
			break;
		}
	}
	m_combo_resolution.SetCurSel(nCapIndex);
	
	//����
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
			nCapValue = (int)(m_pUI->GetCapValueFloat(UDSCAP_BRIGHTNESSFC));
			break;
		case 1: //����
			nCapValue = (int)(m_pUI->GetCapValueFloat(UDSCAP_BRIGHTNESSBC));
			break;
		}
	}
	m_slider_brightness.SetPos(nCapValue);
	strText.Format("%d",nCapValue);
	SetDlgItemText(IDC_TABCOLOR_EDIT_BRIGHTNESS,strText);

	// �Աȶ� 
	if(MultiCapValue == 0) //����δѡ��
	{
		nCapValue = (int)(m_pUI->GetCapValueFloat(ICAP_CONTRAST)); 
	}
	else
	{
		switch(basebutton)
		{
		case 0: //����
			nCapValue = (int)(m_pUI->GetCapValueFloat(UDSCAP_CONTRASTFC));
			break;
		case 1: //����
			nCapValue = (int)(m_pUI->GetCapValueFloat(UDSCAP_CONTRASTBC));
			break;
		}
	}
	m_slider_contrast.SetPos(nCapValue);
	strText.Format("%d",nCapValue);
	SetDlgItemText(IDC_TABCOLOR_EDIT_CONTRAST,strText);

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
			nCapIndex = m_pUI->GetCurrentCapIndex(UDSCAP_COMPRESSQUALITYFC);
			break;
		case 1: //����
			nCapIndex = m_pUI->GetCurrentCapIndex(UDSCAP_COMPRESSQUALITYBC);
			break;
		}
	}
	m_combo_compressquality.SetCurSel(nCapIndex);
	SetCompressValue();

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
			nCapValue = (int)(m_pUI->GetCapValueFloat(UDSCAP_COMPRESSIONVALUEFC)); 
			break;
		case 1: //����
			nCapValue = (int)(m_pUI->GetCapValueFloat(UDSCAP_COMPRESSIONVALUEBC));
			break;
		}
	}	
	m_slider_compressvalue.SetPos(nCapValue);
	strText.Format("%d",nCapValue);
	SetDlgItemText(IDC_TABCOLOR_EDIT_COMPRESSVALUE,strText);

	// ɫ���Ż�
	m_combo_colorcorrect.ResetContent(); 
	lstCapValues = m_pUI->GetValidCap(UDSCAP_COLORCORRECT);
	for(unsigned int i=0; i<lstCapValues->size();i++)
	{
		switch(lstCapValues->at(i))
		{
		case TWCC_AUTO:
			m_combo_colorcorrect.InsertString(i,"�Զ�"); 
			break;
		case TWCC_IMG:
			m_combo_colorcorrect.InsertString(i,"ͼƬ"); 
			break;
		case TWCC_WORD:
			m_combo_colorcorrect.InsertString(i,"����"); 
			break;
		case TWCC_IMGTEXT:
			m_combo_colorcorrect.InsertString(i,"ͼ��ģʽ"); 
			break;
		default:
			continue;
		}
	}
	if(MultiCapValue == 0) //����δѡ��
	{
		nCapIndex = m_pUI->GetCurrentCapIndex(UDSCAP_COLORCORRECT);
	}
	else
	{
		switch(basebutton)
		{
		case 0: //����
			nCapIndex = m_pUI->GetCurrentCapIndex(UDSCAP_COLORCORRECTFC);
			break;
		case 1: //����
			nCapIndex = m_pUI->GetCurrentCapIndex(UDSCAP_COLORCORRECTBC);
			break;
		}
	}
	m_combo_colorcorrect.SetCurSel(nCapIndex);

	//���ر���
	nCapIndex = m_pUI->GetCurrentCapIndex(UDSCAP_NATIVESAVE);
	m_check_nativesave.SetCheck(nCapIndex);

}


BOOL CBase_Tab_Color::OnInitDialog()
{
	__super::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	InitSliderCtrl();
	UpdateControls();

	//2.0�汾���ؿؼ�
	GetDlgItem(IDC_TABCOLOR_COMBO_COLOROPTI)->ShowWindow(FALSE);
	GetDlgItem(IDC_STATIC_COLOROPTI)->ShowWindow(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
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
	m_slider_contrast.SetTicFreq((int)fStep);  // ���û������̶ȵ�Ƶ��Ϊ1����λ������Ҫ����������们���ʼλ�ò���

}


void CBase_Tab_Color::OnNMCustomdrawTabcolor_Slider_Compressionvalue(NMHDR *pNMHDR, LRESULT *pResult)
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
			m_pUI->SetCapValueFloat(UDSCAP_COMPRESSIONVALUEFC, (float)nval);
			break;
		case 1:
			m_pUI->SetCapValueFloat(UDSCAP_COMPRESSIONVALUEBC, (float)nval);
			break;
		}
	}

	m_edit_compressvalue.SetSel(str.GetLength(), str.GetLength(),TRUE);  // ���ñ༭��ؼ���Χ

	UpdateData(FALSE);  // ���¿ؼ�
}


void CBase_Tab_Color::OnCbnSelchangeTabcolor_Combo_Resolution()
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
			m_pUI->SetCapValueInt(UDSCAP_RESOLUTIONFC, nval); 
			break;
		case 1: //����
			m_pUI->SetCapValueInt(UDSCAP_RESOLUTIONBC, nval); 
			break;
		}
	}
	m_combo_resolution.SetCurSel(nIndex);
}


void CBase_Tab_Color::OnNMCustomdrawTabcolor_Slider_Brightness(NMHDR *pNMHDR, LRESULT *pResult)
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
			m_pUI->SetCapValueFloat(UDSCAP_BRIGHTNESSFC, (float)sldValue);
			break;
		case 1:
			m_pUI->SetCapValueFloat(UDSCAP_BRIGHTNESSBC, (float)sldValue);
			break;
		}
	}
	str.Format("%d", sldValue);
	SetDlgItemText(IDC_TABCOLOR_EDIT_BRIGHTNESS,str);

	UpdateData(FALSE);  // ���¿ؼ�
	*pResult = 0;
}


void CBase_Tab_Color::OnNMCustomdrawTabcolor_Slider_Contrast(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);  // ��������
	CString str;
	int sldValue = m_slider_contrast.GetPos();  // ��ȡ���鵱ǰλ��

	int nCapValue = (int)(m_pUI->GetCapValueBool(UDSCAP_MULTISTREAM));
	if(nCapValue == 0) //����δѡ��
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

	UpdateData(FALSE);  // ���¿ؼ�

	*pResult = 0;
}


void CBase_Tab_Color::OnEnChangeTabcolor_Edit_Brightness()
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
			m_pUI->SetCapValueFloat(UDSCAP_BRIGHTNESSFC, (float)nval);
			break;
		case 1:
			m_pUI->SetCapValueFloat(UDSCAP_BRIGHTNESSBC, (float)nval);
			break;
		}
	}
	
	m_edit_brightness.SetSel(str.GetLength(), str.GetLength(),TRUE);  // ���ñ༭��ؼ���Χ

	UpdateData(FALSE);  // ���¿ؼ�
}


void CBase_Tab_Color::OnEnChangeTabcolor_Edit_Contrast()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д __super::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);  // ��������
	CString str;
	m_edit_contrast.GetWindowText(str);
	int nval = _ttoi(str);
	m_slider_contrast.SetPos(nval);

	int nCapValue = (int)(m_pUI->GetCapValueBool(UDSCAP_MULTISTREAM));
	if(nCapValue == 0) //����δѡ��
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

	m_edit_contrast.SetSel(str.GetLength(), str.GetLength(),TRUE);  // ���ñ༭��ؼ���Χ

	UpdateData(FALSE);  // ���¿ؼ�
}

void CBase_Tab_Color::OnCbnSelchangeTabcolor_Combo_Compressquality()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	int nIndex = m_combo_compressquality.GetCurSel();
	CString strCBText; 
	m_combo_compressquality.GetLBText( nIndex, strCBText);
	int nval;
	CString str;

	if (strCBText.Find("���") >= 0) 
	{
		nval = TWCQ_BEST;	
		m_slider_compressvalue.SetPos(100);
		str.Format("%d", 100);
		SetDlgItemText(IDC_TABCOLOR_EDIT_COMPRESSVALUE, str);
	}
	else if(strCBText.Find("����") >= 0) 
	{
		nval = TWCQ_FINE; 
		m_slider_compressvalue.SetPos(80);
		str.Format("%d", 80);
		SetDlgItemText(IDC_TABCOLOR_EDIT_COMPRESSVALUE, str);
	}
	else if(strCBText.Find("һ��") >= 0) 
	{
		nval = TWCQ_JUST; 
		m_slider_compressvalue.SetPos(60);
		str.Format("%d", 60);
		SetDlgItemText(IDC_TABCOLOR_EDIT_COMPRESSVALUE, str);
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
			m_pUI->SetCapValueInt(UDSCAP_COMPRESSQUALITYFC, nval); 
			break;
		case 1: //����
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

void CBase_Tab_Color::OnCbnSelchangeBase_ComboColoropti()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nIndex = m_combo_colorcorrect.GetCurSel();
	CString strCBText; 
	m_combo_colorcorrect.GetLBText( nIndex, strCBText);
	int nval;
	if (strCBText.Find("�Զ�") >= 0)
	{
		nval = TWCC_AUTO;
	}
	else if(strCBText.Find("ͼƬ") >= 0)
	{
		nval = TWCC_IMG; 
	}
	else if(strCBText.Find("����") >= 0)
	{
		nval = TWCC_WORD; 
	}
	else if(strCBText.Find("ͼ��ģʽ") >= 0)
	{
		nval = TWCC_IMGTEXT;
	}
	else
	{}

	int MultiCapValue = (int)(m_pUI->GetCapValueBool(UDSCAP_MULTISTREAM));
	if(MultiCapValue == 0) //����δѡ��
	{
		m_pUI->SetCapValueInt(UDSCAP_COLORCORRECT,nval); 
	}
	else
	{
		switch(basebutton)
		{
		case 0: //����
			m_pUI->SetCapValueInt(UDSCAP_COLORCORRECTFC,nval); 
			break;
		case 1: //����
			m_pUI->SetCapValueInt(UDSCAP_COLORCORRECTBC,nval); 
			break;
		}
	}
	m_combo_colorcorrect.SetCurSel(nIndex);
}

void CBase_Tab_Color::OnBaseTab_Color_Btn_Check_Nativesave()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nval;
	if (m_check_nativesave.GetCheck())
	{
		nval = TRUE;
	} 
	else
	{
		nval = FALSE;
	}
	
	m_pUI->SetCapValueInt(UDSCAP_NATIVESAVE,nval);
}

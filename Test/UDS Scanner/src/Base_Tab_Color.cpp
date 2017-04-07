// Base_Tab_Color.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "UDS Scanner.h"
#include "Base_Tab_Color.h"
#include "afxdialogex.h"


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
	DDX_Control(pDX, IDC_TABCOLOR_COMBO_COMPRESS, m_combo_compress);
	DDX_Control(pDX, IDC_TABCOLOR_SLIDER_COMPRESSION, m_slider_compressvalue);
	DDX_Control(pDX, IDC_TABCOLOR_EDIT_COMPRESSVALUE, m_edit_compressvalue);
	DDX_Control(pDX, IDC_TABCOLOR_COMBO_RESOLUTION, m_combo_resolution);
	DDX_Control(pDX, IDC_TABCOLOR_SLIDER_CONTRAST, m_slider_contrast);
	DDX_Control(pDX, IDC_TABCOLOR_EDIT_CONTRAST, m_edit_contrast);
	DDX_Control(pDX, IDC_TABCOLOR_EDIT_BRIGHTNESS, m_edit_brightness);
	DDX_Control(pDX, IDC_TABCOLOR_SLIDER_BRIGHTNESS, m_slider_brightness);
}


BEGIN_MESSAGE_MAP(CBase_Tab_Color, CDialogEx)
	ON_CBN_SELCHANGE(IDC_TABCOLOR_COMBO_COMPRESS, &CBase_Tab_Color::OnCbnSelchangeTabcolor_Combo_Compress)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_TABCOLOR_SLIDER_COMPRESSION, &CBase_Tab_Color::OnNMCustomdrawTabcolor_Slider_Compressionvalue)
	ON_EN_CHANGE(IDC_TABCOLOR_EDIT_COMPRESSVALUE, &CBase_Tab_Color::OnEnChangeTabcolor_Edit_Compressvalue)
	ON_CBN_SELCHANGE(IDC_TABCOLOR_COMBO_RESOLUTION, &CBase_Tab_Color::OnCbnSelchangeTabcolor_Combo_Resolution)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_TABCOLOR_SLIDER_CONTRAST, &CBase_Tab_Color::OnNMCustomdrawTabcolor_Slider_Contrast)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_TABCOLOR_SLIDER_BRIGHTNESS, &CBase_Tab_Color::OnNMCustomdrawTabcolor_Slider_Brightness)
	ON_EN_CHANGE(IDC_TABCOLOR_EDIT_CONTRAST, &CBase_Tab_Color::OnEnChangeTabcolor_Edit_Contrast)
	ON_EN_CHANGE(IDC_TABCOLOR_EDIT_BRIGHTNESS, &CBase_Tab_Color::OnEnChangeTabcolor_Edit_Brightness)
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
	int nval;

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

	// ѹ���� 
	nCapValue = (int)(m_pUI->GetCapValueFloat(UDSCAP_COMPRESSVALUE)); 
	m_slider_compressvalue.SetPos(nCapValue);
	strText.Format("%d",nCapValue);
	SetDlgItemText(IDC_TABCOLOR_EDIT_COMPRESSVALUE,strText);

	// �ֱ���
	m_combo_resolution.ResetContent();
	nCapIndex = m_pUI->GetCurrentCapIndex(ICAP_XRESOLUTION);
	lstCapValuesFlt = m_pUI->GetValidCapFloat(ICAP_XRESOLUTION);
	for(unsigned int i=0; i<lstCapValuesFlt->size();i++)
	{
		CString strTemp;
		strTemp.Format("%d",(int)lstCapValuesFlt->at(i));
		m_combo_resolution.InsertString(i,strTemp);
	}
	m_combo_resolution.SetCurSel(nCapIndex);
	nval = (int)lstCapValuesFlt->at(nCapIndex);

	// ���� 
	nCapValue = (int)(m_pUI->GetCapValueFloat(ICAP_BRIGHTNESS));
	m_slider_brightness.SetPos(nCapValue);
	strText.Format("%d",nCapValue);
	SetDlgItemText(IDC_TABCOLOR_EDIT_BRIGHTNESS,strText);

	// �Աȶ� 
	nCapValue = (int)(m_pUI->GetCapValueFloat(ICAP_CONTRAST)); 
	m_slider_contrast.SetPos(nCapValue);
	strText.Format("%d",nCapValue);
	SetDlgItemText(IDC_TABCOLOR_EDIT_CONTRAST,strText);


}


BOOL CBase_Tab_Color::OnInitDialog()
{
	__super::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	InitSliderCtrl();
	UpdateControls();

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


void CBase_Tab_Color::OnCbnSelchangeTabcolor_Combo_Compress()
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
	m_pUI->SetCapValueInt(ICAP_COMPRESSION, nval); 
	m_combo_compress.SetCurSel(nIndex);
}


void CBase_Tab_Color::OnNMCustomdrawTabcolor_Slider_Compressionvalue(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);  // ��������
	CString str;
	int sldValue = m_slider_compressvalue.GetPos();  // ��ȡ���鵱ǰλ��
	m_pUI->SetCapValueInt(UDSCAP_COMPRESSVALUE, sldValue); 

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

	m_pUI->SetCapValueFloat(UDSCAP_COMPRESSVALUE, (float)nval);

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

	m_pUI->SetCapValueInt(ICAP_XRESOLUTION, nval); 
	m_pUI->SetCapValueInt(ICAP_YRESOLUTION, nval); 
	m_combo_resolution.SetCurSel(nIndex);
}


void CBase_Tab_Color::OnNMCustomdrawTabcolor_Slider_Brightness(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);  // ��������
	CString str;
	int sldValue = m_slider_brightness.GetPos();  // ��ȡ���鵱ǰλ��
	m_pUI->SetCapValueFloat(ICAP_BRIGHTNESS, (float)sldValue); 
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
	m_pUI->SetCapValueFloat(ICAP_CONTRAST, (float)sldValue); 
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
	m_pUI->SetCapValueFloat(ICAP_BRIGHTNESS, (float)nval);
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
	m_pUI->SetCapValueFloat(ICAP_CONTRAST, (float)nval); 

	m_edit_contrast.SetSel(str.GetLength(), str.GetLength(),TRUE);  // ���ñ༭��ؼ���Χ

	UpdateData(FALSE);  // ���¿ؼ�
}




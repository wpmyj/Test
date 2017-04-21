// Base_AutoColor.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "UDS Scanner.h"
#include "Base_AutoColor.h"
#include "afxdialogex.h"


// CBase_AutoColor �Ի���

IMPLEMENT_DYNAMIC(CBase_AutoColor, CPropertyPage)

CBase_AutoColor::CBase_AutoColor(MFC_UI *pUI)
	:m_pUI(pUI),CPropertyPage(CBase_AutoColor::IDD)
{
}

CBase_AutoColor::~CBase_AutoColor()
{
}

void CBase_AutoColor::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TABAUTOCOLOR_COMBO_RESOLUTION, m_combo_resolution);
	DDX_Control(pDX, IDC_TABAUTOCOLOR_COMBO_COMPRESS, m_combo_compress);
	DDX_Control(pDX, IDC_TABAUTOCOLOR_SLIDER_COMPRESSION, m_slider_compressvalue);
	DDX_Control(pDX, IDC_TABAUTOCOLOR_EDIT_COMPRESSVALUE, m_edit_compressvalue);
	DDX_Control(pDX, IDC_TABAUTOCOLOR_COMBO_NOCOLOR, m_combo_nocolor);
	DDX_Control(pDX, IDC_TABAUTOCOLOR_EDIT_COLORTHRES, m_edit_colorthres);
	DDX_Control(pDX, IDC_TABAUTOCOLOR_SLIDER_COLORTHRES, m_slider_colorthres);
}


BEGIN_MESSAGE_MAP(CBase_AutoColor, CPropertyPage)
	ON_CBN_SELCHANGE(IDC_TABAUTOCOLOR_COMBO_COMPRESS, &CBase_AutoColor::OnCbnSelchangeTabautocolor_Combo_Compress)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_TABAUTOCOLOR_SLIDER_COMPRESSION, &CBase_AutoColor::OnNMCustomdrawTabautocolor_Slider_Compressionvalue)
	ON_EN_CHANGE(IDC_TABAUTOCOLOR_EDIT_COMPRESSVALUE, &CBase_AutoColor::OnEnChangeTabautocolor_Edit_Compressvalue)
	ON_CBN_SELCHANGE(IDC_TABAUTOCOLOR_COMBO_RESOLUTION, &CBase_AutoColor::OnCbnSelchangeTabautocolor_Combo_Resolution)
	ON_CBN_SELCHANGE(IDC_TABAUTOCOLOR_COMBO_NOCOLOR, &CBase_AutoColor::OnCbnSelchangeTabautocolor_Combo_Nocolor)
	ON_EN_CHANGE(IDC_TABAUTOCOLOR_EDIT_COLORTHRES, &CBase_AutoColor::OnEnChangeTabautocolor_Edit_Colorthres)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_TABAUTOCOLOR_SLIDER_COLORTHRES, &CBase_AutoColor::OnNMCustomdrawTabautocolor_Slider_Colorthres)
END_MESSAGE_MAP()


// CBase_AutoColor ��Ϣ�������

void CBase_AutoColor::UpdateControls(void)
{
	int nCapIndex;
	const IntVector* lstCapValues;
	const FloatVector* lstCapValuesFlt;
	float fCapValue;
	int nCapValue;
	CString strText;

	//�ǲ�ɫʱɨ��
	m_combo_nocolor.ResetContent();  // �������
	nCapIndex = m_pUI->GetCurrentCapIndex(UDSCAP_NOCOLOR);
	lstCapValues = m_pUI->GetValidCap(UDSCAP_NOCOLOR);
	for(unsigned int i=0; i<lstCapValues->size();i++)
	{
		switch(lstCapValues->at(i))
		{
		case TWNC_GRAY:
			m_combo_nocolor.InsertString(i,"�ҽ�"); 
			break;
		case TWNC_BLACK:
			m_combo_nocolor.InsertString(i,"�ڰ�"); 
			break;
		default:
			continue;
		}
	}
	m_combo_nocolor.SetCurSel(nCapIndex);

	// ɫ����ֵ 
	nCapValue = (int)(m_pUI->GetCapValueFloat(UDSCAP_COLORTHRESHOLD)); 
	m_slider_colorthres.SetPos(nCapValue);
	strText.Format("%d",nCapValue);
	SetDlgItemText(IDC_TABAUTOCOLOR_EDIT_COLORTHRES,strText);

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

	// ѹ���� 
	nCapValue = (int)(m_pUI->GetCapValueFloat(UDSCAP_COMPRESSVALUE)); 
	m_slider_compressvalue.SetPos(nCapValue);
	strText.Format("%d",nCapValue);
	SetDlgItemText(IDC_TABAUTOCOLOR_EDIT_COMPRESSVALUE,strText);

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

}


BOOL CBase_AutoColor::OnInitDialog()
{
	__super::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��	
	InitSliderCtrl();
	UpdateControls();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CBase_AutoColor::InitSliderCtrl()
{
	float fMin,fMax,fStep;
	m_pUI->GetCapRangeFloat(UDSCAP_COMPRESSVALUE, fMin, fMax, fStep);
	m_slider_compressvalue.SetRange((int)fMin, (int)fMax);
	m_slider_compressvalue.SetTicFreq((int)fStep);

	m_pUI->GetCapRangeFloat(UDSCAP_COLORTHRESHOLD, fMin, fMax, fStep);
	m_slider_colorthres.SetRange((int)fMin, (int)fMax);
	m_slider_colorthres.SetTicFreq((int)fStep);
}


void CBase_AutoColor::OnCbnSelchangeTabautocolor_Combo_Compress()
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


void CBase_AutoColor::OnNMCustomdrawTabautocolor_Slider_Compressionvalue(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);  // ��������
	CString str;
	int sldValue = m_slider_compressvalue.GetPos();  // ��ȡ���鵱ǰλ��
	m_pUI->SetCapValueInt(UDSCAP_COMPRESSVALUE, sldValue); 

	str.Format("%d", sldValue);
	SetDlgItemText(IDC_TABAUTOCOLOR_EDIT_COMPRESSVALUE, str);

	*pResult = 0;
}


void CBase_AutoColor::OnEnChangeTabautocolor_Edit_Compressvalue()
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


void CBase_AutoColor::OnCbnSelchangeTabautocolor_Combo_Resolution()
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

void CBase_AutoColor::OnCbnSelchangeTabautocolor_Combo_Nocolor()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nIndex = m_combo_nocolor.GetCurSel();
	CString strCBText; 
	m_combo_nocolor.GetLBText( nIndex, strCBText);
	int nval;
	if (strCBText.Find("�ҽ�") >= 0)
	{
		nval = TWNC_GRAY;
	}
	else if(strCBText.Find("�ڰ�") >= 0)
	{
		nval = TWNC_BLACK; 
	}
	else
	{}
	m_pUI->SetCapValueInt(UDSCAP_NOCOLOR, nval); 
	m_combo_nocolor.SetCurSel(nIndex);
}


void CBase_AutoColor::OnEnChangeTabautocolor_Edit_Colorthres()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д __super::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);  // ��������
	CString str;
	m_edit_colorthres.GetWindowText(str);
	int nval = _ttoi(str);
	m_slider_colorthres.SetPos(nval);

	m_pUI->SetCapValueFloat(UDSCAP_COLORTHRESHOLD, (float)nval);

	m_edit_colorthres.SetSel(str.GetLength(), str.GetLength(),TRUE);  // ���ñ༭��ؼ���Χ

	UpdateData(FALSE);  // ���¿ؼ�
}


void CBase_AutoColor::OnNMCustomdrawTabautocolor_Slider_Colorthres(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);  // ��������
	CString str;
	int sldValue = m_slider_colorthres.GetPos();  // ��ȡ���鵱ǰλ��
	m_pUI->SetCapValueInt(UDSCAP_COLORTHRESHOLD, sldValue); 

	str.Format("%d", sldValue);
	SetDlgItemText(IDC_TABAUTOCOLOR_EDIT_COLORTHRES, str);

	*pResult = 0;
}

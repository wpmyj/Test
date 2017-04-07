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
	//m_TabAutoColormap.swap(map<int, float>());  // ��ղ��ͷ��ڴ�
}

void CBase_AutoColor::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TABAUTOCOLOR_COMBO_RESOLUTION, m_combo_resolution);
	DDX_Control(pDX, IDC_TABAUTOCOLOR_COMBO_COMPRESS, m_combo_compress);
	DDX_Control(pDX, IDC_TABAUTOCOLOR_SLIDER_COMPRESSION, m_slider_compressvalue);
	DDX_Control(pDX, IDC_TABAUTOCOLOR_EDIT_COMPRESSVALUE, m_edit_compressvalue);
}


BEGIN_MESSAGE_MAP(CBase_AutoColor, CPropertyPage)
	ON_CBN_SELCHANGE(IDC_TABAUTOCOLOR_COMBO_COMPRESS, &CBase_AutoColor::OnCbnSelchangeTabautocolor_Combo_Compress)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_TABAUTOCOLOR_SLIDER_COMPRESSION, &CBase_AutoColor::OnNMCustomdrawTabautocolor_Slider_Compressionvalue)
	ON_EN_CHANGE(IDC_TABAUTOCOLOR_EDIT_COMPRESSVALUE, &CBase_AutoColor::OnEnChangeTabautocolor_Edit_Compressvalue)
	ON_CBN_SELCHANGE(IDC_TABAUTOCOLOR_COMBO_RESOLUTION, &CBase_AutoColor::OnCbnSelchangeTabautocolor_Combo_Resolution)
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
	nval = (int)lstCapValuesFlt->at(nCapIndex);

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
	//m_TabAutoColormap[ICAP_COMPRESSION] = nval;
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
	//m_TabAutoColormap[UDSCAP_COMPRESSVALUE] = sldValue;
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

	//m_TabAutoColormap[UDSCAP_COMPRESSVALUE] = (float)nval;
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

//	m_TabAutoColormap[ICAP_XRESOLUTION] = (float)nval;
	//m_TabAutoColormap[ICAP_YRESOLUTION] = (float)nval;
	m_pUI->SetCapValueInt(ICAP_XRESOLUTION, nval); 
	m_pUI->SetCapValueInt(ICAP_YRESOLUTION, nval); 
	m_combo_resolution.SetCurSel(nIndex);
}




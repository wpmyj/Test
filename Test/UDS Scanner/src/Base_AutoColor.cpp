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
	,m_dlg_gray(pUI)
	,m_dlg_bw(pUI)
	,m_dlg_color(pUI)
{
	m_radio_graybw = 0;
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
	//  DDX_Control(pDX, IDC_TABAUTOCOLOR_COMBO_NOCOLOR, m_combo_nocolor);
	DDX_Control(pDX, IDC_TABAUTOCOLOR_EDIT_COLORTHRES, m_edit_colorthres);
	DDX_Control(pDX, IDC_TABAUTOCOLOR_SLIDER_COLORTHRES, m_slider_colorthres);
	DDX_Control(pDX, IDC_STATIC_OTHERDIALOG, m_staic_otherdialog);
	//  DDX_Control(pDX, IDC_CHECK_NOCOLOR, m_check_nocolor);
	DDX_Radio(pDX, IDC_AUTOCOLOR_RADIO_NOCOLOR_GRAY, m_radio_graybw);
	//  DDX_Control(pDX, IDC_AUTOCOLOR_CHECK_COLOR, m_radio_color);
	DDX_Control(pDX, IDC_AUTOCOLOR_RADIO_COLOR, m_radio_color);
}


BEGIN_MESSAGE_MAP(CBase_AutoColor, CPropertyPage)
	ON_CBN_SELCHANGE(IDC_TABAUTOCOLOR_COMBO_COMPRESS, &CBase_AutoColor::OnCbnSelchangeTabautocolor_Combo_Compress)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_TABAUTOCOLOR_SLIDER_COMPRESSION, &CBase_AutoColor::OnNMCustomdrawTabautocolor_Slider_Compressionvalue)
	ON_EN_CHANGE(IDC_TABAUTOCOLOR_EDIT_COMPRESSVALUE, &CBase_AutoColor::OnEnChangeTabautocolor_Edit_Compressvalue)
	ON_CBN_SELCHANGE(IDC_TABAUTOCOLOR_COMBO_RESOLUTION, &CBase_AutoColor::OnCbnSelchangeTabautocolor_Combo_Resolution)
//	ON_CBN_SELCHANGE(IDC_TABAUTOCOLOR_COMBO_NOCOLOR, &CBase_AutoColor::OnCbnSelchangeTabautocolor_Combo_Nocolor)
	ON_EN_CHANGE(IDC_TABAUTOCOLOR_EDIT_COLORTHRES, &CBase_AutoColor::OnEnChangeTabautocolor_Edit_Colorthres)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_TABAUTOCOLOR_SLIDER_COLORTHRES, &CBase_AutoColor::OnNMCustomdrawTabautocolor_Slider_Colorthres)
//	ON_BN_CLICKED(IDC_CHECK_NOCOLOR, &CBase_AutoColor::OnBase_Btn_Check_Nocolor)
	ON_BN_CLICKED(IDC_AUTOCOLOR_RADIO_NOCOLOR_GRAY, &CBase_AutoColor::OnAuotoColor_RadioBtn_Nocolor_Gray)
	ON_BN_CLICKED(IDC_AUTOCOLOR_RADIO_NOCOLOR_BW, &CBase_AutoColor::OnAuotoColor_RadioBtn_Nocolor_Gray)
//	ON_BN_CLICKED(IDC_AUTOCOLOR_CHECK_COLOR, &CBase_AutoColor::OnBase_Btn_Check_color)
ON_BN_CLICKED(IDC_AUTOCOLOR_RADIO_COLOR, &CBase_AutoColor::OnBase_Btn_Radio_color)
ON_BN_CLICKED(IDC_AUTOCOLOR_BUTTON_COLOR, &CBase_AutoColor::OnAutocolor_Btn_Color)
ON_BN_CLICKED(AUTOCOLOR_BUTTON_NOCOLOR_GRAY, &CBase_AutoColor::OnAutocolor_Btn_NocolorGray)
ON_BN_CLICKED(IDC_AUTOCOLOR_BUTTON_NOCOLOR_BW, &CBase_AutoColor::OnAutocolor_Btn_NocolorBw)
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
	//m_combo_nocolor.ResetContent();  // �������
	//nCapIndex = m_pUI->GetCurrentCapIndex(UDSCAP_NOCOLOR);
	//lstCapValues = m_pUI->GetValidCap(UDSCAP_NOCOLOR);
	//for(unsigned int i=0; i<lstCapValues->size();i++)
	//{
	//	switch(lstCapValues->at(i))
	//	{
	//	case TWNC_GRAY:
	//		m_combo_nocolor.InsertString(i,"�ҽ�"); 
	//		break;
	//	case TWNC_BLACK:
	//		m_combo_nocolor.InsertString(i,"�ڰ�"); 
	//		break;
	//	default:
	//		continue;
	//	}
	//}
	//m_combo_nocolor.SetCurSel(nCapIndex);

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

	GetDlgItem(IDC_TABAUTOCOLOR_COMBO_RESOLUTION)->ShowWindow(FALSE); 
	GetDlgItem(IDC_TABAUTOCOLOR_COMBO_COMPRESS)->ShowWindow(FALSE);
	GetDlgItem(IDC_TABAUTOCOLOR_SLIDER_COMPRESSION)->ShowWindow(FALSE);
	GetDlgItem(IDC_TABAUTOCOLOR_EDIT_COMPRESSVALUE)->ShowWindow(FALSE);

	//m_combo_nocolor.EnableWindow(FALSE);
	m_slider_colorthres.EnableWindow(FALSE);
	m_edit_colorthres.EnableWindow(FALSE);
	
	GetDlgItem(IDC_CHECK_NOCOLOR)->EnableWindow(FALSE);
	
	m_radio_color.SetCheck(TRUE);

	CRect m_dlgrect;
	//�����Ӵ���
	m_dlg_color.Create(IDD_BASETAB_COLOR, GetDlgItem(IDC_STATIC_OTHERDIALOG));
	m_dlg_gray.Create(IDD_BASETAB_GRAY, GetDlgItem(IDC_STATIC_OTHERDIALOG));
	m_dlg_bw.Create(IDD_BASETAB_BW, GetDlgItem(IDC_STATIC_OTHERDIALOG));
	//�����Ӵ��ڴ�С��ӦSTA
	m_staic_otherdialog.GetClientRect(&m_dlgrect);
	m_dlg_color.MoveWindow(&m_dlgrect);
	m_dlg_gray.MoveWindow(&m_dlgrect);
	m_dlg_bw.MoveWindow(&m_dlgrect);
	
	ShowOtherDialog();

	m_dlg_color.ShowWindow(SW_SHOW);
	m_dlg_gray.ShowWindow(SW_HIDE);
	m_dlg_bw.ShowWindow(SW_HIDE);

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

//void CBase_AutoColor::OnCbnSelchangeTabautocolor_Combo_Nocolor()
//{
//	// TODO: �ڴ���ӿؼ�֪ͨ����������
//	int nIndex = m_combo_nocolor.GetCurSel();
//	CString strCBText; 
//	m_combo_nocolor.GetLBText( nIndex, strCBText);
//	int nval;
//	if (strCBText.Find("�Ҷ�") >= 0)
//	{
//		nval = TWNC_GRAY;
//	}
//	else if(strCBText.Find("�ڰ�") >= 0)
//	{
//		nval = TWNC_BLACK; 
//	}
//	else
//	{}
//	m_pUI->SetCapValueInt(UDSCAP_NOCOLOR, nval); 
//	m_combo_nocolor.SetCurSel(nIndex);
//
//	ShowOtherDialog();
//}

void CBase_AutoColor::ShowOtherDialog()
{
	/*int nIndex = m_combo_nocolor.GetCurSel();
	if(m_combo_nocolor.IsWindowEnabled())
	{
		if(nIndex == 0)
		{
			m_dlg_color.ShowWindow(SW_HIDE);
			m_dlg_bw.ShowWindow(SW_SHOW);
			m_dlg_gray.ShowWindow(SW_HIDE);
		}
		else if(nIndex == 1)
		{
			m_dlg_color.ShowWindow(SW_HIDE);
			m_dlg_gray.ShowWindow(SW_SHOW);
			m_dlg_bw.ShowWindow(SW_HIDE);
		}
		else{}
	}
	else
	{
		m_dlg_color.ShowWindow(SW_SHOW);
		m_dlg_gray.ShowWindow(SW_HIDE);
		m_dlg_bw.ShowWindow(SW_HIDE);
	}*/
	//if(m_check_nocolor.GetCheck())
	{
		if(m_radio_graybw == 0)
		{
			m_dlg_color.ShowWindow(SW_HIDE);
			m_dlg_gray.ShowWindow(SW_SHOW);
			m_dlg_bw.ShowWindow(SW_HIDE);
		}
		else if(m_radio_graybw == 1)
		{
			m_dlg_color.ShowWindow(SW_HIDE);
			m_dlg_gray.ShowWindow(SW_HIDE);
			m_dlg_bw.ShowWindow(SW_SHOW);
		}
		else{}
	}
	/*else
	{
	m_dlg_color.ShowWindow(SW_SHOW);
	m_dlg_gray.ShowWindow(SW_HIDE);
	m_dlg_bw.ShowWindow(SW_HIDE);
	}
	*/
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


//void CBase_AutoColor::OnBase_Btn_Check_Nocolor()
//{
//	// TODO: �ڴ���ӿؼ�֪ͨ����������
//	UpdateData(TRUE);
//	if(m_check_nocolor.GetCheck())
//	{
//		/*m_combo_nocolor.EnableWindow(TRUE);
//		m_slider_colorthres.EnableWindow(TRUE);
//		m_edit_colorthres.EnableWindow(TRUE);*/
//		GetDlgItem(IDC_AUTOCOLOR_RADIO_NOCOLOR_GRAY)->EnableWindow(TRUE); 
//		GetDlgItem(IDC_AUTOCOLOR_RADIO_NOCOLOR_BW)->EnableWindow(TRUE);
//	}
//	else
//	{
//		/*m_combo_nocolor.EnableWindow(FALSE);
//		m_slider_colorthres.EnableWindow(FALSE);
//		m_edit_colorthres.EnableWindow(FALSE);*/
//		GetDlgItem(IDC_AUTOCOLOR_RADIO_NOCOLOR_GRAY)->EnableWindow(FALSE); 
//		GetDlgItem(IDC_AUTOCOLOR_RADIO_NOCOLOR_BW)->EnableWindow(FALSE);
//	}
//	ShowOtherDialog();
//	UpdateData(FALSE);
//}


void CBase_AutoColor::OnAuotoColor_RadioBtn_Nocolor_Gray()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	int nval;
	switch(m_radio_graybw)
	{
	case 0:
		nval = TWNC_GRAY;
		break;
	case 1:
		nval = TWNC_BLACK; 
		break;
	}
	m_pUI->SetCapValueInt(UDSCAP_NOCOLOR, nval); 
	ShowOtherDialog();
	UpdateData(FALSE);
}


void CBase_AutoColor::OnBase_Btn_Radio_color()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	m_radio_color.SetCheck(TRUE);

	m_dlg_color.ShowWindow(SW_SHOW);
	m_dlg_gray.ShowWindow(SW_HIDE);
	m_dlg_bw.ShowWindow(SW_HIDE);

	UpdateData(FALSE);
}


void CBase_AutoColor::OnAutocolor_Btn_Color()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	m_radio_color.SetCheck(TRUE);

	m_dlg_color.ShowWindow(SW_SHOW);
	m_dlg_gray.ShowWindow(SW_HIDE);
	m_dlg_bw.ShowWindow(SW_HIDE);

	UpdateData(FALSE);
}


void CBase_AutoColor::OnAutocolor_Btn_NocolorGray()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(m_radio_graybw==0)
	{
		m_dlg_color.ShowWindow(SW_HIDE);
		m_dlg_gray.ShowWindow(SW_SHOW);
		m_dlg_bw.ShowWindow(SW_HIDE);
	}

}


void CBase_AutoColor::OnAutocolor_Btn_NocolorBw()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(m_radio_graybw==1)
	{
		m_dlg_color.ShowWindow(SW_HIDE);
		m_dlg_gray.ShowWindow(SW_HIDE);
		m_dlg_bw.ShowWindow(SW_SHOW);
	}
	
}

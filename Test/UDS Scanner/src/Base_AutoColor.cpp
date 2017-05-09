// Base_AutoColor.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "UDS Scanner.h"
#include "Base_AutoColor.h"
#include "afxdialogex.h"


// CBase_AutoColor �Ի���

IMPLEMENT_DYNAMIC(CBase_AutoColor, CDialogEx)

CBase_AutoColor::CBase_AutoColor(MFC_UI *pUI)
	:m_pUI(pUI),CDialogEx(CBase_AutoColor::IDD)
	,m_dlg_gray(pUI)
	,m_dlg_bw(pUI)
	,m_dlg_color(pUI)
{
}

CBase_AutoColor::~CBase_AutoColor()
{
}

void CBase_AutoColor::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TABAUTOCOLOR_EDIT_COLORTHRES, m_edit_colorthres);
	DDX_Control(pDX, IDC_TABAUTOCOLOR_SLIDER_COLORTHRES, m_slider_colorthres);
	DDX_Control(pDX, IDC_STATIC_OTHERDIALOG, m_staic_otherdialog);
	DDX_Radio(pDX, IDC_AUTOCOLOR_RADIO_NOCOLOR_GRAY, m_radio_graybw);
	DDX_Control(pDX, IDC_AUTOCOLOR_RADIO_COLOR, m_radio_color);
}


BEGIN_MESSAGE_MAP(CBase_AutoColor, CDialogEx)
	ON_EN_CHANGE(IDC_TABAUTOCOLOR_EDIT_COLORTHRES, &CBase_AutoColor::OnEnChangeTabautocolor_Edit_Colorthres)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_TABAUTOCOLOR_SLIDER_COLORTHRES, &CBase_AutoColor::OnNMCustomdrawTabautocolor_Slider_Colorthres)
	ON_BN_CLICKED(IDC_AUTOCOLOR_RADIO_NOCOLOR_GRAY, &CBase_AutoColor::OnAuotoColor_RadioBtn_Nocolor_Gray)
	ON_BN_CLICKED(IDC_AUTOCOLOR_RADIO_NOCOLOR_BW, &CBase_AutoColor::OnAuotoColor_RadioBtn_Nocolor_Gray)
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

	// ɫ����ֵ 
	nCapValue = (int)(m_pUI->GetCapValueFloat(UDSCAP_COLORTHRESHOLD)); 
	m_slider_colorthres.SetPos(nCapValue);
	strText.Format("%d",nCapValue);
	SetDlgItemText(IDC_TABAUTOCOLOR_EDIT_COLORTHRES,strText);

	nCapIndex = m_pUI->GetCurrentCapIndex(UDSCAP_NOCOLOR);
	m_radio_graybw = nCapIndex;

	m_dlg_color.ShowWindow(SW_SHOW);
	m_dlg_gray.ShowWindow(SW_HIDE);
	m_dlg_bw.ShowWindow(SW_HIDE);
}


BOOL CBase_AutoColor::OnInitDialog()
{
	__super::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��	
	InitSliderCtrl();
	UpdateControls();
	
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
	
	m_dlg_color.ShowWindow(SW_SHOW);
	m_dlg_gray.ShowWindow(SW_HIDE);
	m_dlg_bw.ShowWindow(SW_HIDE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CBase_AutoColor::InitSliderCtrl()
{
	float fMin,fMax,fStep;
	m_pUI->GetCapRangeFloat(UDSCAP_COLORTHRESHOLD, fMin, fMax, fStep);
	m_slider_colorthres.SetRange((int)fMin, (int)fMax);
	m_slider_colorthres.SetTicFreq((int)fStep);
}

void CBase_AutoColor::ShowOtherDialog()
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

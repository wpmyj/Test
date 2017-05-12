// Page_Set.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "UDS Scanner.h"
#include "Page_Set.h"
#include "afxdialogex.h"

extern int g_nDeviceNumber;
extern void GetFilePath( char* szFileName, char* szFilePath);
// CPage_Set �Ի���

IMPLEMENT_DYNAMIC(CPage_Set, CPropertyPage)

CPage_Set::CPage_Set(MFC_UI *pUI)
	: m_pUI(pUI), CPropertyPage(CPage_Set::IDD)
{
	m_hDLL = NULL;
}

CPage_Set::~CPage_Set()
{
	m_setmap.swap(map<int, float>());  // ��ղ��ͷ��ڴ�
}

void CPage_Set::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SET_CHECK_SAVEPOWER, m_check_savepower);
	DDX_Control(pDX, IDC_SET_CHECK_OFFTIME, m_check_offtime);
	DDX_Control(pDX, IDC_SET_SLIDER_OFFTIME, m_slider_offtime);
	DDX_Control(pDX, IDC_SET_SLIDER_SAVEPOWER, m_slider_savepower);
	DDX_Control(pDX, IDC_SET_CHECK_TURNVIDEO, m_check_turnvideo);
	DDX_Control(pDX, IDC_SET_CHECK_SHOWSCHEDULE, m_check_indicator);
	DDX_Control(pDX, IDC_SET_CHECK_EMERGENCY, m_check_enmergency);
}


BEGIN_MESSAGE_MAP(CPage_Set, CPropertyPage)
	ON_BN_CLICKED(IDC_SET_CHECK_SAVEPOWER, &CPage_Set::OnSet_Btn_Check_SavePower)
	ON_BN_CLICKED(IDC_SET_CHECK_OFFTIME, &CPage_Set::OnSet_Btn_Check_OffTime)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SET_SLIDER_SAVEPOWER, &CPage_Set::OnNMCustomdrawSet_Slider_SavePower)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SET_SLIDER_OFFTIME, &CPage_Set::OnNMCustomdrawSet_Slider_Offtime)
  ON_BN_CLICKED(IDC_SET_CHECK_TURNVIDEO, &CPage_Set::OnSet_Btn_Check_TurnVideo)
  ON_BN_CLICKED(IDC_SET_CHECK_SHOWSCHEDULE, &CPage_Set::OnSet_Btn_Check_Showschedule)
  ON_BN_CLICKED(IDC_SET_CHECK_EMERGENCY, &CPage_Set::OnSet_Btn_Check_Emergency)
END_MESSAGE_MAP()


// CPage_Set ��Ϣ�������
void CPage_Set::SetCapValue(void)
{
	MAP_CAP::iterator iter;
	if (true == m_setmap.empty())
	{
		return;
	}

	// ���ô���ʱ����ػ�ʱ��
	if (false == LoadDLL())
	{
		return;
	}
	BOOL bPowerSaving = static_cast<BOOL>(m_setmap[UDSCAP_POWERSAVING]);
	BOOL bPowerOff =  static_cast<BOOL>(m_setmap[UDSCAP_POWEROFF]);
	WORD dwPowerSaving = static_cast<WORD>(m_setmap[UDSCAP_POWERSAVING_TIME]);
	WORD dwPowerOff = static_cast<WORD>(m_setmap[UDSCAP_POWEROFF_TIME]);

	if (FALSE == bPowerSaving)
	{
		dwPowerSaving = 0;
	}

	if (FALSE == bPowerOff)
	{
		dwPowerOff = 0;
	}

	SetNVRAMValue( &dwPowerSaving, sizeof(dwPowerSaving), 0x000A);
	SetNVRAMValue( &dwPowerOff, sizeof(dwPowerOff), 0x0010);
	TerminateDriver();
	FreeLibrary(m_hDLL);

}


void CPage_Set::UpdateControls(void)
{
	UpdateData(TRUE);
	int nCapValue;
	int nval;
	CString strText;

	//�ڵ�ģʽ
	nCapValue = (int)(m_pUI->GetCapValueBool(UDSCAP_POWERSAVING));
	m_check_savepower.SetCheck(nCapValue);
	SetSavePower();
  m_setmap[UDSCAP_POWERSAVING] = float(nCapValue);
	//�ڵ�ģʽֵ
	nCapValue = (int)(m_pUI->GetCapValueFloat(UDSCAP_POWERSAVING_TIME));
	m_slider_savepower.SetPos(nCapValue);
	SetDlgItemText(IDC_SET_STATIC_SAVEPOWER, strText);
	m_setmap[UDSCAP_POWERSAVING_TIME] = float(nCapValue);

	//�ػ�ʱ��
	nCapValue = (int)(m_pUI->GetCapValueBool(UDSCAP_POWEROFF));
	m_check_offtime.SetCheck(nCapValue);
	SetOffTime();
  m_setmap[UDSCAP_POWEROFF] = float(nCapValue);
	//�ػ�ʱ��ֵ
	nCapValue = (int)(m_pUI->GetCapValueFloat(UDSCAP_POWEROFF_TIME));
	m_slider_offtime.SetPos(nCapValue);
	SetDlgItemText(IDC_SET_STATIC_OFFTIME, strText);
	m_setmap[UDSCAP_POWEROFF_TIME] = float(nCapValue);

	//��ֽת����
	nCapValue = (int)(m_pUI->GetCapValueBool(UDSCAP_TURNVIDEO));
	m_check_turnvideo.SetCheck(nCapValue);
	//��ʾɨ�����
	nCapValue = (int)(m_pUI->GetCapValueBool(CAP_INDICATORS));
	m_check_indicator.SetCheck(nCapValue);
	//������ť
	nCapValue = (int)(m_pUI->GetCapValueBool(UDSCAP_EMERGENCY));
	m_check_enmergency.SetCheck(nCapValue);
	UpdateData(FALSE);
}


BOOL CPage_Set::OnInitDialog()
{
	__super::OnInitDialog();
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	InitSliderCtrl(); //��ʼ������ Ҫ����UpdateControls֮ǰ���������û���Ĳ�����Ч
	InitSetmap();
	UpdateControls();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CPage_Set::InitSetmap(void)
{
	m_setmap.erase(m_setmap.begin(),m_setmap.end());//���

	int nCapValue = (int)(m_pUI->GetCapValueBool(UDSCAP_POWERSAVING));
	m_setmap[UDSCAP_POWERSAVING] = (float)nCapValue;
	nCapValue = (int)(m_pUI->GetCapValueBool(UDSCAP_POWEROFF));
	m_setmap[UDSCAP_POWEROFF] = (float)nCapValue;

	float value = m_pUI->GetCapValueFloat(UDSCAP_POWERSAVING_TIME);
	m_setmap[UDSCAP_POWERSAVING_TIME] = value; // ��ʼ��ʱ���UDSCAP_MULTISTREAM_VALUE����֤SetCapValue()����¸�Cap��ֵ
	value = m_pUI->GetCapValueFloat(UDSCAP_POWEROFF_TIME);
	m_setmap[UDSCAP_POWEROFF_TIME] = value;
}

void CPage_Set::InitSliderCtrl()
{
	float fMin,fMax,fStep;
	m_pUI->GetCapRangeFloat(UDSCAP_POWERSAVING_TIME, fMin, fMax, fStep);
	m_slider_savepower.SetRange((int)fMin, (int)fMax, TRUE);
	m_slider_savepower.SetTicFreq((int)fStep); //����

	m_pUI->GetCapRangeFloat(UDSCAP_POWEROFF_TIME, fMin, fMax, fStep);
	m_slider_offtime.SetRange((int)fMin, (int)fMax, TRUE);
	m_slider_offtime.SetTicFreq((int)fStep); //����

	UpdateData(FALSE);  // ���¿ؼ�
}

//�ڵ�ģʽ
void CPage_Set::OnSet_Btn_Check_SavePower()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nval;
	if (m_check_savepower.GetCheck())
	{
		nval = TWSP_AUTO;
	} 
	else
	{
		nval = TWSP_DISABLE;
	}
	m_setmap[UDSCAP_POWERSAVING] = (float)nval;
	m_pUI->SetCapValueInt(UDSCAP_POWERSAVING,nval);
	SetSavePower();
}

//�ػ�ʱ��
void CPage_Set::OnSet_Btn_Check_OffTime()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nval;
	if (m_check_offtime.GetCheck())
	{
		nval = TWOT_AUTO;
	} 
	else
	{
		nval = TWOT_DISABLE;
	}
	m_setmap[UDSCAP_POWEROFF] = (float)nval;
	m_pUI->SetCapValueInt(UDSCAP_POWEROFF,nval);	
	SetOffTime();
}


void CPage_Set::OnNMCustomdrawSet_Slider_SavePower(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);  // ��������
	CString str;
	int sldValue = m_slider_savepower.GetPos();  // ��ȡ���鵱ǰλ��
	m_setmap[UDSCAP_POWERSAVING_TIME] = (float)sldValue;
	m_pUI->SetCapValueFloat(UDSCAP_POWERSAVING_TIME,(float)sldValue);

	str.Format("%d��", sldValue);
	SetDlgItemText(IDC_SET_STATIC_SAVEPOWER, str);
	UpdateData(FALSE);  // ���¿ؼ�

	*pResult = 0;
}


void CPage_Set::OnNMCustomdrawSet_Slider_Offtime(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);  // ��������
	CString str;
	int sldValue = m_slider_offtime.GetPos();  // ��ȡ���鵱ǰλ��
	
	int sldValue_temp = m_slider_savepower.GetPos();  
	if(sldValue <= sldValue_temp)//�ػ�ʱ�������ڽڵ�ģʽ���õ�ʱ��
	{
		sldValue = sldValue_temp;
	}
	
	m_setmap[UDSCAP_POWEROFF_TIME] = (float)sldValue;
	m_pUI->SetCapValueFloat(UDSCAP_POWEROFF_TIME,(float)sldValue);
	
	str.Format("%d��", sldValue);
	SetDlgItemText(IDC_SET_STATIC_OFFTIME, str);
	UpdateData(FALSE);  // ���¿ؼ�
	
	*pResult = 0;
}


void CPage_Set::SetSavePower(void)
{
	if(m_check_savepower.GetCheck())
	{
		m_slider_savepower.EnableWindow(TRUE);
	}
	else
	{
		m_slider_savepower.EnableWindow(FALSE);
	}
}


void CPage_Set::SetOffTime(void)
{
	if(m_check_offtime.GetCheck())
	{
		m_slider_offtime.EnableWindow(TRUE);
	}
	else
	{
		m_slider_offtime.EnableWindow(FALSE);
	}
}


bool CPage_Set::LoadDLL()
{
	//::MessageBox(g_hwndDLG,TEXT("Into LoadDLL!"),MB_CAPTION,MB_OK);
	TCHAR DllPath[MAX_PATH];

	if (DEVICE_G6400 == g_nDeviceNumber)
	{
		GetFilePath(FILENAME_DLL_GL1, DllPath);
	} 
	else
	{
		GetFilePath(FILENAME_DLL_GL2, DllPath);
	}

	m_hDLL = LoadLibrary(DllPath);
	if(m_hDLL == NULL)
	{
		::MessageBox(NULL, TEXT("Load Dll Failed!"), MB_CAPTION, MB_OK);
		return false;
	}

	InitializeDriver = (InitializeDriverProc)GetProcAddress(m_hDLL, "InitializeDriver");
	if(InitializeDriver == NULL)
	{
		::MessageBox(NULL, TEXT("Load InitializeDriver Failed!"), MB_CAPTION, MB_OK);
		return false;
	}

	InitializeScanner = (InitializeScannerProc)GetProcAddress(m_hDLL, "InitializeScanner");
	if(InitializeScanner == NULL)
	{
		return false;
	}
	
	TerminateDriver = (TerminateDriverProc)GetProcAddress(m_hDLL, "TerminateDriver");
	if(TerminateDriver == NULL)
	{
		return false;
	}

	ReadNVMData = (ReadNVMDataProc)GetProcAddress(m_hDLL, "ReadNVMData");
	if(ReadNVMData == NULL)
	{
		return false;
	}
	
	SetNVRAMValue = (SetNVRAMValueProc)GetProcAddress(m_hDLL, "SetNVRAMValue");
	if(SetNVRAMValue == NULL)
	{
		return false;
	}

	if (FALSE == InitializeDriver())
	{
		return false;
	}

	if(FALSE == InitializeScanner())
	{
		return false;
	}

	return true;
}

void CPage_Set::OnSet_Btn_Check_TurnVideo()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nval;
	if (m_check_turnvideo.GetCheck())
	{
		nval = TWTV_AUTO;
	} 
	else
	{
		nval = TWTV_DISABLE;
	}
	m_pUI->SetCapValueInt(UDSCAP_TURNVIDEO,nval);
}


BOOL CPage_Set::OnSetActive()
{
	// TODO: �ڴ����ר�ô����/����û���
	m_pUI->PreViewStatus();
	UpdateControls();
	return __super::OnSetActive();
}


void CPage_Set::OnSet_Btn_Check_Showschedule()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nval;
	if(m_check_indicator.GetCheck())
	{
		nval = TRUE;
	} 
	else
	{
		nval = FALSE;
	}
	m_pUI->SetCapValueInt(CAP_INDICATORS,nval);
}


void CPage_Set::OnSet_Btn_Check_Emergency()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nval;
	if (m_check_enmergency.GetCheck())
	{
		nval = TRUE;
	} 
	else
	{
		nval = FALSE;
	}
	m_pUI->SetCapValueInt(UDSCAP_EMERGENCY,nval);
}


//void CPage_Set::OnSet_Btn_Default()
//{
//	// TODO: �ڴ���ӿؼ�֪ͨ����������
//}

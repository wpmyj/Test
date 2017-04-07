// Page_Set.cpp : 实现文件
//

#include "stdafx.h"
#include "UDS Scanner.h"
#include "Page_Set.h"
#include "afxdialogex.h"

extern int g_nDeviceNumber;
extern void GetFilePath( char* szFileName, char* szFilePath);
// CPage_Set 对话框

IMPLEMENT_DYNAMIC(CPage_Set, CPropertyPage)

CPage_Set::CPage_Set(MFC_UI *pUI)
	: m_pUI(pUI), CPropertyPage(CPage_Set::IDD)
{
	m_hDLL = NULL;
}

CPage_Set::~CPage_Set()
{
	m_setmap.swap(map<int, float>());  // 清空并释放内存
}

void CPage_Set::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SET_CHECK_SAVEPOWER, m_check_savepower);
	DDX_Control(pDX, IDC_SET_CHECK_OFFTIME, m_check_offtime);
	DDX_Control(pDX, IDC_SET_SLIDER_OFFTIME, m_slider_offtime);
	DDX_Control(pDX, IDC_SET_SLIDER_SAVEPOWER, m_slider_savepower);
	//  DDX_Control(pDX, IDC_SET_EDIT_SAVEPOWER, m_edit_savepower);
	//  DDX_Control(pDX, IDC_SET_EDIT_OFFTIME, m_edit_offtime);
	DDX_Control(pDX, IDC_SET_CHECK_TURNVIDEO, m_check_turnvideo);
}


BEGIN_MESSAGE_MAP(CPage_Set, CPropertyPage)
	ON_BN_CLICKED(IDC_SET_CHECK_SAVEPOWER, &CPage_Set::OnSet_Btn_Check_SavePower)
	ON_BN_CLICKED(IDC_SET_CHECK_OFFTIME, &CPage_Set::OnSet_Btn_Check_OffTime)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SET_SLIDER_SAVEPOWER, &CPage_Set::OnNMCustomdrawSet_Slider_SavePower)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SET_SLIDER_OFFTIME, &CPage_Set::OnNMCustomdrawSet_Slider_Offtime)
//	ON_WM_HSCROLL()
//ON_EN_CHANGE(IDC_SET_EDIT_SAVEPOWER, &CPage_Set::OnEnChangeSet_Edit_Savepower)
//ON_EN_CHANGE(IDC_SET_EDIT_OFFTIME, &CPage_Set::OnEnChangeSet_Edit_Offtime)
ON_BN_CLICKED(IDC_SET_CHECK_TURNVIDEO, &CPage_Set::OnSet_Btn_Check_TurnVideo)
END_MESSAGE_MAP()


// CPage_Set 消息处理程序
void CPage_Set::SetCapValue(void)
{
	MAP_CAP::iterator iter;
	if (true == m_setmap.empty())
	{
		return;
	}
	for(iter = m_setmap.begin(); iter != m_setmap.end(); iter++)
	{
		switch(iter->first)
		{
		case UDSCAP_POWERSAVING: //节电模式
		case UDSCAP_POWEROFF: //关机时间
		case UDSCAP_TURNVIDEO: //扫描仪无纸时转高拍仪
			{
				m_pUI->SetCapValueInt(iter->first,(int)(iter->second));
				break;
			}	

		case UDSCAP_POWERSAVING_TIME: //节电模式值
		case UDSCAP_POWEROFF_TIME: //关机时间值
			{			
				m_pUI->SetCapValueFloat(iter->first,iter->second);
				break;
			}
		}
	}

	// 设置待机时间与关机时间
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
	int nCapValue;
	int nval;
	CString strText;

	//节电模式
	nCapValue = (int)(m_pUI->GetCapValueBool(UDSCAP_POWERSAVING));
	m_check_savepower.SetCheck(nCapValue);
	SetSavePower();
    m_setmap[UDSCAP_POWERSAVING] = float(nCapValue);
	//节电模式值
	nCapValue = (int)(m_pUI->GetCapValueFloat(UDSCAP_POWERSAVING_TIME));
	m_slider_savepower.SetPos(nCapValue);
	SetDlgItemText(IDC_SET_STATIC_SAVEPOWER, strText);
	m_setmap[UDSCAP_POWERSAVING_TIME] = float(nCapValue);

	//关机时间
	nCapValue = (int)(m_pUI->GetCapValueBool(UDSCAP_POWEROFF));
	m_check_offtime.SetCheck(nCapValue);
	SetOffTime();
    m_setmap[UDSCAP_POWEROFF] = float(nCapValue);
	//关机时间值
	nCapValue = (int)(m_pUI->GetCapValueFloat(UDSCAP_POWEROFF_TIME));
	m_slider_offtime.SetPos(nCapValue);
	SetDlgItemText(IDC_SET_STATIC_OFFTIME, strText);
	m_setmap[UDSCAP_POWEROFF_TIME] = float(nCapValue);

	//关机时间
	nCapValue = (int)(m_pUI->GetCapValueBool(UDSCAP_TURNVIDEO));
	m_check_turnvideo.SetCheck(nCapValue);
}


BOOL CPage_Set::OnInitDialog()
{
	__super::OnInitDialog();

	// TODO:  在此添加额外的初始化
	InitSliderCtrl(); //初始化滑块 要放在UpdateControls之前，否则设置滑块的步长无效
	UpdateControls();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CPage_Set::InitSliderCtrl()
{
	float fMin,fMax,fStep;
	m_pUI->GetCapRangeFloat(UDSCAP_POWERSAVING_TIME, fMin, fMax, fStep);
	m_slider_savepower.SetRange((int)fMin, (int)fMax, TRUE);
	m_slider_savepower.SetTicFreq((int)fStep); //步长

	m_pUI->GetCapRangeFloat(UDSCAP_POWEROFF_TIME, fMin, fMax, fStep);
	m_slider_offtime.SetRange((int)fMin, (int)fMax, TRUE);
	m_slider_offtime.SetTicFreq((int)fStep); //步长

	UpdateData(FALSE);  // 更新控件
}

//节电模式
void CPage_Set::OnSet_Btn_Check_SavePower()
{
	// TODO: 在此添加控件通知处理程序代码
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
	SetSavePower();
}

//关机时间
void CPage_Set::OnSet_Btn_Check_OffTime()
{
	// TODO: 在此添加控件通知处理程序代码
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
	SetOffTime();
}


void CPage_Set::OnNMCustomdrawSet_Slider_SavePower(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);  // 接收数据
	CString str;
	int sldValue = m_slider_savepower.GetPos();  // 获取滑块当前位置
	m_setmap[UDSCAP_POWERSAVING_TIME] = (float)sldValue;

	str.Format("%d分", sldValue);
	SetDlgItemText(IDC_SET_STATIC_SAVEPOWER, str);
	UpdateData(FALSE);  // 更新控件

	*pResult = 0;
}


void CPage_Set::OnNMCustomdrawSet_Slider_Offtime(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);  // 接收数据
	CString str;
	int sldValue = m_slider_offtime.GetPos();  // 获取滑块当前位置
	
	int sldValue_temp = m_slider_savepower.GetPos();  
	if(sldValue <= sldValue_temp)//关机时间必须大于节电模式设置的时间
	{
		sldValue = sldValue_temp;
		//m_slider_offtime.SetPos(sldValue);
	}
	
	m_setmap[UDSCAP_POWEROFF_TIME] = (float)sldValue;
	
	str.Format("%d分", sldValue);
	SetDlgItemText(IDC_SET_STATIC_OFFTIME, str);
	UpdateData(FALSE);  // 更新控件
	
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
		::MessageBox(NULL, TEXT("Load InitializeScanner Failed!"), MB_CAPTION, MB_OK);
		return false;
	}
	
	TerminateDriver = (TerminateDriverProc)GetProcAddress(m_hDLL, "TerminateDriver");
	if(TerminateDriver == NULL)
	{
		::MessageBox(NULL, TEXT("Load TerminateDriver Failed!"), MB_CAPTION, MB_OK);
		return false;
	}

	ReadNVMData = (ReadNVMDataProc)GetProcAddress(m_hDLL, "ReadNVMData");
	if(ReadNVMData == NULL)
	{
		::MessageBox(NULL, TEXT("Load ReadNVMData Failed!"), MB_CAPTION, MB_OK);
		return false;
	}
	
	SetNVRAMValue = (SetNVRAMValueProc)GetProcAddress(m_hDLL, "SetNVRAMValue");
	if(SetNVRAMValue == NULL)
	{
		::MessageBox(NULL, TEXT("Load SetNVRAMValue Failed!"), MB_CAPTION, MB_OK);
		return false;
	}

	if (FALSE == InitializeDriver())
	{
		::MessageBox(NULL, TEXT("Run InitializeDriver Failed!"), MB_CAPTION, MB_OK);
		return false;
	}

	if(FALSE == InitializeScanner())
	{
		::MessageBox(NULL, TEXT("Run InitializeScanner Failed!"), MB_CAPTION, MB_OK);
		return false;
	}

	return true;
}

void CPage_Set::OnSet_Btn_Check_TurnVideo()
{
	// TODO: 在此添加控件通知处理程序代码
	int nval;
	if (m_check_turnvideo.GetCheck())
	{
		nval = TWTV_AUTO;
	} 
	else
	{
		nval = TWTV_DISABLE;
	}
	m_setmap[UDSCAP_TURNVIDEO] = (float)nval;
}


BOOL CPage_Set::OnSetActive()
{
	// TODO: 在此添加专用代码和/或调用基类
	m_pUI->PreViewStatus();
	UpdateControls();
	return __super::OnSetActive();
}

#include "MFC_UI.h"
//#include "UDS Scanner.h"
#include "Sheet_Scanner.h"  // 必须放cpp文件中，因为重复包含
#include "public.h"
#include "CDlg_Camera.h"
/**
* @file   MFC_UI.cpp
* @brief This is a brief description.
* @author UDS
* @par   Copyright (c) 2016 UDS.
*         All Rights Reserved.
* @date   2016/9/12 
* @note   matters needing attention
*/ 
extern HINSTANCE   g_hinstance;
extern HWND   g_hwndDLG;
extern int g_nDeviceNumber;  // 设备编号
HWND g_hwndSheet;



CTWAIN_UI* CreateUI(CTWAINDS_UDS *pDS)
{
	return new MFC_UI(pDS);
}

void DestroyUI(CTWAIN_UI* pUI)
{
	if(pUI)
	{
		delete ((MFC_UI*)pUI);
		//AfxMessageBox("DestroyUI");
	}
}

//MFC_UI* MFC_UI::m_mfcUi = NULL;

//MFC_UI::MFC_UI(void)
//{
//}

MFC_UI::MFC_UI(CTWAINDS_UDS *pDS):CTWAIN_UI(pDS)
{
	m_pSheet = NULL;
	m_pDlgCamera = NULL;
}


MFC_UI::~MFC_UI(void)
{
	if (DEVICE_G6400 == g_nDeviceNumber)
	{
		m_pSheet->DestroyWindow();
	}
	
	if (m_pSheet)
	{
		delete m_pSheet;
		m_pSheet = NULL;		
		//AfxMessageBox("MFC 析构");
	}

	if (DEVICE_CAMERA == g_nDeviceNumber)
	{
		if (m_pDlgCamera)
		{
			//::MessageBox(g_hwndDLG,TEXT(":~MFC_UI()!"),MB_CAPTION,MB_OK);
			m_pDlgCamera->DestroyWindow();
			delete m_pDlgCamera;
			m_pDlgCamera = NULL;
		}
	}
	
}

TW_INT16 MFC_UI::DisplayTWAINGUI(TW_USERINTERFACE Data, bool bSetup, bool bIndicators)
{
	
  TW_INT16 nRes = CTWAIN_UI::DisplayTWAINGUI(Data, bSetup, bIndicators);
	//::MessageBox(g_hwndDLG,TEXT("MFC_UI::DisplayTWAINGUI!"),MB_CAPTION,MB_OK);
	if(nRes)
	{
		return nRes;
	}
	if(bSetup)
	{
		Data.ShowUI=1;
	}

	if (DEVICE_CAMERA != g_nDeviceNumber)  // Camera始终显示界面
	{
		if(Data.ShowUI==0 && !bIndicators)
		{
			return TWRC_SUCCESS;
		}
	}

	HWND hMainWnd =  (HWND)Data.hParent;
	CWnd *pMainWnd = CWnd::FromHandle(hMainWnd);
	
	if (DEVICE_CAMERA == g_nDeviceNumber)  // 进入Camera界面
	{
		m_pDlgCamera = new CDlg_Camera(this);
		if (m_pDlgCamera)
		{
			m_pDlgCamera->Create(CDlg_Camera::IDD,pMainWnd);
			m_pDlgCamera->ShowWindow(SW_SHOW);
		} 
		else
		{
			return TWRC_FAILURE;
		}
	}
	else // 其它界面: FREEIMAGE \ OPENCV \ G6400
	{
		if(Data.ShowUI)
		{
			switch (g_nDeviceNumber)
			{
			case DEVICE_OPENCV:
			case DEVICE_FREEIMAGE:
				{
					m_pSheet = new CSheet_Scanner(this,IDS_DS_CAPTION);
					if (m_pSheet)
					{
						m_pSheet->DoModal();
					} 
					else 
					{
						return TWRC_FAILURE;
					}
					break;
				}			
			case DEVICE_G6400:
				{
					m_pSheet = new CSheet_Scanner(this,IDS_DS_CAPTION);
					if (m_pSheet) 
					{
						m_pSheet->Create(pMainWnd);
						m_pSheet->ShowWindow(SW_SHOW);
						g_hwndSheet = m_pSheet->GetSafeHwnd();
					} 
					else 
					{
						return TWRC_FAILURE;
					}
					break;
				}
			default:
				{
					::MessageBox(g_hwndDLG,TEXT("不支持的设备！"),MB_CAPTION,MB_OK);
					return TWRC_FAILURE;
					break;
				}
			}

		}	
	
	}
	return TWRC_SUCCESS;

}

void MFC_UI::DestroyTWAINGUI()
{
	CTWAIN_UI::DestroyTWAINGUI();
	//::MessageBox(g_hwndDLG,TEXT("MFC_UI::DestroyTWAINGUI!"),MB_CAPTION,MB_OK);
	if (m_pSheet)
	{
		delete m_pSheet;
		m_pSheet = NULL;	
		//::MessageBox(g_hwndDLG,TEXT("delete m_pSheet!"),MB_CAPTION,MB_OK);
	}
	//::MessageBox(g_hwndDLG,TEXT("before m_pDlgCamera!"),MB_CAPTION,MB_OK);

	if (DEVICE_CAMERA == g_nDeviceNumber)
	{
		if (m_pDlgCamera)
		{
			//m_pDlgCamera->DestroyWindow();
			delete m_pDlgCamera;
			m_pDlgCamera = NULL;
		}
	}

	//::MessageBox(g_hwndDLG,TEXT("end!"),MB_CAPTION,MB_OK);
}


void MFC_UI::UpdateProgress(bool bShow, unsigned char ucProgress, 
	unsigned int unPageNo, string strProgressTitle)
{
	;
}

unsigned int MFC_UI::MessageBox(string strMessage,string strTitle, unsigned int unIconID)
{
	//QMessageBox msgBox(QMessageBox::NoIcon, strTitle.c_str(),strMessage.c_str());
	//return msgBox.exec();
	//return ::MessageBox(m_pDlg->m_hWnd,strMessage.c_str(),strTitle.c_str(),unIconID);
	return ::MessageBox(m_pSheet->m_hWnd,strMessage.c_str(),strTitle.c_str(),unIconID);
	//return 0;
}


bool MFC_UI::processEvent(pTW_EVENT _pEvent)
{
//#ifdef TWNDS_OS_WIN
//	if(qApp)
//	{
//		qApp->sendPostedEvents();
//	}
//#endif
//	return false;
	//？？
	return false;
}


//void MFC_UI::ShowDlg()     
//{     
//
//	hwndDLG = CreateDialog(g_hinstance,MAKEINTRESOURCE(IDD_DLG_SHOW),     
//		NULL,(DLGPROC)DlgProc);     
//	ShowWindow(hwndDLG,   SW_SHOW);     
//
//}


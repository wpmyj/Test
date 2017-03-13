#include "MFC_UI.h"
//#include "UDS Scanner.h"
#include "Sheet_Scanner.h"  // 必须放cpp文件中，因为重复包含
#include "public.h"
#include "Dlg_Video.h"
//#include "Dlg_Camera.h"
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
	m_pDlgVideo = NULL;
	//m_pDlgCamera = NULL;
}


MFC_UI::~MFC_UI(void)
{
	//if (DEVICE_G6400 == g_nDeviceNumber)
	//{
	//	m_pSheet->DestroyWindow();
	//}

	if (DEVICE_CAMERA == g_nDeviceNumber)
	{
		if (m_pDlgVideo)
		{
			//::MessageBox(g_hwndDLG,TEXT(":~MFC_UI()!"),MB_CAPTION,MB_OK);
			m_pDlgVideo->DestroyWindow();
			delete m_pDlgVideo;
			m_pDlgVideo = NULL;
		}
	}
	else
	{
		if (m_pSheet)
		{
			delete m_pSheet;
			m_pSheet = NULL;		
		}
	}
	
}


TW_INT16 MFC_UI::DisplayTWAINGUI(TW_USERINTERFACE Data, bool bSetup, bool bIndicators)
{
	//::MessageBox(g_hwndDLG,TEXT("MFC_UI::DisplayTWAINGUI!"),MB_CAPTION,MB_OK);
  TW_INT16 nRes = CTWAIN_UI::DisplayTWAINGUI(Data, bSetup, bIndicators);
	
	if(nRes)
	{
		return nRes;
	}
	if(bSetup)
	{
		Data.ShowUI=1;
		if (DEVICE_CAMERA == g_nDeviceNumber)  // 高拍仪EnableDSUIOnly,不显示界面
		{
			return TWRC_FAILURE;
		}
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
		//::MessageBox(g_hwndDLG,TEXT("DEVICE_CAMERA == g_nDeviceNumber!"),MB_CAPTION,MB_OK);
		m_pDlgVideo = new CDlg_Video(this);
		if (m_pDlgVideo)
		{
			m_pDlgVideo->Create(CDlg_Video::IDD,pMainWnd);
			m_pDlgVideo->ShowWindow(SW_SHOW);
		} 
		else
		{
			return TWRC_FAILURE;
		}
	}
	else // 其它界面: OPENCV 
	{
		if(Data.ShowUI)
		{
			switch (g_nDeviceNumber)
			{
			case DEVICE_OPENCV:
			case DEVICE_G6400:
			case DEVICE_G6600:
			//case DEVICE_FREEIMAGE:
				{
					//::MessageBox(g_hwndDLG,TEXT("new CSheet_Scanner!"),MB_CAPTION,MB_OK);
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
			//case DEVICE_G6400:
			//	{
			//		m_pSheet = new CSheet_Scanner(this,IDS_DS_CAPTION);
			//		if (m_pSheet) 
			//		{
			//			m_pSheet->Create(pMainWnd);
			//			m_pSheet->ShowWindow(SW_SHOW);
			//			g_hwndSheet = m_pSheet->GetSafeHwnd();
			//		} 
			//		else 
			//		{
			//			return TWRC_FAILURE;
			//		}
			//		break;
			//	}
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
	
	if (DEVICE_CAMERA == g_nDeviceNumber)
	{
		if (m_pDlgVideo)
		{
			m_pDlgVideo->DestroyWindow();
			delete m_pDlgVideo;
			m_pDlgVideo = NULL;
		}
	}
	else  // 其它设备
	{
		if (m_pSheet)
		{
			m_pSheet->DestroyWindow();
			delete m_pSheet;
			m_pSheet = NULL;	
			//::MessageBox(g_hwndDLG,TEXT("delete m_pSheet!"),MB_CAPTION,MB_OK);
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


//重命名模板名  add by zhu
bool MFC_UI::RenameProfile(string strOldName, string strNewName) 
{
	if(!strOldName.empty())
	{
		strOldName = m_strProfilesDirectory + strOldName + FILEEXTENTION;
		strNewName = m_strProfilesDirectory + strNewName + FILEEXTENTION;
		
		return rename(strOldName.c_str(), strNewName.c_str())==0; // strOldName.c_str():string->CString
		//rename成功返回0，失败返回-1
	}
	else
	{	
		return false;
	}
}

string MFC_UI::GetProfileNamePath()
{
	return m_strProfilesDirectory;
}

void MFC_UI::PreViewStatus()
{
	m_pSheet->SetPreViewStatus();
}
#include "stdafx.h"
#include "ShowIndicators.h"
#include "CTWAINDS_UDS.h"

IMPLEMENT_DYNCREATE(CShowIndicators, CWinThread)

CShowIndicators::CShowIndicators(void)
{
	m_pDlgIndicators = NULL;
}


CShowIndicators::~CShowIndicators(void)
{
}


BOOL CShowIndicators::InitInstance()
{
	// TODO: 在此添加专用代码和/或调用基类

	return TRUE;
	//return CWinThread::InitInstance();
}


int CShowIndicators::ExitInstance()
{
	// TODO: 在此添加专用代码和/或调用基类

	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CShowIndicators, CWinThread)
	ON_THREAD_MESSAGE(WM_THREADINFO,&CShowIndicators::OnThreadInfo)
END_MESSAGE_MAP()

void CShowIndicators::OnThreadInfo(WPARAM wParam,LPARAM lParam)
{
	PageInfo* pPageInfo = (PageInfo*)lParam;
	CTWAINDS_UDS *pDS = (CTWAINDS_UDS*)lParam;
	switch (wParam)
	{
	case INDICATORS_SHOWUI:
			m_pDlgIndicators = new CDlg_Indicators;
			m_pDlgIndicators->Create(CDlg_Indicators::IDD);
			m_pDlgIndicators->UpdateData(FALSE);
			m_pDlgIndicators->ShowWindow(SW_SHOW);
			m_pDlgIndicators->StartTimer();
		break;
	case INDICATORS_PROGRESS:
		m_pDlgIndicators->UpdateProgress(lParam);	
		break;
	case INDICATORS_PAGEINFO:		
		m_pDlgIndicators->UpdatePageInfo(pPageInfo->strPageCount,
			pPageInfo->strPageSize, pPageInfo->strTotalSize);
		break;
	case INDICATORS_CANCEL:
		m_pDlgIndicators->OnCancel();
		break;
	case 	INDICATORS_DESTROY:
		m_pDlgIndicators->EndTimer();
		m_pDlgIndicators->DestroyWindow();
		break;
	case INDICATORS_SPEED:
		m_pDlgIndicators->UpdateSpeed(lParam);
		break;
	case INDICATORS_DS:
		m_pDlgIndicators->m_pDS = pDS;
		break;
	default:
		break;
	}
}

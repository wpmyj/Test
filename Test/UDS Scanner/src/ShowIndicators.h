/**
* @file   ShowIndicators.h
* @brief 用户界面线程类.
* @author Tony.
* @par   Copyright (c) 2017 UDS. All Rights Reserved.
* @date  05/05/2017.
*/ 
#pragma once
#include "afxwin.h"
#include "Dlg_Indicators.h"

class CShowIndicators : public CWinThread
{
	DECLARE_DYNCREATE(CShowIndicators)

protected:
	CShowIndicators(void);
	virtual ~CShowIndicators(void);

public:
	CDlg_Indicators *m_pDlgIndicators;


public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	afx_msg void OnThreadInfo(WPARAM wParam,LPARAM lParam);

protected:
	DECLARE_MESSAGE_MAP()
};


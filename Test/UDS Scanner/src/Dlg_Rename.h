#pragma once
#include "afxwin.h"

/**
* @file   CDlg_Rename.h
* @brief  �������Ի�����
* @author UDS
* @par    Copyright (c) 2016 UDS.
*         All Rights Reserved.
* @date   2017/2/7 
*/ 

// CDlg_Rename �Ի���

class CDlg_Rename : public CDialog
{
	DECLARE_DYNAMIC(CDlg_Rename)

public:
	CDlg_Rename(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlg_Rename();

// �Ի�������
	enum { IDD = IDD_DLG_RENAME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual void OnOK();

	DECLARE_MESSAGE_MAP()	

public:
	CEdit m_edit_newname;

public:
	CString GetNewName();  ///< ��ȡ������
	BOOL m_bOk;		///< �ж��Ƿ���ȷ����ť

private:
	CString m_strNewName;  ///< ��ǰ������
};

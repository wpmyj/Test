#pragma once
#include "afxwin.h"


// CDlg_Error �Ի���

class CDlg_Error : public CDialogEx
{
	DECLARE_DYNAMIC(CDlg_Error)

public:
	//CDlg_Error(CWnd* pParent = NULL);   // ��׼���캯��
	CDlg_Error(CString strMsg, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlg_Error();

// �Ի�������
	enum { IDD = IDD_DLG_ERROR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CStatic m_static_msg;

protected:
	CString m_strMsg;
public:
	virtual BOOL OnInitDialog();
};

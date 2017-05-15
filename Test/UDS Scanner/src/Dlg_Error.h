#pragma once
#include "afxwin.h"


// CDlg_Error 对话框

class CDlg_Error : public CDialogEx
{
	DECLARE_DYNAMIC(CDlg_Error)

public:
	//CDlg_Error(CWnd* pParent = NULL);   // 标准构造函数
	CDlg_Error(CString strMsg, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlg_Error();

// 对话框数据
	enum { IDD = IDD_DLG_ERROR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CStatic m_static_msg;

protected:
	CString m_strMsg;
public:
	virtual BOOL OnInitDialog();
};

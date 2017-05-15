// Dlg_Error.cpp : 实现文件
//

#include "stdafx.h"
#include "UDS Scanner.h"
#include "Dlg_Error.h"
#include "afxdialogex.h"


// CDlg_Error 对话框

IMPLEMENT_DYNAMIC(CDlg_Error, CDialogEx)

//CDlg_Error::CDlg_Error(CWnd* pParent /*=NULL*/)
//	: CDialogEx(CDlg_Error::IDD, pParent)
//{
//
//}

CDlg_Error::CDlg_Error(CString strMsg, CWnd* pParent /*=NULL*/)
	: m_strMsg(strMsg), CDialogEx(CDlg_Error::IDD, pParent)
{

}

CDlg_Error::~CDlg_Error()
{
}

void CDlg_Error::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ERROR_STATIC_MSG, m_static_msg);
}


BEGIN_MESSAGE_MAP(CDlg_Error, CDialogEx)
END_MESSAGE_MAP()


// CDlg_Error 消息处理程序


BOOL CDlg_Error::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_static_msg.SetWindowText(m_strMsg);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

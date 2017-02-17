// Dlg_Rename.cpp : 实现文件
//

#include "stdafx.h"
#include "UDS Scanner.h"
#include "Dlg_Rename.h"
#include "afxdialogex.h"


// CDlg_Rename 对话框

IMPLEMENT_DYNAMIC(CDlg_Rename, CDialog)

CDlg_Rename::CDlg_Rename(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_Rename::IDD, pParent)
{
	m_bOk = FALSE;
}

CDlg_Rename::~CDlg_Rename()
{
}

void CDlg_Rename::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RENAME_EDIT_NEWNAME, m_edit_newname);
}


BEGIN_MESSAGE_MAP(CDlg_Rename, CDialog)
END_MESSAGE_MAP()


// CDlg_Rename 消息处理程序


void CDlg_Rename::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	UpdateData(TRUE);  // 接收数据
	m_edit_newname.GetWindowText(m_strNewName);  // 获取Edit内容
	if(m_strNewName.IsEmpty())
	{
		m_bOk = FALSE; 
		AfxMessageBox("新名称为空，请重新输入！");
	}
	else
	{
		m_bOk = TRUE; 
		CDialog::OnOK();
	}
}


CString CDlg_Rename::GetNewName()
{
	return m_strNewName;
}

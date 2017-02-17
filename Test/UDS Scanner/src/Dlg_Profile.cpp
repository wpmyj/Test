// Dlg_Profile.cpp : 实现文件
//

#include "stdafx.h"
#include "UDS Scanner.h"
#include "Dlg_Profile.h"
#include "afxdialogex.h"

// CDlg_Profile 对话框

IMPLEMENT_DYNAMIC(CDlg_Profile, CDialog)

CDlg_Profile::CDlg_Profile(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_Profile::IDD, pParent)
{
		m_bOk = FALSE;
}

CDlg_Profile::~CDlg_Profile()
{
}

void CDlg_Profile::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROFILE_EDIT_PROFILENAME, m_edit_profile);
}


BEGIN_MESSAGE_MAP(CDlg_Profile, CDialog)
END_MESSAGE_MAP()

CString CDlg_Profile::GetProfileName()
{
  return m_strProfileName;
}


// CDlg_Profile 消息处理程序

void CDlg_Profile::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	UpdateData(TRUE);  // 接收数据
	m_edit_profile.GetWindowText(m_strProfileName);  // 获取Edit内容
	if(m_strProfileName.IsEmpty())
	{
		m_bOk = FALSE; 
		AfxMessageBox("模板名为空，请重新输入！");
	}
	else
	{
		m_bOk = TRUE; 
		CDialog::OnOK();
	}
}

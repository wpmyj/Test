// Dlg_Rename.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "UDS Scanner.h"
#include "Dlg_Rename.h"
#include "afxdialogex.h"


// CDlg_Rename �Ի���

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


// CDlg_Rename ��Ϣ�������


void CDlg_Rename::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���
	UpdateData(TRUE);  // ��������
	m_edit_newname.GetWindowText(m_strNewName);  // ��ȡEdit����
	if(m_strNewName.IsEmpty())
	{
		m_bOk = FALSE; 
		AfxMessageBox("������Ϊ�գ����������룡");
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

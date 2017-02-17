// Dlg_Profile.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "UDS Scanner.h"
#include "Dlg_Profile.h"
#include "afxdialogex.h"

// CDlg_Profile �Ի���

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


// CDlg_Profile ��Ϣ�������

void CDlg_Profile::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���
	UpdateData(TRUE);  // ��������
	m_edit_profile.GetWindowText(m_strProfileName);  // ��ȡEdit����
	if(m_strProfileName.IsEmpty())
	{
		m_bOk = FALSE; 
		AfxMessageBox("ģ����Ϊ�գ����������룡");
	}
	else
	{
		m_bOk = TRUE; 
		CDialog::OnOK();
	}
}

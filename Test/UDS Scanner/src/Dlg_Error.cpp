// Dlg_Error.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "UDS Scanner.h"
#include "Dlg_Error.h"
#include "afxdialogex.h"


// CDlg_Error �Ի���

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


// CDlg_Error ��Ϣ�������


BOOL CDlg_Error::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_static_msg.SetWindowText(m_strMsg);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

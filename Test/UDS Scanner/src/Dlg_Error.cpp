// Dlg_Error.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "UDS Scanner.h"
#include "Dlg_Error.h"
#include "afxdialogex.h"

#define  SKIN_BACK                   _T("skin\\panel_bk.png")
#define  SKIN_BUTTON                 _T("skin\\button2.png")
#define  SKIN_BUTTON_CLOSE           _T("skin\\btn_close.png")

// ��ɫ����
/** ��ɫ */    
#define COLOR_WHITE                  RGB(254,254,254)
/** ����ɫ */    
#define COLOR_GREEN_BRIGHT           RGB(2, 232, 0)   
/** ����ɫ */    
#define COLOR_GREEN_PALE             RGB(55, 141, 0)
/** ��ɫ */    
#define COLOR_VIOLET                 RGB(129, 42, 142)
/** ��ɫ */    
#define COLOR_RED                    RGB(255, 0, 0)
/** ����ɫ */    
#define COLOR_BLUE_LIGHT             RGB(8, 150, 154)

extern void GetFilePath( char* szFileName, char* szFilePath);

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
	DDX_Control(pDX, IDOK, m_button_ok);
}


BEGIN_MESSAGE_MAP(CDlg_Error, CDialogEx)
	ON_WM_PAINT()
	ON_WM_NCHITTEST()
END_MESSAGE_MAP()


// CDlg_Error ��Ϣ�������


BOOL CDlg_Error::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	InitUI();
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CDlg_Error::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
	TCHAR path[MAX_PATH] = {0};
	CImage imageBack;
	CRect rect;
	GetClientRect(&rect);	
	GetFilePath(SKIN_BACK, path);

	if (!imageBack.IsNull()) 
	{
		imageBack.Destroy();
	}
	imageBack.Load(path);
	if (!imageBack.IsNull())
	{
		imageBack.TransparentBlt(dc.m_hDC, 0, 0, rect.Width(), rect.Height(), 
			RGB(255, 255, 255));
	}
}


LRESULT CDlg_Error::OnNcHitTest(CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	UINT nHitTest = CDialog::OnNcHitTest(point);
	// �������ڴ��ڿͻ������򷵻ر��������Ÿ�Windows
	// ʹWindows������ڱ�����������д������ɵ����ƶ�����
	return (nHitTest == HTCLIENT) ? HTCAPTION : nHitTest;
	//return CDialogEx::OnNcHitTest(point);
}

void CDlg_Error::InitUI()
{
	TCHAR path[MAX_PATH] = {0};
	GetFilePath(SKIN_BACK, path);
	m_static_msg.SetTextColor(COLOR_WHITE);	      //tc:�����ı���ɫ
	m_static_msg.SetMyFont(120,_T("΢���ź�"));			  //tc:�������弰���С
	m_static_msg.SetWindowText(m_strMsg);

	GetFilePath(SKIN_BUTTON, path);
	m_button_ok.SetButtonImage(path, CXIMAGE_FORMAT_PNG);
	m_button_ok.SetToolTips(_T("�رմ���"));
}

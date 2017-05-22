// Dlg_Error.cpp : 实现文件
//

#include "stdafx.h"
#include "UDS Scanner.h"
#include "Dlg_Error.h"
#include "afxdialogex.h"

#define  SKIN_BACK                   _T("skin\\panel_bk.png")
#define  SKIN_BUTTON                 _T("skin\\button2.png")
#define  SKIN_BUTTON_CLOSE           _T("skin\\btn_close.png")

// 颜色常量
/** 白色 */    
#define COLOR_WHITE                  RGB(254,254,254)
/** 亮绿色 */    
#define COLOR_GREEN_BRIGHT           RGB(2, 232, 0)   
/** 淡绿色 */    
#define COLOR_GREEN_PALE             RGB(55, 141, 0)
/** 紫色 */    
#define COLOR_VIOLET                 RGB(129, 42, 142)
/** 紫色 */    
#define COLOR_RED                    RGB(255, 0, 0)
/** 淡蓝色 */    
#define COLOR_BLUE_LIGHT             RGB(8, 150, 154)

extern void GetFilePath( char* szFileName, char* szFilePath);

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
	DDX_Control(pDX, IDOK, m_button_ok);
}


BEGIN_MESSAGE_MAP(CDlg_Error, CDialogEx)
	ON_WM_PAINT()
	ON_WM_NCHITTEST()
END_MESSAGE_MAP()


// CDlg_Error 消息处理程序


BOOL CDlg_Error::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	InitUI();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlg_Error::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()
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
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	UINT nHitTest = CDialog::OnNcHitTest(point);
	// 如果鼠标在窗口客户区，则返回标题条代号给Windows
	// 使Windows按鼠标在标题条上类进行处理，即可单击移动窗口
	return (nHitTest == HTCLIENT) ? HTCAPTION : nHitTest;
	//return CDialogEx::OnNcHitTest(point);
}

void CDlg_Error::InitUI()
{
	TCHAR path[MAX_PATH] = {0};
	GetFilePath(SKIN_BACK, path);
	m_static_msg.SetTextColor(COLOR_WHITE);	      //tc:设置文本颜色
	m_static_msg.SetMyFont(120,_T("微软雅黑"));			  //tc:设置字体及其大小
	m_static_msg.SetWindowText(m_strMsg);

	GetFilePath(SKIN_BUTTON, path);
	m_button_ok.SetButtonImage(path, CXIMAGE_FORMAT_PNG);
	m_button_ok.SetToolTips(_T("关闭窗口"));
}

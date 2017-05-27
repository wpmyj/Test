// Dlg_Indicators.cpp : 实现文件
//

#include "stdafx.h"
#include "UDS Scanner.h"
#include "Dlg_Indicators.h"
#include "afxdialogex.h"

#define  GBYTES  1073741824  
#define  MBYTES  1048576  
#define  KBYTES  1024
#define  DKBYTES 1024.0 

#define  SKIN_BACK                   _T("skin\\panel_bk.png")
#define  SKIN_BUTTON                 _T("skin\\button2.png")
#define  SKIN_BUTTON_CLOSE           _T("skin\\btn_close.png")
#define  SKIN_CHECK                  _T("skin\\check.png")
#define  SKIN_PROGRESS_BACKIMAGE     _T("skin\\sliderbk.png")
#define  SKIN_PROGRESS_FRONTIMAGE    _T("skin\\sliderforebk.png")

// 颜色常量
/** 白色 */    
#define COLOR_WHITE                  RGB(254,254,254)
/** 亮绿色 */    
#define COLOR_GREEN_BRIGHT           RGB(2, 232, 0)   
/** 淡绿色 */    
#define COLOR_GREEN_PALE             RGB(55, 141, 0)
/** 紫色 */    
#define COLOR_VIOLET                 RGB(129, 42, 142)
                                 
// CDlg_Indicators 对话框

extern void GetFilePath( char* szFileName, char* szFilePath);


IMPLEMENT_DYNAMIC(CDlg_Indicators, CDialogEx)

CDlg_Indicators::CDlg_Indicators(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlg_Indicators::IDD, pParent)
{
	m_dwElaspedTime = 0;
	m_dwSpeed = 0;
	m_pDS = NULL;
}

CDlg_Indicators::~CDlg_Indicators()
{
}

void CDlg_Indicators::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_INDICATOR_STATIC_PAGESIZE, m_static_pagesize);
	DDX_Control(pDX, IDC_INDICATOR_PROGRESS_TRANSFER, m_Progress);
	DDX_Control(pDX, IDC_INDICATOR_STATIC_PROGRESS, m_static_progress);
	DDX_Control(pDX, IDC_INDICATOR_STATIC_ELAPSEDTIME, m_static_elapsedtime);
	DDX_Control(pDX, IDC_INDICATOR_STATIC_PAGECOUNT, m_static_pagecount);
	DDX_Control(pDX, IDC_INDICATOR_STATIC_PAGESIZE, m_static_pagesize);
	DDX_Control(pDX, IDC_INDICATOR_STATIC_TOTALSIZE, m_static_totalsize);
	DDX_Control(pDX, IDC_INDICATOR_STATIC_SPEED, m_static_speed);
	DDX_Control(pDX, IDOK, m_button_close);
	DDX_Control(pDX, IDCANCEL, m_button_cancel);
}


BEGIN_MESSAGE_MAP(CDlg_Indicators, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlg_Indicators::OnBnClickedOk)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDCANCEL, &CDlg_Indicators::OnCancel)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_NCHITTEST()
END_MESSAGE_MAP()

bool CDlg_Indicators::UpdatePageInfo(CString& _strPageCount, CString& _strPageSize, CString& _strTotalSize)
{
	m_static_pagecount.ShowWindow(SW_HIDE); // 刷新，否则会重叠显示内容
	m_static_pagesize.ShowWindow(SW_HIDE); // 刷新，否则会重叠显示内容
	m_static_totalsize.ShowWindow(SW_HIDE); // 刷新，否则会重叠显示内容

	m_static_pagecount.SetWindowText(_strPageCount);
	m_static_pagesize.SetWindowText(_strPageSize);
	m_static_totalsize.SetWindowText(_strTotalSize);

	m_static_pagecount.ShowWindow(SW_SHOW); // 刷新，否则会重叠显示内容
	m_static_pagesize.ShowWindow(SW_SHOW); // 刷新，否则会重叠显示内容
	m_static_totalsize.ShowWindow(SW_SHOW); // 刷新，否则会重叠显示内容

	UpdateData(FALSE);  // 更新控件，刷新当前对话框
	return true;
}



// CDlg_Indicators 消息处理程序


BOOL CDlg_Indicators::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CenterWindow();
	InitControls();

	//m_static_progress.SetWindowText(_T("%d %%"));

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlg_Indicators::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}


void CDlg_Indicators::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_dwElaspedTime ++;
	CString strElapsedTime;

	strElapsedTime.Format("%02u:%02u:%02u",m_dwElaspedTime/3600,
		m_dwElaspedTime/60%60,m_dwElaspedTime%60);

	m_static_elapsedtime.ShowWindow(SW_HIDE); // 刷新，否则会重叠显示内容
	m_static_elapsedtime.SetWindowText(strElapsedTime);
	m_static_elapsedtime.ShowWindow(SW_SHOW);

	UpdateSpeed(m_dwSpeed); // 每秒钟更新速度
	m_dwSpeed = 0; // 必须调用
	UpdateData(FALSE);  // 更新控件，刷新当前对话框
	CDialogEx::OnTimer(nIDEvent);
}

void CDlg_Indicators::StartTimer()
{
	// 启动ID为1的定时器，定时时间为1秒   
	SetTimer(1, 1000, NULL); 
}

void CDlg_Indicators::EndTimer()
{
	KillTimer(1);
}

void CDlg_Indicators::UpdateProgress(int _nPos)
{
	m_Progress.SetPos(_nPos);
	CString str;
	str.Format("%d %%",_nPos);

	//m_static_progress.ShowWindow(SW_HIDE);
	m_static_progress.SetWindowText(str);
	//m_static_progress.ShowWindow(SW_SHOW);
	UpdateData(FALSE);  // 更新控件，刷新当前对话框
}


void CDlg_Indicators::OnCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	//DestroyWindow();
	if (m_pDS)
	{
		m_pDS->CancelScan();
	}
	CDialogEx::OnCancel();
}


void CDlg_Indicators::PostNcDestroy()
{
	// TODO: 在此添加专用代码和/或调用基类

	CDialogEx::PostNcDestroy();
	delete this;
}

void CDlg_Indicators::UpdateSpeed(const DWORD _dwSpeed)
{
	double dSpeed = 0.0;
	CString szStr;
	if (_dwSpeed >= KBYTES)
	{
		dSpeed = _dwSpeed/(KBYTES * 1.0);
		szStr.Format("%.2f Kb/s",dSpeed);
		if(_dwSpeed >= MBYTES)
		{
			dSpeed = _dwSpeed/(MBYTES * 1.0);
			szStr.Format("%.2f Mb/s",dSpeed);
			if(_dwSpeed >= GBYTES)
			{
				dSpeed = _dwSpeed/(GBYTES * 1.0);
				szStr.Format("%.2f Gb/s",dSpeed);
			}
		}
	}
	else
	{
		szStr.Format("%d b/s",_dwSpeed);
	}
	// 刷新显示内容
	//m_static_speed.ShowWindow(SW_HIDE);
	m_static_speed.SetWindowText(szStr);
	//m_static_speed.ShowWindow(SW_SHOW);
}

void CDlg_Indicators::SetSpeed(const DWORD _dwSpeed)
{
	m_dwSpeed += _dwSpeed;
}

void CDlg_Indicators::InitControls()
{
	TCHAR path[MAX_PATH] = {0};
	GetFilePath(SKIN_PROGRESS_BACKIMAGE, path);
	m_Progress.SetProgressBackImage(path, CXIMAGE_FORMAT_PNG);
	GetFilePath(SKIN_PROGRESS_FRONTIMAGE, path);
	m_Progress.SetProgressImage(path, CXIMAGE_FORMAT_PNG);
	m_Progress.SetRange(0, 100);
	m_Progress.SetStep(1);
	m_Progress.SetPos(0);
		
	GetFilePath(SKIN_BUTTON_CLOSE, path);
	m_button_close.SetButtonImage(path, CXIMAGE_FORMAT_PNG);
	m_button_close.SetToolTips(_T("关闭"));

	GetFilePath(SKIN_BUTTON, path);
	m_button_cancel.SetButtonImage(path, CXIMAGE_FORMAT_PNG);
	m_button_cancel.SetToolTips(_T("停止扫描与传输数据"));

	m_static_speed.SetTextColor(COLOR_GREEN_BRIGHT);	//tc:设置文本颜色
	m_static_speed.SetMyFont(90,_T("微软雅黑"));			//tc:设置字体及其大小

	m_static_progress.SetTextColor(COLOR_GREEN_BRIGHT);	//tc:设置文本颜色
	m_static_progress.SetMyFont(90,_T("微软雅黑"));			//tc:设置字体及其大小

	m_static_elapsedtime.SetTextColor(COLOR_GREEN_BRIGHT);	//tc:设置文本颜色
	m_static_elapsedtime.SetMyFont(90,_T("微软雅黑"));			//tc:设置字体及其大小

	m_static_pagecount.SetTextColor(COLOR_GREEN_BRIGHT);	//tc:设置文本颜色
	m_static_pagecount.SetMyFont(90,_T("微软雅黑"));			//tc:设置字体及其大小

	m_static_pagesize.SetTextColor(COLOR_GREEN_BRIGHT);	//tc:设置文本颜色
	m_static_pagesize.SetMyFont(90,_T("微软雅黑"));			//tc:设置字体及其大小

	m_static_totalsize.SetTextColor(COLOR_GREEN_BRIGHT);	//tc:设置文本颜色
	m_static_totalsize.SetMyFont(90,_T("微软雅黑"));			//tc:设置字体及其大小

}


void CDlg_Indicators::OnPaint()
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


HBRUSH CDlg_Indicators::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	CFont font1/*, font2, font3, font4, font5*/;
	font1.CreatePointFont(90, _T("微软雅黑"));
	//font2.CreatePointFont(110, _T("微软雅黑"));
	//font3.CreatePointFont(100, _T("微软雅黑"));
	//font4.CreatePointFont(140, _T("微软雅黑"));
	//font5.CreatePointFont(90, _T("微软雅黑"));
	// TODO:  Change any attributes of the DC here

	switch (pWnd->GetDlgCtrlID())
	{
	case IDC_STATIC:
		{
			pDC->SetBkMode(TRANSPARENT);
			//pDC->SetTextColor(RGB(124, 153, 163));
			pDC->SetTextColor(COLOR_WHITE);  
			pDC->SelectObject(&font1);
			return (HBRUSH)GetStockObject(HOLLOW_BRUSH);
		}
		break;
	//case IDC_INDICATOR_STATIC_PAGECOUNT:
	//case IDC_INDICATOR_STATIC_ELAPSEDTIME:
	//case IDC_INDICATOR_STATIC_PAGESIZE:
	//case IDC_INDICATOR_STATIC_PROGRESS:
	////case IDC_INDICATOR_STATIC_SPEED:
	//case IDC_INDICATOR_STATIC_TOTALSIZE:
	//	{
	//		pDC->SetBkMode(TRANSPARENT);
	//		//pDC->SetTextColor(RGB(124, 153, 163));
	//		pDC->SetTextColor(RGB(2, 232, 0));  // 亮绿色
	//		//pDC->SetTextColor(RGB(129, 42, 142));  // 紫色
	//		//pDC->SetTextColor(RGB(55, 141, 0));  // 淡绿色
	//		//pDC->SetTextColor(RGB(255, 0, 0));
	//		pDC->SelectObject(&font1);
	//		return (HBRUSH)GetStockObject(HOLLOW_BRUSH);
	//	}
	//	break;
	//case IDOK:
	//	//case IDC_VIDEO_SLIDER_EXP:
	//	{
	//		pDC->SetTextColor(RGB(255, 255, 255));
	//		//pDC->SetTextColor(RGB(44, 173, 219));
	//		//pDC->SetTextColor(RGB(129, 42, 142));
	//		pDC->SelectObject(&font2);
	//		pDC->SetBkMode(TRANSPARENT);
	//		return (HBRUSH)GetStockObject(HOLLOW_BRUSH);
	//	}
	//	break;
	default: 
		break;
	}
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}


LRESULT CDlg_Indicators::OnNcHitTest(CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	UINT nHitTest = CDialog::OnNcHitTest(point);
	// 如果鼠标在窗口客户区，则返回标题条代号给Windows
	// 使Windows按鼠标在标题条上类进行处理，即可单击移动窗口
	return (nHitTest == HTCLIENT) ? HTCAPTION : nHitTest;
	//return CDialogEx::OnNcHitTest(point);
}

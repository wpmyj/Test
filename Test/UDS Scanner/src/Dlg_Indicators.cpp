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
// CDlg_Indicators 对话框

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
}


BEGIN_MESSAGE_MAP(CDlg_Indicators, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlg_Indicators::OnBnClickedOk)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDCANCEL, &CDlg_Indicators::OnCancel)
END_MESSAGE_MAP()

bool CDlg_Indicators::UpdatePageInfo(CString& _strPageCount, CString& _strPageSize, CString& _strTotalSize)
{
	m_static_pagecount.SetWindowText(_strPageCount);
	m_static_pagesize.SetWindowText(_strPageSize);
	m_static_totalsize.SetWindowText(_strTotalSize);
	UpdateData(FALSE);  // 更新控件，刷新当前对话框
	return true;
}



// CDlg_Indicators 消息处理程序


BOOL CDlg_Indicators::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CenterWindow();
	m_Progress.SetRange(0, 100);
	m_Progress.SetStep(1);
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

	strElapsedTime.Format("%u小时%u分%u秒",m_dwElaspedTime/3600,
		m_dwElaspedTime/60%60,m_dwElaspedTime%60);
	m_static_elapsedtime.SetWindowText(strElapsedTime);

	UpdateSpeed(m_dwSpeed); // 每秒钟更新速度


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
	m_static_progress.SetWindowText(str);
}


void CDlg_Indicators::OnCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	//DestroyWindow();
	//if (m_pDS)
	//{
	//	m_pDS->CancelScan();
	//}
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
	m_static_speed.SetWindowText(szStr);
	m_dwSpeed = 0; // 必须调用
}

void CDlg_Indicators::SetSpeed(const DWORD _dwSpeed)
{
	m_dwSpeed = _dwSpeed;
}

// Page_About.cpp : 实现文件
//

#include "stdafx.h"
#include "UDS Scanner.h"
#include "Page_About.h"
#include "afxdialogex.h"


// CPage_About 对话框

IMPLEMENT_DYNAMIC(CPage_About, CPropertyPage)

CPage_About::CPage_About(MFC_UI *pUI)
	: m_pUI(pUI),CPropertyPage(CPage_About::IDD)
{

}

CPage_About::~CPage_About()
{
}

void CPage_About::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPage_About, CPropertyPage)
	ON_STN_CLICKED(IDC_STATIC_WEB, &CPage_About::OnAbout_Static_Web)
	ON_WM_CTLCOLOR()
	ON_WM_SETCURSOR()
END_MESSAGE_MAP()


// CPage_About 消息处理程序


void CPage_About::OnAbout_Static_Web()
{
	// TODO: 在此添加控件通知处理程序代码
	ShellExecute(NULL,_T("open"),_T("http://www.arc-uds.com"),NULL,NULL,SW_SHOWNORMAL);
}


HBRUSH CPage_About::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	switch (pWnd->GetDlgCtrlID())  
	{  
		case IDC_STATIC_WEB: 
			pDC->SetTextColor(RGB(0,0,255)); 
			break;  
	}  

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}


BOOL CPage_About::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CPoint pos;   
	GetCursorPos(&pos);  //获取当前鼠标位置    
	 
	GetDlgItem(IDC_STATIC_WEB)->GetWindowRect(&m_pRectLink);  
	if(m_pRectLink.PtInRect(pos)) //如果鼠标在这个范围之内   
	{  
		SetCursor(LoadCursor(NULL, IDC_HAND)); //设置成手状   
		return TRUE; //记得一定要返回TRUE   
	}   

	return CPropertyPage::OnSetCursor(pWnd, nHitTest, message);
}



BOOL CPage_About::OnSetActive()
{
	// TODO: 在此添加专用代码和/或调用基类
	m_pUI->PreViewStatus();
	return CPropertyPage::OnSetActive();
}

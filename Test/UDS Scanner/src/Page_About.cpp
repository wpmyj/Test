// Page_About.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "UDS Scanner.h"
#include "Page_About.h"
#include "afxdialogex.h"


// CPage_About �Ի���

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


// CPage_About ��Ϣ�������


void CPage_About::OnAbout_Static_Web()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	ShellExecute(NULL,_T("open"),_T("http://www.arc-uds.com"),NULL,NULL,SW_SHOWNORMAL);
}


HBRUSH CPage_About::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	switch (pWnd->GetDlgCtrlID())  
	{  
		case IDC_STATIC_WEB: 
			pDC->SetTextColor(RGB(0,0,255)); 
			break;  
	}  

	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}


BOOL CPage_About::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CPoint pos;   
	GetCursorPos(&pos);  //��ȡ��ǰ���λ��    
	 
	GetDlgItem(IDC_STATIC_WEB)->GetWindowRect(&m_pRectLink);  
	if(m_pRectLink.PtInRect(pos)) //�������������Χ֮��   
	{  
		SetCursor(LoadCursor(NULL, IDC_HAND)); //���ó���״   
		return TRUE; //�ǵ�һ��Ҫ����TRUE   
	}   

	return CPropertyPage::OnSetCursor(pWnd, nHitTest, message);
}



BOOL CPage_About::OnSetActive()
{
	// TODO: �ڴ����ר�ô����/����û���
	m_pUI->PreViewStatus();
	return CPropertyPage::OnSetActive();
}

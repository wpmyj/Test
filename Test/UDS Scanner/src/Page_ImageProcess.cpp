// Page_ImageProcess.cpp : 实现文件
//

#include "stdafx.h"
#include "UDS Scanner.h"
#include "Page_ImageProcess.h"
#include "afxdialogex.h"


// CPage_ImageProcess 对话框

IMPLEMENT_DYNAMIC(CPage_ImageProcess, CPropertyPage)

CPage_ImageProcess::CPage_ImageProcess(MFC_UI *pUI)
	: m_pUI(pUI),CPropertyPage(CPage_ImageProcess::IDD)
{

}

CPage_ImageProcess::~CPage_ImageProcess()
{
}

void CPage_ImageProcess::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPage_ImageProcess, CPropertyPage)
END_MESSAGE_MAP()


// CPage_ImageProcess 消息处理程序

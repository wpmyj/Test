// Base_Tab_Spiltshow.cpp : 实现文件
//

#include "stdafx.h"
#include "UDS Scanner.h"
#include "Base_Tab_Spiltshow.h"
#include "afxdialogex.h"


// CBase_Tab_Spiltshow 对话框

IMPLEMENT_DYNAMIC(CBase_Tab_Spiltshow, CDialogEx)

CBase_Tab_Spiltshow::CBase_Tab_Spiltshow(MFC_UI *pUI)
	: m_pUI(pUI),CDialogEx(CBase_Tab_Spiltshow::IDD)
{

}

CBase_Tab_Spiltshow::~CBase_Tab_Spiltshow()
{
}

void CBase_Tab_Spiltshow::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CBase_Tab_Spiltshow, CDialogEx)
END_MESSAGE_MAP()


// CBase_Tab_Spiltshow 消息处理程序

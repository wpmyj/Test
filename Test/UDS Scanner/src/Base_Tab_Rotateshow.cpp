// Base_Tab_Rotateshow.cpp : 实现文件
//

#include "stdafx.h"
#include "UDS Scanner.h"
#include "Base_Tab_Rotateshow.h"
#include "afxdialogex.h"


// CBase_Tab_Rotateshow 对话框

IMPLEMENT_DYNAMIC(CBase_Tab_Rotateshow, CDialogEx)

CBase_Tab_Rotateshow::CBase_Tab_Rotateshow(MFC_UI *pUI)
	: m_pUI(pUI),CDialogEx(CBase_Tab_Rotateshow::IDD)
{

}

CBase_Tab_Rotateshow::~CBase_Tab_Rotateshow()
{
}

void CBase_Tab_Rotateshow::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CBase_Tab_Rotateshow, CDialogEx)
END_MESSAGE_MAP()


// CBase_Tab_Rotateshow 消息处理程序

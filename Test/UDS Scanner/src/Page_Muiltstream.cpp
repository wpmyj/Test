// Page_Muiltstream.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "UDS Scanner.h"
#include "Page_Muiltstream.h"
#include "afxdialogex.h"


// CPage_Muiltstream �Ի���

IMPLEMENT_DYNAMIC(CPage_Muiltstream, CPropertyPage)

CPage_Muiltstream::CPage_Muiltstream(MFC_UI *pUI)
	: m_pUI(pUI),CPropertyPage(CPage_Muiltstream::IDD)
{

}

CPage_Muiltstream::~CPage_Muiltstream()
{
}

void CPage_Muiltstream::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPage_Muiltstream, CPropertyPage)
END_MESSAGE_MAP()


// CPage_Muiltstream ��Ϣ�������

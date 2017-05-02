// Base_Tab_Spiltshow.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "UDS Scanner.h"
#include "Base_Tab_Spiltshow.h"
#include "afxdialogex.h"

extern char m_szSourceImagePath[PATH_MAX];
extern char szTWAIN_DS_DIR[PATH_MAX]; /**< ����DS��·�� */

// CBase_Tab_Spiltshow �Ի���

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
	DDX_Control(pDX, IDC_STATIC_SPILTPICTURE, m_spiltpicture);
}


BEGIN_MESSAGE_MAP(CBase_Tab_Spiltshow, CDialogEx)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CBase_Tab_Spiltshow ��Ϣ�������


void CBase_Tab_Spiltshow::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� __super::OnPaint()
	CWnd* m_pWnd = this->GetDlgItem(IDC_STATIC_SPILTPICTURE);
	DrawImgOnCtrl(m_image, m_pWnd); 
}


BOOL CBase_Tab_Spiltshow::OnInitDialog()
{
	__super::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	strcpy_s(m_szSourceImagePath, PATH_MAX, szTWAIN_DS_DIR);
	strcat(m_szSourceImagePath,  "\\");
	strcat(m_szSourceImagePath, "Spilt.jpg");

	m_image.Load(m_szSourceImagePath,CXIMAGE_FORMAT_JPG);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

//����ͼ����  
void CBase_Tab_Spiltshow::DrawImgOnCtrl(CxImage pImg , CWnd* wndDraw)  
{  
	int nImgWidth,nImgHeight;  //ͼ��ĳ���  
	CRect rectWndDraw;         //��ͼ���ڵľ���  
	CRect rectDispaly;         //ͼ����ʾ����  

	//��ȡͼ��ĳ���  
	nImgWidth  = pImg.GetWidth();  
	nImgHeight = pImg.GetHeight();  

	//��ȡ���ƴ��ڵĳ���  
	wndDraw->GetClientRect(&rectWndDraw);  

	//����ͼ��ͻ�������ı�������ͼ��  
	CDC* pDC;  
	pDC = wndDraw->GetDC();   //��ȡ���ھ��  

	pDC->FillSolidRect(rectWndDraw,RGB(0,0,0));  

	//����ͼ�񵽴�������  
	float xRadio,yRadio;  
	float fRadio;  
	xRadio = (float)rectWndDraw.Width()/(float)nImgWidth;  
	yRadio = (float)rectWndDraw.Height()/(float)nImgHeight;  
	if(xRadio >= yRadio)  
		fRadio = yRadio;  
	else  
		fRadio = xRadio;  

	//����ͼ����ʾ��λ��  
	rectDispaly.left = int((rectWndDraw.Width() - nImgWidth * fRadio)/2) + rectWndDraw.left;  
	rectDispaly.top = int((rectWndDraw.Height() - nImgHeight * fRadio)/2) + rectWndDraw.top;  
	rectDispaly.right = int(rectDispaly.left + nImgWidth * fRadio);  
	rectDispaly.bottom = int(rectDispaly.top + nImgHeight * fRadio);  

	pImg.Draw(pDC->GetSafeHdc(),rectDispaly);  
}  
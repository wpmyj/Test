// Base_Tab_Rotateshow.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "UDS Scanner.h"
#include "Base_Tab_Rotateshow.h"
#include "afxdialogex.h"

extern void GetFilePath( char* szFileName, char* szFilePath);
// CBase_Tab_Rotateshow �Ի���

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
	DDX_Control(pDX, IDC_STATIC_ROTATEPICTURE, m_picture);
}


BEGIN_MESSAGE_MAP(CBase_Tab_Rotateshow, CDialogEx)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CBase_Tab_Rotateshow ��Ϣ�������


BOOL CBase_Tab_Rotateshow::OnInitDialog()
{
	__super::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	char path[PATH_MAX]; 
	GetFilePath(PICTURENAME_ROTATE,path);
	m_image.Load(path,CXIMAGE_FORMAT_JPG);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CBase_Tab_Rotateshow::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� __super::OnPaint()
	//�������ؼ�����Ϊ��ͼ��ͬһ�ߴ�
	CWnd* m_pWnd = this->GetDlgItem(IDC_STATIC_ROTATEPICTURE);
	DrawImgOnCtrl(m_image, m_pWnd); 
}

//����ͼ����  
void CBase_Tab_Rotateshow::DrawImgOnCtrl(CxImage pImg , CWnd* wndDraw)  
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
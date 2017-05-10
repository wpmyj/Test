// Base_Tab_Rotateshow.cpp : 实现文件
//

#include "stdafx.h"
#include "UDS Scanner.h"
#include "Base_Tab_Rotateshow.h"
#include "afxdialogex.h"

extern void GetFilePath( char* szFileName, char* szFilePath);
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
	DDX_Control(pDX, IDC_STATIC_ROTATEPICTURE, m_picture);
}


BEGIN_MESSAGE_MAP(CBase_Tab_Rotateshow, CDialogEx)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CBase_Tab_Rotateshow 消息处理程序


BOOL CBase_Tab_Rotateshow::OnInitDialog()
{
	__super::OnInitDialog();

	// TODO:  在此添加额外的初始化
	char path[PATH_MAX]; 
	GetFilePath(PICTURENAME_ROTATE,path);
	m_image.Load(path,CXIMAGE_FORMAT_JPG);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CBase_Tab_Rotateshow::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 __super::OnPaint()
	//将整个控件调整为与图像同一尺寸
	CWnd* m_pWnd = this->GetDlgItem(IDC_STATIC_ROTATEPICTURE);
	DrawImgOnCtrl(m_image, m_pWnd); 
}

//绘制图像函数  
void CBase_Tab_Rotateshow::DrawImgOnCtrl(CxImage pImg , CWnd* wndDraw)  
{  
	int nImgWidth,nImgHeight;  //图像的长宽  
	CRect rectWndDraw;         //绘图窗口的矩形  
	CRect rectDispaly;         //图像显示区域  

	//获取图像的长宽  
	nImgWidth  = pImg.GetWidth();  
	nImgHeight = pImg.GetHeight();  

	//获取绘制窗口的长宽  
	wndDraw->GetClientRect(&rectWndDraw);  

	//根据图像和绘制区域的比例绘制图像  
	CDC* pDC;  
	pDC = wndDraw->GetDC();   //获取窗口句柄  

	pDC->FillSolidRect(rectWndDraw,RGB(0,0,0));  

	//绘制图像到窗口区域  
	float xRadio,yRadio;  
	float fRadio;  
	xRadio = (float)rectWndDraw.Width()/(float)nImgWidth;  
	yRadio = (float)rectWndDraw.Height()/(float)nImgHeight;  
	if(xRadio >= yRadio)  
		fRadio = yRadio;  
	else  
		fRadio = xRadio;  

	//计算图像显示的位置  
	rectDispaly.left = int((rectWndDraw.Width() - nImgWidth * fRadio)/2) + rectWndDraw.left;  
	rectDispaly.top = int((rectWndDraw.Height() - nImgHeight * fRadio)/2) + rectWndDraw.top;  
	rectDispaly.right = int(rectDispaly.left + nImgWidth * fRadio);  
	rectDispaly.bottom = int(rectDispaly.top + nImgHeight * fRadio);  

	pImg.Draw(pDC->GetSafeHdc(),rectDispaly);  
}  
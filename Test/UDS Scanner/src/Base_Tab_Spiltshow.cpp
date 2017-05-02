// Base_Tab_Spiltshow.cpp : 实现文件
//

#include "stdafx.h"
#include "UDS Scanner.h"
#include "Base_Tab_Spiltshow.h"
#include "afxdialogex.h"

extern char m_szSourceImagePath[PATH_MAX];
extern char szTWAIN_DS_DIR[PATH_MAX]; /**< 驱动DS的路径 */

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
	DDX_Control(pDX, IDC_STATIC_SPILTPICTURE, m_spiltpicture);
}


BEGIN_MESSAGE_MAP(CBase_Tab_Spiltshow, CDialogEx)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CBase_Tab_Spiltshow 消息处理程序


void CBase_Tab_Spiltshow::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 __super::OnPaint()
	CWnd* m_pWnd = this->GetDlgItem(IDC_STATIC_SPILTPICTURE);
	DrawImgOnCtrl(m_image, m_pWnd); 
}


BOOL CBase_Tab_Spiltshow::OnInitDialog()
{
	__super::OnInitDialog();

	// TODO:  在此添加额外的初始化
	strcpy_s(m_szSourceImagePath, PATH_MAX, szTWAIN_DS_DIR);
	strcat(m_szSourceImagePath,  "\\");
	strcat(m_szSourceImagePath, "Spilt.jpg");

	m_image.Load(m_szSourceImagePath,CXIMAGE_FORMAT_JPG);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

//绘制图像函数  
void CBase_Tab_Spiltshow::DrawImgOnCtrl(CxImage pImg , CWnd* wndDraw)  
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
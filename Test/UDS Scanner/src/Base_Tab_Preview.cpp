// Base_Tab_Preview.cpp : 实现文件
//

#include "stdafx.h"
#include "UDS Scanner.h"
#include "Base_Tab_Preview.h"
#include "afxdialogex.h"


// CBase_Tab_Preview 对话框

IMPLEMENT_DYNAMIC(CBase_Tab_Preview, CDialogEx)

CBase_Tab_Preview::CBase_Tab_Preview(MFC_UI *pUI)
	: m_pUI(pUI),CDialogEx(CBase_Tab_Preview::IDD)
{
}

CBase_Tab_Preview::~CBase_Tab_Preview()
{
}

void CBase_Tab_Preview::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CBase_Tab_Preview, CDialogEx)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CBase_Tab_Preview 消息处理程序


BOOL CBase_Tab_Preview::OnInitDialog()
{
	__super::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//橡皮筋类初始化
	m_rectTracker.m_nStyle = CRectTracker::solidLine;//设置RectTracker样式,实线CRectTracker::resizeOutside|
	m_rectTracker.m_nHandleSize = 5; //控制柄的像素大小

	//初始化纸张单位，并为m_endrect赋值
	int paperindex = m_pUI->GetCurrentCapIndex(ICAP_SUPPORTEDSIZES);
	int papervalue = FindPaperSize(paperindex);
	int unitindex  = m_pUI->GetCurrentCapIndex(ICAP_UNITS);
	int unitvalue = FindUnit(unitindex);
	UpdatePicRectangle(papervalue, unitvalue, 0, 0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CBase_Tab_Preview::UpdatePicRectangle(int index, int unitindex, int xpos, int ypos)
{
	InvalidateRect(NULL);
	UpdateWindow();

	//纸张大小尺寸：单位不同，值不同
	TW_FRAME frame;
	frame = m_pUI->GetCurrentFrame();
	float right = FIX32ToFloat(frame.Right); //A4: 8.27英寸; 1653.40像素; 21.00厘米
	float bottom = FIX32ToFloat(frame.Bottom);

	//图片控件的宽、高
	CRect picrect;
	GetDlgItem(IDC_TABPREVIEW_PREPICTURE)->GetWindowRect(picrect);
	float width = (float)picrect.Width(); //257
	float height = (float)picrect.Height(); //392

	float widthscale;
	float heightscale;
	//根据英寸的值，设置比例，最终得到需要画出区域的大小
	if(unitindex == TWUN_INCHES)//英寸
	{
		widthscale = 100.00f * width / 900.00f;//width/900=x/(right*100)算x
		heightscale = 100.00f * height / 1400.00f;
	}
	else if(unitindex == TWUN_PIXELS)//像素
	{
		int nCapIndex = m_pUI->GetCurrentCapIndex(ICAP_XRESOLUTION);
		const FloatVector* lstCapValuesFlt = m_pUI->GetValidCapFloat(ICAP_XRESOLUTION);
		int reso = (int)lstCapValuesFlt->at(nCapIndex); //x分辨率
		float fscale = (float)reso/200.00; //200dpi对应1800 不同分辨率下，横坐标的最大值

		widthscale = width / (1800.00f * fscale);//width/1800=x/right算x
		heightscale = height / (2800.00f * fscale);
	}
	else if(unitindex == TWUN_CENTIMETERS)//厘米
	{
		widthscale = width / 27.00f;//width/27=x/right算x
		heightscale = height / 42.00f;
	}
	else{}
	m_endrect.right = (int)(right * widthscale);
	m_endrect.bottom = (int)(bottom * heightscale);

	//画图
	CDC *pDC = GetDlgItem(IDC_TABPREVIEW_PREPICTURE)->GetDC();
	CPen pen(PS_SOLID, 1, RGB(255,0,0));  
	CPen *pOldPen = pDC->SelectObject(&pen);  
	CBrush *pOldBr = (CBrush *)pDC->SelectStockObject(NULL_BRUSH); 
	pDC->Rectangle(xpos, ypos, m_endrect.right, m_endrect.bottom);
	pDC->SelectObject(pOldBr);  
	pDC->SelectObject(pOldPen);
}


void CBase_Tab_Preview::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 __super::OnPaint()
	DrawImage(); //画预览图

	m_rectTracker.Draw(&dc); //橡皮筋类画好矩形框

	//画标尺
	//获得控件位置
	CRect rect;
	GetDlgItem(IDC_TABPREVIEW_PREPICTURE)->GetWindowRect(rect);
	ScreenToClient(rect);

	CPoint beginpoint;	//指定起点横坐标偏移
	int width = rect.Width();; //指定标尺宽度
	int height = rect.Height();; //指定标尺长度
	//画刻度线
	int degree;
	dc.SetTextAlign(TA_CENTER | TA_BOTTOM);//将刻度线数字标注在刻度线的上方
	dc.SetBkMode(TRANSPARENT);//消除白色背景

	int nIndex = m_pUI->GetCurrentCapIndex(ICAP_UNITS);
	int nval = FindUnit(nIndex);
	if(nval == TWUN_INCHES)//英寸
	{
		beginpoint.x = rect.left;
		beginpoint.y = rect.top-2; //画在图片上方
		//画大刻度
		for(degree = 0; degree <= 900; degree += 100)
		{
			dc.MoveTo(beginpoint.x + degree*width/900, beginpoint.y-width/9/3);
			dc.LineTo(beginpoint.x + degree*width/900, beginpoint.y);
			CString str;
			str.Format(_T("%d"), (degree/100));
			dc.TextOut(beginpoint.x + degree*width/900, beginpoint.y-width/9/3, str);
		}
		//画小刻度
		for(degree = 20; degree <= 900; degree += 20)  
		{  
			dc.MoveTo(beginpoint.x + degree*width/900, beginpoint.y - width/9/5);  
			dc.LineTo(beginpoint.x + degree*width/900, beginpoint.y);  
		}  

		//画左侧标尺
		//指定起点横坐标偏移
		beginpoint.x = rect.left-2;//画在图片左侧
		beginpoint.y = rect.top; 
		//画刻度线
		dc.SetTextAlign(TA_CENTER | TA_LEFT);//将刻度线数字标注在刻度线的左侧
		dc.SetBkMode(TRANSPARENT);//消除白色背景
		//画大刻度
		for(degree = 0; degree <= 1400; degree += 100)
		{
			dc.MoveTo(beginpoint.x - height/14/3, beginpoint.y + degree*height/1400);
			dc.LineTo(beginpoint.x, beginpoint.y + degree*height/1400);
			CString str;
			str.Format(_T("%d"), (degree/100));
			dc.TextOut(beginpoint.x - height/14/3-10, beginpoint.y + degree*height/1400-7, str); //使数字与线居中
		}
		//画小刻度
		for(degree = 20; degree <= 1400; degree += 20)  
		{  
			dc.MoveTo(beginpoint.x - height/14/5, beginpoint.y + degree*height/1400);  
			dc.LineTo(beginpoint.x, beginpoint.y + degree*height/1400);  
		} 
	}
	else if(nval == TWUN_PIXELS)//像素
	{
		int nCapIndex = m_pUI->GetCurrentCapIndex(ICAP_XRESOLUTION);
		const FloatVector* lstCapValuesFlt = m_pUI->GetValidCapFloat(ICAP_XRESOLUTION);
		int reso = (int)lstCapValuesFlt->at(nCapIndex); //x分辨率
		float fscale = (float)reso/200.00; //200dpi对应1800
		int xmax = (int)(1800.00 * fscale);//不同分辨率下，横坐标的最大值
		int ymax = (int)(2800.00 * fscale);//不同分辨率下，纵坐标的最大值

		int xpos = 0;
		beginpoint.x = rect.left;
		beginpoint.y = rect.top-2; //画在图片上方

		int xMax = xmax * 100;
		int yMax = ymax * 100;
		int step = (int)(fscale * 20000.00f);
		//画大刻度
		for(degree = 0; degree <= xMax; degree += step)
		{
			dc.MoveTo(beginpoint.x + degree*width/xMax, beginpoint.y-width/9/3);
			dc.LineTo(beginpoint.x + degree*width/xMax, beginpoint.y);
			CString str;
			str.Format(_T("%d"), (degree/100));

			if(degree <= (int)(40000 * fscale))
			{
				dc.TextOut(beginpoint.x + degree*width/xMax - 3, beginpoint.y-width/9/3, str);
			}
			else
			{
				xpos += 1;
				dc.TextOut(beginpoint.x + degree*width/xMax + xpos - 2, beginpoint.y-width/9/3, str);
			}
		}
		//画小刻度
		for(degree = step/5; degree <= xMax; degree += step/5)  
		{  
			dc.MoveTo(beginpoint.x + degree*width/xMax, beginpoint.y - width/9/5);  
			dc.LineTo(beginpoint.x + degree*width/xMax, beginpoint.y);  
		}  

		//画左侧标尺
		//指定起点横坐标偏移
		beginpoint.x = rect.left-2;//画在图片左侧
		beginpoint.y = rect.top; 

		//画刻度线
		dc.SetTextAlign(TA_CENTER | TA_LEFT);//将刻度线数字标注在刻度线的左侧
		dc.SetBkMode(TRANSPARENT);//消除白色背景
		//画大刻度
		for(degree = 0; degree <= yMax; degree += step)
		{
			dc.MoveTo(beginpoint.x - height/14/3, beginpoint.y + degree*height/yMax);
			dc.LineTo(beginpoint.x, beginpoint.y + degree*height/yMax);
			CString str;
			str.Format(_T("%d"), (degree/100));
			dc.TextOut(beginpoint.x - height/14/3-17, beginpoint.y + degree*height/yMax-7, str); //使数字与线居中
		}
		//画小刻度
		for(degree = step/5; degree <= yMax; degree += step/5)  
		{  
			dc.MoveTo(beginpoint.x - height/14/5, beginpoint.y + degree*height/yMax);  
			dc.LineTo(beginpoint.x, beginpoint.y + degree*height/yMax);  
		} 
	}
	else if(nval == TWUN_CENTIMETERS)//厘米
	{
		beginpoint.x = rect.left;
		beginpoint.y = rect.top-2; //画在图片上方
		//画大刻度
		for(degree = 0; degree <= 2700; degree += 300)
		{
			dc.MoveTo(beginpoint.x + degree*width/2700, beginpoint.y-width/9/3);
			dc.LineTo(beginpoint.x + degree*width/2700, beginpoint.y);
			CString str;
			str.Format(_T("%d"), (degree/100));
			dc.TextOut(beginpoint.x + degree*width/2700, beginpoint.y-width/9/3, str);
		}
		//画小刻度
		for(degree = 60; degree <= 2700; degree += 60)  
		{  
			dc.MoveTo(beginpoint.x + degree*width/2700, beginpoint.y - width/9/5);  
			dc.LineTo(beginpoint.x + degree*width/2700, beginpoint.y);  
		}  

		//画左侧标尺
		//指定起点横坐标偏移
		beginpoint.x = rect.left-2;//画在图片左侧
		beginpoint.y = rect.top; 
		//画刻度线
		dc.SetTextAlign(TA_CENTER | TA_LEFT);//将刻度线数字标注在刻度线的左侧
		dc.SetBkMode(TRANSPARENT);//消除白色背景

		//画大刻度
		for(degree = 0; degree <= 4200; degree += 300)
		{
			dc.MoveTo(beginpoint.x - height/14/3, beginpoint.y + degree*height/4200);
			dc.LineTo(beginpoint.x, beginpoint.y + degree*height/4200);
			CString str;
			str.Format(_T("%d"), (degree/100));
			dc.TextOut(beginpoint.x - height/14/3-10, beginpoint.y + degree*height/4200-7, str); //使数字与线居中
		}
		//画小刻度
		for(degree = 60; degree <= 4200; degree += 60)  
		{  
			dc.MoveTo(beginpoint.x - height/14/5, beginpoint.y + degree*height/4200);  
			dc.LineTo(beginpoint.x, beginpoint.y + degree*height/4200);  
		} 
	}
	else{}
}

void CBase_Tab_Preview::PreView()
{
	UpdateData(TRUE);

	m_pUI->SetCapValueInt(UDSCAP_DOCS_IN_ADF, 1);//预览时只扫描一张
	m_pUI->TW_SaveProfileToFile("上次使用模板");

	BYTE *data = NULL; //图像数据
	data = m_pUI->PreView();  

	if(data != NULL)
	{
		// 保存图片
		GetBmpFilePath();//为m_bmpFilePath赋值

		CFile file;
		try
		{
			if(file.Open(m_bmpFilePath, CFile::modeWrite | CFile::modeCreate))
			{
				//写入文件
				file.Write((LPSTR)&(m_pUI->m_bmpFileHeader), sizeof(BITMAPFILEHEADER)); // 写文件头
				file.Write((LPSTR)&(m_pUI->m_bmpInfoHeader), sizeof(BITMAPINFOHEADER)); // 写信息头
				if (m_pUI->m_nBpp < 16)
				{			
					DWORD dwColors = 0;
					if (true == m_pUI->GetColorsUsed(m_pUI->m_nBpp, dwColors))
					{
						file.Write((LPSTR)(m_pUI->m_bmpLpRGB),sizeof(RGBQUAD) * dwColors); // 写调色板
					}	

					if (m_pUI->m_bmpLpRGB)
					{
						delete []m_pUI->m_bmpLpRGB;
						m_pUI->m_bmpLpRGB = NULL;
					}
				}
				file.Write(data, m_pUI->m_nDIBSize); // 写数据
				file.Close();
				if (data)
				{
					delete []data;
					data = NULL;
				}	
			}
		}
		catch (...) 
		{
			AfxMessageBox("SaveDIB2Bmp Error!");
		}	

		Invalidate(); //直接刷新，OnPaint中实现DrawImage();
	}

	UpdateData(FALSE);
}

void CBase_Tab_Preview::DrawImage(void) 
{
	UpdateData(TRUE);
	//显示图片
	CWnd *pWnd = GetDlgItem(IDC_TABPREVIEW_PREPICTURE); 
	CDC* pDC = pWnd->GetDC();
	HDC hDC = pDC->GetSafeHdc();

	IplImage* img = cvLoadImage((CT2CA)m_bmpFilePath, 1);
	if(img != NULL)
	{
		//调整长宽比例因子，使图像显示不失真
		CRect newRect;
		int width = img->width;
		int height = img->height;

		if(width <= m_endrect.Width() && height <= m_endrect.Height()) //小图片，不缩放
		{
			newRect = CRect(m_endrect.TopLeft(), CSize(width,height));
		}
		else
		{
			float xScale = (float)m_endrect.Width() / (float)width;
			float yScale = (float)m_endrect.Height() / (float)height;
			float scale = xScale>=yScale?yScale:xScale; 
			newRect = CRect(m_endrect.TopLeft(), CSize((int)width*scale, (int)height*scale));
		}

		DrawToHDC(hDC, &newRect, img);
		ReleaseDC(pDC);
		cvReleaseImage(&img);

		UpdateData(FALSE);
	}
}

void CBase_Tab_Preview::GetBmpFilePath()
{
	if(GetTempSavePath(m_bmpFilePath))
	{
		strcat(m_bmpFilePath, "preview.bmp");
	}
}

bool CBase_Tab_Preview::GetTempSavePath(TCHAR* pszPath)
{
	TCHAR szTempPath[MAX_PATH];
	memset(szTempPath, 0, MAX_PATH);
	GetTempPath(MAX_PATH, szTempPath);

	SSTRCAT(szTempPath, MAX_PATH, MB_CAPTION);
	SSTRCAT(szTempPath, MAX_PATH, TEXT("\\"));

	if(false == CreateDir(szTempPath))
	{
		MessageBox(TEXT("创建临时文件夹失败！"));
		return false;
	}

	SSTRCPY(pszPath, MAX_PATH, szTempPath);
	return true;
}

bool CBase_Tab_Preview::CreateDir(const CString& strPath)
{
	if (!PathFileExists(strPath))
	{
		if (!CreateDirectory(strPath, NULL))
		{	
			return false;
		}
		return true;
	}

	return true;		
}

//DrawToHdc系列函数
RECT CBase_Tab_Preview::NormalizeRect(RECT r)  
{  
	int t;  

	if( r.left > r.right )  
	{  
		t = r.left;  
		r.left = r.right;  
		r.right = t;  
	}  

	if( r.top > r.bottom )  
	{  
		t = r.top;  
		r.top = r.bottom;  
		r.bottom = t;  
	}  

	return r;  
}  
CvRect CBase_Tab_Preview::RectToCvRect(RECT sr)  
{  
	sr = NormalizeRect( sr );  
	return cvRect( sr.left, sr.top, sr.right - sr.left, sr.bottom - sr.top );  
} 
void  CBase_Tab_Preview::FillBitmapInfo(BITMAPINFO* bmi, int width, int height, int bpp, int origin)  
{  
	assert( bmi && width >= 0 && height >= 0 && (bpp == 8 || bpp == 24 || bpp == 32));  

	BITMAPINFOHEADER* bmih = &(bmi->bmiHeader);  

	memset(bmih, 0, sizeof(*bmih));  
	bmih->biSize = sizeof(BITMAPINFOHEADER);  
	bmih->biWidth = width;  
	bmih->biHeight = origin ? abs(height) : -abs(height);  
	bmih->biPlanes = 1;  
	bmih->biBitCount = (unsigned short)bpp;  
	bmih->biCompression = BI_RGB;  

	if(bpp == 8)  
	{  
		RGBQUAD* palette = bmi->bmiColors;  
		int i;  
		for( i = 0; i < 256; i++ )  
		{  
			palette[i].rgbBlue = palette[i].rgbGreen = palette[i].rgbRed = (BYTE)i;  
			palette[i].rgbReserved = 0;  
		}  
	}  
}  
void  CBase_Tab_Preview::Show(IplImage* img, HDC dc, int x, int y, int w, int h, int from_x, int from_y)  
{  
	if( img && img->depth == IPL_DEPTH_8U )  
	{  
		uchar buffer[sizeof(BITMAPINFOHEADER) + 1024];  
		BITMAPINFO* bmi = (BITMAPINFO*)buffer;  
		int bmp_w = img->width, bmp_h = img->height;  

		int bpp = img ? (img->depth & 255)*img->nChannels : 0;
		FillBitmapInfo(bmi, bmp_w, bmp_h, bpp, img->origin);  

		from_x = MIN(MAX( from_x, 0 ), bmp_w - 1);  
		from_y = MIN(MAX( from_y, 0 ), bmp_h - 1);  

		int sw = MAX(MIN( bmp_w - from_x, w ), 0);  
		int sh = MAX(MIN( bmp_h - from_y, h ), 0);  

		SetDIBitsToDevice(  
			dc, x, y, sw, sh, from_x, from_y, from_y, sh,  
			img->imageData + from_y*img->widthStep,  
			bmi, DIB_RGB_COLORS);  
	}  
}  
void  CBase_Tab_Preview::DrawToHDC(HDC hDCDst, RECT* pDstRect, IplImage* img )  
{  
	if(pDstRect && img && img->depth == IPL_DEPTH_8U && img->imageData )  
	{  
		uchar buffer[sizeof(BITMAPINFOHEADER) + 1024];  
		BITMAPINFO* bmi = (BITMAPINFO*)buffer;  
		int bmp_w = img->width, bmp_h = img->height;  

		CvRect roi = cvGetImageROI(img);
		CvRect dst = RectToCvRect(*pDstRect);  

		if( roi.width == dst.width && roi.height == dst.height )  
		{  
			Show(img, hDCDst, dst.x, dst.y, dst.width, dst.height, roi.x, roi.y);  
			return;  
		}  

		if(roi.width > dst.width)  
		{  
			SetStretchBltMode(hDCDst, // handle to device context  
				HALFTONE );  
		}  
		else  
		{  
			SetStretchBltMode(hDCDst, // handle to device context  
				COLORONCOLOR );  
		}  

		int bpp = img ? (img->depth & 255)*img->nChannels : 0;
		FillBitmapInfo(bmi, bmp_w, bmp_h, bpp, img->origin);  
		::StretchDIBits(
			hDCDst,  
			dst.x, dst.y, dst.width, dst.height,  
			roi.x, roi.y, roi.width, roi.height,  
			img->imageData, bmi, DIB_RGB_COLORS, SRCCOPY);  

	}  
}  
// Base_Tab_Preview.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "UDS Scanner.h"
#include "Base_Tab_Preview.h"
#include "afxdialogex.h"


// CBase_Tab_Preview �Ի���

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


// CBase_Tab_Preview ��Ϣ�������


BOOL CBase_Tab_Preview::OnInitDialog()
{
	__super::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//��Ƥ�����ʼ��
	m_rectTracker.m_nStyle = CRectTracker::solidLine;//����RectTracker��ʽ,ʵ��CRectTracker::resizeOutside|
	m_rectTracker.m_nHandleSize = 5; //���Ʊ������ش�С

	//��ʼ��ֽ�ŵ�λ����Ϊm_endrect��ֵ
	int paperindex = m_pUI->GetCurrentCapIndex(ICAP_SUPPORTEDSIZES);
	int papervalue = FindPaperSize(paperindex);
	int unitindex  = m_pUI->GetCurrentCapIndex(ICAP_UNITS);
	int unitvalue = FindUnit(unitindex);
	UpdatePicRectangle(papervalue, unitvalue, 0, 0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CBase_Tab_Preview::UpdatePicRectangle(int index, int unitindex, int xpos, int ypos)
{
	InvalidateRect(NULL);
	UpdateWindow();

	//ֽ�Ŵ�С�ߴ磺��λ��ͬ��ֵ��ͬ
	TW_FRAME frame;
	frame = m_pUI->GetCurrentFrame();
	float right = FIX32ToFloat(frame.Right); //A4: 8.27Ӣ��; 1653.40����; 21.00����
	float bottom = FIX32ToFloat(frame.Bottom);

	//ͼƬ�ؼ��Ŀ���
	CRect picrect;
	GetDlgItem(IDC_TABPREVIEW_PREPICTURE)->GetWindowRect(picrect);
	float width = (float)picrect.Width(); //257
	float height = (float)picrect.Height(); //392

	float widthscale;
	float heightscale;
	//����Ӣ���ֵ�����ñ��������յõ���Ҫ��������Ĵ�С
	if(unitindex == TWUN_INCHES)//Ӣ��
	{
		widthscale = 100.00f * width / 900.00f;//width/900=x/(right*100)��x
		heightscale = 100.00f * height / 1400.00f;
	}
	else if(unitindex == TWUN_PIXELS)//����
	{
		int nCapIndex = m_pUI->GetCurrentCapIndex(ICAP_XRESOLUTION);
		const FloatVector* lstCapValuesFlt = m_pUI->GetValidCapFloat(ICAP_XRESOLUTION);
		int reso = (int)lstCapValuesFlt->at(nCapIndex); //x�ֱ���
		float fscale = (float)reso/200.00; //200dpi��Ӧ1800 ��ͬ�ֱ����£�����������ֵ

		widthscale = width / (1800.00f * fscale);//width/1800=x/right��x
		heightscale = height / (2800.00f * fscale);
	}
	else if(unitindex == TWUN_CENTIMETERS)//����
	{
		widthscale = width / 27.00f;//width/27=x/right��x
		heightscale = height / 42.00f;
	}
	else{}
	m_endrect.right = (int)(right * widthscale);
	m_endrect.bottom = (int)(bottom * heightscale);

	//��ͼ
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
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� __super::OnPaint()
	DrawImage(); //��Ԥ��ͼ

	m_rectTracker.Draw(&dc); //��Ƥ���໭�þ��ο�

	//�����
	//��ÿؼ�λ��
	CRect rect;
	GetDlgItem(IDC_TABPREVIEW_PREPICTURE)->GetWindowRect(rect);
	ScreenToClient(rect);

	CPoint beginpoint;	//ָ����������ƫ��
	int width = rect.Width();; //ָ����߿��
	int height = rect.Height();; //ָ����߳���
	//���̶���
	int degree;
	dc.SetTextAlign(TA_CENTER | TA_BOTTOM);//���̶������ֱ�ע�ڿ̶��ߵ��Ϸ�
	dc.SetBkMode(TRANSPARENT);//������ɫ����

	int nIndex = m_pUI->GetCurrentCapIndex(ICAP_UNITS);
	int nval = FindUnit(nIndex);
	if(nval == TWUN_INCHES)//Ӣ��
	{
		beginpoint.x = rect.left;
		beginpoint.y = rect.top-2; //����ͼƬ�Ϸ�
		//����̶�
		for(degree = 0; degree <= 900; degree += 100)
		{
			dc.MoveTo(beginpoint.x + degree*width/900, beginpoint.y-width/9/3);
			dc.LineTo(beginpoint.x + degree*width/900, beginpoint.y);
			CString str;
			str.Format(_T("%d"), (degree/100));
			dc.TextOut(beginpoint.x + degree*width/900, beginpoint.y-width/9/3, str);
		}
		//��С�̶�
		for(degree = 20; degree <= 900; degree += 20)  
		{  
			dc.MoveTo(beginpoint.x + degree*width/900, beginpoint.y - width/9/5);  
			dc.LineTo(beginpoint.x + degree*width/900, beginpoint.y);  
		}  

		//�������
		//ָ����������ƫ��
		beginpoint.x = rect.left-2;//����ͼƬ���
		beginpoint.y = rect.top; 
		//���̶���
		dc.SetTextAlign(TA_CENTER | TA_LEFT);//���̶������ֱ�ע�ڿ̶��ߵ����
		dc.SetBkMode(TRANSPARENT);//������ɫ����
		//����̶�
		for(degree = 0; degree <= 1400; degree += 100)
		{
			dc.MoveTo(beginpoint.x - height/14/3, beginpoint.y + degree*height/1400);
			dc.LineTo(beginpoint.x, beginpoint.y + degree*height/1400);
			CString str;
			str.Format(_T("%d"), (degree/100));
			dc.TextOut(beginpoint.x - height/14/3-10, beginpoint.y + degree*height/1400-7, str); //ʹ�������߾���
		}
		//��С�̶�
		for(degree = 20; degree <= 1400; degree += 20)  
		{  
			dc.MoveTo(beginpoint.x - height/14/5, beginpoint.y + degree*height/1400);  
			dc.LineTo(beginpoint.x, beginpoint.y + degree*height/1400);  
		} 
	}
	else if(nval == TWUN_PIXELS)//����
	{
		int nCapIndex = m_pUI->GetCurrentCapIndex(ICAP_XRESOLUTION);
		const FloatVector* lstCapValuesFlt = m_pUI->GetValidCapFloat(ICAP_XRESOLUTION);
		int reso = (int)lstCapValuesFlt->at(nCapIndex); //x�ֱ���
		float fscale = (float)reso/200.00; //200dpi��Ӧ1800
		int xmax = (int)(1800.00 * fscale);//��ͬ�ֱ����£�����������ֵ
		int ymax = (int)(2800.00 * fscale);//��ͬ�ֱ����£�����������ֵ

		int xpos = 0;
		beginpoint.x = rect.left;
		beginpoint.y = rect.top-2; //����ͼƬ�Ϸ�

		int xMax = xmax * 100;
		int yMax = ymax * 100;
		int step = (int)(fscale * 20000.00f);
		//����̶�
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
		//��С�̶�
		for(degree = step/5; degree <= xMax; degree += step/5)  
		{  
			dc.MoveTo(beginpoint.x + degree*width/xMax, beginpoint.y - width/9/5);  
			dc.LineTo(beginpoint.x + degree*width/xMax, beginpoint.y);  
		}  

		//�������
		//ָ����������ƫ��
		beginpoint.x = rect.left-2;//����ͼƬ���
		beginpoint.y = rect.top; 

		//���̶���
		dc.SetTextAlign(TA_CENTER | TA_LEFT);//���̶������ֱ�ע�ڿ̶��ߵ����
		dc.SetBkMode(TRANSPARENT);//������ɫ����
		//����̶�
		for(degree = 0; degree <= yMax; degree += step)
		{
			dc.MoveTo(beginpoint.x - height/14/3, beginpoint.y + degree*height/yMax);
			dc.LineTo(beginpoint.x, beginpoint.y + degree*height/yMax);
			CString str;
			str.Format(_T("%d"), (degree/100));
			dc.TextOut(beginpoint.x - height/14/3-17, beginpoint.y + degree*height/yMax-7, str); //ʹ�������߾���
		}
		//��С�̶�
		for(degree = step/5; degree <= yMax; degree += step/5)  
		{  
			dc.MoveTo(beginpoint.x - height/14/5, beginpoint.y + degree*height/yMax);  
			dc.LineTo(beginpoint.x, beginpoint.y + degree*height/yMax);  
		} 
	}
	else if(nval == TWUN_CENTIMETERS)//����
	{
		beginpoint.x = rect.left;
		beginpoint.y = rect.top-2; //����ͼƬ�Ϸ�
		//����̶�
		for(degree = 0; degree <= 2700; degree += 300)
		{
			dc.MoveTo(beginpoint.x + degree*width/2700, beginpoint.y-width/9/3);
			dc.LineTo(beginpoint.x + degree*width/2700, beginpoint.y);
			CString str;
			str.Format(_T("%d"), (degree/100));
			dc.TextOut(beginpoint.x + degree*width/2700, beginpoint.y-width/9/3, str);
		}
		//��С�̶�
		for(degree = 60; degree <= 2700; degree += 60)  
		{  
			dc.MoveTo(beginpoint.x + degree*width/2700, beginpoint.y - width/9/5);  
			dc.LineTo(beginpoint.x + degree*width/2700, beginpoint.y);  
		}  

		//�������
		//ָ����������ƫ��
		beginpoint.x = rect.left-2;//����ͼƬ���
		beginpoint.y = rect.top; 
		//���̶���
		dc.SetTextAlign(TA_CENTER | TA_LEFT);//���̶������ֱ�ע�ڿ̶��ߵ����
		dc.SetBkMode(TRANSPARENT);//������ɫ����

		//����̶�
		for(degree = 0; degree <= 4200; degree += 300)
		{
			dc.MoveTo(beginpoint.x - height/14/3, beginpoint.y + degree*height/4200);
			dc.LineTo(beginpoint.x, beginpoint.y + degree*height/4200);
			CString str;
			str.Format(_T("%d"), (degree/100));
			dc.TextOut(beginpoint.x - height/14/3-10, beginpoint.y + degree*height/4200-7, str); //ʹ�������߾���
		}
		//��С�̶�
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

	m_pUI->SetCapValueInt(UDSCAP_DOCS_IN_ADF, 1);//Ԥ��ʱֻɨ��һ��
	m_pUI->TW_SaveProfileToFile("�ϴ�ʹ��ģ��");

	BYTE *data = NULL; //ͼ������
	data = m_pUI->PreView();  

	if(data != NULL)
	{
		// ����ͼƬ
		GetBmpFilePath();//Ϊm_bmpFilePath��ֵ

		CFile file;
		try
		{
			if(file.Open(m_bmpFilePath, CFile::modeWrite | CFile::modeCreate))
			{
				//д���ļ�
				file.Write((LPSTR)&(m_pUI->m_bmpFileHeader), sizeof(BITMAPFILEHEADER)); // д�ļ�ͷ
				file.Write((LPSTR)&(m_pUI->m_bmpInfoHeader), sizeof(BITMAPINFOHEADER)); // д��Ϣͷ
				if (m_pUI->m_nBpp < 16)
				{			
					DWORD dwColors = 0;
					if (true == m_pUI->GetColorsUsed(m_pUI->m_nBpp, dwColors))
					{
						file.Write((LPSTR)(m_pUI->m_bmpLpRGB),sizeof(RGBQUAD) * dwColors); // д��ɫ��
					}	

					if (m_pUI->m_bmpLpRGB)
					{
						delete []m_pUI->m_bmpLpRGB;
						m_pUI->m_bmpLpRGB = NULL;
					}
				}
				file.Write(data, m_pUI->m_nDIBSize); // д����
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

		Invalidate(); //ֱ��ˢ�£�OnPaint��ʵ��DrawImage();
	}

	UpdateData(FALSE);
}

void CBase_Tab_Preview::DrawImage(void) 
{
	UpdateData(TRUE);
	//��ʾͼƬ
	CWnd *pWnd = GetDlgItem(IDC_TABPREVIEW_PREPICTURE); 
	CDC* pDC = pWnd->GetDC();
	HDC hDC = pDC->GetSafeHdc();

	IplImage* img = cvLoadImage((CT2CA)m_bmpFilePath, 1);
	if(img != NULL)
	{
		//��������������ӣ�ʹͼ����ʾ��ʧ��
		CRect newRect;
		int width = img->width;
		int height = img->height;

		if(width <= m_endrect.Width() && height <= m_endrect.Height()) //СͼƬ��������
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
		MessageBox(TEXT("������ʱ�ļ���ʧ�ܣ�"));
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

//DrawToHdcϵ�к���
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
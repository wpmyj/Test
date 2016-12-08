
#include "stdafx.h"
//#include "udsprework.h"
#include "SGCallBack.h"
#include "public.h"
#include "ximage.h"  // CXImage
#include <vector>

#pragma comment(lib,"cximage.lib")
#pragma comment(lib,"jasper.lib")
#pragma comment(lib,"Jpeg.lib")
#pragma comment(lib,"jbig.lib")
#pragma comment(lib,"libdcr.lib")
#pragma comment(lib,"mng.lib")
#pragma comment(lib,"png.lib")
#pragma comment(lib,"zlib.lib")
#pragma comment(lib,"Tiff.lib")

#pragma comment(lib, "Winmm.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern  HINSTANCE g_hinstance;
extern void GetFilePath( char* szFileName, char* szFilePath);

vector<CUST_IMAGEINFO> g_vecCust_ImageInfo;

//extern CUdsPreWorkApp NEAR theApp;
/////////////////////////////////////////////////////////////////////////////
// CUDSCapture

CSGCallBack::CSGCallBack()
{
	m_nWidth = m_nHeight = 0;
	m_nPixels = m_nFrames = m_nBackFrames = 0;
	m_strPath.Empty();
	m_pCapture = NULL;
	m_pBaseBuffer = m_pTmpBuffer = m_pBuf1 = m_pBuf2 = NULL;
	m_pHoughBuf = NULL;
	m_nPreviewWidth = m_nPreviewHeight = m_nPreviewBytes = 0;
	m_lTmpBufferSize = 0;
	m_nAngle = m_nDistance = 0;

	m_nTempFileCount = 1;
}

CSGCallBack::~CSGCallBack()
{
	if (m_pTmpBuffer != NULL)  // Release m_pTmpBuffer buffer for Rotating Preview
	{
		delete []m_pTmpBuffer;  m_pTmpBuffer = NULL;  m_lTmpBufferSize = 0;
	}
	ReleaseBuffer();
}

void CSGCallBack::ReleaseBuffer()
{
	m_nPreviewWidth = m_nPreviewHeight = m_nPreviewBytes = 0;
	if (m_pBaseBuffer != NULL)  // Release m_pBaseBuffer buffer
		delete []m_pBaseBuffer;
	if (m_pBuf1 != NULL)  // Release m_pBuf1 buffer
		delete []m_pBuf1;
	if (m_pBuf2 != NULL)  // Release m_pBuf2 buffer
		delete []m_pBuf2;
	if (m_pHoughBuf != NULL)  // Release m_pHoughBuf buffer
		delete []m_pHoughBuf;
	if (m_pTmpBuffer != NULL)  // Release m_pTmpBuffer buffer
		delete []m_pTmpBuffer;
}

STDMETHODIMP_(ULONG) CSGCallBack::AddRef()
{ return 2; }

STDMETHODIMP_(ULONG) CSGCallBack::Release()
{ return 1; }

STDMETHODIMP CSGCallBack::QueryInterface(REFIID riid, void ** ppv)
{
	if( riid == IID_ISampleGrabberCB || riid == IID_IUnknown )
	{ 
		*ppv = (void *) static_cast<ISampleGrabberCB*> ( this );
		return NOERROR;
	} 
	return E_NOINTERFACE;
}

STDMETHODIMP CSGCallBack::SampleCB( double SampleTime, IMediaSample * pSample )
{
	return 0;
}

#define WM_IMAGEREADY (WM_USER + 1)
#define WM_IMAGESAVED (WM_USER + 2)
STDMETHODIMP CSGCallBack::BufferCB( double dblSampleTime, BYTE * pBuffer, long lBufferSize )
// STDMETHODIMP=HRESULT   __stdcall 
{
	if ( !pBuffer )
		return E_POINTER;
	long x, y, x0, y0, x1, y1, x2, y2;

	// Pre process: Gray, Predefined field
	if (m_pCapture->m_Auto.autoClip == false && m_pCapture->m_Auto.docWidth != -1)  // 手动拍摄: Predefined field
	{
		long n = m_pCapture->m_Auto.docWidth * m_nWidth / 100;
		if (n > 0 && n < m_nWidth)
		{
			x1 = (m_nWidth - n)  / 2;
			x2 = x1 + n - 1;
		}
		else
		{
			x1 = 0;  x2 = m_nWidth - 1;
		}
		n = m_pCapture->m_Auto.docHeight * m_nHeight / 100;
		if (n > 0 && n < m_nHeight)
		{
			y1 = (m_nHeight - n) / 2;
			y2 = y1 + n - 1;
		}
		else
		{
			y1 = 0;  y2 = m_nHeight - 1;
		}
	}
	else  // Full field or Auto clip
	{
		x1 = 0;  x2 = m_nWidth - 1;
		y1 = 0;  y2 = m_nHeight - 1;
	}
	if (m_pCapture->m_Auto.autoClip == true)  // 自动拍摄：检查是否转灰度
	{
		if (m_pCapture->m_Auto.imageType == 1)  // 1-Gray: Change Color to Gray
		{
			long nBytes = (m_nWidth*3 + 3) / 4 * 4;  // DIB;
			long nPos, R, G, B;
			for (y = 0; y < m_nHeight; y++)
			{
				nPos = y * nBytes;
				for (x = 0; x < m_nWidth; x++)
				{
					B = pBuffer[nPos];  G = pBuffer[nPos+1];  R = pBuffer[nPos+2];
#if 1  // 0.299R + 0.587G + 0.114B
						pBuffer[nPos] = pBuffer[nPos+1] = pBuffer[nPos+2] = (BYTE)(0.299*R + 0.587*G + 0.114*B);
#else  // (R+G+B) / 3
					pBuffer[nPos] = pBuffer[nPos+1] = pBuffer[nPos+2] = (BYTE)((R+G+B) / 3);
#endif
					nPos += 3;
				}
			}
		}
	}
	else  // 手动拍摄：检查是否转灰度、画边框
	{
		int FW = 10;  // Frame Width
		if (m_pCapture->m_Auto.imageType == 1)  // 1-Gray: Change Color to Gray
		{
			long nBytes = (m_nWidth*3 + 3) / 4 * 4;  // DIB;
			long nPos, R, G, B;
			for (y = 0; y < m_nHeight; y++)
			{
				nPos = y * nBytes;
				for (x = 0; x < m_nWidth; x++)
				{
					B = pBuffer[nPos];  G = pBuffer[nPos+1];  R = pBuffer[nPos+2];
			
					if ((x1-x>=1 && x1-x<=FW || x-x2>=1 && x-x2<=FW) && (y >= y1-FW && y <= y2+FW) ||
						    (x >= x1 && x <= x2) && (y1-y>=1 && y1-y<=FW || y-y2>=1 && y-y2<=FW) )  // 边缘像素：紫色显示
					{  pBuffer[nPos] = 255;  pBuffer[nPos+1] = 0;  pBuffer[nPos+2] = 255;  }
					else
					{
#if 1  // 0.299R + 0.587G + 0.114B
						pBuffer[nPos] = pBuffer[nPos+1] = pBuffer[nPos+2] = (BYTE)(0.299*R + 0.587*G + 0.114*B);
#else  // (R+G+B) / 3
						pBuffer[nPos] = pBuffer[nPos+1] = pBuffer[nPos+2] = (BYTE)((R+G+B) / 3);
#endif
					}
					nPos += 3;
				}
			}
		}
		else if (m_pCapture->m_Auto.docWidth != -1)  // 手动拍摄时指定区域的预览：画边框
		{
			long nBytes = (m_nWidth*3 + 3) / 4 * 4;  // DIB;
			long nPos;
			for (y = 0; y < m_nHeight; y++)
			{
				nPos = y * nBytes;
				for (x = 0; x < m_nWidth; x++)
				{
					/*
					if (x < x1 || x > x2 || y < y1 || y > y2)  // 不在指定区域的像素变暗
					{  pBuffer[nPos] /= 2;  pBuffer[nPos+1] /= 2;  pBuffer[nPos+2] /= 2;  }
					*/
					if ((x1-x>=1 && x1-x<=FW || x-x2>=1 && x-x2<=FW) && (y >= y1-FW && y <= y2+FW) ||
						(x >= x1 && x <= x2) && (y1-y>=1 && y1-y<=FW || y-y2>=1 && y-y2<=FW) )  // 边缘像素：紫色显示
					{  pBuffer[nPos] = 255;  pBuffer[nPos+1] = 0;  pBuffer[nPos+2] = 255;  }
					nPos += 3;
				}
			}
		}
	}

	if ( m_pCapture->m_bIsPreview)  // 检查图像，启动拍照
	{
		if ( IsStableDocument(pBuffer, lBufferSize) == TRUE )
			m_pCapture->m_pEvent->SetNotifyWindow((OAHWND)(m_pCapture->m_hDlgWnd), WM_IMAGEREADY, 0);
	}
	else  // 保存图像，再启动预览
	{
		SaveImage(pBuffer, lBufferSize);
		//::MessageBox(NULL,TEXT("After SaveImage()!"),NULL,MB_OK);
		m_pCapture->m_pEvent->SetNotifyWindow((OAHWND)(m_pCapture->m_hDlgWnd), WM_IMAGESAVED, 0);
	}

	// 旋转显示
	if (m_pCapture->m_Auto.imageOrientation >= 1 && m_pCapture->m_Auto.imageOrientation <= 3)  // 旋转预览
	{
		if (m_pTmpBuffer == NULL || m_lTmpBufferSize < lBufferSize)  // 分配内存
		{
			if (m_pTmpBuffer != NULL)
			{
				delete []m_pTmpBuffer;  m_pTmpBuffer = NULL;  m_lTmpBufferSize = 0;
			}
			m_pTmpBuffer = new BYTE[lBufferSize];
			if (m_pTmpBuffer == NULL)
			{
				m_lTmpBufferSize = 0;  return 0;
			}
			m_lTmpBufferSize = lBufferSize;
		}
		// Copy pBuffer to m_pTmpBuffer
		memcpy(m_pTmpBuffer, pBuffer, lBufferSize);

		long nBytes, nPos, nPos0;
		nBytes = (m_nWidth*3 + 3) / 4 * 4;  // DIB;
		if (m_pCapture->m_Auto.imageOrientation == 2) // Rotate 180
		{
			for (y = 0; y < m_nHeight; y++)
				for (x = 0; x < m_nWidth; x++)
				{
					nPos0 = y*nBytes + x*3;  nPos = (m_nHeight-y-1)*nBytes + (m_nWidth-x-1)*3;
					pBuffer[nPos0] = m_pTmpBuffer[nPos];
					pBuffer[nPos0+1] = m_pTmpBuffer[nPos+1];
					pBuffer[nPos0+2] = m_pTmpBuffer[nPos+2];
				}
			return 0;
		}

		memset(pBuffer, 128, lBufferSize);
		/*
		for (y = 0; y < m_nHeight; y++)
			for (x = 0; x < m_nWidth; x++)
			{
				nPos = y*nBytes + x*3;
				pBuffer[nPos] = 128;  pBuffer[nPos+1] = 0;  pBuffer[nPos+2] = 128;
			}
		*/
		double f;
		if (m_nWidth > m_nHeight)
			f = (double)m_nHeight / m_nWidth;
		else
			f = (double)m_nWidth / m_nHeight;
		long w, h;
		w = (long)(f * m_nHeight);  h = (long)(f * m_nWidth);
		x1 = (m_nWidth -  w) / 2;  x2 = x1 + w - 1;
		y1 = (m_nHeight - h) / 2;  y2 = y1 + h - 1;
		for (y0 = y1; y0 <= y2; y0++)
			for (x0 = x1; x0 <= x2; x0++)
			{
				nPos0 = y0*nBytes + x0*3;
				x = (y0 - y1) / f;  y = (x0 - x1) / f;
				if (m_pCapture->m_Auto.imageOrientation == 1) // Rotate 90
					nPos = y*nBytes + (m_nWidth-x-1)*3;
				else //if (m_pCapture->m_Auto.imageOrientation == 3) // Rotate 270
					nPos = (m_nHeight-y-1)*nBytes + x*3;
				pBuffer[nPos0] = m_pTmpBuffer[nPos];
				pBuffer[nPos0+1] = m_pTmpBuffer[nPos+1];
				pBuffer[nPos0+2] = m_pTmpBuffer[nPos+2];
			}
	}

	return 0;
}

BOOL CSGCallBack::IsStableDocument( BYTE * pBuffer, long lBufferSize )  // 预处理预览图像
{
	if (m_pCapture->m_bCaptureBackground == true)
	{
		m_pCapture->m_bCaptureBackground = false;
		// Save the Background Image
		if (m_nPreviewWidth < m_nWidth || m_nPreviewHeight < m_nHeight ||
			m_pBaseBuffer == NULL)  // Allocate or Reallocate memory
		{
			ReleaseBuffer();
			// Allocate memory:m_pBaseBuffer, m_pHoughBuf, m_pBuf1, m_pBuf2
			m_nAngle = 180;  m_nDistance = (long)( sqrt((double)m_nWidth*m_nWidth + (double)m_nHeight*m_nHeight) + 1);
			m_pBaseBuffer = new BYTE[lBufferSize];  m_pHoughBuf = new long[m_nAngle*(2*m_nDistance+1)];
			m_pBuf1 = new BYTE[m_nWidth*m_nHeight];  m_pBuf2 = new BYTE[m_nWidth*m_nHeight];
			if ( m_pBaseBuffer == NULL || m_pHoughBuf == NULL || m_pBuf1 == NULL || m_pBuf2 == NULL)  // Memory error
			{
				ReleaseBuffer();  return FALSE;
			}
		}
		memcpy(m_pBaseBuffer, pBuffer, lBufferSize);  // 抓拍背景
		//memset(m_pBaseBuffer, 20, lBufferSize);  // 固定背景
		m_pCapture->m_bIsBackground = true;
		m_pCapture->m_bIsAutoClipping = true;
		m_nPreviewWidth = m_nWidth;  m_nPreviewHeight = m_nHeight;
		m_nPreviewBytes = (m_nPreviewWidth*3 + 3) / 4 * 4;  // DIB;
		m_nPixels = m_nFrames = 0;
		return FALSE;
	}

	if (m_pCapture->m_bIsAutoClipping != true)  // m_pCapture->m_bIsAutoClipping == true means being in Auto Clip Photo
		return FALSE;

	// Compare with the Background Image
	long x, y, nPos1, nPos2, nPixels, nAllPixels, nRatio, nEdgePixels, nMaxEdgePixels;
	long x0, y0, x1, y1, x2, y2, W=50;
	long Rb, Gb, Bb, R, G, B, Tm, Te, E;
	bool isMoving=false;
	Tm = m_pCapture->m_Auto.grayT;  Te = Tm / 2;  E = m_pCapture->m_Auto.edgeWidth;
	nAllPixels = 0;  nMaxEdgePixels = 0;
	for (y = 0; y < m_nHeight; y += W)
		for (x = 0; x < m_nWidth; x += W)
		{
			x1 = x;  y1 = y;
			x2 = x + W - 1;  if (x2 >= m_nWidth)  x2 = m_nWidth - 1;
			y2 = y + W - 1;  if (y2 >= m_nHeight)  y2 = m_nHeight - 1;
			nPixels = 0;  nEdgePixels = 0;
			for (y0 = y1; y0 <= y2;  y0++)
			{
				nPos1 = y0*m_nPreviewBytes + x1*3;  nPos2 = y0*m_nWidth + x1;
				for (x0 = x1; x0 <= x2;  x0++)
				{
					Rb = m_pBaseBuffer[nPos1];  Gb = m_pBaseBuffer[nPos1+1];  Bb = m_pBaseBuffer[nPos1+2];
					R = pBuffer[nPos1];  G = pBuffer[nPos1+1];  B = pBuffer[nPos1+2];
					if (abs(R-Rb) > Tm || abs(G-Gb) > Tm || abs(B-Bb) > Tm)  // Moving pixel ?
					{
						m_pBuf1[nPos2] = 1;  // Moving pixel
						nPixels++;
					}
					else
						m_pBuf1[nPos2] = 0;  // Stable pixel
					if (x0 < E || x0 >= m_nWidth-E || y0 < E || y0 >= m_nHeight-E)  // Moving edge pixel?
					{
						if (abs(R-Rb) > Te || abs(G-Gb) > Te || abs(B-Bb) > Te)
							nEdgePixels++;
					}
					//pBuffer[nPos1] /= 2;  pBuffer[nPos1+1] /= 2;  pBuffer[nPos1+2] /= 2;  // 先变暗
					nPos1 += 3;  nPos2++;
				}
			}
			nRatio = 100 * nPixels / (x2-x1+1) / (y2-y1+1);
			if (nRatio >= m_pCapture->m_Auto.moveRatio)  // Document move in
				isMoving = true;
			nAllPixels += nPixels;  // Total Moving Pixels
			if (nEdgePixels > nMaxEdgePixels)
				nMaxEdgePixels = nEdgePixels;  // Max Edge Moving Pixels
		}
	if ( isMoving )  // Document move in
	{
		m_nBackFrames = 0;
		if (m_nFrames == 0)  // The 1st stable image
			m_nPixels = nAllPixels;
		if ( abs(nAllPixels-m_nPixels) <= m_pCapture->m_Auto.stayPixel )  // Document stable
		{
			if (nMaxEdgePixels > m_pCapture->m_Auto.edgePixel)  // Hand in Camera field
			{
				
				m_nPixels = m_nFrames = 0;
				return FALSE;
			}
			m_nFrames++;
			if (m_nFrames >= m_pCapture->m_Auto.stayFrame)  // Document stable reach a certain frames
			{
				m_nPixels = m_nFrames = 0;
				if (m_pCapture->m_bIsBackground == true)  // The document is not captured
				{
					AutoClip(pBuffer, lBufferSize);
					m_pCapture->m_bIsBackground = false;
					m_nPixels = m_nFrames = 0;
					return TRUE;  // TRUE: Start Capture
					//return FALSE;  // TRUE: Do not Start Capture
				}
			}
		}
		else  // Document unstable
		{
			m_nPixels = nAllPixels;  m_nFrames = 1;
		}
	}
	else  // No Document move in
	{
		m_nPixels = m_nFrames = 0;
		m_nBackFrames++;
		if (m_nBackFrames >= 1)
			m_pCapture->m_bIsBackground = true;
	}

	return FALSE;
}

BOOL CSGCallBack::AutoClip( BYTE * pBuffer, long lBufferSize )
{
	double f = 3.141592653589793 / 180;

	// m_pBuf1: 1-Moving pixel, 0-Stable pixel
	long x, y, nPos;
	long x0, y0, x1, y1, x2, y2, mpNo, W=2, T=(2*W+1)*(2*W+1)*2/3;
	for (y = 0; y < m_nHeight; y++)  // 去除噪声: Save to m_pBuf2
	{
		nPos = y * m_nWidth;
		for (x = 0; x < m_nWidth; x++)
		{
			if (m_pBuf1[nPos] == 0)  // Stable pixel
			{
				m_pBuf2[nPos] = 0;  nPos++;  continue;
			}
			x1 = x - W;  if (x1 < 0)  x1 = 0;
			x2 = x + W;  if (x2 >= m_nWidth)  x2 = m_nWidth-1;
			y1 = y - W;  if (y1 < 0)  y1 = 0;
			y2 = y + W;  if (y2 >= m_nHeight)  y2 = m_nHeight-1;
			mpNo = 0;
			for (y0 = y1; y0 <= y2; y0++)
				for (x0 = x1; x0 <= x2; x0++)
					mpNo += m_pBuf1[y0*m_nWidth + x0];
			if (mpNo >= T)  // Less than T is Noise
				m_pBuf2[nPos] = 1;
			else
				m_pBuf2[nPos] = 0;
			nPos++;
		}
	}
	for (y = 0; y < m_nHeight; y++)  // 取纵向边缘像素: Save 2 to m_pBuf1
	{
		nPos = y*m_nWidth;  x1 = 0;
		for (x = 0; x < m_nWidth; x++)
		{
			if (m_pBuf2[nPos] == 1)  // The 1st Moving Edge pixel
			{
				m_pBuf1[nPos] = 2;  x1 = x;  break;
			}
			nPos++;
		}
		nPos = y*m_nWidth + m_nWidth-1;
		for (x = m_nWidth-1; x > x1; x--)
		{
			if (m_pBuf2[nPos] == 1)  // The last Moving Edge pixel
			{
				m_pBuf1[nPos] = 2;  break;
			}
			nPos--;
		}
	}
	for (x = 0; x < m_nWidth; x++)  // 取横向边缘像素: Save 2 to m_pBuf1
	{
		nPos = x;  y1 = 0;
		for (y = 0; y < m_nHeight; y++)
		{
			if (m_pBuf2[nPos] == 1)  // The 1st Moving Edge pixel
			{
				m_pBuf1[nPos] = 2;  y1 = y;  break;
			}
			nPos += m_nWidth;
		}
		nPos = (m_nHeight-1)*m_nWidth + x;
		for (y = m_nHeight-1; y > y1; y--)
		{
			if (m_pBuf2[nPos] == 1)  // The last Moving Edge pixel
			{
				m_pBuf1[nPos] = 2;  break;
			}
			nPos--;
		}
	}
	// Hough to find the longest line
	long a, r;
	for (r = -m_nDistance; r <= m_nDistance; r++)  // 找方向: Clear m_pHoughBuf
	{
		nPos = (r+m_nDistance) * 180;
		for (a = -89; a <= 90; a++)
		{
			m_pHoughBuf[nPos] = 0;  nPos++;
		}
	}
	for (y = 0; y < m_nHeight; y++)  // 找方向: Save to m_pHoughBuf and Moving Edge pixel
	{
		nPos = y * m_nWidth;
		for (x = 0; x < m_nWidth; x++)
		{
			if (m_pBuf1[nPos] != 2)  // Not Moving Edge pixel
			{
				nPos++;  continue;
			}
			for (a = -89; a <= 90; a++)
			{
				r = (long)(x*cos(a*f) + y*sin(a*f) + 0.5);
				if (r < -m_nDistance || r > m_nDistance)
				{
					AfxMessageBox("Internal Error: Out of Range");
					continue;
				}
				m_pHoughBuf[(r+m_nDistance)*180 + (a+89)]++;
			}
			nPos++;
		}
	}
	long maxPixel=0, maxA, maxR=m_nDistance+1;
	for (r = -m_nDistance; r <= m_nDistance; r++)  // 找方向: Find the largest
	{
		nPos = (r+m_nDistance) * 180;
		for (a = -89; a <= 90; a++)
		{
			if (m_pHoughBuf[nPos] > maxPixel)
			{
				maxPixel = m_pHoughBuf[nPos];  maxA = a;  maxR = r;
			}
			nPos++;
		}
	}
	if (maxA >= -45 && maxA <= 45)  // The largest is Width direction
	{
		m_nWidthA = maxA;
		if (maxA > 0)
			m_nHeightA = maxA - 90;
		else
			m_nHeightA = maxA + 90;
	}
	else  // The largest is Height direction
	{
		m_nHeightA = maxA;
		if (maxA > 0)
			m_nWidthA = maxA - 90;
		else
			m_nWidthA = maxA + 90;
	}
	m_nWidthR1 = m_nWidthR2 = m_nDistance + 1;
	m_nHeightR1 = m_nHeightR2 = m_nDistance + 1;
	for (r = m_nDistance; r >= -m_nDistance; r--)  // Find the widthR1 & heightR1
	{
		if (m_pHoughBuf[(r+m_nDistance)*180 + (m_nWidthA+89)] > 0 && m_nWidthR1 == m_nDistance+1)
			m_nWidthR1 = r;
		if (m_pHoughBuf[(r+m_nDistance)*180 + (m_nHeightA+89)] > 0 && m_nHeightR1 == m_nDistance+1)
			m_nHeightR1 = r;
		if (m_nWidthR1 != m_nDistance+1 && m_nHeightR1 != m_nDistance+1)
			break;
	}
	for (r = -m_nDistance; r <= m_nDistance; r++)  // Find the widthR2 & heightR2
	{
		if (m_pHoughBuf[(r+m_nDistance)*180 + (m_nWidthA+89)] > 0 && m_nWidthR2 == m_nDistance+1)
			m_nWidthR2 = r;
		if (m_pHoughBuf[(r+m_nDistance)*180 + (m_nHeightA+89)] > 0 && m_nHeightR2 == m_nDistance+1)
			m_nHeightR2 = r;
		if (m_nWidthR2 != m_nDistance+1 && m_nHeightR2 != m_nDistance+1)
			break;
	}
	if (m_nWidthR1 > m_nWidthR2)
	{
		r = m_nWidthR1;  m_nWidthR1 = m_nWidthR2;  m_nWidthR2 = r;
	}
	if (m_nHeightR1 > m_nHeightR2)
	{
		r = m_nHeightR1;  m_nHeightR1 = m_nHeightR2;  m_nHeightR2 = r;
	}
	if (maxR == m_nDistance+1 || m_nWidthR1 == m_nDistance+1 || m_nWidthR2 == m_nDistance+1 ||
		m_nHeightR1 == m_nDistance+1 || m_nHeightR2 == m_nDistance+1)
	{
		AfxMessageBox("No edge found");
	}
	long Rw, Rh;
	for (y = 0; y < m_nHeight; y++)  // 找方向: DrawLine
	{
		nPos = y*m_nPreviewBytes;
		for (x = 0; x < m_nWidth; x++)
		{
			Rw = (long)(x*cos(m_nWidthA*f) + y*sin(m_nWidthA*f) + 0.5);
			Rh = (long)(x*cos(m_nHeightA*f) + y*sin(m_nHeightA*f) + 0.5);
			if ( (Rw == m_nWidthR1 || Rw == m_nWidthR2) && Rh >= m_nHeightR1 && Rh <= m_nHeightR2 ||
				 (Rh == m_nHeightR1 || Rh == m_nHeightR2) && Rw >= m_nWidthR1 && Rw <= m_nWidthR2 )
			{
				pBuffer[nPos] = 255;  pBuffer[nPos+1] = 0;  pBuffer[nPos+2] = 255;
			}
			else if (Rw > m_nWidthR1 && Rw < m_nWidthR2 && Rh > m_nHeightR1 && Rh < m_nHeightR2)
			{
				//pBuffer[nPos] *= 2;  pBuffer[nPos+1] *= 2;  pBuffer[nPos+2] *= 2;  // 恢复自动裁切区域的亮度
			}
			nPos += 3;
		}
	}
	// Center point coordinate
	m_nX0 = ( (m_nHeightR1+m_nHeightR2)/2.0*sin(m_nWidthA*f) - (m_nWidthR1+m_nWidthR2)/2.0*sin(m_nHeightA*f) ) /
			( sin(m_nWidthA*f)*cos(m_nHeightA*f) - cos(m_nWidthA*f)*sin(m_nHeightA*f) );
	m_nY0 = ( (m_nHeightR1+m_nHeightR2)/2.0*cos(m_nWidthA*f) - (m_nWidthR1+m_nWidthR2)/2.0*cos(m_nHeightA*f) ) /
			( cos(m_nWidthA*f)*sin(m_nHeightA*f) - sin(m_nWidthA*f)*cos(m_nHeightA*f) );
	
	return TRUE;
}

BOOL CSGCallBack::SaveImage( BYTE * pBuffer, long lBufferSize )
{

	// 播放音效
	//AfxMessageBox("SaveImage");
	TCHAR szFilePath[MAX_PATH];
	GetFilePath(FILENAME_WAV_CAMERA1, szFilePath);
	sndPlaySound(szFilePath, SND_ASYNC);
	
	long nAllBytes = (m_nWidth*3  + 3) / 4 * 4;  // DIB;
	BYTE *pDIB=NULL, *pNewBuffer=NULL;
	long nDIBSize, nNewWidth, nNewHeight, nNewBytes;
	if (m_pCapture->m_Auto.autoClip == false)  // 手动拍摄
	{
		if (m_pCapture->m_Auto.docWidth == -1)  // Full field
		{
			nNewWidth = m_nWidth;  nNewHeight = m_nHeight;
			nNewBytes = (nNewWidth*3 + 3) / 4 * 4;  // DIB;
			nDIBSize = sizeof(BITMAPINFOHEADER) + nNewBytes*nNewHeight;
			pDIB = new BYTE[nDIBSize];
			if (pDIB == NULL)
				return FALSE;
			pNewBuffer = pDIB + sizeof(BITMAPINFOHEADER);
			memcpy(pNewBuffer, pBuffer, lBufferSize);
		}
		else  // Predefined field
		{
			long x1, x2, y1, y2, n;
			n = m_pCapture->m_Auto.docWidth * m_nWidth / 100;
			if (n > 0 && n < m_nWidth)
			{
				x1 = (m_nWidth - n)  / 2;
				x2 = x1 + n - 1;
			}
			else
			{
				x1 = 0;  x2 = m_nWidth - 1;
			}
			n = m_pCapture->m_Auto.docHeight * m_nHeight / 100;
			if (n > 0 && n < m_nHeight)
			{
				y1 = (m_nHeight - n) / 2;
				y2 = y1 + n - 1;
			}
			else
			{
				y1 = 0;  y2 = m_nHeight - 1;
			}

			nNewWidth  = x2 - x1 + 1;  nNewHeight = y2 - y1 + 1;
			nNewBytes = (nNewWidth*3 + 3) / 4 * 4;  // DIB;
			nDIBSize = sizeof(BITMAPINFOHEADER) + nNewBytes*nNewHeight;
			pDIB = new BYTE[nDIBSize];
			if (pDIB == NULL)
				return FALSE;
			pNewBuffer = pDIB + sizeof(BITMAPINFOHEADER);
			for (long y = y1; y <= y2; y++)
			{
				memcpy(pNewBuffer+(y-y1)*nNewBytes, pBuffer+y*nAllBytes+x1*3, nNewWidth*3);
			}
		}
	}
	else  // Auto Clip: Clip parameters generated by AutoClip()
	{
		double f = 3.141592653589793 / 180;
		double WRatio = (double)m_nWidth  / m_nPreviewWidth;
		double HRatio = (double)m_nHeight / m_nPreviewHeight;
		double x0 = m_nX0 * WRatio;
		double y0 = m_nY0 * HRatio;
		nNewWidth  = (long)( (m_nWidthR2-m_nWidthR1+1)   * WRatio );
		nNewHeight = (long)( (m_nHeightR2-m_nHeightR1+1) * HRatio );
		nNewBytes = (nNewWidth*3 + 3) / 4 * 4;  // DIB;
		
		nDIBSize = sizeof(BITMAPINFOHEADER) + nNewBytes*nNewHeight;
		pDIB = new BYTE[nDIBSize];
		if (pDIB == NULL)
			return FALSE;
		pNewBuffer = pDIB + sizeof(BITMAPINFOHEADER);

		long x, y, x1, y1, nPos, nPos1;
		double x2, y2;
		for (y = 0; y < nNewHeight; y++)
		{
			nPos = y * nNewBytes;
			for (x = 0; x < nNewWidth; x++)
			{
				x2 = x - nNewWidth/2.0;
				y2 = y - nNewHeight/2.0;
				x1 = (long)( x2*cos(m_nWidthA*f) - y2*sin(m_nWidthA*f) + x0 + 0.5);  // Move + Rotate
				y1 = (long)( x2*sin(m_nWidthA*f) + y2*cos(m_nWidthA*f) + y0 + 0.5);
				if (x1 >= 0 && x1 < m_nWidth && y1 >= 0 && y1 < m_nHeight)
				{
					nPos1 = y1*nAllBytes + x1*3;
					pNewBuffer[nPos]   = pBuffer[nPos1];
					pNewBuffer[nPos+1] = pBuffer[nPos1+1];
					pNewBuffer[nPos+2] = pBuffer[nPos1+2];
				}
				else
				{
					pNewBuffer[nPos] = pNewBuffer[nPos+1] = pNewBuffer[nPos+2] = 0;
				}
				nPos += 3;
			}
		}
	}
	BITMAPINFOHEADER *pBIH = (BITMAPINFOHEADER *)pDIB;
	memset( pBIH, 0, sizeof(BITMAPINFOHEADER) );
	pBIH->biSize = sizeof(BITMAPINFOHEADER);
	pBIH->biWidth = nNewWidth;
	pBIH->biHeight = nNewHeight;
	pBIH->biPlanes = 1;
	pBIH->biBitCount = 24;
	pBIH->biCompression = BI_RGB;
	pBIH->biXPelsPerMeter = 150;
	pBIH->biYPelsPerMeter = 150;
	pBIH->biClrUsed = 0;

	// 增强文档
	if (m_pCapture->m_Auto.autoEnhance == true)  // Auto document enhancement
	{
		long x, y, nPos, x0, y0, x1, y1, x2, y2, W;
		long R, G, B, GT=24;
		long r=0, g=0, b=0, pNo=0, rSum=0, gSum=0, bSum=0;

#if 0  // 低通滤波
		if (m_pTmpBuffer == NULL || m_lTmpBufferSize < nNewBytes*nNewHeight)  // 分配内存
		{
			if (m_pTmpBuffer != NULL)
			{
				delete []m_pTmpBuffer;  m_pTmpBuffer = NULL;  m_lTmpBufferSize = 0;
			}
			m_lTmpBufferSize = nNewBytes * nNewHeight;
			m_pTmpBuffer = new BYTE[m_lTmpBufferSize];
			if (m_pTmpBuffer == NULL)
				m_lTmpBufferSize = 0;
		}
		if (m_pTmpBuffer != NULL && m_lTmpBufferSize >= nNewBytes*nNewHeight)  // 分配内存
		{
			W = 1;  // Filter window width
			for (y0 = 0; y0 < nNewHeight; y0++)
			{
				y1 = y0 - W;  if (y1 < 0)  y1 = 0;
				y2 = y0 + W;  if (y2 >= nNewHeight)  y2 = nNewHeight - 1;
				for (x0 = 0; x0 < nNewWidth; x0++)
				{
					nPos = y0*nNewBytes + x0*3;
					R = pNewBuffer[nPos];  G = pNewBuffer[nPos+1];  B = pNewBuffer[nPos+2];
					x1 = x0 - W;  if (x1 < 0)  x1 = 0;
					x2 = x0 + W;  if (x2 >= nNewWidth)  x2 = nNewWidth - 1;
					pNo = 0;  rSum = gSum = bSum = 0;
					for (y = y1; y <= y2; y++)
					{
						nPos = y*nNewBytes + x1*3;
						for (x = x1; x <= x2; x++)
						{
							r = pNewBuffer[nPos];  g = pNewBuffer[nPos+1];  b = pNewBuffer[nPos+2];
							if (abs(r-R) > GT || abs(g-G) > GT || abs(b-B) > GT)
								nPos += 3;
							else
							{
								rSum += r;  gSum += g;  bSum += b;  pNo++;  nPos += 3;
							}
						}
					}
					if (pNo > 0)
					{
						nPos = y0*nNewBytes + x0*3;
						m_pTmpBuffer[nPos]   = (BYTE)(rSum / pNo);
						m_pTmpBuffer[nPos+1] = (BYTE)(gSum / pNo);
						m_pTmpBuffer[nPos+2] = (BYTE)(bSum / pNo);
					}
				}
			}
			memcpy(pNewBuffer, m_pTmpBuffer, nNewBytes*nNewHeight);  // Copy m_pTmpBuffer to pNewBuffer
		}
#endif

#if 1  // 分隔增强
		long T, A, Rb, Gb, Bb, P[256], GW=1, BT=16;
		double Rsum, Gsum, Bsum, H, S, I, fac=180/3.141592653589793;
		W = 100;  // Block window width
		if (m_pCapture->m_Auto.imageType == 1)  // 1-Gray: Background
		{
			Rb = Gb = Bb = 255;
		}
		else  // Color: Background
		{
			Rsum = Gsum = Bsum = 0;
			for (y0 = 0; y0 < nNewHeight; y0++)
			{
				nPos = y0 * nNewBytes;
				for (x0 = 0; x0 < nNewWidth; x0++)
				{
					Bsum += pNewBuffer[nPos++];  Gsum += pNewBuffer[nPos++];  Rsum += pNewBuffer[nPos++];
				}
			}
			// Sb: Saturation of background
			S = Rsum;
			if (Gsum < S)  S = Gsum;
			if (Bsum < S)  S = Bsum;
			S = 1 - 3.0*S / (Rsum+Gsum+Bsum);
			if (S < 0)
			{
				Rb = Gb = Bb = 255;
			}
			else
			{
				// Hb: Hue of background
				H = ((Rsum-Gsum)+(Rsum-Bsum))/2.0 / sqrt( (Rsum-Gsum)*(Rsum-Gsum)+(Rsum-Bsum)*(Gsum-Bsum) + 1);
				H = acos(H) * fac;
				if (Bsum > Gsum)  H = 360 - H;
				if (H >= 360)  H = 0;
				// Background Color: I=255, keeping S and H
				I = 255;
				if (H < 120)
				{
					H = 1 + S * cos(H/fac) / cos((60-H)/fac);
					Rb = (long)(I * H);
					Bb = (long)(I * (1-S));
					Gb = 3*I - (Rb+Bb);  if (Gb < 0)  Gb = 0;
				}
				else if (H < 240)
				{
					H -= 120;  H = 1 + S * cos(H/fac) / cos((60-H)/fac);
					Gb = (long)(I * H);
					Rb = (long)(I * (1-S));
					Bb = 3*I - (Rb+Gb);  if (Bb < 0)  Bb = 0;
				}
				else
				{
					H -= 240;  H = 1 + S * cos(H/fac) / cos((60-H)/fac);
					Bb = (long)(I * H);
					Gb = (long)(I * (1-S));
					Rb = 3*I - (Gb+Bb);  if (Rb < 0)  Rb = 0;
				}
				I = Rb;  if (Gb > I)  I = Gb;  if (Bb > I)  I = Bb;
				I = 255 / I;
				Rb = (long)(Rb * I);  Gb = (long)(Gb * I);  Bb = (long)(Bb * I);
			}
		}

		for (y0 = 0; y0 < nNewHeight; y0 += W)
		{
			for (x0 = 0; x0 < nNewWidth; x0 += W)
			{
				x1 = x0;  y1 = y0;
				x2 = x1 + W - 1;  if (x2 >= nNewWidth)   x2 = nNewWidth - 1;
				y2 = y1 + W - 1;  if (y2 >= nNewHeight)  y2 = nNewHeight - 1;
				for (x = 0; x < 256; x++)
					P[x] = 0;
				// Segmentation
				if (m_pCapture->m_Auto.imageType == 1)  // 1-Gray: Segmentation
				{
					for (y = y1; y <= y2; y++)  // Histogram
					{
						nPos = y*nNewBytes + 3*x1;
						for (x = x1; x <= x2; x++)
						{
							P[pNewBuffer[nPos]]++;  nPos += 3;
						}
					}
					T = Otsu(P, &A);  // Otsu Threshold
					//T = Threshold(P, &A);  // Adaptive Threshold
					for (y = y1; y <= y2; y++)
					{
						nPos = y*nNewBytes + 3*x1;
						for (x = x1; x <= x2; x++)
						{
							R = pNewBuffer[nPos];
							if (T == -1)  // No Threshold
							{
								if (R > 100 && R > A-BT)
								{
									pNewBuffer[nPos] = (BYTE)Bb;
									pNewBuffer[nPos+1] = (BYTE)Gb;
									pNewBuffer[nPos+2] = (BYTE)Rb;
								}
							}
							else
							{
								if (R > T && R > A-BT)
								{
									pNewBuffer[nPos] = (BYTE)Bb;
									pNewBuffer[nPos+1] = (BYTE)Gb;
									pNewBuffer[nPos+2] = (BYTE)Rb;
								}
							}
							nPos += 3;
						}
					}
				}
				else  // Color: Segmentation
				{
					for (y = y1; y <= y2; y++)  // Histogram
					{
						nPos = y*nNewBytes + 3*x1;
						for (x = x1; x <= x2; x++)
						{
							R = pNewBuffer[nPos];  G = pNewBuffer[nPos+1];  B = pNewBuffer[nPos+2];
#if 1  // 0.299R + 0.587G + 0.114B
							R = (long)(0.299*R + 0.587*G + 0.114*B);
#else  // (R+G+B) / 3
							R = (R+G+B) / 3;
#endif
							P[R]++;  nPos += 3;
						}
					}
					T = Otsu(P, &A);  // Otsu Threshold
					//T = Threshold(P, &A);  // Adaptive Threshold
					for (y = y1; y <= y2; y++)
					{
						nPos = y*nNewBytes + 3*x1;
						for (x = x1; x <= x2; x++)
						{
							B = pNewBuffer[nPos];  G = pNewBuffer[nPos+1];  R = pNewBuffer[nPos+2];
#if 1  // 0.299R + 0.587G + 0.114B
							R = (long)(0.299*R + 0.587*G + 0.114*B);
#else  // (R+G+B) / 3
							R = (R+G+B) / 3;
#endif
							if (T == -1)  // No Threshold
							{
								if (R > 100 && R > A-BT)
								{
									pNewBuffer[nPos] = (BYTE)Bb;
									pNewBuffer[nPos+1] = (BYTE)Gb;
									pNewBuffer[nPos+2] = (BYTE)Rb;
								}
							}
							else
							{
								if (R > T && R > A-BT)
								{
									pNewBuffer[nPos] = (BYTE)Bb;
									pNewBuffer[nPos+1] = (BYTE)Gb;
									pNewBuffer[nPos+2] = (BYTE)Rb;
								}
							}
							nPos += 3;
						}
					}
				}
				// End of block
			}
		}
#endif

#if 1  // 去噪声
		if (m_pTmpBuffer == NULL || m_lTmpBufferSize < nNewBytes*nNewHeight)  // 分配内存
		{
			if (m_pTmpBuffer != NULL)
			{
				delete []m_pTmpBuffer;  m_pTmpBuffer = NULL;  m_lTmpBufferSize = 0;
			}
			m_lTmpBufferSize = nNewBytes * nNewHeight;
			m_pTmpBuffer = new BYTE[m_lTmpBufferSize];
			if (m_pTmpBuffer == NULL)
				m_lTmpBufferSize = 0;
		}
		if (m_pTmpBuffer != NULL && m_lTmpBufferSize >= nNewBytes*nNewHeight)  // 分配内存
		{
			W = 1;  GT = 32;
			for (y0 = 0; y0 < nNewHeight; y0++)
			{
				y1 = y0 - W;  if (y1 < 0)  y1 = 0;
				y2 = y0 + W;  if (y2 >= nNewHeight)  y2 = nNewHeight - 1;
				for (x0 = 0; x0 < nNewWidth; x0++)
				{
					nPos = y0*nNewBytes + x0*3;
					R = pNewBuffer[nPos];  G = pNewBuffer[nPos+1];  B = pNewBuffer[nPos+2];
					x1 = x0 - W;  if (x1 < 0)  x1 = 0;
					x2 = x0 + W;  if (x2 >= nNewWidth)  x2 = nNewWidth - 1;
					pNo = 0;;
					for (y = y1; y <= y2; y++)
					{
						nPos = y*nNewBytes + x1*3;
						for (x = x1; x <= x2; x++)
						{
							r = pNewBuffer[nPos];  g = pNewBuffer[nPos+1];  b = pNewBuffer[nPos+2];
							if (r > R+GT || g > G+GT || b > B+GT)
								pNo++;
							nPos += 3;
						}
					}
					nPos = y0*nNewBytes + x0*3;
					if (pNo >= (2*W+1)*(2*W+1)-2)
					{
						m_pTmpBuffer[nPos] = m_pTmpBuffer[nPos+1] = m_pTmpBuffer[nPos+2] = 255;
					}
					else
					{
						m_pTmpBuffer[nPos  ] = pNewBuffer[nPos];
						m_pTmpBuffer[nPos+1] = pNewBuffer[nPos+1];
						m_pTmpBuffer[nPos+2] = pNewBuffer[nPos+2];
					}
				}
			}
			memcpy(pNewBuffer, m_pTmpBuffer, nNewBytes*nNewHeight);  // Copy m_pTmpBuffer to pNewBuffer
		}
#endif
	}

	// 识别条码
//	CString strBarcode;
	// 从内存DIB识别条码
//	strBarcode = theApp.BarcodeReader((HANDLE)pDIB, "", m_pCapture->m_bMultiBarcode,
//		m_pCapture->m_strBarcodeType, m_pCapture->m_strBarcodeFormat, m_pCapture->m_nBarcodeLength, m_pCapture->m_bBarcodeRotate);  // 识别条码方向
//	if (strBarcode != "***")  // 播放音效
//		sndPlaySound("SmartScan2.WAV", SND_ASYNC);

	//m_Camera_DirectX.SetImageData(pDIB, nDIBSize);
	//delete []pDIB;
	
	
	// 处理并保存文档
	CxImage *pImage = new CxImage();
	pImage->CreateFromHANDLE( (HANDLE)pDIB );
	delete []pDIB;

	if (m_pCapture->m_Auto.imageType == 1)  // Gray
		pImage->GrayScale();



	bool hasRotate = false;
	


	//// 根据条码自动旋转图像
	//int i = strBarcode.Find(":::", 0);
	//if (i >= 0)  // 自动旋转标记 ":::"
	//{
	//	CString strDirection = strBarcode.Left(i);
	//	strBarcode = strBarcode.Mid(i+3);
	//	if (strDirection == "1")  // Left to Right
	//		hasRotate = true;
	//	else if (strDirection == "2")  // Top to Bottom
	//	{
	//		pImage->RotateLeft();  hasRotate = true;
	//	}
	//	else if (strDirection == "8")  // Bottom to Top
	//	{
	//		pImage->RotateRight();  hasRotate = true;
	//	}
	//	else if (strDirection == "4")  // Right to Left
	//	{
	//		pImage->Rotate180();  hasRotate = true;
	//	}
	//}



	if ( !hasRotate )  // 未根据条码旋转: 根据指定方向旋转
	{
		if (m_pCapture->m_Auto.autoRotate == true)  // 自动旋转
			pImage->RotateRight();
	}
	if (m_nWidth <= 1280)  // Set DPI
	{
		pImage->SetXDPI(100);  pImage->SetYDPI(100);
	}
	else if (m_nWidth <= 2048)
	{
		pImage->SetXDPI(150);  pImage->SetYDPI(150);
	}
	else
	{
		pImage->SetXDPI(200);  pImage->SetYDPI(200);
	}
	
	if (m_pCapture->m_bActive == false)  // 未激活，加水印：SmartCamera Sample
	{
		CxImage::CXTEXTINFO TxtInfo;
		pImage->InitTextInfo(&TxtInfo);
		_tcscpy(TxtInfo.text, _T("SmartCamera Sample"));
		TxtInfo.lfont.lfHeight = -48;//-96;
		TxtInfo.lfont.lfItalic = TRUE;
		TxtInfo.fcolor = RGB(0, 0, 0);
		TxtInfo.opaque = FALSE;  // No background
		pImage->DrawStringEx(0, pImage->GetWidth()/2, pImage->GetHeight()/3, &TxtInfo);
		pImage->DrawStringEx(0, pImage->GetWidth()/2, pImage->GetHeight()/3*2, &TxtInfo);
	}

	bool retval = false;
	CString fileName;
	if (pImage->GetBpp() == 1)  // 黑白图像: TIFF
	{
		fileName.Format("%s~Un%d.tif", m_pCapture->m_strImagePath, m_nTempFileCount);
		pImage->SetCodecOption(3, CXIMAGE_FORMAT_TIF);  // G4 compression
		retval = pImage->Save(fileName, CXIMAGE_FORMAT_TIF);
		
	}
	else
	{
		fileName.Format("%s~Un%d.jpg", m_pCapture->m_strImagePath, m_nTempFileCount);
		//pImage->SetJpegQuality((BYTE)m_pCapture->m_nQuality);  // JPEG compression
		retval = pImage->Save(fileName, CXIMAGE_FORMAT_JPG);
		
	}

	CUST_IMAGEINFO temp;
	temp.imagePath = fileName.GetBuffer();
	temp.imageWidth = pImage->GetWidth();
	temp.imageHeight = pImage->GetHeight();
	temp.XResolution = pImage->GetXDPI();
	temp.YResolution = pImage->GetYDPI();

	g_vecCust_ImageInfo.push_back(temp);

	if ( retval )  // Save file success: Generate Thumbnail JPG
	{
		float h_w = (float)pImage->GetHeight() / (float)pImage->GetWidth();
		pImage->Resample(150, (int)(150*h_w), 1, NULL);
		if ( pImage->GetBpp() < 24 )  // 缩略图全转为24位彩色图，并存为JPG文件
			pImage->IncreaseBpp(24);
		fileName.Replace("~Un", "~UnTh");
		pImage->SetJpegQuality((BYTE)m_pCapture->m_nQuality);  // compression
		retval = pImage->Save(fileName, CXIMAGE_FORMAT_JPG);
		fileName.Replace("~UnTh", "~Un");
	}
	delete pImage;
	CFile hf(fileName, CFile::modeRead);  hf.Close();  // Flush file
	
	
	// 添加到临时文件列表
	//theApp.m_tempFileList.Add(fileName);  // Add image to file list
	//theApp.m_tempBarcodeList.Add(strBarcode);  // Add barcode to file list
	//theApp.m_nTempFileCount += 1;  // 临时文件名编号
	  m_nTempFileCount += 1;
	//m_pCapture->m_strBarcode = strBarcode;  // Save strBarcode for showing in dialog
		m_pCapture->m_nPhotoNo++;
		
	return TRUE;

}

long CSGCallBack::Otsu(long *P, long *bgAverage)  // Select optimal threshold
{
	long t, T, A, minG, maxG;
	double pNo1, pSum1, pNo2, pSum2, A1, A2, Var, maxVar;

	pNo1 = 0;  pSum1 = 0;  pNo2 = 1;  pSum2 = 0;
	minG = 255;  maxG = 0;
	for (t = 0; t < 256; t++)
	{
		if (P[t] == 0)  continue;
		pSum2 += (double)(t+1) * P[t];  pNo2 += P[t];
		if (t < minG)  minG = t;
		if (t > maxG)  maxG = t;
	}
	A = (long)(pSum2 / pNo2 + 0.5) - 1;
	*bgAverage = A;
	maxVar = 0;  T = -1;
	for (t = minG; t < maxG; t++)
	{
		if (P[t] == 0)  continue;
		pSum1 += (double)(t+1) * P[t];  pNo1 += P[t];  A1 = pSum1 / pNo1;
		pSum2 -= (double)(t+1) * P[t];  pNo2 -= P[t];  A2 = pSum2 / pNo2;
		Var = pNo1 * pNo2 * (A1-A2)*(A1-A2);  // between-class variance
		if (Var > maxVar && A2-A1 > 12 && pNo1 > 50 && pNo2 > (pNo1+pNo2)/10)
		{  maxVar = Var;  T = t;  *bgAverage = (long)(A2+0.5) - 1;  }
	}
	return T;
}

long CSGCallBack::Threshold(long *P, long *bgAverage)  // Select optimal threshold
{
	long t, T0, T, A, minG, maxG;
	double pNo1, pSum1, pNo2, pSum2;

	pNo1 = 0;  pSum1 = 0;  pNo2 = 1;  pSum2 = 0;
	minG = 255;  maxG = 0;
	for (t = 0; t < 256; t++)
	{
		if (P[t] == 0)  continue;
		pSum2 += (double)t * P[t];  pNo2 += P[t];
		if (t < minG)  minG = t;
		if (t > maxG)  maxG = t;
	}
	A = (long)(pSum2 / pNo2 + 0.5);
	*bgAverage = A;

	T0 = A;
	for (int i = 0; i < 5; i++)
	{
		pNo1 = 0;  pSum1 = 0;  pNo2 = 1;  pSum2 = 0;
		for (t = minG; t <= maxG; t++)
		{
			if (P[t] == 0)  continue;
			if (t <= T0)
			{
				pSum1 += (double)t * P[t];  pNo1 += P[t];
			}
			else
			{
				pSum2 += (double)t * P[t];  pNo2 += P[t];
			}
		}
		T = (pSum1/pNo1 + pSum2/pNo2) / 2;
		if (abs(T-T0) == 0)
		{
			*bgAverage = (long)(pSum2 / pNo2 + 0.5);  break;
		}
		T0 = T;
	}
	if (abs(pSum1/pNo1 - pSum2/pNo2) <= 16 || pNo2 <= 20)
		T = -1;

	return T;
}

long CSGCallBack::DynamicThreshold(BYTE * pBuffer, long Width, long Height, long WidthBytes)  // Select optimal local threshold
{
	long x, y, x0, y0, x1, y1, x2, y2, nPos, nPos1, WW=100;
	long R, G, B, A, P[256];
	BYTE *buf = new BYTE[Width*Height], *T = new BYTE[Width*Height];

	if (buf == NULL || T == NULL)
	{
		delete []buf;  delete []T;
		return 1;
	}

	nPos = 0;
	for (y = 0; y < Height; y++)  // Get Gray Image
	{
		nPos1 = y*WidthBytes;
		for (x = 0; x < Width; x++)
		{
			R = pBuffer[nPos1];  G = pBuffer[nPos1+1];  B = pBuffer[nPos1+2];  nPos1 += 3;
			buf[nPos] = (R + G + B) / 3;  T[nPos] = -1;  nPos++;
		}
	}

	for (y0 = 0; y0 < Height; y0 += WW/2)  // Block Threshold
	{
		for (x0 = 0; x0 < Width; x0 += WW/2)
		{
			x1 = x0;  y1 = y0;
			x2 = x1 + WW - 1;  if (x2 >= Width)   x2 = Width - 1;
			y2 = y1 + WW - 1;  if (y2 >= Height)  y2 = Height - 1;
			for (x = 0; x < 256; x++)
				P[x] = 0;
			for (y = y1; y <= y2; y++)  // Histogram
			{
				nPos = y*Width + x1;
				for (x = x1; x <= x2; x++)
				{
					P[ buf[nPos] ]++;  nPos++;
				}
			}
			T[ (y1+(y2-y1)/4)*Width + x1+(x2-x1)/4 ] = Threshold(P, &A);
		}
	}

	long T1, T1x, T1y, T2, T2x, T2y;
	for (y0 = 0; y0 < Height; y0++)  // Pixel Threshold Interpolation
	{
		for (x0 = 0; x0 < Width; x0++)
		{
			nPos = y0*Width + x0;
			if (T[nPos] != -1)
			{
				buf[nPos] = T[nPos];  continue;
			}
			// Find T1
			x1 = x0 - 1;  if (x1 < 0)  x1 = 0;
			x2 = x0 - 1;
			y1 = y0;  y2 = y0;
			T1 = -1;
			while (true)
			{
				for (y = y1; y <= y2; y++)
				{
					if (y == y0)  x2 = x0 - 1;
					for (x = x1; x <= x2; x++)
					{
						if (T[ y*Width+x ] != -1)
						{
							T1 = T[ y*Width+x ];  T1x = x;  T1y = y;
							break;
						}
					}
					if (T1 != -1)
						break;
				}
				y1 -= WW / 2;  if (y1 < 0)  break;
				x1 -= WW / 2;  if (x1 < 0)  x1 = 0;
				x2 = x0 + (x0 - x1);  if (x2 >= Width)  x2 = Width - 1;
			}
			// Find T2
			x1 = x0 + 1;  if (x1 >= Width)  x1 = Width - 1;
			x2 = x0 + 1;
			y1 = y0;  y2 = y0;
			T2 = -1;
			while (true)
			{
				for (y = y2; y >= y1; y--)
				{
					if (y == y0)  x1 = x0 + 1;
					for (x = x1; x <= x2; x++)
					{
						if (T[ y*Width+x ] != -1)
						{
							T2 = T[ y*Width+x ];  T2x = x;  T2y = y;
							break;
						}
					}
					if (T2 != -1)
						break;
				}
				y2 += WW / 2;  if (y2 >= Height)  break;
				x2 += WW / 2;  if (x2 >= Width)  x2 = Width - 1;
				x1 = x0 - (x2 - x0);  if (x1 < 0)  x1 = 0;
			}
			if (T1 == -1 && T2 == -1)
				buf[nPos] = 128;
			else if (T1 == -1)
				buf[nPos] = T2;
			else if (T2 == -1)
				buf[nPos] = T1;
			else  // Interpolation
			{
			}
		}
	}

	delete []buf;  delete []T;
	return 0;
}

void CSGCallBack::AutoBalance(long *P)
{
	long t;
	double totalSum, p[256];
	totalSum = 0;
	for (t = 0; t < 256; t++)
		totalSum += P[t];
	p[0] = P[0] / totalSum;
	for (t = 1; t < 256; t++)  // 累积直方图
		p[t] = p[t-1] + P[t]/totalSum;
	for (t = 0; t < 256; t++)  // 映射
		P[t] = (long)(255*p[t] + 0.5);
}

BYTE CSGCallBack::Median(BYTE *v, long pNo)
{
	short x, g, min=255, max=0;
	BYTE p[256];

	memset(p, 0, 256);
	for (x = 0; x < pNo; x++)
	{
		g = v[x];  p[g]++;
		if (g > max)  max = g;
		if (g < min)  min = g;
	}
	g = 0;
	for (x = min; x <= max; x++)
	{
		g += p[x];
		if (g >= pNo/2)  return (BYTE)x;
	}
	return max;
}

#if !defined(AFX_SGCALLBACK_H__33E0E325_2EB2_4647_8FFC_EF88BEFDF216__INCLUDED_)
#define AFX_SGCALLBACK_H__33E0E325_2EB2_4647_8FFC_EF88BEFDF216__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CSGCallBack.h : header file
//

#include "UDScapture.h"
#include "Camera_DirectX.h"
/////////////////////////////////////////////////////////////////////////////
// CSGCallBack class

class CSGCallBack : public ISampleGrabberCB//  provides callback methods for the ISampleGrabber::SetCallback method.
{
public:
	long m_nWidth, m_nHeight;
	long m_nPixels, m_nFrames, m_nBackFrames;
	CString m_strPath;
	CUDSCapture *m_pCapture;
	BYTE *m_pTmpBuffer, *m_pBaseBuffer, *m_pBuf1, *m_pBuf2;
	long m_lTmpBufferSize;
	long m_nAngle, m_nDistance, *m_pHoughBuf;
	long m_nWidthA, m_nWidthR1, m_nWidthR2;
	long m_nHeightA, m_nHeightR1, m_nHeightR2;
	long m_nPreviewWidth, m_nPreviewHeight, m_nPreviewBytes;
	double m_nX0, m_nY0;

	int m_nTempFileCount;
	CCamera_DirectX m_Camera_DirectX;
	BYTE *pDIB;
public:
	CSGCallBack();
	~CSGCallBack();
	STDMETHODIMP_(ULONG) AddRef();
	STDMETHODIMP_(ULONG) Release();
	STDMETHODIMP QueryInterface(REFIID riid, void ** ppv);
	STDMETHODIMP SampleCB( double SampleTime, IMediaSample * pSample );
	STDMETHODIMP BufferCB( double dblSampleTime, BYTE * pBuffer, long lBufferSize );  // STDMETHODIMP=HRESULT   __stdcall 
	BOOL IsStableDocument(BYTE * pBuffer, long lBufferSize );   //Y
	BOOL AutoClip(BYTE * pBuffer, long lBufferSize );
	BOOL SaveImage(BYTE * pBuffer, long lBufferSize );    //Y
	BYTE Median(BYTE *v, long pNo);
	long Otsu(long *P, long *bgAverage);       //// Otsu Threshold，保存图像分隔增强用到
	long Threshold(long *P, long *bgAverage);  /// Adaptive Threshold
	long DynamicThreshold(BYTE * pBuffer, long Width, long Height, long WidthBytes);
	void AutoBalance(long *P);
	void ReleaseBuffer();

	/**
	*  @brief  命名文件
	*  @param[in]  szExtName 文件扩展名 
	*  @return 文件名
	*/
	CString NameFile(const TCHAR* szExtName);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SGCALLBACK_H__33E0E325_2EB2_4647_8FFC_EF88BEFDF216__INCLUDED_)

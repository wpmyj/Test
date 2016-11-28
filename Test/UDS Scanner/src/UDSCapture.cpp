// UDSCapture.cpp : 实现文件
//

#include "stdafx.h"
#include <atlbase.h>      
#include "UDSCapture.h"
#include "SGCallBack.h"

// CUDSCapture
CSGCallBack mCB;

IMPLEMENT_DYNAMIC(CUDSCapture, CWnd)

CUDSCapture::CUDSCapture()
{
	m_pGB = NULL; 
	m_pMC = NULL;  
	m_pVW = NULL;  
	m_pCapture = NULL;  
	m_pCameraFilter = NULL;  
	m_pGrabberFilter = NULL;
	m_pGrabber = NULL;
	if ( FAILED(CoInitialize(NULL)) )
		AfxMessageBox("Failed to CoInitialize"); 
	m_pEvent = NULL;

	m_hDlgWnd = m_hPreviewWnd = NULL;
	m_bActive = false;
	m_bIsPreview = true;
	m_bCaptureBackground = false;
	m_bIsAutoClipping = false;

	m_Auto.grayT = 32;                         // 最大灰度差
	m_Auto.colorT = 32;                        // 最大色彩差
	m_Auto.moveRatio = 10;                     // 最小移动比例
	m_Auto.edgeWidth = 1;                      // 边缘宽度，用来检查手
	m_Auto.edgePixel = 30;                     // 最大边缘像素
	m_Auto.stayPixel = 400;                    // 最大不稳定像素
	m_Auto.stayFrame = 1;                      // 最小稳定帧数
	m_Auto.imageType = 0;                      // 0-彩色  1-灰度  2-自动
	m_Auto.imageOrientation = 0;               // 调整图像方向: 0-0, 1-90, 2-180, 3-270
	m_Auto.fastPreview = false;                // 是否采用快速预览
	m_Auto.autoClip = true;                    // 是否智能裁切旋转拍摄
	m_Auto.autoRotate = false;                 // 是否自动顺时针转90度
	m_Auto.autoEnhance = false;                // 是否进行文档增强
	m_Auto.docWidth = m_Auto.docHeight = -1;   // 指定手动拍摄时取视场的百分比
	m_Auto.strCamrea.Empty();                  // 默认摄像机
	m_Auto.strSize.Empty();                    // 默认图像帧尺寸

	m_strMinSize.Empty();  m_strMaxSize.Empty();
	m_strBarcodeType = "39/128", m_strBarcodeFormat.Empty();
	m_nBarcodeLength = 0;
	m_bMultiBarcode = false;  m_bBarcodeRotate = false;
	m_strImagePath.Empty();  m_strBarcode.Empty();
	m_nQuality = 40;  m_nPhotoNo = 0;

}

CUDSCapture::~CUDSCapture()
{
}


BEGIN_MESSAGE_MAP(CUDSCapture, CWnd)
END_MESSAGE_MAP()

bool CUDSCapture::CreateComInstance()
{
	 HRESULT hr;

	 /**   CoCreateInstance函数原型
	   STDAPI CoCreateInstance(
		 REFCLSID rclsid,           //创建的Com对象的类标识符(CLSID)
		 LPUNKNOWN pUnkOuter,       //指向接口IUnknown的指针
		 DWORD dwClsContext,        //运行可执行代码的上下文
		 REFIID riid,               //创建的Com对象的接口标识符
		 LPVOID * ppv               //用来接收指向Com对象接口地址的指针变量
		 );
		 */

	 // Create CaptureGraphBuilder2 Instance 
	 hr = CoCreateInstance (CLSID_CaptureGraphBuilder2, NULL, CLSCTX_INPROC, IID_ICaptureGraphBuilder2, (void **)&m_pCapture);
	 if ( FAILED(hr) ) 
	 { 
		  ReleaseAll(); 
			AfxMessageBox("Failed to Create IID_ICaptureGraphBuilder2");
	    return(false);  
	 };

	 // Create the FilterGraph->GraphBuilder Instance
	 hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void **)&m_pGB);
	 if ( FAILED(hr) ) 
	 { 
		 ReleaseAll(); 
		 AfxMessageBox("Failed to Create IID_IGraphBuilder");
		 return(false);  
	 };
	 
	 m_pCapture->SetFiltergraph(m_pGB);

	 //查询m_pMC组件是否支持IID_IMediaControl接口
	 hr = m_pGB->QueryInterface(IID_IMediaControl, (void **)&m_pMC);
	 if ( FAILED(hr) ) 
	 { 
		 ReleaseAll(); 
		 AfxMessageBox("Failed to Create IID_IMediaControl");
		 return(false);  
	 };
	
	 //查询m_pVW组件是否支持IID_IVideoWindow接口
	 hr = m_pGB->QueryInterface(IID_IVideoWindow, (LPVOID *) &m_pVW);
	 if ( FAILED(hr) ) 
	 { 
		 ReleaseAll(); 
		 AfxMessageBox("Failed to Create IID_IVideoWindow");
		 return(false);  
	 };

	 // Create Sample Grabber Instance
	 hr = CoCreateInstance(CLSID_SampleGrabber, NULL, CLSCTX_INPROC_SERVER, IID_IBaseFilter, (void **)&m_pGrabberFilter);
	 if ( FAILED(hr) ) 
	 { 
		 ReleaseAll(); 
		 AfxMessageBox("Failed to Create IID_IBaseFilter");
		 return(false);  
	 };

	 //查询m_pGrabber组件是否支持IID_ISampleGrabber接口
	 hr = m_pGrabberFilter->QueryInterface(IID_ISampleGrabber, (void **)&m_pGrabber);
	 if ( FAILED(hr) ) 
	 { 
		 ReleaseAll(); 
		 AfxMessageBox("Failed to Create IID_ISampleGrabber");
		 return(false);  
	 };

	 AM_MEDIA_TYPE mt;
	 ZeroMemory(&mt, sizeof(mt));
	 mt.majortype = MEDIATYPE_Video;
	 mt.subtype = MEDIASUBTYPE_RGB24;
	 hr = m_pGrabber->SetMediaType(&mt);
	 if ( FAILED(hr) ) 
	 { 
		 ReleaseAll(); 
		 AfxMessageBox("Failed to Set Media Type");
		 return(false);  
	 };

	 return(true);
}

template <class T> void SafeRelease(T *ppT)  // 函数模板
{
	if ( ppT )
	{
		ppT->Release();  ppT = NULL;
	}
}

void CUDSCapture::ReleaseAll()
{
	SafeRelease(m_pVW);
	SafeRelease(m_pMC);
	SafeRelease(m_pGB);
	SafeRelease(m_pCapture);
	SafeRelease(m_pGrabberFilter);
	SafeRelease(m_pGrabber);
}

long CUDSCapture::GetDevicesList(CComboBox *pCamList)
{
	if ( !pCamList )  return -1;

	//创建一个系统枚举组件（CLSID_SystemDeviceEnum），并获得ICreateDevEnum接口
	HRESULT hr;
	ICreateDevEnum *pCreateDevEnum;
	hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER, IID_ICreateDevEnum, (void**)&pCreateDevEnum);
	if (hr != NOERROR) 
	{
		AfxMessageBox("Failed to IID_ICreateDevEnum");
		return -1;
	}

	//使用接口方法ICreateDevEnum::CreateClassEnumerator为指定目录创建一个枚举器，
	//即IEnumMonker对象，同时获得IEnumMoniker接口
	CComPtr <IEnumMoniker> pEm;
	hr = pCreateDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory, &pEm, 0);
	if (hr != NOERROR)  return -1;

	pEm->Reset();
	ULONG cFetched;
	CString devName;
	IMoniker *pM;

	//枚举指定类型目录下所有的设备标识（DeviceMoniker）,每个设备标识对象上都实现了IMoniker接口
	while (hr = pEm->Next(1, &pM, &cFetched), hr == S_OK)  
	{
		IPropertyBag *pBag;
		hr = pM->BindToStorage(0, 0, IID_IPropertyBag, (void **)&pBag);
		if ( SUCCEEDED(hr) ) 
		{
			VARIANT var;
			var.vt = VT_BSTR;
			hr = pBag->Read(L"FriendlyName", &var, NULL);
			if (hr == NOERROR) 
			{
				TCHAR str[2048]; 
				WideCharToMultiByte(CP_ACP, 0, var.bstrVal, -1, str, 2048, NULL, NULL);
				devName.Format("%s", str);
				if (pCamList->FindString(0, devName) >= 0)
					devName += "_副";
				pCamList->AddString(devName);
				SysFreeString(var.bstrVal);
			}
			
			pBag->Release();
		}
		pM->Release();
	}
	int devSel = pCamList->FindStringExact(-1, m_Auto.strCamrea);
	if (devSel < 0)  devSel = 0;
	pCamList->SetCurSel(devSel);

	return pCamList->GetCount();
}

bool CUDSCapture::SetupCamera()
{
	HRESULT hr;

	IAMStreamConfig *pSConfig = NULL;
	hr = m_pCapture->FindInterface(&PIN_CATEGORY_PREVIEW, 0, m_pCameraFilter, IID_IAMStreamConfig, (void**)&pSConfig);
	if ( FAILED(hr) )
		hr = m_pCapture->FindInterface(&PIN_CATEGORY_CAPTURE, 0, m_pCameraFilter, IID_IAMStreamConfig, (void**)&pSConfig);
	if ( FAILED(hr) )  return(false);
	//AfxMessageBox("Succeed to FindInterface");

	CString msg, strSize;
	int iCount=0, iSize=0;
	hr = pSConfig->GetNumberOfCapabilities(&iCount, &iSize);
	msg.Format("iCount=%d, iSize=%d", iCount, iSize);
	//AfxMessageBox(msg);
	// Check the size to make sure we pass in the correct structure.
	if (iSize != sizeof(VIDEO_STREAM_CONFIG_CAPS))  {  pSConfig->Release();  return(false);  }

	VIDEOINFOHEADER *pVih;
	LONG minWidth=10000, minHeight=10000;
	LONG maxWidth=0, maxHeight=0;
	AM_MEDIA_TYPE *pMT;
	VIDEO_STREAM_CONFIG_CAPS scc;
	// Use the video capabilities structure.
	for (int iFormat = 0; iFormat < iCount; iFormat++)
	{
		hr = pSConfig->GetStreamCaps(iFormat, &pMT, (BYTE*)&scc);
		if (SUCCEEDED(hr))
		{
			/* Examine the format, and possibly use it. */
			if ((pMT->majortype == MEDIATYPE_Video) &&
				//(pMT->subtype == MEDIASUBTYPE_RGB24) &&
				(pMT->formattype == FORMAT_VideoInfo) &&
				(pMT->cbFormat >= sizeof (VIDEOINFOHEADER)) &&
				(pMT->pbFormat != NULL))
			{
				pVih = (VIDEOINFOHEADER*)pMT->pbFormat;
				msg.Format("Size=[%d, %d], Time=[%d, %d]",
					pVih->bmiHeader.biWidth, pVih->bmiHeader.biHeight,
					scc.MinFrameInterval, scc.MaxFrameInterval);
				//AfxMessageBox(msg);
				// pVih contains the detailed format information.
				if (pVih->bmiHeader.biWidth > 0 && pVih->bmiHeader.biHeight > 0)
				{
					strSize.Format("%3d万: %d×%d", pVih->bmiHeader.biWidth*pVih->bmiHeader.biHeight/10000,
													pVih->bmiHeader.biWidth, pVih->bmiHeader.biHeight);
					pVih->bmiHeader.biSizeImage = DIBSIZE(pVih->bmiHeader);
					if ( m_bIsPreview && m_Auto.fastPreview )  // 快速预览时改用最小图像尺寸，否则用指定的图像尺寸
					{
						if (strSize == m_strMinSize)
						{
							m_Auto.edgePixel = 5;
							m_Auto.stayPixel = 50;
							hr = pSConfig->SetFormat(pMT);
						}
					}
					else  // 用指定的图像尺寸
					{
						if (strSize == m_Auto.strSize)
						{
							m_Auto.edgePixel = 30;
							m_Auto.stayPixel = 400;
							hr = pSConfig->SetFormat(pMT);
						}
					}
				}
			}
			// Delete the media type when you are done.
			if (pMT->cbFormat != 0) 
			{
				CoTaskMemFree((PVOID)pMT->pbFormat);  pMT->pbFormat = NULL;
			}
			if (pMT->pUnk != NULL) 
			{
				pMT->pUnk->Release();  pMT->pUnk = NULL;
			}
			CoTaskMemFree((PVOID)pMT);
		}
	}
	pSConfig->Release();
	msg.Format("min=[%d, %d], max=[%d, %d]", minWidth, minHeight, maxWidth, maxHeight);
	//AfxMessageBox(msg);

	hr = m_pGB->AddFilter(m_pGrabberFilter, L"UDS Grabber Filter");
	if ( FAILED(hr) )  {  ReleaseAll();  return(false);  };
	//AfxMessageBox("Succeed to AddFilter");

	// try to render preview/capture pin
	//CComQIPtr< IBaseFilter, &IID_IBaseFilter > GrabberFilter(m_Grabber);
	hr = m_pCapture->RenderStream(&PIN_CATEGORY_PREVIEW, &MEDIATYPE_Video, m_pCameraFilter, m_pGrabberFilter, NULL);
	if( FAILED(hr) )
		hr = m_pCapture->RenderStream(&PIN_CATEGORY_CAPTURE, &MEDIATYPE_Video, m_pCameraFilter, m_pGrabberFilter, NULL);
	/*
	switch (hr)
	{
	case S_OK:  AfxMessageBox("S_OK");  break;
	case VFW_S_NOPREVIEWPIN:  AfxMessageBox("VFW_S_NOPREVIEWPIN");  break;
	case E_FAIL:  AfxMessageBox("E_FAIL");  break;
	case E_INVALIDARG:  AfxMessageBox("E_INVALIDARG");  break;
	case E_POINTER:  AfxMessageBox("E_POINTER");  break;
	case VFW_E_NOT_IN_GRAPH:  AfxMessageBox("VFW_E_NOT_IN_GRAPH");  break;
	default:  AfxMessageBox("Unkown Error");  break;
	}
	*/
	if ( FAILED(hr) )  return(false);
	//AfxMessageBox("Succeed to RenderStream");

	AM_MEDIA_TYPE mt; 
	ZeroMemory(&mt, sizeof(AM_MEDIA_TYPE));
	hr = m_pGrabber->GetConnectedMediaType( &mt );
	if ( FAILED(hr) )  return(false);
	//AfxMessageBox("Succeed to GetConnectedMediaType");

	pVih = (VIDEOINFOHEADER*)mt.pbFormat;
	mCB.m_nWidth = pVih->bmiHeader.biWidth;
	mCB.m_nHeight = pVih->bmiHeader.biHeight;
	if (mt.cbFormat != 0) 
	{
		CoTaskMemFree((PVOID)mt.pbFormat);
		mt.cbFormat = 0;  mt.pbFormat = NULL;
	}
	if (mt.pUnk != NULL) 
	{
		mt.pUnk->Release();  mt.pUnk = NULL;
	}
	msg.Format("ImageSize=[%d, %d]", mCB.m_nWidth, mCB.m_nHeight);
	//AfxMessageBox(msg);

	hr = m_pGrabber->SetBufferSamples( FALSE );

/**

ISampleGrabber::SetCallback(ISampleGrabberCB *pCallback, int WhichMethodToCallback )

param[in] WhichMethodToCallback:
0:	The Sample Grabber filter calls the ISampleGrabberCB::SampleCB method.
    This method receives an IMediaSample pointer.
1:	The Sample Grabber filter calls the ISampleGrabberCB::BufferCB method. 
    This method receives a pointer to the buffer that is contained in the media sample.
*/

	hr = m_pGrabber->SetCallback( &mCB, 1 );
	hr = m_pGrabber->SetOneShot( FALSE );
	if ( FAILED(hr) )  return(false);
	//AfxMessageBox("Succeed to Set m_pGrabber");

	hr = m_pVW->put_Owner((OAHWND)m_hPreviewWnd);
	hr = m_pVW->put_WindowStyle(WS_CHILD |WS_CLIPSIBLINGS);// WS_CLIPCHILDREN);
	CRect rc;
	::GetClientRect(m_hPreviewWnd, &rc);
	m_pVW->SetWindowPosition(0, 0, rc.right, rc.bottom);
	hr = m_pVW->put_Visible(OATRUE);
	if ( FAILED(hr) )  return(false);
	//AfxMessageBox("Succeed to Set m_pVW");

	mCB.m_pCapture = this;
	hr = m_pGB->QueryInterface(IID_IMediaEventEx, (void **)&m_pEvent);
	if ( FAILED(hr) )  {  ReleaseAll();  return(false);  };
	//AfxMessageBox("Succeed to Create IID_IMediaEventEx");

	hr = m_pMC->Run();  // Start to capture video
	if ( FAILED(hr) )  return(false);
	//AfxMessageBox("Succeed to Run");

	return(true);
}

void CUDSCapture::StopCamera()
{
	if ( m_pMC )  // Stop Capture
		m_pMC->Stop();

	mCB.m_pCapture = NULL;
	if ( m_pEvent )  // Stop Event and Release m_pEvent
	{
		m_pEvent->SetNotifyWindow(NULL, 0, 0);
		m_pEvent->Release();
		m_pEvent = NULL;
	}

	if ( m_pCameraFilter )  // Remove & Release m_pCameraFilter
	{
		if ( FAILED(m_pGB->RemoveFilter(m_pCameraFilter)) )
			AfxMessageBox("Failed to Remove m_pCameraFilter");
		m_pCameraFilter->Release();  m_pCameraFilter = NULL;
	}
	if ( m_pGrabberFilter )  // Remove m_pCameraFilter
	{
		if ( FAILED(m_pGB->RemoveFilter(m_pGrabberFilter)) )
			AfxMessageBox("Failed to Remove m_pGrabberFilter");
	}
}


bool CUDSCapture::SelectCamera(int devNo, CComboBox *pSizeList)
{
	if (devNo < 0 || !pSizeList)  return(false);
	for (int i = pSizeList->GetCount()-1; i >= 0; i--)
		pSizeList->DeleteString(i);
	StopCamera();  // Stop the current camera

	// enumerate all video capture devices
	HRESULT hr;
	CComPtr <ICreateDevEnum> pCreateDevEnum;
	hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER, IID_ICreateDevEnum, (void**)&pCreateDevEnum);
	if (hr != NOERROR)  return(false);
	//AfxMessageBox("Succeed to IID_ICreateDevEnum");

	CComPtr <IEnumMoniker> pEm;
	hr = pCreateDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory, &pEm, 0);
	if (hr != NOERROR)  {  pCreateDevEnum.Release();  return(false);  }
	pEm->Reset();
	//AfxMessageBox("Succeed to CreateClassEnumerator");

	ULONG cFetched;
	IMoniker *pM;
	int index=0;
	m_pCameraFilter = NULL;
	while (hr = pEm->Next(1, &pM, &cFetched), hr == S_OK, index <= devNo)
	{
		IPropertyBag *pBag;
		hr = pM->BindToStorage(0, 0, IID_IPropertyBag, (void **)&pBag);
		if ( SUCCEEDED(hr) ) 
		{
			VARIANT var;
			var.vt = VT_BSTR;
			hr = pBag->Read(L"FriendlyName", &var, NULL);
			if (hr == NOERROR) 
			{
				// Check whether camera is authorized
				TCHAR str[2048]; 
				WideCharToMultiByte(CP_ACP, 0, var.bstrVal, -1, str, 2048, NULL, NULL);
				CString devName;
				devName.Format("%s", str);
				//devName.Format("%s: [%d-%d]", str, devNo, index);
				//AfxMessageBox(devName);
				SysFreeString(var.bstrVal);
				if (index == devNo)
				{
					pM->BindToObject(0, 0, IID_IBaseFilter, (void **)&m_pCameraFilter);
					pBag->Release();
					pM->Release();
					break;
				}
				index++;
			}
			pBag->Release();
		}
		pM->Release();
	}
	pEm.Release();
	pCreateDevEnum.Release();
	if ( m_pCameraFilter == NULL )  return(false);
	//AfxMessageBox("Succeed to BindToObject");

	hr = m_pGB->AddFilter(m_pCameraFilter, L"UDS Camera Filter");
	if ( FAILED(hr) )  return(false);
	//AfxMessageBox("Succeed to AddFilter");

	///////////////////////////////////////////////////////////////////////////////////////////////
	// Enumerate all supported image size
	IAMStreamConfig *pSConfig = NULL;
	hr = m_pCapture->FindInterface(&PIN_CATEGORY_PREVIEW, 0, m_pCameraFilter, IID_IAMStreamConfig, (void**)&pSConfig);
	if ( FAILED(hr) )
		hr = m_pCapture->FindInterface(&PIN_CATEGORY_CAPTURE, 0, m_pCameraFilter, IID_IAMStreamConfig, (void**)&pSConfig);
	if ( FAILED(hr) )  return(false);
	//AfxMessageBox("Succeed to FindInterface");

	CString msg, strSize;
	int iCount=0, iSize=0;
	hr = pSConfig->GetNumberOfCapabilities(&iCount, &iSize);
	msg.Format("iCount=%d, iSize=%d", iCount, iSize);
	//AfxMessageBox(msg);
	// Check the size to make sure we pass in the correct structure.
	if (iSize != sizeof(VIDEO_STREAM_CONFIG_CAPS))  {  pSConfig->Release();  return(false);  }

	VIDEOINFOHEADER *pVih;
	LONG minWidth=10000, minHeight=10000;
	LONG maxWidth=0, maxHeight=0;
	AM_MEDIA_TYPE *pMT;
	VIDEO_STREAM_CONFIG_CAPS scc;
	// Use the video capabilities structure.
	for (int iFormat = 0; iFormat < iCount; iFormat++)
	{
		hr = pSConfig->GetStreamCaps(iFormat, &pMT, (BYTE*)&scc);
		if ( SUCCEEDED(hr) )
		{
			/* Examine the format, and possibly use it. */
			if ((pMT->majortype == MEDIATYPE_Video) &&
				//(pMT->subtype == MEDIASUBTYPE_RGB24) &&
				(pMT->formattype == FORMAT_VideoInfo) &&
				(pMT->cbFormat >= sizeof(VIDEOINFOHEADER)) &&
				(pMT->pbFormat != NULL))
			{
				pVih = (VIDEOINFOHEADER*)pMT->pbFormat;
				msg.Format("Size=[%d, %d], Time=[%d, %d]",
					pVih->bmiHeader.biWidth, pVih->bmiHeader.biHeight,
					scc.MinFrameInterval, scc.MaxFrameInterval);
				//AfxMessageBox(msg);
				if (pVih->bmiHeader.biWidth > 0 && pVih->bmiHeader.biHeight > 0)
				{
					strSize.Format("%3d万: %d×%d", pVih->bmiHeader.biWidth*pVih->bmiHeader.biHeight/10000,
						pVih->bmiHeader.biWidth, pVih->bmiHeader.biHeight);
					//::SendMessage(hList, CB_ADDSTRING, 0, (LPARAM)strSize.GetBuffer(0));
					pSizeList->AddString(strSize);
					if (pVih->bmiHeader.biWidth < minWidth && pVih->bmiHeader.biHeight < minHeight)
					{
						minWidth = pVih->bmiHeader.biWidth;
						minHeight = pVih->bmiHeader.biHeight;
					}
					if (pVih->bmiHeader.biWidth > maxWidth && pVih->bmiHeader.biHeight > maxHeight)
					{
						maxWidth = pVih->bmiHeader.biWidth;
						maxHeight = pVih->bmiHeader.biHeight;
					}
				}
			}
			// Delete the media type when you are done.
			if (pMT->cbFormat != 0) 
			{
				CoTaskMemFree((PVOID)pMT->pbFormat);  pMT->pbFormat = NULL;
			}
			if (pMT->pUnk != NULL) 
			{
				pMT->pUnk->Release();  pMT->pUnk = NULL;
			}
			CoTaskMemFree((PVOID)pMT);
		}
	}
	pSConfig->Release();
	msg.Format("min=[%d, %d], max=[%d, %d]", minWidth, minHeight, maxWidth, maxHeight);
	//AfxMessageBox(msg);
	m_strMinSize.Format("%3d万: %d×%d", minWidth*minHeight/10000, minWidth, minHeight);
	m_strMaxSize.Format("%3d万: %d×%d", maxWidth*maxHeight/10000, maxWidth, maxHeight);
	int sizeSel = pSizeList->FindStringExact(-1, m_Auto.strSize);
	if (sizeSel < 0)  sizeSel = 0;
	pSizeList->SetCurSel(sizeSel);

	return(true);

}

bool CUDSCapture::GetExposure(CSliderCtrl *pSlider)
{
	if (m_pCameraFilter == NULL)
		return(false);

	// Query the capture filter for the IAMCameraControl interface.
	IAMCameraControl *m_pCameraControl;
    HRESULT	hr = m_pCameraFilter->QueryInterface(IID_IAMCameraControl, (void**)&m_pCameraControl);
	if ( FAILED(hr) )
		return(false);

	long Min, Max, Step, Default, Flags;
	// Get the range and default value.
	hr = m_pCameraControl->GetRange(CameraControl_Exposure, &Min, &Max, &Step, &Default, &Flags);
	if ( SUCCEEDED(hr) )
	{
	
		pSlider->EnableWindow(TRUE);
		pSlider->SetRange(Min, Max, TRUE);

		// Set Exposure to Manual
		if (m_nExposure < Min || m_nExposure > Max)  // Get the current value
			hr = m_pCameraControl->Get(CameraControl_Exposure, &m_nExposure, &Flags);
		hr = m_pCameraControl->Set(CameraControl_Exposure, m_nExposure, CameraControl_Flags_Manual);
		m_pCameraControl->Release();
		if ( SUCCEEDED(hr) )
			pSlider->SetPos(m_nExposure);
		return(true);
	}
	else
	{
		m_pCameraControl->Release();
		pSlider->EnableWindow(FALSE);
		return(false);
	}
}

bool CUDSCapture::SetExposure(CSliderCtrl *pSlider)
{
	if (m_pCameraFilter == NULL)
		return(false);

	// Query the capture filter for the IAMCameraControl interface.
	IAMCameraControl *m_pCameraControl;
	HRESULT	hr = m_pCameraFilter->QueryInterface(IID_IAMCameraControl, (void**)&m_pCameraControl);
	if ( FAILED(hr) )
		return(false);

	m_nExposure = pSlider->GetPos();
	hr = m_pCameraControl->Set(CameraControl_Exposure, m_nExposure, CameraControl_Flags_Manual);
	m_pCameraControl->Release();
	if ( SUCCEEDED(hr) )
		return(true);
	else
		return(false);
}

bool CUDSCapture::GetBrightness(CSliderCtrl *pSlider)
{
	if (m_pCameraFilter == NULL)
		return(false);

	// Query the capture filter for the IAMVideoProcAmp interface.
	IAMVideoProcAmp *m_pProcAmp;
    HRESULT	hr = m_pCameraFilter->QueryInterface(IID_IAMVideoProcAmp, (void**)&m_pProcAmp);
	if ( FAILED(hr) )
		return(false);

	long Min, Max, Step, Default, Flags;
	// Get the range and default value.
	hr = m_pProcAmp->GetRange(VideoProcAmp_Brightness, &Min, &Max, &Step, &Default, &Flags);
	if ( SUCCEEDED(hr) )
	{
	
		pSlider->EnableWindow(TRUE);
		pSlider->SetRange(Min, Max, TRUE);
		// Set Brightness to Manual
		if (m_nBrightness < Min || m_nBrightness > Max)  // Get the current value
			hr = m_pProcAmp->Get(VideoProcAmp_Brightness, &m_nBrightness, &Flags);
		hr = m_pProcAmp->Set(VideoProcAmp_Brightness, m_nBrightness, VideoProcAmp_Flags_Manual);
		if ( SUCCEEDED(hr) )
			pSlider->SetPos(m_nBrightness);
/*
		// Set WhiteBalance, Sharpness, Gamma, BacklightCompensation, Contrast, Hue, Saturation to Manual
		hr = m_pProcAmp->GetRange(VideoProcAmp_WhiteBalance, &Min, &Max, &Step, &Default, &Flags);
		if ( !SUCCEEDED(hr) )  Default = 0;
		hr = m_pProcAmp->Set(VideoProcAmp_WhiteBalance, Default, VideoProcAmp_Flags_Manual);  // WhiteBalance

		hr = m_pProcAmp->GetRange(VideoProcAmp_Sharpness, &Min, &Max, &Step, &Default, &Flags);
		if ( !SUCCEEDED(hr) )  Default = 0;
		hr = m_pProcAmp->Set(VideoProcAmp_Sharpness, Default, VideoProcAmp_Flags_Manual);  // Sharpness

		hr = m_pProcAmp->GetRange(VideoProcAmp_Gamma, &Min, &Max, &Step, &Default, &Flags);
		if ( !SUCCEEDED(hr) )  Default = 0;
		hr = m_pProcAmp->Set(VideoProcAmp_Gamma, Default, VideoProcAmp_Flags_Manual);  // Gamma

		hr = m_pProcAmp->GetRange(VideoProcAmp_BacklightCompensation, &Min, &Max, &Step, &Default, &Flags);
		if ( !SUCCEEDED(hr) )  Default = 0;
		hr = m_pProcAmp->Set(VideoProcAmp_BacklightCompensation, Default, VideoProcAmp_Flags_Manual);  // BacklightCompensation

		hr = m_pProcAmp->GetRange(VideoProcAmp_Contrast, &Min, &Max, &Step, &Default, &Flags);
		if ( !SUCCEEDED(hr) )  Default = 0;
		hr = m_pProcAmp->Set(VideoProcAmp_Contrast, Default, VideoProcAmp_Flags_Manual);  // Contrast

		hr = m_pProcAmp->GetRange(VideoProcAmp_Hue, &Min, &Max, &Step, &Default, &Flags);
		if ( !SUCCEEDED(hr) )  Default = 0;
		hr = m_pProcAmp->Set(VideoProcAmp_Hue, Default, VideoProcAmp_Flags_Manual);  // Hue

		hr = m_pProcAmp->GetRange(VideoProcAmp_Saturation, &Min, &Max, &Step, &Default, &Flags);
		if ( !SUCCEEDED(hr) )  Default = 0;
		hr = m_pProcAmp->Set(VideoProcAmp_Saturation, Default, VideoProcAmp_Flags_Manual);  // Saturation
*/
		m_pProcAmp->Release();
		return(true);
	}
	else
	{
		m_pProcAmp->Release();
		pSlider->EnableWindow(FALSE);
		return(false);
	}
}

bool CUDSCapture::SetBrightness(CSliderCtrl *pSlider)
{
	if (m_pCameraFilter == NULL)
		return(false);

	// Query the capture filter for the IAMVideoProcAmp interface.
	IAMVideoProcAmp *m_pProcAmp;
	HRESULT	hr = m_pCameraFilter->QueryInterface(IID_IAMVideoProcAmp, (void**)&m_pProcAmp);
	if ( FAILED(hr) )
		return(false);

	// Set Brightness
	m_nBrightness = pSlider->GetPos();
	hr = m_pProcAmp->Set(VideoProcAmp_Brightness, m_nBrightness, VideoProcAmp_Flags_Manual);
	m_pProcAmp->Release();
	if ( SUCCEEDED(hr) )
		return(true);
	else
		return(false);
}

bool CUDSCapture::ConfigCameraFilter(HWND hParentWnd)
{
	if (m_pCameraFilter == NULL)
		return(false);

	HRESULT hr=0 ;
	ISpecifyPropertyPages *pProp ;
	hr = m_pCameraFilter->QueryInterface(IID_ISpecifyPropertyPages, (void**)&pProp);
	if ( SUCCEEDED(hr) )
	{
		//获取滤波器名称和IUnknown接口指针
		FILTER_INFO FilterInfo ;
		hr = m_pCameraFilter->QueryFilterInfo(&FilterInfo);
		IUnknown *pFilterUnk ;
		m_pCameraFilter->QueryInterface(IID_IUnknown, (void**)&pFilterUnk);
		//显示该页
		CAUUID caGUID ;
		pProp->GetPages(&caGUID);
		OleCreatePropertyFrame(hParentWnd, 0, 0, FilterInfo.achName, 1, &pFilterUnk,
			caGUID.cElems, caGUID.pElems, 0, 0, NULL);
		//释放内存、资源
		CoTaskMemFree(caGUID.pElems);
		pFilterUnk->Release();
		FilterInfo.pGraph ->Release();
		pProp->Release();
	}
	//m_pMC->Run();
	return(true);
}

bool CUDSCapture::ConfigCameraPin(HWND hParentWnd)
{
	if (m_pCameraFilter == NULL)
		return(false);

	HRESULT hr ;
	IAMStreamConfig *pSConfig ;	//流配置接口
	ISpecifyPropertyPages *pSpec ;	//属性页接口

	m_pMC->Stop();  //只有停止后，才能进行引脚属性的设置
	//首先查询捕获CAPTURE、视频VIDEO接口
	hr = m_pCapture->FindInterface(&PIN_CATEGORY_CAPTURE, &MEDIATYPE_Video, m_pCameraFilter,
		IID_IAMStreamConfig, (void **)&pSConfig);
	hr = pSConfig->QueryInterface(IID_ISpecifyPropertyPages, (void **)&pSpec);
	if ( SUCCEEDED(hr) )
	{
		CAUUID cauuid ;		//所有属性页结构体
		hr = pSpec->GetPages(&cauuid);	//获取所有属性页
		//显示属性页
		hr = OleCreatePropertyFrame(hParentWnd, 0, 0, NULL, 1, (IUnknown **)&pSConfig,
			cauuid.cElems, (GUID*)cauuid.pElems, 0, 0, NULL);
		//释放内存、资源
		CoTaskMemFree(cauuid.pElems);
		pSpec->Release();
		pSConfig->Release();
	}
	m_pMC->Run();
	return(true);
}

bool CUDSCapture::IsAuthorizedCamera(CString devName)
{
	if ( devName.Find("紫光图文") >= 0 )
		return true;
	else
		return false;
}

bool CUDSCapture::GetContrast(CSliderCtrl *pSlider)
{
	if (m_pCameraFilter == NULL)
		return(false);
	
		// Query the capture filter for the IAMVideoProcAmp interface.
	IAMVideoProcAmp *m_pProcAmp;
    HRESULT	hr = m_pCameraFilter->QueryInterface(IID_IAMVideoProcAmp, (void**)&m_pProcAmp);
	if ( FAILED(hr) )
		return(false);

	
	long Min, Max, Step, Default, Flags;
	// Get the range and default value.
	hr = m_pProcAmp->GetRange(VideoProcAmp_Contrast, &Min, &Max, &Step, &Default, &Flags);
	
	if ( SUCCEEDED(hr) )
	{
		pSlider->EnableWindow(TRUE);
		pSlider->SetRange(Min, Max, TRUE);
		// Set Brightness to Manual
		if (m_nContrast < Min || m_nContrast > Max)  // Get the current value
			hr = m_pProcAmp->Get(VideoProcAmp_Contrast, &m_nContrast, &Flags);
		hr = m_pProcAmp->Set(VideoProcAmp_Contrast, m_nContrast, VideoProcAmp_Flags_Manual);
		if ( SUCCEEDED(hr) )
			pSlider->SetPos(m_nContrast);

		m_pProcAmp->Release();
		return(true);
	}
	else
	{
		m_pProcAmp->Release();
		pSlider->EnableWindow(FALSE);
		return(false);
	}
	

}

bool CUDSCapture::SetContrast(CSliderCtrl *pSlider)
{
	if (m_pCameraFilter == NULL)
		return(false);

	// Query the capture filter for the IAMVideoProcAmp interface.
	IAMVideoProcAmp *m_pProcAmp;
	HRESULT	hr = m_pCameraFilter->QueryInterface(IID_IAMVideoProcAmp, (void**)&m_pProcAmp);
	if ( FAILED(hr) )
		return(false);

	// Set Contrast
	m_nContrast = pSlider->GetPos();
	hr = m_pProcAmp->Set(VideoProcAmp_Contrast, m_nContrast, VideoProcAmp_Flags_Manual);
	m_pProcAmp->Release();
	if ( SUCCEEDED(hr) )
		return(true);
	else
		return(false);
}

bool CUDSCapture::GetHue(CSliderCtrl *pSlider)
{
	if (m_pCameraFilter == NULL)
		return(false);

	// Query the capture filter for the IAMVideoProcAmp interface.
	IAMVideoProcAmp *m_pProcAmp;
	HRESULT	hr = m_pCameraFilter->QueryInterface(IID_IAMVideoProcAmp, (void**)&m_pProcAmp);
	if ( FAILED(hr) )
		return(false);

	long Min, Max, Step, Default, Flags;
	// Get the range and default value.
	hr = m_pProcAmp->GetRange(VideoProcAmp_Hue, &Min, &Max, &Step, &Default, &Flags);

	if ( SUCCEEDED(hr) )
	{
		pSlider->EnableWindow(TRUE);
		pSlider->SetRange(Min, Max, TRUE);
		// Set Hue to Manual
		if (m_nHue < Min || m_nHue > Max)  // Get the current value
			hr = m_pProcAmp->Get(VideoProcAmp_Hue, &m_nHue, &Flags);
		hr = m_pProcAmp->Set(VideoProcAmp_Hue, m_nHue, VideoProcAmp_Flags_Manual);
		if ( SUCCEEDED(hr) )
			pSlider->SetPos(m_nHue);

		m_pProcAmp->Release();
		return(true);
	}
	else
	{
		m_pProcAmp->Release();
		pSlider->EnableWindow(FALSE);
		return(false);
	}

}

bool CUDSCapture::SetHue(CSliderCtrl *pSlider)
{
	if (m_pCameraFilter == NULL)
		return(false);

	// Query the capture filter for the IAMVideoProcAmp interface.
	IAMVideoProcAmp *m_pProcAmp;
	HRESULT	hr = m_pCameraFilter->QueryInterface(IID_IAMVideoProcAmp, (void**)&m_pProcAmp);
	if ( FAILED(hr) )
		return(false);

	// Set Hue
	m_nHue = pSlider->GetPos();
	hr = m_pProcAmp->Set(VideoProcAmp_Hue, m_nHue, VideoProcAmp_Flags_Manual);
	m_pProcAmp->Release();
	if ( SUCCEEDED(hr) )
		return(true);
	else
		return(false);
}

bool CUDSCapture::GetSaturation(CSliderCtrl *pSlider)
{
	if (m_pCameraFilter == NULL)
		return(false);

	// Query the capture filter for the IAMVideoProcAmp interface.
	IAMVideoProcAmp *m_pProcAmp;
	HRESULT	hr = m_pCameraFilter->QueryInterface(IID_IAMVideoProcAmp, (void**)&m_pProcAmp);
	if ( FAILED(hr) )
		return(false);

	long Min, Max, Step, Default, Flags;
	// Get the range and default value.
	hr = m_pProcAmp->GetRange(VideoProcAmp_Saturation, &Min, &Max, &Step, &Default, &Flags);

	if ( SUCCEEDED(hr) )
	{
		pSlider->EnableWindow(TRUE);
		pSlider->SetRange(Min, Max, TRUE);
		// Set Hue to Manual
		if (m_nSaturation < Min || m_nSaturation > Max)  // Get the current value
			hr = m_pProcAmp->Get(VideoProcAmp_Saturation, &m_nSaturation, &Flags);
		hr = m_pProcAmp->Set(VideoProcAmp_Saturation, m_nSaturation, VideoProcAmp_Flags_Manual);
		if ( SUCCEEDED(hr) )
			pSlider->SetPos(m_nSaturation);

		m_pProcAmp->Release();
		return(true);
	}
	else
	{
		m_pProcAmp->Release();
		pSlider->EnableWindow(FALSE);
		return(false);
	}
}

bool CUDSCapture::SetSaturation(CSliderCtrl *pSlider)
{
	if (m_pCameraFilter == NULL)
		return(false);

	// Query the capture filter for the IAMVideoProcAmp interface.
	IAMVideoProcAmp *m_pProcAmp;
	HRESULT	hr = m_pCameraFilter->QueryInterface(IID_IAMVideoProcAmp, (void**)&m_pProcAmp);
	if ( FAILED(hr) )
		return(false);

	// Set Hue
	m_nSaturation = pSlider->GetPos();
	hr = m_pProcAmp->Set(VideoProcAmp_Saturation, m_nSaturation, VideoProcAmp_Flags_Manual);
	m_pProcAmp->Release();
	if ( SUCCEEDED(hr) )
		return(true);
	else
		return(false);
}

bool CUDSCapture::GetSharpness(CSliderCtrl *pSlider)
{
	if (m_pCameraFilter == NULL)
		return(false);

	// Query the capture filter for the IAMVideoProcAmp interface.
	IAMVideoProcAmp *m_pProcAmp;
	HRESULT	hr = m_pCameraFilter->QueryInterface(IID_IAMVideoProcAmp, (void**)&m_pProcAmp);
	if ( FAILED(hr) )
		return(false);

	long Min, Max, Step, Default, Flags;
	// Get the range and default value.
	hr = m_pProcAmp->GetRange(VideoProcAmp_Sharpness, &Min, &Max, &Step, &Default, &Flags);

	if ( SUCCEEDED(hr) )
	{
		pSlider->EnableWindow(TRUE);
		pSlider->SetRange(Min, Max, TRUE);
		// Set Sharpness to Manual
		if (m_nSharpness < Min || m_nSharpness > Max)  // Get the current value
			hr = m_pProcAmp->Get(VideoProcAmp_Sharpness, &m_nSharpness, &Flags);
		hr = m_pProcAmp->Set(VideoProcAmp_Sharpness, m_nSharpness, VideoProcAmp_Flags_Manual);
		if ( SUCCEEDED(hr) )
			pSlider->SetPos(m_nSharpness);

		m_pProcAmp->Release();
		return(true);
	}
	else
	{
		m_pProcAmp->Release();
		pSlider->EnableWindow(FALSE);
		return(false);
	}
}

bool CUDSCapture::SetSharpness(CSliderCtrl *pSlider)
{
	if (m_pCameraFilter == NULL)
		return(false);

	// Query the capture filter for the IAMVideoProcAmp interface.
	IAMVideoProcAmp *m_pProcAmp;
	HRESULT	hr = m_pCameraFilter->QueryInterface(IID_IAMVideoProcAmp, (void**)&m_pProcAmp);
	if ( FAILED(hr) )
		return(false);

	// Set Sharpness
	m_nSharpness = pSlider->GetPos();
	hr = m_pProcAmp->Set(VideoProcAmp_Sharpness, m_nSharpness, VideoProcAmp_Flags_Manual);
	m_pProcAmp->Release();
	if ( SUCCEEDED(hr) )
		return(true);
	else
		return(false);
}

bool CUDSCapture::GetGamma(CSliderCtrl *pSlider)
{
	if (m_pCameraFilter == NULL)
		return(false);

	// Query the capture filter for the IAMVideoProcAmp interface.
	IAMVideoProcAmp *m_pProcAmp;
	HRESULT	hr = m_pCameraFilter->QueryInterface(IID_IAMVideoProcAmp, (void**)&m_pProcAmp);
	if ( FAILED(hr) )
		return(false);

	long Min, Max, Step, Default, Flags;
	// Get the range and default value.
	hr = m_pProcAmp->GetRange(VideoProcAmp_Gamma, &Min, &Max, &Step, &Default, &Flags);

	if ( SUCCEEDED(hr) )
	{
		pSlider->EnableWindow(TRUE);
		pSlider->SetRange(Min, Max, TRUE);
		// Set Gamma to Manual
		if (m_nGamma < Min || m_nGamma > Max)  // Get the current value
			hr = m_pProcAmp->Get(VideoProcAmp_Gamma, &m_nGamma, &Flags);
		hr = m_pProcAmp->Set(VideoProcAmp_Gamma, m_nGamma, VideoProcAmp_Flags_Manual);
		if ( SUCCEEDED(hr) )
			pSlider->SetPos(m_nGamma);

		m_pProcAmp->Release();
		return(true);
	}
	else
	{
		m_pProcAmp->Release();
		pSlider->EnableWindow(FALSE);
		return(false);
	}
}

bool CUDSCapture::SetGamma(CSliderCtrl *pSlider)
{
	if (m_pCameraFilter == NULL)
		return(false);

	// Query the capture filter for the IAMVideoProcAmp interface.
	IAMVideoProcAmp *m_pProcAmp;
	HRESULT	hr = m_pCameraFilter->QueryInterface(IID_IAMVideoProcAmp, (void**)&m_pProcAmp);
	if ( FAILED(hr) )
		return(false);

	// Set Gamma
	m_nGamma = pSlider->GetPos();
	hr = m_pProcAmp->Set(VideoProcAmp_Gamma, m_nGamma, VideoProcAmp_Flags_Manual);
	m_pProcAmp->Release();
	if ( SUCCEEDED(hr) )
		return(true);
	else
		return(false);
}

bool CUDSCapture::GetWhiteBalance(CSliderCtrl *pSlider)
{
	if (m_pCameraFilter == NULL)
		return(false);

	// Query the capture filter for the IAMVideoProcAmp interface.
	IAMVideoProcAmp *m_pProcAmp;
	HRESULT	hr = m_pCameraFilter->QueryInterface(IID_IAMVideoProcAmp, (void**)&m_pProcAmp);
	if ( FAILED(hr) )
		return(false);

	long Min, Max, Step, Default, Flags;
	// Get the range and default value.
	hr = m_pProcAmp->GetRange(VideoProcAmp_WhiteBalance, &Min, &Max, &Step, &Default, &Flags);

	if ( SUCCEEDED(hr) )
	{
		pSlider->EnableWindow(TRUE);
		pSlider->SetRange(Min, Max, TRUE);
		// Set WhiteBalance to Manual
		if (m_nWhiteBalance < Min || m_nWhiteBalance > Max)  // Get the current value
			hr = m_pProcAmp->Get(VideoProcAmp_WhiteBalance, &m_nWhiteBalance, &Flags);
		hr = m_pProcAmp->Set(VideoProcAmp_WhiteBalance, m_nWhiteBalance, VideoProcAmp_Flags_Manual);
		if ( SUCCEEDED(hr) )
			pSlider->SetPos(m_nWhiteBalance);

		m_pProcAmp->Release();
		return(true);
	}
	else
	{
		m_pProcAmp->Release();
		pSlider->EnableWindow(FALSE);
		return(false);
	}
}

bool CUDSCapture::SetWhiteBalance(CSliderCtrl *pSlider)
{
	if (m_pCameraFilter == NULL)
		return(false);

	// Query the capture filter for the IAMVideoProcAmp interface.
	IAMVideoProcAmp *m_pProcAmp;
	HRESULT	hr = m_pCameraFilter->QueryInterface(IID_IAMVideoProcAmp, (void**)&m_pProcAmp);
	if ( FAILED(hr) )
		return(false);

	// Set WhiteBalance
	m_nWhiteBalance = pSlider->GetPos();
	hr = m_pProcAmp->Set(VideoProcAmp_WhiteBalance, m_nWhiteBalance, VideoProcAmp_Flags_Manual);
	m_pProcAmp->Release();
	if ( SUCCEEDED(hr) )
		return(true);
	else
		return(false);


}

bool CUDSCapture::GetBacklightCompensation(CSliderCtrl *pSlider)
{
	if (m_pCameraFilter == NULL)
		return(false);

	// Query the capture filter for the IAMVideoProcAmp interface.
	IAMVideoProcAmp *m_pProcAmp;
	HRESULT	hr = m_pCameraFilter->QueryInterface(IID_IAMVideoProcAmp, (void**)&m_pProcAmp);
	if ( FAILED(hr) )
		return(false);

	long Min, Max, Step, Default, Flags;
	// Get the range and default value.
	hr = m_pProcAmp->GetRange(VideoProcAmp_BacklightCompensation, &Min, &Max, &Step, &Default, &Flags);

	if ( SUCCEEDED(hr) )
	{
		pSlider->EnableWindow(TRUE);
		pSlider->SetRange(Min, Max, TRUE);
		// Set BacklightCompensation to Manual
		if (m_nBacklightCompensation < Min || m_nBacklightCompensation > Max)  // Get the current value
			hr = m_pProcAmp->Get(VideoProcAmp_BacklightCompensation, &m_nBacklightCompensation, &Flags);
		hr = m_pProcAmp->Set(VideoProcAmp_BacklightCompensation, m_nBacklightCompensation, VideoProcAmp_Flags_Manual);
		if ( SUCCEEDED(hr) )
			pSlider->SetPos(m_nBacklightCompensation);

		m_pProcAmp->Release();
		return(true);
	}
	else
	{
		m_pProcAmp->Release();
		pSlider->EnableWindow(FALSE);
		return(false);
	}
}

bool CUDSCapture::SetBacklightCompensation(CSliderCtrl *pSlider)
{
	if (m_pCameraFilter == NULL)
		return(false);

	// Query the capture filter for the IAMVideoProcAmp interface.
	IAMVideoProcAmp *m_pProcAmp;
	HRESULT	hr = m_pCameraFilter->QueryInterface(IID_IAMVideoProcAmp, (void**)&m_pProcAmp);
	if ( FAILED(hr) )
		return(false);

	// Set BacklightCompensation
	m_nBacklightCompensation = pSlider->GetPos();
	hr = m_pProcAmp->Set(VideoProcAmp_BacklightCompensation, m_nBacklightCompensation, VideoProcAmp_Flags_Manual);
	m_pProcAmp->Release();
	if ( SUCCEEDED(hr) )
		return(true);
	else
		return(false);
}

bool CUDSCapture::GetGain(CSliderCtrl *pSlider)
{
	if (m_pCameraFilter == NULL)
		return(false);

	// Query the capture filter for the IAMVideoProcAmp interface.
	IAMVideoProcAmp *m_pProcAmp;
	HRESULT	hr = m_pCameraFilter->QueryInterface(IID_IAMVideoProcAmp, (void**)&m_pProcAmp);
	if ( FAILED(hr) )
		return(false);

	long Min, Max, Step, Default, Flags;
	// Get the range and default value.
	hr = m_pProcAmp->GetRange(VideoProcAmp_Gain, &Min, &Max, &Step, &Default, &Flags);

	if ( SUCCEEDED(hr) )
	{
		pSlider->EnableWindow(TRUE);
		pSlider->SetRange(Min, Max, TRUE);
		// Set Gain to Manual
		if (m_nGain < Min || m_nGain > Max)  // Get the current value
			hr = m_pProcAmp->Get(VideoProcAmp_Gain, &m_nGain, &Flags);
		hr = m_pProcAmp->Set(VideoProcAmp_Gain, m_nGain, VideoProcAmp_Flags_Manual);
		if ( SUCCEEDED(hr) )
			pSlider->SetPos(m_nGain);

		m_pProcAmp->Release();
		return(true);
	}
	else
	{
		m_pProcAmp->Release();
		pSlider->EnableWindow(FALSE);
		return(false);
	}
}

bool CUDSCapture::SetGain(CSliderCtrl *pSlider)
{
	if (m_pCameraFilter == NULL)
		return(false);

	// Query the capture filter for the IAMVideoProcAmp interface.
	IAMVideoProcAmp *m_pProcAmp;
	HRESULT	hr = m_pCameraFilter->QueryInterface(IID_IAMVideoProcAmp, (void**)&m_pProcAmp);
	if ( FAILED(hr) )
		return(false);

	// Set Gain
	m_nGain = pSlider->GetPos();
	hr = m_pProcAmp->Set(VideoProcAmp_Gain, m_nGain, VideoProcAmp_Flags_Manual);
	m_pProcAmp->Release();
	if ( SUCCEEDED(hr) )
		return(true);
	else
		return(false);
}

bool CUDSCapture::GetZoom(CSliderCtrl *pSlider)
{
	if (m_pCameraFilter == NULL)
		return(false);

	// Query the capture filter for the IAMCameraControl interface.
	IAMCameraControl *m_pCameraControl;
	HRESULT	hr = m_pCameraFilter->QueryInterface(IID_IAMCameraControl, (void**)&m_pCameraControl);
	if ( FAILED(hr) )
		return(false);

	long Min, Max, Step, Default, Flags;
	// Get the range and default value.
	hr = m_pCameraControl->GetRange(CameraControl_Zoom, &Min, &Max, &Step, &Default, &Flags);
	if ( SUCCEEDED(hr) )
	{

		pSlider->EnableWindow(TRUE);
		pSlider->SetRange(Min, Max, TRUE);

		// Set Zoom to Manual
		if (m_nZoom < Min || m_nZoom > Max)  // Get the current value
			hr = m_pCameraControl->Get(CameraControl_Zoom, &m_nZoom, &Flags);
		hr = m_pCameraControl->Set(CameraControl_Zoom, m_nZoom, CameraControl_Flags_Manual);
		m_pCameraControl->Release();
		if ( SUCCEEDED(hr) )
			pSlider->SetPos(m_nZoom);
		return(true);
	}
	else
	{
		m_pCameraControl->Release();
		pSlider->EnableWindow(FALSE);
		return(false);
	}
}

bool CUDSCapture::SetZoom(CSliderCtrl *pSlider)
{
	if (m_pCameraFilter == NULL)
		return(false);

	// Query the capture filter for the IAMCameraControl interface.
	IAMCameraControl *m_pCameraControl;
	HRESULT	hr = m_pCameraFilter->QueryInterface(IID_IAMCameraControl, (void**)&m_pCameraControl);
	if ( FAILED(hr) )
		return(false);

	m_nZoom = pSlider->GetPos();
	hr = m_pCameraControl->Set(CameraControl_Zoom, m_nZoom, CameraControl_Flags_Manual);
	m_pCameraControl->Release();
	if ( SUCCEEDED(hr) )
		return(true);
	else
		return(false);
}

bool CUDSCapture::GetFocus(CSliderCtrl *pSlider)
{
	if (m_pCameraFilter == NULL)
		return(false);

	// Query the capture filter for the IAMCameraControl interface.
	IAMCameraControl *m_pCameraControl;
	HRESULT	hr = m_pCameraFilter->QueryInterface(IID_IAMCameraControl, (void**)&m_pCameraControl);
	if ( FAILED(hr) )
		return(false);

	long Min, Max, Step, Default, Flags;
	// Get the range and default value.
	hr = m_pCameraControl->GetRange(CameraControl_Focus, &Min, &Max, &Step, &Default, &Flags);
	if ( SUCCEEDED(hr) )
	{

		pSlider->EnableWindow(TRUE);
		pSlider->SetRange(Min, Max, TRUE);

		// Set Focus to Manual
		if (m_nFocus < Min || m_nFocus > Max)  // Get the current value
			hr = m_pCameraControl->Get(CameraControl_Focus, &m_nFocus, &Flags);
		hr = m_pCameraControl->Set(CameraControl_Focus, m_nFocus, CameraControl_Flags_Manual);
		m_pCameraControl->Release();
		if ( SUCCEEDED(hr) )
			pSlider->SetPos(m_nFocus);
		return(true);
	}
	else
	{
		m_pCameraControl->Release();
		pSlider->EnableWindow(FALSE);
		return(false);
	}
}

bool CUDSCapture::SetFocus(CSliderCtrl *pSlider)
{
	if (m_pCameraFilter == NULL)
		return(false);

	// Query the capture filter for the IAMCameraControl interface.
	IAMCameraControl *m_pCameraControl;
	HRESULT	hr = m_pCameraFilter->QueryInterface(IID_IAMCameraControl, (void**)&m_pCameraControl);
	if ( FAILED(hr) )
		return(false);

	m_nFocus = pSlider->GetPos();
	hr = m_pCameraControl->Set(CameraControl_Focus, m_nFocus, CameraControl_Flags_Manual);
	m_pCameraControl->Release();
	if ( SUCCEEDED(hr) )
		return(true);
	else
		return(false);
}

bool CUDSCapture::GetIris(CSliderCtrl *pSlider)
{
	if (m_pCameraFilter == NULL)
		return(false);

	// Query the capture filter for the IAMCameraControl interface.
	IAMCameraControl *m_pCameraControl;
	HRESULT	hr = m_pCameraFilter->QueryInterface(IID_IAMCameraControl, (void**)&m_pCameraControl);
	if ( FAILED(hr) )
		return(false);

	long Min, Max, Step, Default, Flags;
	// Get the range and default value.
	hr = m_pCameraControl->GetRange(CameraControl_Iris, &Min, &Max, &Step, &Default, &Flags);
	if ( SUCCEEDED(hr) )
	{

		pSlider->EnableWindow(TRUE);
		pSlider->SetRange(Min, Max, TRUE);

		// Set Iris to Manual
		if (m_nIris < Min || m_nIris > Max)  // Get the current value
			hr = m_pCameraControl->Get(CameraControl_Iris, &m_nIris, &Flags);
		hr = m_pCameraControl->Set(CameraControl_Iris, m_nIris, CameraControl_Flags_Manual);
		m_pCameraControl->Release();
		if ( SUCCEEDED(hr) )
			pSlider->SetPos(m_nIris);
		return(true);
	}
	else
	{
		m_pCameraControl->Release();
		pSlider->EnableWindow(FALSE);
		return(false);
	}
}

bool CUDSCapture::SetIris(CSliderCtrl *pSlider)
{
	if (m_pCameraFilter == NULL)
		return(false);

	// Query the capture filter for the IAMCameraControl interface.
	IAMCameraControl *m_pCameraControl;
	HRESULT	hr = m_pCameraFilter->QueryInterface(IID_IAMCameraControl, (void**)&m_pCameraControl);
	if ( FAILED(hr) )
		return(false);

	m_nIris = pSlider->GetPos();
	hr = m_pCameraControl->Set(CameraControl_Iris, m_nIris, CameraControl_Flags_Manual);
	m_pCameraControl->Release();
	if ( SUCCEEDED(hr) )
		return(true);
	else
		return(false);
}

bool CUDSCapture::GetPan(CSliderCtrl *pSlider)
{
	if (m_pCameraFilter == NULL)
		return(false);

	// Query the capture filter for the IAMCameraControl interface.
	IAMCameraControl *m_pCameraControl;
	HRESULT	hr = m_pCameraFilter->QueryInterface(IID_IAMCameraControl, (void**)&m_pCameraControl);
	if ( FAILED(hr) )
		return(false);

	long Min, Max, Step, Default, Flags;
	// Get the range and default value.
	hr = m_pCameraControl->GetRange(CameraControl_Pan, &Min, &Max, &Step, &Default, &Flags);
	if ( SUCCEEDED(hr) )
	{

		pSlider->EnableWindow(TRUE);
		pSlider->SetRange(Min, Max, TRUE);

		// Set Pan to Manual
		if (m_nPan < Min || m_nPan > Max)  // Get the current value
			hr = m_pCameraControl->Get(CameraControl_Pan, &m_nPan, &Flags);
		hr = m_pCameraControl->Set(CameraControl_Pan, m_nPan, CameraControl_Flags_Manual);
		m_pCameraControl->Release();
		if ( SUCCEEDED(hr) )
			pSlider->SetPos(m_nPan);
		return(true);
	}
	else
	{
		m_pCameraControl->Release();
		pSlider->EnableWindow(FALSE);
		return(false);
	}
}

bool CUDSCapture::SetPan(CSliderCtrl *pSlider)
{
	if (m_pCameraFilter == NULL)
		return(false);

	// Query the capture filter for the IAMCameraControl interface.
	IAMCameraControl *m_pCameraControl;
	HRESULT	hr = m_pCameraFilter->QueryInterface(IID_IAMCameraControl, (void**)&m_pCameraControl);
	if ( FAILED(hr) )
		return(false);

	m_nPan = pSlider->GetPos();
	hr = m_pCameraControl->Set(CameraControl_Pan, m_nPan, CameraControl_Flags_Manual);
	m_pCameraControl->Release();
	if ( SUCCEEDED(hr) )
		return(true);
	else
		return(false);
}

bool CUDSCapture::GetTilt(CSliderCtrl *pSlider)
{
	if (m_pCameraFilter == NULL)
		return(false);

	// Query the capture filter for the IAMCameraControl interface.
	IAMCameraControl *m_pCameraControl;
	HRESULT	hr = m_pCameraFilter->QueryInterface(IID_IAMCameraControl, (void**)&m_pCameraControl);
	if ( FAILED(hr) )
		return(false);

	long Min, Max, Step, Default, Flags;
	// Get the range and default value.
	hr = m_pCameraControl->GetRange(CameraControl_Tilt, &Min, &Max, &Step, &Default, &Flags);
	if ( SUCCEEDED(hr) )
	{

		pSlider->EnableWindow(TRUE);
		pSlider->SetRange(Min, Max, TRUE);

		// Set Tilt to Manual
		if (m_nTilt < Min || m_nTilt > Max)  // Get the current value
			hr = m_pCameraControl->Get(CameraControl_Tilt, &m_nTilt, &Flags);
		hr = m_pCameraControl->Set(CameraControl_Tilt, m_nTilt, CameraControl_Flags_Manual);
		m_pCameraControl->Release();
		if ( SUCCEEDED(hr) )
			pSlider->SetPos(m_nTilt);
		return(true);
	}
	else
	{
		m_pCameraControl->Release();
		pSlider->EnableWindow(FALSE);
		return(false);
	}
}

bool CUDSCapture::SetTilt(CSliderCtrl *pSlider)
{
	if (m_pCameraFilter == NULL)
		return(false);

	// Query the capture filter for the IAMCameraControl interface.
	IAMCameraControl *m_pCameraControl;
	HRESULT	hr = m_pCameraFilter->QueryInterface(IID_IAMCameraControl, (void**)&m_pCameraControl);
	if ( FAILED(hr) )
		return(false);

	m_nTilt = pSlider->GetPos();
	hr = m_pCameraControl->Set(CameraControl_Tilt, m_nTilt, CameraControl_Flags_Manual);
	m_pCameraControl->Release();
	if ( SUCCEEDED(hr) )
		return(true);
	else
		return(false);
}

bool CUDSCapture::GetRoll(CSliderCtrl *pSlider)
{
	if (m_pCameraFilter == NULL)
		return(false);

	// Query the capture filter for the IAMCameraControl interface.
	IAMCameraControl *m_pCameraControl;
	HRESULT	hr = m_pCameraFilter->QueryInterface(IID_IAMCameraControl, (void**)&m_pCameraControl);
	if ( FAILED(hr) )
		return(false);

	long Min, Max, Step, Default, Flags;
	// Get the range and default value.
	hr = m_pCameraControl->GetRange(CameraControl_Roll, &Min, &Max, &Step, &Default, &Flags);
	if ( SUCCEEDED(hr) )
	{

		pSlider->EnableWindow(TRUE);
		pSlider->SetRange(Min, Max, TRUE);

		// Set Tilt to Manual
		if (m_nRoll < Min || m_nRoll > Max)  // Get the current value
			hr = m_pCameraControl->Get(CameraControl_Roll, &m_nRoll, &Flags);
		hr = m_pCameraControl->Set(CameraControl_Roll, m_nRoll, CameraControl_Flags_Manual);
		m_pCameraControl->Release();
		if ( SUCCEEDED(hr) )
			pSlider->SetPos(m_nRoll);
		return(true);
	}
	else
	{
		m_pCameraControl->Release();
		pSlider->EnableWindow(FALSE);
		return(false);
	}
}

bool CUDSCapture::SetRoll(CSliderCtrl *pSlider)
{
	if (m_pCameraFilter == NULL)
		return(false);

	// Query the capture filter for the IAMCameraControl interface.
	IAMCameraControl *m_pCameraControl;
	HRESULT	hr = m_pCameraFilter->QueryInterface(IID_IAMCameraControl, (void**)&m_pCameraControl);
	if ( FAILED(hr) )
		return(false);

	m_nRoll = pSlider->GetPos();
	hr = m_pCameraControl->Set(CameraControl_Roll, m_nRoll, CameraControl_Flags_Manual);
	m_pCameraControl->Release();
	if ( SUCCEEDED(hr) )
		return(true);
	else
		return(false);
}

// CUDSCapture 消息处理程序



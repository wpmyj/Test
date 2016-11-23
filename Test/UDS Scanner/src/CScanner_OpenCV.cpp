#include "CScanner_OpenCV.h"
//#include <iostream>
#include <time.h>
//#include <stdio.h>
//#include <sys/types.h>
//#include <sys/stat.h>
//#include <fcntl.h>
//#ifdef TWH_CMP_MSC
//#include <io.h>
//#else //#ifdef TWH_CMP_MSC
//#include <sys/io.h>
//#endif //#ifdef TWH_CMP_MSC
//
//#ifdef TWNDS_OS_LINUX
//#define kTWAIN_DS_DIR "/usr/local/lib/twain/sample2"
//#endif

//#include "DSMInterface.h"
#include "public.h"

extern HWND g_hwndDLG;

//using namespace std;

/**
* Environment vars to get the Xfer Count.  Create this enviroment Varable on your system to simulate the 
* number of pages sitting in the scanner waiting to be scanned.
*/
#define kGETENV_XFERCOUNT "CAP_XFERCOUNT"

#ifdef TWH_CMP_MSC
extern HINSTANCE   g_hinstance;
#endif

CScanner_OpenCV::CScanner_OpenCV(void) :
	m_nScanLine(0),
	m_nDestBytesPerRow(0),
	m_nDocCount(0),
	m_nSourceWidth(0),
	m_nSourceHeight(0)
{
	memset(m_szSourceImagePath, 0, PATH_MAX);

	char szTWAIN_DS_DIR[PATH_MAX];

#ifdef TWH_CMP_MSC
	GetModuleFileName(g_hinstance, szTWAIN_DS_DIR, PATH_MAX);
	// strip filename from path
	size_t x = strlen(szTWAIN_DS_DIR);
	while(x > 0)
	{
		if(PATH_SEPERATOR == szTWAIN_DS_DIR[x-1])
		{
			szTWAIN_DS_DIR[x-1] = 0;
			break;
		}
		--x;
	}
#else
	// In non-windows, kTWAIN_DS_DIR is set on the compiler command line
	strncpy(szTWAIN_DS_DIR, kTWAIN_DS_DIR, PATH_MAX);
#endif

	SSNPRINTF(m_szSourceImagePath, sizeof(m_szSourceImagePath),
		PATH_MAX, "%s%cTWAIN_logo.png", szTWAIN_DS_DIR, PATH_SEPERATOR);

	// set default caps
	resetScanner();
	
}


CScanner_OpenCV::~CScanner_OpenCV(void)
{
	if (m_mat_image.empty())
	{
		m_mat_image.release();
	}
	
}

bool CScanner_OpenCV::resetScanner()
{
	bool bret = true;

	// Unlock the scanner 
	Unlock();

	m_nScanLine        = 0;
	m_nDestBytesPerRow = 0;

	m_nDocCount     = m_nMaxDocCount = getDocumentCount();// Reloaded the scanner with paper
	m_nPixelType    = TWPT_RGB;
	m_nPaperSource  = SFI_PAPERSOURCE_ADF;
	m_bDuplex       = false;
	m_nWidth        = 0;
	m_nHeight       = 0;
	m_fXResolution  = 200.0;
	m_fYResolution  = 200.0;
	m_fGamma        = 100.0; //zhu 默认为1

	m_nRotation     = 0; //zhu
	m_nOrientation  = TWOR_ROT0; //zhu 纵向

	m_nBinarization = TWBZ_DYNATHRESHOLD; //zhu 动态阈值
	m_nSpiltImage   = TWSI_NONE; //zhu 不分割

	//默认不选中
	m_fRemoveBlank  = TWRP_DISABLE; 
	m_fRemovePunch  = TWSP_DISABLE;
	m_fSharpen      = TWGM_DISABLE;
	m_fMirror       = TWMR_DISABLE;
	m_fRemoveBack   = TWRB_DISABLE;
	m_fDescreen     = TWDS_DISABLE;
	m_fDenoise      = TWDN_DISABLE;
	m_fAutoCrop     = TWAC_DISABLE;

	//if(0 != m_pDIB)   //如果m_pDIB（保存着位图信息和像素数据的指针）不为 0
	//{
	//	FreeImage_Unload(m_pDIB);
	//	m_pDIB = 0;
	//}

	return bret;
}

void CScanner_OpenCV::setSetting(CScanner_Base settings)
{
	CScanner_Base::setSetting(settings);  // 调用父类的方法
	m_nDocCount = m_nMaxDocCount;
}

bool CScanner_OpenCV::acquireImage()
{
	//::MessageBox(g_hwndDLG,TEXT("CScanner_OpenCV::acquireImage()!"),"UDS",MB_OK);
	if(false == m_mat_image.empty())
	{
		m_mat_image.release();
	}

	// get the image if it exists
	if(FALSE == FILE_EXISTS(m_szSourceImagePath))
	{
		::MessageBox(g_hwndDLG,TEXT("ds: Could not find required file: "),"UDS",MB_OK);
		//cerr << "ds: Could not find required file: " << m_szSourceImagePath << endl;
		return false;
	}

	m_mat_image = cv::imread(m_szSourceImagePath);

	if(true == m_mat_image.empty())
	{
		::MessageBox(g_hwndDLG,TEXT("ds: Failed - could not acquire image!"),MB_CAPTION,MB_OK);
		//cout << "ds: Failed - could not acquire image" << endl;
		return false;
	}

	//Document scanned, remove it from simulated intray
	m_nDocCount--;

	// do whatever tranforms to the scanned image that was requested by the app
	// before the image is sent to the app.
	if(false == preScanPrep())
	{
		return false;
	}

	return true;
}

bool CScanner_OpenCV::preScanPrep()
{
	IplImage img= IplImage(m_mat_image);  // Mat->IplImage
	
	// 获取影像的宽高，都以像素为单位 
	m_nSourceWidth   = img.width;
	m_nSourceHeight  = img.height;
	WORD res = 0;

	//// 获取影像的水平分辨率，以每米像素数为单位 
	//res = FreeImage_GetDotsPerMeterX( m_pDIB );
	//WORD unNewWidth  = WORD(m_nSourceWidth /(WORD)((float)res/39.37 + 0.5)* m_fXResolution);

	//// 获取影像的垂直分辨率，以每米像素数为单位 
	//res = FreeImage_GetDotsPerMeterY( m_pDIB );
	//WORD unNewHeight = WORD(m_nSourceHeight/(WORD)((float)res/39.37 + 0.5)* m_fYResolution);

	//cout << "ds: rescaling... to " << unNewWidth << " x " << unNewHeight << endl;
	////pDib = FreeImage_Rescale( m_pDIB, unNewWidth, unNewHeight, FILTER_BILINEAR);


	
	double dFx = (double)m_fXResolution/100;
	double dFy = (double)m_fYResolution/100;

	WORD unNewWidth = (WORD)(m_nSourceWidth * dFx);
	WORD unNewHeight = (WORD)(m_nSourceHeight * dFy);


	cv::Mat matTemp;
	//cv::Size size(unNewWidth, unNewHeight);
	//resize(m_mat_image, matTemp, size, 0, 0);

	resize(m_mat_image, matTemp, cv::Size(unNewWidth, unNewHeight), 0, 0);		
	m_mat_image = matTemp;

	m_dRat = (double)unNewWidth/unNewHeight;

	if(m_nOrientation == TWOR_LANDSCAPE) //横向
	{		
		//方法1
	  RotateImage(90);

		//方法2
		/*Mat matTrans;
		flip(m_mat_image, matTrans, 0);
		m_mat_image = matTrans;*/
	}

	// 旋转
	switch(m_nRotation)
	{
	case TWOR_ROT0:
		{
			RotateImage(0);
		}
		break;
	case TWOR_ROT90:
		{
			RotateImage(90);
    }
		break;
	case TWOR_ROT180:
		{
			RotateImage(180);
		}
		break;
	case TWOR_ROT270:
		{
			RotateImage(270);
		}
		break;
	default:
		break;
	} 
	
	
	switch(m_nPixelType)
	{
	case TWPT_BW:
		m_nDestBytesPerRow = BYTES_PERLINE(m_nWidth, 1);
		break;

	case TWPT_GRAY:
		m_nDestBytesPerRow = BYTES_PERLINE(m_nWidth, 8);
		break;

	case TWPT_RGB:
		m_nDestBytesPerRow = BYTES_PERLINE(m_nWidth, 24);
		break;
	}

	// setup some convenience vars because they are used during 
	// every strip request
	m_nScanLine       = 0;

	return true;
}

bool CScanner_OpenCV::getScanStrip(BYTE *pTransferBuffer, DWORD dwRead, DWORD &dwReceived)
{
	////::MessageBox(g_hwndDLG,"getScanStrip","UDS",MB_OK);
	


	IplImage* pImg= new IplImage(m_mat_image);  // Mat->IplImage*
	BYTE* pTempBuffer = (BYTE*)pImg->imageData;   // IplImage*->BYTE*

	memcpy(pTransferBuffer, pTempBuffer, pImg->imageSize);

	return true;
}

short CScanner_OpenCV::getDocumentCount() const
{
	// Simulate the number of pages sitting in the scanner.
	int nCount = 1;

	// Read this value from the environment. This will allow the simulation
	// of a sheet feeder.      从外界环境读取此值，这可以模拟送纸器
	// If the value is <= 0, then a random number of pages will be scanned, else
	// the exact number will be used.     如果 nCount <= 0，则返回一个随机扫描页数，否则返回一个精确的页数
	char szCount[10];
	memset(szCount, 0, sizeof(szCount));

	if( 0 != SGETENV(szCount, sizeof(szCount), kGETENV_XFERCOUNT) )   // 从外界环境读取此值，以模拟送纸器
	{
		// something found, convert it to an int
		nCount = atoi(szCount);                    // 字符串转换成整型数

		if(nCount <= 0)
		{
			srand(int(time(0)));
			nCount = rand();
			nCount = nCount%15;// upto 15 pages
		}
	}

	return nCount;
}

bool CScanner_OpenCV::isFeederLoaded()
{
	bool rtn = true;
	if(m_nDocCount<=0)
	{
		rtn = false;
		m_nDocCount = m_nMaxDocCount;// Reloaded the scanner with paper
	}
	return rtn;
}

bool CScanner_OpenCV::getDeviceOnline() const
{
	return true;
}

void CScanner_OpenCV::GetImageData(BYTE *buffer, DWORD &dwReceived)
{
	/* 方法一 */
	//IplImage* pImg= new IplImage(m_mat_image);  // Mat->IplImage*
	//BYTE* pTempBuffer = (BYTE*)pImg->imageData;   // IplImage*->BYTE*
	//memcpy(buffer, pTempBuffer, pImg->imageSize);

	/* 方法二 */
	//IplImage pImg= IplImage(m_mat_image);  // Mat->IplImage
	//BYTE* pTempBuffer = (BYTE*)(pImg.imageData);   // IplImage*->BYTE*
	//memcpy(buffer, pTempBuffer, pImg.imageSize);

	/* 方法三 */
	//int size = m_mat_image.total() * m_mat_image.elemSize();
	//std::memcpy(buffer,m_mat_image.data,size * sizeof(BYTE));
	MatToBYTEs(m_mat_image, buffer);


	//dwReceived = size * sizeof(BYTE);

	//char buf[200];
	//ltoa(dwReceived, buf, 10);
	//::MessageBox(g_hwndDLG,TEXT(buf),MB_CAPTION,MB_OK);
	//buffer += pImg->imageSize;

	//dwReceived = pImg->imageSize;
}

void CScanner_OpenCV::MatToBYTEs(cv::Mat matIn, BYTE* bytesOut)
{
	int size = matIn.total() * matIn.elemSize();
	std::memcpy(bytesOut, matIn.data, size * sizeof(BYTE));
}


void CScanner_OpenCV::RotateImage(double angle)
{
	double scale = 1; // 缩放尺度 

	if (90 == angle || 270 == angle)
	{
		scale = m_dRat;
	}
	cv::Point2f center = cv::Point2f((float)m_mat_image.cols / 2, 
		(float)m_mat_image.rows / 2);  // 旋转中心   
	 

	cv::Mat rotateMat;   
	rotateMat = cv::getRotationMatrix2D(center, angle, scale);  

	cv::Mat rotateImg;  
	cv::warpAffine(m_mat_image, rotateImg, rotateMat, m_mat_image.size());  

	m_mat_image = rotateImg;
}

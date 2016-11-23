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

	m_nScanLine           = 0;
	m_nDestBytesPerRow    = 0;

	m_nDocCount           = m_nMaxDocCount = getDocumentCount();// Reloaded the scanner with paper

	//Base����
	m_nPaperSource        = SFI_PAPERSOURCE_ADF;  //ɨ��ģʽ-�Զ���ֽ��
	m_nPixelType          = TWPT_RGB; //ͼ������-��ɫ zhu
	m_fXResolution        = 200.0;
	m_fYResolution        = 200.0; //�ֱ���-200.0
	m_bDuplex             = false; //����/˫��-����
	m_fContrast           = 0.0; //�Աȶ�-0.0
	m_fBrightness         = 0.0; //����-0.0
	m_fThreshold          = 128.0; //��ֵ-128.0 ������Ĭ��128.G6400Ĭ��230
	m_bMultifeedDetection = true; //���ż��-ѡ��

	//Advanced����
	m_nOrientation        = TWOR_ROT0; //zhu ֽ�ŷ���-����
	m_nStandardsizes      = TWSS_USLETTER; //zhu ��ӦICAP_SUPPORTEDSIZES��ֽ�Ŵ�С-TWSS_USLETTER
	m_nUnits              = TWUN_INCHES;  //zhu ��λ-Ӣ��

	m_nWidth              = 0;
	m_nHeight             = 0;

	m_nRotation           = 0; //��ת-����תzhu
	m_nSpiltImage         = TWSI_NONE; //zhu �ָ�-���ָ�
	m_fGamma              = 100.0; //zhu GammaУ��-Ĭ��Ϊ100
	m_fMirror             = TWMR_DISABLE; //����-��ѡ��

	m_nBinarization       = TWBZ_DYNATHRESHOLD; //zhu ��ֵ��-��̬��ֵ
	m_bMultiStream        = false; //�������-��ѡ��
	m_fSensitiveThreshold = 0.0; //ȥ���ߵ�-0.0

	//����ͼ����
	//Ĭ�ϲ�ѡ��
	m_fRemoveBlank        = TWRP_DISABLE; 
	m_fRemovePunch        = TWSP_DISABLE;
	m_fSharpen            = TWGM_DISABLE;
	m_fRemoveBack         = TWRB_DISABLE;
	m_fDescreen           = TWDS_DISABLE;
	m_fDenoise            = TWDN_DISABLE;
	m_fAutoCrop           = TWAC_DISABLE;

	if (m_mat_image.empty())
	{
		m_mat_image.release();
	}

	return bret;
}

void CScanner_OpenCV::setSetting(CScanner_Base settings)
{
	CScanner_Base::setSetting(settings);  // ���ø���ķ���
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
	

	//// ��ȡӰ���ˮƽ�ֱ��ʣ���ÿ��������Ϊ��λ 
	//res = FreeImage_GetDotsPerMeterX( m_pDIB );
	//WORD unNewWidth  = WORD(m_nSourceWidth /(WORD)((float)res/39.37 + 0.5)* m_fXResolution);

	//// ��ȡӰ��Ĵ�ֱ�ֱ��ʣ���ÿ��������Ϊ��λ 
	//res = FreeImage_GetDotsPerMeterY( m_pDIB );
	//WORD unNewHeight = WORD(m_nSourceHeight/(WORD)((float)res/39.37 + 0.5)* m_fYResolution);

	//cout << "ds: rescaling... to " << unNewWidth << " x " << unNewHeight << endl;
	////pDib = FreeImage_Rescale( m_pDIB, unNewWidth, unNewHeight, FILTER_BILINEAR);
	
	
	IplImage img= IplImage(m_mat_image);  // Mat->IplImage

	// ��ȡӰ��Ŀ�ߣ���������Ϊ��λ 
	m_nSourceWidth   = img.width;
	m_nSourceHeight  = img.height;
	WORD res = 0;
	double dFx = (double)m_fXResolution/100;
	double dFy = (double)m_fYResolution/100;

	WORD unNewWidth = (WORD)(m_nSourceWidth * dFx);
	WORD unNewHeight = (WORD)(m_nSourceHeight * dFy);


	cv::Mat matTemp;
	cv::resize(m_mat_image, matTemp, cv::Size(unNewWidth, unNewHeight), 0, 0);		
	//m_mat_image = matTemp;
	matTemp.copyTo(m_mat_image);  // ���
	m_dRat = (double)unNewWidth/unNewHeight;
	


	if(m_nOrientation == TWOR_LANDSCAPE) //����
	{		
		RotateImage(90);
		//m_mat_image = angelRotate(m_mat_image, 90);
	}

	// ��ת
	switch(m_nRotation)
	{
	case TWOR_ROT0:
		{
			RotateImage(0);
		}
		break;
	case TWOR_ROT90:
		{
			RotateImage(-90);
    }
		break;
	case TWOR_ROT180:
		{
			RotateImage(-180);
		}
		break;
	case TWOR_ROT270:
		{
			RotateImage(-270);
		}
		break;
	default:
		break;
	} 
	
	//ͼ������
	if(m_fMirror == TWMR_AUTO)
	{ 
		Mat mat_hMirror;
		hMirrorTrans(m_mat_image, mat_hMirror);
		mat_hMirror.copyTo(m_mat_image);
	}

	//��ɫ
	if(m_nPixelType != TWPT_RGB)
	{
		Mat dstImage;
		// Apply bit depth color transforms
		switch(m_nPixelType)
		{
		case TWPT_BW:
			{
		
			}
			break;

		case TWPT_GRAY:
			{
				
				dstImage.create(m_mat_image.size(), m_mat_image.type());
				cvtColor(m_mat_image, dstImage, CV_BGR2GRAY);
				dstImage.copyTo(m_mat_image);
			}	
			break;
		}
	}

	if(m_nWidth <= 0 || m_nHeight <= 0)
	{
		m_nWidth  = m_nSourceWidth;
		m_nHeight = m_nSourceHeight;
	}

	/*switch(m_nPixelType)
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
	}*/

	// setup some convenience vars because they are used during 
	// every strip request
	m_nScanLine       = 0;


	return true;
}


//bool CScanner_OpenCV::getScanStrip(BYTE *pTransferBuffer, DWORD dwRead, DWORD &dwReceived)
//{
//	////::MessageBox(g_hwndDLG,"getScanStrip","UDS",MB_OK);
//	
//
//
//	IplImage* pImg= new IplImage(m_mat_image);  // Mat->IplImage*
//	BYTE* pTempBuffer = (BYTE*)pImg->imageData;   // IplImage*->BYTE*
//
//	memcpy(pTransferBuffer, pTempBuffer, pImg->imageSize);
//
//	return true;
//}

short CScanner_OpenCV::getDocumentCount() const
{
	// Simulate the number of pages sitting in the scanner.
	int nCount = 1;

	// Read this value from the environment. This will allow the simulation
	// of a sheet feeder.      ����绷����ȡ��ֵ�������ģ����ֽ��
	// If the value is <= 0, then a random number of pages will be scanned, else
	// the exact number will be used.     ��� nCount <= 0���򷵻�һ�����ɨ��ҳ�������򷵻�һ����ȷ��ҳ��
	char szCount[10];
	memset(szCount, 0, sizeof(szCount));

	if( 0 != SGETENV(szCount, sizeof(szCount), kGETENV_XFERCOUNT) )   // ����绷����ȡ��ֵ����ģ����ֽ��
	{
		// something found, convert it to an int
		nCount = atoi(szCount);                    // �ַ���ת����������

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
	MatToBYTEs(m_mat_image, buffer);
}

void CScanner_OpenCV::MatToBYTEs(cv::Mat matIn, BYTE* bytesOut)
{
	int size = matIn.total() * matIn.elemSize();
	std::memcpy(bytesOut, matIn.data, size * sizeof(BYTE));
}


void CScanner_OpenCV::RotateImage(double angle)
{
	double scale = 1; // ���ų߶� 

	if (90 == angle || 270 == angle || -90 == angle || -270 == angle)
	{
		scale = m_dRat;
	}
	cv::Point2f center = cv::Point2f((float)m_mat_image.cols / 2, 
		(float)m_mat_image.rows / 2);  // ��ת����   
	 

	cv::Mat rotateMat;   
	rotateMat = cv::getRotationMatrix2D(center, angle, scale);  

	cv::Mat rotateImg;  
	cv::warpAffine(m_mat_image, rotateImg, rotateMat, m_mat_image.size());  

	m_mat_image = rotateImg;
}

void CScanner_OpenCV::hMirrorTrans(const Mat &src, Mat &dst)
{
	CV_Assert(src.depth() == CV_8U);
	dst.create(src.rows, src.cols, src.type());

	int rows = src.rows;
	int cols = src.cols;

	switch (src.channels())
	{
	case 1:
		const uchar *origal;
		uchar *p;
		for (int i = 0; i < rows; i++){
			origal = src.ptr<uchar>(i);
			p = dst.ptr<uchar>(i);
			for (int j = 0; j < cols; j++){
				p[j] = origal[cols - 1 - j];
			}
		}
		break;
	case 3:
		const Vec3b *origal3;
		Vec3b *p3;
		for (int i = 0; i < rows; i++) {
			origal3 = src.ptr<Vec3b>(i);
			p3 = dst.ptr<Vec3b>(i);
			for(int j = 0; j < cols; j++){
				p3[j] = origal3[cols - 1 - j];
			}
		}
		break;
	default:
		break;
	}

}

void CScanner_OpenCV::vMirrorTrans(const Mat &src, Mat &dst)
{
	CV_Assert(src.depth() == CV_8U);
	dst.create(src.rows, src.cols, src.type());

	int rows = src.rows;

	for (int i = 0; i < rows; i++)
		src.row(rows - i - 1).copyTo(dst.row(i));
}




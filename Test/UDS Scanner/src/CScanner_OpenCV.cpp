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

	//Base界面
	m_nPaperSource        = SFI_PAPERSOURCE_ADF;  //扫描模式-自动进纸器
	m_nPixelType          = TWPT_RGB; //图形类型-彩色 zhu
	m_fXResolution        = 200.0;
	m_fYResolution        = 200.0; //分辨率-200.0
	m_bDuplex             = false; //单面/双面-单面
	m_fContrast           = 0.0; //对比度-0.0
	m_fBrightness         = 0.0; //亮度-0.0
	m_fThreshold          = 128.0; //阈值-128.0 ，虚拟默认128.G6400默认230
	m_bMultifeedDetection = true; //重张检测-选中

	//Advanced界面
	m_nOrientation        = TWOR_ROT0; //zhu 纸张方向-纵向
	m_nStandardsizes      = TWSS_USLETTER; //zhu 对应ICAP_SUPPORTEDSIZES，纸张大小-TWSS_USLETTER
	m_nUnits              = TWUN_INCHES;  //zhu 单位-英寸

	m_nWidth              = 0;
	m_nHeight             = 0;

	m_nRotation           = 0; //旋转-不旋转zhu
	m_nSpiltImage         = TWSI_NONE; //zhu 分割-不分割
	m_fGamma              = 100.0; //zhu Gamma校正-默认为100
	m_bMirror             = TWMR_DISABLE; //镜像-不选中

	m_nBinarization       = TWBZ_DYNATHRESHOLD; //zhu 二值化-动态阈值
	m_bMultiStream        = false; //多流输出-不选中
	m_fSensitiveThreshold_removespots = 0.0; //去除斑点-0.0
	m_fSensitiveThreshold_colorretent = 128.0; //底色保留-128.0

	//其他图像处理
	//默认不选中
	m_fRemoveBlank        = TWRP_DISABLE; 
	m_bRemovePunch        = TWSP_DISABLE;
	m_bSharpen            = TWGM_DISABLE;
	m_bRemoveBack         = TWRB_DISABLE;
	m_bDescreen           = TWDS_DISABLE;
	m_bDenoise            = TWDN_DISABLE;
	m_bAutoCrop           = TWAC_DISABLE;

	if (m_mat_image.empty())
	{
		m_mat_image.release();
	}

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
	

	//// 获取影像的水平分辨率，以每米像素数为单位 
	//res = FreeImage_GetDotsPerMeterX( m_pDIB );
	//WORD unNewWidth  = WORD(m_nSourceWidth /(WORD)((float)res/39.37 + 0.5)* m_fXResolution);

	//// 获取影像的垂直分辨率，以每米像素数为单位 
	//res = FreeImage_GetDotsPerMeterY( m_pDIB );
	//WORD unNewHeight = WORD(m_nSourceHeight/(WORD)((float)res/39.37 + 0.5)* m_fYResolution);

	//cout << "ds: rescaling... to " << unNewWidth << " x " << unNewHeight << endl;
	////pDib = FreeImage_Rescale( m_pDIB, unNewWidth, unNewHeight, FILTER_BILINEAR);
	
	
	IplImage img= IplImage(m_mat_image);  // Mat->IplImage

	// 获取影像的宽高，都以像素为单位 
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
	matTemp.copyTo(m_mat_image);  // 深拷贝
	m_dRat = (double)unNewWidth/unNewHeight;
	


	if(m_nOrientation == TWOR_LANDSCAPE) //横向
	{		
		RotateImage(90);
		//m_mat_image = angelRotate(m_mat_image, 90);
	}

	// 旋转
	switch(m_nRotation)
	{
	case TWOR_ROT0: {
			RotateImage(0);
			break;
		}	
	case TWOR_ROT90: {
			RotateImage(-90);
			break;
    }		
	case TWOR_ROT180: {
			RotateImage(-180);
			break;
		}		
	case TWOR_ROT270: {
			RotateImage(-270);
			break;
		}		
	default: {
		break;
		}

	} 
	
	//图像镜像处理
	if(m_bMirror == TWMR_AUTO)
	{ 
		Mat mat_hMirror;
		hMirrorTrans(m_mat_image, mat_hMirror);
		mat_hMirror.copyTo(m_mat_image);
	}

	if(m_bDenoise == TWDN_AUTO) //去除噪声
	{	
		MedianSmooth();
	}

	//// 对比度和亮度
	//if (m_nPixelType != TWPT_BW)
	//{
	//	Mat matTemp;
	//	ContrastAndBright(&m_mat_image, &matTemp, (int)m_fBrightness, (int)m_fContrast);
	//	matTemp.copyTo(m_mat_image);
	//}

	//颜色
	if(m_nPixelType != TWPT_RGB)
	{
		Mat dstImage;
		// Apply bit depth color transforms
		switch(m_nPixelType)
		{
		// 黑白为：先将彩色转为灰度,再设置阈值,但是得到的图像大小与灰度相同,
		// BitsPerPixel = 8,黑白应为1.
		case TWPT_BW: {
				dstImage.create(m_mat_image.size(), m_mat_image.type());
				cvtColor(m_mat_image, dstImage, CV_BGR2GRAY);
				dstImage.copyTo(m_mat_image);
				SetThreshold((int)m_fThreshold);  
				break;
			}		
		case TWPT_GRAY: {			
				dstImage.create(m_mat_image.size(), m_mat_image.type());
				cvtColor(m_mat_image, dstImage, CV_BGR2GRAY);
				dstImage.copyTo(m_mat_image);	
				break;
			}	
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
	MatToBYTEs(m_mat_image, buffer);
}

void CScanner_OpenCV::MatToBYTEs(cv::Mat matIn, BYTE* bytesOut)
{
	int size = matIn.total() * matIn.elemSize();
	/*char buf[10];
  itoa(size, buf, 10);
	::MessageBox(g_hwndDLG,TEXT(buf),"size",MB_OK);*/
	std::memcpy(bytesOut, matIn.data, size * sizeof(BYTE));
}


void CScanner_OpenCV::RotateImage(double angle)
{
	double scale = 1; // 缩放尺度 

	if (90 == angle || 270 == angle || -90 == angle || -270 == angle)
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


void CScanner_OpenCV::MedianSmooth(void) //中值滤波
{
	IplImage in = IplImage(m_mat_image); /*Mat -> IplImage*/
	IplImage *out = cvCreateImage(cvGetSize(&in),IPL_DEPTH_8U,in.nChannels); 

	cvSmooth(&in,out,CV_MEDIAN,3,in.nChannels);  //  中值滤波
	m_mat_image = out; //IplImage -> Mat

	//::MessageBox(g_hwndDLG,TEXT("中值滤波"),MB_CAPTION,MB_OK);
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


bool CScanner_OpenCV::ContrastAndBright(Mat *pdstImage,Mat *psrcImage,
	int nBrightValue,int nContraValue)
{

	*pdstImage = Mat::zeros(psrcImage->size(), psrcImage->type());   //将g_srcImage的大小和格式赋给g_dstImage

	//三个for循环，执行运算 g_dstImage(i,j) =a*g_srcImage(i,j) + b  
	for(int y = 0; y < psrcImage->rows; y++ )  
	{  
		for(int x = 0; x < psrcImage->cols; x++ )  
		{  
			for(int c = 0; c < 3; c++ )  
			{  
				/*
				saturate_cast为了安全转换，运算结果可能超出像素取值范围（溢出），还可能是非整数（如果是浮点数的话），
				用saturate_cast对结果进行转换，以确保它为有效值。		
				*/
				pdstImage->at<Vec3b>(y,x)[c]= saturate_cast<uchar>( (nContraValue*0.01)*(psrcImage->at<Vec3b>(y,x)[c] ) + nBrightValue);  
			}  
		}  
	}  

	return true;
}

void CScanner_OpenCV::SetThreshold(int value)
{
  	
	IplImage* pImage= new IplImage(m_mat_image);  // Mat->IplImage*
	char* pImageData = pImage->imageData;
	int width = pImage->width;
	int height = pImage->height;
	int nChannels = pImage->nChannels;
	int widthStep = pImage->widthStep;

	char *RImageData,*GImageData,*BImageData ;

	for(int i=0; i < height; i++)
	{
		for(int j =0 ; j < width; j++)
		{
			RImageData = pImageData + i*widthStep + j*nChannels + 2 ;//获得R通道数据
			GImageData = pImageData + i*widthStep + j*nChannels + 1 ;//获得G通道数据
			BImageData = pImageData + i*widthStep + j*nChannels + 0 ;//获得B通道数据

			//对R通道的数据进行判断
			if((uchar)*RImageData > value)
			{
				*RImageData = (uchar)255 ;
				*GImageData = (uchar)255 ;
				*BImageData = (uchar)255 ;
			}
			else
			{
				*RImageData = 0 ;
				*GImageData = 0 ;
				*BImageData = 0 ;
			}
		}
	}

}



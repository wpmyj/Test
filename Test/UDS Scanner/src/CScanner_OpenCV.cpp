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

#include "ximage.h"  // CXImage
//#pragma comment(lib,"cximage.lib")
//#pragma comment(lib,"jasper.lib")
//#pragma comment(lib,"Jpeg.lib")
//#pragma comment(lib,"jbig.lib")
//#pragma comment(lib,"libdcr.lib")
//#pragma comment(lib,"mng.lib")
//#pragma comment(lib,"png.lib")
//#pragma comment(lib,"zlib.lib")
//#pragma comment(lib,"Tiff.lib")

extern HWND g_hwndDLG;
extern BYTE g_MuiltStream;

//using namespace std;

/**
* Environment vars to get the Xfer Count.  Create this enviroment Varable on your system to simulate the 
* number of pages sitting in the scanner waiting to be scanned.
*/
#define kGETENV_XFERCOUNT "CAP_XFERCOUNT"

#ifdef TWH_CMP_MSC
extern HINSTANCE   g_hinstance;
#endif 

#define IMAGENAME_TWAINLOGO "TWAIN_logo.png"
#define IMAGENAME_TWAINLOGO_BACK "TWAIN_logo_back.jpg"

CScanner_OpenCV::CScanner_OpenCV(void) :
	m_nScanLine(0),
	m_nDestBytesPerRow(0),
	m_nDocCount(0),
	m_nSourceWidth(0),
	m_nSourceHeight(0),
	m_i(0)
{
	memset(m_szSourceImagePath, 0, PATH_MAX);

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

	//SSNPRINTF(m_szSourceImagePath, sizeof(m_szSourceImagePath),
		//PATH_MAX, "%s%cTWAIN_logo.png", szTWAIN_DS_DIR, PATH_SEPERATOR);
	SSTRCPY(m_szSourceImagePath, sizeof(szTWAIN_DS_DIR), szTWAIN_DS_DIR);
	strcat(m_szSourceImagePath,  "\\");
	strcat(m_szSourceImagePath, IMAGENAME_TWAINLOGO);
	//::MessageBox(g_hwndDLG,m_szSourceImagePath,"路径1",MB_OK);

	// set default caps
	resetScanner();
	
}


CScanner_OpenCV::~CScanner_OpenCV(void)
{
	if (false == m_mat_image.empty())
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

	m_fRotation           = 0.0; //旋转-不旋转zhu
	m_nSpiltImage         = TWSI_NONE; //zhu 分割-不分割
	m_fGamma              = 100.0; //zhu Gamma校正-默认为100
	m_bMirror             = TWMR_DISABLE; //镜像-不选中

	m_nBinarization       = TWBZ_DYNATHRESHOLD; //zhu 二值化-动态阈值
	m_bMultiStream        = false; //多流输出-不选中
	m_fSensitiveThreshold_removespots = 0.0; //去除斑点-0.0
	m_fSensitiveThreshold_colorretent = 128.0; //底色保留-128.0

	//其他图像处理
	//默认不选中
	m_fRemoveBlank        = TWBP_DISABLE; 
	m_bRemovePunch        = TWRP_DISABLE;
	m_bSharpen            = TWSP_DISABLE;
	m_bRemoveBack         = TWRB_DISABLE;
	m_bDescreen           = TWDS_DISABLE;
	m_bDenoise            = TWDN_DISABLE;
	m_bAutoCrop           = TWAC_DISABLE;

	if (false == m_mat_image.empty())
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
	
	m_totalImageCount = BitCount(g_MuiltStream); //计算g_MuiltStream中1的个数
	
	BYTE m_tempMuilt;
	m_tempMuilt = g_MuiltStream;
	m_tempMuilt = m_tempMuilt & 0x0F;
	m_frontImageCount = BitCount(m_tempMuilt); //低四位中1的个数
	
	if(m_i < m_frontImageCount)
	{
		SSTRCPY(m_szSourceImagePath, sizeof(szTWAIN_DS_DIR), szTWAIN_DS_DIR);
		strcat(m_szSourceImagePath,  "\\");
		strcat(m_szSourceImagePath, IMAGENAME_TWAINLOGO);

		m_i++;		
	}
	//当背面选中时，换一张图片
	else
	{
		SSTRCPY(m_szSourceImagePath, sizeof(szTWAIN_DS_DIR), szTWAIN_DS_DIR);
		strcat(m_szSourceImagePath,  "\\");
		strcat(m_szSourceImagePath, IMAGENAME_TWAINLOGO_BACK);
		m_i++;
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


//该十进制数n的二进制表示中1的个数 
int CScanner_OpenCV::BitCount(BYTE n)
{
	unsigned int c = 0;
	for(c = 0; n; ++c)
	{
		n &= (n - 1) ; // 清除最低位的1 等同于n = n&(n-1);
	}
	return c;
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

	CxImage *pImage = new CxImage();
	pImage->Load(m_szSourceImagePath);
	long lXDPI = pImage->GetXDPI();
	long lYDPI = pImage->GetYDPI();
	::delete pImage;

	double dFx = (double)m_fXResolution/lXDPI;
	double dFy = (double)m_fYResolution/lYDPI;

	WORD unNewWidth = (WORD)(m_nSourceWidth * dFx); //1700
	WORD unNewHeight = (WORD)(m_nSourceHeight * dFy); //2200

	/*char buf[10];
	itoa(unNewHeight, buf, 10);
	::MessageBox(g_hwndDLG,TEXT(buf),"unNewHeight",MB_OK);*/

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
	switch((int)m_fRotation)
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
	
	// 对比度和亮度
	if (m_nPixelType != TWPT_BW)
	{
		Mat matTemp;
		ContrastAndBright(&matTemp, &m_mat_image, (int)m_fBrightness , (int)m_fContrast);
		matTemp.copyTo(m_mat_image);
	}

	//图像镜像处理
	if(m_bMirror == TWMR_AUTO)
	{ 
		Mat mat_hMirror;
		hMirrorTrans(m_mat_image, mat_hMirror);
		mat_hMirror.copyTo(m_mat_image);
	}
	
	Mat matMuilt;
	m_mat_image.copyTo(matMuilt); //m_mat_image不管多流选什么，都是彩色图

	BYTE m_byteMuilt = g_MuiltStream;
	m_byteMuilt = SwitchBYTE(m_byteMuilt);
	m_mat_image = SetMuiltStream(matMuilt, m_byteMuilt);
	
	//多流输出不使用时
	if(g_MuiltStream == 0x00)
	{
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
				m_mat_image = SetThreshold(m_mat_image, (int)m_fThreshold);  // 设置阈值
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
	}
	
	//此时m_mat_image已经是灰度图了
	if(m_bDenoise == TWDN_AUTO) //去除噪声
	{	
		MedianSmooth(m_mat_image);
	}

	//Gamma校正
	if(m_fGamma != 100.0) //zhu
	{
		/*Mat& matGamma = m_mat_image; //matGamma相当于m_mat_image的别名(绰号)，对matGamma的任何操作就是对m_mat_image的操作。
		matGamma = GammaCorrection(m_mat_image,m_fGamma/100);//取值范围为10~255，此处需要除以100，缩小取值
		m_mat_image = matGamma;*/
		
		Mat matGamma;//Mat& matGamma = m_mat_image;
		GammaCorrection(m_mat_image, matGamma, m_fGamma/100);
		/*namedWindow("5图", CV_WINDOW_AUTOSIZE); 
		imshow("5图",matGamma);*/
		m_mat_image = matGamma;
	}

	//图像分割
	if(m_nSpiltImage == TWSI_NONE)
	{
	}
	else if(m_nSpiltImage == TWSI_HORIZONTAL) //水平分割
	{
		//SpiltImage(m_mat_image, unNewWidth, unNewHeight/2); //(1700,2200/2)---(1700,1100)
		//imwrite( "C://Users//Administrator//Desktop//水平.tif", m_mat_image);
		SpiltImage(m_mat_image,2,1);
	}
	else if(m_nSpiltImage == TWSI_VERTICAL) //垂直分割
	{
		//SpiltImage(m_mat_image, unNewWidth/2, unNewHeight); //(1700/2, 2200)---(850,2200)
		//imwrite( "C://Users//Administrator//Desktop//垂直.jpg", m_mat_image);
		SpiltImage(m_mat_image,1,2);
	}

	if(m_bSharpen == TWSP_AUTO) //锐化图像
	{	
		Mat matSharpen;
		int index = FindDepth(m_mat_image);
		Laplacian( m_mat_image, matSharpen, index, 3, 1, 0, BORDER_DEFAULT ); //必须是与输入图像的深度相同
		matSharpen = m_mat_image + matSharpen;//直接相加，使拉普拉斯滤波后的图与原图有个对比
		matSharpen.copyTo(m_mat_image);
		//m_mat_image = matSharpen;
	}

	//m_mat_image = HoughLinesTransfer(m_mat_image,50,200,160);  //canny边缘检测,阈值1、2（50--200）可调 ; 霍夫变换阈值150，可调
	//m_mat_image = HoughCirclesTransfer(m_mat_image,1,200,55); // canny边缘检测阈值200,基本不变；霍夫圆变换累加器阈值100
	
	if(m_bRemovePunch == TWRP_AUTO) //去除穿孔
	{	 
		Mat matRemovepunch;
		matRemovepunch = RemovePunch(200, 22); //去除穿孔
		matRemovepunch.copyTo(m_mat_image);
	}

	if(m_bAutoCrop == TWAC_AUTO) //自动裁切与校正
	{
		Mat matAutoCrop;
		//matAutoCrop = AutoCorrect(m_mat_image); //先自动校正	
		matAutoCrop = AutoCorrect();
		matAutoCrop = RemoveBlack(matAutoCrop);
		matAutoCrop.copyTo(m_mat_image);
		//imwrite( "C://Users//Administrator//Desktop//自动校正后的图.jpg", matAutoCrop);
		//imwrite( "C://Users//Administrator//Desktop//去黑边后的图.jpg", m_mat_image);
	}

	IplImage imgTemp= IplImage(m_mat_image);  // Mat->IplImage 直接改变框架长、宽

	m_nWidth  = m_nSourceWidth = imgTemp.width;
	m_nHeight = m_nSourceHeight = imgTemp.height;

	/*
	char buf[10];
	itoa(m_nWidth, buf, 10);
	::MessageBox(g_hwndDLG,TEXT(buf),"width",MB_OK); //1700
	itoa(m_nHeight, buf, 10);
	::MessageBox(g_hwndDLG,TEXT(buf),"height",MB_OK); //2200
	*/
	/*
	if(m_nWidth <= 0 || m_nHeight <= 0)
	{
		m_nWidth  = m_nSourceWidth = imgTemp.width;
		m_nHeight = m_nSourceHeight = imgTemp.height;
	}
	else
	{
		m_nWidth = m_nSourceWidth = imgTemp.width;
		m_nHeight = m_nSourceHeight = imgTemp.height;
	}*/

	/*
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
	}*/

	// setup some convenience vars because they are used during 
	// every strip request
	m_nScanLine       = 0;


	return true;
}

BYTE CScanner_OpenCV::SwitchBYTE(const BYTE src)
{
	static BYTE tempbyte = src;
	if((tempbyte & 0x01) == 0x01)
	{
		tempbyte = tempbyte & 0xFE;
		return 0x01;
	}
	else if((tempbyte & 0x02) == 0x02)
	{
		tempbyte = tempbyte & 0xFD;
		return 0x02;
	}
	else if((tempbyte & 0x03) == 0x03)
	{
		tempbyte = tempbyte & 0xFC;
		return 0x03;
	}
	else if((tempbyte & 0x04) == 0x04)
	{
		tempbyte = tempbyte & 0xFB;
		return 0x04;
	}
	else if((tempbyte & 0x05) == 0x05)
	{
		tempbyte = tempbyte & 0xFA;
		return 0x05;
	}
	else if((tempbyte & 0x06) == 0x06)
	{
		tempbyte = tempbyte & 0xF9;
		return 0x06;
	} 
	else if((tempbyte & 0x07) == 0x07)
	{
		tempbyte = tempbyte & 0xF8;
		return 0x07;
	}
	else
	{
		return tempbyte;
	}

	if((tempbyte & 0x10) == 0x10)
	{
		tempbyte = tempbyte & 0xEF;
		return 0x10;
	}
	else if((tempbyte & 0x20) == 0x20)
	{
		tempbyte = tempbyte & 0xDF;
		return 0x20;
	}
	else if((tempbyte & 0x30) == 0x30)
	{
		tempbyte = tempbyte & 0xCF;
		return 0x30;
	}
	else if((tempbyte & 0x40) == 0x40)
	{
		tempbyte = tempbyte & 0xBF;
		return 0x40;
	}
	else if((tempbyte & 0x50) == 0x50)
	{
		tempbyte = tempbyte & 0xAF;
		return 0x50;
	}
	else if((tempbyte & 0x60) == 0x60)
	{
		tempbyte = tempbyte & 0x9F;
		return 0x60;
	}
	else if((tempbyte & 0x70) == 0x70)
	{
		tempbyte = tempbyte & 0x8F;
		return 0x70;
	}
	else
	{
		return tempbyte;
	}

}

Mat CScanner_OpenCV::SetMuiltStream(Mat src_img, BYTE muilt)
{
	Mat dst_img;
	switch(muilt)
	{
		//正面
	case 0x01:  //彩色单张
	case 0x10:
		{
			m_nPixelType = TWPT_RGB;	
			src_img.copyTo(dst_img);
		}	
		break;
	case 0x02:  //灰度单张
	case 0x20:
		{
			m_nPixelType = TWPT_GRAY;
			cvtColor(src_img, src_img, CV_BGR2GRAY);
			src_img.copyTo(dst_img);
		}		
		break;
	case 0x03:  //灰度、彩色
	case 0x30:
		{
			if(1 == m_nDocCount) //两张中的第一张
			{
				m_nPixelType = TWPT_RGB;
			}
			else if(0 == m_nDocCount) //两张中的第二张
			{
				m_nPixelType = TWPT_GRAY;
				cvtColor(src_img, src_img, CV_BGR2GRAY);//matMuilt彩色转为灰度m_mat_image
			}
			else{}
			src_img.copyTo(dst_img);
		}	
		break;

	case 0x04:  //黑白单张
	case 0x40:
		{	
			m_nPixelType = TWPT_BW;
			cvtColor(src_img, src_img, CV_BGR2GRAY);
			src_img = SetThreshold(src_img, (int)m_fThreshold);
			src_img.copyTo(dst_img);
		}	
		break;
	case 0x05:  //黑白、彩色
	case 0x50:
		{
			if(1 == m_nDocCount) //两张中的第一张
			{
				m_nPixelType = TWPT_RGB;
			}
			else if(0 == m_nDocCount) //两张中的第二张
			{
				m_nPixelType = TWPT_BW;
				cvtColor(src_img, src_img, CV_BGR2GRAY);//matMuilt彩色转为灰度m_mat_image
				threshold(src_img, src_img, m_fThreshold, 255, CV_THRESH_BINARY); //灰度变黑白
			}
			else{}
			src_img.copyTo(dst_img);
		}
		break;
	case 0x06:  //黑白、灰度
	case 0x60:
		{
			if(1 == m_nDocCount) //两张中的第一张
			{
				m_nPixelType = TWPT_GRAY;
				cvtColor(src_img, src_img, CV_BGR2GRAY);
			}
			else if(0 == m_nDocCount) //两张中的第二张
			{
				m_nPixelType = TWPT_BW;
				cvtColor(src_img, src_img, CV_BGR2GRAY);
				threshold(src_img, src_img, m_fThreshold, 255, CV_THRESH_BINARY); //灰度变黑白
			}
			else{}
			src_img.copyTo(dst_img);
		}
		break;
	case 0x07:  //黑白、灰度、彩色
	case 0x70:
		{
			if(2 == m_nDocCount) //三张中的第一张
			{
				m_nPixelType = TWPT_RGB;
			}
			else if(1 == m_nDocCount) //三张中的第二张
			{
				m_nPixelType = TWPT_GRAY;
				cvtColor(src_img, src_img, CV_BGR2GRAY);//matMuilt彩色转为灰度bwMat
			}
			else if(0 == m_nDocCount) //三张中的第三张
			{
				m_nPixelType = TWPT_BW;
				cvtColor(src_img, src_img, CV_BGR2GRAY);//matMuilt彩色转为灰度bwMat
				threshold(src_img, src_img, m_fThreshold, 255, CV_THRESH_BINARY); //灰度变黑白		
			}
			else{}
			src_img.copyTo(dst_img);
		}
		break;
/*
		//背面
	case 0x10:  //彩色单张
		{
			m_nPixelType = TWPT_RGB;
			src_img.copyTo(dst_img);
		}	
		break;
	case 0x20:  //灰度单张
		{
			m_nPixelType = TWPT_GRAY;
			cvtColor(src_img, src_img, CV_BGR2GRAY);
			src_img.copyTo(dst_img);
		}		
		break;
	case 0x30:  //灰度、彩色
		{
			if(1 == m_nDocCount) //两张中的第一张
			{
				m_nPixelType = TWPT_RGB;
			}
			else if(0 == m_nDocCount) //两张中的第二张
			{
				m_nPixelType = TWPT_GRAY;
				cvtColor(src_img, src_img, CV_BGR2GRAY);//matMuilt彩色转为灰度m_mat_image
			}
			else{}
			src_img.copyTo(dst_img);
		}	
		break;
	case 0x40:  //黑白单张
		{	
			m_nPixelType = TWPT_BW;
			cvtColor(src_img, src_img, CV_BGR2GRAY);
			src_img.copyTo(m_mat_image); //暂时直接拷贝到m_mat_image
			SetThreshold((int)m_fThreshold);		
		}	
		break;
	case 0x50:  //黑白、彩色
		{
			if(1 == m_nDocCount) //两张中的第一张
			{
				m_nPixelType = TWPT_RGB;
			}
			else if(0 == m_nDocCount) //两张中的第二张
			{
				m_nPixelType = TWPT_BW;
				cvtColor(src_img, src_img, CV_BGR2GRAY);//matMuilt彩色转为灰度m_mat_image
				threshold(src_img, src_img, m_fThreshold, 255, CV_THRESH_BINARY); //灰度变黑白
			}
			else{}
			src_img.copyTo(dst_img);
		}
		break;
	case 0x60:  //黑白、灰度
		{
			if(1 == m_nDocCount) //两张中的第一张
			{
				m_nPixelType = TWPT_GRAY;
				cvtColor(src_img, src_img, CV_BGR2GRAY);
			}
			else if(0 == m_nDocCount) //两张中的第二张
			{
				m_nPixelType = TWPT_BW;
				cvtColor(src_img, src_img, CV_BGR2GRAY);
				threshold(src_img, src_img, m_fThreshold, 255, CV_THRESH_BINARY); //灰度变黑白
			}
			else{}
			src_img.copyTo(dst_img);
		}
		break;
	case 0x70:  //黑白、灰度、彩色
		{
			if(2 == m_nDocCount) //三张中的第一张
			{
				m_nPixelType = TWPT_RGB;
			}
			else if(1 == m_nDocCount) //三张中的第二张
			{
				m_nPixelType = TWPT_GRAY;
				cvtColor(src_img, src_img, CV_BGR2GRAY);//matMuilt彩色转为灰度bwMat
			}
			else if(0 == m_nDocCount) //三张中的第三张
			{
				m_nPixelType = TWPT_BW;
				cvtColor(src_img, src_img, CV_BGR2GRAY);//matMuilt彩色转为灰度bwMat
				threshold(src_img, src_img, m_fThreshold, 255, CV_THRESH_BINARY); //灰度变黑白		
			}
			else{}
			src_img.copyTo(dst_img);
		}
		break;
	default:
		break;*/
	}
	return dst_img;
}

const int black = 10;
const int white = 250;
Mat CScanner_OpenCV::RemoveBlack(Mat src_img)
{
	Mat inputImg = src_img;
	Mat tmpMat = inputImg.clone();

	if (tmpMat.channels() != 1)
	{
		cvtColor(tmpMat, tmpMat, CV_RGB2GRAY);
	} 
	else
	{
		tmpMat.copyTo(tmpMat);
	}

	int left = 0;
	int right = tmpMat.cols; //列
	int up = 0;
	int down = tmpMat.rows; //行
	char buf[20];

	int num = 0;
	int range = tmpMat.cols / 5; //318 列
	//左侧
	for(int i = 0; i < tmpMat.rows; i++)
	{
		for(int j = 0; j < range; j++)
		{
			if((int)tmpMat.at<uchar>(i,j) <= black)
			{
				left = j;
				if((int)tmpMat.at<uchar>(i,j-1) <= black && (int)tmpMat.at<uchar>(i,j+1) >= white)
				{
					break;
				}
			}	
		}
	}
	//右侧
	for(int i = 0; i < tmpMat.rows; i++)
	{
		for(int j = tmpMat.cols-1; j >= (tmpMat.cols-range); j--)
		{
			if((int)tmpMat.at<uchar>(i,j) <= black)
			{
				right = j;
				if((int)tmpMat.at<uchar>(i,j+1) <= black && (int)tmpMat.at<uchar>(i,j-1) >= white)
				{
					break;
				}
			}	
		}
	}
	
	range = tmpMat.rows / 2; //520
	//上侧
	for(int j = 0; j < tmpMat.cols; j++)
	{
		for(int i = 0; i < range; i++)
		{
			if((int)tmpMat.at<uchar>(i,j) <= black)
			{
				up = i;
				if((int)tmpMat.at<uchar>(i-1,j) <= black && (int)tmpMat.at<uchar>(i+1,j) >= white)
				{
					break;
				}
			}	
		}
	}
	//下侧	
	for(int j = 0; j < tmpMat.cols; j++)
	{
		for(int i = tmpMat.rows-1; i >= (tmpMat.rows-range); i--)
		{
			if((int)tmpMat.at<uchar>(i,j) <= black)
			{
				down = i;
				if((int)tmpMat.at<uchar>(i+1,j) <= black && (int)tmpMat.at<uchar>(i-1,j) >= white)
				{
					break;
				}
			}	
		}
	}
	
	Rect rect(left, up, right - left, down - up);
	
	/*
	itoa(left, buf, 10);
	::MessageBox(g_hwndDLG,buf,"左侧",MB_OK);
	itoa(up, buf, 10);
	::MessageBox(g_hwndDLG,buf,"上侧",MB_OK);
	itoa(right, buf, 10);
	::MessageBox(g_hwndDLG,buf,"新右侧",MB_OK);
	itoa(down, buf, 10);
	::MessageBox(g_hwndDLG,buf,"新下侧",MB_OK);*/

	Mat imageSave = inputImg(rect);
	return imageSave;
}

//霍夫线变换
Mat CScanner_OpenCV::HoughLinesTransfer(const Mat& src_img,double threshold1, double threshold2, int threshold)
{
	Mat midImage,dstImage;//临时变量和目标图的定义  

	//【2】进行边缘检测和转化为灰度图  
	Canny(src_img, midImage, threshold1, threshold2, 3);//进行一次canny边缘检测 ，阈值1、2（50--200）可调 
	cvtColor(midImage,dstImage, CV_GRAY2BGR);//转化边缘检测后的图为灰度图  

	//【3】进行霍夫线变换  
	vector<Vec2f> lines;//定义一个矢量结构lines用于存放得到的线段矢量集合  
	HoughLines(midImage, lines, 1, CV_PI/180, threshold, 0, 0 );  //150为阈值，可调 canny边缘检测 ，阈值1、2（50--200）可调

	//【4】依次在图中绘制出每条线段  
	for( size_t i = 0; i < lines.size(); i++ )  
	{  
		float rho = lines[i][0], theta = lines[i][1];  
		Point pt1, pt2;  
		double a = cos(theta), b = sin(theta);  
		double x0 = a*rho, y0 = b*rho;  
		pt1.x = cvRound(x0 + 1000*(-b));  
		pt1.y = cvRound(y0 + 1000*(a));  
		pt2.x = cvRound(x0 - 1000*(-b));  
		pt2.y = cvRound(y0 - 1000*(a));  
		line( dstImage, pt1, pt2, Scalar(55,100,195), 1, CV_AA);   //Scalar(55,100,195)该值来确定线条颜色
	}  
	/*
	//【5】显示原始图    
	imwrite( "C://Users//Administrator//Desktop//原始图.jpg", src_img);
	//【6】边缘检测后的图   
	imwrite( "C://Users//Administrator//Desktop//边缘检测后的图.jpg", midImage);
	//【7】显示效果图    
	imwrite( "C://Users//Administrator//Desktop//霍夫变换效果图.jpg", dstImage);
	*/
	return dstImage;
}


//#define DEGREE 27
Mat CScanner_OpenCV::AutoCorrect()
{
	Mat img = imread("c:\\windows\\twain_32\\UDS General TWAIN DS\\UDS_AutoCorrect.jpg", CV_LOAD_IMAGE_UNCHANGED);
	Point center(img.cols/2, img.rows/2);

#ifdef DEGREE
	//旋转没有倾斜的图像，调试使用
	Mat rotMatS = getRotationMatrix2D(center, DEGREE, 1.0);//获取旋转矩阵
	warpAffine(img, img, rotMatS, img.size(), 1, 0, Scalar(255,255,255));//o实现坐标系仿射变换
#endif
	//img = imread("C:\\Users\\Administrator\\Desktop\\RotatedSrc.jpg", CV_LOAD_IMAGE_UNCHANGED);
	
	//获取图像的DFT尺寸转换
	Mat srcImg;

	int i = img.channels();
	if (img.channels() != 1)
	{
		cvtColor(img, srcImg, CV_RGB2GRAY);
	} 
	else
	{
		img.copyTo(srcImg);
	}

	//Expand image to an optimal size, for faster processing speed
	//Set widths of borders in four directions
	//If borderType==BORDER_CONSTANT, fill the borders with (0,0,0)
	//图像延扩iiiikkkl
	//OpenCV中的DFT采用的是快速算法，这种算法要求图像的尺寸是2、3和5的倍数时处理速度最快。
	//所以需要用getOptimalDFTSize()找到最适合的尺寸，然后用copyMakeBorder()填充多余的部分。
	//这里是让原图像和扩大的图像左上角对齐。填充的颜色如果是纯色对变换结果的影响不会很大，
	//后面寻找倾斜线的过程又会完全忽略这一点影响。
	const int nRows = srcImg.rows;
	const int nCols = srcImg.cols;
	int cRows = getOptimalDFTSize(nRows);//尺寸转换为DFT可用的Size
	int cCols = getOptimalDFTSize(nCols);
	Mat sizeConvMat;
	copyMakeBorder(srcImg, sizeConvMat, 0, cRows-nRows, 0, cCols-nCols, BORDER_CONSTANT, Scalar::all(0));;//复制图像，超过边界区域填充0
	
	//离散傅里叶变换DFT
	//DFT要分别计算实部和虚部，把要处理的图像作为输入的实部、一个全零的图像作为输入的虚部。
	//dft()输入和输出应该分别为单张图像，所以要先用merge()把实虚部图像合并，
	//分别处于图像comImg的两个通道内。计算得到的实虚部仍然保存在comImg的两个通道内。
	//通道组建立
	Mat groupMats[] = {cv::Mat_<float>(sizeConvMat), cv::Mat::zeros(sizeConvMat.size(), CV_32F)};//建立通道组
	Mat mergeMat;
	//通道合并//Merge into a double-channel image
	merge(groupMats, 2, mergeMat);//把实虚部图像合并
	//DFT变换
	//Use the same image as input and output,
	//so that the results can fit in Mat well
	dft(mergeMat, mergeMat);//离散傅里叶变换

	//获得DFT图像
	//Compute the magnitude
	//planes[0]=Re(DFT(I)), planes[1]=Im(DFT(I))
	//magnitude=sqrt(Re^2+Im^2) 傅里叶谱计算公式
	// 一般都会用幅度图像来表示图像傅里叶的变换结果（傅里叶谱）。
	//幅度的计算公式：magnitude = sqrt(Re(DFT)^2 + Im(DFT)^2)。
	//由于幅度的变化范围很大，而一般图像亮度范围只有[0,255]，容易造成一大片漆黑，只有几个点很亮。
	//所以要用log函数把数值的范围缩小。
	//分离通道
	split(mergeMat, groupMats);//分离实、虚部通道
	//计算幅值
	magnitude(groupMats[0], groupMats[1], groupMats[0]);//函数根据输入的微分处理后的x和y来计算梯度幅值
	Mat magnitudeMat = groupMats[0].clone();
	//归一化，幅值加1
	magnitudeMat += Scalar::all(1);//归一化操作，幅值加1
	//对数变换
	log(magnitudeMat, magnitudeMat); //返回指定数字的自然对数

	//Crop the spectrum
	//Width and height of magMat should be even, so that they can be divided by 2
	//-2 is 11111110 in binary system, operator & make sure width and height are always even
	//dft()直接获得的结果中，低频部分位于四角，高频部分位于中间。
	//习惯上会把图像做四等份，互相对调，使低频部分位于图像中心，也就是让频域原点位于中心。
	magnitudeMat = magnitudeMat(Rect(0, 0, magnitudeMat.cols & -2, magnitudeMat.rows & -2));


	//Normalize the magnitude to [0,1], then to[0,255]
	//虽然用log()缩小了数据范围，但仍然不能保证数值都落在[0,255]之内，
	//所以要先用normalize()规范化到[0,1]内，再用convertTo()把小数映射到[0,255]内的整数。
	//结果保存在一幅单通道图像内：
	normalize(magnitudeMat, magnitudeMat, 0, 1, CV_MINMAX);//归一化
	//图像类型转换
	magnitudeMat.convertTo(magnitudeMat, CV_8UC1, 255, 0);//图像类型转换
	magnitudeMat = magnitudeMat(Rect(0, 0, magnitudeMat.cols & -2, magnitudeMat.rows & -2));
	
	//频域中心移动
	//Rearrange the quadrants of Fourier image,
	//so that the origin is at the center of image,
	//and move the high frequency to the corners
	int cx = (magnitudeMat.cols)/2;
	int cy = magnitudeMat.rows/2;
	Mat tmp;
	//Top-Left为第一象限创建ROI
	Mat q0(magnitudeMat, Rect(0,0,cx,cy));//左上
	//Top_Right
	Mat q1(magnitudeMat,Rect(cx,0,cx,cy));//左下
	//Bottom-Left
	Mat q2(magnitudeMat,Rect(0,cy,cx,cy));//右上
	//Bottom-Right
	Mat q3(magnitudeMat,Rect(cx,cy,cx,cy));//右下
	//变换象限//交换象限:左上-->右下
	q0.copyTo(tmp);
	q3.copyTo(q0);
	tmp.copyTo(q3);
	//交换象限:右上-->左下
	q1.copyTo(tmp);
	q2.copyTo(q1);
	tmp.copyTo(q2);
	
	//倾斜角检测*************************************************
	//固定阈值二值化处理
	//Hough直线检测
	//从傅里叶谱可以明显地看到一条过中心点的倾斜直线。要想求出这个倾斜角，
	//首先要在图像上找出这条直线。一个很方便的方法是采用霍夫（Hough）变换检测直线。
	//Hough变换要求输入图像是二值的，所以要用threshold()把图像二值化。
	//Turn into binary image
	Mat binaryMagnMat;
	threshold(magnitudeMat, binaryMagnMat, 142, 255,CV_THRESH_BINARY);

	//霍夫变换
	//Find lines with Hough Transformation
	//这一部分用HoughLines()检测图像中可能存在的直线，
	//并把直线参数保存在向量组lines中，然后绘制出找到的直线。

	//两个参数GRAY_THRESH和HOUGH_VOTE需要手动指定，不同的图像需要设置不同的参数，
	//同一段文本旋转不同的角度也需要不同的参数。GRAY_THRESH越大，二值化的阈值就越高；
	//HOUGH_VOTE越大，霍夫检测的投票数就越高（需要更多的共线点来确定一条直线）。
	//说白了，如果发现二值化图像中直线附近有很多散点，就要适当提高GRAY_THRESH；
	//如果发现从二值图像的一条直线上检测到了几条角度相差很小的直线，就需要适当提高HOUGH_VOTE。
	//我们希望得到的结果时刚好检测到三条直线
	vector<Vec2f> lines;
	binaryMagnMat.convertTo(binaryMagnMat, CV_8UC1, 255, 0);
	HoughLines(binaryMagnMat, lines, 1, CV_PI/180, 100, 0, 0);//霍夫线变换
	
	Mat houghMat(binaryMagnMat.size(), CV_8UC3);
	//检测线个数
	for (size_t i=0; i<lines.size(); i++)
	{
		float rho = lines[i][0], theta = lines[i][1];
		Point pt1, pt2;
		//坐标变换生成线表达式
		double a = cos(theta), b = sin(theta);
		double x0 = a * rho, y0 = b * rho;
		pt1.x = cvRound(x0 + 1000*(-b));
		pt1.y = cvRound(y0 + 1000*(a));
		pt2.x = cvRound(x0 - 1000*(-b));
		pt2.y = cvRound(y0 - 1000*(a));
		line(houghMat, pt1, pt2, Scalar(0, 0, 255), 3, CV_AA);
	}
	
	//Find the proper angel from the three found angels
	//计算倾斜角
	//上面得到了三个角度，一个是0度，一个是90度，另一个就是我们所需要的倾斜角。
	//要把这个角找出来，而且要考虑误差。
	//检测线角度判断
	float theta = 0;
	//检测线角度判断
	for(size_t i=0; i<lines.size(); i++)
	{
		float thetaTemp = lines[i][1]*180/CV_PI;
		if (thetaTemp>0 && thetaTemp<90)
		{
			theta = thetaTemp;
			break;
		}
	}

	//Calculate the rotation angel
	//The image has to be square,
	//so that the rotation angel can be calculate right
	//由于DFT的特点，只有输入图像是正方形时，检测到的角才是文本真正旋转的角度。
	//但我们的输入图像不一定是正方形的，所以要根据图像的长宽比改变这个角度。
	//还有一个需要注意的细节，虽然HoughLines()输出的倾斜角在[0,180)之间，
	//但在[0,90]和(90,180)之间这个角的含义是不同的。
	//当倾斜角大于90度时，(180-倾斜角)才是直线相对竖直方向的偏离角度。
	//在OpenCV中，逆时针旋转，角度为正。要把图像转回去，这个角度就变成了(倾斜角-180)。
	//角度转换
	float angleT = nRows * tan(theta / 180 * CV_PI)/nCols;
	theta=atan(angleT) * 180/CV_PI;

	//仿射变换校正***********************
	//取图像中心
	//Rotate the image to recover
	//校正图像
	//最后一步，当然是把图像转回去
	//先用getRotationMatrix2D()获得一个2*3的仿射变换矩阵，
	//再把这个矩阵输入warpAffine()，做一个单纯旋转的仿射变换。
	//warpAffine()的最后一个参数Scalar(255,255,255)是把由于旋转产生的空白用白色填充。
	/*
	//灰度图
	Point2f centerPoint = Point2f(nCols/2, nRows/2);	
	Mat warpMat = getRotationMatrix2D(centerPoint, theta, 1.0);
	Mat resultImage = Mat::ones(srcImg.size(), srcImg.type());
	warpAffine(srcImg, resultImage, warpMat, resultImage.size(), 1, 0, Scalar(255,255,255));
	//warpAffine(img, resultImage, warpMat, resultImage.size()); //使用该句，程序崩溃*/

	//彩图
	Point2f centerPoint = Point2f(img.cols/2, img.rows/2);
	Mat warpMat = getRotationMatrix2D(centerPoint, theta, 1.0);
	Mat resultImage = Mat::ones(img.size(), img.type());
	warpAffine(img, resultImage, warpMat, resultImage.size(), 1, 0, Scalar(255,255,255));

	//imwrite( "C://Users//Administrator//Desktop//result.jpg", resultImage);
	//灰度、黑白图能够正常校正，但RemoveBlack崩溃
	return resultImage;
}


//霍夫圆变换
Mat CScanner_OpenCV::HoughCirclesTransfer(Mat src_img ,double dp,double threshold1, double threshold2)
{
	CvScalar scalar;
	IplImage src_img_ipl = IplImage(src_img);

	Mat midImage;//临时变量和目标图的定义

	if(src_img_ipl.nChannels == 3)
	{
		//::MessageBox(g_hwndDLG,"3通道",MB_CAPTION,MB_OK);
		//【3】转为灰度图，进行图像平滑  
		cvtColor(src_img,midImage, CV_BGR2GRAY); 
		GaussianBlur( midImage, midImage, Size(9, 9), 2, 2 ); 
	}
	else
	{
		midImage = src_img; //不变为灰度图
	}
	
	//【4】进行霍夫圆变换  
	vector<Vec3f> circles;  //存储下面三个参数: x_{c}, y_{c}, r 集合的容器来表示每个检测到的圆
	double minDist;//src_gray.rows/8: 为霍夫变换检测到的圆的圆心之间的最小距离
	minDist = midImage.rows/15;
	HoughCircles( midImage, circles, CV_HOUGH_GRADIENT,dp, minDist, threshold1, threshold2, 0, 0 );  //200,100 

	//【5】依次在图中绘制出圆  
	for( size_t i = 0; i < circles.size(); i++ )  
	{  
		Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));  
		int radius = cvRound(circles[i][2]);  
		int temp = 1;
		scalar = cvGet2D(&src_img_ipl, center.y+radius, center.x+radius); //cvGet2D(图片 y坐标，x坐标)获取 CvScalar对象,是y,x不是x,y
		
		if(radius < threshold2) //新增，半径小于阈值2时才填充
		{
			//circle( src_img, center, radius, Scalar(255,255,255), -1, 8, 0 );   //B（蓝）G（绿）R（红）；线条的类型。默认是8；0圆心坐标点和半径值的小数点位数
			circle( src_img, center, (int)(1.5*radius), scalar, -1, 8, 0 );
		}
		else //大于时，只画圆
		{
			//绘制圆心
			//circle( src_img, center, 3, Scalar(0,255,0), -1, 8, 0 ); //-1表示填充，为正数表示线条粗细
			//绘制圆轮廓 
			//circle( src_img, center, radius, Scalar(155,50,255), 3, 8, 0 ); 
		}
	}  
	
	//【6】边缘检测后的图   
	//imwrite( "C://Users//Administrator//Desktop//边缘检测后的图.jpg", midImage);
	//【7】显示效果图    
	//imwrite( "C://Users//Administrator//Desktop//霍夫变换效果图.jpg", src_img);

	return src_img;
}

Mat CScanner_OpenCV::RemovePunch(double threshold1, double threshold2)
{
	Mat src_img = imread("c:\\windows\\twain_32\\UDS General TWAIN DS\\UDS_RemovePunch.jpg", CV_LOAD_IMAGE_UNCHANGED);
	vector<Rect> rects;
	Rect rectTemp(0, 0, 3*src_img.cols/30, 3*src_img.rows/30); //宽、高只取十分之一,但rect宽高需要是3的倍数
	rects.push_back(Rect(0, 0, src_img.cols, rectTemp.height)); //上侧
	rects.push_back(Rect(0, src_img.rows-rectTemp.height, src_img.cols, rectTemp.height));	 //下侧	
	//rects.push_back(Rect(0, 0, rectTemp.width, unNewHeight)); //全部左侧
	//rects.push_back(Rect(width-rectTemp.width, 0, rectTemp.width, height)); //全部右侧
	rects.push_back(Rect(0, rectTemp.height, rectTemp.width, src_img.rows-2*rectTemp.height)); //左侧  只是中间部分
	rects.push_back(Rect(src_img.cols-rectTemp.width, rectTemp.height, rectTemp.width, src_img.rows-2*rectTemp.height)); //右侧

	vector<Mat> subImages;
	for(size_t i = 0; i < rects.size(); i++)
	{
		Mat tempImg;
		src_img(rects[i]).copyTo(tempImg);
		tempImg = HoughCirclesTransfer(tempImg,1,threshold1,threshold2);
		subImages.push_back(tempImg);	
	}

	Mat dst_img;
	src_img.copyTo(dst_img);	
	for(size_t i = 0; i < rects.size(); i++)
	{	
		IplImage IplHough = IplImage(dst_img);
		IplImage IplHoughTemp = IplImage(subImages[i]);

		cvSetImageROI(&IplHough, rects[i]);
		cvCopy(&IplHoughTemp, &IplHough);
		cvResetImageROI(&IplHough); 
	}
	//imwrite( "C://Users//Administrator//Desktop//去除穿孔后的图片.jpg", dst_img);	
	return dst_img;
}

int CScanner_OpenCV::FindDepth(const Mat& src_img)
{
	int Index,Outdex;
	Index = src_img.depth(); //图像深度
	switch(Index)
	{
		case 0:
			Outdex = CV_8U;
			break;
		case 1:
			Outdex = CV_8S;
			break;
		case 2:
			Outdex = CV_16U;
			break;
		case 3:
			Outdex = CV_16S;
			break;
		case 4:
			Outdex = CV_32S;
			break;
		case 5:
			Outdex = CV_32F;
			break;
		case 6:
			Outdex = CV_64F;
			break;
		case 7:
			Outdex = CV_USRTYPE1;
			break;
		default:
			Outdex = CV_8U;
			break;
	}
	return Outdex;
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


void CScanner_OpenCV::MedianSmooth(const Mat &src) //中值滤波
{
	IplImage in = IplImage(src); /*Mat -> IplImage*/
	IplImage *out = cvCreateImage(cvGetSize(&in),IPL_DEPTH_8U,in.nChannels); 

	cvSmooth(&in,out,CV_MEDIAN,3,in.nChannels);  //  中值滤波
	m_mat_image = out; //IplImage -> Mat
}

void CScanner_OpenCV::GammaCorrection(const Mat& src, Mat& dst, float fGamma)
{
	CV_Assert(src.data);  
	// accept only char type matrices  
  CV_Assert(src.depth() != sizeof(uchar));  
  
  // build look up table 创建Gamma查找表 
	//为 0～255 之间的每个整数执行一次预补偿操作 ,将其对应的预补偿值存入一个预先建立的
	//gamma 校正查找表(LUT:Look Up Table) ,就可以使用该表对任何像素值在 0～255 之 间的图像进行 gamma 校正
  unsigned char lut[256];  
  for( int i = 0; i < 256; i++ )  
  {  
		//先归一化，i/255,然后进行预补偿(i/255)^fGamma,最后进行反归一化(i/255)^fGamma*255
		lut[i] = saturate_cast<uchar>(pow((float)(i/255.0), fGamma) * 255.0f);  //pow(x,y)计算x的y次幂
  } 

	dst = src.clone();
	//对图像的每个像素进行查找表矫正
  const int channels = dst.channels(); 
  switch(channels)  
  {  
	case 1: 
		{  
			//::MessageBox(g_hwndDLG,TEXT("1通道"),MB_CAPTION,MB_OK);
			MatIterator_<uchar> it, end;  
			//运用迭代器访问矩阵元素
			for( it = dst.begin<uchar>(), end = dst.end<uchar>(); it != end; it++ )  
			{
				//*it = pow((float)(((*it))/255.0), fGamma) * 255.0f;  
				*it = lut[(*it)];   
			}
			break;
		}  
	case 3:  
		{ 
		//::MessageBox(g_hwndDLG,TEXT("3通道"),MB_CAPTION,MB_OK);
			MatIterator_<Vec3b> it, end;
			for( it = dst.begin<Vec3b>(), end = dst.end<Vec3b>(); it != end; it++ )  
      {  
				//(*it)[0] = pow((float)(((*it)[0])/255.0), fGamma) * 255.0;  
        //(*it)[1] = pow((float)(((*it)[1])/255.0), fGamma) * 255.0;  
        //(*it)[2] = pow((float)(((*it)[2])/255.0), fGamma) * 255.0;  
        (*it)[0] = lut[((*it)[0])];  
        (*it)[1] = lut[((*it)[1])];  
        (*it)[2] = lut[((*it)[2])];  
			}  
			break;
		}
	default:
		{
			::MessageBox(g_hwndDLG,TEXT("Gamma校正其他"),MB_CAPTION,MB_OK);
			break;
		}	
	} 
}

void CScanner_OpenCV::SpiltImage(const Mat& src_img,int m,int n)
{
	int ceil_width  = src_img.cols/n; //列  
	int ceil_height = src_img.rows/m;   //行/m

	IplImage Iplsrc = IplImage(src_img);
	IplImage *Ipldst;
	Mat matTemp;

	if(m_nDocCount == 1) //总张数暂时取2
	{
		cvSetImageROI(&Iplsrc,cvRect(0, 0, ceil_width, ceil_height)); 
		Ipldst = cvCreateImage(cvSize(ceil_width, ceil_height),  IPL_DEPTH_8U,  Iplsrc.nChannels); 

		cvCopy(&Iplsrc,Ipldst,0); 
		cvResetImageROI(&Iplsrc); 

		matTemp = Ipldst; //IplImage->Mat

		matTemp.copyTo(m_mat_image);
		cvReleaseImage(&Ipldst);  		
	}
	else if(m_nDocCount == 0)
	{
		if(m_nSpiltImage == TWSI_HORIZONTAL)
		{
			cvSetImageROI(&Iplsrc,cvRect(0, ceil_height, ceil_width, ceil_height)); 
			Ipldst = cvCreateImage(cvSize(ceil_width, ceil_height),  IPL_DEPTH_8U,  Iplsrc.nChannels); 

			cvCopy(&Iplsrc,Ipldst,0); 
			cvResetImageROI(&Iplsrc); 

			matTemp = Ipldst; //IplImage->Mat

			matTemp.copyTo(m_mat_image);
			cvReleaseImage(&Ipldst); 
		}
		else //垂直
		{
			cvSetImageROI(&Iplsrc,cvRect(ceil_width, 0, ceil_width, ceil_height)); 
			Ipldst = cvCreateImage(cvSize(ceil_width, ceil_height),  IPL_DEPTH_8U,  Iplsrc.nChannels); 

			cvCopy(&Iplsrc,Ipldst,0); 
			cvResetImageROI(&Iplsrc); 

			matTemp = Ipldst; //IplImage->Mat

			matTemp.copyTo(m_mat_image);
			cvReleaseImage(&Ipldst); 
		}	
	}
	else
	{
		char buf[10];
		itoa(m_nDocCount, buf, 10);
		::MessageBox(g_hwndDLG,TEXT(buf),"SpiltImage--m_nDocCount",MB_OK);
	}

	//namedWindow("5图", CV_WINDOW_AUTOSIZE); 
	//imshow("5图",matTemp);
	/*
	IplImage Iplsrc = IplImage(src_img);
	IplImage *Ipldst;
	Mat matTemp;
	for(int i = 0; i<m; i++) 
	{
		for(int j = 0; j<n; j++)
		{  
			cvSetImageROI(&Iplsrc,cvRect(i+j*ceil_width, j+i*ceil_height, ceil_width, ceil_height)); 
			Ipldst = cvCreateImage(cvSize(ceil_width, ceil_height),  IPL_DEPTH_8U,  Iplsrc.nChannels); 

			cvCopy(&Iplsrc,Ipldst,0); 

			::MessageBox(g_hwndDLG,TEXT("end for循环"),MB_CAPTION,MB_OK);
			cvResetImageROI(&Iplsrc); 

			matTemp = Ipldst; //IplImage->Mat
			cvReleaseImage(&Ipldst); 

			m_ceil_img.push_back(matTemp);  
			//imshow("roi_img",roi_img);  
			waitKey(0);  	
		}   
	}*/
	/*
	vector<Mat>::iterator iter;
	Mat roi_img;
	if(m_nDocCount == 2) //暂时取2
	{
		::MessageBox(g_hwndDLG,TEXT("2"),"width",MB_OK);
		roi_img = m_ceil_img[0];
	}
	else if(m_nDocCount == 1)
	{
		::MessageBox(g_hwndDLG,TEXT("1"),"width",MB_OK);
		roi_img = m_ceil_img[1];
	}
	else
	{
		::MessageBox(g_hwndDLG,TEXT("其他值"),"width",MB_OK);
		roi_img = m_mat_image;		
	}
	roi_img.copyTo(m_mat_image); */
}

/*
void CScanner_OpenCV::SpiltImage(const Mat& src, int width, int height)
{
	/*char buf[10];
	itoa(width, buf, 10);
	::MessageBox(g_hwndDLG,TEXT(buf),"width",MB_OK);

	char buff[10];
	itoa(height, buff, 10);
	::MessageBox(g_hwndDLG,TEXT(buff),"height",MB_OK);/

	IplImage Iplsrc = IplImage(src);
	IplImage *Ipldst;

	cvSetImageROI(&Iplsrc,cvRect(0,0,width,height)); 
	Ipldst = cvCreateImage(cvSize(width,height),  IPL_DEPTH_8U,  Iplsrc.nChannels);  
	cvCopy(&Iplsrc,Ipldst,0);  
	cvResetImageROI(&Iplsrc); 

	Mat matTemp = Ipldst;
	matTemp.copyTo(m_mat_image);
	//m_mat_image = matTemp;

	//namedWindow("5图", CV_WINDOW_AUTOSIZE); 
	//imshow("5图",m_mat_image);

	cvReleaseImage(&Ipldst);  	
}*/

void CScanner_OpenCV::hMirrorTrans(const Mat &src, Mat &dst)
{
	CV_Assert(src.depth() == CV_8U); //若括号中的表达式值为false，则返回一个错误信息
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
	nContraValue += 100; // 界面与OpenCV统一
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

Mat CScanner_OpenCV::SetThreshold(Mat src_img, int value)
{  	
	Mat dst_img;

	IplImage* pImage= new IplImage(src_img);  // Mat->IplImage*
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
	src_img.copyTo(dst_img);
	return dst_img;
}



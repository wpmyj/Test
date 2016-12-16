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

	m_fRotation           = 0.0; //��ת-����תzhu
	m_nSpiltImage         = TWSI_NONE; //zhu �ָ�-���ָ�
	m_fGamma              = 100.0; //zhu GammaУ��-Ĭ��Ϊ100
	m_bMirror             = TWMR_DISABLE; //����-��ѡ��

	m_nBinarization       = TWBZ_DYNATHRESHOLD; //zhu ��ֵ��-��̬��ֵ
	m_bMultiStream        = false; //�������-��ѡ��
	m_fSensitiveThreshold_removespots = 0.0; //ȥ���ߵ�-0.0
	m_fSensitiveThreshold_colorretent = 128.0; //��ɫ����-128.0

	//����ͼ����
	//Ĭ�ϲ�ѡ��
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
	matTemp.copyTo(m_mat_image);  // ���
	m_dRat = (double)unNewWidth/unNewHeight;

	if(m_nOrientation == TWOR_LANDSCAPE) //����
	{		
		RotateImage(90);
		//m_mat_image = angelRotate(m_mat_image, 90);
	}

	// ��ת
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
	
	//ͼ������
	if(m_bMirror == TWMR_AUTO)
	{ 
		Mat mat_hMirror;
		hMirrorTrans(m_mat_image, mat_hMirror);
		mat_hMirror.copyTo(m_mat_image);
	}

	// �ԱȶȺ�����
	if (m_nPixelType != TWPT_BW)
	{
		Mat matTemp;
		ContrastAndBright(&matTemp, &m_mat_image, (int)m_fBrightness , (int)m_fContrast);
		matTemp.copyTo(m_mat_image);
	}

	//��ɫ
	if(m_nPixelType != TWPT_RGB)
	{
		Mat dstImage;
		// Apply bit depth color transforms
		switch(m_nPixelType)
		{
		// �ڰ�Ϊ���Ƚ���ɫתΪ�Ҷ�,��������ֵ,���ǵõ���ͼ���С��Ҷ���ͬ,
		// BitsPerPixel = 8,�ڰ�ӦΪ1.
		case TWPT_BW: {
				dstImage.create(m_mat_image.size(), m_mat_image.type());
				cvtColor(m_mat_image, dstImage, CV_BGR2GRAY);
				dstImage.copyTo(m_mat_image);
				SetThreshold((int)m_fThreshold);  // ������ֵ
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

	if(m_bDenoise == TWDN_AUTO) //ȥ������
	{	
		MedianSmooth(m_mat_image);
	}

	//GammaУ��
	if(m_fGamma!=100.0) //zhu
	{
		/*Mat& matGamma = m_mat_image; //matGamma�൱��m_mat_image�ı���(�º�)����matGamma���κβ������Ƕ�m_mat_image�Ĳ�����
		matGamma = GammaCorrection(m_mat_image,m_fGamma/100);//ȡֵ��ΧΪ10~255���˴���Ҫ����100����Сȡֵ
		m_mat_image = matGamma;*/
		
		Mat matGamma;//Mat& matGamma = m_mat_image;
		GammaCorrection(m_mat_image, matGamma, m_fGamma/100);
		/*namedWindow("5ͼ", CV_WINDOW_AUTOSIZE); 
		imshow("5ͼ",matGamma);*/
		m_mat_image = matGamma;
	}

	//ͼ��ָ�
	if(m_nSpiltImage == TWSI_NONE)
	{
	}
	else if(m_nSpiltImage == TWSI_HORIZONTAL) //ˮƽ�ָ�
	{
		//SpiltImage(m_mat_image, unNewWidth, unNewHeight/2); //(1700,2200/2)---(1700,1100)
		//imwrite( "C://Users//Administrator//Desktop//ˮƽ.tif", m_mat_image);
		SpiltImage(m_mat_image,2,1);
	}
	else if(m_nSpiltImage == TWSI_VERTICAL) //��ֱ�ָ�
	{
		//SpiltImage(m_mat_image, unNewWidth/2, unNewHeight); //(1700/2, 2200)---(850,2200)
		//imwrite( "C://Users//Administrator//Desktop//��ֱ.jpg", m_mat_image);
		SpiltImage(m_mat_image,1,2);
	}

	if(m_bSharpen == TWSP_AUTO) //��ͼ��
	{	
		Mat matSharpen;
		int index = FindDepth(m_mat_image);
		Laplacian( m_mat_image, matSharpen, index, 3, 1, 0, BORDER_DEFAULT ); //������������ͼ��������ͬ
		matSharpen = m_mat_image + matSharpen;
		matSharpen.copyTo(m_mat_image);
		//m_mat_image = matSharpen;
	}

	//m_mat_image = HoughLinesTransfer(m_mat_image,50,200,160);  //canny��Ե���,��ֵ1��2��50--200���ɵ� ; ����任��ֵ150���ɵ�
	//m_mat_image = HoughCirclesTransfer(m_mat_image,1,200,55); // canny��Ե�����ֵ200,�������䣻����Բ�任�ۼ�����ֵ100

	
	IplImage imgTemp= IplImage(m_mat_image);  // Mat->IplImage ֱ�Ӹı��ܳ�����
	m_nWidth  = m_nSourceWidth = imgTemp.width;
	m_nHeight = m_nSourceHeight = imgTemp.height;
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
		/*
		char buf[10];
		itoa(m_nWidth, buf, 10);
		::MessageBox(g_hwndDLG,TEXT(buf),"m_nWidth",MB_OK);
		char buff[10];
		itoa(m_nHeight, buff, 10);
		::MessageBox(g_hwndDLG,TEXT(buff),"m_nHeight",MB_OK);/
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

//�����߱任
Mat CScanner_OpenCV::HoughLinesTransfer(const Mat& src_img,double threshold1, double threshold2, int threshold)
{
	Mat midImage,dstImage;//��ʱ������Ŀ��ͼ�Ķ���  

	//��2�����б�Ե����ת��Ϊ�Ҷ�ͼ  
	Canny(src_img, midImage, threshold1, threshold2, 3);//����һ��canny��Ե��� ����ֵ1��2��50--200���ɵ� 
	cvtColor(midImage,dstImage, CV_GRAY2BGR);//ת����Ե�����ͼΪ�Ҷ�ͼ  

	//��3�����л����߱任  
	vector<Vec2f> lines;//����һ��ʸ���ṹlines���ڴ�ŵõ����߶�ʸ������  
	HoughLines(midImage, lines, 1, CV_PI/180, threshold, 0, 0 );  //150Ϊ��ֵ���ɵ� canny��Ե��� ����ֵ1��2��50--200���ɵ�

	//��4��������ͼ�л��Ƴ�ÿ���߶�  
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
		line( dstImage, pt1, pt2, Scalar(55,100,195), 1, CV_AA);   //Scalar(55,100,195)��ֵ��ȷ��������ɫ
	}  
	/*
	//��5����ʾԭʼͼ    
	imwrite( "C://Users//Administrator//Desktop//ԭʼͼ.jpg", src_img);
	//��6����Ե�����ͼ   
	imwrite( "C://Users//Administrator//Desktop//��Ե�����ͼ.jpg", midImage);
	//��7����ʾЧ��ͼ    
	imwrite( "C://Users//Administrator//Desktop//����任Ч��ͼ.jpg", dstImage);
	*/
	return dstImage;
}

//����Բ�任
Mat CScanner_OpenCV::HoughCirclesTransfer(Mat src_img ,double dp,double threshold1, double threshold2)
{
	Mat midImage;//��ʱ������Ŀ��ͼ�Ķ���
	//��3��תΪ�Ҷ�ͼ������ͼ��ƽ��  
	cvtColor(src_img,midImage, CV_BGR2GRAY);//ת����Ե�����ͼΪ�Ҷ�ͼ  
	GaussianBlur( midImage, midImage, Size(9, 9), 2, 2 );  

	//��4�����л���Բ�任  
	vector<Vec3f> circles;  //�洢������������: x_{c}, y_{c}, r ���ϵ���������ʾÿ����⵽��Բ
	double minDist;//src_gray.rows/8: Ϊ����任��⵽��Բ��Բ��֮�����С����
	minDist = midImage.rows/20;
	HoughCircles( midImage, circles, CV_HOUGH_GRADIENT,dp, minDist, threshold1, threshold2, 0, 0 );  //200,100
	

	//��5��������ͼ�л��Ƴ�Բ  
	for( size_t i = 0; i < circles.size(); i++ )  
	{  
		Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));  
		int radius = cvRound(circles[i][2]);  
		//����Բ��  
		circle( src_img, center, 3, Scalar(0,255,0), -1, 8, 0 );  
		//����Բ����  
		circle( src_img, center, radius, Scalar(155,50,255), 3, 8, 0 );  
	}  
	/*
	//��6����Ե�����ͼ   
	imwrite( "C://Users//Administrator//Desktop//��Ե�����ͼ.jpg", midImage);
	//��7����ʾЧ��ͼ    
	imwrite( "C://Users//Administrator//Desktop//����任Ч��ͼ.jpg", src_img);*/

	return src_img;
}

int CScanner_OpenCV::FindDepth(const Mat& src_img)
{
	int Index,Outdex;
	Index = src_img.depth(); //ͼ�����
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
	/*char buf[10];
  itoa(size, buf, 10);
	::MessageBox(g_hwndDLG,TEXT(buf),"size",MB_OK);*/
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


void CScanner_OpenCV::MedianSmooth(const Mat &src) //��ֵ�˲�
{
	IplImage in = IplImage(src); /*Mat -> IplImage*/
	IplImage *out = cvCreateImage(cvGetSize(&in),IPL_DEPTH_8U,in.nChannels); 

	cvSmooth(&in,out,CV_MEDIAN,3,in.nChannels);  //  ��ֵ�˲�
	m_mat_image = out; //IplImage -> Mat
}

void CScanner_OpenCV::GammaCorrection(const Mat& src, Mat& dst, float fGamma)
{
	CV_Assert(src.data);  
	// accept only char type matrices  
  CV_Assert(src.depth() != sizeof(uchar));  
  
  // build look up table ����Gamma���ұ� 
	//Ϊ 0��255 ֮���ÿ������ִ��һ��Ԥ�������� ,�����Ӧ��Ԥ����ֵ����һ��Ԥ�Ƚ�����
	//gamma У�����ұ�(LUT:Look Up Table) ,�Ϳ���ʹ�øñ���κ�����ֵ�� 0��255 ֮ ���ͼ����� gamma У��
  unsigned char lut[256];  
  for( int i = 0; i < 256; i++ )  
  {  
		//�ȹ�һ����i/255,Ȼ�����Ԥ����(i/255)^fGamma,�����з���һ��(i/255)^fGamma*255
		lut[i] = saturate_cast<uchar>(pow((float)(i/255.0), fGamma) * 255.0f);  //pow(x,y)����x��y����
  } 

	dst = src.clone();
	//��ͼ���ÿ�����ؽ��в��ұ����
  const int channels = dst.channels(); 
  switch(channels)  
  {  
	case 1: 
		{  
			//::MessageBox(g_hwndDLG,TEXT("1ͨ��"),MB_CAPTION,MB_OK);
			MatIterator_<uchar> it, end;  
			//���õ��������ʾ���Ԫ��
			for( it = dst.begin<uchar>(), end = dst.end<uchar>(); it != end; it++ )  
			{
				//*it = pow((float)(((*it))/255.0), fGamma) * 255.0f;  
				*it = lut[(*it)];   
			}
			break;
		}  
	case 3:  
		{ 
		//::MessageBox(g_hwndDLG,TEXT("3ͨ��"),MB_CAPTION,MB_OK);
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
			::MessageBox(g_hwndDLG,TEXT("GammaУ������"),MB_CAPTION,MB_OK);
			break;
		}	
	} 
}

void CScanner_OpenCV::SpiltImage(const Mat& src_img,int m,int n)
{
	int ceil_width  = src_img.cols/n; //��  
	int ceil_height = src_img.rows/m;   //��/m

	IplImage Iplsrc = IplImage(src_img);
	IplImage *Ipldst;
	Mat matTemp;

	if(m_nDocCount == 1) //��������ʱȡ2
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
		else //��ֱ
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

	//namedWindow("5ͼ", CV_WINDOW_AUTOSIZE); 
	//imshow("5ͼ",matTemp);
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

			::MessageBox(g_hwndDLG,TEXT("end forѭ��"),MB_CAPTION,MB_OK);
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
	if(m_nDocCount == 2) //��ʱȡ2
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
		::MessageBox(g_hwndDLG,TEXT("����ֵ"),"width",MB_OK);
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

	//namedWindow("5ͼ", CV_WINDOW_AUTOSIZE); 
	//imshow("5ͼ",m_mat_image);

	cvReleaseImage(&Ipldst);  	
}*/

void CScanner_OpenCV::hMirrorTrans(const Mat &src, Mat &dst)
{
	CV_Assert(src.depth() == CV_8U); //�������еı��ʽֵΪfalse���򷵻�һ��������Ϣ
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
	nContraValue += 100; // ������OpenCVͳһ
	*pdstImage = Mat::zeros(psrcImage->size(), psrcImage->type());   //��g_srcImage�Ĵ�С�͸�ʽ����g_dstImage

	//����forѭ����ִ������ g_dstImage(i,j) =a*g_srcImage(i,j) + b  
	for(int y = 0; y < psrcImage->rows; y++ )  
	{  
		for(int x = 0; x < psrcImage->cols; x++ )  
		{  
			for(int c = 0; c < 3; c++ )  
			{  
				/*
				saturate_castΪ�˰�ȫת�������������ܳ�������ȡֵ��Χ����������������Ƿ�����������Ǹ������Ļ�����
				��saturate_cast�Խ������ת������ȷ����Ϊ��Чֵ��		
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
			RImageData = pImageData + i*widthStep + j*nChannels + 2 ;//���Rͨ������
			GImageData = pImageData + i*widthStep + j*nChannels + 1 ;//���Gͨ������
			BImageData = pImageData + i*widthStep + j*nChannels + 0 ;//���Bͨ������

			//��Rͨ�������ݽ����ж�
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



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
	//::MessageBox(g_hwndDLG,m_szSourceImagePath,"·��1",MB_OK);

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
	
	m_totalImageCount = BitCount(g_MuiltStream); //����g_MuiltStream��1�ĸ���
	
	BYTE m_tempMuilt;
	m_tempMuilt = g_MuiltStream;
	m_tempMuilt = m_tempMuilt & 0x0F;
	m_frontImageCount = BitCount(m_tempMuilt); //����λ��1�ĸ���
	
	if(m_i < m_frontImageCount)
	{
		SSTRCPY(m_szSourceImagePath, sizeof(szTWAIN_DS_DIR), szTWAIN_DS_DIR);
		strcat(m_szSourceImagePath,  "\\");
		strcat(m_szSourceImagePath, IMAGENAME_TWAINLOGO);

		m_i++;		
	}
	//������ѡ��ʱ����һ��ͼƬ
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


//��ʮ������n�Ķ����Ʊ�ʾ��1�ĸ��� 
int CScanner_OpenCV::BitCount(BYTE n)
{
	unsigned int c = 0;
	for(c = 0; n; ++c)
	{
		n &= (n - 1) ; // ������λ��1 ��ͬ��n = n&(n-1);
	}
	return c;
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
	
	// �ԱȶȺ�����
	if (m_nPixelType != TWPT_BW)
	{
		Mat matTemp;
		ContrastAndBright(&matTemp, &m_mat_image, (int)m_fBrightness , (int)m_fContrast);
		matTemp.copyTo(m_mat_image);
	}

	//ͼ������
	if(m_bMirror == TWMR_AUTO)
	{ 
		Mat mat_hMirror;
		hMirrorTrans(m_mat_image, mat_hMirror);
		mat_hMirror.copyTo(m_mat_image);
	}
	
	Mat matMuilt;
	m_mat_image.copyTo(matMuilt); //m_mat_image���ܶ���ѡʲô�����ǲ�ɫͼ

	BYTE m_byteMuilt = g_MuiltStream;
	m_byteMuilt = SwitchBYTE(m_byteMuilt);
	m_mat_image = SetMuiltStream(matMuilt, m_byteMuilt);
	
	//���������ʹ��ʱ
	if(g_MuiltStream == 0x00)
	{
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
				m_mat_image = SetThreshold(m_mat_image, (int)m_fThreshold);  // ������ֵ
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
	
	//��ʱm_mat_image�Ѿ��ǻҶ�ͼ��
	if(m_bDenoise == TWDN_AUTO) //ȥ������
	{	
		MedianSmooth(m_mat_image);
	}

	//GammaУ��
	if(m_fGamma != 100.0) //zhu
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
		matSharpen = m_mat_image + matSharpen;//ֱ����ӣ�ʹ������˹�˲����ͼ��ԭͼ�и��Ա�
		matSharpen.copyTo(m_mat_image);
		//m_mat_image = matSharpen;
	}

	//m_mat_image = HoughLinesTransfer(m_mat_image,50,200,160);  //canny��Ե���,��ֵ1��2��50--200���ɵ� ; ����任��ֵ150���ɵ�
	//m_mat_image = HoughCirclesTransfer(m_mat_image,1,200,55); // canny��Ե�����ֵ200,�������䣻����Բ�任�ۼ�����ֵ100
	
	if(m_bRemovePunch == TWRP_AUTO) //ȥ������
	{	 
		Mat matRemovepunch;
		matRemovepunch = RemovePunch(200, 22); //ȥ������
		matRemovepunch.copyTo(m_mat_image);
	}

	if(m_bAutoCrop == TWAC_AUTO) //�Զ�������У��
	{
		Mat matAutoCrop;
		//matAutoCrop = AutoCorrect(m_mat_image); //���Զ�У��	
		matAutoCrop = AutoCorrect();
		matAutoCrop = RemoveBlack(matAutoCrop);
		matAutoCrop.copyTo(m_mat_image);
		//imwrite( "C://Users//Administrator//Desktop//�Զ�У�����ͼ.jpg", matAutoCrop);
		//imwrite( "C://Users//Administrator//Desktop//ȥ�ڱߺ��ͼ.jpg", m_mat_image);
	}

	IplImage imgTemp= IplImage(m_mat_image);  // Mat->IplImage ֱ�Ӹı��ܳ�����

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
		//����
	case 0x01:  //��ɫ����
	case 0x10:
		{
			m_nPixelType = TWPT_RGB;	
			src_img.copyTo(dst_img);
		}	
		break;
	case 0x02:  //�Ҷȵ���
	case 0x20:
		{
			m_nPixelType = TWPT_GRAY;
			cvtColor(src_img, src_img, CV_BGR2GRAY);
			src_img.copyTo(dst_img);
		}		
		break;
	case 0x03:  //�Ҷȡ���ɫ
	case 0x30:
		{
			if(1 == m_nDocCount) //�����еĵ�һ��
			{
				m_nPixelType = TWPT_RGB;
			}
			else if(0 == m_nDocCount) //�����еĵڶ���
			{
				m_nPixelType = TWPT_GRAY;
				cvtColor(src_img, src_img, CV_BGR2GRAY);//matMuilt��ɫתΪ�Ҷ�m_mat_image
			}
			else{}
			src_img.copyTo(dst_img);
		}	
		break;

	case 0x04:  //�ڰ׵���
	case 0x40:
		{	
			m_nPixelType = TWPT_BW;
			cvtColor(src_img, src_img, CV_BGR2GRAY);
			src_img = SetThreshold(src_img, (int)m_fThreshold);
			src_img.copyTo(dst_img);
		}	
		break;
	case 0x05:  //�ڰס���ɫ
	case 0x50:
		{
			if(1 == m_nDocCount) //�����еĵ�һ��
			{
				m_nPixelType = TWPT_RGB;
			}
			else if(0 == m_nDocCount) //�����еĵڶ���
			{
				m_nPixelType = TWPT_BW;
				cvtColor(src_img, src_img, CV_BGR2GRAY);//matMuilt��ɫתΪ�Ҷ�m_mat_image
				threshold(src_img, src_img, m_fThreshold, 255, CV_THRESH_BINARY); //�Ҷȱ�ڰ�
			}
			else{}
			src_img.copyTo(dst_img);
		}
		break;
	case 0x06:  //�ڰס��Ҷ�
	case 0x60:
		{
			if(1 == m_nDocCount) //�����еĵ�һ��
			{
				m_nPixelType = TWPT_GRAY;
				cvtColor(src_img, src_img, CV_BGR2GRAY);
			}
			else if(0 == m_nDocCount) //�����еĵڶ���
			{
				m_nPixelType = TWPT_BW;
				cvtColor(src_img, src_img, CV_BGR2GRAY);
				threshold(src_img, src_img, m_fThreshold, 255, CV_THRESH_BINARY); //�Ҷȱ�ڰ�
			}
			else{}
			src_img.copyTo(dst_img);
		}
		break;
	case 0x07:  //�ڰס��Ҷȡ���ɫ
	case 0x70:
		{
			if(2 == m_nDocCount) //�����еĵ�һ��
			{
				m_nPixelType = TWPT_RGB;
			}
			else if(1 == m_nDocCount) //�����еĵڶ���
			{
				m_nPixelType = TWPT_GRAY;
				cvtColor(src_img, src_img, CV_BGR2GRAY);//matMuilt��ɫתΪ�Ҷ�bwMat
			}
			else if(0 == m_nDocCount) //�����еĵ�����
			{
				m_nPixelType = TWPT_BW;
				cvtColor(src_img, src_img, CV_BGR2GRAY);//matMuilt��ɫתΪ�Ҷ�bwMat
				threshold(src_img, src_img, m_fThreshold, 255, CV_THRESH_BINARY); //�Ҷȱ�ڰ�		
			}
			else{}
			src_img.copyTo(dst_img);
		}
		break;
/*
		//����
	case 0x10:  //��ɫ����
		{
			m_nPixelType = TWPT_RGB;
			src_img.copyTo(dst_img);
		}	
		break;
	case 0x20:  //�Ҷȵ���
		{
			m_nPixelType = TWPT_GRAY;
			cvtColor(src_img, src_img, CV_BGR2GRAY);
			src_img.copyTo(dst_img);
		}		
		break;
	case 0x30:  //�Ҷȡ���ɫ
		{
			if(1 == m_nDocCount) //�����еĵ�һ��
			{
				m_nPixelType = TWPT_RGB;
			}
			else if(0 == m_nDocCount) //�����еĵڶ���
			{
				m_nPixelType = TWPT_GRAY;
				cvtColor(src_img, src_img, CV_BGR2GRAY);//matMuilt��ɫתΪ�Ҷ�m_mat_image
			}
			else{}
			src_img.copyTo(dst_img);
		}	
		break;
	case 0x40:  //�ڰ׵���
		{	
			m_nPixelType = TWPT_BW;
			cvtColor(src_img, src_img, CV_BGR2GRAY);
			src_img.copyTo(m_mat_image); //��ʱֱ�ӿ�����m_mat_image
			SetThreshold((int)m_fThreshold);		
		}	
		break;
	case 0x50:  //�ڰס���ɫ
		{
			if(1 == m_nDocCount) //�����еĵ�һ��
			{
				m_nPixelType = TWPT_RGB;
			}
			else if(0 == m_nDocCount) //�����еĵڶ���
			{
				m_nPixelType = TWPT_BW;
				cvtColor(src_img, src_img, CV_BGR2GRAY);//matMuilt��ɫתΪ�Ҷ�m_mat_image
				threshold(src_img, src_img, m_fThreshold, 255, CV_THRESH_BINARY); //�Ҷȱ�ڰ�
			}
			else{}
			src_img.copyTo(dst_img);
		}
		break;
	case 0x60:  //�ڰס��Ҷ�
		{
			if(1 == m_nDocCount) //�����еĵ�һ��
			{
				m_nPixelType = TWPT_GRAY;
				cvtColor(src_img, src_img, CV_BGR2GRAY);
			}
			else if(0 == m_nDocCount) //�����еĵڶ���
			{
				m_nPixelType = TWPT_BW;
				cvtColor(src_img, src_img, CV_BGR2GRAY);
				threshold(src_img, src_img, m_fThreshold, 255, CV_THRESH_BINARY); //�Ҷȱ�ڰ�
			}
			else{}
			src_img.copyTo(dst_img);
		}
		break;
	case 0x70:  //�ڰס��Ҷȡ���ɫ
		{
			if(2 == m_nDocCount) //�����еĵ�һ��
			{
				m_nPixelType = TWPT_RGB;
			}
			else if(1 == m_nDocCount) //�����еĵڶ���
			{
				m_nPixelType = TWPT_GRAY;
				cvtColor(src_img, src_img, CV_BGR2GRAY);//matMuilt��ɫתΪ�Ҷ�bwMat
			}
			else if(0 == m_nDocCount) //�����еĵ�����
			{
				m_nPixelType = TWPT_BW;
				cvtColor(src_img, src_img, CV_BGR2GRAY);//matMuilt��ɫתΪ�Ҷ�bwMat
				threshold(src_img, src_img, m_fThreshold, 255, CV_THRESH_BINARY); //�Ҷȱ�ڰ�		
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
	int right = tmpMat.cols; //��
	int up = 0;
	int down = tmpMat.rows; //��
	char buf[20];

	int num = 0;
	int range = tmpMat.cols / 5; //318 ��
	//���
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
	//�Ҳ�
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
	//�ϲ�
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
	//�²�	
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
	::MessageBox(g_hwndDLG,buf,"���",MB_OK);
	itoa(up, buf, 10);
	::MessageBox(g_hwndDLG,buf,"�ϲ�",MB_OK);
	itoa(right, buf, 10);
	::MessageBox(g_hwndDLG,buf,"���Ҳ�",MB_OK);
	itoa(down, buf, 10);
	::MessageBox(g_hwndDLG,buf,"���²�",MB_OK);*/

	Mat imageSave = inputImg(rect);
	return imageSave;
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


//#define DEGREE 27
Mat CScanner_OpenCV::AutoCorrect()
{
	Mat img = imread("c:\\windows\\twain_32\\UDS General TWAIN DS\\UDS_AutoCorrect.jpg", CV_LOAD_IMAGE_UNCHANGED);
	Point center(img.cols/2, img.rows/2);

#ifdef DEGREE
	//��תû����б��ͼ�񣬵���ʹ��
	Mat rotMatS = getRotationMatrix2D(center, DEGREE, 1.0);//��ȡ��ת����
	warpAffine(img, img, rotMatS, img.size(), 1, 0, Scalar(255,255,255));//oʵ������ϵ����任
#endif
	//img = imread("C:\\Users\\Administrator\\Desktop\\RotatedSrc.jpg", CV_LOAD_IMAGE_UNCHANGED);
	
	//��ȡͼ���DFT�ߴ�ת��
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
	//ͼ������iiiikkkl
	//OpenCV�е�DFT���õ��ǿ����㷨�������㷨Ҫ��ͼ��ĳߴ���2��3��5�ı���ʱ�����ٶ���졣
	//������Ҫ��getOptimalDFTSize()�ҵ����ʺϵĳߴ磬Ȼ����copyMakeBorder()������Ĳ��֡�
	//��������ԭͼ��������ͼ�����ϽǶ��롣������ɫ����Ǵ�ɫ�Ա任�����Ӱ�첻��ܴ�
	//����Ѱ����б�ߵĹ����ֻ���ȫ������һ��Ӱ�졣
	const int nRows = srcImg.rows;
	const int nCols = srcImg.cols;
	int cRows = getOptimalDFTSize(nRows);//�ߴ�ת��ΪDFT���õ�Size
	int cCols = getOptimalDFTSize(nCols);
	Mat sizeConvMat;
	copyMakeBorder(srcImg, sizeConvMat, 0, cRows-nRows, 0, cCols-nCols, BORDER_CONSTANT, Scalar::all(0));;//����ͼ�񣬳����߽��������0
	
	//��ɢ����Ҷ�任DFT
	//DFTҪ�ֱ����ʵ�����鲿����Ҫ�����ͼ����Ϊ�����ʵ����һ��ȫ���ͼ����Ϊ������鲿��
	//dft()��������Ӧ�÷ֱ�Ϊ����ͼ������Ҫ����merge()��ʵ�鲿ͼ��ϲ���
	//�ֱ���ͼ��comImg������ͨ���ڡ�����õ���ʵ�鲿��Ȼ������comImg������ͨ���ڡ�
	//ͨ���齨��
	Mat groupMats[] = {cv::Mat_<float>(sizeConvMat), cv::Mat::zeros(sizeConvMat.size(), CV_32F)};//����ͨ����
	Mat mergeMat;
	//ͨ���ϲ�//Merge into a double-channel image
	merge(groupMats, 2, mergeMat);//��ʵ�鲿ͼ��ϲ�
	//DFT�任
	//Use the same image as input and output,
	//so that the results can fit in Mat well
	dft(mergeMat, mergeMat);//��ɢ����Ҷ�任

	//���DFTͼ��
	//Compute the magnitude
	//planes[0]=Re(DFT(I)), planes[1]=Im(DFT(I))
	//magnitude=sqrt(Re^2+Im^2) ����Ҷ�׼��㹫ʽ
	// һ�㶼���÷���ͼ������ʾͼ����Ҷ�ı任���������Ҷ�ף���
	//���ȵļ��㹫ʽ��magnitude = sqrt(Re(DFT)^2 + Im(DFT)^2)��
	//���ڷ��ȵı仯��Χ�ܴ󣬶�һ��ͼ�����ȷ�Χֻ��[0,255]���������һ��Ƭ��ڣ�ֻ�м����������
	//����Ҫ��log��������ֵ�ķ�Χ��С��
	//����ͨ��
	split(mergeMat, groupMats);//����ʵ���鲿ͨ��
	//�����ֵ
	magnitude(groupMats[0], groupMats[1], groupMats[0]);//�������������΢�ִ�����x��y�������ݶȷ�ֵ
	Mat magnitudeMat = groupMats[0].clone();
	//��һ������ֵ��1
	magnitudeMat += Scalar::all(1);//��һ����������ֵ��1
	//�����任
	log(magnitudeMat, magnitudeMat); //����ָ�����ֵ���Ȼ����

	//Crop the spectrum
	//Width and height of magMat should be even, so that they can be divided by 2
	//-2 is 11111110 in binary system, operator & make sure width and height are always even
	//dft()ֱ�ӻ�õĽ���У���Ƶ����λ���Ľǣ���Ƶ����λ���м䡣
	//ϰ���ϻ��ͼ�����ĵȷݣ�����Ե���ʹ��Ƶ����λ��ͼ�����ģ�Ҳ������Ƶ��ԭ��λ�����ġ�
	magnitudeMat = magnitudeMat(Rect(0, 0, magnitudeMat.cols & -2, magnitudeMat.rows & -2));


	//Normalize the magnitude to [0,1], then to[0,255]
	//��Ȼ��log()��С�����ݷ�Χ������Ȼ���ܱ�֤��ֵ������[0,255]֮�ڣ�
	//����Ҫ����normalize()�淶����[0,1]�ڣ�����convertTo()��С��ӳ�䵽[0,255]�ڵ�������
	//���������һ����ͨ��ͼ���ڣ�
	normalize(magnitudeMat, magnitudeMat, 0, 1, CV_MINMAX);//��һ��
	//ͼ������ת��
	magnitudeMat.convertTo(magnitudeMat, CV_8UC1, 255, 0);//ͼ������ת��
	magnitudeMat = magnitudeMat(Rect(0, 0, magnitudeMat.cols & -2, magnitudeMat.rows & -2));
	
	//Ƶ�������ƶ�
	//Rearrange the quadrants of Fourier image,
	//so that the origin is at the center of image,
	//and move the high frequency to the corners
	int cx = (magnitudeMat.cols)/2;
	int cy = magnitudeMat.rows/2;
	Mat tmp;
	//Top-LeftΪ��һ���޴���ROI
	Mat q0(magnitudeMat, Rect(0,0,cx,cy));//����
	//Top_Right
	Mat q1(magnitudeMat,Rect(cx,0,cx,cy));//����
	//Bottom-Left
	Mat q2(magnitudeMat,Rect(0,cy,cx,cy));//����
	//Bottom-Right
	Mat q3(magnitudeMat,Rect(cx,cy,cx,cy));//����
	//�任����//��������:����-->����
	q0.copyTo(tmp);
	q3.copyTo(q0);
	tmp.copyTo(q3);
	//��������:����-->����
	q1.copyTo(tmp);
	q2.copyTo(q1);
	tmp.copyTo(q2);
	
	//��б�Ǽ��*************************************************
	//�̶���ֵ��ֵ������
	//Houghֱ�߼��
	//�Ӹ���Ҷ�׿������Եؿ���һ�������ĵ����бֱ�ߡ�Ҫ����������б�ǣ�
	//����Ҫ��ͼ�����ҳ�����ֱ�ߡ�һ���ܷ���ķ����ǲ��û���Hough���任���ֱ�ߡ�
	//Hough�任Ҫ������ͼ���Ƕ�ֵ�ģ�����Ҫ��threshold()��ͼ���ֵ����
	//Turn into binary image
	Mat binaryMagnMat;
	threshold(magnitudeMat, binaryMagnMat, 142, 255,CV_THRESH_BINARY);

	//����任
	//Find lines with Hough Transformation
	//��һ������HoughLines()���ͼ���п��ܴ��ڵ�ֱ�ߣ�
	//����ֱ�߲���������������lines�У�Ȼ����Ƴ��ҵ���ֱ�ߡ�

	//��������GRAY_THRESH��HOUGH_VOTE��Ҫ�ֶ�ָ������ͬ��ͼ����Ҫ���ò�ͬ�Ĳ�����
	//ͬһ���ı���ת��ͬ�ĽǶ�Ҳ��Ҫ��ͬ�Ĳ�����GRAY_THRESHԽ�󣬶�ֵ������ֵ��Խ�ߣ�
	//HOUGH_VOTEԽ�󣬻������ͶƱ����Խ�ߣ���Ҫ����Ĺ��ߵ���ȷ��һ��ֱ�ߣ���
	//˵���ˣ�������ֶ�ֵ��ͼ����ֱ�߸����кܶ�ɢ�㣬��Ҫ�ʵ����GRAY_THRESH��
	//������ִӶ�ֵͼ���һ��ֱ���ϼ�⵽�˼����Ƕ�����С��ֱ�ߣ�����Ҫ�ʵ����HOUGH_VOTE��
	//����ϣ���õ��Ľ��ʱ�պü�⵽����ֱ��
	vector<Vec2f> lines;
	binaryMagnMat.convertTo(binaryMagnMat, CV_8UC1, 255, 0);
	HoughLines(binaryMagnMat, lines, 1, CV_PI/180, 100, 0, 0);//�����߱任
	
	Mat houghMat(binaryMagnMat.size(), CV_8UC3);
	//����߸���
	for (size_t i=0; i<lines.size(); i++)
	{
		float rho = lines[i][0], theta = lines[i][1];
		Point pt1, pt2;
		//����任�����߱��ʽ
		double a = cos(theta), b = sin(theta);
		double x0 = a * rho, y0 = b * rho;
		pt1.x = cvRound(x0 + 1000*(-b));
		pt1.y = cvRound(y0 + 1000*(a));
		pt2.x = cvRound(x0 - 1000*(-b));
		pt2.y = cvRound(y0 - 1000*(a));
		line(houghMat, pt1, pt2, Scalar(0, 0, 255), 3, CV_AA);
	}
	
	//Find the proper angel from the three found angels
	//������б��
	//����õ��������Ƕȣ�һ����0�ȣ�һ����90�ȣ���һ��������������Ҫ����б�ǡ�
	//Ҫ��������ҳ���������Ҫ������
	//����߽Ƕ��ж�
	float theta = 0;
	//����߽Ƕ��ж�
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
	//����DFT���ص㣬ֻ������ͼ����������ʱ����⵽�Ľǲ����ı�������ת�ĽǶȡ�
	//�����ǵ�����ͼ��һ���������εģ�����Ҫ����ͼ��ĳ���ȸı�����Ƕȡ�
	//����һ����Ҫע���ϸ�ڣ���ȻHoughLines()�������б����[0,180)֮�䣬
	//����[0,90]��(90,180)֮������ǵĺ����ǲ�ͬ�ġ�
	//����б�Ǵ���90��ʱ��(180-��б��)����ֱ�������ֱ�����ƫ��Ƕȡ�
	//��OpenCV�У���ʱ����ת���Ƕ�Ϊ����Ҫ��ͼ��ת��ȥ������ǶȾͱ����(��б��-180)��
	//�Ƕ�ת��
	float angleT = nRows * tan(theta / 180 * CV_PI)/nCols;
	theta=atan(angleT) * 180/CV_PI;

	//����任У��***********************
	//ȡͼ������
	//Rotate the image to recover
	//У��ͼ��
	//���һ������Ȼ�ǰ�ͼ��ת��ȥ
	//����getRotationMatrix2D()���һ��2*3�ķ���任����
	//�ٰ������������warpAffine()����һ��������ת�ķ���任��
	//warpAffine()�����һ������Scalar(255,255,255)�ǰ�������ת�����Ŀհ��ð�ɫ��䡣
	/*
	//�Ҷ�ͼ
	Point2f centerPoint = Point2f(nCols/2, nRows/2);	
	Mat warpMat = getRotationMatrix2D(centerPoint, theta, 1.0);
	Mat resultImage = Mat::ones(srcImg.size(), srcImg.type());
	warpAffine(srcImg, resultImage, warpMat, resultImage.size(), 1, 0, Scalar(255,255,255));
	//warpAffine(img, resultImage, warpMat, resultImage.size()); //ʹ�øþ䣬�������*/

	//��ͼ
	Point2f centerPoint = Point2f(img.cols/2, img.rows/2);
	Mat warpMat = getRotationMatrix2D(centerPoint, theta, 1.0);
	Mat resultImage = Mat::ones(img.size(), img.type());
	warpAffine(img, resultImage, warpMat, resultImage.size(), 1, 0, Scalar(255,255,255));

	//imwrite( "C://Users//Administrator//Desktop//result.jpg", resultImage);
	//�Ҷȡ��ڰ�ͼ�ܹ�����У������RemoveBlack����
	return resultImage;
}


//����Բ�任
Mat CScanner_OpenCV::HoughCirclesTransfer(Mat src_img ,double dp,double threshold1, double threshold2)
{
	CvScalar scalar;
	IplImage src_img_ipl = IplImage(src_img);

	Mat midImage;//��ʱ������Ŀ��ͼ�Ķ���

	if(src_img_ipl.nChannels == 3)
	{
		//::MessageBox(g_hwndDLG,"3ͨ��",MB_CAPTION,MB_OK);
		//��3��תΪ�Ҷ�ͼ������ͼ��ƽ��  
		cvtColor(src_img,midImage, CV_BGR2GRAY); 
		GaussianBlur( midImage, midImage, Size(9, 9), 2, 2 ); 
	}
	else
	{
		midImage = src_img; //����Ϊ�Ҷ�ͼ
	}
	
	//��4�����л���Բ�任  
	vector<Vec3f> circles;  //�洢������������: x_{c}, y_{c}, r ���ϵ���������ʾÿ����⵽��Բ
	double minDist;//src_gray.rows/8: Ϊ����任��⵽��Բ��Բ��֮�����С����
	minDist = midImage.rows/15;
	HoughCircles( midImage, circles, CV_HOUGH_GRADIENT,dp, minDist, threshold1, threshold2, 0, 0 );  //200,100 

	//��5��������ͼ�л��Ƴ�Բ  
	for( size_t i = 0; i < circles.size(); i++ )  
	{  
		Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));  
		int radius = cvRound(circles[i][2]);  
		int temp = 1;
		scalar = cvGet2D(&src_img_ipl, center.y+radius, center.x+radius); //cvGet2D(ͼƬ y���꣬x����)��ȡ CvScalar����,��y,x����x,y
		
		if(radius < threshold2) //�������뾶С����ֵ2ʱ�����
		{
			//circle( src_img, center, radius, Scalar(255,255,255), -1, 8, 0 );   //B������G���̣�R���죩�����������͡�Ĭ����8��0Բ�������Ͱ뾶ֵ��С����λ��
			circle( src_img, center, (int)(1.5*radius), scalar, -1, 8, 0 );
		}
		else //����ʱ��ֻ��Բ
		{
			//����Բ��
			//circle( src_img, center, 3, Scalar(0,255,0), -1, 8, 0 ); //-1��ʾ��䣬Ϊ������ʾ������ϸ
			//����Բ���� 
			//circle( src_img, center, radius, Scalar(155,50,255), 3, 8, 0 ); 
		}
	}  
	
	//��6����Ե�����ͼ   
	//imwrite( "C://Users//Administrator//Desktop//��Ե�����ͼ.jpg", midImage);
	//��7����ʾЧ��ͼ    
	//imwrite( "C://Users//Administrator//Desktop//����任Ч��ͼ.jpg", src_img);

	return src_img;
}

Mat CScanner_OpenCV::RemovePunch(double threshold1, double threshold2)
{
	Mat src_img = imread("c:\\windows\\twain_32\\UDS General TWAIN DS\\UDS_RemovePunch.jpg", CV_LOAD_IMAGE_UNCHANGED);
	vector<Rect> rects;
	Rect rectTemp(0, 0, 3*src_img.cols/30, 3*src_img.rows/30); //����ֻȡʮ��֮һ,��rect�����Ҫ��3�ı���
	rects.push_back(Rect(0, 0, src_img.cols, rectTemp.height)); //�ϲ�
	rects.push_back(Rect(0, src_img.rows-rectTemp.height, src_img.cols, rectTemp.height));	 //�²�	
	//rects.push_back(Rect(0, 0, rectTemp.width, unNewHeight)); //ȫ�����
	//rects.push_back(Rect(width-rectTemp.width, 0, rectTemp.width, height)); //ȫ���Ҳ�
	rects.push_back(Rect(0, rectTemp.height, rectTemp.width, src_img.rows-2*rectTemp.height)); //���  ֻ���м䲿��
	rects.push_back(Rect(src_img.cols-rectTemp.width, rectTemp.height, rectTemp.width, src_img.rows-2*rectTemp.height)); //�Ҳ�

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
	//imwrite( "C://Users//Administrator//Desktop//ȥ�����׺��ͼƬ.jpg", dst_img);	
	return dst_img;
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
	src_img.copyTo(dst_img);
	return dst_img;
}



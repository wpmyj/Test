#include "CScanner_OpenCV.h"
#include <time.h>

#include "public.h"
#include "uds_cap.h"

#include "ximage.h"  // CXImage

extern HWND g_hwndDLG;
/**
* Environment vars to get the Xfer Count.  Create this enviroment Varable on your system to simulate the 
* number of pages sitting in the scanner waiting to be scanned.
*/
#define kGETENV_XFERCOUNT "CAP_XFERCOUNT"

#ifdef TWH_CMP_MSC
	extern HINSTANCE   g_hinstance;
#endif 

#define IMAGENAME_FRONT _T("image\\Scanner_leaflet_front.jpg")
#define IMAGENAME_BACK _T("image\\Scanner_leaflet_back.jpg")
#define IMAGENAME_REMOVEPUNCH _T("image\\RemovePunch.jpg")
#define IMAGENAME_AUTOCORRECT _T("image\\AutoCorrect.jpg")

CScanner_OpenCV::CScanner_OpenCV(void) :
	m_nScanLine(0),
	m_nDestBytesPerRow(0),
	m_nDocCount(0),
	m_nSourceWidth(0),
	m_nSourceHeight(0),
	m_i(0)
{
	m_byte_image = NULL;
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
		//PATH_MAX, "%s%cTWAIN_logo.jpg", szTWAIN_DS_DIR, PATH_SEPERATOR);

	ChangeImage(IMAGENAME_FRONT);

	// set default caps
	resetScanner();
	
}


CScanner_OpenCV::~CScanner_OpenCV(void)
{
	if (false == m_mat_image.empty())
	{
		m_mat_image.release();
	}
	
	if(m_byte_image != NULL)
	{
		free(m_byte_image);
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
	m_nPaperSource        = TRUE;  //扫描模式-自动进纸器
	m_nPixelType          = TWPT_RGB; //图形类型-彩色 zhu
	m_fXResolution        = 200.0;
	m_fYResolution        = 200.0; //分辨率-200.0
	m_bDuplex             = false; //单面/双面-单面
	m_fContrast           = 0.0; //对比度-0.0
	m_fBrightness         = 0.0; //亮度-0.0
	m_fThreshold          = 128.0; //阈值-128.0 ，虚拟默认128.G6400默认230
	m_bMultifeedDetection = true; //重张检测-选中
	m_nMD_value           = TWMV_PAUSE;
	//Advanced界面
	m_nOrientation        = TWOR_ROT0; //zhu 纸张方向-纵向
	m_nStandardsizes      = TWSS_A4; //zhu 对应ICAP_SUPPORTEDSIZES，纸张大小-A4
	m_nUnits              = TWUN_INCHES;  //zhu 单位-英寸

	m_nWidth              = 0;
	m_nHeight             = 0;

	m_fRotation           = 0.0; //旋转-不旋转zhu
	m_nSpiltImage         = TWSI_NONE; //zhu 分割-不分割
	m_fGamma              = 100.0; //zhu Gamma校正-默认为100
	m_bMirror             = TWMR_DISABLE; //镜像-不选中

	m_nBinarization       = TWBZ_DYNATHRESHOLD; //zhu 二值化-动态阈值
	m_bMultiStream        = true; //多流输出-选中
	m_fSensitiveThreshold_removespots = 0.0; //去除斑点-0.0
	m_fSensitiveThreshold_colorretent = 128.0; //底色保留-128.0

	//其他图像处理
	//默认不选中
	m_fRemoveBlank        = TWBP_DISABLE; 
	m_bRemoveBlank        = TWRA_DISABLE;
	m_bRemovePunch        = TWRP_DISABLE;
	m_bSharpen            = TWSN_DISABLE;
	m_bRemoveBack         = TWRB_DISABLE;
	m_bDescreen           = TWDS_DISABLE;
	m_bDenoise            = TWDN_DISABLE;
	m_bAutoCrop           = TWAC_DISABLE;

	//2017新增
	m_nMaxDocCount        = 1;
	m_byteMultiValue      = 1.0; //表示只有正面彩色选中
	m_fEdgeUp             = 0.0;
	m_fEdgeDown           = 0.0;
	m_fEdgeLeft           = 0.0;
	m_fEdgeRight          = 0.0;
	m_nEdgeColor          = TWEC_BLACK;
	m_nEdgeOrientation    = TWEO_OUT;
	m_nEdgeCornercolor    = TWEN_BLACK;

	m_fXPos               = 0.0;
	m_fYPos               = 0.0;
	m_nCompress           = TWCP_NONE;
	m_fCompressValue      = 0.0;
	m_nCompressQuality    = TWCQ_BEST;

	m_bColorFlip          = FALSE;

	m_nCacheMode          = TWCM_NONE; //自动
	m_fCMAuto             = 1.0;
	m_fCMPaperNum         = 1.0;
	m_fCMMemorySize       = 1.0;

	m_bSavePower          = TRUE;
	m_fSavePowerValue     = 15.0;
	m_bOffTime            = TRUE;
	m_fOffTimeValue       = 240.0;

	m_bTurnVideo          = TRUE;
	m_nCutMethod          = TWCT_NONE;

	m_fNoiseNum           = 0.0;
	m_fNoiseRange         = 1.0;

	m_nFilterColor        = TWFL_NONE;
	m_nFilterMode         = TWFM_AUTO;
	m_fFilterLevel        = 10.0;

	m_nNoColor            = TWNC_GRAY;
	m_fColorThres         = 37.0;

	m_bOverLength           = FALSE;
	m_bSeperatePage         = FALSE;
	m_bPreFeed              = FALSE;
	m_bCodeIdentity         = FALSE;
	m_fOverLengthValue      = 0.00;
	m_fPreFeedValue         = 2.0;
	m_fWaitTime             = 0;
	m_nCodeStandard         = TWCS_1D;

	m_bIndicators           = TRUE;
	m_bNativeSave           = FALSE;
	m_bEnmergency           = FALSE;

	for(int i=0; i<6; i++)
	{
		m_fBright[i]  = m_fBrightness;
		m_fResolu[i]  = m_fXResolution;
		m_nCompre[i]  = m_nCompress;
		m_fCompVal[i] = m_fCompressValue;
		m_nCompQua[i] = m_nCompressQuality;
	}

	for(int i=0; i<4; i++)
	{
		m_fContra[i] = m_fContrast;
	}

	for(int i=0; i<2; i++)
	{
		m_nBinari[i] = m_nBinarization;
		m_fThres[i]  = m_fThreshold;
		m_fRemovespots[i] = m_fSensitiveThreshold_removespots;
	}
	
	m_byte_image = NULL;
	if (false == m_mat_image.empty())
	{
		m_mat_image.release();
	}

	return bret;
}

void CScanner_OpenCV::setSetting(CDevice_Base settings)
{
	CDevice_Base::setSetting(settings);  // 调用父类的方法
	m_nDocCount = m_nMaxDocCount;
	if ( (false == m_bMultiStream) && (true == m_bDuplex) )
	{
		m_nDocCount = 2;
	}
}

bool CScanner_OpenCV::acquireImage()
{
	if(false == m_mat_image.empty())
	{
		m_mat_image.release();
	}
	if(m_byte_image)
	{
		free(m_byte_image);
		m_byte_image = NULL;
	}	
	
	if(m_bMultiStream)
	{
		m_totalImageCount = BitCount(m_byteMultiValue); //计算1的个数
		BYTE m_tempMuilt;
		m_tempMuilt = m_byteMultiValue;
		m_tempMuilt = m_tempMuilt & 0x0F;
		m_frontImageCount = BitCount(m_tempMuilt); //低四位中1的个数

		if(m_i < m_frontImageCount)
		{
			ChangeImage(IMAGENAME_FRONT);
			m_i++;		
		}
		//当背面选中时，换一张图片
		else
		{
			ChangeImage(IMAGENAME_BACK);
			m_i++;
		}

		if (m_i > m_totalImageCount-1)
		{
			m_i = 0;
		}
	}
	else //多流未选中
	//基本界面单双面
	{
		if(m_bDuplex) //双面
		{
			if(m_nDocCount == 2) //正面
			{
				ChangeImage(IMAGENAME_FRONT);
			}
			else if(m_nDocCount == 1) //反面
			{
				ChangeImage(IMAGENAME_BACK);
			}
		} // if(m_bDuplex) end
		else // 单面
		{
			ChangeImage(IMAGENAME_FRONT);
		}
	}
	
	// get the image if it exists
	if(FALSE == FILE_EXISTS(m_szSourceImagePath))
	{
		::MessageBox(g_hwndDLG,TEXT("ds: Could not find required file: "),"UDS",MB_OK);
		return false;
	}

	m_mat_image = cv::imread(m_szSourceImagePath);

	if(true == m_mat_image.empty())
	{
		::MessageBox(g_hwndDLG,TEXT("ds: Failed - could not acquire image!"),MB_CAPTION,MB_OK);
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
	//此时进入的m_mat_image均为彩色
	IplImage img = IplImage(m_mat_image);  // Mat->IplImage

	// 获取影像的宽高，都以像素为单位 
	m_nSourceWidth   = img.width; //1100
	m_nSourceHeight  = img.height;
	WORD res = 0;

	//CxImage *pImage = new CxImage();
	//pImage->Load(m_szSourceImagePath);
	//long lXDPI = pImage->GetXDPI(); //获得图像x轴分辨率
	//long lYDPI = pImage->GetYDPI();
	//::delete pImage;

	float XReso;
	float YReso;
	float Bright;
	float Contra;
	int Compre;
	float CompVal;
	int Binari;
	float Thres;
	float Removespots;

	//多流输出
	if(m_bMultiStream)
	{
		Mat matMuilt;
		m_mat_image.copyTo(matMuilt); //m_mat_image不管多流选什么，都是彩色图
		BYTE m_byteMuilt = m_byteMultiValue;
		m_byteMuilt = SwitchBYTE(m_byteMuilt);
		m_mat_image = SetMuiltStream(matMuilt, m_byteMuilt, 
			XReso, Bright, Contra, Compre, CompVal, Binari, Thres, Removespots);
		YReso = XReso;
	}
	else//多流输出不使用时
	{
		XReso = m_fXResolution;
		YReso = m_fYResolution;
		Bright = m_fBrightness;
		Contra = m_fContrast;

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
				threshold(m_mat_image, m_mat_image, m_fThreshold, 255, THRESH_OTSU); //THRESH_BINARY
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

	long lXDPI = 200; //获得图像x轴分辨率
	long lYDPI = 200;
	
	double dFx = (double)XReso/lXDPI;
	double dFy = (double)YReso/lYDPI;

	WORD unNewWidth = (WORD)(m_nSourceWidth * dFx); //1770 ;根据DPI需要调节图像大小
	WORD unNewHeight = (WORD)(m_nSourceHeight * dFy); //2200

	cv::Mat matTemp;
	cv::resize(m_mat_image, matTemp, cv::Size(unNewWidth, unNewHeight), 0, 0);		
	matTemp.copyTo(m_mat_image);  // 深拷贝
	m_dRat = (double)unNewWidth/unNewHeight;
	
	//imwrite("C:\\Users\\Administrator\\Desktop\\resize.bmp", m_mat_image);

	if(m_nOrientation == TWOR_LANDSCAPE) //横向
	{		
		RotateImage(90);
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
	//if (m_nPixelType != TWPT_BW)
	if (m_nPixelType == TWPT_RGB) //暂时改为只对彩图处理
	{
		Mat matTemp;
		//ContrastAndBright(&matTemp, &m_mat_image, (int)m_fBrightness , (int)m_fContrast);
		ContrastAndBright(&matTemp, &m_mat_image, (int)Bright , (int)Contra);
		matTemp.copyTo(m_mat_image);
	}
	 
	//图像镜像处理
	if(m_bMirror == TWMR_AUTO)
	{ 
		Mat mat_hMirror;
		hMirrorTrans(m_mat_image, mat_hMirror);
		mat_hMirror.copyTo(m_mat_image);
	}

	//色彩翻转
	if(m_bColorFlip == TWCF_AUTO)
	{ 
		Mat mat_hColorflip;
		ColorFlip(m_mat_image, mat_hColorflip);
		mat_hColorflip.copyTo(m_mat_image);
	}

	//Gamma校正
	if(m_fGamma != 100.0) //zhu
	{
		Mat matGamma;//Mat& matGamma = m_mat_image;
		GammaCorrection(m_mat_image, matGamma, m_fGamma/100);
		m_mat_image = matGamma;
	}

	//去除噪声
	if(m_bDenoise == TWDN_AUTO) 
	{	
		Mat matDenoise;
		medianBlur(m_mat_image, matDenoise, 3);
		matDenoise.copyTo(m_mat_image);
		//MedianSmooth(m_mat_image);
	}

	//去网纹
	if(m_bDescreen == TWDS_AUTO) 
	{
		Mat matDescreen;
		m_mat_image.copyTo(matDescreen);

		//Size(5,5)模板大小，为奇数，要更精确需要自己生成模板
		//x方向方差
		//Y方向方差
		GaussianBlur(matDescreen, matDescreen, Size(5,5), 0, 0);  //  高斯滤波
		matDescreen.copyTo(m_mat_image);
	}

	//锐化
	int index = FindDepth(m_mat_image); //index为图像的深度
	//锐化图像
	if(m_bSharpen == TWSN_AUTO) 
	{	
		if(m_nPixelType != TWPT_BW)
		{
			Mat matSharpen;
		
			//USM 锐化
			float amount = 1;  
			m_mat_image.copyTo(matSharpen);
			Mat imgblur;
			Mat imgdst;
			Mat lowContrastMask;
			GaussianBlur(matSharpen, imgblur, Size(), 3, 3);  
			lowContrastMask = abs(matSharpen - imgblur)<0;  
			imgdst  = matSharpen*(1+amount) + imgblur*(-amount);  
			matSharpen.copyTo(imgdst, lowContrastMask); 
			imgdst.copyTo(m_mat_image);
		}	
	}

	//锐化
	//去除背景
	if(m_bRemoveBack == TWRB_AUTO) 
	{
		if(m_nPixelType != TWPT_BW)
		{	
			Mat matRemoveBack;	
			m_mat_image.copyTo(matRemoveBack);
		
			Mat bwMat;
			GaussianBlur(matRemoveBack, bwMat, Size(), 3, 3);  
			cvtColor(bwMat, bwMat, CV_BGR2GRAY);
			int thresoldvalue = otsu(bwMat); //171	
			threshold(bwMat, bwMat, (double)thresoldvalue, 255, THRESH_BINARY);  //OTSU也是171
			Mat dstMat(matRemoveBack.rows, matRemoveBack.cols, CV_8UC3);
			//将黑白图中的黑色像素点还原为原图中的像素点
			for(int j = 0; j < bwMat.rows; j++)
			{
				for(int i = 0; i < bwMat.cols; i++)
				{
					if((int)(bwMat.at<uchar>(j,i)) == 0)
					{
						dstMat.at<Vec3b>(j,i)[0] = matRemoveBack.at<Vec3b>(j,i)[0];
						dstMat.at<Vec3b>(j,i)[1] = matRemoveBack.at<Vec3b>(j,i)[1];
						dstMat.at<Vec3b>(j,i)[2] = matRemoveBack.at<Vec3b>(j,i)[2];
					}
					else
					{
						dstMat.at<Vec3b>(j,i)[0] = 255;
						dstMat.at<Vec3b>(j,i)[1] = 255;
						dstMat.at<Vec3b>(j,i)[2] = 255;
					}
				} //i for end
			} //j for end		
			dstMat.copyTo(m_mat_image);
		}//if end
	}

	//去除穿孔
	if(m_bRemovePunch == TWRP_AUTO) 
	{	 
		Mat matRemovepunch;
		matRemovepunch = RemovePunch(200, 30); //去除穿孔 原来22
		matRemovepunch.copyTo(m_mat_image);

		m_nWidth = m_mat_image.cols; 
		m_nHeight = m_mat_image.rows;
	}

	//自动裁切与校正
	if(m_bAutoCrop == TWAC_AUTO)
	{
		Mat matAutoCrop; 
		matAutoCrop = AutoCorrect();//先自动校正	
		matAutoCrop = RemoveBlack(matAutoCrop);
		matAutoCrop.copyTo(m_mat_image);
		//imwrite("C:\\Users\\Administrator\\Desktop\\校正图.jpg", m_mat_image);

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
				threshold(m_mat_image, m_mat_image, m_fThreshold, 255, THRESH_OTSU); //THRESH_BINARY
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

		m_nWidth = m_mat_image.cols; 
		m_nHeight = m_mat_image.rows;
	}

	if(m_nWidth <= 0 || m_nHeight <= 0)
	{
		m_nWidth  = m_nSourceWidth = m_mat_image.cols;
		m_nHeight = m_nSourceHeight = m_mat_image.rows;
	}
	else
	{
		m_nSourceWidth  = m_mat_image.cols;
		m_nSourceHeight = m_mat_image.rows;
	}

	//偏移量以及边缘扩充
	float temp[10]; 
	temp[0] = ConvertUnits(m_fXPos, m_nUnits, TWUN_PIXELS, m_fXResolution); //转换为像素		
	temp[1] = ConvertUnits(m_fEdgeUp, m_nUnits, TWUN_PIXELS, m_fXResolution); 
	temp[2] = ConvertUnits(m_fEdgeDown, m_nUnits, TWUN_PIXELS, m_fXResolution); 

	temp[3] = ConvertUnits(m_fYPos, m_nUnits, TWUN_PIXELS, m_fXResolution); 
	temp[4] = ConvertUnits(m_fEdgeLeft, m_nUnits, TWUN_PIXELS, m_fXResolution); 
	temp[5] = ConvertUnits(m_fEdgeRight, m_nUnits, TWUN_PIXELS, m_fXResolution); 
	m_nHeight = m_nHeight + (int)temp[3] + (int)temp[1] + (int)temp[2];
	m_nWidth = m_nWidth + (int)temp[0] + (int)temp[4] + (int)temp[5];

	Mat borderMat;
	copyMakeBorder(m_mat_image, borderMat, (int)temp[1], (int)temp[2]+(int)temp[3], (int)temp[4], (int)temp[5]+(int)temp[0], BORDER_CONSTANT, cv::Scalar(0,0,0)); //以常量形式扩充边界,为BORDER_CONSTANT时，最后一个是填充所需的像素的值
	borderMat.copyTo(m_mat_image);

	//图像分割
	if(m_nSpiltImage == TWSI_NONE)
	{}
	else if(m_nSpiltImage == TWSI_HORIZONTAL) //水平分割
	{
		SpiltImage(m_mat_image,2,1);
		m_nWidth = m_mat_image.cols; 
		m_nHeight = m_mat_image.rows;
	}
	else if(m_nSpiltImage == TWSI_VERTICAL) //垂直分割
	{
		SpiltImage(m_mat_image,1,2);
		m_nWidth = m_mat_image.cols; 
		m_nHeight = m_mat_image.rows;
	}
	else if(m_nSpiltImage == TWSI_DEFINED)
	{}

	switch(m_nPixelType)
	{
		case TWPT_BW:
			m_nDestBytesPerRow = BYTES_PERLINE(m_nWidth, 8); //为1时崩溃
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
	m_nScanLine = 0;

	//去除空白页
	bool status = false; //默认不是空白页
	if(m_bRemoveBlank == TWRA_AUTO)//checkbox可用
	{
		Mat matRemoveBlank;
		m_mat_image.copyTo(matRemoveBlank);
		status = RemoveBlank(matRemoveBlank, m_fRemoveBlank);
	}

	//Mat数据转为字节对齐的uchar,必须放在最后，否则其他图像处理操作无效
	Mat tempmat;
	m_mat_image.copyTo(tempmat);
	Mat2uchar(tempmat);

	if(status) //若为真，表示是空白页
	{
		return false;
	}
	else
	{
		return true;
	}
}

//OTSU最大类间方差算法
int CScanner_OpenCV::otsu(Mat image) 
{
	int width = image.cols;
	int height = image.rows;
	int pixelCount[256]; //每个像素在整幅图像中的个数 
	float pixelPro[256]; //每个像素在整幅图像中的比例
	int i, j;
	int pixelSum = width * height;
	int threshold = 0;

	uchar* data = (uchar*)image.data;

	//初始化  
	for(i = 0; i < 256; i++)
	{
		pixelCount[i] = 0;
		pixelPro[i] = 0;
	}

	//统计灰度级中每个像素在整幅图像中的个数  
	for(i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			pixelCount[data[i * image.step+ j]]++;
		}
	}

	//计算每个像素在整幅图像中的比例  
	for (i = 0; i < 256; i++)
	{
		pixelPro[i] = (float)(pixelCount[i]) / (float)(pixelSum);
	}

	//经典ostu算法,得到前景和背景的分割  
	//遍历灰度级[0,255],计算出方差最大的灰度值,为最佳阈值  
	float w0, w1, u0tmp, u1tmp, u0, u1, u, deltaTmp, deltaMax = 0;
	for (i = 0; i < 256; i++)
	{
		w0 = w1 = u0tmp = u1tmp = u0 = u1 = u = deltaTmp = 0;

		for (j = 0; j < 256; j++)
		{
			if (j <= i) //背景部分  
			{
				//以i为阈值分类，第一类总的概率  
				w0 += pixelPro[j];
				u0tmp += j * pixelPro[j];
			}
			else       //前景部分  
			{
				//以i为阈值分类，第二类总的概率  
				w1 += pixelPro[j];
				u1tmp += j * pixelPro[j];
			}
		}

		u0 = u0tmp / w0;        //第一类的平均灰度  
		u1 = u1tmp / w1;        //第二类的平均灰度  
		u = u0tmp + u1tmp;      //整幅图像的平均灰度  
		//计算类间方差  
		deltaTmp = w0 * (u0 - u)*(u0 - u) + w1 * (u1 - u)*(u1 - u);
		//找出最大类间方差以及对应的阈值  
		if (deltaTmp > deltaMax)
		{
			deltaMax = deltaTmp;
			threshold = i;
		}
	}
	//返回最佳阈值;  
	return threshold;
}

bool CScanner_OpenCV::RemoveBlank(Mat src_img, float fValue)
{
	Mat dst_img = src_img;
	int width = dst_img.cols; //列
	int height = dst_img.rows; //行

	int count = 0; //记录黑点的个数
	const float range = fValue/100; //当整个图中黑点占总像素的比例小于range时，就认为是空白页

	if(m_nPixelType == TWPT_RGB)
	{
		cvtColor(dst_img, dst_img, CV_BGR2GRAY);
		threshold(dst_img, dst_img, 0, 255, THRESH_OTSU); 	
	}
	else if(m_nPixelType == TWPT_GRAY)
	{
		threshold(dst_img, dst_img, 0, 255, THRESH_OTSU); //通过 Otsu 算法自行选择阈值，此时对于threshold的设定不在起作用
	}
	else{}

	for(int j = 0; j < width; j++)
	{
		for(int i = 0; i < height; i++)
		{
			if(dst_img.channels() == 3)
			{
				if( (int)(dst_img.at<Vec3b>(i,j)[0]) == 0  
					&& (int)(dst_img.at<Vec3b>(i,j)[1]) == 0
					&& (int)(dst_img.at<Vec3b>(i,j)[2]) == 0 )
				{
					count++;
				}
			}
			else if(dst_img.channels() == 1)
			{
				if( (int)(dst_img.at<uchar>(i,j)) == 0)
				{
					count++;
				}
			}
		}
	}

	float per = (float)count / (float)(width * height); //224170/(1770*2291)=0.05528 < 0.10000

	if(per > range)  
	{ 
		return false;
	}  
	else  
	{  
		return true;
	}  
}

BYTE CScanner_OpenCV::SwitchBYTE(const BYTE src)
{
	static BYTE tempbyte = src;

	if (0x00 == tempbyte)
	{
		tempbyte = src;
	}
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
	else if((tempbyte & 0x10) == 0x10)
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
	if((tempbyte & 0x60) == 0x60)
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
		return 0x00;
	}
}

Mat CScanner_OpenCV::SetMuiltStream(Mat src_img, BYTE muilt, float& resol, float& bright, float& contra,
	int &compre, float &compval, int &binari, float &thres, float &removespots)
{
	Mat dst_img;
	switch(muilt)
	{
		//正面
	case 0x01:  //彩色单张
		{
			m_nPixelType = TWPT_RGB;	
			src_img.copyTo(dst_img);

			resol = m_fResolu[0];
			bright = m_fBright[0];
			contra = m_fContra[0];
			compre = m_nCompre[0];
			compval = m_fCompVal[0];
		}
		break;
	case 0x10:
		{
			m_nPixelType = TWPT_RGB;	
			src_img.copyTo(dst_img);

			resol = m_fResolu[3];
			bright = m_fBright[3];
			contra = m_fContra[2];
			compre = m_nCompre[3];
			compval = m_fCompVal[3];
		}	
		break;
	
		//正面
	case 0x02:  //灰度单张
		{
			m_nPixelType = TWPT_GRAY;
			cvtColor(src_img, src_img, CV_BGR2GRAY);
			src_img.copyTo(dst_img);

			resol = m_fResolu[1];
			bright = m_fBright[1];
			contra = m_fContra[1];
			compre = m_nCompre[1];
			compval = m_fCompVal[1];
		}		
		break;
	case 0x20:
		{
			m_nPixelType = TWPT_GRAY;
			cvtColor(src_img, src_img, CV_BGR2GRAY);
			src_img.copyTo(dst_img);

			resol = m_fResolu[4];
			bright = m_fBright[4];
			contra = m_fContra[3];
			compre = m_nCompre[4];
			compval = m_fCompVal[4];
		}		
		break;

		//正面
	case 0x03:  //灰度、彩色
		{
			if(1 == m_nDocCount) //两张中的第一张
			{
				m_nPixelType = TWPT_RGB;

				resol = m_fResolu[0];
				bright = m_fBright[0];
				contra = m_fContra[0];
				compre = m_nCompre[0];
				compval = m_fCompVal[0];
			}
			else if(0 == m_nDocCount) //两张中的第二张
			{
				m_nPixelType = TWPT_GRAY;
				cvtColor(src_img, src_img, CV_BGR2GRAY);//matMuilt彩色转为灰度m_mat_image

				resol = m_fResolu[1];
				bright = m_fBright[1];
				contra = m_fContra[1];
				compre = m_nCompre[1];
				compval = m_fCompVal[1];
			}
			else{}
			src_img.copyTo(dst_img);
		}	
		break;
	case 0x30: //背面
		{
			if(1 == m_nDocCount) //两张中的第一张
			{
				m_nPixelType = TWPT_RGB;

				resol = m_fResolu[3];
				bright = m_fBright[3];
				contra = m_fContra[2];
				compre = m_nCompre[3];
				compval = m_fCompVal[3];
			}
			else if(0 == m_nDocCount) //两张中的第二张
			{
				m_nPixelType = TWPT_GRAY;
				cvtColor(src_img, src_img, CV_BGR2GRAY);//matMuilt彩色转为灰度m_mat_image

				resol = m_fResolu[4];
				bright = m_fBright[4];
				contra = m_fContra[3];
				compre = m_nCompre[4];
				compval = m_fCompVal[4];
			}
			else{}
			src_img.copyTo(dst_img);
		}	
		break;

		//正面
	case 0x04:  //黑白单张
		{	
			m_nPixelType = TWPT_BW;
			cvtColor(src_img, src_img, CV_BGR2GRAY);
			threshold(src_img, src_img, 0, 255, THRESH_OTSU);
			src_img.copyTo(dst_img);

			resol = m_fResolu[2];
			bright = m_fBright[2];
			//contra = m_fContra[2]; 黑白无对比度
			compre = m_nCompre[2];
			compval = m_fCompVal[2];

			binari = m_nBinari[0];
			thres = m_fThres[0];
			removespots = m_fRemovespots[0];
		}	
		break;
	case 0x40: //背面
		{	
			m_nPixelType = TWPT_BW;
			cvtColor(src_img, src_img, CV_BGR2GRAY);
			threshold(src_img, src_img, 0, 255, THRESH_OTSU);
			src_img.copyTo(dst_img);

			resol = m_fResolu[5];
			bright = m_fBright[5];
			//contra = m_fContra[5];
			compre = m_nCompre[5];
			compval = m_fCompVal[5];

			binari = m_nBinari[1];
			thres = m_fThres[1];
			removespots = m_fRemovespots[1];
		}	
		break;

		//正面
	case 0x05:  //黑白、彩色 z正面
		{
			if(1 == m_nDocCount) //两张中的第一张
			{
				m_nPixelType = TWPT_RGB;

				resol = m_fResolu[0];
				bright = m_fBright[0];
				contra = m_fContra[0];
				compre = m_nCompre[0];
				compval = m_fCompVal[0];
			}
			else if(0 == m_nDocCount) //两张中的第二张
			{
				m_nPixelType = TWPT_BW;
				cvtColor(src_img, src_img, CV_BGR2GRAY);//matMuilt彩色转为灰度m_mat_image
				threshold(src_img, src_img, m_fThreshold, 255, CV_THRESH_BINARY); //灰度变黑白

				resol = m_fResolu[2];
				bright = m_fBright[2];
				//contra = m_fContra[2];
				compre = m_nCompre[2];
				compval = m_fCompVal[2];

				binari = m_nBinari[0];
				thres = m_fThres[0];
				removespots = m_fRemovespots[0];
			}
			else{}
			src_img.copyTo(dst_img);
		}
		break;
	case 0x50:
		{
			if(1 == m_nDocCount) //两张中的第一张
			{
				m_nPixelType = TWPT_RGB;

				resol = m_fResolu[3];
				bright = m_fBright[3];
				contra = m_fContra[2];
				compre = m_nCompre[3];
				compval = m_fCompVal[3];
			}
			else if(0 == m_nDocCount) //两张中的第二张
			{
				m_nPixelType = TWPT_BW;
				cvtColor(src_img, src_img, CV_BGR2GRAY);//matMuilt彩色转为灰度m_mat_image
				threshold(src_img, src_img, m_fThreshold, 255, CV_THRESH_BINARY); //灰度变黑白

				resol = m_fResolu[5];
				bright = m_fBright[5];
				//contra = m_fContra[5];
				compre = m_nCompre[5];
				compval = m_fCompVal[5];

				binari = m_nBinari[1];
				thres = m_fThres[1];
				removespots = m_fRemovespots[1];
			}
			else{}
			src_img.copyTo(dst_img);
		}
		break;

		//正面
	case 0x06:  //黑白、灰度 正面
		{
			if(1 == m_nDocCount) //两张中的第一张
			{
				m_nPixelType = TWPT_GRAY;
				cvtColor(src_img, src_img, CV_BGR2GRAY);

				resol = m_fResolu[1];
				bright = m_fBright[1];
				contra = m_fContra[1];
				compre = m_nCompre[1];
				compval = m_fCompVal[1];
			}
			else if(0 == m_nDocCount) //两张中的第二张
			{
				m_nPixelType = TWPT_BW;
				cvtColor(src_img, src_img, CV_BGR2GRAY);
				threshold(src_img, src_img, m_fThreshold, 255, CV_THRESH_BINARY); //灰度变黑白

				resol = m_fResolu[2];
				bright = m_fBright[2];
				//contra = m_fContra[2];
				compre = m_nCompre[2];
				compval = m_fCompVal[2];

				binari = m_nBinari[0];
				thres = m_fThres[0];
				removespots = m_fRemovespots[0];
			}
			else{}
			src_img.copyTo(dst_img);
		}
		break;
	case 0x60:
		{
			if(1 == m_nDocCount) //两张中的第一张
			{
				m_nPixelType = TWPT_GRAY;
				cvtColor(src_img, src_img, CV_BGR2GRAY);

				resol = m_fResolu[4];
				bright = m_fBright[4];
				contra = m_fContra[3];
				compre = m_nCompre[4];
				compval = m_fCompVal[4];
			}
			else if(0 == m_nDocCount) //两张中的第二张
			{
				m_nPixelType = TWPT_BW;
				cvtColor(src_img, src_img, CV_BGR2GRAY);
				threshold(src_img, src_img, m_fThreshold, 255, CV_THRESH_BINARY); //灰度变黑白

				resol = m_fResolu[5];
				bright = m_fBright[5];
				//contra = m_fContra[5];
				compre = m_nCompre[5];
				compval = m_fCompVal[5];

				binari = m_nBinari[1];
				thres = m_fThres[1];
				removespots = m_fRemovespots[1];
			}
			else{}
			src_img.copyTo(dst_img);
		}
		break;

		//正面
	case 0x07:  //黑白、灰度、彩色 正面
		{
			if(2 == m_nDocCount) //三张中的第一张
			{
				m_nPixelType = TWPT_RGB;

				resol = m_fResolu[0];
				bright = m_fBright[0];
				contra = m_fContra[0];
				compre = m_nCompre[0];
				compval = m_fCompVal[0];
			}
			else if(1 == m_nDocCount) //三张中的第二张
			{
				m_nPixelType = TWPT_GRAY;
				cvtColor(src_img, src_img, CV_BGR2GRAY);//matMuilt彩色转为灰度bwMat

				resol = m_fResolu[1];
				bright = m_fBright[1];
				contra = m_fContra[1];
				compre = m_nCompre[1];
				compval = m_fCompVal[1];
			}
			else if(0 == m_nDocCount) //三张中的第三张
			{
				m_nPixelType = TWPT_BW;
				cvtColor(src_img, src_img, CV_BGR2GRAY);//matMuilt彩色转为灰度bwMat
				threshold(src_img, src_img, m_fThreshold, 255, CV_THRESH_BINARY); //灰度变黑白

				resol = m_fResolu[2];
				bright = m_fBright[2];
				//contra = m_fContra[2];
				compre = m_nCompre[2];
				compval = m_fCompVal[2];

				binari = m_nBinari[0];
				thres = m_fThres[0];
				removespots = m_fRemovespots[0];
			}
			else{}
			src_img.copyTo(dst_img);
		}
		break;
	case 0x70:
		{
			if(2 == m_nDocCount) //三张中的第一张
			{
				m_nPixelType = TWPT_RGB;

				resol = m_fResolu[3];
				bright = m_fBright[3];
				contra = m_fContra[2];
				compre = m_nCompre[3];
				compval = m_fCompVal[3];
			}
			else if(1 == m_nDocCount) //三张中的第二张
			{
				m_nPixelType = TWPT_GRAY;
				cvtColor(src_img, src_img, CV_BGR2GRAY);//matMuilt彩色转为灰度bwMat

				resol = m_fResolu[4];
				bright = m_fBright[4];
				contra = m_fContra[3];
				compre = m_nCompre[4];
				compval = m_fCompVal[4];
			}
			else if(0 == m_nDocCount) //三张中的第三张
			{
				m_nPixelType = TWPT_BW;
				cvtColor(src_img, src_img, CV_BGR2GRAY);//matMuilt彩色转为灰度bwMat
				threshold(src_img, src_img, m_fThreshold, 255, CV_THRESH_BINARY); //灰度变黑白

				resol = m_fResolu[5];
				bright = m_fBright[5];
				//contra = m_fContra[5];
				compre = m_nCompre[5];
				compval = m_fCompVal[5];

				binari = m_nBinari[1];
				thres = m_fThres[1];
				removespots = m_fRemovespots[1];
			}
			else{}
			src_img.copyTo(dst_img);
		}
		break;
	}

	return dst_img;
}


Mat CScanner_OpenCV::RemoveBlack(Mat src_img)
{
	const int black = 10;
	const int white = 250;

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
	threshold(tmpMat, tmpMat, 128, 255, CV_THRESH_OTSU);
	
	int width = tmpMat.cols;
	int height = tmpMat.rows;
	
	int left = 0;
	int right = width; //列 宽2323
	int up = 0;
	int down = height; //行 高2808

	int i = 0, j = 0;
	//上侧
	for(i = 0; i < height; i++)
	{
		for(j = 1; j < width/2; j++)
		{
			if((int)tmpMat.at<uchar>(i,j) <= black && 
					(int)tmpMat.at<uchar>(i,j-1) <= black && (int)tmpMat.at<uchar>(i,j+1) >= white)
			{
				break;
			}	
		}
		if((int)tmpMat.at<uchar>(i,j) <= black && 
			(int)tmpMat.at<uchar>(i,j-1) <= black && (int)tmpMat.at<uchar>(i,j+1) >= white)
		{
			up = i; 
			break;
		}	
	}
	//左侧
	for(j = 0; j < width; j++)
	{
		for(i = 1; i < height/2; i++)
		{
			if((int)tmpMat.at<uchar>(i,j) <= black && 
				(int)tmpMat.at<uchar>(i-1,j) <= black && (int)tmpMat.at<uchar>(i+1,j) >= white)
			{
				break;
			}	
		}
		if((int)tmpMat.at<uchar>(i,j) <= black && 
			(int)tmpMat.at<uchar>(i-1,j) <= black && (int)tmpMat.at<uchar>(i+1,j) >= white)
		{
			left = j; 
			break;
		}	
	}

	//下侧
	for(i = height-2; i >= height/2; i--)
	{
		for(j = width-2; j >= width/2; j--)
		{
			if((int)tmpMat.at<uchar>(i,j) <= black
				 &&(int)tmpMat.at<uchar>(i,j+1) <= black && (int)tmpMat.at<uchar>(i,j-1) >= white)
			{
				break;
			}	
		}

		if((int)tmpMat.at<uchar>(i,j) <= black
			&& (int)tmpMat.at<uchar>(i,j+1) <= black && (int)tmpMat.at<uchar>(i,j-1) >= white)
		{
			down = i;	
			break;
		}	
	}
	//右侧
	for(j = width-2; j >= width/2; j--)
	{
		for(i = height-2; i >= height/2; i--)
		{
			if((int)tmpMat.at<uchar>(i,j) <= black
				&&(int)tmpMat.at<uchar>(i+1,j) <= black && (int)tmpMat.at<uchar>(i-1,j) >= white)
			{
				break;
			}	
		}

		if((int)tmpMat.at<uchar>(i,j) <= black
			&& (int)tmpMat.at<uchar>(i+1,j) <= black && (int)tmpMat.at<uchar>(i-1,j) >= white)
		{
			right = j; 	
			break;
		}	
	}
	
	Rect rect(left, up, right-left, down-up); //(856.1030)
	
	Mat imageSave = inputImg(rect);
	return imageSave;
}

Mat CScanner_OpenCV::AutoCorrect()
{
	ChangeImage(IMAGENAME_AUTOCORRECT);
	Mat img_src = imread(m_szSourceImagePath, CV_LOAD_IMAGE_UNCHANGED);

	Mat srcImage;
	img_src.copyTo(srcImage);

	Mat midImage,dstImage;
	Canny(srcImage, midImage, 200, 150, 3);
	cvtColor(midImage, dstImage, CV_GRAY2BGR);//转化边缘检测后的图为彩图，但实际看起来仍然是灰度图

	//【3】进行概率霍夫线变换
	vector<Vec4i> lines;//定义一个矢量结构lines用于存放得到的线段矢量集合
	HoughLinesP(midImage, lines, 1, CV_PI/180, 20, min(srcImage.cols/2,srcImage.rows/2), 50);

	//【4】依次在图中绘制出每条线段
	Vec4i maxline = lines[0];
	float dx = 0.0;
	float dy = 0.0;
	for( size_t i = 0; i < lines.size(); i++ )
	{
		Vec4i l = lines[i];
		//每一条线由具有四个元素的矢量(x_1,y_1, x_2, y_2）表示，
		//其中，(x_1, y_1)和(x_2, y_2) 是是每个检测到的线段的起点和结束点。
		float mi = (l[2]-l[0]) * (l[2]-l[0]) + (l[3]-l[1]) * (l[3]-l[1]);
		int xi = sqrt(mi);

		float maxi = (maxline[2]-maxline[0])*(maxline[2]-maxline[0]) + (maxline[3]-maxline[1]) * (maxline[3]-maxline[1]);
		int maxsqrt = sqrt(maxi);

		if(xi > maxsqrt)
		{
			maxline = lines[i];
		}
	}
	line(dstImage, Point(maxline[0], maxline[1]), Point(maxline[2], maxline[3]), Scalar(0,0,255), 1, CV_AA);

	//imwrite("C:\\Users\\Administrator\\Desktop\\dstImage.jpg", dstImage);

	bool mark = false;

	dx = (float)(maxline[2]-maxline[0]);
	dy = (float)(maxline[3]-maxline[1]);
	double temp;
	double theta;
	if(dy > 0)
	{//负表示顺时针，正表示逆时针
		if(dx > dy)
		{	//右斜短边：上边
			temp = dx/dy;
			theta = CV_PI/2.0 - atan(temp);
		}
		else
		{ //左斜长边：右边
			temp = dy/dx; 		
			theta = atan(temp); 
			mark = true;
		}
	}
	else
	{
		if(dx > fabs(dy))
		{//左斜短边：上边
			temp = dx/fabs(dy);
			theta = atan(temp);
			mark = true;
		}
		else
		{//右斜长边：右边
			temp = fabs(dy)/dx;
			theta = CV_PI/2.0 - atan(temp);
		}
	}
	theta = theta/CV_PI*180; //弧度转角度

	int nRows = srcImage.rows; //高11420
	int nCols = srcImage.cols; //宽4202

	//仿射变换校正***********************
	float   degree = theta;
	double  angle  = degree/180*CV_PI; //角度转弧度
	double  a = sin(angle), b = cos(angle);
	int     m_width_rotate = int(nRows *fabs(a) + nCols *fabs(b));
	int     m_height_rotate = int(nCols *fabs(a) + nRows *fabs(b));
	float   map[6];
	Mat     m_map_matrix(2,3,CV_32F, map);

	CvPoint2D32f center = cvPoint2D32f(nCols/2, nRows/2);  
	CvMat map_matrix2 = m_map_matrix;  
	cv2DRotationMatrix(center, degree, 1.0, &map_matrix2);  //degree为角度，不是弧度
	map[2] += (m_width_rotate - nCols)/2;
	map[5] += (m_height_rotate - nRows)/2;

	Mat m_image_out;
	m_image_out.create(Size(m_width_rotate, m_height_rotate), srcImage.type());
	warpAffine(srcImage, m_image_out, m_map_matrix, Size( m_width_rotate, m_height_rotate),1,0,0);
	//imwrite("C:\\Users\\Administrator\\Desktop\\m_image_out.jpg", m_image_out);
	
	if(mark)
	{
		copyMakeBorder(m_image_out, m_image_out, m_nHeight/8, m_nHeight/8, m_nWidth/8, m_nWidth/8, BORDER_CONSTANT, Scalar::all(0));

		double scale = m_dRat;
		Point2f tempcenter = Point2f((float)m_image_out.cols / 2, (float)m_image_out.rows / 2);  // 旋转中心   	 
		Mat rotateMat;   
		rotateMat = getRotationMatrix2D(tempcenter, -90, scale);  

		Mat rotateImg = Mat::ones(Size(m_image_out.rows, m_image_out.cols), m_image_out.type()); 
		warpAffine(m_image_out, rotateImg, rotateMat, rotateImg.size());  

		//imwrite("C:\\Users\\Administrator\\Desktop\\rotateImg.jpg", rotateImg);
		return rotateImg;
	}
	else
	{
		return m_image_out;
	}
}


//霍夫圆变换
Mat CScanner_OpenCV::HoughCirclesTransfer(Mat src_img ,double dp,double threshold1, double threshold2)
{
	CvScalar scalar;
	IplImage src_img_ipl = IplImage(src_img);

	Mat midImage;//临时变量和目标图的定义

	if(src_img_ipl.nChannels == 3)
	{
		//【3】转为灰度图，进行图像平滑  
		cvtColor(src_img, midImage, CV_BGR2GRAY); 
		GaussianBlur(midImage, midImage, Size(9, 9), 2, 2 ); 
	}
	else
	{
		midImage = src_img; //不变为灰度图
		GaussianBlur(midImage, midImage, Size(9, 9), 2, 2 ); 	
	}
	
	//【4】进行霍夫圆变换  
	vector<Vec3f> circles;  //存储下面三个参数: x_{c}, y_{c}, r 集合的容器来表示每个检测到的圆;圆心横坐标，圆心纵坐标和圆半径
	double minDist;//src_gray.rows/8: 为霍夫变换检测到的圆的圆心之间的最小距离，如果检测到的两个圆心之间距离小于该值，则认为它们是同一个圆心
	minDist = midImage.rows/15;

	double dFx = (double)m_fXResolution/200.00; //1
	WORD width = (m_nSourceWidth > m_nSourceHeight)?m_nSourceHeight:m_nSourceWidth; //1652
	WORD newWidth = (WORD)(width * dFx);
	int maxradius =  newWidth/30; //圆孔的直径  55
	
	HoughCircles(midImage, circles, CV_HOUGH_GRADIENT, dp, minDist, threshold1, threshold2, 0, 2*maxradius/3);
	//HoughCircles(midImage, circles, CV_HOUGH_GRADIENT, dp, minDist, threshold1, threshold2, 0, 0);  //200,100 CV_HOUGH_GRADIENT表示霍夫梯度法 
	//threshold1是Canny的边缘阀值上限high，下限low被置为上限的一半;  minRadius和maxRadius为所检测到的圆半径的最小值和最大值
	//【5】依次在图中绘制出圆  
	for(size_t i = 0; i < circles.size(); i++)  
	{ 
		Point center(cvRound(circles[i][0]), cvRound(circles[i][1])); 
		int radius = cvRound(circles[i][2]);  
	
		int tempcentery = center.y+radius;
		int tempcenterx = center.x+radius;
		if(tempcentery >= midImage.rows)
		{
			tempcentery = midImage.rows-1; //防止cvGet2D崩溃
		}
		if(tempcenterx >= midImage.cols)
		{
			tempcenterx = midImage.cols-1;
		}
		
		scalar = cvGet2D(&src_img_ipl, tempcentery, tempcenterx); //cvGet2D(图片 y坐标，x坐标)获取 CvScalar对象,是y,x不是x,y
		if(radius < threshold2) //新增，半径小于阈值2时才填充
		{
			circle(src_img, center, (int)(1.5*radius), scalar, -1, 8, 0 );
		}
		else //大于时，只画圆
		{
			//绘制圆心
			//circle( src_img, center, 3, Scalar(0,255,0), -1, 8, 0 ); //-1表示填充，为正数表示线条粗细
			//绘制圆轮廓 
			//circle(src_img, center, radius, Scalar(155,50,255), 3, 8, 0 ); 
		}
	}  
	
	return src_img;
}

Mat CScanner_OpenCV::RemovePunch(double threshold1, double threshold2)
{
	ChangeImage(IMAGENAME_REMOVEPUNCH);
	Mat src_img = imread(m_szSourceImagePath, CV_LOAD_IMAGE_UNCHANGED);
	
	vector<Rect> rects;
	Rect rectTemp(0, 0, 3*src_img.cols/30, 3*src_img.rows/30); //宽、高只取十分之一,但rect宽高需要是3的倍数
	rects.push_back(Rect(0, 0, src_img.cols, rectTemp.height)); //上侧
	rects.push_back(Rect(0, src_img.rows-rectTemp.height, src_img.cols, rectTemp.height));	 //下侧	
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
		Release();  // 传输结束，清理内存
	}
	return rtn;
}

bool CScanner_OpenCV::getDeviceOnline() const
{
	return true;
}


void CScanner_OpenCV::Mat2uchar(Mat src_img)
{
	if(m_byte_image)  // 释放内存，否则会导致内存泄露
	{
		free(m_byte_image);
		m_byte_image = NULL;
	}	

	m_widthstep = (src_img.step+7)/8*8; //8字节对齐   4字节对齐：(src_img.step+3)/4*4
	m_byte_image = (BYTE *)calloc(src_img.rows*m_widthstep, sizeof(BYTE)); // 申请内存
	int channel = src_img.channels(); 

	// 逐一复制数据  
	BYTE *p1, *p2;
	for (int i = 0; i < src_img.rows; i++)  
	{ 
		p1 = src_img.data + i * src_img.step;  //p1 = src_img.data + scanline * src_img.cols * src_img.channels();  
		p2 = m_byte_image + i * m_widthstep;
	
		for(int j = 0; j < src_img.cols; j++)  
		{ 
			if(1 == channel)
			{
				*(p2) = *(p1);
				p1    += 1;
				p2    += 1;
			}
			else if(3 == channel)
			{
				*(p2)   = *(p1);  
				*(p2+1) = *(p1+1);  
				*(p2+2) = *(p1+2);        
				p1  += 3;  
				p2  += 3;  
			}	
		}
	}  
}


BYTE *CScanner_OpenCV::GetScanLine(int scanline)
{
	BYTE *ps;
	ps = m_byte_image + scanline * m_widthstep;  //memcpy(p2, p1, widthStep);
	return ps;
}


bool CScanner_OpenCV::getScanStrip(BYTE *pTransferBuffer, DWORD dwRead, DWORD &dwReceived)
{
	dwReceived = 0;

	if( NULL == pTransferBuffer ||    // Invalid paramiter
		dwRead < m_nDestBytesPerRow ) // Need enough memory to transfer at least an entire row
	{
		return false;
	}

	BYTE    *pBits    = NULL;
	WORD    nRow      = 0;
	WORD    nMaxRows  = (WORD)(dwRead / m_nDestBytesPerRow); //number of rows行 to be transfered during this call (function of buffer size and line size)

	if( m_nScanLine < MIN(m_nSourceHeight, m_nHeight) ) //0<min(2250,2200)
	{
		//fill the buffer line by line to take care of alignment differences
		for(nRow = 0; nRow < nMaxRows; nRow++) //nMaxRows = 12
		{
			//get the next scan line position and copy it
			pBits = GetScanLine(m_nScanLine);

			memcpy( pTransferBuffer, pBits, MIN(m_nDestBytesPerRow, m_widthstep)); //MIN(5100,4956) step表示以字节为单位的每行的长度
		
			// Check to see if the result image width is wider than what we have.
			// If it is wider fill it in with 0es
			if(m_nDestBytesPerRow > m_widthstep)
			{
				memset( pTransferBuffer + m_widthstep, 0, m_nDestBytesPerRow - m_widthstep );
			}

			//increment the destination by the aligned line size
			pTransferBuffer += m_nDestBytesPerRow;

			// increment the current scanline for next pass
			m_nScanLine++;

			//update the number of bytes written 
			dwReceived += m_nDestBytesPerRow; //dwReceived=20400
		
			// check for finished scan
			if( m_nScanLine >= m_nSourceHeight || //m_nScanLine = 2200
				m_nScanLine >= m_nHeight )
			{
				//we are done early
				break;
			}
		}
	}

	// Check to see if the result image length is longer than we have.
	// If it is longer fill it in with 0es
	if(m_nHeight > m_nScanLine )
	{
		nMaxRows  = (WORD)((dwRead-dwReceived) / m_nDestBytesPerRow);
		memset( pTransferBuffer, 0, m_nDestBytesPerRow * nMaxRows );
		m_nScanLine += nMaxRows;
		dwReceived += m_nDestBytesPerRow * nMaxRows;
	}

	return true;
}

//色彩翻转
void CScanner_OpenCV::ColorFlip(const Mat &src, Mat &dst)
{
	CV_Assert(src.depth() == CV_8U); //若括号中的表达式值为false，则返回一个错误信息
	dst.create(src.rows, src.cols, src.type());

	int width = src.cols; //1652
	int height = src.rows; //2250
	int i,j;
	/*char buf[60];
	itoa(width, buf, 10);
	::MessageBox(g_hwndDLG, TEXT(buf),"width",MB_OK);
	itoa(height, buf, 10);
	::MessageBox(g_hwndDLG, TEXT(buf),"height",MB_OK);
	*/
	for(j = 0; j < width; j++)
	{
		for(i = 0; i < height; i++)
		{
			switch (src.channels())
			{
			case 1:
				dst.at<uchar>(i,j) = 255 - (int)src.at<uchar>(i,j);
				break;
			case 3:
				dst.at<Vec3b>(i,j)[0] = 255 - (int)(src.at<Vec3b>(i,j)[0]);
				dst.at<Vec3b>(i,j)[1] = 255 - (int)(src.at<Vec3b>(i,j)[1]);
				dst.at<Vec3b>(i,j)[2] = 255 - (int)(src.at<Vec3b>(i,j)[2]);
				break;			
			}
		}
	}
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

	cvSmooth(&in, out, CV_MEDIAN, 3, in.nChannels);  //  中值滤波 medianBlur(src,dst,3);
	m_mat_image = out; //IplImage -> Mat
	cvReleaseImage(&out);
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
			MatIterator_<uchar> it, end;  
			//运用迭代器访问矩阵元素
			for( it = dst.begin<uchar>(), end = dst.end<uchar>(); it != end; it++ )  
			{  
				*it = lut[(*it)];   
			}
			break;
		}  
	case 3:  
		{ 
			MatIterator_<Vec3b> it, end;
			for( it = dst.begin<Vec3b>(), end = dst.end<Vec3b>(); it != end; it++ )  
      {    
        (*it)[0] = lut[((*it)[0])];  
        (*it)[1] = lut[((*it)[1])];  
        (*it)[2] = lut[((*it)[2])];  
			}  
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
	else if(m_nDocCount  == 0)
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
}


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
	
	for(int y = 0; y < psrcImage->rows; y++ ) //y < psrcImage->rows  
	{  
		for(int x = 0; x < psrcImage->cols; x++ )   //y < psrcImage->cols
		{  
			for(int c = 0; c < 3; c++ )  
			{  
				/*
				saturate_cast为了安全转换，运算结果可能超出像素取值范围（溢出），还可能是非整数（如果是浮点数的话），
				用saturate_cast对结果进行转换，以确保它为有效值。		
				*/
				pdstImage->at<Vec3b>(y,x)[c]= 
					saturate_cast<uchar>( (nContraValue*0.01)*(psrcImage->at<Vec3b>(y,x)[c] ) + nBrightValue);  
			}
		}  
	}  
	return true;
}


void CScanner_OpenCV::ChangeImage(const TCHAR* imageName)
{
	SSTRCPY(m_szSourceImagePath, sizeof(szTWAIN_DS_DIR), szTWAIN_DS_DIR);
	strcat(m_szSourceImagePath,  "\\");
	strcat(m_szSourceImagePath, imageName);
}

void CScanner_OpenCV::Release()
{
	m_mat_image.release();
	if(m_byte_image)
	{
		free(m_byte_image);
		m_byte_image = NULL;
	}	
}


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

#define IMAGENAME_FRONT "Scanner_leaflet_front.jpg"
#define IMAGENAME_BACK "Scanner_leaflet_back.jpg"
#define IMAGENAME_REMOVEPUNCH "RemovePunch.jpg"
#define IMAGENAME_AUTOCORRECT "AutoCorrect.jpg"


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
	m_bRemoveBlank        = FALSE;
	m_bRemovePunch        = TWRP_DISABLE;
	m_bSharpen            = TWSP_DISABLE;
	m_bRemoveBack         = TWRB_DISABLE;
	m_bDescreen           = TWDS_DISABLE;
	m_bDenoise            = TWDN_DISABLE;
	m_bAutoCrop           = TWAC_DISABLE;

	//2017新增
	m_nMaxDocCount        = 1;
	m_byteMultiValue      = 0.0;
	m_fEdgeUp             = 0.0;
	m_fEdgeDown           = 0.0;
	m_fEdgeLeft           = 0.0;
	m_fEdgeRight          = 0.0;
	m_fXPos               = 0.0;
	m_fYPos               = 0.0;
	m_nCompress           = TRUE;
	m_fCompressValue      = 0.0;

	m_bColorFlip          = FALSE;

	m_nCacheMode          = TWCM_NONE; //自动
	m_fCMAuto             = 1.0;
	m_fCMPaperNum         = 1.0;
	m_fCMMemorySize       = 1.0;
	
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

	long lXDPI = 200; //获得图像x轴分辨率
	long lYDPI = 200;

	double dFx = (double)m_fXResolution/lXDPI;
	double dFy = (double)m_fYResolution/lYDPI;

	WORD unNewWidth = (WORD)(m_nSourceWidth * dFx); //1770 ;根据DPI需要调节图像大小
	WORD unNewHeight = (WORD)(m_nSourceHeight * dFy); //2200

	cv::Mat matTemp;
	cv::resize(m_mat_image, matTemp, cv::Size(unNewWidth, unNewHeight), 0, 0);		
	matTemp.copyTo(m_mat_image);  // 深拷贝
	m_dRat = (double)unNewWidth/unNewHeight;

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

	//色彩翻转
	if(m_bColorFlip == TWCF_AUTO)
	{ 
		Mat mat_hColorflip;
		ColorFlip(m_mat_image, mat_hColorflip);
		mat_hColorflip.copyTo(m_mat_image);
	}
	
	//多流输出
	if(m_bMultiStream)
	{
		Mat matMuilt;
		m_mat_image.copyTo(matMuilt); //m_mat_image不管多流选什么，都是彩色图
		BYTE m_byteMuilt = m_byteMultiValue;
		m_byteMuilt = SwitchBYTE(m_byteMuilt);
		m_mat_image = SetMuiltStream(matMuilt, m_byteMuilt);
	}
	else//多流输出不使用时
	{
		//颜色
		if(m_nPixelType != TWPT_RGB)//
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
	if(m_bSharpen == TWSP_AUTO) 
	{	
		if(m_nPixelType != TWPT_BW)
		{
			Mat matSharpen;
		
			/*//sobel锐化
			m_mat_image.copyTo(matSharpen);
			GaussianBlur(matSharpen, matSharpen, Size(3,3), 0, 0, BORDER_DEFAULT);
			Mat grad_x, grad_y;
			Mat abs_grad_x, abs_grad_y;	
			//Sobel参数为：源图像，结果图像，图像深度，x方向阶数，y方向阶数，核的大小，尺度因子，增加的值
			Sobel(matSharpen, grad_x, index, 1, 0, 3, 1, 0, BORDER_DEFAULT); //分别计算x方向和y方向的导数，index为图像的深度
			Sobel(matSharpen, grad_y, index, 0, 1, 3, 1, 0, BORDER_DEFAULT);		
			convertScaleAbs(grad_x, abs_grad_x); //将其转成CV_8U 
			convertScaleAbs(grad_y, abs_grad_y);
			Mat grad;
			addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad); //用两个方向的倒数去模拟梯度 
			grad = matSharpen + grad;
			grad.copyTo(m_mat_image);*/

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
			cvtColor(matRemoveBack, bwMat, CV_BGR2GRAY);
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
	/*
	if(m_bRemoveBack == TWRB_AUTO) 
	{
		Mat matRemoveBack;	
		m_mat_image.copyTo(matRemoveBack);
		Mat bwmat;
		cvtColor(matRemoveBack,bwmat,CV_BGR2GRAY); 
		threshold( bwmat, bwmat, 0, 255, THRESH_OTSU);
		vector<vector<Point>> contours; 
		// find 
		findContours(bwmat,contours,CV_RETR_LIST,CV_CHAIN_APPROX_NONE); 
		// draw 
		/*Mat result(matRemoveBack.size(),matRemoveBack.type(),Scalar(0)); 
		drawContours(result,contours,-1,Scalar(255,255,255),2);		
		result.copyTo(m_mat_image);*/
		/*
		Mat result(matRemoveBack.size(),matRemoveBack.type(),Scalar(0)); 
		drawContours(result,contours,-1,Scalar(255),2);
		
		Mat dstMat(matRemoveBack.rows, matRemoveBack.cols, CV_8UC3);
		//将黑白图中的黑色像素点还原为原图中的像素点
		for(int j = 0; j < result.rows; j++)
		{
			for(int i = 0; i < result.cols; i++)
			{
				if((int)(result.at<uchar>(j,i)) == 0)
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
	*/
		/*
		MatND hist;

		double maxVal = 0;
		double minVal = 0;

		if(m_nPixelType!=TWPT_BW)
		{
			const int channels[1]={0};
			const int histSize[1]={256};
			float hranges[2]={0,255};
			const float* ranges[1]={hranges};	
			calcHist(&matRemoveBack,1,channels,Mat(),hist,1,histSize,ranges);

			//找到直方图中的最大值和最小值 最多出现了多少次
			minMaxLoc(hist,&minVal,&maxVal,0,0);

			int gray; //最大取值对应的灰度
			int size = hist.rows; //256
			for(int h = 0; h < size; h++)
			{
				float binVal = hist.at<float>(h);
				if(binVal == maxVal)
				{
					gray = h;
				}
			}

			for(int i = 0; i < 256; i++)
			{
				if(i == gray)
				{
					matRemoveBack.at<uchar>(i) = 255; 
				}
			}

			matRemoveBack.copyTo(m_mat_image);
		}
		*/
	//}

	//去除穿孔
	if(m_bRemovePunch == TWRP_AUTO) 
	{	 
		Mat matRemovepunch;
		matRemovepunch = RemovePunch(200, 22); //去除穿孔
		matRemovepunch.copyTo(m_mat_image);

		m_nWidth = m_mat_image.cols; 
		m_nHeight = m_mat_image.rows;
	}

	//自动裁切与校正
	if(m_bAutoCrop == TWAC_AUTO) 
	{
		Mat matAutoCrop; 
		matAutoCrop = AutoCorrect();//先自动校正	

		//int width = matAutoCrop.cols;
		//int height = matAutoCrop.rows;
		//Mat img;
		//resize(matAutoCrop, img, Size(width*width/height, width), 0, 0, 3); //等比例缩放
		//imwrite("C:\\Users\\Administrator\\Desktop\\a.jpg", img);

		matAutoCrop = RemoveBlack(matAutoCrop);
		matAutoCrop.copyTo(m_mat_image);
		
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
	m_nHeight = m_nHeight + (int)temp[0] + (int)temp[1] + (int)temp[2];

	temp[3] = ConvertUnits(m_fYPos, m_nUnits, TWUN_PIXELS, m_fXResolution); 
	temp[4] = ConvertUnits(m_fEdgeLeft, m_nUnits, TWUN_PIXELS, m_fXResolution); 
	temp[5] = ConvertUnits(m_fEdgeRight, m_nUnits, TWUN_PIXELS, m_fXResolution); 
	m_nWidth = m_nWidth + (int)temp[3] + (int)temp[4] + (int)temp[5];

	Mat borderMat;
	copyMakeBorder(m_mat_image, borderMat, (int)temp[1]+(int)temp[3], (int)temp[2], (int)temp[4]+(int)temp[0], (int)temp[5], BORDER_CONSTANT, cv::Scalar(0,0,0)); //以常量形式扩充边界,为BORDER_CONSTANT时，最后一个是填充所需的像素的值
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
	//BYTE *temp = NULL;
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

Mat CScanner_OpenCV::applyLookUp(const Mat &src, const Mat &lookup)
{  
	//输出图像  
	Mat result;  
	//应用查找表  
	LUT(src, lookup, result);  
	return result;  
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
			threshold(src_img, src_img, 0, 255, THRESH_OTSU);
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
	
	Mat imageSave = inputImg(rect);
	return imageSave;
}

//#define DEGREE 27
Mat CScanner_OpenCV::AutoCorrect()
{
	ChangeImage(IMAGENAME_AUTOCORRECT);
	Mat img = imread(m_szSourceImagePath, CV_LOAD_IMAGE_UNCHANGED);

	Point center(img.cols/2, img.rows/2);

#ifdef DEGREE
	//旋转没有倾斜的图像，调试使用
	Mat rotMatS = getRotationMatrix2D(center, DEGREE, 1.0);//获取旋转矩阵
	warpAffine(img, img, rotMatS, img.size(), 1, 0, Scalar(255,255,255));//o实现坐标系仿射变换
#endif
	
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
	//magnitudeMat = magnitudeMat(Rect(0, 0, magnitudeMat.cols & -2, magnitudeMat.rows & -2));

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
	theta = atan(angleT) * 180/CV_PI;

	//仿射变换校正***********************
	//取图像中心
	//Rotate the image to recover
	//校正图像
	//最后一步，当然是把图像转回去
	//先用getRotationMatrix2D()获得一个2*3的仿射变换矩阵，
	//再把这个矩阵输入warpAffine()，做一个单纯旋转的仿射变换。
	//warpAffine()的最后一个参数Scalar(255,255,255)是把由于旋转产生的空白用白色填充。

	//彩图	
	Point2f centerPoint = Point2f(img.cols/2, img.rows/2);
	Mat warpMat = getRotationMatrix2D(centerPoint, theta, 1.0);
	Mat resultImage = Mat::ones(img.size(), img.type());
	warpAffine(img, resultImage, warpMat, resultImage.size(), 1, 0, Scalar(255,255,255));
	//imwrite("C:\\Users\\Administrator\\Desktop\\a.jpg", resultImage);
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
	double minDist;//src_gray.rows/8: 为霍夫变换检测到的圆的圆心之间的最小距离
	minDist = midImage.rows/15;
	HoughCircles(midImage, circles, CV_HOUGH_GRADIENT, dp, minDist, threshold1, threshold2, 0, 0);  //200,100 

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
		/*
		char buf[60];
		itoa(tempcenterx, buf, 10);
		::MessageBox(g_hwndDLG, TEXT(buf),"tempcenterx",MB_OK);
		itoa(tempcentery, buf, 10);
		::MessageBox(g_hwndDLG, TEXT(buf),"tempcentery",MB_OK);*/

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
	}
	return rtn;
}

bool CScanner_OpenCV::getDeviceOnline() const
{
	return true;
}


void CScanner_OpenCV::Mat2uchar(Mat src_img)
{
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


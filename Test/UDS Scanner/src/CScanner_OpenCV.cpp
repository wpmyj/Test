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

	//Base����
	m_nPaperSource        = TRUE;  //ɨ��ģʽ-�Զ���ֽ��
	m_nPixelType          = TWPT_RGB; //ͼ������-��ɫ zhu
	m_fXResolution        = 200.0;
	m_fYResolution        = 200.0; //�ֱ���-200.0
	m_bDuplex             = false; //����/˫��-����
	m_fContrast           = 0.0; //�Աȶ�-0.0
	m_fBrightness         = 0.0; //����-0.0
	m_fThreshold          = 128.0; //��ֵ-128.0 ������Ĭ��128.G6400Ĭ��230
	m_bMultifeedDetection = true; //���ż��-ѡ��
	m_nMD_value           = TWMV_PAUSE;
	//Advanced����
	m_nOrientation        = TWOR_ROT0; //zhu ֽ�ŷ���-����
	m_nStandardsizes      = TWSS_A4; //zhu ��ӦICAP_SUPPORTEDSIZES��ֽ�Ŵ�С-A4
	m_nUnits              = TWUN_INCHES;  //zhu ��λ-Ӣ��

	m_nWidth              = 0;
	m_nHeight             = 0;

	m_fRotation           = 0.0; //��ת-����תzhu
	m_nSpiltImage         = TWSI_NONE; //zhu �ָ�-���ָ�
	m_fGamma              = 100.0; //zhu GammaУ��-Ĭ��Ϊ100
	m_bMirror             = TWMR_DISABLE; //����-��ѡ��

	m_nBinarization       = TWBZ_DYNATHRESHOLD; //zhu ��ֵ��-��̬��ֵ
	m_bMultiStream        = true; //�������-ѡ��
	m_fSensitiveThreshold_removespots = 0.0; //ȥ���ߵ�-0.0
	m_fSensitiveThreshold_colorretent = 128.0; //��ɫ����-128.0

	//����ͼ����
	//Ĭ�ϲ�ѡ��
	m_fRemoveBlank        = TWBP_DISABLE; 
	m_bRemoveBlank        = TWRA_DISABLE;
	m_bRemovePunch        = TWRP_DISABLE;
	m_bSharpen            = TWSN_DISABLE;
	m_bRemoveBack         = TWRB_DISABLE;
	m_bDescreen           = TWDS_DISABLE;
	m_bDenoise            = TWDN_DISABLE;
	m_bAutoCrop           = TWAC_DISABLE;

	//2017����
	m_nMaxDocCount        = 1;
	m_byteMultiValue      = 1.0; //��ʾֻ�������ɫѡ��
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

	m_nCacheMode          = TWCM_NONE; //�Զ�
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
	CDevice_Base::setSetting(settings);  // ���ø���ķ���
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
		m_totalImageCount = BitCount(m_byteMultiValue); //����1�ĸ���
		BYTE m_tempMuilt;
		m_tempMuilt = m_byteMultiValue;
		m_tempMuilt = m_tempMuilt & 0x0F;
		m_frontImageCount = BitCount(m_tempMuilt); //����λ��1�ĸ���

		if(m_i < m_frontImageCount)
		{
			ChangeImage(IMAGENAME_FRONT);
			m_i++;		
		}
		//������ѡ��ʱ����һ��ͼƬ
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
	else //����δѡ��
	//�������浥˫��
	{
		if(m_bDuplex) //˫��
		{
			if(m_nDocCount == 2) //����
			{
				ChangeImage(IMAGENAME_FRONT);
			}
			else if(m_nDocCount == 1) //����
			{
				ChangeImage(IMAGENAME_BACK);
			}
		} // if(m_bDuplex) end
		else // ����
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
	//��ʱ�����m_mat_image��Ϊ��ɫ
	IplImage img = IplImage(m_mat_image);  // Mat->IplImage

	// ��ȡӰ��Ŀ�ߣ���������Ϊ��λ 
	m_nSourceWidth   = img.width; //1100
	m_nSourceHeight  = img.height;
	WORD res = 0;

	//CxImage *pImage = new CxImage();
	//pImage->Load(m_szSourceImagePath);
	//long lXDPI = pImage->GetXDPI(); //���ͼ��x��ֱ���
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

	//�������
	if(m_bMultiStream)
	{
		Mat matMuilt;
		m_mat_image.copyTo(matMuilt); //m_mat_image���ܶ���ѡʲô�����ǲ�ɫͼ
		BYTE m_byteMuilt = m_byteMultiValue;
		m_byteMuilt = SwitchBYTE(m_byteMuilt);
		m_mat_image = SetMuiltStream(matMuilt, m_byteMuilt, 
			XReso, Bright, Contra, Compre, CompVal, Binari, Thres, Removespots);
		YReso = XReso;
	}
	else//���������ʹ��ʱ
	{
		XReso = m_fXResolution;
		YReso = m_fYResolution;
		Bright = m_fBrightness;
		Contra = m_fContrast;

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

	long lXDPI = 200; //���ͼ��x��ֱ���
	long lYDPI = 200;
	
	double dFx = (double)XReso/lXDPI;
	double dFy = (double)YReso/lYDPI;

	WORD unNewWidth = (WORD)(m_nSourceWidth * dFx); //1770 ;����DPI��Ҫ����ͼ���С
	WORD unNewHeight = (WORD)(m_nSourceHeight * dFy); //2200

	cv::Mat matTemp;
	cv::resize(m_mat_image, matTemp, cv::Size(unNewWidth, unNewHeight), 0, 0);		
	matTemp.copyTo(m_mat_image);  // ���
	m_dRat = (double)unNewWidth/unNewHeight;
	
	//imwrite("C:\\Users\\Administrator\\Desktop\\resize.bmp", m_mat_image);

	if(m_nOrientation == TWOR_LANDSCAPE) //����
	{		
		RotateImage(90);
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
	//if (m_nPixelType != TWPT_BW)
	if (m_nPixelType == TWPT_RGB) //��ʱ��Ϊֻ�Բ�ͼ����
	{
		Mat matTemp;
		//ContrastAndBright(&matTemp, &m_mat_image, (int)m_fBrightness , (int)m_fContrast);
		ContrastAndBright(&matTemp, &m_mat_image, (int)Bright , (int)Contra);
		matTemp.copyTo(m_mat_image);
	}
	 
	//ͼ������
	if(m_bMirror == TWMR_AUTO)
	{ 
		Mat mat_hMirror;
		hMirrorTrans(m_mat_image, mat_hMirror);
		mat_hMirror.copyTo(m_mat_image);
	}

	//ɫ�ʷ�ת
	if(m_bColorFlip == TWCF_AUTO)
	{ 
		Mat mat_hColorflip;
		ColorFlip(m_mat_image, mat_hColorflip);
		mat_hColorflip.copyTo(m_mat_image);
	}

	//GammaУ��
	if(m_fGamma != 100.0) //zhu
	{
		Mat matGamma;//Mat& matGamma = m_mat_image;
		GammaCorrection(m_mat_image, matGamma, m_fGamma/100);
		m_mat_image = matGamma;
	}

	//ȥ������
	if(m_bDenoise == TWDN_AUTO) 
	{	
		Mat matDenoise;
		medianBlur(m_mat_image, matDenoise, 3);
		matDenoise.copyTo(m_mat_image);
		//MedianSmooth(m_mat_image);
	}

	//ȥ����
	if(m_bDescreen == TWDS_AUTO) 
	{
		Mat matDescreen;
		m_mat_image.copyTo(matDescreen);

		//Size(5,5)ģ���С��Ϊ������Ҫ����ȷ��Ҫ�Լ�����ģ��
		//x���򷽲�
		//Y���򷽲�
		GaussianBlur(matDescreen, matDescreen, Size(5,5), 0, 0);  //  ��˹�˲�
		matDescreen.copyTo(m_mat_image);
	}

	//��
	int index = FindDepth(m_mat_image); //indexΪͼ������
	//��ͼ��
	if(m_bSharpen == TWSN_AUTO) 
	{	
		if(m_nPixelType != TWPT_BW)
		{
			Mat matSharpen;
		
			//USM ��
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

	//��
	//ȥ������
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
			threshold(bwMat, bwMat, (double)thresoldvalue, 255, THRESH_BINARY);  //OTSUҲ��171
			Mat dstMat(matRemoveBack.rows, matRemoveBack.cols, CV_8UC3);
			//���ڰ�ͼ�еĺ�ɫ���ص㻹ԭΪԭͼ�е����ص�
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

	//ȥ������
	if(m_bRemovePunch == TWRP_AUTO) 
	{	 
		Mat matRemovepunch;
		matRemovepunch = RemovePunch(200, 30); //ȥ������ ԭ��22
		matRemovepunch.copyTo(m_mat_image);

		m_nWidth = m_mat_image.cols; 
		m_nHeight = m_mat_image.rows;
	}

	//�Զ�������У��
	if(m_bAutoCrop == TWAC_AUTO)
	{
		Mat matAutoCrop; 
		matAutoCrop = AutoCorrect();//���Զ�У��	
		matAutoCrop = RemoveBlack(matAutoCrop);
		matAutoCrop.copyTo(m_mat_image);
		//imwrite("C:\\Users\\Administrator\\Desktop\\У��ͼ.jpg", m_mat_image);

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

	//ƫ�����Լ���Ե����
	float temp[10]; 
	temp[0] = ConvertUnits(m_fXPos, m_nUnits, TWUN_PIXELS, m_fXResolution); //ת��Ϊ����		
	temp[1] = ConvertUnits(m_fEdgeUp, m_nUnits, TWUN_PIXELS, m_fXResolution); 
	temp[2] = ConvertUnits(m_fEdgeDown, m_nUnits, TWUN_PIXELS, m_fXResolution); 

	temp[3] = ConvertUnits(m_fYPos, m_nUnits, TWUN_PIXELS, m_fXResolution); 
	temp[4] = ConvertUnits(m_fEdgeLeft, m_nUnits, TWUN_PIXELS, m_fXResolution); 
	temp[5] = ConvertUnits(m_fEdgeRight, m_nUnits, TWUN_PIXELS, m_fXResolution); 
	m_nHeight = m_nHeight + (int)temp[3] + (int)temp[1] + (int)temp[2];
	m_nWidth = m_nWidth + (int)temp[0] + (int)temp[4] + (int)temp[5];

	Mat borderMat;
	copyMakeBorder(m_mat_image, borderMat, (int)temp[1], (int)temp[2]+(int)temp[3], (int)temp[4], (int)temp[5]+(int)temp[0], BORDER_CONSTANT, cv::Scalar(0,0,0)); //�Գ�����ʽ����߽�,ΪBORDER_CONSTANTʱ�����һ���������������ص�ֵ
	borderMat.copyTo(m_mat_image);

	//ͼ��ָ�
	if(m_nSpiltImage == TWSI_NONE)
	{}
	else if(m_nSpiltImage == TWSI_HORIZONTAL) //ˮƽ�ָ�
	{
		SpiltImage(m_mat_image,2,1);
		m_nWidth = m_mat_image.cols; 
		m_nHeight = m_mat_image.rows;
	}
	else if(m_nSpiltImage == TWSI_VERTICAL) //��ֱ�ָ�
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
			m_nDestBytesPerRow = BYTES_PERLINE(m_nWidth, 8); //Ϊ1ʱ����
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

	//ȥ���հ�ҳ
	bool status = false; //Ĭ�ϲ��ǿհ�ҳ
	if(m_bRemoveBlank == TWRA_AUTO)//checkbox����
	{
		Mat matRemoveBlank;
		m_mat_image.copyTo(matRemoveBlank);
		status = RemoveBlank(matRemoveBlank, m_fRemoveBlank);
	}

	//Mat����תΪ�ֽڶ����uchar,���������󣬷�������ͼ���������Ч
	Mat tempmat;
	m_mat_image.copyTo(tempmat);
	Mat2uchar(tempmat);

	if(status) //��Ϊ�棬��ʾ�ǿհ�ҳ
	{
		return false;
	}
	else
	{
		return true;
	}
}

//OTSU�����䷽���㷨
int CScanner_OpenCV::otsu(Mat image) 
{
	int width = image.cols;
	int height = image.rows;
	int pixelCount[256]; //ÿ������������ͼ���еĸ��� 
	float pixelPro[256]; //ÿ������������ͼ���еı���
	int i, j;
	int pixelSum = width * height;
	int threshold = 0;

	uchar* data = (uchar*)image.data;

	//��ʼ��  
	for(i = 0; i < 256; i++)
	{
		pixelCount[i] = 0;
		pixelPro[i] = 0;
	}

	//ͳ�ƻҶȼ���ÿ������������ͼ���еĸ���  
	for(i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			pixelCount[data[i * image.step+ j]]++;
		}
	}

	//����ÿ������������ͼ���еı���  
	for (i = 0; i < 256; i++)
	{
		pixelPro[i] = (float)(pixelCount[i]) / (float)(pixelSum);
	}

	//����ostu�㷨,�õ�ǰ���ͱ����ķָ�  
	//�����Ҷȼ�[0,255],������������ĻҶ�ֵ,Ϊ�����ֵ  
	float w0, w1, u0tmp, u1tmp, u0, u1, u, deltaTmp, deltaMax = 0;
	for (i = 0; i < 256; i++)
	{
		w0 = w1 = u0tmp = u1tmp = u0 = u1 = u = deltaTmp = 0;

		for (j = 0; j < 256; j++)
		{
			if (j <= i) //��������  
			{
				//��iΪ��ֵ���࣬��һ���ܵĸ���  
				w0 += pixelPro[j];
				u0tmp += j * pixelPro[j];
			}
			else       //ǰ������  
			{
				//��iΪ��ֵ���࣬�ڶ����ܵĸ���  
				w1 += pixelPro[j];
				u1tmp += j * pixelPro[j];
			}
		}

		u0 = u0tmp / w0;        //��һ���ƽ���Ҷ�  
		u1 = u1tmp / w1;        //�ڶ����ƽ���Ҷ�  
		u = u0tmp + u1tmp;      //����ͼ���ƽ���Ҷ�  
		//������䷽��  
		deltaTmp = w0 * (u0 - u)*(u0 - u) + w1 * (u1 - u)*(u1 - u);
		//�ҳ������䷽���Լ���Ӧ����ֵ  
		if (deltaTmp > deltaMax)
		{
			deltaMax = deltaTmp;
			threshold = i;
		}
	}
	//���������ֵ;  
	return threshold;
}

bool CScanner_OpenCV::RemoveBlank(Mat src_img, float fValue)
{
	Mat dst_img = src_img;
	int width = dst_img.cols; //��
	int height = dst_img.rows; //��

	int count = 0; //��¼�ڵ�ĸ���
	const float range = fValue/100; //������ͼ�кڵ�ռ�����صı���С��rangeʱ������Ϊ�ǿհ�ҳ

	if(m_nPixelType == TWPT_RGB)
	{
		cvtColor(dst_img, dst_img, CV_BGR2GRAY);
		threshold(dst_img, dst_img, 0, 255, THRESH_OTSU); 	
	}
	else if(m_nPixelType == TWPT_GRAY)
	{
		threshold(dst_img, dst_img, 0, 255, THRESH_OTSU); //ͨ�� Otsu �㷨����ѡ����ֵ����ʱ����threshold���趨����������
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
		//����
	case 0x01:  //��ɫ����
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
	
		//����
	case 0x02:  //�Ҷȵ���
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

		//����
	case 0x03:  //�Ҷȡ���ɫ
		{
			if(1 == m_nDocCount) //�����еĵ�һ��
			{
				m_nPixelType = TWPT_RGB;

				resol = m_fResolu[0];
				bright = m_fBright[0];
				contra = m_fContra[0];
				compre = m_nCompre[0];
				compval = m_fCompVal[0];
			}
			else if(0 == m_nDocCount) //�����еĵڶ���
			{
				m_nPixelType = TWPT_GRAY;
				cvtColor(src_img, src_img, CV_BGR2GRAY);//matMuilt��ɫתΪ�Ҷ�m_mat_image

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
	case 0x30: //����
		{
			if(1 == m_nDocCount) //�����еĵ�һ��
			{
				m_nPixelType = TWPT_RGB;

				resol = m_fResolu[3];
				bright = m_fBright[3];
				contra = m_fContra[2];
				compre = m_nCompre[3];
				compval = m_fCompVal[3];
			}
			else if(0 == m_nDocCount) //�����еĵڶ���
			{
				m_nPixelType = TWPT_GRAY;
				cvtColor(src_img, src_img, CV_BGR2GRAY);//matMuilt��ɫתΪ�Ҷ�m_mat_image

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

		//����
	case 0x04:  //�ڰ׵���
		{	
			m_nPixelType = TWPT_BW;
			cvtColor(src_img, src_img, CV_BGR2GRAY);
			threshold(src_img, src_img, 0, 255, THRESH_OTSU);
			src_img.copyTo(dst_img);

			resol = m_fResolu[2];
			bright = m_fBright[2];
			//contra = m_fContra[2]; �ڰ��޶Աȶ�
			compre = m_nCompre[2];
			compval = m_fCompVal[2];

			binari = m_nBinari[0];
			thres = m_fThres[0];
			removespots = m_fRemovespots[0];
		}	
		break;
	case 0x40: //����
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

		//����
	case 0x05:  //�ڰס���ɫ z����
		{
			if(1 == m_nDocCount) //�����еĵ�һ��
			{
				m_nPixelType = TWPT_RGB;

				resol = m_fResolu[0];
				bright = m_fBright[0];
				contra = m_fContra[0];
				compre = m_nCompre[0];
				compval = m_fCompVal[0];
			}
			else if(0 == m_nDocCount) //�����еĵڶ���
			{
				m_nPixelType = TWPT_BW;
				cvtColor(src_img, src_img, CV_BGR2GRAY);//matMuilt��ɫתΪ�Ҷ�m_mat_image
				threshold(src_img, src_img, m_fThreshold, 255, CV_THRESH_BINARY); //�Ҷȱ�ڰ�

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
			if(1 == m_nDocCount) //�����еĵ�һ��
			{
				m_nPixelType = TWPT_RGB;

				resol = m_fResolu[3];
				bright = m_fBright[3];
				contra = m_fContra[2];
				compre = m_nCompre[3];
				compval = m_fCompVal[3];
			}
			else if(0 == m_nDocCount) //�����еĵڶ���
			{
				m_nPixelType = TWPT_BW;
				cvtColor(src_img, src_img, CV_BGR2GRAY);//matMuilt��ɫתΪ�Ҷ�m_mat_image
				threshold(src_img, src_img, m_fThreshold, 255, CV_THRESH_BINARY); //�Ҷȱ�ڰ�

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

		//����
	case 0x06:  //�ڰס��Ҷ� ����
		{
			if(1 == m_nDocCount) //�����еĵ�һ��
			{
				m_nPixelType = TWPT_GRAY;
				cvtColor(src_img, src_img, CV_BGR2GRAY);

				resol = m_fResolu[1];
				bright = m_fBright[1];
				contra = m_fContra[1];
				compre = m_nCompre[1];
				compval = m_fCompVal[1];
			}
			else if(0 == m_nDocCount) //�����еĵڶ���
			{
				m_nPixelType = TWPT_BW;
				cvtColor(src_img, src_img, CV_BGR2GRAY);
				threshold(src_img, src_img, m_fThreshold, 255, CV_THRESH_BINARY); //�Ҷȱ�ڰ�

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
			if(1 == m_nDocCount) //�����еĵ�һ��
			{
				m_nPixelType = TWPT_GRAY;
				cvtColor(src_img, src_img, CV_BGR2GRAY);

				resol = m_fResolu[4];
				bright = m_fBright[4];
				contra = m_fContra[3];
				compre = m_nCompre[4];
				compval = m_fCompVal[4];
			}
			else if(0 == m_nDocCount) //�����еĵڶ���
			{
				m_nPixelType = TWPT_BW;
				cvtColor(src_img, src_img, CV_BGR2GRAY);
				threshold(src_img, src_img, m_fThreshold, 255, CV_THRESH_BINARY); //�Ҷȱ�ڰ�

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

		//����
	case 0x07:  //�ڰס��Ҷȡ���ɫ ����
		{
			if(2 == m_nDocCount) //�����еĵ�һ��
			{
				m_nPixelType = TWPT_RGB;

				resol = m_fResolu[0];
				bright = m_fBright[0];
				contra = m_fContra[0];
				compre = m_nCompre[0];
				compval = m_fCompVal[0];
			}
			else if(1 == m_nDocCount) //�����еĵڶ���
			{
				m_nPixelType = TWPT_GRAY;
				cvtColor(src_img, src_img, CV_BGR2GRAY);//matMuilt��ɫתΪ�Ҷ�bwMat

				resol = m_fResolu[1];
				bright = m_fBright[1];
				contra = m_fContra[1];
				compre = m_nCompre[1];
				compval = m_fCompVal[1];
			}
			else if(0 == m_nDocCount) //�����еĵ�����
			{
				m_nPixelType = TWPT_BW;
				cvtColor(src_img, src_img, CV_BGR2GRAY);//matMuilt��ɫתΪ�Ҷ�bwMat
				threshold(src_img, src_img, m_fThreshold, 255, CV_THRESH_BINARY); //�Ҷȱ�ڰ�

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
			if(2 == m_nDocCount) //�����еĵ�һ��
			{
				m_nPixelType = TWPT_RGB;

				resol = m_fResolu[3];
				bright = m_fBright[3];
				contra = m_fContra[2];
				compre = m_nCompre[3];
				compval = m_fCompVal[3];
			}
			else if(1 == m_nDocCount) //�����еĵڶ���
			{
				m_nPixelType = TWPT_GRAY;
				cvtColor(src_img, src_img, CV_BGR2GRAY);//matMuilt��ɫתΪ�Ҷ�bwMat

				resol = m_fResolu[4];
				bright = m_fBright[4];
				contra = m_fContra[3];
				compre = m_nCompre[4];
				compval = m_fCompVal[4];
			}
			else if(0 == m_nDocCount) //�����еĵ�����
			{
				m_nPixelType = TWPT_BW;
				cvtColor(src_img, src_img, CV_BGR2GRAY);//matMuilt��ɫתΪ�Ҷ�bwMat
				threshold(src_img, src_img, m_fThreshold, 255, CV_THRESH_BINARY); //�Ҷȱ�ڰ�

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
	int right = width; //�� ��2323
	int up = 0;
	int down = height; //�� ��2808

	int i = 0, j = 0;
	//�ϲ�
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
	//���
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

	//�²�
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
	//�Ҳ�
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
	cvtColor(midImage, dstImage, CV_GRAY2BGR);//ת����Ե�����ͼΪ��ͼ����ʵ�ʿ�������Ȼ�ǻҶ�ͼ

	//��3�����и��ʻ����߱任
	vector<Vec4i> lines;//����һ��ʸ���ṹlines���ڴ�ŵõ����߶�ʸ������
	HoughLinesP(midImage, lines, 1, CV_PI/180, 20, min(srcImage.cols/2,srcImage.rows/2), 50);

	//��4��������ͼ�л��Ƴ�ÿ���߶�
	Vec4i maxline = lines[0];
	float dx = 0.0;
	float dy = 0.0;
	for( size_t i = 0; i < lines.size(); i++ )
	{
		Vec4i l = lines[i];
		//ÿһ�����ɾ����ĸ�Ԫ�ص�ʸ��(x_1,y_1, x_2, y_2����ʾ��
		//���У�(x_1, y_1)��(x_2, y_2) ����ÿ����⵽���߶ε����ͽ����㡣
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
	{//����ʾ˳ʱ�룬����ʾ��ʱ��
		if(dx > dy)
		{	//��б�̱ߣ��ϱ�
			temp = dx/dy;
			theta = CV_PI/2.0 - atan(temp);
		}
		else
		{ //��б���ߣ��ұ�
			temp = dy/dx; 		
			theta = atan(temp); 
			mark = true;
		}
	}
	else
	{
		if(dx > fabs(dy))
		{//��б�̱ߣ��ϱ�
			temp = dx/fabs(dy);
			theta = atan(temp);
			mark = true;
		}
		else
		{//��б���ߣ��ұ�
			temp = fabs(dy)/dx;
			theta = CV_PI/2.0 - atan(temp);
		}
	}
	theta = theta/CV_PI*180; //����ת�Ƕ�

	int nRows = srcImage.rows; //��11420
	int nCols = srcImage.cols; //��4202

	//����任У��***********************
	float   degree = theta;
	double  angle  = degree/180*CV_PI; //�Ƕ�ת����
	double  a = sin(angle), b = cos(angle);
	int     m_width_rotate = int(nRows *fabs(a) + nCols *fabs(b));
	int     m_height_rotate = int(nCols *fabs(a) + nRows *fabs(b));
	float   map[6];
	Mat     m_map_matrix(2,3,CV_32F, map);

	CvPoint2D32f center = cvPoint2D32f(nCols/2, nRows/2);  
	CvMat map_matrix2 = m_map_matrix;  
	cv2DRotationMatrix(center, degree, 1.0, &map_matrix2);  //degreeΪ�Ƕȣ����ǻ���
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
		Point2f tempcenter = Point2f((float)m_image_out.cols / 2, (float)m_image_out.rows / 2);  // ��ת����   	 
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


//����Բ�任
Mat CScanner_OpenCV::HoughCirclesTransfer(Mat src_img ,double dp,double threshold1, double threshold2)
{
	CvScalar scalar;
	IplImage src_img_ipl = IplImage(src_img);

	Mat midImage;//��ʱ������Ŀ��ͼ�Ķ���

	if(src_img_ipl.nChannels == 3)
	{
		//��3��תΪ�Ҷ�ͼ������ͼ��ƽ��  
		cvtColor(src_img, midImage, CV_BGR2GRAY); 
		GaussianBlur(midImage, midImage, Size(9, 9), 2, 2 ); 
	}
	else
	{
		midImage = src_img; //����Ϊ�Ҷ�ͼ
		GaussianBlur(midImage, midImage, Size(9, 9), 2, 2 ); 	
	}
	
	//��4�����л���Բ�任  
	vector<Vec3f> circles;  //�洢������������: x_{c}, y_{c}, r ���ϵ���������ʾÿ����⵽��Բ;Բ�ĺ����꣬Բ���������Բ�뾶
	double minDist;//src_gray.rows/8: Ϊ����任��⵽��Բ��Բ��֮�����С���룬�����⵽������Բ��֮�����С�ڸ�ֵ������Ϊ������ͬһ��Բ��
	minDist = midImage.rows/15;

	double dFx = (double)m_fXResolution/200.00; //1
	WORD width = (m_nSourceWidth > m_nSourceHeight)?m_nSourceHeight:m_nSourceWidth; //1652
	WORD newWidth = (WORD)(width * dFx);
	int maxradius =  newWidth/30; //Բ�׵�ֱ��  55
	
	HoughCircles(midImage, circles, CV_HOUGH_GRADIENT, dp, minDist, threshold1, threshold2, 0, 2*maxradius/3);
	//HoughCircles(midImage, circles, CV_HOUGH_GRADIENT, dp, minDist, threshold1, threshold2, 0, 0);  //200,100 CV_HOUGH_GRADIENT��ʾ�����ݶȷ� 
	//threshold1��Canny�ı�Ե��ֵ����high������low����Ϊ���޵�һ��;  minRadius��maxRadiusΪ����⵽��Բ�뾶����Сֵ�����ֵ
	//��5��������ͼ�л��Ƴ�Բ  
	for(size_t i = 0; i < circles.size(); i++)  
	{ 
		Point center(cvRound(circles[i][0]), cvRound(circles[i][1])); 
		int radius = cvRound(circles[i][2]);  
	
		int tempcentery = center.y+radius;
		int tempcenterx = center.x+radius;
		if(tempcentery >= midImage.rows)
		{
			tempcentery = midImage.rows-1; //��ֹcvGet2D����
		}
		if(tempcenterx >= midImage.cols)
		{
			tempcenterx = midImage.cols-1;
		}
		
		scalar = cvGet2D(&src_img_ipl, tempcentery, tempcenterx); //cvGet2D(ͼƬ y���꣬x����)��ȡ CvScalar����,��y,x����x,y
		if(radius < threshold2) //�������뾶С����ֵ2ʱ�����
		{
			circle(src_img, center, (int)(1.5*radius), scalar, -1, 8, 0 );
		}
		else //����ʱ��ֻ��Բ
		{
			//����Բ��
			//circle( src_img, center, 3, Scalar(0,255,0), -1, 8, 0 ); //-1��ʾ��䣬Ϊ������ʾ������ϸ
			//����Բ���� 
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
	Rect rectTemp(0, 0, 3*src_img.cols/30, 3*src_img.rows/30); //����ֻȡʮ��֮һ,��rect�����Ҫ��3�ı���
	rects.push_back(Rect(0, 0, src_img.cols, rectTemp.height)); //�ϲ�
	rects.push_back(Rect(0, src_img.rows-rectTemp.height, src_img.cols, rectTemp.height));	 //�²�	
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
		Release();  // ��������������ڴ�
	}
	return rtn;
}

bool CScanner_OpenCV::getDeviceOnline() const
{
	return true;
}


void CScanner_OpenCV::Mat2uchar(Mat src_img)
{
	if(m_byte_image)  // �ͷ��ڴ棬����ᵼ���ڴ�й¶
	{
		free(m_byte_image);
		m_byte_image = NULL;
	}	

	m_widthstep = (src_img.step+7)/8*8; //8�ֽڶ���   4�ֽڶ��룺(src_img.step+3)/4*4
	m_byte_image = (BYTE *)calloc(src_img.rows*m_widthstep, sizeof(BYTE)); // �����ڴ�
	int channel = src_img.channels(); 

	// ��һ��������  
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
	WORD    nMaxRows  = (WORD)(dwRead / m_nDestBytesPerRow); //number of rows�� to be transfered during this call (function of buffer size and line size)

	if( m_nScanLine < MIN(m_nSourceHeight, m_nHeight) ) //0<min(2250,2200)
	{
		//fill the buffer line by line to take care of alignment differences
		for(nRow = 0; nRow < nMaxRows; nRow++) //nMaxRows = 12
		{
			//get the next scan line position and copy it
			pBits = GetScanLine(m_nScanLine);

			memcpy( pTransferBuffer, pBits, MIN(m_nDestBytesPerRow, m_widthstep)); //MIN(5100,4956) step��ʾ���ֽ�Ϊ��λ��ÿ�еĳ���
		
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

//ɫ�ʷ�ת
void CScanner_OpenCV::ColorFlip(const Mat &src, Mat &dst)
{
	CV_Assert(src.depth() == CV_8U); //�������еı��ʽֵΪfalse���򷵻�һ��������Ϣ
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

	cvSmooth(&in, out, CV_MEDIAN, 3, in.nChannels);  //  ��ֵ�˲� medianBlur(src,dst,3);
	m_mat_image = out; //IplImage -> Mat
	cvReleaseImage(&out);
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
			MatIterator_<uchar> it, end;  
			//���õ��������ʾ���Ԫ��
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
}


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
	
	for(int y = 0; y < psrcImage->rows; y++ ) //y < psrcImage->rows  
	{  
		for(int x = 0; x < psrcImage->cols; x++ )   //y < psrcImage->cols
		{  
			for(int c = 0; c < 3; c++ )  
			{  
				/*
				saturate_castΪ�˰�ȫת�������������ܳ�������ȡֵ��Χ����������������Ƿ�����������Ǹ������Ļ�����
				��saturate_cast�Խ������ת������ȷ����Ϊ��Чֵ��		
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


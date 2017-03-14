#include "Scanner_G6X00.h"
#include "public.h"
#include "uds_cap.h"
#include <time.h>
#include <vector>

#include "ximage.h"  // CXImage

#define DPI 100
#define SCANWIDTH_INCH   8.27
#define SCANLENGTH_INCH  /*118*/11.69 // ��ֽģʽ


extern HWND g_hwndDLG;
extern std::vector<std::string> g_vector_imagepath;
extern int g_nDeviceNumber;
extern void GetFilePath( char* szFileName, char* szFilePath);
/**
* Environment vars to get the Xfer Count.  Create this enviroment Varable on your system to simulate the 
* number of pages sitting in the scanner waiting to be scanned.
*/

#define kGETENV_XFERCOUNT "CAP_XFERCOUNT"

CScanner_G6X00::CScanner_G6X00(void)
	: m_nDocCount(0)
	, m_nSourceWidth(0)
	, m_nSourceHeight(0)
	, m_nImageNumber(0)
	, m_nScanLine(0)
	, m_nDestBytesPerRow(0)
	, m_bSkip(false)
{
	// set default cap value
	resetScanner();
	//LoadDLL();
}


CScanner_G6X00::~CScanner_G6X00(void)
{
	TerminateDriver();
	FreeLibrary(m_hDLL);
}

bool CScanner_G6X00::resetScanner()
{
	//::MessageBox(g_hwndDLG,TEXT("IN resetScanner()!"),MB_CAPTION,MB_OK);
	// Unlock the scanner 
	Unlock();

	m_nDocCount           = m_nMaxDocCount = getDocumentCount();// Reloaded the scanner with paper
	//Base����
	m_nPixelType          = TWPT_RGB; //ͼ������-��ɫ zhu
	m_fXResolution        = 200.0;
	m_fYResolution        = 200.0; //�ֱ���-200.0
	m_bDuplex             = false; //����/˫��-����
	m_fContrast           = 0.0; //�Աȶ�-0.0
	m_fBrightness         = 0.0; //����-0.0
	m_fThreshold          = 128.0; //��ֵ-128.0 ������Ĭ��128.G6400Ĭ��230
	m_bMultifeedDetection = true; //���ż��-ѡ��
	m_bMD_value           = false;
	//Advanced����
	m_nOrientation        = TWOR_ROT0; //zhu ֽ�ŷ���-����
	m_nStandardsizes      = TWSS_USLETTER; //zhu ��ӦICAP_SUPPORTEDSIZES��ֽ�Ŵ�С-TWSS_USLETTER
	m_nUnits              = TWUN_INCHES;  //zhu ��λ-Ӣ��

	m_nWidth              = 0;
	m_nHeight             = 0;

	m_fRotation           = 0.0;           //��ת-����תzhu
	m_nSpiltImage         = TWSI_NONE;     //zhu �ָ�-���ָ�
	m_fGamma              = 100.0;         //zhu GammaУ��-Ĭ��Ϊ100
	m_bMirror             = TWMR_DISABLE;  //����-��ѡ��

	m_nBinarization       = TWBZ_DYNATHRESHOLD;  //zhu ��ֵ��-��̬��ֵ
	m_bMultiStream        = false;               //�������-��ѡ��
	m_fSensitiveThreshold_removespots = 0.0;     //ȥ���ߵ�-0.0
	m_fSensitiveThreshold_colorretent = 128.0;   //��ɫ����-128.0

	//����ͼ����
	//Ĭ�ϲ�ѡ��
	m_fRemoveBlank        = TWRP_DISABLE; 
	m_bRemovePunch        = TWSP_DISABLE;
	m_bSharpen            = TWGM_DISABLE;
	m_bRemoveBack         = TWRB_DISABLE;
	m_bDescreen           = TWDS_DISABLE;
	m_bDenoise            = TWDN_DISABLE;
	m_bAutoCrop           = TWAC_DISABLE;

	m_fEdgeUp             = 0.0;      // ��Ե���� 
	m_fEdgeDown           = 0.0;      // ��Ե���� 
	m_fEdgeLeft           = 0.0;      // ��Ե���� 
	m_fEdgeRight          = 0.0;      // ��Ե���� 

	m_fXPos               = 0.0;      // Xƫ���� 
	m_fYPos               = 0.0;      // Yƫ���� 

	m_nImageNumber        = 0;

	m_byte_image = NULL;
	//m_bSkip = false;
	if (false == m_mat_image.empty())
	{
		m_mat_image.release();
	}

	//m_pCxImage = NULL;
	LoadDLL();
	InitDriverParamter();

	return true;
}

bool CScanner_G6X00::isFeederLoaded()
{
	//::MessageBox(g_hwndDLG,TEXT("isFeederLoaded()"),MB_CAPTION,MB_OK);

	bool rtn = true;
	//static bool bRunned = false;
	//if (bRunned)
	//{
	//	if ( (m_byteADFStatus & 0x1 ) != 1)
	//	{
	//		::MessageBox(g_hwndDLG,TEXT("No Paper!"),MB_CAPTION,MB_OK);
	//		m_nDocCount = 0;
	//		rtn =  false;
	//	}
	//}
	//bRunned = true;
	if(m_nDocCount<=0)
	{
		rtn = false;
		m_nDocCount = m_nMaxDocCount;// Reloaded the scanner with paper
	}
	return rtn;
}

short CScanner_G6X00::getDocumentCount() const
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

bool CScanner_G6X00::acquireImage()
{
	//::MessageBox(g_hwndDLG,TEXT("CScanner_G6X00::acquireImage!"),MB_CAPTION,MB_OK);
	//GetADFStatus(&m_byteADFStatus);
	//if ( (m_byteADFStatus & 0x1 ) != 1)
	//{
	//	::MessageBox(g_hwndDLG,TEXT("No Paper!"),MB_CAPTION,MB_OK);
	//	return false;
	//}
	int pixeltype;
	if(m_nPixelType == TWPT_BW)
	{
		m_nPixelType = TWPT_GRAY;
		pixeltype = TWPT_GRAY;
	}
	

	if(!m_bSkip)
	{
		static bool bFlag = false;

		if (!bFlag)
		{
			StartScanJob();
			AdjustParameter();
			SetParameter();
			bFlag = true;
		}

		if(m_pTempBuffer)                                 
		{
			delete []m_pTempBuffer;     
			m_pTempBuffer = NULL;
		}

		m_pTempBuffer = new BYTE[m_dwTotal];    
		memset(m_pTempBuffer,0,m_dwTotal);
		m_pSaveBuffer = m_pTempBuffer;

		StartScan();
		// ����2��ReadScanEx
		{
			memset(&m_ioStatus,0,sizeof(IO_STATUS));
			m_ioStatus.pBuffer = m_pTempBuffer;
			m_ioStatus.dwRequestedBytes = m_dwTotal;
			ReadScanEx(&m_ioStatus);
		}
		StopScan();
		m_pTempBuffer = m_pSaveBuffer; 


		{
			switch(m_scanParameter.BitsPerPixel)
			{
			case 1:							
				Invert(m_pSaveBuffer, m_dwTotal, m_scanParameter.BitsPerPixel);									
				break;
			case 24:
				RGB2BGR(m_pSaveBuffer, m_dwTotal, m_scanParameter.BitsPerPixel);												
				break;
			default:
				break;
			}
		}


		if (m_nSpiltImage  == TWSI_NONE )
		{
			GetADFStatus(&m_byteADFStatus);
			if ( (m_byteADFStatus & 0x1) != 1)
			{
				m_nDocCount = 0;
				EndScanJob ();
				//::MessageBox(g_hwndDLG,TEXT("m_nDocCount = 0!"),MB_CAPTION,MB_OK);
			}	
		}
		//GetADFStatus(&m_byteADFStatus);
		//if ( (m_byteADFStatus & 0x1) != 1)
		//{
		//	m_nDocCount = 0;
		//	EndScanJob ();
		//	//::MessageBox(g_hwndDLG,TEXT("m_nDocCount = 0!"),MB_CAPTION,MB_OK);
		//}			
	}



	if (m_pGammaTable)
	{
		delete []m_pGammaTable;
		m_pGammaTable = NULL;
	}


	if(pixeltype == TWPT_GRAY)
	{
		m_nPixelType = TWPT_BW;
	}

	//Document scanned, remove it from simulated intray
	//m_nDocCount --;

	// do whatever tranforms to the scanned image that was requested by the app
	// before the image is sent to the app.	
	if(false == preScanPrep())
	{
		return false;
	}


	return true;
}

void CScanner_G6X00::setSetting(CDevice_Base settings)
{
	CDevice_Base::setSetting(settings);  // ���ø���ķ���
	m_nDocCount = m_nMaxDocCount;
}

bool CScanner_G6X00::preScanPrep()
{
	//int nWidth = m_scanParameter.PixelNum; 
	//int	nHeight = m_scanParameter.LineNum;
	//int	nHeight = ios1.dwEffectiveLines;

	//if(m_nWidth <= 0 || m_nHeight <= 0)
	{
		m_nWidth  = m_nSourceWidth  = m_scanParameter.PixelNum;
		m_nHeight = m_nSourceHeight = m_scanParameter.LineNum;
	}
	//else
	//{
	//	// ��ȡӰ��Ŀ�ߣ���������Ϊ��λ 
	//	m_nSourceWidth   = m_scanParameter.PixelNum;
	//	m_nSourceHeight  = m_scanParameter.LineNum;
	//}

	m_dRat = (double)m_nSourceWidth/m_nSourceHeight;

	int nType = CV_8UC3;
	switch(m_nPixelType)
	{
	case TWPT_BW:
		nType = CV_8UC1; // �ڰ���Ҷ���ͬ
		break;
	case TWPT_GRAY:
		nType = CV_8UC1;
		break;
	case TWPT_RGB:
		nType = CV_8UC3;
		break;
	default:
		nType = CV_8UC3;
		break;
	}
	
	Mat iMat(m_nSourceHeight, m_nSourceWidth, nType, m_pSaveBuffer, m_dwBytesPerRow); 
	iMat.copyTo(m_mat_image);

	if(m_nPixelType == TWPT_BW)
	{
		threshold(m_mat_image, m_mat_image, m_fThreshold, 255, THRESH_OTSU);
	} 

	if(m_nOrientation == TWOR_LANDSCAPE) //����
	{		
		RotateImage(m_mat_image, m_mat_image, 90);
	}

	// ��ת
	switch((int)m_fRotation)
	{
	case TWOR_ROT0: 
		RotateImage(m_mat_image, m_mat_image, 0);
		break;								
	case TWOR_ROT90: 
		RotateImage(m_mat_image, m_mat_image, -90);
		break;										
	case TWOR_ROT180: 
		RotateImage(m_mat_image, m_mat_image, -180);
		break;										
	case TWOR_ROT270: 
		RotateImage(m_mat_image, m_mat_image, -270);
		break;									
	default: 
		break;					 
	} 

	//ͼ������
	if(m_bMirror == TWMR_AUTO)
	{ 
		Mat mat_hMirror;
		hMirrorTrans(m_mat_image, mat_hMirror);
		mat_hMirror.copyTo(m_mat_image);
	}


	//�Զ�������У��
	if(m_bAutoCrop == TWAC_AUTO) 
	{
		Mat matAutoCrop; 
		matAutoCrop = AutoCorrect(m_mat_image);//���Զ�У��	
		Rect rect = RemoveBlack(matAutoCrop);
		Mat imageSave = matAutoCrop(rect);
		imageSave.copyTo(m_mat_image);
		//matAutoCrop = RemoveBlack(matAutoCrop);
		//matAutoCrop.copyTo(m_mat_image);

		m_nWidth = m_mat_image.cols; 
		m_nHeight = m_mat_image.rows;
	}
	//imwrite("C:\\Users\\Administrator\\Desktop\\У��ͼ.jpg", m_mat_image);

	//ɫ�ʷ�ת
	if(m_bColorFlip == TWCF_AUTO)
	{ 
		Mat mat_hColorflip;
		ColorFlip(m_mat_image, mat_hColorflip);
		mat_hColorflip.copyTo(m_mat_image);
	}


	//ȥ������
	if(m_bDenoise == TWDN_AUTO) 
	{	
		Mat matDenoise;
		medianBlur(m_mat_image, matDenoise, 3);
		matDenoise.copyTo(m_mat_image);
		//IplImage *out;
	//	MedianSmooth(m_mat_image/*, out*/);
		//Mat mTemp(out,0);
		//mTemp.copyTo(m_mat_image);
	}

	//ȥ����
	if(m_bDescreen == TWDS_AUTO) 
	{
		//::MessageBox(g_hwndDLG,TEXT("ȥ����!"),MB_CAPTION,MB_OK);
		Mat matDescreen;
		m_mat_image.copyTo(matDescreen);
		//Size(5,5)ģ���С��Ϊ������Ҫ����ȷ��Ҫ�Լ�����ģ��
		//x���򷽲�
		//Y���򷽲�
		GaussianBlur(matDescreen, matDescreen, Size(5,5), 0, 0);  //  ��˹�˲�
		matDescreen.copyTo(m_mat_image);		
	}

	//ȥ������
	if(m_bRemovePunch == TWRP_AUTO) 
	{	 
		Mat matRemovepunch(m_mat_image);
		matRemovepunch = RemovePunch(matRemovepunch, 200, 30); //ȥ������ ԭ22
		matRemovepunch.copyTo(m_mat_image);

		m_nWidth = m_mat_image.cols; 
		m_nHeight = m_mat_image.rows;
	}


	//��
	int index = FindDepth(m_mat_image); //indexΪͼ������
	//��ͼ��
	if(m_bSharpen == TWSP_AUTO) 
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


	//ȥ���հ�ҳ
	bool bEmpty = false; //Ĭ�ϲ��ǿհ�ҳ
	if(m_bRemoveBlank == TWRA_AUTO)//checkbox����
	{
		Mat matRemoveBlank;
		m_mat_image.copyTo(matRemoveBlank);
		bEmpty = RemoveBlank(matRemoveBlank, m_fRemoveBlank);
	}

  if (bEmpty)
  {
		return false;
  }


	switch(m_nPixelType)
	{
	case TWPT_BW:
		//m_pCxImage->Negative();
		m_nDestBytesPerRow = BYTES_PERLINE(m_nWidth, 8);  // OpenCV��λ��1��ͼ��֧��
		break;
	case TWPT_GRAY:
		m_nDestBytesPerRow = BYTES_PERLINE(m_nWidth, 8);
		break;
	case TWPT_RGB:
		m_nDestBytesPerRow = BYTES_PERLINE(m_nWidth, 24);
		break;
	}

	
	//Mat����תΪ�ֽڶ����uchar,���������󣬷�������ͼ���������Ч
	Mat tempmat;
	m_mat_image.copyTo(tempmat);
	//BYTE *temp = NULL;
	Mat2uchar(tempmat);



	// setup some convenience vars because they are used during 
	// every strip request
	m_nScanLine       = 0;
	return true;
}

bool CScanner_G6X00::getScanStrip(BYTE *pTransferBuffer, DWORD dwRead, DWORD &dwReceived)
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

	if( m_nScanLine < MIN(m_nSourceHeight, (WORD)m_nHeight) ) //0<min(2250,2200)
	{
		//fill the buffer line by line to take care of alignment differences
		for(nRow = 0; nRow < nMaxRows; nRow++) //nMaxRows = 12
		{
			//get the next scan line position and copy it
			//pBits = MyGetScanLine(m_nScanLine);
			pBits = GetScanLine(m_nScanLine);

			memcpy( pTransferBuffer, pBits, MIN(m_nDestBytesPerRow, m_dwBytesPerRow)); //MIN(5100,4956) step��ʾ���ֽ�Ϊ��λ��ÿ�еĳ���

			// Check to see if the result image width is wider than what we have.
			// If it is wider fill it in with 0es
			if(m_nDestBytesPerRow > m_dwBytesPerRow)
			{
				memset( pTransferBuffer + m_dwBytesPerRow, 0, m_nDestBytesPerRow - m_dwBytesPerRow );
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

void CScanner_G6X00::Release()
{
	if(m_pSaveBuffer)                                 
	{
		delete []m_pSaveBuffer;     
		m_pSaveBuffer = NULL;
	}

	m_mat_image.release();
	if(m_byte_image)
	{
		free(m_byte_image);
		m_byte_image = NULL;
	}	
}

void CScanner_G6X00::InitDriverParamter()
{
	m_pGammaTable = NULL;
	m_pSaveBuffer= NULL;
	m_pTempBuffer = NULL;
	m_hDLL = NULL;
	m_dwTotal = 0;
	m_byteADFStatus = 0;
	m_dwBytesPerRow = 0;
	memset(&m_ioStatus, 0, sizeof(IO_STATUS));
	memset(&m_scannerAbility, 0, sizeof(SCANNERABILITY));
	memset(&m_scanParameter, 0, sizeof(SCANPARAMETER));
	memset(&m_ePixelBound, 0, sizeof(PIXELBOUND));
}

bool CScanner_G6X00::LoadDLL()
{
	//::MessageBox(g_hwndDLG,TEXT("Into LoadDLL!"),MB_CAPTION,MB_OK);
	TCHAR DllPath[MAX_PATH];

	if (DEVICE_G6400 == g_nDeviceNumber)
	{
		GetFilePath(FILENAME_DLL_GL1, DllPath);
	} 
	else
	{
		GetFilePath(FILENAME_DLL_GL2, DllPath);
	}

	//::MessageBox(g_hwndDLG, DllPath, MB_CAPTION,MB_OK);

  m_hDLL = LoadLibrary(DllPath);
	if(m_hDLL == NULL)
	{
		::MessageBox(g_hwndDLG, TEXT("Load Dll Failed!"), MB_CAPTION, MB_OK);
		return false;
	}

	InitializeDriver = (InitializeDriverProc)GetProcAddress(m_hDLL, "InitializeDriver");
	if(InitializeDriver == NULL)
	{
		::MessageBox(g_hwndDLG, TEXT("Load InitializeDriver Failed!"), MB_CAPTION, MB_OK);
		return false;
	}

	InitializeScanner = (InitializeScannerProc)GetProcAddress(m_hDLL, "InitializeScanner");
	if(InitializeScanner == NULL)
	{
		::MessageBox(g_hwndDLG, TEXT("Load InitializeScanner Failed!"), MB_CAPTION, MB_OK);
		return false;
	}

	GetScannerAbility = (GetScannerAbilityProc)GetProcAddress(m_hDLL, "GetScannerAbility");
	if(GetScannerAbility == NULL)
	{
		::MessageBox(g_hwndDLG, TEXT("Load GetScannerAbility Failed!"), MB_CAPTION, MB_OK);
		return false;
	}

	GetScannerAbilityEx = (GetScannerAbilityExProc)GetProcAddress(m_hDLL, "GetScannerAbilityEx");
	if(GetScannerAbilityEx == NULL)
	{
		::MessageBox(g_hwndDLG, TEXT("Load GetScannerAbilityEx Failed!"), MB_CAPTION, MB_OK);
		return false;
	}

	GetADFStatus = (GetADFStatusProc)GetProcAddress(m_hDLL, "GetADFStatus");
	if(GetADFStatus == NULL)
	{
		::MessageBox(g_hwndDLG, TEXT("Load GetADFStatus Failed!"), MB_CAPTION, MB_OK);
		return false;
	}

	StartScanJob = (StartScanJobProc)GetProcAddress(m_hDLL, "StartScanJob");
	if(StartScanJob == NULL)
	{
		::MessageBox(g_hwndDLG, TEXT("Load StartScanJob Failed!"), MB_CAPTION, MB_OK);
		return false;
	}

	SetScanParameter = (SetScanParameterProc)GetProcAddress(m_hDLL, "SetScanParameter");
	if(SetScanParameter == NULL)
	{
		::MessageBox(g_hwndDLG, TEXT("Load SetScanParameter Failed!"), MB_CAPTION, MB_OK);
		return false;
	}

	SetGammaTable = (SetGammaTableProc)GetProcAddress(m_hDLL, "SetGammaTable");
	if(SetGammaTable == NULL)
	{
		::MessageBox(g_hwndDLG, TEXT("Load SetGammaTable Failed!"), MB_CAPTION, MB_OK);
		return false;
	}

	StartScan = (StartScanProc)GetProcAddress(m_hDLL, "StartScan");
	if(StartScan == NULL)
	{
		::MessageBox(g_hwndDLG, TEXT("Load StartScan Failed!"), MB_CAPTION, MB_OK);
		return false;
	}

	ReadScan  = (ReadScanProc)GetProcAddress(m_hDLL, "ReadScan");
	if(ReadScan  == NULL)
	{
		::MessageBox(g_hwndDLG, TEXT("Load ReadScan Failed!"), MB_CAPTION, MB_OK);
		return false;
	}

	ReadScanEx = (ReadScanExProc)GetProcAddress(m_hDLL, "ReadScanEx");
	if(ReadScanEx == NULL)
	{
		::MessageBox(g_hwndDLG, TEXT("Load ReadScanEx Failed!"), MB_CAPTION, MB_OK);
		return false;
	}

	ReadScanEx2 = (ReadScanEx2Proc)GetProcAddress(m_hDLL, "ReadScanEx2");
	if(ReadScanEx2 == NULL)
	{
		::MessageBox(g_hwndDLG, TEXT("Load ReadScanEx2 Failed!"), MB_CAPTION, MB_OK);
		return false;
	}

	StopScan = (StopScanProc)GetProcAddress(m_hDLL, "StopScan");
	if(StopScan == NULL)
	{
		::MessageBox(g_hwndDLG, TEXT("Load StopScan Failed!"), MB_CAPTION, MB_OK);
		return false;
	}

	EndScanJob = (EndScanJobProc)GetProcAddress(m_hDLL, "EndScanJob");
	if(EndScanJob == NULL)
	{
		::MessageBox(g_hwndDLG, TEXT("Load EndScanJob Failed!"), MB_CAPTION, MB_OK);
		return false;
	}

	DoEjectPaperAfterScan = (DoEjectPaperAfterScanProc)GetProcAddress(m_hDLL, "DoEjectPaperAfterScan");
	if(DoEjectPaperAfterScan == NULL)
	{
		::MessageBox(g_hwndDLG, TEXT("Load DoEjectPaperAfterScan Failed!"), MB_CAPTION, MB_OK);
		return false;
	}

	TerminateDriver  = (TerminateDriverProc)GetProcAddress(m_hDLL, "TerminateDriver");
	if(TerminateDriver  == NULL)
	{
		::MessageBox(g_hwndDLG, TEXT("Load TerminateDriver Failed!"), MB_CAPTION, MB_OK);
		return false;
	}

	GetLastStatusCode  = (GetLastStatusCodeProc)GetProcAddress(m_hDLL, "GetLastStatusCode");
	if(GetLastStatusCode  == NULL)
	{
		::MessageBox(g_hwndDLG, TEXT("Load GetLastStatusCode Failed!"), MB_CAPTION, MB_OK);
		return false;
	}

	GetDeviceList  = (GetDeviceListProc)GetProcAddress(m_hDLL, "GetDeviceList");
	if(GetDeviceList  == NULL)
	{
		::MessageBox(g_hwndDLG, TEXT("Load GetDeviceList Failed!"), MB_CAPTION, MB_OK);
		return false;
	}

	DestroyDeviceList  = (DestroyDeviceListProc)GetProcAddress(m_hDLL, "DestroyDeviceList");
	if(DestroyDeviceList  == NULL)
	{
		::MessageBox(g_hwndDLG, TEXT("Load DestroyDeviceList Failed!"), MB_CAPTION, MB_OK);
		return false;
	}

	SelectDevice  = (SelectDeviceProc)GetProcAddress(m_hDLL, "SelectDevice");
	if(SelectDevice  == NULL)
	{
		::MessageBox(g_hwndDLG, TEXT("Load SelectDevice Failed!"), MB_CAPTION, MB_OK);
		return false;
	}

	GeneralOperation = (GeneralOperationProc)GetProcAddress(m_hDLL, "GeneralOperation");
	if(GeneralOperation  == NULL)
	{
		::MessageBox(g_hwndDLG, TEXT("Load GeneralOperation Failed!"), MB_CAPTION, MB_OK);
		return false;
	}

	ResetScanner = (ResetScannerProc)GetProcAddress(m_hDLL, "ResetScanner");
	if(ResetScanner  == NULL)
	{
		::MessageBox(g_hwndDLG, TEXT("Load ResetScanner Failed!"), MB_CAPTION, MB_OK);
		return false;
	}
	
	LampOn = (LampOnProc)GetProcAddress(m_hDLL, "LampOn");
	if(LampOn  == NULL)
	{
		::MessageBox(g_hwndDLG, TEXT("Load LampOn Failed!"), MB_CAPTION, MB_OK);
		return false;
	}

	GetBackgroundRaster = (GetBackgroundRasterProc)GetProcAddress(m_hDLL, "GetBackgroundRaster");
	if(GetBackgroundRaster  == NULL)
	{
		::MessageBox(g_hwndDLG, TEXT("Load GetBackgroundRaster Failed!"), MB_CAPTION, MB_OK);
		return false;
	}

	TuneScanLength = (TuneScanLengthProc)GetProcAddress(m_hDLL, "TuneScanLength");
	if(TuneScanLength == NULL)
	{
		::MessageBox(g_hwndDLG, TEXT("Load TuneScanLength Failed!"), MB_CAPTION, MB_OK);
		return false;
	}

	ReadNVMData = (ReadNVMDataProc)GetProcAddress(m_hDLL, "ReadNVMData");
	if(ReadNVMData == NULL)
	{
		::MessageBox(g_hwndDLG, TEXT("Load ReadNVMData Failed!"), MB_CAPTION, MB_OK);
		return false;
	}
	
	SetBatchScanParameter = (SetBatchScanParameterProc)GetProcAddress(m_hDLL, "SetBatchScanParameter");
	if(SetBatchScanParameter == NULL)
	{
		::MessageBox(g_hwndDLG, TEXT("Load SetBatchScanParameter Failed!"), MB_CAPTION, MB_OK);
		return false;
	}
	
	SetSpeedMode = (SetSpeedModeProc)GetProcAddress(m_hDLL, "SetSpeedMode");
	if(SetSpeedMode == NULL)
	{
		::MessageBox(g_hwndDLG, TEXT("Load SetSpeedMode Failed!"), MB_CAPTION, MB_OK);
		return false;
	}

	if (FALSE == InitializeDriver())
	{
		::MessageBox(g_hwndDLG, TEXT("Run InitializeDriver Failed!"), MB_CAPTION, MB_OK);
		return false;
	}

	if(FALSE == InitializeScanner())
	{
		::MessageBox(g_hwndDLG, TEXT("Run InitializeScanner Failed!"), MB_CAPTION, MB_OK);
		return false;
	}

	return true;
}

void CScanner_G6X00::CalGammaTable(LPBYTE GammaTable, int Brightness, int Contrast, BYTE Highlight, BYTE Shadow,float Gamma)
{
	int i, j;
	int Start, End;
	long ii, jj;
	float fPixel;
	for (i=0; i<255; i++)
		GammaTable[i] = i;
	Start = Contrast + ((WORD)Shadow*(255-Contrast*2)/255);
	End = Contrast + ((WORD)(255-Highlight)*(255-Contrast*2)/255);
	if ((Start>=0) && (End>=0))
	{
		for (i=0; i<Start; i++)
			GammaTable[i]=0;
		for (i=(255-End); i<256; i++)
			GammaTable[i]=255;
		for (i=Start, jj=0; i<(255-End); i++, jj++)
			GammaTable[i] = (BYTE)(jj*255/(255-Start-End));
	}
	else if ((Start>=0) && (End<0))
	{
		for (i=0; i<Start; i++)
			GammaTable[i]=0;
		for (i=Start, jj=0; i<256; i++, jj++)
			GammaTable[i] = (BYTE)(jj*255/(255-Start-End));
	}
	else if ((Start<0) && (End>=0))
	{
		for (i=(255-End); i<256; i++)
			GammaTable[i]=255;
		for (i=0, jj=0; i<(255-End); i++, jj++)
			GammaTable[i] = (BYTE)(jj*255/(255-Start-End)) - Start;
	}
	else //Start < 0 and End < 0
	{
		for (i=0, jj=0; i<256; i++, jj++)
			GammaTable[i] = (BYTE)(jj*255/(255-Start-End)) - Start;
	}
	if (Brightness!=0)
	{
		for (i=0; i<256; i++)
		{
			j = GammaTable[i];
			j += Brightness;
			if (j<0) j = 0;
			if (j>255) j = 255;
			GammaTable[i] = (BYTE)j;
		}
	}
	if (Gamma == 0)
		Gamma = 1;
	else
		Gamma = Gamma/100;
	Gamma = 1/Gamma;
	for (i=0; i<256; i++)
	{
		j = GammaTable[i];
		jj = j;
		fPixel = (float)jj;
		fPixel = fPixel / 255;
		fPixel = pow(fPixel, Gamma);
		fPixel = fPixel * 255;
		j = (BYTE)(fPixel+0.5);
		GammaTable[i] = j;
	}
}

void CScanner_G6X00::AdjustParameter()
{

	m_scanParameter.Left = static_cast<WORD>(m_fXPos);
	m_scanParameter.Top  = static_cast<WORD>(m_fYPos);
	switch(m_nPixelType)
	{
	case TWPT_BW:
		m_scanParameter.ScanMode = SMO_LINEART;
		break;
	case TWPT_GRAY:
		m_scanParameter.ScanMode = SMO_GRAY;
		break;
	case TWPT_RGB:
		m_scanParameter.ScanMode = SMO_COLOR_1PASS;
		break;
	default:
		m_scanParameter.ScanMode = SMO_COLOR_1PASS;  // Ĭ�ϲ�ɫ
		break;
	}

	if (m_bDuplex)
	{
		m_scanParameter.ScanMethod = SME_DUPLEX;
	} 
	else
	{
		m_scanParameter.ScanMethod = SME_ADFFRONT;
	}

	m_scanParameter.ScanSpeed = 0;
	m_scanParameter.Contrast = 0;                 //always keep 0 for newer model
	m_scanParameter.Brightness = 0 ;              //always keep 0 for newer model
	m_scanParameter.HTPatternNo =0;               // Halftone:1~5 ; Error Diffusion:6
	m_scanParameter.Highlight = 255;              //always keep 255 for newer model
	m_scanParameter.Shadow = 0;                   //always keep 0 for newer model
	m_scanParameter.ColorFilter = 0;	
	m_scanParameter.Invert = 0;                   //always keep 0 for newer model

	if (m_bMultifeedDetection)
	{
		if (m_bMD_value)  
		{
			m_scanParameter.ExtScanParam = 0x9018;  //�������ż�⣬��ͣɨ�� 
		} 
		else
		{
			m_scanParameter.ExtScanParam = 0x9410;  //�������ż�⣬����ɨ��
		}
	}
	else
	{
		m_scanParameter.ExtScanParam = 0x9000;   // �ر����ż��
	}
	//m_scanParameter.ExtScanParam = 0x9000 ;       //Enable extended scan parameter and set XRes and YRes for newer model
	//m_scanParameter.ExtScanParam2 |= 0x01000000;
	m_scanParameter.RExposure = 0;                //Keep original exposure time ,Not all scanner model can support exposure time control
	m_scanParameter.GExposure = 0;
	m_scanParameter.BExposure = 0;
	m_scanParameter.XRes = static_cast<WORD>(m_fXResolution);  // float->WORD
	m_scanParameter.YRes = static_cast<WORD>(m_fYResolution);  // float->WORD
	//m_scanParameter.CompressionArgument = 80;
	m_scanParameter.EnableBatchScan = 0x01;
	//m_scanParameter.IntelligentMultiFeedStyle = 1;
	//m_scanParameter.byTotalpages = 2;

	switch(m_scanParameter.ScanMode)
	{
	case SMO_LINEART:
		{
			m_ePixelBound = PB_LINEART;
			m_scanParameter.BitsPerPixel = 1;
		}
		break;
	case SMO_HALFTONE:
		{
			m_ePixelBound = PB_HALFTONE;
			m_scanParameter.BitsPerPixel = 1;
		}
		break;
	case SMO_GRAY:
		{
			m_scanParameter.BitsPerPixel = 8;
			m_ePixelBound = PB_DEFAULT;	
		}		
		break;
	case SMO_COLOR_1PASS:
		{
			m_scanParameter.BitsPerPixel = 24;
			m_ePixelBound = PB_DEFAULT;	
		}
		break;
	default:
		{
			m_scanParameter.BitsPerPixel = 24;
			m_ePixelBound = PB_DEFAULT;
		}
		break;
	}

	float fWeight = 0.0f;
	float fHeight = 0.0f;

	GetCurrentScanRange(m_nStandardsizes, fWeight, fHeight);	

	// Max: 8.5'*14'
	m_scanParameter.Width = static_cast<WORD>(fWeight * 300);  //A4     // 8.27*300    Unit: 1/300 inch
	m_scanParameter.Length = static_cast<WORD>(fHeight * 300);          // 11.69*300    Unit: 1/300 inch
	m_scanParameter.LineNum  =  static_cast<WORD>(fHeight * m_fXResolution);  

	WORD wOldPixelNum = static_cast<WORD>(fWeight * m_fXResolution);
	if(0 != wOldPixelNum % m_ePixelBound) 
	{
		m_scanParameter.PixelNum =  ((wOldPixelNum/m_ePixelBound) +1) * m_ePixelBound;
	}
	else
	{
		m_scanParameter.PixelNum = wOldPixelNum;
	}

	m_dwBytesPerRow = m_scanParameter.PixelNum * m_scanParameter.BitsPerPixel / 8; // һ����ռ���ֽ���	
	m_dwTotal = m_dwBytesPerRow * m_scanParameter.LineNum ;

}

void CScanner_G6X00::SetParameter()
{
	BatchScanParameter Param;
	memset(&Param,0,sizeof(BatchScanParameter));
	//Param.dwMaximumCachePage = 10;
	//Param.dwMaximumCacheSize = 640*1024*1024; // 640MB
	SetBatchScanParameter(&Param);
	SetScanParameter(&m_scanParameter);	
	m_pGammaTable = new BYTE[256];
	CalGammaTable(m_pGammaTable, m_fBrightness, m_fContrast, 255, 0, m_fGamma);  //CalGammaTable(m_pGammaTable,0,0,255,0,100);
	SetGammaTable(m_pGammaTable, 0x0, 256);
}

void CScanner_G6X00::GetCurrentScanRange(const int& _nSize, float& _fWeight, float& _fHeight)
{
	switch (_nSize)
	{
	case TWSS_USLETTER:
		{
			_fWeight = 8.5f;
			_fHeight = 11.0f;
		}
		break;
	case TWSS_USLEGAL:
		{
			_fWeight = 8.5f;
			_fHeight = 14.0f;
		}
		break;
	case TWSS_A4:
		{
			_fWeight = 8.27f;
			_fHeight = 11.69f;
		}
		break;
	case TWSS_A5:
		{
			_fWeight = 5.83f;
			_fHeight = 8.27f;
		}
		break;
	case TWSS_A6:
		{
			_fWeight = 4.13f;
			_fHeight = 5.83f;
		}
		break;
	case TWSS_A7:
		{
			_fWeight = 2.91f;
			_fHeight = 4.13f;
		}
		break;
	case TWSS_ISOB5:
		{
			_fWeight = 6.93f;
			_fHeight = 9.84f;
		}
		break;
	case TWSS_ISOB6:
		{
			_fWeight = 4.92f;
			_fHeight = 6.93f;
		}
		break;
	case TWSS_ISOB7:
		{
			_fWeight = 3.46f;
			_fHeight = 4.92f;
		}
		break;
	case TWSS_MAXSIZE:
		{
			_fWeight = 8.5f;
			_fHeight = 14.0f;
		}
		break;
	case UDSCAP_LONGDOCUMENT:
		{
			_fWeight = 8.5f;
			_fHeight = 118.0f;
		}
		break;
	}
}

BYTE* CScanner_G6X00::MyGetScanLine(int _nLine)
{
	BYTE *ps;
	ps = m_pSaveBuffer + m_dwBytesPerRow * _nLine;  
	return ps;
}

void CScanner_G6X00::Invert(BYTE* pBuffer, int length, int BitsPerPixel)
{
	if (pBuffer)
	{
		BYTE temp;
		for (int i=0; i<length; i++)
		{
			temp = ~pBuffer[i]; 
			pBuffer[i] = temp;
		}
	}
}

void CScanner_G6X00::RGB2BGR(BYTE *buffer, int length, int BitsPerPixel)
{
	if (buffer && (BitsPerPixel == 24))
	{
		BYTE temp;
		for (int i=0;i<length;i+=3)
		{
			temp = buffer[i]; buffer[i] = buffer[i+2]; buffer[i+2] = temp;
		}
	}
}

void CScanner_G6X00::RotateImage(const Mat &_src, Mat &_dst, double angle)
{
	double scale = 1; // ���ų߶� 
	cv::Mat src(_src);
	if (90 == angle || 270 == angle || -90 == angle || -270 == angle)
	{
		scale = m_dRat;
	}
	cv::Point2f center = cv::Point2f((float)src.cols / 2, 
		(float)src.rows / 2);  // ��ת����   	 

	cv::Mat rotateMat;   
	rotateMat = cv::getRotationMatrix2D(center, angle, scale);  

	cv::Mat rotateImg;  
	cv::warpAffine(src, rotateImg, rotateMat, src.size());  

	_dst = rotateImg;
}

void CScanner_G6X00::hMirrorTrans(const Mat &src, Mat &dst)
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

void CScanner_G6X00::Mat2uchar(Mat src_img)
{
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

BYTE * CScanner_G6X00::GetScanLine(int scanline)
{
	BYTE *ps;
	ps = m_byte_image + scanline * m_widthstep;  //memcpy(p2, p1, widthStep);
	return ps;
}

cv::Mat CScanner_G6X00::AutoCorrect(Mat src_img)
{
	//ChangeImage(IMAGENAME_AUTOCORRECT);
	//Mat img = imread(m_szSourceImagePath, CV_LOAD_IMAGE_UNCHANGED);
	//imwrite("C:\\Users\\Administrator\\Desktop\\ԭͼ.jpg", src_img);

	Mat img(src_img);
	resize(src_img, img, Size(m_nWidth/4, m_nHeight/4), (0, 0), (0, 0), cv::INTER_LINEAR);

	Mat srcImage;
	if (img.channels() != 1)
	{
		cvtColor(img, srcImage, CV_RGB2GRAY);
	} 
	else
	{
		img.copyTo(srcImage);
	}	

	int nRows = srcImage.rows;
	int nCols = srcImage.cols;

	//��ȡDFT�ߴ�***********************
	int cRows = getOptimalDFTSize(nRows);
	int cCols = getOptimalDFTSize(nCols);
	//����ͼ�񣬳����߽粿�����Ϊ0
	Mat sizeConvMat;
	copyMakeBorder(srcImage, sizeConvMat, 0, cRows-nRows, 0, cCols-nCols, BORDER_CONSTANT, Scalar::all(0));

	//DFT�任************************
	//ͨ���齨��
	Mat groupMats[] = {cv::Mat_<float>(sizeConvMat), cv::Mat::zeros(sizeConvMat.size(), CV_32F)};
	Mat mergeMat;
	//ͨ���ϲ�
	merge(groupMats, 2, mergeMat);
	//DFT�任
	dft(mergeMat, mergeMat);
	//����ͨ��
	split(mergeMat, groupMats);
	//�����ֵ
	magnitude(groupMats[0], groupMats[1], groupMats[0]);
	Mat magnitudeMat = groupMats[0].clone();
	//��һ������ֵ��1
	magnitudeMat += Scalar::all(1);
	//�����任
	log(magnitudeMat, magnitudeMat);
	//��һ��
	normalize(magnitudeMat, magnitudeMat, 0, 1, CV_MINMAX);
	//ͼ������ת��
	magnitudeMat.convertTo(magnitudeMat, CV_8UC1, 255, 0);

	//Ƶ�������ƶ�**************************
	int cx = (magnitudeMat.cols)/2;
	int cy = magnitudeMat.rows/2;
	Mat tmp;
	//Top-LeftΪ��һ���޴���ROI
	Mat q0(magnitudeMat, Rect(0,0,cx,cy));
	//Top_Right
	Mat q1(magnitudeMat, Rect(cx,0,cx,cy));
	//Bottom-Left
	Mat q2(magnitudeMat, Rect(0,cy,cx,cy));
	//Bottom-Right
	Mat q3(magnitudeMat, Rect(cx,cy,cx,cy));
	//�任����
	q0.copyTo(tmp);
	q3.copyTo(q0);
	tmp.copyTo(q3);

	q1.copyTo(tmp);
	q2.copyTo(q1);
	tmp.copyTo(q2);

	//��б�Ǽ��*************************************************
	//�̶���ֵ��ֵ������
	Mat binaryMagnMat;
	threshold(magnitudeMat, binaryMagnMat, 128, 255,CV_THRESH_BINARY);

	//����任
	vector<Vec2f> lines;
	binaryMagnMat.convertTo(binaryMagnMat, CV_8UC1, 255, 0);
	HoughLines(binaryMagnMat, lines, 1, CV_PI/180, 100, 0, 0);
	//����߸���
	Mat houghMat(binaryMagnMat.size(), CV_8UC3);
	//���Ƽ����
	for (size_t i=0; i<lines.size(); i++)
	{
		float rho = lines[i][0], theta = lines[i][1];
		Point pt1, pt2;
		//����任�����߱��ʽ
		double a=cos(theta), b=sin(theta);
		double x0=a*rho, y0=b*rho;
		pt1.x = cvRound(x0+1000*(-b));
		pt1.y = cvRound(y0+1000*(a));
		pt2.x = cvRound(x0-1000*(-b));
		pt2.y = cvRound(y0-1000*(a));
		line(houghMat, pt1, pt2, Scalar(0, 0, 255), 3, CV_AA);
	}

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
	bool mark = false;
	if (theta > 45)
	{
		mark = true;
	} 

	//�Ƕ�ת��
	float angleT = nRows*tan(theta/180*CV_PI)/nCols;  //�Ƕ�ת����
	theta = atan(angleT)*180/CV_PI;                   //����ת�Ƕ�

	nRows = src_img.rows;
	nCols = src_img.cols;

	//����任У��***********************
	float   degree = theta;
	double  angle  = degree*CV_PI/180;
	double  a = sin(angle), b = cos(angle);
	int     m_width_rotate = int(nRows *fabs(a) + nCols *fabs(b));
	int     m_height_rotate = int(nCols *fabs(a) + nRows *fabs(b));
	float   map[6];
	Mat     m_map_matrix(2,3,CV_32F, map);

	CvPoint2D32f center = cvPoint2D32f(nCols / 2, nRows / 2);  
	CvMat map_matrix2 = m_map_matrix;  
	cv2DRotationMatrix(center, degree, 1.0, &map_matrix2); 
	map[2] += (m_width_rotate - nCols)/2;
	map[5] += (m_height_rotate - nRows)/2;

	Mat m_image_out;
	m_image_out.create(Size(m_width_rotate, m_height_rotate), src_img.type());
	warpAffine(src_img, m_image_out, m_map_matrix, Size( m_width_rotate, m_height_rotate),1,0,0);
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


//cv::Mat CScanner_G6X00::RemoveBlack(Mat src_img)
Rect CScanner_G6X00::RemoveBlack(Mat src_img)
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
	
	return rect;
	//Mat imageSave = inputImg(rect);
	//return imageSave;
}

void CScanner_G6X00::ColorFlip(const Mat& src, Mat& dst)
{

	CV_Assert(src.depth() == CV_8U); //�������еı��ʽֵΪfalse���򷵻�һ��������Ϣ
	dst.create(src.rows, src.cols, src.type());

	int width = src.cols;
	int height = src.rows;

	for(int j = 0; j < width; j++)
	{
		for(int i = 0; i < height; i++)
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

void CScanner_G6X00::MedianSmooth(const Mat &src/*, IplImage *out*/)
{
	IplImage in = IplImage(src); /*Mat -> IplImage*/
	IplImage *out = cvCreateImage(cvGetSize(&in),IPL_DEPTH_8U,in.nChannels); 

	cvSmooth(&in, out, CV_MEDIAN, 3, in.nChannels);  //  ��ֵ�˲� medianBlur(src,dst,3);
	m_mat_image = out; //IplImage -> Mat
}

cv::Mat CScanner_G6X00::RemovePunch(const Mat &src, double threshold1, double threshold2)
{
	//ChangeImage(IMAGENAME_REMOVEPUNCH);
	//Mat src = imread(m_szSourceImagePath, CV_LOAD_IMAGE_UNCHANGED);
	Rect rect = RemoveBlack(src);
	Mat tempMat = src(rect);  //���ø���Ȥ����

	vector<Rect> rects;
	Rect rectTemp(0, 0, 3*tempMat.cols/30, 3*tempMat.rows/30); //����ֻȡʮ��֮һ,��rect�����Ҫ��3�ı���
	rects.push_back(Rect(0, 0, tempMat.cols, rectTemp.height)); //�ϲ�
	rects.push_back(Rect(0, tempMat.rows-rectTemp.height, tempMat.cols, rectTemp.height));	 //�²�	
	rects.push_back(Rect(0, rectTemp.height, rectTemp.width, tempMat.rows-2*rectTemp.height)); //���  ֻ���м䲿��
	rects.push_back(Rect(tempMat.cols-rectTemp.width, rectTemp.height, rectTemp.width, tempMat.rows-2*rectTemp.height)); //�Ҳ�

	vector<Mat> subImages;
	for(size_t i = 0; i < rects.size(); i++)
	{
		Mat tempImg;
		tempMat(rects[i]).copyTo(tempImg);
		tempImg = HoughCirclesTransfer(tempImg,1,threshold1,threshold2);
		subImages.push_back(tempImg);	
	}


	Mat dst_img;
	tempMat.copyTo(dst_img);	
	for(size_t i = 0; i < rects.size(); i++)
	{	
		IplImage IplHough = IplImage(dst_img);
		IplImage IplHoughTemp = IplImage(subImages[i]);

		cvSetImageROI(&IplHough, rects[i]);
		cvCopy(&IplHoughTemp, &IplHough);
		cvResetImageROI(&IplHough); 
	}

	dst_img.copyTo(tempMat, dst_img);	
	return src;
}

cv::Mat CScanner_G6X00::HoughCirclesTransfer(Mat src_img ,double dp, double threshold1, double threshold2)
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
	double minDist;//src_gray.rows/8: Ϊ����任��⵽��Բ��Բ��֮�����С����
	minDist = midImage.rows/15;
	double dFx = (double)m_fXResolution/200.00; //1
	WORD width = (m_nSourceWidth > m_nSourceHeight)?m_nSourceHeight:m_nSourceWidth; //1652
	WORD newWidth = (WORD)(width * dFx);
	int maxradius =  newWidth/30; //Բ�׵�ֱ��  55
	HoughCircles(midImage, circles, CV_HOUGH_GRADIENT, dp, minDist, threshold1, threshold2, 0, 2*maxradius/3);  //200,100 

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
		/*
		char buf[60];
		itoa(tempcenterx, buf, 10);
		::MessageBox(g_hwndDLG, TEXT(buf),"tempcenterx",MB_OK);
		itoa(tempcentery, buf, 10);
		::MessageBox(g_hwndDLG, TEXT(buf),"tempcentery",MB_OK);*/

		scalar = cvGet2D(&src_img_ipl, tempcentery, tempcenterx); //cvGet2D(ͼƬ y���꣬x����)��ȡ CvScalar����,��y,x����x,y
		if(radius < threshold2) //�������뾶С����ֵ2ʱ�����
		{
			circle(src_img, center, (int)(1.2*radius), scalar, -1, 8, 0 );
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

int CScanner_G6X00::FindDepth(const Mat &src_img)
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

int CScanner_G6X00::otsu(Mat image)
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

void CScanner_G6X00::SpiltImage(const Mat &src_img, int m, int n)
{
	int ceil_width  = src_img.cols/n; //��  
	int ceil_height = src_img.rows/m;   //��/m

	IplImage Iplsrc = IplImage(src_img);
	IplImage *Ipldst;
	Mat matTemp;


	if( false == m_bSkip) //��ֵĵ�һ��
	{
		cvSetImageROI(&Iplsrc,cvRect(0, 0, ceil_width, ceil_height)); 
		Ipldst = cvCreateImage(cvSize(ceil_width, ceil_height),  IPL_DEPTH_8U,  Iplsrc.nChannels); 

		cvCopy(&Iplsrc,Ipldst,0); 
		cvResetImageROI(&Iplsrc); 

		matTemp = Ipldst; //IplImage->Mat

		matTemp.copyTo(m_mat_image);
		cvReleaseImage(&Ipldst);  

		m_bSkip = true;  // ����Acquire����һ��ɨ��
	}
	else
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

		m_bSkip = false;
		GetADFStatus(&m_byteADFStatus);
		if ( (m_byteADFStatus & 0x1) != 1)
		{
			m_nDocCount = 0;
			EndScanJob ();
			//::MessageBox(g_hwndDLG,TEXT("m_nDocCount = 0!"),MB_CAPTION,MB_OK);
		}	
	}
	
}

bool CScanner_G6X00::RemoveBlank(Mat src_img, float fValue)
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



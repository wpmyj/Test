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
	, m_bMultiSkip(false)
	, m_nMultiTotal(0) 
  , m_nMultiBack(0) 
  , m_nMultiFront(0)    
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
	m_fRemoveBlank        = TWBP_DISABLE; 
	m_bRemoveBlank        = TWRA_DISABLE;
	m_bRemovePunch        = TWRP_DISABLE;
	m_bSharpen            = TWSN_DISABLE;
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
	//LoadDLL();
	if (false == LoadDLL())
	{
		return false;
	}
	InitDriverParamter();

	return true;
}

bool CScanner_G6X00::isFeederLoaded()
{
	//::MessageBox(g_hwndDLG,TEXT("isFeederLoaded()"),MB_CAPTION,MB_OK);
	bool rtn = true;
	if(m_nDocCount<=0)
	{
		rtn = false;
		m_nDocCount = m_nMaxDocCount;// Reloaded the scanner with paper
		Release(); // ��������������ڴ�
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
	//::MessageBox(g_hwndDLG,TEXT("acquireImage!"),MB_CAPTION,MB_OK);
	
	if (true == m_bMultiStream)
	{
		if(false == m_bMultiSkip)  // ��ͬһ��ֽ��Ҫ����ɨ��
		{
			if (false == RunScan())
			{
				return false;
			}
		}
	}
	else
	{
		// �ڰ�ͼ���ԻҶ�ģʽɨ��
		bool bChanged = false;  // ��־λ�������ж��Ƿ񽫺ڰ�ת�Ҷ�ɨ
		if(m_nPixelType == TWPT_BW)
		{
			m_nPixelType = TWPT_GRAY;
			bChanged = true;
		}

		if(false == m_bSkip)  // ����֣���ͬһ��ֽ��Ҫ����ɨ��
		{
			if (false == RunScan())
			{
				return false;
			}
		}
		// �ڰ�ͼ���ԻҶ�ģʽɨ�裬ɨ���ͼ�����͸���Ϊ�ڰ�
		if(true == bChanged)
		{
			m_nPixelType = TWPT_BW;
		}
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
	
	m_nWidth  = m_nSourceWidth  = m_scanParameter.PixelNum;
	m_nHeight = m_nSourceHeight = m_scanParameter.LineNum;
	// ����ԭ�򣺶���ɨ���������
	//if(m_bAutoCrop == TWAC_AUTO) // �Զ������ȸ�����Ч����
	//{
	//	m_nHeight = m_nSourceHeight = m_ioStatus.dwEffectiveLines;  
	//}
	
	m_dRat = (double)m_nSourceWidth/m_nSourceHeight;

	//�������
	if(m_bMultiStream)
	{
		BYTE byteLow = m_byteMultiValue;
		byteLow = byteLow & 0x0F;
		m_nMultiFront = BitCount(byteLow); //����λ��1�ĸ���(����)

		BYTE byteHigh = m_byteMultiValue;
		byteHigh = byteHigh & 0xF0;   
		m_nMultiBack = BitCount(byteHigh); //����λ��1�ĸ��������棩

		m_nMultiTotal = BitCount(m_byteMultiValue); //����1�ĸ���
		static int nTempCount = m_nMultiTotal;

		static bool bSet = false;
		if (true == bSet)  // ÿɨһ�Σ����¸�ֵ��ɨ����
		{
			nTempCount = m_nMultiTotal;  // ���¸�ֵ
		}

		if (nTempCount > 0)
		{
			Mat matMuilt(m_nSourceHeight, m_nSourceWidth, CV_8UC3, m_pSaveBuffer, m_dwBytesPerRow); // �������ǰ���ɫɨ��

			BYTE m_byteMuilt = m_byteMultiValue;
			m_byteMuilt = SwitchBYTE(m_byteMuilt);  // �ж�����������ɫ���ܹ�6�֣�
			m_mat_image = SetMuiltStream(matMuilt, m_byteMuilt);
			nTempCount --;

			if ((0 == m_nMultiFront) || (0 == m_nMultiBack)) // ֻ��һ��ѡ��
			{
				m_bMultiSkip = true;  // ��һ�Ų�ɨ
			} 
			else  // ˫��ѡ��
			{
				if (nTempCount == m_nMultiFront)  // �ȴ����棨��4λ�����ٴ�����(��4λ)
				{
					m_bMultiSkip = false;  // ��ȡ����
				}
				else
				{
					m_bMultiSkip = true;  // ��һ�Ų�ɨ
				}
			}

			if (0 == nTempCount)  // �������
			{
				bSet = true;
				m_bMultiSkip = false;
				GetADFStatus(&m_byteADFStatus);
				if ( (m_byteADFStatus & 0x1) != 1)  // ADF����ֽ����ֹͣɨ��
				{
					m_nDocCount = 0; 
					EndScanJob ();
				}	
			}
			else  // ����δ���
			{
				bSet = false;
			}
		}	
	}
	else  // �Ƕ���ɨ��
	{
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

		{
			Mat iMat(m_nSourceHeight, m_nSourceWidth, nType, m_pSaveBuffer, m_dwBytesPerRow); 
			iMat.copyTo(m_mat_image);
		}
	}


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
	//if(m_bAutoCrop == TWAC_AUTO) 
	if(m_nCutMethod == TWCT_AUTO)
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
	
	{
		Mat borderMat;
		copyMakeBorder(m_mat_image, borderMat, (int)temp[1], (int)temp[2]+(int)temp[3], (int)temp[4], (int)temp[5]+(int)temp[0], BORDER_CONSTANT, cv::Scalar(0,0,0)); //�Գ�����ʽ����߽�,ΪBORDER_CONSTANTʱ�����һ���������������ص�ֵ
		borderMat.copyTo(m_mat_image);
	}

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

	//::MessageBox(g_hwndDLG,TEXT("CScanner_G6X00::Release!"),MB_CAPTION,MB_OK);
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

bool CScanner_G6X00::AdjustParameter()
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

	if(m_bMultiStream)  // ����ѡ��
	{
		BYTE byteTemp, byteFront, byteRear;
		byteTemp = m_byteMultiValue;

		byteFront = byteTemp & 0X0F;  // ����
		byteTemp = m_byteMultiValue;
		byteRear = byteTemp & 0XF0;   // ����

		if ( (byteFront != 0X00) && (byteRear != 0X00) ) 	 // �����뱳�涼ѡ��
		{
			m_scanParameter.ScanMethod = SME_DUPLEX;
		}
		else if ((byteFront != 0X00) && (byteRear == 0X00)) // ����ѡ�У�����δѡ��
		{
			m_scanParameter.ScanMethod = SME_ADFFRONT;
		}
		else if ((byteFront == 0X00) && (byteRear != 0X00)) // ����δѡ�У�����ѡ��
		{
			m_scanParameter.ScanMethod = SME_ADFREAR;
		}
		else
		{
			::MessageBox(g_hwndDLG,TEXT("û�ж���ɨ��ѡ�ѡ��!"),MB_CAPTION,MB_OK);
			return false;
		}
		m_scanParameter.ScanMode = SMO_COLOR_1PASS;  // ����ʱ����ɫɨ��
	}
	else  // ��/˫�� ɨ
	{
		if (m_bDuplex)
		{
			m_scanParameter.ScanMethod = SME_DUPLEX;
		} 
		else
		{
			m_scanParameter.ScanMethod = SME_ADFFRONT;
		}
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

	return true;
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
	case TWSS_JISB5:
		{
			_fWeight = 7.17f;
			_fHeight = 10.12f;
		}
		break;
	case TWSS_JISB6:
		{
			_fWeight = 5.04f;
			_fHeight = 7.17f;
		}
		break;
	case TWSS_JISB7:
		{
			_fWeight = 3.58f;
			_fHeight = 5.04f;
		}
		break;
	case TWSS_MAXSIZE:
	case TWSS_NONE:
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
	default:  // Ĭ�ϰ����Χɨ
		{
			_fWeight = 8.5f;
			_fHeight = 14.0f;
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
	//imwrite("C:\\Users\\Administrator\\Desktop\\src_img.jpg", src_img);

	Mat srcImage;
	src_img.copyTo(srcImage);

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
		{	//��ȷ
			temp = dx/dy;
			theta = CV_PI/2.0 - atan(temp);
		}
		else
		{
			temp = dy/dx; 		
			theta = atan(temp); 
			mark = true;
		}
	}
	else
	{
		if(dx > fabs(dy))
		{
			temp = dx/fabs(dy);
			theta = atan(temp);
			mark = true;
		}
		else
		{
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
	cvReleaseImage(&out);
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

const TCHAR* CScanner_G6X00::TranslateError(const long error)
{
	TCHAR* szError = TEXT("UnKnown Error!");
	switch(error)
	{
	case CODE_SUCCESS: 
		szError = TEXT("Success");
		break;
	case ERROR_SHORT_LM9800_CHECK_ERROR: 
		szError = TEXT("Can not find scanner");
		break;
	case ERROR_SHORT_LM9800_RWTEST_FAILED: 
		szError = TEXT("Scanner R/W test failed");
		break;
	case ERROR_SHORT_SPP_NIBBLE_READ_ERROR: 
		szError = TEXT("Scanner connection failed");
		break;
	case ERROR_SHORT_PARTICULAR_RAM_TEST_FAILED: 
		szError = TEXT("Scanner RAM test failed");
		break;
	case ERROR_SHORT_RING_RAM_TEST_FAILED: 
		szError = TEXT("Scanner RAM test failed");
		break;
	case ERROR_SHORT_DMA_TEST_ERROR:                      
		szError = TEXT("Scanner DMA test/power cable failed");
		break;
	case ERROR_SHORT_CALIBRATION_ERROR:                      
		szError = TEXT("Calibration failed");
		break;
	case ERROR_SHORT_SCAN_ERROR:                      
		szError = TEXT("Scanner internal error");
		break;
	case ERROR_SHORT_PAPER_FEED_ERROR:                      
		szError = TEXT("Paper feed error");
		break;
	case ERROR_SHORT_SENSOR_ERROR:                      
		szError = TEXT("Sensor error. Unlock the scanner by turning the scanner lock to the use position");
		break;
	case ERROR_SHORT_OUTER_LIGHT_OFF:                      
		szError = TEXT("Transparency calibration error");
		break;	
	case ERROR_SHORT_SCSI_POWER_OFF:                      
		//szError = TEXT("Power off or Cable failure");
		szError = TEXT("��Դ�رջ���¹���");
		break;	
	case ERROR_SHORT_SCSI_REMOVE_RETAINER:                      
		//szError = TEXT("Please remove shipping retainer");
		szError = TEXT("���Ƴ�ɨ��������װ��");
		break;	
	case ERROR_SHORT_SCSI_PAPERJAM:                      
		//szError = TEXT("Paper Jam");
		szError = TEXT("��ֽ��");
		break;	
	case ERROR_SHORT_SCSI_ADF_OPEN:                      
		//szError = TEXT("ADF Cover Open");
		szError = TEXT("�Զ��ļ���ֽ��ǰ�ǿ���");
		break;	
	case ERROR_SHORT_SCSI_NO_PAPER:                      
		//szError = TEXT("No paper in Automatic Document Feeder");
		szError = TEXT("�Զ��ļ���ֽ����û��ֽ��");
		break;	
	case ERROR_SHORT_SCSI_SERERATION_SHEET_DETECTED :                      
		//szError = TEXT("Job separation sheet detected");
		szError = TEXT("��⵽�������뵥��");
		break;	
	case ERROR_SHORT_SCSI_NO_TRANSPARENCY_KIT:                      
		//szError = TEXT("Can't find transparency kit");
		szError = TEXT("Ҫʹ��ɨ��͸����Ƭ���ܣ��������Ȱ�װɨ��͸����Ƭ�׼�");
		break;	
	case ERROR_SHORT_SCSI_TRANS_KIT_IS_OFF:                      
		//szError = TEXT("Transparency kit switch is off");
		szError = TEXT("͸����Ƭ�׼����عر�");
		break;	
	case ERROR_SHORT_SCSI_INTERNAL_FAILURE:                      
		//szError = TEXT("Internal Target Failure");
		szError = TEXT("�ڲ�Ŀ�����");
		break;	
	case ERROR_SHORT_SCSI_FUSE_BLOWN1:                      
		//szError = TEXT("Fuse blown");
		szError = TEXT("����˿�۶�");
		break;	
	case ERROR_SHORT_SCSI_FUSE_BLOWN2:                      
		//szError = TEXT("Fuse blown");
		szError = TEXT("����˿�۶�");
		break;	
	case ERROR_SHORT_SCSI_FUSE_BLOWN3:                      
		//szError = TEXT("Fuse blown");
		szError = TEXT("����˿�۶�");
		break;	
	case ERROR_SHORT_SCSI_FUSE_BLOWN4:                      
		//szError = TEXT("Fuse blown");
		szError = TEXT("����˿�۶�");
		break;	
	case ERROR_SHORT_SCSI_SYSTEM_FAILURE:                      
		//szError = TEXT("Mechanical system failure");
		szError = TEXT("��еϵͳ����");
		break;
	case ERROR_SHORT_SCSI_OPTICAL_ERR:                      
		//szError = TEXT("Optical system failure");
		szError = TEXT("��ѧϵͳ����");
		break;	
	case ERROR_SHORT_SCSI_INVALID_PARAMETER1:                      
		//szError = TEXT("Illegal Scanner parameter");
		szError = TEXT("ɨ���ǲ������ô���");
		break;		
	case ERROR_SHORT_SCSI_INVALID_PARAMETER2:                      
		//szError = TEXT("Illegal Scanner parameter");
		szError = TEXT("ɨ���ǲ������ô���");
		break;		
	case ERROR_SHORT_SCSI_INVALID_PARAMETER3:                      
		//szError = TEXT("Illegal Scanner parameter");
		szError = TEXT("ɨ���ǲ������ô���");
		break;		
	case ERROR_SHORT_SCSI_INVALID_PARAMETER4:                      
		//szError = TEXT("Illegal Scanner parameter");
		szError = TEXT("ɨ���ǲ������ô���");
		break;		
	case ERROR_SHORT_SCSI_INVALID_PARAMETER5:                      
		//szError = TEXT("Illegal Scanner parameter");
		szError = TEXT("ɨ���ǲ������ô���");
		break;		
	case ERROR_SHORT_SCSI_MESSAGE_ERROR:                      
		//szError = TEXT("Message Error");
		szError = TEXT("��Ϣ����");
		break;			
	case ERROR_SHORT_SCSI_IMAGE_TRANSFER_ERR:                      
		//szError = TEXT("Image transfer error");
		szError = TEXT("ͼ�������");
		break;		
	case ERROR_SHORT_SCSI_UNKNOWN_REASON:                      
		//szError = TEXT("Unknown error reason");
		szError = TEXT("δ֪�Ĵ���");
		break;		
	case ERROR_SHORT_SCSI_NO_ASPI:                      
		//szError = TEXT("No ASPI managers were found");
		szError = TEXT("δ�ҵ��κ� ASPI ������");
		break;		
	case ERROR_SHORT_SCSI_NOSCSI_ASPI:                      
		//szError = TEXT("Can't find SCSI ASPI");
		szError = TEXT("δ�ҵ� SCSI ASPI");
		break;		
	case ERROR_SHORT_SCSI_ASPI_NOT_SUPPORT_WIN:                      
		//szError = TEXT("An ASPI manager which does not support windows is resident");
		szError = TEXT("һ����֧���Ӵ��ĳ�פ ASPI ������");
		break;		
	case ERROR_SHORT_SCSI_MEMORY_NOT_ENOUGH:                      
		//szError = TEXT("Memory is not enough");
		szError = TEXT("�ڴ治��");
		break;		
	case ERROR_SHORT_SCSI_NO_DLL:                      
		//szError = TEXT("Can't find winaspi.dll or wnaspi32.dll");
		szError = TEXT("δ�ҵ� winaspi.dll �� wnaspi32.dll");
		break;		
	case ERROR_SHORT_SCANNER_NOT_CONNECTED:                      
		//szError = TEXT("Can't find scanner");
		szError = TEXT("�Ҳ���ɨ����");
		break;		
	case ERROR_SHORT_SCSI_CANT_INITIAL_SCSI_MANAGER:                      
		//szError = TEXT("Can't initialize SCSI manager");
		szError = TEXT("�Ҳ�����ʼ���� SCSI ������");
		break;		
	case ERROR_SHORT_SCSI_TIME_OUT:                      
		szError = TEXT("Time out");
		break;		
	case ERROR_SHORT_SCSI_SCAN_HEAD_POSITION_ERR:                      
		szError = TEXT("Scan head positioning error");
		break;		
	case ERROR_SHORT_DRIVER_BUSY:                      
		//szError = TEXT("Driver Busy");
		szError = TEXT("��������æµ��");
		break;		
	case ERROR_SHORT_PARAMETER_CHECK_ERROR:                      
		//szError = TEXT("Parameter Check Error");
		szError = TEXT("��Ч��ɨ���ǲ���");
		break;		
	case ERROR_SHORT_ALLOCATE_MEMORY_FAILED:                      
		//szError = TEXT("Allocate Memory Failed");
		szError = TEXT("�����ڴ�ʧ��");
		break;		
	case ERROR_SHORT_DRIVER_ERROR:                      
		//szError = TEXT("Driver Initial Failed");
		szError = TEXT("������ʼ��ʧ��");
		break;		
	case ERROR_SHORT_SCANNER_ERROR:                      
		//szError = TEXT("Scanner internal error");
		szError = TEXT("ɨ���ǳ�ʼ��ʧ��");
		break;		
	case ERROR_SHORT_SCANNER_CONNECT_FAILED:                      
		//szError = TEXT("Scanner Connect Failed");
		szError = TEXT("ɨ��������ʧ��");
		break;		
	case ERROR_SHORT_BEFORE_READ_ERROR:                      
		//szError = TEXT("Before Read Image Error(May be calibration error)");
		szError = TEXT("��ȡͼ�����֮ǰ��������У׼����");
		break;		
	case ERROR_SHORT_READ_ERROR:                      
		//szError = TEXT("Read Image Error");
		szError = TEXT("��ȡͼ�����");
		break;		
	case ERROR_SHORT_END_ERROR:                      
		szError = TEXT("End Image Error");
		break;		
	case ERROR_SHORT_READ_ABORT:                      
		//szError = TEXT("Operation canceled(Read Abort, Maybe caused by user pushing cancel button or AP/TWAIN send cancel commands)");
		szError = TEXT("ɨ��ȡ��(��ȡֹͣ,�������û�����ȡ������APP����ȡ������)");
		break;		
	case ERROR_SHORT_SHUTTLE_INITIALIZE_FAILED:                      
		//szError = TEXT("Driver Initial Failed");
		szError = TEXT("������ʼ��ʧ��");
		break;		
	case ERROR_SHORT_SEND_SCAN_PARAMETER_ERROR:                      
		//szError = TEXT("Send parameter failed");
		szError = TEXT("��������ʧ��");
		break;		
	case ERROR_SHORT_SCANNER_BUSY:                      
		//szError = TEXT("Scanner Busy");
		szError = TEXT("ɨ������æ");
		break;		
	case ERROR_SHORT_SEND_ERROR:                      
		//szError = TEXT("Send Command Error");
		szError = TEXT("�����ʧ��");
		break;		
	case ERROR_SHORT_SCANNER_WARM_UP:                      
		//szError = TEXT("Scanner Warm Up");
		szError = TEXT("ɨ����ů����");
		break;		
	case ERROR_SHORT_POWER_TEST_OFF:                      
		//szError = TEXT("Power Test Off");
		szError = TEXT("�ѹػ�");
		break;		
	case ERROR_SHORT_FIRMWARE_DISAPPEAR:                      
		szError = TEXT("Firmware is disappear");
		break;		
	case ERROR_SHORT_DONT_DOWNLOAD_FIRMWARE:                      
		szError = TEXT("don't download firmware");
		break;		
	case ERROR_SHORT_PORT_DETECT_ERROR:                      
		szError = TEXT("Parallel Not Found");
		break;		
	case ERROR_SHORT_SEND_ADDRESS_TIMEOUT:                      
		//szError = TEXT("Scanner Connect Failed");
		szError = TEXT("ɨ��������ʧ��");
		break;		
	case ERROR_SHORT_SEND_DATA_TIMEOUT:                      
		//szError = TEXT("Scanner Connect Failed");
		szError = TEXT("ɨ��������ʧ��");
		break;		
	case ERROR_SHORT_READ_DATA_TIMEOUT:                      
		//szError = TEXT("Scanner Connect Failed");
		szError = TEXT("ɨ��������ʧ��");
		break;		
	case ERROR_SHORT_REGISTER_RW_ERROR:                      
		//szError = TEXT("Scanner Initial Failed");
		szError = TEXT("ɨ���ǳ�ʼ��ʧ��");
		break;		
	case ERROR_SHORT_GAMMA_RW_ERROR:                      
		//szError = TEXT("Scanner Initial Failed");
		szError = TEXT("ɨ���ǳ�ʼ��ʧ��");
		break;		
	case ERROR_SHORT_HOME_SENSOR_ERROR:                      
		szError = TEXT("Back to Home Position");
		break;		
	case ERROR_SHORT_SEARCH_START_ERROR:                      
		//szError = TEXT("Search Start Position Error");
		szError = TEXT("Ѱ�ҳ�ʼλ��ʧ��");
		break;		
	case ERROR_SHORT_LAMP_ERROR:                      
		szError = TEXT("Can't lamp on");
		break;		
	case ERROR_SHORT_LOCK_ERROR:                      
		//szError = TEXT("Scanner lock");
		szError = TEXT("ɨ��������װ�ùر�");
		break;		
	case ERROR_SHORT_INTERUPT_ERR:                      
		szError = TEXT("ERROR_SHORT_INTERUPT_ERR");
		break;		
	case ERROR_SHORT_BAD_PIXEL:                      
		szError = TEXT("Bad pixel");
		break;		
	case ERROR_SHORT_LOW_VOTAGE:                      
		szError = TEXT("row data value too low");
		break;		
	case ERROR_SHORT_HIGH_VOTAGE:                      
		szError = TEXT("row data value too high");
		break;		
	case ERROR_SHORT_END_OF_PAGE:                      
		szError = TEXT("end of page");
		break;		
	case ERROR_SHORT_WRONG_CHART:                      
		szError = TEXT("Wrong chart");
		break;		
	case ERROR_SHORT_DISK_SPACE_UNERROR_SHORTAILABLE:                      
		szError = TEXT("Disk have no enough space for storing temporary scanning data. (The disk may used for storing rear page of interlace duplex scan)");
		break;		
	case ERROR_SHORT_MULTI_FEED:                      
		//szError = TEXT("Multiple feed");
		szError = TEXT("�������Ž�ֽ��");
		break;		
	case ERROR_SHORT_ACCESSARIES_UNPLUGGED:                      
		szError = TEXT("Accessories such as ADF was plugged out");
		break;		
	case ERROR_SHORT_END_OF_PAGE_WITH_MULTI_FEED:                      
		szError = TEXT("End of page with multi feed occurred");
		break;		
	case ERROR_SHORT_FB_HOME_SENSOR_ERR:                      
		szError = TEXT("Flatbed Home Sensor Error");
		break;		
	case ERROR_SHORT_ADF_HOME_SENSOR_ERR:                      
		szError = TEXT("ADF Home Sensor Error");
		break;		
	case ERROR_SHORT_FB_DRAM_ERR:                      
		szError = TEXT("Flatbed DRAM Error");
		break;		
	case ERROR_SHORT_ADF_DRAM_ERR:                      
		szError = TEXT("ADF DRAM Error");
		break;		
	case ERROR_SHORT_FB_LAMP_ERR:                      
		szError = TEXT("Flatbed Lamp Error");
		break;		
	case ERROR_SHORT_ADF_LAMP_ERR:                      
		szError = TEXT("ADF front Lamp Error");
		break;		
	case ERROR_SHORT_FB_COVER_OPEN:                      
		szError = TEXT("Flatbed cover open");
		break;		
	case ERROR_SHORT_IP_FORMAT_INCORRECT:                      
		szError = TEXT("IP format incorrect(ASC=0x80 ASCQ=0x16)");
		break;		
	case ERROR_SHORT_IP_GETWAY_INVALID:                      
		szError = TEXT("Invalid IP getway(ASC=0x80 ASCQ=0x17)");
		break;		
	case ERROR_SHORT_IP_MASK_INVALID:                      
		szError = TEXT("Invalid IP mask(ASC=0x80 ASCQ=0x18)");
		break;		
	case ERROR_SHORT_IP_INVALID:                      
		szError = TEXT("Invalid IP (ASC=0x80 ASCQ=0x19)");
		break;		
	case ERROR_SHORT_IP_DISCONTINUEOUS:                      
		szError = TEXT("Discontinuous mask(ASC=0x80 ASCQ=0x1A)");
		break;		
	case ERROR_SHORT_SYSTEM_ERROR:                      
		//szError = TEXT("Operation system return error. (On Windows platform caller could use GetLastError() to get detail reason.)");
		szError = TEXT("����ϵͳ����");
		break;		
	case ERROR_SHORT_INTERNAL_ERROR:                      
		szError = TEXT("Driver Internal(unknown) Error");
		break;		
	case ERROR_SHORT_ADF_REAR_LAMP_ERR:                      
		szError = TEXT("ADF Rear Lamp Error");
		break;		
	case ERROR_SHORT_SKEW_ERROR:                      
		szError = TEXT("Occur skew");
		break;		
	case ERROR_SHORT_NEED_CALIBRATION:                      
		szError = TEXT("Need calibration first");
		break;		
	case ERROR_SHORT_FUNCTION_SENSOR_ERROR:                      
		szError = TEXT("Function sensor return error. Call tag AG_FUNCTION_SENSOR_WORK_STATUS for more detail.");
		break;	
	case ERROR_LONG_DRIVER_BUSY:                      
		//szError = TEXT("Driver Busy");
		szError = TEXT("��������æµ��");
		break;	
	case ERROR_LONG_SCANNER_BUSY:                      
		//szError = TEXT("Scanner Busy");
		szError = TEXT("ɨ����æµ��");
		break;	
	case ERROR_LONG_PORT_BUSY:                      
		szError = TEXT("IO port is busy");
		break;	
	case ERROR_LONG_HANDSHAKE_TIMEOUT:                      
		szError = TEXT("Hand Shake Time Out");
		break;	
	case ERROR_LONG_SCANNER_CONNECT_FAILED:                      
		//szError = TEXT("Scanner Connect Failed");
		szError = TEXT("ɨ��������ʧ��");
		break;	
	case ERROR_LONG_PARAMETER_CHECK_ERROR:                      
		//szError = TEXT("Parameter Check Error");
		szError = TEXT("��Ч��ɨ���ǲ���");
		break;	
	case ERROR_LONG_ALLOCATE_MEMORY_FAILED:                      
		//szError = TEXT("Allocate Memory Failed");
		szError = TEXT("�����ڴ�ʧ��");
		break;	
	case ERROR_LONG_LAMP_ERROR:                      
		//szError = TEXT("Can't Lamp On");
		szError = TEXT("�޷������ƹ�");
		break;	
	case ERROR_LONG_LOCK_ERROR:                      
		//szError = TEXT("Scanner locked");
		szError = TEXT("ɨ��������װ�ùر�");
		break;	
	case ERROR_LONG_SCANNER_WARM_UP:                      
		//szError = TEXT("Scanner Warm up");
		szError = TEXT("ɨ����ů����");
		break;	
	case ERROR_LONG_READ_ABORT:                      
		//szError = TEXT("Read Abort. For push stop button");
		szError = TEXT("����ֹͣ����, ɨ���ж�");
		break;	
	case ERROR_LONG_REGISTER_RW_FAILED:                      
		szError = TEXT("Register R/W Verify Failed");
		break;	
	case ERROR_LONG_MEMORY_RW_FAILED:                      
		szError = TEXT("Memory R/W Verify Failed");
		break;	
	case ERROR_LONG_PAPER_JAM:                      
		//szError = TEXT("Paper Jam");
		szError = TEXT("��ֽ");
		break;	
	case ERROR_LONG_ADF_COVER_OPEN:                      
		//szError = TEXT("ADF Cover Open");
		szError = TEXT("�Զ��ļ���ֽ��ǰ�ǿ���");
		break;	
	case ERROR_LONG_NO_PAPER_IN_ADF:                      
		//szError = TEXT("No paper in ADF");
		szError = TEXT("�Զ��ļ���ֽ����û��ֽ��");
		break;	
	case ERROR_LONG_NOT_FOUND_XPA_KIT:                      
		//szError = TEXT("Can't find Transparency kit");
		szError = TEXT("Ҫʹ��ɨ��͸����Ƭ���ܣ��������Ȱ�װɨ��͸����Ƭ�׼�");
		break;	
	case ERROR_LONG_XPA_KIT_POWER_OFF:                      
		//szError = TEXT("Transparency Kit power off");
		szError = TEXT("͸����Ƭ�׼����عر�");
		break;	
	case ERROR_LONG_NOT_FOUNT_WINASPI:                      
		//szError = TEXT("Can't find winaspi");
		szError = TEXT("�Ҳ��� winaspi.dll �� wnaspi32.dll");
		break;	
	case ERROR_LONG_NOT_FOUND_SCANNER:                      
		//szError = TEXT("Can't find scanner");
		szError = TEXT("δ�ҵ�ɨ����");
		break;	
	case ERROR_LONG_CALIBRATION_ERROR:                      
		//szError = TEXT("calibration fail");
		szError = TEXT("ɨ����У��ʧ��");
		break;	
	case ERROR_LONG_HOME_SENSOR_ERROR:                      
		//szError = TEXT("Back to Home Position Failed");
		szError = TEXT("������ʼ��ʧ��");
		break;	
	case ERROR_LONG_END_OF_PAGE:                      
		szError = TEXT("end of page");
		break;	
	case ERROR_LONG_SYSTEM_ERROR:                      
		//szError = TEXT("Operation system returns error");
		szError = TEXT("����ϵͳ����");
		break;	
	case ERROR_LONG_WRITE_NVM_ERROR:                      
		szError = TEXT("Write NVM returns error");
		break;	
	case ERROR_LONG_PAPER_IN_MULTI_TRAY:                      
		szError = TEXT("Papers were fed from multiple tray");
		break;	
	case ERROR_LONG_FRONT_DOOR_OPEN:                      
		szError = TEXT("MFP printer's Front door open");
		break;	
	case ERROR_LONG_END_OF_PAGE_MULTI_FEED:                      
		szError = TEXT("End of page with multi feed occurred");
		break;	
	case ERROR_LONG_ENDORSER_ADF_COVER_OPEN:                      
		szError = TEXT("Endorser ADF Cover Open");
		break;	
	case ERROR_LONG_ENDORSER_PAPER_JAM:                      
		szError = TEXT("Endorser Paper Jam");
		break;	
	case ERROR_LONG_ADF_SLIDER_OPEN:                      
		szError = TEXT("The ADF slider cover is not closed");
		break;	
	case ERROR_LONG_NEED_PREPARE_DATA:                      
		szError = TEXT("Need to call GetPrepareStatus() to wait status to ready");
		break;	
	case ERROR_LONG_WAIT_MEDIA:                      
		szError = TEXT("Wait media into scanner");
		break;	
	case ERROR_LONG_SKEW_ERROR:                      
		szError = TEXT("Occur skew");
		break;	
	case ERROR_LONG_FUNCTION_SENSOR_ERROR:                      
		szError = TEXT("Function sensor return error. Call tag TAG_FUNCTION_SENSOR_WORK_STATUS for	more detail");
		break;	
	case ERROR_LONG_LOGICAL_UNIT_ERROR:                      
		szError = TEXT("Logical Unit Not Supported	(ASC=0x25 ASCQ=0x00)");
		break;	
	case ERROR_LONG_ULTRASONIC_ERROR:                      
		szError = TEXT("Ultrasonic error (ASC=0x15 ASCQ=0x05)	");
		break;	
	case ERROR_LONG_AFE_RW_TEST_ERROR:                      
		szError = TEXT("AFE R/W test error (ASC=0x44 ASCQ=0x04)");
		break;	
	case ERROR_LONG_UNKNOW_ERROR:                      
		//szError = TEXT("unknown error");
		szError = TEXT("δ֪����");
		break;	
	case ERROR_LONG_FB_HOME_SENSOR_ERR:                      
		//szError = TEXT("Flatbed Home Sensor Error (ASC=0x15 ASCQ=0x02)");
		szError = TEXT("ƽ��ԭ�����ʧ�� (ASC=0x15 ASCQ=0x02)");
		break;	
	case ERROR_LONG_ADF_HOME_SENSOR_ERR:                      
		//szError = TEXT("ADF Home Sensor Error (ASC=0x15 ASCQ=0x03)");
		szError = TEXT("�Զ���ֽ��ԭ�����ʧ�� (ASC=0x15 ASCQ=0x03)");
		break;	
	case ERROR_LONG_FB_DRAM_ERR:                      
		//szError = TEXT("Flatbed DRAM Error (ASC=0x44 ASCQ=0x01)");
		szError = TEXT("ƽ��DRAM���� (ASC=0x44 ASCQ=0x01)");
		break;	
	case ERROR_LONG_ADF_DRAM_ERR:                      
		//szError = TEXT("ADF DRAM Error (ASC=0x44 ASCQ=0x02)");
		szError = TEXT("�Զ���ֽ��DRAM���� (ASC=0x44 ASCQ=0x02)");
		break;	
	case ERROR_LONG_FB_LAMP_ERR:                      
		//szError = TEXT("Flatbed Lamp Error (ASC=0x60 ASCQ=0x01)");
		szError = TEXT("ƽ��ƹܴ��� (ASC=0x60 ASCQ=0x01)");
		break;	
	case ERROR_LONG_ADF_LAMP_ERR:                      
		//szError = TEXT("ADF Lamp Error (ASC=0x60 ASCQ=0x02)");
		szError = TEXT("�Զ���ֽ���ƹܴ��� (ASC=0x60 ASCQ=0x02)");
		break;	
	case ERROR_LONG_FB_COVER_OPEN:                      
		//szError = TEXT("Flatbed cover open (ASC=0x80 ASCQ=0x07)");
		szError = TEXT("ƽ���ļ��ϸǿ��� (ASC=0x80 ASCQ=0x07)");
		break;	
	case ERROR_LONG_ACCESSARIES_UNPLUGGED:                      
		szError = TEXT("Accessaries such as ADF was pluged out");
		break;	
	case ERROR_LONG_PREFEEDING:                      
		szError = TEXT("Prefeeding");
		break;	
	case ERROR_LONG_ADF_REAR_LAMP_ERR:                      
		szError = TEXT("ADF Lamp Error (ASC=0x60 ASCQ=0x03)");
		break;	
	case ERROR_LONG_NEED_CALIBRATION:                      
		szError = TEXT("Need calibration first");
		break;	
	case ERROR_LONG_SCANNER_BUSY_CURRENT_TASK:                      
		szError = TEXT("Scanner busy for current task (Job)");
		break;	
	case ERROR_LONG_MULTI_FEED:                      
		//szError = TEXT("Multiple feed");
		szError = TEXT("�������Ž�ֽ��");
		break;	
	case ERROR_LONG_MANUAL_CALIBRATION_FAIL:                      
		szError = TEXT("Manual Calibration Fail");
		break;	
	case ERROR_LONG_MANUAL_CALIBRATION_FAIL_LEAD_EDGE_FAIL:                      
		szError = TEXT("Manual Calibration Fail - Lead Edge Fail");
		break;	
	case ERROR_LONG_MANUAL_CALIBRATION_FAIL_SIDE_EDGE_FAIL:                      
		szError = TEXT("Manual Calibration Fail - Side Edge Fail");
		break;	
	case ERROR_LONG_MANUAL_CALIBRATION_FAIL_SHADING_TARGET_FAIL:                      
		szError = TEXT("Manual Calibration Fail - Shading Target Fail");
		break;	
	case ERROR_LONG_PAPER_JAM2:                      
		//szError = TEXT("Paper Jam 2");
		szError = TEXT("�������Ž�ֽ��");
		break;	
	case ERROR_LONG_PAPER_JAM3:                      
		//szError = TEXT("Paper Jam 3");
		szError = TEXT("�������Ž�ֽ��");
		break;	
	case ERROR_LONG_PAPER_JAM4:                      
		//szError = TEXT("Paper Jam 4");
		szError = TEXT("�������Ž�ֽ��");
		break;	
	case ERROR_LONG_PICK_UP_ROLLER_ERROR:                      
		szError = TEXT("Pick Up Roller");
		break;	
	case ERROR_LONG_ENDORSER_MODULE_ERR:                      
		szError = TEXT("Endorser module not installed");
		break;	
	case ERROR_LONG_ENDORSER_HEAD_ERR:                      
		szError = TEXT("Printer head not installed");
		break;	
	case ERROR_LONG_ADF_NEXT_PAGE_MISS_FEED:                      
		//szError = TEXT("Next page miss feed");
		szError = TEXT("�Զ���ֽ��©ֽ��");
		break;	
	case ERROR_LONG_ADF_NEXT_PAGE_GAP_ERR:                      
		szError = TEXT("Next page too close current page");
		break;	
	case ERROR_LONG_CUSTOM_PAPER_JAM_MIN:                      
		szError = TEXT("ERROR_LONG_CUSTOM_PAPER_JAM_MIN.Mapping paper jam error (ASC=0x80 ASCQ=0x01) when additional information BYTE 3 if not zero. Mapping code = 3301 +Information BYTE 3");
		break;	
	case ERROR_LONG_CUSTOM_PAPER_JAM_MAX:                      
		szError = TEXT("ERROR_LONG_CUSTOM_PAPER_JAM_MAX.Mapping paper jam error (ASC=0x80 ASCQ=0x01) when additional information BYTE 3 if not zero. Mapping code = 3301 +Information BYTE 3");
		break;	
	case ERROR_LONG_CARD_LENGTH_EXCEED:                      
		szError = TEXT("Card Length > 2.5 Inch an error,for AW820");
		break;	
	case ERROR_LONG_IP_FORMAT_INCORRECT:                      
		szError = TEXT("IP format incorrect	(ASC=0x80 ASCQ=0x16)");
		break;	
	case ERROR_LONG_IP_GETWAY_INVALID:                      
		szError = TEXT("Invalid IP getway	(ASC=0x80 ASCQ=0x17)");
		break;	
	case ERROR_LONG_IP_MASK_INVALID:                      
		szError = TEXT("Invalid IP mask	(ASC=0x80 ASCQ=0x18)");
		break;	
	case ERROR_LONG_IP_INVALID:                      
		szError = TEXT("Invalid IP (ASC=0x80 ASCQ=0x19)");
		break;	
	case ERROR_LONG_IP_DISCONTINUEOUS:                      
		szError = TEXT("Discontinuous mask	(ASC=0x80 ASCQ=0x1A)");
		break;	
	case ERROR_LONG_NO_FRONT_ADF:                      
		szError = TEXT("If front ADF can't work or disable");
		break;	
	case ERROR_LONG_NO_BACK_ADF:                      
		szError = TEXT("If rear ADF can't work or disable");
		break;	
	case ERROR_LONG_NO_ADF:                      
		szError = TEXT("If front and rear both ADF can't work or disable");
		break;	
	case ERROR_LONG_NO_FLATBED:                      
		szError = TEXT("If flatbed can't work or disable");
		break;	
	case ERROR_LONG_NO_IMPRINTERF:                      
		szError = TEXT("If imprinter can't work or disable");
		break;	
	case ERROR_LONG_NO_MULTIFEED:                      
		szError = TEXT("If multifeed can't work or disable");
		break;	
	case ERROR_LONGINF_INFO_WARM_UP:                      
		szError = TEXT("Scanner Warm up, call GetLastStatusCode() and wait status to ready (AJAX_SUCCESS)");
		break;	
	case ERROR_LONGINF_INFO_NEED_PREPARE_DATA:                      
		szError = TEXT("Need prepare data, call GetLastStatusCode() and wait status to ready (AJAX_SUCCESS)");
		break;	
	case ERROR_LONGINF_BUSY_CALIBRATION:                      
		szError = TEXT("Busy calibration, call GetLastStatusCode() and wait status to ready (AJAX_SUCCESS)");
		break;
	}

	return szError;
}

bool CScanner_G6X00::RunScan()
{
	//GetADFStatus(&m_byteADFStatus);
	//if ( (m_byteADFStatus & 0x1 ) != 1)
	//{
	//	::MessageBox(g_hwndDLG,TEXT("No Paper!"),MB_CAPTION,MB_OK);
	//	return false;
	//}

	//static bool bFlag = false;
	//if (!bFlag)  // ����ԭ��EndScanJob()ִ�к󣬱������ٴ�ִ��StartScanJob()����������
	{
		StartScanJob();
		if (!AdjustParameter())
		{
			return false;
		}
		SetParameter();
		//bFlag = true;
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

	// �ж��Ƿ��д���
	{
		long lStatus;
		GetLastStatusCode(NULL, &lStatus);
		if ( CODE_SUCCESS != lStatus )
		{
			::MessageBox(g_hwndDLG,TranslateError(lStatus),MB_CAPTION,MB_ICONERROR);						
			m_nDocCount = 0;
			EndScanJob ();
			return false;
		}
	}

	m_pTempBuffer = m_pSaveBuffer; 

	// ԭʼͼ����Ҫ��������ʹ��
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

	// �Ƕ���ɨ��ģʽ����ADF����ֽʱֹͣɨ��
	if (false == m_bMultiStream)
	{
		GetADFStatus(&m_byteADFStatus);
		if ( (m_byteADFStatus & 0x1) != 1)
		{
			m_nDocCount = 0;
			EndScanJob ();
		}	
	}

	if (m_pGammaTable)
	{
		delete []m_pGammaTable;
		m_pGammaTable = NULL;
	}

	return true;
}

cv::Mat CScanner_G6X00::SetMuiltStream(Mat src_img, BYTE muilt)
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
			//::MessageBox(g_hwndDLG,TEXT("��ɫ����!"),MB_CAPTION,MB_OK);
		}	
		break;
	case 0x02:  //�Ҷȵ���
	case 0x20:
		{
			m_nPixelType = TWPT_GRAY;
			cvtColor(src_img, src_img, CV_BGR2GRAY);
			src_img.copyTo(dst_img);
			//::MessageBox(g_hwndDLG,TEXT("�Ҷȵ���!"),MB_CAPTION,MB_OK);
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
			threshold(src_img, src_img, 0, 255, THRESH_OTSU);
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
	}

	return dst_img;
}

BYTE CScanner_G6X00::SwitchBYTE(const BYTE src)
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

int CScanner_G6X00::BitCount(BYTE n)
{
	unsigned int c = 0;
	for(c = 0; n; ++c)
	{
		n &= (n - 1) ; // ������λ��1 ��ͬ��n = n&(n-1);
	}
	return c;
}



#include "Scanner_G6X00.h"
#include "public.h"
#include "uds_cap.h"
#include <time.h>
#include <vector>

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
	GetADFStatus(&m_byteADFStatus);
	if ( (m_byteADFStatus & 0x1 ) != 1)
	{
		::MessageBox(g_hwndDLG,TEXT("No Paper!"),MB_CAPTION,MB_OK);
		return false;
	}

	static bool bFlag = false;

	if (!bFlag)
	{
		StartScanJob();
		AdjustParameter();
		SetParameter();
		bFlag = true;
	}

	//while(1)
	//{

		if(m_pTempBuffer)                                 
		{
			delete []m_pTempBuffer;     
			m_pTempBuffer = NULL;
		}

		m_pTempBuffer = new BYTE[m_dwTotal];    
		memset(m_pTempBuffer,0,m_dwTotal);
		m_pSaveBuffer = m_pTempBuffer;

		//CString str2,str3;
		//char* str1 = "C:\\image\\";
		//str2 = NameFile(TEXT("raw"));
		//str3.Format("%s%s",str1,str2);

		//FILE *fptr_RAW=fopen("C:\\test.raw","wb");

		StartScan();
		// ����2��ReadScanEx
		{
			memset(&m_ioStatus,0,sizeof(IO_STATUS));
			m_ioStatus.pBuffer = m_pTempBuffer;
			m_ioStatus.dwRequestedBytes = m_dwTotal;
			ReadScanEx(&m_ioStatus);
			//fwrite(m_pSaveBuffer,m_dwTotal,1,fptr_RAW);
		}
		StopScan();
		m_pTempBuffer = m_pSaveBuffer; 
		//fwrite(m_pSaveBuffer,m_dwTotal,1,fptr_RAW);
		//fclose(fptr_RAW);

		GetADFStatus(&m_byteADFStatus);
		if ( (m_byteADFStatus & 0x1) != 1)
		{
			m_nDocCount = 0;
			EndScanJob ();
			//::MessageBox(g_hwndDLG,TEXT("m_nDocCount = 0!"),MB_CAPTION,MB_OK);
		}		
		//break;	
	
	//}			
	//EndScanJob ();//Please see scan flow issue 1		

	//if(m_pSaveBuffer)                                 
	//{
	//	delete []m_pSaveBuffer;     
	//	m_pTempBuffer = NULL;
	//	m_pSaveBuffer = NULL;
	//}

	if (m_pGammaTable)
	{
		delete []m_pGammaTable;
		m_pGammaTable = NULL;
	}


	//Document scanned, remove it from simulated intray
	//m_nDocCount--;

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


	//int nWidth = m_scanParameter.PixelNum; 
	//int	nHeight = m_scanParameter.LineNum;
	//int	nHeight = ios1.dwEffectiveLines;


	if(m_nWidth <= 0 || m_nHeight <= 0)
	{
		m_nWidth  = m_nSourceWidth  = m_scanParameter.PixelNum;
		m_nHeight = m_nSourceHeight = m_scanParameter.LineNum;
	}
	else
	{
		// ��ȡӰ��Ŀ�ߣ���������Ϊ��λ 
		m_nSourceWidth   = m_scanParameter.PixelNum;
		m_nSourceHeight  = m_scanParameter.LineNum;
	}

	m_dRat = (double)m_nSourceWidth/m_nSourceHeight;

	int nType = CV_8UC3;
	switch(m_nPixelType)
	{
	case  TWPT_BW:
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
		//matAutoCrop = RemoveBlack(matAutoCrop);
		matAutoCrop.copyTo(m_mat_image);

		m_nWidth = m_mat_image.cols; 
		m_nHeight = m_mat_image.rows;
	}
	//imwrite("C:\\b.bmp", m_mat_image);

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
		//IplImage *out;
		MedianSmooth(m_mat_image/*, out*/);
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
		Mat matRemovepunch;
		matRemovepunch = RemovePunch(m_mat_image, 200, 22); //ȥ������
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
	m_scanParameter.ExtScanParam = 0x9000 ;       //Enable extended scan parameter and set XRes and YRes for newer model
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
	Mat img(src_img);

	Point center(img.cols/2, img.rows/2);

#ifdef DEGREE
	//��תû����б��ͼ�񣬵���ʹ��
	Mat rotMatS = getRotationMatrix2D(center, DEGREE, 1.0);//��ȡ��ת����
	warpAffine(img, img, rotMatS, img.size(), 1, 0, Scalar(255,255,255));//oʵ������ϵ����任
#endif

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
	//magnitudeMat = magnitudeMat(Rect(0, 0, magnitudeMat.cols & -2, magnitudeMat.rows & -2));

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
	theta = atan(angleT) * 180/CV_PI;

	//����任У��***********************
	//ȡͼ������
	//Rotate the image to recover
	//У��ͼ��
	//���һ������Ȼ�ǰ�ͼ��ת��ȥ
	//����getRotationMatrix2D()���һ��2*3�ķ���任����
	//�ٰ������������warpAffine()����һ��������ת�ķ���任��
	//warpAffine()�����һ������Scalar(255,255,255)�ǰ�������ת�����Ŀհ��ð�ɫ��䡣

	//��ͼ	
	Point2f centerPoint = Point2f(img.cols/2, img.rows/2);
	Mat warpMat = getRotationMatrix2D(centerPoint, theta, 1.0);
	Mat resultImage = Mat::ones(img.size(), img.type());
	warpAffine(img, resultImage, warpMat, resultImage.size(), 1, 0, Scalar(255,255,255));
	//imwrite("C:\\Users\\Administrator\\Desktop\\a.jpg", resultImage);
	//�Ҷȡ��ڰ�ͼ�ܹ�����У������RemoveBlack����

	return resultImage;
}

cv::Mat CScanner_G6X00::RemoveBlack(Mat src_img)
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

	Mat imageSave = inputImg(rect);
	return imageSave;
}

void CScanner_G6X00::ColorFlip(const Mat& src, Mat& dst)
{

	CV_Assert(src.depth() == CV_8U); //�������еı��ʽֵΪfalse���򷵻�һ��������Ϣ
	dst.create(src.rows, src.cols, src.type());

	int width = src.rows;
	int height = src.cols;

	for(int j = 0; j < height; j++)
	{
		for(int i = 0; i < width; i++)
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

	vector<Rect> rects;
	Rect rectTemp(0, 0, 3*src.cols/30, 3*src.rows/30); //����ֻȡʮ��֮һ,��rect�����Ҫ��3�ı���
	rects.push_back(Rect(0, 0, src.cols, rectTemp.height)); //�ϲ�
	rects.push_back(Rect(0, src.rows-rectTemp.height, src.cols, rectTemp.height));	 //�²�	
	rects.push_back(Rect(0, rectTemp.height, rectTemp.width, src.rows-2*rectTemp.height)); //���  ֻ���м䲿��
	rects.push_back(Rect(src.cols-rectTemp.width, rectTemp.height, rectTemp.width, src.rows-2*rectTemp.height)); //�Ҳ�

	vector<Mat> subImages;
	for(size_t i = 0; i < rects.size(); i++)
	{
		Mat tempImg;
		src(rects[i]).copyTo(tempImg);
		tempImg = HoughCirclesTransfer(tempImg,1,threshold1,threshold2);
		subImages.push_back(tempImg);	
	}

	Mat dst_img;
	src.copyTo(dst_img);	
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
	HoughCircles(midImage, circles, CV_HOUGH_GRADIENT, dp, minDist, threshold1, threshold2, 0, 0);  //200,100 

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



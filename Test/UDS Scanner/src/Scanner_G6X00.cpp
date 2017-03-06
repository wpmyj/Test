#include "Scanner_G6X00.h"
#include "public.h"
#include "uds_cap.h"
#include <time.h>
#include <vector>

#define DPI 100
#define SCANWIDTH_INCH   8.27
#define SCANLENGTH_INCH  /*118*/11.69 // 长纸模式


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
	//Base界面
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

	m_fRotation           = 0.0;           //旋转-不旋转zhu
	m_nSpiltImage         = TWSI_NONE;     //zhu 分割-不分割
	m_fGamma              = 100.0;         //zhu Gamma校正-默认为100
	m_bMirror             = TWMR_DISABLE;  //镜像-不选中

	m_nBinarization       = TWBZ_DYNATHRESHOLD;  //zhu 二值化-动态阈值
	m_bMultiStream        = false;               //多流输出-不选中
	m_fSensitiveThreshold_removespots = 0.0;     //去除斑点-0.0
	m_fSensitiveThreshold_colorretent = 128.0;   //底色保留-128.0

	//其他图像处理
	//默认不选中
	m_fRemoveBlank        = TWRP_DISABLE; 
	m_bRemovePunch        = TWSP_DISABLE;
	m_bSharpen            = TWGM_DISABLE;
	m_bRemoveBack         = TWRB_DISABLE;
	m_bDescreen           = TWDS_DISABLE;
	m_bDenoise            = TWDN_DISABLE;
	m_bAutoCrop           = TWAC_DISABLE;

	m_fEdgeUp             = 0.0;      // 边缘扩充 
	m_fEdgeDown           = 0.0;      // 边缘扩充 
	m_fEdgeLeft           = 0.0;      // 边缘扩充 
	m_fEdgeRight          = 0.0;      // 边缘扩充 

	m_fXPos               = 0.0;      // X偏移量 
	m_fYPos               = 0.0;      // Y偏移量 

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
		// 方法2：ReadScanEx
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
	CDevice_Base::setSetting(settings);  // 调用父类的方法
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
		// 获取影像的宽高，都以像素为单位 
		m_nSourceWidth   = m_scanParameter.PixelNum;
		m_nSourceHeight  = m_scanParameter.LineNum;
	}

	m_dRat = (double)m_nSourceWidth/m_nSourceHeight;

	int nType = CV_8UC3;
	switch(m_nPixelType)
	{
	case  TWPT_BW:
		nType = CV_8UC1; // 黑白与灰度相同
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

	if(m_nOrientation == TWOR_LANDSCAPE) //横向
	{		

		RotateImage(m_mat_image, m_mat_image, 90);
	}

	// 旋转
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

	//图像镜像处理
	if(m_bMirror == TWMR_AUTO)
	{ 
		Mat mat_hMirror;
		hMirrorTrans(m_mat_image, mat_hMirror);
		mat_hMirror.copyTo(m_mat_image);
	}


	//自动裁切与校正
	if(m_bAutoCrop == TWAC_AUTO) 
	{
		Mat matAutoCrop; 
		matAutoCrop = AutoCorrect(m_mat_image);//先自动校正	
		//matAutoCrop = RemoveBlack(matAutoCrop);
		matAutoCrop.copyTo(m_mat_image);

		m_nWidth = m_mat_image.cols; 
		m_nHeight = m_mat_image.rows;
	}
	//imwrite("C:\\b.bmp", m_mat_image);

	//色彩翻转
	if(m_bColorFlip == TWCF_AUTO)
	{ 
		Mat mat_hColorflip;
		ColorFlip(m_mat_image, mat_hColorflip);
		mat_hColorflip.copyTo(m_mat_image);
	}


	//去除噪声
	if(m_bDenoise == TWDN_AUTO) 
	{	
		//IplImage *out;
		MedianSmooth(m_mat_image/*, out*/);
		//Mat mTemp(out,0);
		//mTemp.copyTo(m_mat_image);
	}

	//去网纹
	if(m_bDescreen == TWDS_AUTO) 
	{
		//::MessageBox(g_hwndDLG,TEXT("去网纹!"),MB_CAPTION,MB_OK);
		Mat matDescreen;
		m_mat_image.copyTo(matDescreen);
		//Size(5,5)模板大小，为奇数，要更精确需要自己生成模板
		//x方向方差
		//Y方向方差
		GaussianBlur(matDescreen, matDescreen, Size(5,5), 0, 0);  //  高斯滤波
		matDescreen.copyTo(m_mat_image);
		
	}

	//去除穿孔
	if(m_bRemovePunch == TWRP_AUTO) 
	{	 
		Mat matRemovepunch;
		matRemovepunch = RemovePunch(m_mat_image, 200, 22); //去除穿孔
		matRemovepunch.copyTo(m_mat_image);

		m_nWidth = m_mat_image.cols; 
		m_nHeight = m_mat_image.rows;
	}


	//锐化
	int index = FindDepth(m_mat_image); //index为图像的深度
	//锐化图像
	if(m_bSharpen == TWSP_AUTO) 
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

	switch(m_nPixelType)
	{
	case TWPT_BW:
		//m_pCxImage->Negative();
		m_nDestBytesPerRow = BYTES_PERLINE(m_nWidth, 8);  // OpenCV对位深1的图像不支持
		break;
	case TWPT_GRAY:
		m_nDestBytesPerRow = BYTES_PERLINE(m_nWidth, 8);
		break;
	case TWPT_RGB:
		m_nDestBytesPerRow = BYTES_PERLINE(m_nWidth, 24);
		break;
	}

	
	//Mat数据转为字节对齐的uchar,必须放在最后，否则其他图像处理操作无效
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
	WORD    nMaxRows  = (WORD)(dwRead / m_nDestBytesPerRow); //number of rows行 to be transfered during this call (function of buffer size and line size)

	if( m_nScanLine < MIN(m_nSourceHeight, (WORD)m_nHeight) ) //0<min(2250,2200)
	{
		//fill the buffer line by line to take care of alignment differences
		for(nRow = 0; nRow < nMaxRows; nRow++) //nMaxRows = 12
		{
			//get the next scan line position and copy it
			//pBits = MyGetScanLine(m_nScanLine);
			pBits = GetScanLine(m_nScanLine);

			memcpy( pTransferBuffer, pBits, MIN(m_nDestBytesPerRow, m_dwBytesPerRow)); //MIN(5100,4956) step表示以字节为单位的每行的长度

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
		m_scanParameter.ScanMode = SMO_COLOR_1PASS;  // 默认彩色
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

	m_dwBytesPerRow = m_scanParameter.PixelNum * m_scanParameter.BitsPerPixel / 8; // 一行所占的字节数	
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
	double scale = 1; // 缩放尺度 
	cv::Mat src(_src);
	if (90 == angle || 270 == angle || -90 == angle || -270 == angle)
	{
		scale = m_dRat;
	}
	cv::Point2f center = cv::Point2f((float)src.cols / 2, 
		(float)src.rows / 2);  // 旋转中心   	 

	cv::Mat rotateMat;   
	rotateMat = cv::getRotationMatrix2D(center, angle, scale);  

	cv::Mat rotateImg;  
	cv::warpAffine(src, rotateImg, rotateMat, src.size());  

	_dst = rotateImg;
}

void CScanner_G6X00::hMirrorTrans(const Mat &src, Mat &dst)
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

void CScanner_G6X00::Mat2uchar(Mat src_img)
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

void CScanner_G6X00::ColorFlip(const Mat& src, Mat& dst)
{

	CV_Assert(src.depth() == CV_8U); //若括号中的表达式值为false，则返回一个错误信息
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

	cvSmooth(&in, out, CV_MEDIAN, 3, in.nChannels);  //  中值滤波 medianBlur(src,dst,3);
	m_mat_image = out; //IplImage -> Mat
}

cv::Mat CScanner_G6X00::RemovePunch(const Mat &src, double threshold1, double threshold2)
{
	//ChangeImage(IMAGENAME_REMOVEPUNCH);
	//Mat src = imread(m_szSourceImagePath, CV_LOAD_IMAGE_UNCHANGED);

	vector<Rect> rects;
	Rect rectTemp(0, 0, 3*src.cols/30, 3*src.rows/30); //宽、高只取十分之一,但rect宽高需要是3的倍数
	rects.push_back(Rect(0, 0, src.cols, rectTemp.height)); //上侧
	rects.push_back(Rect(0, src.rows-rectTemp.height, src.cols, rectTemp.height));	 //下侧	
	rects.push_back(Rect(0, rectTemp.height, rectTemp.width, src.rows-2*rectTemp.height)); //左侧  只是中间部分
	rects.push_back(Rect(src.cols-rectTemp.width, rectTemp.height, rectTemp.width, src.rows-2*rectTemp.height)); //右侧

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

int CScanner_G6X00::FindDepth(const Mat &src_img)
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



#include "CScanner_G6400.h"
#include "Common.h"
#include "CTWAINDS_Base.h"
#include <list>

#define SFI_PAPERSOURCE_ADF 0

extern HWND   g_hwndDLG;

CCommonFunc		CommonFunc;

#ifdef TWH_CMP_MSC
extern HINSTANCE   g_hinstance;
#endif

extern HWND g_hwndSheet;

CScanner_G6400::CScanner_G6400(void)
{
	hDriver = NULL;
	m_bReadOnly = false;
	m_nDocCount = 1;
	m_pSaveData = NULL;
	//AfxMessageBox("CScanner_G6400()");
	//if(InitLibrary()==TRUE)  // 初始化SDK函数，得到各函数的地址
	//{
	//		if(SelectScanner() == FALSE)  // 选择扫描仪并加载扫描仪
	//		{
	//			AfxMessageBox("Run SelectScanner() Failed！");
	//		}				
	//}
	//else
	//	AfxMessageBox("Run InitLibrary() Failed！");
}

CScanner_G6400::~CScanner_G6400(void)
{
	if(hDriver)
	{	
		UnloadScannerFun();
		if(TerminateLibFun)
			TerminateLibFun();
		FreeLibrary(hDriver);
	}


}

bool CScanner_G6400::InitLibrary(void)
{
	char szDll_Path[MAX_PATH];  // AVSDK.dll的路径
	GetDLLPath(szDll_Path);
	//::MessageBox(g_hwndDLG,szDll_Path,"UDS",MB_OK);

	hDriver = LoadLibrary(szDll_Path);

	//hDriver = LoadLibrary(TEXT("AVSDK.dll"));
	if(hDriver == NULL)

  {
		AfxMessageBox("Load AVSDK.dll Failed!");
		return FALSE;
	}
	//AfxMessageBox("Load AVSDK.dll Success!");
	InitializeLibFun = (InitializeLibProc)GetProcAddress(hDriver, "InitializeLib");
	TerminateLibFun = (TerminateLibProc)GetProcAddress(hDriver, "TerminateLib");

	INITLIBPARAM InitLibParam = {0};

	char szLicense[64] = "105e0929-0dab-4ecc-8213-47436c53c690";  //序列号必须有效
	char szProduct[64] = "SDKDemo";
	char szManufacturer[64]= "Avision";

	HWND hwnd = GetForegroundWindow();  //获取当前窗口的句柄

	InitLibParam.szLicense = szLicense;
	InitLibParam.DriverMode = DRIVER_TWAIN;  //DRIVER_ISIS;
	//InitLibParam.APWnd = hwnd;
	//InitLibParam.APWnd = (HWND)g_DS_init.AppId.Id;
	InitLibParam.APWnd = g_hwndSheet;
	InitLibParam.szManufacturer = szManufacturer;
	InitLibParam.szProduct = szProduct;
	InitLibParam.MajorNumber = 1;
	InitLibParam.MinorNumber = 0;

	AVSDKINFO sdk_info;

	//CString str;
	//str.Format("g_hwndDLG: %ld",g_hwndDLG);
	//AfxMessageBox(str);

	//str.Format("hwnd: %ld",hwnd);
	//AfxMessageBox(str);

	//str.Format("g_DS_init.AppId.Id: %ld",(HWND)g_DS_init.AppId.Id);
	//AfxMessageBox(str);

	//long ret = InitializeLibFun(InitLibParam, &sdk_info);
	//CString str;
	//str.Format("%ld",ret);
	//AfxMessageBox(str);

	

	if (InitializeLibFun(InitLibParam, &sdk_info) == SDK_SUCCESS) 
	{
		AfxMessageBox("Run InitializeLibFun Success!");
		SelectScannerFun = (SelectScannerProc)GetProcAddress(hDriver, "SelectScanner");
		if(SelectScannerFun == NULL)
		{
           AfxMessageBox("Load SelectScannerProc Failed!");
		   return FALSE;
		}

		LoadScannerFun = (LoadScannerProc)GetProcAddress(hDriver, "LoadScanner");
		if(LoadScannerFun == NULL)
		{
			AfxMessageBox("Load LoadScannerProc Failed!");
			return FALSE;
		}

		UnloadScannerFun = (UnloadScannerProc)GetProcAddress(hDriver, "UnloadScanner");
		if(UnloadScannerFun == NULL)
		{
			AfxMessageBox("Load UnloadScannerProc Failed!");
			return FALSE;
		}

		SetAdvancedSettingFun = (SetAdvancedSettingProc)GetProcAddress(hDriver, "SetAdvancedSetting");
		if(SetAdvancedSettingFun == NULL)
		{
			AfxMessageBox("Load SetAdvancedSettingProc Failed!");
			return FALSE;
		}

		SetGeneralSettingFun = (SetGeneralSettingProc)GetProcAddress(hDriver, "SetGeneralSetting");
		if(SetGeneralSettingFun == NULL)
		{
			AfxMessageBox("Load SetGeneralSettingProc Failed!");
			return FALSE;
		}

		AcquireImageFun = (AcquireImageProc)GetProcAddress(hDriver, "AcquireImage");
		if(AcquireImageFun == NULL)
		{
			AfxMessageBox("Load AcquireImageProc Failed!");
			return FALSE;
		}

		GetImageInformationFun = (GetImageInformationProc)GetProcAddress(hDriver, "GetImageInformation");
		if(GetImageInformationFun == NULL)
		{
			AfxMessageBox("Load GetImageInformationProc Failed!");
			return FALSE;
		}

		ImageTransferFun = (ImageTransferProc)GetProcAddress(hDriver, "ImageTransfer");
		if(ImageTransferFun == NULL){
			AfxMessageBox("Load ImageTransferProc Failed!");
			return FALSE;
		}

		CancelImageTransferFun = (CancelImageTransferProc)GetProcAddress(hDriver, "CancelImageTransfer");
		if(CancelImageTransferFun == NULL)
		{
			AfxMessageBox("Load CancelImageTransferProc Failed!");
			return FALSE;
		}

		ProfileScanFun = (ProfileScanProc)GetProcAddress(hDriver, "ProfileScan");
		if(ProfileScanFun == NULL)
		{
			AfxMessageBox("Load ProfileScanProc Failed!");
			return FALSE;
		}

		CommandOperationFun = (CommandOperationProc)GetProcAddress(hDriver, "CommandOperation");
		if(CommandOperationFun == NULL)
		{
			AfxMessageBox("Load CommandOperationProc Failed!");
			return FALSE;
		}

	}
	else
	{
		  AfxMessageBox("Run InitializeLibFun Failed!");
		  return FALSE;
	}
	return TRUE;
}

bool CScanner_G6400::acquireImage()
{
	
	//AfxMessageBox("acquireImage()");
	static bool bRunned = false;

	if (false == bRunned)
	{
		if(InitLibrary()==TRUE)                // 初始化SDK函数，得到各函数的地址
		{
			bRunned = true;
			if(SelectScanner() == FALSE)              // 选择扫描仪并加载扫描仪
			{
				AfxMessageBox("Run SelectScanner() Failed！");
			}

		}
		else
			AfxMessageBox("Run InitLibrary() Failed！");
	}
	

	GENERALSETTING GeneralSetting = {0};
	GeneralSetting.PixelType = PT_BW;
	GeneralSetting.ScanSource = 1;
	GeneralSetting.Resolution = 200;
	GeneralSetting.PaperSize = 0;

	if(SetGeneralSettingFun(GeneralSetting)==SDK_SUCCESS)
	{
		AfxMessageBox("Run SetGeneralSettingProc SUCCESS!");
		//return false;
	}

	/*if(SetGeneralSettingFun(m_generalsetting)!=SDK_SUCCESS)
	{
	AfxMessageBox("Run SetGeneralSettingProc Failed!");
	return false;
	}
	if(SetAdvancedSettingFun(m_advancedsetting)!=SDK_SUCCESS)
	{
	AfxMessageBox("Run SetAdvancedSettingProc Failed!");
	return false;
	}*/

	ACQUIREIMAGEPARAM	AcquireImageParam = {0};
	AcquireImageParam.ShowUI =UI_NONE;
	//AfxMessageBox("Run  before AcquireImageFun !");

	if(AcquireImageFun(AcquireImageParam) == SDK_SUCCESS)  //启动扫描，若获取图像成功
	{
		//AfxMessageBox("Run after AcquireImageFun !");
		ImageTransfer();
		m_nDocCount --;
		AfxMessageBox("return!");
		return true;
	}
	else
	{
		AfxMessageBox("Run AcquireImageProc Failed!");
		return false;
	}
	
}

void CScanner_G6400::ImageTransfer(void)
{
	// TODO: Add your control notification handler code here
	//AfxMessageBox("Run ImageTransfer !");

	IMAGETRANSFERPARAM TransferParam = {0};
	DWORD dwTotal;
	DWORD dwBlock = BUFFERSIZE;
	//BYTE  *pBuffer = new BYTE[dwBlock];
	BYTE  *psaveBuffer = NULL;
	BYTE  *psaveTempBuffer = NULL;
	int	  fileNumber = 1;	


	//OutputDebugString(TEXT("Batch ImageTransfer In\n"));

	//m_btnBatchImage.ShowWindow(SW_HIDE);
	TransferParam.Status = TRANSFER_DONE;     ///默认发送完成  
	do
	{
		if(TransferParam.Status == TRANSFER_DONE || TransferParam.Status == TRANSFER_PAGEDONE)   ///如果发送完成
		{
			GetImageInformationFun(&m_ImageInfo);                       ///获取图像信息
			ZeroMemory(&TransferParam, sizeof(IMAGETRANSFERPARAM));  ///ZeroMemory:用0来填充一块内存区域
			dwTotal = m_ImageInfo.BytesPerLine * m_ImageInfo.Length; ///总字节数

			if(psaveTempBuffer)   ///若psaveTempBuffer不为空，将之前数据清零
			{
				delete []psaveTempBuffer;     
				psaveTempBuffer = NULL;
			}

			//if(psaveTempBuffer == NULL)
			{
				psaveTempBuffer = new BYTE[dwTotal];   ///重新分配新的内存
				psaveBuffer = psaveTempBuffer;
			}
		}

		TransferParam.pImageData = psaveTempBuffer;//pBuffer;
		TransferParam.XferSize = dwBlock;

		ImageTransferFun(&TransferParam);    ///将图像数据传输至结构体TransferParam

		psaveTempBuffer += dwBlock;   ///dwBlock = 1024*1024

		//int result = TransferParam.Status;
		//CString str;
		//str.Format(TEXT("%d"),result);
		//AfxMessageBox(str);

		if(TransferParam.Status != TRANSFER_SUCCESS)   ///
		{
			if(TransferParam.Status == TRANSFER_PAGEDONE) ///若一页传输成功
			{
				CString strPath;
				strPath.Format(TEXT("d:\\a\\AVSDK%04d_sdk.tif"), fileNumber);        ///图片名字
				CommonFunc.GDISaveImage(psaveBuffer, strPath, m_ImageInfo);
				fileNumber++;
				psaveTempBuffer = psaveBuffer;
				continue;
			}
			else
				break;
		}

	}while(1);

	CString strPath;
	strPath.Format(TEXT("d:\\a\\AVSDK%04d_sdk.tif"), fileNumber);
	CommonFunc.GDISaveImage(psaveBuffer, strPath, m_ImageInfo);

	//m_pSaveData = psaveBuffer;  // 获取数据

	//psaveBuffer += dwTotal;
	//size_t size;
	m_pSaveData = (BYTE *)malloc(dwTotal);
	memcpy(m_pSaveData,psaveBuffer,dwTotal);

	psaveTempBuffer = psaveBuffer;
	delete []psaveTempBuffer;
	psaveTempBuffer = NULL;


}

bool CScanner_G6400::SelectScanner(void)
{
	//AfxMessageBox("SelectScanner!");
	//选择扫描仪
	SELECTSCANNERPARAM SelectScannerParam = {0};
	SelectScannerParam.ShowSelect = FALSE;

	SelectScannerFun(&SelectScannerParam);  // SelectScannerProc SelectScanner;
	strcpy_s(szSelectScanner, (const char*)SelectScannerParam.DefaultScanner);

	//加载扫描仪

	SCANNERABILITY scannerAbility;
	if(LoadScannerFun(szSelectScanner, &scannerAbility) == SDK_SUCCESS)
	{
		return TRUE;
	}
	else
	{
		AfxMessageBox("Run LoadScannerProc Failed!");
		return FALSE;
	}

}

void CScanner_G6400::CancelTransfer(void)
{
	CancelImageTransferFun();
}


void CScanner_G6400::setSetting(CScanner_Base settings)
{
  CScanner_Base::setSetting(settings);
}

bool CScanner_G6400::resetScanner()
{
	 //::MessageBox(g_hwndDLG,"resetScanner()","UDS",MB_OK);
		bool bret = true;

		// Unlock the scanner 
		Unlock();

		m_nPixelType    = TWPT_RGB;
		m_nPaperSource  = SFI_PAPERSOURCE_ADF;
		m_bDuplex       = false;
		m_nWidth        = 0;
		m_nHeight       = 0;
		m_fXResolution  = 200.0;
		m_fYResolution  = 200.0;



		return bret;
}

void CScanner_G6400::UpdateSetting()
{
	m_generalsetting.ScanSource = HIBYTE(m_nPaperSource);  // WORD低字节->BYTE
	m_generalsetting.PixelType = HIBYTE(m_nPixelType);  // WORD低字节->BYTE
	m_generalsetting.Duplex = (BYTE)m_bDuplex;  // bool->BYTE
	m_generalsetting.Width = (DWORD)m_nWidth;  // int->DWORD
	m_generalsetting.Length =(DWORD)m_nHeight;  // int->DWORD
	m_generalsetting.Resolution = (WORD)m_fXResolution;  // float->WORD
	
	m_advancedsetting.Advanced.Gamma = (BYTE)m_fGamma;  // float->BYTE
	m_generalsetting.Contrast = (SHORT)m_fContrast;  // float->SHORT
	m_generalsetting.Brightness = (SHORT)m_fBrightness;  // float->SHORT
	m_generalsetting.Threshold = (BYTE)m_fThreshold;  // float->BYTE
}

bool CScanner_G6400::getDeviceOnline() const
{
	return true;
}

bool CScanner_G6400::isFeederLoaded()
{
	AfxMessageBox("isFeederLoaded!");

	//if(InitLibrary()==TRUE)                ///< 初始化SDK函数，得到各函数的地址
	//{
	//	if(SelectScanner() == FALSE)              ///< 选择扫描仪并加载扫描仪
	//	{
	//		AfxMessageBox("Run SelectScanner() Failed！");
	//	}

	//}
	//else
	//	AfxMessageBox("Run InitLibrary() Failed！");



	//bool rtn = true;
	//if(m_nDocCount<=0)
	//{
	//	rtn = false;
	//	m_nDocCount = m_nMaxDocCount;// Reloaded the scanner with paper
	//}
	//return rtn;

	//char str[10];
	//void *pBuffer=str;

	//if (CommandOperationFun(CMDOP_READ ,TAG_PAPERLOAD , DT_BYTE,1, pBuffer) == SDK_SUCCESS )
	//{
	//	if(str[0] == 0)
	//	{
	//		AfxMessageBox(TEXT("没纸"));
	//		return false;            //AfxMessageBox(TEXT("没纸"));
	//	}
	//	else  
	//	{
	//		AfxMessageBox(TEXT("有纸"));
	//		 return true;             //AfxMessageBox(TEXT("有纸"));
	//	}

	//}

	//AfxMessageBox("Run CommandOperationFun Failed!");
	//return false;
	//return true;



	
	bool rtn = true;
	if(m_nDocCount<=0)
	{
		rtn = false;
		m_nDocCount = 1;// Reloaded the scanner with paper
	}
	return rtn;
}

void CScanner_G6400::GetImageData(BYTE *buffer, DWORD &dwReceived)
{
	//bool bret = acquireImage();
	//CString str;
	//str.Format("%d",bret);
	//AfxMessageBox(str);
	//buffer = m_pSaveData;

	//buffer = (BYTE *)malloc(dwTotal);
	memcpy(buffer,m_pSaveData,sizeof(m_pSaveData));
	dwReceived = sizeof(m_pSaveData);
}

void CScanner_G6400::GetDLLPath(char* dllPath)
{
	char szTWAIN_DS_DIR[PATH_MAX];  // 当前DS的路径
	char szDll_Path[MAX_PATH];  // AVSDK.dll的路径

#ifdef TWH_CMP_MSC
	GetModuleFileName(g_hinstance, szTWAIN_DS_DIR, MAX_PATH);

	// strip filename from path
	size_t len = strlen(szTWAIN_DS_DIR);

	while(len > 0)
	{
		if(PATH_SEPERATOR == szTWAIN_DS_DIR[len-1])
		{
			szTWAIN_DS_DIR[len-1] = 0;
			break;
		}
		--len;
	}
	len = strlen(szTWAIN_DS_DIR);

	strncpy_s(szDll_Path, szTWAIN_DS_DIR, MAX_PATH);
#else
	// In non-windows, kTWAIN_DS_DIR is set on the compiler command line
	strncpy(szDll_Path, kTWAIN_DS_DIR, MAX_PATH);
#endif

#if (TWNDS_CMP_VERSION >= 1400)
	if (szDll_Path[len-1] != '\\')
	{
		strcat_s(szDll_Path, MAX_PATH, "\\");
	}		
	strcat_s (szDll_Path, MAX_PATH, "AVSDK.dll");

#else
	if (szDll_Path[len-1] != '\\')
	{
		strcat(szDll_Path, "\\");
	}		
	strcat(szDll_Path, "AVSDK.dll");
#endif

	strncpy_s(dllPath, MAX_PATH, szDll_Path, MAX_PATH);
}



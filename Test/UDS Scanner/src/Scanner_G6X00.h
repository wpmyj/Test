/**
* @file   Scanner_G6X00.h
* @brief 供扫描仪G6400与G6600使用.
* @author UDS
* @par   Copyright (c) UDS. All Rights Reserved.
* @date  03/02/2017
*/ 
#pragma once
#include "Device_Base.h"
#include "GL1.h"

#define  FILENAME_DLL_GL1   TEXT("GL1.dll")

#define  FILENAME_DLL_GL2   TEXT("GL2.dll")

/**
* The scanner define for PaperSource is ADF
*/
#define SFI_PAPERSOURCE_ADF 0
/**
* The scanner define for PaperSource is Flatbed
*/
#define SFI_PAPERSOURCE_FB  1

class CScanner_G6X00 : public CDevice_Base
{
public:
	CScanner_G6X00(void);
	~CScanner_G6X00(void);

public:
/**
  * Resets the scanner to factory default settings.
  * Sets the scanners caps back to defaults.
  * @return true if successfully reset.
  */
  bool resetScanner();

  /**
  * Determine if there is paper sitting in the feeder.
  * IF empty loads again for next time.
  * @return true if paper in feeder else return false.
  */
  bool isFeederLoaded();

	/**
  * Retrieves the image to scan from disk into memory.
  * @return true if image was loaded into memory successfully.
  */
  bool acquireImage();

	/**
  *  @brief  获取ADF中纸张个数
	*  @return 纸张数
	*/
	short GetMaxPagesInADF(void){return m_nMaxDocCount;}

	/**
	*  @brief  设置ADF中最大待扫纸张数
	*  @param[in]  nVal 设置的数目 
	*/
  void SetMaxPagesInADF(short nVal){m_nMaxDocCount = nVal;};

	/**
  * set the current settings
  * @param[in] settings the new settings for the scanner
  */
  void setSetting(CDevice_Base settings);

	/**
	*  @brief  是否内存  
	*/
	void Release();

protected:
	/**
	*  @brief  获取图片数据.
	*  @note 仅供扫描仪G6400使用
	*  @param[out] buffer 接收图片数据的buffer.
	*  @param[out] dwReceived 传输的实际大小（以字节为单位）
	*/
	void GetImageData(BYTE *buffer, DWORD &dwReceived);

	/**
  * Return the number of documents sitting in the feeder.
  * This number can be set with enviroment varible kGETENV_XFERCOUNT "CAP_XFERCOUNT"
  * this is so we can tell when the feeder is empty
  * - If CAP_XFERCOUNT is not set will return 1.
  * - if < 0 will return random number.
  * @return default number of documents.
  */
  short getDocumentCount() const;

	/**
  * Transform the image according to the caps set by the application.
  * @return true if successful
  */
  bool preScanPrep();

	/**
  * Get a scan line and put it into the _ImageXfer, fill values in _ImageXfer.
  * @param[out] pTransferBuffer a pointer to an array of bytes to store the image data
  * @param[in]  dwRead the number of bytes to read from scanner
  * @param[out] dwReceived the actual number of bytes transfered
  * @return true if successful
  */
  bool getScanStrip(BYTE *pTransferBuffer, DWORD dwRead, DWORD &dwReceived);

	/**
	*  @brief  初始化驱动参数
	*/
	void InitDriverParamter();

	/**
	*  @brief  加载DLL及其函数
	*  @retval true 表示成功
	*  @retval false 表示失败  
	*/
	bool LoadDLL();

	/**
	*  @brief  生成Gamma表与设置其它参数
	*  @param[in]  GammaTable 256字节的GammaTable 
	*  @param[in] Brightness 亮度：-127 - 127
  *  @param[in] Contrast 对比度：-127 - 127
	*  @param[in] Highlight 高亮：1 - 255
	*  @param[in] Shadow 阴影：0 - 254  (Shadow should be smaller than Highlight)
	*  @param[in] Gamma Gamma：10 - 999 (means 0.1 - 99.9)
	*  @param[out] outArgName   
	*/
	void CalGammaTable(LPBYTE GammaTable, int Brightness, int Contrast, BYTE Highlight, BYTE Shadow,float Gamma);

	/**
	*  @brief  调整参数，将UI得到的值赋值给成员变量 
	*/
	void AdjustParameter();

  /**
	*  @brief  设置参数
	*/
	void SetParameter();

	/**
	*  @brief  获取当前扫描区域宽度与高度
	*  @param[in] _nSize ICAP_SUPPORTEDSIZES 对应值
	*  @param[out]  _fWeight 宽度 
	*  @param[out]  _fHeight 高度 
	*/
	void GetCurrentScanRange(const int& _nSize, float& _fWeight, float& _fHeight);

	/**
	*  @brief  获取图像数据行指针
	*  @param[in]  _nLine 行数 
  *  @return 第_nLine行指针  
	*/
	BYTE* MyGetScanLine(int _nLine);


	/**
	* swaps the blue and red components (for RGB images)
	* @param[in] buffer : pointer to the pixels
	* @param[in] length : number of bytes to swap. lenght may not exceed the scan line.
	* @param[in] BitsPerPixel 
	*/
	void RGB2BGR(BYTE *buffer, int length, int BitsPerPixel);


	/**
	*  @brief  实现黑白图像反转
	*  @param[in]  pBuffer  pointer to the pixels 
	*  @param[in] BitsPerPixel 
	*/
	void Invert(BYTE* pBuffer, int length, int BitsPerPixel);

protected:
	InitializeDriverProc               InitializeDriver;
	InitializeScannerProc              InitializeScanner;
	GetScannerAbilityProc              GetScannerAbility;  
	GetScannerAbilityExProc            GetScannerAbilityEx;    
	GetADFStatusProc                   GetADFStatus;
	StartScanJobProc                 	 StartScanJob;
	SetScanParameterProc               SetScanParameter;
	SetGammaTableProc                  SetGammaTable;
	StartScanProc                      StartScan;
	ReadScanProc                       ReadScan;
	ReadScanExProc                     ReadScanEx;
	ReadScanEx2Proc                    ReadScanEx2;
	StopScanProc                       StopScan;
	EndScanJobProc                     EndScanJob ;
	DoEjectPaperAfterScanProc          DoEjectPaperAfterScan;
	TerminateDriverProc                TerminateDriver;
	GetLastStatusCodeProc              GetLastStatusCode;
	GetDeviceListProc                  GetDeviceList;
	DestroyDeviceListProc              DestroyDeviceList;
	SelectDeviceProc                   SelectDevice;
	GeneralOperationProc               GeneralOperation;
	ResetScannerProc                   ResetScanner;
	LampOnProc                         LampOn;
	GetBackgroundRasterProc            GetBackgroundRaster;
	TuneScanLengthProc                 TuneScanLength;
	ReadNVMDataProc                    ReadNVMData;
	SetBatchScanParameterProc          SetBatchScanParameter;
	SetSpeedModeProc                   SetSpeedMode;

protected:
	short             m_nDocCount;              /**< number of documents waiting to transfer */
	short             m_nMaxDocCount;           /**< Max number of documents waiting to transfer */
	WORD              m_nSourceWidth;           /**< Width of image in FreeImage */
	WORD              m_nSourceHeight;          /**< Height of image in FreeImage */
	TCHAR             m_szIniPath[PATH_MAX];    /**< INI配置文件路径 */   
	int               m_nImageNumber;           /**< 图片编号 */
	//CxImage*          m_pCxImage;               /**< 图片数据指针 */
	WORD              m_nScanLine;              /**< Current scan line of image in FreeImage */
	DWORD             m_nDestBytesPerRow;       /**< number of bytes needed for a row of data */


private:
	BYTE* m_pGammaTable;
	BYTE* m_pFrontBuffer;
	BYTE* m_pRearBuffer;
	BYTE* m_pTempBuffer;
	BYTE  m_byteADFStatus;
	DWORD m_dwTotal;
	DWORD m_dwBytesPerRow;
	PIXELBOUND m_ePixelBound;
	IO_STATUS m_ioStatus;
	SCANNERABILITY m_scannerAbility;
	SCANPARAMETER m_scanParameter;
  HMODULE m_hDLL;

};


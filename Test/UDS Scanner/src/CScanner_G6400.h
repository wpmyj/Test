/**
* @file   CScanner_G6400.h
* @brief 扫描仪G6400 SDK封装文件.
* @author UDS
* @par   Copyright (c) UDS All Rights Reserved.
* @date  11/10/2016
*/ 

#ifndef __CSCANNER_G6400_H__
#define __CSCANNER_G6400_H__

//*#include*/ "Common.h"
//#include "FreeImage.h"
//#include "twain.h"
#include <sdkddkver.h>

#include "afxwin.h"
#include "AVSDK.h"
#include "CommonFunc.h"
#include "Device_Base.h"

#define BUFFERSIZE		(1024*1024)


class CScanner_G6400 : public CDevice_Base
{
public:
	CScanner_G6400(void);
	~CScanner_G6400(void);

protected:
	InitializeLibProc                InitializeLibFun;
	TerminateLibProc                 TerminateLibFun;
	SelectScannerProc                SelectScannerFun;
	LoadScannerProc		               LoadScannerFun;
	UnloadScannerProc	               UnloadScannerFun;
	SetAdvancedSettingProc	         SetAdvancedSettingFun;
	SetGeneralSettingProc	           SetGeneralSettingFun;
	AcquireImageProc	               AcquireImageFun;
	GetImageInformationProc	         GetImageInformationFun;
	ImageTransferProc	               ImageTransferFun;
	CancelImageTransferProc          CancelImageTransferFun;
	ProfileScanProc		               ProfileScanFun;
	CommandOperationProc	           CommandOperationFun;

public:

	/**
	*  @brief  初始化SDK并获取个接口函数地址.
	*  @retval true  表示成功
	*  @retval false 表示失败  
	*/
  bool InitLibrary(void);

	/**
	*  @brief  选择并加载扫描仪.
	*  @retval true  表示成功
	*  @retval false 表示失败
  */
	bool SelectScanner(void);

	/**
	*  @brief  采集图片数据.
	*  @retval true 表示成功
	*  @retval false 表示失败  
	*/
	bool acquireImage(void);              

	/**
	*  @brief  传递图片数据.
	*/
	void ImageTransfer(void);  

	/**
	*  @brief  取消传递图片数据.
	*/
	void CancelTransfer(void);           

	/**
  * Resets the scanner to factory default settings.
  * Sets the scanners caps back to defaults.
  * @return true if successfully reset.
  */
  bool resetScanner();

	/**
  * set the current settings
  * @param[in] settings the new settings for the scanner
  */
  void setSetting(CDevice_Base settings);

	/**
  * Determine if there is paper sitting in the feeder.
  * IF empty loads again for next time.
  * @return true if paper in feeder else return false.
  */
  bool isFeederLoaded();

  /**
  * Return status of the device is online or not.
  * @return true if online.
  */
  bool getDeviceOnline() const;

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
	*  @brief  获取DLL的绝对路径.
	*  @param[out] dllPath DLL的绝对路径.   
	*/
	void GetDLLPath(char* dllPath);

	/**
	*  @brief  获取图片数据.
	*  @note 仅供扫描仪G6400使用
	*  @param[out] buffer 保存图片数据buffer.
	*  @param[out] dwReceived 传输的实际大小（以字节为单位）
	*/
	void GetImageData(BYTE *buffer, DWORD &dwReceived);

	/**
	* @brief 更新参数设置
	*/
	void UpdateSetting();

protected:
	short                   m_nDocCount;              /**< number of documents waiting to transfer */
	short                   m_nMaxDocCount;           /**< Max number of documents waiting to transfer */	
	HMODULE                 hDriver;                  /**< DLL句柄 */
	GENERALSETTING          m_generalsetting;         /**< 通用设置 */
	ADVANCEDSETTING         m_advancedsetting;        /**< 高级设置 */
	IMAGEINFORMATION	      m_ImageInfo;              /**< 图片信息 */
	char	                  szSelectScanner[64];      /**< 扫描仪的名称 */
	BYTE                   *m_pSaveData;              /**< 图片内存地址 */
	DWORD                   m_dwImageSize;              /**< 图片大小 */
};

#endif  // __CSCANNER_G6400_H__
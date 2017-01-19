/**
* @file   CScanner_G6400.h
* @brief ɨ����G6400 SDK��װ�ļ�.
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
	*  @brief  ��ʼ��SDK����ȡ���ӿں�����ַ.
	*  @retval true  ��ʾ�ɹ�
	*  @retval false ��ʾʧ��  
	*/
  bool InitLibrary(void);

	/**
	*  @brief  ѡ�񲢼���ɨ����.
	*  @retval true  ��ʾ�ɹ�
	*  @retval false ��ʾʧ��
  */
	bool SelectScanner(void);

	/**
	*  @brief  �ɼ�ͼƬ����.
	*  @retval true ��ʾ�ɹ�
	*  @retval false ��ʾʧ��  
	*/
	bool acquireImage(void);              

	/**
	*  @brief  ����ͼƬ����.
	*/
	void ImageTransfer(void);  

	/**
	*  @brief  ȡ������ͼƬ����.
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
	*  @brief  ��ȡDLL�ľ���·��.
	*  @param[out] dllPath DLL�ľ���·��.   
	*/
	void GetDLLPath(char* dllPath);

	/**
	*  @brief  ��ȡͼƬ����.
	*  @note ����ɨ����G6400ʹ��
	*  @param[out] buffer ����ͼƬ����buffer.
	*  @param[out] dwReceived �����ʵ�ʴ�С�����ֽ�Ϊ��λ��
	*/
	void GetImageData(BYTE *buffer, DWORD &dwReceived);

	/**
	* @brief ���²�������
	*/
	void UpdateSetting();

protected:
	short                   m_nDocCount;              /**< number of documents waiting to transfer */
	short                   m_nMaxDocCount;           /**< Max number of documents waiting to transfer */	
	HMODULE                 hDriver;                  /**< DLL��� */
	GENERALSETTING          m_generalsetting;         /**< ͨ������ */
	ADVANCEDSETTING         m_advancedsetting;        /**< �߼����� */
	IMAGEINFORMATION	      m_ImageInfo;              /**< ͼƬ��Ϣ */
	char	                  szSelectScanner[64];      /**< ɨ���ǵ����� */
	BYTE                   *m_pSaveData;              /**< ͼƬ�ڴ��ַ */
	DWORD                   m_dwImageSize;              /**< ͼƬ��С */
};

#endif  // __CSCANNER_G6400_H__
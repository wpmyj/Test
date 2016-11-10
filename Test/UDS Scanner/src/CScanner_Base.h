/**
* @file   CScanner_Base.h
* @brief 扫描仪基类.
* @author UDS
* @par   Copyright (c) UDS All Rights Reserved.
* @date  11/10/2016
*/ 

#ifndef __CSCANNER_BASE_H__
#define __CSCANNER_BASE_H__

#include "Common.h"
#include "twain.h"

//class CScanner_Base
class CScanner_Base
{

public:
	CScanner_Base();
	~CScanner_Base();

public:
	/**
  * Resets the scanner to factory default settings.
  * Sets the scanners caps back to defaults.
  * @return true if successfully reset.
  */
	virtual bool resetScanner() {return false;}

  /**
  * Get a scan line and put it into the _ImageXfer, fill values in _ImageXfer.
  * @param[out] pTransferBuffer a pointer to an array of bytes to store the image data
  * @param[in]  dwRead the number of bytes to read from scanner
  * @param[out] dwReceived the actual number of bytes transfered
  * @return true if successful
  */
  virtual bool getScanStrip(BYTE *pTransferBuffer, DWORD dwRead, DWORD &dwReceived){return false;}

  /**
  * Sets all the scanners capabilities to read only.
  */
  virtual void Lock(void) {m_bReadOnly = true;}

  /**
  * Sets all the scanners capabilities to read and write.
  */
  virtual void Unlock(void) {m_bReadOnly = false;}

  /**
  * Retrieves the image to scan from disk into memory.
  * @return true if image was loaded into memory successfully.
  */
  virtual bool acquireImage() {return false;}

  ////////////////
  // Accessors

  /**
  * get the current settings
  * @return the current scanner settngs
  */
  virtual CScanner_Base getSetting() const  ;

  /**
  * set the current settings
  * @param[in] settings the new settings for the scanner
  */
  virtual void setSetting(CScanner_Base settings) ;

  /**
  * Determine if there is paper sitting in the feeder.
  * IF empty loads again for next time.
  * @return true if paper in feeder else return false.
  */
  virtual bool isFeederLoaded() {return false;}

  /**
  * Return status of the device is online or not.
  * @return true if online.
  */
  virtual bool getDeviceOnline() const {return false;}

	/**
	*  @brief  获取自动进纸器(ADF)中纸张数量.
	*  @note  仅供虚拟扫描仪使用
	*  @Return:  short:纸张数量
	*/
  virtual short GetMaxPagesInADF(void) {return 0;}

	/**
	*  @brief  设置自动进纸器(ADF)中纸张数量.
	*  @note  仅供虚拟扫描仪使用
	*  @param[in]  nVal 纸张数量   
	*/
  virtual void SetMaxPagesInADF(short nVal) {}

	/**
	*  @brief  获取图片数据.
	*  @note 仅供扫描仪G6400使用
	*  @param[out] buffer 保存图片数据buffer.
	*/
	virtual void GetImageData(BYTE *buffer) {}

public:
	WORD              m_nPaperSource;           /**< the current paper source ADF or Flatbed*/
	bool              m_bDuplex;                /**< True to use duplex false for simplex, ignored if flatbed*/
	WORD              m_nPixelType;             /**< type of pixels to transfer image as */
	int               m_nWidth;                 /**< frame left edge */
	int               m_nHeight;                /**< frame top edge */
	float             m_fXResolution;           /**< horizontal resolution */
	float             m_fYResolution;           /**< vertical resolution */
	float             m_fGamma;                 /**< Gamma */
	float             m_fContrast;              /**< Contrast */
	float             m_fBrightness;            /**< Brightness */
	float             m_fThreshold;             /**< Threshold */

	//WORD              m_nRotation; //zhu

protected:
	bool              m_bReadOnly;              /**< current mode */

};


#endif  //__CSCANNER_BASE_H__
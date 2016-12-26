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
	virtual bool resetScanner() {return true;}

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
  virtual CScanner_Base* getSetting() const  ;

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
	*  @param[out] dwReceived 传输的实际大小（以字节为单位）
	*/
	virtual void GetImageData(BYTE *buffer, DWORD &dwReceived) {}

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

	bool              m_bMultifeedDetection;//zhu/**<重张检测 */

	int               m_nOrientation; //zhu     /**< 纸张方向 */
	int               m_nStandardsizes; //zhu   /**< 纸张大小 */
	int               m_nUnits;     //zhu       /**< 单位*/

	float             m_fRotation; //zhu        /**< 旋转 */	
	int               m_nBinarization; //zhu    /**< 二值化 */
	int               m_nSpiltImage; //zhu      /**< 分割图像 */

	float             m_fSensitiveThreshold_removespots;    /**< 去除斑点 */
	float             m_fSensitiveThreshold_colorretent;    /**< 底色保留 */

	bool              m_bMultiStream;    //zhu  /**< 多流输出 */
	bool              m_bFrontColor;   /**< 彩色正面 */
	bool              m_bFrontGray;    /**< 灰度正面 */
	bool              m_bFrontBW;      /**< 黑白正面 */
	bool              m_bBackColor;    /**< 彩色背面 */
	bool              m_bBackGray;     /**< 灰度背面 */
	bool              m_bBackBW;       /**< 黑白背面 */


	float             m_fRemoveBlank;           /**< 去除空白页 */
	bool              m_bRemoveBlank;           /**< 去除空白页的checkBox */
	bool              m_bRemovePunch;           /**< 去除穿孔 */
	bool              m_bSharpen;               /**< 图像锐化*/
	bool              m_bMirror;                /**< 图像镜像处理 */
	bool              m_bRemoveBack;            /**< 去除背景 */
	bool              m_bDescreen;              /**< 去网纹 */ //weizhaojian
	bool              m_bDenoise;               /**< 去噪声 */
	bool              m_bAutoCrop;              /**< 自动裁切与校正 */

protected:
	bool              m_bReadOnly;              /**< current mode */

};


#endif  //__CSCANNER_BASE_H__
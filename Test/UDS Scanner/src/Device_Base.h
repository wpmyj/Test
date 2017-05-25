/**
* @file   Device_Base.h
* @brief 扫描仪基类.
* @author UDS
* @par   Copyright (c) UDS All Rights Reserved.
* @date  11/10/2016
*/ 

#ifndef __CSCANNER_BASE_H__
#define __CSCANNER_BASE_H__

#include "Common.h"
#include "twain.h"

/** 多流信息 */    
typedef struct _MULTI_INFO
{
	float resol;
	float bright;
	float contra;
	int compre;
	float compval;
	int compqua;
	int binari;
	float thres;
	float removespots;
}MULTISTREAM_INFO;

//class CDevice_Base
class CDevice_Base
{

public:
	CDevice_Base();
	~CDevice_Base();

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
  virtual CDevice_Base* getSetting() const  ;

  /**
  * set the current settings
  * @param[in] settings the new settings for the scanner
  */
  virtual void setSetting(CDevice_Base settings) ;

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
  virtual short GetMaxPagesInADF(void) {return m_nMaxDocCount;}

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
	//virtual void GetImageData(BYTE *buffer, DWORD &dwReceived) {}

	/**
	*  @brief  用于释放最后一张纸占用的内存
  *  @see CTWAINDS_UDS::CloseDs 
	*/
	virtual void Release(){};

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

	bool              m_bMultifeedDetection;    /**<重张检测 */
	int               m_nMD_value;              /**<重张检测故障值 */

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
  short             m_nMaxDocCount;           /**< Max number of documents waiting to transfer */

	float             m_fEdgeUp;                /**< 边缘扩充 */
	float             m_fEdgeDown;              /**< 边缘扩充 */
	float             m_fEdgeLeft;              /**< 边缘扩充 */
	float             m_fEdgeRight;             /**< 边缘扩充 */
	int               m_nEdgeColor;             /**< 扩充颜色 */
	int               m_nEdgeOrientation;       /**< 扩充方向 */
	int               m_nEdgeCornercolor;       /**< 边缘扩充--未扫描部分边角填充颜色*/

	float             m_fXPos;                  /**< X偏移量 */
	float             m_fYPos;                  /**< X偏移量 */

	int               m_nCompress;              /**< 压缩格式*/
	float             m_fCompressValue;         /**< 压缩比的值*/
	int               m_nCompressQuality;       /**< 压缩质量*/
	
	bool              m_bColorFlip;             /**< 色彩翻转 */

	int               m_nCacheMode;             /**< 缓存模式 */
	float             m_fCMAuto;                /**< 缓存模式选项-自动 */
	float             m_fCMPaperNum;            /**< 缓存模式选项-纸张数量 */
	float             m_fCMMemorySize;          /**< 缓存模式选项-内存大小 */

	bool              m_bSavePower;             /**< 节电模式 */
	float             m_fSavePowerValue;        /**< 节电模式值 */
	bool              m_bOffTime;               /**< 关机时间 */ 
	float             m_fOffTimeValue;          /**< 关机时间值 */

	bool              m_bTurnVideo;             /**< 扫描仪无纸时转高拍仪 */
	int               m_nCutMethod;             /**< 图像裁切方式 */

	float             m_fNoiseNum;              /**< 去除噪声-噪声数目 */
	float             m_fNoiseRange;            /**< 去除噪声-噪声范围 */

	int               m_nFilterColor;           /**< 滤除颜色 */
	int               m_nFilterMode;            /**< 滤除模式 */
	float             m_fFilterLevel;           /**< 滤除程度 */

	//新界面新增
	int               m_nRotateMode;                /**< 图像旋转--模式 */
	int               m_nBackImgRotate;             /**< 图像旋转--背面图像旋转 */
	int               m_nJoinImage;                 /**< 图像拼接 */
	int               m_nBackProcess;               /**< 背景处理 */
	int               m_nBackProMode;               /**< 背景处理模式 */
	int               m_nBackProFill;               /**< 背景处理填补颜色 */
	float             m_fBackProStrenth;            /**< 背景处理强度 */

	bool              m_bNativeSave;             /**< 本地保存 */

	bool              m_bOverLength;                /**< 超出预定长度检测 */
	bool              m_bSeperatePage;              /**< 分隔页 */
	bool              m_bPreFeed;                   /**< 预先进纸 */
	bool              m_bCodeIdentity;              /**< 一二维码识别 */
	float             m_fOverLengthValue;           /**< 超出预定长度检测值 */  
	float             m_fPreFeedValue;              /**< 预先进纸值 */
	float             m_fWaitTime;                  /**< 连续扫描等待放纸时间 */
	int               m_nCodeStandard;              /**< 条码格式 */       

	bool              m_bIndicators;                /**< 扫描进度 */
	bool              m_bEnmergency;                /**< 紧急按钮 */

	int               m_nNoColor;               /**< 非彩色时扫描 */
	float             m_fColorThres;            /**< 色彩阈值 */
	int               m_nColorCorrect;          /**< 色彩校正 --多流Tab页面共用*/ 

	/**********************************************************
	*   数组    7    6   5    4                3    2   1    0
	* （背面） 保留 黑白 灰度 彩色    （正面）  保留 黑白 灰度 彩色
	**********************************************************/
	float             m_fBright[8];             /**< 多流亮度 */
	float             m_fResolu[8];             /**< 多流分辨率 */
	float             m_fCompVal[8];            /**< 多流压缩比的值 */
	int               m_nCompQua[8];            /**< 多流压缩质量*/

	/**********************************************************
	*   数组    7    6   5    4                3    2   1    0
	* （背面） 保留 保留 灰度 彩色    （正面）  保留 保留 灰度 彩色
	**********************************************************/
	float             m_fContra[8];             /**< 多流对比度 */

	/**********************************************************
	*   数组    7    6   5    4                3   2   1    0
	* （背面） 保留 黑白 保留 保留    （正面）  保留 黑白 保留 保留
	**********************************************************/
	int               m_nCompre[8];             /**< 多流压缩格式 */
	int               m_nBinari[8];             /**< 多流黑白二值化 */
	float             m_fThres[8];              /**< 多流黑白阈值 */
	float             m_fRemovespots[8];        /**< 多流黑白去除斑点 */

	/**********************************************************
	*  高4位 ： 7    6   5    4      低四位 ： 3   2   1    0
	* （背面） 保留 黑白 灰度 彩色    （正面）  保留 黑白 灰度 彩色
	**********************************************************/   
	BYTE              m_byteMultiValue;         /**< 多流选项值 */


	//Bayer有序抖动矩阵
	float             m_fpArray8[8][8]; /**< 8*8 */
	float             m_fpArray4[4][4];
	float             m_fpArray2[2][2];

protected:
	bool              m_bReadOnly;              /**< current mode */

};


#endif  //__CSCANNER_BASE_H__
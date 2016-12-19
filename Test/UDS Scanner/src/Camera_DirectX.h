#ifndef __CCAMERA_DIRECTX_H__
#define __CCAMERA_DIRECTX_H__
#include "CScanner_base.h"
//#include "opencv.hpp"
#include "ximage.h"  // CXImage
/**
* The FreeImage scanner define for PaperSource is ADF
*/
#define SFI_PAPERSOURCE_ADF 0
/**
* The FreeImage scanner define for PaperSource is Flatbed
*/
#define SFI_PAPERSOURCE_FB  1

//using namespace cv;

class CCamera_DirectX : public CScanner_Base
{
public:
	CCamera_DirectX(void);
	~CCamera_DirectX(void);

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

	void AdjustSetting();

	/**
  * set the current settings
  * @param[in] settings the new settings for the scanner
  */
  void setSetting(CScanner_Base settings);

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
protected:
	short             m_nDocCount;              /**< number of documents waiting to transfer */
	short             m_nMaxDocCount;           /**< Max number of documents waiting to transfer */
	WORD              m_nSourceWidth;           /**< Width of image in FreeImage */
	WORD              m_nSourceHeight;          /**< Height of image in FreeImage */
	char              m_szIniPath[PATH_MAX];    /**< INI配置文件路径 */   
	//BYTE*             m_pImageBuffer;           /**< 图片数据指针 */
	//DWORD             m_dwSize;                 /**< 图片数据大小 */
	//Mat               m_mat_image;              /**< 存储图像数据的Mat */
	int               m_nImageNumber;           /**< 图片编号 */
	CxImage*          m_pCxImage;               /**< 图片数据指针 */
	WORD              m_nScanLine;              /**< Current scan line of image in FreeImage */
	DWORD             m_nDestBytesPerRow;       /**< number of bytes needed for a row of data */

};

#endif  __CCAMERA_DIRECTX_H__

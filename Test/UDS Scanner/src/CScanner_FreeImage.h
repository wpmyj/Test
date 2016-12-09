/**
* @file CScanner_FreeImage.h
* Defines a scanner.
* Create a virtual scanner.
* @author TWAIN Working Group
* @date April 2007
*/

#ifndef __CSCANNER_H__
#define __CSCANNER_H__

#include "Common.h"
#include "FreeImage.h"
#include "twain.h"
#include "CScanner_Base.h"

//wan
#include <vector>
using namespace std;
//wan end

/**
* This is a virtual scanner.  The virtual scanner is used by the data source 
* to try and somewhat simulate some real hardware interaction.
* - It acquires the same image each time from an image file on the hard disk.
* - It uses the FreeImage library to apply transforms to the base image.
*
* For simulation purposes the virtual Scanner is set up with the 
* following specifications:
*
*   - Scanner Types
*           - ADF(Automatic Document Feeder)
*           - Flatbed
*
*   - Image sensors
*           - CCD x 3 ( Front / Back / Flatbed )
*
*   - Scanning area Minimum
*           - ADF       - A8 @ 127g/m2 paper
*           - Flatbed   - unlimited
*     
*   - Scanning area Maximum
*           - ADF       - Legal paper
*           - Flatbed   - A4 letter paper
*
*   - Internal Optical Resolution
*           - 600dpi
*
*   - Output Resolution
*           - Binary    - 50, 100, 150, 200, 300, 400, 500, & 600
*           - Grayscale - 50, 100, 150, 200, 300, 400, 500, & 600
*           - Color     - 50, 100, 150, 200, 300, 400, 500, & 600
*
*   - Internal Bit Depth
*           - 8 bits per color
*
*   - Output Bit Depth
*           - Binary    - 1 bits
*           - Grayscale - 8 bits 
*           - Color     - 8 bits per color
*
*   - Halftone Patterns
*           - Dither /error diffusion
*
*   - compression: JPEG, and FAX4 (CCITT G4)
*
*/

// Defines used by Scanner FreeImage class to set and get attributes of the vurtual scanner.

/**
* The FreeImage scanner define for PaperSource is ADF
*/
#define SFI_PAPERSOURCE_ADF 0
/**
* The FreeImage scanner define for PaperSource is Flatbed
*/
#define SFI_PAPERSOURCE_FB  1


/** 节名为“Image” */    
#define INI_SECTION_IMAGE  TEXT("Image")

/** 键名前缀为“ImagePath” */    
#define INI_KEY_PREFIX_IMAGEPATH TEXT("ImagePath")

/** 键名为“ImageCount” */    
#define INI_KEY_IMAGECOUNT TEXT("ImageCount")


/**
* The FreeImage scanner.  The software scanner using FreeImage.
* 
*/
class CScanner_FreeImage : public CScanner_Base
{
public:
  /**
  * Constructor for CScanner_FreeImage.
  */
  CScanner_FreeImage();

  /**
  * Deconstructor for CScanner_FreeImage.
  */
  ~CScanner_FreeImage();

  /**
  * Resets the scanner to factory default settings.
  * Sets the scanners caps back to defaults.
  * @return true if successfully reset.
  */
  bool resetScanner();

  /**
  * Get a scan line and put it into the _ImageXfer, fill values in _ImageXfer.
  * @param[out] pTransferBuffer a pointer to an array of bytes to store the image data
  * @param[in]  dwRead the number of bytes to read from scanner
  * @param[out] dwReceived the actual number of bytes transfered
  * @return true if successful
  */
  bool getScanStrip(BYTE *pTransferBuffer, DWORD dwRead, DWORD &dwReceived);

  /**
  * Retrieves the image to scan from disk into memory.
  * @return true if image was loaded into memory successfully.
  */
  bool acquireImage();

  /**
  * set the current settings
  * @param[in] settings the new settings for the scanner
  */
  void setSetting(CScanner_Base settings);

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
  
  short GetMaxPagesInADF(void){return m_nMaxDocCount;}
  void SetMaxPagesInADF(short nVal){m_nMaxDocCount = nVal;};

	///** 
	//* @brief 设置单个待扫图片路劲
	//* @param[in] szImagePath 新图片路径
	//*/
	//void SetImagePath_Signal(char* szImagePath);

	/**
	*  @brief  设置多张待扫图片路径.
	*  @param[in]  vector_string_imagepath 图片路径
	*/
	void SetImagePath_Multi(vector<std::string> vector_string_imagepath);

	   
	char* GetINIPath(){return m_szIniPath;};

	///**
	//*  @brief  从INI文件读取所有图片路径.  
	//*/
	//void GetImagePathFromINI();




protected:
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
	*  @brief  图像分割-裁切
	*  @param[in]  src 输入图像
	*  @param[in]  width 分割后的图像的宽
	*  @param[in]  height 分割后的图像的高
	*  @param[out]  分割后的图像
	*/
	FIBITMAP* SpiltImage(FIBITMAP *dib,int m,int n);

protected:
  FIBITMAP         *m_pDIB;                   /**< Pointer to current scanned image, 保存着位图信息和像素数据，是FreeImage 的核心 */ 
  WORD              m_nScanLine;              /**< Current scan line of image in FreeImage */
//  bool              m_bReadOnly;              /**< current mode */
  DWORD             m_nDestBytesPerRow;       /**< number of bytes needed for a row of data */
  short             m_nDocCount;              /**< number of documents waiting to transfer */
  short             m_nMaxDocCount;           /**< Max number of documents waiting to transfer */
  WORD              m_nSourceWidth;           /**< Width of image in FreeImage */
  WORD              m_nSourceHeight;          /**< Height of image in FreeImage */
  char              m_szSourceImagePath[PATH_MAX];  /**< image used with FreeImage */

	char              m_szIniPath[PATH_MAX];    /**< INI配置文件路径 */  
  
	vector<std::string>    m_vector_string_imagepath;  /**< 多张图片路径 */
};

/**
* FreeImage error handler.
* @param[in] fif format  / plugin responsible for the error.
* @param[in] message Error Message.
*/
void FreeImageErrorHandler(FREE_IMAGE_FORMAT fif, const char *message);

#endif // __CSCANNER_H__


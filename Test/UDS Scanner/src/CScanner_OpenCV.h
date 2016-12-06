/**
* @file   CScanner_OpenCV.h
* Defines a virtual scanner based on OpenCV.
* Create a virtual scanner.
* @author UDS
* @par   Copyright (c) UDS All Rights Reserved.
* @date  11/21/2016
*/ 

#ifndef __CSCANNER_OPENCV_H__
#define __CSCANNER_OPENCV_H__

//#include "Common.h"
//#include "twain.h"
#include "CScanner_Base.h"
#include <vector>
#include "opencv.hpp"

using namespace std;
using namespace cv;


#if _DEBUG

#pragma comment(lib,"opencv_core249d.lib") 
#pragma comment(lib,"opencv_highgui249d.lib") 
//#pragma comment(lib,"opencv_calib3d249d.lib") 
//#pragma comment(lib,"opencv_contrib249d.lib") 
//#pragma comment(lib,"opencv_features2d249d.lib") 
//#pragma comment(lib,"opencv_flann249d.lib") 
//#pragma comment(lib,"opencv_gpu249d.lib") 
#pragma comment(lib,"opencv_imgproc249d.lib") 
//#pragma comment(lib,"opencv_legacy249d.lib") 
//#pragma comment(lib,"opencv_ml249d.lib") 
//#pragma comment(lib,"opencv_objdetect249d.lib") 
//#pragma comment(lib,"opencv_ts249d.lib") 
//#pragma comment(lib,"opencv_video249d.lib") 

#else

#pragma comment(lib,"opencv_core249.lib") 
#pragma comment(lib,"opencv_highgui249.lib") 
//#pragma comment(lib,"opencv_calib3d249.lib") 
//#pragma comment(lib,"opencv_contrib249.lib") 
//#pragma comment(lib,"opencv_features2d249.lib") 
//#pragma comment(lib,"opencv_flann249.lib") 
//#pragma comment(lib,"opencv_gpu249.lib") 
#pragma comment(lib,"opencv_imgproc249.lib") 
//#pragma comment(lib,"opencv_legacy249.lib") 
//#pragma comment(lib,"opencv_ml249.lib") 
//#pragma comment(lib,"opencv_objdetect249.lib") 
//#pragma comment(lib,"opencv_ts249.lib") 
//#pragma comment(lib,"opencv_video249.lib") 

#endif //_DEBUG

/**
* The FreeImage scanner define for PaperSource is ADF
*/
#define SFI_PAPERSOURCE_ADF 0
/**
* The FreeImage scanner define for PaperSource is Flatbed
*/
#define SFI_PAPERSOURCE_FB  1


class CScanner_OpenCV : public CScanner_Base
{
public:
	/**
  * Constructor for CScanner_OpenCV.
  */
	CScanner_OpenCV(void);

	 /**
  * Deconstructor for CScanner_OpenCV.
  */
	~CScanner_OpenCV(void);


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
  //bool getScanStrip(BYTE *pTransferBuffer, DWORD dwRead, DWORD &dwReceived);

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
	*  @brief  设置多张待扫图片路径.
	*  @param[in]  vector_string_imagepath 图片路径
	*/
	void SetImagePath_Multi(vector<std::string> vector_string_imagepath);

	   
	char* GetINIPath(){return m_szIniPath;};


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
	*  @brief  获取图片数据.
	*  @note 仅供扫描仪G6400使用
	*  @param[out] buffer 保存图片数据buffer.
	*  @param[out] dwReceived 传输的实际大小（以字节为单位）
	*/
	void GetImageData(BYTE *buffer, DWORD &dwReceived);

	/**
	*  @brief  Mat类型转BYTE*
	*  @param[in]  matIn 待转换的Mat型变量
	*  @param[out] bytesOut  转换完成的BYTE*类型变量
	*/
	void MatToBYTEs(cv::Mat matIn, BYTE* bytesOut);

	/**
	*  @brief  逆时针旋转图像（原尺寸）
	*  @param[in]  angle 旋转角度 
	*/
	void RotateImage(double angle);

	/**
	*  @brief  中值滤波
	*/
	void MedianSmooth(const Mat &src);

	/**
	*  @brief  Gamma校正
	*  @param[in]  src 输入图像
	*  @param[in]  fGamma Gamma系数
	*  @param[out] dst 输出图像
	*/
	void GammaCorrection(const Mat& src, Mat& dst, float fGamma);

	/**
	*  @brief  图像分割-裁切
	*  @param[in]  src 输入图像
	*  @param[in]  width 分割后的图像的宽
	*  @param[in]  height 分割后的图像的高
	*/
	//void SpiltImage(const Mat& src, int width, int height);
	void SpiltImage(const Mat& src_img,int m,int n);

	/**
	*  @brief  水平镜像
	*  @param[in]  src 原图 
	*  @param[out] dst 目标图像 
	*/
	void hMirrorTrans(const Mat &src, Mat &dst);

	/**
	*  @brief  垂直镜像
	*  @param[in]  src 原图 
	*  @param[out] dst 目标图像 
	*/
	void vMirrorTrans(const Mat &src, Mat &dst);

	/**
	*  @brief  对比度与亮度调节
	*  @param[out]  pdstImage 目标图 
	*  @param[in] psrcImage 原图 
	*  @param[in] nBrightValue 亮度值
	*  @param[in] nContraValue 对比度值
	*  @retval true 表示成功
	*  @retval false 表示失败  
	*/
	bool ContrastAndBright(Mat *pdstImage,Mat *psrcImage,int nBrightValue,int nContraValue);

	/**
	*  @brief  设置阈值
	*  @param[in]  value  阈值
	*/
	void SetThreshold(int value);

protected:
  //FIBITMAP         *m_pDIB;                   /**< Pointer to current scanned image, 保存着位图信息和像素数据，是FreeImage 的核心 */ 
  WORD              m_nScanLine;              /**< Current scan line of image in FreeImage */
  DWORD             m_nDestBytesPerRow;       /**< number of bytes needed for a row of data */
  short             m_nDocCount;              /**< number of documents waiting to transfer */
  short             m_nMaxDocCount;           /**< Max number of documents waiting to transfer */
  WORD              m_nSourceWidth;           /**< Width of image in FreeImage */
  WORD              m_nSourceHeight;          /**< Height of image in FreeImage */
 
	char              m_szSourceImagePath[PATH_MAX];  /**< image used with FreeImage */
	char              m_szIniPath[PATH_MAX];    /**< INI配置文件路径 */   
	vector<std::string>    m_vector_string_imagepath;  /**< 多张图片路径 */

	cv::Mat           m_mat_image;              /**< 存储图像数据的Mat */

	double            m_dRat;                   /**< 宽/高 */
	Vector<Mat>       m_ceil_img;               /**< 分割后的图像 */
};

#endif// __CSCANNER_OPENCV_H__
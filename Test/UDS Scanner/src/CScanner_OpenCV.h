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
#include "Device_Base.h"
#include <vector>
#include "opencv.hpp"
#include "CommonDS.h"

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


class CScanner_OpenCV : public CDevice_Base
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
  * Retrieves the image to scan from disk into memory.
  * @return true if image was loaded into memory successfully.
  */
  bool acquireImage();

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
  *  @brief  获取ADF中纸张个数
	*  @return 纸张数
	*/
	short GetMaxPagesInADF(void){
		return m_nMaxDocCount;
	}

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


	void Release();
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
  * Get a scan line and put it into the _ImageXfer, fill values in _ImageXfer.
  * @param[out] pTransferBuffer a pointer to an array of bytes to store the image data
  * @param[in]  dwRead the number of bytes to read from scanner
  * @param[out] dwReceived the actual number of bytes transfered
  * @return true if successful
  */
	bool getScanStrip(BYTE *pTransferBuffer, DWORD dwRead, DWORD &dwReceived);

	/**
	*  @brief  mat转换为uchar格式
	*  @param[in]  src_img 输入，mat格式的图像数据
	*  @param[out] 输出 uchar数据
	*/
	void Mat2uchar(Mat src_img);

	/**
	*  @brief  得到scanline行的数据
	*  @param[in]  src_img 输入，mat格式的图像数据
	*  @param[out] 输出 uchar数据
	*/
	BYTE *GetScanLine(int scanline);

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
	void GammaCorrection(const Mat &src, Mat &dst, float fGamma);

	/**
	*  @brief  图像分割-裁切
	*  @param[in]  src 输入图像
	*  @param[in]  width 分割后的图像的宽
	*  @param[in]  height 分割后的图像的高
	*/
	void SpiltImage(const Mat &src_img, int m, int n);

	/**
	*  @brief  判断输入图像深度
	*  @param[in]  src 输入图像
	*  @param[out]  输出图像深度
	*/
	int FindDepth(const Mat &src_img);

	/**
	*  @brief  霍夫圆变换
	*  @param[in]  src ：输入图像 (灰度图)
	*  @param[in]  dp ：检测圆心的累加器图像的分辨率与输入图像之比的倒数，即累加器图像的反比分辨率	
	*  @param[in]  threshold1 ：第三个参数method设置的检测方法的对应的参数,Canny边缘函数的高阈值，而低阈值为高阈值的一半
	*  @param[in]  threshold2 ：表示在检测阶段圆心的累加器阈值；霍夫空间内累加和大于该阈值的点就对应于圆心；距离相同的“值"得数量大于该阈值时，认为该"值"是该圆心对应的圆半径
	                           。它越小的话，就可以检测到更多根本不存在的圆，而它越大的话，能通过检测的圆就更加接近完美的圆形
	*  @param[out]  输出变换后图像
	*/
	Mat HoughCirclesTransfer(Mat src_img ,double dp, double threshold1, double threshold2);

	/**
	*  @brief  去除穿孔
	*  @param[in]  threshold1 ：霍夫变换的第三个参数method设置的检测方法的对应的参数,Canny边缘函数的高阈值，而低阈值为高阈值的一半
	*  @param[in]  threshold2 ：霍夫变换的，表示在检测阶段圆心的累加器阈值(也就是有多少条梯度经过中心点)。它越小的话，就可以检测到更多根本不存在的圆，而它越大的话，能通过检测的圆就更加接近完美的圆形
	*  @param[out]  输出变换后图像
	*/
	Mat RemovePunch(double threshold1, double threshold2);

	/**
	*  @brief  去除黑边
	*  @param[in]  src ：输入图像
	*  @param[out]  输出去黑边后的图像
	*/
	Mat RemoveBlack(Mat src_img);

	/**
	*  @brief  去除空白页
	*  @param[in]  src ：输入图像
	*  @param[out]  是否是空白页
	*/
	bool RemoveBlank(Mat src_img, float fValue);


	/**
	*  @brief  自动校正
	*  @param[in]  src：无，直接从驱动中读取有旋转角度的图
	*  @param[out]  输出校正后的图像
	*/
	Mat AutoCorrect(); 
	
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
	*  @brief  设置多流输出
	*  @param[in]  src 原图 
	*  @param[in]  muilt BYTE类型的数据
	*  @param[out] resol 分辨率
	*  @param[out] bright 亮度
	*  @param[out] contra 对比度
	*  @param[out] dst 目标图像 
	*/
	Mat SetMuiltStream(Mat img, BYTE muilt, float& resol, float& bright, float& contra);

	/**
	*  @brief  多流输出相关函数，用来判断输入BYTE是哪一位为1
	*  @param[in]  src 原图 
	*  @param[out] BYTE 仅有为1的那一位还是1，其他均为0
	*/
	BYTE SwitchBYTE(const BYTE src);

	/**
	*  @brief 求十进制n的二进制表示中1的个数
	*  @param[in]  n  十进制数
	*  @param[out] n 该十进制数的二进制表示中1的个数 
	*/
	int BitCount(BYTE n);

	/**
	*  @brief  更换图片
	*  @param[in]  imageName 图片名  
	*/
	void ChangeImage(const TCHAR* imageName);

	/**
	*  @brief  OTSU算法
	*  @param[in]  阈值   
	*/
	int otsu(Mat image);//返回阈值

	/**
	*  @brief  色彩翻转
	*  @param[in]  src 原图 
	*  @param[out] dst 目标图像 
	*/
	void ColorFlip(const Mat &src, Mat &dst);

	//Mat applyLookUp(const Mat &src, const Mat &lookup);
	

protected:
  //FIBITMAP         *m_pDIB;                   /**< Pointer to current scanned image, 保存着位图信息和像素数据，是FreeImage 的核心 */ 
  WORD              m_nScanLine;              /**< Current scan line of image in FreeImage */
  DWORD             m_nDestBytesPerRow;       /**< number of bytes needed for a row of data */
  short             m_nDocCount;              /**< number of documents waiting to transfer */
  //short             m_nMaxDocCount;           /**< Max number of documents waiting to transfer */
  WORD              m_nSourceWidth;           /**< Width of image in FreeImage */
  WORD              m_nSourceHeight;          /**< Height of image in FreeImage */
 
	char              m_szSourceImagePath[PATH_MAX];  /**< image used with FreeImage */
	char              m_szIniPath[PATH_MAX];    /**< INI配置文件路径 */   
	vector<std::string>    m_vector_string_imagepath;  /**< 多张图片路径 */

	cv::Mat           m_mat_image;              /**< 存储图像数据的Mat */

	double            m_dRat;                   /**< 宽/高 */
	Vector<Mat>       m_ceil_img;               /**< 分割后的图像 */

	char szTWAIN_DS_DIR[PATH_MAX];              /**< 驱动DS的路径 */

	int               m_totalImageCount;        /**< 多流输出的总数 */
	int               m_frontImageCount;        /**< 多流输出中“正面图片”输出的数量，背面的相减 */
	int               m_i;                      /**< count循环变量*/

	uchar             *m_byte_image;            /**< m_mat_image转为的字节对齐的uchar类型数据*/
	int               m_widthstep;              /**< 字节对齐后的每行的字节数*/

}; 

#endif// __CSCANNER_OPENCV_H__
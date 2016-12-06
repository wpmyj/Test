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
  *  @brief  ��ȡADF��ֽ�Ÿ���
	*  @return ֽ����
	*/
	short GetMaxPagesInADF(void){return m_nMaxDocCount;}

	/**
	*  @brief  ����ADF������ɨֽ����
	*  @param[in]  nVal ���õ���Ŀ 
	*/
  void SetMaxPagesInADF(short nVal){m_nMaxDocCount = nVal;};


	/**
	*  @brief  ���ö��Ŵ�ɨͼƬ·��.
	*  @param[in]  vector_string_imagepath ͼƬ·��
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
	*  @brief  ��ȡͼƬ����.
	*  @note ����ɨ����G6400ʹ��
	*  @param[out] buffer ����ͼƬ����buffer.
	*  @param[out] dwReceived �����ʵ�ʴ�С�����ֽ�Ϊ��λ��
	*/
	void GetImageData(BYTE *buffer, DWORD &dwReceived);

	/**
	*  @brief  Mat����תBYTE*
	*  @param[in]  matIn ��ת����Mat�ͱ���
	*  @param[out] bytesOut  ת����ɵ�BYTE*���ͱ���
	*/
	void MatToBYTEs(cv::Mat matIn, BYTE* bytesOut);

	/**
	*  @brief  ��ʱ����תͼ��ԭ�ߴ磩
	*  @param[in]  angle ��ת�Ƕ� 
	*/
	void RotateImage(double angle);

	/**
	*  @brief  ��ֵ�˲�
	*/
	void MedianSmooth(const Mat &src);

	/**
	*  @brief  GammaУ��
	*  @param[in]  src ����ͼ��
	*  @param[in]  fGamma Gammaϵ��
	*  @param[out] dst ���ͼ��
	*/
	void GammaCorrection(const Mat& src, Mat& dst, float fGamma);

	/**
	*  @brief  ͼ��ָ�-����
	*  @param[in]  src ����ͼ��
	*  @param[in]  width �ָ���ͼ��Ŀ�
	*  @param[in]  height �ָ���ͼ��ĸ�
	*/
	//void SpiltImage(const Mat& src, int width, int height);
	void SpiltImage(const Mat& src_img,int m,int n);

	/**
	*  @brief  ˮƽ����
	*  @param[in]  src ԭͼ 
	*  @param[out] dst Ŀ��ͼ�� 
	*/
	void hMirrorTrans(const Mat &src, Mat &dst);

	/**
	*  @brief  ��ֱ����
	*  @param[in]  src ԭͼ 
	*  @param[out] dst Ŀ��ͼ�� 
	*/
	void vMirrorTrans(const Mat &src, Mat &dst);

	/**
	*  @brief  �Աȶ������ȵ���
	*  @param[out]  pdstImage Ŀ��ͼ 
	*  @param[in] psrcImage ԭͼ 
	*  @param[in] nBrightValue ����ֵ
	*  @param[in] nContraValue �Աȶ�ֵ
	*  @retval true ��ʾ�ɹ�
	*  @retval false ��ʾʧ��  
	*/
	bool ContrastAndBright(Mat *pdstImage,Mat *psrcImage,int nBrightValue,int nContraValue);

	/**
	*  @brief  ������ֵ
	*  @param[in]  value  ��ֵ
	*/
	void SetThreshold(int value);

protected:
  //FIBITMAP         *m_pDIB;                   /**< Pointer to current scanned image, ������λͼ��Ϣ���������ݣ���FreeImage �ĺ��� */ 
  WORD              m_nScanLine;              /**< Current scan line of image in FreeImage */
  DWORD             m_nDestBytesPerRow;       /**< number of bytes needed for a row of data */
  short             m_nDocCount;              /**< number of documents waiting to transfer */
  short             m_nMaxDocCount;           /**< Max number of documents waiting to transfer */
  WORD              m_nSourceWidth;           /**< Width of image in FreeImage */
  WORD              m_nSourceHeight;          /**< Height of image in FreeImage */
 
	char              m_szSourceImagePath[PATH_MAX];  /**< image used with FreeImage */
	char              m_szIniPath[PATH_MAX];    /**< INI�����ļ�·�� */   
	vector<std::string>    m_vector_string_imagepath;  /**< ����ͼƬ·�� */

	cv::Mat           m_mat_image;              /**< �洢ͼ�����ݵ�Mat */

	double            m_dRat;                   /**< ��/�� */
	Vector<Mat>       m_ceil_img;               /**< �ָ���ͼ�� */
};

#endif// __CSCANNER_OPENCV_H__
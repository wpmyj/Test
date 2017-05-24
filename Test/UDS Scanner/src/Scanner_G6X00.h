/**
* @file   Scanner_G6X00.h
* @brief 供扫描仪G6400与G6600使用.
* @author UDS
* @par   Copyright (c) UDS. All Rights Reserved.
* @date  03/02/2017
*/ 
#pragma once
#include "Device_Base.h"
#include "GL1.h"
#include "opencv.hpp"
#include "CommonDS.h"
#include <time.h>
//#include "Dlg_Indicators.h"

using namespace std;
using namespace cv;

#define  FILENAME_DLL_GL1   TEXT("GL1.dll")

#define  FILENAME_DLL_GL2   TEXT("GL2.dll")

/**
* The scanner define for PaperSource is ADF
*/
#define SFI_PAPERSOURCE_ADF 0
/**
* The scanner define for PaperSource is Flatbed
*/
#define SFI_PAPERSOURCE_FB  1

class CScanner_G6X00 : public CDevice_Base
{
public:
	CScanner_G6X00(void);
	~CScanner_G6X00(void);

public:
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

	/**
  * set the current settings
  * @param[in] settings the new settings for the scanner
  */
  void setSetting(CDevice_Base settings);

	/**
	*  @brief  是否内存  
	*/
	void Release();

	/**
	*  @brief  获取错误编号对应内容
	*  @param[in]  error  错误编号
	*  @return 错误内容
	*/
	const TCHAR* TranslateError(const long error);

	/**
	*  @brief  执行扫描
	*  @param[in]  inArgName 
	*  @retval true 表示成功
	*  @retval false 表示失败  
	*/
  bool RunScan();

	/**
	*  @brief  获取当前扫描页大小  
	*/
	void GetCurrentPageSize(DWORD& _dwSize);

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

	/**
	*  @brief  初始化驱动参数
	*/
	void InitDriverParamter();

	/**
	*  @brief  加载DLL及其函数
	*  @retval true 表示成功
	*  @retval false 表示失败  
	*/
	bool LoadDLL();

	/**
	*  @brief  生成Gamma表与设置其它参数
	*  @param[in]  GammaTable 256字节的GammaTable 
	*  @param[in] Brightness 亮度：-127 - 127
  *  @param[in] Contrast 对比度：-127 - 127
	*  @param[in] Highlight 高亮：1 - 255
	*  @param[in] Shadow 阴影：0 - 254  (Shadow should be smaller than Highlight)
	*  @param[in] Gamma Gamma：10 - 999 (means 0.1 - 99.9)
	*  @param[out] outArgName   
	*/
	void CalGammaTable(LPBYTE GammaTable, int Brightness, int Contrast, BYTE Highlight, BYTE Shadow, float Gamma);

	/**
	*  @brief  调整参数，将UI得到的值赋值给成员变量 
	*  @retval true 表示成功
	*  @retval false 表示失败  
	*/
	bool AdjustParameter();

  /**
	*  @brief  设置参数
	*/
	void SetParameter();

	/**
	*  @brief  获取当前扫描区域宽度与高度
	*  @param[in] _nSize ICAP_SUPPORTEDSIZES 对应值
	*  @param[out]  _fWeight 宽度 
	*  @param[out]  _fHeight 高度 
	*/
	void GetCurrentScanRange(const int& _nSize, float& _fWeight, float& _fHeight);

	/**
	*  @brief  获取图像数据行指针
	*  @param[in]  _nLine 行数 
  *  @return 第_nLine行指针  
	*/
	BYTE* MyGetScanLine(int _nLine);


	/**
	* swaps the blue and red components (for RGB images)
	* @param[in] buffer : pointer to the pixels
	* @param[in] length : number of bytes to swap. lenght may not exceed the scan line.
	* @param[in] BitsPerPixel 
	*/
	void RGB2BGR(BYTE *buffer, int length, int BitsPerPixel);


	/**
	*  @brief  实现黑白图像反转
	*  @param[in]  pBuffer  pointer to the pixels 
	*  @param[in] BitsPerPixel 
	*/
	void Invert(BYTE* pBuffer, int length, int BitsPerPixel);

	/**
	*  @brief  获取最大的DPI值
	*  @param[in]  _multi 多流值 
	*  @param[in]  _resolution  存储多流DPI的数组指针 
	*  @param[out] _max 最大DPI 
	*/
	void GetMaxResolution(const BYTE _multi, float (&_resolution)[8], float& _max);

////////////////////////////////////////////////////////////////////////////////
/// 利用OpenCV进行图像处理函数
	/**
	*  @brief  逆时针旋转图像（原尺寸）
	*  @param[in]  _src 原图 
	*  @param[out] _dst 目标图像 
	*  @param[in]  angle 旋转角度 
	*/
	void RotateImage(const Mat &_src, Mat &_dst, double angle);

	/**
	*  @brief  水平镜像
	*  @param[in]  src 原图 
	*  @param[out] dst 目标图像 
	*/
	void hMirrorTrans(const Mat &src, Mat &dst);

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
	*  @brief  去除扫描仪自身的两条线段
	*  @param[in]  src：黑白图像
	*/
	void RemoveScnnerLine(Mat &src_img);

	/**
	*  @brief  获取图像中的线段
	*  @param[in]  src：黑白图像
	*  @param[out]  图像上端的中的线段
	*/
	bool GetLines(Mat src_img, Vec4i &line); 

	/**
	*  @brief  自动校正
	*  @param[in]  src：无，直接从驱动中读取有旋转角度的图
	*  @param[out]  输出校正后的图像
	*/
	bool AutoCorrect(Mat src_img , Mat &dst_img);

	/**
	*  @brief  去除黑边
	*  @param[in]  src ：输入图像
	*  @param[out]  输出去黑边后的图像
	*/
	Rect RemoveBlack(Mat src_img);

	/**
	*  @brief  色彩翻转
	*  @param[in]  src 原图 
	*  @param[out] dst 目标图像 
	*/
	void ColorFlip(const Mat& _src, Mat& _dst);

  /**
	*  @brief  中值滤波
	*/
	void MedianSmooth(const Mat &src/*, IplImage *out*/);

	/**
	*  @brief  霍夫圆变换
	*  @param[in]  src ：输入图像 (灰度图)
	*  @param[in]  dp ：检测圆心的累加器图像的分辨率与输入图像之比的倒数，即累加器图像的反比分辨率	
	*  @param[in]  threshold1 ：第三个参数method设置的检测方法的对应的参数,Canny边缘函数的高阈值，而低阈值为高阈值的一半
	*  @param[in]  threshold2 ：表示在检测阶段圆心的累加器阈值。它越小的话，就可以检测到更多根本不存在的圆，而它越大的话，能通过检测的圆就更加接近完美的圆形
	*  @param[out]  输出变换后图像
	*/
	Mat HoughCirclesTransfer(Mat src_img ,double dp, double threshold1, double threshold2);

	/**
	*  @brief  去除穿孔
	*  @param[in] src 原图
	*  @param[in]  threshold1 ：霍夫变换的第三个参数method设置的检测方法的对应的参数,Canny边缘函数的高阈值，而低阈值为高阈值的一半
	*  @param[in]  threshold2 ：霍夫变换的，表示在检测阶段圆心的累加器阈值。它越小的话，就可以检测到更多根本不存在的圆，而它越大的话，能通过检测的圆就更加接近完美的圆形
	*  @param[out]  输出变换后图像
	*/
	Mat RemovePunch(const Mat &src, double threshold1, double threshold2);

	/**
	*  @brief  判断输入图像深度
	*  @param[in]  src 输入图像
	*  @param[out]  输出图像深度
	*/
	int FindDepth(const Mat &src_img);

	/**
	*  @brief  OTSU算法
	*  @param[in]  阈值   
	*/
	int otsu(Mat image);//返回阈值

	/**
	*  @brief  图像分割-裁切
	*  @param[in]  src 输入图像
	*  @param[in]  width 分割后的图像的宽
	*  @param[in]  height 分割后的图像的高
	*/
	void SpiltImage(const Mat &src_img, int m, int n);

	/**
	*  @brief  锐化图像
	*  @param[in]  src 输入图像
	*  @param[out] dst_img 锐化后的图像
	*/
	void SharpenImage(const Mat &src_img, Mat &dst_img);

	/**
	*  @brief  去除背景
	*  @param[in]  src_img ：输入图像
	*  @param[out]  dst_img :去背景后的图像
	*/
	void RemoveBack(const Mat &src_img, Mat &dst_img);

	/**
	*  @brief  边缘扩充
	*  @param[in]  src_img ：输入图像
	*  @param[out]  dst_img : 扩充后的数据
	*/
	void EdgeBorder(const Mat &src_img, Mat &dst_img);

	/**
	*  @brief  去除空白页
	*  @param[in]  src ：输入图像
	*  @param[out]  是否是空白页
	*/
	bool RemoveBlank(Mat src_img, float fValue);

	/**
	*  @brief  设置多流输出
	*  @param[in]  src 原图 
	*  @param[in]  muilt BYTE类型的数据
	*  @param[out] dst 目标图像 
	*/
	Mat SetMuiltStream(Mat src_img, BYTE muilt);

	/**
	*  @brief  多流输出相关函数，用来判断输入BYTE是哪一位为1
	*  @param[in]  src 原图 
	*  @note 仅有为1的那一位还是1，其他均为0
	*  return  被选中的那位
	*/
	BYTE SwitchBYTE(const BYTE src);

	/**
	*  @brief 求十进制n的二进制表示中1的个数
	*  @param[in]  n  十进制数
	*  @param[out] n 该十进制数的二进制表示中1的个数 
	*/
	int BitCount(BYTE n);

	/**
	*  @brief  设置多流输出
	*  @param[in]  src 原图 
	*  @param[in]  muilt BYTE类型的数据

	*  @param[out] resol 分辨率
	*  @param[out] bright 亮度
	*  @param[out] contra 对比度
	*  @param[out] compre 压缩选项
	*  @param[out] compval 压缩值
	*  @param[out] binari 二值化
	*  @param[out] thres 阈值
	*  @param[out] removespots 去除斑点

	*  @param[out] mat 目标图像 
	*/
	Mat SetMuiltStream(Mat src_img, BYTE muilt, float& resol, float& bright, float& contra, 
		int &compre, float &compval, int &compqua, int &binari, float &thres, float &removespots);

protected:
	InitializeDriverProc               InitializeDriver;
	InitializeScannerProc              InitializeScanner;
	GetScannerAbilityProc              GetScannerAbility;  
	GetScannerAbilityExProc            GetScannerAbilityEx;    
	GetADFStatusProc                   GetADFStatus;
	StartScanJobProc                 	 StartScanJob;
	SetScanParameterProc               SetScanParameter;
	SetGammaTableProc                  SetGammaTable;
	StartScanProc                      StartScan;
	ReadScanProc                       ReadScan;
	ReadScanExProc                     ReadScanEx;
	ReadScanEx2Proc                    ReadScanEx2;
	StopScanProc                       StopScan;
	EndScanJobProc                     EndScanJob ;
	DoEjectPaperAfterScanProc          DoEjectPaperAfterScan;
	TerminateDriverProc                TerminateDriver;
	GetLastStatusCodeProc              GetLastStatusCode;
	GetDeviceListProc                  GetDeviceList;
	DestroyDeviceListProc              DestroyDeviceList;
	SelectDeviceProc                   SelectDevice;
	GeneralOperationProc               GeneralOperation;
	ResetScannerProc                   ResetScanner;
	LampOnProc                         LampOn;
	GetBackgroundRasterProc            GetBackgroundRaster;
	TuneScanLengthProc                 TuneScanLength;
	ReadNVMDataProc                    ReadNVMData;
	SetBatchScanParameterProc          SetBatchScanParameter;
	SetSpeedModeProc                   SetSpeedMode;
	SetNVRAMValueProc                  SetNVRAMValue;  // 设置待机时间、关机时间
	DoCancelProc                       DoCancel;
protected:
	short             m_nDocCount;              /**< number of documents waiting to transfer */
	short             m_nMaxDocCount;           /**< Max number of documents waiting to transfer */
	WORD              m_nSourceWidth;           /**< Width of image in FreeImage */
	WORD              m_nSourceHeight;          /**< Height of image in FreeImage */
	TCHAR             m_szIniPath[PATH_MAX];    /**< INI配置文件路径 */   
	int               m_nImageNumber;           /**< 图片编号 */
	//CxImage*          m_pCxImage;               /**< 图片数据指针 */
	WORD              m_nScanLine;              /**< Current scan line of image in FreeImage */
	DWORD             m_nDestBytesPerRow;       /**< number of bytes needed for a row of data */


	cv::Mat           m_mat_image;              /**< 存储图像数据的Mat */
	double            m_dRat;                   /**< 宽/高 */
	Vector<Mat>       m_ceil_img;               /**< 分割后的图像 */

	char szTWAIN_DS_DIR[PATH_MAX];              /**< 驱动DS的路径 */
	uchar             *m_byte_image;            /**< m_mat_image转为的字节对齐的uchar类型数据*/
	int               m_widthstep;              /**< 字节对齐后的每行的字节数*/

	bool              m_bSpiltSkip;            /**< 是否跳过下次扫描 */
	bool              m_bMultiSkip;
	bool              m_bImageProSkip;         /**< 是否跳过图像处理*/
	cv::Mat           m_mat_muilt;             /**< 多流时，最高等级的数据 */

private:
	BYTE* m_pGammaTable;
	BYTE* m_pSaveBuffer;
	BYTE* m_pTempBuffer;
	BYTE  m_byteADFStatus;
	DWORD m_dwTotal;
	DWORD m_dwBytesPerRow;
	PIXELBOUND m_ePixelBound;
	IO_STATUS m_ioStatus;
	SCANNERABILITY m_scannerAbility;
	SCANPARAMETER m_scanParameter;
  HMODULE m_hDLL;

	int   m_nMultiTotal;  /**< 多流总的纸张数 */
	int   m_nMultiBack;   /**< 多流背面纸张数 */
	int   m_nMultiFront;  /**< 多流正面纸张数 */
public:
	bool  m_bCancel;
};


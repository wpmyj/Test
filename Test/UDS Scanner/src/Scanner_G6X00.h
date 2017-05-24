/**
* @file   Scanner_G6X00.h
* @brief ��ɨ����G6400��G6600ʹ��.
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
  * set the current settings
  * @param[in] settings the new settings for the scanner
  */
  void setSetting(CDevice_Base settings);

	/**
	*  @brief  �Ƿ��ڴ�  
	*/
	void Release();

	/**
	*  @brief  ��ȡ�����Ŷ�Ӧ����
	*  @param[in]  error  ������
	*  @return ��������
	*/
	const TCHAR* TranslateError(const long error);

	/**
	*  @brief  ִ��ɨ��
	*  @param[in]  inArgName 
	*  @retval true ��ʾ�ɹ�
	*  @retval false ��ʾʧ��  
	*/
  bool RunScan();

	/**
	*  @brief  ��ȡ��ǰɨ��ҳ��С  
	*/
	void GetCurrentPageSize(DWORD& _dwSize);

protected:
	/**
	*  @brief  ��ȡͼƬ����.
	*  @note ����ɨ����G6400ʹ��
	*  @param[out] buffer ����ͼƬ���ݵ�buffer.
	*  @param[out] dwReceived �����ʵ�ʴ�С�����ֽ�Ϊ��λ��
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
	*  @brief  ��ʼ����������
	*/
	void InitDriverParamter();

	/**
	*  @brief  ����DLL���亯��
	*  @retval true ��ʾ�ɹ�
	*  @retval false ��ʾʧ��  
	*/
	bool LoadDLL();

	/**
	*  @brief  ����Gamma����������������
	*  @param[in]  GammaTable 256�ֽڵ�GammaTable 
	*  @param[in] Brightness ���ȣ�-127 - 127
  *  @param[in] Contrast �Աȶȣ�-127 - 127
	*  @param[in] Highlight ������1 - 255
	*  @param[in] Shadow ��Ӱ��0 - 254  (Shadow should be smaller than Highlight)
	*  @param[in] Gamma Gamma��10 - 999 (means 0.1 - 99.9)
	*  @param[out] outArgName   
	*/
	void CalGammaTable(LPBYTE GammaTable, int Brightness, int Contrast, BYTE Highlight, BYTE Shadow, float Gamma);

	/**
	*  @brief  ������������UI�õ���ֵ��ֵ����Ա���� 
	*  @retval true ��ʾ�ɹ�
	*  @retval false ��ʾʧ��  
	*/
	bool AdjustParameter();

  /**
	*  @brief  ���ò���
	*/
	void SetParameter();

	/**
	*  @brief  ��ȡ��ǰɨ����������߶�
	*  @param[in] _nSize ICAP_SUPPORTEDSIZES ��Ӧֵ
	*  @param[out]  _fWeight ��� 
	*  @param[out]  _fHeight �߶� 
	*/
	void GetCurrentScanRange(const int& _nSize, float& _fWeight, float& _fHeight);

	/**
	*  @brief  ��ȡͼ��������ָ��
	*  @param[in]  _nLine ���� 
  *  @return ��_nLine��ָ��  
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
	*  @brief  ʵ�ֺڰ�ͼ��ת
	*  @param[in]  pBuffer  pointer to the pixels 
	*  @param[in] BitsPerPixel 
	*/
	void Invert(BYTE* pBuffer, int length, int BitsPerPixel);

	/**
	*  @brief  ��ȡ����DPIֵ
	*  @param[in]  _multi ����ֵ 
	*  @param[in]  _resolution  �洢����DPI������ָ�� 
	*  @param[out] _max ���DPI 
	*/
	void GetMaxResolution(const BYTE _multi, float (&_resolution)[8], float& _max);

////////////////////////////////////////////////////////////////////////////////
/// ����OpenCV����ͼ������
	/**
	*  @brief  ��ʱ����תͼ��ԭ�ߴ磩
	*  @param[in]  _src ԭͼ 
	*  @param[out] _dst Ŀ��ͼ�� 
	*  @param[in]  angle ��ת�Ƕ� 
	*/
	void RotateImage(const Mat &_src, Mat &_dst, double angle);

	/**
	*  @brief  ˮƽ����
	*  @param[in]  src ԭͼ 
	*  @param[out] dst Ŀ��ͼ�� 
	*/
	void hMirrorTrans(const Mat &src, Mat &dst);

  /**
	*  @brief  matת��Ϊuchar��ʽ
	*  @param[in]  src_img ���룬mat��ʽ��ͼ������
	*  @param[out] ��� uchar����
	*/
	void Mat2uchar(Mat src_img);

	/**
	*  @brief  �õ�scanline�е�����
	*  @param[in]  src_img ���룬mat��ʽ��ͼ������
	*  @param[out] ��� uchar����
	*/
	BYTE *GetScanLine(int scanline);

	/**
	*  @brief  ȥ��ɨ��������������߶�
	*  @param[in]  src���ڰ�ͼ��
	*/
	void RemoveScnnerLine(Mat &src_img);

	/**
	*  @brief  ��ȡͼ���е��߶�
	*  @param[in]  src���ڰ�ͼ��
	*  @param[out]  ͼ���϶˵��е��߶�
	*/
	bool GetLines(Mat src_img, Vec4i &line); 

	/**
	*  @brief  �Զ�У��
	*  @param[in]  src���ޣ�ֱ�Ӵ������ж�ȡ����ת�Ƕȵ�ͼ
	*  @param[out]  ���У�����ͼ��
	*/
	bool AutoCorrect(Mat src_img , Mat &dst_img);

	/**
	*  @brief  ȥ���ڱ�
	*  @param[in]  src ������ͼ��
	*  @param[out]  ���ȥ�ڱߺ��ͼ��
	*/
	Rect RemoveBlack(Mat src_img);

	/**
	*  @brief  ɫ�ʷ�ת
	*  @param[in]  src ԭͼ 
	*  @param[out] dst Ŀ��ͼ�� 
	*/
	void ColorFlip(const Mat& _src, Mat& _dst);

  /**
	*  @brief  ��ֵ�˲�
	*/
	void MedianSmooth(const Mat &src/*, IplImage *out*/);

	/**
	*  @brief  ����Բ�任
	*  @param[in]  src ������ͼ�� (�Ҷ�ͼ)
	*  @param[in]  dp �����Բ�ĵ��ۼ���ͼ��ķֱ���������ͼ��֮�ȵĵ��������ۼ���ͼ��ķ��ȷֱ���	
	*  @param[in]  threshold1 ������������method���õļ�ⷽ���Ķ�Ӧ�Ĳ���,Canny��Ե�����ĸ���ֵ��������ֵΪ����ֵ��һ��
	*  @param[in]  threshold2 ����ʾ�ڼ��׶�Բ�ĵ��ۼ�����ֵ����ԽС�Ļ����Ϳ��Լ�⵽������������ڵ�Բ������Խ��Ļ�����ͨ������Բ�͸��ӽӽ�������Բ��
	*  @param[out]  ����任��ͼ��
	*/
	Mat HoughCirclesTransfer(Mat src_img ,double dp, double threshold1, double threshold2);

	/**
	*  @brief  ȥ������
	*  @param[in] src ԭͼ
	*  @param[in]  threshold1 ������任�ĵ���������method���õļ�ⷽ���Ķ�Ӧ�Ĳ���,Canny��Ե�����ĸ���ֵ��������ֵΪ����ֵ��һ��
	*  @param[in]  threshold2 ������任�ģ���ʾ�ڼ��׶�Բ�ĵ��ۼ�����ֵ����ԽС�Ļ����Ϳ��Լ�⵽������������ڵ�Բ������Խ��Ļ�����ͨ������Բ�͸��ӽӽ�������Բ��
	*  @param[out]  ����任��ͼ��
	*/
	Mat RemovePunch(const Mat &src, double threshold1, double threshold2);

	/**
	*  @brief  �ж�����ͼ�����
	*  @param[in]  src ����ͼ��
	*  @param[out]  ���ͼ�����
	*/
	int FindDepth(const Mat &src_img);

	/**
	*  @brief  OTSU�㷨
	*  @param[in]  ��ֵ   
	*/
	int otsu(Mat image);//������ֵ

	/**
	*  @brief  ͼ��ָ�-����
	*  @param[in]  src ����ͼ��
	*  @param[in]  width �ָ���ͼ��Ŀ�
	*  @param[in]  height �ָ���ͼ��ĸ�
	*/
	void SpiltImage(const Mat &src_img, int m, int n);

	/**
	*  @brief  ��ͼ��
	*  @param[in]  src ����ͼ��
	*  @param[out] dst_img �񻯺��ͼ��
	*/
	void SharpenImage(const Mat &src_img, Mat &dst_img);

	/**
	*  @brief  ȥ������
	*  @param[in]  src_img ������ͼ��
	*  @param[out]  dst_img :ȥ�������ͼ��
	*/
	void RemoveBack(const Mat &src_img, Mat &dst_img);

	/**
	*  @brief  ��Ե����
	*  @param[in]  src_img ������ͼ��
	*  @param[out]  dst_img : ����������
	*/
	void EdgeBorder(const Mat &src_img, Mat &dst_img);

	/**
	*  @brief  ȥ���հ�ҳ
	*  @param[in]  src ������ͼ��
	*  @param[out]  �Ƿ��ǿհ�ҳ
	*/
	bool RemoveBlank(Mat src_img, float fValue);

	/**
	*  @brief  ���ö������
	*  @param[in]  src ԭͼ 
	*  @param[in]  muilt BYTE���͵�����
	*  @param[out] dst Ŀ��ͼ�� 
	*/
	Mat SetMuiltStream(Mat src_img, BYTE muilt);

	/**
	*  @brief  ���������غ����������ж�����BYTE����һλΪ1
	*  @param[in]  src ԭͼ 
	*  @note ����Ϊ1����һλ����1��������Ϊ0
	*  return  ��ѡ�е���λ
	*/
	BYTE SwitchBYTE(const BYTE src);

	/**
	*  @brief ��ʮ����n�Ķ����Ʊ�ʾ��1�ĸ���
	*  @param[in]  n  ʮ������
	*  @param[out] n ��ʮ�������Ķ����Ʊ�ʾ��1�ĸ��� 
	*/
	int BitCount(BYTE n);

	/**
	*  @brief  ���ö������
	*  @param[in]  src ԭͼ 
	*  @param[in]  muilt BYTE���͵�����

	*  @param[out] resol �ֱ���
	*  @param[out] bright ����
	*  @param[out] contra �Աȶ�
	*  @param[out] compre ѹ��ѡ��
	*  @param[out] compval ѹ��ֵ
	*  @param[out] binari ��ֵ��
	*  @param[out] thres ��ֵ
	*  @param[out] removespots ȥ���ߵ�

	*  @param[out] mat Ŀ��ͼ�� 
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
	SetNVRAMValueProc                  SetNVRAMValue;  // ���ô���ʱ�䡢�ػ�ʱ��
	DoCancelProc                       DoCancel;
protected:
	short             m_nDocCount;              /**< number of documents waiting to transfer */
	short             m_nMaxDocCount;           /**< Max number of documents waiting to transfer */
	WORD              m_nSourceWidth;           /**< Width of image in FreeImage */
	WORD              m_nSourceHeight;          /**< Height of image in FreeImage */
	TCHAR             m_szIniPath[PATH_MAX];    /**< INI�����ļ�·�� */   
	int               m_nImageNumber;           /**< ͼƬ��� */
	//CxImage*          m_pCxImage;               /**< ͼƬ����ָ�� */
	WORD              m_nScanLine;              /**< Current scan line of image in FreeImage */
	DWORD             m_nDestBytesPerRow;       /**< number of bytes needed for a row of data */


	cv::Mat           m_mat_image;              /**< �洢ͼ�����ݵ�Mat */
	double            m_dRat;                   /**< ��/�� */
	Vector<Mat>       m_ceil_img;               /**< �ָ���ͼ�� */

	char szTWAIN_DS_DIR[PATH_MAX];              /**< ����DS��·�� */
	uchar             *m_byte_image;            /**< m_mat_imageתΪ���ֽڶ����uchar��������*/
	int               m_widthstep;              /**< �ֽڶ�����ÿ�е��ֽ���*/

	bool              m_bSpiltSkip;            /**< �Ƿ������´�ɨ�� */
	bool              m_bMultiSkip;
	bool              m_bImageProSkip;         /**< �Ƿ�����ͼ����*/
	cv::Mat           m_mat_muilt;             /**< ����ʱ����ߵȼ������� */

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

	int   m_nMultiTotal;  /**< �����ܵ�ֽ���� */
	int   m_nMultiBack;   /**< ��������ֽ���� */
	int   m_nMultiFront;  /**< ��������ֽ���� */
public:
	bool  m_bCancel;
};


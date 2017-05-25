/**
* @file   Device_Base.h
* @brief ɨ���ǻ���.
* @author UDS
* @par   Copyright (c) UDS All Rights Reserved.
* @date  11/10/2016
*/ 

#ifndef __CSCANNER_BASE_H__
#define __CSCANNER_BASE_H__

#include "Common.h"
#include "twain.h"

/** ������Ϣ */    
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
	*  @brief  ��ȡ�Զ���ֽ��(ADF)��ֽ������.
	*  @note  ��������ɨ����ʹ��
	*  @Return:  short:ֽ������
	*/
  virtual short GetMaxPagesInADF(void) {return m_nMaxDocCount;}

	/**
	*  @brief  �����Զ���ֽ��(ADF)��ֽ������.
	*  @note  ��������ɨ����ʹ��
	*  @param[in]  nVal ֽ������   
	*/
  virtual void SetMaxPagesInADF(short nVal) {}

	/**
	*  @brief  ��ȡͼƬ����.
	*  @note ����ɨ����G6400ʹ��
	*  @param[out] buffer ����ͼƬ����buffer.
	*  @param[out] dwReceived �����ʵ�ʴ�С�����ֽ�Ϊ��λ��
	*/
	//virtual void GetImageData(BYTE *buffer, DWORD &dwReceived) {}

	/**
	*  @brief  �����ͷ����һ��ֽռ�õ��ڴ�
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

	bool              m_bMultifeedDetection;    /**<���ż�� */
	int               m_nMD_value;              /**<���ż�����ֵ */

	int               m_nOrientation; //zhu     /**< ֽ�ŷ��� */
	int               m_nStandardsizes; //zhu   /**< ֽ�Ŵ�С */
	int               m_nUnits;     //zhu       /**< ��λ*/

	float             m_fRotation; //zhu        /**< ��ת */	
	int               m_nBinarization; //zhu    /**< ��ֵ�� */
	int               m_nSpiltImage; //zhu      /**< �ָ�ͼ�� */

	float             m_fSensitiveThreshold_removespots;    /**< ȥ���ߵ� */
	float             m_fSensitiveThreshold_colorretent;    /**< ��ɫ���� */

	bool              m_bMultiStream;    //zhu  /**< ������� */
	bool              m_bFrontColor;   /**< ��ɫ���� */
	bool              m_bFrontGray;    /**< �Ҷ����� */
	bool              m_bFrontBW;      /**< �ڰ����� */
	bool              m_bBackColor;    /**< ��ɫ���� */
	bool              m_bBackGray;     /**< �Ҷȱ��� */
	bool              m_bBackBW;       /**< �ڰױ��� */

	float             m_fRemoveBlank;           /**< ȥ���հ�ҳ */
	bool              m_bRemoveBlank;           /**< ȥ���հ�ҳ��checkBox */
	bool              m_bRemovePunch;           /**< ȥ������ */
	bool              m_bSharpen;               /**< ͼ����*/
	bool              m_bMirror;                /**< ͼ������ */
	bool              m_bRemoveBack;            /**< ȥ������ */
	bool              m_bDescreen;              /**< ȥ���� */ //weizhaojian
	bool              m_bDenoise;               /**< ȥ���� */
	bool              m_bAutoCrop;              /**< �Զ�������У�� */
  short             m_nMaxDocCount;           /**< Max number of documents waiting to transfer */

	float             m_fEdgeUp;                /**< ��Ե���� */
	float             m_fEdgeDown;              /**< ��Ե���� */
	float             m_fEdgeLeft;              /**< ��Ե���� */
	float             m_fEdgeRight;             /**< ��Ե���� */
	int               m_nEdgeColor;             /**< ������ɫ */
	int               m_nEdgeOrientation;       /**< ���䷽�� */
	int               m_nEdgeCornercolor;       /**< ��Ե����--δɨ�貿�ֱ߽������ɫ*/

	float             m_fXPos;                  /**< Xƫ���� */
	float             m_fYPos;                  /**< Xƫ���� */

	int               m_nCompress;              /**< ѹ����ʽ*/
	float             m_fCompressValue;         /**< ѹ���ȵ�ֵ*/
	int               m_nCompressQuality;       /**< ѹ������*/
	
	bool              m_bColorFlip;             /**< ɫ�ʷ�ת */

	int               m_nCacheMode;             /**< ����ģʽ */
	float             m_fCMAuto;                /**< ����ģʽѡ��-�Զ� */
	float             m_fCMPaperNum;            /**< ����ģʽѡ��-ֽ������ */
	float             m_fCMMemorySize;          /**< ����ģʽѡ��-�ڴ��С */

	bool              m_bSavePower;             /**< �ڵ�ģʽ */
	float             m_fSavePowerValue;        /**< �ڵ�ģʽֵ */
	bool              m_bOffTime;               /**< �ػ�ʱ�� */ 
	float             m_fOffTimeValue;          /**< �ػ�ʱ��ֵ */

	bool              m_bTurnVideo;             /**< ɨ������ֽʱת������ */
	int               m_nCutMethod;             /**< ͼ����з�ʽ */

	float             m_fNoiseNum;              /**< ȥ������-������Ŀ */
	float             m_fNoiseRange;            /**< ȥ������-������Χ */

	int               m_nFilterColor;           /**< �˳���ɫ */
	int               m_nFilterMode;            /**< �˳�ģʽ */
	float             m_fFilterLevel;           /**< �˳��̶� */

	//�½�������
	int               m_nRotateMode;                /**< ͼ����ת--ģʽ */
	int               m_nBackImgRotate;             /**< ͼ����ת--����ͼ����ת */
	int               m_nJoinImage;                 /**< ͼ��ƴ�� */
	int               m_nBackProcess;               /**< �������� */
	int               m_nBackProMode;               /**< ��������ģʽ */
	int               m_nBackProFill;               /**< �����������ɫ */
	float             m_fBackProStrenth;            /**< ��������ǿ�� */

	bool              m_bNativeSave;             /**< ���ر��� */

	bool              m_bOverLength;                /**< ����Ԥ�����ȼ�� */
	bool              m_bSeperatePage;              /**< �ָ�ҳ */
	bool              m_bPreFeed;                   /**< Ԥ�Ƚ�ֽ */
	bool              m_bCodeIdentity;              /**< һ��ά��ʶ�� */
	float             m_fOverLengthValue;           /**< ����Ԥ�����ȼ��ֵ */  
	float             m_fPreFeedValue;              /**< Ԥ�Ƚ�ֵֽ */
	float             m_fWaitTime;                  /**< ����ɨ��ȴ���ֽʱ�� */
	int               m_nCodeStandard;              /**< �����ʽ */       

	bool              m_bIndicators;                /**< ɨ����� */
	bool              m_bEnmergency;                /**< ������ť */

	int               m_nNoColor;               /**< �ǲ�ɫʱɨ�� */
	float             m_fColorThres;            /**< ɫ����ֵ */
	int               m_nColorCorrect;          /**< ɫ��У�� --����Tabҳ�湲��*/ 

	/**********************************************************
	*   ����    7    6   5    4                3    2   1    0
	* �����棩 ���� �ڰ� �Ҷ� ��ɫ    �����棩  ���� �ڰ� �Ҷ� ��ɫ
	**********************************************************/
	float             m_fBright[8];             /**< �������� */
	float             m_fResolu[8];             /**< �����ֱ��� */
	float             m_fCompVal[8];            /**< ����ѹ���ȵ�ֵ */
	int               m_nCompQua[8];            /**< ����ѹ������*/

	/**********************************************************
	*   ����    7    6   5    4                3    2   1    0
	* �����棩 ���� ���� �Ҷ� ��ɫ    �����棩  ���� ���� �Ҷ� ��ɫ
	**********************************************************/
	float             m_fContra[8];             /**< �����Աȶ� */

	/**********************************************************
	*   ����    7    6   5    4                3   2   1    0
	* �����棩 ���� �ڰ� ���� ����    �����棩  ���� �ڰ� ���� ����
	**********************************************************/
	int               m_nCompre[8];             /**< ����ѹ����ʽ */
	int               m_nBinari[8];             /**< �����ڰ׶�ֵ�� */
	float             m_fThres[8];              /**< �����ڰ���ֵ */
	float             m_fRemovespots[8];        /**< �����ڰ�ȥ���ߵ� */

	/**********************************************************
	*  ��4λ �� 7    6   5    4      ����λ �� 3   2   1    0
	* �����棩 ���� �ڰ� �Ҷ� ��ɫ    �����棩  ���� �ڰ� �Ҷ� ��ɫ
	**********************************************************/   
	BYTE              m_byteMultiValue;         /**< ����ѡ��ֵ */


	//Bayer���򶶶�����
	float             m_fpArray8[8][8]; /**< 8*8 */
	float             m_fpArray4[4][4];
	float             m_fpArray2[2][2];

protected:
	bool              m_bReadOnly;              /**< current mode */

};


#endif  //__CSCANNER_BASE_H__
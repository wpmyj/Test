#pragma once

/**< ����qedit.hʱ�����޷��򿪰����ļ�:"dxtrans.h"�Ĵ�����������漸��  */
#pragma include_alias("dxtrans.h", "qedit.h")   ///<�������ڱ�������лὫ�ϸ�dxtrans.h�滻��qedit.h
#define __IDxtCompositor_INTERFACE_DEFINED__
#define __IDxtAlphaSetter_INTERFACE_DEFINED__
#define __IDxtJpeg_INTERFACE_DEFINED__
#define __IDxtKey_INTERFACE_DEFINED__


#include "Dshow.h"    //DirectX 11
#include "qedit.h"

#define WM_IMAGEREADY (WM_USER + 1)
#define WM_IMAGESAVED (WM_USER + 2)

// CUDSCapture

typedef struct
{
	long grayT;                        /**< ���ҶȲ� */
	long colorT;                       /**< ���ɫ�ʲ� */
	long moveRatio;                    /**< ��С�ƶ����� */
	long edgeWidth;                    /**< ��Ե��ȣ���������� */
	long edgePixel;                    /**< ����Ե���� */
	long stayPixel;                    /**< ����ȶ����� */
	long stayFrame;                    /**< ��С�ȶ�֡�� */
	long imageType;                    /**< 0-�ڰ�  1-�Ҷ�  2-��ɫ */
	long imageOrientation;             /**< ����ͼ����: 0-0, 1-90, 2-180, 3-270 */
	bool fastPreview;                  /**< �Ƿ���ÿ���Ԥ��*/
	bool autoClip;                     /**< �Ƿ����ܲ�����ת����*/
	bool autoRotate;                   /**< �Ƿ��Զ�˳ʱ��ת90�� */
	bool autoEnhance;                  /**< �Ƿ�����ĵ���ǿ */
	long docSize;                      /**< �ֶ�����ʱ���ĵ��ߴ�*/
	long docWidth, docHeight;          /**< �Զ����ֶ���������*/
  long threshold;                    /**< ��ֵ */
	long XDPI;                         /**< ˮƽ����ֱ��� */ 
	long YDPI;                         /**< ��ֱ����ֱ��� */
	CString strCamrea;                 /**< Ĭ�������*/
	CString strSize;                   /**< Ĭ��ͼ��֡�ߴ�*/
} AUTOPARAMETER; 



class CUDSCapture : public CWnd
{
	DECLARE_DYNAMIC(CUDSCapture)

public:
	CUDSCapture();
	virtual ~CUDSCapture();

public:

	/** 
	Provides methods to build Filter Graph(by Filter Graph Manager)
	*/
	IGraphBuilder *m_pGB;  

	/**
	* Provides methods for controlling the flow of data through the filter
	* graph(by Filter Graph Manager)
	*/
	IMediaControl *m_pMC; 

	/**
	* Sets properties on the video window(by Filter Graph Manager)
	*/
	IVideoWindow *m_pVW;  

	/**
	* Provides methods for building capture graph and other custom filter graph
	*/
	ICaptureGraphBuilder2 *m_pCapture; 

	/**
	* Provides methods for controlling Camera Filter
	*/
	IBaseFilter *m_pCameraFilter;  

	IBaseFilter *m_pGrabberFilter;

	ISampleGrabber *m_pGrabber;

	IMediaEventEx *m_pEvent;


	HWND m_hDlgWnd, m_hPreviewWnd;                                  /**< �Ի�������Ԥ�����ھ�� */                         
	bool m_bActive;                                                 /**< �Ƿ񼤻� */ 
	bool m_bIsPreview;                                              /**< �Ƿ�Ԥ�� */ 
	bool m_bCaptureBackground;                                      /**< �Ƿ񲶻񱳾� */
	bool m_bIsAutoClipping;                                         /**< �Ƿ�����ץ�� */
	bool m_bIsBackground;
	AUTOPARAMETER m_Auto;
	CString m_strMinSize, m_strMaxSize;
	CString m_strBarcodeType, m_strBarcodeFormat;
	long m_nBarcodeLength;
	bool m_bMultiBarcode, m_bBarcodeRotate;
	CString m_strImagePath, m_strBarcode;
	long m_nQuality, m_nPhotoNo;
	
	//��������
	long m_nBrightness;                      /**< ���� */
	long m_nContrast;                        /**< �Աȶ� */
	long m_nHue;                             /**< ɫ�� */
	long m_nSaturation;                      /**< ���Ͷ� */
	long m_nSharpness;                       /**< �� */
	long m_nGamma;                           /**< GammaУ�� */
	long m_nWhiteBalance;                    /**< ��ƽ�� */
	long m_nBacklightCompensation;           /**< ���Ա� */
	long m_nGain;                            /**< ���� */


	//���������
	long m_nZoom;                            /**< ���� */
	long m_nFocus;                           /**< ���� */
	long m_nExposure;                        /**< �ع�� */
	long m_nIris;                            /**< ��Ȧ��ʮ�ֹ�դ�� */
	long m_nPan;                             /**< ȫ�� */
	long m_nTilt;                            /**< ��б */
	long m_nRoll;                            /**< ���� */

	// Implementation
public:

		/**
	  * @brief ����δ��ʼ����Com����
	  */
	  bool CreateComInstance();

///<����Ϊ����ͼ������

		/**
		* @brief ��ȡͼƬ����
		* @param[in] *pSlider   �����������ָ��.
		* @retval  true ��ʾ�ɹ���false ��ʾʧ�� 
		*/
  	bool GetBrightness(CSliderCtrl *pSlider);              

		/**
		* @brief ����ͼƬ����
		* @param[in] *pSlider   �����������ָ��.
		* @retval  true ��ʾ�ɹ���false ��ʾʧ�� 
		*/
  	bool SetBrightness(CSliderCtrl *pSlider);

		/**
		* @brief ��ȡͼƬ�Աȶ�
		* @param[in] *pSlider   �����������ָ��.
		* @retval  true ��ʾ�ɹ���false ��ʾʧ�� 
		*/
		bool GetContrast(CSliderCtrl *pSlider);

		/**
		* @brief ����ͼƬ�Աȶ�
		* @param[in] *pSlider   �����������ָ��.
		* @retval  true ��ʾ�ɹ���false ��ʾʧ�� 
		*/
		bool SetContrast(CSliderCtrl *pSlider);

		/**
		* @brief ��ȡͼƬɫ��
		* @param[in] *pSlider   �����������ָ��.
		* @retval  true ��ʾ�ɹ���false ��ʾʧ�� 
		*/
		bool GetHue(CSliderCtrl *pSlider);

		/**
		* @brief ����ͼƬɫ��
		* @param[in] *pSlider   �����������ָ��.
		* @retval  true ��ʾ�ɹ���false ��ʾʧ�� 
		*/
		bool SetHue(CSliderCtrl *pSlider);

		/**
		* @brief ��ȡͼƬ���Ͷ�
		* @param[in] *pSlider   �����������ָ��.
		* @retval  true ��ʾ�ɹ���false ��ʾʧ�� 
		*/
		bool GetSaturation(CSliderCtrl *pSlider);

		/**
		* @brief ����ͼƬ���Ͷ�
		* @param[in] *pSlider   �����������ָ��.
		* @retval  true ��ʾ�ɹ���false ��ʾʧ�� 
		*/
		bool SetSaturation(CSliderCtrl *pSlider);

		/**
		* @brief ��ȡͼƬ�񻯶�
		* @param[in] *pSlider   �����������ָ��.
		* @retval  true ��ʾ�ɹ���false ��ʾʧ�� 
		*/
		bool GetSharpness(CSliderCtrl *pSlider);

		/**
		* @brief ����ͼƬ�񻯶�
		* @param[in] *pSlider   �����������ָ��.
		* @retval  true ��ʾ�ɹ���false ��ʾʧ�� 
		*/
		bool SetSharpness(CSliderCtrl *pSlider);

		/**
		* @brief ��ȡͼƬGammaУ������
		* @param[in] *pSlider   �����������ָ��.
		* @retval  true ��ʾ�ɹ���false ��ʾʧ�� 
		*/
		bool GetGamma(CSliderCtrl *pSlider);

		/**
		* @brief ����ͼƬGammaУ������
		* @param[in] *pSlider   �����������ָ��.
		* @retval  true ��ʾ�ɹ���false ��ʾʧ�� 
		*/
		bool SetGamma(CSliderCtrl *pSlider);

		/**
		* @brief ��ȡͼƬ��ƽ��
		* @param[in] *pSlider   �����������ָ��.
		* @retval  true ��ʾ�ɹ���false ��ʾʧ�� 
		*/
		bool GetWhiteBalance(CSliderCtrl *pSlider);

		/**
		* @brief ����ͼƬ��ƽ��
		* @param[in] *pSlider   �����������ָ��.
		* @retval  true ��ʾ�ɹ���false ��ʾʧ�� 
		*/
		bool SetWhiteBalance(CSliderCtrl *pSlider);

		/**
		* @brief ��ȡͼƬ���Ա�
		* @param[in] *pSlider   �����������ָ��.
		* @retval  true ��ʾ�ɹ���false ��ʾʧ�� 
		*/
		bool GetBacklightCompensation(CSliderCtrl *pSlider);

		/**
		* @brief ����ͼƬ���Ա�
		* @param[in] *pSlider   �����������ָ��.
		* @retval  true ��ʾ�ɹ���false ��ʾʧ�� 
		*/
		bool SetBacklightCompensation(CSliderCtrl *pSlider);

			/**
		* @brief ��ȡͼƬ����
		* @param[in] *pSlider   �����������ָ��.
		* @retval  true ��ʾ�ɹ���false ��ʾʧ�� 
		*/
		bool GetGain(CSliderCtrl *pSlider);

		/**
		* @brief ����ͼƬ����
		* @param[in] *pSlider   �����������ָ��.
		* @retval  true ��ʾ�ɹ���false ��ʾʧ�� 
		*/
		bool SetGain(CSliderCtrl *pSlider);

///<����Ϊ���������

		/**
		* @brief ��ȡ��������ű�
		* @param[in] *pSlider   �����������ָ��.
		* @retval  true ��ʾ�ɹ���false ��ʾʧ�� 
		*/
  	bool GetZoom(CSliderCtrl *pSlider);

		/**
		* @brief ������������ű�
		* @param[in] *pSlider   �����������ָ��.
		* @retval  true ��ʾ�ɹ���false ��ʾʧ�� 
		*/
  	bool SetZoom(CSliderCtrl *pSlider); 

		/**
		* @brief ��ȡ���������
		* @param[in] *pSlider   �����������ָ��.
		* @retval  true ��ʾ�ɹ���false ��ʾʧ�� 
		*/
  	bool GetFocus(CSliderCtrl *pSlider);

		/**
		* @brief �������������
		* @param[in] *pSlider   �����������ָ��.
		* @retval  true ��ʾ�ɹ���false ��ʾʧ�� 
		*/
  	bool SetFocus(CSliderCtrl *pSlider); 

    /**
		* @brief ��ȡ������ع��
		* @param[in] *pSlider   �����������ָ��.
		* @retval  true ��ʾ�ɹ���false ��ʾʧ�� 
		*/
  	bool GetExposure(CSliderCtrl *pSlider);

		/**
		* @brief ����������ع��
		* @param[in] *pSlider   �����������ָ��.
		* @retval  true ��ʾ�ɹ���false ��ʾʧ�� 
		*/
  	bool SetExposure(CSliderCtrl *pSlider); 

		/**
		* @brief ��ȡ�������Ȧ��ʮ�ֹ�դ��
		* @param[in] *pSlider   �����������ָ��.
		* @retval  true ��ʾ�ɹ���false ��ʾʧ�� 
		*/
  	bool GetIris(CSliderCtrl *pSlider);

		/**
		* @brief �����������Ȧ��ʮ�ֹ�դ��
		* @param[in] *pSlider   �����������ָ��.
		* @retval  true ��ʾ�ɹ���false ��ʾʧ�� 
		*/
  	bool SetIris(CSliderCtrl *pSlider); 

		/**
		* @brief ��ȡ�����ȫ��
		* @param[in] *pSlider   �����������ָ��.
		* @retval  true ��ʾ�ɹ���false ��ʾʧ�� 
		*/
  	bool GetPan(CSliderCtrl *pSlider);

		/**
		* @brief ���������ȫ��
		* @param[in] *pSlider   �����������ָ��.
		* @retval  true ��ʾ�ɹ���false ��ʾʧ�� 
		*/
  	bool SetPan(CSliderCtrl *pSlider); 

		/**
		* @brief ��ȡ�������б��
		* @param[in] *pSlider   �����������ָ��.
		* @retval  true ��ʾ�ɹ���false ��ʾʧ�� 
		*/
  	bool GetTilt(CSliderCtrl *pSlider);

		/**
		* @brief �����������б��
		* @param[in] *pSlider   �����������ָ��.
		* @retval  true ��ʾ�ɹ���false ��ʾʧ�� 
		*/
  	bool SetTilt(CSliderCtrl *pSlider); 

			/**
		* @brief ��ȡ�����������
		* @param[in] *pSlider   �����������ָ��.
		* @retval  true ��ʾ�ɹ���false ��ʾʧ�� 
		*/
  	bool GetRoll(CSliderCtrl *pSlider);

		/**
		* @brief ���������������
		* @param[in] *pSlider   �����������ָ��.
		* @retval  true ��ʾ�ɹ���false ��ʾʧ�� 
		*/
  	bool SetRoll(CSliderCtrl *pSlider); 


		/**
		* @brief ������������
		* @param[in] hParentWnd   ���Ի�����.
		* @retval  true ��ʾ�ɹ���false ��ʾʧ�� 
		*/
  	bool ConfigCameraPin(HWND hParentWnd);

		/**
		* @brief �����˲���
		* @param[in] hParentWnd   ���Ի�����.
		* @retval  true ��ʾ�ɹ���false ��ʾʧ�� 
		*/
  	bool ConfigCameraFilter(HWND hParentWnd);

		/**
		* @brief ���ұ������ӵ������豸�б�
		* @param[in] pCamList   �豸�б���Ͽ��������ָ��.
		* @retval -1��ʾʧ�� ,���򷵻����ӵ��豸��Ŀ 
		*/
  	long GetDevicesList(CComboBox *pCamList);     

		/**
		* @brief                ѡ��������
		* @param[in] devNo      �豸���.
		* @param[in] pSizeListt �����б���Ͽ��������ָ��.
		* @retval true ��ʾ�ɹ���false ��ʾʧ��
		*/
	  bool SelectCamera(int devNo, CComboBox *pSizeList);    

		/**
		* @brief                      �Ƿ���Ȩ
		* @param[in] devName          �豸����.
		* @retval true ��ʾ�ɹ���false ��ʾʧ��
		*/
  	bool IsAuthorizedCamera(CString devName);              

		/**
		* @brief ����������
		* @retval true ��ʾ�ɹ� 
		* @retval false ��ʾʧ��  
		*/
	  bool SetupCamera();  

		/**
		* @brief ֹͣ���� 
		*/
		void StopCamera();                                     

		/**
		* @brief �ͷ���Դ
		*/
		void ReleaseAll();                                       


protected:
	DECLARE_MESSAGE_MAP()
};



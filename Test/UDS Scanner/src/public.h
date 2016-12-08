/**
* @file   public.h
* @brief Common defines and typedefs used by the UI, DS, and scanner.
* @author UDS
* @par   Copyright (c) UDS All Rights Reserved.
* @date  10/31/2016
*/ 

#ifndef __PUBLIC_H__
#define __PUBLIC_H__

/** �ļ������� */
#define FILENAME_INI TEXT("UDS General TWAIN DS.INI")
#define FILENAME_WAV_CAMERA1 TEXT("Camera1.WAV")
#define FILENAME_WAV_CAMERA2 TEXT("Camera2.WAV")


/** INI�ڳ��� */
#define INI_APP_DEVICE TEXT("Device")
#define INI_APP_CAMERASETTING TEXT("Camera Setting")

/** INI��(Key)���� */
#define INI_KEY_DEVICENUMBER TEXT("DeviceNumber")
#define INI_KEY_CAMERA TEXT("Camera")
#define INI_KEY_CAMTEMPPATH TEXT("CamTempPath")
#define INI_KEY_CAMFRAMESIZE TEXT("CamFrameSize")
#define INI_KEY_CAMEXPOSURE TEXT("CamExposure")
#define INI_KEY_CAMBRIGHTNESS TEXT("CamBrightness")
#define INI_KEY_CAMIMAGETYPE TEXT("CamImageType")
#define INI_KEY_CAMDOCSIZE TEXT("CamDocSize")
#define INI_KEY_CAMDOCWIDTH TEXT("CamDocWidth")
#define INI_KEY_CAMDOCHEIGHT TEXT("CamDocHeight")
#define INI_KEY_CAMAUTOCLIP TEXT("CamAutoClip")
#define INI_KEY_CAMAUTOENHANCE TEXT("CamAutoEnhance")
#define INI_KEY_CAMORIENTATION TEXT("CamOrientation")
#define INI_KEY_CAMAUTOROTATE TEXT("CamAutoRotate")

/** ��Ϣ�������������� */    
#define MB_CAPTION TEXT("UDS General TWAIN DS")

/** ������汾�� */
#define VERSION_MAJORNUMBER 2

/** ������汾�� */
#define VERSION_MINORNUMBER 3

/** �豸��� */
typedef enum DEVICE_NUMBER
{
	DEVICE_FREEIMAGE	= 1,   /**< ����FreeIamge������ɨ���� */
	DEVICE_G6400      = 2,   /**< ɨ����G6400 */
	DEVICE_OPENCV     = 3,   /**< ����OpenCV������ɨ���� */
	DEVICE_CAMERA     = 4,   /**< ����ͷ */
};

/** �Զ���ͼƬ��Ϣ,���ڱ��������ͼƬ */
typedef struct _CUST_IMAGEIFO
{
	string imagePath;      /**< ͼƬ����·�� */
	int    imageWidth;     /**< ͼƬ��ȣ���λ:������ */
	int    imageHeight;    /**< ͼƬ�߶ȣ���λ:������ */
	float  XResolution;    /**< ˮƽ����ֱ��ʣ���λ:DPI */
	float  YResolution;    /**< ��ֱ����ֱ��ʣ���λ:DPI */
}CUST_IMAGEINFO;

///** INI�ļ��е�Camera���� */
//struct INI_CAMERA
//{
//	CString Camera;
//	CString CamFrameSize;
//	long CamExposure;
//	long CamBrightness;
//	long CamImageType;
//	long CamDocSize;
//	long CamDocWidth;
//	long CamDocHeight;
//	long CamOrientation;
//	bool CamAutoClip ;
//	bool CamAutoEnhance;
//	bool CamAutoRotate;
//};


#define UDSCAP_LONGDOCUMENT             CAP_CUSTOMBASE+1
#define UDSCAP_DOCS_IN_ADF              CAP_CUSTOMBASE+2
/* Added by zhu */
/** ��ӵĹ��ܼ���ֵ */   
#define UDSCAP_BINARIZATION             CAP_CUSTOMBASE+3  /**< ��ֵ�� */
#define UDSCAP_SPLITIMAGE               CAP_CUSTOMBASE+4  /**< ͼ��ָ� */
#define UDSCAP_SENSITIVETHRESHOLD_REMOVESPOTS       CAP_CUSTOMBASE+5  /**< ȥ���ߵ� ����һ��slider����CAP��Ϊ������*/
#define UDSCAP_SENSITIVETHRESHOLD_COLORRETENT       CAP_CUSTOMBASE+8  /**< ��ɫ���� */

#define UDSCAP_PUNCHHOLEREMOVEL         CAP_CUSTOMBASE+6  /**< ȥ������ */
#define UDSCAP_SHARPEN                  CAP_CUSTOMBASE+7  /**< ͼ���� */
//#define CUSTCAP_GAMMA                 CAP_CUSTOMBASE+8  /**< Gammaͼ��У��    Twain����ICAP_GAMMA*/
#define UDSCAP_MIRROR                   CAP_CUSTOMBASE+9  /**< ͼ������ */
#define UDSCAP_REMOVEBACKGROUND         CAP_CUSTOMBASE+10 /**< ȥ������ */
#define UDSCAP_DESCREEN                 CAP_CUSTOMBASE+11 /**< ȥ���� */
#define UDSCAP_DENOISE                  CAP_CUSTOMBASE+12 /**< ȥ���� */
#define UDSCAP_AUTOCROP                 CAP_CUSTOMBASE+13 /**< �Զ�����У�� */

#define UDSCAP_MULTIFEEDDETECT          CAP_CUSTOMBASE+14 /**< ���ż�� */
#define UDSCAP_MULTISTREAM              CAP_CUSTOMBASE+15 /**< ������� */

/** UDSCAP_BINARIZATION values (BZ_ means BINARIZATION) ��ֵ����Ӧȡֵ */
#define TWBZ_DYNATHRESHOLD         0
#define TWBZ_FIXEDTHRESHOLD        1
#define TWBZ_HALFTONE1             2
#define TWBZ_HALFTONE2             3
#define TWBZ_HALFTONE3             4
#define TWBZ_HALFTONE4             5
#define TWBZ_HALFTONE5             6
#define TWBZ_ERRORDIFF             7

/** UDSCAP_SPLITIMAGE values (SI_ means SPLITIMAGE) �ָ�ͼ���Ӧȡֵ */
#define TWSI_NONE                  0
#define TWSI_HORIZONTAL            1
#define TWSI_VERTICAL              2

/* UDSCAP_PUNCHHOLEREMOVEL values (RP_ means REMOVELPUNCHHOLE)ȥ�����ס�*/
#define TWRP_DISABLE              FALSE 
#define TWRP_AUTO                 TRUE  

/* UDSCAP_SHARPEN values (SP_ means SHARPEN)ͼ���񻯡�*/
#define TWSP_DISABLE              FALSE
#define TWSP_AUTO                 TRUE 

/* CUSTCAP_GAMMA values (GM_ means GAMMA)Gammaͼ��У����*/
#define TWGM_DISABLE              FALSE
#define TWGM_AUTO                 TRUE 

/* UDSCAP_MIRROR values (MR_ means MIRROR)ͼ������*/
#define TWMR_DISABLE              FALSE
#define TWMR_AUTO                 TRUE 

/* UDSCAP_REMOVEBACKGROUND values (RB_ means REMOVEBACKGROUND)ȥ��������*/
#define TWRB_DISABLE              FALSE
#define TWRB_AUTO                 TRUE 

/* UDSCAP_DESCREEN values (DS_ means DESCREEN)ȥ���ơ�*/
#define TWDS_DISABLE              FALSE
#define TWDS_AUTO                 TRUE 

/* UDSCAP_DENOISE values (DN_ means DENOISE)ȥ������*/
#define TWDN_DISABLE              FALSE
#define TWDN_AUTO                 TRUE 

/* UDSCAP_AUTOCROP values (AC_ means AUTOCROP)�Զ�����У����*/
#define TWAC_DISABLE              FALSE
#define TWAC_AUTO                 TRUE


/* ICAP_SUPPORTEDSIZES values (SS_ means Supported Sizes) ����ҳ���С�Ĺ̶���֡��С��added by zhu*/
#define TWSS_PHOT64               55
#define TWSS_PHOT53               56

#endif //__PUBLIC_H__
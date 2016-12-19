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

#define UDSCAP_FRONTCOLOR               CAP_CUSTOMBASE+16 /**< ��ɫ���� */
#define UDSCAP_FRONTGRAY                CAP_CUSTOMBASE+17 /**< �Ҷ����� */
#define UDSCAP_FRONTBW                  CAP_CUSTOMBASE+18 /**< �ڰ����� */
#define UDSCAP_BACKCOLOR                CAP_CUSTOMBASE+19 /**< ��ɫ���� */
#define UDSCAP_BACKGRAY                 CAP_CUSTOMBASE+20 /**< �Ҷȱ��� */
#define UDSCAP_BACKBW                   CAP_CUSTOMBASE+21 /**< �ڰױ��� */


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


/* UDSCAP_FRONTCOLOR values (FC_ means FRONTCOLOR)��ɫ���档*/
#define TWFC_DISABLE              FALSE 
#define TWFC_AUTO                 TRUE 

/* UDSCAP_FRONTGRAY values (FG_ means FRONTGRAY)�Ҷ����档*/
#define TWFG_DISABLE              FALSE 
#define TWFG_AUTO                 TRUE 

/* UDSCAP_FRONTBW values (FB_ means FRONTBW)�ڰ����档*/
#define TWFB_DISABLE              FALSE 
#define TWFB_AUTO                 TRUE 

/* UDSCAP_BACKCOLOR values (BC_ means BACKCOLOR)��ɫ���档*/
#define TWBC_DISABLE              FALSE 
#define TWBC_AUTO                 TRUE 

/* UDSCAP_BACKGRAY values (BG_ means BACKGRAY)�Ҷȱ��档*/
#define TWBG_DISABLE              FALSE 
#define TWBG_AUTO                 TRUE 

/* UDSCAP_BACKBW values (BB_ means BACKBW)�ڰױ��档*/
#define TWBB_DISABLE              FALSE 
#define TWBB_AUTO                 TRUE 

#endif //__PUBLIC_H__
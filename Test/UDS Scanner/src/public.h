/**
* @file   public.h
* @brief Common defines and typedefs used by the UI, DS, and scanner.
* @author UDS
* @par   Copyright (c) UDS All Rights Reserved.
* @date  10/31/2016
*/ 

#ifndef __PUBLIC_H__
#define __PUBLIC_H__

/** 文件名常量 */
#define FILENAME_INI TEXT("UDS General TWAIN DS.INI")
#define FILENAME_WAV_CAMERA1 TEXT("Camera1.WAV")
#define FILENAME_WAV_CAMERA2 TEXT("Camera2.WAV")


/** INI节常量 */
#define INI_APP_DEVICE TEXT("Device")
#define INI_APP_CAMERASETTING TEXT("Camera Setting")

/** INI键(Key)常量 */
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

/** 消息框标题或驱动名称 */    
#define MB_CAPTION TEXT("UDS General TWAIN DS")

/** 软件主版本号 */
#define VERSION_MAJORNUMBER 2

/** 软件副版本号 */
#define VERSION_MINORNUMBER 3

/** 设备编号 */
typedef enum DEVICE_NUMBER
{
	DEVICE_FREEIMAGE	= 1,   /**< 基于FreeIamge的虚拟扫描仪 */
	DEVICE_G6400      = 2,   /**< 扫描仪G6400 */
	DEVICE_OPENCV     = 3,   /**< 基于OpenCV的虚拟扫描仪 */
	DEVICE_CAMERA     = 4,   /**< 摄像头 */
};


#define UDSCAP_LONGDOCUMENT             CAP_CUSTOMBASE+1
#define UDSCAP_DOCS_IN_ADF              CAP_CUSTOMBASE+2
/* Added by zhu */
/** 添加的功能及其值 */   
#define UDSCAP_BINARIZATION             CAP_CUSTOMBASE+3  /**< 二值化 */
#define UDSCAP_SPLITIMAGE               CAP_CUSTOMBASE+4  /**< 图像分割 */
#define UDSCAP_SENSITIVETHRESHOLD_REMOVESPOTS       CAP_CUSTOMBASE+5  /**< 去除斑点 共用一个slider，但CAP分为两部分*/
#define UDSCAP_SENSITIVETHRESHOLD_COLORRETENT       CAP_CUSTOMBASE+8  /**< 底色保留 */

#define UDSCAP_PUNCHHOLEREMOVEL         CAP_CUSTOMBASE+6  /**< 去除穿孔 */
#define UDSCAP_SHARPEN                  CAP_CUSTOMBASE+7  /**< 图像锐化 */
//#define CUSTCAP_GAMMA                 CAP_CUSTOMBASE+8  /**< Gamma图像校正    Twain已有ICAP_GAMMA*/
#define UDSCAP_MIRROR                   CAP_CUSTOMBASE+9  /**< 图像镜像处理 */
#define UDSCAP_REMOVEBACKGROUND         CAP_CUSTOMBASE+10 /**< 去除背景 */
#define UDSCAP_DESCREEN                 CAP_CUSTOMBASE+11 /**< 去网纹 */
#define UDSCAP_DENOISE                  CAP_CUSTOMBASE+12 /**< 去噪声 */
#define UDSCAP_AUTOCROP                 CAP_CUSTOMBASE+13 /**< 自动裁切校正 */

#define UDSCAP_MULTIFEEDDETECT          CAP_CUSTOMBASE+14 /**< 重张检测 */
#define UDSCAP_MULTISTREAM              CAP_CUSTOMBASE+15 /**< 多流输出 */

#define UDSCAP_FRONTCOLOR               CAP_CUSTOMBASE+16 /**< 彩色正面 */
#define UDSCAP_FRONTGRAY                CAP_CUSTOMBASE+17 /**< 灰度正面 */
#define UDSCAP_FRONTBW                  CAP_CUSTOMBASE+18 /**< 黑白正面 */
#define UDSCAP_BACKCOLOR                CAP_CUSTOMBASE+19 /**< 彩色背面 */
#define UDSCAP_BACKGRAY                 CAP_CUSTOMBASE+20 /**< 灰度背面 */
#define UDSCAP_BACKBW                   CAP_CUSTOMBASE+21 /**< 黑白背面 */


/** UDSCAP_BINARIZATION values (BZ_ means BINARIZATION) 二值化对应取值 */
#define TWBZ_DYNATHRESHOLD         0
#define TWBZ_FIXEDTHRESHOLD        1
#define TWBZ_HALFTONE1             2
#define TWBZ_HALFTONE2             3
#define TWBZ_HALFTONE3             4
#define TWBZ_HALFTONE4             5
#define TWBZ_HALFTONE5             6
#define TWBZ_ERRORDIFF             7

/** UDSCAP_SPLITIMAGE values (SI_ means SPLITIMAGE) 分割图像对应取值 */
#define TWSI_NONE                  0
#define TWSI_HORIZONTAL            1
#define TWSI_VERTICAL              2

/* UDSCAP_PUNCHHOLEREMOVEL values (RP_ means REMOVELPUNCHHOLE)去除穿孔。*/
#define TWRP_DISABLE              FALSE 
#define TWRP_AUTO                 TRUE  

/* UDSCAP_SHARPEN values (SP_ means SHARPEN)图像锐化。*/
#define TWSP_DISABLE              FALSE
#define TWSP_AUTO                 TRUE 

/* CUSTCAP_GAMMA values (GM_ means GAMMA)Gamma图像校正。*/
#define TWGM_DISABLE              FALSE
#define TWGM_AUTO                 TRUE 

/* UDSCAP_MIRROR values (MR_ means MIRROR)图像镜像处理。*/
#define TWMR_DISABLE              FALSE
#define TWMR_AUTO                 TRUE 

/* UDSCAP_REMOVEBACKGROUND values (RB_ means REMOVEBACKGROUND)去除背景。*/
#define TWRB_DISABLE              FALSE
#define TWRB_AUTO                 TRUE 

/* UDSCAP_DESCREEN values (DS_ means DESCREEN)去网纹。*/
#define TWDS_DISABLE              FALSE
#define TWDS_AUTO                 TRUE 

/* UDSCAP_DENOISE values (DN_ means DENOISE)去噪声。*/
#define TWDN_DISABLE              FALSE
#define TWDN_AUTO                 TRUE 

/* UDSCAP_AUTOCROP values (AC_ means AUTOCROP)自动裁切校正。*/
#define TWAC_DISABLE              FALSE
#define TWAC_AUTO                 TRUE


/* ICAP_SUPPORTEDSIZES values (SS_ means Supported Sizes) 典型页面大小的固定的帧大小。added by zhu*/
#define TWSS_PHOT64               55
#define TWSS_PHOT53               56


/* UDSCAP_FRONTCOLOR values (FC_ means FRONTCOLOR)彩色正面。*/
#define TWFC_DISABLE              FALSE 
#define TWFC_AUTO                 TRUE 

/* UDSCAP_FRONTGRAY values (FG_ means FRONTGRAY)灰度正面。*/
#define TWFG_DISABLE              FALSE 
#define TWFG_AUTO                 TRUE 

/* UDSCAP_FRONTBW values (FB_ means FRONTBW)黑白正面。*/
#define TWFB_DISABLE              FALSE 
#define TWFB_AUTO                 TRUE 

/* UDSCAP_BACKCOLOR values (BC_ means BACKCOLOR)彩色背面。*/
#define TWBC_DISABLE              FALSE 
#define TWBC_AUTO                 TRUE 

/* UDSCAP_BACKGRAY values (BG_ means BACKGRAY)灰度背面。*/
#define TWBG_DISABLE              FALSE 
#define TWBG_AUTO                 TRUE 

/* UDSCAP_BACKBW values (BB_ means BACKBW)黑白背面。*/
#define TWBB_DISABLE              FALSE 
#define TWBB_AUTO                 TRUE 

#endif //__PUBLIC_H__
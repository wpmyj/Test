/**
* @file   public.h
* @brief Common defines and typedefs used by the UI, DS, and scanner.
* @author UDS
* @par   Copyright (c) UDS All Rights Reserved.
* @date  10/31/2016
*/ 

#ifndef __PUBLIC_H__
#define __PUBLIC_H__

/** INI配置文件名常量 */
#define INI_FILENAME TEXT("UDS_Scanner.ini")

/** INI节常量 */
#define INI_APP_DEVICE TEXT("Device")

/** INI键(Key)常量 */
#define INI_KEY_DEVICENUMBER TEXT("DeviceNumber")

/** 消息框标题或驱动名称 */    
#define MB_CAPTION TEXT("UDS General TWAIN Scanner")

/** 设备编号 */
typedef enum DEVICE_NUMBER
{
	DEVICE_FREEIMAGE	= 1,  /**< 虚拟扫描仪 */
	DEVICE_G6400      = 2   /**< 扫描仪G6400 */
};


/* Added by zhu */
/** 添加的功能及其值 */   
#define CUSTCAP_BINARIZATION             CAP_CUSTOMBASE+3  /**< 二值化 */
#define CUSTCAP_SPLITIMAGE               CAP_CUSTOMBASE+4  /**< 图像分割 */
#define CUSTCAP_SENSITIVETHRESHOLD       CAP_CUSTOMBASE+5  /**< 去除斑点 */

#define CUSTCAP_PUNCHHOLEREMOVEL         CAP_CUSTOMBASE+6  /**< 去除穿孔 */
#define CUSTCAP_SHARPEN                  CAP_CUSTOMBASE+7  /**< 图像锐化 */
//#define CUSTCAP_GAMMA                    CAP_CUSTOMBASE+8  /**< Gamma图像校正 Twain已有ICAP_GAMMA*/
#define CUSTCAP_MIRROR                   CAP_CUSTOMBASE+9  /**< 图像镜像处理 */
#define CUSTCAP_REMOVEBACKGROUND         CAP_CUSTOMBASE+10 /**< 去除背景 */
#define CUSTCAP_DESCREEN                 CAP_CUSTOMBASE+11 /**< 去网纹 */
#define CUSTCAP_DENOISE                  CAP_CUSTOMBASE+12 /**< 去噪声 */
#define CUSTCAP_AUTOCROP                 CAP_CUSTOMBASE+13 /**< 自动裁切校正 */

/** CUSTCAP_BINARIZATION values (BZ_ means BINARIZATION) 二值化对应取值 */
#define TWBZ_DYNATHRESHOLD         0
#define TWBZ_FIXEDTHRESHOLD        1
#define TWBZ_HALFTONE1             2
#define TWBZ_HALFTONE2             3
#define TWBZ_HALFTONE3             4
#define TWBZ_HALFTONE4             5
#define TWBZ_HALFTONE5             6
#define TWBZ_ERRORDIFF             7

/** CUSTCAP_SPLITIMAGE values (SI_ means SPLITIMAGE) 分割图像对应取值 */
#define TWSI_NONE                  0
#define TWSI_HORIZONTAL            1
#define TWSI_VERTICAL              2

/* CUSTCAP_PUNCHHOLEREMOVEL values (RP_ means REMOVELPUNCHHOLE)去除穿孔。*/
#define TWRP_DISABLE              -2
#define TWRP_AUTO                 -1  

/* CUSTCAP_SHARPEN values (SP_ means SHARPEN)图像锐化。*/
#define TWSP_DISABLE              -2
#define TWSP_AUTO                 -1 

/* CUSTCAP_GAMMA values (GM_ means GAMMA)Gamma图像校正。*/
#define TWGM_DISABLE              -2
#define TWGM_AUTO                 -1 

/* CUSTCAP_MIRROR values (MR_ means MIRROR)图像镜像处理。*/
#define TWMR_DISABLE              -2
#define TWMR_AUTO                 -1 

/* CUSTCAP_REMOVEBACKGROUND values (RB_ means REMOVEBACKGROUND)去除背景。*/
#define TWRB_DISABLE              -2
#define TWRB_AUTO                 -1 

/* CUSTCAP_DESCREEN values (DS_ means DESCREEN)去网纹。*/
#define TWDS_DISABLE              -2
#define TWDS_AUTO                 -1 

/* CUSTCAP_DENOISE values (DN_ means DENOISE)去噪声。*/
#define TWDN_DISABLE              -2
#define TWDN_AUTO                 -1 

/* CUSTCAP_AUTOCROP values (AC_ means AUTOCROP)自动裁切校正。*/
#define TWAC_DISABLE              -2
#define TWAC_AUTO                 -1 

#endif //__PUBLIC_H__
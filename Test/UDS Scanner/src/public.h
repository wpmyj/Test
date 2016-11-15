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


//zhu 添加的功能及其值
#define CUSTCAP_BINARIZATION CAP_CUSTOMBASE+3  //zhu 二值化
#define CUSTCAP_SPLITIMAGE CAP_CUSTOMBASE+4  //zhu 图像分割
#define CUSTCAP_SENSITIVETHRESHOLD CAP_CUSTOMBASE+5 //zhu 去除斑点

/* CUSTCAP_BINARIZATION values (BZ_ means BINARIZATION) 二值化对应取值zhu*/
#define TWBZ_DYNATHRESHOLD         0
#define TWBZ_FIXEDTHRESHOLD        1
#define TWBZ_HALFTONE1             2
#define TWBZ_HALFTONE2             3
#define TWBZ_HALFTONE3             4
#define TWBZ_HALFTONE4             5
#define TWBZ_HALFTONE5             6
#define TWBZ_ERRORDIFF             7
/* Added by zhu */

/* CUSTCAP_SPLITIMAGE values (SI_ means SPLITIMAGE) 分割图像对应取值zhu*/
#define TWSI_NONE                  0
#define TWSI_HORIZONTAL            1
#define TWSI_VERTICAL              2

  

#endif //__PUBLIC_H__
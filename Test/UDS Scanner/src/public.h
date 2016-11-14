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
  

#endif //__PUBLIC_H__
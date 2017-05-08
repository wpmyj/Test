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
#define FILENAME_INI TEXT("UDS Universal TWAIN DS.INI")
#define FILENAME_WAV_CAMERA1 TEXT("Camera1.WAV")
#define FILENAME_WAV_CAMERA2 TEXT("Camera2.WAV")


/** INI节常量 */
#define INI_APP_DEVICE TEXT("Device")
#define INI_APP_CAMERASETTING TEXT("Camera Setting")

/** INI键(Key)常量 */
#define INI_KEY_DEVICENUMBER                 TEXT("DeviceNumber")
//#define INI_KEY_CAMERAINDEX                  TEXT("CameraIndex")
#define INI_KEY_DPIINDEX                     TEXT("DpiIndex")
#define INI_KEY_AUTOCROP                     TEXT("AutoCrop")
#define INI_KEY_SHOWINFO                     TEXT("ShowInfo")
#define INI_KEY_PIXELTYPE                    TEXT("PixelType")
#define INI_KEY_CAMERANAME                   TEXT("CameraName")
#define INI_KEY_PLAYSOUND                    TEXT("PlaySound")
#define INI_KEY_NOUI                         TEXT("NoUI")
#define INI_KEY_DSNAME                       TEXT("DSName")
/** 消息框标题或驱动名称 */    
#define MB_CAPTION TEXT("UDS Universal TWAIN DS")

/** 软件主版本号 */
#define VERSION_MAJORNUMBER 1

/** 软件副版本号 */
#define VERSION_MINORNUMBER 0

/** 设备编号 */
typedef enum DEVICE_NUMBER
{
	DEVICE_OPENCV     = 1,   /**< 基于OpenCV的虚拟扫描仪 */
	DEVICE_CAMERA     = 2,   /**< 摄像头 */
	DEVICE_G6400      = 3,   /**< 扫描仪G6400 */
	DEVICE_G6600	    = 4,   /**< 扫描仪G6600 */
};

#endif //__PUBLIC_H__
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
#define INI_KEY_DEVICENUMBER                 TEXT("DeviceNumber")
//#define INI_KEY_CAMERAINDEX                  TEXT("CameraIndex")
#define INI_KEY_DPIINDEX                     TEXT("DpiIndex")
#define INI_KEY_AUTOCROP                     TEXT("AutoCrop")
#define INI_KEY_SHOWINFO                     TEXT("ShowInfo")
#define INI_KEY_PIXELTYPE                    TEXT("PixelType")
#define INI_KEY_CAMERANAME                   TEXT("CameraName")

/** ��Ϣ�������������� */    
#define MB_CAPTION TEXT("UDS General TWAIN DS")

/** ������汾�� */
#define VERSION_MAJORNUMBER 2

/** ������汾�� */
#define VERSION_MINORNUMBER 3

/** �豸��� */
typedef enum DEVICE_NUMBER
{
	DEVICE_OPENCV     = 1,   /**< ����OpenCV������ɨ���� */
	DEVICE_CAMERA     = 2,   /**< ����ͷ */
	//DEVICE_G6400      = 3,   /**< ɨ����G6400 */
	//DEVICE_FREEIMAGE	= 1,   /**< ����FreeIamge������ɨ���� */
};

#endif //__PUBLIC_H__
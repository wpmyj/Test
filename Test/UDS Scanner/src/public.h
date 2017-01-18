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
#define INI_KEY_CAMERA                       TEXT("Camera")
#define INI_KEY_CAMTEMPPATH                  TEXT("ImagePath")
#define INI_KEY_CAMFRAMESIZE                 TEXT("CamFrameSize")
#define INI_KEY_CAMEXPOSURE                  TEXT("CamExposure")
#define INI_KEY_CAMBRIGHTNESS                TEXT("CamBrightness")
#define INI_KEY_CAMIMAGETYPE                 TEXT("CamImageType")
#define INI_KEY_CAMDOCSIZE                   TEXT("CamDocSize")
#define INI_KEY_CAMDOCWIDTH                  TEXT("CamDocWidth")
#define INI_KEY_CAMDOCHEIGHT                 TEXT("CamDocHeight")
#define INI_KEY_CAMORIENTATION               TEXT("CamOrientation")
#define INI_KEY_JPEGQUALITY                  TEXT("JpegQuality ")
#define INI_KEY_CAMAUTOCLIP                  TEXT("CamAutoClip")
#define INI_KEY_CAMAUTOENHANCE               TEXT("CamAutoEnhance")
#define INI_KEY_CAMAUTOROTATE                TEXT("CamAutoRotate")
#define INI_KEY_UPLOADFILEONEBYONE           TEXT("UploadFileOnebyOne")
#define INI_KEY_SHOWTHUMBNAIL                TEXT("ShowThumbnail")
#define INI_KEY_THRESHOLD                    TEXT("Threshold")
#define INI_KEY_DPINUMBER                    TEXT("DpiNumber")
#define INI_KEY_SAVEAS                       TEXT("SaveAs")

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

#endif //__PUBLIC_H__
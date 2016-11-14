/**
* @file   public.h
* @brief Common defines and typedefs used by the UI, DS, and scanner.
* @author UDS
* @par   Copyright (c) UDS All Rights Reserved.
* @date  10/31/2016
*/ 

#ifndef __PUBLIC_H__
#define __PUBLIC_H__

/** INI�����ļ������� */
#define INI_FILENAME TEXT("UDS_Scanner.ini")

/** INI�ڳ��� */
#define INI_APP_DEVICE TEXT("Device")

/** INI��(Key)���� */
#define INI_KEY_DEVICENUMBER TEXT("DeviceNumber")

/** ��Ϣ�������������� */    
#define MB_CAPTION TEXT("UDS General TWAIN Scanner")

/** �豸��� */
typedef enum DEVICE_NUMBER
{
	DEVICE_FREEIMAGE	= 1,  /**< ����ɨ���� */
	DEVICE_G6400      = 2   /**< ɨ����G6400 */
};
  

#endif //__PUBLIC_H__
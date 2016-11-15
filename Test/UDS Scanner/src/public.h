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


//zhu ��ӵĹ��ܼ���ֵ
#define CUSTCAP_BINARIZATION CAP_CUSTOMBASE+3  //zhu ��ֵ��
#define CUSTCAP_SPLITIMAGE CAP_CUSTOMBASE+4  //zhu ͼ��ָ�
#define CUSTCAP_SENSITIVETHRESHOLD CAP_CUSTOMBASE+5 //zhu ȥ���ߵ�

/* CUSTCAP_BINARIZATION values (BZ_ means BINARIZATION) ��ֵ����Ӧȡֵzhu*/
#define TWBZ_DYNATHRESHOLD         0
#define TWBZ_FIXEDTHRESHOLD        1
#define TWBZ_HALFTONE1             2
#define TWBZ_HALFTONE2             3
#define TWBZ_HALFTONE3             4
#define TWBZ_HALFTONE4             5
#define TWBZ_HALFTONE5             6
#define TWBZ_ERRORDIFF             7
/* Added by zhu */

/* CUSTCAP_SPLITIMAGE values (SI_ means SPLITIMAGE) �ָ�ͼ���Ӧȡֵzhu*/
#define TWSI_NONE                  0
#define TWSI_HORIZONTAL            1
#define TWSI_VERTICAL              2

  

#endif //__PUBLIC_H__
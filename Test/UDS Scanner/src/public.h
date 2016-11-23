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
	DEVICE_G6400      = 2,   /**< ɨ����G6400 */
	DEVICE_OPENCV     = 3,
};


#define UDSCAP_LONGDOCUMENT             CAP_CUSTOMBASE+1
#define UDSCAP_DOCS_IN_ADF              CAP_CUSTOMBASE+2
/* Added by zhu */
/** ��ӵĹ��ܼ���ֵ */   
#define UDSCAP_BINARIZATION             CAP_CUSTOMBASE+3  /**< ��ֵ�� */
#define UDSCAP_SPLITIMAGE               CAP_CUSTOMBASE+4  /**< ͼ��ָ� */
#define UDSCAP_SENSITIVETHRESHOLD       CAP_CUSTOMBASE+5  /**< ȥ���ߵ� */

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
#define TWRP_DISABLE              -2
#define TWRP_AUTO                 -1  

/* UDSCAP_SHARPEN values (SP_ means SHARPEN)ͼ���񻯡�*/
#define TWSP_DISABLE              -2
#define TWSP_AUTO                 -1 

/* CUSTCAP_GAMMA values (GM_ means GAMMA)Gammaͼ��У����*/
#define TWGM_DISABLE              -2
#define TWGM_AUTO                 -1 

/* UDSCAP_MIRROR values (MR_ means MIRROR)ͼ������*/
#define TWMR_DISABLE              -2
#define TWMR_AUTO                 -1 

/* UDSCAP_REMOVEBACKGROUND values (RB_ means REMOVEBACKGROUND)ȥ��������*/
#define TWRB_DISABLE              -2
#define TWRB_AUTO                 -1 

/* UDSCAP_DESCREEN values (DS_ means DESCREEN)ȥ���ơ�*/
#define TWDS_DISABLE              -2
#define TWDS_AUTO                 -1 

/* UDSCAP_DENOISE values (DN_ means DENOISE)ȥ������*/
#define TWDN_DISABLE              -2
#define TWDN_AUTO                 -1 

/* UDSCAP_AUTOCROP values (AC_ means AUTOCROP)�Զ�����У����*/
#define TWAC_DISABLE              -2
#define TWAC_AUTO                 -1


/* ICAP_SUPPORTEDSIZES values (SS_ means Supported Sizes) ����ҳ���С�Ĺ̶���֡��С��added by zhu*/
#define TWSS_PHOT64               55
#define TWSS_PHOT53               56

#endif //__PUBLIC_H__
/**
* @file  UDS_Cap.h 
* @brief New CAP used by the DSand scanner.
* @author UDS
* @par   Copyright (c) UDS All Rights Reserved.
* @date  1/18/2017
*/

#define UDSCAP_LONGDOCUMENT             CAP_CUSTOMBASE+1
#define UDSCAP_DOCS_IN_ADF              CAP_CUSTOMBASE+2
/* Added by zhu */
/** ��ӵĹ��ܼ���ֵ */   
#define UDSCAP_BINARIZATION             CAP_CUSTOMBASE+3  /**< ��ֵ�� */
#define UDSCAP_SPLITIMAGE               CAP_CUSTOMBASE+4  /**< ͼ��ָ� */
#define UDSCAP_REMOVESPOTS              CAP_CUSTOMBASE+5  /**< ȥ���ߵ� ����һ��slider����CAP��Ϊ������*/
#define UDSCAP_SENSITIVETHRESHOLD_COLORRETENT       CAP_CUSTOMBASE+8  /**< ��ɫ���� */

#define UDSCAP_PUNCHHOLEREMOVEL         CAP_CUSTOMBASE+6  /**< ȥ������ */
#define UDSCAP_SHARPEN                  CAP_CUSTOMBASE+7  /**< ͼ���� */

#define UDSCAP_MIRROR                   CAP_CUSTOMBASE+9  /**< ͼ������ */
#define UDSCAP_REMOVEBACKGROUND         CAP_CUSTOMBASE+10 /**< ȥ������ */
#define UDSCAP_DESCREEN                 CAP_CUSTOMBASE+11 /**< ȥ���� */
#define UDSCAP_DENOISE                  CAP_CUSTOMBASE+12 /**< ȥ���� */
#define UDSCAP_AUTOCROP                 CAP_CUSTOMBASE+13 /**< �Զ�����У�� */

#define UDSCAP_MULTIFEEDDETECT          CAP_CUSTOMBASE+14 /**< ���ż�� */
#define UDSCAP_MULTISTREAM              CAP_CUSTOMBASE+15 /**< ������� */

#define UDSCAP_REMOVEBLANK              CAP_CUSTOMBASE+16 /**< ȥ���հ�ҳ ������¼CheckBox��״̬��TWAIN�Դ���������¼������ֵ */
#define UDSCAP_MULTISTREAM_VALUE        CAP_CUSTOMBASE+17 /**< �������ѡ��ֵ */

//#define UDSCAP_SPLITIMAGE_VH            CAP_CUSTOMBASE+18 /**< ͼ��ָ��Զ����Ǵ�ֱ��ˮƽ*/

#define UDSCAP_EDGE_UP                  CAP_CUSTOMBASE+18   /**< ��Ե������������ */
#define UDSCAP_EDGE_DOWN                CAP_CUSTOMBASE+19
#define UDSCAP_EDGE_LEFT                CAP_CUSTOMBASE+20
#define UDSCAP_EDGE_RIGHT               CAP_CUSTOMBASE+21

#define UDSCAP_XPOS                     CAP_CUSTOMBASE+22  /**< Xƫ���� */
#define UDSCAP_YPOS                     CAP_CUSTOMBASE+23

#define UDSCAP_COMPRESSVALUE            CAP_CUSTOMBASE+24  /**< ѹ���� */

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
#define TWSI_DEFINED               3 //�Զ���

/* UDSCAP_PUNCHHOLEREMOVEL values (RP_ means REMOVELPUNCHHOLE)ȥ�����ס�*/
#define TWRP_DISABLE              FALSE 
#define TWRP_AUTO                 TRUE  

/* UDSCAP_SHARPEN values (SP_ means SHARPEN)ͼ���񻯡�*/
#define TWSP_DISABLE              FALSE
#define TWSP_AUTO                 TRUE 

/* UDSCAP_REMOVEBLANK values (RA_ means REMOVEBLANK)ȥ���հ�ҳ��*/
#define TWRA_DISABLE              FALSE 
#define TWRA_AUTO                 TRUE  

/* CUSTCAP_GAMMA values (GM_ means GAMMA)Gammaͼ��У����*/
#define TWGM_DISABLE              FALSE
#define TWGM_AUTO                 TRUE 

/* UDSCAP_MIRROR values (MR_ means MIRROR)ͼ������*/
#define TWMR_DISABLE              FALSE
#define TWMR_AUTO                 TRUE 

/* UDSCAP_REMOVEBACKGROUND values (RB_ means REMOVEBACKGROUND)ȥ��������*/
#define TWRB_DISABLE              FALSE
#define TWRB_AUTO                 TRUE 

/* UDSCAP_DESCREEN values (DS_ means DESCREEN)ȥ���ơ�*/
#define TWDS_DISABLE              FALSE
#define TWDS_AUTO                 TRUE 

/* UDSCAP_DENOISE values (DN_ means DENOISE)ȥ������*/
#define TWDN_DISABLE              FALSE
#define TWDN_AUTO                 TRUE 

/* UDSCAP_AUTOCROP values (AC_ means AUTOCROP)�Զ�����У����*/
#define TWAC_DISABLE              FALSE
#define TWAC_AUTO                 TRUE

/* UDSCAP_FRONTCOLOR values (FC_ means FRONTCOLOR)��ɫ���档*/
#define TWFC_DISABLE              FALSE 
#define TWFC_AUTO                 TRUE 

/* UDSCAP_FRONTGRAY values (FG_ means FRONTGRAY)�Ҷ����档*/
#define TWFG_DISABLE              FALSE 
#define TWFG_AUTO                 TRUE 

/* UDSCAP_FRONTBW values (FB_ means FRONTBW)�ڰ����档*/
#define TWFB_DISABLE              FALSE 
#define TWFB_AUTO                 TRUE 

/* UDSCAP_BACKCOLOR values (BC_ means BACKCOLOR)��ɫ���档*/
#define TWBC_DISABLE              FALSE 
#define TWBC_AUTO                 TRUE 

/* UDSCAP_BACKGRAY values (BG_ means BACKGRAY)�Ҷȱ��档*/
#define TWBG_DISABLE              FALSE 
#define TWBG_AUTO                 TRUE 

/* UDSCAP_BACKBW values (BB_ means BACKBW)�ڰױ��档*/
#define TWBB_DISABLE              FALSE 
#define TWBB_AUTO                 TRUE 

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
/** 添加的功能及其值 */   
#define UDSCAP_BINARIZATION             CAP_CUSTOMBASE+3  /**< 二值化 */
#define UDSCAP_SPLITIMAGE               CAP_CUSTOMBASE+4  /**< 图像分割 */
#define UDSCAP_REMOVESPOTS              CAP_CUSTOMBASE+5  /**< 去除斑点 共用一个slider，但CAP分为两部分*/
#define UDSCAP_SENSITIVETHRESHOLD_COLORRETENT       CAP_CUSTOMBASE+8  /**< 底色保留 */

#define UDSCAP_PUNCHHOLEREMOVEL         CAP_CUSTOMBASE+6  /**< 去除穿孔 */
#define UDSCAP_SHARPEN                  CAP_CUSTOMBASE+7  /**< 图像锐化 */

#define UDSCAP_MIRROR                   CAP_CUSTOMBASE+9  /**< 图像镜像处理 */
#define UDSCAP_REMOVEBACKGROUND         CAP_CUSTOMBASE+10 /**< 去除背景 */
#define UDSCAP_DESCREEN                 CAP_CUSTOMBASE+11 /**< 去网纹 */
#define UDSCAP_DENOISE                  CAP_CUSTOMBASE+12 /**< 去噪声 */
#define UDSCAP_AUTOCROP                 CAP_CUSTOMBASE+13 /**< 自动裁切校正 */

#define UDSCAP_MULTIFEEDDETECT          CAP_CUSTOMBASE+14 /**< 重张检测 */
#define UDSCAP_MULTISTREAM              CAP_CUSTOMBASE+15 /**< 多流输出 */

#define UDSCAP_REMOVEBLANK              CAP_CUSTOMBASE+16 /**< 去除空白页 用来记录CheckBox的状态，TWAIN自带的用来记录滑动条值 */
#define UDSCAP_MULTISTREAM_VALUE        CAP_CUSTOMBASE+17 /**< 多流输出选项值 */

//#define UDSCAP_SPLITIMAGE_VH            CAP_CUSTOMBASE+18 /**< 图像分割自定义是垂直、水平*/

#define UDSCAP_EDGE_UP                  CAP_CUSTOMBASE+18   /**< 边缘扩充上下左右 */
#define UDSCAP_EDGE_DOWN                CAP_CUSTOMBASE+19
#define UDSCAP_EDGE_LEFT                CAP_CUSTOMBASE+20
#define UDSCAP_EDGE_RIGHT               CAP_CUSTOMBASE+21

#define UDSCAP_XPOS                     CAP_CUSTOMBASE+22  /**< X偏移量 */
#define UDSCAP_YPOS                     CAP_CUSTOMBASE+23

#define UDSCAP_COMPRESSVALUE            CAP_CUSTOMBASE+24  /**< 压缩比 */

/** UDSCAP_BINARIZATION values (BZ_ means BINARIZATION) 二值化对应取值 */
#define TWBZ_DYNATHRESHOLD         0
#define TWBZ_FIXEDTHRESHOLD        1
#define TWBZ_HALFTONE1             2
#define TWBZ_HALFTONE2             3
#define TWBZ_HALFTONE3             4
#define TWBZ_HALFTONE4             5
#define TWBZ_HALFTONE5             6
#define TWBZ_ERRORDIFF             7

/** UDSCAP_SPLITIMAGE values (SI_ means SPLITIMAGE) 分割图像对应取值 */
#define TWSI_NONE                  0
#define TWSI_HORIZONTAL            1
#define TWSI_VERTICAL              2
#define TWSI_DEFINED               3 //自定义

/* UDSCAP_PUNCHHOLEREMOVEL values (RP_ means REMOVELPUNCHHOLE)去除穿孔。*/
#define TWRP_DISABLE              FALSE 
#define TWRP_AUTO                 TRUE  

/* UDSCAP_SHARPEN values (SP_ means SHARPEN)图像锐化。*/
#define TWSP_DISABLE              FALSE
#define TWSP_AUTO                 TRUE 

/* UDSCAP_REMOVEBLANK values (RA_ means REMOVEBLANK)去除空白页。*/
#define TWRA_DISABLE              FALSE 
#define TWRA_AUTO                 TRUE  

/* CUSTCAP_GAMMA values (GM_ means GAMMA)Gamma图像校正。*/
#define TWGM_DISABLE              FALSE
#define TWGM_AUTO                 TRUE 

/* UDSCAP_MIRROR values (MR_ means MIRROR)图像镜像处理。*/
#define TWMR_DISABLE              FALSE
#define TWMR_AUTO                 TRUE 

/* UDSCAP_REMOVEBACKGROUND values (RB_ means REMOVEBACKGROUND)去除背景。*/
#define TWRB_DISABLE              FALSE
#define TWRB_AUTO                 TRUE 

/* UDSCAP_DESCREEN values (DS_ means DESCREEN)去网纹。*/
#define TWDS_DISABLE              FALSE
#define TWDS_AUTO                 TRUE 

/* UDSCAP_DENOISE values (DN_ means DENOISE)去噪声。*/
#define TWDN_DISABLE              FALSE
#define TWDN_AUTO                 TRUE 

/* UDSCAP_AUTOCROP values (AC_ means AUTOCROP)自动裁切校正。*/
#define TWAC_DISABLE              FALSE
#define TWAC_AUTO                 TRUE

/* UDSCAP_FRONTCOLOR values (FC_ means FRONTCOLOR)彩色正面。*/
#define TWFC_DISABLE              FALSE 
#define TWFC_AUTO                 TRUE 

/* UDSCAP_FRONTGRAY values (FG_ means FRONTGRAY)灰度正面。*/
#define TWFG_DISABLE              FALSE 
#define TWFG_AUTO                 TRUE 

/* UDSCAP_FRONTBW values (FB_ means FRONTBW)黑白正面。*/
#define TWFB_DISABLE              FALSE 
#define TWFB_AUTO                 TRUE 

/* UDSCAP_BACKCOLOR values (BC_ means BACKCOLOR)彩色背面。*/
#define TWBC_DISABLE              FALSE 
#define TWBC_AUTO                 TRUE 

/* UDSCAP_BACKGRAY values (BG_ means BACKGRAY)灰度背面。*/
#define TWBG_DISABLE              FALSE 
#define TWBG_AUTO                 TRUE 

/* UDSCAP_BACKBW values (BB_ means BACKBW)黑白背面。*/
#define TWBB_DISABLE              FALSE 
#define TWBB_AUTO                 TRUE 

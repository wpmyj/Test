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
#define UDSCAP_SENSITIVETHRESHOLD_COLORRETENT       CAP_CUSTOMBASE+6  /**< 底色保留 */

#define UDSCAP_PUNCHHOLEREMOVEL         CAP_CUSTOMBASE+7  /**< 去除穿孔 */
#define UDSCAP_SHARPEN                  CAP_CUSTOMBASE+8  /**< 图像锐化 */

#define UDSCAP_MIRROR                   CAP_CUSTOMBASE+9  /**< 图像镜像处理 */
#define UDSCAP_REMOVEBACKGROUND         CAP_CUSTOMBASE+10 /**< 去除背景 */
#define UDSCAP_DESCREEN                 CAP_CUSTOMBASE+11 /**< 去网纹 */
#define UDSCAP_DENOISE                  CAP_CUSTOMBASE+12 /**< 去噪声 */
#define UDSCAP_AUTOCROP                 CAP_CUSTOMBASE+13 /**< 自动裁切校正 */

#define UDSCAP_MULTIFEEDDETECT          CAP_CUSTOMBASE+14 /**< 重张检测 */
#define UDSCAP_MULTIFEEDDETECT_VALUE    CAP_CUSTOMBASE+15 /**< 重张检测故障值 */

#define UDSCAP_MULTISTREAM              CAP_CUSTOMBASE+16 /**< 多流输出 */

#define UDSCAP_REMOVEBLANK              CAP_CUSTOMBASE+17 /**< 去除空白页 用来记录CheckBox的状态，TWAIN自带的用来记录滑动条值 */
#define UDSCAP_MULTISTREAM_VALUE        CAP_CUSTOMBASE+18 /**< 多流输出选项值 */

//#define UDSCAP_SPLITIMAGE_VH            CAP_CUSTOMBASE+18 /**< 图像分割自定义是垂直、水平*/

#define UDSCAP_EDGE_UP                  CAP_CUSTOMBASE+19   /**< 边缘扩充上下左右 */
#define UDSCAP_EDGE_DOWN                CAP_CUSTOMBASE+20
#define UDSCAP_EDGE_LEFT                CAP_CUSTOMBASE+21
#define UDSCAP_EDGE_RIGHT               CAP_CUSTOMBASE+22

#define UDSCAP_XPOS                     CAP_CUSTOMBASE+23  /**< X偏移量 */
#define UDSCAP_YPOS                     CAP_CUSTOMBASE+24

#define UDSCAP_COMPRESSVALUE            CAP_CUSTOMBASE+25  /**< 压缩比 */
#define UDSCAP_COLORFLIP                CAP_CUSTOMBASE+26  /**< 色彩翻转 */

#define UDSCAP_POWERSAVING               CAP_CUSTOMBASE+27  /**< 节电模式 */
#define UDSCAP_POWERSAVING_TIME          CAP_CUSTOMBASE+28  /**< 节电模式值：时间 */

#define UDSCAP_POWEROFF                  CAP_CUSTOMBASE+29  /**< 关机时间 */
#define UDSCAP_POWEROFF_TIME             CAP_CUSTOMBASE+30  /**< 关机时间值：时间 */

#define UDSCAP_TURNVIDEO                CAP_CUSTOMBASE+31  /**< 扫描仪无纸时转高拍仪 */

#define UDSCAP_CUTMETHOD                CAP_CUSTOMBASE+32  /**< 图像裁切方式 */

#define UDSCAP_NOISENUM                 CAP_CUSTOMBASE+33  /**< 去除噪声-噪声数目 */
#define UDSCAP_NOISERANGE               CAP_CUSTOMBASE+34  /**< 去除噪声-噪声范围 */

#define UDSCAP_FILTERCOLOR              CAP_CUSTOMBASE+35  /**< 滤除颜色 */
#define UDSCAP_FILTERMODE               CAP_CUSTOMBASE+36  /**< 滤除模式 */
#define UDSCAP_FILTERLEVEL              CAP_CUSTOMBASE+37  /**< 滤除程度 */

//多流亮度 Brightness
#define UDSCAP_BRIGHTNESSFC                 CAP_CUSTOMBASE+38
#define UDSCAP_BRIGHTNESSFG                 CAP_CUSTOMBASE+39
#define UDSCAP_BRIGHTNESSFB                 CAP_CUSTOMBASE+40
#define UDSCAP_BRIGHTNESSBC                 CAP_CUSTOMBASE+41
#define UDSCAP_BRIGHTNESSBG                 CAP_CUSTOMBASE+42
#define UDSCAP_BRIGHTNESSBB                 CAP_CUSTOMBASE+43

//多流分辨率 Resolution
#define UDSCAP_RESOLUTIONFC                 CAP_CUSTOMBASE+44
#define UDSCAP_RESOLUTIONFG                 CAP_CUSTOMBASE+45
#define UDSCAP_RESOLUTIONFB                 CAP_CUSTOMBASE+46
#define UDSCAP_RESOLUTIONBC                 CAP_CUSTOMBASE+47
#define UDSCAP_RESOLUTIONBG                 CAP_CUSTOMBASE+48
#define UDSCAP_RESOLUTIONBB                 CAP_CUSTOMBASE+49

//多流对比度 Contrast
#define UDSCAP_CONTRASTFC                   CAP_CUSTOMBASE+50
#define UDSCAP_CONTRASTFG                   CAP_CUSTOMBASE+51
#define UDSCAP_CONTRASTBC                   CAP_CUSTOMBASE+52
#define UDSCAP_CONTRASTBG                   CAP_CUSTOMBASE+53

//多流压缩  ICAP_COMPRESSION
#define UDSCAP_COMPRESSIONFC                 CAP_CUSTOMBASE+54
#define UDSCAP_COMPRESSIONFG                 CAP_CUSTOMBASE+55
#define UDSCAP_COMPRESSIONFB                 CAP_CUSTOMBASE+56
#define UDSCAP_COMPRESSIONBC                 CAP_CUSTOMBASE+57
#define UDSCAP_COMPRESSIONBG                 CAP_CUSTOMBASE+58
#define UDSCAP_COMPRESSIONBB                 CAP_CUSTOMBASE+59

//多流压缩比的值  ICAP_COMPRESSIONVALUE
#define UDSCAP_COMPRESSIONVALUEFC            CAP_CUSTOMBASE+60
#define UDSCAP_COMPRESSIONVALUEFG            CAP_CUSTOMBASE+61
#define UDSCAP_COMPRESSIONVALUEFB            CAP_CUSTOMBASE+62
#define UDSCAP_COMPRESSIONVALUEBC            CAP_CUSTOMBASE+63
#define UDSCAP_COMPRESSIONVALUEBG            CAP_CUSTOMBASE+64
#define UDSCAP_COMPRESSIONVALUEBB            CAP_CUSTOMBASE+65

//多流二值化(仅黑白)
#define UDSCAP_BINARIZATIONFB                CAP_CUSTOMBASE+66
#define UDSCAP_BINARIZATIONBB                CAP_CUSTOMBASE+67
//阈值
#define UDSCAP_THRESHOLDFB                   CAP_CUSTOMBASE+68
#define UDSCAP_THRESHOLDBB                   CAP_CUSTOMBASE+69
//去除斑点
#define UDSCAP_REMOVESPOTSFB                 CAP_CUSTOMBASE+70
#define UDSCAP_REMOVESPOTSBB                 CAP_CUSTOMBASE+71

#define UDSCAP_EDGE_COLOR                    CAP_CUSTOMBASE+72  /**< 边缘扩充颜色 */

#define UDSCAP_NOCOLOR                       CAP_CUSTOMBASE+73  /**< 非彩色时扫描 */
#define UDSCAP_COLORTHRESHOLD                CAP_CUSTOMBASE+74  /**< 色彩阈值 */

#define UDSCAP_COMPRESSQUALITY               CAP_CUSTOMBASE+75  /**< 压缩质量 */                   

//多流压缩质量  UDSCAP_COMPRESSQUALITY
#define UDSCAP_COMPRESSQUALITYFC             CAP_CUSTOMBASE+76
#define UDSCAP_COMPRESSQUALITYFG             CAP_CUSTOMBASE+77
#define UDSCAP_COMPRESSQUALITYFB             CAP_CUSTOMBASE+78
#define UDSCAP_COMPRESSQUALITYBC             CAP_CUSTOMBASE+79
#define UDSCAP_COMPRESSQUALITYBG             CAP_CUSTOMBASE+80
#define UDSCAP_COMPRESSQUALITYBB             CAP_CUSTOMBASE+81 

//新界面增加参数
//（图像处理部分）
#define UDSCAP_ROTATEMODE                    CAP_CUSTOMBASE+82 /**< 图像旋转--模式 */
#define UDSCAP_BACKIMGROTATE                 CAP_CUSTOMBASE+83 /**< 图像旋转--背面图像旋转 */

#define UDSCAP_JOINIMAGE                     CAP_CUSTOMBASE+84 /**< 图像拼接 */

#define UDSCAP_BACKPROCESS                   CAP_CUSTOMBASE+85 /**< 背景处理 */
#define UDSCAP_BACKPROSTRENTH                CAP_CUSTOMBASE+86 /**< 背景处理强度 */
#define UDSCAP_BACKPROMODE                   CAP_CUSTOMBASE+87 /**< 背景处理模式 */
#define UDSCAP_BACKPROFILLCOLOR              CAP_CUSTOMBASE+88 /**< 背景处理填补颜色 */

#define UDSCAP_COLORCORRECT                  CAP_CUSTOMBASE+89  /**< 色彩校正 */ 
#define UDSCAP_COLORCORRECTFC                CAP_CUSTOMBASE+90  /**<  多流色彩优化*/
#define UDSCAP_COLORCORRECTBC                CAP_CUSTOMBASE+91  /**<  多流色彩优化*/

#define UDSCAP_NATIVESAVE                    CAP_CUSTOMBASE+92  /**<  本地保存*/

#define UDSCAP_EDGE_ORIENTATION              CAP_CUSTOMBASE+93 /**<  边缘扩充方向*/
#define UDSCAP_EDGE_CORNERCOLOR              CAP_CUSTOMBASE+94 /**<  边缘扩充--未扫描部分边角填充颜色*/

#define UDSCAP_OVERLENGTH                    CAP_CUSTOMBASE+95  /**<  超出预定长度检测*/
#define UDSCAP_OVERLENGTHVALUE               CAP_CUSTOMBASE+96  /**<  超出预定长度检测值*/

#define UDSCAP_SEPARATEPAGE                  CAP_CUSTOMBASE+97  /**<  分隔页*/

#define UDSCAP_CODEIDENTITY                  CAP_CUSTOMBASE+98  /**<  一二维码识别*/
#define UDSCAP_CODESTANDARD                  CAP_CUSTOMBASE+99  /**<  条码格式*/

#define UDSCAP_PREFEED                       CAP_CUSTOMBASE+100  /**<  预先进纸*/
#define UDSCAP_PREFEEDVALUE                  CAP_CUSTOMBASE+101  /**<  预先进纸值*/

#define UDSCAP_WAITTIME                      CAP_CUSTOMBASE+102 /**<  连续扫描等待放纸时间*/
//end 2.0版本

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
#define TWSN_DISABLE              FALSE
#define TWSN_AUTO                 TRUE 

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

/* UDSCAP_COLORFLIP values (CF_ means COLORFLIP)色彩翻转。*/
#define TWCF_DISABLE              FALSE
#define TWCF_AUTO                 TRUE

/* UDSCAP_CACHEMODE values (CM_ means CACHEMODE) 缓存模式对应取值 */
#define TWCM_NONE                  0 //自动
#define TWCM_PAPERNUM              1 //纸张数量
#define TWCM_MEMORYSIZE            2 //缓存大小

/* UDSCAP_POWERSAVING values (SP_ means SAVEPOWER)节电模式。*/
#define TWSP_DISABLE              FALSE
#define TWSP_AUTO                 TRUE

/* UDSCAP_POWEROFF values (OT_ means OFFTIME)关机时间。*/
#define TWOT_DISABLE              FALSE
#define TWOT_AUTO                 TRUE

/* UDSCAP_TURNVIDEO values (TV_ means TURNVIDEO)扫描仪无纸时转高拍仪。*/
#define TWTV_DISABLE              FALSE
#define TWTV_AUTO                 TRUE

/* UDSCAP_CUTMETHOD values (CT_ means UDSCAP_CUTMETHOD)图像裁切方式*/
#define TWCT_NONE                 0  //不裁切
#define TWCT_AUTO                 1  //自动裁切与纠偏
#define TWCT_SPECIFY              2  //指定长度、宽度

/* UDSCAP_FILTERCOLOR values (FL_ means UDSCAP_FILTERCOLOR)滤除颜色*/
#define TWFL_NONE                 0  //不滤除
#define TWFL_RED                  1  //红
#define TWFL_GREEN                2  //绿
#define TWFL_BLUE                 3  //蓝
#define TWFL_AUTO                 4  //自动

/* UDSCAP_FILTERMODE values (FM_ means UDSCAP_FILTERMODE)滤除模式*/
#define TWFM_AUTO                 0  //自动
#define TWFM_DEFINED              1  //自定义

/* UDSCAP_COMPRESSQUALITY values (CQ_ means COMPRESSQUALITY)压缩质量*/
#define TWCQ_BEST                 0  //最佳
#define TWCQ_FINE                 1  //良好
#define TWCQ_JUST                 2  //一般
#define TWCQ_DEFINED              3  //自定义

/* UDSCAP_EDGE_COLOR values (EC_ means EDGE_COLOR )扩充颜色*/
#define TWEC_WHITE                 0  //白色
#define TWEC_BLACK                 1  //黑色
#define TWEC_DEFINED               2  //自定义

/* UDSCAP_NOCOLOR values (NC_ means NOCOLOR )非彩色时扫描*/
#define TWNC_GRAY                  0  //灰阶
#define TWNC_BLACK                 1  //黑白

/* UDSCAP_COLORCORRECT values (CC_ means UDSCAP_COLORCORRECT )色彩校正*/
#define TWCC_AUTO                  0  //自动
#define TWCC_IMG                   1  //图片
#define TWCC_WORD                  2  //文字
#define TWCC_IMGTEXT               3  //图文模式

//新界面新增
/* UDSCAP_ROTATEMODE values (RM_ means UDSCAP_ROTATEMODE )图像旋转--模式*/
#define TWRM_FAST                  0  //快速
#define TWRM_ALL                   1  //全文
#define TWRM_SLOW                  2  //复杂

/* UDSCAP_BACKIMGROTATE values (BI_ means UDSCAP_BACKIMGROTATE ) 图像旋转--背面图像旋转*/
#define TWBI_SAME                  0 //相同朝向
#define TWBI_DIFF                  1 //相反朝向

/* UDSCAP_JOINIMAGE values (JI_ means UDSCAP_JOINIMAGE )图像拼接*/
#define TWJI_LEFTRIGHT               0  //左右拼接
#define TWJI_UPDOWN                  1  //上下拼接
#define TWJI_UPDOWNBACKTURN          2  //上下拼接（背面倒转）

/* UDSCAP_BACKPROCESS values (BS_ means UDSCAP_BACKPROCESS )背景处理*/
#define TWBS_NONE                    0  //无
#define TWBS_SMOOTH                  1  //平滑
#define TWBS_FILTER                  2  //滤除

/* UDSCAP_BACKPROMODE values (BM_ means UDSCAP_BACKPROMODE )背景处理模式*/
#define TWBM_AUTO                    0  //自动
#define TWBI_SCREEN                  1  //网纹

/* UDSCAP_BACKPROFILLCOLOR values (BF_ means UDSCAP_BACKPROFILLCOLOR )背景处理填补颜色*/
#define TWBF_AUTO                    0  //自动
#define TWBF_WHITE                   1  //白色 

/*ICAP_ROTATION values (BF_ means UDSCAP_BACKPROFILLCOLOR )图像旋转添加值*/
#define TWOR_DEFINED                 7  //自定义

/*UDSCAP_EDGE_ORIENTATION values (EO_ means EDGE_ORIENTATION )边缘扩充方向*/
#define TWEO_IN                       0  //向内
#define TWEO_OUT                      1  //向外

/*UDSCAP_EDGE_CORNERCOLOR values (EN_ means EDGE_CORNERCOLOR )边缘扩充--未扫描部分边角填充颜色*/
#define TWEN_WHITE                     0  //白色
#define TWEN_BLACK                     1  //黑色

/*UDSCAP_MULTIFEEDDETECT_VALUE values (MV_ means MULTIFEEDDETECT_VALUE )重张选择值*/
#define TWMV_PAUSE                     0  //暂停
#define TWMV_REMIND                    1  //继续扫描，扫描完后提示
#define TWMV_STOP                      2  //继续扫描，扫描后停止

/*UDSCAP_CODESTANDARD values (CS_ means CODESTANDARD )重张选择值*/
#define TWCS_1D                        0  //1D
#define TWCS_2D                        1  //2D
          

          
          
               
        

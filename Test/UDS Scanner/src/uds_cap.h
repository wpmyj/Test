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
#define UDSCAP_SENSITIVETHRESHOLD_COLORRETENT       CAP_CUSTOMBASE+6  /**< ��ɫ���� */

#define UDSCAP_PUNCHHOLEREMOVEL         CAP_CUSTOMBASE+7  /**< ȥ������ */
#define UDSCAP_SHARPEN                  CAP_CUSTOMBASE+8  /**< ͼ���� */

#define UDSCAP_MIRROR                   CAP_CUSTOMBASE+9  /**< ͼ������ */
#define UDSCAP_REMOVEBACKGROUND         CAP_CUSTOMBASE+10 /**< ȥ������ */
#define UDSCAP_DESCREEN                 CAP_CUSTOMBASE+11 /**< ȥ���� */
#define UDSCAP_DENOISE                  CAP_CUSTOMBASE+12 /**< ȥ���� */
#define UDSCAP_AUTOCROP                 CAP_CUSTOMBASE+13 /**< �Զ�����У�� */

#define UDSCAP_MULTIFEEDDETECT          CAP_CUSTOMBASE+14 /**< ���ż�� */
#define UDSCAP_MULTIFEEDDETECT_VALUE    CAP_CUSTOMBASE+15 /**< ���ż�����ֵ */

#define UDSCAP_MULTISTREAM              CAP_CUSTOMBASE+16 /**< ������� */

#define UDSCAP_REMOVEBLANK              CAP_CUSTOMBASE+17 /**< ȥ���հ�ҳ ������¼CheckBox��״̬��TWAIN�Դ���������¼������ֵ */
#define UDSCAP_MULTISTREAM_VALUE        CAP_CUSTOMBASE+18 /**< �������ѡ��ֵ */

//#define UDSCAP_SPLITIMAGE_VH            CAP_CUSTOMBASE+18 /**< ͼ��ָ��Զ����Ǵ�ֱ��ˮƽ*/

#define UDSCAP_EDGE_UP                  CAP_CUSTOMBASE+19   /**< ��Ե������������ */
#define UDSCAP_EDGE_DOWN                CAP_CUSTOMBASE+20
#define UDSCAP_EDGE_LEFT                CAP_CUSTOMBASE+21
#define UDSCAP_EDGE_RIGHT               CAP_CUSTOMBASE+22

#define UDSCAP_XPOS                     CAP_CUSTOMBASE+23  /**< Xƫ���� */
#define UDSCAP_YPOS                     CAP_CUSTOMBASE+24

#define UDSCAP_COMPRESSVALUE            CAP_CUSTOMBASE+25  /**< ѹ���� */
#define UDSCAP_COLORFLIP                CAP_CUSTOMBASE+26  /**< ɫ�ʷ�ת */

#define UDSCAP_POWERSAVING               CAP_CUSTOMBASE+27  /**< �ڵ�ģʽ */
#define UDSCAP_POWERSAVING_TIME          CAP_CUSTOMBASE+28  /**< �ڵ�ģʽֵ��ʱ�� */

#define UDSCAP_POWEROFF                  CAP_CUSTOMBASE+29  /**< �ػ�ʱ�� */
#define UDSCAP_POWEROFF_TIME             CAP_CUSTOMBASE+30  /**< �ػ�ʱ��ֵ��ʱ�� */

#define UDSCAP_TURNVIDEO                CAP_CUSTOMBASE+31  /**< ɨ������ֽʱת������ */

#define UDSCAP_CUTMETHOD                CAP_CUSTOMBASE+32  /**< ͼ����з�ʽ */

#define UDSCAP_NOISENUM                 CAP_CUSTOMBASE+33  /**< ȥ������-������Ŀ */
#define UDSCAP_NOISERANGE               CAP_CUSTOMBASE+34  /**< ȥ������-������Χ */

#define UDSCAP_FILTERCOLOR              CAP_CUSTOMBASE+35  /**< �˳���ɫ */
#define UDSCAP_FILTERMODE               CAP_CUSTOMBASE+36  /**< �˳�ģʽ */
#define UDSCAP_FILTERLEVEL              CAP_CUSTOMBASE+37  /**< �˳��̶� */

//�������� Brightness
#define UDSCAP_BRIGHTNESSFC                 CAP_CUSTOMBASE+38
#define UDSCAP_BRIGHTNESSFG                 CAP_CUSTOMBASE+39
#define UDSCAP_BRIGHTNESSFB                 CAP_CUSTOMBASE+40
#define UDSCAP_BRIGHTNESSBC                 CAP_CUSTOMBASE+41
#define UDSCAP_BRIGHTNESSBG                 CAP_CUSTOMBASE+42
#define UDSCAP_BRIGHTNESSBB                 CAP_CUSTOMBASE+43

//�����ֱ��� Resolution
#define UDSCAP_RESOLUTIONFC                 CAP_CUSTOMBASE+44
#define UDSCAP_RESOLUTIONFG                 CAP_CUSTOMBASE+45
#define UDSCAP_RESOLUTIONFB                 CAP_CUSTOMBASE+46
#define UDSCAP_RESOLUTIONBC                 CAP_CUSTOMBASE+47
#define UDSCAP_RESOLUTIONBG                 CAP_CUSTOMBASE+48
#define UDSCAP_RESOLUTIONBB                 CAP_CUSTOMBASE+49

//�����Աȶ� Contrast
#define UDSCAP_CONTRASTFC                   CAP_CUSTOMBASE+50
#define UDSCAP_CONTRASTFG                   CAP_CUSTOMBASE+51
#define UDSCAP_CONTRASTBC                   CAP_CUSTOMBASE+52
#define UDSCAP_CONTRASTBG                   CAP_CUSTOMBASE+53

//����ѹ��  ICAP_COMPRESSION
#define UDSCAP_COMPRESSIONFC                 CAP_CUSTOMBASE+54
#define UDSCAP_COMPRESSIONFG                 CAP_CUSTOMBASE+55
#define UDSCAP_COMPRESSIONFB                 CAP_CUSTOMBASE+56
#define UDSCAP_COMPRESSIONBC                 CAP_CUSTOMBASE+57
#define UDSCAP_COMPRESSIONBG                 CAP_CUSTOMBASE+58
#define UDSCAP_COMPRESSIONBB                 CAP_CUSTOMBASE+59

//����ѹ���ȵ�ֵ  ICAP_COMPRESSIONVALUE
#define UDSCAP_COMPRESSIONVALUEFC            CAP_CUSTOMBASE+60
#define UDSCAP_COMPRESSIONVALUEFG            CAP_CUSTOMBASE+61
#define UDSCAP_COMPRESSIONVALUEFB            CAP_CUSTOMBASE+62
#define UDSCAP_COMPRESSIONVALUEBC            CAP_CUSTOMBASE+63
#define UDSCAP_COMPRESSIONVALUEBG            CAP_CUSTOMBASE+64
#define UDSCAP_COMPRESSIONVALUEBB            CAP_CUSTOMBASE+65

//������ֵ��(���ڰ�)
#define UDSCAP_BINARIZATIONFB                CAP_CUSTOMBASE+66
#define UDSCAP_BINARIZATIONBB                CAP_CUSTOMBASE+67
//��ֵ
#define UDSCAP_THRESHOLDFB                   CAP_CUSTOMBASE+68
#define UDSCAP_THRESHOLDBB                   CAP_CUSTOMBASE+69
//ȥ���ߵ�
#define UDSCAP_REMOVESPOTSFB                 CAP_CUSTOMBASE+70
#define UDSCAP_REMOVESPOTSBB                 CAP_CUSTOMBASE+71

#define UDSCAP_EDGE_COLOR                    CAP_CUSTOMBASE+72  /**< ��Ե������ɫ */

#define UDSCAP_NOCOLOR                       CAP_CUSTOMBASE+73  /**< �ǲ�ɫʱɨ�� */
#define UDSCAP_COLORTHRESHOLD                CAP_CUSTOMBASE+74  /**< ɫ����ֵ */

#define UDSCAP_COMPRESSQUALITY               CAP_CUSTOMBASE+75  /**< ѹ������ */                   

//����ѹ������  UDSCAP_COMPRESSQUALITY
#define UDSCAP_COMPRESSQUALITYFC             CAP_CUSTOMBASE+76
#define UDSCAP_COMPRESSQUALITYFG             CAP_CUSTOMBASE+77
#define UDSCAP_COMPRESSQUALITYFB             CAP_CUSTOMBASE+78
#define UDSCAP_COMPRESSQUALITYBC             CAP_CUSTOMBASE+79
#define UDSCAP_COMPRESSQUALITYBG             CAP_CUSTOMBASE+80
#define UDSCAP_COMPRESSQUALITYBB             CAP_CUSTOMBASE+81 

//�½������Ӳ���
//��ͼ�����֣�
#define UDSCAP_ROTATEMODE                    CAP_CUSTOMBASE+82 /**< ͼ����ת--ģʽ */
#define UDSCAP_BACKIMGROTATE                 CAP_CUSTOMBASE+83 /**< ͼ����ת--����ͼ����ת */

#define UDSCAP_JOINIMAGE                     CAP_CUSTOMBASE+84 /**< ͼ��ƴ�� */

#define UDSCAP_BACKPROCESS                   CAP_CUSTOMBASE+85 /**< �������� */
#define UDSCAP_BACKPROSTRENTH                CAP_CUSTOMBASE+86 /**< ��������ǿ�� */
#define UDSCAP_BACKPROMODE                   CAP_CUSTOMBASE+87 /**< ��������ģʽ */
#define UDSCAP_BACKPROFILLCOLOR              CAP_CUSTOMBASE+88 /**< �����������ɫ */

#define UDSCAP_COLORCORRECT                  CAP_CUSTOMBASE+89  /**< ɫ��У�� */ 
#define UDSCAP_COLORCORRECTFC                CAP_CUSTOMBASE+90  /**<  ����ɫ���Ż�*/
#define UDSCAP_COLORCORRECTBC                CAP_CUSTOMBASE+91  /**<  ����ɫ���Ż�*/

#define UDSCAP_NATIVESAVE                    CAP_CUSTOMBASE+92  /**<  ���ر���*/

#define UDSCAP_EDGE_ORIENTATION              CAP_CUSTOMBASE+93 /**<  ��Ե���䷽��*/
#define UDSCAP_EDGE_CORNERCOLOR              CAP_CUSTOMBASE+94 /**<  ��Ե����--δɨ�貿�ֱ߽������ɫ*/

#define UDSCAP_OVERLENGTH                    CAP_CUSTOMBASE+95  /**<  ����Ԥ�����ȼ��*/
#define UDSCAP_OVERLENGTHVALUE               CAP_CUSTOMBASE+96  /**<  ����Ԥ�����ȼ��ֵ*/

#define UDSCAP_SEPARATEPAGE                  CAP_CUSTOMBASE+97  /**<  �ָ�ҳ*/

#define UDSCAP_CODEIDENTITY                  CAP_CUSTOMBASE+98  /**<  һ��ά��ʶ��*/
#define UDSCAP_CODESTANDARD                  CAP_CUSTOMBASE+99  /**<  �����ʽ*/

#define UDSCAP_PREFEED                       CAP_CUSTOMBASE+100  /**<  Ԥ�Ƚ�ֽ*/
#define UDSCAP_PREFEEDVALUE                  CAP_CUSTOMBASE+101  /**<  Ԥ�Ƚ�ֵֽ*/

#define UDSCAP_WAITTIME                      CAP_CUSTOMBASE+102 /**<  ����ɨ��ȴ���ֽʱ��*/
//end 2.0�汾

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
#define TWSN_DISABLE              FALSE
#define TWSN_AUTO                 TRUE 

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

/* UDSCAP_COLORFLIP values (CF_ means COLORFLIP)ɫ�ʷ�ת��*/
#define TWCF_DISABLE              FALSE
#define TWCF_AUTO                 TRUE

/* UDSCAP_CACHEMODE values (CM_ means CACHEMODE) ����ģʽ��Ӧȡֵ */
#define TWCM_NONE                  0 //�Զ�
#define TWCM_PAPERNUM              1 //ֽ������
#define TWCM_MEMORYSIZE            2 //�����С

/* UDSCAP_POWERSAVING values (SP_ means SAVEPOWER)�ڵ�ģʽ��*/
#define TWSP_DISABLE              FALSE
#define TWSP_AUTO                 TRUE

/* UDSCAP_POWEROFF values (OT_ means OFFTIME)�ػ�ʱ�䡣*/
#define TWOT_DISABLE              FALSE
#define TWOT_AUTO                 TRUE

/* UDSCAP_TURNVIDEO values (TV_ means TURNVIDEO)ɨ������ֽʱת�����ǡ�*/
#define TWTV_DISABLE              FALSE
#define TWTV_AUTO                 TRUE

/* UDSCAP_CUTMETHOD values (CT_ means UDSCAP_CUTMETHOD)ͼ����з�ʽ*/
#define TWCT_NONE                 0  //������
#define TWCT_AUTO                 1  //�Զ��������ƫ
#define TWCT_SPECIFY              2  //ָ�����ȡ����

/* UDSCAP_FILTERCOLOR values (FL_ means UDSCAP_FILTERCOLOR)�˳���ɫ*/
#define TWFL_NONE                 0  //���˳�
#define TWFL_RED                  1  //��
#define TWFL_GREEN                2  //��
#define TWFL_BLUE                 3  //��
#define TWFL_AUTO                 4  //�Զ�

/* UDSCAP_FILTERMODE values (FM_ means UDSCAP_FILTERMODE)�˳�ģʽ*/
#define TWFM_AUTO                 0  //�Զ�
#define TWFM_DEFINED              1  //�Զ���

/* UDSCAP_COMPRESSQUALITY values (CQ_ means COMPRESSQUALITY)ѹ������*/
#define TWCQ_BEST                 0  //���
#define TWCQ_FINE                 1  //����
#define TWCQ_JUST                 2  //һ��
#define TWCQ_DEFINED              3  //�Զ���

/* UDSCAP_EDGE_COLOR values (EC_ means EDGE_COLOR )������ɫ*/
#define TWEC_WHITE                 0  //��ɫ
#define TWEC_BLACK                 1  //��ɫ
#define TWEC_DEFINED               2  //�Զ���

/* UDSCAP_NOCOLOR values (NC_ means NOCOLOR )�ǲ�ɫʱɨ��*/
#define TWNC_GRAY                  0  //�ҽ�
#define TWNC_BLACK                 1  //�ڰ�

/* UDSCAP_COLORCORRECT values (CC_ means UDSCAP_COLORCORRECT )ɫ��У��*/
#define TWCC_AUTO                  0  //�Զ�
#define TWCC_IMG                   1  //ͼƬ
#define TWCC_WORD                  2  //����
#define TWCC_IMGTEXT               3  //ͼ��ģʽ

//�½�������
/* UDSCAP_ROTATEMODE values (RM_ means UDSCAP_ROTATEMODE )ͼ����ת--ģʽ*/
#define TWRM_FAST                  0  //����
#define TWRM_ALL                   1  //ȫ��
#define TWRM_SLOW                  2  //����

/* UDSCAP_BACKIMGROTATE values (BI_ means UDSCAP_BACKIMGROTATE ) ͼ����ת--����ͼ����ת*/
#define TWBI_SAME                  0 //��ͬ����
#define TWBI_DIFF                  1 //�෴����

/* UDSCAP_JOINIMAGE values (JI_ means UDSCAP_JOINIMAGE )ͼ��ƴ��*/
#define TWJI_LEFTRIGHT               0  //����ƴ��
#define TWJI_UPDOWN                  1  //����ƴ��
#define TWJI_UPDOWNBACKTURN          2  //����ƴ�ӣ����浹ת��

/* UDSCAP_BACKPROCESS values (BS_ means UDSCAP_BACKPROCESS )��������*/
#define TWBS_NONE                    0  //��
#define TWBS_SMOOTH                  1  //ƽ��
#define TWBS_FILTER                  2  //�˳�

/* UDSCAP_BACKPROMODE values (BM_ means UDSCAP_BACKPROMODE )��������ģʽ*/
#define TWBM_AUTO                    0  //�Զ�
#define TWBI_SCREEN                  1  //����

/* UDSCAP_BACKPROFILLCOLOR values (BF_ means UDSCAP_BACKPROFILLCOLOR )�����������ɫ*/
#define TWBF_AUTO                    0  //�Զ�
#define TWBF_WHITE                   1  //��ɫ 

/*ICAP_ROTATION values (BF_ means UDSCAP_BACKPROFILLCOLOR )ͼ����ת���ֵ*/
#define TWOR_DEFINED                 7  //�Զ���

/*UDSCAP_EDGE_ORIENTATION values (EO_ means EDGE_ORIENTATION )��Ե���䷽��*/
#define TWEO_IN                       0  //����
#define TWEO_OUT                      1  //����

/*UDSCAP_EDGE_CORNERCOLOR values (EN_ means EDGE_CORNERCOLOR )��Ե����--δɨ�貿�ֱ߽������ɫ*/
#define TWEN_WHITE                     0  //��ɫ
#define TWEN_BLACK                     1  //��ɫ

/*UDSCAP_MULTIFEEDDETECT_VALUE values (MV_ means MULTIFEEDDETECT_VALUE )����ѡ��ֵ*/
#define TWMV_PAUSE                     0  //��ͣ
#define TWMV_REMIND                    1  //����ɨ�裬ɨ�������ʾ
#define TWMV_STOP                      2  //����ɨ�裬ɨ���ֹͣ

/*UDSCAP_CODESTANDARD values (CS_ means CODESTANDARD )����ѡ��ֵ*/
#define TWCS_1D                        0  //1D
#define TWCS_2D                        1  //2D
          

          
          
               
        

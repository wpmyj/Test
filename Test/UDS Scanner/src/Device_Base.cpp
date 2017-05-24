#include "Device_Base.h"


CDevice_Base::CDevice_Base():
	m_bReadOnly(false),
	m_byteMultiValue(0x00)
{
	//2*2矩阵
	m_fpArray2[0][0] = 0.0 ;
	m_fpArray2[0][1] = 2.0 ;
	m_fpArray2[0][2] = 3.0 ;
	m_fpArray2[0][3] = 1.0 ;
	//4*4矩阵
	m_fpArray4[0][0] = 0.0 ;
	m_fpArray4[0][1] = 8.0 ;
	m_fpArray4[0][2] = 2.0 ;
	m_fpArray4[0][3] = 10.0 ;
	m_fpArray4[1][0] = 12.0 ;
	m_fpArray4[1][1] = 4.0 ;
	m_fpArray4[1][2] = 14.0 ;
	m_fpArray4[1][3] = 6.0 ;
	m_fpArray4[2][0] = 3.0 ;
	m_fpArray4[2][1] = 11.0 ;
	m_fpArray4[2][2] = 1.0 ;
	m_fpArray4[2][3] = 9.0 ;
	m_fpArray4[3][0] = 15.0 ;
	m_fpArray4[3][1] = 7.0 ;
	m_fpArray4[3][2] = 13.0 ;
	m_fpArray4[3][3] = 5.0 ;
	//8*8
	m_fpArray8[0][0] = 0.0f;  						
	m_fpArray8[0][1] = 32.0f;
	m_fpArray8[0][2] = 8.0f;
	m_fpArray8[0][3] = 40.0f;
	m_fpArray8[0][4] = 2.0f;
	m_fpArray8[0][5] = 34.0f;
	m_fpArray8[0][6] = 10.0f;
	m_fpArray8[0][7] = 42.0f;
	m_fpArray8[1][0] = 48.0f;
	m_fpArray8[1][1] = 16.0f;
	m_fpArray8[1][2] = 56.0f;
	m_fpArray8[1][3] = 24.0f;
	m_fpArray8[1][4] = 50.0f;
	m_fpArray8[1][5] = 18.0f;
	m_fpArray8[1][6] = 58.0f;
	m_fpArray8[1][7] = 26.0f;
	m_fpArray8[2][0] = 12.0f;
	m_fpArray8[2][1] = 44.0f;
	m_fpArray8[2][2] = 4.0f;
	m_fpArray8[2][3] = 36.0f;
	m_fpArray8[2][4] = 14.0f;
	m_fpArray8[2][5] = 46.0f;
	m_fpArray8[2][6] = 6.0f;
	m_fpArray8[2][7] = 38.0f;
	m_fpArray8[3][0] = 60.0f;
	m_fpArray8[3][1] = 28.0f;
	m_fpArray8[3][2] = 52.0f;
	m_fpArray8[3][3] = 20.0f;
	m_fpArray8[3][4] = 62.0f;
	m_fpArray8[3][5] = 30.0f;
	m_fpArray8[3][6] = 54.0f;
	m_fpArray8[3][7] = 22.0f;  
	m_fpArray8[4][0] = 3.0f;
	m_fpArray8[4][1] = 35.0f;
	m_fpArray8[4][2] = 11.0f;
	m_fpArray8[4][3] = 43.0f;
	m_fpArray8[4][4] = 1.0f;
	m_fpArray8[4][5] = 33.0f;
	m_fpArray8[4][6] = 9.0f;
	m_fpArray8[4][7] = 41.0f;
	m_fpArray8[5][0] = 51.0f;
	m_fpArray8[5][1] = 19.0f;
	m_fpArray8[5][2] = 59.0f;
	m_fpArray8[5][3] = 27.0f;
	m_fpArray8[5][4] = 49.0f;
	m_fpArray8[5][5] = 17.0f;
	m_fpArray8[5][6] = 57.0f;
	m_fpArray8[5][7] = 25.0f;
	m_fpArray8[6][0] = 15.0f;
	m_fpArray8[6][1] = 47.0f;
	m_fpArray8[6][2] = 7.0f;
	m_fpArray8[6][3] = 39.0f;
	m_fpArray8[6][4] = 13.0f;
	m_fpArray8[6][5] = 45.0f;
	m_fpArray8[6][6] = 5.0f;
	m_fpArray8[6][7] = 37.0f;
	m_fpArray8[7][0] = 63.0f;
	m_fpArray8[7][1] = 31.0f;
	m_fpArray8[7][2] = 55.0f;
	m_fpArray8[7][3] = 23.0f;
	m_fpArray8[7][4] = 61.0f;
	m_fpArray8[7][5] = 29.0f;
	m_fpArray8[7][6] = 53.0f;
	m_fpArray8[7][7] = 21.0f;
}


CDevice_Base::~CDevice_Base()
{

}

void CDevice_Base::setSetting(CDevice_Base settings)
{
	m_nPaperSource				= settings.m_nPaperSource;
	m_bDuplex							= settings.m_bDuplex;
	m_nPixelType					= settings.m_nPixelType;
	m_nWidth							= settings.m_nWidth;
	m_nHeight							= settings.m_nHeight;
	m_fXResolution				= settings.m_fXResolution;
	m_fYResolution				= settings.m_fYResolution;
	m_fGamma							= settings.m_fGamma;
	m_fContrast						= settings.m_fContrast;
	m_fBrightness					= settings.m_fBrightness;
	m_fThreshold					= settings.m_fThreshold;
	m_bMultifeedDetection = settings.m_bMultifeedDetection;
	m_nMD_value           = settings.m_nMD_value;

	m_fRotation						= settings.m_fRotation; //zhu
	m_nOrientation				= settings.m_nOrientation; //zhu
	m_nStandardsizes      = settings.m_nStandardsizes;
	m_nUnits              = settings.m_nUnits;
	m_nBinarization				= settings.m_nBinarization;
	m_nSpiltImage					= settings.m_nSpiltImage;
	m_fSensitiveThreshold_removespots = settings.m_fSensitiveThreshold_removespots;
	m_fSensitiveThreshold_colorretent = settings.m_fSensitiveThreshold_colorretent;
	m_bMultiStream			  = settings.m_bMultiStream;

	m_fRemoveBlank				= settings.m_fRemoveBlank;
	m_bRemoveBlank				= settings.m_bRemoveBlank;
	m_bRemovePunch				= settings.m_bRemovePunch;
	m_bSharpen						= settings.m_bSharpen;
	m_bMirror							= settings.m_bMirror;
	m_bRemoveBack					= settings.m_bRemoveBack;
	m_bDescreen						= settings.m_bDescreen;
	m_bDenoise						= settings.m_bDenoise;
	m_bAutoCrop						= settings.m_bAutoCrop;
	m_nMaxDocCount        = settings.m_nMaxDocCount;
	m_byteMultiValue      = settings.m_byteMultiValue;

	m_fEdgeUp             = settings.m_fEdgeUp;
	m_fEdgeDown           = settings.m_fEdgeDown;
	m_fEdgeLeft           = settings.m_fEdgeLeft;
	m_fEdgeRight          = settings.m_fEdgeRight;
	m_nEdgeColor          = settings.m_nEdgeColor;
	m_nEdgeOrientation    = settings.m_nEdgeOrientation;
	m_nEdgeCornercolor    = settings.m_nEdgeCornercolor;

	m_fXPos               = settings.m_fXPos;
	m_fYPos               = settings.m_fYPos;

	m_nCompress           = settings.m_nCompress;
	m_fCompressValue      = settings.m_fCompressValue;
	m_nCompressQuality    = settings.m_nCompressQuality;

	m_bColorFlip          = settings.m_bColorFlip;

	m_nCacheMode          = settings.m_nCacheMode;
	m_fCMAuto             = settings.m_fCMAuto;
	m_fCMPaperNum         = settings.m_fCMPaperNum;
	m_fCMMemorySize       = settings.m_fCMMemorySize;

	m_bSavePower          = settings.m_bSavePower;
	m_fSavePowerValue     = settings.m_fSavePowerValue;
	m_bOffTime            = settings.m_bOffTime;
	m_fOffTimeValue       = settings.m_fOffTimeValue;

	m_bTurnVideo          = settings.m_bTurnVideo;
	m_nCutMethod          = settings.m_nCutMethod;

	m_fNoiseNum           = settings.m_fNoiseNum;
	m_fNoiseRange         = settings.m_fNoiseRange;

	m_nFilterColor        = settings.m_nFilterColor;
	m_nFilterMode         = settings.m_nFilterMode;
	m_fFilterLevel        = settings.m_fFilterLevel;

	m_nNoColor            = settings.m_nNoColor;
	m_fColorThres         = settings.m_fColorThres;
	m_nColorCorrect       = settings.m_nColorCorrect;

	m_bOverLength          = settings.m_bOverLength;
	m_bSeperatePage        = settings.m_bSeperatePage;
	m_bPreFeed             = settings.m_bPreFeed;
	m_bCodeIdentity        = settings.m_bCodeIdentity;
	m_fOverLengthValue     = settings.m_fOverLengthValue;
	m_fPreFeedValue        = settings.m_fPreFeedValue;
	m_fWaitTime            = settings.m_fWaitTime;
	m_nCodeStandard        = settings.m_nCodeStandard;
	m_bNativeSave          = settings.m_bNativeSave;
	m_bEnmergency          = settings.m_bEnmergency;

	for(int i=0; i<8; i++)
	{
		m_fBright[i]  = settings.m_fBright[i];
		m_fResolu[i]  = settings.m_fResolu[i];
		m_nCompre[i]  = settings.m_nCompre[i];
		m_fCompVal[i] = settings.m_fCompVal[i];
		m_nCompQua[i] = settings.m_nCompQua[i];

		m_fContra[i] = settings.m_fContra[i];
		m_nBinari[i] = settings.m_nBinari[i];
		m_fThres[i]  = settings.m_fThres[i];
		m_fRemovespots[i] = settings.m_fRemovespots[i];
	}

	//for(int i=0; i<4; i++)
	{
		//m_fContra[i] = settings.m_fContra[i];
	}

//	for(int i=0; i<2; i++)
	{
		//m_nBinari[i] = settings.m_nBinari[i];
		//m_fThres[i]  = settings.m_fThres[i];
		//m_fRemovespots[i] = settings.m_fRemovespots[i];
	}

	//新界面新增
	m_nRotateMode             = settings.m_nRotateMode;
	m_nBackImgRotate          = settings.m_nBackImgRotate;
	m_nJoinImage              = settings.m_nJoinImage;
	m_nBackProcess            = settings.m_nBackProcess;
	m_nBackProMode            = settings.m_nBackProMode;
	m_nBackProFill            = settings.m_nBackProFill;
	m_fBackProStrenth         = settings.m_fBackProStrenth;
	m_bIndicators             = settings.m_bIndicators;
}

CDevice_Base* CDevice_Base::getSetting() const
{
	return (CDevice_Base*)(this);
}
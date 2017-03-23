#include "Device_Base.h"


CDevice_Base::CDevice_Base():
	m_bReadOnly(false),
	m_byteMultiValue(0x00)
{
        
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
	m_bMD_value           = settings.m_bMD_value;

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

	m_fXPos               = settings.m_fXPos;
	m_fYPos               = settings.m_fYPos;

	m_nCompress           = settings.m_nCompress;
	m_fCompressValue      = settings.m_fCompressValue;

	m_bColorFlip          = settings.m_bColorFlip;

	m_nCacheMode          = settings.m_nCacheMode;
	m_fCMAuto             = settings.m_fCMAuto;
	m_fCMPaperNum         = settings.m_fCMPaperNum;
	m_fCMMemorySize       = settings.m_fCMMemorySize;

	m_bSavePower          = settings.m_bSavePower;
	m_fSavePowerValue     = settings.m_fSavePowerValue;
	m_bOffTime            = settings.m_bOffTime;
	m_fOffTimeValue       = settings.m_fOffTimeValue;
}

CDevice_Base* CDevice_Base::getSetting() const
{
	//return (CDevice_Base)(*this);
	return (CDevice_Base*)(this);
}
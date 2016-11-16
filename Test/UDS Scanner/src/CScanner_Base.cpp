#include "CScanner_Base.h"


CScanner_Base::CScanner_Base():
m_bReadOnly(false)
{

}


CScanner_Base::~CScanner_Base()
{

}

void CScanner_Base::setSetting(CScanner_Base settings)
{
	m_nPaperSource  = settings.m_nPaperSource;
	m_bDuplex       = settings.m_bDuplex;
	m_nPixelType    = settings.m_nPixelType;
	m_nWidth        = settings.m_nWidth;
	m_nHeight       = settings.m_nHeight;
	m_fXResolution  = settings.m_fXResolution;
	m_fYResolution  = settings.m_fYResolution;
	m_fGamma        = settings.m_fGamma;
	m_fContrast     = settings.m_fContrast;
	m_fBrightness   = settings.m_fBrightness;
	m_fThreshold    = settings.m_fThreshold;

	m_nRotation     = settings.m_nRotation; //zhu
	m_nOrientation  = settings.m_nOrientation; //zhu
	m_nBinarization = settings.m_nBinarization;
	m_nSpiltImage   = settings.m_nSpiltImage;
	m_nRemoveBlank  = settings.m_nRemoveBlank;
	m_nRemovePunch  = settings.m_nRemovePunch;
	m_nSharpen      = settings.m_nSharpen;
	m_nMirror       = settings.m_nMirror;
	m_nRemoveBack   = settings.m_nRemoveBack;
	m_nDescreen     = settings.m_nDescreen;
	m_nDenoise      = settings.m_nDenoise;
	m_nAutoCrop     = settings.m_nAutoCrop;
}

CScanner_Base* CScanner_Base::getSetting() const
{
	//return (CScanner_Base)(*this);
	return (CScanner_Base*)(this);
}
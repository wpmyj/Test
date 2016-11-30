#include "Camera_DirectX.h"
#include "public.h"

extern HWND g_hwndDLG;

CCamera_DirectX::CCamera_DirectX(void) :
	m_nDocCount(0),
	m_nSourceWidth(0),
	m_nSourceHeight(0)
{
	// set default caps
	resetScanner();
}


CCamera_DirectX::~CCamera_DirectX(void)
{
	if (m_pImageBuffer)
	{
		delete [] m_pImageBuffer;
		m_pImageBuffer = NULL;
	}
	
}

bool CCamera_DirectX::resetScanner()
{
	// Unlock the scanner 
	Unlock();

	
	//Base����
	m_nPixelType          = TWPT_RGB; //ͼ������-��ɫ zhu
	m_fXResolution        = 200.0;
	m_fYResolution        = 200.0; //�ֱ���-200.0
	m_bDuplex             = false; //����/˫��-����
	m_fContrast           = 0.0; //�Աȶ�-0.0
	m_fBrightness         = 0.0; //����-0.0
	m_fThreshold          = 128.0; //��ֵ-128.0 ������Ĭ��128.G6400Ĭ��230
	m_bMultifeedDetection = true; //���ż��-ѡ��

	//Advanced����
	m_nOrientation        = TWOR_ROT0; //zhu ֽ�ŷ���-����
	m_nStandardsizes      = TWSS_USLETTER; //zhu ��ӦICAP_SUPPORTEDSIZES��ֽ�Ŵ�С-TWSS_USLETTER
	m_nUnits              = TWUN_INCHES;  //zhu ��λ-Ӣ��

	m_nWidth              = 0;
	m_nHeight             = 0;

	m_nRotation           = 0; //��ת-����תzhu
	m_nSpiltImage         = TWSI_NONE; //zhu �ָ�-���ָ�
	m_fGamma              = 100.0; //zhu GammaУ��-Ĭ��Ϊ100
	m_bMirror             = TWMR_DISABLE; //����-��ѡ��

	m_nBinarization       = TWBZ_DYNATHRESHOLD; //zhu ��ֵ��-��̬��ֵ
	m_bMultiStream        = false; //�������-��ѡ��
	m_fSensitiveThreshold_removespots = 0.0; //ȥ���ߵ�-0.0
	m_fSensitiveThreshold_colorretent = 128.0; //��ɫ����-128.0

	//����ͼ����
	//Ĭ�ϲ�ѡ��
	m_fRemoveBlank        = TWRP_DISABLE; 
	m_bRemovePunch        = TWSP_DISABLE;
	m_bSharpen            = TWGM_DISABLE;
	m_bRemoveBack         = TWRB_DISABLE;
	m_bDescreen           = TWDS_DISABLE;
	m_bDenoise            = TWDN_DISABLE;
	m_bAutoCrop           = TWAC_DISABLE;

	return true;
}

bool CCamera_DirectX::isFeederLoaded()
{
	return true;
}

void CCamera_DirectX::GetImageData(BYTE *buffer, DWORD &dwReceived)
{
	memcpy(buffer, m_pImageBuffer, m_dwSize * sizeof(BYTE));
}

bool CCamera_DirectX::SetImageData(BYTE *buffer, DWORD dwSize)
{
	if (0 == dwSize)
	{
		::MessageBox(g_hwndDLG,TEXT("����Ϊ��!"),MB_CAPTION,MB_OK);
		return false;
	}
	m_pImageBuffer = new BYTE[dwSize];
	char buf[10];
  itoa(dwSize, buf, 10);
	//::MessageBox(g_hwndDLG, TEXT(buf),"dwsize",MB_OK);
	m_dwSize = dwSize;
	memcpy(m_pImageBuffer, buffer, dwSize * sizeof(BYTE));
	//::MessageBox(g_hwndDLG, TEXT(buf),"After memcpy()",MB_OK);
	return true;
}

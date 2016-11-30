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

	
	//Base界面
	m_nPixelType          = TWPT_RGB; //图形类型-彩色 zhu
	m_fXResolution        = 200.0;
	m_fYResolution        = 200.0; //分辨率-200.0
	m_bDuplex             = false; //单面/双面-单面
	m_fContrast           = 0.0; //对比度-0.0
	m_fBrightness         = 0.0; //亮度-0.0
	m_fThreshold          = 128.0; //阈值-128.0 ，虚拟默认128.G6400默认230
	m_bMultifeedDetection = true; //重张检测-选中

	//Advanced界面
	m_nOrientation        = TWOR_ROT0; //zhu 纸张方向-纵向
	m_nStandardsizes      = TWSS_USLETTER; //zhu 对应ICAP_SUPPORTEDSIZES，纸张大小-TWSS_USLETTER
	m_nUnits              = TWUN_INCHES;  //zhu 单位-英寸

	m_nWidth              = 0;
	m_nHeight             = 0;

	m_nRotation           = 0; //旋转-不旋转zhu
	m_nSpiltImage         = TWSI_NONE; //zhu 分割-不分割
	m_fGamma              = 100.0; //zhu Gamma校正-默认为100
	m_bMirror             = TWMR_DISABLE; //镜像-不选中

	m_nBinarization       = TWBZ_DYNATHRESHOLD; //zhu 二值化-动态阈值
	m_bMultiStream        = false; //多流输出-不选中
	m_fSensitiveThreshold_removespots = 0.0; //去除斑点-0.0
	m_fSensitiveThreshold_colorretent = 128.0; //底色保留-128.0

	//其他图像处理
	//默认不选中
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
		::MessageBox(g_hwndDLG,TEXT("数据为空!"),MB_CAPTION,MB_OK);
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

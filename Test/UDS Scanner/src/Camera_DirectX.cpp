#include "Camera_DirectX.h"
#include "public.h"
#include <time.h>
/**
* Environment vars to get the Xfer Count.  Create this enviroment Varable on your system to simulate the 
* number of pages sitting in the scanner waiting to be scanned.
*/
#define kGETENV_XFERCOUNT "CAP_XFERCOUNT"

extern HWND g_hwndDLG;
extern vector<CUST_IMAGEINFO> g_vecCust_ImageInfo;

CCamera_DirectX::CCamera_DirectX(void) :
	m_nDocCount(0),
	m_nSourceWidth(0),
	m_nSourceHeight(0),
	m_nImageNumber(0)
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
	
	if (false == m_mat_image.empty())
	{
		m_mat_image.release();
	}
}

bool CCamera_DirectX::resetScanner()
{
	// Unlock the scanner 
	Unlock();

		m_nDocCount           = m_nMaxDocCount = getDocumentCount();// Reloaded the scanner with paper
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

	m_fRotation           = 0.0; //��ת-����תzhu
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

	m_nImageNumber = 0;
	if (false == m_mat_image.empty())
	{
		m_mat_image.release();
	}

	return true;
}

bool CCamera_DirectX::isFeederLoaded()
{
	//::MessageBox(g_hwndDLG,TEXT("isFeederLoaded()"),MB_CAPTION,MB_OK);
	bool rtn = true;
	if(m_nDocCount<=0)
	{
		rtn = false;
		m_nDocCount = m_nMaxDocCount;// Reloaded the scanner with paper
	}
	return rtn;
}

void CCamera_DirectX::GetImageData(BYTE *buffer, DWORD &dwReceived)
{
	//memcpy(buffer, m_pImageBuffer, m_dwSize * sizeof(BYTE));
	//::MessageBox(g_hwndDLG,TEXT("CCamera_DirectX::GetImageData()"),MB_CAPTION,MB_OK);


	int size = m_mat_image.total() * m_mat_image.elemSize();
	std::memcpy(buffer, m_mat_image.data, size * sizeof(BYTE));
}

bool CCamera_DirectX::SetImageData(BYTE *buffer, DWORD dwSize)
{
	if (0 == dwSize)
	{
		::MessageBox(g_hwndDLG,TEXT("����Ϊ��!"),MB_CAPTION,MB_OK);
		return false;
	}
	m_pImageBuffer = new BYTE[dwSize];
	/*char buf[10];
	itoa(dwSize, buf, 10);
	::MessageBox(g_hwndDLG, TEXT(buf),"SetImageData::dwsize",MB_OK);*/
	m_dwSize = dwSize;
	memcpy(m_pImageBuffer, buffer, dwSize * sizeof(BYTE));
	//::MessageBox(g_hwndDLG, TEXT(buf),"After memcpy()",MB_OK);
	return true;
}

short CCamera_DirectX::getDocumentCount() const
{
	// Simulate the number of pages sitting in the scanner.
	int nCount = 1;

	// Read this value from the environment. This will allow the simulation
	// of a sheet feeder.      ����绷����ȡ��ֵ�������ģ����ֽ��
	// If the value is <= 0, then a random number of pages will be scanned, else
	// the exact number will be used.     ��� nCount <= 0���򷵻�һ�����ɨ��ҳ�������򷵻�һ����ȷ��ҳ��
	char szCount[10];
	memset(szCount, 0, sizeof(szCount));

	if( 0 != SGETENV(szCount, sizeof(szCount), kGETENV_XFERCOUNT) )   // ����绷����ȡ��ֵ����ģ����ֽ��
	{
		// something found, convert it to an int
		nCount = atoi(szCount);                    // �ַ���ת����������

		if(nCount <= 0)
		{
			srand(int(time(0)));
			nCount = rand();
			nCount = nCount%15;// upto 15 pages
		}
	}

	return nCount;
}

bool CCamera_DirectX::acquireImage()
{
	//static int nCount = 0;
	//string filename = g_vecFilePath[nCount];
	string filename = g_vecCust_ImageInfo[m_nImageNumber].imagePath;
	//nCount++;
	m_nImageNumber++;

	//::MessageBox(g_hwndDLG,TEXT(filename.c_str()),MB_CAPTION,MB_OK);
	m_mat_image = cv::imread(filename);
	if(true == m_mat_image.empty())
	{
		::MessageBox(g_hwndDLG,TEXT("ds: Failed - could not acquire image!"),MB_CAPTION,MB_OK);
		//cout << "ds: Failed - could not acquire image" << endl;
		return false ;
	}
	m_nDocCount--;

	// do whatever tranforms to the scanned image that was requested by the app
	// before the image is sent to the app.
	if(false == preScanPrep())
	{
		return false;
	}

	return true;
}

void CCamera_DirectX::setSetting(CScanner_Base settings)
{
	CScanner_Base::setSetting(settings);  // ���ø���ķ���
	m_nDocCount = m_nMaxDocCount;
}

bool CCamera_DirectX::preScanPrep()
{
	m_nWidth = g_vecCust_ImageInfo[m_nImageNumber].imageWidth;
	m_nHeight = g_vecCust_ImageInfo[m_nImageNumber].imageHeight;
	m_fXResolution = g_vecCust_ImageInfo[m_nImageNumber].XResolution;
	m_fXResolution = g_vecCust_ImageInfo[m_nImageNumber].YResolution;

	return true;
}

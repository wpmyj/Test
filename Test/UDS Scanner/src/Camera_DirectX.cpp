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

	m_fRotation           = 0.0; //旋转-不旋转zhu
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
		::MessageBox(g_hwndDLG,TEXT("数据为空!"),MB_CAPTION,MB_OK);
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
	// of a sheet feeder.      从外界环境读取此值，这可以模拟送纸器
	// If the value is <= 0, then a random number of pages will be scanned, else
	// the exact number will be used.     如果 nCount <= 0，则返回一个随机扫描页数，否则返回一个精确的页数
	char szCount[10];
	memset(szCount, 0, sizeof(szCount));

	if( 0 != SGETENV(szCount, sizeof(szCount), kGETENV_XFERCOUNT) )   // 从外界环境读取此值，以模拟送纸器
	{
		// something found, convert it to an int
		nCount = atoi(szCount);                    // 字符串转换成整型数

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
	CScanner_Base::setSetting(settings);  // 调用父类的方法
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

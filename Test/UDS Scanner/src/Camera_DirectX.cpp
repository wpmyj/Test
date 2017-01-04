#include "Camera_DirectX.h"
#include "public.h"
#include <time.h>
#include <vector>
/**
* Environment vars to get the Xfer Count.  Create this enviroment Varable on your system to simulate the 
* number of pages sitting in the scanner waiting to be scanned.
*/
#define kGETENV_XFERCOUNT "CAP_XFERCOUNT"

extern HWND g_hwndDLG;
//extern vector<CUST_IMAGEINFO> g_vecCust_ImageInfo;
extern 	std::vector<std::string> g_vector_imagepath;
//DWORD  g_dwImageSize;     // ȫ�ֱ��������ڱ���ͼƬ��С

CCamera_DirectX::CCamera_DirectX(void) :
	m_nDocCount(0),
	m_nSourceWidth(0),
	m_nSourceHeight(0),
	m_nImageNumber(0),
	m_nScanLine(0),
	m_nDestBytesPerRow(0),
	m_pCxImage(NULL)
{
	// set default ca
	resetScanner();
}


CCamera_DirectX::~CCamera_DirectX(void)
{
	//if (m_pImageBuffer)
	//{
	//	delete [] m_pImageBuffer;
	//	m_pImageBuffer = NULL;
	//}
	//
	//if (false == m_mat_image.empty())
	//{
	//	m_mat_image.release();
	//}


	if (!m_pCxImage)
	{
		::delete m_pCxImage;
		m_pCxImage = NULL;
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
	//if (false == m_mat_image.empty())
	//{
	//	m_mat_image.release();
	//}

	if (!m_pCxImage)
	{
		::delete m_pCxImage;
		m_pCxImage = NULL;
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


	//long size = m_mat_image.total() * m_mat_image.elemSize();
	//char buf[60];
	//itoa(size, buf, 10);
	//::MessageBox(g_hwndDLG, TEXT(buf),"GetImageData::size",MB_OK);
	//std::memcpy(buffer, m_mat_image.data, size * sizeof(BYTE));
	//::MessageBox(g_hwndDLG,TEXT("After memcpy!"),MB_CAPTION,MB_OK);


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
	//string filename = g_vecCust_ImageInfo[m_nImageNumber].imagePath;
	//
	//::MessageBox(g_hwndDLG,TEXT(filename.c_str()),MB_CAPTION,MB_OK);
	//m_mat_image = cv::imread(filename);
	//if(true == m_mat_image.empty())
	//{
	//	::MessageBox(g_hwndDLG,TEXT("ds: Failed - could not acquire image!"),MB_CAPTION,MB_OK);
	//	//cout << "ds: Failed - could not acquire image" << endl;
	//	return false ;
	//}

	if (!m_pCxImage)
	{
		delete m_pCxImage;
		m_pCxImage = NULL;
	}
	
	m_pCxImage = new CxImage();
	//if(false == m_pCxImage->CreateFromHANDLE(g_vector_imagepath[m_nImageNumber]))
	//{
	//	::MessageBox(g_hwndDLG,TEXT("ds: Failed - could not acquire image!"),MB_CAPTION,MB_OK);
	//	return false ;
	//}
	//::MessageBox(g_hwndDLG,TEXT(g_vector_imagepath[m_nImageNumber].c_str()),"acquire",MB_OK);
	if(false == m_pCxImage->Load(g_vector_imagepath[m_nImageNumber].c_str()))
	{
		::MessageBox(g_hwndDLG,TEXT("ds: Failed - could not acquire image!"),MB_CAPTION,MB_OK);
		return false ;
	}

	// do whatever tranforms to the scanned image that was requested by the app
	// before the image is sent to the app.
	if(false == preScanPrep())
	{
		return false;
	}

	m_nDocCount--;
	m_nImageNumber++;

	return true;
}

void CCamera_DirectX::setSetting(CScanner_Base settings)
{
	CScanner_Base::setSetting(settings);  // ���ø���ķ���
	m_nDocCount = m_nMaxDocCount;
}

bool CCamera_DirectX::preScanPrep()
{
	//m_nWidth = g_vecCust_ImageInfo[m_nImageNumber].imageWidth;
	//m_nHeight = g_vecCust_ImageInfo[m_nImageNumber].imageHeight;
	//m_fXResolution = g_vecCust_ImageInfo[m_nImageNumber].XResolution;
	//m_fXResolution = g_vecCust_ImageInfo[m_nImageNumber].YResolution;

	//DWORD size = m_mat_image.total() * m_mat_image.elemSize();
 // g_dwImageSize = size;
	//m_pCxImage->Rotate180();  // CxIamge����洢ͼƬ����

	m_pCxImage->SetXDPI((long)m_fXResolution);
	m_pCxImage->SetYDPI((long)m_fYResolution);

	m_nWidth  = m_nSourceWidth = m_pCxImage->GetWidth(); 
	m_nHeight = m_nSourceHeight = m_pCxImage->GetHeight() ;
	/*}*/
	
	switch(m_nPixelType)
	{
	case TWPT_BW:
		//m_pCxImage->Negative();
		m_nDestBytesPerRow = BYTES_PERLINE(m_nWidth, 1);
		break;

	case TWPT_GRAY:
		m_nDestBytesPerRow = BYTES_PERLINE(m_nWidth, 8);
		break;

	case TWPT_RGB:
		m_nDestBytesPerRow = BYTES_PERLINE(m_nWidth, 24);
		break;
	}


	//char buf[60];
	//itoa(m_pCxImage->GetSize(), buf, 10);
	//::MessageBox(g_hwndDLG, TEXT(buf),"preScanPrep::m_pCxImage->GetSize()",MB_OK);
	//itoa(m_nHeight, buf, 10);
	//::MessageBox(g_hwndDLG, TEXT(buf),"preScanPrep::m_nHeight",MB_OK);
	//itoa(m_fXResolution, buf, 10);
	//::MessageBox(g_hwndDLG, TEXT(buf),"preScanPrep::m_fXResolution",MB_OK);


	// setup some convenience vars because they are used during 
	// every strip request
	m_nScanLine       = 0;
	return true;
}

bool CCamera_DirectX::getScanStrip(BYTE *pTransferBuffer, DWORD dwRead, DWORD &dwReceived)
{
	//::MessageBox(g_hwndDLG,"getScanStrip","UDS",MB_OK);
	dwReceived = 0;

	if( NULL == pTransferBuffer ||    // Invalid paramiter
		dwRead < m_nDestBytesPerRow ) // Need enough memory to transfer at least an entire row
	{
		return false;
	}

	BYTE   *pBits     = NULL;
	WORD    nRow      = 0;
	WORD    nMaxRows  = (WORD)(dwRead / m_nDestBytesPerRow); //number of rows�� to be transfered during this call (function of buffer size and line size)

	if( m_nScanLine < MIN(m_nSourceHeight, m_nHeight) )
	{
		//fill the buffer line by line to take care of alignment differences
		for(nRow = 0; nRow<nMaxRows; nRow++)
		{
			//get the next scan line position and copy it
			//pBits = (BYTE*)FreeImage_GetScanLine(m_pDIB, m_nSourceHeight-m_nScanLine-1);
			pBits = m_pCxImage->GetBits(m_nSourceHeight-m_nScanLine-1);
			
			memcpy( pTransferBuffer, pBits, m_nDestBytesPerRow);

			// Check to see if the result image width is wider than what we have.
			// If it is wider fill it in with 0es
			/*if(m_nDestBytesPerRow > FreeImage_GetLine(m_pDIB))
			{
				memset( pTransferBuffer+FreeImage_GetLine(m_pDIB), 255, m_nDestBytesPerRow - FreeImage_GetLine(m_pDIB) );
			}*/

			//increment the destination by the aligned line size
			pTransferBuffer += m_nDestBytesPerRow;

			// increment the current scanline for next pass
			m_nScanLine++;

			//update the number of bytes written
			dwReceived += m_nDestBytesPerRow;

			// check for finished scan
			if( m_nScanLine >= m_nSourceHeight ||
				m_nScanLine >= m_nHeight )
			{
				//we are done early
				break;
			}
		}
	}

	// Check to see if the result image length is longer than we have.
	// If it is longer fill it in with 0es
	if(m_nHeight > m_nScanLine )
	{
		nMaxRows  = (WORD)((dwRead-dwReceived) / m_nDestBytesPerRow);
		memset( pTransferBuffer, 255, m_nDestBytesPerRow * nMaxRows );
		m_nScanLine += nMaxRows;
		dwReceived += m_nDestBytesPerRow * nMaxRows;
	}

	return true;
}


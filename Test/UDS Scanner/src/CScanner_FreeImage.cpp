/***************************************************************************
* Copyright ?2007 TWAIN Working Group:  
*   Adobe Systems Incorporated, AnyDoc Software Inc., Eastman Kodak Company, 
*   Fujitsu Computer Products of America, JFL Peripheral Solutions Inc., 
*   Ricoh Corporation, and Xerox Corporation.
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in the
*       documentation and/or other materials provided with the distribution.
*     * Neither the name of the TWAIN Working Group nor the
*       names of its contributors may be used to endorse or promote products
*       derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY TWAIN Working Group ``AS IS'' AND ANY
* EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL TWAIN Working Group BE LIABLE FOR ANY
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
***************************************************************************/

/**
* @file CScanner_FreeImage.cpp
* Defines a scanner.
* Create a virtual scanner.
* @author TWAIN Working Group
* @date April 2007
*/


#include "CScanner_FreeImage.h"

#include <iostream>
#include <time.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#ifdef TWH_CMP_MSC
#include <io.h>
#else //#ifdef TWH_CMP_MSC
#include <sys/io.h>
#endif //#ifdef TWH_CMP_MSC

#ifdef TWNDS_OS_LINUX
#define kTWAIN_DS_DIR "/usr/local/lib/twain/sample2"
#endif

#include "DSMInterface.h"
#include "public.h"

extern HWND g_hwndDLG;

//using namespace std;

/**
* Environment vars to get the Xfer Count.  Create this enviroment Varable on your system to simulate the 
* number of pages sitting in the scanner waiting to be scanned.
*/
#define kGETENV_XFERCOUNT "CAP_XFERCOUNT"

#ifdef TWH_CMP_MSC
extern HINSTANCE   g_hinstance;
#endif

//////////////////////////////////////////////////////////////////////////////
void FreeImageErrorHandler(FREE_IMAGE_FORMAT fif, const char *message)
{
  cout << "\n*** ";
  cout << FreeImage_GetFormatFromFIF(fif) << " Format" << endl;
  cout << message << " ***" << endl;
  return;
}


//////////////////////////////////////////////////////////////////////////////
CScanner_FreeImage::CScanner_FreeImage() :
  m_pDIB(0),
  m_nScanLine(0),
//  m_bReadOnly(false),
  m_nDestBytesPerRow(0),
  m_nDocCount(0),
  m_nSourceWidth(0),
  m_nSourceHeight(0)
{
  memset(m_szSourceImagePath, 0, PATH_MAX);

  char szTWAIN_DS_DIR[PATH_MAX];
	
#ifdef TWH_CMP_MSC
  GetModuleFileName(g_hinstance, szTWAIN_DS_DIR, PATH_MAX);
  // strip filename from path
  size_t x = strlen(szTWAIN_DS_DIR);
  while(x > 0)
  {
	  if(PATH_SEPERATOR == szTWAIN_DS_DIR[x-1])
	  {
		  szTWAIN_DS_DIR[x-1] = 0;
		  break;
	  }
	  --x;
  }
#else
  // In non-windows, kTWAIN_DS_DIR is set on the compiler command line
  strncpy(szTWAIN_DS_DIR, kTWAIN_DS_DIR, PATH_MAX);
#endif

  SSNPRINTF(m_szSourceImagePath, sizeof(m_szSourceImagePath), PATH_MAX, "%s%cTWAIN_logo.jpg", szTWAIN_DS_DIR, PATH_SEPERATOR);
	//char IniPath[PATH_MAX];
	//strncpy_s(m_szIniPath,szTWAIN_DS_DIR,PATH_MAX);
	//strncat_s(m_szIniPath,"\\SetUp.ini",PATH_MAX);
	//::MessageBox(g_hwndDLG,m_szSourceImagePath,"UDS",MB_OK);
	

	//char InitialImagePath[PATH_MAX];
	//strncpy_s(InitialImagePath,szTWAIN_DS_DIR,PATH_MAX);
	//strncat_s(InitialImagePath,"\\TWAIN_logo.jpg",PATH_MAX);
	//GetPrivateProfileString("ImagePath Setting","ImagePath",InitialImagePath,
	//	m_szSourceImagePath,MAX_PATH,m_szIniPath);
	
	//GetImagePathFormINI();
	
	//vector<string>::iterator it = m_vector_string_imagepath.begin();
	//for (it; it!=m_vector_string_imagepath.end(); it++)
	//{
	//	MessageBox(g_hwndDLG,(*it).c_str(),NULL,MB_OK);
	//}


  // set default caps
  resetScanner();

  FreeImage_Initialise();  // 初始化FreeImge
  FreeImage_SetOutputMessage(FreeImageErrorHandler);
}

//////////////////////////////////////////////////////////////////////////////
CScanner_FreeImage::~CScanner_FreeImage()
{
  if(0 != m_pDIB)
  {
    FreeImage_Unload(m_pDIB);  // 释放位图
  }

  FreeImage_DeInitialise();  // 释放FreeImage
}


//////////////////////////////////////////////////////////////////////////////
bool CScanner_FreeImage::resetScanner()
{
  bool bret = true;

  // Unlock the scanner 
  Unlock();

  m_nScanLine           = 0;
  m_nDestBytesPerRow    = 0;

  m_nDocCount           = m_nMaxDocCount = getDocumentCount();// Reloaded the scanner with paper

	//Base界面
  m_nPaperSource        = SFI_PAPERSOURCE_ADF;  //扫描模式-自动进纸器
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
	m_fRemoveBlank        = TWBP_DISABLE; 
	m_bRemovePunch        = TWRP_DISABLE;
	m_bSharpen            = TWSP_DISABLE;
	m_bRemoveBack         = TWRB_DISABLE;
	m_bDescreen           = TWDS_DISABLE;
	m_bDenoise            = TWDN_DISABLE;
	m_bAutoCrop           = TWAC_DISABLE;

  if(0 != m_pDIB)   //如果m_pDIB（保存着位图信息和像素数据的指针）不为 0
  {
    FreeImage_Unload(m_pDIB);
    m_pDIB = 0;
  }

  return bret;
}


//////////////////////////////////////////////////////////////////////////////
void CScanner_FreeImage::setSetting(CScanner_Base settings)
{
  CScanner_Base::setSetting(settings);  // 调用父类的方法
  m_nDocCount = m_nMaxDocCount;
}

//////////////////////////////////////////////////////////////////////////////
bool CScanner_FreeImage::acquireImage()
{
	//if (false == m_vector_string_imagepath.empty())  
	//{
	//	static vector<std::string.::iterator it = m_vector_string_imagepat..begin();

	//	if (it != m_vector_string.imagepath.end())
	//	{
	//		strcpy_s(m_szSourceImagePath,(*it).c_str());
	//		//::MessageBox(g_hwndDLG, (*it).c_str(),TEXT("图片路径"), MB_OK);
	//		it++;
	//	}  

	//}
		
	// first, avoid a memory leak by cleaning up past images
	if(0 != m_pDIB)
	{
		FreeImage_Unload(m_pDIB);
		m_pDIB = 0;
	}

	// get the image if it exists
	if(FALSE == FILE_EXISTS(m_szSourceImagePath))
	{
		::MessageBox(g_hwndDLG,TEXT("ds: Could not find required file: "),"UDS",MB_OK);
		//cerr << "ds: Could not find required file: " << m_szSourceImagePath << endl;
		return false;
	}

	FREE_IMAGE_FORMAT fif = FreeImage_GetFileType(m_szSourceImagePath);  // 由文件头分析文件类型
	m_pDIB = FreeImage_Load(fif, m_szSourceImagePath);  // 加载位图

	if(0 == m_pDIB)
	{
		::MessageBox(g_hwndDLG,TEXT("ds: Failed - could not acquire image!"),MB_CAPTION,MB_OK);
		//cout << "ds: Failed - could not acquire image" << endl;
		return false;
	}

	//Document scanned, remove it from simulated intray
	m_nDocCount--;

	// do whatever tranforms to the scanned image that was requested by the app
	// before the image is sent to the app.
	if(false == preScanPrep())
	{
		return false;
	}
	
	return true;
}

//////////////////////////////////////////////////////////////////////////////
bool CScanner_FreeImage::preScanPrep()
{
  // convert all scanned images to 24 bit as a starting point, if they are less
  // then 24 bit.  This is because the scaling functions only work on 8/24/32
  // bit images.
	// 每个像素占用位数 
  if(FreeImage_GetBPP(m_pDIB) < 24 )
  {
    FIBITMAP *pDib = FreeImage_ConvertTo24Bits(m_pDIB);
    FreeImage_Unload(m_pDIB);
    m_pDIB = pDib;
  }

  FIBITMAP *pDib = 0;

  if(m_nPixelType != TWPT_BW)
  {
    if(m_fBrightness!=0)
    {
      FreeImage_AdjustBrightness(m_pDIB, m_fBrightness/10);
    }
    if(m_fContrast!=0)
    {
      FreeImage_AdjustContrast(m_pDIB, m_fContrast/10);
    }

    //if(m_fGamma!=1.0)
		if(m_fGamma!=100.0) //zhu
    {
			FreeImage_AdjustGamma(m_pDIB, m_fGamma/100); //取值范围为10~255，此处需要除以100，缩小取值
    }
  }

	// 获取影像的宽高，都以像素为单位 
  m_nSourceWidth   = FreeImage_GetWidth(m_pDIB);
  m_nSourceHeight  = FreeImage_GetHeight(m_pDIB);
  WORD res = 0;

	// 获取影像的水平分辨率，以每米像素数为单位 
  res = FreeImage_GetDotsPerMeterX( m_pDIB );
  WORD unNewWidth  = WORD(m_nSourceWidth /(WORD)((float)res/39.37 + 0.5)* m_fXResolution);

	// 获取影像的垂直分辨率，以每米像素数为单位 
  res = FreeImage_GetDotsPerMeterY( m_pDIB );
  WORD unNewHeight = WORD(m_nSourceHeight/(WORD)((float)res/39.37 + 0.5)* m_fYResolution);

  cout << "ds: rescaling... to " << unNewWidth << " x " << unNewHeight << endl;
  //pDib = FreeImage_Rescale( m_pDIB, unNewWidth, unNewHeight, FILTER_BILINEAR);
  
	//zhu
	
	if (m_nOrientation == TWOR_PORTRAIT) //纵向
	{
		pDib = FreeImage_Rescale( m_pDIB, unNewWidth, unNewHeight, FILTER_BILINEAR);
	} 
	else if(m_nOrientation == TWOR_LANDSCAPE) //横向
	{
		pDib = FreeImage_Rescale( m_pDIB, unNewHeight, unNewWidth, FILTER_BILINEAR);
	}
	else{} 

	//zhu FreeImage_RotateClassic
	/*char buf[1024];
	itoa((int)m_fRotation,buf,10);
	::MessageBox(g_hwndDLG,buf,"UDS",MB_OK);*/

	FIBITMAP *rotatedimg = 0;
	switch((int)m_fRotation)
	{
	case TWOR_ROT0:
		rotatedimg = FreeImage_RotateClassic(pDib, 0);
		break;
	case TWOR_ROT90:
		rotatedimg = FreeImage_RotateClassic(pDib, -90);
		break;
	case TWOR_ROT180:
		rotatedimg = FreeImage_RotateClassic(pDib, -180);
		break;
	case TWOR_ROT270:
		rotatedimg = FreeImage_RotateClassic(pDib, -270);
		break;
	default:
		rotatedimg = pDib;
		break;
	} 
	pDib = rotatedimg;
	//zhu 或者使用FreeImage_RotateEx

	//FreeImage_Invert(pDib);//翻转每一个像素值，0变为1，1变为0；
	
	//图像镜像处理
	if(m_bMirror == TWMR_AUTO)
	{
		//::MessageBox(g_hwndDLG,TEXT("镜像"),MB_CAPTION,MB_OK);
		FreeImage_FlipHorizontal(pDib);
	}
	else
	{}

	FIBITMAP *splitimg = 0;
	//图像分割
	if(m_nSpiltImage == TWSI_NONE)
	{
		//::MessageBox(g_hwndDLG,TEXT("不分割图像"),MB_CAPTION,MB_OK);
		splitimg = pDib;
	}
	else if(m_nSpiltImage == TWSI_HORIZONTAL) //水平分割
	{
		//splitimg = FreeImage_Copy(pDib,0,0,unNewWidth,unNewHeight/2);
		splitimg = SpiltImage(pDib,2,1);
	}
	else if(m_nSpiltImage == TWSI_VERTICAL) //垂直分割
	{
		//splitimg = FreeImage_Copy(pDib,0,0,unNewWidth/2,unNewHeight);
		splitimg = SpiltImage(pDib,1,2);
	}
	pDib = splitimg;
	
	//需要增加二值化、图像分割及其他checkbox对应图像处理的处理
	
	if(0 == pDib)
  {
    return false;
  }
  else
  {
    FreeImage_Unload(m_pDIB);
    m_pDIB = pDib;
  }

  if(m_nWidth <= 0 || m_nHeight <= 0)
	{
    m_nWidth  = m_nSourceWidth  = FreeImage_GetWidth(m_pDIB);
    m_nHeight = m_nSourceHeight = FreeImage_GetHeight(m_pDIB);
  }
  else
  {
    m_nSourceWidth  = FreeImage_GetWidth(m_pDIB);
    m_nSourceHeight = FreeImage_GetHeight(m_pDIB);
  }

	/*
	if (m_nOrientation == TWOR_PORTRAIT) //纵向
	{
		m_nWidth  = m_nSourceWidth  = FreeImage_GetWidth(m_pDIB);
		m_nHeight = m_nSourceHeight = FreeImage_GetHeight(m_pDIB);
	} 
	else if(m_nOrientation == TWOR_LANDSCAPE) //横向
	{
		m_nWidth  = m_nSourceWidth  = FreeImage_GetHeight(m_pDIB);
		m_nHeight = m_nSourceHeight = FreeImage_GetWidth(m_pDIB);
	}
	else{}*/
	
	/*
	m_nWidth  = m_nSourceWidth = FreeImage_GetWidth(m_pDIB);
	m_nHeight = m_nSourceHeight = FreeImage_GetHeight(m_pDIB);*/

  FreeImage_SetDotsPerMeterX( m_pDIB, WORD(m_fXResolution*39.37 + 0.5) );
  FreeImage_SetDotsPerMeterY( m_pDIB, WORD(m_fYResolution*39.37 + 0.5) );
  
  if(m_nPixelType != TWPT_RGB)
  {
    // Apply bit depth color transforms
    switch(m_nPixelType)
    {
      case TWPT_BW:
        cout << "ds: converting to TWPT_BW..." << endl;
        /// @todo impliment Dithering
        /// @todo add Threshold setting
        pDib = FreeImage_Threshold(m_pDIB, (BYTE)m_fThreshold);
      break;

      case TWPT_GRAY:
        cout << "ds: converting to TWPT_GRAY..." << endl;
        pDib = FreeImage_ConvertTo8Bits(m_pDIB);
      break;

      case TWPT_RGB:
        cout << "ds: converting toTWPT_RGB..." << endl;
        pDib = FreeImage_ConvertTo24Bits(m_pDIB);
      break;
    }

    if(0 == pDib)
    {
      return false;
    }
    else
    {
      FreeImage_Unload(m_pDIB);
      m_pDIB = pDib;
    }
  }

  switch(m_nPixelType)
  {
    case TWPT_BW:
      m_nDestBytesPerRow = BYTES_PERLINE(m_nWidth, 1);
      break;

    case TWPT_GRAY:
      m_nDestBytesPerRow = BYTES_PERLINE(m_nWidth, 8);
      break;

    case TWPT_RGB:
      m_nDestBytesPerRow = BYTES_PERLINE(m_nWidth, 24);
      break;
  }

  // setup some convenience vars because they are used during 
  // every strip request
  m_nScanLine       = 0;

  return true;
}

//void CScanner_FreeImage::SpiltImage(const Mat& src_img,int m,int n)
FIBITMAP* CScanner_FreeImage::SpiltImage(FIBITMAP *dib,int m,int n)
{
	int ceil_width = FreeImage_GetWidth(dib)/n; //上下分割：1600，1100
	int ceil_height = FreeImage_GetHeight(dib)/m; //左右分割：800，2200

	FIBITMAP* dst;
	if(m_nDocCount == 1) //总张数暂时取2
	{
		dst = FreeImage_Copy(dib,0,0,ceil_width,ceil_height);
	}
	else if(m_nDocCount == 0)
	{
		if(m_nSpiltImage == TWSI_HORIZONTAL)
		{
			dst = FreeImage_Copy(dib,0,ceil_height,ceil_width,ceil_height*m);
		}
		else //垂直
		{		
			dst = FreeImage_Copy(dib,ceil_width,0,ceil_width*n,ceil_height);
		}	
	}
	else
	{
		char buf[10];
		itoa(m_nDocCount, buf, 10);
		::MessageBox(g_hwndDLG,TEXT(buf),"SpiltImage--m_nDocCount",MB_OK);
	}
	return dst;
	//FreeImage_Save(FIF_JPEG,dst,"C://Users//Administrator//Desktop//边缘检测后的图.jpeg",0);
	//::MessageBox(g_hwndDLG,TEXT("垂直"),MB_CAPTION,MB_OK);
}

//////////////////////////////////////////////////////////////////////////////
// We want to simulate getting a scan form a scanner.   模拟扫描仪的扫描过程
// if a size larger than the paper is scanned then there will be black on the bottom 
// and to the right of the image.  We want to transfer the image top to bottom, 
// the black will be transfered after the image if neccessary.
bool CScanner_FreeImage::getScanStrip(BYTE *pTransferBuffer, DWORD dwRead, DWORD &dwReceived)
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
  WORD    nMaxRows  = (WORD)(dwRead / m_nDestBytesPerRow); //number of rows行 to be transfered during this call (function of buffer size and line size)

  if( m_nScanLine < MIN(m_nSourceHeight, m_nHeight) )
  {
    //fill the buffer line by line to take care of alignment differences
    for(nRow = 0; nRow<nMaxRows; nRow++)
    {
      //get the next scan line position and copy it
      pBits = (BYTE*)FreeImage_GetScanLine(m_pDIB, m_nSourceHeight-m_nScanLine-1);

      memcpy( pTransferBuffer, pBits, MIN(m_nDestBytesPerRow, FreeImage_GetLine(m_pDIB)) );

      // Check to see if the result image width is wider than what we have.
      // If it is wider fill it in with 0es
      if(m_nDestBytesPerRow > FreeImage_GetLine(m_pDIB))
      {
        memset( pTransferBuffer+FreeImage_GetLine(m_pDIB), 0, m_nDestBytesPerRow - FreeImage_GetLine(m_pDIB) );
      }

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
    memset( pTransferBuffer, 0, m_nDestBytesPerRow * nMaxRows );
    m_nScanLine += nMaxRows;
    dwReceived += m_nDestBytesPerRow * nMaxRows;
  }

  return true;
}

//////////////////////////////////////////////////////////////////////////////
short CScanner_FreeImage::getDocumentCount() const
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

//////////////////////////////////////////////////////////////////////////////
bool CScanner_FreeImage::isFeederLoaded()
{
  bool rtn = true;
  if(m_nDocCount<=0)
  {
    rtn = false;
    m_nDocCount = m_nMaxDocCount;// Reloaded the scanner with paper
  }
  return rtn;
}

////////////////////////////////////////////////////////////////////////////////
bool CScanner_FreeImage::getDeviceOnline() const
{
  return true;
}

////////////////////////////////////////////////////////////////////////////////
//void CScanner_FreeImage::SetImagePath_Signal(char* szImagePath)
//{
//	FREE_IMAGE_FORMAT fif = FreeImage_GetFileType(szImagePath);  // 由文件头分析文件类型
//	if(fif == FIF_UNKNOWN)
//	{
//		fif  =  FreeImage_GetFIFFromFilename(szImagePath);  // 从文件扩展名判断文件类型
//	}
//	if( (fif == FIF_UNKNOWN) || !FreeImage_FIFSupportsReading(fif) )  // 判断是否支持该位图类型的读操作
//	{
//		::MessageBox(g_hwndDLG, "不支持该类型文件", NULL, 0);  
//		return;
//	}
//	strcpy_s(m_szSourceImagePath,szImagePath);
//	WritePrivateProfileString("ImagePath Setting","ImagePath",szImagePath,m_szIniPath);  // 讲图片路径保存在INI文件中
//	strcpy_s(m_szSourceImagePath,szImagePath);
//	
//}

////////////////////////////////////////////////////////////////////////////////
void CScanner_FreeImage::SetImagePath_Multi(vector<string> vector_string_imagepath)
{
	if (vector_string_imagepath.empty())  // 先判断vector_string_imagepath是否为空
	{
		::MessageBox(g_hwndDLG, TEXT("未选择图片!"), NULL, 0);  
		return;
	}


	//vector<string>::iterator it = vector_string_imagepath.begin();
	//unsigned int unCount = vector_string_imagepath.size();  // 获取图片数
	//unsigned int unImageIndex = 0;  // 序号从0开始
	//
	//string strINIKeyName = INI_KEY_PREFIX_IMAGEPATH;  // INI键名前缀

	//WritePrivateProfileString(INI_SECTION_IMAGE,NULL,NULL,m_szIniPath); // 清空节[Image Setting]

	//for (it; it != vector_string_imagepath.end(); it++)
	//{

	//	FREE_IMAGE_FORMAT fif = FreeImage_GetFileType((*it).c_str());  // 由文件头分析文件类型
	//	if(fif == FIF_UNKNOWN)
	//	{
	//		fif  =  FreeImage_GetFIFFromFilename((*it).c_str());  // 从文件扩展名判断文件类型
	//	}
	//	if( (fif == FIF_UNKNOWN) || !FreeImage_FIFSupportsReading(fif) )  // 判断是否支持该位图类型的读操作
	//	{
	//		string strMsg;
	//		strMsg = (*it).c_str();
	//		strMsg += " 文件不支持，请重新选择！";
	//		::MessageBox(g_hwndDLG, strMsg.c_str(), NULL, 0);  
	//		break;
	//		return;
	//	}

	//	unImageIndex++;	
	//	char szTemp[64] = {0};
	//	_itoa_s(unImageIndex,szTemp,10);  // int->char 与 sprintf_s(szTemp,"%d",unImageIndex); 作用相同
	//	
	//	string strImageIndex(szTemp);  // char->string
	//	strINIKeyName += strImageIndex;
	//
	//	WritePrivateProfileString(INI_SECTION_IMAGE,strINIKeyName.c_str(),(*it).c_str(),m_szIniPath);  // 图片绝对路径循环写入INI文件
	//	strINIKeyName = "ImagePath";  // 键名前缀重置为"ImagePath"
	//}

	//char szImageCount[64] = {0}; 
	//_itoa_s(unImageIndex,szImageCount,10);

	//WritePrivateProfileString(INI_SECTION_IMAGE,INI_KEY_IMAGECOUNT,szImageCount,m_szIniPath);  // 图片数写入键INI文件

	////string temp = szImageCount;
	////temp += ";图片数\r\n";  // 注释
 //// //WritePrivateProfileSection("ImagePath Setting",temp.c_str(),m_szIniPath);
	////WritePrivateProfileString("ImagePath Setting","ImageCount",temp.c_str(),m_szIniPath);  // 图片数写入键INI文件

	m_vector_string_imagepath = vector_string_imagepath;
	
}

//void CScanner_FreeImage::GetImagePathFromINI()
//{
//	unsigned int unCount = GetPrivateProfileInt(INI_SECTION_IMAGE,
//		INI_KEY_IMAGECOUNT,0,m_szIniPath);  // 获取图片总数
//
//	for (unsigned int i=1;i <= unCount; i++)
//	{
//		char szTemp[64] = {0};
//		_itoa_s(i,szTemp,10); 
//
//		//::MessageBox(g_hwndDLG,szTemp,"UDS",MB_OK);
//
//		char szImagePath[MAX_PATH];
//		string str_key_imagepath = INI_KEY_PREFIX_IMAGEPATH ;
//		string str_image_index(szTemp);  // char->string
//		str_key_imagepath += str_image_index;
//
//		GetPrivateProfileString(INI_SECTION_IMAGE,str_key_imagepath.c_str(),
//			"",szImagePath,MAX_PATH,m_szIniPath);
//
//		string str_image_path(szImagePath);
//		m_vector_string_imagepath.push_back(str_image_path);
//	}
//
//	//m_pUI->SetCapValueInt(UDSCAP_DOCS_IN_ADF,nCount);	
//}





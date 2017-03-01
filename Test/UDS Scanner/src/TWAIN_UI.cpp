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
* @file CTWAINDS_FreeImage.cpp
* The main Data Source class.
* This class is derived from the Base class describing a TWAIN DS.
* @author TWAIN Working Group
* @date April 2007
*/
#include "CTWAINDS_UDS.h"  // 加了CScanner_G6400后，会出现WINDOWS.h重复包含问题，需要将该语句移到#include "CommonDS.h"之上
#include "CommonDS.h"
#include "TWAIN_UI.h"
#include <typeinfo>

#ifdef TWNDS_OS_LINUX
  #include <unistd.h>
  #include <sys/stat.h>
  #include <sys/types.h>
  #include <dirent.h>
  #include <pwd.h>
  #define MAX_PATH 1024
#elif defined(TWNDS_OS_WIN)
  #include <shlobj.h>
#else

#endif

extern HWND g_hwndDLG;

CTWAIN_UI::CTWAIN_UI(CTWAINDS_UDS *pDS)
{
  m_pDS = pDS;
  m_bScanning=false;
  memset(&m_EnableDSdata,0,sizeof(TW_USERINTERFACE));
  m_strProfilesDirectory=="";

  char strProfilesPath[MAX_PATH];
  char strOldPath[MAX_PATH];
#ifdef TWNDS_OS_LINUX
  getcwd(strOldPath,MAX_PATH);

  strcpy(strProfilesPath , getenv ("HOME"));
#elif defined(TWNDS_OS_WIN)
  GetCurrentDirectory(MAX_PATH, strOldPath);
  // get temp Application data directory for all users
  if(!SHGetSpecialFolderPath(NULL, strProfilesPath, CSIDL_COMMON_APPDATA|CSIDL_FLAG_CREATE, TRUE)) 
  {
    return;
  }
#else

#endif
  
  if(strProfilesPath[strlen(strProfilesPath)-1] != '/')
  {
    SSTRCAT(strProfilesPath, MAX_PATH, "/");
  }

  SSTRCAT(strProfilesPath, MAX_PATH, PROFILELOCATION);
  if(strProfilesPath[strlen(strProfilesPath)-1] != '/')
  {
    SSTRCAT(strProfilesPath, MAX_PATH, "/");
  }
  SSTRCAT(strProfilesPath, MAX_PATH, kCUSTOMDSGUI);
  if(strProfilesPath[strlen(strProfilesPath)-1] != '/')
  {
    SSTRCAT(strProfilesPath, MAX_PATH, "/");
  }

  // Set current directory to profiles directory 
#ifdef TWNDS_OS_LINUX
  if(chdir(strProfilesPath))
#elif defined(TWNDS_OS_WIN)
  if(!SetCurrentDirectory(strProfilesPath))
#else

#endif
  {
    char     szPath[MAX_PATH];
    char     szTempPath[MAX_PATH];
    char     *pPath;
    char     *pSeparator;

    SSTRCPY(szPath, MAX_PATH, strProfilesPath);
    pPath    = szPath;
    // creates profiles path if it is needed
    while( pSeparator = strchr(pPath, '/') )
    {
      *pSeparator = '\0';
      SSTRCPY(szTempPath, MAX_PATH, pPath);
      SSTRCAT(szTempPath, MAX_PATH, "/");

#ifdef TWNDS_OS_LINUX
      if(chdir(szTempPath))
#elif defined(TWNDS_OS_WIN)
      if(!SetCurrentDirectory(szTempPath))
#else

#endif
      {
#ifdef TWNDS_OS_LINUX
        if(mkdir(szTempPath,S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IWGRP | S_IXGRP | S_IROTH | S_IWOTH | S_IXOTH))
#elif defined(TWNDS_OS_WIN)
        if( !CreateDirectory(szTempPath, NULL) )
#else

#endif
        {
          return;
        }
#ifdef TWNDS_OS_LINUX
        chmod(szTempPath,S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IWGRP | S_IXGRP | S_IROTH | S_IWOTH | S_IXOTH);
        if(chdir(szTempPath))
#elif defined(TWNDS_OS_WIN)
        if(!SetCurrentDirectory(szTempPath))
#else

#endif
        {
          return;
        }
      }
      pPath = pSeparator+1;
    }
  }

  m_strProfilesDirectory = strProfilesPath;

#ifdef TWNDS_OS_LINUX
  chdir(strOldPath);
#elif defined(TWNDS_OS_WIN)
  SetCurrentDirectory(strOldPath);
#else

#endif
}

TW_INT16 CTWAIN_UI::DisplayTWAINGUI(TW_USERINTERFACE Data, bool bSetup,
	bool bIndicators)
{
  m_bIndicators = bIndicators;
  m_bSetup = bSetup;
  m_EnableDSdata = Data;
  m_bScanning =(Data.ShowUI==0);
  return 0;
}

void CTWAIN_UI::DestroyTWAINGUI()
{
  m_bScanning=false;
  memset(&m_EnableDSdata,0,sizeof(TW_USERINTERFACE));
	//::MessageBox(g_hwndDLG,TEXT("DestroyTWAINGUI!"),MB_CAPTION,MB_OK);
}

void CTWAIN_UI::UpdateProgress(bool bShow, unsigned char ucProgress, 
	unsigned int unPageNo, string strProgressTitle)
{
  ;
}

void CTWAIN_UI::Scan()
{
  if(m_pDS->StartScanning())
  {
    m_bScanning = m_pDS->DoXferReadyEvent();
  }
	else
	{
		Cancel();
	}
}

bool CTWAIN_UI::GetColorsUsed(int nBpp, DWORD& dwColors)
{
	switch(nBpp)
	{
	case 1:
		dwColors = 2;
		break;
	case 4:
		dwColors = 16;
		break;
	case 8:
		dwColors = 256;
		break;
	case 24:
		dwColors = 0;//对于真彩色图，没用到调色板
		break;
	default:
		//不处理其它的颜色数，认为出错。
		MessageBox("Invalidcolornumbers!","ErrorMessage",
			MB_OK|MB_ICONEXCLAMATION);
		break;
		return false;
	}
	return true;
}

void CTWAIN_UI::FillZero(BYTE* pIn, BYTE* pOut, int nWidth, int nHeight, int nBpp)
{
	if (NULL == pIn)
	{
		return;
	}

	int bytesPerRow = (nWidth * nBpp) >> 3;  // 原图每行占用的字节数
	int widthStep = (((nWidth * nBpp) + 31) >> 5) << 2; //每行实际占用的大小（每行都被填充到一个4字节边界）

	for(int y=0; y < nHeight; y++)
	{
		for(int x=0; x < widthStep; x++)
		{
			if(x >= bytesPerRow)
			{
				pOut[y*widthStep+x] = 0;
				continue;
			}
			pOut[y*widthStep+x] = *pIn;
			pIn ++;
		}
	}
}


BYTE* CTWAIN_UI::PreView()
{
	BYTE *pSourceBuff = NULL;
	BYTE *pTemp = NULL;
	TW_INT16 twrc = TWRC_FAILURE;

	if(m_pDS->StartScanning())
	{
		m_bScanning = m_pDS->DoXferReadyEvent();	
		// Get the image that should be waiting for us.
		twrc = m_pDS->transfer();
		pSourceBuff = (BYTE*)_DSM_LockMemory(m_pDS->m_hImageData);
		
		m_nBpp = m_pDS->m_ImageInfo.BitsPerPixel;
		int SrcWidth  = m_pDS->m_ImageInfo.ImageWidth;
		int SrcLength = m_pDS->m_ImageInfo.ImageLength;

		//构建BMP文件信息头
		int widthStep = (((SrcWidth * m_nBpp) + 31) >> 5) << 2; //每行实际占用的大小（每行都被填充到一个4字节边界）
		m_bmpInfoHeader.biSize = 40;       // header size
		m_bmpInfoHeader.biWidth = SrcWidth;
		m_bmpInfoHeader.biHeight = -SrcLength; //BMP图片从最后一个点开始扫描，显示时图片是倒着的，所以用-height，这样图片就正了
		m_bmpInfoHeader.biPlanes = 1;
		m_bmpInfoHeader.biBitCount = m_nBpp;     // RGB encoded, 24 bits
		m_bmpInfoHeader.biCompression = BI_RGB;   // no compression 非压缩
		m_bmpInfoHeader.biSizeImage = widthStep * SrcLength/**3*/;
		m_bmpInfoHeader.biXPelsPerMeter = static_cast<LONG>(FIX32ToFloat(m_pDS->m_ImageInfo.XResolution) * 39.37F + 0.5);//像素每米;
		m_bmpInfoHeader.biYPelsPerMeter = static_cast<LONG>(FIX32ToFloat(m_pDS->m_ImageInfo.YResolution) * 39.37F + 0.5);//像素每米;
		m_bmpInfoHeader.biClrImportant = 0; 
		GetColorsUsed(m_nBpp, m_bmpInfoHeader.biClrUsed);  // bpp==1?2:bpp==8?256:0;// B&W = 2,  Grey = 256,  Color = 0

		//构建BMP位图文件头 
		widthStep = (((SrcWidth * m_nBpp) + 31) >> 5) << 2; //每行实际占用的大小（每行都被填充到一个4字节边界）
		m_bmpFileHeader.bfType = ((WORD) ('M' << 8) | 'B');  //'BM'
		m_bmpFileHeader.bfSize = (DWORD)sizeof(BITMAPFILEHEADER)
			+ (DWORD)sizeof(BITMAPINFOHEADER)
			+ widthStep * SrcLength;
		m_bmpFileHeader.bfReserved1 = 0;
		m_bmpFileHeader.bfReserved2 = 0;
		m_bmpFileHeader.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER)	+ (DWORD)sizeof(BITMAPINFOHEADER);
		
		DWORD dwColors = 0;
		GetColorsUsed(m_nBpp, dwColors);
		if (m_nBpp < 16)
		{
			m_bmpFileHeader.bfSize    += sizeof(RGBQUAD) * dwColors;
			m_bmpFileHeader.bfOffBits += sizeof(RGBQUAD) * dwColors;
		} 

		//调色板
		m_bmpLpRGB = NULL;
		switch(m_nBpp)
		{
		case 1:
			m_bmpLpRGB = new RGBQUAD[2]; //申请颜色表需要的空间
			break;
		case 4:
			m_bmpLpRGB = new RGBQUAD[16]; //申请颜色表需要的空间
			break;
		case 8:
			m_bmpLpRGB = new RGBQUAD[256]; //申请颜色表需要的空间
			break;
		default:
			break;
		}

		dwColors = 0;
		if(GetColorsUsed(m_nBpp, dwColors))
		{
			switch(m_nBpp)
			{
			case 1:
				{
					m_bmpLpRGB[0].rgbBlue=0;
					m_bmpLpRGB[0].rgbGreen=0;
					m_bmpLpRGB[0].rgbRed=0;
					m_bmpLpRGB[0].rgbReserved=0;

					m_bmpLpRGB[1].rgbBlue=255;
					m_bmpLpRGB[1].rgbGreen=255;
					m_bmpLpRGB[1].rgbRed=255;
					m_bmpLpRGB[1].rgbReserved=0;
				}
				break;
			case 4:
				{
					for(unsigned int i=0; i < 16; i++) //对于256个灰度等级调色板索引进行赋值
					{
						unsigned int nTemp  = i * 17;
						m_bmpLpRGB[i].rgbBlue     = nTemp;
						m_bmpLpRGB[i].rgbGreen    = nTemp;
						m_bmpLpRGB[i].rgbRed      = nTemp;
						m_bmpLpRGB[i].rgbReserved = 0;
					}
				}
				break;
			case 8:
				{
					for(unsigned int i=0; i < 256; i++) //对于256个灰度等级调色板索引进行赋值
					{
						m_bmpLpRGB[i].rgbBlue     = i;
						m_bmpLpRGB[i].rgbGreen    = i;
						m_bmpLpRGB[i].rgbRed      = i;
						m_bmpLpRGB[i].rgbReserved = 0;				
					}
				}
				break;
			default:
				break;
			}
		}

		//补零
		//每行实际占用的大小（每行都被填充到一个4字节边界）
		m_nDIBSize = widthStep * SrcLength;  //buffer的大小 （字节为单位）

		pTemp = new BYTE[m_nDIBSize];
		memset(pTemp, 0, m_nDIBSize);
		FillZero(pSourceBuff, pTemp, SrcWidth, SrcLength, m_nBpp);

		m_pDS->m_CurrentState = dsState_Enabled;  //预览必须把状态设回5,否则预览后不能扫描
		// cleanup
		if(pSourceBuff)
		{
			_DSM_UnlockMemory(m_pDS->m_hImageData);
		}

		if(pTemp != NULL)
		{
			return pTemp;
		}		
	}
	else
	{
		Cancel();
	}	
}


void CTWAIN_UI::Cancel()
{
  if(m_bScanning)
  {
    m_pDS->StopScanning();
  }
  m_pDS->DoCloseDSRequestEvent();
}

void CTWAIN_UI::Save()
{
  m_pDS->DoCloseDSOkEvent();
}

unsigned int CTWAIN_UI::MessageBox(string strMessage,string strTitle,
	unsigned int unIconID)
{
  return 0;
}

bool CTWAIN_UI::processEvent(pTW_EVENT _pEvent)
{
  return false;
}

int CTWAIN_UI::GetCurrentCapIndex(const TW_UINT16 _unCap)
{
  CTWAINContainer  *pfCap = m_pDS->findCapability(_unCap);
	//::MessageBox(hwndDLG,"findCapability",MB_CAPTION,MB_OK);
  if(pfCap==NULL)
  {
    assert(0);
    return 0;//@TODO
  }

  return pfCap->m_nCurrent;
}

TW_FRAME CTWAIN_UI::GetCurrentFrame()
{
  CTWAINContainerFrame *pfCap = (CTWAINContainerFrame *)m_pDS->findCapability(ICAP_FRAMES);
  if(pfCap==NULL)
  {
    assert(0);
    return TW_FRAME();//@TODO
  }
  InternalFrame frame = pfCap->m_listFrames.at(pfCap->m_nCurrent);
  int   unit = TWUN_PIXELS;
  float Xres = 100;
  float Yres = 100;
  m_pDS->getCurrentUnits(unit, Xres, Yres);
  return frame.AsTW_FRAME(unit, Xres, Yres);
}

const IntVector* CTWAIN_UI::GetValidCap(const TW_UINT16 _unCap)
{
  CTWAINContainer  *pfCap = m_pDS->findCapability(_unCap);
  if(pfCap==NULL)
  {
    assert(0);
    return 0;//@TODO
  }
  if(typeid(*pfCap) == typeid(CTWAINContainerBool))
  {
    CTWAINContainerBool *pfBoolCap = (CTWAINContainerBool*)pfCap;
    return &pfBoolCap->m_listBools;
  }
  else if(typeid(*pfCap) == typeid(CTWAINContainerInt))
  {
    CTWAINContainerInt *pfIntCap = (CTWAINContainerInt*)pfCap;
    return &pfIntCap->m_listInts;
  } 
  assert(0);
  return 0;
}

const FloatVector* CTWAIN_UI::GetValidCapFloat(const TW_UINT16 _unCap)
{
  CTWAINContainer  *pfCap = m_pDS->findCapability(_unCap);
  if(pfCap==NULL)
  {
    assert(0);
    return 0;//@TODO
  }
  if(typeid(*pfCap) == typeid(CTWAINContainerFix32))
  {
    CTWAINContainerFix32 *pfFix32Cap = (CTWAINContainerFix32*)pfCap;
    return &pfFix32Cap->m_listFloats;
  }
  assert(0);
  return 0;
}

float CTWAIN_UI::GetCapValueFloat(const TW_UINT16 _unCap)
{
  CTWAINContainer  *pfCap = m_pDS->findCapability(_unCap);
  if(pfCap==NULL)
  {
    assert(0);
    return 0;//@TODO
  }
  
  float fVal;
  if(typeid(*pfCap) == typeid(CTWAINContainerFix32))
  {
    CTWAINContainerFix32 *pfFix32Cap = (CTWAINContainerFix32*)pfCap;
    if(pfFix32Cap->GetCurrent(fVal))
    {
      return fVal;
    }
  }
  else if(typeid(*pfCap) == typeid(CTWAINContainerFix32Range))
  {
    CTWAINContainerFix32Range *pfFix32Cap = (CTWAINContainerFix32Range*)pfCap;
    if(pfFix32Cap->GetCurrent(fVal))
    {
      return fVal;
    }
  }  
  assert(0);
  return 0;
}

//zhu
bool CTWAIN_UI::GetCapValueBool(const TW_UINT16 _unCap)
{
	CTWAINContainer  *pfCap = m_pDS->findCapability(_unCap);
	if(pfCap==NULL)
	{
		assert(0);
		return 0;//@TODO
	}

	bool bVal;
	if(typeid(*pfCap) == typeid(CTWAINContainerBool))
	{
		CTWAINContainerBool *pfBoolCap = (CTWAINContainerBool*)pfCap;
		if(pfBoolCap->GetCurrent(bVal))
		{
			return bVal;
		}
	}
	
	assert(0);
	return 0;
}

bool CTWAIN_UI::SetCurrentFrame(float fTop, float fLeft, float fBottom, float fRight)
{

  TW_CAPABILITY cap;
  cap.Cap = ICAP_FRAMES;
  cap.ConType = TWON_ONEVALUE;
  cap.hContainer = _DSM_Alloc(sizeof(TW_ONEVALUE_FRAME));
  TW_INT16 twRC;  
  if(0 != cap.hContainer)
  {
    TW_ONEVALUE_FRAME* pCap = (TW_ONEVALUE_FRAME*)_DSM_LockMemory(cap.hContainer);
    pCap->ItemType = TWTY_FRAME;
    pCap->Item.Top = FloatToFIX32(fTop);
    pCap->Item.Left = FloatToFIX32(fLeft);
    pCap->Item.Bottom = FloatToFIX32(fBottom);
    pCap->Item.Right = FloatToFIX32(fRight);

    _DSM_UnlockMemory(cap.hContainer);
    twRC = m_pDS->dat_capability(MSG_SET,&cap);
    _DSM_Free(cap.hContainer);
  }
  else
  {
    return false;
  }

  m_pDS->setConditionCode(TWCC_SUCCESS); // clear condition code 
  if(twRC==TWRC_SUCCESS || (twRC==TWRC_CHECKSTATUS))
  {
    return true;
  }

  return false;
}

bool CTWAIN_UI::GetCapRangeFloat(const TW_UINT16 _unCap, float &fMin, float &fMax, float &fStep)
{
  CTWAINContainer  *pfCap = m_pDS->findCapability(_unCap);
  if(pfCap==NULL)
  {
    return false;
  }
  if(typeid(*pfCap) == typeid(CTWAINContainerFix32Range))
  {
    CTWAINContainerFix32Range *pfFloatCap = (CTWAINContainerFix32Range*)pfCap;
    pfFloatCap->GetMinMaxStep(fMin, fMax, fStep);
    return true;
  } 
  return false;
}

bool CTWAIN_UI::SetCapValueFloat(const TW_UINT16 _unCap, float fVal)
{
  TW_CAPABILITY cap;
  cap.Cap = _unCap;
  cap.ConType = TWON_ONEVALUE;
  cap.hContainer = _DSM_Alloc(sizeof(TW_FRAME));
  TW_INT16 twRC;  
  if(0 != cap.hContainer)
  {
    TW_ONEVALUE_FIX32* pCap = (TW_ONEVALUE_FIX32*)_DSM_LockMemory(cap.hContainer);
    pCap->ItemType = TWTY_FIX32;
    pCap->Item = FloatToFIX32(fVal);
    _DSM_UnlockMemory(cap.hContainer);
    twRC = m_pDS->dat_capability(MSG_SET,&cap);
    _DSM_Free(cap.hContainer);
  }
  else
  {
    return false;
  }

  m_pDS->setConditionCode(TWCC_SUCCESS); // clear condition code 
  if(twRC==TWRC_SUCCESS || (twRC==TWRC_CHECKSTATUS))
  {
    return true;
  }

  return false;
}

bool CTWAIN_UI::SetCapValueInt(const TW_UINT16 _unCap, int nVal)
{
  CTWAINContainer *pCapCont = m_pDS->findCapability(_unCap);

  if(pCapCont == 0)
  {
    return false;
  }

  TW_CAPABILITY cap;
  cap.Cap = _unCap;
  cap.ConType = TWON_ONEVALUE;
  cap.hContainer = _DSM_Alloc(sizeof(TW_ONEVALUE));
  TW_INT16 twRC;  
  if(0 != cap.hContainer)
  {
    TW_ONEVALUE* pCap = (TW_ONEVALUE*)_DSM_LockMemory(cap.hContainer);
    pCap->ItemType = pCapCont->GetItemType();
    pCap->Item = nVal;
    _DSM_UnlockMemory(cap.hContainer);
    twRC = m_pDS->dat_capability(MSG_SET,&cap);
    _DSM_Free(cap.hContainer);
  }
  else
  {
    return false;
  }

  m_pDS->setConditionCode(TWCC_SUCCESS); // clear condition code 
  if(twRC==TWRC_SUCCESS || (twRC==TWRC_CHECKSTATUS))
  {
    return true;
  }

	//::MessageBox(g_hwndDLG,TEXT("返回false"),MB_CAPTION,MB_OK);
  return false;
}

bool CTWAIN_UI::ResetAllCaps()
{
  TW_CAPABILITY cap;
  cap.Cap = CAP_SUPPORTEDCAPS;
  cap.ConType = TWON_DONTCARE16;
  cap.hContainer = 0;
  TW_INT16 twRC;  
  twRC = m_pDS->dat_capability(MSG_RESETALL,&cap);
  m_pDS->setConditionCode(TWCC_SUCCESS); // clear condition code 
  if(twRC==TWRC_SUCCESS || (twRC==TWRC_CHECKSTATUS))
  {
    return true;
  }

  return false;
}

#ifdef TWNDS_OS_LINUX
void CTWAIN_UI::TW_GetAllProfiles(lstString& strFileNames)
{
  strFileNames.clear();
  // Set current directory to profiles directory 
  DIR *dir;
  dirent *dirInfo;
  if((dir  = opendir(m_strProfilesDirectory.c_str())) == NULL)
  {
    return;
  }
  TW_CUSTOMDSDATA DSDataBackUp;
  if(m_pDS->GetGustomDSData(&DSDataBackUp)!=TWRC_SUCCESS)
  {
    return ;
  }

  while(dirInfo=readdir(dir)) 
  {
    string strName = dirInfo->d_name;
    if(strName.size()>strlen(FILEEXTENTION) && 
			strName.substr(strName.size()-strlen(FILEEXTENTION),strlen(FILEEXTENTION))==FILEEXTENTION)
    {
      strName = strName.substr(0,strName.size()-strlen(FILEEXTENTION));
      try
      {
        // check if profile is valid
        if(TW_LoadProfileFromFile(strName))
        {
          strFileNames.push_back(strName);
        }
      }
      catch(...)
      {
      }

    }
  }

  m_pDS->SetGustomDSData(&DSDataBackUp);

  closedir(dir);
}
#elif defined(TWNDS_OS_WIN)

////////////////////////////////////////////////////////////////////////////////
// 获取模版文件
void CTWAIN_UI::TW_GetAllProfiles(lstString& strFileNames)
{
  char strOldPath[MAX_PATH];
  GetCurrentDirectory(MAX_PATH, strOldPath);

  strFileNames.clear();
  // Set current directory to profiles directory 
  if(!SetCurrentDirectory(m_strProfilesDirectory.c_str()))
  {
    return;
  }
  TW_CUSTOMDSDATA DSDataBackUp;
  if(m_pDS->GetGustomDSData(&DSDataBackUp)!=TWRC_SUCCESS)
  {
    return ;
  }

  WIN32_FIND_DATA FindFileData;
  HANDLE          hFind   = NULL;
  char            szFileName[MAX_PATH];
  char            *pDot;
  
  hFind = FindFirstFile("*"FILEEXTENTION, &FindFileData);
  while(hFind != INVALID_HANDLE_VALUE)
  {
    SSTRCPY(szFileName, MAX_PATH, FindFileData.cFileName);
    pDot = strchr(szFileName, '.');
    if(pDot)
    {
      *pDot = '\0';
    }

    try
    {
      // check if profile is valid
      if(TW_LoadProfileFromFile(szFileName))
      {
        strFileNames.push_back(szFileName);
      }
    }
    catch(...)
    {
    }
    if(!FindNextFile(hFind, &FindFileData))
    {
      FindClose(hFind);
      break;
    }
  }

  m_pDS->SetGustomDSData(&DSDataBackUp);
  SetCurrentDirectory(strOldPath);
}
#else

#endif

bool CTWAIN_UI::TW_LoadProfileFromFile(string strFileName)
{
  TW_HANDLE hData;
  strFileName  = m_strProfilesDirectory+strFileName+FILEEXTENTION;
	//CString str(strFileName);
	//::MessageBox(g_hwndDLG,strFileName.c_str(),MB_CAPTION,MB_OK);
  FILE *pFile = NULL;
  //open file
  FOPEN(pFile, strFileName.c_str(), "rb");
  if(pFile==0)
  {
    return false;
  }
  // get file size
  fseek(pFile, 0, SEEK_END);
  DWORD dwDataSize = (DWORD)ftell(pFile);
  rewind(pFile);
  bool bRes = true;
  // it has to contains at least DS identity
  if(dwDataSize==0)
  {
    fclose(pFile);
    return false;
  }
  // allocate storage and read CustomDSdata
  void * pData = NULL;
  if(hData = _DSM_Alloc(dwDataSize))
  {
    pData = _DSM_LockMemory(hData);
    if(pData)
    {
      if(fread(pData, dwDataSize, 1, pFile)!=1)
      {
        bRes = false;
      }
    }
    else
    {
      bRes = false;
    }
  }
  else
  {
    bRes = false;
  }
  fclose(pFile);
  
  if(hData)
  {
    _DSM_UnlockMemory(hData);
    if(bRes)
    {
      TW_CUSTOMDSDATA DSData;
      DSData.hData = hData;
      DSData.InfoLength = dwDataSize;
      bRes = m_pDS->SetGustomDSData(&DSData)==TWRC_SUCCESS;
    }
    // free resource on error
    _DSM_Free(hData);
  }
  
  return bRes;
}

bool CTWAIN_UI::TW_SaveProfileToFile(string strFileName)
{
  strFileName  = m_strProfilesDirectory+strFileName+FILEEXTENTION;

  TW_CUSTOMDSDATA DSData;
  if(m_pDS->GetGustomDSData(&DSData)!=TWRC_SUCCESS)
  {
    return false;
  }
  void* pData = _DSM_LockMemory(DSData.hData);
  if(!pData)
  {
    _DSM_Free(DSData.hData);
    return false;
  }

  FILE *pFile  = NULL;
  bool  bError = false;
  //opens file
  FOPEN(pFile, strFileName.c_str(), "wb"); // 只写打开或新建一个二进制文件；只允许写数据
  if(pFile)
  {
    bError = true;
    //store CustomDSdata
    if(fwrite(pData, DSData.InfoLength, 1, pFile)!=1)
    {
      bError = false;
    }
    fclose(pFile);
    //remove file on error
    if(!bError)
    {
      int i = remove(strFileName.c_str());
    }
  }
  
  _DSM_UnlockMemory(DSData.hData);
  _DSM_Free(DSData.hData);

  return bError;
}

bool CTWAIN_UI::TW_DeleteProfile(string strFileName)
{
  if(!strFileName.empty())// if profile is selected
  {
    strFileName  = m_strProfilesDirectory+strFileName+FILEEXTENTION;

    //delete file from the disk
    return remove(strFileName.c_str())==0; //remove成功返回0，失败返回-1
  }
  else
  {
    return false;
  }
}



/**
* @file CTWAINDS_FreeImage.cpp
* The main Data Source class.
* This class is derived from the Base class describing a TWAIN DS.
* @author TWAIN Working Group
* @date April 2007
*/
#include "Dlg_Device.h"
#include "CTWAINDS_UDS.h"
#include "TWAIN_UI.h"
//#include "FreeImage.h"
#include <sstream>

#include <iostream>
#include <cstdlib>
#include <assert.h>
#include <signal.h>
#include <typeinfo>
//#include "stdafx.h"
#ifdef TWH_CMP_MSC
#include <Winbase.h>
#endif
#include "FindDevice.h"

#include "public.h"


extern HWND g_hwndDLG;
extern int g_nDeviceNumber;  // 设备编号
extern DWORD  g_dwImageSize; 
extern void GetFilePath( char* szFileName, char* szFilePath);
// I found that compiling using the sunfreeware.com stuff on Solaris 9
// required this typedef. This is related to the inclusion of signal.h
#if defined (__SVR4) && defined (__sun)
typedef union {
  long double  _q;
  uint32_t     _l[4];
} upad128_t;
#endif

using namespace std;


// 中文版identity定义
TW_IDENTITY g_myIdentity_Chinese =
{
	0,                       // TW_UINT32  Id;               Unique number.  In Windows, application hWnd
	{                        // TW_VERSION Version;          Identifies the piece of code
		VERSION_MAJORNUMBER,   // TW_UINT16  MajorNum;         Major revision number of the software
		VERSION_MINORNUMBER,   // TW_UINT16  MinorNum;         Incremental revision number of the software
		TWLG_CHINESE_PRC,      // TW_UINT16  Language;         e.g. TWLG_SWISSFRENCH
		TWCY_CHINA,            // TW_UINT16  Country;          e.g. TWCY_SWITZERLAND
		"1.0.0"                // TW_STR32   Info;             e.g. "1.0b3 Beta release"
#ifdef _DEBUG
		" debug"
#else
		" release"
#endif
#ifdef TWH_32BIT
		" 32bit"
#else
		" 64bit"
#endif
	},
	2,                                  // TW_UINT16  ProtocolMajor;    Application and DS must set to TWON_PROTOCOLMAJOR
	1,                                  // TW_UINT16  ProtocolMinor;    Application and DS must set to TWON_PROTOCOLMINOR
	DG_IMAGE | DG_CONTROL | DF_DS2,     // TW_UINT32  SupportedGroups;  Bit field OR combination of DG_ constants
	"UDS",                              // TW_STR32   Manufacturer;     Manufacturer name, e.g. "Hewlett-Packard"
	MB_CAPTION,                         // TW_STR32   ProductFamily;    Product family name, e.g. "ScanJet"
	MB_CAPTION                          // TW_STR32   ProductName;      Product name, e.g. "ScanJet Plus"
};

// 原版identity定义
TW_IDENTITY g_myIdentity_Original =
{
	0,                                  // TW_UINT32  Id;               Unique number.  In Windows, application hWnd
	{                                       // TW_VERSION Version;          Identifies the piece of code
		2,                                    // TW_UINT16  MajorNum;         Major revision number of the software
			1,                                    // TW_UINT16  MinorNum;         Incremental revision number of the software
			TWLG_ENGLISH,                         // TW_UINT16  Language;         e.g. TWLG_SWISSFRENCH
			TWCY_USA,                             // TW_UINT16  Country;          e.g. TWCY_SWITZERLAND
			"2.1.3 sample"                        // TW_STR32   Info;             e.g. "1.0b3 Beta release"
#ifdef _DEBUG
			" debug"
#else
			" release"
#endif
#ifdef TWH_32BIT
			" 32bit"
#else
			" 64bit"
#endif
	},
	2,                                  // TW_UINT16  ProtocolMajor;    Application and DS must set to TWON_PROTOCOLMAJOR
	1,                                  // TW_UINT16  ProtocolMinor;    Application and DS must set to TWON_PROTOCOLMINOR
	DG_IMAGE | DG_CONTROL | DF_DS2,     // TW_UINT32  SupportedGroups;  Bit field OR combination of DG_ constants
	"TWAIN Working Group",              // TW_STR32   Manufacturer;     Manufacturer name, e.g. "Hewlett-Packard"
	"Software Scan",                    // TW_STR32   ProductFamily;    Product family name, e.g. "ScanJet"
	"TWAIN2 FreeImage Software Scanner" // TW_STR32   ProductName;      Product name, e.g. "ScanJet Plus"
};

//* Initialize global identiy for this DS. */
TW_IDENTITY CTWAINDS_Base::m_TheIdentity = g_myIdentity_Chinese;


//////////////////////////////////////////////////////////////////////////////
CTWAINDS_UDS::CTWAINDS_UDS(TW_IDENTITY AppID) :
  m_pICAP_FRAMES(0)
{
  m_AppID = AppID;
  // Setup our identity
  fillIdentityStructure(*getIdentity());
  m_pGUI = CreateUI(this);
//	GetImagePathFromINI();  

	CString strNoDeviceUI, strShowOnce;
	int nMaxLength = 512;
	TCHAR szINIPath[MAX_PATH];  // INI文件路径
	GetFilePath(FILENAME_INI,szINIPath);
	GetPrivateProfileString(INI_APP_DEVICE,INI_KEY_NOUI,TEXT(""),
		strNoDeviceUI.GetBufferSetLength(nMaxLength),nMaxLength,szINIPath);

	if (strNoDeviceUI.Find(TEXT("N")) >= 0)  // 显示界面
	{
		CDlg_Device dlg;
		dlg.DoModal();
	}
	else  // 不显示，需要判断上次设备是否连接
	{
		STRING vid,pid;
		BOOL ret = FALSE;

		// 判断上次设备是否处于连接状态
		g_nDeviceNumber = GetPrivateProfileInt(INI_APP_DEVICE,INI_KEY_DEVICENUMBER,1,szINIPath);
		switch(g_nDeviceNumber)
		{
		case DEVICE_G6400:		
			ret = FindScanner(TEXT("0638"), TEXT("2c73"));		
			break;
		case DEVICE_G6600:
			ret = FindScanner(TEXT("0638"), TEXT("2c74"));
			break;
		case DEVICE_CAMERA:
			ret = GetCameraCount();  // 这里只判断是否有摄像头设备
			break;
		case DEVICE_OPENCV:  // 虚拟扫描仪始终不需要显示
			ret = TRUE;
			break;
		default:
			break;
		}

		if (FALSE == ret) // 上次设备未连接，需要显示
		{
			CDlg_Device dlg;
			dlg.DoModal();
		}
	}

	switch (g_nDeviceNumber)
	{
	case DEVICE_G6400:
	case DEVICE_G6600:
		{
			m_pScanner = new CScanner_G6X00;
			break;
		}
	case DEVICE_OPENCV:
		{
			m_pScanner = new CScanner_OpenCV;
			break;
		}		
	case DEVICE_CAMERA:
		{
			m_pScanner = new CCamera_CxImage;
			break;
		}
	default:
		{
			m_pScanner = NULL;
			::MessageBox(g_hwndDLG,TEXT("不支持的设备!"),MB_CAPTION,MB_OK);
			break;
		}		
	}

	return;
	
}

bool CTWAINDS_UDS::StoreCapInStream(stringstream &_DsData, TW_UINT16 _unCapID, TW_UINT16 _unCapIdx, TW_UINT16 unContType)
{
	//::MessageBox(g_hwndDLG,"StoreCapInStream",MB_CAPTION,MB_OK);
  CUST_DS_DATA_ELEMENT *pCapCon =(CUST_DS_DATA_ELEMENT*) new BYTE[sizeof(CUST_DS_DATA_ELEMENT)];
  CTWAINContainer *pCap = findCapability(_unCapID);
  TW_UINT16 unType = pCap->GetItemType();
  pCapCon->unItemType = unType;
  pCapCon->unCapID = _unCapID;
  pCapCon->unCapIdx=_unCapIdx;
  pCapCon->unContType = unContType;
  pCapCon->dwSize = sizeof(CUST_DS_DATA_ELEMENT);

  if(unContType!=TWON_ONEVALUE)//currentlly storing a single value
  {
    delete []pCapCon;
    return false;
  }
  
  if(typeid(*pCap) == typeid(CTWAINContainerBool))
  {

    CTWAINContainerBool *pfBoolCap = (CTWAINContainerBool*)pCap;
    bool bVal;
    if(!pfBoolCap->GetCurrent(bVal))
    {
      delete []pCapCon;
      return false;
    }
    pCapCon->dwVal[0] = bVal?1:0;
  }
  else if(typeid(*pCap) == typeid(CTWAINContainerInt))
  {
    CTWAINContainerInt *pfIntCap = (CTWAINContainerInt*)pCap;
    int nVal;
    if(!pfIntCap->GetCurrent(nVal))
    {
      delete []pCapCon;
      return false;
    }
    pCapCon->dwVal[0] = nVal;
  } 
  else if(typeid(*pCap) == typeid(CTWAINContainerFix32))
  {
    CTWAINContainerFix32 *pfFix32Cap = (CTWAINContainerFix32*)pCap;
    float fVal;
    if(!pfFix32Cap->GetCurrent(fVal))
    {
      delete []pCapCon;
      return false;
    }
    *((float*)pCapCon->dwVal) = fVal;
  } 
  else if(typeid(*pCap) == typeid(CTWAINContainerFix32Range))
  {
    CTWAINContainerFix32Range *pfFix32Cap = (CTWAINContainerFix32Range*)pCap;
    float fVal;
    if(!pfFix32Cap->GetCurrent(fVal))
    {
     delete []pCapCon;
     return false;
    }
    *((float*)pCapCon->dwVal) = fVal;
  } 
  else if(typeid(*pCap) == typeid(CTWAINContainerFrame))
  {

    CTWAINContainerFrame *pfFrameCap = (CTWAINContainerFrame*)pCap;
    InternalFrame frmVal;
    if(!pfFrameCap->GetCurrent(frmVal))
    {
      delete []pCapCon;
      return false;
    }
    CUST_DS_DATA_ELEMENT *pCapCon1 =(CUST_DS_DATA_ELEMENT*) new BYTE[sizeof(CUST_DS_DATA_ELEMENT) + (4*sizeof(int)-sizeof(DWORD))];
    *pCapCon1=*pCapCon;
    delete []pCapCon;
    pCapCon=pCapCon1;
    pCapCon->dwSize +=(4*sizeof(int)-sizeof(DWORD)); 
    pCapCon->dwVal[0] = frmVal.nBottom;
    pCapCon->dwVal[1] = frmVal.nLeft;
    pCapCon->dwVal[2] = frmVal.nRight;
    pCapCon->dwVal[3] = frmVal.nTop;
  } 
  else
  {
    delete []pCapCon;
    return false;
  }

  _DsData.write((char*)pCapCon,pCapCon->dwSize);
  delete []pCapCon;
  return true;
}

bool CTWAINDS_UDS::ReadCapFromStream(stringstream &_DsData, TW_UINT16 _unCapID, TW_UINT16 _unCapIdx)
{//::MessageBox(g_hwndDLG,TEXT("ReadCapFromStream"),MB_CAPTION,MB_OK);
  _DsData.seekg(0, ios_base::beg);
  DWORD dwSize = sizeof(CUST_DS_DATA_ELEMENT);
  CUST_DS_DATA_ELEMENT *pCapCon =(CUST_DS_DATA_ELEMENT*) new BYTE[dwSize];
  pCapCon->unCapID=-1;
  pCapCon->unCapIdx=-1;
  pCapCon->dwSize = 0;
  while(!_DsData.eof() && (pCapCon->unCapID!=_unCapID || pCapCon->unCapIdx!=_unCapIdx))
  {
    _DsData.read((char*)pCapCon,sizeof(CUST_DS_DATA_ELEMENT));
    if(!_DsData.eof() && pCapCon->dwSize>sizeof(CUST_DS_DATA_ELEMENT))
    {
      if(pCapCon->dwSize>dwSize)
      {
        BYTE *pTemp = new BYTE[pCapCon->dwSize];
        memcpy(pTemp,pCapCon,sizeof(CUST_DS_DATA_ELEMENT));
        delete []pCapCon;
        pCapCon = (CUST_DS_DATA_ELEMENT*) pTemp;
        dwSize = pCapCon->dwSize;
      }
      _DsData.read((char*)pCapCon+sizeof(CUST_DS_DATA_ELEMENT),pCapCon->dwSize-sizeof(CUST_DS_DATA_ELEMENT));
    }
  }

  if(pCapCon->unCapID!=_unCapID || pCapCon->unCapIdx!=_unCapIdx)
  {
    delete []pCapCon;
    return false;
  }
  CTWAINContainer *pCap = findCapability(_unCapID);
  TW_UINT16 unType = pCap->GetItemType();
  if(unType !=pCapCon->unItemType)
  {
    delete []pCapCon;
    return false;
  }

  if(pCapCon->unContType!=TWON_ONEVALUE)//currentlly storing a single value
  {
    delete []pCapCon;
    return false;
  }

  bool bRes =true;
  TW_ONEVALUE conVal;
  conVal.ItemType = pCapCon->unItemType;
  conVal.Item =pCapCon->dwVal[0];
  if(typeid(*pCap) == typeid(CTWAINContainerBool))
  {
    bRes = validateCapabilitySet(_unCapID,TWON_ONEVALUE,(BYTE*)&conVal)!=TWRC_FAILURE;
    if(bRes)
    {
      CTWAINContainerBool *pfBoolCap = (CTWAINContainerBool*)pCap;
      bRes = pfBoolCap->SetCurrent(pCapCon->dwVal[0]!=0);
    }
  }
  else if(typeid(*pCap) == typeid(CTWAINContainerInt))
  {
    bRes = validateCapabilitySet(_unCapID,TWON_ONEVALUE,(BYTE*)&conVal)!=TWRC_FAILURE;
    if(bRes)
    {
      CTWAINContainerInt *pfIntCap = (CTWAINContainerInt*)pCap;
      bRes = pfIntCap->SetCurrent(pCapCon->dwVal[0]);
    }
  } 
  else if(typeid(*pCap) == typeid(CTWAINContainerFix32))
  {
    bRes = validateCapabilitySet(_unCapID,TWON_ONEVALUE,(BYTE*)&conVal)!=TWRC_FAILURE;
    if(bRes)
    {
      CTWAINContainerFix32 *pfFix32Cap = (CTWAINContainerFix32*)pCap;
      bRes = pfFix32Cap->SetCurrent(*(float*)pCapCon->dwVal);
    }
  } 
  else if(typeid(*pCap) == typeid(CTWAINContainerFix32Range))
  {
    bRes = validateCapabilitySet(_unCapID,TWON_ONEVALUE,(BYTE*)&conVal)!=TWRC_FAILURE;
    if(bRes)
    {
      CTWAINContainerFix32Range *pfFix32Cap = (CTWAINContainerFix32Range*)pCap;
      bRes=pfFix32Cap->SetCurrent(*(float*)pCapCon->dwVal);
    }
  } 
  else if(typeid(*pCap) == typeid(CTWAINContainerFrame))
  {
    InternalFrame frmVal;
    frmVal.nBottom = pCapCon->dwVal[0];
    frmVal.nLeft   = pCapCon->dwVal[1];
    frmVal.nRight  = pCapCon->dwVal[2];
    frmVal.nTop    = pCapCon->dwVal[3];
    ConstrainFrameToScanner(frmVal);
    CTWAINContainerFrame *pfFrameCap = (CTWAINContainerFrame*)pCap;
    bRes = pfFrameCap->Set(frmVal);
  } 
  else
  {
    bRes = false;
  }

  delete []pCapCon;
  return bRes;
}

bool CTWAINDS_UDS::StoreCustomDSdata(stringstream &DsData)
{//::MessageBox(g_hwndDLG,TEXT("StoreCustomDSdata"),MB_CAPTION,MB_OK);
  bool bResult = true;
  bResult = bResult && StoreCapInStream(DsData,CAP_FEEDERENABLED,0,TWON_ONEVALUE); //扫描模式
  bResult = bResult && StoreCapInStream(DsData,CAP_DUPLEXENABLED,0,TWON_ONEVALUE); //单双面
  bResult = bResult && StoreCapInStream(DsData,CAP_AUTOFEED,0,TWON_ONEVALUE);
  bResult = bResult && StoreCapInStream(DsData,ICAP_BITDEPTH,0,TWON_ONEVALUE);
  bResult = bResult && StoreCapInStream(DsData,ICAP_BITORDER,0,TWON_ONEVALUE);
  bResult = bResult && StoreCapInStream(DsData,ICAP_COMPRESSION,0,TWON_ONEVALUE);
	bResult = bResult && StoreCapInStream(DsData,UDSCAP_COMPRESSVALUE,0,TWON_ONEVALUE);
  bResult = bResult && StoreCapInStream(DsData,ICAP_FRAMES,0,TWON_ONEVALUE);
  bResult = bResult && StoreCapInStream(DsData,ICAP_IMAGEFILEFORMAT,0,TWON_ONEVALUE);
  bResult = bResult && StoreCapInStream(DsData,ICAP_PIXELFLAVOR,0,TWON_ONEVALUE); 
  bResult = bResult && StoreCapInStream(DsData,ICAP_PIXELTYPE,0,TWON_ONEVALUE); //图形类型
  bResult = bResult && StoreCapInStream(DsData,ICAP_PLANARCHUNKY,0,TWON_ONEVALUE);
  bResult = bResult && StoreCapInStream(DsData,ICAP_SUPPORTEDSIZES,0,TWON_ONEVALUE); //纸张大小
  //bResult = bResult && StoreCapInStream(DsData,ICAP_ORIENTATION,0,TWON_ONEVALUE); //纸张方向
  bResult = bResult && StoreCapInStream(DsData,ICAP_UNITS,0,TWON_ONEVALUE); //单位
  bResult = bResult && StoreCapInStream(DsData,ICAP_XRESOLUTION,0,TWON_ONEVALUE);
  bResult = bResult && StoreCapInStream(DsData,ICAP_YRESOLUTION,0,TWON_ONEVALUE); //分辨率
  bResult = bResult && StoreCapInStream(DsData,ICAP_THRESHOLD,0,TWON_ONEVALUE); //阈值
  bResult = bResult && StoreCapInStream(DsData,ICAP_CONTRAST,0,TWON_ONEVALUE); //对比度
  bResult = bResult && StoreCapInStream(DsData,ICAP_BRIGHTNESS,0,TWON_ONEVALUE); //亮度
  bResult = bResult && StoreCapInStream(DsData,ICAP_GAMMA,0,TWON_ONEVALUE); //Gamma图像校正
  bResult = bResult && StoreCapInStream(DsData,UDSCAP_LONGDOCUMENT,0,TWON_ONEVALUE); //长纸型

	bResult = bResult && StoreCapInStream(DsData,UDSCAP_MULTIFEEDDETECT,0,TWON_ONEVALUE); //zhu  重张进纸检测
	bResult = bResult && StoreCapInStream(DsData,UDSCAP_MULTIFEEDDETECT_VALUE,0,TWON_ONEVALUE); //zhu  重张进纸检测

	bResult = bResult && StoreCapInStream(DsData,ICAP_ROTATION,0,TWON_ONEVALUE); //zhu 旋转
	bResult = bResult && StoreCapInStream(DsData,UDSCAP_BINARIZATION,0,TWON_ONEVALUE); //zhu 二值化
	bResult = bResult && StoreCapInStream(DsData,UDSCAP_SPLITIMAGE,0,TWON_ONEVALUE); //zhu  分割图像

	bResult = bResult && StoreCapInStream(DsData,UDSCAP_MULTISTREAM,0,TWON_ONEVALUE); //多流输出
	
	bResult = bResult && StoreCapInStream(DsData,UDSCAP_REMOVESPOTS,0,TWON_ONEVALUE); //去除斑点
	
	bResult = bResult && StoreCapInStream(DsData,ICAP_AUTODISCARDBLANKPAGES,0,TWON_ONEVALUE); //zhu  去除空白页
	bResult = bResult && StoreCapInStream(DsData,UDSCAP_REMOVEBLANK,0,TWON_ONEVALUE); //zhu  去除空白页的checkBox
	bResult = bResult && StoreCapInStream(DsData,UDSCAP_PUNCHHOLEREMOVEL,0,TWON_ONEVALUE); //zhu  去除穿孔
	bResult = bResult && StoreCapInStream(DsData,UDSCAP_SHARPEN,0,TWON_ONEVALUE); //zhu  锐化图像
	bResult = bResult && StoreCapInStream(DsData,UDSCAP_MIRROR,0,TWON_ONEVALUE); //zhu  图像镜像处理
	bResult = bResult && StoreCapInStream(DsData,UDSCAP_REMOVEBACKGROUND,0,TWON_ONEVALUE); //zhu  去除背景
	bResult = bResult && StoreCapInStream(DsData,UDSCAP_DESCREEN,0,TWON_ONEVALUE); //zhu  去除网纹
	bResult = bResult && StoreCapInStream(DsData,UDSCAP_DENOISE,0,TWON_ONEVALUE); //zhu  去除噪声
	bResult = bResult && StoreCapInStream(DsData,UDSCAP_AUTOCROP,0,TWON_ONEVALUE); //zhu  自动裁切及校正 
	bResult = bResult && StoreCapInStream(DsData,UDSCAP_DOCS_IN_ADF,0,TWON_ONEVALUE); // ADF纸张数 
	bResult = bResult && StoreCapInStream(DsData,UDSCAP_MULTISTREAM_VALUE,0,TWON_ONEVALUE); // 多流输出选项值

	bResult = bResult && StoreCapInStream(DsData,UDSCAP_EDGE_UP,0,TWON_ONEVALUE); // 边缘扩充
	bResult = bResult && StoreCapInStream(DsData,UDSCAP_EDGE_DOWN,0,TWON_ONEVALUE); // 边缘扩充
	bResult = bResult && StoreCapInStream(DsData,UDSCAP_EDGE_LEFT,0,TWON_ONEVALUE); // 边缘扩充
	bResult = bResult && StoreCapInStream(DsData,UDSCAP_EDGE_RIGHT,0,TWON_ONEVALUE); // 边缘扩充

	bResult = bResult && StoreCapInStream(DsData,UDSCAP_XPOS,0,TWON_ONEVALUE);
	bResult = bResult && StoreCapInStream(DsData,UDSCAP_YPOS,0,TWON_ONEVALUE);

	bResult = bResult && StoreCapInStream(DsData,UDSCAP_COLORFLIP,0,TWON_ONEVALUE); //色彩翻转
	bResult = bResult && StoreCapInStream(DsData,UDSCAP_POWERSAVING,0,TWON_ONEVALUE); //节电模式
	bResult = bResult && StoreCapInStream(DsData,UDSCAP_POWERSAVING_TIME,0,TWON_ONEVALUE);
	bResult = bResult && StoreCapInStream(DsData,UDSCAP_POWEROFF,0,TWON_ONEVALUE);
	bResult = bResult && StoreCapInStream(DsData,UDSCAP_POWEROFF_TIME,0,TWON_ONEVALUE);
  bResult = bResult && StoreCapInStream(DsData,UDSCAP_TURNVIDEO,0,TWON_ONEVALUE); //高拍转高扫
	bResult = bResult && StoreCapInStream(DsData,UDSCAP_CUTMETHOD,0,TWON_ONEVALUE);  //图像裁切方式

	bResult = bResult && StoreCapInStream(DsData,UDSCAP_NOISENUM,0,TWON_ONEVALUE);  //去除噪声-噪声数目
	bResult = bResult && StoreCapInStream(DsData,UDSCAP_NOISERANGE,0,TWON_ONEVALUE);  //去除噪声-噪声范围

	bResult = bResult && StoreCapInStream(DsData,UDSCAP_FILTERCOLOR,0,TWON_ONEVALUE); //滤除颜色
	bResult = bResult && StoreCapInStream(DsData,UDSCAP_FILTERMODE,0,TWON_ONEVALUE); //滤除模式
	bResult = bResult && StoreCapInStream(DsData,UDSCAP_FILTERLEVEL,0,TWON_ONEVALUE); //滤除程度

	//多流亮度
	bResult = bResult && StoreCapInStream(DsData,UDSCAP_BRIGHTNESSFC,0,TWON_ONEVALUE);
	bResult = bResult && StoreCapInStream(DsData,UDSCAP_BRIGHTNESSFG,0,TWON_ONEVALUE);
	bResult = bResult && StoreCapInStream(DsData,UDSCAP_BRIGHTNESSFB,0,TWON_ONEVALUE);
	bResult = bResult && StoreCapInStream(DsData,UDSCAP_BRIGHTNESSBC,0,TWON_ONEVALUE);
	bResult = bResult && StoreCapInStream(DsData,UDSCAP_BRIGHTNESSBG,0,TWON_ONEVALUE);
	bResult = bResult && StoreCapInStream(DsData,UDSCAP_BRIGHTNESSBB,0,TWON_ONEVALUE);
	//多流分辨率
	bResult = bResult && StoreCapInStream(DsData,UDSCAP_RESOLUTIONFC,0,TWON_ONEVALUE);
	bResult = bResult && StoreCapInStream(DsData,UDSCAP_RESOLUTIONFG,0,TWON_ONEVALUE);
	bResult = bResult && StoreCapInStream(DsData,UDSCAP_RESOLUTIONFB,0,TWON_ONEVALUE);
	bResult = bResult && StoreCapInStream(DsData,UDSCAP_RESOLUTIONBC,0,TWON_ONEVALUE);
	bResult = bResult && StoreCapInStream(DsData,UDSCAP_RESOLUTIONBG,0,TWON_ONEVALUE);
	bResult = bResult && StoreCapInStream(DsData,UDSCAP_RESOLUTIONBB,0,TWON_ONEVALUE);
	//多流对比度
	bResult = bResult && StoreCapInStream(DsData,UDSCAP_CONTRASTFC,0,TWON_ONEVALUE);
	bResult = bResult && StoreCapInStream(DsData,UDSCAP_CONTRASTFG,0,TWON_ONEVALUE);
	bResult = bResult && StoreCapInStream(DsData,UDSCAP_CONTRASTBC,0,TWON_ONEVALUE);
	bResult = bResult && StoreCapInStream(DsData,UDSCAP_CONTRASTBG,0,TWON_ONEVALUE);
	//多流压缩
	bResult = bResult && StoreCapInStream(DsData,UDSCAP_COMPRESSIONFC,0,TWON_ONEVALUE);
	bResult = bResult && StoreCapInStream(DsData,UDSCAP_COMPRESSIONFG,0,TWON_ONEVALUE);
	bResult = bResult && StoreCapInStream(DsData,UDSCAP_COMPRESSIONFB,0,TWON_ONEVALUE);
	bResult = bResult && StoreCapInStream(DsData,UDSCAP_COMPRESSIONBC,0,TWON_ONEVALUE);
	bResult = bResult && StoreCapInStream(DsData,UDSCAP_COMPRESSIONBG,0,TWON_ONEVALUE);
	bResult = bResult && StoreCapInStream(DsData,UDSCAP_COMPRESSIONBB,0,TWON_ONEVALUE);
	//多流压缩比的值
	bResult = bResult && StoreCapInStream(DsData,UDSCAP_COMPRESSIONVALUEFC,0,TWON_ONEVALUE);
	bResult = bResult && StoreCapInStream(DsData,UDSCAP_COMPRESSIONVALUEFG,0,TWON_ONEVALUE);
	bResult = bResult && StoreCapInStream(DsData,UDSCAP_COMPRESSIONVALUEFB,0,TWON_ONEVALUE);
	bResult = bResult && StoreCapInStream(DsData,UDSCAP_COMPRESSIONVALUEBC,0,TWON_ONEVALUE);
	bResult = bResult && StoreCapInStream(DsData,UDSCAP_COMPRESSIONVALUEBG,0,TWON_ONEVALUE);
	bResult = bResult && StoreCapInStream(DsData,UDSCAP_COMPRESSIONVALUEBB,0,TWON_ONEVALUE);
	//多流二值化
	bResult = bResult && StoreCapInStream(DsData,UDSCAP_BINARIZATIONFB,0,TWON_ONEVALUE);
	bResult = bResult && StoreCapInStream(DsData,UDSCAP_BINARIZATIONBB,0,TWON_ONEVALUE);
	//多流阈值
	bResult = bResult && StoreCapInStream(DsData,UDSCAP_THRESHOLDFB,0,TWON_ONEVALUE);
	bResult = bResult && StoreCapInStream(DsData,UDSCAP_THRESHOLDBB,0,TWON_ONEVALUE);
	//多流去除斑点
	bResult = bResult && StoreCapInStream(DsData,UDSCAP_REMOVESPOTSFB,0,TWON_ONEVALUE);
	bResult = bResult && StoreCapInStream(DsData,UDSCAP_REMOVESPOTSBB,0,TWON_ONEVALUE);
	return bResult;
}

bool CTWAINDS_UDS::ReadCustomDSdata(stringstream &DsData)
{//::MessageBox(g_hwndDLG,TEXT("ReadCustomDSdata"),MB_CAPTION,MB_OK);
  // When adding to Capabiltiy remember the order of operations 
  // Some capabilities are dependent on others.
  // see: http://www.twain.org/docs/CapOrderForWeb.PDF
  bool bResult = true;
  bResult = bResult && ReadCapFromStream(DsData,UDSCAP_LONGDOCUMENT,0);
  bResult = bResult && ReadCapFromStream(DsData,ICAP_BITORDER,0);
  bResult = bResult && ReadCapFromStream(DsData,CAP_FEEDERENABLED,0);
  bResult = bResult && ReadCapFromStream(DsData,CAP_DUPLEXENABLED,0);
  bResult = bResult && ReadCapFromStream(DsData,CAP_AUTOFEED,0);
  bResult = bResult && ReadCapFromStream(DsData,ICAP_UNITS,0);
  bResult = bResult && ReadCapFromStream(DsData,ICAP_PIXELTYPE,0);
  bResult = bResult && ReadCapFromStream(DsData,ICAP_BITDEPTH,0);
  bResult = bResult && ReadCapFromStream(DsData,ICAP_XRESOLUTION,0);
  bResult = bResult && ReadCapFromStream(DsData,ICAP_YRESOLUTION,0);
  bResult = bResult && ReadCapFromStream(DsData,ICAP_PIXELFLAVOR,0);
  bResult = bResult && ReadCapFromStream(DsData,ICAP_PLANARCHUNKY,0);
  bResult = bResult && ReadCapFromStream(DsData,ICAP_THRESHOLD,0);
  bResult = bResult && ReadCapFromStream(DsData,ICAP_IMAGEFILEFORMAT,0);
  bResult = bResult && ReadCapFromStream(DsData,ICAP_COMPRESSION,0);
	bResult = bResult && ReadCapFromStream(DsData,UDSCAP_COMPRESSVALUE,0);
  bResult = bResult && ReadCapFromStream(DsData,ICAP_CONTRAST,0);
  bResult = bResult && ReadCapFromStream(DsData,ICAP_BRIGHTNESS,0);
  bResult = bResult && ReadCapFromStream(DsData,ICAP_GAMMA,0);  //gamma校正 
  bResult = bResult && ReadCapFromStream(DsData,ICAP_SUPPORTEDSIZES,0);
  //bResult = bResult && ReadCapFromStream(DsData,ICAP_ORIENTATION,0);
  bResult = bResult && ReadCapFromStream(DsData,ICAP_FRAMES,0);
	bResult = bResult && ReadCapFromStream(DsData,ICAP_ROTATION,0); //zhu
	bResult = bResult && ReadCapFromStream(DsData,UDSCAP_BINARIZATION,0); //zhu 
	bResult = bResult && ReadCapFromStream(DsData,UDSCAP_SPLITIMAGE,0); //zhu
	bResult = bResult && ReadCapFromStream(DsData,UDSCAP_MULTISTREAM,0); //zhu 多流输出
	bResult = bResult && ReadCapFromStream(DsData,UDSCAP_REMOVESPOTS,0); //zhu 去除斑点
	bResult = bResult && ReadCapFromStream(DsData,ICAP_AUTODISCARDBLANKPAGES,0);
	bResult = bResult && ReadCapFromStream(DsData,UDSCAP_REMOVEBLANK,0);
	bResult = bResult && ReadCapFromStream(DsData,UDSCAP_PUNCHHOLEREMOVEL,0);
	bResult = bResult && ReadCapFromStream(DsData,UDSCAP_SHARPEN,0);
	bResult = bResult && ReadCapFromStream(DsData,UDSCAP_MIRROR,0);
	bResult = bResult && ReadCapFromStream(DsData,UDSCAP_REMOVEBACKGROUND,0);
	bResult = bResult && ReadCapFromStream(DsData,UDSCAP_DESCREEN,0);
	bResult = bResult && ReadCapFromStream(DsData,UDSCAP_DENOISE,0);
	bResult = bResult && ReadCapFromStream(DsData,UDSCAP_AUTOCROP,0);
	bResult = bResult && ReadCapFromStream(DsData,UDSCAP_MULTIFEEDDETECT,0);
	bResult = bResult && ReadCapFromStream(DsData,UDSCAP_MULTIFEEDDETECT_VALUE,0);
	bResult = bResult && ReadCapFromStream(DsData,UDSCAP_DOCS_IN_ADF,0); // ADF纸张数 
	bResult = bResult && ReadCapFromStream(DsData,UDSCAP_MULTISTREAM_VALUE,0); // 多流输出选项值 

	bResult = bResult && ReadCapFromStream(DsData,UDSCAP_EDGE_UP,0); 
	bResult = bResult && ReadCapFromStream(DsData,UDSCAP_EDGE_DOWN,0); 
	bResult = bResult && ReadCapFromStream(DsData,UDSCAP_EDGE_LEFT,0); 
	bResult = bResult && ReadCapFromStream(DsData,UDSCAP_EDGE_RIGHT,0); 

	bResult = bResult && ReadCapFromStream(DsData,UDSCAP_XPOS,0); //X偏移量
	bResult = bResult && ReadCapFromStream(DsData,UDSCAP_YPOS,0); 

	bResult = bResult && ReadCapFromStream(DsData,UDSCAP_COLORFLIP,0);  //色彩翻转
	bResult = bResult && ReadCapFromStream(DsData,UDSCAP_POWERSAVING,0); //节电模式
	bResult = bResult && ReadCapFromStream(DsData,UDSCAP_POWERSAVING_TIME,0);
	bResult = bResult && ReadCapFromStream(DsData,UDSCAP_POWEROFF,0);
	bResult = bResult && ReadCapFromStream(DsData,UDSCAP_POWEROFF_TIME,0);
  bResult = bResult && ReadCapFromStream(DsData,UDSCAP_TURNVIDEO,0);
	bResult = bResult && ReadCapFromStream(DsData,UDSCAP_CUTMETHOD,0);

	bResult = bResult && ReadCapFromStream(DsData,UDSCAP_NOISENUM,0);  //去除噪声-噪声数目
	bResult = bResult && ReadCapFromStream(DsData,UDSCAP_NOISERANGE,0); //去除噪声-噪声范围

	bResult = bResult && ReadCapFromStream(DsData,UDSCAP_FILTERCOLOR,0);
	bResult = bResult && ReadCapFromStream(DsData,UDSCAP_FILTERMODE,0);
	bResult = bResult && ReadCapFromStream(DsData,UDSCAP_FILTERLEVEL,0);
	//多流亮度
	bResult = bResult && ReadCapFromStream(DsData,UDSCAP_BRIGHTNESSFC,0);
	bResult = bResult && ReadCapFromStream(DsData,UDSCAP_BRIGHTNESSFG,0);  
	bResult = bResult && ReadCapFromStream(DsData,UDSCAP_BRIGHTNESSFB,0); 
	bResult = bResult && ReadCapFromStream(DsData,UDSCAP_BRIGHTNESSBC,0);
	bResult = bResult && ReadCapFromStream(DsData,UDSCAP_BRIGHTNESSBG,0);
	bResult = bResult && ReadCapFromStream(DsData,UDSCAP_BRIGHTNESSBB,0);
	//多流分辨率
	bResult = bResult && ReadCapFromStream(DsData,UDSCAP_RESOLUTIONFC,0);
	bResult = bResult && ReadCapFromStream(DsData,UDSCAP_RESOLUTIONFG,0);  
	bResult = bResult && ReadCapFromStream(DsData,UDSCAP_RESOLUTIONFB,0); 
	bResult = bResult && ReadCapFromStream(DsData,UDSCAP_RESOLUTIONBC,0);
	bResult = bResult && ReadCapFromStream(DsData,UDSCAP_RESOLUTIONBG,0);
	bResult = bResult && ReadCapFromStream(DsData,UDSCAP_RESOLUTIONBB,0);
	//多流对比度
	bResult = bResult && ReadCapFromStream(DsData,UDSCAP_CONTRASTFC,0);
	bResult = bResult && ReadCapFromStream(DsData,UDSCAP_CONTRASTFG,0);  
	bResult = bResult && ReadCapFromStream(DsData,UDSCAP_CONTRASTBC,0); 
	bResult = bResult && ReadCapFromStream(DsData,UDSCAP_CONTRASTBG,0);
	//多流压缩
	bResult = bResult && ReadCapFromStream(DsData,UDSCAP_COMPRESSIONFC,0);
	bResult = bResult && ReadCapFromStream(DsData,UDSCAP_COMPRESSIONFG,0);  
	bResult = bResult && ReadCapFromStream(DsData,UDSCAP_COMPRESSIONFB,0); 
	bResult = bResult && ReadCapFromStream(DsData,UDSCAP_COMPRESSIONBC,0);
	bResult = bResult && ReadCapFromStream(DsData,UDSCAP_COMPRESSIONBG,0);
	bResult = bResult && ReadCapFromStream(DsData,UDSCAP_COMPRESSIONBB,0);
	//多流压缩比的值
	bResult = bResult && ReadCapFromStream(DsData,UDSCAP_COMPRESSIONVALUEFC,0);
	bResult = bResult && ReadCapFromStream(DsData,UDSCAP_COMPRESSIONVALUEFG,0);  
	bResult = bResult && ReadCapFromStream(DsData,UDSCAP_COMPRESSIONVALUEFB,0); 
	bResult = bResult && ReadCapFromStream(DsData,UDSCAP_COMPRESSIONVALUEBC,0);
	bResult = bResult && ReadCapFromStream(DsData,UDSCAP_COMPRESSIONVALUEBG,0);
	bResult = bResult && ReadCapFromStream(DsData,UDSCAP_COMPRESSIONVALUEBB,0);
	//多流二值化
	bResult = bResult && ReadCapFromStream(DsData,UDSCAP_BINARIZATIONFB,0);
	bResult = bResult && ReadCapFromStream(DsData,UDSCAP_BINARIZATIONBB,0);  
	//多流阈值
	bResult = bResult && ReadCapFromStream(DsData,UDSCAP_THRESHOLDFB,0); 
	bResult = bResult && ReadCapFromStream(DsData,UDSCAP_THRESHOLDBB,0);
	//多流去除斑点
	bResult = bResult && ReadCapFromStream(DsData,UDSCAP_REMOVESPOTSFB,0);
	bResult = bResult && ReadCapFromStream(DsData,UDSCAP_REMOVESPOTSBB,0);
  return bResult;
}

//////////////////////////////////////////////////////////////////////////////
TW_INT16 CTWAINDS_UDS::Initialize()
{ //::MessageBox(g_hwndDLG,TEXT("CTWAINDS_UDS::Initialize"),MB_CAPTION,MB_OK);
  // When adding to Capabiltiy remember the order of operations 
  // Some capabilities are dependent on others.
  // see: http://www.twain.org/docs/CapOrderForWeb.PDF

  // setup the supported independant caps
  CTWAINContainerInt* pnCap = 0;
  CTWAINContainerBool* pbCap = 0;
  CTWAINContainerString* pstrCap = 0;
  CTWAINContainerFix32* pfixCap = 0;
	FLOAT_RANGE fRange;

  m_IndependantCapMap[CAP_SUPPORTEDCAPS] = new CTWAINContainerInt(CAP_SUPPORTEDCAPS, TWTY_UINT16, TWON_ARRAY, TWQC_GETS);
  if( NULL == (pnCap = dynamic_cast<CTWAINContainerInt*>(m_IndependantCapMap[CAP_SUPPORTEDCAPS]))
   || !pnCap->Add(CAP_DEVICEONLINE)   //
   || !pnCap->Add(CAP_INDICATORS)  //指示灯
   || !pnCap->Add(CAP_ENABLEDSUIONLY)  //打开驱动界面
   || !pnCap->Add(CAP_PAPERDETECTABLE)  //纸张检测
   || !pnCap->Add(CAP_FEEDERENABLED)  //启动进纸，扫描模式
   || !pnCap->Add(CAP_FEEDERLOADED)  //加载进纸
   || !pnCap->Add(CAP_DUPLEX)  //单双面
   || !pnCap->Add(CAP_DUPLEXENABLED) //启动单双面
   || !pnCap->Add(CAP_AUTOFEED)  //自动机制
   || !pnCap->Add(CAP_SUPPORTEDCAPS)
   || !pnCap->Add(CAP_UICONTROLLABLE)
   || !pnCap->Add(CAP_XFERCOUNT)
   || !pnCap->Add(ICAP_BITDEPTH)
   || !pnCap->Add(ICAP_BITORDER)
   || !pnCap->Add(ICAP_COMPRESSION)  //压缩
	 || !pnCap->Add(UDSCAP_COMPRESSVALUE) //压缩比的值
   || !pnCap->Add(ICAP_FRAMES)
   || !pnCap->Add(ICAP_MAXFRAMES)
   || !pnCap->Add(ICAP_IMAGEFILEFORMAT)
   || !pnCap->Add(ICAP_PHYSICALHEIGHT)
   || !pnCap->Add(ICAP_PHYSICALWIDTH)
   || !pnCap->Add(ICAP_PIXELFLAVOR)
   || !pnCap->Add(ICAP_PIXELTYPE)    //图形类型
   || !pnCap->Add(ICAP_PLANARCHUNKY)
   || !pnCap->Add(ICAP_SUPPORTEDSIZES)  //纸张大小
	 || !pnCap->Add(UDSCAP_MULTISTREAM)  //多流输出
	 || !pnCap->Add(UDSCAP_REMOVESPOTS)  //去除斑点
   || !pnCap->Add(ICAP_UNITS) //单位
   || !pnCap->Add(ICAP_XFERMECH)
   || !pnCap->Add(ICAP_XRESOLUTION) //分辨率
   || !pnCap->Add(ICAP_YRESOLUTION) 
   || !pnCap->Add(ICAP_THRESHOLD) //阈值 
   || !pnCap->Add(ICAP_CONTRAST)  //对比度
   || !pnCap->Add(ICAP_BRIGHTNESS)  //亮度
   || !pnCap->Add(ICAP_GAMMA) //Gamma校正
   || !pnCap->Add(CAP_CUSTOMINTERFACEGUID)
   || !pnCap->Add(UDSCAP_LONGDOCUMENT)  //长纸
   || !pnCap->Add(UDSCAP_DOCS_IN_ADF) 
   || !pnCap->Add(CAP_CUSTOMDSDATA) 
	 || !pnCap->Add(ICAP_ROTATION) //图像旋转 zhu
	 || !pnCap->Add(UDSCAP_BINARIZATION) // 二值化zhu
	 || !pnCap->Add(UDSCAP_SPLITIMAGE) //分割图像zhu
	 || !pnCap->Add(ICAP_AUTODISCARDBLANKPAGES) //去除空白页
	 || !pnCap->Add(UDSCAP_REMOVEBLANK)
	 || !pnCap->Add(UDSCAP_PUNCHHOLEREMOVEL)
	 || !pnCap->Add(UDSCAP_SHARPEN)
	 || !pnCap->Add(UDSCAP_MIRROR)
	 || !pnCap->Add(UDSCAP_REMOVEBACKGROUND)
	 || !pnCap->Add(UDSCAP_DESCREEN)
	 || !pnCap->Add(UDSCAP_DENOISE)
	 || !pnCap->Add(UDSCAP_AUTOCROP)
	 || !pnCap->Add(UDSCAP_MULTIFEEDDETECT) //多张进纸检测
	 || !pnCap->Add(UDSCAP_MULTIFEEDDETECT_VALUE) //多张进纸故障值
	 || !pnCap->Add(UDSCAP_MULTISTREAM_VALUE) // 多流输出选项值
	 || !pnCap->Add(UDSCAP_EDGE_UP)
	 || !pnCap->Add(UDSCAP_EDGE_DOWN)
	 || !pnCap->Add(UDSCAP_EDGE_LEFT)
	 || !pnCap->Add(UDSCAP_EDGE_RIGHT)
	 || !pnCap->Add(UDSCAP_XPOS)
	 || !pnCap->Add(UDSCAP_YPOS)
	 || !pnCap->Add(UDSCAP_COLORFLIP)
	 || !pnCap->Add(UDSCAP_POWERSAVING)
	 || !pnCap->Add(UDSCAP_POWERSAVING_TIME)
	 || !pnCap->Add(UDSCAP_POWEROFF)
	 || !pnCap->Add(UDSCAP_POWEROFF_TIME)
   || !pnCap->Add(UDSCAP_TURNVIDEO)	 
	 || !pnCap->Add(UDSCAP_CUTMETHOD)	 
	 || !pnCap->Add(UDSCAP_NOISENUM)	
	 || !pnCap->Add(UDSCAP_NOISERANGE)
	 || !pnCap->Add(UDSCAP_FILTERCOLOR)	 
	 || !pnCap->Add(UDSCAP_FILTERMODE)	
	 || !pnCap->Add(UDSCAP_FILTERLEVEL)	
	 //多流亮度
	 || !pnCap->Add(UDSCAP_BRIGHTNESSFC)
	 || !pnCap->Add(UDSCAP_BRIGHTNESSFG)
	 || !pnCap->Add(UDSCAP_BRIGHTNESSFB)
	 || !pnCap->Add(UDSCAP_BRIGHTNESSBC)
	 || !pnCap->Add(UDSCAP_BRIGHTNESSBG)
	 || !pnCap->Add(UDSCAP_BRIGHTNESSBB)
	 //多流分辨率
	 || !pnCap->Add(UDSCAP_RESOLUTIONFC)
	 || !pnCap->Add(UDSCAP_RESOLUTIONFG)
	 || !pnCap->Add(UDSCAP_RESOLUTIONFB)
	 || !pnCap->Add(UDSCAP_RESOLUTIONBC)
	 || !pnCap->Add(UDSCAP_RESOLUTIONBG)
	 || !pnCap->Add(UDSCAP_RESOLUTIONBB)
	 //多流对比度
	 || !pnCap->Add(UDSCAP_CONTRASTFC)
	 || !pnCap->Add(UDSCAP_CONTRASTFG)
	 || !pnCap->Add(UDSCAP_CONTRASTBC)
	 || !pnCap->Add(UDSCAP_CONTRASTBG)
	 //多流压缩
	 || !pnCap->Add(UDSCAP_COMPRESSIONFC)
	 || !pnCap->Add(UDSCAP_COMPRESSIONFG)
	 || !pnCap->Add(UDSCAP_COMPRESSIONFB)
	 || !pnCap->Add(UDSCAP_COMPRESSIONBC)
	 || !pnCap->Add(UDSCAP_COMPRESSIONBG)
	 || !pnCap->Add(UDSCAP_COMPRESSIONBB)
	 //多流压缩比的值
	 || !pnCap->Add(UDSCAP_COMPRESSIONVALUEFC)
	 || !pnCap->Add(UDSCAP_COMPRESSIONVALUEFG)
	 || !pnCap->Add(UDSCAP_COMPRESSIONVALUEFB)
	 || !pnCap->Add(UDSCAP_COMPRESSIONVALUEBC)
	 || !pnCap->Add(UDSCAP_COMPRESSIONVALUEBG)
	 || !pnCap->Add(UDSCAP_COMPRESSIONVALUEBB)
	 //多流二值化
	 || !pnCap->Add(UDSCAP_BINARIZATIONFB)
	 || !pnCap->Add(UDSCAP_BINARIZATIONBB)
	  //多流阈值
	 || !pnCap->Add(UDSCAP_THRESHOLDFB)
	 || !pnCap->Add(UDSCAP_THRESHOLDBB)
	  //多流去除斑点
	 || !pnCap->Add(UDSCAP_REMOVESPOTSFB)
	 || !pnCap->Add(UDSCAP_REMOVESPOTSBB)
   )
  {
		::MessageBox(g_hwndDLG,TEXT("Could not create CAP_SUPPORTEDCAPS !"),MB_CAPTION,MB_OK);
    //cerr << "Could not create CAP_SUPPORTEDCAPS" << endl;
    setConditionCode(TWCC_LOWMEMORY);
    return TWRC_FAILURE;
  }

	//扫描仪无纸时转高拍仪
	m_IndependantCapMap[UDSCAP_TURNVIDEO] = new CTWAINContainerBool(UDSCAP_TURNVIDEO, (m_AppID.SupportedGroups&DF_APP2)!=0, TWQC_ALL);
	if( NULL == (pbCap = dynamic_cast<CTWAINContainerBool*>(m_IndependantCapMap[UDSCAP_TURNVIDEO]))
		|| !pbCap->Add(FALSE)
		|| !pbCap->Add(TRUE, true) )
	{
		::MessageBox(g_hwndDLG,TEXT("Could not create UDSCAP_TURNVIDEO !"),MB_CAPTION,MB_OK);
		setConditionCode(TWCC_LOWMEMORY);
		return TWRC_FAILURE;
	}

	//滤除颜色
	m_IndependantCapMap[UDSCAP_FILTERCOLOR] = new CTWAINContainerInt(UDSCAP_FILTERCOLOR, TWTY_UINT16, TWON_ENUMERATION);
	if( NULL == (pnCap = dynamic_cast<CTWAINContainerInt*>(m_IndependantCapMap[UDSCAP_FILTERCOLOR]))
		|| !pnCap->Add(TWFL_NONE, true)
		|| !pnCap->Add(TWFL_RED)
		|| !pnCap->Add(TWFL_GREEN)
		|| !pnCap->Add(TWFL_BLUE)
		|| !pnCap->Add(TWFL_AUTO))
	{
		::MessageBox(g_hwndDLG,TEXT("Could not create UDSCAP_FILTERCOLOR !"),MB_CAPTION,MB_OK);
		setConditionCode(TWCC_LOWMEMORY);
		return TWRC_FAILURE;
	}

	//滤除模式
	m_IndependantCapMap[UDSCAP_FILTERMODE] = new CTWAINContainerInt(UDSCAP_FILTERMODE, TWTY_UINT16, TWON_ENUMERATION);
	if( NULL == (pnCap = dynamic_cast<CTWAINContainerInt*>(m_IndependantCapMap[UDSCAP_FILTERMODE]))
		|| !pnCap->Add(TWFM_AUTO, true)
		|| !pnCap->Add(TWFM_DEFINED))
	{
		::MessageBox(g_hwndDLG,TEXT("Could not create UDSCAP_FILTERMODE !"),MB_CAPTION,MB_OK);
		setConditionCode(TWCC_LOWMEMORY);
		return TWRC_FAILURE;
	}

	//滤除程度
	fRange.fCurrentValue = 10.0f; 
	fRange.fMaxValue = 20.0f;
	fRange.fMinValue = 0.0f;
	fRange.fStepSize = 1.0f;
	//去除噪声-噪声数目
	m_IndependantCapMap[UDSCAP_FILTERLEVEL] = new CTWAINContainerFix32Range(UDSCAP_FILTERLEVEL,fRange, TWQC_ALL);
	if( NULL == dynamic_cast<CTWAINContainerFix32Range*>(m_IndependantCapMap[UDSCAP_FILTERLEVEL]))
	{
		::MessageBox(g_hwndDLG,TEXT("Could not create UDSCAP_FILTERLEVEL !"),MB_CAPTION,MB_OK);
		setConditionCode(TWCC_LOWMEMORY);
		return TWRC_FAILURE;
	}

	fRange.fCurrentValue = 0.0f; 
	fRange.fMaxValue = 3600.0f;
	fRange.fMinValue = 0.0f;
	fRange.fStepSize = 1.0f;
	//去除噪声-噪声数目
	m_IndependantCapMap[UDSCAP_NOISENUM] = new CTWAINContainerFix32Range(UDSCAP_NOISENUM,fRange, TWQC_ALL);
	if( NULL == dynamic_cast<CTWAINContainerFix32Range*>(m_IndependantCapMap[UDSCAP_NOISENUM]))
	{
		::MessageBox(g_hwndDLG,TEXT("Could not create UDSCAP_NOISENUM !"),MB_CAPTION,MB_OK);
		setConditionCode(TWCC_LOWMEMORY);
		return TWRC_FAILURE;
	}

	fRange.fCurrentValue = 1.0f; 
	fRange.fMaxValue = 30.0f;
	fRange.fMinValue = 1.0f;
	fRange.fStepSize = 1.0f;
	//去除噪声-噪声范围
	m_IndependantCapMap[UDSCAP_NOISERANGE] = new CTWAINContainerFix32Range(UDSCAP_NOISERANGE,fRange, TWQC_ALL);
	if( NULL == dynamic_cast<CTWAINContainerFix32Range*>(m_IndependantCapMap[UDSCAP_NOISERANGE]))
	{
		::MessageBox(g_hwndDLG,TEXT("Could not create UDSCAP_NOISERANGE !"),MB_CAPTION,MB_OK);
		setConditionCode(TWCC_LOWMEMORY);
		return TWRC_FAILURE;
	}

	//节电模式
	m_IndependantCapMap[UDSCAP_POWERSAVING] = new CTWAINContainerBool(UDSCAP_POWERSAVING, (m_AppID.SupportedGroups&DF_APP2)!=0, TWQC_ALL);
	if( NULL == (pbCap = dynamic_cast<CTWAINContainerBool*>(m_IndependantCapMap[UDSCAP_POWERSAVING]))
		|| !pbCap->Add(FALSE)
		|| !pbCap->Add(TRUE, true) )
	{
		::MessageBox(g_hwndDLG,TEXT("Could not create UDSCAP_POWERSAVING !"),MB_CAPTION,MB_OK);
		setConditionCode(TWCC_LOWMEMORY);
		return TWRC_FAILURE;
	}

	fRange.fCurrentValue = 15.0f; 
	fRange.fMaxValue = 240.0f;
	fRange.fMinValue = 1.0f;
	fRange.fStepSize = 1.0f;
	//节电模式值
	m_IndependantCapMap[UDSCAP_POWERSAVING_TIME] = new CTWAINContainerFix32Range(UDSCAP_POWERSAVING_TIME,fRange, TWQC_ALL);
	if( NULL == dynamic_cast<CTWAINContainerFix32Range*>(m_IndependantCapMap[UDSCAP_POWERSAVING_TIME]))
	{
		::MessageBox(g_hwndDLG,TEXT("Could not create UDSCAP_POWERSAVING_TIME !"),MB_CAPTION,MB_OK);
		setConditionCode(TWCC_LOWMEMORY);
		return TWRC_FAILURE;
	}

	//关机时间
	m_IndependantCapMap[UDSCAP_POWEROFF] = new CTWAINContainerBool(UDSCAP_POWEROFF, (m_AppID.SupportedGroups&DF_APP2)!=0, TWQC_ALL);
	if( NULL == (pbCap = dynamic_cast<CTWAINContainerBool*>(m_IndependantCapMap[UDSCAP_POWEROFF]))
		|| !pbCap->Add(FALSE)
		|| !pbCap->Add(TRUE, true) )
	{
		::MessageBox(g_hwndDLG,TEXT("Could not create UDSCAP_POWEROFF !"),MB_CAPTION,MB_OK);
		setConditionCode(TWCC_LOWMEMORY);
		return TWRC_FAILURE;
	}

	fRange.fCurrentValue = 240.0f; 
	fRange.fMaxValue = 480.0f;
	fRange.fMinValue = 1.0f;
	fRange.fStepSize = 1.0f;
	//关机时间值
	m_IndependantCapMap[UDSCAP_POWEROFF_TIME] = new CTWAINContainerFix32Range(UDSCAP_POWEROFF_TIME,fRange, TWQC_ALL);
	if( NULL == dynamic_cast<CTWAINContainerFix32Range*>(m_IndependantCapMap[UDSCAP_POWEROFF_TIME]))
	{
		::MessageBox(g_hwndDLG,TEXT("Could not create UDSCAP_POWEROFF_TIME !"),MB_CAPTION,MB_OK);
		setConditionCode(TWCC_LOWMEMORY);
		return TWRC_FAILURE;
	}
	
  //m_IndependantCapMap[ICAP_COMPRESSION] = new CTWAINContainerInt(ICAP_COMPRESSION, TWTY_UINT16, TWON_ENUMERATION);
  //if( NULL == (pnCap = dynamic_cast<CTWAINContainerInt*>(m_IndependantCapMap[ICAP_COMPRESSION]))
  // || !pnCap->Add(TWCP_NONE, true) )
  //{
		//::MessageBox(g_hwndDLG,TEXT("Could not create ICAP_COMPRESSION !"),MB_CAPTION,MB_OK);
  //  //cerr << "Could not create ICAP_COMPRESSION" << endl;
  //  setConditionCode(TWCC_LOWMEMORY);
  //  return TWRC_FAILURE;
  //}

  m_IndependantCapMap[ICAP_PLANARCHUNKY] = new CTWAINContainerInt(ICAP_PLANARCHUNKY, TWTY_UINT16, TWON_ENUMERATION);
  if( NULL == (pnCap = dynamic_cast<CTWAINContainerInt*>(m_IndependantCapMap[ICAP_PLANARCHUNKY]))
   || !pnCap->Add(TWPC_CHUNKY, true) )
  /// @todo support TWPC_PLANAR
  // || !pnCap->Add(TWPC_PLANAR)
  {
		::MessageBox(g_hwndDLG,TEXT("Could not create ICAP_PLANARCHUNKY !"),MB_CAPTION,MB_OK);
    //cerr << "Could not create ICAP_PLANARCHUNKY" << endl;
    setConditionCode(TWCC_LOWMEMORY);
    return TWRC_FAILURE;
  }

  m_IndependantCapMap[ICAP_UNITS] = new CTWAINContainerInt(ICAP_UNITS, TWTY_UINT16, TWON_ENUMERATION);
  if( NULL == (pnCap = dynamic_cast<CTWAINContainerInt*>(m_IndependantCapMap[ICAP_UNITS]))
   || !pnCap->Add(TWUN_INCHES, true)
   || !pnCap->Add(TWUN_PIXELS)
   || !pnCap->Add(TWUN_CENTIMETERS) )
  {
		::MessageBox(g_hwndDLG,TEXT("Could not create ICAP_UNITS !"),MB_CAPTION,MB_OK);
    //cerr << "Could not create ICAP_UNITS" << endl;
    setConditionCode(TWCC_LOWMEMORY);
    return TWRC_FAILURE;
  }

  m_IndependantCapMap[ICAP_XFERMECH] = new CTWAINContainerInt(ICAP_XFERMECH, TWTY_UINT16, TWON_ENUMERATION);
  if( NULL == (pnCap = dynamic_cast<CTWAINContainerInt*>(m_IndependantCapMap[ICAP_XFERMECH]))
   || !pnCap->Add(TWSX_FILE)
   || !pnCap->Add(TWSX_MEMORY)
   || !pnCap->Add(TWSX_NATIVE, true)
   )
  {
		::MessageBox(g_hwndDLG,TEXT("Could not create ICAP_XFERMECH !"),MB_CAPTION,MB_OK);
    //cerr << "Could not create ICAP_XFERMECH" << endl;
    setConditionCode(TWCC_LOWMEMORY);
    return TWRC_FAILURE;
  }

  m_IndependantCapMap[ICAP_PIXELTYPE] = new CTWAINContainerInt(ICAP_PIXELTYPE, TWTY_UINT16, TWON_ENUMERATION);
  if( NULL == (pnCap = dynamic_cast<CTWAINContainerInt*>(m_IndependantCapMap[ICAP_PIXELTYPE]))
   || !pnCap->Add(TWPT_BW)
   || !pnCap->Add(TWPT_GRAY)
   || !pnCap->Add(TWPT_RGB, true)  //图形类型，默认彩色
   ) 
  {
		::MessageBox(g_hwndDLG,TEXT("Could not create ICAP_PIXELTYPE !"),MB_CAPTION,MB_OK);
    //cerr << "Could not create ICAP_PIXELTYPE" << endl;
    setConditionCode(TWCC_LOWMEMORY);
    return TWRC_FAILURE;
  }

  m_IndependantCapMap[CAP_UICONTROLLABLE] = new CTWAINContainerBool(CAP_UICONTROLLABLE, (m_AppID.SupportedGroups&DF_APP2)!=0, TWQC_GETS);
  if( NULL == (pbCap = dynamic_cast<CTWAINContainerBool*>(m_IndependantCapMap[CAP_UICONTROLLABLE]))
   || !pbCap->Add(TRUE, true) )
  {
		::MessageBox(g_hwndDLG,TEXT("Could not create CAP_UICONTROLLABLE !"),MB_CAPTION,MB_OK);
    //cerr << "Could not create CAP_UICONTROLLABLE" << endl;
    setConditionCode(TWCC_LOWMEMORY);
    return TWRC_FAILURE;
  }

  m_IndependantCapMap[CAP_ENABLEDSUIONLY] = new CTWAINContainerBool(CAP_ENABLEDSUIONLY, (m_AppID.SupportedGroups&DF_APP2)!=0, TWQC_GETS);
  if( NULL == (pbCap = dynamic_cast<CTWAINContainerBool*>(m_IndependantCapMap[CAP_ENABLEDSUIONLY]))
   || !pbCap->Add(TRUE, true) )
  {
		::MessageBox(g_hwndDLG,TEXT("Could not create CAP_ENABLEDSUIONLY !"),MB_CAPTION,MB_OK);
    //cerr << "Could not create CAP_ENABLEDSUIONLY" << endl;
    setConditionCode(TWCC_LOWMEMORY);
    return TWRC_FAILURE;
  }

  m_IndependantCapMap[CAP_XFERCOUNT] = new CTWAINContainerInt(CAP_XFERCOUNT, TWTY_INT16, TWON_ONEVALUE);
  if( NULL == (pnCap = dynamic_cast<CTWAINContainerInt*>(m_IndependantCapMap[CAP_XFERCOUNT]))
   || !pnCap->Add(TWON_DONTCARE32, true) )
  {
		::MessageBox(g_hwndDLG,TEXT("Could not create CAP_XFERCOUNT !"),MB_CAPTION,MB_OK);
    //cerr << "Could not create CAP_XFERCOUNT" << endl;
    setConditionCode(TWCC_LOWMEMORY);
    return TWRC_FAILURE;
  }

  m_IndependantCapMap[ICAP_BITORDER] = new CTWAINContainerInt(ICAP_BITORDER, TWTY_UINT16, TWON_ENUMERATION);
  if( NULL == (pnCap = dynamic_cast<CTWAINContainerInt*>(m_IndependantCapMap[ICAP_BITORDER]))
   || !pnCap->Add(TWBO_LSBFIRST, true)
   || !pnCap->Add(TWBO_MSBFIRST) )
  {
		::MessageBox(g_hwndDLG,TEXT("Could not create ICAP_BITORDER !"),MB_CAPTION,MB_OK);
    //cerr << "Could not create ICAP_BITORDER" << endl;
    setConditionCode(TWCC_LOWMEMORY);
    return TWRC_FAILURE;
  }

  m_IndependantCapMap[ICAP_IMAGEFILEFORMAT] = new CTWAINContainerInt(ICAP_IMAGEFILEFORMAT, TWTY_UINT16, TWON_ENUMERATION);
  if( NULL == (pnCap = dynamic_cast<CTWAINContainerInt*>(m_IndependantCapMap[ICAP_IMAGEFILEFORMAT]))
   || !pnCap->Add(TWFF_BMP, true)
   || !pnCap->Add(TWFF_TIFF) )
  {
		::MessageBox(g_hwndDLG,TEXT("Could not create ICAP_IMAGEFILEFORMAT !"),MB_CAPTION,MB_OK);
    //cerr << "Could not create ICAP_IMAGEFILEFORMAT" << endl;
    setConditionCode(TWCC_LOWMEMORY);
    return TWRC_FAILURE;
  }
  SSTRCPY(m_CurFileExferName, sizeof(m_CurFileExferName), "sample.bmp");
  SSTRCPY(m_DefFileExferName, sizeof(m_DefFileExferName), "sample.bmp");

  m_IndependantCapMap[ICAP_PIXELFLAVOR] = new CTWAINContainerInt(ICAP_PIXELFLAVOR, TWTY_UINT16, TWON_ENUMERATION);
  if( NULL == (pnCap = dynamic_cast<CTWAINContainerInt*>(m_IndependantCapMap[ICAP_PIXELFLAVOR]))
   || !pnCap->Add(TWPF_CHOCOLATE, true))
  {
		::MessageBox(g_hwndDLG,TEXT("Could not create ICAP_PIXELFLAVOR !"),MB_CAPTION,MB_OK);
    //cerr << "Could not create ICAP_PIXELFLAVOR" << endl;
    setConditionCode(TWCC_LOWMEMORY);
    return TWRC_FAILURE;
  }

	//zhu 重张检测
	m_IndependantCapMap[UDSCAP_MULTIFEEDDETECT] = new CTWAINContainerBool(UDSCAP_MULTIFEEDDETECT, (m_AppID.SupportedGroups&DF_APP2)!=0, TWQC_ALL);
	if( NULL == (pbCap = dynamic_cast<CTWAINContainerBool*>(m_IndependantCapMap[UDSCAP_MULTIFEEDDETECT]))
		|| !pbCap->Add(FALSE)
		|| !pbCap->Add(TRUE, true) )
	{
		::MessageBox(g_hwndDLG,TEXT("Could not create UDSCAP_MULTIFEEDDETECT !"),MB_CAPTION,MB_OK);
		//cerr << "Could not create UDSCAP_MULTIFEEDDETECT" << endl;
		setConditionCode(TWCC_LOWMEMORY);
		return TWRC_FAILURE;
	}

	//zhu 重张检测故障值
	m_IndependantCapMap[UDSCAP_MULTIFEEDDETECT_VALUE] = new CTWAINContainerBool(UDSCAP_MULTIFEEDDETECT_VALUE, (m_AppID.SupportedGroups&DF_APP2)!=0, TWQC_ALL);
	if( NULL == (pbCap = dynamic_cast<CTWAINContainerBool*>(m_IndependantCapMap[UDSCAP_MULTIFEEDDETECT_VALUE]))
		|| !pbCap->Add(FALSE, true)
		|| !pbCap->Add(TRUE) )
	{
		::MessageBox(g_hwndDLG,TEXT("Could not create UDSCAP_MULTIFEEDDETECT_VALUE !"),MB_CAPTION,MB_OK);
		setConditionCode(TWCC_LOWMEMORY);
		return TWRC_FAILURE;
	}

	//色彩翻转
	m_IndependantCapMap[UDSCAP_COLORFLIP] = new CTWAINContainerBool(UDSCAP_COLORFLIP, (m_AppID.SupportedGroups&DF_APP2)!=0, TWQC_ALL);
	if( NULL == (pbCap = dynamic_cast<CTWAINContainerBool*>(m_IndependantCapMap[UDSCAP_COLORFLIP]))
		|| !pbCap->Add(FALSE, true)
		|| !pbCap->Add(TRUE) )
	{
		::MessageBox(g_hwndDLG,TEXT("Could not create UDSCAP_COLORFLIP !"),MB_CAPTION,MB_OK);
		setConditionCode(TWCC_LOWMEMORY);
		return TWRC_FAILURE;
	}


	//多流输出
	m_IndependantCapMap[UDSCAP_MULTISTREAM] = new CTWAINContainerBool(UDSCAP_MULTISTREAM, (m_AppID.SupportedGroups&DF_APP2)!=0, TWQC_ALL);
	if( NULL == (pbCap = dynamic_cast<CTWAINContainerBool*>(m_IndependantCapMap[UDSCAP_MULTISTREAM]))
		|| !pbCap->Add(TRUE) 
		|| !pbCap->Add(FALSE, true) )
	{
		::MessageBox(g_hwndDLG,TEXT("Could not create UDSCAP_MULTISTREAM !"),MB_CAPTION,MB_OK);
		setConditionCode(TWCC_LOWMEMORY);
		return TWRC_FAILURE;
	}

	//纸张大小
  m_IndependantCapMap[ICAP_SUPPORTEDSIZES] = new CTWAINContainerInt(ICAP_SUPPORTEDSIZES, TWTY_UINT16, TWON_ENUMERATION);
  if( NULL == (pnCap = dynamic_cast<CTWAINContainerInt*>(m_IndependantCapMap[ICAP_SUPPORTEDSIZES]))
   || !pnCap->Add(TWSS_NONE) //zhu
	 || !pnCap->Add(TWSS_USLETTER)  //纸张大小，默认USLETTER
	 || !pnCap->Add(TWSS_USLEGAL)
	 //|| !pnCap->Add(TWSS_A3)  
	 || !pnCap->Add(TWSS_A4, true) 
	 || !pnCap->Add(TWSS_A5)  //wan
	 || !pnCap->Add(TWSS_A6)  //
	 || !pnCap->Add(TWSS_A7)  //wan
	 //|| !pnCap->Add(TWSS_ISOB4) //zhu
	 || !pnCap->Add(TWSS_ISOB5)  //wan
	 || !pnCap->Add(TWSS_ISOB6)  //wan
	 || !pnCap->Add(TWSS_ISOB7)  //wan
	 //|| !pnCap->Add(TWSS_JISB4)  //zhu 
	 || !pnCap->Add(TWSS_JISB5)  //wan
	 || !pnCap->Add(TWSS_JISB6)  //wan
	 || !pnCap->Add(TWSS_JISB7)  //wan
	 || !pnCap->Add(UDSCAP_LONGDOCUMENT) //zhu 长纸模式
	 || !pnCap->Add(TWSS_MAXSIZE)  //zhu
	 )
  {
		::MessageBox(g_hwndDLG,TEXT("Could not create ICAP_SUPPORTEDSIZES !"),MB_CAPTION,MB_OK);
    //cerr << "Could not create ICAP_SUPPORTEDSIZES" << endl;
    setConditionCode(TWCC_LOWMEMORY);
    return TWRC_FAILURE;
  }

	//zhu旋转
	m_IndependantCapMap[ICAP_ROTATION] = new CTWAINContainerFix32(ICAP_ROTATION,TWON_ENUMERATION, TWQC_ALL);
	if( NULL == (pfixCap = dynamic_cast<CTWAINContainerFix32*>(m_IndependantCapMap[ICAP_ROTATION]))
		|| !pfixCap->Add(TWOR_ROT0, true)
		|| !pfixCap->Add(TWOR_ROT90)
		|| !pfixCap->Add(TWOR_ROT180)
		|| !pfixCap->Add(TWOR_ROT270)) //zhu
	{
		::MessageBox(g_hwndDLG,TEXT("Could not create ICAP_ROTATION !"),MB_CAPTION,MB_OK);
		//cerr << "Could not create ICAP_ROTATION" << endl;
		setConditionCode(TWCC_LOWMEMORY);
		return TWRC_FAILURE;
	}
		
	//zhu 分割
	m_IndependantCapMap[UDSCAP_SPLITIMAGE] = new CTWAINContainerInt(UDSCAP_SPLITIMAGE, TWTY_UINT16, TWON_ENUMERATION);
	if(NULL == (pnCap = dynamic_cast<CTWAINContainerInt*>(m_IndependantCapMap[UDSCAP_SPLITIMAGE]))
		|| !pnCap->Add(TWSI_NONE, true)  
		|| !pnCap->Add(TWSI_HORIZONTAL)
		|| !pnCap->Add(TWSI_VERTICAL)
		//|| !pnCap->Add(TWSI_DEFINED)  暂时隐藏
		)  
	{
		::MessageBox(g_hwndDLG,TEXT("Could not create UDSCAP_SPLITIMAGE !"),MB_CAPTION,MB_OK);
		//cerr<<"Could not create UDSCAP_SPLITIMAGE"<<endl;
		setConditionCode(TWCC_LOWMEMORY);
		return TWRC_FAILURE;
	} //zhu

	//zhu 二值化
	m_IndependantCapMap[UDSCAP_BINARIZATION] = new CTWAINContainerInt(UDSCAP_BINARIZATION, TWTY_UINT16, TWON_ENUMERATION);
	if(NULL == (pnCap = dynamic_cast<CTWAINContainerInt*>(m_IndependantCapMap[UDSCAP_BINARIZATION]))
	  || !pnCap->Add(TWBZ_DYNATHRESHOLD) //默认动态阈值
		|| !pnCap->Add(TWBZ_FIXEDTHRESHOLD, true)
		|| !pnCap->Add(TWBZ_HALFTONE1)
		|| !pnCap->Add(TWBZ_HALFTONE2)
		|| !pnCap->Add(TWBZ_HALFTONE3)
		|| !pnCap->Add(TWBZ_HALFTONE4)
		|| !pnCap->Add(TWBZ_HALFTONE5)
		|| !pnCap->Add(TWBZ_ERRORDIFF)
		)  
	{
		::MessageBox(g_hwndDLG,TEXT("Could not create UDSCAP_BINARIZATION !"),MB_CAPTION,MB_OK);
		//cerr<<"Could not create UDSCAP_BINARIZATION"<<endl;
		setConditionCode(TWCC_LOWMEMORY);
		return TWRC_FAILURE;
	} //zhu
	//多流二值化
	m_IndependantCapMap[UDSCAP_BINARIZATIONFB] = new CTWAINContainerInt(UDSCAP_BINARIZATIONFB, TWTY_UINT16, TWON_ENUMERATION);
	if(NULL == (pnCap = dynamic_cast<CTWAINContainerInt*>(m_IndependantCapMap[UDSCAP_BINARIZATIONFB]))
		|| !pnCap->Add(TWBZ_DYNATHRESHOLD) //默认动态阈值
		|| !pnCap->Add(TWBZ_FIXEDTHRESHOLD, true)
		|| !pnCap->Add(TWBZ_HALFTONE1)
		|| !pnCap->Add(TWBZ_HALFTONE2)
		|| !pnCap->Add(TWBZ_HALFTONE3)
		|| !pnCap->Add(TWBZ_HALFTONE4)
		|| !pnCap->Add(TWBZ_HALFTONE5)
		|| !pnCap->Add(TWBZ_ERRORDIFF)
		)  
	{
		::MessageBox(g_hwndDLG,TEXT("Could not create UDSCAP_BINARIZATIONFB !"),MB_CAPTION,MB_OK);
		setConditionCode(TWCC_LOWMEMORY);
		return TWRC_FAILURE;
	} //zhu
	m_IndependantCapMap[UDSCAP_BINARIZATIONBB] = new CTWAINContainerInt(UDSCAP_BINARIZATIONBB, TWTY_UINT16, TWON_ENUMERATION);
	if(NULL == (pnCap = dynamic_cast<CTWAINContainerInt*>(m_IndependantCapMap[UDSCAP_BINARIZATIONBB]))
		|| !pnCap->Add(TWBZ_DYNATHRESHOLD) //默认动态阈值
		|| !pnCap->Add(TWBZ_FIXEDTHRESHOLD, true)
		|| !pnCap->Add(TWBZ_HALFTONE1)
		|| !pnCap->Add(TWBZ_HALFTONE2)
		|| !pnCap->Add(TWBZ_HALFTONE3)
		|| !pnCap->Add(TWBZ_HALFTONE4)
		|| !pnCap->Add(TWBZ_HALFTONE5)
		|| !pnCap->Add(TWBZ_ERRORDIFF)
		)  
	{
		::MessageBox(g_hwndDLG,TEXT("Could not create UDSCAP_BINARIZATIONBB !"),MB_CAPTION,MB_OK);
		setConditionCode(TWCC_LOWMEMORY);
		return TWRC_FAILURE;
	} //zhu

  m_IndependantCapMap[CAP_DEVICEONLINE] = new CTWAINContainerBool(CAP_DEVICEONLINE, (m_AppID.SupportedGroups&DF_APP2)!=0, TWQC_GETS);
  if( NULL == (pbCap = dynamic_cast<CTWAINContainerBool*>(m_IndependantCapMap[CAP_DEVICEONLINE]))
   || !pbCap->Add(TRUE, true)
   || !pbCap->Add(FALSE) )
  {
		::MessageBox(g_hwndDLG,TEXT("Could not create CAP_DEVICEONLINE !"),MB_CAPTION,MB_OK);
    //cerr << "Could not create CAP_DEVICEONLINE" << endl;
    setConditionCode(TWCC_LOWMEMORY);
    return TWRC_FAILURE;
  }

  m_IndependantCapMap[CAP_INDICATORS] = new CTWAINContainerBool(CAP_INDICATORS, (m_AppID.SupportedGroups&DF_APP2)!=0, TWQC_ALL);
  if( NULL == (pbCap = dynamic_cast<CTWAINContainerBool*>(m_IndependantCapMap[CAP_INDICATORS]))
   || !pbCap->Add(TRUE, true)
   || !pbCap->Add(FALSE))
  {
		::MessageBox(g_hwndDLG,TEXT("Could not create CAP_INDICATORS !"),MB_CAPTION,MB_OK);
    //cerr << "Could not create CAP_INDICATORS" << endl;
    setConditionCode(TWCC_LOWMEMORY);
    return TWRC_FAILURE;
  }

  m_IndependantCapMap[ICAP_MAXFRAMES] = new CTWAINContainerInt(ICAP_MAXFRAMES, TWTY_UINT16, TWON_ONEVALUE, TWQC_ALL);
  if( NULL == (pnCap = dynamic_cast<CTWAINContainerInt*>(m_IndependantCapMap[ICAP_MAXFRAMES]))
   || !pnCap->Add(1, true) )
  {
		::MessageBox(g_hwndDLG,TEXT("Could not create ICAP_MAXFRAMES !"),MB_CAPTION,MB_OK);
    //cerr << "Could not create ICAP_MAXFRAMES" << endl;
    setConditionCode(TWCC_LOWMEMORY);
    return TWRC_FAILURE;
  }

  m_IndependantCapMap[CAP_FEEDERENABLED] = new CTWAINContainerBool(CAP_FEEDERENABLED, (m_AppID.SupportedGroups&DF_APP2)!=0, TWQC_ALL);
  if( NULL == (pbCap = dynamic_cast<CTWAINContainerBool*>(m_IndependantCapMap[CAP_FEEDERENABLED]))
   || !pbCap->Add(FALSE)
	 || !pbCap->Add(TRUE, true) //wan，扫描模式，默认自动进纸器
	  )  
  {
		::MessageBox(g_hwndDLG,TEXT("Could not create CAP_FEEDERENABLED !"),MB_CAPTION,MB_OK);
    //cerr << "Could not create CAP_FEEDERENABLED" << endl;
    setConditionCode(TWCC_LOWMEMORY);
    return TWRC_FAILURE;
  }

  m_IndependantCapMap[CAP_DUPLEX] = new CTWAINContainerInt(CAP_DUPLEX, TWTY_UINT16, TWON_ONEVALUE, TWQC_GETS);
  if( NULL == (pnCap = dynamic_cast<CTWAINContainerInt*>(m_IndependantCapMap[CAP_DUPLEX]))
   || !pnCap->Add(TWDX_NONE, true)
	 || !pnCap->Add(TWDX_1PASSDUPLEX)  // wan
	 )
  {
		::MessageBox(g_hwndDLG,TEXT("Could not create CAP_DUPLEX !"),MB_CAPTION,MB_OK);
    //cerr << "Could not create CAP_DUPLEX" << endl;
    setConditionCode(TWCC_LOWMEMORY);
    return TWRC_FAILURE;
  }

  m_IndependantCapMap[CAP_DUPLEXENABLED] = new CTWAINContainerBool(CAP_DUPLEXENABLED, (m_AppID.SupportedGroups&DF_APP2)!=0, TWQC_ALL);
  if( NULL == (pbCap = dynamic_cast<CTWAINContainerBool*>(m_IndependantCapMap[CAP_DUPLEXENABLED]))
   || !pbCap->Add(FALSE, true)  // wan,单面/双面，默认单面扫
	 || !pbCap->Add(TRUE)   //TRUE为双面
	 )
  {
		::MessageBox(g_hwndDLG,TEXT("Could not create CAP_DUPLEXENABLED !"),MB_CAPTION,MB_OK);
    //cerr << "Could not create CAP_DUPLEXENABLED" << endl;
    setConditionCode(TWCC_LOWMEMORY);
    return TWRC_FAILURE;
  }

  m_IndependantCapMap[CAP_FEEDERLOADED] = new CTWAINContainerBool(CAP_FEEDERLOADED, (m_AppID.SupportedGroups&DF_APP2)!=0, TWQC_GETS);
  if( NULL == (pbCap = dynamic_cast<CTWAINContainerBool*>(m_IndependantCapMap[CAP_FEEDERLOADED]))
   || !pbCap->Add(TRUE)
   || !pbCap->Add(FALSE, true) )  
  {
		::MessageBox(g_hwndDLG,TEXT("Could not create CAP_FEEDERLOADED !"),MB_CAPTION,MB_OK);
    //cerr << "Could not create CAP_FEEDERLOADED" << endl;
    setConditionCode(TWCC_LOWMEMORY);
    return TWRC_FAILURE;
  }

  m_IndependantCapMap[CAP_AUTOFEED] = new CTWAINContainerBool(CAP_AUTOFEED, (m_AppID.SupportedGroups&DF_APP2)!=0, TWQC_ALL);
  if( NULL == (pbCap = dynamic_cast<CTWAINContainerBool*>(m_IndependantCapMap[CAP_AUTOFEED]))
   || !pbCap->Add(TRUE, true)
   || !pbCap->Add(FALSE) )
  {
		::MessageBox(g_hwndDLG,TEXT("Could not create CAP_AUTOFEED !"),MB_CAPTION,MB_OK);
    //cerr << "Could not create CAP_AUTOFEED" << endl;
    setConditionCode(TWCC_LOWMEMORY);
    return TWRC_FAILURE;
  }
  
  m_IndependantCapMap[CAP_PAPERDETECTABLE] = new CTWAINContainerBool(CAP_PAPERDETECTABLE, (m_AppID.SupportedGroups&DF_APP2)!=0, TWQC_GETS);
  if( NULL == (pbCap = dynamic_cast<CTWAINContainerBool*>(m_IndependantCapMap[CAP_PAPERDETECTABLE]))
   || !pbCap->Add(TRUE, true) )
  {
		::MessageBox(g_hwndDLG,TEXT("Could not create CAP_PAPERDETECTABLE !"),MB_CAPTION,MB_OK);
    //cerr << "Could not create CAP_PAPERDETECTABLE" << endl;
    setConditionCode(TWCC_LOWMEMORY);
    return TWRC_FAILURE;
  }

  m_IndependantCapMap[UDSCAP_LONGDOCUMENT] = new CTWAINContainerBool(UDSCAP_LONGDOCUMENT, (m_AppID.SupportedGroups&DF_APP2)!=0, TWQC_ALL);
  if( NULL == (pbCap = dynamic_cast<CTWAINContainerBool*>(m_IndependantCapMap[UDSCAP_LONGDOCUMENT]))
   || !pbCap->Add(TRUE)
   || !pbCap->Add(FALSE, true) )
  {
		::MessageBox(g_hwndDLG,TEXT("Could not create UDSCAP_LONGDOCUMENT !"),MB_CAPTION,MB_OK);
    //cerr << "Could not create UDSCAP_LONGDOCUMENT" << endl;
    setConditionCode(TWCC_LOWMEMORY);
    return TWRC_FAILURE;
  }

  m_IndependantCapMap[UDSCAP_DOCS_IN_ADF] = new CTWAINContainerInt(UDSCAP_DOCS_IN_ADF, TWTY_UINT16, TWON_ONEVALUE, TWQC_ALL);
  if( NULL == (pnCap = dynamic_cast<CTWAINContainerInt*>(m_IndependantCapMap[UDSCAP_DOCS_IN_ADF]))
   || !pnCap->Add(m_pScanner->GetMaxPagesInADF()))
  {
		::MessageBox(g_hwndDLG,TEXT("Could not create UDSCAP_DOCS_IN_ADF !"),MB_CAPTION,MB_OK);
    //cerr << "Could not create UDSCAP_DOCS_IN_ADF" << endl;
    setConditionCode(TWCC_LOWMEMORY);
    return TWRC_FAILURE;
  }

  m_IndependantCapMap[CAP_CUSTOMDSDATA] = new CTWAINContainerBool(CAP_CUSTOMDSDATA, (m_AppID.SupportedGroups&DF_APP2)!=0, TWQC_GETS);
  if( NULL == (pbCap = dynamic_cast<CTWAINContainerBool*>(m_IndependantCapMap[CAP_CUSTOMDSDATA]))
   || !pbCap->Add(TRUE, true) )
  {
		::MessageBox(g_hwndDLG,TEXT("Could not create CAP_CUSTOMDSDATA !"),MB_CAPTION,MB_OK);
    //cerr << "Could not create CAP_CUSTOMDSDATA" << endl;
    setConditionCode(TWCC_LOWMEMORY);
    return TWRC_FAILURE;
  }

  // setup dependant caps
  if( NULL == (m_BitDepthMap[TWPT_BW] = new CTWAINContainerInt(ICAP_BITDEPTH, TWTY_UINT16, TWON_ENUMERATION))
   || !m_BitDepthMap[TWPT_BW]->Add(1, true) )
  {
		::MessageBox(g_hwndDLG,TEXT("Could not create ICAP_BITDEPTH !"),MB_CAPTION,MB_OK);
    //cerr << "Could not create ICAP_BITDEPTH" << endl;
    setConditionCode(TWCC_LOWMEMORY);
    return TWRC_FAILURE;
  }

  if( NULL == (m_BitDepthMap[TWPT_GRAY] = new CTWAINContainerInt(ICAP_BITDEPTH, TWTY_UINT16, TWON_ENUMERATION))
   || !m_BitDepthMap[TWPT_GRAY]->Add(8, true) )
  {
		::MessageBox(g_hwndDLG,TEXT("Could not create ICAP_BITDEPTH !"),MB_CAPTION,MB_OK);
    //cerr << "Could not create ICAP_BITDEPTH" << endl;
    setConditionCode(TWCC_LOWMEMORY);
    return TWRC_FAILURE;
  }

  if( NULL == (m_BitDepthMap[TWPT_RGB] = new CTWAINContainerInt(ICAP_BITDEPTH, TWTY_UINT16, TWON_ENUMERATION))
   || !m_BitDepthMap[TWPT_RGB]->Add(24, true) )
  {
		::MessageBox(g_hwndDLG,TEXT("Could not create ICAP_BITDEPTH !"),MB_CAPTION,MB_OK);
    //cerr << "Could not create ICAP_BITDEPTH" << endl;
    setConditionCode(TWCC_LOWMEMORY);
    return TWRC_FAILURE;
  }

  m_IndependantCapMap[CAP_CUSTOMINTERFACEGUID] = new CTWAINContainerString(CAP_CUSTOMINTERFACEGUID,TWTY_STR255,TWON_ONEVALUE, TWQC_GETS);
  if( NULL == (pstrCap = dynamic_cast<CTWAINContainerString*>(m_IndependantCapMap[CAP_CUSTOMINTERFACEGUID]))
    || !pstrCap->Add(kCUSTOMDSGUI, true))
  {
		::MessageBox(g_hwndDLG,TEXT("Could not create CAP_CUSTOMINTERFACEGUID !"),MB_CAPTION,MB_OK);
    //cerr << "Could not create CAP_CUSTOMINTERFACEGUID" << endl;
    setConditionCode(TWCC_LOWMEMORY);
    return TWRC_FAILURE;
  }
	
	//压缩
	m_IndependantCapMap[ICAP_COMPRESSION] = new CTWAINContainerInt(ICAP_COMPRESSION, TWTY_UINT16, TWON_ENUMERATION);
	if( NULL == (pnCap = dynamic_cast<CTWAINContainerInt*>(m_IndependantCapMap[ICAP_COMPRESSION]))
		|| !pnCap->Add(TWCP_NONE, true)
		|| !pnCap->Add(TWCP_JPEG) //JPEG压缩
		|| !pnCap->Add(TWCP_GROUP4)) //G4压缩
	{
		::MessageBox(g_hwndDLG,TEXT("Could not create ICAP_COMPRESSION !"),MB_CAPTION,MB_OK);
		setConditionCode(TWCC_LOWMEMORY);
		return TWRC_FAILURE;
	}
	//多流压缩
	m_IndependantCapMap[UDSCAP_COMPRESSIONFC] = new CTWAINContainerInt(UDSCAP_COMPRESSIONFC, TWTY_UINT16, TWON_ENUMERATION);
	if( NULL == (pnCap = dynamic_cast<CTWAINContainerInt*>(m_IndependantCapMap[UDSCAP_COMPRESSIONFC]))
		|| !pnCap->Add(TWCP_NONE, true)
		|| !pnCap->Add(TWCP_JPEG) //JPEG压缩
		|| !pnCap->Add(TWCP_GROUP4)) //G4压缩
	{
		::MessageBox(g_hwndDLG,TEXT("Could not create UDSCAP_COMPRESSIONFC !"),MB_CAPTION,MB_OK);
		setConditionCode(TWCC_LOWMEMORY);
		return TWRC_FAILURE;
	}
	m_IndependantCapMap[UDSCAP_COMPRESSIONFG] = new CTWAINContainerInt(UDSCAP_COMPRESSIONFG, TWTY_UINT16, TWON_ENUMERATION);
	if( NULL == (pnCap = dynamic_cast<CTWAINContainerInt*>(m_IndependantCapMap[UDSCAP_COMPRESSIONFG]))
		|| !pnCap->Add(TWCP_NONE, true)
		|| !pnCap->Add(TWCP_JPEG) //JPEG压缩
		|| !pnCap->Add(TWCP_GROUP4)) //G4压缩
	{
		::MessageBox(g_hwndDLG,TEXT("Could not create UDSCAP_COMPRESSIONFG !"),MB_CAPTION,MB_OK);
		setConditionCode(TWCC_LOWMEMORY);
		return TWRC_FAILURE;
	}
	m_IndependantCapMap[UDSCAP_COMPRESSIONFB] = new CTWAINContainerInt(UDSCAP_COMPRESSIONFB, TWTY_UINT16, TWON_ENUMERATION);
	if( NULL == (pnCap = dynamic_cast<CTWAINContainerInt*>(m_IndependantCapMap[UDSCAP_COMPRESSIONFB]))
		|| !pnCap->Add(TWCP_NONE, true)
		|| !pnCap->Add(TWCP_JPEG) //JPEG压缩
		|| !pnCap->Add(TWCP_GROUP4)) //G4压缩
	{
		::MessageBox(g_hwndDLG,TEXT("Could not create UDSCAP_COMPRESSIONFB !"),MB_CAPTION,MB_OK);
		setConditionCode(TWCC_LOWMEMORY);
		return TWRC_FAILURE;
	}
	m_IndependantCapMap[UDSCAP_COMPRESSIONBC] = new CTWAINContainerInt(UDSCAP_COMPRESSIONBC, TWTY_UINT16, TWON_ENUMERATION);
	if( NULL == (pnCap = dynamic_cast<CTWAINContainerInt*>(m_IndependantCapMap[UDSCAP_COMPRESSIONBC]))
		|| !pnCap->Add(TWCP_NONE, true)
		|| !pnCap->Add(TWCP_JPEG) //JPEG压缩
		|| !pnCap->Add(TWCP_GROUP4)) //G4压缩
	{
		::MessageBox(g_hwndDLG,TEXT("Could not create UDSCAP_COMPRESSIONBC !"),MB_CAPTION,MB_OK);
		setConditionCode(TWCC_LOWMEMORY);
		return TWRC_FAILURE;
	}
	m_IndependantCapMap[UDSCAP_COMPRESSIONBG] = new CTWAINContainerInt(UDSCAP_COMPRESSIONBG, TWTY_UINT16, TWON_ENUMERATION);
	if( NULL == (pnCap = dynamic_cast<CTWAINContainerInt*>(m_IndependantCapMap[UDSCAP_COMPRESSIONBG]))
		|| !pnCap->Add(TWCP_NONE, true)
		|| !pnCap->Add(TWCP_JPEG) //JPEG压缩
		|| !pnCap->Add(TWCP_GROUP4)) //G4压缩
	{
		::MessageBox(g_hwndDLG,TEXT("Could not create UDSCAP_COMPRESSIONBG !"),MB_CAPTION,MB_OK);
		setConditionCode(TWCC_LOWMEMORY);
		return TWRC_FAILURE;
	}
	m_IndependantCapMap[UDSCAP_COMPRESSIONBB] = new CTWAINContainerInt(UDSCAP_COMPRESSIONBB, TWTY_UINT16, TWON_ENUMERATION);
	if( NULL == (pnCap = dynamic_cast<CTWAINContainerInt*>(m_IndependantCapMap[UDSCAP_COMPRESSIONBB]))
		|| !pnCap->Add(TWCP_NONE, true)
		|| !pnCap->Add(TWCP_JPEG) //JPEG压缩
		|| !pnCap->Add(TWCP_GROUP4)) //G4压缩
	{
		::MessageBox(g_hwndDLG,TEXT("Could not create UDSCAP_COMPRESSIONBB !"),MB_CAPTION,MB_OK);
		setConditionCode(TWCC_LOWMEMORY);
		return TWRC_FAILURE;
	}

	fRange.fCurrentValue = 0.0f; 
	fRange.fMaxValue = 100.0f;
	fRange.fMinValue = 0.0f;
	fRange.fStepSize = 1.0f;
	//压缩比的值
	m_IndependantCapMap[UDSCAP_COMPRESSVALUE] = new CTWAINContainerFix32Range(UDSCAP_COMPRESSVALUE,fRange, TWQC_ALL);
	if( NULL == dynamic_cast<CTWAINContainerFix32Range*>(m_IndependantCapMap[UDSCAP_COMPRESSVALUE]))
	{
		::MessageBox(g_hwndDLG,TEXT("Could not create UDSCAP_COMPRESSVALUE !"),MB_CAPTION,MB_OK);
		setConditionCode(TWCC_LOWMEMORY);
		return TWRC_FAILURE;
	}
	//多流压缩比的值
	m_IndependantCapMap[UDSCAP_COMPRESSIONVALUEFC] = new CTWAINContainerFix32Range(UDSCAP_COMPRESSIONVALUEFC,fRange, TWQC_ALL);
	if( NULL == dynamic_cast<CTWAINContainerFix32Range*>(m_IndependantCapMap[UDSCAP_COMPRESSIONVALUEFC]))
	{
		::MessageBox(g_hwndDLG,TEXT("Could not create UDSCAP_COMPRESSIONVALUEFC !"),MB_CAPTION,MB_OK);
		setConditionCode(TWCC_LOWMEMORY);
		return TWRC_FAILURE;
	}
	m_IndependantCapMap[UDSCAP_COMPRESSIONVALUEFG] = new CTWAINContainerFix32Range(UDSCAP_COMPRESSIONVALUEFG,fRange, TWQC_ALL);
	if( NULL == dynamic_cast<CTWAINContainerFix32Range*>(m_IndependantCapMap[UDSCAP_COMPRESSIONVALUEFG]))
	{
		::MessageBox(g_hwndDLG,TEXT("Could not create UDSCAP_COMPRESSIONVALUEFG !"),MB_CAPTION,MB_OK);
		setConditionCode(TWCC_LOWMEMORY);
		return TWRC_FAILURE;
	}
	m_IndependantCapMap[UDSCAP_COMPRESSIONVALUEFB] = new CTWAINContainerFix32Range(UDSCAP_COMPRESSIONVALUEFB,fRange, TWQC_ALL);
	if( NULL == dynamic_cast<CTWAINContainerFix32Range*>(m_IndependantCapMap[UDSCAP_COMPRESSIONVALUEFB]))
	{
		::MessageBox(g_hwndDLG,TEXT("Could not create UDSCAP_COMPRESSIONVALUEFB !"),MB_CAPTION,MB_OK);
		setConditionCode(TWCC_LOWMEMORY);
		return TWRC_FAILURE;
	}
	m_IndependantCapMap[UDSCAP_COMPRESSIONVALUEBC] = new CTWAINContainerFix32Range(UDSCAP_COMPRESSIONVALUEBC,fRange, TWQC_ALL);
	if( NULL == dynamic_cast<CTWAINContainerFix32Range*>(m_IndependantCapMap[UDSCAP_COMPRESSIONVALUEBC]))
	{
		::MessageBox(g_hwndDLG,TEXT("Could not create UDSCAP_COMPRESSIONVALUEBC !"),MB_CAPTION,MB_OK);
		setConditionCode(TWCC_LOWMEMORY);
		return TWRC_FAILURE;
	}
	m_IndependantCapMap[UDSCAP_COMPRESSIONVALUEBG] = new CTWAINContainerFix32Range(UDSCAP_COMPRESSIONVALUEBG,fRange, TWQC_ALL);
	if( NULL == dynamic_cast<CTWAINContainerFix32Range*>(m_IndependantCapMap[UDSCAP_COMPRESSIONVALUEBG]))
	{
		::MessageBox(g_hwndDLG,TEXT("Could not create UDSCAP_COMPRESSIONVALUEBG !"),MB_CAPTION,MB_OK);
		setConditionCode(TWCC_LOWMEMORY);
		return TWRC_FAILURE;
	}
	m_IndependantCapMap[UDSCAP_COMPRESSIONVALUEBB] = new CTWAINContainerFix32Range(UDSCAP_COMPRESSIONVALUEBB,fRange, TWQC_ALL);
	if( NULL == dynamic_cast<CTWAINContainerFix32Range*>(m_IndependantCapMap[UDSCAP_COMPRESSIONVALUEBB]))
	{
		::MessageBox(g_hwndDLG,TEXT("Could not create UDSCAP_COMPRESSIONVALUEBB !"),MB_CAPTION,MB_OK);
		setConditionCode(TWCC_LOWMEMORY);
		return TWRC_FAILURE;
	}

	//Gamma校正范围
	fRange.fCurrentValue = 100.0f; 
	fRange.fMaxValue = 400.0f;
	fRange.fMinValue = 1.0f;
	fRange.fStepSize = 1.0f;
	//Gamma校正
	m_IndependantCapMap[ICAP_GAMMA] = new CTWAINContainerFix32Range(ICAP_GAMMA,fRange, TWQC_ALL);
	if( NULL == dynamic_cast<CTWAINContainerFix32Range*>(m_IndependantCapMap[ICAP_GAMMA]))
	{
		::MessageBox(g_hwndDLG,TEXT("Could not create ICAP_GAMMA !"),MB_CAPTION,MB_OK);
		//cerr << "Could not create ICAP_GAMMA" << endl;
		setConditionCode(TWCC_LOWMEMORY);
		return TWRC_FAILURE;
	}

	//去除空白页的checkbox状态
	m_IndependantCapMap[UDSCAP_REMOVEBLANK] = new CTWAINContainerBool(UDSCAP_REMOVEBLANK, (m_AppID.SupportedGroups&DF_APP2)!=0, TWQC_ALL);
	if( NULL == (pbCap = dynamic_cast<CTWAINContainerBool*>(m_IndependantCapMap[UDSCAP_REMOVEBLANK]))
		|| !pbCap->Add(TRUE)
		|| !pbCap->Add(FALSE, true) )
	{
		::MessageBox(g_hwndDLG,TEXT("Could not create UDSCAP_REMOVEBLANK !"),MB_CAPTION,MB_OK);
		//cerr << "Could not create UDSCAP_REMOVEBLANK" << endl;
		setConditionCode(TWCC_LOWMEMORY);
		return TWRC_FAILURE;
	}

	// 去除空白页等
	fRange.fCurrentValue = 10.0f; 
	fRange.fMaxValue = 100.0f;
	fRange.fMinValue = 0.0f;
	fRange.fStepSize = 1.0f;
	// 去除空白页等
	m_IndependantCapMap[ICAP_AUTODISCARDBLANKPAGES] = new CTWAINContainerFix32Range(ICAP_AUTODISCARDBLANKPAGES,fRange, TWQC_ALL);
	if( NULL == dynamic_cast<CTWAINContainerFix32Range*>(m_IndependantCapMap[ICAP_AUTODISCARDBLANKPAGES]))
	{
		::MessageBox(g_hwndDLG,TEXT("Could not create ICAP_AUTODISCARDBLANKPAGES !"),MB_CAPTION,MB_OK);
		//cerr << "Could not create ICAP_AUTODISCARDBLANKPAGES" << endl;
		setConditionCode(TWCC_LOWMEMORY);
		return TWRC_FAILURE;
	}

	m_IndependantCapMap[UDSCAP_PUNCHHOLEREMOVEL] = new CTWAINContainerBool(UDSCAP_PUNCHHOLEREMOVEL, (m_AppID.SupportedGroups&DF_APP2)!=0, TWQC_ALL);
	if( NULL == (pbCap = dynamic_cast<CTWAINContainerBool*>(m_IndependantCapMap[UDSCAP_PUNCHHOLEREMOVEL]))
		|| !pbCap->Add(TRUE)
		|| !pbCap->Add(FALSE, true) )
	{
		::MessageBox(g_hwndDLG,TEXT("Could not create UDSCAP_PUNCHHOLEREMOVEL !"),MB_CAPTION,MB_OK);
		//cerr << "Could not create UDSCAP_PUNCHHOLEREMOVEL" << endl;
		setConditionCode(TWCC_LOWMEMORY);
		return TWRC_FAILURE;
	}

	m_IndependantCapMap[UDSCAP_SHARPEN] = new CTWAINContainerBool(UDSCAP_SHARPEN, (m_AppID.SupportedGroups&DF_APP2)!=0, TWQC_ALL);
	if( NULL == (pbCap = dynamic_cast<CTWAINContainerBool*>(m_IndependantCapMap[UDSCAP_SHARPEN]))
		|| !pbCap->Add(TRUE)
		|| !pbCap->Add(FALSE, true) )
	{
		::MessageBox(g_hwndDLG,TEXT("Could not create UDSCAP_SHARPEN !"),MB_CAPTION,MB_OK);
		//cerr << "Could not create UDSCAP_SHARPEN" << endl;
		setConditionCode(TWCC_LOWMEMORY);
		return TWRC_FAILURE;
	}

	m_IndependantCapMap[UDSCAP_MIRROR] = new CTWAINContainerBool(UDSCAP_MIRROR, (m_AppID.SupportedGroups&DF_APP2)!=0, TWQC_ALL);
	if( NULL == (pbCap = dynamic_cast<CTWAINContainerBool*>(m_IndependantCapMap[UDSCAP_MIRROR]))
		|| !pbCap->Add(TRUE)
		|| !pbCap->Add(FALSE, true) )
	{
		::MessageBox(g_hwndDLG,TEXT("Could not create UDSCAP_MIRROR !"),MB_CAPTION,MB_OK);
		//cerr << "Could not create UDSCAP_MIRROR" << endl;
		setConditionCode(TWCC_LOWMEMORY);
		return TWRC_FAILURE;
	}

	m_IndependantCapMap[UDSCAP_REMOVEBACKGROUND] = new CTWAINContainerBool(UDSCAP_REMOVEBACKGROUND, (m_AppID.SupportedGroups&DF_APP2)!=0, TWQC_ALL);
	if( NULL == (pbCap = dynamic_cast<CTWAINContainerBool*>(m_IndependantCapMap[UDSCAP_REMOVEBACKGROUND]))
		|| !pbCap->Add(TRUE)
		|| !pbCap->Add(FALSE, true) )
	{
		::MessageBox(g_hwndDLG,TEXT("Could not create UDSCAP_REMOVEBACKGROUND !"),MB_CAPTION,MB_OK);
		//cerr << "Could not create UDSCAP_REMOVEBACKGROUND" << endl;
		setConditionCode(TWCC_LOWMEMORY);
		return TWRC_FAILURE;
	}

	m_IndependantCapMap[UDSCAP_DESCREEN] = new CTWAINContainerBool(UDSCAP_DESCREEN, (m_AppID.SupportedGroups&DF_APP2)!=0, TWQC_ALL);
	if( NULL == (pbCap = dynamic_cast<CTWAINContainerBool*>(m_IndependantCapMap[UDSCAP_DESCREEN]))
		|| !pbCap->Add(TRUE)
		|| !pbCap->Add(FALSE, true) )
	{
		::MessageBox(g_hwndDLG,TEXT("Could not create UDSCAP_DESCREEN !"),MB_CAPTION,MB_OK);
		//cerr << "Could not create UDSCAP_DESCREEN" << endl;
		setConditionCode(TWCC_LOWMEMORY);
		return TWRC_FAILURE;
	}

	m_IndependantCapMap[UDSCAP_DENOISE] = new CTWAINContainerBool(UDSCAP_DENOISE, (m_AppID.SupportedGroups&DF_APP2)!=0, TWQC_ALL);
	if( NULL == (pbCap = dynamic_cast<CTWAINContainerBool*>(m_IndependantCapMap[UDSCAP_DENOISE]))
		|| !pbCap->Add(TRUE)
		|| !pbCap->Add(FALSE, true) )
	{
		::MessageBox(g_hwndDLG,TEXT("Could not create UDSCAP_DENOISE !"),MB_CAPTION,MB_OK);
		//cerr << "Could not create UDSCAP_DENOISE" << endl;
		setConditionCode(TWCC_LOWMEMORY);
		return TWRC_FAILURE;
	}

	m_IndependantCapMap[UDSCAP_AUTOCROP] = new CTWAINContainerBool(UDSCAP_AUTOCROP, (m_AppID.SupportedGroups&DF_APP2)!=0, TWQC_ALL);
	if( NULL == (pbCap = dynamic_cast<CTWAINContainerBool*>(m_IndependantCapMap[UDSCAP_AUTOCROP]))
		|| !pbCap->Add(TRUE, true)
		|| !pbCap->Add(FALSE) )
	{
		::MessageBox(g_hwndDLG,TEXT("Could not create UDSCAP_AUTOCROP !"),MB_CAPTION,MB_OK);
		//cerr << "Could not create UDSCAP_AUTOCROP" << endl;
		setConditionCode(TWCC_LOWMEMORY);
		return TWRC_FAILURE;
	}

    //图像裁切方式
	m_IndependantCapMap[UDSCAP_CUTMETHOD] = new CTWAINContainerInt(UDSCAP_CUTMETHOD, TWTY_UINT16, TWON_ENUMERATION);
	if( NULL == (pnCap = dynamic_cast<CTWAINContainerInt*>(m_IndependantCapMap[UDSCAP_CUTMETHOD]))
		|| !pnCap->Add(TWCT_NONE)
		|| !pnCap->Add(TWCT_AUTO, true)
		|| !pnCap->Add(TWCT_SPECIFY) )
	{
		::MessageBox(g_hwndDLG,TEXT("Could not create UDSCAP_CUTMETHOD !"),MB_CAPTION,MB_OK);
		setConditionCode(TWCC_LOWMEMORY);
		return TWRC_FAILURE;
	}
//FLOAT_RANGE fRange;
  fRange.fCurrentValue = 128.0f; 
  fRange.fMaxValue = 255.0f;
  fRange.fMinValue = 1.0f;
  fRange.fStepSize = 1.0f;
  m_IndependantCapMap[ICAP_THRESHOLD] = new CTWAINContainerFix32Range(ICAP_THRESHOLD,fRange, TWQC_ALL);
  if( NULL == dynamic_cast<CTWAINContainerFix32Range*>(m_IndependantCapMap[ICAP_THRESHOLD]))
  {
		::MessageBox(g_hwndDLG,TEXT("Could not create ICAP_THRESHOLD !"),MB_CAPTION,MB_OK);
    //cerr << "Could not create ICAP_THRESHOLD" << endl;
    setConditionCode(TWCC_LOWMEMORY);
    return TWRC_FAILURE;
	}
	//多流阈值
	m_IndependantCapMap[UDSCAP_THRESHOLDFB] = new CTWAINContainerFix32Range(UDSCAP_THRESHOLDFB,fRange, TWQC_ALL);
	if( NULL == dynamic_cast<CTWAINContainerFix32Range*>(m_IndependantCapMap[UDSCAP_THRESHOLDFB]))
	{
		::MessageBox(g_hwndDLG,TEXT("Could not create UDSCAP_THRESHOLDFB !"),MB_CAPTION,MB_OK);
		setConditionCode(TWCC_LOWMEMORY);
		return TWRC_FAILURE;
	}
	m_IndependantCapMap[UDSCAP_THRESHOLDBB] = new CTWAINContainerFix32Range(UDSCAP_THRESHOLDBB,fRange, TWQC_ALL);
	if( NULL == dynamic_cast<CTWAINContainerFix32Range*>(m_IndependantCapMap[UDSCAP_THRESHOLDBB]))
	{
		::MessageBox(g_hwndDLG,TEXT("Could not create UDSCAP_THRESHOLDBB !"),MB_CAPTION,MB_OK);
		setConditionCode(TWCC_LOWMEMORY);
		return TWRC_FAILURE;
	}

	//去除斑点
	fRange.fCurrentValue = 20.0f; 
	fRange.fMaxValue = 30.0f;
	fRange.fMinValue = 1.0f;
	fRange.fStepSize = 1.0f;
	m_IndependantCapMap[UDSCAP_REMOVESPOTS] = new CTWAINContainerFix32Range(UDSCAP_REMOVESPOTS,fRange, TWQC_ALL);
	if( NULL == dynamic_cast<CTWAINContainerFix32Range*>(m_IndependantCapMap[UDSCAP_REMOVESPOTS]))
	{
		::MessageBox(g_hwndDLG,TEXT("Could not create UDSCAP_SENSITIVETHRESHOLD_REMOVESPOTS !"),MB_CAPTION,MB_OK);
		//cerr << "Could not create UDSCAP_SENSITIVETHRESHOLD_REMOVESPOTS" << endl;
		setConditionCode(TWCC_LOWMEMORY);
		return TWRC_FAILURE;
	}
	//多流去除斑点
	m_IndependantCapMap[UDSCAP_REMOVESPOTSFB] = new CTWAINContainerFix32Range(UDSCAP_REMOVESPOTSFB,fRange, TWQC_ALL);
	if( NULL == dynamic_cast<CTWAINContainerFix32Range*>(m_IndependantCapMap[UDSCAP_REMOVESPOTSFB]))
	{
		::MessageBox(g_hwndDLG,TEXT("Could not create UDSCAP_REMOVESPOTSFB !"),MB_CAPTION,MB_OK);
		setConditionCode(TWCC_LOWMEMORY);
		return TWRC_FAILURE;
	}
	m_IndependantCapMap[UDSCAP_REMOVESPOTSBB] = new CTWAINContainerFix32Range(UDSCAP_REMOVESPOTSBB,fRange, TWQC_ALL);
	if( NULL == dynamic_cast<CTWAINContainerFix32Range*>(m_IndependantCapMap[UDSCAP_REMOVESPOTSBB]))
	{
		::MessageBox(g_hwndDLG,TEXT("Could not create UDSCAP_REMOVESPOTSBB !"),MB_CAPTION,MB_OK);
		setConditionCode(TWCC_LOWMEMORY);
		return TWRC_FAILURE;
	}

  fRange.fCurrentValue = 0.0f;
  fRange.fMaxValue = 100.0f;
  fRange.fMinValue = -100.0f;
  fRange.fStepSize = 1.0f;
  m_IndependantCapMap[ICAP_CONTRAST] = new CTWAINContainerFix32Range(ICAP_CONTRAST,fRange, TWQC_ALL);
  if( NULL == dynamic_cast<CTWAINContainerFix32Range*>(m_IndependantCapMap[ICAP_CONTRAST]))
  {
		::MessageBox(g_hwndDLG,TEXT("Could not create ICAP_CONTRAST !"),MB_CAPTION,MB_OK);
    //cerr << "Could not create ICAP_CONTRAST" << endl;
    setConditionCode(TWCC_LOWMEMORY);
    return TWRC_FAILURE;
  }
	//多流对比度
	m_IndependantCapMap[UDSCAP_CONTRASTFC] = new CTWAINContainerFix32Range(UDSCAP_CONTRASTFC,fRange, TWQC_ALL);
	if( NULL == dynamic_cast<CTWAINContainerFix32Range*>(m_IndependantCapMap[UDSCAP_CONTRASTFC]))
	{
		::MessageBox(g_hwndDLG,TEXT("Could not create UDSCAP_CONTRASTFC !"),MB_CAPTION,MB_OK);
		setConditionCode(TWCC_LOWMEMORY);
		return TWRC_FAILURE;
	}
	m_IndependantCapMap[UDSCAP_CONTRASTFG] = new CTWAINContainerFix32Range(UDSCAP_CONTRASTFG,fRange, TWQC_ALL);
	if( NULL == dynamic_cast<CTWAINContainerFix32Range*>(m_IndependantCapMap[UDSCAP_CONTRASTFG]))
	{
		::MessageBox(g_hwndDLG,TEXT("Could not create UDSCAP_CONTRASTFG !"),MB_CAPTION,MB_OK);
		setConditionCode(TWCC_LOWMEMORY);
		return TWRC_FAILURE;
	}
	m_IndependantCapMap[UDSCAP_CONTRASTBC] = new CTWAINContainerFix32Range(UDSCAP_CONTRASTBC,fRange, TWQC_ALL);
	if( NULL == dynamic_cast<CTWAINContainerFix32Range*>(m_IndependantCapMap[UDSCAP_CONTRASTBC]))
	{
		::MessageBox(g_hwndDLG,TEXT("Could not create UDSCAP_CONTRASTBC !"),MB_CAPTION,MB_OK);
		setConditionCode(TWCC_LOWMEMORY);
		return TWRC_FAILURE;
	}
	m_IndependantCapMap[UDSCAP_CONTRASTBG] = new CTWAINContainerFix32Range(UDSCAP_CONTRASTBG,fRange, TWQC_ALL);
	if( NULL == dynamic_cast<CTWAINContainerFix32Range*>(m_IndependantCapMap[UDSCAP_CONTRASTBG]))
	{
		::MessageBox(g_hwndDLG,TEXT("Could not create UDSCAP_CONTRASTBG !"),MB_CAPTION,MB_OK);
		setConditionCode(TWCC_LOWMEMORY);
		return TWRC_FAILURE;
	}

	m_IndependantCapMap[ICAP_BRIGHTNESS] = new CTWAINContainerFix32Range(ICAP_BRIGHTNESS,fRange, TWQC_ALL);
  if( NULL == dynamic_cast<CTWAINContainerFix32Range*>(m_IndependantCapMap[ICAP_BRIGHTNESS]))
  {
		::MessageBox(g_hwndDLG,TEXT("Could not create ICAP_BRIGHTNESS !"),MB_CAPTION,MB_OK);
    //cerr << "Could not create ICAP_BRIGHTNESS" << endl;
    setConditionCode(TWCC_LOWMEMORY);
    return TWRC_FAILURE;
  }
	//多流亮度
	m_IndependantCapMap[UDSCAP_BRIGHTNESSFC] = new CTWAINContainerFix32Range(UDSCAP_BRIGHTNESSFC,fRange, TWQC_ALL);
	if( NULL == dynamic_cast<CTWAINContainerFix32Range*>(m_IndependantCapMap[UDSCAP_BRIGHTNESSFC]))
	{
		::MessageBox(g_hwndDLG,TEXT("Could not create UDSCAP_FRONTCOLORBRIGHT !"),MB_CAPTION,MB_OK);
		//cerr << "Could not create ICAP_BRIGHTNESS" << endl;
		setConditionCode(TWCC_LOWMEMORY);
		return TWRC_FAILURE;
	}
	m_IndependantCapMap[UDSCAP_BRIGHTNESSFG] = new CTWAINContainerFix32Range(UDSCAP_BRIGHTNESSFG,fRange, TWQC_ALL);
	if( NULL == dynamic_cast<CTWAINContainerFix32Range*>(m_IndependantCapMap[UDSCAP_BRIGHTNESSFG]))
	{
		::MessageBox(g_hwndDLG,TEXT("Could not create UDSCAP_FRONTGRAYBRIGHT !"),MB_CAPTION,MB_OK);
		//cerr << "Could not create ICAP_BRIGHTNESS" << endl;
		setConditionCode(TWCC_LOWMEMORY);
		return TWRC_FAILURE;
	}
	m_IndependantCapMap[UDSCAP_BRIGHTNESSFB] = new CTWAINContainerFix32Range(UDSCAP_BRIGHTNESSFB,fRange, TWQC_ALL);
	if( NULL == dynamic_cast<CTWAINContainerFix32Range*>(m_IndependantCapMap[UDSCAP_BRIGHTNESSFB]))
	{
		::MessageBox(g_hwndDLG,TEXT("Could not create UDSCAP_FRONTBWBRIGHT !"),MB_CAPTION,MB_OK);
		//cerr << "Could not create ICAP_BRIGHTNESS" << endl;
		setConditionCode(TWCC_LOWMEMORY);
		return TWRC_FAILURE;
	}
	m_IndependantCapMap[UDSCAP_BRIGHTNESSBC] = new CTWAINContainerFix32Range(UDSCAP_BRIGHTNESSBC,fRange, TWQC_ALL);
	if( NULL == dynamic_cast<CTWAINContainerFix32Range*>(m_IndependantCapMap[UDSCAP_BRIGHTNESSBC]))
	{
		::MessageBox(g_hwndDLG,TEXT("Could not create UDSCAP_BACKCOLORBRIGHT !"),MB_CAPTION,MB_OK);
		//cerr << "Could not create ICAP_BRIGHTNESS" << endl;
		setConditionCode(TWCC_LOWMEMORY);
		return TWRC_FAILURE;
	}
	m_IndependantCapMap[UDSCAP_BRIGHTNESSBG] = new CTWAINContainerFix32Range(UDSCAP_BRIGHTNESSBG,fRange, TWQC_ALL);
	if( NULL == dynamic_cast<CTWAINContainerFix32Range*>(m_IndependantCapMap[UDSCAP_BRIGHTNESSBG]))
	{
		::MessageBox(g_hwndDLG,TEXT("Could not create UDSCAP_BACKGRAYBRIGHT !"),MB_CAPTION,MB_OK);
		//cerr << "Could not create ICAP_BRIGHTNESS" << endl;
		setConditionCode(TWCC_LOWMEMORY);
		return TWRC_FAILURE;
	}
	m_IndependantCapMap[UDSCAP_BRIGHTNESSBB] = new CTWAINContainerFix32Range(UDSCAP_BRIGHTNESSBB,fRange, TWQC_ALL);
	if( NULL == dynamic_cast<CTWAINContainerFix32Range*>(m_IndependantCapMap[UDSCAP_BRIGHTNESSBB]))
	{
		::MessageBox(g_hwndDLG,TEXT("Could not create UDSCAP_BACKBWBRIGHT !"),MB_CAPTION,MB_OK);
		//cerr << "Could not create ICAP_BRIGHTNESS" << endl;
		setConditionCode(TWCC_LOWMEMORY);
		return TWRC_FAILURE;
	}

  // expressed internally as pixels per inch
  if( NULL == (m_ICAP_UNIT_Dependant[ICAP_XRESOLUTION] = new CTWAINContainerFix32(ICAP_XRESOLUTION, TWON_ENUMERATION, TWQC_ALL))
   || !m_ICAP_UNIT_Dependant[ICAP_XRESOLUTION]->Add(50) 
   || !m_ICAP_UNIT_Dependant[ICAP_XRESOLUTION]->Add(75)
   || !m_ICAP_UNIT_Dependant[ICAP_XRESOLUTION]->Add(100)
   || !m_ICAP_UNIT_Dependant[ICAP_XRESOLUTION]->Add(150)
   || !m_ICAP_UNIT_Dependant[ICAP_XRESOLUTION]->Add(200, true)  //分辨率，默认200
   || !m_ICAP_UNIT_Dependant[ICAP_XRESOLUTION]->Add(300)
   || !m_ICAP_UNIT_Dependant[ICAP_XRESOLUTION]->Add(400)
   || !m_ICAP_UNIT_Dependant[ICAP_XRESOLUTION]->Add(500)
   || !m_ICAP_UNIT_Dependant[ICAP_XRESOLUTION]->Add(600))
  {
		::MessageBox(g_hwndDLG,TEXT("Could not create ICAP_XRESOLUTION !"),MB_CAPTION,MB_OK);
    //cerr << "Could not create ICAP_XRESOLUTION" << endl;
    setConditionCode(TWCC_LOWMEMORY);
    return TWRC_FAILURE;
  }

  // expressed internally as pixels per inch
  if( NULL == (m_ICAP_UNIT_Dependant[ICAP_YRESOLUTION] = new CTWAINContainerFix32(ICAP_YRESOLUTION, TWON_ENUMERATION, TWQC_ALL))
   || !m_ICAP_UNIT_Dependant[ICAP_YRESOLUTION]->Add(50)
   || !m_ICAP_UNIT_Dependant[ICAP_YRESOLUTION]->Add(75)
   || !m_ICAP_UNIT_Dependant[ICAP_YRESOLUTION]->Add(100)
   || !m_ICAP_UNIT_Dependant[ICAP_YRESOLUTION]->Add(150)
   || !m_ICAP_UNIT_Dependant[ICAP_YRESOLUTION]->Add(200, true) //分辨率,默认200
   || !m_ICAP_UNIT_Dependant[ICAP_YRESOLUTION]->Add(300)
   || !m_ICAP_UNIT_Dependant[ICAP_YRESOLUTION]->Add(400)
   || !m_ICAP_UNIT_Dependant[ICAP_YRESOLUTION]->Add(500)
   || !m_ICAP_UNIT_Dependant[ICAP_YRESOLUTION]->Add(600))
  {
		::MessageBox(g_hwndDLG,TEXT("Could not create ICAP_YRESOLUTION !"),MB_CAPTION,MB_OK);
    //cerr << "Could not create ICAP_YRESOLUTION" << endl;
    setConditionCode(TWCC_LOWMEMORY);
    return TWRC_FAILURE;
  }
	//多流分辨率
	if( NULL == (m_ICAP_UNIT_Dependant[UDSCAP_RESOLUTIONFC] = new CTWAINContainerFix32(UDSCAP_RESOLUTIONFC, TWON_ENUMERATION, TWQC_ALL))
		|| !m_ICAP_UNIT_Dependant[UDSCAP_RESOLUTIONFC]->Add(50) 
		|| !m_ICAP_UNIT_Dependant[UDSCAP_RESOLUTIONFC]->Add(75)
		|| !m_ICAP_UNIT_Dependant[UDSCAP_RESOLUTIONFC]->Add(100)
		|| !m_ICAP_UNIT_Dependant[UDSCAP_RESOLUTIONFC]->Add(150)
		|| !m_ICAP_UNIT_Dependant[UDSCAP_RESOLUTIONFC]->Add(200, true)  //分辨率，默认200
		|| !m_ICAP_UNIT_Dependant[UDSCAP_RESOLUTIONFC]->Add(300)
		|| !m_ICAP_UNIT_Dependant[UDSCAP_RESOLUTIONFC]->Add(400)
		|| !m_ICAP_UNIT_Dependant[UDSCAP_RESOLUTIONFC]->Add(500)
		|| !m_ICAP_UNIT_Dependant[UDSCAP_RESOLUTIONFC]->Add(600))
	{
		::MessageBox(g_hwndDLG,TEXT("Could not create UDSCAP_RESOLUTIONFC !"),MB_CAPTION,MB_OK);
		setConditionCode(TWCC_LOWMEMORY);
		return TWRC_FAILURE;
	}
	if( NULL == (m_ICAP_UNIT_Dependant[UDSCAP_RESOLUTIONFG] = new CTWAINContainerFix32(UDSCAP_RESOLUTIONFG, TWON_ENUMERATION, TWQC_ALL))
		|| !m_ICAP_UNIT_Dependant[UDSCAP_RESOLUTIONFG]->Add(50) 
		|| !m_ICAP_UNIT_Dependant[UDSCAP_RESOLUTIONFG]->Add(75)
		|| !m_ICAP_UNIT_Dependant[UDSCAP_RESOLUTIONFG]->Add(100)
		|| !m_ICAP_UNIT_Dependant[UDSCAP_RESOLUTIONFG]->Add(150)
		|| !m_ICAP_UNIT_Dependant[UDSCAP_RESOLUTIONFG]->Add(200, true)  //分辨率，默认200
		|| !m_ICAP_UNIT_Dependant[UDSCAP_RESOLUTIONFG]->Add(300)
		|| !m_ICAP_UNIT_Dependant[UDSCAP_RESOLUTIONFG]->Add(400)
		|| !m_ICAP_UNIT_Dependant[UDSCAP_RESOLUTIONFG]->Add(500)
		|| !m_ICAP_UNIT_Dependant[UDSCAP_RESOLUTIONFG]->Add(600))
	{
		::MessageBox(g_hwndDLG,TEXT("Could not create UDSCAP_RESOLUTIONFG !"),MB_CAPTION,MB_OK);
		setConditionCode(TWCC_LOWMEMORY);
		return TWRC_FAILURE;
	}
	if( NULL == (m_ICAP_UNIT_Dependant[UDSCAP_RESOLUTIONFB] = new CTWAINContainerFix32(UDSCAP_RESOLUTIONFB, TWON_ENUMERATION, TWQC_ALL))
		|| !m_ICAP_UNIT_Dependant[UDSCAP_RESOLUTIONFB]->Add(50) 
		|| !m_ICAP_UNIT_Dependant[UDSCAP_RESOLUTIONFB]->Add(75)
		|| !m_ICAP_UNIT_Dependant[UDSCAP_RESOLUTIONFB]->Add(100)
		|| !m_ICAP_UNIT_Dependant[UDSCAP_RESOLUTIONFB]->Add(150)
		|| !m_ICAP_UNIT_Dependant[UDSCAP_RESOLUTIONFB]->Add(200, true)  //分辨率，默认200
		|| !m_ICAP_UNIT_Dependant[UDSCAP_RESOLUTIONFB]->Add(300)
		|| !m_ICAP_UNIT_Dependant[UDSCAP_RESOLUTIONFB]->Add(400)
		|| !m_ICAP_UNIT_Dependant[UDSCAP_RESOLUTIONFB]->Add(500)
		|| !m_ICAP_UNIT_Dependant[UDSCAP_RESOLUTIONFB]->Add(600))
	{
		::MessageBox(g_hwndDLG,TEXT("Could not create UDSCAP_RESOLUTIONFB !"),MB_CAPTION,MB_OK);
		setConditionCode(TWCC_LOWMEMORY);
		return TWRC_FAILURE;
	}
	if( NULL == (m_ICAP_UNIT_Dependant[UDSCAP_RESOLUTIONBC] = new CTWAINContainerFix32(UDSCAP_RESOLUTIONBC, TWON_ENUMERATION, TWQC_ALL))
		|| !m_ICAP_UNIT_Dependant[UDSCAP_RESOLUTIONBC]->Add(50) 
		|| !m_ICAP_UNIT_Dependant[UDSCAP_RESOLUTIONBC]->Add(75)
		|| !m_ICAP_UNIT_Dependant[UDSCAP_RESOLUTIONBC]->Add(100)
		|| !m_ICAP_UNIT_Dependant[UDSCAP_RESOLUTIONBC]->Add(150)
		|| !m_ICAP_UNIT_Dependant[UDSCAP_RESOLUTIONBC]->Add(200, true)  //分辨率，默认200
		|| !m_ICAP_UNIT_Dependant[UDSCAP_RESOLUTIONBC]->Add(300)
		|| !m_ICAP_UNIT_Dependant[UDSCAP_RESOLUTIONBC]->Add(400)
		|| !m_ICAP_UNIT_Dependant[UDSCAP_RESOLUTIONBC]->Add(500)
		|| !m_ICAP_UNIT_Dependant[UDSCAP_RESOLUTIONBC]->Add(600))
	{
		::MessageBox(g_hwndDLG,TEXT("Could not create UDSCAP_RESOLUTIONBC !"),MB_CAPTION,MB_OK);
		setConditionCode(TWCC_LOWMEMORY);
		return TWRC_FAILURE;
	}
	if( NULL == (m_ICAP_UNIT_Dependant[UDSCAP_RESOLUTIONBG] = new CTWAINContainerFix32(UDSCAP_RESOLUTIONBG, TWON_ENUMERATION, TWQC_ALL))
		|| !m_ICAP_UNIT_Dependant[UDSCAP_RESOLUTIONBG]->Add(50) 
		|| !m_ICAP_UNIT_Dependant[UDSCAP_RESOLUTIONBG]->Add(75)
		|| !m_ICAP_UNIT_Dependant[UDSCAP_RESOLUTIONBG]->Add(100)
		|| !m_ICAP_UNIT_Dependant[UDSCAP_RESOLUTIONBG]->Add(150)
		|| !m_ICAP_UNIT_Dependant[UDSCAP_RESOLUTIONBG]->Add(200, true)  //分辨率，默认200
		|| !m_ICAP_UNIT_Dependant[UDSCAP_RESOLUTIONBG]->Add(300)
		|| !m_ICAP_UNIT_Dependant[UDSCAP_RESOLUTIONBG]->Add(400)
		|| !m_ICAP_UNIT_Dependant[UDSCAP_RESOLUTIONBG]->Add(500)
		|| !m_ICAP_UNIT_Dependant[UDSCAP_RESOLUTIONBG]->Add(600))
	{
		::MessageBox(g_hwndDLG,TEXT("Could not create UDSCAP_RESOLUTIONBG !"),MB_CAPTION,MB_OK);
		setConditionCode(TWCC_LOWMEMORY);
		return TWRC_FAILURE;
	}
	if( NULL == (m_ICAP_UNIT_Dependant[UDSCAP_RESOLUTIONBB] = new CTWAINContainerFix32(UDSCAP_RESOLUTIONBB, TWON_ENUMERATION, TWQC_ALL))
		|| !m_ICAP_UNIT_Dependant[UDSCAP_RESOLUTIONBB]->Add(50) 
		|| !m_ICAP_UNIT_Dependant[UDSCAP_RESOLUTIONBB]->Add(75)
		|| !m_ICAP_UNIT_Dependant[UDSCAP_RESOLUTIONBB]->Add(100)
		|| !m_ICAP_UNIT_Dependant[UDSCAP_RESOLUTIONBB]->Add(150)
		|| !m_ICAP_UNIT_Dependant[UDSCAP_RESOLUTIONBB]->Add(200, true)  //分辨率，默认200
		|| !m_ICAP_UNIT_Dependant[UDSCAP_RESOLUTIONBB]->Add(300)
		|| !m_ICAP_UNIT_Dependant[UDSCAP_RESOLUTIONBB]->Add(400)
		|| !m_ICAP_UNIT_Dependant[UDSCAP_RESOLUTIONBB]->Add(500)
		|| !m_ICAP_UNIT_Dependant[UDSCAP_RESOLUTIONBB]->Add(600))
	{
		::MessageBox(g_hwndDLG,TEXT("Could not create UDSCAP_RESOLUTIONBB !"),MB_CAPTION,MB_OK);
		setConditionCode(TWCC_LOWMEMORY);
		return TWRC_FAILURE;
	}

  // expressed internally as 1000 pixels per inch
  /// @todo Get physical dimentions from scanner
  /// @todo Add seperate dimentions for ADF and Flatbed
  //  Flatbed   - A4 letter paper
  //  ConvertUnits(29.7f, TWUN_CENTIMETERS, TWUN_INCHES, 1000);
  //  ConvertUnits(21.0f, TWUN_CENTIMETERS, TWUN_INCHES, 1000);
  if( NULL == (m_ICAP_UNIT_Dependant[ICAP_PHYSICALWIDTH] = new CTWAINContainerFix32(ICAP_PHYSICALWIDTH, TWON_ONEVALUE, TWQC_GETS))
   || !m_ICAP_UNIT_Dependant[ICAP_PHYSICALWIDTH]->Add(8.5, true) )
  {
		::MessageBox(g_hwndDLG,TEXT("Could not create ICAP_PHYSICALWIDTH !"),MB_CAPTION,MB_OK);
    //cerr << "Could not create ICAP_PHYSICALWIDTH" << endl;
    setConditionCode(TWCC_LOWMEMORY);
    return TWRC_FAILURE;
  }

  if( NULL == (m_ICAP_UNIT_Dependant[ICAP_PHYSICALHEIGHT] = new CTWAINContainerFix32(ICAP_PHYSICALHEIGHT, TWON_ONEVALUE, TWQC_GETS))
   || !m_ICAP_UNIT_Dependant[ICAP_PHYSICALHEIGHT]->Add(14.0, true) )
  {
		::MessageBox(g_hwndDLG,TEXT("Could not create ICAP_PHYSICALHEIGHT !"),MB_CAPTION,MB_OK);
    //cerr << "Could not create ICAP_PHYSICALHEIGHT" << endl;
    setConditionCode(TWCC_LOWMEMORY);
    return TWRC_FAILURE;
  }
	
	// 多流输出想选项值
	if( NULL == (m_ICAP_UNIT_Dependant[UDSCAP_MULTISTREAM_VALUE] = new CTWAINContainerFix32(UDSCAP_MULTISTREAM_VALUE, TWON_ONEVALUE, TWQC_ALL))
		|| !m_ICAP_UNIT_Dependant[UDSCAP_MULTISTREAM_VALUE]->Add(0.0, true) )
	{
		::MessageBox(g_hwndDLG,TEXT("Could not create UDSCAP_MULTISTREAM_VALUE !"),MB_CAPTION,MB_OK);
		setConditionCode(TWCC_LOWMEMORY);
		return TWRC_FAILURE;
	}

	//边缘扩展上下左右
	fRange.fCurrentValue = 0.00f; 
	fRange.fMaxValue = 0.50f;
	fRange.fMinValue = 0.00f;
	fRange.fStepSize = 0.01f;
	m_IndependantCapMap[UDSCAP_EDGE_UP] = new CTWAINContainerFix32Range(UDSCAP_EDGE_UP,fRange, TWQC_ALL);
	if( NULL == dynamic_cast<CTWAINContainerFix32Range*>(m_IndependantCapMap[UDSCAP_EDGE_UP]))
	{
		::MessageBox(g_hwndDLG,TEXT("Could not create UDSCAP_EDGE_UP !"),MB_CAPTION,MB_OK);
		setConditionCode(TWCC_LOWMEMORY);
		return TWRC_FAILURE;
	}
	m_IndependantCapMap[UDSCAP_EDGE_DOWN] = new CTWAINContainerFix32Range(UDSCAP_EDGE_DOWN,fRange, TWQC_ALL);
	if( NULL == dynamic_cast<CTWAINContainerFix32Range*>(m_IndependantCapMap[UDSCAP_EDGE_DOWN]))
	{
		::MessageBox(g_hwndDLG,TEXT("Could not create UDSCAP_EDGE_DOWN !"),MB_CAPTION,MB_OK);
		setConditionCode(TWCC_LOWMEMORY);
		return TWRC_FAILURE;
	}

	fRange.fCurrentValue = 0.00f; 
	fRange.fMaxValue = 0.19f;
	fRange.fMinValue = 0.00f;
	fRange.fStepSize = 0.01f;
	m_IndependantCapMap[UDSCAP_EDGE_LEFT] = new CTWAINContainerFix32Range(UDSCAP_EDGE_LEFT,fRange, TWQC_ALL);
	if( NULL == dynamic_cast<CTWAINContainerFix32Range*>(m_IndependantCapMap[UDSCAP_EDGE_LEFT]))
	{
		::MessageBox(g_hwndDLG,TEXT("Could not create UDSCAP_EDGE_LEFT !"),MB_CAPTION,MB_OK);
		setConditionCode(TWCC_LOWMEMORY);
		return TWRC_FAILURE;
	}
	m_IndependantCapMap[UDSCAP_EDGE_RIGHT] = new CTWAINContainerFix32Range(UDSCAP_EDGE_RIGHT,fRange, TWQC_ALL);
	if( NULL == dynamic_cast<CTWAINContainerFix32Range*>(m_IndependantCapMap[UDSCAP_EDGE_RIGHT]))
	{
		::MessageBox(g_hwndDLG,TEXT("Could not create UDSCAP_EDGE_RIGHT !"),MB_CAPTION,MB_OK);
		setConditionCode(TWCC_LOWMEMORY);
		return TWRC_FAILURE;
	}

	//XY偏移量
	fRange.fCurrentValue = 0.00f; 
	fRange.fMaxValue = 5.73f;
	fRange.fMinValue = 0.00f;
	fRange.fStepSize = 0.01f;
	m_IndependantCapMap[UDSCAP_XPOS] = new CTWAINContainerFix32Range(UDSCAP_XPOS,fRange, TWQC_ALL);
	if( NULL == dynamic_cast<CTWAINContainerFix32Range*>(m_IndependantCapMap[UDSCAP_XPOS]))
	{
		::MessageBox(g_hwndDLG,TEXT("Could not create UDSCAP_XPOS !"),MB_CAPTION,MB_OK);
		setConditionCode(TWCC_LOWMEMORY);
		return TWRC_FAILURE;
	}
	m_IndependantCapMap[UDSCAP_YPOS] = new CTWAINContainerFix32Range(UDSCAP_YPOS,fRange, TWQC_ALL);
	if( NULL == dynamic_cast<CTWAINContainerFix32Range*>(m_IndependantCapMap[UDSCAP_YPOS]))
	{
		::MessageBox(g_hwndDLG,TEXT("Could not create UDSCAP_YPOS !"),MB_CAPTION,MB_OK);
		setConditionCode(TWCC_LOWMEMORY);
		return TWRC_FAILURE;
	}

	
	if( NULL == (m_ICAP_UNIT_Dependant[UDSCAP_EDGE_UP] = new CTWAINContainerFix32(UDSCAP_EDGE_UP, TWON_ONEVALUE, TWQC_ALL))
		|| !m_ICAP_UNIT_Dependant[UDSCAP_EDGE_UP]->Add(0.0, true) )
	{
		::MessageBox(g_hwndDLG,TEXT("Could not create UDSCAP_EDGE_UP !"),MB_CAPTION,MB_OK);
		setConditionCode(TWCC_LOWMEMORY);
		return TWRC_FAILURE;
	}

	if( NULL == (m_ICAP_UNIT_Dependant[UDSCAP_EDGE_DOWN] = new CTWAINContainerFix32(UDSCAP_EDGE_DOWN, TWON_ONEVALUE, TWQC_ALL))
		|| !m_ICAP_UNIT_Dependant[UDSCAP_EDGE_DOWN]->Add(0.0, true) )
	{
		::MessageBox(g_hwndDLG,TEXT("Could not create UDSCAP_EDGE_DOWN !"),MB_CAPTION,MB_OK);
		setConditionCode(TWCC_LOWMEMORY);
		return TWRC_FAILURE;
	}

	if( NULL == (m_ICAP_UNIT_Dependant[UDSCAP_EDGE_LEFT] = new CTWAINContainerFix32(UDSCAP_EDGE_LEFT, TWON_ONEVALUE, TWQC_ALL))
		|| !m_ICAP_UNIT_Dependant[UDSCAP_EDGE_LEFT]->Add(0.0, true) )
	{
		::MessageBox(g_hwndDLG,TEXT("Could not create UDSCAP_EDGE_LEFT !"),MB_CAPTION,MB_OK);
		setConditionCode(TWCC_LOWMEMORY);
		return TWRC_FAILURE;
	}

	if( NULL == (m_ICAP_UNIT_Dependant[UDSCAP_EDGE_RIGHT] = new CTWAINContainerFix32(UDSCAP_EDGE_RIGHT, TWON_ONEVALUE, TWQC_ALL))
		|| !m_ICAP_UNIT_Dependant[UDSCAP_EDGE_RIGHT]->Add(0.0, true) )
	{
		::MessageBox(g_hwndDLG,TEXT("Could not create UDSCAP_EDGE_RIGHT !"),MB_CAPTION,MB_OK);
		setConditionCode(TWCC_LOWMEMORY);
		return TWRC_FAILURE;
	}

	if( NULL == (m_ICAP_UNIT_Dependant[UDSCAP_XPOS] = new CTWAINContainerFix32(UDSCAP_XPOS, TWON_ONEVALUE, TWQC_ALL))
		|| !m_ICAP_UNIT_Dependant[UDSCAP_XPOS]->Add(0.0, true) )
	{
		::MessageBox(g_hwndDLG,TEXT("Could not create UDSCAP_XPOS !"),MB_CAPTION,MB_OK);
		setConditionCode(TWCC_LOWMEMORY);
		return TWRC_FAILURE;
	}

	if( NULL == (m_ICAP_UNIT_Dependant[UDSCAP_YPOS] = new CTWAINContainerFix32(UDSCAP_YPOS, TWON_ONEVALUE, TWQC_ALL))
		|| !m_ICAP_UNIT_Dependant[UDSCAP_YPOS]->Add(0.0, true) )
	{
		::MessageBox(g_hwndDLG,TEXT("Could not create UDSCAP_XPOS !"),MB_CAPTION,MB_OK);
		setConditionCode(TWCC_LOWMEMORY);
		return TWRC_FAILURE;
	}

  // setup the ICAP_FRAMES
  // expressed internally as 1000 pixels per inch ；1000像素没英寸
  // Currently only supports one frame see: ICAP_MAXFRAMES
  if( NULL == (m_pICAP_FRAMES = new CTWAINContainerFrame(ICAP_FRAMES, TWON_ENUMERATION, TWQC_ALL))
   || !m_pICAP_FRAMES->Add(0, 0, 8500, 11000, true) //8.5*11 英寸 US Letter
	 )   	
  {
		::MessageBox(g_hwndDLG,TEXT("Could not create ICAP_FRAMES !"),MB_CAPTION,MB_OK);
    //cerr << "Could not create ICAP_FRAMES" << endl;
    setConditionCode(TWCC_LOWMEMORY);
    return TWRC_FAILURE;
  }

  int   unit;
  float Xres, Yres;
  if(getCurrentUnits(unit, Xres, Yres)==TWRC_SUCCESS)
  {
    m_pICAP_FRAMES->setCurrentUnits(unit, Xres, Yres);
  }
  else
  {
		::MessageBox(g_hwndDLG,TEXT("Could not getCurrentUnits !"),MB_CAPTION,MB_OK);
    //cerr << "Could not getCurrentUnits" << endl;
    setConditionCode(TWCC_BUMMER);
    return TWRC_FAILURE;
   }
	//::MessageBox(g_hwndDLG,TEXT("CTWAINDS_UDS::Initialize end"),MB_CAPTION,MB_OK);
  return TWRC_SUCCESS;
}


//////////////////////////////////////////////////////////////////////////////
CTWAINDS_UDS::~CTWAINDS_UDS()
{//::MessageBox(g_hwndDLG,"~CTWAINDS_UDS",MB_CAPTION,MB_OK);
  DestroyUI(m_pGUI);
	
  // free all resources belonging to m_IndependantCapMap
  TWAINCapabilitiesMap_int::iterator cur_int = m_BitDepthMap.begin();
  while(cur_int != m_BitDepthMap.end())
  {
    delete cur_int->second;
    cur_int++;
  }

  TWAINCapabilitiesMap_FIX32::iterator cur_Fix32 = m_ICAP_UNIT_Dependant.begin();
  while(cur_Fix32 != m_ICAP_UNIT_Dependant.end())
  {
    delete cur_Fix32->second;
    cur_Fix32++;
  }

  if(m_pICAP_FRAMES)
  {
    delete m_pICAP_FRAMES;
  }
  m_pICAP_FRAMES = 0;

	// 释放指针
	if (m_pScanner)
	{
		delete m_pScanner;
		m_pScanner = NULL;
	}

  return;
}

//////////////////////////////////////////////////////////////////////////////
void CTWAINDS_UDS::fillIdentityStructure(TW_IDENTITY& _idStruct)
{//::MessageBox(g_hwndDLG,"fillIdentityStructure",MB_CAPTION,MB_OK);
  _idStruct = m_TheIdentity;
}

//////////////////////////////////////////////////////////////////////////////
TW_INT16 CTWAINDS_UDS::getImageInfo(pTW_IMAGEINFO _pImageInfo)
{
  TW_INT16 twrc = TWRC_SUCCESS;

  memset(_pImageInfo, 0, sizeof(TW_IMAGEINFO));

  // Only valid in state 6 and 7(State 7 only after receiving TWRC_XFERDONE)
  if( dsState_XferReady > m_CurrentState )
  {
    setConditionCode(TWCC_SEQERROR);
    return TWRC_FAILURE;
  }

  // Get the actual values used by the scanner.
  CDevice_Base settings = *(m_pScanner->getSetting());

  _pImageInfo->XResolution = FloatToFIX32(settings.m_fXResolution);
  _pImageInfo->YResolution = FloatToFIX32(settings.m_fYResolution);
  _pImageInfo->ImageWidth  = settings.m_nWidth; //1700
  _pImageInfo->ImageLength = settings.m_nHeight; //2200

  // Our sample scanner only does one combination for each PixelType.
  switch(settings.m_nPixelType)
  {
    case TWPT_BW:
      _pImageInfo->PixelType = TWPT_BW;
      //_pImageInfo->BitsPerPixel = 1;
      _pImageInfo->SamplesPerPixel = 1;
      _pImageInfo->BitsPerSample[0] = 1;
			/*if (DEVICE_OPENCV == g_nDeviceNumber)
			{
				_pImageInfo->BitsPerPixel = 8;
			} 
			else*/
			{
				_pImageInfo->BitsPerPixel = 8;
			}
    break;

    case TWPT_GRAY:
      _pImageInfo->PixelType = TWPT_GRAY;
      _pImageInfo->BitsPerPixel = 8;
      _pImageInfo->SamplesPerPixel = 1;
      _pImageInfo->BitsPerSample[0] = 8;
    break;

    case TWPT_RGB:
      _pImageInfo->PixelType = TWPT_RGB;
      _pImageInfo->BitsPerPixel = 24;
      _pImageInfo->SamplesPerPixel = 3;
      _pImageInfo->BitsPerSample[0] = 8;
      _pImageInfo->BitsPerSample[1] = 8;
      _pImageInfo->BitsPerSample[2] = 8;
    break;
  }

	_pImageInfo->Planar = FALSE;
	_pImageInfo->Compression = TWCP_NONE;
	
  return twrc;
}

//////////////////////////////////////////////////////////////////////////////
TW_INT16 CTWAINDS_UDS::openDS(pTW_IDENTITY  _pOrigin)
{
  TW_INT16 ret = TWRC_SUCCESS;
  // this basic version of the DS only supports one connection from the DSM
  if( m_App.Id != 0 )
  {
    setConditionCode(TWCC_MAXCONNECTIONS);
    return TWRC_FAILURE;
  }

  if( dsState_Loaded != m_CurrentState )
  {
    setConditionCode(TWCC_SEQERROR);
    return TWRC_FAILURE;
  }

  // store the pointer of the originating scan requestor.
  m_App = *_pOrigin;

  m_DocumentNumber = 0;
  m_PageNumber = 0;

  if(!m_pScanner->resetScanner())
  {	
    setConditionCode(TWCC_BUMMER);
    assert(0);
    ret = TWRC_FAILURE;
  }
  else
  {
		//::MessageBox(g_hwndDLG,"resetScanner() Success",MB_CAPTION,MB_OK);
    m_CurrentState = dsState_Open;
  }

	m_pGUI->TW_LoadProfileFromFile("上次使用模板");

  return ret;
}

//////////////////////////////////////////////////////////////////////////////
TW_INT16 CTWAINDS_UDS::closeDS()
{//::MessageBox(g_hwndDLG,"closeDS",MB_CAPTION,MB_OK);
  // check if ok to return success
  // Must be in state 4
  if( dsState_Open != m_CurrentState )
  {
    setConditionCode(TWCC_SEQERROR);
    return TWRC_FAILURE;
  }
  memset(&m_App, 0, sizeof(m_App));

	// 释放最后一张纸占用内存
	m_pScanner->Release();

  return TWRC_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////////
TW_INT16 CTWAINDS_UDS::enableDS(pTW_USERINTERFACE _pData)
{
	g_hwndDLG = (HWND)_pData->hParent;
  if( dsState_Open != m_CurrentState )
  {
    setConditionCode(TWCC_SEQERROR);
    return TWRC_FAILURE;
  }
  m_CurrentState = dsState_Enabled;
  m_bCanceled = false;

	// 防止有些APP再次扫描，未OpenDS,直接EnableDS，需要重置参数
	if(!m_pScanner->resetScanner())
	{	
		setConditionCode(TWCC_BUMMER);
		assert(0);
	}
  //set pending xfers to whatever the user configured for XferCount
  int Count = TWON_DONTCARE32;
  CTWAINContainerInt *pnCap = dynamic_cast<CTWAINContainerInt*>(findCapability(CAP_XFERCOUNT));
  if(pnCap)
  {
    pnCap->GetCurrent(Count);
  }

  m_Xfers.Count = Count;

  // Indicate we have not transferred any images yet
  m_DocumentNumber = 0;
  m_PageNumber     = 0;

  // The application will move to state 5 after this triplet which means that
  // no more capabilities can be set until we are brought back to state 4.
  m_pScanner->Lock();
	if (DEVICE_CAMERA != g_nDeviceNumber)  // Camera始终显示界面
	{
		if(FALSE == _pData->ShowUI)
		{   
			// Update the scanner with the latest negotiated caps
			//m_pGUI->TW_LoadProfileFromFile("上次使用模板");
			if(!updateScannerFromCaps())
			{
				//::MessageBox(g_hwndDLG,TEXT("enableDS::There was an error while prepping the image for scanning !"),MB_CAPTION,MB_OK);
				//cerr << "ds: There was an error while prepping the image for scanning" << endl;
				setConditionCode(TWCC_BADVALUE);
				return TWRC_FAILURE;
			}
			// The application will move to state 5 after this triplet which means that
			// no more capabilities can be set until we are brought back to state 4.
			m_pScanner->Lock();
			// Because there is no user interface, there isn't anything to show here.
			// But, at this point, the application is not allowed to set any more
			// capabilities.  This means that we can do any initializations we
			// need in order to prepare for the next few calls and the scan.
			// get the scanner to load the image so that image info calls can be done

			if(!m_pScanner->acquireImage())
			{
				//::MessageBox(g_hwndDLG,TEXT("enableDS::There was an error while trying to get scanner to acquire image!"),MB_CAPTION,MB_OK);
				//cerr << "ds: There was an error while trying to get scanner to acquire image" << endl;
				m_CurrentState = dsState_Open;
				setConditionCode(TWCC_SEQERROR);
				return TWRC_FAILURE;
			}

			if(! DoXferReadyEvent())
			{
				m_CurrentState = dsState_Open;
				setConditionCode(TWCC_SEQERROR);
				return TWRC_FAILURE;
			}
		}
	}
  CTWAINContainerBool *pbCap = dynamic_cast<CTWAINContainerBool*>(findCapability(CAP_INDICATORS));
  bool bIndicators = FALSE;
  if(pbCap)
  {
    pbCap->GetCurrent(bIndicators);
  }  
  if(m_pGUI->DisplayTWAINGUI(*_pData,false,bIndicators)!=TWRC_SUCCESS)
  {
    // A user interface is not supported as of right now because we are
    // in text mode.
    m_CurrentState = dsState_Open;
    setConditionCode(TWCC_OPERATIONERROR);
    return TWRC_FAILURE;
  }

  return TWRC_SUCCESS;
}
//////////////////////////////////////////////////////////////////////////////
TW_INT16 CTWAINDS_UDS::enableDSOnly()
{//::MessageBox(g_hwndDLG,TEXT("enableDSOnly"),MB_CAPTION,MB_OK);
  if( dsState_Open != m_CurrentState )
  {
    setConditionCode(TWCC_SEQERROR);
    return TWRC_FAILURE;
  }
  m_CurrentState = dsState_Enabled;

  TW_USERINTERFACE Data;
  memset(&Data,0,sizeof(TW_USERINTERFACE));
  Data.ShowUI = 1;

  if(m_pGUI->DisplayTWAINGUI(Data,true,false)!=TWRC_SUCCESS)
  {
    // A user interface is not supported as of right now because we are
    // in text mode.
    m_CurrentState = dsState_Open;
    setConditionCode(TWCC_OPERATIONERROR);
    return TWRC_FAILURE;
  }

  return TWRC_SUCCESS;
}
//////////////////////////////////////////////////////////////////////////////
TW_INT16 CTWAINDS_UDS::disableDS(pTW_USERINTERFACE _pData)
{//::MessageBox(g_hwndDLG,"disableDS",MB_CAPTION,MB_OK);
  if( dsState_Enabled != m_CurrentState )
  {
    setConditionCode(TWCC_SEQERROR);
    return TWRC_FAILURE;
  }
  m_pGUI->DestroyTWAINGUI();

  // allow the scanners caps to be writeable again because we are moving back
  // to state 4. If this source had a UI, it would be lowered at this time.
  m_pScanner->Unlock();

  
  // There is no UI in this text interface so there is nothing
  // to do here.
  m_CurrentState = dsState_Open;
  return TWRC_SUCCESS;
}
//////////////////////////////////////////////////////////////////////////////
TW_INT16 CTWAINDS_UDS::getMemoryXfer(pTW_SETUPMEMXFER _pData)
{//::MessageBox(g_hwndDLG,"getMemoryXfer",MB_CAPTION,MB_OK);
  // valid to call for 4, 5, & 6
  if( !(dsState_Open      == m_CurrentState ||
        dsState_Enabled   == m_CurrentState ||
        dsState_XferReady == m_CurrentState   ))
  {
    setConditionCode(TWCC_SEQERROR);
    return TWRC_FAILURE;
  }

  // Min is the physical width at highest res and hishest bits/pixel count
  TW_UINT32 bytes = (TW_UINT32)BYTES_PERLINE_ALIGN4(8.5*600, 24);
  _pData->MinBufSize = bytes;

  // chose 64k because this is a typical sweet spot for most real world scanners
  // (max transfer size for SCSI read of typical SCSI devices)
  _pData->MaxBufSize = MAX((unsigned long)64*1024, bytes);//bytes;
  _pData->Preferred  = MAX((unsigned long)64*1024, bytes);//bytes;

  // Update the min based on current settings.
  if(m_CurrentState == dsState_XferReady)
  {
    // If we have an image ready then we can use that info to get the memory requirements
    //get the number of bytes per line we'll need for this image, and make sure it's DWORD-aligned
    _pData->MinBufSize = BYTES_PERLINE_ALIGN4(m_ImageInfo.ImageWidth, m_ImageInfo.BitsPerPixel);
  }
  else
  {
    // calculate the min based on the current settings
    int                   nBitDepth     = 24;
    float                 fXResolution  = 600.0f;
    float                 fWidth        = 8.5f * fXResolution;
    CTWAINContainerInt   *pnCap         = 0;
    CTWAINContainerFix32 *pfCap         = 0;
    InternalFrame         frame;
   
    if(0 == (pnCap = dynamic_cast<CTWAINContainerInt*>(findCapability(ICAP_BITDEPTH))))
    {
			::MessageBox(g_hwndDLG,TEXT("Could not get ICAP_BITDEPTH!"),MB_CAPTION,MB_OK);
      //cerr << "Could not get ICAP_BITDEPTH" << endl;
      setConditionCode(TWCC_BUMMER);
      return TWRC_FAILURE;
    }
    else
    {
      pnCap->GetCurrent(nBitDepth);
    }

    if(0 == (pfCap = dynamic_cast<CTWAINContainerFix32*>(findCapability(ICAP_XRESOLUTION))))
    {
			::MessageBox(g_hwndDLG,TEXT("Could not get ICAP_XRESOLUTION!"),MB_CAPTION,MB_OK);
      //cerr << "Could not get ICAP_XRESOLUTION" << endl;
      setConditionCode(TWCC_BUMMER);
      return TWRC_FAILURE;
    }
    else
    {
      pfCap->GetCurrent(fXResolution);
    }

    if(!m_pICAP_FRAMES->GetCurrent(frame))
    {
			::MessageBox(g_hwndDLG,TEXT("Could not get ICAP_FRAMES!"),MB_CAPTION,MB_OK);
      //cerr << "Could not get ICAP_FRAMES" << endl;
      setConditionCode(TWCC_BUMMER);
      return TWRC_FAILURE;
    }
    else
    {
      // Get the scanning window by converting from our internal units to pixels.
      TW_FRAME TWframe = frame.AsTW_FRAME(TWUN_PIXELS, fXResolution, fXResolution);
      fWidth  = FIX32ToFloat(TWframe.Right) - FIX32ToFloat(TWframe.Left);
    }
    _pData->MinBufSize = (TW_UINT32)BYTES_PERLINE_ALIGN4(fWidth, nBitDepth);
  }

  return TWRC_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////////
TW_INT16 CTWAINDS_UDS::processEvent(pTW_EVENT _pEvent)
{//::MessageBox(g_hwndDLG,"processEvent",MB_CAPTION,MB_OK);
  TW_UINT16 twRc = TWRC_SUCCESS;

  if( dsState_Enabled > m_CurrentState )
  {
    setConditionCode(TWCC_SEQERROR);
    return TWRC_FAILURE;
  }

  if(0 == _pEvent)
  {
    setConditionCode(TWCC_BADVALUE);
    return TWRC_FAILURE;
  }

  if (m_pGUI && m_pGUI->processEvent(_pEvent))//hImageDlg && IsDialogMessage(hImageDlg, (LPMSG)(((pTW_EVENT)pData)->pEvent)))
  {
    twRc = TWRC_DSEVENT;
    // The source should, for proper form, return a MSG_NULL for
    // all Windows messages processed by the Data Source
    _pEvent->TWMessage = MSG_NULL;
  }
  else
  {
    // notify the application that the source did not
    // consume this message
    twRc = TWRC_NOTDSEVENT;
    _pEvent->TWMessage = MSG_NULL;
  }
  return twRc;
}


//////////////////////////////////////////////////////////////////////////////
TW_INT16 CTWAINDS_UDS::transfer()
{//::MessageBox(g_hwndDLG,"transfer",MB_CAPTION,MB_OK);
	getImageInfo(&m_ImageInfo); // 必须调用
  TW_INT16 twrc = TWRC_SUCCESS;
  if(m_bCanceled)
  {
    m_bCanceled = false;
    return TWRC_CANCEL;
  }
  if( dsState_XferReady != m_CurrentState )
  {
    setConditionCode(TWCC_SEQERROR);
    return TWRC_FAILURE;
  }
  m_nDestScanLine = 0;

  //This is for know image size.
  if(m_ImageInfo.ImageWidth>0 && m_ImageInfo.ImageLength>0)
  {
		DWORD nImageSize = 0;
		DWORD nDestBytesPerRow = 0;
		//if (DEVICE_CAMERA == g_nDeviceNumber)
		//{
		//	nImageSize = g_dwImageSize;
		//} 
		//else
		//{
			nDestBytesPerRow = BYTES_PERLINE(m_ImageInfo.ImageWidth, m_ImageInfo.BitsPerPixel);
			nImageSize       = nDestBytesPerRow * m_ImageInfo.ImageLength;
		//}
		
    //If we had a previous image then get rid of it.
    if(m_hImageData)
    {
      _DSM_Free(m_hImageData);
    }
    m_hImageData           = _DSM_Alloc(nImageSize);
    if(!m_hImageData)
    {
      setConditionCode(TWCC_LOWMEMORY);
      return TWRC_FAILURE;
    }

    TW_MEMREF   pData       = _DSM_LockMemory(m_hImageData);
    BYTE       *pImageData  = (BYTE*)pData;

		DWORD       dwRead;
		DWORD       dwReceived;

		switch (g_nDeviceNumber)
		{
		case DEVICE_CAMERA:
		case DEVICE_OPENCV:  //CScanner_OpencV
		case DEVICE_G6400:
		case DEVICE_G6600:
		//case DEVICE_FREEIMAGE:  //CScanner_FreeImage
			{				
				do
				{
					dwRead = MIN((unsigned long)64000, nImageSize) / nDestBytesPerRow * nDestBytesPerRow;//dwRead=61200
					dwReceived = 0;

					if( !m_pScanner->getScanStrip(pImageData, dwRead, dwReceived) ||
						dwReceived != dwReceived / nDestBytesPerRow * nDestBytesPerRow)
					{
						//No more data to recieve fill the rest
						break;
					}
					pImageData += dwReceived;

					nImageSize -= dwReceived;  //dwReceived=20400 nImageSize=11158800
				}while(nImageSize>0 && twrc == TWRC_SUCCESS);
				
			}
			break;
		//case DEVICE_G6400:  // CScanner_G6400
		//	{				
		//		m_pScanner->GetImageData(pImageData,dwReceived);
		//		pImageData += dwReceived;
		//	}
		//	break;
		//case DEVICE_OPENCV:
		//	{
		//		m_pScanner->GetImageData(pImageData,dwReceived);
		//		//::MessageBox(g_hwndDLG,TEXT("GetImageData success!"),MB_CAPTION,MB_OK);
		//	}
		//	break;
		default:
			{
				::MessageBox(g_hwndDLG,TEXT("不支持的设备!"),MB_CAPTION,MB_OK);
			}
			break;
		}

    _DSM_UnlockMemory(m_hImageData);
  }
  else
  {
    /// @todo Unknow paper size
    // for unknow paper size need to use reallocate and keep transfering and adding to image data.
    setConditionCode(TWCC_CAPUNSUPPORTED);
    return TWRC_FAILURE;
  }

  if(twrc == TWRC_FAILURE)
  {
    setConditionCode(TWCC_BADVALUE);
  }

  return twrc;
}

//////////////////////////////////////////////////////////////////////////////
TW_INT16 CTWAINDS_UDS::endXfer(pTW_PENDINGXFERS _pXfers)
{
	//::MessageBox(g_hwndDLG,TEXT("CTWAINDS_UDS::endXfer!"),MB_CAPTION,MB_OK);
  TW_INT16 twrc = TWRC_SUCCESS;

  if( !( dsState_XferReady == m_CurrentState ||
         dsState_Xferring  == m_CurrentState    ) )
  {
    setConditionCode(TWCC_SEQERROR);
    return TWRC_FAILURE;
  }

  if(m_Xfers.Count != TW_UINT16(TWON_DONTCARE16))
  {
    --m_Xfers.Count;
  }
  
  if(!m_pScanner->isFeederLoaded())
  {
		//::MessageBox(g_hwndDLG,TEXT("m_Xfers.Count = 0!"),MB_CAPTION,MB_OK);
    m_Xfers.Count = 0;
  }
  if(m_bCanceled)
  {
    m_bCanceled = false;
    m_Xfers.Count = 0;
  }
  if(0 != m_Xfers.Count)
  {
    // Check to see if autofeed is turned on if so automaticly go get next image.
    CTWAINContainerBool *pbCap = dynamic_cast<CTWAINContainerBool*>(findCapability(CAP_AUTOFEED));
    bool bAutoFeed = FALSE;
    if(pbCap)
    {
      pbCap->GetCurrent(bAutoFeed);
    }
    if(bAutoFeed)
    {
      // More images are requested, go to scan and try to get the next
      // image in pre-emption of the app asking for it
			//::MessageBox(g_hwndDLG," endXfer: acquireImage()! ",MB_CAPTION,MB_OK);
      if(!m_pScanner->acquireImage())
      {
				//::MessageBox(g_hwndDLG,TEXT("endXfer: There was an error while prepping the image for scanning!"),MB_CAPTION,MB_OK);
        //cerr << "ds: There was an error while prepping the image for scanning" << endl;
        setConditionCode(TWCC_BUMMER);
        m_Xfers.Count = 0;  // 重张检测时，若未加这句程序会崩溃
        twrc = TWRC_FAILURE;
      }
    }

    m_CurrentState = dsState_XferReady;
  }
  else
  {
    m_CurrentState = dsState_Enabled;
    m_pScanner->Unlock();
  }

  if( _pXfers == 0 )
  {
    setConditionCode(TWCC_BADVALUE);
    // Did everyting but return the currect count.
    return TWRC_CHECKSTATUS;
  }

  *_pXfers = m_Xfers;

  return twrc;
}

//////////////////////////////////////////////////////////////////////////////
TW_INT16 CTWAINDS_UDS::getXfer(pTW_PENDINGXFERS _pXfers)
{//::MessageBox(g_hwndDLG,"getXfer",MB_CAPTION,MB_OK);
  TW_INT16 twrc = TWRC_SUCCESS;

  if( dsState_XferReady != m_CurrentState )
  {
    setConditionCode(TWCC_SEQERROR);
    return TWRC_FAILURE;
  }

  if( _pXfers == 0 )
  {
    setConditionCode(TWCC_BADVALUE);
    // Did everyting but return the currect count.
    return TWRC_CHECKSTATUS;
  }

  *_pXfers = m_Xfers;
  return twrc;
}

//////////////////////////////////////////////////////////////////////////////
TW_INT16 CTWAINDS_UDS::resetXfer(pTW_PENDINGXFERS _pXfers)
{//::MessageBox(g_hwndDLG,"resetXfer",MB_CAPTION,MB_OK);
  TW_INT16 twrc = TWRC_SUCCESS;

  if( dsState_Loaded == m_CurrentState )
  {
    setConditionCode(TWCC_SEQERROR);
    return TWRC_FAILURE;
  }

  m_Xfers.Count = 0;
  m_CurrentState = dsState_Enabled;
  m_pScanner->Unlock();

  if( _pXfers == 0 )
  {
    setConditionCode(TWCC_BADVALUE);
    return TWRC_FAILURE;
  }

  *_pXfers = m_Xfers;
  return twrc;
}


//////////////////////////////////////////////////////////////////////////////
bool CTWAINDS_UDS::updateScannerFromCaps()
{
	//::MessageBox(g_hwndDLG,"updateScannerFromCaps",MB_CAPTION,MB_OK);
  int   nVal;
  float fVal;
	bool  bVal; //zhu
  bool  bret = true;  // Set to false if anything fails
  CDevice_Base settings;

  // Get current before updating
  settings = *(m_pScanner->getSetting());

  CTWAINContainerInt    *pnCap = 0;
  CTWAINContainerFix32  *pfCap = 0;
  CTWAINContainerFix32Range *pfRCap = 0;

	CTWAINContainerBool* pbCap = 0; //zhu

	//压缩
	if(0 == (pnCap = dynamic_cast<CTWAINContainerInt*>(findCapability(ICAP_COMPRESSION))))
	{
		//cerr << "Could not get ICAP_SUPPORTEDSIZES" << endl;
		::MessageBox(g_hwndDLG,TEXT("Could not get ICAP_COMPRESSION!"),MB_CAPTION,MB_OK);
		bret = false;
	}
	else
	{
		pnCap->GetCurrent(nVal);
		settings.m_nCompress = nVal;
	}
	//多流
	if(0 == (pnCap = dynamic_cast<CTWAINContainerInt*>(findCapability(ICAP_COMPRESSION))))
	{
		//cerr << "Could not get ICAP_SUPPORTEDSIZES" << endl;
		::MessageBox(g_hwndDLG,TEXT("Could not get ICAP_COMPRESSION!"),MB_CAPTION,MB_OK);
		bret = false;
	}
	else
	{
		pnCap->GetCurrent(nVal);
		settings.m_nCompre[0] = nVal;
	}
	if(0 == (pnCap = dynamic_cast<CTWAINContainerInt*>(findCapability(ICAP_COMPRESSION))))
	{
		//cerr << "Could not get ICAP_SUPPORTEDSIZES" << endl;
		::MessageBox(g_hwndDLG,TEXT("Could not get ICAP_COMPRESSION!"),MB_CAPTION,MB_OK);
		bret = false;
	}
	else
	{
		pnCap->GetCurrent(nVal);
		settings.m_nCompre[1] = nVal;
	}
	if(0 == (pnCap = dynamic_cast<CTWAINContainerInt*>(findCapability(ICAP_COMPRESSION))))
	{
		//cerr << "Could not get ICAP_SUPPORTEDSIZES" << endl;
		::MessageBox(g_hwndDLG,TEXT("Could not get ICAP_COMPRESSION!"),MB_CAPTION,MB_OK);
		bret = false;
	}
	else
	{
		pnCap->GetCurrent(nVal);
		settings.m_nCompre[2] = nVal;
	}
	if(0 == (pnCap = dynamic_cast<CTWAINContainerInt*>(findCapability(ICAP_COMPRESSION))))
	{
		//cerr << "Could not get ICAP_SUPPORTEDSIZES" << endl;
		::MessageBox(g_hwndDLG,TEXT("Could not get ICAP_COMPRESSION!"),MB_CAPTION,MB_OK);
		bret = false;
	}
	else
	{
		pnCap->GetCurrent(nVal);
		settings.m_nCompre[3] = nVal;
	}
	if(0 == (pnCap = dynamic_cast<CTWAINContainerInt*>(findCapability(ICAP_COMPRESSION))))
	{
		//cerr << "Could not get ICAP_SUPPORTEDSIZES" << endl;
		::MessageBox(g_hwndDLG,TEXT("Could not get ICAP_COMPRESSION!"),MB_CAPTION,MB_OK);
		bret = false;
	}
	else
	{
		pnCap->GetCurrent(nVal);
		settings.m_nCompre[4] = nVal;
	}
	if(0 == (pnCap = dynamic_cast<CTWAINContainerInt*>(findCapability(ICAP_COMPRESSION))))
	{
		//cerr << "Could not get ICAP_SUPPORTEDSIZES" << endl;
		::MessageBox(g_hwndDLG,TEXT("Could not get ICAP_COMPRESSION!"),MB_CAPTION,MB_OK);
		bret = false;
	}
	else
	{
		pnCap->GetCurrent(nVal);
		settings.m_nCompre[5] = nVal;
	}

    //图像裁切方式
	if(0 == (pnCap = dynamic_cast<CTWAINContainerInt*>(findCapability(UDSCAP_CUTMETHOD))))
	{
		::MessageBox(g_hwndDLG,TEXT("Could not get UDSCAP_CUTMETHOD!"),MB_CAPTION,MB_OK);
		bret = false;
	}
	else
	{
		pnCap->GetCurrent(nVal);
		settings.m_nCutMethod = nVal;
	}

	//扫描仪无纸时转高拍仪
	if(0 == (pbCap = dynamic_cast<CTWAINContainerBool*>(findCapability(UDSCAP_TURNVIDEO))))
	{
		::MessageBox(g_hwndDLG,TEXT("Could not get UDSCAP_TURNVIDEO!"),MB_CAPTION,MB_OK);
		bret = false;
	}
	else
	{
		pbCap->GetCurrent(bVal);
		settings.m_bTurnVideo = bVal;
	}

	//去除噪声-噪声范围
	if(0 == (pfRCap = dynamic_cast<CTWAINContainerFix32Range*>(findCapability(UDSCAP_NOISERANGE))))
	{
		::MessageBox(g_hwndDLG,TEXT("Could not get UDSCAP_NOISERANGE!"),MB_CAPTION,MB_OK);
		bret = false;
	}
	else
	{
		pfRCap->GetCurrent(fVal);
		settings.m_fNoiseRange = fVal;
	}

	//去除噪声-噪声数目
	if(0 == (pfRCap = dynamic_cast<CTWAINContainerFix32Range*>(findCapability(UDSCAP_NOISENUM))))
	{
		::MessageBox(g_hwndDLG,TEXT("Could not get UDSCAP_NOISENUM!"),MB_CAPTION,MB_OK);
		bret = false;
	}
	else
	{
		pfRCap->GetCurrent(fVal);
		settings.m_fNoiseNum = fVal;
	}
	//滤除颜色
	if(0 == (pnCap = dynamic_cast<CTWAINContainerInt*>(findCapability(UDSCAP_FILTERCOLOR))))
	{
		::MessageBox(g_hwndDLG,TEXT("Could not get UDSCAP_FILTERCOLOR!"),MB_CAPTION,MB_OK);
		bret = false;
	}
	else
	{
		pnCap->GetCurrent(nVal);
		settings.m_nFilterColor = nVal;
	}
	//滤除模式
	if(0 == (pnCap = dynamic_cast<CTWAINContainerInt*>(findCapability(UDSCAP_FILTERMODE))))
	{
		::MessageBox(g_hwndDLG,TEXT("Could not get UDSCAP_FILTERMODE!"),MB_CAPTION,MB_OK);
		bret = false;
	}
	else
	{
		pnCap->GetCurrent(nVal);
		settings.m_nFilterMode = nVal;
	}
	//滤除程度
	if(0 == (pfRCap = dynamic_cast<CTWAINContainerFix32Range*>(findCapability(UDSCAP_FILTERLEVEL))))
	{
		::MessageBox(g_hwndDLG,TEXT("Could not get UDSCAP_FILTERLEVEL!"),MB_CAPTION,MB_OK);
		bret = false;
	}
	else
	{
		pfRCap->GetCurrent(fVal);
		settings.m_fFilterLevel= fVal;
	}
	//节电模式
	if(0 == (pbCap = dynamic_cast<CTWAINContainerBool*>(findCapability(UDSCAP_POWERSAVING))))
	{
		::MessageBox(g_hwndDLG,TEXT("Could not get UDSCAP_POWERSAVING!"),MB_CAPTION,MB_OK);
		bret = false;
	}
	else
	{
		pbCap->GetCurrent(bVal);
		settings.m_bSavePower = bVal;
	}

	if(0 == (pfRCap = dynamic_cast<CTWAINContainerFix32Range*>(findCapability(UDSCAP_POWERSAVING_TIME))))
	{
		::MessageBox(g_hwndDLG,TEXT("Could not get UDSCAP_POWERSAVING_TIME!"),MB_CAPTION,MB_OK);
		bret = false;
	}
	else
	{
		pfRCap->GetCurrent(fVal);
		settings.m_fSavePowerValue= fVal;
	}

	//关机时间
	if(0 == (pbCap = dynamic_cast<CTWAINContainerBool*>(findCapability(UDSCAP_POWEROFF))))
	{
		::MessageBox(g_hwndDLG,TEXT("Could not get UDSCAP_POWEROFF!"),MB_CAPTION,MB_OK);
		bret = false;
	}
	else
	{
		pbCap->GetCurrent(bVal);
		settings.m_bOffTime = bVal;
	}

	if(0 == (pfRCap = dynamic_cast<CTWAINContainerFix32Range*>(findCapability(UDSCAP_POWEROFF_TIME))))
	{
		::MessageBox(g_hwndDLG,TEXT("Could not get UDSCAP_POWEROFF_TIME!"),MB_CAPTION,MB_OK);
		bret = false;
	}
	else
	{
		pfRCap->GetCurrent(fVal);
		settings.m_fOffTimeValue = fVal;
	}

	//压缩比的值
	if(0 == (pfRCap = dynamic_cast<CTWAINContainerFix32Range*>(findCapability(UDSCAP_COMPRESSVALUE))))
	{
		::MessageBox(g_hwndDLG,TEXT("Could not get UDSCAP_COMPRESSVALUE!"),MB_CAPTION,MB_OK);
		bret = false;
	}
	else
	{
		pfRCap->GetCurrent(fVal);
		settings.m_fCompressValue = fVal;
	}
	//多流
	if(0 == (pfRCap = dynamic_cast<CTWAINContainerFix32Range*>(findCapability(UDSCAP_COMPRESSIONVALUEFC))))
	{
		::MessageBox(g_hwndDLG,TEXT("Could not get UDSCAP_COMPRESSIONVALUEFC!"),MB_CAPTION,MB_OK);
		bret = false;
	}
	else
	{
		pfRCap->GetCurrent(fVal);
		settings.m_fCompVal[0] = fVal;
	}
	if(0 == (pfRCap = dynamic_cast<CTWAINContainerFix32Range*>(findCapability(UDSCAP_COMPRESSIONVALUEFG))))
	{
		::MessageBox(g_hwndDLG,TEXT("Could not get UDSCAP_COMPRESSIONVALUEFG!"),MB_CAPTION,MB_OK);
		bret = false;
	}
	else
	{
		pfRCap->GetCurrent(fVal);
		settings.m_fCompVal[1] = fVal;
	}
	if(0 == (pfRCap = dynamic_cast<CTWAINContainerFix32Range*>(findCapability(UDSCAP_COMPRESSIONVALUEFB))))
	{
		::MessageBox(g_hwndDLG,TEXT("Could not get UDSCAP_COMPRESSIONVALUEFB!"),MB_CAPTION,MB_OK);
		bret = false;
	}
	else
	{
		pfRCap->GetCurrent(fVal);
		settings.m_fCompVal[2] = fVal;
	}
	if(0 == (pfRCap = dynamic_cast<CTWAINContainerFix32Range*>(findCapability(UDSCAP_COMPRESSIONVALUEBC))))
	{
		::MessageBox(g_hwndDLG,TEXT("Could not get UDSCAP_COMPRESSIONVALUEBC!"),MB_CAPTION,MB_OK);
		bret = false;
	}
	else
	{
		pfRCap->GetCurrent(fVal);
		settings.m_fCompVal[3] = fVal;
	}
	if(0 == (pfRCap = dynamic_cast<CTWAINContainerFix32Range*>(findCapability(UDSCAP_COMPRESSIONVALUEBG))))
	{
		::MessageBox(g_hwndDLG,TEXT("Could not get UDSCAP_COMPRESSIONVALUEBG!"),MB_CAPTION,MB_OK);
		bret = false;
	}
	else
	{
		pfRCap->GetCurrent(fVal);
		settings.m_fCompVal[4] = fVal;
	}
	if(0 == (pfRCap = dynamic_cast<CTWAINContainerFix32Range*>(findCapability(UDSCAP_COMPRESSIONVALUEBB))))
	{
		::MessageBox(g_hwndDLG,TEXT("Could not get UDSCAP_COMPRESSIONVALUEBB!"),MB_CAPTION,MB_OK);
		bret = false;
	}
	else
	{
		pfRCap->GetCurrent(fVal);
		settings.m_fCompVal[5] = fVal;
	}

	//纸张大小
	if(0 == (pnCap = dynamic_cast<CTWAINContainerInt*>(findCapability(ICAP_SUPPORTEDSIZES))))
	{
		//cerr << "Could not get ICAP_SUPPORTEDSIZES" << endl;
		::MessageBox(g_hwndDLG,TEXT("Could not get ICAP_SUPPORTEDSIZES!"),MB_CAPTION,MB_OK);
		bret = false;
	}
	else
	{
		pnCap->GetCurrent(nVal);
		settings.m_nStandardsizes = nVal;
	}

	//单位
	if(0 == (pnCap = dynamic_cast<CTWAINContainerInt*>(findCapability(ICAP_UNITS))))
	{
		//cerr << "Could not get ICAP_UNITS" << endl;
		::MessageBox(g_hwndDLG,TEXT("Could not get ICAP_UNITS!"),MB_CAPTION,MB_OK);
		bret = false;
	}
	else
	{
		pnCap->GetCurrent(nVal);
		settings.m_nUnits = nVal;
	}

	//zhu 重张检测
	if(0 == (pbCap = dynamic_cast<CTWAINContainerBool*>(findCapability(UDSCAP_MULTIFEEDDETECT))))
	{
		//cerr << "Could not get ICAP_PIXELTYPE" << endl;
		::MessageBox(g_hwndDLG,TEXT("Could not get UDSCAP_MULTIFEEDDETECT!"),MB_CAPTION,MB_OK);
		bret = false;
	}
	else
	{
		pbCap->GetCurrent(bVal);
		settings.m_bMultifeedDetection = bVal;
	}

	//zhu 重张检测故障值
	if(0 == (pbCap = dynamic_cast<CTWAINContainerBool*>(findCapability(UDSCAP_MULTIFEEDDETECT_VALUE))))
	{
		::MessageBox(g_hwndDLG,TEXT("Could not get UDSCAP_MULTIFEEDDETECT_VALUE!"),MB_CAPTION,MB_OK);
		bret = false;
	}
	else
	{
		pbCap->GetCurrent(bVal);
		settings.m_bMD_value = bVal;
	}

	//色彩翻转
	if(0 == (pbCap = dynamic_cast<CTWAINContainerBool*>(findCapability(UDSCAP_COLORFLIP))))
	{
		::MessageBox(g_hwndDLG,TEXT("Could not get UDSCAP_COLORFLIP!"),MB_CAPTION,MB_OK);
		bret = false;
	}
	else
	{
		pbCap->GetCurrent(bVal);
		settings.m_bColorFlip = bVal;
	}

	//zhu 多流输出
	if(0 == (pbCap = dynamic_cast<CTWAINContainerBool*>(findCapability(UDSCAP_MULTISTREAM))))
	{
		//cerr << "Could not get ICAP_PIXELTYPE" << endl;
		::MessageBox(g_hwndDLG,TEXT("Could not get UDSCAP_MULTISTREAM!"),MB_CAPTION,MB_OK);
		bret = false;
	}
	else
	{
		pbCap->GetCurrent(bVal);
		settings.m_bMultiStream = bVal;
	}

	// 图像类型
  if(0 == (pnCap = dynamic_cast<CTWAINContainerInt*>(findCapability(ICAP_PIXELTYPE))))
  {
		::MessageBox(g_hwndDLG,TEXT("Could not get ICAP_PIXELTYPE!"),MB_CAPTION,MB_OK);
    bret = false;
  }
  else
  {
    pnCap->GetCurrent(nVal);
    settings.m_nPixelType = nVal;
  }

	// 扫描模式
	if(0 == (pbCap = dynamic_cast<CTWAINContainerBool*>(findCapability(CAP_FEEDERENABLED))))
	{
		//cerr << "Could not get ICAP_PIXELTYPE" << endl;
		::MessageBox(g_hwndDLG,TEXT("Could not get CAP_FEEDERENABLED!"),MB_CAPTION,MB_OK);
		bret = false;
	}
	else
	{
		pbCap->GetCurrent(bVal);
		settings.m_nPaperSource = bVal;
	}

	// 单双面
	if(0 == (pbCap = dynamic_cast<CTWAINContainerBool*>(findCapability(CAP_DUPLEXENABLED))))
	{
		//cerr << "Could not get ICAP_PIXELTYPE" << endl;
		::MessageBox(g_hwndDLG,TEXT("Could not get CAP_DUPLEXENABLED!"),MB_CAPTION,MB_OK);
		bret = false;
	}
	else
	{
		pbCap->GetCurrent(bVal);
		settings.m_bDuplex = bVal;
	}

  // X resolution the same.
  if(0 == (pfCap = dynamic_cast<CTWAINContainerFix32*>(findCapability(ICAP_XRESOLUTION))))
  {
		::MessageBox(g_hwndDLG,TEXT("Could not get ICAP_XRESOLUTION!"),MB_CAPTION,MB_OK);
    //cerr << "Could not get ICAP_XRESOLUTION" << endl;
    bret = false;
  }
  else
  {
    pfCap->GetCurrent(fVal);
    settings.m_fXResolution = fVal;
  }

  // Y resolution the same.
  if(0 == (pfCap = dynamic_cast<CTWAINContainerFix32*>(findCapability(ICAP_YRESOLUTION))))
  {
		::MessageBox(g_hwndDLG,TEXT("Could not get ICAP_YRESOLUTION!"),MB_CAPTION,MB_OK);
    //cerr << "Could not get ICAP_YRESOLUTION" << endl;
    bret = false;
  }
  else
  {
    pfCap->GetCurrent(fVal);
    settings.m_fYResolution = fVal;
  }
	
	//多流分辨率
	if(0 == (pfCap = dynamic_cast<CTWAINContainerFix32*>(findCapability(UDSCAP_RESOLUTIONFC))))
	{
		::MessageBox(g_hwndDLG,TEXT("Could not get UDSCAP_RESOLUTIONFC!"),MB_CAPTION,MB_OK);
		bret = false;
	}
	else
	{
		pfCap->GetCurrent(fVal);
		settings.m_fResolu[0] = fVal;
	}
	if(0 == (pfCap = dynamic_cast<CTWAINContainerFix32*>(findCapability(UDSCAP_RESOLUTIONFG))))
	{
		::MessageBox(g_hwndDLG,TEXT("Could not get UDSCAP_RESOLUTIONFG!"),MB_CAPTION,MB_OK);
		bret = false;
	}
	else
	{
		pfCap->GetCurrent(fVal);
		settings.m_fResolu[1] = fVal;
	}
	if(0 == (pfCap = dynamic_cast<CTWAINContainerFix32*>(findCapability(UDSCAP_RESOLUTIONFB))))
	{
		::MessageBox(g_hwndDLG,TEXT("Could not get UDSCAP_RESOLUTIONFB!"),MB_CAPTION,MB_OK);
		bret = false;
	}
	else
	{
		pfCap->GetCurrent(fVal);
		settings.m_fResolu[2] = fVal;
	}
	if(0 == (pfCap = dynamic_cast<CTWAINContainerFix32*>(findCapability(UDSCAP_RESOLUTIONBC))))
	{
		::MessageBox(g_hwndDLG,TEXT("Could not get UDSCAP_RESOLUTIONBC!"),MB_CAPTION,MB_OK);
		bret = false;
	}
	else
	{
		pfCap->GetCurrent(fVal);
		settings.m_fResolu[3] = fVal;
	}
	if(0 == (pfCap = dynamic_cast<CTWAINContainerFix32*>(findCapability(UDSCAP_RESOLUTIONBG))))
	{
		::MessageBox(g_hwndDLG,TEXT("Could not get UDSCAP_RESOLUTIONBG!"),MB_CAPTION,MB_OK);
		bret = false;
	}
	else
	{
		pfCap->GetCurrent(fVal);
		settings.m_fResolu[4] = fVal;
	}
	if(0 == (pfCap = dynamic_cast<CTWAINContainerFix32*>(findCapability(UDSCAP_RESOLUTIONBB))))
	{
		::MessageBox(g_hwndDLG,TEXT("Could not get UDSCAP_RESOLUTIONBB!"),MB_CAPTION,MB_OK);
		bret = false;
	}
	else
	{
		pfCap->GetCurrent(fVal);
		settings.m_fResolu[5] = fVal;
	}

  // set the image dimensions设置图像尺寸
  InternalFrame frame;
  if(!m_pICAP_FRAMES->GetCurrent(frame))
  {
		::MessageBox(g_hwndDLG,TEXT("Could not get ICAP_FRAMES!"),MB_CAPTION,MB_OK);
    //cerr << "Could not get ICAP_FRAMES" << endl;
    bret = false;
  }
  else
  {
    // prepare to set the scanning window by converting from our internal units to pixels.
    TW_FRAME TWframe = frame.AsTW_FRAME(TWUN_PIXELS, settings.m_fXResolution, settings.m_fYResolution);

    // I'm not expecting any fractional values here so casting to int is safe
    /** @todo the scanner only supports width and height if the image is intended to be cropped on the 
    *    left or top then need to crop the image as it is coming off the scanner.
    *    Should be passing to the scanner the width as the right and height as bottom; not the diff.
    */
    settings.m_nWidth  = int(FIX32ToFloat(TWframe.Right)  - FIX32ToFloat(TWframe.Left));
    settings.m_nHeight = int(FIX32ToFloat(TWframe.Bottom) - FIX32ToFloat(TWframe.Top));
  }

	//阈值
  if(0 == (pfRCap = dynamic_cast<CTWAINContainerFix32Range*>(findCapability(ICAP_THRESHOLD))))
  {
		::MessageBox(g_hwndDLG,TEXT("Could not get ICAP_THRESHOLD!"),MB_CAPTION,MB_OK);
    //cerr << "Could not get ICAP_THRESHOLD" << endl;
    bret = false;
  }
  else
  {
    pfRCap->GetCurrent(fVal);
    settings.m_fThreshold = fVal;
  }
	//多流阈值
	if(0 == (pfRCap = dynamic_cast<CTWAINContainerFix32Range*>(findCapability(UDSCAP_THRESHOLDFB))))
	{
		::MessageBox(g_hwndDLG,TEXT("Could not get UDSCAP_THRESHOLDFB!"),MB_CAPTION,MB_OK);
		bret = false;
	}
	else
	{
		pfRCap->GetCurrent(fVal);
		settings.m_fThres[0] = fVal;
	}
	if(0 == (pfRCap = dynamic_cast<CTWAINContainerFix32Range*>(findCapability(UDSCAP_THRESHOLDBB))))
	{
		::MessageBox(g_hwndDLG,TEXT("Could not get UDSCAP_THRESHOLDBB!"),MB_CAPTION,MB_OK);
		bret = false;
	}
	else
	{
		pfRCap->GetCurrent(fVal);
		settings.m_fThres[1] = fVal;
	}

	// 对比度
  if(0 == (pfRCap = dynamic_cast<CTWAINContainerFix32Range*>(findCapability(ICAP_CONTRAST))))
  {
		::MessageBox(g_hwndDLG,TEXT("Could not get ICAP_CONTRAST!"),MB_CAPTION,MB_OK);
    //cerr << "Could not get ICAP_CONTRAST" << endl;
    bret = false;
  }
  else
  {
    pfRCap->GetCurrent(fVal);
    settings.m_fContrast = fVal;
  }
	//多流对比度
	if(0 == (pfRCap = dynamic_cast<CTWAINContainerFix32Range*>(findCapability(UDSCAP_CONTRASTFC))))
	{
		::MessageBox(g_hwndDLG,TEXT("Could not get UDSCAP_CONTRASTFC!"),MB_CAPTION,MB_OK);
		bret = false;
	}
	else
	{
		pfRCap->GetCurrent(fVal);
		settings.m_fContra[0] = fVal;
	}
	if(0 == (pfRCap = dynamic_cast<CTWAINContainerFix32Range*>(findCapability(UDSCAP_CONTRASTFG))))
	{
		::MessageBox(g_hwndDLG,TEXT("Could not get UDSCAP_CONTRASTFG!"),MB_CAPTION,MB_OK);
		bret = false;
	}
	else
	{
		pfRCap->GetCurrent(fVal);
		settings.m_fContra[1] = fVal;
	}
	if(0 == (pfRCap = dynamic_cast<CTWAINContainerFix32Range*>(findCapability(UDSCAP_CONTRASTBC))))
	{
		::MessageBox(g_hwndDLG,TEXT("Could not get UDSCAP_CONTRASTBC!"),MB_CAPTION,MB_OK);
		bret = false;
	}
	else
	{
		pfRCap->GetCurrent(fVal);
		settings.m_fContra[2] = fVal;
	}
	if(0 == (pfRCap = dynamic_cast<CTWAINContainerFix32Range*>(findCapability(UDSCAP_CONTRASTBG))))
	{
		::MessageBox(g_hwndDLG,TEXT("Could not get UDSCAP_CONTRASTBG!"),MB_CAPTION,MB_OK);
		bret = false;
	}
	else
	{
		pfRCap->GetCurrent(fVal);
		settings.m_fContra[3] = fVal;
	}

	// 亮度
	if(0 == (pfRCap = dynamic_cast<CTWAINContainerFix32Range*>(findCapability(ICAP_BRIGHTNESS))))
  {
		::MessageBox(g_hwndDLG,TEXT("Could not get ICAP_BRIGHTNESS!"),MB_CAPTION,MB_OK);
    //cerr << "Could not get ICAP_BRIGHTNESS" << endl;
    bret = false;
  }
  else
  {
    pfRCap->GetCurrent(fVal);
    settings.m_fBrightness = fVal;
	}
	//多流亮度
	if(0 == (pfRCap = dynamic_cast<CTWAINContainerFix32Range*>(findCapability(UDSCAP_BRIGHTNESSFC))))
	{
		::MessageBox(g_hwndDLG,TEXT("Could not get UDSCAP_BRIGHTNESSFC!"),MB_CAPTION,MB_OK);
		bret = false;
	}
	else
	{
		pfRCap->GetCurrent(fVal);
		settings.m_fBright[0] = fVal;
	}
	if(0 == (pfRCap = dynamic_cast<CTWAINContainerFix32Range*>(findCapability(UDSCAP_BRIGHTNESSFG))))
	{
		::MessageBox(g_hwndDLG,TEXT("Could not get UDSCAP_BRIGHTNESSFG!"),MB_CAPTION,MB_OK);
		bret = false;
	}
	else
	{
		pfRCap->GetCurrent(fVal);
		settings.m_fBright[1] = fVal;
	}
	if(0 == (pfRCap = dynamic_cast<CTWAINContainerFix32Range*>(findCapability(UDSCAP_BRIGHTNESSFB))))
	{
		::MessageBox(g_hwndDLG,TEXT("Could not get UDSCAP_BRIGHTNESSFB!"),MB_CAPTION,MB_OK);
		bret = false;
	}
	else
	{
		pfRCap->GetCurrent(fVal);
		settings.m_fBright[2] = fVal;
	}
	if(0 == (pfRCap = dynamic_cast<CTWAINContainerFix32Range*>(findCapability(UDSCAP_BRIGHTNESSBC))))
	{
		::MessageBox(g_hwndDLG,TEXT("Could not get UDSCAP_BRIGHTNESSBC!"),MB_CAPTION,MB_OK);
		bret = false;
	}
	else
	{
		pfRCap->GetCurrent(fVal);
		settings.m_fBright[3] = fVal;
	}
	if(0 == (pfRCap = dynamic_cast<CTWAINContainerFix32Range*>(findCapability(UDSCAP_BRIGHTNESSBG))))
	{
		::MessageBox(g_hwndDLG,TEXT("Could not get UDSCAP_BRIGHTNESSBG!"),MB_CAPTION,MB_OK);
		bret = false;
	}
	else
	{
		pfRCap->GetCurrent(fVal);
		settings.m_fBright[4] = fVal;
	}
	if(0 == (pfRCap = dynamic_cast<CTWAINContainerFix32Range*>(findCapability(UDSCAP_BRIGHTNESSBB))))
	{
		::MessageBox(g_hwndDLG,TEXT("Could not get UDSCAP_BRIGHTNESSBB!"),MB_CAPTION,MB_OK);
		bret = false;
	}
	else
	{
		pfRCap->GetCurrent(fVal);
		settings.m_fBright[5] = fVal;
	}

	//去除斑点
	if(0 == (pfRCap = dynamic_cast<CTWAINContainerFix32Range*>(findCapability(UDSCAP_REMOVESPOTS))))
  {
		::MessageBox(g_hwndDLG,TEXT("Could not get UDSCAP_SENSITIVETHRESHOLD_REMOVESPOTS!"),MB_CAPTION,MB_OK);
    bret = false;
  }
  else
  {
    pfRCap->GetCurrent(fVal);
    settings.m_fSensitiveThreshold_removespots = fVal;
  }
	//多流去除斑点
	if(0 == (pfRCap = dynamic_cast<CTWAINContainerFix32Range*>(findCapability(UDSCAP_REMOVESPOTSFB))))
	{
		::MessageBox(g_hwndDLG,TEXT("Could not get UDSCAP_REMOVESPOTSFB!"),MB_CAPTION,MB_OK);
		bret = false;
	}
	else
	{
		pfRCap->GetCurrent(fVal);
		settings.m_fRemovespots[0] = fVal;
	}
	if(0 == (pfRCap = dynamic_cast<CTWAINContainerFix32Range*>(findCapability(UDSCAP_REMOVESPOTSBB))))
	{
		::MessageBox(g_hwndDLG,TEXT("Could not get UDSCAP_REMOVESPOTSBB!"),MB_CAPTION,MB_OK);
		bret = false;
	}
	else
	{
		pfRCap->GetCurrent(fVal);
		settings.m_fRemovespots[1] = fVal;
	}

	// 图像旋转
	if(0 == (pfCap = dynamic_cast<CTWAINContainerFix32*>(findCapability(ICAP_ROTATION))))
	{
		::MessageBox(g_hwndDLG,TEXT("Could not get ICAP_ROTATION!"),MB_CAPTION,MB_OK);
		//cerr << "Could not get ICAP_ROTATION" << endl;
		bret = false;
	}
	else
	{
		pfCap->GetCurrent(fVal);
		settings.m_fRotation = fVal;
	}

	// 多流选项值
	if(0 == (pfCap = dynamic_cast<CTWAINContainerFix32*>(findCapability(UDSCAP_MULTISTREAM_VALUE))))
	{
		::MessageBox(g_hwndDLG,TEXT("Could not get UDSCAP_MULTISTREAM_VALUE!"),MB_CAPTION,MB_OK);
		bret = false;
	}
	else
	{
		pfCap->GetCurrent(fVal);
		settings.m_byteMultiValue = (BYTE)fVal;
	}

	//边缘扩充
	if(0 == (pfCap = dynamic_cast<CTWAINContainerFix32*>(findCapability(UDSCAP_EDGE_UP))))
	{
		::MessageBox(g_hwndDLG,TEXT("Could not get UDSCAP_EDGE_UP!"),MB_CAPTION,MB_OK);
		bret = false;
	}
	else
	{
		pfCap->GetCurrent(fVal);
		settings.m_fEdgeUp = fVal;
	}

	if(0 == (pfCap = dynamic_cast<CTWAINContainerFix32*>(findCapability(UDSCAP_EDGE_DOWN))))
	{
		::MessageBox(g_hwndDLG,TEXT("Could not get UDSCAP_EDGE_DOWN!"),MB_CAPTION,MB_OK);
		bret = false;
	}
	else
	{
		pfCap->GetCurrent(fVal);
		settings.m_fEdgeDown = fVal;
	}

	if(0 == (pfCap = dynamic_cast<CTWAINContainerFix32*>(findCapability(UDSCAP_EDGE_LEFT))))
	{
		::MessageBox(g_hwndDLG,TEXT("Could not get UDSCAP_EDGE_LEFT!"),MB_CAPTION,MB_OK);
		bret = false;
	}
	else
	{
		pfCap->GetCurrent(fVal);
		settings.m_fEdgeLeft = fVal;
	}

	if(0 == (pfCap = dynamic_cast<CTWAINContainerFix32*>(findCapability(UDSCAP_EDGE_RIGHT))))
	{
		::MessageBox(g_hwndDLG,TEXT("Could not get UDSCAP_EDGE_RIGHT!"),MB_CAPTION,MB_OK);
		bret = false;
	}
	else
	{
		pfCap->GetCurrent(fVal);
		settings.m_fEdgeRight = fVal;
	}

	//X偏移量
	if(0 == (pfCap = dynamic_cast<CTWAINContainerFix32*>(findCapability(UDSCAP_XPOS))))
	{
		::MessageBox(g_hwndDLG,TEXT("Could not get UDSCAP_XPOS!"),MB_CAPTION,MB_OK);
		bret = false;
	}
	else
	{
		pfCap->GetCurrent(fVal);
		settings.m_fXPos = fVal;
	}

	if(0 == (pfCap = dynamic_cast<CTWAINContainerFix32*>(findCapability(UDSCAP_YPOS))))
	{
		::MessageBox(g_hwndDLG,TEXT("Could not get UDSCAP_YPOS!"),MB_CAPTION,MB_OK);
		bret = false;
	}
	else
	{
		pfCap->GetCurrent(fVal);
		settings.m_fYPos = fVal;
	}

	// 图像切割
	if(0 == (pnCap = dynamic_cast<CTWAINContainerInt*>(findCapability(UDSCAP_SPLITIMAGE))))
	{
		::MessageBox(g_hwndDLG,TEXT("Could not get UDSCAP_SPLITIMAGE!"),MB_CAPTION,MB_OK);
		bret = false;
	}
	else
	{
		pnCap->GetCurrent(nVal);
		settings.m_nSpiltImage = nVal;
	}//zhu
	

	//去除空白页的checkbox
	if(0 == (pbCap = dynamic_cast<CTWAINContainerBool*>(findCapability(UDSCAP_REMOVEBLANK))))
	{
		::MessageBox(g_hwndDLG,TEXT("Could not get UDSCAP_REMOVEBLANK!"),MB_CAPTION,MB_OK);
		bret = false;
	}
	else
	{
		pbCap->GetCurrent(bVal);
		settings.m_bRemoveBlank = bVal; 
	}

	//去除空白页等8项
	if(0 == (pfRCap = dynamic_cast<CTWAINContainerFix32Range*>(findCapability(ICAP_AUTODISCARDBLANKPAGES))))
	{
		::MessageBox(g_hwndDLG,TEXT("Could not get ICAP_AUTODISCARDBLANKPAGES!"),MB_CAPTION,MB_OK);
		bret = false;
	}
	else
	{
		pfRCap->GetCurrent(fVal);
		settings.m_fRemoveBlank = fVal;
	}

	if(0 == (pbCap = dynamic_cast<CTWAINContainerBool*>(findCapability(UDSCAP_PUNCHHOLEREMOVEL))))
	{
		::MessageBox(g_hwndDLG,TEXT("Could not get UDSCAP_PUNCHHOLEREMOVEL!"),MB_CAPTION,MB_OK);
		bret = false;
	}
	else
	{
		pbCap->GetCurrent(bVal);
		settings.m_bRemovePunch = bVal;
	}

	if(0 == (pbCap = dynamic_cast<CTWAINContainerBool*>(findCapability(UDSCAP_SHARPEN))))
	{
		//cerr << "Could not get UDSCAP_SHARPEN" << endl;
		::MessageBox(g_hwndDLG,TEXT("Could not get UDSCAP_SHARPEN!"),MB_CAPTION,MB_OK);
		bret = false;
	}
	else
	{
		pbCap->GetCurrent(bVal);
		settings.m_bSharpen = bVal;
	}

	if(0 == (pbCap = dynamic_cast<CTWAINContainerBool*>(findCapability(UDSCAP_MIRROR))))
	{
		//cerr << "Could not get UDSCAP_MIRROR" << endl;
		::MessageBox(g_hwndDLG,TEXT("Could not get UDSCAP_MIRROR!"),MB_CAPTION,MB_OK);
		bret = false;
	}
	else
	{
		pbCap->GetCurrent(bVal);
		settings.m_bMirror = bVal;
	}

	if(0 == (pbCap = dynamic_cast<CTWAINContainerBool*>(findCapability(UDSCAP_REMOVEBACKGROUND))))
	{
		//cerr << "Could not get UDSCAP_REMOVEBACKGROUND" << endl;
		::MessageBox(g_hwndDLG,TEXT("Could not get UDSCAP_REMOVEBACKGROUND!"),MB_CAPTION,MB_OK);
		bret = false;
	}
	else
	{
		pbCap->GetCurrent(bVal);
		settings.m_bRemoveBack = bVal;
	}

	if(0 == (pbCap = dynamic_cast<CTWAINContainerBool*>(findCapability(UDSCAP_DESCREEN))))
	{
		//cerr << "Could not get UDSCAP_DESCREEN" << endl;
		::MessageBox(g_hwndDLG,TEXT("Could not get UDSCAP_DESCREEN!"),MB_CAPTION,MB_OK);
		bret = false;
	}
	else
	{
		pbCap->GetCurrent(bVal);
		settings.m_bDescreen = bVal;
	}

	if(0 == (pbCap = dynamic_cast<CTWAINContainerBool*>(findCapability(UDSCAP_DENOISE))))
	{
		//cerr << "Could not get UDSCAP_DENOISE" << endl;
		::MessageBox(g_hwndDLG,TEXT("Could not get UDSCAP_DENOISE!"),MB_CAPTION,MB_OK);
		bret = false;
	}
	else
	{
		pbCap->GetCurrent(bVal);
		settings.m_bDenoise = bVal;
	}

	// 图像裁切
	if(0 == (pbCap = dynamic_cast<CTWAINContainerBool*>(findCapability(UDSCAP_AUTOCROP))))
	{
		::MessageBox(g_hwndDLG,TEXT("Could not get UDSCAP_AUTOCROP!"),MB_CAPTION,MB_OK);
		bret = false;
	}
	else
	{
		pbCap->GetCurrent(bVal);
		settings.m_bAutoCrop = bVal;
	}

	//zhu二值化
	if(0 == (pnCap = dynamic_cast<CTWAINContainerInt*>(findCapability(UDSCAP_BINARIZATION))))
	{
		::MessageBox(g_hwndDLG,TEXT("Could not get UDSCAP_BINARIZATION!"),MB_CAPTION,MB_OK);
		bret = false;
	}
	else
	{
		pnCap->GetCurrent(nVal);
		settings.m_nBinarization = nVal;
	}//zhu
	//多流二值化
	if(0 == (pnCap = dynamic_cast<CTWAINContainerInt*>(findCapability(UDSCAP_BINARIZATIONFB))))
	{
		::MessageBox(g_hwndDLG,TEXT("Could not get UDSCAP_BINARIZATIONFB!"),MB_CAPTION,MB_OK);
		bret = false;
	}
	else
	{
		pnCap->GetCurrent(nVal);
		settings.m_nBinari[0] = nVal;
	}//zhu
	if(0 == (pnCap = dynamic_cast<CTWAINContainerInt*>(findCapability(UDSCAP_BINARIZATIONBB))))
	{
		::MessageBox(g_hwndDLG,TEXT("Could not get UDSCAP_BINARIZATIONBB!"),MB_CAPTION,MB_OK);
		bret = false;
	}
	else
	{
		pnCap->GetCurrent(nVal);
		settings.m_nBinari[1] = nVal;
	}//zhu

	// 纸张数
	if(0 == (pnCap = dynamic_cast<CTWAINContainerInt*>(findCapability(UDSCAP_DOCS_IN_ADF))))
	{
		::MessageBox(g_hwndDLG,TEXT("Could not get UDSCAP_DOCS_IN_ADF!"),MB_CAPTION,MB_OK);
		bret = false;
	}
	else
	{
		pnCap->GetCurrent(nVal);
		settings.m_nMaxDocCount = nVal;
	}

	// Gamma
	if(0 == (pfRCap = dynamic_cast<CTWAINContainerFix32Range*>(findCapability(ICAP_GAMMA))))
	{
		::MessageBox(g_hwndDLG,TEXT("Could not get ICAP_GAMMA!"),MB_CAPTION,MB_OK);
		bret = false;
	}
	else
	{
		pfRCap->GetCurrent(fVal);
		settings.m_fGamma = fVal;
	}

  if(bret)
  {
    m_pScanner->setSetting(settings);
  }

  return bret;
}

//////////////////////////////////////////////////////////////////////////////
bool CTWAINDS_UDS::DoCloseDSOkEvent()
{//::MessageBox(g_hwndDLG,"DoCloseDSOkEvent",MB_CAPTION,MB_OK);
  if( dsState_Enabled != m_CurrentState )
  {
    setConditionCode(TWCC_SEQERROR);
    return false;
  }

  // Update the scanner with the latest negotiated caps
  if(!updateScannerFromCaps())
  {
    return false;
  }

  return CTWAINDS_Base::DoCloseDSOkEvent();
}


//////////////////////////////////////////////////////////////////////////////
CTWAINContainer* CTWAINDS_UDS::findCapability(const TW_UINT16 _unCap)
{//::MessageBox(g_hwndDLG,"findCapability",MB_CAPTION,MB_OK);
  CTWAINContainer *pRet = 0;
  // When adding to findCapabiltiy remember the order of operations 
  // Some capabilities are dependent on others.
  // see: http://www.twain.org/docs/CapOrderForWeb.PDF
  switch(_unCap)
  {
    case ICAP_FRAMES:
      pRet = m_pICAP_FRAMES;
    break;

    case ICAP_BITDEPTH:
      pRet = getICAP_BITDEPTH();
    break;

    case CAP_DEVICEONLINE:
      {
        CTWAINContainerBool *pBoolCon = dynamic_cast<CTWAINContainerBool*>(m_IndependantCapMap[CAP_DEVICEONLINE]);
        if( NULL != pBoolCon )
        {
          pBoolCon->SetCurrent( m_pScanner->getDeviceOnline()?TRUE:FALSE );
          pRet = pBoolCon;
        }
      }
    break;
    //case UDSCAP_DOCS_IN_ADF:
    //  {
    //    CTWAINContainerInt *pIntCon = dynamic_cast<CTWAINContainerInt*>(m_IndependantCapMap[UDSCAP_DOCS_IN_ADF]);
    //    if( NULL != pIntCon )
    //    {
				//	::MessageBox(g_hwndDLG,TEXT("CTWAINDS_UDS::findCapability!"),MB_CAPTION,MB_OK);
    //      pIntCon->SetCurrent( m_pScanner->GetMaxPagesInADF());
    //      pRet = pIntCon;
    //    }
    //  }
    //break;

    case CAP_FEEDERLOADED:
      {
        CTWAINContainerBool *pBoolCon = dynamic_cast<CTWAINContainerBool*>(m_IndependantCapMap[CAP_FEEDERLOADED]);
        if( NULL != pBoolCon )
        {
          pBoolCon->SetCurrent( m_pScanner->isFeederLoaded()?TRUE:FALSE );
          pRet = pBoolCon;
        }
      }
    break;

    case ICAP_XRESOLUTION:
    case ICAP_YRESOLUTION:
    case ICAP_PHYSICALWIDTH:
    case ICAP_PHYSICALHEIGHT:
		case UDSCAP_MULTISTREAM_VALUE:
		case UDSCAP_EDGE_UP:
		case UDSCAP_EDGE_DOWN:
		case UDSCAP_EDGE_LEFT:
		case UDSCAP_EDGE_RIGHT:
		case UDSCAP_XPOS:
		case UDSCAP_YPOS:
		case UDSCAP_RESOLUTIONFC:  //多流分辨率
		case UDSCAP_RESOLUTIONFG:      
		case UDSCAP_RESOLUTIONFB:           
		case UDSCAP_RESOLUTIONBC:    
		case UDSCAP_RESOLUTIONBG:
		case UDSCAP_RESOLUTIONBB:
    {
      TWAINCapabilitiesMap_FIX32::iterator itCap = m_ICAP_UNIT_Dependant.find(_unCap);

      if(itCap != m_ICAP_UNIT_Dependant.end())
      {
        pRet = itCap->second;
      }
    }
    break;

    default:
      // cycle through m_IndependantCapMap
      pRet = CTWAINDS_Base::findCapability(_unCap);
    break;
  }
  
  return pRet;
}

//////////////////////////////////////////////////////////////////////////////
CTWAINContainer* CTWAINDS_UDS::getICAP_BITDEPTH()
{//::MessageBox(g_hwndDLG,"getICAP_BITDEPTH",MB_CAPTION,MB_OK);
  CTWAINContainer* pRet = 0;

  // BitDepth depends on PixelType
  TWAINCapabilitiesMap::iterator itCap = m_IndependantCapMap.find(ICAP_PIXELTYPE);

  if(itCap != m_IndependantCapMap.end())
  {
    int nCurrentVal = -1;
    CTWAINContainerInt* pnCap = dynamic_cast<CTWAINContainerInt*>(itCap->second);

    if((0 != pnCap) && (pnCap->GetCurrent(nCurrentVal)))
    {
      TWAINCapabilitiesMap_int::iterator itBDCap = m_BitDepthMap.find(nCurrentVal);
      if(itBDCap != m_BitDepthMap.end())
      {
        pRet = itBDCap->second;
      }
    }
  }
  
  return pRet;
}

//////////////////////////////////////////////////////////////////////////////
TW_INT16 CTWAINDS_UDS::dat_imagelayout(TW_UINT16         _MSG,
                                             pTW_IMAGELAYOUT   _pData)
{//::MessageBox(g_hwndDLG,TEXT("CTWAINDS_UDS::dat_imagelayout"),MB_CAPTION,MB_OK);
  TW_INT16 twrc = TWRC_SUCCESS;

  switch(_MSG)
  {
    case MSG_GET:
    case MSG_GETCURRENT:
    case MSG_GETDEFAULT:
      if( !( dsState_Open      == m_CurrentState || 
             dsState_Enabled   == m_CurrentState || 
             dsState_XferReady == m_CurrentState   ) )
      {
        setConditionCode(TWCC_SEQERROR);
        return TWRC_FAILURE;
      }
      break;

    case MSG_RESET:
    case MSG_SET:
      if( dsState_Open != m_CurrentState )
      {
        setConditionCode(TWCC_SEQERROR);
        return TWRC_FAILURE;
      }
      break;
  }

  if((0 == _pData) || (0 == m_pICAP_FRAMES))
  {
    setConditionCode(TWCC_BADVALUE);
    return TWRC_FAILURE;
  }

  CTWAINContainerInt* pDepCap = dynamic_cast<CTWAINContainerInt*>(findCapability(ICAP_SUPPORTEDSIZES));

  switch(_MSG)
  {
    case MSG_RESET:
      if(!pDepCap->Reset())
      {
        setConditionCode(TWCC_CAPBADOPERATION);
        twrc = TWRC_FAILURE;
      }
      else
      {
        // this will update the frame to match the papersize
        updatePostDependencies(MSG_RESET, ICAP_SUPPORTEDSIZES);
      }

      //change the behavior to a get current
      _MSG = MSG_GETCURRENT;
    case MSG_GET:
    case MSG_GETCURRENT:
    case MSG_GETDEFAULT:
    {
      InternalFrame frame;
      
      if(MSG_GETDEFAULT == _MSG)
      {
        if(!m_pICAP_FRAMES->GetDefault(frame))
        {
          twrc = TWRC_FAILURE;
          setConditionCode(TWCC_CAPBADOPERATION);
        }
      }
      else
      {
        if(!m_pICAP_FRAMES->GetCurrent(frame))
        {
          twrc = TWRC_FAILURE;
          setConditionCode(TWCC_CAPBADOPERATION);
        }
      }
      
      if(TWRC_SUCCESS == twrc)
      {
        int   unit = TWUN_PIXELS;
        float Xres = 100;
        float Yres = 100;
        getCurrentUnits(unit, Xres, Yres);
        _pData->Frame = frame.AsTW_FRAME(unit, Xres, Yres);
        _pData->FrameNumber = m_pICAP_FRAMES->getIndexForValue(frame);
        _pData->DocumentNumber = m_DocumentNumber;
        _pData->PageNumber = m_PageNumber;
      }
    }
    break;

    case MSG_SET:
    {
      /// @todo check frame with physical extents.  If frame is too big or too small modify it 
      ///  and return TWCC_BADVALUE if too big else TWRC_CHECKSTATUS if changed.

      //  Application that want multiple frames should negotiated frames through ICAP_FRAMES
      //  Layout can only set the current frame.
      //  Our simple source only supports one frame.
      TW_CAPABILITY cap;

      TW_HANDLE hContainer = _DSM_Alloc(sizeof(TW_ONEVALUE_FRAME));
      if(0 != hContainer)
      {
        cap.hContainer = hContainer;
        cap.Cap = ICAP_FRAMES;
        cap.ConType = TWON_ONEVALUE;
        pTW_ONEVALUE_FRAME pCap = (pTW_ONEVALUE_FRAME)_DSM_LockMemory(cap.hContainer);
        pCap->Item = _pData->Frame;

        int   unit = TWUN_PIXELS;
        float Xres = 100;
        float Yres = 100;
        twrc = getCurrentUnits(unit, Xres, Yres);
        InternalFrame frame(pCap->Item, unit, Xres, Yres);
        if(ConstrainFrameToScanner(frame))
        {
          pCap->Item = frame.AsTW_FRAME(unit, Xres, Yres);
          twrc = TWRC_CHECKSTATUS;
        }

        pCap->ItemType = TWTY_FRAME;
        _DSM_UnlockMemory(hContainer);

        TW_INT16 Condition;
        if(m_pICAP_FRAMES->Set(&cap, Condition))
        {
          updatePostDependencies(MSG_SET, ICAP_FRAMES);
        }

        _DSM_Free(hContainer);
      }
      else
      {
        twrc = TWRC_FAILURE;
        setConditionCode(TWCC_CAPBADOPERATION);
      }

      if(_pData->DocumentNumber != TWON_DONTCARE32)
      {
        m_DocumentNumber = _pData->DocumentNumber;
      }
      if(_pData->PageNumber != TWON_DONTCARE32)
      {
        m_PageNumber = _pData->PageNumber;
      }
    }
    break;

    default:
      setConditionCode(TWCC_BADPROTOCOL);
      assert(0);
      twrc = TWRC_FAILURE;
    break;
  }

  return twrc;
}

//////////////////////////////////////////////////////////////////////////////
TW_INT16 CTWAINDS_UDS::GetGustomDSData(pTW_CUSTOMDSDATA _pDSData)
{//::MessageBox(g_hwndDLG,"GetGustomDSData",MB_CAPTION,MB_OK);
  stringstream DsData;
  if(StoreCustomDSdata(DsData))
  {
    DsData.seekp(0, ios_base::end);
    DWORD dwSize =(DWORD) DsData.tellp();
    _pDSData->InfoLength = dwSize+sizeof(TW_GUID);
    TW_HANDLE hData = _DSM_Alloc(_pDSData->InfoLength);
    if(hData==0)
    {
      setConditionCode(TWCC_LOWMEMORY);
      return TWRC_FAILURE;
    }
    char * pData = (char*)_DSM_LockMemory(hData);
    if(pData==0)
    {
      _DSM_Free(hData);
      setConditionCode(TWCC_LOWMEMORY);
      return TWRC_FAILURE;
    }
    memcpy(pData,&CustomDSGUI,sizeof(TW_GUID));
    pData +=sizeof(TW_GUID);
    DsData.seekg(0, ios_base::beg);
    DsData.read(pData,dwSize);
    _DSM_UnlockMemory(hData);
    _pDSData->hData=hData;
    return TWRC_SUCCESS;
  }
  setConditionCode(TWCC_BUMMER);
  return TWRC_FAILURE;
}

//////////////////////////////////////////////////////////////////////////////
TW_INT16 CTWAINDS_UDS::SetGustomDSData(pTW_CUSTOMDSDATA _pDSData)
{//::MessageBox(g_hwndDLG,"SetGustomDSData",MB_CAPTION,MB_OK);
  if(_pDSData->hData==0 || _pDSData->InfoLength<=sizeof(TW_GUID))
  {
    setConditionCode(TWCC_BADVALUE);
    return TWRC_FAILURE;
  }

  stringstream DsData;
  char * pData = (char*)_DSM_LockMemory(_pDSData->hData);
  if(pData==0)
  {
    setConditionCode(TWCC_LOWMEMORY);
    return TWRC_FAILURE;
  }
  if(memcmp(pData,&CustomDSGUI,sizeof(TW_GUID))==0)
  {
    pData +=sizeof(TW_GUID);
    DsData.write(pData,_pDSData->InfoLength);
    _DSM_UnlockMemory(_pDSData->hData);
    stringstream DsDataBackUp;
    if(StoreCustomDSdata(DsDataBackUp))//Back up current settings
    {
      if(ReadCustomDSdata(DsData))
      {
        return TWRC_SUCCESS;
      }
      ReadCustomDSdata(DsDataBackUp);//restore current settings
    }
  }
  setConditionCode(TWCC_BADVALUE);
  return TWRC_FAILURE;
}

//////////////////////////////////////////////////////////////////////////////
TW_INT16 CTWAINDS_UDS::validateCapabilitySet(TW_UINT16 _Cap, TW_UINT16  _ConType, BYTE* _pContainer)
{//::MessageBox(g_hwndDLG,"validateCapabilitySet",MB_CAPTION,MB_OK);
  TW_INT16 twrc  = CTWAINDS_Base::validateCapabilitySet(_Cap,_ConType,_pContainer);
  if(twrc!=TWRC_SUCCESS)
  {
    return twrc;
  }

  switch(_Cap)
  {
    case UDSCAP_DOCS_IN_ADF:
    {
      twrc = TWRC_FAILURE;
      if(TWON_ONEVALUE == _ConType)
      {
        pTW_ONEVALUE pCap = (pTW_ONEVALUE)_pContainer;

        if(pCap)
        {
          if( (TW_INT16)pCap->Item >= 0 )
          {
            twrc = TWRC_SUCCESS;
            m_pScanner->SetMaxPagesInADF((TW_INT16)pCap->Item);
						//::MessageBox(g_hwndDLG,TEXT("SetMaxPagesInADF!"),MB_CAPTION,MB_OK);
          }
        }
      }
      break;
    }
    default:
      break;
  }

  return twrc;
}

bool CTWAINDS_UDS::StartScanning()
{//::MessageBox(g_hwndDLG,"StartScanning",MB_CAPTION,MB_OK);
  // Update the scanner with the latest negotiated caps
  if(!updateScannerFromCaps())
  {
		::MessageBox(g_hwndDLG,TEXT("StartScanning::There was an error while prepping the image for scanning!"),MB_CAPTION,MB_OK);
    //cerr << "ds: There was an error while prepping the image for scanning" << endl;
    setConditionCode(TWCC_BADVALUE);
    return false;
  }
	//::MessageBox(g_hwndDLG,"Before acquireImage",MB_CAPTION,MB_OK);
  return m_pScanner->acquireImage();
};


void CTWAINDS_UDS::SetScannerImagePath_Multi(vector<string> vector_string_imagepath)
{
//	m_pScanner->SetImagePath_Multi(vector_string_imagepath);
}

//void CTWAINDS_UDS::GetImagePathFromINI()
//{
//	char  szIniPath[PATH_MAX];    // INI配置文件路径  
//	vector<string> vector_string_imagepath;
//	strncpy_s(szIniPath,m_pScanner->GetINIPath(),MAX_PATH);
//
//	unsigned int unCount = GetPrivateProfileInt(INI_SECTION_IMAGE,
//	INI_KEY_IMAGECOUNT,0,szIniPath);  // 获取图片总数
//
//	for (unsigned int i=1;i <= unCount; i++)
//	{
//		char szTemp[64] = {0};
//		_itoa_s(i,szTemp,10); 
//		
//		//::MessageBox(g_hwndDLG,szTemp,MB_CAPTION,MB_OK);
//		
//		char szImagePath[MAX_PATH];
//		string str_key_imagepath = INI_KEY_PREFIX_IMAGEPATH ;
//		string str_image_index(szTemp);  // char->string
//		str_key_imagepath += str_image_index;
//		
//		GetPrivateProfileString(INI_SECTION_IMAGE,str_key_imagepath.c_str(),
//			"",szImagePath,MAX_PATH,szIniPath);
//		
//		string str_image_path(szImagePath);
//		vector_string_imagepath.push_back(str_image_path);
//		//::MessageBox(g_hwndDLG,str_image_path.c_str(),MB_CAPTION,MB_OK);
//	}
//	
//	m_pScanner->SetImagePath_Multi(vector_string_imagepath);
//  m_pGUI->SetCapValueInt(UDSCAP_DOCS_IN_ADF,unCount);	
//}









/**
* @file CTWAINDS_FreeImage.cpp
* The main Data Source class.
* This class is derived from the Base class describing a TWAIN DS.
* @author TWAIN Working Group
* @date April 2007
*/

#include "CTWAINDS_UDS.h"
#include "TWAIN_UI.h"
#include "FreeImage.h"
#include <sstream>

#include <iostream>
#include <cstdlib>
#include <assert.h>
#include <signal.h>
#include <typeinfo>

#ifdef TWH_CMP_MSC
  #include <Winbase.h>
#endif

#include "public.h"

extern HWND g_hwndDLG;
extern int g_nDeviceNumber;  // 设备编号

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
		1,                     // TW_UINT16  MajorNum;         Major revision number of the software
			0,                   // TW_UINT16  MinorNum;         Incremental revision number of the software
			TWLG_CHINESE_PRC,    // TW_UINT16  Language;         e.g. TWLG_SWISSFRENCH
			TWCY_CHINA,          // TW_UINT16  Country;          e.g. TWCY_SWITZERLAND
			"3.2.0"       // TW_STR32   Info;             e.g. "1.0b3 Beta release"
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
	3,                                  // TW_UINT16  ProtocolMajor;    Application and DS must set to TWON_PROTOCOLMAJOR
	2,                                  // TW_UINT16  ProtocolMinor;    Application and DS must set to TWON_PROTOCOLMINOR
	DG_IMAGE | DG_CONTROL | DF_DS2,     // TW_UINT32  SupportedGroups;  Bit field OR combination of DG_ constants
	MB_CAPTION,              // TW_STR32   Manufacturer;     Manufacturer name, e.g. "Hewlett-Packard"
	"UDS Scanner",    // TW_STR32   ProductFamily;    Product family name, e.g. "ScanJet"
	MB_CAPTION // TW_STR32   ProductName;      Product name, e.g. "ScanJet Plus"
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

	//wan
	switch (g_nDeviceNumber)
	{
	case DEVICE_FREEIMAGE:
		{
			m_pScanner = new CScanner_FreeImage;
		}
		break;
	case DEVICE_G6400:
		{
			m_pScanner = new CScanner_G6400;
			//::MessageBox(g_hwndDLG,"G6400",MB_CAPTION,MB_OK);
		}
		break;
	default:
		{
			::MessageBox(g_hwndDLG,TEXT("不支持的设备!"),MB_CAPTION,MB_OK);
		}
		break;
	}//wan
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
  bResult = bResult && StoreCapInStream(DsData,CAP_FEEDERENABLED,0,TWON_ONEVALUE);
  bResult = bResult && StoreCapInStream(DsData,CAP_DUPLEXENABLED,0,TWON_ONEVALUE);
  bResult = bResult && StoreCapInStream(DsData,CAP_AUTOFEED,0,TWON_ONEVALUE);
  bResult = bResult && StoreCapInStream(DsData,ICAP_BITDEPTH,0,TWON_ONEVALUE);
  bResult = bResult && StoreCapInStream(DsData,ICAP_BITORDER,0,TWON_ONEVALUE);
  bResult = bResult && StoreCapInStream(DsData,ICAP_COMPRESSION,0,TWON_ONEVALUE);
  bResult = bResult && StoreCapInStream(DsData,ICAP_FRAMES,0,TWON_ONEVALUE);
  bResult = bResult && StoreCapInStream(DsData,ICAP_IMAGEFILEFORMAT,0,TWON_ONEVALUE);
  bResult = bResult && StoreCapInStream(DsData,ICAP_PIXELFLAVOR,0,TWON_ONEVALUE);
  bResult = bResult && StoreCapInStream(DsData,ICAP_PIXELTYPE,0,TWON_ONEVALUE);
  bResult = bResult && StoreCapInStream(DsData,ICAP_PLANARCHUNKY,0,TWON_ONEVALUE);
  bResult = bResult && StoreCapInStream(DsData,ICAP_SUPPORTEDSIZES,0,TWON_ONEVALUE);
  bResult = bResult && StoreCapInStream(DsData,ICAP_ORIENTATION,0,TWON_ONEVALUE);
  bResult = bResult && StoreCapInStream(DsData,ICAP_UNITS,0,TWON_ONEVALUE);
  bResult = bResult && StoreCapInStream(DsData,ICAP_XRESOLUTION,0,TWON_ONEVALUE);
  bResult = bResult && StoreCapInStream(DsData,ICAP_YRESOLUTION,0,TWON_ONEVALUE);
  bResult = bResult && StoreCapInStream(DsData,ICAP_THRESHOLD,0,TWON_ONEVALUE);
  bResult = bResult && StoreCapInStream(DsData,ICAP_CONTRAST,0,TWON_ONEVALUE);
  bResult = bResult && StoreCapInStream(DsData,ICAP_BRIGHTNESS,0,TWON_ONEVALUE);
  bResult = bResult && StoreCapInStream(DsData,ICAP_GAMMA,0,TWON_ONEVALUE);
  bResult = bResult && StoreCapInStream(DsData,CUSTCAP_LONGDOCUMENT,0,TWON_ONEVALUE);
	bResult = bResult && StoreCapInStream(DsData,ICAP_ROTATION,0,TWON_ONEVALUE); //zhu
  return bResult;
}

bool CTWAINDS_UDS::ReadCustomDSdata(stringstream &DsData)
{//::MessageBox(g_hwndDLG,TEXT("ReadCustomDSdata"),MB_CAPTION,MB_OK);
  // When adding to Capabiltiy remember the order of operations 
  // Some capabilities are dependent on others.
  // see: http://www.twain.org/docs/CapOrderForWeb.PDF
  bool bResult = true;
  bResult = bResult && ReadCapFromStream(DsData,CUSTCAP_LONGDOCUMENT,0);
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
  bResult = bResult && ReadCapFromStream(DsData,ICAP_CONTRAST,0);
  bResult = bResult && ReadCapFromStream(DsData,ICAP_BRIGHTNESS,0);
  bResult = bResult && ReadCapFromStream(DsData,ICAP_GAMMA,0);
  bResult = bResult && ReadCapFromStream(DsData,ICAP_SUPPORTEDSIZES,0);
  bResult = bResult && ReadCapFromStream(DsData,ICAP_ORIENTATION,0);
  bResult = bResult && ReadCapFromStream(DsData,ICAP_FRAMES,0);
	bResult = bResult && ReadCapFromStream(DsData,ICAP_ROTATION,0); //zhu
  return bResult;
}

//////////////////////////////////////////////////////////////////////////////
TW_INT16 CTWAINDS_UDS::Initialize()
{//::MessageBox(g_hwndDLG,TEXT("Initialize"),MB_CAPTION,MB_OK);
  // When adding to Capabiltiy remember the order of operations 
  // Some capabilities are dependent on others.
  // see: http://www.twain.org/docs/CapOrderForWeb.PDF

  // setup the supported independant caps
  CTWAINContainerInt* pnCap = 0;
  CTWAINContainerBool* pbCap = 0;
  CTWAINContainerString* pstrCap = 0;
  CTWAINContainerFix32* pfixCap = 0;

  m_IndependantCapMap[CAP_SUPPORTEDCAPS] = new CTWAINContainerInt(CAP_SUPPORTEDCAPS, TWTY_UINT16, TWON_ARRAY, TWQC_GETS);
  if( NULL == (pnCap = dynamic_cast<CTWAINContainerInt*>(m_IndependantCapMap[CAP_SUPPORTEDCAPS]))
   || !pnCap->Add(CAP_DEVICEONLINE)   //
   || !pnCap->Add(CAP_INDICATORS)  //指示灯
   || !pnCap->Add(CAP_ENABLEDSUIONLY)  //打开驱动界面
   || !pnCap->Add(CAP_PAPERDETECTABLE)  //纸张检测
   || !pnCap->Add(CAP_FEEDERENABLED)  //启动进纸
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
   || !pnCap->Add(ICAP_FRAMES)
   || !pnCap->Add(ICAP_MAXFRAMES)
   || !pnCap->Add(ICAP_IMAGEFILEFORMAT)
   || !pnCap->Add(ICAP_PHYSICALHEIGHT)
   || !pnCap->Add(ICAP_PHYSICALWIDTH)
   || !pnCap->Add(ICAP_PIXELFLAVOR)
   || !pnCap->Add(ICAP_PIXELTYPE)    //图形类型
   || !pnCap->Add(ICAP_PLANARCHUNKY)
   || !pnCap->Add(ICAP_SUPPORTEDSIZES)  //纸张大小
   || !pnCap->Add(ICAP_ORIENTATION) //方向
   || !pnCap->Add(ICAP_UNITS) //单位
   || !pnCap->Add(ICAP_XFERMECH)
   || !pnCap->Add(ICAP_XRESOLUTION)
   || !pnCap->Add(ICAP_YRESOLUTION) 
   || !pnCap->Add(ICAP_THRESHOLD) //阈值 
   || !pnCap->Add(ICAP_CONTRAST)  //分辨率
   || !pnCap->Add(ICAP_BRIGHTNESS)  //亮度
   || !pnCap->Add(ICAP_GAMMA) //Gamma校正
   || !pnCap->Add(CAP_CUSTOMINTERFACEGUID)
   || !pnCap->Add(CUSTCAP_LONGDOCUMENT)  //长纸
   || !pnCap->Add(CUSTCAP_DOCS_IN_ADF) 
   || !pnCap->Add(CAP_CUSTOMDSDATA) 
	 || !pnCap->Add(ICAP_ROTATION) //图像旋转 zhu

   )
  {
		::MessageBox(g_hwndDLG,TEXT("Could not create CAP_SUPPORTEDCAPS !"),MB_CAPTION,MB_OK);
    //cerr << "Could not create CAP_SUPPORTEDCAPS" << endl;
    setConditionCode(TWCC_LOWMEMORY);
    return TWRC_FAILURE;
  }

  m_IndependantCapMap[ICAP_COMPRESSION] = new CTWAINContainerInt(ICAP_COMPRESSION, TWTY_UINT16, TWON_ENUMERATION);
  if( NULL == (pnCap = dynamic_cast<CTWAINContainerInt*>(m_IndependantCapMap[ICAP_COMPRESSION]))
   || !pnCap->Add(TWCP_NONE, true) )
  {
		::MessageBox(g_hwndDLG,TEXT("Could not create ICAP_COMPRESSION !"),MB_CAPTION,MB_OK);
    //cerr << "Could not create ICAP_COMPRESSION" << endl;
    setConditionCode(TWCC_LOWMEMORY);
    return TWRC_FAILURE;
  }

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
   || !pnCap->Add(TWUN_CENTIMETERS)
   || !pnCap->Add(TWUN_PICAS)
   || !pnCap->Add(TWUN_POINTS)
   || !pnCap->Add(TWUN_TWIPS) )
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

  m_IndependantCapMap[ICAP_SUPPORTEDSIZES] = new CTWAINContainerInt(ICAP_SUPPORTEDSIZES, TWTY_UINT16, TWON_ENUMERATION);
  if( NULL == (pnCap = dynamic_cast<CTWAINContainerInt*>(m_IndependantCapMap[ICAP_SUPPORTEDSIZES]))
   //|| !pnCap->Add(TWSS_NONE) //wan
   || !pnCap->Add(TWSS_USLETTER, true)  //纸张大小,默认USLETTER
   || !pnCap->Add(TWSS_USLEGAL)
   || !pnCap->Add(TWSS_A4)  //wan
   || !pnCap->Add(TWSS_A5)  //wan
   || !pnCap->Add(TWSS_A6)  //
   || !pnCap->Add(TWSS_A7)  //wan
   || !pnCap->Add(TWSS_ISOB5)  //wan
   || !pnCap->Add(TWSS_ISOB6)  //wan
   || !pnCap->Add(TWSS_ISOB7)  //wan
   || !pnCap->Add(TWSS_JISB5)  //wan
   || !pnCap->Add(TWSS_JISB6)  //wan
   || !pnCap->Add(TWSS_JISB7)  //wan
	 )
  {
		::MessageBox(g_hwndDLG,TEXT("Could not create ICAP_SUPPORTEDSIZES !"),MB_CAPTION,MB_OK);
    //cerr << "Could not create ICAP_SUPPORTEDSIZES" << endl;
    setConditionCode(TWCC_LOWMEMORY);
    return TWRC_FAILURE;
  }

	//zhu
	m_IndependantCapMap[ICAP_ROTATION] = new CTWAINContainerInt(ICAP_ROTATION, TWTY_UINT16, TWON_ENUMERATION);
	if(NULL == (pnCap = dynamic_cast<CTWAINContainerInt*>(m_IndependantCapMap[ICAP_ROTATION]))
		|| !pnCap->Add(TWOR_ROT0, true)  
		|| !pnCap->Add(TWOR_ROT90)
		|| !pnCap->Add(TWOR_ROT180)
		|| !pnCap->Add(TWOR_ROT270))  
	{
		::MessageBox(g_hwndDLG,TEXT("Could not create ICAP_ROTATION !"),MB_CAPTION,MB_OK);
		//cerr<<"Could not create ICAP_ROTATION"<<endl;
		setConditionCode(TWCC_LOWMEMORY);
		return TWRC_FAILURE;
	} //zhu
		

  m_IndependantCapMap[ICAP_ORIENTATION] = new CTWAINContainerInt(ICAP_ORIENTATION, TWTY_UINT16, TWON_ENUMERATION);
  if( NULL == (pnCap = dynamic_cast<CTWAINContainerInt*>(m_IndependantCapMap[ICAP_ORIENTATION]))
   || !pnCap->Add(TWOR_LANDSCAPE)  //wan
	 || !pnCap->Add(TWOR_PORTRAIT, true))
  {
		::MessageBox(g_hwndDLG,TEXT("Could not create ICAP_ORIENTATION !"),MB_CAPTION,MB_OK);
    //cerr << "Could not create ICAP_ORIENTATION" << endl;
    setConditionCode(TWCC_LOWMEMORY);
    return TWRC_FAILURE;
  }

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
   || !pbCap->Add(TRUE, true) )  //wan，扫描模式，默认自动进纸器
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
	 || !pbCap->Add(TRUE)  
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

  m_IndependantCapMap[CUSTCAP_LONGDOCUMENT] = new CTWAINContainerBool(CUSTCAP_LONGDOCUMENT, (m_AppID.SupportedGroups&DF_APP2)!=0, TWQC_ALL);
  if( NULL == (pbCap = dynamic_cast<CTWAINContainerBool*>(m_IndependantCapMap[CUSTCAP_LONGDOCUMENT]))
   || !pbCap->Add(TRUE)
   || !pbCap->Add(FALSE, true) )
  {
		::MessageBox(g_hwndDLG,TEXT("Could not create CUSTCAP_LONGDOCUMENT !"),MB_CAPTION,MB_OK);
    //cerr << "Could not create CUSTCAP_LONGDOCUMENT" << endl;
    setConditionCode(TWCC_LOWMEMORY);
    return TWRC_FAILURE;
  }

  m_IndependantCapMap[CUSTCAP_DOCS_IN_ADF] = new CTWAINContainerInt(CUSTCAP_DOCS_IN_ADF, TWTY_UINT16, TWON_ONEVALUE, TWQC_ALL);
  if( NULL == (pnCap = dynamic_cast<CTWAINContainerInt*>(m_IndependantCapMap[CUSTCAP_DOCS_IN_ADF]))
   || !pnCap->Add(m_pScanner->GetMaxPagesInADF()))
  {
		::MessageBox(g_hwndDLG,TEXT("Could not create CUSTCAP_DOCS_IN_ADF !"),MB_CAPTION,MB_OK);
    //cerr << "Could not create CUSTCAP_DOCS_IN_ADF" << endl;
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
  
  m_IndependantCapMap[ICAP_GAMMA] = new CTWAINContainerFix32(ICAP_GAMMA,TWON_ONEVALUE, TWQC_ALL);
  if( NULL == (pfixCap = dynamic_cast<CTWAINContainerFix32*>(m_IndependantCapMap[ICAP_GAMMA]))
    || !pfixCap->Add(1, true))
  {
		::MessageBox(g_hwndDLG,TEXT("Could not create ICAP_GAMMA !"),MB_CAPTION,MB_OK);
    //cerr << "Could not create ICAP_GAMMA" << endl;
    setConditionCode(TWCC_LOWMEMORY);
    return TWRC_FAILURE;
  }
  FLOAT_RANGE fRange;
  fRange.fCurrentValue = 128.0f;
  fRange.fMaxValue = 255.0f;
  fRange.fMinValue = 0.0f;
  fRange.fStepSize = 1.0f;
  m_IndependantCapMap[ICAP_THRESHOLD] = new CTWAINContainerFix32Range(ICAP_THRESHOLD,fRange, TWQC_ALL);
  if( NULL == dynamic_cast<CTWAINContainerFix32Range*>(m_IndependantCapMap[ICAP_THRESHOLD]))
  {
		::MessageBox(g_hwndDLG,TEXT("Could not create ICAP_THRESHOLD !"),MB_CAPTION,MB_OK);
    //cerr << "Could not create ICAP_THRESHOLD" << endl;
    setConditionCode(TWCC_LOWMEMORY);
    return TWRC_FAILURE;
  }

  fRange.fCurrentValue = 0.0f;
  fRange.fMaxValue = 1000.0f;
  fRange.fMinValue = -1000.0f;
  fRange.fStepSize = 1.0f;
  m_IndependantCapMap[ICAP_CONTRAST] = new CTWAINContainerFix32Range(ICAP_CONTRAST,fRange, TWQC_ALL);
  if( NULL == dynamic_cast<CTWAINContainerFix32Range*>(m_IndependantCapMap[ICAP_CONTRAST]))
  {
		::MessageBox(g_hwndDLG,TEXT("Could not create ICAP_CONTRAST !"),MB_CAPTION,MB_OK);
    //cerr << "Could not create ICAP_CONTRAST" << endl;
    setConditionCode(TWCC_LOWMEMORY);
    return TWRC_FAILURE;
  }

  fRange.fCurrentValue = 0.0f;
  fRange.fMaxValue = 1000.0f;
  fRange.fMinValue = -1000.0f;
  fRange.fStepSize = 1.0f;
  m_IndependantCapMap[ICAP_BRIGHTNESS] = new CTWAINContainerFix32Range(ICAP_BRIGHTNESS,fRange, TWQC_ALL);
  if( NULL == dynamic_cast<CTWAINContainerFix32Range*>(m_IndependantCapMap[ICAP_BRIGHTNESS]))
  {
		::MessageBox(g_hwndDLG,TEXT("Could not create ICAP_BRIGHTNESS !"),MB_CAPTION,MB_OK);
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
		::MessageBox(g_hwndDLG,TEXT(" !"),MB_CAPTION,MB_OK);
    //cerr << "Could not create ICAP_PHYSICALHEIGHT" << endl;
    setConditionCode(TWCC_LOWMEMORY);
    return TWRC_FAILURE;
  }
	
  // setup the ICAP_FRAMES
  // expressed internally as 1000 pixels per inch
  // Currently only supports one frame see: ICAP_MAXFRAMES
  if( NULL == (m_pICAP_FRAMES = new CTWAINContainerFrame(ICAP_FRAMES, TWON_ENUMERATION, TWQC_ALL))
   || !m_pICAP_FRAMES->Add(0, 0, 8500, 11000, true) )
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
{//::MessageBox(g_hwndDLG,"getImageInfo",MB_CAPTION,MB_OK);
  TW_INT16 twrc = TWRC_SUCCESS;

  memset(_pImageInfo, 0, sizeof(TW_IMAGEINFO));

  // Only valid in state 6 and 7(State 7 only after receiving TWRC_XFERDONE)
  if( dsState_XferReady > m_CurrentState )
  {
    setConditionCode(TWCC_SEQERROR);
    return TWRC_FAILURE;
  }

  // Get the actual values used by the scanner.
  CScanner_Base settings = *(m_pScanner->getSetting());

  _pImageInfo->XResolution = FloatToFIX32(settings.m_fXResolution);
  _pImageInfo->YResolution = FloatToFIX32(settings.m_fYResolution);
  _pImageInfo->ImageWidth  = settings.m_nWidth;
  _pImageInfo->ImageLength = settings.m_nHeight;
  
  // Our sample scanner only does one combination for each PixelType.
  switch(settings.m_nPixelType)
  {
    case TWPT_BW:
      _pImageInfo->PixelType = TWPT_BW;
      _pImageInfo->BitsPerPixel = 1;
      _pImageInfo->SamplesPerPixel = 1;
      _pImageInfo->BitsPerSample[0] = 1;
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

	//zhu
	/*switch(settings.m_nRotation)
	{
	case TWOR_ROT0:
	_pImageInfo->Rotation = 0;
	break;
	case TWOR_ROT90:
	_pImageInfo->Rotation = 90;
	break;
	case TWOR_ROT180:
	_pImageInfo->Rotation = 180;
	break;
	case TWOR_ROT270:
	_pImageInfo->Rotation = 270;
	break;
	}*/

  return twrc;
}

//////////////////////////////////////////////////////////////////////////////
TW_INT16 CTWAINDS_UDS::openDS(pTW_IDENTITY  _pOrigin)
{
//::MessageBox(g_hwndDLG,"openDS",MB_CAPTION,MB_OK);
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
  return TWRC_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////////
TW_INT16 CTWAINDS_UDS::enableDS(pTW_USERINTERFACE _pData)
{
	//::MessageBox(g_hwndDLG,"enableDS",MB_CAPTION,MB_OK);
	g_hwndDLG = (HWND)_pData->hParent;
  if( dsState_Open != m_CurrentState )
  {
    setConditionCode(TWCC_SEQERROR);
    return TWRC_FAILURE;
  }
  m_CurrentState = dsState_Enabled;
  m_bCanceled = false;


  //set pending xfers to whatever the user configured for XferCount
  int Count = TWON_DONTCARE32;
  CTWAINContainerInt *pnCap = dynamic_cast<CTWAINContainerInt*>(findCapability(CAP_XFERCOUNT));
  if(pnCap)
  {
    pnCap->GetCurrent(Count);
  }
	//::MessageBox(g_hwndDLG,"CAP_XFERCOUNT",MB_CAPTION,MB_OK);
  m_Xfers.Count = Count;

  // Indicate we have not transferred any images yet
  m_DocumentNumber = 0;
  m_PageNumber     = 0;

  // The application will move to state 5 after this triplet which means that
  // no more capabilities can be set until we are brought back to state 4.
  m_pScanner->Lock();

	//::MessageBox((HWND)_pData->hParent,"Enable",MB_CAPTION,MB_OK);

  if(FALSE == _pData->ShowUI)
  {   
		//::MessageBox(g_hwndDLG,"ShowUI",MB_CAPTION,MB_OK);
		//::MessageBox((HWND)_pData->hParent,"NoUI",MB_CAPTION,MB_OK);
    // Update the scanner with the latest negotiated caps
    if(!updateScannerFromCaps())
    {
			::MessageBox(g_hwndDLG,TEXT("There was an error while prepping the image for scanning !"),MB_CAPTION,MB_OK);
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

		
		//::MessageBox(g_hwndDLG," EnableDS: acquireImage! ",MB_CAPTION,MB_OK);
    if(!m_pScanner->acquireImage())
    {
			::MessageBox(g_hwndDLG,TEXT("There was an error while trying to get scanner to acquire image!"),MB_CAPTION,MB_OK);
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
		//::MessageBox(hwndDLG,"DisplayTWAINGUI","UDS Scanner",MB_OK);
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
  _pData->MaxBufSize = MAX(64*1024, bytes);//bytes;
  _pData->Preferred  = MAX(64*1024, bytes);//bytes;

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
    DWORD nDestBytesPerRow = BYTES_PERLINE(m_ImageInfo.ImageWidth, m_ImageInfo.BitsPerPixel);
    DWORD nImageSize       = nDestBytesPerRow * m_ImageInfo.ImageLength;

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
		case DEVICE_FREEIMAGE:  //CScanner_FreeImage
			{				
				do
				{
					dwRead = MIN(64000, nImageSize) / nDestBytesPerRow * nDestBytesPerRow;
					dwReceived =0;

					if( !m_pScanner->getScanStrip(pImageData, dwRead, dwReceived) ||
						dwReceived != dwReceived / nDestBytesPerRow * nDestBytesPerRow)
					{
						//No more data to recieve fill the rest
						break;
					}
					pImageData += dwReceived;

					nImageSize -= dwReceived;
				}while(nImageSize>0 && twrc == TWRC_SUCCESS);

			}
			break;
		case DEVICE_G6400:  // CScanner_G6400
			{				
				m_pScanner->GetImageData(pImageData,dwReceived);
				pImageData += dwReceived;
			}
			break;
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
{//::MessageBox(g_hwndDLG,"endXfer",MB_CAPTION,MB_OK);
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
				::MessageBox(g_hwndDLG,TEXT("ds: There was an error while prepping the image for scanning!"),MB_CAPTION,MB_OK);
        //cerr << "ds: There was an error while prepping the image for scanning" << endl;
        setConditionCode(TWCC_BUMMER);
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
{//::MessageBox(g_hwndDLG,"updateScannerFromCaps",MB_CAPTION,MB_OK);
  int   nVal;
  float fVal;
  bool  bret = true;  // Set to false if anything fails
  CScanner_Base settings;

  // Get current before updating
  settings = *(m_pScanner->getSetting());

//	::MessageBox(g_hwndDLG,"getSetting",MB_CAPTION,MB_OK);

  CTWAINContainerInt    *pnCap = 0;
  CTWAINContainerFix32  *pfCap = 0;
  CTWAINContainerFix32Range *pfRCap = 0;

  if(0 == (pnCap = dynamic_cast<CTWAINContainerInt*>(findCapability(ICAP_PIXELTYPE))))
  {
    //cerr << "Could not get ICAP_PIXELTYPE" << endl;
		::MessageBox(g_hwndDLG,TEXT("Could not get ICAP_PIXELTYPE!"),MB_CAPTION,MB_OK);
    bret = false;
  }
  else
  {
    pnCap->GetCurrent(nVal);
    settings.m_nPixelType = nVal;
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

  // set the image dimensions
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

	//zhu
	if(0 == (pnCap = dynamic_cast<CTWAINContainerInt*>(findCapability(ICAP_ROTATION))))
	{
		::MessageBox(g_hwndDLG,TEXT("Could not get ICAP_ROTATION!"),MB_CAPTION,MB_OK);
		//cerr << "Could not get ICAP_ROTATION" << endl;
		bret = false;
	}
	else
	{
		pnCap->GetCurrent(nVal);
		settings.m_nRotation = nVal;
	}//zhu

	//zhu
	if(0 == (pnCap = dynamic_cast<CTWAINContainerInt*>(findCapability(ICAP_ORIENTATION))))
	{
		::MessageBox(g_hwndDLG,TEXT("Could not get ICAP_ORIENTATION!"),MB_CAPTION,MB_OK);
		//cerr << "Could not get ICAP_ORIENTATION" << endl;
		bret = false;
	}
	else
	{
		pnCap->GetCurrent(nVal);
		settings.m_nOrientation = nVal;
	}//zhu


  if(0 == (pfRCap = dynamic_cast<CTWAINContainerFix32Range*>(findCapability(ICAP_BRIGHTNESS))))
  {
		::MessageBox(g_hwndDLG,TEXT("Could not get ICAP_BRIGHTNESS!"),MB_CAPTION,MB_OK);
    //cerr << "Could not get ICAP_BRIGHTNESS" << endl;
    bret = false;
  }
  else
  {
    pfRCap->GetCurrent(fVal);
	/*	char buf[1024];
    itoa((int)fVal,buf,10);
		::MessageBox(hwndDLG,buf,MB_CAPTION,MB_OK);*/
    settings.m_fBrightness = fVal;
  }
  // Y resolution the same.
  if(0 == (pfCap = dynamic_cast<CTWAINContainerFix32*>(findCapability(ICAP_GAMMA))))
  {
		::MessageBox(g_hwndDLG,TEXT("Could not get ICAP_GAMMA!"),MB_CAPTION,MB_OK);
    //cerr << "Could not get ICAP_GAMMA" << endl;
    bret = false;
  }
  else
  {
    pfCap->GetCurrent(fVal);
    settings.m_fGamma = fVal;
  }

  if(bret)
  {
		//::MessageBox(g_hwndDLG,"setSetting",MB_CAPTION,MB_OK);
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
    case CUSTCAP_DOCS_IN_ADF:
      {
        CTWAINContainerInt *pIntCon = dynamic_cast<CTWAINContainerInt*>(m_IndependantCapMap[CUSTCAP_DOCS_IN_ADF]);
        if( NULL != pIntCon )
        {
          pIntCon->SetCurrent( m_pScanner->GetMaxPagesInADF());
          pRet = pIntCon;
        }
      }
    break;

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
{//::MessageBox(g_hwndDLG,TEXT("dat_imagelayout"),MB_CAPTION,MB_OK);
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
    case CUSTCAP_DOCS_IN_ADF:
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
		::MessageBox(g_hwndDLG,TEXT("There was an error while prepping the image for scanning!"),MB_CAPTION,MB_OK);
    //cerr << "ds: There was an error while prepping the image for scanning" << endl;
    setConditionCode(TWCC_BADVALUE);
    return false;
  }
	//::MessageBox(g_hwndDLG,"Before acquireImage",MB_CAPTION,MB_OK);
  return m_pScanner->acquireImage();
};

//void CTWAINDS_UDS::SetScannerImagePath_Signal(char* szImagePath)
//{
//	//::MessageBox(NULL, szImagePath, "2", 0);  
//	m_pScanner->SetImagePath_Signal(szImagePath);
//}

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
//  m_pGUI->SetCapValueInt(CUSTCAP_DOCS_IN_ADF,unCount);	
//}








// Win32.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"

#include "dllmain.h"
#include <list>
#include "resource.h"
#include "public.h"
//////////////////////////////////////////////////////////////////////////////
/************************************************************************
Most DLL developers have faced the challenge of detecting a HMODULE/HINSTANCE
handle within the module you're running in. It may be a difficult task if you 
wrote the DLL without a DLLMain() function or you are unaware of its name. For 
example:Your DLL was built without ATL/MFC, so the DLLMain() function exists,
but it's hidden from you code and you cannot access the hinstDLL parameter. 
 You do not know the DLL's real file name because it could be renamedby everyone
, so GetModuleHandle() is not for you.This small code can help you solve this 
problem:                                                                      
************************************************************************/


#if _MSC_VER >= 1300    // for VC 7.0
// from ATL 7.0 sources
#ifndef _delayimp_h
extern "C" IMAGE_DOS_HEADER __ImageBase;
#endif
#endif

int g_nDeviceNumber;


// Globals
/**
* gloabal pointer of the Data Source, for access to the main DS_Entry.
*/
typedef struct _DS_inst
{
  TW_IDENTITY AppId;
  CTWAINDS_Base *pDS; 
}DS_inst;

typedef list<DS_inst> lstDS;
lstDS g_lstDS;

DS_inst g_DS_init;

// Globals
/** 
* @brief 获取DLL自身的句柄 
* @return 句柄
*/
static HMODULE GetCurrentModule()
{
#if _MSC_VER < 1300    // earlier than .NET compiler (VC 6.0)

	// Here's a trick that will get you the handle of the module
	// you're running in without any a-priori knowledge:
	MEMORY_BASIC_INFORMATION mbi;
	static int dummy;
	VirtualQuery( &dummy, &mbi, sizeof(mbi) );

	return reinterpret_cast<HMODULE>(mbi.AllocationBase);
#else    // VC 7.0
	// from ATL 7.0 sources
	return reinterpret_cast<HMODULE>(&__ImageBase);
#endif
}


#ifdef TWH_CMP_MSC
  /** 
  * gloadbal Windows Instance handle for the DSM DLL... 
  */
  HINSTANCE g_hinstance = (HINSTANCE)GetCurrentModule();
#endif

  HWND g_hwndDLG = NULL;   



// Globals
/**
*  @brief  获取指定文件的绝对路径.
*  @param[in] szFileName 文件名
*  @param[out] szFilePath 文件绝对路径.  
*/
void GetFilePath( char* szFileName, char* szFilePath)
{
	char szCurrentModuleDIR[PATH_MAX];  // 当前DS的路径
	char szTempPath[MAX_PATH];  // 临时存储的路径

#ifdef TWH_CMP_MSC
	GetModuleFileName(g_hinstance, szCurrentModuleDIR, MAX_PATH);

	// strip filename from path
	size_t len = strlen(szCurrentModuleDIR);

	while(len > 0)
	{
		if(PATH_SEPERATOR == szCurrentModuleDIR[len-1])
		{
			szCurrentModuleDIR[len-1] = 0;
			break;
		}
		--len;
	}
	len = strlen(szCurrentModuleDIR);

	strncpy_s(szTempPath, szCurrentModuleDIR, MAX_PATH);
#else
	// In non-windows, kTWAIN_DS_DIR is set on the compiler command line
	strncpy(szTempPath, szCurrentModuleDIR, MAX_PATH);
#endif

#if (TWNDS_CMP_VERSION >= 1400)
	if (szTempPath[len-1] != '\\')
	{
		strcat_s(szTempPath, MAX_PATH, "\\");
	}		
	strcat_s (szTempPath, MAX_PATH, szFileName);

#else
	if (szTempPath[len-1] != '\\')
	{
		strcat(szTempPath, "\\");
	}		
	strcat(szTempPath, szFileName);
#endif

	strncpy_s(szFilePath, MAX_PATH, szTempPath, MAX_PATH);
}

// Globals
/**
* 全局函数，通过INI文件获取当前调用的设备编号（扫描仪、高拍仪、虚拟扫描仪），
* @return 设备编号
* - 1表示FreeImage
* - 2表示G6400
*/
int GetDeviceNumberFromINI()
{
	char szIniFilePath[MAX_PATH];  // INI文件路径

	GetFilePath(FILENAME_INI,szIniFilePath);
	
	int nDeviceNumber = 0;
	// 获取设备编号,获取失败为FreeImage(1)
	nDeviceNumber = GetPrivateProfileInt(INI_APP_DEVICE,INI_KEY_DEVICENUMBER,1,szIniFilePath);

	return nDeviceNumber;
}


//////////////////////////////////////////////////////////////////////////////
// This is the main entry point. This function is dlsym'd by the DSM.

#ifdef TWH_CMP_MSC
TW_UINT16 FAR PASCAL
#else
FAR PASCAL TW_UINT16 
#endif
DS_Entry( pTW_IDENTITY _pOrigin,
          TW_UINT32    _DG,
          TW_UINT16    _DAT,
          TW_UINT16    _MSG,
          TW_MEMREF    _pData)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());  // 很重要，导出函数必须添加这一句

	g_nDeviceNumber = GetDeviceNumberFromINI();  // 保存INI中的设备编号
	
  CTWAINDS_Base* pTWAINLayer = 0;

  if(_pOrigin)
  {
    lstDS::iterator llIter = g_lstDS.begin();
    for(;llIter!=g_lstDS.end();llIter++)
    {
      if((*llIter).AppId.Id==_pOrigin->Id)
      {
        pTWAINLayer=(*llIter).pDS;
      }
    }
  }
  // Curently we are not open
  if( 0 == pTWAINLayer )
  {
    // Special case DSM can request to get identity information about 
    // DS before it is open.  In this special case, where the DS is not
    // open, we return this static Idenity.
    if( DG_CONTROL == _DG && DAT_IDENTITY == _DAT && MSG_GET == _MSG )
    {
      // Copy the ID assigned by the DSM eventhough the spec states
      // that the id will not be assigned until MSG_OPENDS
      CTWAINDS_Base::m_TheIdentity.Id = ((pTW_IDENTITY)_pData)->Id;
      memcpy( _pData, &CTWAINDS_Base::m_TheIdentity, sizeof(CTWAINDS_Base::m_TheIdentity) );

      return TWRC_SUCCESS;
    }

    // The DS is not open.  If we get a request to close DS do not open 
    // just to close, instead return that it is success closed.
    if( DG_CONTROL == _DG && DAT_IDENTITY == _DAT && MSG_CLOSEDS == _MSG )
    {
      return TWRC_SUCCESS;
    }

    // Open the DS
    pTWAINLayer = new CTWAINDS_UDS(*_pOrigin);
    if( NULL == pTWAINLayer 
     || TWRC_SUCCESS != pTWAINLayer->Initialize())
    {
      // Failed to create the DS 
      //setConditionCode(TWCC_LOWMEMORY);
      if(pTWAINLayer)
      {
        // Created but could not Initialize
        delete pTWAINLayer;
      }
      return TWRC_FAILURE;
    }
    DS_inst _DS;
    _DS.pDS = pTWAINLayer;
    _DS.AppId = *_pOrigin;
    g_lstDS.push_back(_DS);
		g_DS_init = _DS;

  }

	
  // If we were not open before, we are now, so continue with the TWAIN call
  TW_INT16 result = pTWAINLayer->DS_Entry(_pOrigin, _DG, _DAT, _MSG, _pData);

  /**
  * Special case - free memory if closing DS 
  * @todo keep track of what apps are connecting to the ds and only 
  * delete when count goes down to 0
  */
  if( TWRC_SUCCESS == result && 
      DG_CONTROL == _DG && DAT_IDENTITY == _DAT && MSG_CLOSEDS == _MSG &&
      NULL != pTWAINLayer )
  {
    lstDS::iterator llIter=g_lstDS.begin();
    for(;llIter!=g_lstDS.end();)
    {
      if((*llIter).AppId.Id==_pOrigin->Id)
      {
        delete  (*llIter).pDS;
        llIter = g_lstDS.erase(llIter);
        continue;
      }
      llIter++;
    }
  }

  return result;
}



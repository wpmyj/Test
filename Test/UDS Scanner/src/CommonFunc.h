#pragma once
#include "AVSDK.h"


#define LOG_NO_RETURN		-999


#define DEBUGLEVEL1			1
#define DEBUGLEVEL2			2
#define DEBUGLEVEL3			3

#define NUMBER_IFD			11

#include <gdiplus.h>				// Base include
using namespace Gdiplus;			// The "umbella"
#pragma comment(lib, "gdiplus.lib")	// The GDI+ binary


class CCommonFunc
{
private:
	ULONG_PTR	gdiplusToken;  ///<GDI+标记

public:
	CCommonFunc(void);
	~CCommonFunc(void);
	void Initialize(CString strPath, DWORD Level, DWORD Monitor);
	void DeleteGdiPlusObject();///<删除GDI+对象
	void GDISaveImage(BYTE *Data, CString ImagePath, IMAGEINFORMATION ImageInfo); ///<主要
	BOOL GetImageCLSID(CString format, CLSID *pCLSID);///<获取图像ID
	BOOL GDIReadImage(CString ImagePath, BYTE **pDes, IMAGEINFORMATION* pImageInfo);///<主要
	const wchar_t * cstringtowchar(CString str);
};

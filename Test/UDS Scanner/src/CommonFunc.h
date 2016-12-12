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
	ULONG_PTR	gdiplusToken;  ///<GDI+���

public:
	CCommonFunc(void);
	~CCommonFunc(void);
	void Initialize(CString strPath, DWORD Level, DWORD Monitor);
	void DeleteGdiPlusObject();///<ɾ��GDI+����
	void GDISaveImage(BYTE *Data, CString ImagePath, IMAGEINFORMATION ImageInfo); ///<��Ҫ
	BOOL GetImageCLSID(CString format, CLSID *pCLSID);///<��ȡͼ��ID
	BOOL GDIReadImage(CString ImagePath, BYTE **pDes, IMAGEINFORMATION* pImageInfo);///<��Ҫ
	const wchar_t * cstringtowchar(CString str);
};

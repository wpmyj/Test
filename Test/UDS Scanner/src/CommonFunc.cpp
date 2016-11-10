#include "StdAfx.h"
#include "windowsx.h"
#include "CommonFunc.h"
#include <string>
//using namespace std;
#include <atlbase.h>

CCommonFunc::CCommonFunc(void)
{
	gdiplusToken = NULL;
	GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);  //初始化GDI+函数库
}

CCommonFunc::~CCommonFunc(void)
{
}

void CCommonFunc::Initialize(CString strPath, DWORD Level, DWORD Monitor)
{
}


void CCommonFunc::DeleteGdiPlusObject()
{
	if(!gdiplusToken)
		GdiplusShutdown(gdiplusToken);
	gdiplusToken = NULL;
}

BOOL CCommonFunc::GetImageCLSID(CString format, CLSID *pCLSID)
{
	UINT num = 0;
	UINT size = 0;

	Gdiplus::ImageCodecInfo* pImageCodecInfo = NULL;
	Gdiplus::GetImageEncodersSize(&num, &size);
	if (size == 0)
	{
		return FALSE;
	}
	pImageCodecInfo = (Gdiplus::ImageCodecInfo *)(malloc(size));
	if (pImageCodecInfo == NULL)
		return FALSE;
	Gdiplus::GetImageEncoders(num, size, pImageCodecInfo);

	// Find for the support of format for image in the windows
	for (UINT i = 0; i < num; ++i)
	{
		//MimeType: Depiction for the program image  
//		if (wcscmp(pImageCodecInfo[i].MimeType, format) == 0)
		if (wcscmp(pImageCodecInfo[i].MimeType, cstringtowchar(format)) == 0)
		{
			*pCLSID = pImageCodecInfo[i].Clsid;
			free(pImageCodecInfo);
			return TRUE;
		}
	}
	free(pImageCodecInfo);
	return FALSE;
}

void CCommonFunc::GDISaveImage(BYTE *Buffer, CString ImagePath, IMAGEINFORMATION ImageInfo)
{
	DWORD Width, Length, BytesPerLine;
	WORD  Resolution;
	BYTE  TiffCompressionMode = EncoderValueCompressionNone;
	BYTE  JpgCompression = 50;

	if (!gdiplusToken)
	{
		GdiplusStartupInput gdiplusStartupInput;
		GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	}

	Width = ImageInfo.Width;
	Length = ImageInfo.Length;
	BytesPerLine = ImageInfo.BytesPerLine;
	Resolution = ImageInfo.Resolution;

	Gdiplus::PixelFormat PixelFormat;

	if (ImageInfo.PixelType == PT_RGB)
		PixelFormat = PixelFormat24bppRGB;
	else if (ImageInfo.PixelType == PT_GRAY)
		PixelFormat = PixelFormat8bppIndexed;
	else
		PixelFormat = PixelFormat1bppIndexed;

	Gdiplus::Bitmap     File(Width, Length, PixelFormat);
	Gdiplus::Rect       Rect(0, 0, Width, Length);
	Gdiplus::BitmapData Data;

	if (PixelFormat == PixelFormat8bppIndexed)
	{
		Gdiplus::ColorPalette Palette[256];
		Palette->Count = 256;
		for (int index = 0; index < 256; index++)
			Palette->Entries[index] = Gdiplus::Color::MakeARGB(255, index, index, index);
		File.SetPalette(Palette);  //设置调色板
	}
	if (PixelFormat == PixelFormat1bppIndexed)
	{
		Gdiplus::ColorPalette Palette[2];
		Palette->Count = 2;
		Palette->Entries[0] = Gdiplus::Color::MakeARGB(255, 0, 0, 0);
		Palette->Entries[1] = Gdiplus::Color::MakeARGB(255, 255, 255, 255);
		File.SetPalette(Palette);
	}

	File.LockBits(&Rect, Gdiplus::ImageLockModeRead | Gdiplus::ImageLockModeWrite, PixelFormat, &Data);
	BYTE *Scan0Data = (BYTE*)Data.Scan0;
	BYTE *ImageData = Buffer;
	for (DWORD i = 0; i< Length; i++)
	{
		memcpy(Scan0Data, ImageData, BytesPerLine);

		if (PixelFormat == PixelFormat24bppRGB)
		{
			BYTE Temp = 0;
			for (DWORD i = 0; i < BytesPerLine; i += 3)
			{
				Temp = Scan0Data[i];
				Scan0Data[i] = Scan0Data[i + 2];
				Scan0Data[i + 2] = Temp;
			}
		}

		Scan0Data += Data.Stride;
		ImageData += BytesPerLine;
	}
	File.UnlockBits(&Data);
	File.SetResolution((float)Resolution, (float)Resolution);  //设置分辨率

	CLSID   EncoderClsid;
	BYTE	m_FileFormatMode = 0;
	CString FILE_FileFormat = TEXT("image/");
	if (ImagePath.Find(TEXT(".bmp")) != -1)
	{
		m_FileFormatMode = 1;
		FILE_FileFormat += "bmp";
	}
	else if (ImagePath.Find(TEXT(".jpg")) != -1)
	{
		m_FileFormatMode = 2;
		FILE_FileFormat += "jpeg";
	}
	else if (ImagePath.Find(TEXT(".tif")) != -1)
	{
		m_FileFormatMode = 3;
		FILE_FileFormat += "tiff";
	}
	else if (ImagePath.Find(TEXT(".gif")) != -1)
	{
		m_FileFormatMode = 4;
		FILE_FileFormat += "gif";
	}
	GetImageCLSID(FILE_FileFormat, &EncoderClsid);

	Gdiplus::EncoderParameters		m_EncoderParameter;
	ULONG							m_ParameterValue = 0;
	m_EncoderParameter.Count = 1;
	switch (m_FileFormatMode)
	{
	case 1:	// BMP
		File.Save(cstringtowchar(ImagePath), &EncoderClsid);
		break;
	case 2:	// Jpeg, Compress: 50
		m_EncoderParameter.Parameter[0].Guid = Gdiplus::EncoderQuality;
		m_EncoderParameter.Parameter[0].Type = Gdiplus::EncoderParameterValueTypeLong;
		m_EncoderParameter.Parameter[0].NumberOfValues = 1;
		m_ParameterValue = JpgCompression;
		m_EncoderParameter.Parameter[0].Value = &m_ParameterValue;
		File.Save(cstringtowchar(ImagePath), &EncoderClsid, &m_EncoderParameter);
		break;
	case 3:	// Tiff, Compression mode
		if (TiffCompressionMode == EncoderValueCompressionCCITT4)
		{
			m_ParameterValue = Gdiplus::EncoderValueCompressionCCITT4;
			m_EncoderParameter.Parameter[0].Guid = Gdiplus::EncoderCompression;
			m_EncoderParameter.Parameter[0].Type = Gdiplus::EncoderParameterValueTypeLong;
		}
		else
		{
			if (TiffCompressionMode == EncoderValueCompressionNone)
				m_ParameterValue = Gdiplus::EncoderValueCompressionNone;
			else
				m_ParameterValue = Gdiplus::EncoderValueCompressionLZW;

			m_EncoderParameter.Parameter[0].Guid = Gdiplus::EncoderCompression;
			m_EncoderParameter.Parameter[0].Type = Gdiplus::EncoderParameterValueTypeLong;
		}

		m_EncoderParameter.Parameter[0].NumberOfValues = 1;
		m_EncoderParameter.Parameter[0].Value = &m_ParameterValue;
		File.Save(cstringtowchar(ImagePath), &EncoderClsid, &m_EncoderParameter);
		break;
	case 4:	// Gif
		File.Save(cstringtowchar(ImagePath), &EncoderClsid);
		break;
	}
}

BOOL CCommonFunc::GDIReadImage(CString ImagePath, BYTE **pDes, IMAGEINFORMATION* pImageInfo)
{
	DWORD Width, Length, BytesPerLine;
	WORD  Resolution;
	IMAGEINFORMATION ImageInfo = {0};
	DWORD ImageSize, BufferSize = 0;
	BYTE	*pBuffer = *pDes;
	CString strMsg;

	if (!gdiplusToken)
	{
		GdiplusStartupInput gdiplusStartupInput;
		GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	}

	Gdiplus::Bitmap		*pImage = NULL;
	pImage = Gdiplus::Bitmap::FromFile(cstringtowchar(ImagePath), FALSE);

	if(!pImage)
		return FALSE;

	Width = pImage->GetWidth();
	Length = pImage->GetHeight();
	Resolution = (WORD)pImage->GetHorizontalResolution();
	PixelFormat pixelFormat = pImage->GetPixelFormat();
	
	if(pBuffer)
		BufferSize = GlobalSize(GlobalHandle((BYTE*)pBuffer));
	
	BytesPerLine = Width;

	ImageInfo.Width = Width;
	ImageInfo.Length = Length;
	ImageInfo.Resolution = Resolution;
	ImageInfo.PixelType = PT_GRAY;

	if(pixelFormat == PixelFormat24bppRGB)
	{
		BytesPerLine*=3;
		ImageInfo.PixelType = PT_RGB;
	}
	if(pixelFormat == PixelFormat1bppIndexed)
	{
		BytesPerLine = (Width+7)/8;
		ImageInfo.PixelType = PT_BW;
	}

	ImageInfo.BytesPerLine = BytesPerLine;
	ImageSize = BytesPerLine * Length;
	if(ImageSize > BufferSize)
	{
		if(pBuffer)
		{
			GlobalUnlockPtr(pBuffer);
			GlobalFree(GlobalPtrHandle(pBuffer));
			pBuffer = NULL;
		}
		pBuffer = (BYTE *) GlobalAllocPtr(GHND, ImageSize);
	}

	Gdiplus::BitmapData bitmapData;
	Gdiplus::Rect rect(0, 0, Width, Length);

	*pDes = pBuffer;
	if(Gdiplus::Ok == pImage->LockBits(&rect, Gdiplus::ImageLockModeRead | Gdiplus::ImageLockModeWrite, pixelFormat, &bitmapData))
	{
		INT	  Stride = bitmapData.Stride;
		BYTE *pRaw = pBuffer;

		BYTE  *pLineBuffer = (BYTE*)bitmapData.Scan0;

		for(DWORD i = 0; i < Length; i++)
		{
			memcpy(pRaw, pLineBuffer, Stride);

			if(ImageInfo.PixelType == PT_RGB)
			{
				BYTE Move;
				BYTE *BytePtr = pRaw;

				for(int j = 0; j < (int)BytesPerLine/3; j++)
				{
					Move = *(BytePtr + 2);
					*(BytePtr + 2) = *BytePtr;
			   		*BytePtr = (BYTE)Move;
  					BytePtr += 3;
				}
			}
			pRaw+=BytesPerLine;
			pLineBuffer+=Stride;
		}
		*pImageInfo = ImageInfo;
		pImage->UnlockBits(&bitmapData); 
	}
	else
		return FALSE;

	if(pImage)
		delete pImage;
	
	return TRUE;
}

const wchar_t * CCommonFunc::cstringtowchar(CString strIn)
{
	/*
	string str;  
	str = CStringA(str1);  
	const char* conststr;  
	conststr = str.c_str();  
	//以上是将CString转换为const char* 

	size_t  len = strlen(conststr) + 1;  
	size_t converted = 0;  
	wchar_t *WStr;  
	WStr = (wchar_t*)malloc(len*sizeof(wchar_t));  
	mbstowcs_s(&converted, WStr, len, conststr, _TRUNCATE);  
	return WStr;  */

	//USES_CONVERSION;
	//return A2W((LPCTSTR)str);

	std::string str;
	str = CStringA(strIn);
	const char* str2;
	str2 = str.c_str();
	//以上是将CString转换为const char*
	size_t  len = strlen(str2) + 1;
	size_t converted = 0;
	wchar_t *WStr;
	WStr = (wchar_t*)malloc(len*sizeof(wchar_t));
	mbstowcs_s(&converted, WStr, len, str2, _TRUNCATE);
	//以上是将const char*转换为wchar_t*
	return WStr;
	



}

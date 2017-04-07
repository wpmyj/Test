#include "Page_Custom.h"
#include "stdafx.h"
#include "CommonDS.h"

CPage_Custom::CPage_Custom(void)
{
}


CPage_Custom::~CPage_Custom(void)
{
}

int CPage_Custom::FindPaperSize(int index)
{
	int nval;

	switch(index)
	{
	case 0:
		nval = TWSS_NONE;
		break;
	case 1:
		nval = TWSS_USLETTER;
		break;
	case 2:
		nval = TWSS_USLEGAL;
		break;
	case 3:
		nval = TWSS_A4;
		break;
	case 4:
		nval = TWSS_A5;
		break;
	case 5:
		nval = TWSS_A6;
		break;
	case 6:
		nval = TWSS_A7;
		break;
	case 7:
		nval = TWSS_ISOB5;
		break;
	case 8:
		nval = TWSS_ISOB6;
		break;
	case 9:
		nval = TWSS_ISOB7;
		break;
	case 10:
		nval = TWSS_JISB5;
		break;
	case 11:
		nval = TWSS_JISB6;
		break;
	case 12:
		nval = TWSS_JISB7;
		break;
	case 13:
		nval = UDSCAP_LONGDOCUMENT;
		break;
	case 14:
		nval = TWSS_MAXSIZE;
		break;
	default:
		nval = TWSS_A4;
	}
	return nval;
}

int CPage_Custom::FindUnit(int index)
{
	int nval;
	switch(index)
	{
	case 0:
		nval = TWUN_INCHES;
		break;
	case 1:
		nval = TWUN_PIXELS;
		break;
	case 2:
		nval = TWUN_CENTIMETERS;
		break;
	default:
		nval = TWUN_INCHES;
		break;
	}
	return nval;
}
// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�

#pragma once


#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // �� Windows ͷ���ų�����ʹ�õ�����
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // ĳЩ CString ���캯��������ʽ��

#include <afxwin.h>         // MFC ��������ͱ�׼���
#include <afxext.h>         // MFC ��չ

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE ��
#include <afxodlgs.h>       // MFC OLE �Ի�����
#include <afxdisp.h>        // MFC �Զ�����
#endif // _AFX_NO_OLE_SUPPORT

#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>                      // MFC ODBC ���ݿ���
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>                     // MFC DAO ���ݿ���
#endif // _AFX_NO_DAO_SUPPORT

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC �� Internet Explorer 4 �����ؼ���֧��
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>                     // MFC �� Windows �����ؼ���֧��
#endif // _AFX_NO_AFXCMN_SUPPORT
#include <afxcontrolbars.h>
#include <afxdlgs.h>
#include <afxdisp.h>

/**  �û������߳���Ϣ */    
#define   WM_THREADINFO WM_USER+50 
/** ���߳���Ϣ */    
#define   WM_MAINTHREAD WM_USER+51  

/** �û������߳���Ϣ���� */    
#define   INDICATORS_SHOWUI    0
#define   INDICATORS_PROGRESS  1
#define   INDICATORS_PAGEINFO  2
#define   INDICATORS_CANCEL    3
#define   INDICATORS_DESTROY   4
#define   INDICATORS_SPEED     5
#define   INDICATORS_DS        6

/** ɨ�����ҳ��Ϣ */    
struct PageInfo
{
	CString strPageCount;
	CString strPageSize;
	CString strTotalSize;
};
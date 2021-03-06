// UDS Scanner.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "UDS Scanner.h"

#include "src/dllmain.h"
#include "src/CDUDS_Video.h"
extern HINSTANCE g_hinstance;
extern HWND g_hwndDLG;
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: 如果此 DLL 相对于 MFC DLL 是动态链接的，
//		则从此 DLL 导出的任何调入
//		MFC 的函数必须将 AFX_MANAGE_STATE 宏添加到
//		该函数的最前面。
//
//		例如:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// 此处为普通函数体
//		}
//
//		此宏先于任何 MFC 调用
//		出现在每个函数中十分重要。这意味着
//		它必须作为函数中的第一个语句
//		出现，甚至先于所有对象变量声明，
//		这是因为它们的构造函数可能生成 MFC
//		DLL 调用。
//
//		有关其他详细信息，
//		请参阅 MFC 技术说明 33 和 58。
//

// CUDSScannerApp

BEGIN_MESSAGE_MAP(CUDSScannerApp, CWinApp)
END_MESSAGE_MAP()


// CUDSScannerApp 构造

CUDSScannerApp::CUDSScannerApp()
	//: m_gdiplusToken(0)
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
	
}


// 唯一的一个 CUDSScannerApp 对象

CUDSScannerApp theApp;


// CUDSScannerApp 初始化
//HINSTANCE g_hinstance;
BOOL CUDSScannerApp::InitInstance()
{
	//g_hinstance = (HINSTANCE)GetCurrentModule(); //获取DLL自身的句柄

	// GDI+,必须在CWinApp::InitInstance();之前
	//::MessageBox(g_hwndDLG,TEXT("InitInstance!"),MB_CAPTION,MB_OK);
	//GdiplusStartupInput gdiplusStartupInput;
	//GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);

	CWinApp::InitInstance();
	AfxEnableControlContainer();  // 调用ocx控件必须添加该函数
	//AfxInitRichEdit2(); // 调用RichEdit2控件必须添加该句该函数
	//::MessageBox(hwndDLG,"Init",MB_CAPTION,MB_OK);

	//该方法得到的是调用的exe的句柄
	//AFX_MODULE_STATE* state = AfxGetModuleState();
	//g_hinstance = state->m_hCurrentInstanceHandle;

	//获取DLL的绝对路径
	/*TCHAR lib_name[MAX_PATH]; 
	::GetModuleFileName( GetCurrentModule(), lib_name, MAX_PATH );
	::MessageBox(hwndDLG,lib_name,MB_CAPTION,MB_OK);*/


	return TRUE;
}


int CUDSScannerApp::ExitInstance()
{
	// TODO: 在此添加专用代码和/或调用基类
	unLoadDSMLib();
	g_hinstance = 0;
	//GdiplusShutdown(m_gdiplusToken);
	return CWinApp::ExitInstance();
}



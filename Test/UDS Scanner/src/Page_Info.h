#pragma once

#include "MFC_UI.h"
#include "afxwin.h"
#include "Page_Custom.h"
#include "afxcmn.h"
// CPage_Info 对话框

class MFC_UI;

class CPage_Info : public CPropertyPage, public CPage_Custom
{
	DECLARE_DYNAMIC(CPage_Info)

public:
	CPage_Info(MFC_UI *pUI);
	virtual ~CPage_Info();

// 对话框数据
	enum { IDD = IDD_PROPPAGE_INFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

private:
	/**
	* @brief 主界面
	* @see  MFC_UI::DisplayTWAINGUI()
	* @note 从外部接收值
	*/
	MFC_UI  *m_pUI;
	HMODULE m_hDLL;

protected:
	InitializeDriverProc               InitializeDriver;
	InitializeScannerProc              InitializeScanner;
	TerminateDriverProc                TerminateDriver;
	ReadNVMDataProc                    ReadNVMData;

private:
	virtual BOOL OnInitDialog();

//	afx_msg void OnLbnSelchangeInfo_List_Information();
	afx_msg void OnInfo_Btn_Report();
	afx_msg void OnInfo_Btn_ClearDicideNum(); //自定义按钮响应事件
	afx_msg void OnInfo_Btn_ClearFeedNum();

private:
	CButton m_btn_cleardicidenum; //分纸轮计数清零
	CButton m_btn_clearfeednum;  //进纸轮计数清零
	CListCtrl m_list_infomation;
//	CListBox m_list_infomation;

protected:
	void ShowComputerAndScannerInfo();  ///< 显示用户电脑配置信息与扫描仪硬件信息
	bool LoadDLL();
	
public:
	virtual BOOL OnSetActive();
};

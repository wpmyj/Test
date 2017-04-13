#pragma once
#include "stdafx.h"
//#include "afxwin.h"
//#include "afxcmn.h"
#include "resource.h"
#include "afxwin.h"

#include "SkinControls.h"
#pragma comment(lib, "UDS_Skin.lib")
// CDlg_Device 对话框

class CDlg_Device : public CDialogEx
{
	DECLARE_DYNAMIC(CDlg_Device)

public:
	CDlg_Device(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlg_Device();

// 对话框数据
	enum { IDD = IDD_DLG_DEVICE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnLvnItemchangedDeviceList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedDeviceCheck_Noui();
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();

protected:
	CListCtrl m_list_device;
	CSkinCheckBox m_check_noui;  // CSkinCheckBox 选择与取消选择对应的函数是SetChecked(), GetChecked()
	CSkinButton   m_btnOK;


public:
	/**
	*  @brief  设置对话框界面
	*/
	void SetDialogUI();   

	/**
	*  @brief  初始化List控件，包括显示可用设备，并显示当前选择设备
	*/
	void InitList();


};

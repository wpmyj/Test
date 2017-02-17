#pragma once
#include "afxwin.h"

/**
* @file   CDlg_Rename.h
* @brief  重命名对话框类
* @author UDS
* @par    Copyright (c) 2016 UDS.
*         All Rights Reserved.
* @date   2017/2/7 
*/ 

// CDlg_Rename 对话框

class CDlg_Rename : public CDialog
{
	DECLARE_DYNAMIC(CDlg_Rename)

public:
	CDlg_Rename(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlg_Rename();

// 对话框数据
	enum { IDD = IDD_DLG_RENAME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual void OnOK();

	DECLARE_MESSAGE_MAP()	

public:
	CEdit m_edit_newname;

public:
	CString GetNewName();  ///< 获取新名称
	BOOL m_bOk;		///< 判断是否点击确定按钮

private:
	CString m_strNewName;  ///< 当前新名称
};

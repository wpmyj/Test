#pragma once
#include "stdafx.h"
//#include "afxwin.h"
//#include "afxcmn.h"
#include "resource.h"
#include "afxwin.h"

#include "SkinControls.h"
#pragma comment(lib, "UDS_Skin.lib")
// CDlg_Device �Ի���

class CDlg_Device : public CDialogEx
{
	DECLARE_DYNAMIC(CDlg_Device)

public:
	CDlg_Device(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlg_Device();

// �Ի�������
	enum { IDD = IDD_DLG_DEVICE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
	CSkinCheckBox m_check_noui;  // CSkinCheckBox ѡ����ȡ��ѡ���Ӧ�ĺ�����SetChecked(), GetChecked()
	CSkinButton   m_btnOK;


public:
	/**
	*  @brief  ���öԻ������
	*/
	void SetDialogUI();   

	/**
	*  @brief  ��ʼ��List�ؼ���������ʾ�����豸������ʾ��ǰѡ���豸
	*/
	void InitList();


};

#pragma once

#include "MFC_UI.h"
#include "afxwin.h"
#include "Page_Custom.h"
#include "afxcmn.h"
// CPage_Info �Ի���

class MFC_UI;

class CPage_Info : public CPropertyPage, public CPage_Custom
{
	DECLARE_DYNAMIC(CPage_Info)

public:
	CPage_Info(MFC_UI *pUI);
	virtual ~CPage_Info();

// �Ի�������
	enum { IDD = IDD_PROPPAGE_INFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

private:
	/**
	* @brief ������
	* @see  MFC_UI::DisplayTWAINGUI()
	* @note ���ⲿ����ֵ
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
	afx_msg void OnInfo_Btn_ClearDicideNum(); //�Զ��尴ť��Ӧ�¼�
	afx_msg void OnInfo_Btn_ClearFeedNum();

private:
	CButton m_btn_cleardicidenum; //��ֽ�ּ�������
	CButton m_btn_clearfeednum;  //��ֽ�ּ�������
	CListCtrl m_list_infomation;
//	CListBox m_list_infomation;

protected:
	void ShowComputerAndScannerInfo();  ///< ��ʾ�û�����������Ϣ��ɨ����Ӳ����Ϣ
	bool LoadDLL();
	
public:
	virtual BOOL OnSetActive();
};

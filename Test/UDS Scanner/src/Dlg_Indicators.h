#pragma once
#include "stdafx.h"
#include "resource.h"
#include "afxcmn.h"
#include "afxwin.h"
#include "CTWAINDS_UDS.h"
#include "SkinControls.h" // �Ի�ؼ��� UDS_Skin.dll
//#include "afxwin.h"

// CDlg_Indicators �Ի���

class CDlg_Indicators : public CDialogEx
{
	DECLARE_DYNAMIC(CDlg_Indicators)

public:
	CDlg_Indicators(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlg_Indicators();

// �Ի�������
	enum { IDD = IDD_DLG__INDICATORS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

protected:
	DWORD    m_dwElaspedTime;       /**< ����ʱ�䣨��λ�룩 */
	CSkinProgressCtrl m_Progress;       /**< ��������� */
	CSkinStatic m_static_progress;      /**< �ٷֱȣ�%�� */
	CSkinStatic m_static_elapsedtime;   /**< ����ʱ�� */
	CSkinStatic m_static_pagecount;     /**< ɨ��ҳ�� */
	CSkinStatic m_static_pagesize;      /**< ͼ���С */
	CSkinStatic m_static_totalsize;     /**< �ܹ���С */
	CSkinStatic m_static_speed;         /**< �����ٶ� */
	DWORD   m_dwSpeed;              /**< �ٶ�ֵ */
	CSkinButton m_button_close;
public:
	CTWAINDS_UDS *m_pDS;

public:
	/**
	*  @brief  ���¿ؼ���ʾ����
	*  @param[in]  _strPageSize ���մ�С����ǰҳ��
	*  @param[in]  _strTotalSize �ܹ���С
	*  @param[in]  _strElapsedTime ����ʱ��
	*  @param[in]  _strPageCount ɨ��ҳ��
	*  @retval true ��ʾ�ɹ�
	*  @retval false ��ʾʧ��  
	*/
	bool UpdatePageInfo(CString& _strPageCount, CString& _strPageSize, CString& _strTotalSize);

	/**
	*  @brief ���½�������ٷֱ���ʾ 
	*  @param[in]  _nPos ������λ�� 
	*/
	void UpdateProgress(int _nPos);

	/**
	*  @brief  ������ʱ��
	*/
	void StartTimer();

	/**
	*  @brief  ���ٶ�ʱ��
	*/
	void EndTimer();

	/**
	*  @brief  ���´����ٶ�
	*/
	void UpdateSpeed(const DWORD _dwSpeed);

	/**
	*  @brief  ��ֵ����
	*/
	void SetSpeed(const DWORD _dwSpeed);

	/**
	*  @brief �ؼ���ʼ��
	*/
	void InitControls();

	virtual BOOL OnInitDialog();
	virtual void PostNcDestroy();

	afx_msg void OnBnClickedOk();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnCancel();
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);	
	afx_msg LRESULT OnNcHitTest(CPoint point);
};

#pragma once
#include "stdafx.h"
#include "resource.h"
#include "afxcmn.h"
#include "afxwin.h"
#include "CTWAINDS_UDS.h"
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
	CProgressCtrl m_Progress;       /**< ��������� */
	CStatic m_static_progress;      /**< �ٷֱȣ�%�� */
	CStatic m_static_elapsedtime;   /**< ����ʱ�� */
	CStatic m_static_pagecount;     /**< ɨ��ҳ�� */
	CStatic m_static_pagesize;      /**< ͼ���С */
	CStatic m_static_totalsize;     /**< �ܹ���С */
	CStatic m_static_speed;         /**< �����ٶ� */
	DWORD   m_dwSpeed;              /**< �ٶ�ֵ */

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

	virtual BOOL OnInitDialog();
	virtual void PostNcDestroy();

	afx_msg void OnBnClickedOk();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnCancel();


};

#pragma once
#include "stdafx.h"
#include "resource.h"
#include "afxcmn.h"
#include "afxwin.h"
#include "CTWAINDS_UDS.h"
#include "SkinControls.h" // 自绘控件库 UDS_Skin.dll
//#include "afxwin.h"

// CDlg_Indicators 对话框

class CDlg_Indicators : public CDialogEx
{
	DECLARE_DYNAMIC(CDlg_Indicators)

public:
	CDlg_Indicators(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlg_Indicators();

// 对话框数据
	enum { IDD = IDD_DLG__INDICATORS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

protected:
	DWORD    m_dwElaspedTime;       /**< 经过时间（单位秒） */
	CSkinProgressCtrl m_Progress;       /**< 传输进度条 */
	CSkinStatic m_static_progress;      /**< 百分比（%） */
	CSkinStatic m_static_elapsedtime;   /**< 经过时间 */
	CSkinStatic m_static_pagecount;     /**< 扫描页数 */
	CSkinStatic m_static_pagesize;      /**< 图像大小 */
	CSkinStatic m_static_totalsize;     /**< 总共大小 */
	CSkinStatic m_static_speed;         /**< 传输速度 */
	DWORD   m_dwSpeed;              /**< 速度值 */
	CSkinButton m_button_close;
public:
	CTWAINDS_UDS *m_pDS;

public:
	/**
	*  @brief  更新控件显示内容
	*  @param[in]  _strPageSize 接收大小（当前页）
	*  @param[in]  _strTotalSize 总共大小
	*  @param[in]  _strElapsedTime 经过时间
	*  @param[in]  _strPageCount 扫描页数
	*  @retval true 表示成功
	*  @retval false 表示失败  
	*/
	bool UpdatePageInfo(CString& _strPageCount, CString& _strPageSize, CString& _strTotalSize);

	/**
	*  @brief 更新进度条与百分比显示 
	*  @param[in]  _nPos 进度条位置 
	*/
	void UpdateProgress(int _nPos);

	/**
	*  @brief  启动定时器
	*/
	void StartTimer();

	/**
	*  @brief  销毁定时器
	*/
	void EndTimer();

	/**
	*  @brief  更新传输速度
	*/
	void UpdateSpeed(const DWORD _dwSpeed);

	/**
	*  @brief  赋值函数
	*/
	void SetSpeed(const DWORD _dwSpeed);

	/**
	*  @brief 控件初始化
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

#pragma once

#include "MFC_UI.h"
#include "afxwin.h"
#include "Page_Custom.h"

class MFC_UI;

// CPage_Set 对话框

class CPage_Set : public CPropertyPage, public CPage_Custom
{
	DECLARE_DYNAMIC(CPage_Set)

public:
	CPage_Set(MFC_UI *pUI);
	virtual ~CPage_Set();

// 对话框数据
	enum { IDD = IDD_PROPPAGE_SET };

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

public:
	void UpdateControls(void);  ///< 更新控件状态
	void InitSliderCtrl();  ///< 初始化滑动条控件
	void SetCapValue(void); ///<设置参数
	void SetSavePower(void); ///<设置节点模式滑动条是否可用
	void SetOffTime(void); ///<设置关机时间滑动条是否可用

private:
	MAP_CAP m_setmap;  ///<用于保存参数改变后的值


private:
	afx_msg void OnSet_Btn_Check_SavePower();
	afx_msg void OnSet_Btn_Check_OffTime();
	afx_msg void OnNMCustomdrawSet_Slider_SavePower(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSet_Slider_Offtime(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL OnInitDialog();

private:
	CButton m_check_savepower;
	CButton m_check_offtime;
	CSliderCtrl m_slider_offtime;
	CSliderCtrl m_slider_savepower;

};

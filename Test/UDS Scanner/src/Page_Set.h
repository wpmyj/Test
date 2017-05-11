#pragma once

#include "MFC_UI.h"
#include "afxwin.h"
#include "Page_Custom.h"
#include "GL1.h"

class MFC_UI;

// CPage_Set 

class CPage_Set : public CPropertyPage, public CPage_Custom
{
	DECLARE_DYNAMIC(CPage_Set)

public:
	CPage_Set(MFC_UI *pUI);
	virtual ~CPage_Set();

	enum { IDD = IDD_PROPPAGE_SET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);     // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

private:
	/**
	* @brief 
	* @see  MFC_UI::DisplayTWAINGUI()
	* @note 
	*/
	MFC_UI  *m_pUI;
	HMODULE m_hDLL;
public:
	void UpdateControls(void);  ///< 更新控件状态
	void InitSliderCtrl();  ///< 初始化滑动条
	void SetCapValue(void); ///<设置参数
	void SetSavePower(void); ///<设置节电模式滑动条是否可用
	void SetOffTime(void); ///<设置关机时间滑动条是否可用
	void InitSetmap(void); ///<初始化map

private:
	MAP_CAP m_setmap;  ///<用于保存参数改变后的值

private:
	afx_msg void OnSet_Btn_Check_SavePower();
	afx_msg void OnSet_Btn_Check_OffTime();
	afx_msg void OnNMCustomdrawSet_Slider_SavePower(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSet_Slider_Offtime(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSet_Btn_Check_TurnVideo();
	virtual BOOL OnInitDialog();
	virtual BOOL OnSetActive();

	afx_msg void OnSet_Btn_Check_Showschedule();
	afx_msg void OnSet_Btn_Check_Emergency();

private:
	CButton m_check_savepower;
	CButton m_check_offtime;
	CSliderCtrl m_slider_offtime;
	CSliderCtrl m_slider_savepower;
	CButton m_check_turnvideo;
	CButton m_check_indicator;
	CButton m_check_enmergency;

private:
	bool LoadDLL();   ///< 加载DLL

	InitializeDriverProc               InitializeDriver;
	InitializeScannerProc              InitializeScanner;
	TerminateDriverProc                TerminateDriver;
	ReadNVMDataProc                    ReadNVMData;
	SetNVRAMValueProc                  SetNVRAMValue;  // 

};

#pragma once

#include "MFC_UI.h"
#include "afxwin.h"
#include "Page_Custom.h"
#include "GL1.h"

class MFC_UI;

// CPage_Set ¶Ô»°¿ò

class CPage_Set : public CPropertyPage, public CPage_Custom
{
	DECLARE_DYNAMIC(CPage_Set)

public:
	CPage_Set(MFC_UI *pUI);
	virtual ~CPage_Set();

// ¶Ô»°¿òÊý¾Ý
	enum { IDD = IDD_PROPPAGE_SET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV Ö§³Ö

	DECLARE_MESSAGE_MAP()

private:
	/**
	* @brief Ö÷½çÃæ
	* @see  MFC_UI::DisplayTWAINGUI()
	* @note ´ÓÍâ²¿½ÓÊÕÖµ
	*/
	MFC_UI  *m_pUI;
	HMODULE m_hDLL;
public:
	void UpdateControls(void);  ///< ¸üÐÂ¿Ø¼þ×´Ì¬
	void InitSliderCtrl();  ///< ³õÊ¼»¯»¬¶¯Ìõ¿Ø¼þ
	void SetCapValue(void); ///<ÉèÖÃ²ÎÊý
	void SetSavePower(void); ///<ÉèÖÃ½ÚµãÄ£Ê½»¬¶¯ÌõÊÇ·ñ¿ÉÓÃ
	void SetOffTime(void); ///<ÉèÖÃ¹Ø»úÊ±¼ä»¬¶¯ÌõÊÇ·ñ¿ÉÓÃ
	
private:
	MAP_CAP m_setmap;  ///<ÓÃÓÚ±£´æ²ÎÊý¸Ä±äºóµÄÖµ


private:
	afx_msg void OnSet_Btn_Check_SavePower();
	afx_msg void OnSet_Btn_Check_OffTime();
	afx_msg void OnNMCustomdrawSet_Slider_SavePower(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSet_Slider_Offtime(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSet_Btn_Check_TurnVideo();
	virtual BOOL OnInitDialog();
	virtual BOOL OnSetActive();
private:
	CButton m_check_savepower;
	CButton m_check_offtime;
	CSliderCtrl m_slider_offtime;
	CSliderCtrl m_slider_savepower;
	CButton m_check_turnvideo;
		
private:
	bool LoadDLL();   ///< ¼ÓÔØG6400»òG6600¶ÔÓ¦µÄdll

	InitializeDriverProc               InitializeDriver;
	InitializeScannerProc              InitializeScanner;
	TerminateDriverProc                TerminateDriver;
	ReadNVMDataProc                    ReadNVMData;
	SetNVRAMValueProc                  SetNVRAMValue;  // ÉèÖÃ´ý»úÊ±¼ä¡¢¹Ø»úÊ±¼ä

public:
	CButton m_check_indicator;
	afx_msg void OnSet_Btn_Check_Showschedule();
};

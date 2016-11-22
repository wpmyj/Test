#pragma once

#include "MFC_UI.h"

class MFC_UI;
// CPage_Paper 对话框

class CPage_Paper : public CPropertyPage
{
	DECLARE_DYNAMIC(CPage_Paper)

public:
	CPage_Paper(MFC_UI *pUI);
	virtual ~CPage_Paper();

// 对话框数据
	enum { IDD = IDD_PROPPAGE_PAPER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

private:
	/**
	* @brief 在构造函数中初始化 
	* @see  CPage_Base(MFC_UI *pUI)
	*/
	MFC_UI  *m_pUI; 

private:
	void UpdateControls(void);  ///< 更新控件状态
	void SetScroll(void); ///< 设置滚动条与编辑框的关联

private:
	CComboBox m_combo_orientation;
	CComboBox m_combo_standardsizes;
	CComboBox m_combo_uints;
	CEdit m_edit_width;
	CEdit m_edit_height;
	CEdit m_edit_down;
	CEdit m_edit_left;
	CEdit m_edit_right;
	CEdit m_edit_up;
	CEdit m_edit_xpos;
	CEdit m_edit_ypos;

	CScrollBar m_scroll_xpos;
	CScrollBar m_scroll_ypos;
	CScrollBar m_scroll_width;
	CScrollBar m_scroll_up;
	CScrollBar m_scroll_right;
	CScrollBar m_scroll_left;
	CScrollBar m_scroll_height;
	CScrollBar m_scroll_down;

private:
	virtual BOOL OnInitDialog();

private:
	afx_msg void OnCbnSelchangePaper_Combo_Orientation();
	afx_msg void OnCbnSelchangePaper_Combo_Standardsizes();
	afx_msg void OnCbnSelchangePaper_Combo_Uints();
	
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};

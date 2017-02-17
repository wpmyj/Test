#pragma once

#include "MFC_UI.h"
#include "Page_Custom.h"

class MFC_UI;
// CPage_Paper 对话框

class CPage_Paper : public CPropertyPage, public CPage_Custom
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
	void SetCapValue(void);  ///<设置参数
	void UpdateControls(void);  ///< 更新控件状态
	int FindPaperSize(int index); ///<寻找index对应的纸张大小,返回index对应的纸张大小
	int FindUnit(int index); ///<寻找index对应的单位,返回index对应的单位
	void SetScroll(void); ///<根据edit的值更新滚动条的值;以及根据单位设定宽、高滚动条的范围

private:
	MAP_CAP m_papermap;  ///<用于保存参数改变后的值

	CComboBox m_combo_standardsizes; ///<纸张大小: US Letter/US Legal/A4/A5
	CComboBox m_combo_uints; ///<单位：英尺，像素，毫米

	CEdit m_edit_width;
	CEdit m_edit_height;
	CScrollBar m_scroll_width;
	CScrollBar m_scroll_height;

	CEdit m_edit_xpos;
	CEdit m_edit_ypos;
	CScrollBar m_scroll_xpos;
	CScrollBar m_scroll_ypos;

	CEdit m_edit_up;
	CEdit m_edit_down;
	CEdit m_edit_left;
	CEdit m_edit_right;
	CScrollBar m_scroll_up;
	CScrollBar m_scroll_down;
	CScrollBar m_scroll_right;
	CScrollBar m_scroll_left;

private:
	virtual BOOL OnInitDialog();
	virtual BOOL OnSetActive();
	afx_msg void OnPaint();

private:
	afx_msg void OnCbnSelchangePaper_Combo_Standardsizes();
	afx_msg void OnCbnSelchangePaper_Combo_Uints();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	
};

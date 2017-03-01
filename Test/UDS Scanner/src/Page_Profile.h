#pragma once

#include "MFC_UI.h"
#include "Page_Custom.h"
#include "afxwin.h"

class MFC_UI;
// CPage_Profile 对话框

class CPage_Profile : public CPropertyPage, public CPage_Custom
{
	DECLARE_DYNAMIC(CPage_Profile)

public:
	CPage_Profile(MFC_UI *pUI);
	virtual ~CPage_Profile();

// 对话框数据
	enum { IDD = IDD_PROPPAGE_PROFILE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnSetActive();
	DECLARE_MESSAGE_MAP()

private:
	/**
	* @brief 在构造函数中初始化 
	* @see  CPage_Base(MFC_UI *pUI)
	*/
	MFC_UI  *m_pUI; 

private:
	virtual void OnOK();
	virtual void OnCancel();

public:
	virtual BOOL OnInitDialog();

	afx_msg void OnProfile_Btn_New();
	afx_msg void OnProfile_Btn_Reset();
	afx_msg void OnProfile_Btn_Delete();
	afx_msg void OnProfile_Btn_Rename();
	afx_msg void OnProfile_Btn_Import();
	afx_msg void OnProfile_Btn_Export();

	afx_msg void OnLbnSelchangeProfile_List_Template();

public:
	/**
	* @brief 加载模板，将模板名显示在listbox中
	*/	
	void LoadTemplate(void);

	void InitTemplate();  ///< 初始化模版listbox
	void NewTemplate(); ///<新建常用模板
	void SetLastTemplate(void); ///<往listbox插入模板，并设置存在“上次使用模板”的情况
	void SetDelete(void); ///<设置删除按钮是否可用
	void SetRename(void); ///<设置重命名按钮是否可用

	/**
	*  @brief  新建指定参数模板
	*  @param[in]  profilename 模板名
	*  @param[in]  pixeltype 图像类型 
	*  @param[in]  duplexenabled 单/双面 
	*  @param[in]  resolution 分辨率(默认200dpi)  
	*  @retval true 表示成功
	*  @retval false 表示失败  
	*/
	bool CreateNewTemplate(std::string profilename, int pixeltype, 
		int duplexenabled, int resolution = 200);

public:	
	CListBox m_list_template;
	
public:
	/** 父类指针*/
	CPage_Custom* m_pBasePage;
	CPage_Custom* m_pAdPage;
	CPage_Custom* m_pPaperPage;
};

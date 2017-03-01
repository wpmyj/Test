#pragma once

#include "MFC_UI.h"
#include "Page_Custom.h"
#include "afxwin.h"

class MFC_UI;
// CPage_Profile �Ի���

class CPage_Profile : public CPropertyPage, public CPage_Custom
{
	DECLARE_DYNAMIC(CPage_Profile)

public:
	CPage_Profile(MFC_UI *pUI);
	virtual ~CPage_Profile();

// �Ի�������
	enum { IDD = IDD_PROPPAGE_PROFILE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnSetActive();
	DECLARE_MESSAGE_MAP()

private:
	/**
	* @brief �ڹ��캯���г�ʼ�� 
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
	* @brief ����ģ�壬��ģ������ʾ��listbox��
	*/	
	void LoadTemplate(void);

	void InitTemplate();  ///< ��ʼ��ģ��listbox
	void NewTemplate(); ///<�½�����ģ��
	void SetLastTemplate(void); ///<��listbox����ģ�壬�����ô��ڡ��ϴ�ʹ��ģ�塱�����
	void SetDelete(void); ///<����ɾ����ť�Ƿ����
	void SetRename(void); ///<������������ť�Ƿ����

	/**
	*  @brief  �½�ָ������ģ��
	*  @param[in]  profilename ģ����
	*  @param[in]  pixeltype ͼ������ 
	*  @param[in]  duplexenabled ��/˫�� 
	*  @param[in]  resolution �ֱ���(Ĭ��200dpi)  
	*  @retval true ��ʾ�ɹ�
	*  @retval false ��ʾʧ��  
	*/
	bool CreateNewTemplate(std::string profilename, int pixeltype, 
		int duplexenabled, int resolution = 200);

public:	
	CListBox m_list_template;
	
public:
	/** ����ָ��*/
	CPage_Custom* m_pBasePage;
	CPage_Custom* m_pAdPage;
	CPage_Custom* m_pPaperPage;
};

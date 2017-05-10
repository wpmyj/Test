#pragma once

#include "Page_Custom.h"
#include "MFC_UI.h"
#include "ximage.h"

class MFC_UI;

// CBase_Tab_Spiltshow �Ի���

class CBase_Tab_Spiltshow : public CDialogEx, public CPage_Custom
{
	DECLARE_DYNAMIC(CBase_Tab_Spiltshow)

public:
	CBase_Tab_Spiltshow(MFC_UI *pUI);   // ��׼���캯��
	virtual ~CBase_Tab_Spiltshow();

// �Ի�������
	enum { IDD = IDD_BASETAB_SPILTSHOW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

private:
	/**
	* @brief �ڹ��캯���г�ʼ�� 
	* @see  CPage_Base(MFC_UI *pUI)
	*/
	MFC_UI  *m_pUI; 
public:
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();

	CStatic m_spiltpicture;

public:
	CxImage m_image;
	void DrawImgOnCtrl(CxImage pImg, CWnd* wndDraw); //��picture control�ؼ��и���picture control�ؼ��Ĵ�С��̬��ʾͼƬ
};
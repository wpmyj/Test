#pragma once

#include "Page_Custom.h"
#include "MFC_UI.h"
#include "ximage.h" 

class MFC_UI;
// CBase_Tab_Rotateshow �Ի���

class CBase_Tab_Rotateshow : public CDialogEx, public CPage_Custom
{
	DECLARE_DYNAMIC(CBase_Tab_Rotateshow)

public:
	CBase_Tab_Rotateshow(MFC_UI *pUI);   // ��׼���캯��
	virtual ~CBase_Tab_Rotateshow();

// �Ի�������
	enum { IDD = IDD_BASETAB_ROTATESHOW };

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
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();

	CStatic m_picture;

public:
	CxImage m_image;
	void DrawImgOnCtrl(CxImage pImg, CWnd* wndDraw); //��picture control�ؼ��и���picture control�ؼ��Ĵ�С��̬��ʾͼƬ

};

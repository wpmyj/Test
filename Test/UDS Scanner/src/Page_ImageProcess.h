#pragma once

#include "MFC_UI.h"

class MFC_UI;
// CPage_ImageProcess �Ի���

class CPage_ImageProcess : public CPropertyPage
{
	DECLARE_DYNAMIC(CPage_ImageProcess)

public:
	CPage_ImageProcess(MFC_UI *pUI);
	virtual ~CPage_ImageProcess();

// �Ի�������
	enum { IDD = IDD_PROPPAGE_IMAGEPROCESS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

private:
	/**
	* @brief �ڹ��캯���г�ʼ�� 
	* @see  CPage_Base(MFC_UI *pUI)
	*/
	MFC_UI  *m_pUI;  
};

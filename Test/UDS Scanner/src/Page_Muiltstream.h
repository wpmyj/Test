#pragma once
#include "MFC_UI.h"

class MFC_UI;

// CPage_Muiltstream �Ի���

class CPage_Muiltstream : public CPropertyPage
{
	DECLARE_DYNAMIC(CPage_Muiltstream)

public:
	CPage_Muiltstream(MFC_UI *pUI);
	virtual ~CPage_Muiltstream();

// �Ի�������
	enum { IDD = IDD_PROPPAGE_MUILSTREAM };

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

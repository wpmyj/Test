#pragma once
#include "afxwin.h"

class CUDS_Edit : public CEdit
{
public:
	CUDS_Edit(void);
	~CUDS_Edit(void);

protected:
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	DECLARE_MESSAGE_MAP()
};


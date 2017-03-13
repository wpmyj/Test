#if !defined(AFX_PNGBUTTON_H__F33C59EA_E493_49CD_96C0_9D37FDD09749__INCLUDED_)
#define AFX_PNGBUTTON_H__F33C59EA_E493_49CD_96C0_9D37FDD09749__INCLUDED_

#include "ButtonPublic.h"

#if _MSC_VER > 1000
#pragma once
#endif 

#include <gdiplus.h>  
using namespace Gdiplus;
#pragma comment(lib, "gdiplus.lib")  


#define ORIEN_LEFT 0 // 从左到右排列
#define ORIEN_UP 1  // 从上到下排列

class CPngButton : public CButton
{
public:
	CPngButton();
	virtual ~CPngButton();

public:
	void Init(UINT nImg, int nPartNum, int  orien = ORIEN_LEFT, UINT nBtnType=BTN_TYPE_NORMAL);
	bool ShowImage(CDC* pDC, Image* pImage, UINT nState);
	Image *ImageFromResource(HINSTANCE hInstance,UINT uImgID,LPCTSTR lpType);
	void PaintParent();
	void SetText(CString str);
	CFont *m_pFont;
	CString strCaption;
	LRESULT OnGetCheck(WPARAM, LPARAM);
	LRESULT OnSetCheck(WPARAM, LPARAM);
protected:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseHOver(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnMouseLeave(WPARAM wParam,LPARAM lParam);
	afx_msg void OnPaint();

	DECLARE_MESSAGE_MAP()

protected:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void PreSubclassWindow();
	BOOL m_bChecked;
public:
	bool m_bTracked;
	CString  m_strText;
private:
	int m_nImgPart;

	Image* m_pImage;

	UINT m_nState;
	UINT m_nBtnType;
	UINT m_nOrien;
	BOOL m_bMenuOn;		

	ULONG_PTR m_gdiplusToken;
};

#endif 

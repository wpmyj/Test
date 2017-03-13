#ifndef CHECKBOXEX_H_
#define CHECKBOXEX_H_

#pragma once
#include "atltypes.h"

#include "gdiplus.h" 
using namespace Gdiplus; 
#pragma comment(lib, "gdiplus.lib")

// CCheckBoxEx

class CCheckBoxEx : public CButton
{
	DECLARE_DYNAMIC(CCheckBoxEx)

public:
	char szTitle[MAX_PATH];
	UINT m_frameCount;
	Image *m_pImage;
	Image* m_pTemp;
	CSize m_sizeImage;
	UINT m_buttonState;
	/*enum {
		bs_ncheck_leave = 1,
		bs_ncheck_move,
		bs_ncheck_down,
		bs_check_leave,
		bs_check_move,
		bs_check_down,
	};*/

	enum 
	{
		bsNormal = 1,
		bsHover,
		bsDown,		
		bsHoverDown,
	};
	enum
	{
		BTN_IMG_1 = 1,					
		BTN_IMG_3 = 3,					
		BTN_IMG_4 = 4,					
		BTN_IMG_5 = 5,
		BTN_IMG_6 = 6,
	};
	BOOL m_bCheck;

	CCheckBoxEx();
	virtual ~CCheckBoxEx();

	BOOL GetCheck();
	void SetCheck(BOOL bCheck, BOOL bRepaint = TRUE);

protected:
	DECLARE_MESSAGE_MAP()
	virtual void PreSubclassWindow();
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	CRect m_rc;
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	void DrawContent(CDC& MemDC);
	void BitmapToImage(CBitmap *pBitmap, Image* &img);
	void ImageFromID(int IDB_ID, Image* &_outImg);
	bool ShowImage(CDC* pDC, Image* pImage, UINT nState);
	void Init(UINT nImg, int nPartNum/*, UINT nBtnType*/);
//	void SetBitmap(UINT nResourceID = IDB_PNG_CHECKBOX, UINT nFrameCount = 6);
	void PaintParent();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
protected:
	afx_msg LRESULT OnBmGetcheck(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnBmSetcheck(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnBnClicked();

private:
	UINT m_nBtnType;
	int m_nImgPart;
public:
	afx_msg void OnPaint();
};




#endif


#include "stdafx.h"
//#include "QQRobot.h"
#include "CheckBoxEx.h"


// CCheckBoxEx

IMPLEMENT_DYNAMIC(CCheckBoxEx, CButton)

CCheckBoxEx::CCheckBoxEx()
: m_pTemp(NULL)
{
	//m_buttonState = bs_ncheck_leave;
	m_buttonState = bsNormal;
	m_pImage = NULL;
}

CCheckBoxEx::~CCheckBoxEx()
{
	
	if (m_pImage == NULL)
	{
		delete m_pImage;
		m_pImage = NULL;
	}

}


BEGIN_MESSAGE_MAP(CCheckBoxEx, CButton)
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSELEAVE()
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(BM_GETCHECK, &CCheckBoxEx::OnBmGetcheck)
	ON_MESSAGE(BM_SETCHECK, &CCheckBoxEx::OnBmSetcheck)
	ON_CONTROL_REFLECT(BN_CLICKED, &CCheckBoxEx::OnBnClicked)
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CCheckBoxEx 消息处理程序




void CCheckBoxEx::PreSubclassWindow()
{
	// TODO: 在此添加专用代码和/或调用基类
	SetButtonStyle(GetButtonStyle() | BS_OWNERDRAW);//修改为自绘模式
	GetClientRect(&m_rc);
	GetWindowTextA(/*m_hWnd,*/szTitle, sizeof(szTitle));//复制内容
	//SetBitmap()
	/*CDC *dc;
	dc = GetDC();
	ShowImage(dc, m_pImage, m_buttonState);*/

	CButton::PreSubclassWindow();
}


void CCheckBoxEx::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO:  添加您的代码以绘制指定项
	//CDC *pDc = GetDC();
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CButton::OnPaint()
	//CRect rcClient;
	//GetClientRect(&rcClient);
	//CDC MemDC;
	//MemDC.CreateCompatibleDC(pDc);
	//MemDC.SetBkMode(TRANSPARENT);//不设置透明文字会有背景
	//CBitmap memBmp;
	//memBmp.CreateCompatibleBitmap(pDc, rcClient.Width(), rcClient.Height());
	//CBitmap *pOldmap = MemDC.SelectObject(&memBmp);

	////先填充一下原先的背景
	////MemDC.BitBlt(rcClient.left, rcClient.top, rcClient.Width(), rcClient.Height(), pDc, 0, 0, SRCCOPY);
	////绘制内容
	//DrawContent(MemDC);
	////复制到原来的CDC
	//pDc->BitBlt(0, 0, rcClient.Width(), rcClient.Height(), &MemDC, 0, 0, SRCCOPY);

	////MemDC.SelectObject(pOldmap);
	////旧图应该是不存在的，直接删除
	//pOldmap->DeleteObject();
	//MemDC.DeleteDC();
	//memBmp.DeleteObject();
	//ReleaseDC(pDc);

	CDC dc;
	dc.Attach(lpDrawItemStruct->hDC);

	ShowImage(&dc, m_pImage, m_buttonState);
//	Invalidate();

}


BOOL CCheckBoxEx::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//CDC MemDC;
	//CBitmap memBmp;
	//MemDC.CreateCompatibleDC(pDC);
	//memBmp.CreateCompatibleBitmap(pDC, m_rc.Width(), m_rc.Height());
	//CBitmap *pOldmap = MemDC.SelectObject(&memBmp);
	////先填充一下原先的背景
	//MemDC.BitBlt(m_rc.left, m_rc.top, m_rc.Width(), m_rc.Height(), pDC, 0, 0, SRCCOPY);
	////绘制内容
	//if (m_pTemp == NULL)
	//	BitmapToImage(&memBmp, m_pTemp);
	////旧图应该是黑的无用，直接删除
	//pOldmap->DeleteObject();
	//MemDC.DeleteDC();
	//memBmp.DeleteObject();
	//ReleaseDC(&MemDC);

	ShowImage(pDC, m_pImage, m_buttonState);
//	Invalidate();

	return TRUE;
}

void CCheckBoxEx::DrawContent(CDC& /*MemDC*/)
{
	//Graphics g(MemDC);
	//g.DrawImage(m_pTemp, 0, 0);
	//if ((UINT)m_buttonState < m_frameCount)
	//	g.DrawImage(m_pImage, RectF((REAL)m_rc.left, (REAL)m_rc.top, (REAL)m_sizeImage.cx, (REAL)m_sizeImage.cy),
	//	m_buttonState * (REAL)m_sizeImage.cx, 0, (REAL)m_sizeImage.cx, (REAL)m_sizeImage.cy, UnitPixel);

	//COLORREF clrOld = MemDC.SetTextColor(RGB(0, 28, 48));
	//CFont *pOldFont = MemDC.SelectObject(GetFont());
	//CRect temp_rc = m_rc;
	//temp_rc.left += 6;
	//temp_rc.top -= 2;
	//MemDC.DrawText((CString)szTitle, &temp_rc, DT_VCENTER | DT_CENTER | DT_SINGLELINE | DT_WORD_ELLIPSIS);
	//MemDC.SelectObject(pOldFont);
	//MemDC.SetTextColor(clrOld);
}


//void CCheckBoxEx::SetBitmap(UINT nResourceID, UINT nFrameCount)
//{
//	ImageFromID(nResourceID, m_pImage);
//	this->m_frameCount = nFrameCount;
//	m_sizeImage.SetSize(m_pImage->GetWidth() / m_frameCount, m_pImage->GetHeight());
//}


void CCheckBoxEx::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CWnd::OnLButtonUp(nFlags, point);
	
	/*if (GetCheck() == BST_CHECKED)
		m_buttonState = bs_check_down;
	else
		m_buttonState = bs_ncheck_down;*/
	/*Invalidate();*/

	/*if (m_buttonState == bsDown)
	{
		if (GetCheck() == BST_CHECKED)
			m_buttonState = bsHoverDown;
		else
			m_buttonState = bsHover;
		Invalidate();
	}*/
	
}


void CCheckBoxEx::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CWnd::OnLButtonDown(nFlags, point);
//	this->m_bCheck = !this->m_bCheck;
	/*if (GetCheck() == BST_CHECKED)
		m_buttonState = bs_check_down;
	else
		m_buttonState = bs_ncheck_down;*/

	//this->m_bCheck = !this->m_bCheck;
	if (GetCheck() == BST_CHECKED)
		m_buttonState = bsHoverDown;
	else
		m_buttonState = bsHover;

	::SetCursor(::LoadCursor(NULL, IDC_HAND));
	Invalidate();
	PaintParent();
}


void CCheckBoxEx::OnMouseLeave()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CButton::OnMouseLeave();
	/*if (GetCheck() == BST_CHECKED)
		m_buttonState = bs_check_leave;
	else
		m_buttonState = bs_ncheck_leave;*/
	
	if (m_bCheck)
		m_buttonState = bsDown;
	else
		m_buttonState = bsNormal;

	::SetCursor(::LoadCursor(NULL, IDC_HAND));

	Invalidate();
	PaintParent();
}


void CCheckBoxEx::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CButton::OnMouseMove(nFlags, point);
	TRACKMOUSEEVENT tme;
	tme.cbSize = sizeof(TRACKMOUSEEVENT);
	tme.hwndTrack = this->m_hWnd;
	tme.dwFlags = TME_LEAVE | TME_HOVER;
	_TrackMouseEvent(&tme);

	::SetCursor(::LoadCursor(NULL, IDC_HAND));
	/*if (GetCheck() == BST_CHECKED)
		m_buttonState = bs_check_move;
	else
		m_buttonState = bs_ncheck_move;*/

	if (m_bCheck)
		m_buttonState = bsHoverDown;
	else
		m_buttonState = bsHover;	
	Invalidate();
	PaintParent();
}

afx_msg LRESULT CCheckBoxEx::OnBmGetcheck(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	return GetCheck();
}


afx_msg LRESULT CCheckBoxEx::OnBmSetcheck(WPARAM wParam, LPARAM /*lParam*/)
{
	if (wParam == BST_CHECKED)
		SetCheck(BST_CHECKED);
	else
		SetCheck(BST_UNCHECKED);
	return 0;
}

BOOL CCheckBoxEx::GetCheck()
{
	return m_bCheck;
}

void CCheckBoxEx::SetCheck(BOOL bCheck, BOOL /*bRepaint*//* =TRUE */)
{
	this->m_bCheck = bCheck;
	/*if (m_bCheck)
		m_buttonState = bs_check_down;
	else
		m_buttonState = bs_ncheck_leave;*/
	if (m_bCheck)
		m_buttonState = bsDown;
	else
		m_buttonState = bsNormal;
//	if (bRepaint)
//		Invalidate();
}

void CCheckBoxEx::OnBnClicked()
{
	// TODO: 在此添加控件通知处理程序代码
	
	this->m_bCheck = !this->m_bCheck;
}


void CCheckBoxEx::BitmapToImage(CBitmap *pBitmap, Image* &img)
{
	//拿到位图的句柄 Bitmap是Image的子类
	HBITMAP  hBmp = (HBITMAP)pBitmap->GetSafeHandle();
	//创建一个从位图句柄的Bitmap位图
	Bitmap *bmp = new Bitmap(hBmp, NULL);
	//创建一个临时的位图
	Bitmap *temp = new Bitmap(bmp->GetWidth(), bmp->GetHeight(), PixelFormat24bppRGB);
	Graphics g(temp);//临时位图的画笔
	g.DrawImage(bmp, 0, 0);//将数据画到临时位图上
	img = temp;
	//最后删除位图不然会内存泄漏
	delete bmp;
}

void CCheckBoxEx::ImageFromID(int IDB_ID, Image* &_outImg)
{
	LPCTSTR lpType = _T("PNG");
	if (lpType == RT_BITMAP)
	{
		//GDI+ can not load RT_BITMAP resouce, 
		//because they are predefined resource, 
		//they don't contains the image file header.
		return;
	}
	HINSTANCE hInst = AfxGetResourceHandle();
	HRSRC hRsrc = ::FindResource(hInst, MAKEINTRESOURCE(IDB_ID), lpType);
	if (!hRsrc)
		return;
	DWORD len = SizeofResource(hInst, hRsrc);
	BYTE* lpRsrc = (BYTE*)LoadResource(hInst, hRsrc);
	if (!lpRsrc)
		return;
	HGLOBAL m_hMem = GlobalAlloc(GMEM_FIXED, len);
	BYTE* pmem = (BYTE*)GlobalLock(m_hMem);
	memcpy(pmem, lpRsrc, len);
	IStream* pstm;
	if (CreateStreamOnHGlobal(m_hMem, FALSE, &pstm) == S_OK)
	{
		_outImg = new Image(pstm);
		GlobalUnlock(m_hMem);
		pstm->Release();
	}
	FreeResource(lpRsrc);
}

//// 画图片边框
//void CCheckBoxEx::DrawImageFrame(Graphics &graphics, Image *pIamge, const CRect &rcControl, REAL nX, REAL nY, REAL nW, REAL nH, REAL nFrameSide/* = 3*/)
//{
//	// 左上角
//	graphics.DrawImage(pIamge, RectF((REAL)rcControl.left, (REAL)rcControl.top, nFrameSide, nFrameSide),
//		nX, nY, nFrameSide, nFrameSide, UnitPixel);
//
//	// 左中边框
//	graphics.DrawImage(pIamge, RectF((REAL)rcControl.left, (REAL)rcControl.top + nFrameSide, nFrameSide, (REAL)rcControl.Height() - 2 * nFrameSide),
//		nX, nY + nFrameSide, nFrameSide, nH - 2 * nFrameSide, UnitPixel);
//
//	// 左下角
//	graphics.DrawImage(pIamge, RectF((REAL)rcControl.left, (REAL)rcControl.bottom - nFrameSide, nFrameSide, nFrameSide),
//		nX, nY + nH - nFrameSide, nFrameSide, nFrameSide, UnitPixel);
//
//	// 上中边框
//	graphics.DrawImage(pIamge, RectF((REAL)rcControl.left + nFrameSide, (REAL)rcControl.top, (REAL)rcControl.Width() - 2 * nFrameSide, nFrameSide),
//		nX + nFrameSide, nY, nW - 2 * nFrameSide, nFrameSide, UnitPixel);
//
//	// 右上角
//	graphics.DrawImage(pIamge, RectF((REAL)rcControl.right - nFrameSide, (REAL)rcControl.top, nFrameSide, nFrameSide),
//		nX + nW - nFrameSide, nY, nFrameSide, nFrameSide, UnitPixel);
//
//	// 右中边框
//	graphics.DrawImage(pIamge, RectF((REAL)rcControl.right - nFrameSide, (REAL)rcControl.top + nFrameSide, nFrameSide, (REAL)rcControl.Height() - 2 * nFrameSide),
//		nX + nW - nFrameSide, nY + nFrameSide, nFrameSide, nH - 2 * nFrameSide, UnitPixel);
//
//	// 右下角
//	graphics.DrawImage(pIamge, RectF((REAL)rcControl.right - nFrameSide, (REAL)rcControl.bottom - nFrameSide, nFrameSide, nFrameSide),
//		nX + nW - nFrameSide, nY + nH - nFrameSide, nFrameSide, nFrameSide, UnitPixel);
//
//	// 下中边框
//	graphics.DrawImage(pIamge, RectF((REAL)rcControl.left + nFrameSide, (REAL)rcControl.bottom - nFrameSide, (REAL)rcControl.Width() - 2 * nFrameSide, nFrameSide),
//		nX + nFrameSide, nY + nH - nFrameSide, nW - 2 * nFrameSide, nFrameSide, UnitPixel);
//
//	// 中间
//	graphics.DrawImage(pIamge, RectF((REAL)rcControl.left + nFrameSide, (REAL)rcControl.top + nFrameSide, (REAL)rcControl.Width() - 2 * nFrameSide, (REAL)rcControl.Height() - 2 * nFrameSide),
//		nX + nFrameSide, nY + nFrameSide, nW - 2 * nFrameSide, nH - 2 * nFrameSide, UnitPixel);
//}

void CCheckBoxEx::Init(UINT nImg, int nPartNum/*, UINT nBtnType*/)
{

	ImageFromID(nImg, m_pImage);
	//m_nBtnType = nBtnType;
	m_nImgPart = nPartNum;

	if (m_pImage == NULL)
		return;

	CRect rcButton;

	if (m_nImgPart == BTN_IMG_1)
		rcButton = CRect(0, 0, m_pImage->GetWidth(), m_pImage->GetHeight());
	else if (m_nImgPart == BTN_IMG_3)
		//rcButton = CRect(0, 0, m_pImage->GetWidth()/3, m_pImage->GetHeight());
		rcButton = CRect(0, 0, m_pImage->GetWidth(), m_pImage->GetHeight() / 3);
	else if (m_nImgPart == BTN_IMG_4)
		rcButton = CRect(0, 0, m_pImage->GetWidth(), m_pImage->GetHeight() / 4);
	else if (m_nImgPart == BTN_IMG_5)
		rcButton = CRect(0, 0, m_pImage->GetWidth(), m_pImage->GetHeight() / 5);
	else if (m_nImgPart == BTN_IMG_6)
		rcButton = CRect(0, 0, m_pImage->GetWidth(), m_pImage->GetHeight() / 6);
	else
		return;

//	SetWindowPos(NULL, 0, 0, rcButton.Width(), rcButton.Height(), SWP_NOACTIVATE | SWP_NOMOVE);
}

bool CCheckBoxEx::ShowImage(CDC* pDC, Image* pImage, UINT nState)
{
	bool bSuc = false;

	if (pImage != NULL)
	{
		CRect rcButton;

		if (m_nImgPart == BTN_IMG_1)
		{
			rcButton = CRect(0, 0, m_pImage->GetWidth(), m_pImage->GetHeight());
		}
		//else if (m_nImgPart == BTN_IMG_3)
		//{
		//	if (nState == bsNormal)
		//		//rcButton = CRect(0, 0, m_pImage->GetWidth()/3, m_pImage->GetHeight());
		//		rcButton = CRect(0, 0, m_pImage->GetWidth(), m_pImage->GetHeight() / 3);
		//	else if (nState == bsDown)
		//		//rcButton = CRect(m_pImage->GetWidth()/3, 0, m_pImage->GetWidth()/3 * 2, m_pImage->GetHeight());
		//		rcButton = CRect(0, m_pImage->GetHeight() / 3, m_pImage->GetWidth(), m_pImage->GetHeight() / 3 * 2);
		//	else if (nState == bsHover)
		//		//rcButton = CRect(m_pImage->GetWidth()/3 * 2, 0, m_pImage->GetWidth(), m_pImage->GetHeight());
		//		rcButton = CRect(0, m_pImage->GetHeight() / 3 * 2, m_pImage->GetWidth(), m_pImage->GetHeight());

		//	else
		//		return false;
		//}
		//else if (m_nImgPart == BTN_IMG_4)
		//{
		//	if (nState == bsNormal)
		//		rcButton = CRect(0, 0, m_pImage->GetWidth() / 4, m_pImage->GetHeight());
		//	else if (nState == bsDown)
		//		rcButton = CRect(m_pImage->GetWidth() / 4, 0, m_pImage->GetWidth() / 4 * 2, m_pImage->GetHeight());
		//	else if (nState == bsHover)
		//		rcButton = CRect(m_pImage->GetWidth() / 4 * 2, 0, m_pImage->GetWidth() / 4 * 3, m_pImage->GetHeight());
		//	else if (nState == bsHoverDown)
		//		rcButton = CRect(m_pImage->GetWidth() / 4 * 3, 0, m_pImage->GetWidth(), m_pImage->GetHeight());
		//	else
		//		return false;
		//}
		//else if (m_nImgPart == BTN_IMG_5)
		//{
		//	if (nState == bsNormal)
		//		rcButton = CRect(0, 0, m_pImage->GetWidth(), m_pImage->GetHeight() / 5);
		//	else if (nState == bsDown)
		//		//rcButton = CRect(0, m_pImage->GetWidth() / 5, m_pImage->GetWidth() / 4 * 2, m_pImage->GetHeight());
		//		rcButton = CRect(0, m_pImage->GetHeight() / 5, m_pImage->GetWidth(), m_pImage->GetHeight() / 5 * 2);
		//	else if (nState == bsHover)
		//		//rcButton = CRect(m_pImage->GetWidth() / 4 * 2, 0, m_pImage->GetWidth() / 4 * 3, m_pImage->GetHeight());
		//		rcButton = CRect(0, m_pImage->GetHeight() / 5 * 2, m_pImage->GetWidth(), m_pImage->GetHeight() / 5 * 3);
		//	else if (nState == bsHoverDown)
		//		rcButton = CRect(0, m_pImage->GetHeight() / 5 * 3, m_pImage->GetWidth(), m_pImage->GetHeight());
		//	else
		//		return false;
		//}
		else if (m_nImgPart == BTN_IMG_6)
		{
			
			if (nState == bsNormal)
				rcButton = CRect(0, 0, m_pImage->GetWidth(), m_pImage->GetHeight() / 6);
			else if (nState == bsHover)
				rcButton = CRect(0, m_pImage->GetHeight() / 6, m_pImage->GetWidth(), m_pImage->GetHeight() / 6 * 2);
		//	else if (nState == bs_ncheck_down)
		//		rcButton = CRect(0, m_pImage->GetHeight() / 6 * 2, m_pImage->GetWidth(), m_pImage->GetHeight() / 6 * 3);
	
						
			else if (nState == bsDown)
				rcButton = CRect(0, m_pImage->GetHeight() / 6 * 3, m_pImage->GetWidth(), m_pImage->GetHeight() / 6 * 4);
			else if (nState == bsHoverDown)
				rcButton = CRect(0, m_pImage->GetHeight() / 6 * 4, m_pImage->GetWidth(), m_pImage->GetHeight() / 6 * 5);
		//	else if (nState == bs_check_down)
		//		rcButton = CRect(0, m_pImage->GetHeight() / 6 * 5, m_pImage->GetWidth(), m_pImage->GetHeight());
		else
			return false;


		
			
		}
		else
			return false;

		Graphics graph(pDC->GetSafeHdc());
		graph.DrawImage(pImage, 0, 0, rcButton.left, rcButton.top, rcButton.Width(), rcButton.Height(), UnitPixel);
		//graph.DrawImage(pImage, rcButton.left, rcButton.top, rcButton.Width(), rcButton.Height());
		graph.ReleaseHDC(pDC->GetSafeHdc());
		bSuc = true;
	}

	return bSuc;
}


void CCheckBoxEx::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CButton::OnPaint() for painting messages

	//ShowImage(&dc, m_pImage, m_buttonState);
}

void CCheckBoxEx::PaintParent()
{
	CRect   rect;
	GetWindowRect(&rect);
	GetParent()->ScreenToClient(&rect);
	GetParent()->InvalidateRect(&rect);
}

#include "LinkStatic.h"


CLinkStatic::CLinkStatic()
{
}


CLinkStatic::~CLinkStatic()
{
}
BEGIN_MESSAGE_MAP(CLinkStatic, CStatic)
	//ON_WM_MOUSEMOVE()
	//ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
END_MESSAGE_MAP()


//void CLinkStatic::OnMouseMove(UINT nFlags, CPoint point)
//{
//	// TODO: Add your message handler code here and/or call default
//
//	/*HCURSOR hCursor;
//	hCursor = AfxGetApp()->LoadCursor(IDC_HAND);
//	::SetCursor(hCursor);*/
//	//::SetCursor(::LoadCursor(NULL, IDC_HAND));
//
//	CStatic::OnMouseMove(nFlags, point);
//}
//
//
//void CLinkStatic::OnLButtonDown(UINT nFlags, CPoint point)
//{
//	// TODO: Add your message handler code here and/or call default
//
//	//::SetCursor(::LoadCursor(NULL, IDC_HAND));
//	//ShellExecute(NULL, NULL, url, NULL, NULL, SW_NORMAL);
//	CStatic::OnLButtonDown(nFlags, point);
//}


void CLinkStatic::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CStatic::OnPaint() for painting messages
	CPen pen(PS_SOLID, 0, RGB(0, 0, 255));
	dc.SelectObject(&pen);

	CFont font;
	font.CreatePointFont(100, _T("微软雅黑"));

	CRect rect;
	GetWindowRect(&rect);
	ScreenToClient(&rect);

	dc.SelectObject(&font);
	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(RGB(44, 173, 219));
	dc.TextOut(rect.left, rect.top, m_strCaption);
	dc.SetTextAlign(TA_UPDATECP | TA_TOP);



//	dc.MoveTo(rect.left, rect.top + rect.Height() / 2 + 12);
//	dc.LineTo(rect.left + 205, rect.top + rect.Height() / 2 + 12);
}

void CLinkStatic::SetCaption(LPCTSTR _strCaption)
{
	m_strCaption = _strCaption;
}

// PngButton.cpp : implementation file
//

#include "stdafx.h"
#include "PngButton.h"
#include "resource.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPngButton

CPngButton::CPngButton()
{
	m_bTracked=false;
 	m_bMenuOn = FALSE;

	m_nImgPart = 3;
	m_pImage = NULL;

	m_nState = CTRL_NOFOCUS;
	m_nBtnType = BTN_TYPE_NORMAL;
	m_nOrien = ORIEN_LEFT;
	m_pFont = new CFont;
	m_pFont->CreateFont(17, 9, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
    DEFAULT_CHARSET, OUT_RASTER_PRECIS, CLIP_DEFAULT_PRECIS,
    VARIABLE_PITCH | PROOF_QUALITY, FF_DONTCARE, _T("Arial"));
	strCaption = " ";
	m_bChecked = FALSE;

	m_gdiplusToken = 0;
	GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);
}

CPngButton::~CPngButton()
{
	if(m_pImage == NULL) {
		delete m_pImage;
		m_pImage = NULL;
	}

	if (m_pFont) {
		delete m_pFont;
		m_pFont = NULL;
	}

	GdiplusShutdown(m_gdiplusToken);
}

void CPngButton::Init(UINT nImg, int nPartNum, int  orien,UINT nBtnType)
{
	SetWindowText(strCaption);

	m_pImage = ImageFromResource(AfxGetResourceHandle(), nImg, TEXT("PNG"));
	m_nBtnType = nBtnType;
	m_nImgPart = nPartNum;
	m_nOrien = orien;
  if (m_pImage == NULL) {
    return;
  }
	
	CRect rcButton;

  //if (m_nImgPart == BTN_IMG_1) {
  //  rcButton = CRect(0, 0, m_pImage->GetWidth(), m_pImage->GetHeight());
  //} else if (m_nImgPart == BTN_IMG_3) {
  //  //rcButton = CRect(0, 0, m_pImage->GetWidth()/3, m_pImage->GetHeight());
  //  rcButton = CRect(0, 0, m_pImage->GetWidth(), m_pImage->GetHeight() / 3);
  //} else if (m_nImgPart == BTN_IMG_4) {
  //  rcButton = CRect(0, 0, m_pImage->GetWidth(), m_pImage->GetHeight() / 4);
  //} else if (m_nImgPart == BTN_IMG_5) {
  //  rcButton = CRect(0, 0, m_pImage->GetWidth(), m_pImage->GetHeight() / 5);
  //} else {
  //  return;
  //}

	unsigned int w = m_pImage->GetWidth();
	unsigned int h = m_pImage->GetHeight();


	if (ORIEN_LEFT == orien)
	{
		//rcButton = CRect(0, 0, w / m_nImgPart , h);
		rcButton = CRect(w / m_nImgPart * (m_nImgPart -1), 0, w, h);
	} 
	else
	{
		//rcButton = CRect(0, 0, w  , h / m_nImgPart);
		rcButton = CRect(0, h / m_nImgPart * (m_nImgPart -1), w, h);
	}

	SetWindowPos(NULL, 0, 0, rcButton.Width(), rcButton.Height(), SWP_NOACTIVATE|SWP_NOMOVE);
}

BEGIN_MESSAGE_MAP(CPngButton, CButton)
	//{{AFX_MSG_MAP(CPngButton)
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_PAINT()
	ON_MESSAGE(WM_MOUSEHOVER,OnMouseHOver)
	ON_MESSAGE(WM_MOUSELEAVE,OnMouseLeave)
	ON_MESSAGE(BM_GETCHECK, OnGetCheck)
	ON_MESSAGE(BM_SETCHECK, OnSetCheck)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPngButton message handlers

void CPngButton::OnPaint()
{
	//SetWindowTextW(_T("Install"));
	CButton::OnPaint();
}


void CPngButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	if (!IsWindowEnabled())
		m_nState = CTRL_DISABLE;

	CDC dc;
	dc.Attach(lpDrawItemStruct->hDC);


	ShowImage(&dc, m_pImage, m_nState);

	//得当Button上文字,这里的步骤是:1,先得到在资源里编辑的按钮的文字,
	//然后将此文字重新绘制到按钮上,
	//同时将此文字的背景色设为透明,这样,按钮上仅会显示文字
	//const int bufSize = 512;
	CString buffer;
	//GetWindowText(buffer);
	buffer = strCaption;
	int size = buffer.GetLength();//得到长度

	DrawText(lpDrawItemStruct->hDC, buffer, size, &lpDrawItemStruct->rcItem,
    DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_TABSTOP);//绘制文字
	SetBkMode(lpDrawItemStruct->hDC, TRANSPARENT);//透明
	
	UINT state = lpDrawItemStruct->itemState;
	//获取按钮的状态
	if (state & ODS_CHECKED) {
		CButton::SetCheck(TRUE);
	} else {
		CButton::SetCheck(FALSE);
	}
	

}

bool CPngButton::ShowImage(CDC* pDC, Image* pImage, UINT nState)
{
	bool bSuc = false;

	if(pImage!=NULL) 
	{
		CRect rcButton;

		unsigned int w = m_pImage->GetWidth();
		unsigned int h = m_pImage->GetHeight();
		if(ORIEN_LEFT == m_nOrien) // 从左到右排列
		{
			switch (m_nImgPart)
			{
			case BTN_IMG_1:
				rcButton = CRect(0, 0, w, h);
				break;
			case BTN_IMG_3:
				{
					switch (nState)
					{
					case CTRL_NOFOCUS:
						rcButton = CRect(0, 0, w / 3 , h);
						break;
					case CTRL_FOCUS:
						rcButton = CRect(w / 3, 0, w / 3 * 2, h);
						break;
					case CTRL_SELECTED:
						rcButton = CRect(w / 3 * 2, 0, w , h);
						break;
					case CTRL_DISABLE:
						rcButton = CRect(0, 0, w / 3 , h);
						break;
					default:
						break;
					}
				}
				break;
			case BTN_IMG_4:
				{
					switch (nState)
					{
					case CTRL_DISABLE:
						rcButton = CRect(0, 0, w / 4, h);
						break;
					case CTRL_FOCUS:
						rcButton = CRect(w / 4, 0, w / 4 * 2, h);
						break;
					case CTRL_SELECTED:
						rcButton = CRect(w / 4 * 2, 0, w / 4 * 3, h);
						break;
					case CTRL_NOFOCUS:
						rcButton = CRect(w / 4 * 3, 0, w, h);
						break;
					default:
						break;
					}
				}
				break;

			case BTN_IMG_5:
				{
					switch (nState)
					{
					case CTRL_DISABLE:
						rcButton = CRect(0, 0, w / 5, h);
						break;
					case CTRL_FOCUS:
						rcButton = CRect(w / 5, 0, w / 5 * 2, h);
						break;
					case CTRL_SELECTED:
						rcButton = CRect(w / 5 * 2, 0, w / 5 * 3, h);
						break;
					case CTRL_NOFOCUS:
						rcButton = CRect(w / 5 * 3, 0, w / 5 * 4, h);
						break;
					default:
						break;
					}
				}
				break;

			case BTN_IMG_6:
				{
					if (!m_bChecked) // check button 未选中
					{
						switch (nState)
						{
						case CTRL_NOFOCUS:
						case CTRL_DISABLE:
							rcButton = CRect(0, 0, w / 6 , h);
							break;
						case CTRL_FOCUS:
							rcButton = CRect(w / 6, 0, w / 6 * 2, h);
							break;
						case CTRL_SELECTED:
							rcButton = CRect(w / 6 * 2, 0, w / 6 * 3 , h);
							break;
						default:
							break;
						}
					} 
					else  // 选中
					{
						switch (nState)
						{
						case CTRL_NOFOCUS:
						case CTRL_DISABLE:
							rcButton = CRect(w / 6 * 3, 0, w / 6 * 4 , h);
							break;
						case CTRL_FOCUS:
							rcButton = CRect(w / 6 * 4, 0, w / 6 * 5, h);
							break;
						case CTRL_SELECTED:
							rcButton = CRect(w / 6 * 5, 0, w  , h);
							break;
						default:
							break;
						}
					}			
				} // case BTN_IMG_6:
				break;

			}
		} 
		else  // 从上到下排列
		{
			switch (m_nImgPart)
			{
			case BTN_IMG_1:
				rcButton = CRect(0, 0, w, h);
				break;
			case BTN_IMG_3:
				{
					switch (nState)
					{
					case CTRL_NOFOCUS:
						rcButton = CRect(0, 0, w, h / 3);
						break;
					case CTRL_FOCUS:
						rcButton = CRect(0, h / 3, w, h / 3 * 2);
						break;
					case CTRL_SELECTED:
						rcButton = CRect(0, h / 3 * 2, w , h);
						break;
					case CTRL_DISABLE:
						rcButton = CRect(0, 0, w, h / 3);
						break;
					default:
						break;
					}
				}
				break;
			case BTN_IMG_4:
				{
					switch (nState)
					{
					case CTRL_DISABLE:
						rcButton = CRect(0, 0, w, h / 4);
						break;
					case CTRL_FOCUS:
						rcButton = CRect(0, h / 4, w, h / 4 * 2);
						break;
					case CTRL_SELECTED:
						rcButton = CRect(0, h / 4 * 2, w , h / 4 * 3);
						break;
					case CTRL_NOFOCUS:
						rcButton = CRect(0, h / 4 * 3, w, h);
						break;
					default:
						break;				
					}			
				}
				break;

			case BTN_IMG_5:
				{
					switch (nState)
					{
					case CTRL_NOFOCUS:
					case CTRL_FOCUS:
					case CTRL_SELECTED:
					case CTRL_DISABLE:
						rcButton = CRect(0, 0, w, h / 5);
						break;
					//case CTRL_FOCUS:
					//	rcButton = CRect(0, h / 5, w, h / 5 * 2);
					//	break;
					//case CTRL_SELECTED:
					//	rcButton = CRect(0, h / 5 * 2, w , h / 5 * 3);
					//	break;
					//case CTRL_DISABLE:
					//	rcButton = CRect(0, h / 5 * 3, w, h / 5 * 4);
					//	break;
					default:
						break;				
					}			
				}
				break;

			case BTN_IMG_6:
				{
					if (!m_bChecked) // check button 未选中
					{
						switch (nState)
						{
						case CTRL_NOFOCUS:
						case CTRL_DISABLE:
							rcButton = CRect(0, 0, w, h / 6);
							break;
						case CTRL_FOCUS:
							rcButton = CRect(0, h / 6, w, h / 6 * 2);
							break;
						case CTRL_SELECTED:
							rcButton = CRect(0, h / 6 * 2, w , h / 6 * 3);
							break;
						default:
							break;
						}
					} 
					else  // 选中
					{
						switch (nState)
						{
						case CTRL_NOFOCUS:
						case CTRL_DISABLE:
							rcButton = CRect(0, h / 6 * 3, w , h / 6 * 4);
							break;
						case CTRL_FOCUS:
							rcButton = CRect(0, h / 6 * 4, w, h / 6 * 5);
							break;
						case CTRL_SELECTED:
							rcButton = CRect(0, h / 6 * 5, w , h );
							break;
						default:
							break;
						}
					}			
				} // case BTN_IMG_6:
				break;
			}
		}


		Graphics graph(pDC->GetSafeHdc());
		graph.DrawImage(pImage, 0, 0, rcButton.left, rcButton.top, rcButton.Width(), rcButton.Height(), UnitPixel);
		//graph.DrawImage(pImage, rcButton.left, rcButton.top, rcButton.Width(), rcButton.Height());
		graph.ReleaseHDC(pDC->GetSafeHdc());
		bSuc=true;
	}

	return bSuc;
}

Image *CPngButton::ImageFromResource(HINSTANCE hInstance,UINT uImgID,LPCTSTR lpType)
{
    HRSRC hResInfo=::FindResource(hInstance,MAKEINTRESOURCE(uImgID),lpType);
	if(hResInfo==NULL)
		return NULL; //fail
	DWORD dwSize;
	dwSize=SizeofResource(hInstance,hResInfo); //get resource size(bytes) 
	HGLOBAL hResData;
	hResData=::LoadResource(hInstance,hResInfo);
	if(hResData==NULL)
		return NULL; //fail
    HGLOBAL hMem;
	hMem=::GlobalAlloc(GMEM_MOVEABLE,dwSize);
    if(hMem==NULL) {
		::FreeResource(hResData);
		return NULL;
	}
	LPVOID lpResData,lpMem;
	lpResData=::LockResource(hResData);
	lpMem=::GlobalLock(hMem);
	::CopyMemory(lpMem,lpResData,dwSize); //copy memory
	::GlobalUnlock(hMem);
    ::FreeResource(hResData); //free memory
    
	IStream *pStream;
	HRESULT hr;
	hr=::CreateStreamOnHGlobal(hMem,TRUE,&pStream);//create stream object
	Image *pImage=NULL;
	if(SUCCEEDED(hr)){
		pImage=Image::FromStream(pStream);//get GDI+ pointer
		pStream->Release();
	}
	::GlobalFree(hMem);
	return pImage;
}

void CPngButton::PreSubclassWindow() 
{
	ModifyStyle(0, BS_OWNERDRAW);

	if (NULL != GetSafeHwnd()) {
    if (!(GetButtonStyle() & WS_CLIPSIBLINGS)) {
      SetWindowLong(GetSafeHwnd(), GWL_STYLE, GetWindowLong(GetSafeHwnd(),
        GWL_STYLE) | WS_CLIPSIBLINGS);
    }
	}

	CButton::PreSubclassWindow();
}

BOOL CPngButton::OnEraseBkgnd(CDC* /*pDC*/) 
{
	return TRUE;
}

void CPngButton::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(!m_bTracked) {
		TRACKMOUSEEVENT tme;
		ZeroMemory(&tme,sizeof(TRACKMOUSEEVENT));
		tme.cbSize=sizeof(TRACKMOUSEEVENT);
		tme.dwFlags=TME_HOVER|TME_LEAVE;
		tme.dwHoverTime=1;
		tme.hwndTrack=this->GetSafeHwnd();
    if (::_TrackMouseEvent(&tme)) {
      m_bTracked = true;
    }
	}
	::SetCursor(::LoadCursor(NULL, IDC_HAND));
	CButton::OnMouseMove(nFlags, point);
}

void CPngButton::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_nState != CTRL_SELECTED) {
		m_nState = CTRL_SELECTED;		
    if (!m_bMenuOn) {
      m_bMenuOn = TRUE;
    }
		PaintParent();
	}
//	m_bChecked = TRUE;
	//RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_ERASE | RDW_UPDATENOW | RDW_ERASENOW);
	CButton::OnLButtonDown(nFlags, point);
}

void CPngButton::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_nState != CTRL_FOCUS) {
		m_nState = CTRL_FOCUS;
		PaintParent();
	}
	
	CRect rcClient;
	GetClientRect(&rcClient);
	if (rcClient.PtInRect(point)) {
		m_bChecked = !m_bChecked;
	}

	RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_ERASE | RDW_UPDATENOW | RDW_ERASENOW);

	CButton::OnLButtonUp(nFlags, point);
}

LRESULT CPngButton::OnMouseHOver(WPARAM /*wParam*/,LPARAM /*lParam*/)
{
   
	if (m_nState != CTRL_FOCUS) {
		m_nState = CTRL_FOCUS;
		PaintParent();	
	}
	
	return 0;
}
LRESULT CPngButton::OnMouseLeave(WPARAM /*wParam*/,LPARAM /*lParam*/)
{
	m_bTracked=false;

  if (m_nBtnType == BTN_TYPE_NORMAL) {
    m_nState = CTRL_NOFOCUS;
  } else if (m_nBtnType == BTN_TYPE_MENU) {
    if (m_bMenuOn) {
      m_nState = CTRL_SELECTED;
    } else {
      m_nState = CTRL_NOFOCUS;
    }
	}

	PaintParent(); 
	
	return 0;
}

void CPngButton::PaintParent() 
{   
	CRect   rect;  
	GetWindowRect(&rect);  
	GetParent()-> ScreenToClient(&rect);  
	GetParent()-> InvalidateRect(&rect); 
} 


void CPngButton::SetText(CString str)
{
	m_strText = _T("");
	SetWindowText(str);
}

LRESULT CPngButton::OnGetCheck(WPARAM, LPARAM)
{
	return m_bChecked ? BST_CHECKED : BST_UNCHECKED;
}

LRESULT CPngButton::OnSetCheck(WPARAM fCheck, LPARAM)
{
	ASSERT(fCheck != BST_INDETERMINATE);

	if ((!m_bChecked) != (fCheck == BST_UNCHECKED)) {
		m_bChecked = fCheck != BST_UNCHECKED;
		RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_ERASE | RDW_UPDATENOW | RDW_ERASENOW);
	}

	//if (m_bChecked)
	//{
	//	::MessageBox(NULL,TEXT("Pngbutton选中!"),"Pngbutton",MB_OK);
	//} 
	//else
	//{
	//	::MessageBox(NULL,TEXT("Pngbutton未选中!"),"Pngbutton",MB_OK);

	//}

	return 0;
}
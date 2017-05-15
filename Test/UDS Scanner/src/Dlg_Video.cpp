// Dlg_Video.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "UDS Scanner.h"
#include "Dlg_Video.h"
#include "afxdialogex.h"

#include "public.h"
#include "ximage.h"  // CXImage

//#include <MMSystem.h>
//#pragma comment(lib, "Winmm.lib")  // ������������Ҫ
// CDlg_Video �Ի���



extern 	std::vector<std::string> g_vector_imagepath;
extern void GetFilePath( char* szFileName, char* szFilePath);

IMPLEMENT_DYNAMIC(CDlg_Video, CDialogEx)
CDlg_Video::CDlg_Video(MFC_UI *pUI,CWnd* pParent /*= NULL*/)
	:m_pUI(pUI), CDialogEx(CDlg_Video::IDD, pParent)
	, m_nRotateType(0)
	, m_nDevIndex(0)
	, m_nResIndex(0)
	, m_nFileType(1) // 0-jpg 1-bmp 2-png 3-tif
	, m_bInitialend(false)
	, m_sCaptureName(TEXT(""))
	, m_nFileCount(0)
	, m_nPixelType(0)
	, m_strTempPath(TEXT("C:\\UUTD temp"))
	, m_bIsDPI(true)
  //, m_nDPIIndex(0)
	, m_bAutoCrop(false)
	//, m_bShowInfo(false)
	//, m_bPlaySound(false)
{

}

CDlg_Video::~CDlg_Video()
{
	ClearAndDeleteDir(m_strTempPath);
	g_vector_imagepath.swap(std::vector<std::string>());  // �����������С����������

}

void CDlg_Video::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_VIDEO_UDS_VIDEOCTRL, m_ocx);
	DDX_Control(pDX, IDC_VIDEO_COMBO_DEV, m_CombDev);
	DDX_Control(pDX, IDC_VIDEO_COMBO_RESO, m_CombReso);
	DDX_Control(pDX, IDC_VIDEO_COMBO_COLOR, m_CombColor);
	DDX_Control(pDX, IDC_VIDEO_CHECK_AUTOEXP, m_checkAutoExp);
	DDX_Control(pDX, IDC_VIDEO_CHECK_AUTOCROP, m_CheckAC);
	//DDX_Control(pDX, IDC_VIDEO_CHECK_SHOWINFO, m_CheckMSG);
	DDX_Control(pDX, IDC_VIDEO_SLIDER_EXP, m_sliderExp);
	//DDX_Control(pDX, IDC_VIDEO_EDIT_EXPVALUE, m_editExp);
	//DDX_Control(pDX, IDC_VIDEO_EDIT_PATH, m_editPath);
	DDX_Control(pDX, IDC_VIDEO_CHECK_MANUAL, m_checkManual);
	//DDX_Control(pDX, IDC_VIDEO_RICHEDIT2_MSG, m_richeditMSG);
	//DDX_Control(pDX, IDC_VIDEO_COMBO_FILETYPE, m_combFiletype);
	//DDX_Control(pDX, IDC_VIDEO_EDIT_URL, m_editURL);
	DDX_Control(pDX, IDC_VIDEO_STATIC_CAPTURECOUNT, m_sCaptureCount);
	DDX_Control(pDX, IDC_VIDEO_BUTTON_DPI, m_button_DPI);
	//DDX_Control(pDX, IDC_VIDEO_CHECK_PLAYSOUND, m_checkPlaySound);
	DDX_Control(pDX, IDC_VIDEO_BUTTON_ROTLEFT, m_button_rotleft);
	DDX_Control(pDX, IDC_VIDEO_CLOSE, m_button_close);
	DDX_Control(pDX, IDC_VIDEO_BUTTON_ROTRIGHT, m_button_rotright);
	DDX_Control(pDX, IDOK, m_button_ok);
	DDX_Control(pDX, IDC_VIDEO_BUTTON_CAPTURE, m_button_capture);
	DDX_Control(pDX, IDC_VIDEO_BUTTON_SERCAP, m_button_selfcap);
	DDX_Control(pDX, IDC_VIDEO_LIST_THUMBNAIL, m_listctrl);
}


BEGIN_MESSAGE_MAP(CDlg_Video, CDialogEx)
	ON_WM_CLOSE()
	ON_CBN_SELCHANGE(IDC_VIDEO_COMBO_DEV, &CDlg_Video::OnCbnSelchangeCombo_Dev)
	ON_CBN_SELCHANGE(IDC_VIDEO_COMBO_RESO, &CDlg_Video::OnCbnSelchangeCombo_Reso)
	ON_CBN_SELCHANGE(IDC_VIDEO_COMBO_COLOR, &CDlg_Video::OnCbnSelchangeCombo_Color)
	//ON_CBN_SELCHANGE(IDC_VIDEO_COMBO_FILETYPE, &CDlg_Video::OnCbnSelchangeCombo_Filetype)
	ON_BN_CLICKED(IDC_VIDEO_CHECK_AUTOEXP, &CDlg_Video::OnBnClickedCheck_Autoexp)
	ON_BN_CLICKED(IDC_VIDEO_CHECK_AUTOCROP, &CDlg_Video::OnBnClickedCheck_Autocrop)
	ON_BN_CLICKED(IDC_VIDEO_CHECK_MANUAL, &CDlg_Video::OnBnClickedCheck_Manual)
	//ON_BN_CLICKED(IDC_VIDEO_CHECK_SHOWINFO, &CDlg_Video::OnBnClickedCheck_Showinfo)
	//ON_BN_CLICKED(IDC_VIDEO_BUTTON_CHOSE, &CDlg_Video::OnBnClickedButton_Chose)
	//ON_BN_CLICKED(IDC_VIDEO_BUTTON_UPLOAD, &CDlg_Video::OnBnClickedButton_Upload)
	ON_BN_CLICKED(IDC_VIDEO_BUTTON_CAPTURE, &CDlg_Video::OnBnClickedButton_Capture)
	//ON_BN_CLICKED(IDC_VIDEO_BUTTON_BASE64CAP, &CDlg_Video::OnClickedButton_Base64cap)
	ON_BN_CLICKED(IDC_VIDEO_BUTTON_BARCODECAP, &CDlg_Video::OnClickedButton_Barcodecap)
	//ON_BN_CLICKED(IDC_VIDEO_BUTTON_PDFCAP, &CDlg_Video::OnClickedButton_Pdfcap)
	ON_BN_CLICKED(IDC_VIDEO_BUTTON_SERCAP, &CDlg_Video::OnClickedButton_Sercap)
	ON_BN_CLICKED(IDC_VIDEO_BUTTON_ROTLEFT, &CDlg_Video::OnBnClickedButton_Rotleft)
	ON_BN_CLICKED(IDC_VIDEO_BUTTON_ROTRIGHT, &CDlg_Video::OnBnClickedButton_Rotright)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_VIDEO_SLIDER_EXP, &CDlg_Video::OnNMCustomdrawSlider_Exp)

	ON_BN_CLICKED(IDOK, &CDlg_Video::OnOk)
	ON_BN_CLICKED(IDC_VIDEO_BUTTON_DPI, &CDlg_Video::OnBnClickedButton_Dpi)
	//ON_BN_CLICKED(IDC_VIDEO_CHECK_PLAYSOUND, &CDlg_Video::OnBnClickedCheck_Playsound)
	ON_WM_CTLCOLOR()
	ON_WM_NCHITTEST()
	ON_BN_CLICKED(IDC_VIDEO_CLOSE, &CDlg_Video::OnVideo_Close)
	ON_WM_PAINT()
	ON_COMMAND(ID_IMAGE_DELETE, &CDlg_Video::OnImageDelete)
	ON_COMMAND(ID_IMAGE_LEFT90, &CDlg_Video::OnImageLeft90)
	ON_COMMAND(ID_IMAGE_RIGHT90, &CDlg_Video::OnImageRight90)
	ON_COMMAND(ID_IMAGE_FLIPVERTICAL, &CDlg_Video::OnImageFlipvertical)
	ON_COMMAND(ID_IMAGE_MIRROR, &CDlg_Video::OnImageMirror)
	ON_NOTIFY(NM_RCLICK, IDC_VIDEO_LIST_THUMBNAIL, &CDlg_Video::OnNMRClickVideoListThumbnail)
END_MESSAGE_MAP()


// CDlg_Video ��Ϣ�������


BOOL CDlg_Video::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	//CWnd *pWnd;
	//pWnd = GetDlgItem( IDC_VIDEO_BUTTON_ROTLEFT ); //��ȡ�ؼ�ָ�룬IDC_BUTTON1Ϊ�ؼ�ID��
	//pWnd->SetWindowPos( NULL,0,0,40,40,SWP_NOZORDER | SWP_NOMOVE ); //�ѱ༭�ؼ��Ĵ�С��Ϊ(100,80)��λ�ò���

	AddPngToControl();
	ReadSetting();
	CenterWindow();  // ���ھ���
	OcxInit();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

BOOL CDlg_Video::OcxInit()
{
	long _ret = 0;
	//m_nRotateType = 0;
	_ret = m_ocx.Initial();
	if (_ret != 0)
	{
		MessageBox(TEXT("�ؼ���ʼ��ʧ�ܣ�"));
		SendMessage(WM_CLOSE);
		return FALSE;
	}
	//::MessageBox(NULL,TEXT("�ؼ���ʼ���ɹ�!"),MB_CAPTION,MB_OK);
	int count = m_ocx.GetDeviceCount();	
	if (count > 0)
	{
		bool bFound = false;
		m_CombDev.ResetContent();
		for (int i = 0 ; i < count ; i++)
		{
			CString strName = m_ocx.GetDeviceName(i);
			if (strName == m_sCameraName)
			{
				m_nDevIndex = i;
				bFound = true;
			}
			m_CombDev.InsertString(i, strName);
		}

		if (false == bFound) // ��Ϊ���������ļ�������ͷ����������һ������ͷ
		{
			m_nDevIndex = 0;
		}
		m_CombDev.SetCurSel(m_nDevIndex);
		m_ocx.StartRun(m_nDevIndex);
		m_sCameraName = m_ocx.GetDeviceName(m_nDevIndex);

	}
	else
	{
		m_ocx.Uninitial();
		MessageBox(TEXT("δ�ҵ���Ƶ�豸"));
		SendMessage(WM_CLOSE);
	}
	//m_nFileType = 0;
	//m_combFiletype.SetCurSel(m_nFileType);
	long value = 0,autoexp = 0,expmin = 0,expmax = 0;
	_ret = m_ocx.GetExposureRange(&expmin,&expmax);
	if (_ret != 0)
	{
		m_sliderExp.EnableWindow(FALSE);
		m_checkAutoExp.EnableWindow(FALSE);
		//m_editExp.EnableWindow(FALSE);
		GetDlgItem(IDC_VIDEO_STATIC_EXP)->EnableWindow(FALSE);
	}
	else
	{
		m_ocx.GetExposure(&value,&autoexp);
		m_checkAutoExp.SetCheck(autoexp);
	}
	if (!autoexp)
	{
		m_sliderExp.EnableWindow(TRUE);
		GetDlgItem(IDC_VIDEO_STATIC_EXP)->EnableWindow(TRUE);
		//m_editExp.EnableWindow(TRUE);
	}
	m_sliderExp.SetRange(expmin,expmax);
	m_sliderExp.SetPos(value);
	CString str;
	str.Format(TEXT("%d"),value);
	RefreshCStaticControl(IDC_VIDEO_STATIC_EXP, str);
	//m_editExp.SetWindowText(str);
	//m_editURL.SetWindowText(TEXT("http://192.168.0.200:15972/HFS_Recived/"));


	// ͼ������ 
	int nCapIndex;
	const IntVector* lstCapValues;
	const FloatVector* lstCapValuesFlt;
	m_CombColor.ResetContent();
	nCapIndex = m_pUI->GetCurrentCapIndex(ICAP_PIXELTYPE);
	lstCapValues = m_pUI->GetValidCap(ICAP_PIXELTYPE);
	for(unsigned int i=0; i<lstCapValues->size();i++)
	{
		switch(lstCapValues->at(i))
		{
		case TWPT_BW:
			m_CombColor.InsertString(i,"�ڰ�");
			break;
		case TWPT_GRAY:
			m_CombColor.InsertString(i,"�Ҷ�");
			break;
		case TWPT_RGB:
			m_CombColor.InsertString(i,"��ɫ");
			break;
		default:
			continue;
		}
	}
	m_nPixelType = 2 - nCapIndex;
	m_CombColor.SetCurSel(2 - m_nPixelType); // 0-�ڰ� 1-�Ҷ� 2-��ɫ����TWAIN��ͬ��
	m_ocx.SetColorMode(m_nPixelType);    // 0-��ɫ 1-�Ҷ� 2-�ڰ�


	// �ֱ���
	SwitchToDPIorRes(TRUE);

  //�Զ�����
	m_CheckAC.SetCheck(m_bAutoCrop);
	//m_CheckMSG.SetCheck(m_bShowInfo);
	//m_checkPlaySound.SetCheck(m_bPlaySound);
	OnBnClickedCheck_Autocrop();
	//OnBnClickedCheck_Showinfo();
	
	if (!PathIsDirectory(m_strTempPath))
	{
		CreateDirectory(m_strTempPath, NULL);
	}

	CString msg;
	msg.Format("%d ��", m_nFileCount);
	RefreshCStaticControl(IDC_VIDEO_STATIC_CAPTURECOUNT, msg);
	//m_sCaptureCount.ShowWindow(SW_HIDE);
	//m_sCaptureCount.SetWindowText(msg);
	//m_sCaptureCount.ShowWindow(SW_SHOW);

	m_bInitialend = TRUE;

	return TRUE;
}


void CDlg_Video::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	m_ocx.StopRun();
	m_ocx.Uninitial();
	m_pUI->Cancel();
	CDialogEx::OnClose();
}


void CDlg_Video::OnCbnSelchangeCombo_Dev()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int index = m_CombDev.GetCurSel();
	if (index == m_nDevIndex)
	{
		return;
	}
	m_nDevIndex = index;
	m_sCameraName = m_ocx.GetDeviceName(index);
	m_ocx.StopRun();
	m_ocx.StartRun(m_nDevIndex);
	//m_CombReso.ResetContent();
	//int resCount = m_ocx.GetResCount();
	//for (int i = 0 ; i < resCount ; i++)
	//{
	//	m_CombReso.InsertString(i, m_ocx.GetResolution(i));
	//}
	//m_nResIndex = m_ocx.GetCurResoIndex();
	//m_CombReso.SetCurSel(m_nResIndex);

	SwitchToDPIorRes(false);
	SwitchToDPIorRes(true);

	long value = 0,autoexp = 0,expmin = 0,expmax = 0;
	long _ret = m_ocx.GetExposureRange(&expmin,&expmax);
	if (_ret != 0)
	{
		m_sliderExp.EnableWindow(FALSE);
		m_checkAutoExp.EnableWindow(FALSE);
		GetDlgItem(IDC_VIDEO_STATIC_EXP)->EnableWindow(FALSE);
		//m_editExp.EnableWindow(FALSE);
	}
	else
	{
		m_sliderExp.EnableWindow(TRUE);
		m_checkAutoExp.EnableWindow(TRUE);
		GetDlgItem(IDC_VIDEO_STATIC_EXP)->EnableWindow(TRUE);
		//m_editExp.EnableWindow(TRUE);
		m_ocx.GetExposure(&value,&autoexp);
		m_checkAutoExp.SetCheck(autoexp);
	}
	if (!autoexp)
	{
		m_sliderExp.EnableWindow(TRUE);
		GetDlgItem(IDC_VIDEO_STATIC_EXP)->EnableWindow(TRUE);
		//m_editExp.EnableWindow(TRUE);
	}
	m_sliderExp.SetRange(expmin,expmax);
	m_sliderExp.SetPos(value);
}


void CDlg_Video::OnCbnSelchangeCombo_Reso()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int idx = m_CombReso.GetCurSel();
	if (true == m_bIsDPI)
	{
		//m_nDPIIndex = idx;
		CString strCBText; 
		m_CombReso.GetLBText(idx, strCBText);
		m_nDPI = _ttoi(strCBText);  // CString ת int
	} 
	else
	{	
		if (idx == m_nResIndex)
		{
			return;
		}
		m_nResIndex = idx;
		m_ocx.SetResolution(m_nResIndex);
	}
	
}


void CDlg_Video::OnCbnSelchangeCombo_Color()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_nPixelType = 2 - m_CombColor.GetCurSel();
	m_ocx.SetColorMode(m_nPixelType);
}


//void CDlg_Video::OnCbnSelchangeCombo_Filetype()
//{
//	// TODO: �ڴ���ӿؼ�֪ͨ����������
//	m_nFileType = m_combFiletype.GetCurSel();
//}


void CDlg_Video::OnBnClickedCheck_Autoexp()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_checkAutoExp.GetCheck())
	{ 
		m_sliderExp.EnableWindow(FALSE);
		m_ocx.SetAutoExposure(TRUE);
		//::MessageBox(NULL,TEXT("ѡ��!"),MB_CAPTION,MB_OK);		
	}
	else
	{
		m_sliderExp.EnableWindow(TRUE);
		int value = m_sliderExp.GetPos();
		m_ocx.SetAutoExposure(FALSE);
		m_ocx.SetExposure(value,0);
		//::MessageBox(NULL,TEXT("δѡ��!"),MB_CAPTION,MB_OK);		
	}
}


void CDlg_Video::OnBnClickedCheck_Autocrop()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_CheckAC.GetCheck())
	{
		if (m_checkManual.GetCheck())
		{
			m_checkManual.SetCheck(0);
		}
		m_ocx.AdjuestImageCrop(TRUE);
		m_bAutoCrop = true;
	} 
	else
	{
		m_ocx.AdjuestImageCrop(FALSE);
		m_bAutoCrop = false;
	}
}


void CDlg_Video::OnBnClickedCheck_Manual()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_checkManual.GetCheck())
	{
		if (m_CheckAC.GetCheck())
		{
			m_CheckAC.SetCheck(0);
		}
		m_ocx.ManualImageCrop(TRUE);
	} 
	else
	{
		m_ocx.ManualImageCrop(FALSE);
	}
}


//void CDlg_Video::OnBnClickedCheck_Showinfo()
//{
//	// TODO: �ڴ���ӿؼ�֪ͨ����������
//	if (m_CheckMSG.GetCheck())
//	{
//		m_ocx.ShowVideoInfo(TRUE);
//		m_bShowInfo = true;
//	} 
//	else
//	{
//		m_ocx.ShowVideoInfo(FALSE);
//		m_bShowInfo = false;
//	}
//}


//void CDlg_Video::OnBnClickedButton_Chose()
//{
//	// TODO: �ڴ���ӿؼ�֪ͨ����������
//	CString m_FileDir;
//	BROWSEINFO bi;
//	ZeroMemory(&bi, sizeof(BROWSEINFO));
//	bi.hwndOwner = this->GetSafeHwnd();
//	bi.ulFlags   = BIF_RETURNONLYFSDIRS;
//	LPITEMIDLIST pidl = SHBrowseForFolder(&bi);
//	BOOL bRet = FALSE;
//	TCHAR szFolder[MAX_PATH*2];
//	szFolder[0] = ' ';
//	if (pidl)
//	{
//		if (SHGetPathFromIDList(pidl, szFolder))  
//			bRet = TRUE;
//		IMalloc *pMalloc = NULL;
//		if (SUCCEEDED(SHGetMalloc(&pMalloc)) && pMalloc)
//		{ 
//			pMalloc->Free(pidl); 
//			pMalloc->Release();
//		}
//	}
//
//	m_FileDir = szFolder;
//	if(m_FileDir.GetLength() > 0)	
//		m_editPath.SetWindowText(m_FileDir);
//}


//void CDlg_Video::OnBnClickedButton_Upload()
//{
//	// TODO: �ڴ���ӿؼ�֪ͨ����������
//	if (m_sCaptureName == TEXT(""))
//	{
//		MessageBox(TEXT("δ�����ļ�"));
//	}
//	else
//	{
//		CString str;
//		m_editURL.GetWindowText(str);
//		m_ocx.HTTPUpload(str, m_sCaptureName, TRUE);
//	}
//}


void CDlg_Video::OnBnClickedButton_Capture()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	//CString path = TEXT("");
	//m_editPath.GetWindowText(path);
	if (TEXT("") == m_strTempPath)
	{
		MessageBox(TEXT("��δѡ��洢λ�ã�"));
		return;
	}
	CString _subfix = TEXT("");
	switch (m_nFileType)
	{
	case 0:
		_subfix = TEXT(".jpg");
		break;
	case 1:
		_subfix = TEXT(".bmp");
		break;
	case 2:
		_subfix = TEXT(".png");
		break;
	case 3:
		_subfix = TEXT(".tif");
		break;
	default:
		_subfix = TEXT(".jpg");
		break;
	}
	CTime tt = CTime::GetCurrentTime();
	SYSTEMTIME t1;
	GetSystemTime(&t1) ; 
	int MillSec = (int)t1.wMilliseconds/100;
	CString str;
	str.Format(TEXT("%d"), MillSec);
	m_sCaptureName = m_strTempPath + TEXT("\\") + tt.Format("%Y_%m_%d_%H_%M_%S_") 
		+ str + _subfix;
	int retval = m_ocx.CaptureImage(m_sCaptureName);  // �ɹ���0

	g_vector_imagepath.push_back(m_sCaptureName.GetBuffer());

	//if (m_checkPlaySound.GetCheck())
	//{
	//	MyPlaySound(FILENAME_WAV_CAMERA1);
	//}

	m_nFileCount ++;
	CString msg;
	msg.Format("%d ��", m_nFileCount);
	RefreshCStaticControl(IDC_VIDEO_STATIC_CAPTURECOUNT, msg);

	SaveThumbNail(m_sCaptureName);
	LoadThumbNail();

}


//void CDlg_Video::OnClickedButton_Base64cap()
//{
//	// TODO: �ڴ���ӿؼ�֪ͨ����������
//	CString path = TEXT("");
//	m_editPath.GetWindowText(path);
//	if (TEXT("") == path)
//	{
//		MessageBox(TEXT("��δѡ��洢λ�ã�"));
//		return;
//	}
//	CString str;
//	str = m_ocx.CaptureBase64();
//	CString base64FileName = path + TEXT("\\base64.txt");
//	try
//	{
//		//�����ļ��Ĵ򿪲���;
//		CStdioFile outFile(base64FileName, CFile::modeNoTruncate |
//			CFile::modeCreate | CFile::modeWrite | CFile::typeText);
//		//���ļ�ĩβ�����¼�¼;
//		outFile.SeekToEnd();
//		outFile.WriteString( str );
//		outFile.Close();
//		m_nFileCount ++;
//		CString msg;
//		msg.Format("������: %d��", m_nFileCount);
//		m_sCaptureCount.SetWindowText(msg);
//	}
//	catch(CFileException *fx)
//	{
//		fx->Delete();
//	}
//}


void CDlg_Video::OnClickedButton_Barcodecap()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//CString path = TEXT("");
	//m_editPath.GetWindowText(path);
	if (TEXT("") == m_strTempPath)
	{
		MessageBox(TEXT("��δѡ��洢λ�ã�"));
		return;
	}

	m_ocx.CaptureBarcode(m_strTempPath, m_nFileType);
	//if (m_checkPlaySound.GetCheck())
	//{
	//	MyPlaySound(FILENAME_WAV_CAMERA1);
	//}

	m_nFileCount ++;
	CString msg;
	msg.Format("%d ��", m_nFileCount);
	RefreshCStaticControl(IDC_VIDEO_STATIC_CAPTURECOUNT, msg);
	//m_sCaptureCount.ShowWindow(SW_HIDE);
	//m_sCaptureCount.SetWindowText(msg);
	//m_sCaptureCount.ShowWindow(SW_SHOW);
}


//void CDlg_Video::OnClickedButton_Pdfcap()
//{
//	// TODO: �ڴ���ӿؼ�֪ͨ����������
//	CString path = TEXT("");
//	m_editPath.GetWindowText(path);
//	if (TEXT("") == path)
//	{
//		MessageBox(TEXT("��δѡ��洢λ�ã�"));
//		return;
//	}
//
//	CTime tt = CTime::GetCurrentTime();
//	SYSTEMTIME t1;
//	GetSystemTime(&t1) ; 
//	int MillSec = (int)t1.wMilliseconds/100;
//	CString str;
//	str.Format(TEXT("%d"), MillSec);
//	m_sCaptureName = path + TEXT("\\") + tt.Format("%Y_%m_%d_%H_%M_%S_") 
//		+ str + TEXT(".pdf");
//	m_ocx.CapturePDF(m_sCaptureName);
//
//	m_nFileCount ++;
//	CString msg;
//	msg.Format("������: %d��", m_nFileCount);
//	m_sCaptureCount.SetWindowText(msg);
//}


void CDlg_Video::OnClickedButton_Sercap()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (TEXT("") == m_strTempPath)
	{
		MessageBox(TEXT("��δѡ��洢λ�ã�"));
		return;
	}
	m_checkManual.SetCheck(0);
	if (!m_CheckAC.GetCheck())
	{
		m_CheckAC.SetCheck(1);
		m_bAutoCrop = true;
	}
	m_ocx.AutomaticCapture(m_strTempPath,m_nFileType);
	//if (m_checkPlaySound.GetCheck())
	//{
	//	MyPlaySound(FILENAME_WAV_CAMERA1);
	//}
}


void CDlg_Video::OnBnClickedButton_Rotleft()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_nRotateType = (m_nRotateType + 3) % 4;
	m_ocx.RotatedVideo(m_nRotateType);
}


void CDlg_Video::OnBnClickedButton_Rotright()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_nRotateType = (m_nRotateType + 1) % 4;
	m_ocx.RotatedVideo(m_nRotateType);
}


void CDlg_Video::OnNMCustomdrawSlider_Exp(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_bInitialend)
	{
		int pos = ((CSliderCtrl *)GetDlgItem(IDC_VIDEO_SLIDER_EXP))->GetPos();
		if (!m_checkAutoExp.GetCheck())
		{
			m_ocx.SetExposure(pos,0);
			CString str;
			str.Format(TEXT("%d"), pos);
			RefreshCStaticControl(IDC_VIDEO_STATIC_EXP, str);
			//m_editExp.SetWindowText(str);
		}
	}

	*pResult = 0;
}BEGIN_EVENTSINK_MAP(CDlg_Video, CDialogEx)
	ON_EVENT(CDlg_Video, IDC_VIDEO_UDS_VIDEOCTRL, 1, CDlg_Video::GetBarcodeString_VideoUdsVideoctrl, VTS_BSTR)
	ON_EVENT(CDlg_Video, IDC_VIDEO_UDS_VIDEOCTRL, 2, CDlg_Video::GetAutoCapFileName_VideoUdsVideoctrl, VTS_BSTR)
	ON_EVENT(CDlg_Video, IDC_VIDEO_UDS_VIDEOCTRL, 3, CDlg_Video::DeviceChanged_VideoUdsVideoctrl, VTS_BSTR VTS_BSTR)
	END_EVENTSINK_MAP()


void CDlg_Video::GetBarcodeString_VideoUdsVideoctrl(LPCTSTR Barcode)
{
	// TODO: �ڴ˴������Ϣ����������
	//m_richeditMSG.ReplaceSel(TEXT("���룺"));
	//m_richeditMSG.ReplaceSel(Barcode);
	//m_richeditMSG.ReplaceSel(TEXT("\n"));
	//m_richeditMSG.PostMessage(WM_VSCROLL, SB_BOTTOM, 0);

	TCHAR* szExt = TEXT("");
	switch (m_nFileType)
	{
	case 0:
		szExt = TEXT(".jpg");
		break;
	case 1:
		szExt = TEXT(".bmp");
		break;
	case 2:
		szExt = TEXT(".png");
		break;
	case 3:
		szExt = TEXT(".tif");
		break;
	default:
		break;
	}

	//CString path = TEXT("");
	//m_editPath.GetWindowText(path);
	CString strFileName;
	strFileName = m_strTempPath + TEXT("\\") + Barcode + szExt;
	g_vector_imagepath.push_back(strFileName.GetBuffer());
}


void CDlg_Video::GetAutoCapFileName_VideoUdsVideoctrl(LPCTSTR fileName)
{
	// TODO: �ڴ˴������Ϣ����������
	//m_richeditMSG.ReplaceSel(fileName);
	//m_richeditMSG.ReplaceSel(TEXT("\n"));
	//m_richeditMSG.PostMessage(WM_VSCROLL, SB_BOTTOM, 0);

	m_nFileCount ++;
	CString msg;
	msg.Format("%d��", m_nFileCount);
	RefreshCStaticControl(IDC_VIDEO_STATIC_CAPTURECOUNT, msg);
	//m_sCaptureCount.ShowWindow(SW_HIDE);
	//m_sCaptureCount.SetWindowText(msg);
	//m_sCaptureCount.ShowWindow(SW_SHOW);
	g_vector_imagepath.push_back(fileName);
	SaveThumbNail(fileName);
	LoadThumbNail();
}


void CDlg_Video::DeviceChanged_VideoUdsVideoctrl(LPCTSTR changeType, LPCTSTR deviceName)
{
	// TODO: �ڴ˴������Ϣ����������
	//m_richeditMSG.ReplaceSel(changeType);
	//m_richeditMSG.ReplaceSel(TEXT(" - "));
	//m_richeditMSG.ReplaceSel(deviceName);
	//m_richeditMSG.ReplaceSel(TEXT("\n"));
	//m_richeditMSG.PostMessage(WM_VSCROLL, SB_BOTTOM, 0);
}

void CDlg_Video::SetCapValue(void)
{
	m_pUI->SetCapValueFloat(UDSCAP_DOCS_IN_ADF, (float)m_nFileCount);  // ���ô���ͼƬ����
	m_pUI->SetCapValueInt(ICAP_PIXELTYPE, (2 - m_nPixelType));  // ����ͼƬ���� 
	m_pUI->SetCapValueInt(ICAP_XRESOLUTION, m_nDPI);  // ����ˮƽ����DPI
	m_pUI->SetCapValueInt(ICAP_YRESOLUTION, m_nDPI);  // ���ô�ֱ����DPI

	//CString strCBText; 
	//m_CombReso.GetLBText(m_nDPIIndex, strCBText);
	//int nDPI = _ttoi(strCBText);  // CString ת int
	//m_pUI->SetCapValueInt(ICAP_XRESOLUTION, nDPI);  // ����ˮƽ����DPI
	//m_pUI->SetCapValueInt(ICAP_YRESOLUTION, nDPI);  // ���ô�ֱ����DPI


}


void CDlg_Video::OnOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_ocx.StopRun();
	m_ocx.Uninitial();
	if (0 == m_nFileCount)  // һ�Ŷ�û��ȡ��ɨ��
	{
		m_pUI->Cancel();
		CDialogEx::OnClose();
		return;
	}

	SetCapValue();		
	WriteSetting();
	m_pUI->Scan();
	CDialogEx::OnOK();
}


void CDlg_Video::OnBnClickedButton_Dpi()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//m_button_DPI.SetWindowText(TEXT("�ֱ���"));
	CString strText;
	strText = m_button_DPI.strCaption;
	//m_button_DPI.GetWindowText(strText);

	if( strText.Find(TEXT("DPI")) >=0 )
	{
		SwitchToDPIorRes(false);
	}
	else
	{
		SwitchToDPIorRes(true);
	}

}

void CDlg_Video::SwitchToDPIorRes(bool bisDPI)
{
	if( false == bisDPI )
	{
		//m_button_DPI.SetWindowText(TEXT("�ֱ���"));
		m_button_DPI.strCaption = TEXT("�ֱ���");
		m_bIsDPI = false;

		int resCount = m_ocx.GetResCount();
		m_CombReso.ResetContent();
		for (int i = 0 ; i < resCount ; i++)
		{
			m_CombReso.InsertString(i, m_ocx.GetResolution(i));
		}
		m_nResIndex = m_ocx.GetCurResoIndex();
		m_CombReso.SetCurSel(m_nResIndex);
	}
	else
	{
		//m_button_DPI.SetWindowText(TEXT("DPI"));
		m_button_DPI.strCaption = TEXT("DPI");
		m_bIsDPI = true;
		int nCapIndex;
		const FloatVector* lstCapValuesFlt;
		// �ֱ���
		m_CombReso.ResetContent();
		nCapIndex = m_pUI->GetCurrentCapIndex(ICAP_XRESOLUTION);
		lstCapValuesFlt = m_pUI->GetValidCapFloat(ICAP_XRESOLUTION);
		for(unsigned int i=0; i<lstCapValuesFlt->size();i++)
		{
			CString strTemp;
			strTemp.Format(TEXT("%d"), (int)lstCapValuesFlt->at(i));
			m_CombReso.InsertString(i,strTemp);
		}
		m_CombReso.SetCurSel(nCapIndex);
		//m_CombReso.SetCurSel(m_nDPIIndex);

		CString strCBText; 
		m_CombReso.GetLBText(nCapIndex, strCBText);
		m_nDPI = _ttoi(strCBText);  // CString ת int
	}
}


void CDlg_Video::ReadSetting()
{
	TCHAR szINIPath[MAX_PATH];  // INI�ļ�·��

	GetFilePath(FILENAME_INI,szINIPath);

	INI_VIDEO tempINI;

	//int
	//tempINI.DpiIndex     = GetPrivateProfileInt(INI_APP_CAMERASETTING, INI_KEY_DPIINDEX,    0, szINIPath);
	//tempINI.PixelType    = GetPrivateProfileInt(INI_APP_CAMERASETTING, INI_KEY_PIXELTYPE,  0, szINIPath);

	CString strTemp;
	int nMaxLength = 512;

	//bool
	GetPrivateProfileString(INI_APP_CAMERASETTING,INI_KEY_AUTOCROP,TEXT(""),strTemp.GetBufferSetLength(nMaxLength),nMaxLength,szINIPath);
	if (strTemp.Find(TEXT("Y")) >= 0) {
		tempINI.AutoCrop = true;
	} 
	else {
		tempINI.AutoCrop = false;
	}

	//GetPrivateProfileString(INI_APP_CAMERASETTING,INI_KEY_SHOWINFO,TEXT(""),strTemp.GetBufferSetLength(nMaxLength),nMaxLength,szINIPath);
	//if (strTemp.Find(TEXT("Y")) >= 0) {
	//	tempINI.ShowInfo = true;
	//} 
	//else {
	//	tempINI.ShowInfo = false;
	//}

	//GetPrivateProfileString(INI_APP_CAMERASETTING,INI_KEY_PLAYSOUND,TEXT(""),strTemp.GetBufferSetLength(nMaxLength),nMaxLength,szINIPath);
	//if (strTemp.Find(TEXT("Y")) >= 0) {
	//	tempINI.playSound = true;
	//} 
	//else {
	//	tempINI.playSound = false;
	//}

	//CString
	GetPrivateProfileString(INI_APP_CAMERASETTING,INI_KEY_CAMERANAME,TEXT(""),strTemp.GetBufferSetLength(nMaxLength),nMaxLength,szINIPath);
 
	//m_nDevIndex  = tempINI.CameraIndex;
	m_sCameraName = strTemp;
	//m_nDPIIndex  = tempINI.DpiIndex;
	//m_nPixelType = tempINI.PixelType;
	m_bAutoCrop  = tempINI.AutoCrop;
	//m_bShowInfo  = tempINI.ShowInfo;
	//m_bPlaySound = tempINI.playSound;

	strTemp.ReleaseBuffer(nMaxLength);
}


void CDlg_Video::WriteSetting()
{
	TCHAR szINIPath[MAX_PATH];  // INI�ļ�·��
	GetFilePath(FILENAME_INI,szINIPath);  // ��ȡINI�ļ�·��

	//CString
	CString strTemp;
	strTemp = m_sCameraName;
	WritePrivateProfileString(INI_APP_CAMERASETTING,INI_KEY_CAMERANAME,strTemp,szINIPath);

	//int->CString
	//strTemp.Format(TEXT("%d"), m_nDevIndex);
	//WritePrivateProfileString(INI_APP_CAMERASETTING,INI_KEY_CAMERAINDEX,strTemp,szINIPath);

	//strTemp.Format(TEXT("%d"), m_nDPIIndex);
	//WritePrivateProfileString(INI_APP_CAMERASETTING,INI_KEY_DPIINDEX,strTemp,szINIPath);

	//strTemp.Format(TEXT("%d"), m_nPixelType);
	//WritePrivateProfileString(INI_APP_CAMERASETTING,INI_KEY_PIXELTYPE,strTemp,szINIPath);

	//bool->CString
	if (m_bAutoCrop) {
		strTemp = TEXT("Y");
	} 
	else {
		strTemp = TEXT("N");
	}
	WritePrivateProfileString(INI_APP_CAMERASETTING,INI_KEY_AUTOCROP,strTemp,szINIPath);

	//if (m_bShowInfo) {
	//	strTemp = TEXT("Y");
	//} 
	//else {
	//	strTemp = TEXT("N");
	//}
	//WritePrivateProfileString(INI_APP_CAMERASETTING,INI_KEY_SHOWINFO,strTemp,szINIPath);

	//if (m_bPlaySound) {
	//	strTemp = TEXT("Y");
	//} 
	//else {
	//	strTemp = TEXT("N");
	//}
	//WritePrivateProfileString(INI_APP_CAMERASETTING,INI_KEY_PLAYSOUND,strTemp,szINIPath);

}

//void CDlg_Video::OnBnClickedCheck_Playsound()
//{
//	// TODO: �ڴ���ӿؼ�֪ͨ����������
//	m_checkPlaySound.GetCheck() > 0 ? m_bPlaySound = true:m_bPlaySound = false;
//}

//void CDlg_Video::MyPlaySound(TCHAR* _szFileName)
//{
//	TCHAR szFilePath[MAX_PATH];
//	GetFilePath(_szFileName, szFilePath);
//	sndPlaySound(szFilePath, SND_ASYNC);
//}


HBRUSH CDlg_Video::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	CFont font1, font2/*, font3, font4, font5*/;
	font1.CreatePointFont(90, _T("΢���ź�"));
	font2.CreatePointFont(110, _T("΢���ź�"));
	//font3.CreatePointFont(100, _T("΢���ź�"));
	//font4.CreatePointFont(140, _T("΢���ź�"));
	//font5.CreatePointFont(90, _T("΢���ź�"));
	// TODO:  Change any attributes of the DC here

	switch (pWnd->GetDlgCtrlID())
	{
	case IDC_STATIC:
	case IDC_VIDEO_STATIC_EXP:
	case IDC_VIDEO_LIST_THUMBNAIL:
		{
			pDC->SetBkMode(TRANSPARENT);
			//pDC->SetTextColor(RGB(124, 153, 163));
			pDC->SetTextColor(RGB(2, 232, 0));  // ����ɫ
			//pDC->SetTextColor(RGB(129, 42, 142));  // ��ɫ
			//pDC->SetTextColor(RGB(55, 141, 0));  // ����ɫ
			pDC->SelectObject(&font1);
			return (HBRUSH)GetStockObject(HOLLOW_BRUSH);
		}
		break;
	case IDC_VIDEO_STATIC_CAPTURECOUNT:
		{
			pDC->SetBkMode(TRANSPARENT);
			//pDC->SetTextColor(RGB(124, 153, 163));
			//pDC->SetTextColor(RGB(2, 232, 0));  // ����ɫ
			//pDC->SetTextColor(RGB(129, 42, 142));  // ��ɫ
			//pDC->SetTextColor(RGB(55, 141, 0));  // ����ɫ
			pDC->SetTextColor(RGB(255, 0, 0));
			pDC->SelectObject(&font1);
			return (HBRUSH)GetStockObject(HOLLOW_BRUSH);
		}
		break;
	case IDC_VIDEO_BUTTON_ROTLEFT:
	case IDC_VIDEO_BUTTON_ROTRIGHT:
	case IDC_VIDEO_CHECK_AUTOCROP:
	case IDC_VIDEO_CHECK_AUTOEXP:
	case IDC_VIDEO_CHECK_MANUAL:
	case IDC_VIDEO_CLOSE:
	case IDC_VIDEO_BUTTON_SERCAP:
	case IDC_VIDEO_BUTTON_CAPTURE:
	case IDC_VIDEO_BUTTON_UPLOAD:
	case IDC_VIDEO_BUTTON_DPI:
	case IDOK:
	//case IDC_VIDEO_SLIDER_EXP:
		{
			pDC->SetTextColor(RGB(255, 255, 255));
			//pDC->SetTextColor(RGB(44, 173, 219));
			//pDC->SetTextColor(RGB(129, 42, 142));
			pDC->SelectObject(&font2);
			pDC->SetBkMode(TRANSPARENT);
			return (HBRUSH)GetStockObject(HOLLOW_BRUSH);
		}
		break;
	default: 
		break;
					 
	}
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}

// �ޱ������ƶ�
LRESULT CDlg_Video::OnNcHitTest(CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	// ȡ��������ڵĴ�������
	UINT nHitTest = CDialog::OnNcHitTest(point);
	// �������ڴ��ڿͻ������򷵻ر��������Ÿ�Windows
	// ʹWindows������ڱ�����������д������ɵ����ƶ�����
	return (nHitTest == HTCLIENT) ? HTCAPTION : nHitTest;
	//return CDialogEx::OnNcHitTest(point);
}


void CDlg_Video::OnVideo_Close()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnClose();
}

bool CDlg_Video::LoadImageFromResource(IN CImage* pImage, IN UINT nResID, IN LPCTSTR lpTyp)
{
	if (pImage == NULL) return false;
	pImage->Destroy();

	// ������Դ
	HRSRC hRsrc = ::FindResource(AfxGetResourceHandle(), MAKEINTRESOURCE(nResID), lpTyp);
	if (hRsrc == NULL) {
		return false; }

	// ������Դ
	HGLOBAL hImgData = ::LoadResource(AfxGetResourceHandle(), hRsrc);
	if (hImgData == NULL) {
		::FreeResource(hImgData);
		return false;
	}

	// �����ڴ��е�ָ����Դ
	LPVOID lpVoid = ::LockResource(hImgData);
	LPSTREAM pStream = NULL;
	DWORD dwSize = ::SizeofResource(AfxGetResourceHandle(), hRsrc);
	HGLOBAL hNew = ::GlobalAlloc(GHND, dwSize);
	LPBYTE lpByte = (LPBYTE)::GlobalLock(hNew);
	::memcpy(lpByte, lpVoid, dwSize);
	// ����ڴ��е�ָ����Դ
	::GlobalUnlock(hNew);
	// ��ָ���ڴ洴��������
	HRESULT ht = ::CreateStreamOnHGlobal(hNew, TRUE, &pStream);
	if (ht != S_OK) {
		GlobalFree(hNew);
	} else {
		// ����ͼƬ
		pImage->Load(pStream);
		GlobalFree(hNew);
	}

	// �ͷ���Դ
	::FreeResource(hImgData);

	return true;
}


void CDlg_Video::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	CRect rect;
	GetClientRect(&rect);
	if (!m_image_background.IsNull()) {
		m_image_background.Destroy();
	}
	LoadImageFromResource(&m_image_background, IDB_PNG_BK, TEXT("PNG"));
	if (!m_image_background.IsNull())
		m_image_background.TransparentBlt(dc.m_hDC, 0, 0, rect.Width(),
		rect.Height(), RGB(255, 255, 255));

	// ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
}

void CDlg_Video::AddPngToControl()
{

	m_button_rotleft.Init(IDB_PNG_ROTLEFT, BTN_IMG_4, 0, BTN_TYPE_NORMAL);
	m_button_rotright.Init(IDB_PNG_ROTRIGHT, BTN_IMG_4, 0, BTN_TYPE_NORMAL);
	m_button_close.Init(IDB_PNG_CLOSE, BTN_IMG_4, 0, BTN_TYPE_NORMAL);
	//m_button_ok.Init(IDB_PNG_UPLOAD, BTN_IMG_3, 0, BTN_TYPE_NORMAL);

	m_button_DPI.Init(IDB_PNG_BUTTON, BTN_IMG_4, 0, BTN_TYPE_NORMAL);
	m_button_DPI.strCaption = TEXT("DPI");

	m_button_ok.Init(IDB_PNG_BUTTON, BTN_IMG_4, 0, BTN_TYPE_NORMAL);
	m_button_ok.strCaption = TEXT("�ύ");

	m_button_selfcap.Init(IDB_PNG_BUTTON, BTN_IMG_4, 0, BTN_TYPE_NORMAL);
	m_button_selfcap.strCaption = TEXT("�Զ�����");

	m_button_capture.Init(IDB_PNG_BUTTON, BTN_IMG_4, 0, BTN_TYPE_NORMAL);
	m_button_capture.strCaption = TEXT("����");


	//m_sCaptureCount.SetCaption(TEXT("0 ��"));

	//m_checkAutoExp.Init(IDB_PNG_CHECKBOX6, BTN_IMG_6, 1, BTN_TYPE_NORMAL);
	//m_checkAutoExp.Init(IDB_PNG_CHECKBOX6, BTN_IMG_6);
	//m_checkManual.Init(IDB_PNG_CHECKBOX6, BTN_IMG_6);
	//m_CheckAC.Init(IDB_PNG_CHECKBOX6, BTN_IMG_6);


	CreateToolTipForRect(GetDlgItem(IDC_VIDEO_BUTTON_ROTLEFT)->m_hWnd, TEXT("��ʱ����ת90��"));
	CreateToolTipForRect(GetDlgItem(IDC_VIDEO_BUTTON_ROTRIGHT)->m_hWnd, TEXT("˳ʱ����ת90��"));
	CreateToolTipForRect(GetDlgItem(IDC_VIDEO_CLOSE)->m_hWnd, TEXT("���ύ���˳�"));
	CreateToolTipForRect(GetDlgItem(IDOK)->m_hWnd, TEXT("ȷ�����ύ"));
	CreateToolTipForRect(GetDlgItem(IDC_VIDEO_BUTTON_DPI)->m_hWnd, TEXT("����л�Ϊ�ֱ��ʻ�DPI"));

}


/*----------------------------------------------------------------------------- 
 Description: ������������ToolTip 
-----------------------------------------------------------------------------*/ 
void CDlg_Video::CreateToolTipForRect(HWND hwndParent, PTSTR pszText) 
{ 

    HWND hwndTT = CreateWindowEx(WS_EX_TOPMOST, 
                                 TOOLTIPS_CLASS, 
                                 NULL, 
                                 WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP, 
                                 CW_USEDEFAULT, 
                                 CW_USEDEFAULT, 
                                 CW_USEDEFAULT, 
                                 CW_USEDEFAULT, 
                                 hwndParent, 
                                 NULL, 
                                 AfxGetInstanceHandle(), 
                                 NULL); 
 
    ::SetWindowPos(hwndTT, 
                 HWND_TOPMOST, 
                 0, 0, 0, 0, 
                 SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE); 
 
    TOOLINFO ti = {0}; 
    ti.cbSize = sizeof(TOOLINFO); 
    ti.uFlags = TTF_SUBCLASS; 
    ti.hwnd = hwndParent; 
    ti.hinst = AfxGetInstanceHandle(); 
    ti.lpszText = pszText; 
 
    ::GetClientRect(hwndParent, &ti.rect); 
 
    ::SendMessage(hwndTT, TTM_ADDTOOL, 0, (LPARAM)(LPTOOLINFO)&ti); 
} 


bool CDlg_Video::ClearAndDeleteDir(const TCHAR* szPath, bool deleteDir/*= false*/)
{
	if ((NULL == szPath))
	{
		return false;
	}

	/*�������Ŀ¼�Ƿ��ǺϷ�Ŀ¼*/
	if (!PathIsDirectory(szPath))
	{
		return false;
	}

	/*����ԴĿ¼�в����ļ���ͨ���*/
	CString strWildcard(szPath);
	if (strWildcard.Right(1) != _T('\\'))
	{
		strWildcard += _T("\\");
	}
	strWildcard += _T("*.*");

	CFileFind ff;  
	BOOL bFound;  
	bFound = ff.FindFile(strWildcard);  

	/*char buf[60];
	itoa(bFound,buf,10);
	::MessageBox(NULL,TEXT(buf),MB_CAPTION,MB_OK);*/


	while(bFound)  
	{  
		bFound = ff.FindNextFile();  
		CString sFilePath = ff.GetFilePath();  
		//::MessageBox(NULL,TEXT(sFilePath),MB_CAPTION,MB_OK);
		if(ff.IsDirectory())  
		{  
			if(!ff.IsDots())  
			{  
				ClearAndDeleteDir(sFilePath);  
			}  
		}  
		else  
		{  
			if(ff.IsReadOnly())  
			{  
				SetFileAttributes(sFilePath, FILE_ATTRIBUTE_NORMAL);  
			}  
			DeleteFile(sFilePath); 
			//::MessageBox(NULL,TEXT("DeleteFile!"),MB_CAPTION,MB_OK);
		}  
	}  
	ff.Close(); 

	//�����Ѿ����ļ���������ļ�ȫ��ɾ���ˣ������Ҫ���ļ���Ҳɾ���������һ�´��� 
	if (true == deleteDir)
	{
		SetFileAttributes(szPath, FILE_ATTRIBUTE_NORMAL);  //�����ļ��е�����  
		RemoveDirectory(szPath);  //ɾ���ļ���  
	}
	
	return true;
}

void CDlg_Video::RefreshCStaticControl(int nID, LPCTSTR strText)
{
	GetDlgItem(nID)->ShowWindow(SW_HIDE);
	GetDlgItem(nID)->SetWindowText(strText);
	GetDlgItem(nID)->ShowWindow(SW_SHOW);
}

void CDlg_Video::SaveThumbNail(LPCTSTR strName)
{
	CString strPathName(strName);
	CxImage image;
	image.Load(strPathName);

	//if ( 0 == retval )  // Save file success: Generate Thumbnail JPG
	{
		float h_w = (float)image.GetHeight() / (float)image.GetWidth();
		image.Resample(THUMB_WIDTH, (int)(THUMB_WIDTH*h_w), 1, NULL);
		if ( image.GetBpp() < 24 )  // ����ͼȫתΪ24λ��ɫͼ������ΪJPG�ļ�
			image.IncreaseBpp(24);
		strPathName.Replace("bmp", "jpg");
		image.SetJpegQuality(70);  // compression
		image.Save(strPathName, CXIMAGE_FORMAT_JPG);
		strPathName.Replace("jpg", "bmp");
	}

}

void CDlg_Video::LoadThumbNail()
{
	DWORD dwStyle;
	dwStyle = m_listctrl.GetExtendedStyle();
	dwStyle = dwStyle|LVS_SHOWSELALWAYS|LVS_ALIGNTOP|LVS_ICON|LVS_AUTOARRANGE;
	m_listctrl.SetExtendedStyle(dwStyle);
	m_imagelist.Create(THUMB_WIDTH,THUMB_HEIGHT,ILC_COLOR24,0,1);
	m_listctrl.SetImageList(&m_imagelist,LVSIL_NORMAL);

	//remove m_imagelist
	for(int i=0; i<m_imagelist.GetImageCount(); i++) {
		m_imagelist.Remove(i);
	}

	//remove all items from list view
	m_listctrl.DeleteAllItems();

	//set Number of image for m_imagelist
	//m_imagelist.SetImageCount(g_vecCust_ImageInfo.size());
	m_imagelist.SetImageCount(g_vector_imagepath.size());

	//char path[MAX_PATH];
	std::vector<std::string>::iterator iter;
	//std::vector<CUST_IMAGEINFO>::iterator iter;

	//�ػ�false��ֹ�ػ���˸
	m_listctrl.SetRedraw(false);
	int nIndex=0;

	TCHAR szItem[10];
	memset(szItem,0,sizeof(szItem));
	for(iter=g_vector_imagepath.begin();iter!=g_vector_imagepath.end();iter++,nIndex++)
	{
		//m_listctrl.InsertItem(nIndex,iter->imagePath.c_str(),nIndex);
		sprintf_s(szItem,"%d",nIndex+1);
		m_listctrl.InsertItem(nIndex,szItem,nIndex);
	}

	m_listctrl.SetRedraw(true);
	m_listctrl.Invalidate();

	//Ϊ�����ͱ߿򴴽���ˢ
	HBRUSH hBrushBorder    =::CreateSolidBrush(RGB(220,220,220));
	HBRUSH hBrushBkground  =::CreateSolidBrush(RGB(255,255,255));

	//�߿�ߴ�
	RECT rcBorder;
	rcBorder.left=rcBorder.top  =0;
	rcBorder.right              =THUMB_WIDTH;
	rcBorder.bottom             =THUMB_HEIGHT;

	const float fRatio=(float)THUMB_HEIGHT/THUMB_WIDTH; //const��Ϊ�˷�ֹֻ�������ڳ������й����С����ܡ��ı�

	int XDest,YDest,nDestWidth,nDestHeight;
	nIndex=0;

	for(iter=g_vector_imagepath.begin();iter!=g_vector_imagepath.end();iter++,nIndex++)
	{ 
		//SSTRCPY(path, MAX_PATH, (*iter).c_str());
		//int nImageType = GetTypeFromFileName(path); 
		CString strPath((*iter).c_str());
		strPath.Replace(TEXT("bmp"), TEXT("jpg"));

		int nImageType = CXIMAGE_FORMAT_JPG; // ����ͼ����jpg

		if(nImageType == CXIMAGE_FORMAT_UNKNOWN)
			continue;
		CxImage image(strPath,nImageType);//��ͼ����ص�image��

		if (CXIMAGE_FORMAT_TIF == nImageType)
		{
			image.Negative();  // �ڰ�ͼ����Ҫ�ڰ׷�ת
		}
		//::MessageBox(NULL,TEXT(*iter),"*iter",MB_OK);
		//::MessageBox(NULL,TEXT(path),"LoadThumbNail",MB_OK);
		//image.Save("C:\\11.jpg",CXIMAGE_FORMAT_JPG);
		//CxImage image;
		//image.CreateFromHANDLE(*iter);//��ͼ����ص�image��
		//image.Rotate180();  // CxIamge����洢ͼƬ����
		if(image.IsValid()==false)
			continue;

		//�������rect��Ӧ����
		const float fImgRatio=(float)image.GetHeight()/image.GetWidth();
		if(fImgRatio>fRatio)
		{
			nDestWidth=THUMB_HEIGHT/fImgRatio;
			XDest=(THUMB_WIDTH-nDestWidth)/2;
			YDest=0;
			nDestHeight=THUMB_HEIGHT;
		}
		else
		{
			XDest=0;
			nDestWidth=THUMB_WIDTH;
			nDestHeight=THUMB_WIDTH*fImgRatio;
			YDest=(THUMB_HEIGHT-nDestHeight)/2;
		}

		//����CClientDC
		CClientDC cdc(this);//����һ����thisָ������Ķ�����豸�����Ķ���this����������ָ��
		HDC hDC=::CreateCompatibleDC(cdc.m_hDC);//����һ����ָ���豸���ݵ��ڴ��豸�����Ļ���
		HBITMAP bm=CreateCompatibleBitmap(cdc.m_hDC,THUMB_WIDTH,THUMB_HEIGHT);//������ָ���豸��������ݵ�λͼ
		HBITMAP pOldBitmapImage=(HBITMAP)SelectObject(hDC,bm);//ѡ��bm��DC��

		//draw background
		::FillRect(hDC,&rcBorder,hBrushBkground);

		//draw image
		image.Stretch(hDC,XDest,YDest,nDestWidth,nDestHeight);

		//draw border
		::FrameRect(hDC,&rcBorder,hBrushBorder);

		SelectObject(hDC,pOldBitmapImage);

		//attach to bitmap and replace image in CImageList
		CBitmap bitmap;
		bitmap.Attach(bm);
		m_imagelist.Replace(nIndex,&bitmap,NULL);

		//redraw only a current item for removing flicker and fast speed
		m_listctrl.RedrawItems(nIndex,nIndex);

		//release used DC and Object
		DeleteDC(hDC);
		DeleteObject(bm);
	}
	DeleteObject(hBrushBorder);
	DeleteObject(hBrushBkground);

	m_listctrl.Invalidate();
}

///*----------------------------------------------------------------------------- 
// Description: �����ؼ���Tooltip 
//-----------------------------------------------------------------------------*/ 
//HWND CDlg_Video::CreateToolTip(int toolID, HWND hDlg, PTSTR pszText) 
//{ 
//    if (!toolID || !hDlg || !pszText) 
//    { 
//        return FALSE; 
//    } 
//    HWND hwndTool = ::GetDlgItem(hDlg, toolID); 
// 
//    HWND hwndTip = CreateWindowEx(NULL, 
//                                  TOOLTIPS_CLASS, 
//                                  NULL, 
//                                  WS_POPUP | TTS_ALWAYSTIP | TTS_BALLOON, 
//                                  CW_USEDEFAULT, 
//                                  CW_USEDEFAULT, 
//                                  CW_USEDEFAULT, 
//                                  CW_USEDEFAULT, 
//                                  hDlg, 
//                                  NULL, 
//                                  AfxGetInstanceHandle(), 
//                                  NULL); 
// 
//    if (!hwndTool || !hwndTip) 
//    { 
//        return (HWND)NULL; 
//    } 
// 
//    TOOLINFO toolInfo = {0}; 
//    toolInfo.cbSize = sizeof(TOOLINFO); 
//    toolInfo.hwnd = hDlg; 
//    toolInfo.uFlags = TTF_IDISHWND | TTF_SUBCLASS; 
//    toolInfo.uId = (UINT_PTR)hwndTool; 
//    toolInfo.lpszText = pszText; 
//    ::SendMessage(hwndTip, TTM_ADDTOOL, 0, (LPARAM)&toolInfo); 
// 
//    return hwndTip; 
//} 




void CDlg_Video::OnImageDelete()
{
	// TODO: �ڴ���������������
	int nSelectedIndex = m_listctrl.GetNextItem(-1, LVNI_SELECTED);
	if (nSelectedIndex == -1)
	{
		AfxMessageBox("δѡ��ҳ��");
		return;
	}

	//int nCount = g_vecCust_ImageInfo.size();  // ��ȡͼƬ����
	int nCount = g_vector_imagepath.size();

	//////////////////////////////////////////////////////////////////////

	int nImageCount = m_imagelist.GetImageCount();

	LVITEM lvitem;
	for (int nIndex = 0; nIndex < nImageCount - 1; nIndex++)
	{
		if (nIndex < nSelectedIndex)
			continue;

		m_imagelist.Copy(nIndex, nIndex + 1);

		lvitem.iItem = nIndex;
		lvitem.mask = LVIF_IMAGE;
		m_listctrl.GetItem(&lvitem);
		lvitem.iImage = nIndex;
		m_listctrl.SetItem(&lvitem);

		CString text = m_listctrl.GetItemText(nIndex + 1, 0);
		m_listctrl.SetItemText(nIndex, 0, text);
	}
	m_imagelist.SetImageCount(nImageCount - 1);
	m_listctrl.DeleteItem(nImageCount - 1);

	////////////////////////////////////////////////////////////////////////////////

	int num = 0;
	//std::vector<CUST_IMAGEINFO>::iterator it;
	std::vector<std::string>::iterator it;
	for (it = g_vector_imagepath.begin(); it != g_vector_imagepath.end(); it++)
	{
		if (num < nSelectedIndex)
		{
			num++;
			continue;
		}

		if (num < nCount - 1)
			g_vector_imagepath[num] = g_vector_imagepath[num + 1];

		if (num == nCount - 1)
		{
			g_vector_imagepath.erase(it);  // �Ƴ���ͼ
			if (m_nFileCount > 0)
			{
				m_nFileCount -= 1; 
				//m_Capture.m_strBarcode.Empty();
				CString str;
				str.Format("%d ��", m_nFileCount);
				RefreshCStaticControl(IDC_VIDEO_STATIC_CAPTURECOUNT, str);
				//m_sPhotoNo.SetWindowText(str);
			}
			break;
		}

		num++;
	}

	if (nSelectedIndex > 0)
	{
		m_listctrl.SetItemState(-1, 0, LVIS_SELECTED);
		m_listctrl.SetItemState(nSelectedIndex - 1, LVIS_SELECTED, LVIS_SELECTED);											
	}
	else
	{
		m_listctrl.SetItemState(-1, 0, LVIS_SELECTED);
		m_listctrl.SetItemState(nSelectedIndex, LVIS_SELECTED, LVIS_SELECTED);
	}

	m_listctrl.Invalidate();
}


void CDlg_Video::OnImageLeft90()
{
	// TODO: �ڴ���������������
	ImageHandle(IH_LEFT90);
}


void CDlg_Video::OnImageRight90()
{
	// TODO: �ڴ���������������
	ImageHandle(IH_RIGHT90);
}


void CDlg_Video::OnImageFlipvertical()
{
	// TODO: �ڴ���������������
	ImageHandle(IH_FLIP);
}


void CDlg_Video::OnImageMirror()
{
	// TODO: �ڴ���������������
	ImageHandle(IH_MIRROR);
}


void CDlg_Video::OnNMRClickVideoListThumbnail(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CMenu menu, *pSubMenu; //��������Ҫ�õ���cmenu���� 
	menu.LoadMenu(IDR_POPMENU_THUMBNAIL); //װ���Զ�����Ҽ��˵� 
	pSubMenu = menu.GetSubMenu(0); //��ȡ��һ�������˵� 
	CPoint oPoint; //����һ������ȷ�����λ�õ�λ�� 
	GetCursorPos(&oPoint); //��ȡ��ǰ����λ�� 
	pSubMenu->TrackPopupMenu(TPM_LEFTALIGN, oPoint.x, oPoint.y, this); //��ָ��λ����ʾ�����˵�

	*pResult = 0;
}

void CDlg_Video::ImageHandle(enum_image_handle eMethod)
{
	//��õ�ǰѡ��ͼƬ������
	int nSelectedIndex = m_listctrl.GetNextItem(-1, LVNI_SELECTED);

	if (-1 == nSelectedIndex) {
		return;
	}

	TCHAR szTempPath[MAX_PATH]; 
	SSTRCPY(szTempPath, MAX_PATH, g_vector_imagepath[nSelectedIndex].c_str());

	//int nImageType = GetTypeFromFileName(szTempPath	
	//if(nImageType == CXIMAGE_FORMAT_UNKNOWN) {
	//	return;
	//}
	int nLargeType = CXIMAGE_FORMAT_BMP;  // ��ͼ��bmp
	int nSmallType = CXIMAGE_FORMAT_JPG;  // Сͼ��jpg


	CxImage imageLarge, imageSmall;
	imageLarge.Load(szTempPath); // ��ȡ��ͼ

	CString strSmallPath = g_vector_imagepath[nSelectedIndex].c_str();
	strSmallPath.Replace("bmp", "jpg");
	imageSmall.Load(strSmallPath); // ��ȡ����ͼ

	//::MessageBox(NULL,TEXT(szTempPath),MB_CAPTION,MB_OK);
	//CxImage image;
	//image.CreateFromHANDLE(*iter);//��ͼ����ص�image��
	//image.Rotate180();  // CxIamge����洢ͼƬ����
	if( (imageLarge.IsValid() == false) || (imageSmall.IsValid() == false))
	{
		return;
	}

	switch (eMethod)
	{
	case IH_LEFT90: {
		imageSmall.RotateLeft();
		imageLarge.RotateLeft();
		break;
							 }	
	case IH_RIGHT90: {
		imageSmall.RotateRight();
		imageLarge.RotateRight();
		break;
								}		
	case IH_FLIP: {
		imageSmall.Flip();
		imageLarge.Flip();
		break;
						 }	
	case IH_MIRROR: {
		imageSmall.Mirror();
		imageLarge.Mirror();
		break;
							 }
	default: { 
		break;
					 }		
	}

	//g_vector_imagepath[nSelectedIndex] = pImage->CopyToHandle();
	imageSmall.Save(strSmallPath, nSmallType);
	imageLarge.Save(szTempPath, nLargeType);

	LoadThumbNail();
}

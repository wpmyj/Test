// Dlg_Video.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "UDS Scanner.h"
#include "Dlg_Video.h"
#include "afxdialogex.h"

#include "public.h"
#include <MMSystem.h>
#pragma comment(lib, "Winmm.lib")  // ������������Ҫ
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
	, m_strTempPath(TEXT("C:\\test"))
	, m_bIsDPI(true)
  //, m_nDPIIndex(0)
	, m_bAutoCrop(false)
	, m_bShowInfo(false)
	, m_bPlaySound(false)
{

}

CDlg_Video::~CDlg_Video()
{
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
	DDX_Control(pDX, IDC_VIDEO_CHECK_SHOWINFO, m_CheckMSG);
	DDX_Control(pDX, IDC_VIDEO_SLIDER_EXP, m_sliderExp);
	DDX_Control(pDX, IDC_VIDEO_EDIT_EXPVALUE, m_editExp);
	//DDX_Control(pDX, IDC_VIDEO_EDIT_PATH, m_editPath);
	DDX_Control(pDX, IDC_VIDEO_CHECK_MANUAL, m_checkManual);
	DDX_Control(pDX, IDC_VIDEO_RICHEDIT2_MSG, m_richeditMSG);
	//DDX_Control(pDX, IDC_VIDEO_COMBO_FILETYPE, m_combFiletype);
	//DDX_Control(pDX, IDC_VIDEO_EDIT_URL, m_editURL);
	DDX_Control(pDX, IDC_VIDEO_STATIC_CAPTURECOUNT, m_sCaptureCount);
	DDX_Control(pDX, IDC_VIDEO_BUTTON_DPI, m_btnDPI);
	DDX_Control(pDX, IDC_VIDEO_CHECK_PLAYSOUND, m_checkPlaySound);
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
	ON_BN_CLICKED(IDC_VIDEO_CHECK_SHOWINFO, &CDlg_Video::OnBnClickedCheck_Showinfo)
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
	ON_BN_CLICKED(IDC_VIDEO_CHECK_PLAYSOUND, &CDlg_Video::OnBnClickedCheck_Playsound)
END_MESSAGE_MAP()


// CDlg_Video ��Ϣ�������


BOOL CDlg_Video::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
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
		m_editExp.EnableWindow(FALSE);
	}
	else
	{
		m_ocx.GetExposure(&value,&autoexp);
		m_checkAutoExp.SetCheck(autoexp);
	}
	if (!autoexp)
	{
		m_sliderExp.EnableWindow(TRUE);
		//m_editExp.EnableWindow(TRUE);
	}
	m_sliderExp.SetRange(expmin,expmax);
	m_sliderExp.SetPos(value);
	CString str;
	str.Format(TEXT("%d"),value);
	m_editExp.SetWindowText(str);
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
	m_CheckMSG.SetCheck(m_bShowInfo);
	m_checkPlaySound.SetCheck(m_bPlaySound);
	OnBnClickedCheck_Autocrop();
	OnBnClickedCheck_Showinfo();
	
	if (!PathIsDirectory(m_strTempPath))
	{
		CreateDirectory(m_strTempPath, NULL);
	}

	CString msg;
	msg.Format("������: %d��", m_nFileCount);
	m_sCaptureCount.SetWindowText(msg);
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
		m_editExp.EnableWindow(FALSE);
	}
	else
	{
		m_sliderExp.EnableWindow(TRUE);
		m_checkAutoExp.EnableWindow(TRUE);
		m_editExp.EnableWindow(TRUE);
		m_ocx.GetExposure(&value,&autoexp);
		m_checkAutoExp.SetCheck(autoexp);
	}
	if (!autoexp)
	{
		m_sliderExp.EnableWindow(TRUE);
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
	}
	else
	{
		m_sliderExp.EnableWindow(TRUE);
		int value = m_sliderExp.GetPos();
		m_ocx.SetAutoExposure(FALSE);
		m_ocx.SetExposure(value,0);
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


void CDlg_Video::OnBnClickedCheck_Showinfo()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_CheckMSG.GetCheck())
	{
		m_ocx.ShowVideoInfo(TRUE);
		m_bShowInfo = true;
	} 
	else
	{
		m_ocx.ShowVideoInfo(FALSE);
		m_bShowInfo = false;
	}
}


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
	m_ocx.CaptureImage(m_sCaptureName);
	if (m_checkPlaySound.GetCheck())
	{
		MyPlaySound(FILENAME_WAV_CAMERA1);
	}

	m_nFileCount ++;
	CString msg;
	msg.Format("������: %d��", m_nFileCount);
	m_sCaptureCount.SetWindowText(msg);

	g_vector_imagepath.push_back(m_sCaptureName.GetBuffer());
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
	if (m_checkPlaySound.GetCheck())
	{
		MyPlaySound(FILENAME_WAV_CAMERA1);
	}

	m_nFileCount ++;
	CString msg;
	msg.Format("������: %d��", m_nFileCount);
	m_sCaptureCount.SetWindowText(msg);
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
	if (m_checkPlaySound.GetCheck())
	{
		MyPlaySound(FILENAME_WAV_CAMERA1);
	}
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
			m_editExp.SetWindowText(str);
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
	m_richeditMSG.ReplaceSel(TEXT("���룺"));
	m_richeditMSG.ReplaceSel(Barcode);
	m_richeditMSG.ReplaceSel(TEXT("\n"));
	m_richeditMSG.PostMessage(WM_VSCROLL, SB_BOTTOM, 0);

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
	m_richeditMSG.ReplaceSel(fileName);
	m_richeditMSG.ReplaceSel(TEXT("\n"));
	m_richeditMSG.PostMessage(WM_VSCROLL, SB_BOTTOM, 0);

	m_nFileCount ++;
	CString msg;
	msg.Format("������: %d��", m_nFileCount);
	m_sCaptureCount.SetWindowText(msg);
	g_vector_imagepath.push_back(fileName);
}


void CDlg_Video::DeviceChanged_VideoUdsVideoctrl(LPCTSTR changeType, LPCTSTR deviceName)
{
	// TODO: �ڴ˴������Ϣ����������
	m_richeditMSG.ReplaceSel(changeType);
	m_richeditMSG.ReplaceSel(TEXT(" - "));
	m_richeditMSG.ReplaceSel(deviceName);
	m_richeditMSG.ReplaceSel(TEXT("\n"));
	m_richeditMSG.PostMessage(WM_VSCROLL, SB_BOTTOM, 0);
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
	//m_btnDPI.SetWindowText(TEXT("�ֱ���"));
	CString strText;
	m_btnDPI.GetWindowText(strText);

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
		m_btnDPI.SetWindowText(TEXT("�ֱ���"));
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
		m_btnDPI.SetWindowText(TEXT("DPI"));
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

	GetPrivateProfileString(INI_APP_CAMERASETTING,INI_KEY_SHOWINFO,TEXT(""),strTemp.GetBufferSetLength(nMaxLength),nMaxLength,szINIPath);
	if (strTemp.Find(TEXT("Y")) >= 0) {
		tempINI.ShowInfo = true;
	} 
	else {
		tempINI.ShowInfo = false;
	}

	GetPrivateProfileString(INI_APP_CAMERASETTING,INI_KEY_PLAYSOUND,TEXT(""),strTemp.GetBufferSetLength(nMaxLength),nMaxLength,szINIPath);
	if (strTemp.Find(TEXT("Y")) >= 0) {
		tempINI.playSound = true;
	} 
	else {
		tempINI.playSound = false;
	}

	//CString
	GetPrivateProfileString(INI_APP_CAMERASETTING,INI_KEY_CAMERANAME,TEXT(""),strTemp.GetBufferSetLength(nMaxLength),nMaxLength,szINIPath);
 
	//m_nDevIndex  = tempINI.CameraIndex;
	m_sCameraName = strTemp;
	//m_nDPIIndex  = tempINI.DpiIndex;
	//m_nPixelType = tempINI.PixelType;
	m_bAutoCrop  = tempINI.AutoCrop;
	m_bShowInfo  = tempINI.ShowInfo;
	m_bPlaySound = tempINI.playSound;

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

	if (m_bShowInfo) {
		strTemp = TEXT("Y");
	} 
	else {
		strTemp = TEXT("N");
	}
	WritePrivateProfileString(INI_APP_CAMERASETTING,INI_KEY_SHOWINFO,strTemp,szINIPath);

	if (m_bPlaySound) {
		strTemp = TEXT("Y");
	} 
	else {
		strTemp = TEXT("N");
	}
	WritePrivateProfileString(INI_APP_CAMERASETTING,INI_KEY_PLAYSOUND,strTemp,szINIPath);

}

void CDlg_Video::OnBnClickedCheck_Playsound()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_checkPlaySound.GetCheck() > 0 ? m_bPlaySound = true:m_bPlaySound = false;
}

void CDlg_Video::MyPlaySound(TCHAR* _szFileName)
{
	TCHAR szFilePath[MAX_PATH];
	GetFilePath(_szFileName, szFilePath);
	sndPlaySound(szFilePath, SND_ASYNC);
}

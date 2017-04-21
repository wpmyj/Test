// Page_Profile.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "UDS Scanner.h"
#include "Page_Profile.h"
#include "afxdialogex.h"
#include "Dlg_Profile.h"
#include "Dlg_Rename.h"
// CPage_Profile �Ի���

IMPLEMENT_DYNAMIC(CPage_Profile, CPropertyPage)

CPage_Profile::CPage_Profile(MFC_UI *pUI)
	: m_pUI(pUI),CPropertyPage(CPage_Profile::IDD)
{

}

CPage_Profile::~CPage_Profile()
{
}


void CPage_Profile::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROFILE_LIST_PROFILE, m_list_profile);
}


BEGIN_MESSAGE_MAP(CPage_Profile, CPropertyPage)
	ON_BN_CLICKED(IDC_PROFILE_BTN_RESET, &CPage_Profile::OnProfile_Btn_Reset)
	ON_BN_CLICKED(IDC_PROFILE_BTN_DELETE, &CPage_Profile::OnProfile_Btn_Delete)
	ON_BN_CLICKED(IDC_PROFILE_BTN_NEW, &CPage_Profile::OnProfile_Btn_New)
	ON_BN_CLICKED(IDC_PROFILE_BTN_RENAME, &CPage_Profile::OnProfile_Btn_Rename)
	ON_BN_CLICKED(IDC_PROFILE_BTN_IMPORT, &CPage_Profile::OnProfile_Btn_Import)
	ON_BN_CLICKED(IDC_PROFILE_BTN_EXPORT, &CPage_Profile::OnProfile_Btn_Export)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_PROFILE_LIST_PROFILE, &CPage_Profile::OnItemchangeProfile_List_Profile)
END_MESSAGE_MAP()


// CPage_Profile ��Ϣ�������

void CPage_Profile::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���
	m_pBasePage->SetCapValue();
	m_pAdPage->SetCapValue();
	//m_pPaperPage->SetCapValue();
	//m_pFilterPage->SetCapValue();
	m_pSetPage->SetCapValue();

	int index = m_list_profile.GetNextItem(-1,LVIS_SELECTED); 

	CString str;
	str = m_list_profile.GetItemText(index,0); //(i,j)�У���
	if (str.Find("UDS") >= 0 || str.Find("Ĭ��ģ��") >= 0 || str.Find("�ϴ�ʹ��") >= 0){}
	else{
		m_pUI->TW_SaveProfileToFile(str.GetBuffer()); //������������ģ��ʱ�����浱ǰѡ��ģ��
	}
	m_pUI->TW_SaveProfileToFile("�ϴ�ʹ��ģ��");//�ٴα��桰�ϴ�ʹ��ģ�塱
	m_pUI->TW_SaveProfileToFile("ģ�屸��");//����һ�ݱ���

	
	if(m_pUI->m_bSetup)  // EnableDSOnly
	{
		m_pUI->Save();
	}
	else  
	{
		m_pUI->Scan();
	}

	CPropertyPage::OnOK();
}


void CPage_Profile::OnCancel()
{
	// TODO: �ڴ����ר�ô����/����û���
	bool status = false;
	unsigned int unIndex = 1;
	lstString strFileNames;
	m_pUI->TW_GetAllProfiles(strFileNames);

	lstString::iterator iter = strFileNames.begin();
	for(;iter!=strFileNames.end(); iter++)
	{
		CString strTemp(iter->c_str());		

		if(strTemp.Find("�ϴ�ʹ��ģ��") >=0 ) 
		{
			m_list_profile.SetItemState(unIndex, LVNI_FOCUSED | LVIS_SELECTED, LVNI_FOCUSED | LVIS_SELECTED); 
			LoadTemplate();
			status = true;	
		}
		unIndex ++;
	}

	if(!status) //û�Ҽ��ϴ�ʹ��ģ��
	{
		m_pUI->ResetAllCaps();
		m_list_profile.SetItemState(0, LVNI_FOCUSED | LVIS_SELECTED, LVNI_FOCUSED | LVIS_SELECTED);
	}

	m_pUI->Cancel();
	m_pUI->TW_LoadProfileFromFile("ģ�屸��");

	CPropertyPage::OnCancel();
}

BOOL CPage_Profile::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	InitTemplate();
	LoadTemplate(); 

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CPage_Profile::InitTemplate()
{
	m_list_profile.SetExtendedStyle( LVS_EX_FULLROWSELECT); // ��ʾѡ�����У�����ĳһ��
	//������չ���Ϊ��������ʽ������ѡȡ
	m_list_profile.ShowScrollBar(SB_VERT,TRUE); // ֻ��ʾ��ֱ������
	CImageList m_image; 
	m_image.Create(1,20,ILC_COLOR32,1,0); 
	m_list_profile.SetImageList(&m_image, LVSIL_SMALL);
	m_list_profile.DeleteAllItems(); // ���

	CRect rect;   //��������У�����InserItemʧ��
	m_list_profile.GetClientRect(rect);
	m_list_profile.InsertColumn(0, "", LVCFMT_LEFT, rect.Width());

	m_list_profile.InsertItem(0, "Ĭ��ģ��");
	m_list_profile.SetItemState(0,LVNI_FOCUSED | LVIS_SELECTED, LVNI_FOCUSED | LVIS_SELECTED); //������indexΪѡ�в����������δ����Focuse����ѡ���л��Ϊ��ɫ��

	NewTemplate();
	SetLastTemplate();
	SetDelete();
	SetRename();
}


void CPage_Profile::SetLastTemplate()
{
	lstString strFileNames;
	m_pUI->TW_GetAllProfiles(strFileNames);

	unsigned int unIndex = 1;//û��0��ʼ����ΪĬ��ģ��һֱ�����0��λ��
	lstString::iterator iter = strFileNames.begin();
	for(;iter!=strFileNames.end(); iter++)
	{
		CString strTemp(iter->c_str());	
		if(strTemp.Find("ģ�屸��") >= 0){}
		else
		{
			m_list_profile.InsertItem(unIndex, strTemp);
			
			if(strTemp.Find("�ϴ�ʹ��") >=0 ) {
				m_list_profile.SetItemState(unIndex, LVIS_FOCUSED|LVIS_SELECTED, LVIS_FOCUSED|LVIS_SELECTED);			
				LoadTemplate();
			}
			unIndex++;
		}
	}
	m_list_profile.SetRedraw(true);  // �ػ�
	m_list_profile.Invalidate();
}


void CPage_Profile::LoadTemplate()
{
	UpdateData(TRUE);  // ��������
	int nIndex = m_list_profile.GetNextItem(-1,LVIS_SELECTED);

	if(0 == nIndex)  // Ĭ��ģ�壬������������
	{
		m_pUI->ResetAllCaps();
	}
	else  // ����ģ��
	{	
		CString strProfile; 
		strProfile = m_list_profile.GetItemText(nIndex, 0);
		m_pUI->TW_LoadProfileFromFile(strProfile.GetBuffer()); //��m_pDS->SetGustomDSData
	}

	m_pBasePage->UpdateControls();
	m_pAdPage->UpdateControls();//�߼����ý������Ҳ����
	m_pSetPage->UpdateControls();	

	UpdateData(FALSE);
}


void CPage_Profile::SetDelete(void)
{
	int nIndex = m_list_profile.GetNextItem(-1,LVIS_SELECTED); 
	CString strCBText; 
	strCBText = m_list_profile.GetItemText(nIndex, 0);
	if (strCBText.Find("UDS") >= 0 || strCBText.Find("Ĭ��ģ��") >= 0)
	{
		GetDlgItem(IDC_PROFILE_BTN_DELETE)->EnableWindow(FALSE);
	} 
	else
	{
		GetDlgItem(IDC_PROFILE_BTN_DELETE)->EnableWindow(TRUE);
	}
}


void CPage_Profile::SetRename(void)
{
	int nIndex = m_list_profile.GetNextItem(-1,LVIS_SELECTED); 

	CString strCBText; 
	strCBText = m_list_profile.GetItemText(nIndex,0);
	if (strCBText.Find("UDS") >= 0 || strCBText.Find("Ĭ��ģ��") >= 0 
		|| strCBText.Find("�ϴ�ʹ��") >= 0)
	{
		GetDlgItem(IDC_PROFILE_BTN_RENAME)->EnableWindow(FALSE);
	} 
	else
	{
		GetDlgItem(IDC_PROFILE_BTN_RENAME)->EnableWindow(TRUE);
	}
}

////////////////////////////////////////////////////////////////////////////////
//�½�ģ�� 
void CPage_Profile::NewTemplate()
{
	string strProfileName;

	//�½�����ɫ,����,200dpi��ģ��
	strProfileName = "UDS--��ɫ,����,200dpi";
	if(false == CreateNewTemplate(strProfileName, 2, 0)) {
		return;
	}

	//�½�����ɫ,˫��,200dpi��ģ��
	strProfileName = "UDS--��ɫ,˫��,200dpi";
	if(false == CreateNewTemplate(strProfileName, 2, 1)) {
		return;
	}

	//�½����ڰ�,����,200dpi��ģ��
	strProfileName = "UDS--�ڰ�,����,200dpi";
	if(false == CreateNewTemplate(strProfileName, 0, 0)) {
		return;
	}

	//�½����ڰ�,˫��,200dpi��ģ��
	strProfileName = "UDS--�ڰ�,˫��,200dpi";
	if(false == CreateNewTemplate(strProfileName, 0, 1)) {
		return;
	}

	//�½����Ҷ�,����,200dpi��ģ��
	strProfileName = "UDS--�Ҷ�,����,200dpi";
	if(false == CreateNewTemplate(strProfileName, 1, 0)) {
		return;
	}

	//�½����Ҷ�,˫��,200dpi��ģ��
	strProfileName = "UDS--�Ҷ�,˫��,200dpi";
	if(false == CreateNewTemplate(strProfileName, 1, 1)) {
		return;
	}

	//�½�����ɫ,����,300dpi��ģ��
	strProfileName = "UDS--��ɫ,����,300dpi";
	if(false == CreateNewTemplate(strProfileName, 2, 0, 300)) {
		return;
	}

	//�½�����ɫ,˫��,300dpi��ģ��
	strProfileName = "UDS--��ɫ,˫��,300dpi";
	if(false == CreateNewTemplate(strProfileName, 2, 1, 300)) {
		return;
	}
}


bool CPage_Profile::CreateNewTemplate(std::string profilename, int pixeltype, 
	int duplexenabled, int resolution)
{
	if(false == m_pUI->SetCapValueInt(ICAP_PIXELTYPE,pixeltype)) { 
		return false;
	}
	if(false == m_pUI->SetCapValueInt(CAP_DUPLEXENABLED,duplexenabled)) {
		return false;
	}
	if(false == m_pUI->SetCapValueInt(ICAP_XRESOLUTION,resolution)) { 
		return false;
	}
	if(false == m_pUI->SetCapValueInt(ICAP_YRESOLUTION,resolution)) {
		return false;
	}
	if(false == m_pUI->TW_SaveProfileToFile(profilename)) {
		return false;
	}
	return true;
}


//�½�ģ��
void CPage_Profile::OnProfile_Btn_New()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDlg_Profile dlg;  // �½�ģ��Ի���
	dlg.DoModal();

	CString strExistName;

	if (TRUE == dlg.m_bOk)  // ȷ���½�ģ��
	{
		strExistName = dlg.GetProfileName();  // ��ȡ�Ի����б����ģ����
	}
	else
	{
		return;
	}

	//�Ƿ���Ҫ���´���
	bool recreate = false;
	int renameindex; //���������
	// �ж��½�ģ�����Ƿ��Ѵ���
	CString strCombo;  
	for (int i = 0; i < m_list_profile.GetItemCount(); i++)
	{        
		strCombo = m_list_profile.GetItemText(i, 0);
		if(strCombo == strExistName)
		{
			renameindex = i;
			if(AfxMessageBox("ģ���Ѵ��ڣ�����Ҫ���´�����",MB_OKCANCEL) == IDCANCEL)  
			{
				recreate = false;
				return;  // ȡ���½�ͬ��ģ��
			}
			else
			{
				recreate = true;
			}
		}
		strCombo.ReleaseBuffer();      
	}
	
	CString strName = strExistName;
	string strProfile = strName.GetBuffer();  // CString->string
	strName.ReleaseBuffer();

	if(m_pUI->TW_SaveProfileToFile(strProfile))
	{	
		if(!recreate)
		{
			m_list_profile.InsertItem(m_list_profile.GetItemCount(),strName);//��ʱm_list_profile.GetItemCount()�Ѿ�����
			m_list_profile.SetItemState(m_list_profile.GetItemCount()-1, LVNI_FOCUSED | LVIS_SELECTED, LVNI_FOCUSED | LVIS_SELECTED); 		
		}
		else
		{
			m_list_profile.SetItemState(renameindex, LVNI_FOCUSED | LVIS_SELECTED, LVNI_FOCUSED | LVIS_SELECTED); 
		}
	}
	
	SetDelete();
	SetRename();
}


//������
void CPage_Profile::OnProfile_Btn_Rename()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDlg_Rename dlg;  // �������Ի���
	dlg.DoModal();

	CString strExistName;
	if (TRUE == dlg.m_bOk)  // ȷ��������
	{
		strExistName = dlg.GetNewName();  // ��ȡ�Ի����е�������
	}
	else
	{
		return;
	}

	//�Ƿ���Ҫ���´���
	bool recreate = false;
	int renameindex; //���������
	// �ж��������Ƿ��Ѵ���
	CString strCombo;  
	int nLength;   
	for (int i = 0; i < m_list_profile.GetItemCount(); i++)
	{       
		strCombo = m_list_profile.GetItemText(i,0);
		if (strCombo == strExistName)
		{
			renameindex = i;
			if(AfxMessageBox("�������Ѵ��ڣ�����Ҫ���´�����",MB_OKCANCEL) == IDCANCEL)  
			{
				recreate = false;
				return;  // ȡ��������ģ��
			}
			else
			{
				recreate = true;
			}

		}
		strCombo.ReleaseBuffer();      
	}

	int index = m_list_profile.GetNextItem(-1,LVIS_SELECTED); 

	CString strOldName;
	strOldName = m_list_profile.GetItemText(index,0);
	string OldName = strOldName.GetBuffer();// CString->string
	strOldName.ReleaseBuffer();

	CString strNewName = strExistName;
	string NewName = strNewName.GetBuffer();
	strNewName.ReleaseBuffer();

	if(m_pUI->RenameProfile(OldName, NewName))
	{		
		if(!recreate)
		{
			m_list_profile.DeleteItem(index); //ɾ��ԭ��ѡ����	
			m_list_profile.InsertItem(index, strNewName);
			m_list_profile.SetItemState(index, LVNI_FOCUSED | LVIS_SELECTED, LVNI_FOCUSED | LVIS_SELECTED); 
		}
	}
	else
	{
		if(recreate)
		{	
			m_list_profile.DeleteItem(index); //ɾ��ԭ��ѡ����	
			m_pUI->TW_DeleteProfile(OldName); //����ɾ������ԭ����ģ��
			m_list_profile.SetItemState(renameindex, LVNI_FOCUSED | LVIS_SELECTED, LVNI_FOCUSED | LVIS_SELECTED);
		}	
	}

	SetDelete();
	SetRename();
	UpdateData(FALSE);
}


//ɾ��
void CPage_Profile::OnProfile_Btn_Delete()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);  // ��������
	int nIndex = m_list_profile.GetNextItem(-1,LVIS_SELECTED); 
	CString strProfile; 
	strProfile = m_list_profile.GetItemText(nIndex,0);

	if(m_pUI->TW_DeleteProfile(strProfile.GetBuffer()))
	{
		m_list_profile.DeleteItem(nIndex);
	}

	m_list_profile.SetItemState(0, LVNI_FOCUSED | LVIS_SELECTED, LVNI_FOCUSED | LVIS_SELECTED);

	LoadTemplate();
	SetDelete();
	UpdateData(FALSE);  
}


//����
void CPage_Profile::OnProfile_Btn_Reset()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_pUI->ResetAllCaps();
	m_list_profile.SetItemState(0, LVNI_FOCUSED | LVIS_SELECTED, LVNI_FOCUSED | LVIS_SELECTED);
	LoadTemplate();
}


//����
void CPage_Profile::OnProfile_Btn_Import()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strReadFilePath;  
	CFileDialog fileDlg(true, _T("dsp"), _T("*.dsp"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("ģ���ļ�(*.dsp)|*.dsp||"), NULL);  
	if (fileDlg.DoModal() == IDOK)    //�����Ի���  
	{  
		strReadFilePath = fileDlg.GetPathName();//�õ��������ļ�����Ŀ¼����չ��  
		
		string str = m_pUI->GetProfileNamePath();
		CString strPath = str.c_str();

		CString filename = fileDlg.GetFileName();   //GetFileName( ) �õ��������ļ�����������չ�� 
		strPath = strPath + filename; 
		if(CopyFile(strReadFilePath,strPath,TRUE))  
		{ 
			int num = m_list_profile.GetItemCount(); 
			CString strProfile;
			strProfile = fileDlg.GetFileTitle();
			m_list_profile.InsertItem(num,strProfile); //GetFileTitle�õ��������ļ�����������Ŀ¼������չ��
			m_list_profile.SetItemState(num, LVNI_FOCUSED | LVIS_SELECTED, LVNI_FOCUSED | LVIS_SELECTED);

			//������ģ��
			if(m_pUI->TW_LoadProfileFromFile(strProfile.GetBuffer()))
			{
				m_pBasePage->UpdateControls();
				m_pAdPage->UpdateControls();//�߼����ý������Ҳ����(�зֱ��ʹ�ͬ����)
			}
			else
			{
				MessageBox("����ʧ�ܣ��ļ���ʽ����");
			}
			
		}
		else
		{
			MessageBox("�ļ��Ѿ����ڣ�����ʧ��!");
		}
	}  
}


//����
void CPage_Profile::OnProfile_Btn_Export()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������  
	/*
	// ���öԻ���  
	BROWSEINFO bi; 
	CString szString = TEXT("��ѡ��һ���ļ��д��DSP�ļ�");
	ZeroMemory(&bi, sizeof(bi));  //��ָ�����ڴ������
	bi.pidlRoot = NULL; // �ļ��жԻ���֮��Ŀ¼����ָ���Ļ���Ϊ�ҵĵ���  
	bi.lpszTitle = szString; // ���Բ�ָ��  
	bi.ulFlags = BIF_BROWSEFORCOMPUTER | BIF_DONTGOBELOWDOMAIN | BIF_RETURNONLYFSDIRS;  
	bi.lpfn   =   NULL;   //�ص�������ָ��  
	bi.lParam   =   NULL;   //����ص������Ĳ���

	TCHAR targetPath[MAX_PATH];
	// �򿪶Ի���, �е���DoModal  
	LPITEMIDLIST targetLocation = SHBrowseForFolder(&bi);  //����һ��ITEMIDLIST�ṹ��ָ�룬����ṹ�������û�ѡ���ļ��е���Ϣ
	if (targetLocation == NULL) 
	{   
		return;	
	} 
	SHGetPathFromIDList(targetLocation, targetPath); //ȡ�ô洢��ITEMIDLIST�ṹָ���е�·����Ϣ 

	int index = m_list_template.GetCurSel();
	CString strTemplate;
	m_list_template.GetText(index, strTemplate);

	CString strDirPath = targetPath; //strDirPathΪĿ��·��
	if(strDirPath != "" && strDirPath.Right(1) != '\\'){
		strDirPath += '\\'; //����·�������б��
	}
	strDirPath = strDirPath + strTemplate + ".dsp";

	string str = m_pUI->GetProfileNamePath(); 
	CString strPath = str.c_str() + strTemplate + ".dsp"; //strPathΪģ��ԭ���·��

	if(!CopyFile(strPath,strDirPath,TRUE))  
	{
		AfxMessageBox("�ļ��Ѿ����ڣ�����ʧ�ܣ�");
	}
	else
	{
		//AfxMessageBox("�����ɹ���");
	}

	//��ֹ�ڴ�й©��Ҫʹ��IMalloc�ӿ��ͷ�ָ�� 
	//Ҫע����Ƿ��ص�ָᘱ�����Ӧ�ó����Լ����ͷš�
	//��������SHBrowseForFolder��ͨ������IMalloc Interface������Memory��
	//���ԣ�Ҳ����ͨ�����Interface���ͷš�
	IMalloc* pMalloc;  
	if(SHGetMalloc(&pMalloc) != NOERROR )  
	{  
		// δ������Ч��IMalloc�ӿ�ָ��  
		TRACE(_T("�޷�ȡ����ǳ����IMalloc�ӿ�\n"));  
	}  
	pMalloc->Free(targetLocation);  //�ͷ�ָ��
	if(pMalloc)  
		pMalloc->Release();   
	*/

	int index = m_list_profile.GetNextItem(-1,LVIS_SELECTED); 

	CString strTemplate;
	strTemplate = m_list_profile.GetItemText(index,0);

	string str = m_pUI->GetProfileNamePath(); 
	CString strPath = str.c_str() + strTemplate + ".dsp"; //strPathΪģ��ԭ���·��

	CFileDialog dlg(FALSE, _T("dsp"), strTemplate + ".dsp", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T(".dsp"), NULL);//FALSE��ʾΪ�����Ϊ���Ի��򣬷���Ϊ���򿪡��Ի���
	if(dlg.DoModal() == IDOK)
	{
		CString strDirPath = dlg.GetPathName();//�õ��������ļ�����Ŀ¼����չ�� 
	
		if(!CopyFile(strPath, strDirPath, TRUE))  
		{
			AfxMessageBox("�ļ��Ѿ����ڣ�����ʧ�ܣ�");
		}
		else
		{
			AfxMessageBox("�����ɹ���");
		}
	}
}


BOOL CPage_Profile::OnSetActive()
{
	// TODO: �ڴ����ר�ô����/����û���
	m_pUI->PreViewStatus();	
	return __super::OnSetActive();
}


void CPage_Profile::OnItemchangeProfile_List_Profile(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	LoadTemplate();	
	SetDelete();
	SetRename();
	*pResult = 0;
}

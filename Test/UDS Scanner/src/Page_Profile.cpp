// Page_Profile.cpp : 实现文件
//

#include "stdafx.h"
#include "UDS Scanner.h"
#include "Page_Profile.h"
#include "afxdialogex.h"
#include "Dlg_Profile.h"
#include "Dlg_Rename.h"
// CPage_Profile 对话框

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
	DDX_Control(pDX, IDC_PROFILE_LIST_TEMPLATE, m_list_template);
}


BEGIN_MESSAGE_MAP(CPage_Profile, CPropertyPage)
	ON_BN_CLICKED(IDC_PROFILE_BTN_RESET, &CPage_Profile::OnProfile_Btn_Reset)
	ON_BN_CLICKED(IDC_PROFILE_BTN_DELETE, &CPage_Profile::OnProfile_Btn_Delete)
	ON_LBN_SELCHANGE(IDC_PROFILE_LIST_TEMPLATE, &CPage_Profile::OnLbnSelchangeProfile_List_Template)
	ON_BN_CLICKED(IDC_PROFILE_BTN_NEW, &CPage_Profile::OnProfile_Btn_New)
	ON_BN_CLICKED(IDC_PROFILE_BTN_RENAME, &CPage_Profile::OnProfile_Btn_Rename)
	ON_BN_CLICKED(IDC_PROFILE_BTN_IMPORT, &CPage_Profile::OnProfile_Btn_Import)
	ON_BN_CLICKED(IDC_PROFILE_BTN_EXPORT, &CPage_Profile::OnProfile_Btn_Export)
END_MESSAGE_MAP()


// CPage_Profile 消息处理程序

void CPage_Profile::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	//SetCapValue(); //点击确定后才设置	
	//m_pAdPage->SetCapValue(); //先设置高级界面，再设置基本界面,否则基本的“双面”设置后，高级的“分割”又设置为1了

	m_pBasePage->SetCapValue();
	m_pAdPage->SetCapValue();
	m_pPaperPage->SetCapValue();

	int index = m_list_template.GetCurSel();
	CString str;
	m_list_template.GetText(index, str);
	if (str.Find("UDS") >= 0 || str.Find("默认模板") >= 0 || str.Find("上次使用") >= 0){}
	else{
		m_pUI->TW_SaveProfileToFile(str.GetBuffer()); //不是上述类型模板时，保存当前选中模板
	}
	m_pUI->TW_SaveProfileToFile("上次使用模板");//再次保存“上次使用模板”
	
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
	// TODO: 在此添加专用代码和/或调用基类
	bool status = false;
	unsigned int unIndex = 1;
	lstString strFileNames;
	m_pUI->TW_GetAllProfiles(strFileNames);

	lstString::iterator iter = strFileNames.begin();
	for(;iter!=strFileNames.end(); iter++)
	{
		CString strTemp(iter->c_str());		

		if(strTemp.Find("上次使用模板") >=0 ) 
		{
			m_list_template.SetCurSel(unIndex);
			LoadTemplate();
			status = true;	
		}
		unIndex ++;
	}

	if(!status) //没找见上次使用模板
	{
		m_pUI->ResetAllCaps();
	}

	m_pUI->Cancel();

	CPropertyPage::OnCancel();
}

BOOL CPage_Profile::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	// TODO:  在此添加额外的初始化
	InitTemplate();
	LoadTemplate(); 

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CPage_Profile::InitTemplate()
{
	m_list_template.ResetContent();
	m_list_template.InsertString(0,"默认模板");
	m_list_template.SetCurSel(0); //设置为默认模板

	NewTemplate();
	SetLastTemplate();
	SetDelete();
	SetRename();
}


void CPage_Profile::SetLastTemplate()
{
	lstString strFileNames;
	m_pUI->TW_GetAllProfiles(strFileNames);

	unsigned int unIndex = 1;//没从0开始是因为默认模板一直在序号0的位置
	lstString::iterator iter = strFileNames.begin();
	for(;iter!=strFileNames.end(); iter++)
	{
		CString strTemp(iter->c_str());		
		m_list_template.InsertString(unIndex, strTemp);

		if(strTemp.Find("上次使用") >=0 ) {
			m_list_template.SetCurSel(unIndex);
			LoadTemplate();
		}
		unIndex ++;
	}
}


void CPage_Profile::LoadTemplate()
{
	UpdateData(TRUE);  // 接收数据
	int nIndex = m_list_template.GetCurSel();
	if(0 == nIndex)  // 默认模板，重置驱动参数
	{
		m_pUI->ResetAllCaps();
	}
	else  // 其它模板
	{	
		CString strProfile; 
		m_list_template.GetText( nIndex, strProfile);
		m_pUI->TW_LoadProfileFromFile(strProfile.GetBuffer()); //会m_pDS->SetGustomDSData
	}

	m_pBasePage->UpdateControls();
	m_pAdPage->UpdateControls();//高级设置界面参数也更新(有分辨率共同存在)
}


void CPage_Profile::SetDelete(void)
{
	int nIndex = m_list_template.GetCurSel();
	CString strCBText; 
	m_list_template.GetText( nIndex, strCBText);
	if (strCBText.Find("UDS") >= 0 || strCBText.Find("默认模板") >= 0)
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
	int nIndex = m_list_template.GetCurSel();
	CString strCBText; 
	m_list_template.GetText( nIndex, strCBText);
	if (strCBText.Find("UDS") >= 0 || strCBText.Find("默认模板") >= 0 
		|| strCBText.Find("上次使用") >= 0)
	{
		GetDlgItem(IDC_PROFILE_BTN_RENAME)->EnableWindow(FALSE);
	} 
	else
	{
		GetDlgItem(IDC_PROFILE_BTN_RENAME)->EnableWindow(TRUE);
	}
}

////////////////////////////////////////////////////////////////////////////////
//新建模板 
void CPage_Profile::NewTemplate()
{
	string strProfileName;

	//新建“彩色,单面,200dpi”模板
	strProfileName = "UDS--彩色,单面,200dpi";
	if(false == CreateNewTemplate(strProfileName, 2, 0)) {
		return;
	}

	//新建“彩色,双面,200dpi”模板
	strProfileName = "UDS--彩色,双面,200dpi";
	if(false == CreateNewTemplate(strProfileName, 2, 1)) {
		return;
	}

	//新建“黑白,单面,200dpi”模板
	strProfileName = "UDS--黑白,单面,200dpi";
	if(false == CreateNewTemplate(strProfileName, 0, 0)) {
		return;
	}

	//新建“黑白,双面,200dpi”模板
	strProfileName = "UDS--黑白,双面,200dpi";
	if(false == CreateNewTemplate(strProfileName, 0, 1)) {
		return;
	}

	//新建“灰度,单面,200dpi”模板
	strProfileName = "UDS--灰度,单面,200dpi";
	if(false == CreateNewTemplate(strProfileName, 1, 0)) {
		return;
	}

	//新建“灰度,双面,200dpi”模板
	strProfileName = "UDS--灰度,双面,200dpi";
	if(false == CreateNewTemplate(strProfileName, 1, 1)) {
		return;
	}

	//新建“彩色,单面,300dpi”模板
	strProfileName = "UDS--彩色,单面,300dpi";
	if(false == CreateNewTemplate(strProfileName, 2, 0, 300)) {
		return;
	}

	//新建“彩色,双面,300dpi”模板
	strProfileName = "UDS--彩色,双面,300dpi";
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


void CPage_Profile::OnLbnSelchangeProfile_List_Template()
{
	// TODO: 在此添加控件通知处理程序代码
	LoadTemplate();	
	SetDelete();
	SetRename();
}


//新建模板
void CPage_Profile::OnProfile_Btn_New()
{
	// TODO: 在此添加控件通知处理程序代码
	CDlg_Profile dlg;  // 新建模版对话框
	dlg.DoModal();

	CString strExistName;

	if (TRUE == dlg.m_bOk)  // 确定新建模版
	{
		strExistName = dlg.GetProfileName();  // 获取对话框中保存的模版名
	}
	else
	{
		return;
	}

	// 判断新建模板名是否已存在
	CString strCombo;  
	int nLength;   
	for (int i = 0; i < m_list_template.GetCount(); i++)
	{        
		nLength = m_list_template.GetTextLen( i );  // 获取Combobox内容长度
		m_list_template.GetText( i, strCombo.GetBuffer(nLength));
		if (strCombo == strExistName)
		{
			if (AfxMessageBox("模版已存在，您想要重新创建吗？",MB_OKCANCEL) == IDCANCEL)  
			{
				return;  // 取消新建同名模版
			}
		}
		strCombo.ReleaseBuffer();      
	}

	//保存修改的CapValue
	//SetCapValue();

	CString strName = strExistName;
	string strProfile = strName.GetBuffer();  // CString->string
	strName.ReleaseBuffer();

	if(m_pUI->TW_SaveProfileToFile(strProfile))
	{		
		m_list_template.AddString(strName);
		m_list_template.SetCurSel(m_list_template.GetCount()-1);
	}

	UpdateData(FALSE);
}


//重命名
void CPage_Profile::OnProfile_Btn_Rename()
{
	// TODO: 在此添加控件通知处理程序代码
	CDlg_Rename dlg;  // 重命名对话框
	dlg.DoModal();

	CString strExistName;

	if (TRUE == dlg.m_bOk)  // 确定重命名
	{
		strExistName = dlg.GetNewName();  // 获取对话框中的新名称
	}
	else
	{
		return;
	}

	// 判断新名称是否已存在
	CString strCombo;  
	int nLength;   
	for (int i = 0; i < m_list_template.GetCount(); i++)
	{        
		nLength = m_list_template.GetTextLen( i );  // 获取Combobox内容长度
		m_list_template.GetText( i, strCombo.GetBuffer(nLength));
		if (strCombo == strExistName)
		{
			if (AfxMessageBox("该名称已存在，您想要重新创建吗？",MB_OKCANCEL) == IDCANCEL)  
			{
				return;  // 取消新建同名模版
			}
		}
		strCombo.ReleaseBuffer();      
	}

	int index = m_list_template.GetCurSel();

	CString strOldName;
	m_list_template.GetText(index, strOldName);
	string OldName = strOldName.GetBuffer();// CString->string
	strOldName.ReleaseBuffer();

	CString strNewName = strExistName;
	string NewName = strNewName.GetBuffer();
	strNewName.ReleaseBuffer();

	if(m_pUI->RenameProfile(OldName, NewName))
	{	
		m_list_template.DeleteString(index); //删除原有选中项
		m_list_template.InsertString(index, strNewName);
		m_list_template.SetCurSel(index);
	}

	UpdateData(FALSE);
}


//删除
void CPage_Profile::OnProfile_Btn_Delete()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);  // 接收数据
	int nIndex = m_list_template.GetCurSel();
	CString strProfile; 
	m_list_template.GetText( nIndex, strProfile);

	if(m_pUI->TW_DeleteProfile(strProfile.GetBuffer()))
	{
		m_list_template.DeleteString(nIndex);
	}

	m_list_template.SetCurSel(0);  // 切换到默认模板
	LoadTemplate();
	SetDelete();
}


//重置
void CPage_Profile::OnProfile_Btn_Reset()
{
	// TODO: 在此添加控件通知处理程序代码
	m_pUI->ResetAllCaps();
	m_list_template.SetCurSel(0);

	LoadTemplate();
}


//导入
void CPage_Profile::OnProfile_Btn_Import()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strReadFilePath;  
	CFileDialog fileDlg(true, _T("dsp"), _T("*.dsp"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("模板文件(*.dsp)|*.dsp||"), NULL);  
	if (fileDlg.DoModal() == IDOK)    //弹出对话框  
	{  
		strReadFilePath = fileDlg.GetPathName();//得到完整的文件名和目录名拓展名  
		
		string str = m_pUI->GetProfileNamePath();
		CString strPath = str.c_str();

		CString filename = fileDlg.GetFileName();   //GetFileName( ) 得到完整的文件名，包括扩展名 
		strPath = strPath + filename; 
		if(CopyFile(strReadFilePath,strPath,TRUE))  
		{
			int num = m_list_template.GetCount(); 
			CString strProfile;
			strProfile = fileDlg.GetFileTitle();
			m_list_template.InsertString(num,strProfile); //GetFileTitle得到完整的文件名，不包括目录名和扩展名
			m_list_template.SetCurSel(num);

			//加载新模板
			if(m_pUI->TW_LoadProfileFromFile(strProfile.GetBuffer()))
			{
				m_pBasePage->UpdateControls();
				m_pAdPage->UpdateControls();//高级设置界面参数也更新(有分辨率共同存在)
			}
			else
			{
				MessageBox("导入失败！文件格式错误！");
			}
			
		}
		else
		{
			MessageBox("文件已经存在，导入失败!");
		}
	}  
}


//导出
void CPage_Profile::OnProfile_Btn_Export()
{
	// TODO: 在此添加控件通知处理程序代码  
	/*
	// 配置对话框  
	BROWSEINFO bi; 
	CString szString = TEXT("请选择一个文件夹存放DSP文件");
	ZeroMemory(&bi, sizeof(bi));  //将指定的内存块清零
	bi.pidlRoot = NULL; // 文件夹对话框之根目录，不指定的话则为我的电脑  
	bi.lpszTitle = szString; // 可以不指定  
	bi.ulFlags = BIF_BROWSEFORCOMPUTER | BIF_DONTGOBELOWDOMAIN | BIF_RETURNONLYFSDIRS;  
	bi.lpfn   =   NULL;   //回调函数的指针  
	bi.lParam   =   NULL;   //传向回调函数的参数

	TCHAR targetPath[MAX_PATH];
	// 打开对话框, 有点像DoModal  
	LPITEMIDLIST targetLocation = SHBrowseForFolder(&bi);  //返回一个ITEMIDLIST结构的指针，这个结构包含了用户选择文件夹的信息
	if (targetLocation == NULL) 
	{   
		return;	
	} 
	SHGetPathFromIDList(targetLocation, targetPath); //取得存储于ITEMIDLIST结构指针中的路径信息 

	int index = m_list_template.GetCurSel();
	CString strTemplate;
	m_list_template.GetText(index, strTemplate);

	CString strDirPath = targetPath; //strDirPath为目的路径
	if(strDirPath != "" && strDirPath.Right(1) != '\\'){
		strDirPath += '\\'; //会在路径后加上斜杠
	}
	strDirPath = strDirPath + strTemplate + ".dsp";

	string str = m_pUI->GetProfileNamePath(); 
	CString strPath = str.c_str() + strTemplate + ".dsp"; //strPath为模板原存放路径

	if(!CopyFile(strPath,strDirPath,TRUE))  
	{
		AfxMessageBox("文件已经存在，导出失败！");
	}
	else
	{
		//AfxMessageBox("导出成功！");
	}

	//防止内存泄漏，要使用IMalloc接口释放指针 
	//要注意的是返回的指針必须由应用程序自己来释放。
	//并且由于SHBrowseForFolder是通过调用IMalloc Interface来分配Memory，
	//所以，也必须通过这个Interface來释放。
	IMalloc* pMalloc;  
	if(SHGetMalloc(&pMalloc) != NOERROR )  
	{  
		// 未返回有效的IMalloc接口指针  
		TRACE(_T("无法取得外壳程序的IMalloc接口\n"));  
	}  
	pMalloc->Free(targetLocation);  //释放指针
	if(pMalloc)  
		pMalloc->Release();   
	*/

	int index = m_list_template.GetCurSel();
	CString strTemplate;
	m_list_template.GetText(index, strTemplate);

	string str = m_pUI->GetProfileNamePath(); 
	CString strPath = str.c_str() + strTemplate + ".dsp"; //strPath为模板原存放路径

	CFileDialog dlg(FALSE, _T("dsp"), strTemplate + ".dsp", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T(".dsp"), NULL);//FALSE表示为“另存为”对话框，否则为“打开”对话框
	if(dlg.DoModal() == IDOK)
	{
		CString strDirPath = dlg.GetPathName();//得到完整的文件名和目录名拓展名 
	
		if(!CopyFile(strPath, strDirPath, TRUE))  
		{
			AfxMessageBox("文件已经存在，导出失败！");
		}
		else
		{
			AfxMessageBox("导出成功！");
		}
	}
}


BOOL CPage_Profile::OnSetActive()
{
	// TODO: 在此添加专用代码和/或调用基类
	m_pUI->PreViewStatus();	
	return __super::OnSetActive();
}

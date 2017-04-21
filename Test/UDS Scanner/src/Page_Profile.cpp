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


// CPage_Profile 消息处理程序

void CPage_Profile::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	m_pBasePage->SetCapValue();
	m_pAdPage->SetCapValue();
	//m_pPaperPage->SetCapValue();
	//m_pFilterPage->SetCapValue();
	m_pSetPage->SetCapValue();

	int index = m_list_profile.GetNextItem(-1,LVIS_SELECTED); 

	CString str;
	str = m_list_profile.GetItemText(index,0); //(i,j)行，列
	if (str.Find("UDS") >= 0 || str.Find("默认模板") >= 0 || str.Find("上次使用") >= 0){}
	else{
		m_pUI->TW_SaveProfileToFile(str.GetBuffer()); //不是上述类型模板时，保存当前选中模板
	}
	m_pUI->TW_SaveProfileToFile("上次使用模板");//再次保存“上次使用模板”
	m_pUI->TW_SaveProfileToFile("模板备份");//保存一份备份

	
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
			m_list_profile.SetItemState(unIndex, LVNI_FOCUSED | LVIS_SELECTED, LVNI_FOCUSED | LVIS_SELECTED); 
			LoadTemplate();
			status = true;	
		}
		unIndex ++;
	}

	if(!status) //没找见上次使用模板
	{
		m_pUI->ResetAllCaps();
		m_list_profile.SetItemState(0, LVNI_FOCUSED | LVIS_SELECTED, LVNI_FOCUSED | LVIS_SELECTED);
	}

	m_pUI->Cancel();
	m_pUI->TW_LoadProfileFromFile("模板备份");

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
	m_list_profile.SetExtendedStyle( LVS_EX_FULLROWSELECT); // 表示选中整行，而非某一列
	//设置扩展风格为：网格形式、整行选取
	m_list_profile.ShowScrollBar(SB_VERT,TRUE); // 只显示垂直滚动条
	CImageList m_image; 
	m_image.Create(1,20,ILC_COLOR32,1,0); 
	m_list_profile.SetImageList(&m_image, LVSIL_SMALL);
	m_list_profile.DeleteAllItems(); // 清空

	CRect rect;   //必须插入列，否则InserItem失败
	m_list_profile.GetClientRect(rect);
	m_list_profile.InsertColumn(0, "", LVCFMT_LEFT, rect.Width());

	m_list_profile.InsertItem(0, "默认模板");
	m_list_profile.SetItemState(0,LVNI_FOCUSED | LVIS_SELECTED, LVNI_FOCUSED | LVIS_SELECTED); //设置行index为选中并高亮（如果未设置Focuse，被选中行会变为灰色）

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
		if(strTemp.Find("模板备份") >= 0){}
		else
		{
			m_list_profile.InsertItem(unIndex, strTemp);
			
			if(strTemp.Find("上次使用") >=0 ) {
				m_list_profile.SetItemState(unIndex, LVIS_FOCUSED|LVIS_SELECTED, LVIS_FOCUSED|LVIS_SELECTED);			
				LoadTemplate();
			}
			unIndex++;
		}
	}
	m_list_profile.SetRedraw(true);  // 重绘
	m_list_profile.Invalidate();
}


void CPage_Profile::LoadTemplate()
{
	UpdateData(TRUE);  // 接收数据
	int nIndex = m_list_profile.GetNextItem(-1,LVIS_SELECTED);

	if(0 == nIndex)  // 默认模板，重置驱动参数
	{
		m_pUI->ResetAllCaps();
	}
	else  // 其它模板
	{	
		CString strProfile; 
		strProfile = m_list_profile.GetItemText(nIndex, 0);
		m_pUI->TW_LoadProfileFromFile(strProfile.GetBuffer()); //会m_pDS->SetGustomDSData
	}

	m_pBasePage->UpdateControls();
	m_pAdPage->UpdateControls();//高级设置界面参数也更新
	m_pSetPage->UpdateControls();	

	UpdateData(FALSE);
}


void CPage_Profile::SetDelete(void)
{
	int nIndex = m_list_profile.GetNextItem(-1,LVIS_SELECTED); 
	CString strCBText; 
	strCBText = m_list_profile.GetItemText(nIndex, 0);
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
	int nIndex = m_list_profile.GetNextItem(-1,LVIS_SELECTED); 

	CString strCBText; 
	strCBText = m_list_profile.GetItemText(nIndex,0);
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

	//是否需要重新创建
	bool recreate = false;
	int renameindex; //重名的序号
	// 判断新建模板名是否已存在
	CString strCombo;  
	for (int i = 0; i < m_list_profile.GetItemCount(); i++)
	{        
		strCombo = m_list_profile.GetItemText(i, 0);
		if(strCombo == strExistName)
		{
			renameindex = i;
			if(AfxMessageBox("模版已存在，您想要重新创建吗？",MB_OKCANCEL) == IDCANCEL)  
			{
				recreate = false;
				return;  // 取消新建同名模版
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
			m_list_profile.InsertItem(m_list_profile.GetItemCount(),strName);//此时m_list_profile.GetItemCount()已经增加
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

	//是否需要重新创建
	bool recreate = false;
	int renameindex; //重名的序号
	// 判断新名称是否已存在
	CString strCombo;  
	int nLength;   
	for (int i = 0; i < m_list_profile.GetItemCount(); i++)
	{       
		strCombo = m_list_profile.GetItemText(i,0);
		if (strCombo == strExistName)
		{
			renameindex = i;
			if(AfxMessageBox("该名称已存在，您想要重新创建吗？",MB_OKCANCEL) == IDCANCEL)  
			{
				recreate = false;
				return;  // 取消重命名模版
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
			m_list_profile.DeleteItem(index); //删除原有选中项	
			m_list_profile.InsertItem(index, strNewName);
			m_list_profile.SetItemState(index, LVNI_FOCUSED | LVIS_SELECTED, LVNI_FOCUSED | LVIS_SELECTED); 
		}
	}
	else
	{
		if(recreate)
		{	
			m_list_profile.DeleteItem(index); //删除原有选中项	
			m_pUI->TW_DeleteProfile(OldName); //还需删除本地原重名模板
			m_list_profile.SetItemState(renameindex, LVNI_FOCUSED | LVIS_SELECTED, LVNI_FOCUSED | LVIS_SELECTED);
		}	
	}

	SetDelete();
	SetRename();
	UpdateData(FALSE);
}


//删除
void CPage_Profile::OnProfile_Btn_Delete()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);  // 接收数据
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


//重置
void CPage_Profile::OnProfile_Btn_Reset()
{
	// TODO: 在此添加控件通知处理程序代码
	m_pUI->ResetAllCaps();
	m_list_profile.SetItemState(0, LVNI_FOCUSED | LVIS_SELECTED, LVNI_FOCUSED | LVIS_SELECTED);
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
			int num = m_list_profile.GetItemCount(); 
			CString strProfile;
			strProfile = fileDlg.GetFileTitle();
			m_list_profile.InsertItem(num,strProfile); //GetFileTitle得到完整的文件名，不包括目录名和扩展名
			m_list_profile.SetItemState(num, LVNI_FOCUSED | LVIS_SELECTED, LVNI_FOCUSED | LVIS_SELECTED);

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

	int index = m_list_profile.GetNextItem(-1,LVIS_SELECTED); 

	CString strTemplate;
	strTemplate = m_list_profile.GetItemText(index,0);

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


void CPage_Profile::OnItemchangeProfile_List_Profile(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	LoadTemplate();	
	SetDelete();
	SetRename();
	*pResult = 0;
}

//#pragma once
#include "stdafx.h"
#include "CTWAINDS_UDS.h"

#ifndef __MFC_UI_H__
#define __MFC_UI_H__

#include "TWAIN_UI.h"
class CSheet_Scanner;
class CDlg_Video;
//class CDlg_Camera;

/**
* @file   MFC_UI.h
* @brief This is a brief description.
* @author UDS
* @par   Copyright (c) 2016 UDS.
*         All Rights Reserved.
* @date   2016/9/12 
* @note   matters needing attention
*/ 
class MFC_UI : public CTWAIN_UI
{
public:
	MFC_UI(CTWAINDS_UDS *pDS);
	~MFC_UI(void);

public:
	
	/**
	* Will show the TWAIN GUI
	* @param[in] _pData contains info about if the UI should be shown etc.
	* @return a valid TWRC_xxxx return code.
	*/
	TW_INT16 DisplayTWAINGUI(TW_USERINTERFACE Data, bool bSetup, bool bIndicators);

	/**
	* Close the user interface for TWAIN
	*/
	void DestroyTWAINGUI();
	void UpdateProgress(bool bShow, unsigned char ucProgress, unsigned int unPageNo, string strProgressTitle);
	unsigned int MessageBox(string strMessage,string strTitle, unsigned int unIconID);
	bool processEvent(pTW_EVENT _pEvent);

	/**
	* @brief 重命名模板名
	*  @param[in]  strOldName 原模板名
	*  @param[in]  strNewName 新模板名   
	*  @retval true  表示成功
	*  @retval false 表示失败  
	*/
	bool RenameProfile(string strOldName, string strNewName); //重命名模板名  add by zhu

	/**
	* @brief 返回模板存放路径   
	*/
	string GetProfileNamePath();
	
	/**
	* @brief 判断“预览”按钮是否可见  
	*/
	void PreViewStatus();


	void CameraCapture();
public:
  CSheet_Scanner* m_pSheet;  ///< 属性页对话框
	CDlg_Video*     m_pDlgVideo;  ///< 基于UDS_Video.ocx控件的摄像头对话框
	//CDlg_Camera*    m_pDlgCamera; ///< 摄像头对话框
};

#endif // __MFC_UI_H__
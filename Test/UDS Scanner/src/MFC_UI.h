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
	* @brief ������ģ����
	*  @param[in]  strOldName ԭģ����
	*  @param[in]  strNewName ��ģ����   
	*  @retval true  ��ʾ�ɹ�
	*  @retval false ��ʾʧ��  
	*/
	bool RenameProfile(string strOldName, string strNewName); //������ģ����  add by zhu

	/**
	* @brief ����ģ����·��   
	*/
	string GetProfileNamePath();
	
	/**
	* @brief �жϡ�Ԥ������ť�Ƿ�ɼ�  
	*/
	void PreViewStatus();


	void CameraCapture();
public:
  CSheet_Scanner* m_pSheet;  ///< ����ҳ�Ի���
	CDlg_Video*     m_pDlgVideo;  ///< ����UDS_Video.ocx�ؼ�������ͷ�Ի���
	//CDlg_Camera*    m_pDlgCamera; ///< ����ͷ�Ի���
};

#endif // __MFC_UI_H__
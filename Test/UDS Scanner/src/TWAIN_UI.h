/***************************************************************************
* Copyright ?2007 TWAIN Working Group:  
*   Adobe Systems Incorporated, AnyDoc Software Inc., Eastman Kodak Company, 
*   Fujitsu Computer Products of America, JFL Peripheral Solutions Inc., 
*   Ricoh Corporation, and Xerox Corporation.
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in the
*       documentation and/or other materials provided with the distribution.
*     * Neither the name of the TWAIN Working Group nor the
*       names of its contributors may be used to endorse or promote products
*       derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY TWAIN Working Group ``AS IS'' AND ANY
* EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL TWAIN Working Group BE LIABLE FOR ANY
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
***************************************************************************/

/**
* @file TWAIN_UI.h
* UI base class - framework independent 
* @author TWAIN Working Group
* @date Nov 2009
*/
#ifndef __TWAIN_UI_H__
#define __TWAIN_UI_H__

#include <list>
/** ģ���ļ���չ�� */  
#define FILEEXTENTION ".DSP"  
#ifdef TWNDS_OS_LINUX
  #define PROFILELOCATION ".UDS/UDS Universal TWAIN DS/"
#elif defined(TWNDS_OS_WIN)
/** ģ���ļ����λ�� */
  #define PROFILELOCATION "UDS/UDS Universal TWAIN DS/"  
#else

#endif
CTWAIN_UI* CreateUI(CTWAINDS_UDS *pDS);
void DestroyUI(CTWAIN_UI* pUI);

typedef list<string> lstString;

class CTWAIN_UI
{
public:
  CTWAIN_UI(CTWAINDS_UDS *pDS);
  ~CTWAIN_UI(){};
  /**
  * Will show the TWAIN GUI
  * @param[in] _pData contains info about if the UI should be shown etc.
  * @return a valid TWRC_xxxx return code.
  */
  virtual TW_INT16 DisplayTWAINGUI(TW_USERINTERFACE Data, bool bSetup, bool bIndicators);

  /**
  * Close the user interface for TWAIN
  */
  virtual void DestroyTWAINGUI();
  virtual void UpdateProgress(bool bShow, unsigned char ucProgress, unsigned int unPageNo, string strProgressTitle);
  virtual void Scan();
  virtual void Cancel();
  virtual void Save();
  virtual unsigned int MessageBox(string strMessage,string strTitle, unsigned int unIconID);
  virtual bool processEvent(pTW_EVENT _pEvent);
  int GetCurrentCapIndex(const TW_UINT16 _unCap);
  const IntVector* GetValidCap(const TW_UINT16 _unCap);
  const FloatVector* GetValidCapFloat(const TW_UINT16 _unCap);
  TW_FRAME GetCurrentFrame();
  bool SetCurrentFrame(float fTop, float fLeft, float fBottom, float fRight);
  bool GetCapRangeFloat(const TW_UINT16 _unCap, float &fMin, float &fMax, float &fStep);
  float GetCapValueFloat(const TW_UINT16 _unCap);
  bool SetCapValueFloat(const TW_UINT16 _unCap, float fVal);
  bool SetCapValueInt(const TW_UINT16 _unCap, int nVal);

	bool GetCapValueBool(const TW_UINT16 _unCap); //zhu

  /**
  * Initialize combobox with profile names
  * @param[in] pcbxProfiles a pointer to CComboBox. 
  * @return true on success
  */
  void TW_GetAllProfiles(lstString& strFileNames);
  /**
  * Save CustomDSdata to file
  * @param[in] strFileName a CString. Name of the file 
  * @param[in] hData a HGLOBAL. Handle to CustomDSdata
  * @param[in] dwDataSize a hData. Size of CustomDSdata
  * @return true on success
  */
  bool TW_SaveProfileToFile(string strFileName);
  /**
  * Load CustomDSdata from file
  * @param[in] strFileName a CString. Name of the file 
  * @param[out] hData a pointer to HGLOBAL. Handle to CustomDSdata
  * @param[out] dwDataSize a pointer to hData. Size of CustomDSdata
  * @return true on success
  */
  bool TW_LoadProfileFromFile(string strFileName);
  /**
  * Delete selected profile from the disk
  * @param[in] pcbxProfiles a pointer to CComboBox. 
  * @return true on success
  */
  bool TW_DeleteProfile(string strFileName);

  bool ResetAllCaps();

	
	BYTE* PreView(); //add by zhu ����Ԥ��������ͼ�����ݡ�
	
	/**
	*  @brief  4�ֽڶ��룬���㲹0
	*  @param[in]  pIn �����ͼƬ����
	*  @param[out]  pOut �����ͼƬ���� 
	*  @param[in] nWidth ͼƬ��������Ϊ��λ 
	*  @param[in] nHeight ͼƬ�ߣ�������Ϊ��λ 
	*  @param[in] nBpp ÿ����ռ�õ�λ��
	*/
	void FillZero(BYTE *pIn, BYTE* pOut, int nWidth, int nHeight,  int nBpp);
	/**
	*  @brief  ����BPP��ȡʹ�õ���ɫ��
	*  @param[in]  nBpp ÿ����ռ�õ�λ�� 
	*  @param[out] dwColors ʹ�õ���ɫ��
	*  @see ConstructBih()
	*  @retval true ��ʾ�ɹ�
	*  @retval false ��ʾʧ�� 
	*/
	bool GetColorsUsed(int nBpp, DWORD& dwColors);

public:
  CTWAINDS_UDS *m_pDS;
  TW_USERINTERFACE m_EnableDSdata;
  bool m_bScanning;
  bool m_bSetup;
  bool m_bIndicators;
  string m_strProfilesDirectory;

	//����BMP�ļ���Ϣͷ
	BITMAPINFOHEADER m_bmpInfoHeader;
	//����BMPλͼ�ļ�ͷ
	BITMAPFILEHEADER m_bmpFileHeader;
	//��ɫ��
	LPRGBQUAD m_bmpLpRGB;
	int m_nDIBSize; //ͼ��buffer�Ĵ�С ���ֽ�Ϊ��λ��
	int m_nBpp; //ÿ��������ռ���ֽ���
};

#endif // __TWAIN_UI_H__

/**
* @file   CTWAINDS_UDS.h
* @brief The main Data Source class.
* This class is derived from the Base class describing a TWAIN DS.
* @author UDS
* @par   Copyright (c):UDS.All Rights Reserved.
* @date  10/26/2016
*/ 

#ifndef __CTWAINDS_UDS_H__
#define __CTWAINDS_UDS_H__

#include "CScanner_G6400.h"  // 一定要放在最前面
#include "CTWAINDS_Base.h"
#include "CScanner_FreeImage.h"
#include "CScanner_OpenCV.h"
#include "Camera_CxImage.h"

/** 数据源接口唯一标识  */
#define kCUSTOMDSGUI "{B4FAF845-1383-4036-AEDC-17C3968188B4}"

const TW_GUID CustomDSGUI = 
{ 0xb4faf845, 0x1383, 0x4036, { 0xae, 0xdc, 0x17, 0xc3, 0x96, 0x81, 0x88, 0xb4 } };

   
typedef struct _CUST_DS_DATA_ELEMENT
{
  DWORD dwSize;
  TW_UINT16 unCapID;  
  TW_UINT16 unCapIdx;
  TW_UINT16 unItemType;
  TW_UINT16 unContType;
  DWORD dwVal[1];
}CUST_DS_DATA_ELEMENT;

class CTWAIN_UI;

/**
* This is the main DS class. It inherits the TWAIN base class, implements all
* the pure virtual functions and manages all of the required capabilities
*/
class CTWAINDS_UDS : public CTWAINDS_Base
{
public:
  CTWAINDS_UDS(TW_IDENTITY AppID);
  ~CTWAINDS_UDS();

  /**
  * Fills the passed in identity structure with our information.
  * @param[in] _idStruct the TW_IDENTITY structure to fill
  */
  void fillIdentityStructure(TW_IDENTITY& _idStruct);

  /**
  * Initialize the Datasource.  Allocate memory for Capabilities.
  * Sets condition code if had problem.
  * @return a valid TWRC_xxxx return code.
  */
  TW_INT16 Initialize();

  /**
  * Get image information.  
  * Implementation of base class pure virtual function.
  * Called when a DG_IMAGE / DAT_IMAGEINFO / MSG_GET op is sent.
  * -If in state 6, general info is provided about the image about to be transferred.
  * -If in state 7, specific info is provided about the current image just transferred.
  * @param[out] _pImageInfo a TW_IMAGEINFO structure to return image information.
  * @return a valid TWRC_xxxx return code.
  */
  TW_INT16 getImageInfo(pTW_IMAGEINFO _pImageInfo);

  /**
  * Open the Data Source.
  * Implementation of base class pure virtual function.
  * Called when a DG_CONTROL / DAT_IDENTITY / MSG_OPENDS op is sent.
  * @param[in] _pOrigin a pointer to TW_IDENTITY structure of the Application identity.
  * @return a valid TWRC_xxxx return code.
  */
  TW_INT16 openDS(pTW_IDENTITY  _pOrigin);

  /**
  * Close the Data Source.
  * Implementation of base class pure virtual function.
  * Called when a DG_CONTROL / DAT_IDENTITY / MSG_CLOSEDS op is sent.
  * @return a valid TWRC_xxxx return code.
  */
  TW_INT16 closeDS();

  /**
  * Process events.
  * Implementation of base class pure virtual function.
  * Called when a DG_CONTROL / DAT_EVENT / MSG_PROCESSEVENT op is sent.
  * @param[in] _pEvent a pointer to TW_EVENT structure.
  * @return a valid TWRC_xxxx return code.
  */
  TW_INT16 processEvent(pTW_EVENT _pEvent);

  /**
  * Stop currect transfer if not done.  Single from application that application is 
  * done with all data with current image.
  * Check to see if there is still documents or data remaining to transfer.
  * Implementation of base class pure virtual function.
  * Called when a DG_CONTROL / DAT_PENDINGXFERS / MSG_ENDXFER op is sent.
  * @param[out] _pXfers a pointer to TW_PENDINGXFERS structure.
  * @return a valid TWRC_xxxx return code.
  */
  TW_INT16 endXfer(pTW_PENDINGXFERS _pXfers);

  /**
  * Check to see if there is still documents or data remaining to transfer.
  * Called when a DG_CONTROL / DAT_PENDINGXFERS / MSG_GET op is sent.
  * @param[out] _pXfers a pointer to TW_PENDINGXFERS structure.
  * @return a valid TWRC_xxxx return code.
  */
  TW_INT16 getXfer(pTW_PENDINGXFERS _pXfers);

  /**
  * Flush all pending transfers from the Source..
  * Called when a DG_CONTROL / DAT_PENDINGXFERS / MSG_RESET op is sent.
  * @param[out] _pXfers a pointer to TW_PENDINGXFERS structure.
  * @return a valid TWRC_xxxx return code.
  */
  TW_INT16 resetXfer(pTW_PENDINGXFERS _pXfers);

  /**
  * Called by the base class when the data source is enabled.
  * @param[in] _pData contains info about if the UI should be shown etc.
  * @return a valid TWRC_xxxx return code.
  */
  TW_INT16 enableDS(pTW_USERINTERFACE _pData);

  /**
  * Enable the Data Source in setup mode.
  * Called when a DG_CONTROL / DAT_USERINTERFACE / MSG_ENABLEDS op is sent.
  * @return a valid TWRC_xxxx return code.
  */
  TW_INT16 enableDSOnly();

  /**
  * Called by the base class when the data source is disabled.
  * @param[in] _pData a pointer to a TW_USERINTERFACE struct.
  * @return a valid TWRC_xxxx return code.
  */
  TW_INT16 disableDS(pTW_USERINTERFACE _pData);


  /**
  * handles DAT_IMAGELAYOUT requests
  * @param[in] _MSG the message to handle.
  * @param[in] _pData a pointer to TW_IMAGELAYOUT structure.
  * @return a valid TWRC_xxxx return code.
  */
  TW_INT16 dat_imagelayout(TW_UINT16 _MSG, pTW_IMAGELAYOUT _pData);

  /**
  * Called by the base class to when the application wants to get memory transfer data.
  * @param[in] _pData filled with buffer size data
  * @return a valid TWRC_xxxx return code.
  */
  TW_INT16 getMemoryXfer(pTW_SETUPMEMXFER _pData);

  /**
  * Transfer image data from scanner to memory.  
  * Called during one of the transfer methods.
  * @return a valid TWRC_xxxx return code.
  */
  TW_INT16 transfer();

  /**
  * Return a CTWAINContainer for requested capability.
  * @param[in] _unCap the ICAP_xxxx that is requested.
  * @return a CTWAINContainer for the capability.
  */
  CTWAINContainer* findCapability(const TW_UINT16 _unCap);

  /**
  * Request the DSM the most likly user clicks on the 揙K?button on GUI.
  * @return true if successful.
  */
  virtual bool DoCloseDSOkEvent();
  
  /**
  * Updates the scanner from the current caps.
  * @return true if successful.
  */
  bool updateScannerFromCaps();

  /**
  * Convenience function that will get the proper ICAP_BITDEPTH container
  * @return CTWAINContainer BitDepth for current PixelType.
  */
  CTWAINContainer* getICAP_BITDEPTH();
  
  bool StartScanning();
  bool StopScanning(){m_bCanceled = true; return true;};
  bool ReadCustomDSdata(stringstream &DsData);
  bool StoreCustomDSdata(stringstream &DsData);
  bool StoreCapInStream(stringstream &_DsData, TW_UINT16 _unCapID, TW_UINT16 _unCapIdx, TW_UINT16 unContType);
  bool ReadCapFromStream(stringstream &_DsData, TW_UINT16 _unCapID, TW_UINT16 _unCapIdx);

  /**
  * Get Gustom DS data
  * @param[out] _pDSData a pointer to a TW_CUSTOMDSDATA structure.
  * @return a valid TWRC_xxxx return code, TWRC_SUCCESS on success.
  */
  TW_INT16 GetGustomDSData(pTW_CUSTOMDSDATA _pDSData);

  /**
  * Set Gustom DS data
  * @param[in] _pDSData a pointer to a TW_CUSTOMDSDATA structure.
  * @return a valid TWRC_xxxx return code, TWRC_SUCCESS on success.
  */
  TW_INT16 SetGustomDSData(pTW_CUSTOMDSDATA _pDSData);

  /**
  * Validate the value being used to set a capability.  Ranges and enums can be tested
  * by the capability but OneValues might have only some values that are acceptable.
  * Override this function in base class to support more capabilities
  * @param[in] Cap the Capability ID
  * @param[in] ConType the container type 
  * @param[in] _pCap a pointer to BYTE. Pointer to Cap container
  * @return a valid TWRC_xxxx return code.
  */
  TW_INT16 validateCapabilitySet(TW_UINT16 _Cap, TW_UINT16  _ConType, BYTE* _pContainer);

	///** 
	//* @brief 设置单张待扫图片路径
	//* @param[in] szImagePath 图片路径
	//*/
	//void SetScannerImagePath_Signal(char* szImagePath);

	/**
	*  @brief  设置多张待扫图片路径
	*  @param[in]  vector_string_imagepath 图片路径
	*/
	void SetScannerImagePath_Multi(vector<string> vector_string_imagepath);

	/**
	*  @brief  从INI文件读取图片数量及路径(未完...) 
	*  @todo
	*/
	//void GetImagePathFromINI();

protected:
	//CScanner_FreeImage        m_Scanner;                 /**< The main scanner. */
	//CScanner_G6400            m_Scanner;                 /**< The main scanner. */   
	CDevice_Base               *m_pScanner;               /**< The pointer of main scanner. */         

  TWAINCapabilitiesMap_int    m_BitDepthMap;             /**< Capability for various Bit Depths */
  TWAINCapabilitiesMap_FIX32  m_ICAP_UNIT_Dependant;     /**< Capability for any Fix32 based TWAIN container */
  CTWAINContainerFrame       *m_pICAP_FRAMES;            /**< Capabiltiy for a FRAMES based container */
  TW_IDENTITY m_AppID;
  bool                        m_bCanceled;
  CTWAIN_UI                  *m_pGUI;                   /**< This is the main  UI dialog */

};


#endif // __CTWAINDS_UDS_H__


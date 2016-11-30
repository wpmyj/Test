#ifndef __CCAMERA_DIRECTX_H__
#define __CCAMERA_DIRECTX_H__
#include "CScanner_base.h"
class CCamera_DirectX : public CScanner_Base
{
public:
	CCamera_DirectX(void);
	~CCamera_DirectX(void);

	/**
  * Resets the scanner to factory default settings.
  * Sets the scanners caps back to defaults.
  * @return true if successfully reset.
  */
  bool resetScanner();

  /**
  * Determine if there is paper sitting in the feeder.
  * IF empty loads again for next time.
  * @return true if paper in feeder else return false.
  */
  bool isFeederLoaded();

	/**
  * Retrieves the image to scan from disk into memory.
  * @return true if image was loaded into memory successfully.
  */
  bool acquireImage() {return true;}

protected:
	/**
	*  @brief  ��ȡͼƬ����.
	*  @note ����ɨ����G6400ʹ��
	*  @param[out] buffer ����ͼƬ���ݵ�buffer.
	*  @param[out] dwReceived �����ʵ�ʴ�С�����ֽ�Ϊ��λ��
	*/
	void GetImageData(BYTE *buffer, DWORD &dwReceived);

public:
	/**
	*  @brief  ����ͼƬ����
	*  @param[in]  buffer  ����ͼƬ���ݵ�buffer
	*  @param[in] dwSize buffer��С
	*  @retval true ��ʾ�ɹ�
	*  @retval false ��ʾʧ��
	*/
	bool SetImageData(BYTE *buffer, DWORD dwSize);

protected:
	short             m_nDocCount;              /**< number of documents waiting to transfer */
	short             m_nMaxDocCount;           /**< Max number of documents waiting to transfer */
	WORD              m_nSourceWidth;           /**< Width of image in FreeImage */
	WORD              m_nSourceHeight;          /**< Height of image in FreeImage */
	char              m_szIniPath[PATH_MAX];    /**< INI�����ļ�·�� */   
	BYTE*             m_pImageBuffer;           /**< ͼƬ����ָ�� */
	DWORD             m_dwSize;                 /**< ͼƬ���ݴ�С */
};

#endif  __CCAMERA_DIRECTX_H__

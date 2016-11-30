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
	*  @brief  获取图片数据.
	*  @note 仅供扫描仪G6400使用
	*  @param[out] buffer 接收图片数据的buffer.
	*  @param[out] dwReceived 传输的实际大小（以字节为单位）
	*/
	void GetImageData(BYTE *buffer, DWORD &dwReceived);

public:
	/**
	*  @brief  设置图片数据
	*  @param[in]  buffer  存有图片数据的buffer
	*  @param[in] dwSize buffer大小
	*  @retval true 表示成功
	*  @retval false 表示失败
	*/
	bool SetImageData(BYTE *buffer, DWORD dwSize);

protected:
	short             m_nDocCount;              /**< number of documents waiting to transfer */
	short             m_nMaxDocCount;           /**< Max number of documents waiting to transfer */
	WORD              m_nSourceWidth;           /**< Width of image in FreeImage */
	WORD              m_nSourceHeight;          /**< Height of image in FreeImage */
	char              m_szIniPath[PATH_MAX];    /**< INI配置文件路径 */   
	BYTE*             m_pImageBuffer;           /**< 图片数据指针 */
	DWORD             m_dwSize;                 /**< 图片数据大小 */
};

#endif  __CCAMERA_DIRECTX_H__

#pragma once

/**< 引用qedit.h时出现无法打开包括文件:"dxtrans.h"的错误，需加上下面几句  */
#pragma include_alias("dxtrans.h", "qedit.h")   ///<编译器在编译过程中会将严格将dxtrans.h替换成qedit.h
#define __IDxtCompositor_INTERFACE_DEFINED__
#define __IDxtAlphaSetter_INTERFACE_DEFINED__
#define __IDxtJpeg_INTERFACE_DEFINED__
#define __IDxtKey_INTERFACE_DEFINED__


#include "Dshow.h"    //DirectX 11
#include "qedit.h"

#define WM_IMAGEREADY (WM_USER + 1)
#define WM_IMAGESAVED (WM_USER + 2)

// CUDSCapture

typedef struct
{
	long grayT;                        /**< 最大灰度差 */
	long colorT;                       /**< 最大色彩差 */
	long moveRatio;                    /**< 最小移动比例 */
	long edgeWidth;                    /**< 边缘宽度，用来检查手 */
	long edgePixel;                    /**< 最大边缘像素 */
	long stayPixel;                    /**< 最大不稳定像素 */
	long stayFrame;                    /**< 最小稳定帧数 */
	long imageType;                    /**< 0-黑白  1-灰度  2-彩色 */
	long imageOrientation;             /**< 调整图像方向: 0-0, 1-90, 2-180, 3-270 */
	bool fastPreview;                  /**< 是否采用快速预览*/
	bool autoClip;                     /**< 是否智能裁切旋转拍摄*/
	bool autoRotate;                   /**< 是否自动顺时针转90度 */
	bool autoEnhance;                  /**< 是否进行文档增强 */
	long docSize;                      /**< 手动拍摄时的文档尺寸*/
	long docWidth, docHeight;          /**< 自定义手动拍摄区域*/
  long threshold;                    /**< 阈值 */
	long XDPI;                         /**< 水平方向分辨率 */ 
	long YDPI;                         /**< 垂直方向分辨率 */
	CString strCamrea;                 /**< 默认摄像机*/
	CString strSize;                   /**< 默认图像帧尺寸*/
} AUTOPARAMETER; 



class CUDSCapture : public CWnd
{
	DECLARE_DYNAMIC(CUDSCapture)

public:
	CUDSCapture();
	virtual ~CUDSCapture();

public:

	/** 
	Provides methods to build Filter Graph(by Filter Graph Manager)
	*/
	IGraphBuilder *m_pGB;  

	/**
	* Provides methods for controlling the flow of data through the filter
	* graph(by Filter Graph Manager)
	*/
	IMediaControl *m_pMC; 

	/**
	* Sets properties on the video window(by Filter Graph Manager)
	*/
	IVideoWindow *m_pVW;  

	/**
	* Provides methods for building capture graph and other custom filter graph
	*/
	ICaptureGraphBuilder2 *m_pCapture; 

	/**
	* Provides methods for controlling Camera Filter
	*/
	IBaseFilter *m_pCameraFilter;  

	IBaseFilter *m_pGrabberFilter;

	ISampleGrabber *m_pGrabber;

	IMediaEventEx *m_pEvent;


	HWND m_hDlgWnd, m_hPreviewWnd;                                  /**< 对话框句柄，预览窗口句柄 */                         
	bool m_bActive;                                                 /**< 是否激活 */ 
	bool m_bIsPreview;                                              /**< 是否预览 */ 
	bool m_bCaptureBackground;                                      /**< 是否捕获背景 */
	bool m_bIsAutoClipping;                                         /**< 是否智能抓拍 */
	bool m_bIsBackground;
	AUTOPARAMETER m_Auto;
	CString m_strMinSize, m_strMaxSize;
	CString m_strBarcodeType, m_strBarcodeFormat;
	long m_nBarcodeLength;
	bool m_bMultiBarcode, m_bBarcodeRotate;
	CString m_strImagePath, m_strBarcode;
	long m_nQuality, m_nPhotoNo;
	
	//视屏设置
	long m_nBrightness;                      /**< 亮度 */
	long m_nContrast;                        /**< 对比度 */
	long m_nHue;                             /**< 色调 */
	long m_nSaturation;                      /**< 饱和度 */
	long m_nSharpness;                       /**< 锐化 */
	long m_nGamma;                           /**< Gamma校正 */
	long m_nWhiteBalance;                    /**< 白平衡 */
	long m_nBacklightCompensation;           /**< 逆光对比 */
	long m_nGain;                            /**< 增益 */


	//照相机控制
	long m_nZoom;                            /**< 缩放 */
	long m_nFocus;                           /**< 焦点 */
	long m_nExposure;                        /**< 曝光度 */
	long m_nIris;                            /**< 光圈（十字光栅） */
	long m_nPan;                             /**< 全景 */
	long m_nTilt;                            /**< 倾斜 */
	long m_nRoll;                            /**< 滚动 */

	// Implementation
public:

		/**
	  * @brief 创建未初始化的Com对象
	  */
	  bool CreateComInstance();

///<以下为视屏图像设置

		/**
		* @brief 获取图片亮度
		* @param[in] *pSlider   滑块关联变量指针.
		* @retval  true 表示成功，false 表示失败 
		*/
  	bool GetBrightness(CSliderCtrl *pSlider);              

		/**
		* @brief 设置图片亮度
		* @param[in] *pSlider   滑块关联变量指针.
		* @retval  true 表示成功，false 表示失败 
		*/
  	bool SetBrightness(CSliderCtrl *pSlider);

		/**
		* @brief 获取图片对比度
		* @param[in] *pSlider   滑块关联变量指针.
		* @retval  true 表示成功，false 表示失败 
		*/
		bool GetContrast(CSliderCtrl *pSlider);

		/**
		* @brief 设置图片对比度
		* @param[in] *pSlider   滑块关联变量指针.
		* @retval  true 表示成功，false 表示失败 
		*/
		bool SetContrast(CSliderCtrl *pSlider);

		/**
		* @brief 获取图片色相
		* @param[in] *pSlider   滑块关联变量指针.
		* @retval  true 表示成功，false 表示失败 
		*/
		bool GetHue(CSliderCtrl *pSlider);

		/**
		* @brief 设置图片色相
		* @param[in] *pSlider   滑块关联变量指针.
		* @retval  true 表示成功，false 表示失败 
		*/
		bool SetHue(CSliderCtrl *pSlider);

		/**
		* @brief 获取图片饱和度
		* @param[in] *pSlider   滑块关联变量指针.
		* @retval  true 表示成功，false 表示失败 
		*/
		bool GetSaturation(CSliderCtrl *pSlider);

		/**
		* @brief 设置图片饱和度
		* @param[in] *pSlider   滑块关联变量指针.
		* @retval  true 表示成功，false 表示失败 
		*/
		bool SetSaturation(CSliderCtrl *pSlider);

		/**
		* @brief 获取图片锐化度
		* @param[in] *pSlider   滑块关联变量指针.
		* @retval  true 表示成功，false 表示失败 
		*/
		bool GetSharpness(CSliderCtrl *pSlider);

		/**
		* @brief 设置图片锐化度
		* @param[in] *pSlider   滑块关联变量指针.
		* @retval  true 表示成功，false 表示失败 
		*/
		bool SetSharpness(CSliderCtrl *pSlider);

		/**
		* @brief 获取图片Gamma校正比例
		* @param[in] *pSlider   滑块关联变量指针.
		* @retval  true 表示成功，false 表示失败 
		*/
		bool GetGamma(CSliderCtrl *pSlider);

		/**
		* @brief 设置图片Gamma校正比例
		* @param[in] *pSlider   滑块关联变量指针.
		* @retval  true 表示成功，false 表示失败 
		*/
		bool SetGamma(CSliderCtrl *pSlider);

		/**
		* @brief 获取图片白平衡
		* @param[in] *pSlider   滑块关联变量指针.
		* @retval  true 表示成功，false 表示失败 
		*/
		bool GetWhiteBalance(CSliderCtrl *pSlider);

		/**
		* @brief 设置图片白平衡
		* @param[in] *pSlider   滑块关联变量指针.
		* @retval  true 表示成功，false 表示失败 
		*/
		bool SetWhiteBalance(CSliderCtrl *pSlider);

		/**
		* @brief 获取图片逆光对比
		* @param[in] *pSlider   滑块关联变量指针.
		* @retval  true 表示成功，false 表示失败 
		*/
		bool GetBacklightCompensation(CSliderCtrl *pSlider);

		/**
		* @brief 设置图片逆光对比
		* @param[in] *pSlider   滑块关联变量指针.
		* @retval  true 表示成功，false 表示失败 
		*/
		bool SetBacklightCompensation(CSliderCtrl *pSlider);

			/**
		* @brief 获取图片增益
		* @param[in] *pSlider   滑块关联变量指针.
		* @retval  true 表示成功，false 表示失败 
		*/
		bool GetGain(CSliderCtrl *pSlider);

		/**
		* @brief 设置图片增益
		* @param[in] *pSlider   滑块关联变量指针.
		* @retval  true 表示成功，false 表示失败 
		*/
		bool SetGain(CSliderCtrl *pSlider);

///<以下为照相机控制

		/**
		* @brief 获取照相机缩放比
		* @param[in] *pSlider   滑块关联变量指针.
		* @retval  true 表示成功，false 表示失败 
		*/
  	bool GetZoom(CSliderCtrl *pSlider);

		/**
		* @brief 设置照相机缩放比
		* @param[in] *pSlider   滑块关联变量指针.
		* @retval  true 表示成功，false 表示失败 
		*/
  	bool SetZoom(CSliderCtrl *pSlider); 

		/**
		* @brief 获取照相机焦点
		* @param[in] *pSlider   滑块关联变量指针.
		* @retval  true 表示成功，false 表示失败 
		*/
  	bool GetFocus(CSliderCtrl *pSlider);

		/**
		* @brief 设置照相机焦点
		* @param[in] *pSlider   滑块关联变量指针.
		* @retval  true 表示成功，false 表示失败 
		*/
  	bool SetFocus(CSliderCtrl *pSlider); 

    /**
		* @brief 获取照相机曝光度
		* @param[in] *pSlider   滑块关联变量指针.
		* @retval  true 表示成功，false 表示失败 
		*/
  	bool GetExposure(CSliderCtrl *pSlider);

		/**
		* @brief 设置照相机曝光度
		* @param[in] *pSlider   滑块关联变量指针.
		* @retval  true 表示成功，false 表示失败 
		*/
  	bool SetExposure(CSliderCtrl *pSlider); 

		/**
		* @brief 获取照相机光圈（十字光栅）
		* @param[in] *pSlider   滑块关联变量指针.
		* @retval  true 表示成功，false 表示失败 
		*/
  	bool GetIris(CSliderCtrl *pSlider);

		/**
		* @brief 设置照相机光圈（十字光栅）
		* @param[in] *pSlider   滑块关联变量指针.
		* @retval  true 表示成功，false 表示失败 
		*/
  	bool SetIris(CSliderCtrl *pSlider); 

		/**
		* @brief 获取照相机全景
		* @param[in] *pSlider   滑块关联变量指针.
		* @retval  true 表示成功，false 表示失败 
		*/
  	bool GetPan(CSliderCtrl *pSlider);

		/**
		* @brief 设置照相机全景
		* @param[in] *pSlider   滑块关联变量指针.
		* @retval  true 表示成功，false 表示失败 
		*/
  	bool SetPan(CSliderCtrl *pSlider); 

		/**
		* @brief 获取照相机倾斜度
		* @param[in] *pSlider   滑块关联变量指针.
		* @retval  true 表示成功，false 表示失败 
		*/
  	bool GetTilt(CSliderCtrl *pSlider);

		/**
		* @brief 设置照相机倾斜度
		* @param[in] *pSlider   滑块关联变量指针.
		* @retval  true 表示成功，false 表示失败 
		*/
  	bool SetTilt(CSliderCtrl *pSlider); 

			/**
		* @brief 获取照相机滚动角
		* @param[in] *pSlider   滑块关联变量指针.
		* @retval  true 表示成功，false 表示失败 
		*/
  	bool GetRoll(CSliderCtrl *pSlider);

		/**
		* @brief 设置照相机滚动角
		* @param[in] *pSlider   滑块关联变量指针.
		* @retval  true 表示成功，false 表示失败 
		*/
  	bool SetRoll(CSliderCtrl *pSlider); 


		/**
		* @brief 配置引脚属性
		* @param[in] hParentWnd   主对话框句柄.
		* @retval  true 表示成功，false 表示失败 
		*/
  	bool ConfigCameraPin(HWND hParentWnd);

		/**
		* @brief 配置滤波器
		* @param[in] hParentWnd   主对话框句柄.
		* @retval  true 表示成功，false 表示失败 
		*/
  	bool ConfigCameraFilter(HWND hParentWnd);

		/**
		* @brief 查找本机连接的拍摄设备列表
		* @param[in] pCamList   设备列表组合框关联变量指针.
		* @retval -1表示失败 ,否则返回连接的设备数目 
		*/
  	long GetDevicesList(CComboBox *pCamList);     

		/**
		* @brief                选择速拍仪
		* @param[in] devNo      设备编号.
		* @param[in] pSizeListt 像素列表组合框关联变量指针.
		* @retval true 表示成功，false 表示失败
		*/
	  bool SelectCamera(int devNo, CComboBox *pSizeList);    

		/**
		* @brief                      是否授权
		* @param[in] devName          设备名称.
		* @retval true 表示成功，false 表示失败
		*/
  	bool IsAuthorizedCamera(CString devName);              

		/**
		* @brief 设置拍摄仪
		* @retval true 表示成功 
		* @retval false 表示失败  
		*/
	  bool SetupCamera();  

		/**
		* @brief 停止拍照 
		*/
		void StopCamera();                                     

		/**
		* @brief 释放资源
		*/
		void ReleaseAll();                                       


protected:
	DECLARE_MESSAGE_MAP()
};



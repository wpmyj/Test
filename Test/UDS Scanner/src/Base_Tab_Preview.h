#pragma once

#include "Page_Custom.h"
#include "MFC_UI.h"

// CBase_Tab_Preview 对话框

class CBase_Tab_Preview : public CDialogEx, public CPage_Custom
{
	DECLARE_DYNAMIC(CBase_Tab_Preview)

public:
	CBase_Tab_Preview(MFC_UI *pUI);   // 标准构造函数
	virtual ~CBase_Tab_Preview();

// 对话框数据
	enum { IDD = IDD_BASETAB_PREVIEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

private:
	/**
	* @brief 在构造函数中初始化 
	* @see  CPage_Base(MFC_UI *pUI)
	*/
	MFC_UI  *m_pUI; 


public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();

public:
	CRectTracker m_rectTracker; ///<橡皮筋类
	CRect m_tRect; ///<定义、保存橡皮筋框的矩形
	TCHAR m_bmpFilePath[PATH_MAX]; ///<预览图保存路径
	CRect m_endrect; ///<最终图像需要在预览框显示的区域大小

public:
	void PreView();  //预览按钮

private:
	void UpdatePicRectangle(int index, int unitindex, int xpos, int ypos);

	void DrawImage(void); ///<OnPaint中画图，需要时刷新即可
	void GetBmpFilePath(); ///<为成员变量m_bmpFilePath赋值
	bool GetTempSavePath(TCHAR* pszPath); ///<获取临时文件夹
	bool CreateDir(const CString& strPath); ///<创建文件夹
	//DrawToHdc系列函数
	void DrawToHDC(HDC hDCDst, RECT* pDstRect, IplImage* img); //在指定dDCDst根据pDstRect绘图。
	void FillBitmapInfo(BITMAPINFO* bmi, int width, int height, int bpp, int origin); ///<为bmi写入信息头和调色板。
	void Show(IplImage* img, HDC dc, int x, int y, int w, int h, int from_x, int from_y);	 ///<使用DIB位图和颜色数据对与目标设备环境相关的设备在dc上的指定矩形中的像素进行设置
	CvRect RectToCvRect(RECT sr); ///<将Rect类型转为CvRect类型
	RECT NormalizeRect(RECT r); ///<标准化处理输入rect，防止左侧坐标大于右侧坐标，上侧坐标大于下侧坐标

};

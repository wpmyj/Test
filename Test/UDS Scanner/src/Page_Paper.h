#pragma once

#include "MFC_UI.h"
#include "Page_Custom.h"

class MFC_UI;
// CPage_Paper 对话框

class CPage_Paper : public CPropertyPage, public CPage_Custom
{
	DECLARE_DYNAMIC(CPage_Paper)

public:
	CPage_Paper(MFC_UI *pUI);
	virtual ~CPage_Paper();

// 对话框数据
	enum { IDD = IDD_PROPPAGE_PAPER };

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
	void SetCapValue();
	void UpdateControls(void);  ///< 更新控件状态
	void UpdatePicRectangle(int index, int unitindex, int xpos, int ypos); ///<根据选择的纸张对应序号，更新图片控件上的矩形区域。
	void SetMultistream(void);
	void SetCapMulti(); //设置多流生效
	void InitPapermap(void);

	void PreView();  //预览按钮
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

private:
	TCHAR m_bmpFilePath[PATH_MAX]; ///<预览图保存路径
	CRect m_endrect; ///<最终图像需要在预览框显示的区域大小

	MAP_CAP m_papermap;  ///<用于保存参数改变后的值

	CRectTracker m_rectTracker; ///<橡皮筋类
	CRect m_tRect; ///<定义、保存橡皮筋框的矩形

private:
	virtual BOOL OnInitDialog();
	virtual BOOL OnSetActive();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg void OnPaper_Btn_Check_FrontColor();
	afx_msg void OnPaper_Btn_Check_FrontGray();
	afx_msg void OnPaper_Btn_Check_FrontBw();
	afx_msg void OnPaper_Btn_Check_BackBw();
	afx_msg void OnPaper_Btn_Check_BackColor();
	afx_msg void OnPaper_Btn_Check_BackGray();

private:
	CComboBox m_combo_uints;
	CButton m_check_frontcolor;
	CButton m_check_frontbw;
	CButton m_check_frontgray;
	CButton m_check_backgray;
	CButton m_check_backcolor;
	CButton m_check_backbw;
	
	CButton m_btn_frontcolor;
	CButton m_btn_frontbw;
	CButton m_btn_frontgray;
	CButton m_btn_backgray;
	CButton m_btn_backcolor;
	CButton m_btn_backbw;
public:
	afx_msg void OnCbnSelchangePaper_Combo_Uints();
};

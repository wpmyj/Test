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
	void SetCapValue(void);  ///<设置参数
	void UpdateControls(void);  ///< 更新控件状态
	int FindPaperSize(int index); ///<寻找index对应的纸张大小,返回index对应的纸张大小
	int FindUnit(int index); ///<寻找index对应的单位,返回index对应的单位 
	void SetScroll(); ///<根据edit的值更新滚动条的值;以及根据单位设定宽、高滚动条的范围;
	void InitSliderCtrl();  ///< 初始化滑动条控件
	void UpdatePicRectangle(int index, int unitindex, int xpos, int ypos); ///<根据选择的纸张对应序号，更新图片控件上的矩形区域。
	void SetPaperSize(void);

	void SetXYPos(void); ///<设置XY偏移量是否可用。

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

	//int m_num[6]; //记录6个edit进入的次数
	MAP_CAP m_papermap;  ///<用于保存参数改变后的值

	CRectTracker m_rectTracker; ///<橡皮筋类
	CRect m_tRect; ///<定义、保存橡皮筋框的矩形

	CComboBox m_combo_standardsizes; ///<纸张大小: US Letter/US Legal/A4/A5
	CComboBox m_combo_uints; ///<单位：英尺，像素，毫米
	
	/*int m_standarindex; ///<存最开始纸张大小选中项的索引值
	int m_papervalue; ///<存最开始纸张大小选中的值
	int m_unitindex; ///<存最开始单位选中项的索引值
	int m_unitvalue; ///<存最开始单位选中的值*/
	int m_resolution; ///<当前分辨率

	CEdit m_edit_width;
	CEdit m_edit_height;
	CScrollBar m_scroll_width;
	CScrollBar m_scroll_height;

	//英寸单位下，六者的最大取值，使用时需要根据单位转换
	int maxinches_xpos;
	int maxinches_ypos;
	int maxinches_updown;
	int maxinches_leftright;
	int maxinches_width;
	int maxinches_height;

	CEdit m_edit_xpos;
	CEdit m_edit_ypos;
	CScrollBar m_scroll_xpos;
	CScrollBar m_scroll_ypos;

	CEdit m_edit_up;
	CEdit m_edit_down;
	CEdit m_edit_left;
	CEdit m_edit_right;
	CScrollBar m_scroll_up;
	CScrollBar m_scroll_down;
	CScrollBar m_scroll_right;
	CScrollBar m_scroll_left;

	CComboBox m_combo_compress;
	CSliderCtrl m_slider_compressvalue;
	CEdit m_edit_compressvalue;

private:
	virtual BOOL OnInitDialog();
	virtual BOOL OnSetActive();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

private:
	afx_msg void OnCbnSelchangePaper_Combo_Standardsizes();
	afx_msg void OnCbnSelchangePaper_Combo_Uints();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	afx_msg void OnCbnSelchangePaper_Combo_Compress();
	afx_msg void OnEnChangeBase_Edit_Compressvalue();
	afx_msg void OnNMCustomdrawPaper_Slider_Compressionvalue(NMHDR *pNMHDR, LRESULT *pResult);
	
	afx_msg void OnEnChangeBase_Edit_EdgeRight();
	afx_msg void OnEnChangeBase_Edit_EdgeUp();
	afx_msg void OnEnChangeBase_Edit_EdgeLeft();
	afx_msg void OnEnChangeBase_Edit_EdgeDown();
	afx_msg void OnEnChangeBase_Edit_EdgeXpos();
	afx_msg void OnEnChangeBase_Edit_EdgeYpos();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

#pragma once

#include "MFC_UI.h"
#include "Page_Custom.h"

class MFC_UI;
// CPage_Paper �Ի���

class CPage_Paper : public CPropertyPage, public CPage_Custom
{
	DECLARE_DYNAMIC(CPage_Paper)

public:
	CPage_Paper(MFC_UI *pUI);
	virtual ~CPage_Paper();

// �Ի�������
	enum { IDD = IDD_PROPPAGE_PAPER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

private:
	/**
	* @brief �ڹ��캯���г�ʼ�� 
	* @see  CPage_Base(MFC_UI *pUI)
	*/
	MFC_UI  *m_pUI; 

public:
	void SetCapValue();
	void UpdateControls(void);  ///< ���¿ؼ�״̬
	void UpdatePicRectangle(int index, int unitindex, int xpos, int ypos); ///<����ѡ���ֽ�Ŷ�Ӧ��ţ�����ͼƬ�ؼ��ϵľ�������
	void SetMultistream(void);
	void SetCapMulti(); //���ö�����Ч
	void InitPapermap(void);

	void PreView();  //Ԥ����ť
	void DrawImage(void); ///<OnPaint�л�ͼ����Ҫʱˢ�¼���
	void GetBmpFilePath(); ///<Ϊ��Ա����m_bmpFilePath��ֵ
	bool GetTempSavePath(TCHAR* pszPath); ///<��ȡ��ʱ�ļ���
	bool CreateDir(const CString& strPath); ///<�����ļ���
	//DrawToHdcϵ�к���
	void DrawToHDC(HDC hDCDst, RECT* pDstRect, IplImage* img); //��ָ��dDCDst����pDstRect��ͼ��
	void FillBitmapInfo(BITMAPINFO* bmi, int width, int height, int bpp, int origin); ///<Ϊbmiд����Ϣͷ�͵�ɫ�塣
	void Show(IplImage* img, HDC dc, int x, int y, int w, int h, int from_x, int from_y);	 ///<ʹ��DIBλͼ����ɫ���ݶ���Ŀ���豸������ص��豸��dc�ϵ�ָ�������е����ؽ�������
	CvRect RectToCvRect(RECT sr); ///<��Rect����תΪCvRect����
	RECT NormalizeRect(RECT r); ///<��׼����������rect����ֹ�����������Ҳ����꣬�ϲ���������²�����

private:
	TCHAR m_bmpFilePath[PATH_MAX]; ///<Ԥ��ͼ����·��
	CRect m_endrect; ///<����ͼ����Ҫ��Ԥ������ʾ�������С

	MAP_CAP m_papermap;  ///<���ڱ�������ı���ֵ

	CRectTracker m_rectTracker; ///<��Ƥ����
	CRect m_tRect; ///<���塢������Ƥ���ľ���

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

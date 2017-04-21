#pragma once

#include "Page_Custom.h"
#include "MFC_UI.h"

// CBase_Tab_Preview �Ի���

class CBase_Tab_Preview : public CDialogEx, public CPage_Custom
{
	DECLARE_DYNAMIC(CBase_Tab_Preview)

public:
	CBase_Tab_Preview(MFC_UI *pUI);   // ��׼���캯��
	virtual ~CBase_Tab_Preview();

// �Ի�������
	enum { IDD = IDD_BASETAB_PREVIEW };

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
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();

public:
	CRectTracker m_rectTracker; ///<��Ƥ����
	CRect m_tRect; ///<���塢������Ƥ���ľ���
	TCHAR m_bmpFilePath[PATH_MAX]; ///<Ԥ��ͼ����·��
	CRect m_endrect; ///<����ͼ����Ҫ��Ԥ������ʾ�������С

public:
	void PreView();  //Ԥ����ť

private:
	void UpdatePicRectangle(int index, int unitindex, int xpos, int ypos);

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

};

#pragma once

#include "MFC_UI.h"
#include "afxwin.h"
#include "Dlg_Profile.h"
#include "Page_Custom.h"

class MFC_UI;
// CPage_Base �Ի���

class CPage_Base : public CPropertyPage, public CPage_Custom
{
	DECLARE_DYNAMIC(CPage_Base)

public:
	///< ���ع��캯��������β�MFC_UI *pUI
	CPage_Base(MFC_UI *pUI);
	virtual ~CPage_Base();

// �Ի�������
	enum { IDD = IDD_PROPPAGE_BASE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	virtual BOOL OnSetActive();

	DECLARE_MESSAGE_MAP()

private:
	/**
	* @brief �ڹ��캯���г�ʼ�� 
	* @see  CPage_Base(MFC_UI *pUI)
	*/
	MFC_UI  *m_pUI;  

public:
	void UpdateControls(void);  ///< ���¿ؼ�״̬
	void InitSliderCtrl();  ///< ��ʼ���������ؼ�
	void InitComboPixType(void); ///<��ʼ��ͼ�����ͱ༭��
	void SetCapValue(void);  ///<���ò���
	void InitBasemap(void); ///<��ʼ��Basemap����Ҫ����������CAP_DUPLEXENABLED�ĳ�ʼ��
	void SetFlat(void); ///< ����ɨ��ģʽΪƽ��ʱ��ֻ��Ϊ���档

	void SetMultistream(void); ///<���ö���������ײ���
	void SetColorGrayImage(void); ///<���ò�ɫ�ͻҶȣ����ȡ��Աȶȿ���
	void SetBWImage(void); ///<���úڰ�ͼƬʱ��ֵ������
	int FindPaperSize(int index); ///<Ѱ��index��Ӧ��ֽ�Ŵ�С,����index��Ӧ��ֽ�Ŵ�С

	void PreView();  //Ԥ����ťʵ�ʹ���ʵ��

	void SetBinarization(void); ///<���ö�ֵ���ֱ�ѡ��ֵͬʱ������ʾ��ȥ���ߵ㡱���ǡ���ɫ������

	//bool MyBrowseForSignalImage(PTCHAR strFilePath);
	/**
	* @brief �����ѡ����ͼƬ�ļ�
	* @return �ɹ�����vector<string>����ֵ
	*/
	vector<string> MyBrowseForMultiImages();

	//DrawToHdcϵ�к���
	void DrawToHDC(HDC hDCDst, RECT* pDstRect, IplImage* img); //��ָ��dDCDst����pDstRect��ͼ��
	void FillBitmapInfo(BITMAPINFO* bmi, int width, int height, int bpp, int origin); ///<Ϊbmiд����Ϣͷ�͵�ɫ�塣
	void Show(IplImage* img, HDC dc, int x, int y, int w, int h, int from_x, int from_y);	 ///<ʹ��DIBλͼ����ɫ���ݶ���Ŀ���豸������ص��豸��dc�ϵ�ָ�������е����ؽ�������
	CvRect RectToCvRect(RECT sr); ///<��Rect����תΪCvRect����
	RECT NormalizeRect(RECT r); ///<��׼����������rect����ֹ�����������Ҳ����꣬�ϲ���������²�����

private:
	CComboBox m_combo_colormode;  ///< ͼ������:  �ڰ�/�Ҷ�/��ɫ
	CComboBox m_combo_resolution;  ///< �ֱ���:  50/100/200/.../600
	CComboBox m_combo_binarization; ///<��ֵ�����̶���ֵ����̬��ֵ����ɫ��

	CSliderCtrl m_slider_contrast;  ///< �Աȶ�:  -100~+100
	CSliderCtrl m_slider_brightness;  ///< ����:  -100~+100
	CSliderCtrl m_slider_threshold; ///<��ֵ/ȥ���ߵ� 

	CEdit m_edit_contrast;  ///< ����ͬ����ʾm_slider_contrastֵ
	CEdit m_edit_brightness;   ///< ����ͬ����ʾm_slider_brightnessֵ
	CEdit m_edit_threshold; ///< ����ͬ����ʾm_slider_thresholdֵ

//	CButton m_btn_chooseimage;
	///<"ѡ��ͼƬ��ť"

	int m_radiobtn_scanmode; ///< ɨ�跽ʽ:  ADF�Զ���ֽ��/Flatbedƽ��
	int m_radiobtn_duplex; ///<��˫�棺���桢˫�桢����

	CButton m_check_frontcolor;
	CButton m_check_frontgray;
	CButton m_check_frontbw;
	CButton m_check_backbw;
	CButton m_check_backgray;
	CButton m_check_backcolor;

	CButton m_btn_frontcolor;
	CButton m_btn_frontgray;
	CButton m_btn_frontbw;
	CButton m_btn_backcolor;
	CButton m_btn_backgray;
	CButton m_btn_backbw;

	CStatic m_base_picture;

	MAP_CAP m_basemap;  ///<���ڱ�������ı���ֵ
	
	afx_msg void OnNMCustomdrawBase_Slider_Contrast(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawBase_Slider_Brightness(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawBase_Slider_Threshold(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void OnEnChangeBase_Edit_Contrast();
	afx_msg void OnEnChangeBase_Edit_Brightness();
	afx_msg void OnEnChangeBase_Edit_Threshold();

	afx_msg void OnCbnSelchangeBase_Combo_Colormode();
	afx_msg void OnCbnSelchangeBase_Combo_Resolution();
	afx_msg void OnCbnSelchangeBase_Combo_Binarization();

	afx_msg void OnBase_Btn_Newprofile();

  /** ѡ���ɨͼƬ */
//	afx_msg void OnBase_Btn_Chooseimage();

	afx_msg void OnBase_RadioBtn_Scanmode();
	afx_msg void OnBase_RadioBtn_Duplex();

	afx_msg void OnBase_Btn_Check_FrontColor();
	afx_msg void OnBase_Btn_Check_FrontGray();
	afx_msg void OnBase_Btn_Check_FrontBw();
	afx_msg void OnBase_Btn_Check_BackColor();
	afx_msg void OnBase_Btn_Check_BackGray();
	afx_msg void OnBase_Btn_Check_BackBw();

public:
	/** ����ָ��*/
	CPage_Custom* m_pAdPage;	
	CPage_Custom* m_pPaperPage;	
};

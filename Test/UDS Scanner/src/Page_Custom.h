
#ifndef __CPAGE_CUSTOM_H__
#define __CPAGE_CUSTOM_H__


class CPage_Custom
{
public:
	CPage_Custom(void);
	~CPage_Custom(void);

public:
	virtual void SetCapValue(void){};
	virtual void UpdateControls(void){};
	virtual void SetMultistream(void){}; //�߼���������ö������

	int FindPaperSize(int index); ///<Ѱ��index��Ӧ��ֽ�Ŵ�С,����index��Ӧ��ֽ�Ŵ�С
	int FindUnit(int index); ///<Ѱ��index��Ӧ�ĵ�λ,����index��Ӧ�ĵ�λ 

	int basebutton; ///<��¼base��������ĸ�buttonѡ��

};

#endif // __CPAGE_CUSTOM_H__


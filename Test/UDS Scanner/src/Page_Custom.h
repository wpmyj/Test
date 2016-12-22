
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
	virtual void InitAdvancedmap(void){};
	virtual void SetMultistream(void){}; //�߼���������ö������
	virtual void BaseColorMode(void){}; //�������������ж�ͼ��ģʽ�Ƿ����

	int brightness; //���ڸ߼�����Ĳ���ͬ��
	int contrast;

};

#endif // __CPAGE_CUSTOM_H__


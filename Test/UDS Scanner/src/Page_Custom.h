
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
	virtual void BaseStatus(void){}; //�������������ж�ͼ��ģʽ�Լ���˫���Ƿ����
	
	int scanside; //���ڼ�¼���������ǵ��滹��˫�棬���ڽ����߼���SetCapValue�еġ��ָ����UDSCAP_DOCS_IN_ADF�ĳ�ͻ
};

#endif // __CPAGE_CUSTOM_H__



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

	int brightness; //���ڸ߼�����Ĳ���ͬ��
	int contrast;
};

#endif // __CPAGE_CUSTOM_H__


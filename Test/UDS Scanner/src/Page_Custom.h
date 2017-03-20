
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
	virtual void SetMultistream(void){}; //高级界面的设置多流输出
	virtual void BaseStatus(void){}; //基本界面用于判断图像模式以及单双面是否可用
	
	int scanside; //用于记录基本界面是单面还是双面，便于解决与高级的SetCapValue中的“分割”设置UDSCAP_DOCS_IN_ADF的冲突
};

#endif // __CPAGE_CUSTOM_H__


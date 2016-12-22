
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
	virtual void BaseColorMode(void){}; //基本界面用于判断图像模式是否可用

	int brightness; //用于高级界面的参数同步
	int contrast;

};

#endif // __CPAGE_CUSTOM_H__


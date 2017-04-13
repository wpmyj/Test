
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
	virtual void SetMultistream(void){}; //高级界面的设置多流输出

	int FindPaperSize(int index); ///<寻找index对应的纸张大小,返回index对应的纸张大小
	int FindUnit(int index); ///<寻找index对应的单位,返回index对应的单位 

	int basebutton; ///<记录base界面多流哪个button选中

};

#endif // __CPAGE_CUSTOM_H__


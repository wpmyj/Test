
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

	int brightness; //用于高级界面的参数同步
	int contrast;

	int basecolormode; //用于保存Base界面的图片类型，从而决定高级界面多流输出默认是正面彩色、黑白还是灰度
};

#endif // __CPAGE_CUSTOM_H__



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
};

#endif // __CPAGE_CUSTOM_H__


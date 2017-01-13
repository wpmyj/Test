#include "UDS_Edit.h"


CUDS_Edit::CUDS_Edit(void)
{
}


CUDS_Edit::~CUDS_Edit(void)
{
}

BEGIN_MESSAGE_MAP(CUDS_Edit, CEdit)  
	ON_WM_CHAR()  
END_MESSAGE_MAP()

void CUDS_Edit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CString str;
	GetWindowText(str);// 获取原来编辑框中的字符串

	if('.' == nChar)//小数点只能输入一次
	{
		if("" == str)
		{}
		else if(-1 != str.Find('.'))//若原来的字符串中已经有一个小数点,则不将小数点输入,保证了最多只能输入一个小数点
		{} 
		else// 否则就输入这个小数点
		{
			CEdit::OnChar(nChar, nRepCnt, nFlags);
		}
	}
	else if('-' == nChar)// 保证负号'-'只能出现一次,并且只能出现在第一个字符
	{
		if(str.IsEmpty())
		{
			CEdit::OnChar(nChar, nRepCnt, nFlags);
		}
		else
		{
			int nSource, nDestination;
			GetSel(nSource, nDestination);
			//此时选择了全部内容
			if(nSource == 0 && nDestination == str.GetLength())
			{
				CEdit::OnChar(nChar, nRepCnt, nFlags);
			}
			else
			{}
		}
	}
	// 除了小数点和负号,还允许输入数字,Backspace(ASCII码值位0x08),Delete（0x2E）
	else if((nChar >= '0' && nChar <= '9') || (0x08 == nChar) || (0x2E == nChar)) 
	{
		CEdit::OnChar(nChar,nRepCnt,nFlags);
	}
	else //其他处理</span> 
	{}
}

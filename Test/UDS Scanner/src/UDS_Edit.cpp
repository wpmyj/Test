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
	GetWindowText(str);// ��ȡԭ���༭���е��ַ���

	if('.' == nChar)//С����ֻ������һ��
	{
		if("" == str)
		{}
		else if(-1 != str.Find('.'))//��ԭ�����ַ������Ѿ���һ��С����,�򲻽�С��������,��֤�����ֻ������һ��С����
		{} 
		else// ������������С����
		{
			CEdit::OnChar(nChar, nRepCnt, nFlags);
		}
	}
	else if('-' == nChar)// ��֤����'-'ֻ�ܳ���һ��,����ֻ�ܳ����ڵ�һ���ַ�
	{
		if(str.IsEmpty())
		{
			CEdit::OnChar(nChar, nRepCnt, nFlags);
		}
		else
		{
			int nSource, nDestination;
			GetSel(nSource, nDestination);
			//��ʱѡ����ȫ������
			if(nSource == 0 && nDestination == str.GetLength())
			{
				CEdit::OnChar(nChar, nRepCnt, nFlags);
			}
			else
			{}
		}
	}
	// ����С����͸���,��������������,Backspace(ASCII��ֵλ0x08),Delete��0x2E��
	else if((nChar >= '0' && nChar <= '9') || (0x08 == nChar) || (0x2E == nChar)) 
	{
		CEdit::OnChar(nChar,nRepCnt,nFlags);
	}
	else //��������</span> 
	{}
}

// 9.4.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <vector>

using namespace  std;
/************************************************************************/
/* �沨��ʽ����������������ʽ��������������ʾ����������������ʾʽ���м��㡣
				�������ı��ʽ�����ǳ���Ҳ��������Ҫ����ı��ʽ��
	
   �ݹ��˼�����ڴ����Ͻ��еݹ飬������ϸ�ڡ�
*/
/************************************************************************/
double exp()
{
	char a[10];
	cin>>a;
	if(a[0]=='+')return exp()+exp();
	if(a[0]=='-')return exp()-exp();
	if (a[0]=='*')
	{
		return exp()*exp();
	}
	if(a[0]=='/')return exp()/exp();
	return atof(a);
}

int _tmain(int argc, _TCHAR* argv[])
{
	double r = exp();
	cout<<r;
	system("pause");
	return 0;
}


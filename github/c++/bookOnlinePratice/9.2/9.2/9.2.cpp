// 9.2.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <iostream>
using namespace  std;
int fibonaque(int a)
{
	if(a==1)return 1;
	if(a==2)return 1;
	return fibonaque(a-1)+fibonaque(a-2);
}

int _tmain(int argc, _TCHAR* argv[])
{
	int n,k;
	cin>>n;
	while(n-->0)
	{
		cin>>k;
		cout<<fibonaque(k)<<endl;
	}
	system("pause");
	return 0;
}


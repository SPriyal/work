#include <iostream>
#include<fstream>
#include<math.h>
#define MaxNum 21
using namespace std; 

int num = 0; //�������и����ļ���
template <class T>
/**
* �����ظ������������㷨��p���ڴ洢���ɵ�����
* list�ĳ���Ϊn��p�ĳ���Ϊm���ݹ��λ�ø���k
*/
void repPerm(FILE *file,T list[], int n, T p[], int m, int k){    
	if (k == m) {    //��ʱ�������е����ڲ㣬�������������
		for (int i = 0; i < m; i ++){
			fprintf(file,"%c ",p[i]);
		}
		fprintf(file,"\n");
		num ++;
	}else{
		for (int i = 0; i < n; i ++){ //���ȷ��Ԫ�أ��ݹ鸳ֵ
			p[k] = list[i]; //�ݹ鵽�Ĳ���и�ֵ
			repPerm(file,list, n, p, m, k + 1);
		}
	}
} 
int main(){
	FILE *pFile;
	int m;
	int n;
    char list[MaxNum];     
    char p[MaxNum];
	pFile=fopen("a.txt","w");
	cout<<"������Ҫ���еļ�λ��(���Ϊ10������6���ͽ������6λ�����а�һ����λ������ȫ����)��";
	cin>>n;
	cout<<"������Ҫ��������λ����(���Ϊ21.����7���ͻ����7λ����ȫ����)";
	cin>>m;
	cout<<endl<<"(0��9��"<<n<<"������"<<m<<"λ��ȫ����)���н���Ѿ��洢��result.txt�ļ���\n";
	for(int i=0;i<n;i++){
		list[i]=(char)(rand()%10+48);
	}
    repPerm(pFile,list, n, p, m, 0);	
	fprintf(pFile,"total is %d \n",num);
	fclose(pFile);
    return 0; 

}
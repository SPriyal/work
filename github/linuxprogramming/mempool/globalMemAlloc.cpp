#include <cstdlib>
#include <iostream>
#include <string>
#include <sys/time.h>

using namespace std;


class Relation
{
public:
	Relation(int n= 0,int d = 0):n(n),d(d){}
private:
	int n;
	int d;
};

int main(int argc,char ** argv)
{
	Relation * array[1000];
	#ifdef TEST
 	clock_t t = clock();
 	#endif
	for(int i = 0 ; i< 100000;++i)
	{	
		for(int j= 0;j<1000;++j)
		{
			array[j]  = new Relation(i,j);
		}
		
		for(int j = 0; j<1000;++j)
		{
			delete[]array[j];
		}
	}
	#ifdef TEST
	t =( clock()  -  t  )  / CLOCKS_PER_SEC;
	cout<<"t = "<<t<<endl;
	#endif
	return 0;
}



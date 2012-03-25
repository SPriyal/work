#include <cstdlib>
#include <iostream>
#include <string>
#include <sys/time.h>

using namespace std;

struct FreeList
{
	FreeList * next;
};

class Relation
{
public:
	Relation(int n= 0,int d = 0):n(n),d(d){}
	inline void * operator new(size_t size);
	inline void operator delete(void * doomed ,size_t size);
	static void newMemPool(){expandFreeList();}
	static void deleteMemPool();
private:
	static const int expandSize;
	static void expandFreeList();
	static FreeList * freeList;
	int n;
	int d;
};

const int Relation::expandSize = 1000;
FreeList * Relation:: freeList = NULL;

inline void * Relation::operator new(size_t size)
{
	if(freeList == NULL){
		expandFreeList();
	}
	FreeList * head = freeList;
	freeList = head->next;
	return head;
}

void Relation::expandFreeList()
{
	size_t size = sizeof(Relation) > sizeof(FreeList) ? sizeof(Relation) : sizeof(FreeList);
	FreeList * runner = static_cast<FreeList* >(malloc(size));
	freeList = runner;
	for(int i = 0 ;i < expandSize;++i)
	{
		runner->next = static_cast<FreeList *>(malloc(size));
		runner  = runner->next;
	}
	runner->next = NULL;
}

inline void Relation::operator delete(void * doomed, size_t size)
{
	FreeList * head = static_cast<FreeList *>(doomed);
	head->next = freeList;
	freeList = head;
}

void Relation::deleteMemPool()
{
	FreeList * head = freeList;
	FreeList * temp = NULL;
	for(;head != NULL;)
	{
		temp = head->next;
		delete [] head;
		head = temp;
	}	
}

int main(int argc,char ** argv)
{
	Relation * array[1000];
	Relation::newMemPool();
	int c = 1000;
	int b = 100000;
	#ifdef TEST
	clock_t begin = clock();
	#endif
	for(int i = 0 ; i< b;++i)
	{	
		for(int j= 0;j<c;++j)
		{
			array[j]  = new Relation(i,j);
		}
		
		for(int j = 0; j<c;++j)
		{
			delete array[j];
		}
	}
	#ifdef TEST
		begin = ( clock()  - begin)  / CLOCKS_PER_SEC;
		cout<<"time  = "<<begin<<endl;
	#endif
	
	Relation::deleteMemPool();			
	return 0;
}

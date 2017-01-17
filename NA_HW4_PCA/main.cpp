#include <iostream>
#include "TestPCA.h"
#define MAXEIGENNUM 10
int main()
{	
	TestPCA NA4;	
	NA4.makeY();
	for(int i= MAXEIGENNUM;i>0;--i)//try adjust number of eigenvectors
		NA4.test(i);
	return 0;
}
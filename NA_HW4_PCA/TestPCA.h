#pragma once
#include "JPCA.h"
#include <vector>
using namespace std;

class TestPCA
{
private:
	JPCA mJPCA; //mPCA has projX, eigen, mean matrix
	vector<Pair> TestPath;
	double** testData;
public:
	TestPCA();
	~TestPCA();
	void makeY();
	void test(int numEigen);
	double** projY = nullptr;	
	void sortPath();
};


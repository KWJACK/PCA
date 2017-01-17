#include "TestPCA.h"
#include <algorithm>
#include <ctype.h>
#include <atlstr.h>

#define TESTPATH ".\\PCA_DB\\Test"
#define TRAINNUM 48
#define TESTNUM 12
#define EIGENNUM 10
#define WxH 10304
using namespace std;
struct cmp {
	bool operator()(const Pair& l, const Pair& r) {
		return (l.second < r.second);
	}
};
TestPCA::TestPCA()
{
	cout << "training data set read" << endl;
	mJPCA.ReadData(true, mJPCA.getTDataPathVector(), mJPCA.getData());//opt=1 -> add function MEAN
	mJPCA.RepresentProjection(mJPCA.projX, mJPCA.getData(), TRAINNUM);	
	testData = new double*[TESTNUM]();
}

TestPCA::~TestPCA()
{
	for (int n = 0; n < TESTNUM; n++) {
		delete[] * (projY + n);
		delete[] * (testData + n);
	}
	delete projY;
	delete testData;
}

void TestPCA::makeY()
{
	cout << "test data set read" << endl;
	mJPCA.GetFiles(TestPath, TESTPATH, false);
	sortPath();//sorting path, because order is 1,10,11,12,2,3,4, ... not 1,2,3,4,..., 12
	mJPCA.ReadData(false, TestPath, testData);
	projY = new double*[TESTNUM];
	mJPCA.RepresentProjection(projY, testData,TESTNUM);
	mJPCA.deleteLeftArr();//delete eigenvector, Training Data
}

void TestPCA::test(int numEigen)
{
	cout << "Number of eigenvector " << numEigen << endl;
	vector<double> ListResult;
	vector<int> index;		
	double tempResult;
	int numHit=0;
	for (int n = 0; n < TESTNUM; n++) {//test num
		for (int i = 0; i < TRAINNUM; i++) {//test all with 48 traing sample
			tempResult = 0.0;
			for (int j = 0; j < numEigen; j++) {
				tempResult += ((projY[n][j]- mJPCA.projX[i][j])*(projY[n][j] - mJPCA.projX[i][j]));				
			}			
			ListResult.push_back(tempResult);
			index.push_back(i);
		}
		//sort: Find index of x'(triaing) having the smallest distance with y(test)
		sort(index.begin(), index.end(), 
			[&](const int& a, const int& b) {
			return (ListResult[a] < ListResult[b]);
		});		
		cout << "The test of "<<n+1<<".bmp	"<<"is matched set_"<<(index[0]/8) +1;						
		cout << " in " << index[0]%8 +1 << ".bmp	";
		//calculate recognition rate
		if (n > 3) {//till 4th
			if (((index[0] / 8) + 1) == (n + 2) / 2) {
				++numHit;
				cout << "correct" << endl;
			}else cout << "incorrect" << endl;
		}
		else if (n < 2) {//n=0, n=1
			if (((index[0] / 8) + 1) == 2) {
				++numHit;
				cout << "correct" << endl;
			}else cout << "incorrect" << endl;
		}
		else {//n=2, n=3
			if (((index[0] / 8) + 1) == 1) {
				++numHit;
				cout << "correct" << endl;
			}else cout << "incorrect" << endl;
		}
		ListResult.clear();
		index.clear();
	}
	cout << "Face recognition rate: " << numHit * 100 / TESTNUM <<"%"<< endl;
	//delete left mem
	cout << endl;
}

void TestPCA::sortPath()
{
	sort(TestPath.begin(), TestPath.end(), cmp());
}

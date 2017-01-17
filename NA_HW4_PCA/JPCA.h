#include <windows.h>
#include <string>
#include <iostream>
#include <vector>
using namespace std;
typedef pair<string, int> Pair;
class JPCA
{
private:
	vector<Pair>  TDataPath;
	BITMAPFILEHEADER hf;
	BITMAPINFOHEADER hInfo;
	double **TData;//Training Data	
	double *MEAN;
	double **eigenVec;	
	FILE *infile;	
public:
	JPCA();//jae keun PCA
	~JPCA();
	void ReadData(bool opt, vector<Pair> TDataPath, double ** Data);
	double **projX=nullptr;
	vector<Pair> getTDataPathVector() { return TDataPath; }
	double ** getData() { return TData; }
	void GetMean(bool opt);//mean
	void writeAverageImage();//write average face
	void GetFiles(vector<Pair>& vList, string sPath, bool bAllDirectories);
	void computeEigen();//get eigen from the result by matlab
	void Normalize(double** Data, int num);//subtract mean	
	void RepresentProjection(double ** projT, double ** Data, int num);//projection
	void deleteLeftArr();
	int orderPath(string path);
};
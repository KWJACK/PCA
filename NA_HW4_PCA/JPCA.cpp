#include "JPCA.h"
#include <stdio.h>
#include <ctype.h>
#include <atlstr.h>

#define TRAININGPATH ".\\PCA_DB\\Training"
#define WIDTH 92
#define HEIGHT 112
#define WxH 10304
#define EIGENNUM 10
#define TRAINNUM 48

//option setup not using unicode-->wchar not using rather than using char
JPCA::JPCA()
{
	TData = new double*[TRAINNUM]();//number of training data
	MEAN = new double[WIDTH*HEIGHT]();
	GetFiles(TDataPath, TRAININGPATH, true);
	projX = new double*[TRAINNUM];
}

JPCA::~JPCA()
{
	for (int n = 0; n < TRAINNUM; n++) {
		delete[] * (projX + n);
	}	
	delete projX;	
}
void JPCA::ReadData(bool opt, vector<Pair> TDataPath, double ** Data) {
	int i = 0;
	for (auto s : TDataPath)
	{
		cout << s.first << " file read"<<endl;
		fopen_s(&infile, s.first.c_str(), "rb");//casting char*		
		fread(&hf, sizeof(BITMAPFILEHEADER), 1, infile);//read bmp file
		//header check
		if (hf.bfType != 0x4D42) exit(1);//4D ='M', 42 ='B', little endian
		fread(&hInfo, sizeof(BITMAPINFOHEADER), 1, infile);
		if (hInfo.biBitCount != 24) {//RGB 1byte(8bit)*3
			printf("This file is not matched with BitMap"); return;
		}
		//start point of data
		fseek(infile, hf.bfOffBits, SEEK_SET);	
		BYTE *lpImg = new BYTE[hInfo.biSizeImage]();
		int Tsize = _msize(lpImg);		
		fread(lpImg, sizeof(char), hInfo.biSizeImage, infile);
		fclose(infile);

		*(Data + i) = new double[WIDTH*HEIGHT]();//raw		
		for (int x = HEIGHT - 1; x >= 0; x--){//bmp has data reverse to height
			for (int y = 0; y < WIDTH; y++) {
				 Data[i][(HEIGHT - 1 - x)*WIDTH + y] = lpImg[x*WIDTH * 3 + (y * 3)];						
			}
		}
		delete lpImg;
		i++;//next picture
	}
	//Get Mean Matrix
	GetMean(opt);//opt
	if (opt)computeEigen();
	cout<<endl;
}
void JPCA::GetMean(bool opt)
{
	if (!opt)return;
	for (int i = 0; i < TRAINNUM; i++) {
		for (int j = 0; j < HEIGHT; j++) {
			for (int k = 0; k < WIDTH; k++) {
				MEAN[j*WIDTH + k] += TData[i][j*WIDTH + k];
			}
		}
	}
	for (int j = 0; j < WIDTH*HEIGHT; j++) {
		MEAN[j] /= TRAINNUM;
	}
	writeAverageImage();
}

void JPCA::writeAverageImage()
{
	FILE *fp;
	BYTE* data2 = new BYTE[WxH];
	fopen_s(&fp, "AveragefromC.raw", "wb");
	for (int i = 0; i < WxH; i++) {
		data2[i] = (BYTE)MEAN[i];
	}
	if (!fp)cout << "Can not open file " << endl;	
		fwrite(data2, sizeof(BYTE), WxH, fp);
	fclose(fp);
	delete data2;
}


void JPCA::GetFiles(vector<Pair> &vList, string sPath, bool bAllDirectories)
{
	string sTemp = sPath + string("\\*.*");
	WIN32_FIND_DATA fd;
	HANDLE hFind = FindFirstFile(sTemp.c_str(), &fd);
	if (INVALID_HANDLE_VALUE != hFind)
	{
		do {
			if (fd.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY) {
				if (bAllDirectories) {
					if (fd.cFileName[0] != '.') {//current and parent path ignore
						sTemp = sPath + string("\\") + string(fd.cFileName);
						GetFiles(vList, sTemp, bAllDirectories);
					}
				}
			}
			else {//file
				sTemp = sPath + string("\\") + string(fd.cFileName);
				int i=orderPath(sTemp);			
				vList.push_back(make_pair(sTemp,i));
			}
		} while (FindNextFile(hFind, &fd));					
		FindClose(hFind);		
	}
}

void JPCA::computeEigen()
{
	eigenVec = new double*[EIGENNUM]();
	char buf[32]{ 0, };
	for (int n = 0; n < EIGENNUM; n++) {
		*(eigenVec + n) = new double[WxH]();
		sprintf_s(buf, "eigenvector%d.txt", n + 1);
		fopen_s(&infile, buf, "r");//casting char*
		char temp[32];//file path temp
		for (int i = 0; i < WxH; i++) {			
			fgets(temp, 32, infile);
			eigenVec[n][i] = atof(temp);//eigenvector1.txt is corresponding largest eigenvalue			
		}
		fclose(infile);
	}
}

void JPCA::RepresentProjection(double ** projT, double ** Data, int num) {
	Normalize(Data, num);//Normalize is doing to subtract MEAN from Data

	for (int n = 0; n < num; n++) {//get Representative vector by projection of the training vectors
		*(projT + n) = new double[EIGENNUM]();
		for (int i = 0; i < EIGENNUM; i++)
			for (int j = 0; j < WxH; j++)
				projT[n][i] += eigenVec[i][j] * Data[n][j];
	}
}

void JPCA::Normalize(double ** Data, int num)
{
	for (int n = 0; n < num; n++)//get Normalized vectors
		for (int x = 0; x < HEIGHT; x++)
			for (int y = 0; y < WIDTH; y++)
				Data[n][(x*WIDTH) + y] = Data[n][(x*WIDTH) + y] - MEAN[(x*WIDTH) + y];
}

void JPCA::deleteLeftArr() {
	for (int n = 0; n < EIGENNUM; n++) {
		delete[] * (eigenVec + n);
	}
	for (int n = 0; n < TRAINNUM; n++) {
		delete[] * (TData + n);
	}
	delete eigenVec;
	delete TData;
	delete MEAN;
}
int JPCA::orderPath(string path)//get number of file from path
{
	CString str = path.c_str();
	str.TrimRight(".bmp");
	int pos = str.ReverseFind('/');//back string delete
	str.Delete(0, pos + 1);
	int i;
	for (i = 0; i < str.GetLength(); i++) {//seek num location
		if (isdigit(str[i]) != 0)break;
	}
	str.Delete(0, i);//front string delete
	return atoi(str);
}
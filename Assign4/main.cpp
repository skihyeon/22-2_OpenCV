#include"main.h"
#include"Flag.h"

//#define HOG
#define COR

int main() {
	int i, j;

#ifdef HOG
	Mat img = imread("lecture3.bmp", CV_LOAD_IMAGE_GRAYSCALE);
	Mat imgCompare1 = imread("compare1.bmp", CV_LOAD_IMAGE_GRAYSCALE);
	Mat imgCompare2 = imread("compare2.bmp", CV_LOAD_IMAGE_GRAYSCALE);

	

	float** HOG1, ** HOG2, ** HOG3;

	HOG1 = GetHistogram(img);
	HOG2 = GetHistogram(imgCompare1);
	HOG3 = GetHistogram(imgCompare2);


	//Savecsv(HOG1, 105, 9);
	//Savecsv(HOG2, 105, 9);
	//Savecsv(HOG3, 105, 9);

	CompareHistogram(HOG1, HOG2, 105, 9);
	CompareHistogram(HOG1, HOG3, 105, 9);

	freePtr(HOG1);
	freePtr(HOG2);
	freePtr(HOG3);
#endif // HOG
	Mat imgRef = imread("ref.bmp", CV_LOAD_IMAGE_GRAYSCALE);
	Mat imgTar = imread("tar.bmp", CV_LOAD_IMAGE_GRAYSCALE);
	Mat imgTemp;
	int* corner1, * corner2;
	Point** MatchingPoint;

	imgRef = GetGaussianFilter(imgRef);
	imgTar = GetGaussianFilter(imgTar);
	
	corner1 = GetCornerLocation(imgRef,1000000000, 3);
	corner2 = GetCornerLocation(imgTar,1000000000, 3);

	// HOG ºñ±³
	MatchingPoint = MatchingCorner(imgRef, imgTar, corner1, corner2, 17);

	imgTemp = imgMerge(imgRef, imgTar, MatchingPoint);


	imwrite("imgMatching.bmp", imgTemp);

	for (j = 0; j < 2; j++) free(*(MatchingPoint + j));
	free(MatchingPoint);

	free(corner1);
	free(corner2);


	return 0;

}

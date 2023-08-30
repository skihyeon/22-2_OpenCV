#define _CRT_SECURE_NO_WARNINGS
#include "main.h"
#include <iostream>
#define slicDraw

void SLICsegmentation(Mat image)
{
	SLIC slic;
	Scalar s;

	int x, y;
	int height, width;
	int numlabels; // Generated number of superpixels
	int m_spcount = 200; // Desired number of superpixels ideal count 200;
//#define TEST
	double m_compactness = 10;// 20.0; // compactness factor (1-40)

	height = image.rows;
	width = image.cols;

	unsigned int* ubuff = (unsigned int*)calloc(height * width, sizeof(unsigned int));
	int* labels = (int*)calloc(height * width, sizeof(int));

	for (y = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
		{
			ubuff[y * width + x] = (int)image.at<Vec3b>(y, x)[0] + ((int)image.at<Vec3b>(y, x)[1] << 8) + ((int)image.at<Vec3b>(y, x)[2] << 16);
		}
	}

	slic.DoSuperpixelSegmentation_ForGivenNumberOfSuperpixels(ubuff, width, height, labels, numlabels, m_spcount, m_compactness);
	printf("\n# of superpixels = %d\n", numlabels);

#ifdef slicDraw
	Mat result(height, width, CV_8UC3);
	slic.DrawContoursAroundSegments(ubuff, labels, width, height, 0);

	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			result.at<Vec3b>(y, x)[0] = ubuff[y * width + x] & 0xff;
			result.at<Vec3b>(y, x)[1] = ubuff[y * width + x] >> 8 & 0xff;
			result.at<Vec3b>(y, x)[2] = ubuff[y * width + x] >> 16 & 0xff;
		}
	}
	
#ifdef FINAL

#ifdef TEST
	char filename[100];
	sprintf(filename, "num_%d_merged_building.bmp", m_spcount);
	imwrite(filename,result);
#else
	char filename[100];
	sprintf(filename, "num_%d_notmerged.bmp", m_spcount);
	imwrite(filename, result);
#endif

	for (int i = 0; i < numlabels; i++) {
		//printf("tmpx[%d] = %d tmpy[%d]=%d\n", i, tmpx[i], i, tmpy[i]);
		std::stringstream ss;
		ss << i;
		putText(result, ss.str(), Point(tmpx[i], tmpy[i]), 1, 1, Scalar(255, 0, 0));
		circle(result, Point(tmpx[i], tmpy[i]), 2, cv::Scalar(0, 0, 255), -1);
	}

	//imwrite("tes_res4.bmp", result);
#endif //def FINAL
#endif 
	Mat labelImage(height, width, CV_8UC1);

	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			labelImage.at<uchar>(y, x) = labels[y * width + x] * 100;
		}
	}
	imwrite("labelmap2.bmp", labelImage);

	int cnt = 0;
	int arr[100] = { -1. };

	for (int i = 0; i < 100; i++) {
		for (y = 0; y < height; y++) {
			for (x = 0; x < width; x++) {
				int k = labelImage.at<uchar>(y, x);
				if (k != arr[i]) {
					arr[i] = k;
				}
			}
		}
	}
	
	for (int i = 0; i < 100; i++) {
		if (arr[i] != -1) printf("arr[%d]=%d\n", i, arr[i]);
	}

	free(ubuff);
	free(labels);
}
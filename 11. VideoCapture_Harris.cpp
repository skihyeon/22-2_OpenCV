#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <Windows.h>
//#include "HarrisCorner.cpp"
//#include "utils.h"

#define PI 3.1415926
#define MSIZE 3;
#define BIN 9;

//#define SPEED

using namespace cv;

Mat GetHarrisCorner(Mat img) {
	int x, y, xx, yy, height, width, sumx, sumy, IxIx, IxIy, IyIx, IyIy, det, tr;
	int max = -1, min = 100000;
	float k = 0.04;

	height = img.rows;
	width = img.cols;


	int wsize = 15;
	int mask_x[9] = { -1, 0, 1, -1, 0, 1, -1, 0, 1 };
	int mask_y[9] = { -1, -1, -1, 0, 0, 0, 1, 1, 1 };


	Mat imgCorner(height, width, CV_8UC3);

	int* lx = (int*)calloc(height * width, sizeof(int));
	int* ly = (int*)calloc(height * width, sizeof(int));
	int* corner = (int*)calloc(height * width, sizeof(int));

	float* R = (float*)calloc(height * width, sizeof(float));

	Scalar c;
	Point pCenter;
	int radius = 1;

	// Compute Gradient And Convert Channel
	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			sumx = 0;
			sumy = 0;
			for (yy = y - 1; yy <= y + 1; yy++) {
				for (xx = x - 1; xx <= x + 1; xx++) {
					if (yy >= 0 && yy < height && xx >= 0 && xx < width) {
						sumx += img.at<uchar>(yy, xx) * mask_x[(yy - (y - 1)) * 3 + (xx - (x - 1))];
						sumy += img.at<uchar>(yy, xx) * mask_y[(yy - (y - 1)) * 3 + (xx - (x - 1))];
					}
				}
				lx[y * width + x] = sumx;
				ly[y * width + x] = sumy;

				imgCorner.at<Vec3b>(y, x)[0] = img.at<uchar>(y, x);
				imgCorner.at<Vec3b>(y, x)[1] = img.at<uchar>(y, x);
				imgCorner.at<Vec3b>(y, x)[2] = img.at<uchar>(y, x);

			}
		}
	}

	// Compute R And Detect Corner 
	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			IxIx = 0.0;
			IxIy = 0.0;
			IyIx = 0.0;
			IyIy = 0.0;
			for (yy = y - (wsize / 2); yy <= y + (wsize / 2); yy++) {
				for (xx = x - (wsize / 2); xx <= x + (wsize / 2); xx++) {
					if (yy >= 0 && yy < height && xx >= 0 && xx < width) {
						IxIx += lx[yy * width + xx] * lx[yy * width + xx];
						IxIy += lx[yy * width + xx] * ly[yy * width + xx];
						IyIx += ly[yy * width + xx] * lx[yy * width + xx];
						IyIy += ly[yy * width + xx] * ly[yy * width + xx];
					}
				}
			}
			det = IxIx * IyIy - IxIy * IyIx;
			tr = IxIx + IyIy;
			R[y * width + x] = det - k * tr * tr;
			if (R[y * width + x]>20.0)	corner[y * width + x] = 1;
		}
	}

	int t = 0;
	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			if (corner[y * width + x]) {
				t++;
			}
		}
	}

	free(lx);
	free(ly);
	free(R);


	// Visualization Corner
	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			if (corner[y * width + x]) {
				pCenter.x = x;
				pCenter.y = y;
				c.val[0] = 0;
				c.val[1] = 255;
				c.val[2] = 0;
				circle(imgCorner, pCenter, radius, c, 2, 8, 0);
			}
		}
	}

	free(corner);
	return imgCorner;

}

void main() {
	VideoCapture capture(0);
	Mat frame;
	Mat frameGray;
	Mat cornerMap(frame.rows, frame.cols, CV_8UC3);
	int fridx = 0;
	char filename[100] = { 0, };

#ifdef SPEED
	LARGE_INTEGER freq, start, stop;
	double diff,avg=0.0;
#endif

	if (!capture.isOpened()) {
		printf("Couldn't open the web cam\n");
		return;
	}
	while (true) {
		capture >> frame;
		cvtColor(frame, frameGray, COLOR_BGR2GRAY);
		
#ifdef SPEED
		QueryPerformanceFrequency(&freq);
		QueryPerformanceCounter(&start);
#endif


		cornerMap = GetHarrisCorner(frameGray);


#ifdef SPEED
		QueryPerformanceCounter(&stop);
		diff = (double)(stop.QuadPart - start.QuadPart) / freq.QuadPart;
		avg += diff;
		printf("processing speed : %lf\n", fridx/avg);
#endif

		imshow("Video", cornerMap);
		/*sprintf(filename, "C:\\22-2_CV\\images\\5week\\VideoCapture%04d.bmp", fridx);
		imwrite(filename, frame);*/
		sprintf(filename, "C:\\22-2_CV\\images\\5week\\VideoCapture%04d.bmp", fridx);
		imwrite(filename, cornerMap);
		if (waitKey(30) >= 0) break;
		fridx++;
	}
}
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv2/opencv.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/ml/ml.hpp>

using namespace cv;

static int uniform[256] =
{
	0,1,2,3,4,58,5,6,7,58,58,58,8,58,9,10,11,58,58,58,58,58,58,58,12,58,58,58,13,58,
	14,15,16,58,58,58,58,58,58,58,58,58,58,58,58,58,58,58,17,58,58,58,58,58,58,58,18,
	58,58,58,19,58,20,21,22,58,58,58,58,58,58,58,58,58,58,58,58,58,58,58,58,58,58,58,
	58,58,58,58,58,58,58,58,58,58,58,58,23,58,58,58,58,58,58,58,58,58,58,58,58,58,
	58,58,24,58,58,58,58,58,58,58,25,58,58,58,26,58,27,28,29,30,58,31,58,58,58,32,58,
	58,58,58,58,58,58,33,58,58,58,58,58,58,58,58,58,58,58,58,58,58,58,34,58,58,58,58,
	58,58,58,58,58,58,58,58,58,58,58,58,58,58,58,58,58,58,58,58,58,58,58,58,58,58,
	58,35,36,37,58,38,58,58,58,39,58,58,58,58,58,58,58,40,58,58,58,58,58,58,58,58,58,
	58,58,58,58,58,58,41,42,43,58,44,58,58,58,45,58,58,58,58,58,58,58,46,47,48,58,49,
	58,58,58,50,51,52,58,53,54,55,56,57
};


Mat LBP_lookup(Mat input, int face_xpos, int face_ypos, int face_width, int face_height) {
	int height = input.rows;
	int width = input.cols;
	int x, y;
	int v = 0;

	Mat result(height, width, CV_8UC1);

	//Compute LBP
	for (y = face_ypos; y < face_ypos + face_height; y++) {
		for (x = face_xpos; x < face_xpos + face_width; x++) {
			v = 0;
			if (y >= 0 && y < height && x >= 0 && x < width) {
				//clock-wise
				//start point : -1, -1
				if (input.at<uchar>(y, x) < input.at<uchar>(y - 1, x - 1))  v += 1;
				if (input.at<uchar>(y, x) < input.at<uchar>(y - 1, x))  v += 2;
				if (input.at<uchar>(y, x) < input.at<uchar>(y - 1, x + 1))  v += 4;
				if (input.at<uchar>(y, x) < input.at<uchar>(y, x - 1))  v += 8;
				if (input.at<uchar>(y, x) < input.at<uchar>(y, x))  v += 16;
				if (input.at<uchar>(y, x) < input.at<uchar>(y, x + 1))  v += 32;
				if (input.at<uchar>(y, x) < input.at<uchar>(y + 1, x - 1))  v += 64;
				if (input.at<uchar>(y, x) < input.at<uchar>(y + 1, x))  v += 128;
				if (input.at<uchar>(y, x) < input.at<uchar>(y + 1, x + 1))  v += 256;

				result.at<uchar>(y, x) = uniform[v];
			}
		}
	}
	return result;
}
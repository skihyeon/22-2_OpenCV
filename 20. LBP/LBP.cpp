#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv2/opencv.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/ml/ml.hpp>

using namespace cv;


Mat LBP(Mat input, int face_xpos, int face_ypos, int face_width, int face_height) {
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

				result.at<uchar>(y, x) = v;
			}
		}
	}
	return result;
}

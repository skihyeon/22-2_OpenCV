#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv2/opencv.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/ml/ml.hpp>

using namespace cv;

float* LBP_histo(Mat LBP_img, int face_xpos, int face_ypos, int face_height, int face_width) {
	int BLK = face_height / 7;
	int binNum = 256;
	int x, y, xx, yy;
	float* histo = (float*)calloc(binNum, sizeof(float));
	float* res_histo = (float*)calloc(300 * binNum, sizeof(float));
	float denomi = 0;
	int cnt = 0;

	for (y = face_ypos; y < face_ypos + face_height - BLK; y += BLK / 2) {
		for (x = face_xpos; x < face_xpos + face_width - BLK; x += BLK / 2) {
			for (yy = y; yy < y + BLK; yy++) {
				for (xx = x; xx < x + BLK; xx++) {
					if (LBP_img.at<uchar>(yy, xx) <= 255) {
						int idx = LBP_img.at<uchar>(yy, xx);
						histo[idx] ++;
					}
				}
			}

			denomi = 0;
			for (yy = 0; yy < binNum; yy++) {
				denomi += histo[yy] * histo[yy];
			}
			for (yy = 0; yy < binNum; yy++) {
				histo[yy] /= sqrt(denomi);
				if (denomi == 0) histo[yy] = 0;
				res_histo[cnt * binNum + yy] = histo[yy];
			}
			cnt++;
		}
	}
	free(histo);
	return res_histo;
}

Mat face(Mat input, int face_xpos, int face_ypos, int face_width, int face_height) {
	int height = input.rows;
	int width = input.cols;
	int x, y, xx, yy;
	Mat res(height, width, CV_8UC1);

	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			if (y<face_ypos || x<face_xpos || y>face_ypos + face_height || x>face_xpos + face_width) {
				res.at<uchar>(y, x) = (uchar)0;
			}
			else {
				res.at<uchar>(y, x) = input.at<uchar>(y, x);
			}
		}
	}
	return res;
}

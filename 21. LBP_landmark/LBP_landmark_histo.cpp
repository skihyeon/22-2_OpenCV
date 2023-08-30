#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv2/opencv.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/ml/ml.hpp>

#define UNIFORM 60
#define LANDMARK_NUM 70
using namespace cv;

float* LBP_landmark_histo(Mat LBP_landmark_img, int* pos_x, int* pos_y, int landmarkNum){
	float Win = 0;
	int height = LBP_landmark_img.cols;
	int width = LBP_landmark_img.rows;
	int x, y, xx, yy;
	float* histo = (float*)calloc(LANDMARK_NUM * UNIFORM, sizeof(float));
	float* res_histo = (float*)calloc(LANDMARK_NUM * UNIFORM, sizeof(float));
	float denomi = 0;
	int cnt = 0;

	//Win = 45;

	for(int i=0; i<landmarkNum-1; i++){
		Win = sqrt((pos_y[i+1] - pos_y[i])* (pos_y[i + 1] - pos_y[i]) +(pos_x[i+1] - pos_x[i])* (pos_x[i + 1] - pos_x[i]));
		for(y = pos_y[i]-Win/2; y < pos_y[i] + Win/2; y++){
			for(x = pos_x[i]-Win/2; x < pos_x[i] + Win/2; x++){
				if (y >= 0 && y < height && x >= 0 && x <= width) {
					int idx = LBP_landmark_img.at<uchar>(y, x);
					histo[idx]++;
				}
			}
		}

		denomi = 0;
		for(yy = 0; yy < UNIFORM; yy++){
			denomi += histo[yy]*histo[yy];
		}
		for(yy = 0; yy < UNIFORM; yy++){
			histo[yy] /= sqrt(denomi);
			if(denomi == 0) histo[yy] = 0;
			res_histo[cnt*UNIFORM + yy] = histo[yy];
		}
		cnt++;
	}

	free(histo);
	return res_histo;
}
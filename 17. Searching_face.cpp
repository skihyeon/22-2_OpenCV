//#include <stdio.h>
//#include <opencv2/imgproc.hpp>
//#include <opencv2/highgui.hpp>

//using namespace cv;

#include "main.h"

#define PI 3.1415926
#define WIN 36

void searching_face(Mat ref, Mat tar){
	float ref_HOG[300] = { 0, };
	get_ref_HOG(ref, ref_HOG);
	for (int i = 0; i < 300; i++) {
		if (ref_HOG[i]) printf("%d = %f\n", i, ref_HOG[i]);
	}
	int mask_x[9] = { -1,0,1,-1,0,1,-1,0,1 };
	int mask_y[9] = { -1,-1,-1,0,0,0,1,1,1 };
	int height = tar.rows;
	int width = tar.cols;
	int x, y, xx, yy, idx;
	float conv_x, conv_y, max = -1, min = 100000;
	float* mag = (float*)calloc(height * width, sizeof(float));
	float* dir = (float*)calloc(height * width, sizeof(float));
	float* tar_MAG = (float*)calloc(height * width, sizeof(float));
	float* tar_DIR = (float*)calloc(height * width, sizeof(float));
	float* tar_HOG = (float*)calloc(height * width, sizeof(float));
	float* simmap = (float*)calloc(height * width, sizeof(float));

	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			conv_x = conv_y = 0;
			for (yy = y - 1; yy <= y + 1; yy++) {
				for (xx = x - 1; xx <= x + 1; xx++) {
					if (yy >= 0 && yy < height && xx >= 0 && xx < width) {
						conv_x += tar.at<uchar>(yy, xx) * mask_x[(yy - (y - 1) * 3 + (xx - (x - 1)))];
						conv_y += tar.at<uchar>(yy, xx) * mask_y[(yy - (y - 1) * 3 + (xx - (x - 1)))];
					}
				}
			}
			conv_x /= 9.0;
			conv_y /= 9.0;

			mag[y * width + x] = sqrt(conv_x * conv_x + conv_y * conv_y);
			dir[y * width + x] = atan2(conv_y, conv_x);
			dir[y * width + x] = dir[y * width + x] * 180.0 / PI;
			if (dir[y * width + x] < 0) dir[y * width + x] += 180;
		}
	}


	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			idx = 0;
			for (yy = y - WIN / 2; yy < y + WIN / 2; yy++) {
				for (xx = x - WIN / 2; xx < x + WIN / 2; xx++) {
					if (yy >= 0 && yy < height && xx >= 0 && xx < width) {
						tar_MAG[idx] = mag[yy * width + xx];
						tar_DIR[idx] = dir[yy * width + xx];
						idx++;
					}
				}
			}
			get_tar_HOG(tar_MAG, tar_DIR, tar_HOG);
			
			simmap[y * width + x] = compute_sim(ref_HOG, tar_HOG);

			if (min > simmap[y * width + x]) min = simmap[y * width + x];
			if (max < simmap[y * width + x]) max = simmap[y * width + x];
		}
	}

	Mat result(height, width, CV_8UC1);

	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			result.at<uchar>(y, x) = 255 * (simmap[y * width + x] - min) / (max - min);
		}
	}

	imwrite("C:\\22-2_CV\\images\\result\\9week_result.bmp", result);
	free(mag);
	free(dir);
	free(tar_MAG);
	free(tar_DIR);
	free(tar_HOG);
	free(simmap);
}
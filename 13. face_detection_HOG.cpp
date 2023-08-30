#include <stdio.h>
#include <stdlib.h>
#include <opencv/highgui.h>
#define PI 3.1415926
#define BIN 9
using namespace cv;

float ref_HOG[300] = { 0, };

void get_ref_HOG(Mat img) {
	int mask_x[9] = { -1, 0, 1, -1, 0, 1, -1, 0, 1 };
	int mask_y[9] = { -1, -1, -1, 0, 0, 0, 1, 1, 1 };
	int height = img.rows;
	int width = img.cols;
	int x, y, xx, yy, idx;
	float conv_x, conv_y;

	int BLK = width / 3;
	int binNum = 9;
	int cnt = 0;
	int denomi;

	float* mag = (float*)calloc(height * width, sizeof(float));
	float* dir = (float*)calloc(height * width, sizeof(float));
	float* histo = (float*)calloc(BIN, sizeof(float));

	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			conv_x = conv_y = 0;
			for (yy = y - 1; yy <= y + 1; yy++) {
				for (xx = x - 1; xx <= x + 1; xx++) {
					if (yy >= 0 && yy < height && xx >= 0 && xx < width) {
						conv_x += img.at<uchar>(yy, xx) * mask_x[(yy - (y - 1)) * 3 + (xx - (x - 1))];
						conv_y += img.at<uchar>(yy, xx) * mask_y[(yy - (y - 1)) * 3 + (xx - (x - 1))];
					}
				}
			}
			conv_x /= 9.0;
			conv_y /= 9.0;

			mag[y * width + x] = sqrt(conv_x * conv_x + conv_y + conv_y);
			dir[y * width + x] = atan2(conv_y, conv_x);
			dir[y * width + x] = dir[y * width + x] * 180.0 / PI;
			if (dir[y * width + x] < 0) dir[y * width + x] += 180;
		}
	}

	//compute HOG
	for (y = 0; y < height - BLK; y += BLK / 2) {
		for (x = 0; x < width - BLK; x += BLK / 2) {
			for (yy = 0; yy < binNum; yy++)	histo[yy] = 0;
			for (yy = y; yy < y + BLK; yy++) {
				for (xx = x; xx < x + BLK; xx++) {
					idx = dir[yy * width + xx] / 20.0;
					histo[idx] += mag[yy * width + xx];
				}
			}

			//nomalization
			denomi = 0;
			for (yy = 0; yy < binNum; yy++) {
				denomi += histo[yy] * histo[yy];
			}
			for (yy = 0; yy < binNum; yy++) {
				histo[yy] /= sqrt(denomi);
				if (denomi == 0) histo[yy] = 0;
				ref_HOG[cnt * binNum + yy] = histo[yy];
			}
			cnt++;
		}
	}
	free(mag);
	free(dir);
	free(histo);
}




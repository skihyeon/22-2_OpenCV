#include <stdio.h>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#define PI 3.1415926
#define WIN 36

using namespace cv;

void get_tar_HOG(float* tar_MAG, float* tar_DIR, float* tar_HOG) {
	int x, y, xx, yy, idx;
	int binNum = 9;
	int cnt = 0;
	float denomi;
	int k = 0;
	float* histo = (float*)calloc(binNum, sizeof(float));


	for (yy = 0; yy < binNum; yy++) histo[yy] = 0;
	for (yy = 0; yy < 12; yy++) {
		for (xx = 0; xx < 12; xx++) {
			idx = tar_DIR[yy * 12 + xx] / 20.0;
			histo[idx] += tar_MAG[yy * 12 + xx];
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
		tar_HOG[cnt * binNum + yy] = histo[yy];
	}
	cnt++;
	free(histo);
}
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#define W 3
#define PI 3.1415926
#define BLK 16

using namespace cv;

void Assignment(Mat ref, Mat tar) {
	//loop index
	int x, y, xx, yy;
	//ref img height, width
	int h_r = ref.rows;
	int w_r = ref.cols;
	//tar img height, width
	int h_t = tar.rows;
	int w_t = tar.cols;

	//ref magnitude
	float* mag_x_r = (float*)calloc(h_r * w_r, sizeof(float));
	float* mag_y_r = (float*)calloc(h_r * w_r, sizeof(float));
	//tar magnitude
	float* mag_x_t = (float*)calloc(h_t * w_t, sizeof(float));
	float* mag_y_t = (float*)calloc(h_t * w_t, sizeof(float));

	//convolution variable
	float conv_x_r, conv_y_r, conv_x_t, conv_y_t;
	//to save Corner Point
	int* cornerP_r = (int*)calloc(h_r * w_r * 2, sizeof(int));
	int* cornerP_t = (int*)calloc(h_t * w_t * 2, sizeof(int));

	//ref direction
	float* deg_r = (float*)calloc(h_r * w_r, sizeof(float));
	//tar direction
	float* deg_t = (float*)calloc(h_t * w_t, sizeof(float));

	//to find R value
	float Ix, Iy, IxIy, det, tr;
	float k = 0.04, th = 20;
	float* R_r = (float*)calloc(h_r * w_r, sizeof(float));
	float* R_t = (float*)calloc(h_t * w_t, sizeof(float));

	int mask_x[9] = {-1,-2,-1,0,0,0,1,2,1};
	int mask_y[9] = {-1,0,1,-2,0,2,-1,0,-1};

	//Make Result Img(ref + tar)
	Mat res(h_r, w_r + w_t, CV_8UC3);

	for (y = 0; y < h_r; y++) {
		for (x = 0; x < w_r; x++) {
			res.at<Vec3b>(y, x)[0] = ref.at<uchar>(y, x);
			res.at<Vec3b>(y, x)[1] = ref.at<uchar>(y, x);
			res.at<Vec3b>(y, x)[2] = ref.at<uchar>(y, x);
		}
		for (x = w_r; x <w_r+ w_t; x++) {
			res.at<Vec3b>(y, x)[0] = tar.at<uchar>(y, x-w_r);
			res.at<Vec3b>(y, x)[1] = tar.at<uchar>(y, x-w_r);
			res.at<Vec3b>(y, x)[2] = tar.at<uchar>(y, x-w_r);
		}
	}

	//get the magnitude of ref
	for (y = 0; y < h_r; y++) {
		for (x = 0; x < w_r; x++) {
			conv_x_r = conv_y_r = 0;
			for (yy = y - W / 2; yy <= y + W / 2; yy++) {
				for (xx = x - W / 2; xx <= x + W / 2; xx++) {
					if (yy >= 0 && yy <= h_r && xx >= 0 && xx <= w_r) {
						conv_x_r += (ref.at<uchar>(yy, xx) * mask_x[(yy - (y - W / 2)) * 3 + (xx - (x - W / 2))] / 255.0);
						conv_y_r += (ref.at<uchar>(yy, xx) * mask_y[(yy - (y - W / 2)) * 3 + (xx - (x - W / 2))] / 255.0);
					}
				}
			}
			mag_x_r[y * w_r + x] = conv_x_r;
			mag_y_r[y * w_r + x] = conv_y_r;

			//get the direction of ref
			float dir = atan2(conv_y_r, conv_x_r);
			dir = dir * 180.0 / PI;
			dir = (dir < 0) ? dir += 180 : dir;
			deg_r[y * w_r + x] = dir;
		}
	}

	//get the magnitude of tar
	for (y = 0; y < h_t; y++) {
		for (x = 0; x < w_t; x++) {
			conv_x_t = conv_y_t = 0;
			for (yy = y - W / 2; yy <= y + W / 2; yy++) {
				for (xx = x - W / 2; xx <= x + W / 2; xx++) {
					if (yy >= 0 && yy <= h_r && xx >= 0 && xx <= w_r) {
						conv_x_t += (tar.at<uchar>(yy, xx) * mask_x[(yy - (y - W / 2)) * 3 + (xx - (x - W / 2))] / 255.0);
						conv_y_t += (tar.at<uchar>(yy, xx) * mask_y[(yy - (y - W / 2)) * 3 + (xx - (x - W / 2))] / 255.0);
					}
				}
			}
			mag_x_t[y * w_t + x] = conv_x_t;
			mag_y_t[y * w_t + x] = conv_y_t;

			//get the direction of tar
			float dir = atan2(conv_y_t, conv_x_t);
			dir = dir * 180.0 / PI;
			dir = (dir < 0) ? dir += 180 : dir;
			deg_t[y * w_t + x] = dir;
		}
	}

	//find R of ref
	for (y = 0; y < h_r; y++) {
		for (x = 0; x < w_r; x++) {
			Ix = Iy = IxIy = 0;
			for (yy = y - W / 2; yy <= y + W / 2; yy++) {
				for (xx = x - W / 2; xx <= x + W / 2; xx++) {
					if (yy >= 0 && yy <= h_r && xx >= 0 && xx <= w_r) {
						Ix += mag_x_r[yy * w_r + xx] * mag_x_r[yy * w_r + xx];
						Iy += mag_y_r[yy * w_r + xx] * mag_y_r[yy * w_r + xx];
						IxIy += mag_x_r[yy * w_r + xx] * mag_y_r[yy * w_r + xx];
					}
				}
			}
			det = Iy * Ix - IxIy * IxIy;
			tr = Ix + Iy;

			R_r[y * w_r + x] = det - k * tr * tr;
			if (R_r[y * w_r + x] > th) cornerP_r[y * w_r + x] = 1;
		}
	}
	for (y = 0; y < h_r; y++) {
		for (x = 0; x < w_r; x++) {
			if (cornerP_r[y * w_r + x] == 1) {
				Scalar c;
				Point pCenter;
				int radius = 1;

				pCenter.x = x;
				pCenter.y = y;
				c.val[0] = 0;
				c.val[1] = 255;
				c.val[2] = 0;
				circle(res, pCenter, radius, c, 2, 8, 0);
			}
		}
	}

	imwrite("C:\\22-2_CV\\images\\4week_res.bmp", res);
	
	//find R of tar
	for (y = 0; y < h_t; y++) {
		for (x = 0; x < w_t; x++) {
			Ix = Iy = IxIy = 0;
			for (yy = y - W / 2; yy <= y + W / 2; yy++) {
				for (xx = x - W / 2; xx <= x + W / 2; xx++) {
					if (yy >= 0 && yy <= h_t && xx >= 0 && xx <= w_t) {
						Ix += mag_x_t[yy * w_t + xx] * mag_x_t[yy * w_t + xx];
						Iy += mag_y_t[yy * w_t + xx] * mag_y_t[yy * w_t + xx];
						IxIy += mag_x_t[yy * w_t + xx] * mag_y_t[yy * w_t + xx];
					}
				}
			}
			det = Ix * Iy - IxIy * IxIy;
			tr = Ix + Iy;

			R_t[y * w_t + x] = det - k * tr * tr;
			if (R_t[y * w_t + x] > th) cornerP_t[y * w_t + x] = 1;
		}
	}

	//Corner Point x, y and HOG value save
	float HOG_r[300][300][9] = { 0, };


	//Compute the HOG of corner point_ref
	for (y = 0; y < h_r-BLK; y++) {
		for (x = 0; x < w_r-BLK; x++) {
			if (cornerP_r[y * w_r + x] == 1) {
				for (yy = y; yy <= y + BLK; yy++) {
					for (xx = x; xx <= x + BLK; xx++) {
						int idx = (int)deg_r[yy * BLK + xx];
						HOG_r[y][x][idx] += (sqrt(mag_x_r[yy * BLK + xx] * mag_x_r[yy * BLK + xx] + mag_y_r[yy * BLK + xx] * mag_y_r[yy * BLK + xx]));
					}
				}
			}
		}
	}

	//for (y = 0; y < h_r - BLK; y++) {
	//	for (x = 0; x < w_r - BLK; x++) {
	//		if (cornerP_r[y * w_r + x] == 1) {
	//			for (int i = 0; i < 9; i++) {
	//				printf("HOG[%d][%d][%d] = %f ", y, x, i, HOG_r[y][x][i]);
	//			}
	//			printf("\n");
	//		}
	//	}
	//}

	free(R_r);
	free(R_t);
	free(cornerP_r);
	free(cornerP_t);
	free(mag_x_r);
	free(mag_y_r);
	free(deg_r);
	free(deg_t);

}

int main() {
	Mat img_ref = imread("C:\\22-2_CV\\images\\ref.bmp", CV_LOAD_IMAGE_GRAYSCALE);
	Mat img_tar = imread("C:\\22-2_CV\\images\\tar.bmp", CV_LOAD_IMAGE_GRAYSCALE);
	Assignment(img_ref, img_tar);

	return 0;
}
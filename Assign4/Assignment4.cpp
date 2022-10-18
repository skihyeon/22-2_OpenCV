#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#define W 3
#define PI 3.1415926
#define BLK 16
#define BIN 9
#define Window 15

using namespace cv;

int* non_maxim_supp(float* R, int* cornerMap, int height, int width);
int* get_corner(Mat input);
float** get_HOG_of_corner(Mat input, int* cornerMap);
Point** Matching_corner(Mat ref, Mat tar,int* conrerMap_ref, int* cornerMap_tar);
Mat make_res_img(Mat ref, Mat tar, Point** cornerPos);


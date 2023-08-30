#define _CRT_SECURE_NO_WARNINGS
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <math.h>
#define Pi 3.141592

using namespace cv;

void ImgResize_NN(Mat input, float scale) {
	int x, y;
	int height, width;
	int re_height, re_width;
	float pos_x, pos_y;
	int sx, sy;

	height = input.rows;
	width = input.cols;
	
	re_height = height * scale;
	re_width = width * scale;

	Mat result(re_height, re_width, CV_8UC1);
	for (y = 0; y < re_height; y++) {
		for (x = 0; x < re_width; x++) {
			
			pos_x = (1.0 / scale) * x;
			pos_y = (1.0 / scale) * y;

			////Nearest Neighbor
			sx = (int)(pos_x + 0.5);
			sy = (int)(pos_y + 0.5);

			result.at<uchar>(y, x) = input.at<uchar>(sy, sx);

		}
	}
	//imwrite("C:\\22-2_CV\\images\\result_NN.png",result);
	imshow("original", input);
	imshow("resized", result);
	while (1) {
		waitKey(500);
	}
}


void ImgResize_BiLinear(Mat input, float scale) {
	int x, y;
	int height, width;
	int re_height, re_width;
	float pos_x, pos_y;
	int sx, sy;
	float w_x, w_y;
	height = input.rows;
	width = input.cols;

	re_height = height * scale;
	re_width = width * scale;

	Mat result(re_height, re_width, CV_8UC1);
	for (y = 0; y < re_height; y++) {
		for (x = 0; x < re_width; x++) {

			w_x = pos_x = (1.0 / scale) * x;
			w_y = pos_y = (1.0 / scale) * y;

			//get the weight
			w_x = w_x - (int) w_x;
			w_y = w_y - (int) w_y;

			sx = (int)pos_x;
			sy = (int)pos_y;


			uchar p1 = (1 - w_x) * input.at<uchar>(sy, sx) + w_x * input.at<uchar>(sy+1, sx);
			uchar p2 = (1 - w_y) * input.at<uchar>(sy, sx) + w_y * input.at<uchar>(sy, sx+1);
			uchar p3 = (1 - w_y) * input.at<uchar>(sy+1, sx) + w_y * input.at<uchar>(sy + 1, sx + 1);
			uchar p4 = (1 - w_x) * input.at<uchar>(sy, sx+1) + w_x * input.at<uchar>(sy + 1, sx + 1);

			result.at<uchar>(y, x) = ((1-w_x)*p1 + (1-w_y)*p2 + w_y*p3 + w_x*p4) / 2;

		}
	}
	imwrite("C:\\22-2_CV\\images\\result_BiLinear.png", result);
}

void ImgRotate_NN(Mat input, float deg) {
	int x, y;
	int height, width;
	int cx, cy;
	float pos_x, pos_y;
	int sx, sy;
	float rx, ry;
	float rad = deg * Pi / 180.0;
	float R[2][2] = { {cos(rad),sin(rad)},{-sin(rad),cos(rad)} };

	height = input.rows;
	width = input.cols;

	Mat result(height, width, CV_8UC1);

	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			cx = x - width / 2;
			cy = y - height / 2;
			//if (cx >= 0 && cx < width && cy >= 0 && cy < height) {
				pos_x = R[0][0] * cx + R[0][1] * cy;
				pos_y = R[1][0] * cx + R[1][1] * cy;

				pos_x += width / 2;
				pos_y += height / 2;

				sx = (int)(pos_x + 0.5);
				sy = (int)(pos_y + 0.5);
			//}
			if (sx >= 0 && sx < width && sy >= 0 && sy < height)
				result.at<uchar>(y, x) = input.at<uchar>(sy, sx);
			else
				result.at<uchar>(y, x) = 0;
		}
	}
	imshow("original", input);
	imshow("rotated", result);
	while (1)
		waitKey(500);

	//imwrite("C:\\22-2_CV\\images\\result_rotNN.png", result);
}

void ImgRotate_BiLinear(Mat input, float deg) {
	int x, y;
	int height, width;
	int cx, cy;
	float pos_x, pos_y;
	int sx, sy;
	float w_x, w_y;
	float rad = deg * Pi / 180.0;
	float R[2][2] = { {cos(rad),sin(rad)},{-sin(rad),cos(rad)} };

	height = input.rows;
	width = input.cols;

	Mat result(height, width, CV_8UC1);

	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			cx = x - width / 2;
			cy = y - height / 2;

			pos_x = R[0][0] * cx + R[0][1] * cy;
			pos_y = R[1][0] * cx + R[1][1] * cy;

			w_x = pos_x = pos_x + width / 2;
			w_y = pos_y = pos_y + height / 2;

			//get the weight
			w_x = w_x - (int)w_x;
			w_y = w_y - (int)w_y;

			sx = (int)pos_x;
			sy = (int)pos_y;


			uchar p1 = (1 - w_x) * input.at<uchar>(sy, sx) + w_x * input.at<uchar>(sy + 1, sx);
			uchar p2 = (1 - w_y) * input.at<uchar>(sy, sx) + w_y * input.at<uchar>(sy, sx + 1);
			uchar p3 = (1 - w_y) * input.at<uchar>(sy + 1, sx) + w_y * input.at<uchar>(sy + 1, sx + 1);
			uchar p4 = (1 - w_x) * input.at<uchar>(sy, sx + 1) + w_x * input.at<uchar>(sy + 1, sx + 1);

			if (sx >= 0 && sx < width-1 && sy >= 0 && sy < height-1)
				result.at<uchar>(y, x) = ((1 - w_x) * p1 + (1 - w_y) * p2 + w_y * p3 + w_x * p4) / 2;
			else
				result.at<uchar>(y, x) = 0;
		}
	}


	imwrite("C:\\22-2_CV\\images\\result_rotBL.png", result);
}


int main() {
	Mat img = imread("C:\\22-2_CV\\images\\test.bmp", CV_LOAD_IMAGE_GRAYSCALE);
	float scale = 0;
	float deg = 0;
	//printf("Input your scale factor : ");
	//scanf("%f", &scale);
	printf("Input your Roatate degree : ");
	scanf("%f", &deg);

	//ImgResize_NN(img, scale);
	//ImgResize_BiLinear(img, scale);
	ImgRotate_NN(img, deg);
	//ImgRotate_BiLinear(img, deg);
}
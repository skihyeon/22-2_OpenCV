#define _CRT_SECURE_NO_WARNINGS
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <math.h>
#define Pi 3.141592

using namespace cv;

void ImgRotate(Mat input, float deg) {
	int x, y;
	int height, width;
	int cx, cy;
	float pos_x, pos_y;
	float min_rx, max_rx, min_ry, max_ry;
	int sx, sy;
	float rx, ry;
	float rad = deg * Pi / 180.0;
	float deg45 = 45 * Pi / 180.0;
	float R[2][2] = { {cos(rad),sin(rad)},{-sin(rad),cos(rad)} };

	height = input.rows;
	width = input.cols;

	min_rx = R[0][0] * (0 - width / 2) + R[0][1] * (height / 2);
	max_rx = R[1][0] * (width / 2) + R[1][1] * (0 - height / 2);
	min_ry = R[1][0] * (0 - width / 2) + R[1][1] * (0 - height / 2);
	max_ry = R[1][0] * (width / 2) + R[1][1] * (height / 2);

	int rx_0 = (int)(min_rx + width / 2);
	int rx_1 = (int)(max_rx + width / 2);
	int ry_0 = (int)(min_ry + height / 2);
	int ry_1 = (int)(max_ry + height / 2);

	Mat result(height + (ry_1 - ry_0), width + (ry_1 - ry_0), CV_8UC1);

	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			cx = x - width / 2;
			cy = y - height / 2;

			pos_x = R[0][0] * cx + R[0][1] * cy;
			pos_y = R[1][0] * cx + R[1][1] * cy;

			pos_x += width / 2;
			pos_y += height / 2;

			sx = (int)(pos_x + 0.5);
			sy = (int)(pos_y + 0.5);

			if (sx >= 0 && sx < width && sy >= 0 && sy < height)
				result.at<uchar>(y, x) = input.at<uchar>(sy, sx);
			else
				result.at<uchar>(y, x) = 0;
		}
	}


	imwrite("C:\\22-2_CV\\images\\result_rot_nocrop.png", result);

}


int main() {
	Mat img = imread("C:\\22-2_CV\\images\\lena.png", CV_LOAD_IMAGE_GRAYSCALE);
	float scale = 0;
	float deg = 0;
	//printf("Input your scale factor : ");
	//scanf("%f", &scale);
	printf("Input your Roatate degree : ");
	scanf("%f", &deg);

	//ImgResize(img, scale);
	ImgRotate(img, deg);
}
#define _CRT_SECURE_NO_WARNINGS
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;

void main() {
	Mat img = imread("C:\\22-2_CV\\images\\lena.png", CV_LOAD_IMAGE_COLOR);
	int height = img.rows;
	int width = img.cols;
	int cx, cy;
	int BLK = 20;

	cx = height / 2;
	cy = width / 2;

	//scanf("%d %d", &cx, &cy);
	for (int y = cy-BLK; y < cy+BLK; y++) {
		for (int x = cx - BLK; x < cx + BLK; x++) {
			img.at<Vec3b>(y,x)[0] = 255;
			img.at<Vec3b>(y,x)[1] = 255;
			img.at<Vec3b>(y,x)[2] = 255;
		}
	}

	imshow("img", img);
	waitKey(10000);
}
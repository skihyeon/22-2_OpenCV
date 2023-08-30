#include <stdio.h>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/photo.hpp>

using namespace cv;

int main() {
	Mat img = imread("img.bmp");
	Mat msk(img.rows, img.cols, CV_8UC1);
	Mat res(img.rows, img.cols, CV_8UC3);

	for (int x = 0; x < img.cols; x++) {
		for (int y = 0; y < img.rows; y++) {
			if (img.at<Vec3b>(y, x)[2] == 255 && img.at<Vec3b>(y, x)[1] == 0 && img.at<Vec3b>(y, x)[1] == 0) {
				img.at<Vec3b>(y, x)[2] = 255;
				img.at<Vec3b>(y, x)[1] = 255;
				img.at<Vec3b>(y, x)[0] = 255;
				msk.at<uchar>(y, x) = 255;
			}
			else msk.at<uchar>(y, x) = 0;
		}
	}
	inpaint(img, msk, res, 2, INPAINT_TELEA);
	imwrite("res2.bmp", res);
}
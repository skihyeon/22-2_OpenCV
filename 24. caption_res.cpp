#include <stdio.h>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/photo.hpp>

using namespace cv;

int cornerMap[5000] = { 0, };

//https://btw-e-m.tistory.com/14
Mat Harris(Mat in) {
	Mat gray;

	resize(in, in, Size(500, 500));

	Mat in_co = in.clone();
	Mat show_in = in.clone();

	cvtColor(in, gray, COLOR_BGR2GRAY);

	Mat harris;
	Mat harris_norm = Mat::zeros(gray.size(), CV_32FC1);
	Mat abs;

	cornerHarris(gray, harris, 2, 3, 0.05, BORDER_DEFAULT);

	normalize(harris, harris_norm, 0, 255, NORM_MINMAX, CV_32FC1, Mat());

	convertScaleAbs(harris_norm, abs);

	for (int j = 0; j < harris_norm.rows; j++) {
		for (int i = 0; i < harris_norm.cols; i++) {
			if ((int)harris_norm.at<float>(j, i) > 109) {
				//cornerMap[j + i] = 1;
				circle(in_co, Point(i, j), 1, Scalar(0, 0, 255), 0, 1, 0);
			}
		}
	}
	return in_co;
}

int main() {
	Mat img = imread("cap.bmp");
	Mat corner;

	corner = Harris(img);

	Mat msk(corner.rows, corner.cols, CV_8UC1);
	Mat res(corner.rows, corner.cols, CV_8UC3);

	
	for (int y = 0; y < corner.rows; y++) {
		for (int x = 0; x < corner.cols; x++) {
				if (corner.at<Vec3b>(y, x)[2] == 255 && corner.at<Vec3b>(y, x)[1] == 0 && corner.at<Vec3b>(y, x)[0] == 0) {
					for (int xx = x - 30; xx < x + 30; xx++) {
						for (int yy = y - 30; yy < y+30; yy++) {
							if (xx >= 0 && xx < corner.cols && yy >= 0 && yy < corner.rows) {
								msk.at<uchar>(yy, xx) = 255;
							}
						}
					}
				}
				else msk.at<uchar>(y, x) = 0;
		}
	}
	inpaint(corner, msk, res, 20, INPAINT_TELEA);

	imshow("corner", corner);
	imshow("msk", msk);
	imshow("res", res);
	waitKey();
}
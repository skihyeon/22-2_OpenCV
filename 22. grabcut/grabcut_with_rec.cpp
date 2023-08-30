#include <stdio.h>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>

using namespace cv;

void main() {
	Mat image = imread("C:\\22-2_CV\\images\\grabcut_img.bmp");
	Rect rectangle(0, 0, 280, 180);

	Mat result;
	Mat bg, fg;

	grabCut(image, result, rectangle, bg, fg, 5, GC_INIT_WITH_RECT);
	compare(result, GC_PR_FGD, result, CMP_EQ);

	Mat foreground(image.size(), CV_8UC3, Scalar(255, 255, 255));
	image.copyTo(foreground, result);

	imshow("Foreground", foreground);
	waitKey(0);
}
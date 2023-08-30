#include <stdio.h>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>


#define MASK2


using namespace cv;

void GraphCut_mask(Mat input, Mat input_with_mask) {
	int x, y;
	int height = input.rows;
	int width = input.cols;


	Rect rectangle(0, 0, width - 1, height - 1);
	Mat mask = Mat::ones(height, width, CV_8UC1) * GC_PR_BGD;


	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			if (input_with_mask.at<Vec3b>(y, x)[0] == 255 && input_with_mask.at<Vec3b>(y, x)[1] == 0 && input_with_mask.at<Vec3b>(y, x)[2] == 0) {
				mask.at<uchar>(y, x) = GC_BGD;
			}
			else if(input_with_mask.at<Vec3b>(y, x)[0] == 0 && input_with_mask.at<Vec3b>(y, x)[1] == 0 && input_with_mask.at<Vec3b>(y, x)[2] == 255){
				mask.at<uchar>(y, x) = GC_FGD;
			}
		}
	}

	Mat final_result = Mat::zeros(height, width, CV_8UC3);
	Mat mask_result = Mat::zeros(height, width, CV_8UC1);

	Mat bg, fg;

	final_result = input.clone();

	grabCut(input, mask, rectangle, bg, fg, 8, GC_INIT_WITH_MASK);

	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			if (mask.at<uchar>(y, x) == GC_FGD || mask.at<uchar>(y, x) == GC_PR_FGD) {
				mask_result.at<uchar>(y, x) = 255;
			}
			else {
				final_result.at<Vec3b>(y, x)[0] = 255;
				final_result.at<Vec3b>(y, x)[1] = 255;
				final_result.at<Vec3b>(y, x)[2] = 255;
			}
		}
	}


#ifdef MASK2
	Mat mask2 = Mat::ones(height, width, CV_8UC1) * GC_PR_BGD;

	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			if (input_with_mask.at<Vec3b>(y, x)[0] == 255 && input_with_mask.at<Vec3b>(y, x)[1] == 0 && input_with_mask.at<Vec3b>(y, x)[2] == 0) {
				mask2.at<uchar>(y, x) = GC_PR_BGD;
			}
			else if (input_with_mask.at<Vec3b>(y, x)[0] == 0 && input_with_mask.at<Vec3b>(y, x)[1] == 0 && input_with_mask.at<Vec3b>(y, x)[2] == 255) {
				mask2.at<uchar>(y, x) = GC_PR_FGD;
			}
		}
	}

	Mat final_result2 = Mat::zeros(height, width, CV_8UC3);
	Mat mask_result2 = Mat::zeros(height, width, CV_8UC1);

	final_result2 = input.clone();
	grabCut(input, mask2, rectangle, bg, fg, 8, GC_INIT_WITH_MASK);
	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			if (mask2.at<uchar>(y, x) == GC_FGD || mask2.at<uchar>(y, x) == GC_PR_FGD) {
				mask_result2.at<uchar>(y, x) = 255;
			}
			else {
				final_result2.at<Vec3b>(y, x)[0] = 255;
				final_result2.at<Vec3b>(y, x)[1] = 255;
				final_result2.at<Vec3b>(y, x)[2] = 255;
			}
		}
	}
#endif


	while (1) {
		imshow("Input", input);
		imshow("Input_mask", input_with_mask);
		imshow("res", final_result);
#ifdef MASK2
		imshow("res_PR", final_result2);
#endif
		if (waitKey(30) >= 0) break;
	}
}


int main() {
	Mat input = imread("C:\\22-2_CV\\images\\grabcut_img.bmp");
	Mat input_with_mask = imread("C:\\22-2_CV\\images\\grabcut_img_with_mask.bmp ");

	GraphCut_mask(input, input_with_mask);
}
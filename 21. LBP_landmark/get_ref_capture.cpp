#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;

int ref_exist() {
	FILE* file;
	if ((file = fopen("ref_img.bmp", "r"))) {
		fclose(file);
		return 1;
	}
	return 0;
}

int get_ref_capture(Mat frame) {
	while (1) {
		if (!ref_exist()) {
			imshow("cam", frame);
			printf("There's no Reference Img. press 'c' to capture!\n");

			int key = waitKey(100);

			if (key == 'c') {
				imwrite("ref_img.bmp", frame);
				printf("Img captured!\n");
				break;
			}
		}
		else
			printf("Reference Img Already Exist.\n");
	}
	return 1;
}


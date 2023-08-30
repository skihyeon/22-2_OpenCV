//#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
//#include "utils.h"

using namespace cv;

void main() {
	VideoCapture capture(0);
	Mat frame;

	if (!capture.isOpened()) {
		printf("Couldn't open the web cam\n");
		return;
	}
	while (true) {
		capture >> frame;

		imshow("Video", frame);
		if (waitKey(30) >= 0) break; //if not use waitKey, Can't use video capture well
	}
}
#include <vector>
#include <iostream>
#include <fstream>

#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/objdetect/objdetect.hpp"


#include "main.h"

#include "get_ref_capture.h"
#include "ldmarkmodel.h"


#define UNIFORM 60


using namespace cv;
using namespace std;


void main() {
	ldmarkmodel modelt;
	string modelFilePath = "roboman-landmark-model.bin";

	while (!load_ldmarkmodel(modelFilePath, modelt)) {
		printf("Can't find model!\n");
		return;
	}

	VideoCapture capture(0);
	if (!capture.isOpened()) {
		printf("Cam opening failed\n");
		return;
	}

	Mat frame;
	Mat current_shape;

	int flag = 0;
	float score;
	float* ref_histo = (float*)calloc(100 * UNIFORM, sizeof(float));
	float* tar_histo = (float*)calloc(100 * UNIFORM, sizeof(float));
	Mat ref;

	while(1) {
		capture >> frame;
		modelt.track(frame, current_shape);
		int numLandmarks = current_shape.cols / 2;

		if (numLandmarks>60 && flag == 0) {
			int* ref_pos_x = (int*)calloc(numLandmarks + 1, sizeof(int));
			int* ref_pos_y = (int*)calloc(numLandmarks + 1, sizeof(int));
			flag = 1;
			Mat ref = frame;
			Mat ref_LBP_land(frame.rows, frame.cols, CV_8UC1);
			for (int j = 0; j < numLandmarks; j++) {
				int x = current_shape.at<float>(j);
				int y = current_shape.at<float>(j + numLandmarks);

				ref_pos_x[j] = x;
				ref_pos_y[j] = y;

				cv::circle(frame, cv::Point(x, y), 2, cv::Scalar(0, 0, 255), -1);
			}
			ref_LBP_land = LBP_landmark(ref, ref_pos_x, ref_pos_y, numLandmarks);
			ref_histo = LBP_landmark_histo(ref_LBP_land, ref_pos_x, ref_pos_y, numLandmarks);
			imshow("ref", ref);
		}

		modelt.track(frame, current_shape);
		int Landmark_num = current_shape.cols / 2;

		int* tar_pos_x = (int*)calloc(2*Landmark_num, sizeof(int));
		int* tar_pos_y = (int*)calloc(2*Landmark_num, sizeof(int));
		for (int j = 0; j < Landmark_num; j++) {
			int x = current_shape.at<float>(j);
			int y = current_shape.at<float>(j + Landmark_num);

			tar_pos_x[j] = x;
			tar_pos_y[j] = y;
		}
		Mat LBP_land(frame.cols, frame.rows, CV_8UC1);
		LBP_land = LBP_landmark(frame, tar_pos_x, tar_pos_y, Landmark_num);

		tar_histo = LBP_landmark_histo(LBP_land, tar_pos_x, tar_pos_y, Landmark_num);

		score = compute_sim(ref_histo, tar_histo);
		score *= 100;

		if(score >= 0) printf("score : %2.1f\n", score);

		if (score >= 90) {
			for (int i = 0; i < Landmark_num; i++)
				circle(frame, Point(tar_pos_x[i], tar_pos_y[i]), 2, cv::Scalar(0, 255, 0), -1);
		}
		else {
			for (int i = 0; i < Landmark_num; i++)
				circle(frame, Point(tar_pos_x[i], tar_pos_y[i]), 2, cv::Scalar(0, 0, 255), -1);
		}

		imshow("cam", frame);
		if (waitKey(5) >= 0) break;
	}
	free(ref_histo);
	free(tar_histo);
	return;
}
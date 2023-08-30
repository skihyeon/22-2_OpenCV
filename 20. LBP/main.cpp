#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv2/opencv.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/ml/ml.hpp>
#include "main.h"

#define TH 80
#define TH_lookup 95

//#define NO_LOOK_UP  
#define LOOK_UP  //Using Uniform look-up table

using namespace cv;
using namespace cv::ml;
using namespace std;


void main() {
	VideoCapture capture(0);
	Mat frame, frameGray, frame_face;
	Mat face_LBP;
	Mat face_ref;
	CascadeClassifier cascade;
	cascade.load("C:\\22-2_CV\\opencv\\sources\\data\\lbpcascades\\lbpcascade_frontalface.xml");
	vector<Rect> faces;
	int binNum = 256;
	int flag = 0; // for capture reference image.

	float* ref_histo = (float*)calloc(300 * binNum, sizeof(float));
	float* tar_histo = (float*)calloc(300 * binNum, sizeof(float));
	float score = 0;

	while (1) {
		capture >> frame;
		//resize(frame, frame, Size(160, 120));
		cascade.detectMultiScale(frame, faces, 1.1, 4, 0 | CV_HAAR_SCALE_IMAGE, Size(10, 10));

		for (int y = 0; y < faces.size(); y++) {
			Point lb(faces[y].x + faces[y].width, faces[y].y + faces[y].height);
			Point tr(faces[y].x, faces[y].y);

			cvtColor(frame, frameGray, COLOR_BGR2GRAY);
			frame_face = face(frameGray, faces[y].x, faces[y].y, faces[y].width, faces[y].height);

#ifdef NO_LOOK_UP
			face_LBP = LBP(frame_face, faces[y].x, faces[y].y, faces[y].width, faces[y].height);
#endif

#ifdef LOOK_UP
			face_LBP = LBP_lookup(frame_face, faces[y].x, faces[y].y, faces[y].width, faces[y].height);
#endif

			if (faces[y].width>200&&faces[y].height>200 && flag == 0) {
				flag = 1;
				face_ref = face_LBP;
				int ref_xpos = faces[y].x;
				int ref_ypos = faces[y].y;
				int ref_height = faces[y].height;
				int ref_width = faces[y].width;
				imshow("frace_ref", face_ref);
				ref_histo = LBP_histo(face_ref, ref_xpos, ref_ypos, ref_height, ref_width);
			}
			imshow("face_LBP", face_LBP);
			tar_histo = LBP_histo(face_LBP, faces[y].x, faces[y].y, faces[y].height, faces[y].width);

			score = compute_sim(ref_histo, tar_histo);
			score *= 100;
			printf("score : %2.1f\n", score);
#ifdef NO_LOOK_UP
			if (score > TH)	rectangle(frame, lb, tr, Scalar(0, 255, 0), 3, 8, 0);
#endif
#ifdef LOOK_UP
			if (score > TH_lookup) rectangle(frame, lb, tr, Scalar(0, 255, 0), 3, 8, 0);
#endif
			else rectangle(frame, lb, tr, Scalar(0, 0, 255), 3, 8, 0);
		}

		imshow("Video", frame);
		if (waitKey(30) >= 0) break;
	}
}
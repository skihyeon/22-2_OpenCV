#ifndef __LBP_LANDMARK_HISTO_H__
#define __LBP_LANDMARK_HISTO_H__

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv2/opencv.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/ml/ml.hpp>

using namespace cv;

float* LBP_landmark_histo(Mat LBP_landmark_img, int* pos_x, int* pos_y, int landmarkNum);

#endif
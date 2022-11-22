#ifndef __LBP_LANDMARK_H__
#define __LBP_LANDMARK_H__

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv2/opencv.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/ml/ml.hpp>

using namespace cv;

Mat LBP_landmark(Mat input, int* pos_x, int* pos_y, int landmarkNum);

#endif

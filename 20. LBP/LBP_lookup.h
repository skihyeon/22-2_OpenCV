#ifndef __LBP_LOOKUP_H__
#define __LBP_LOOKUP_H__

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv2/opencv.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/ml/ml.hpp>

using namespace cv;

Mat LBP_lookup(Mat input, int face_xpos, int face_ypos, int face_width, int face_height);

#endif
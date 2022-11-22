#pragma once
#include <stdio.h>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>

#include "SLIC.h"

using namespace cv;

void SLICsegmentation(Mat input);
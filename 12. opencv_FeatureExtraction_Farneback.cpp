#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/video/tracking.hpp> 
#include <vector>
#include <stdio.h>
#include <Windows.h>
#define Width 1024
#define Height 960

using namespace cv; using namespace std;
void main() {
	VideoCapture capture(0);
	int x, y, key;
	Mat flow, frame, prevFrame, img;

	while (1) {
		capture >> frame;
		resize(frame, img, Size(Width, Height));
		cvtColor(img, frame, CV_BGR2GRAY);
		if (prevFrame.empty() == false) {
			calcOpticalFlowFarneback(prevFrame, frame, flow, 0.4, 1, 12, 2, 8, 1.2, 0);
			for (y = 0; y < Height; y += 5) {
				for (x = 0; x < Width; x += 5) {
					const Point2f flowatxy = flow.at<Point2f>(y, x) * 2;
					line(img, Point(x, y), Point(cvRound(x + flowatxy.x), cvRound(y + flowatxy.y)), Scalar(255, 0, 0)); circle(img, Point(x, y), 1, Scalar(0, 0, 0), -1);
				}
			}
		frame.copyTo(prevFrame);
		}
		else frame.copyTo(prevFrame);
		key = waitKey(20);
		imshow("img", img);
	}
}
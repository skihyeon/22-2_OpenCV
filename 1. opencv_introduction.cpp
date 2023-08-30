#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>


using namespace cv;

void main() {
	Mat imgColor = imread("C:/22-2_CV/images/test.jpg", CV_LOAD_IMAGE_COLOR);
	//Mat imgColor = imread("C:\22-2_CV\images\test.jpg", CV_LOAD_IMAGE_COLOR);
	//Mat imgGray = imread("test.jpg", CV_LOAD_IMAGE_GRAYSCALE);

	imshow("color", imgColor);
	imwrite("C:/22-2_CV/images/result.jpg", imgColor);
	//imshow("gray", imgGray);

	waitKey(10000);
}
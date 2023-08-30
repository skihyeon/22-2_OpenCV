#include "main.h"

int main() {
	Mat img = imread("bird.bmp");

	resize(img, img, Size(480, 320));
	SLICsegmentation(img);
}
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <math.h>
#define PI 3.1415926
#define BLK 16


using namespace cv;

void gradient_computation(Mat input) {
	int x, y, xx, yy;
	int height = input.rows;
	int width = input.cols;
	float conv_x, conv_y;
	float min = 1000000, max = -1;
	float* mag = (float*)calloc(height * width, sizeof(float));
	float* deg = (float*)calloc(height * width, sizeof(float));

	int mask_x[9] = { -1,-2,-1,0,0,0,1,2,1 };
	int mask_y[9] = { -1,0,1,-2,0,2,-1,0,1 };

	Mat result_Edge(height, width, CV_8UC1);
	Mat result_Histo(height, width, CV_8UC1);

	int Histo[9] = { 0, };

	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			conv_x = conv_y = 0;
			for (yy = y - 1; yy <= y + 1; yy++) {
				for (xx = x - 1; xx <= x + 1; xx++) {
					if (yy >= 0 && yy <= height && xx >= 0 && xx <= width) {
						conv_x += input.at<uchar>(yy, xx) * mask_x[(yy - (y - 1)) * 3 + (xx - (x - 1))];
						conv_y += input.at<uchar>(yy, xx) * mask_y[(yy - (y - 1)) * 3 + (xx - (x - 1))];
					}
				}
			}
			conv_x /= 9.0;
			conv_y /= 9.0;

			mag[y * width + x] = sqrt(conv_x * conv_x + conv_y * conv_y);

			float dir = atan2(conv_y, conv_x);
			//deg[y * width + x] = atan2(conv_y, conv_x);
			dir = dir * 180.0 / PI;
			dir = (dir < 0) ? dir += 180 : dir;
			
			int index = dir / 20.0;
			Histo[index] += mag[y * width + x];

			if (max < mag[y * width + x]) max = mag[y * width + x];
			if (min > mag[y * width + x]) min = mag[y * width + x];
		}
	}

	for (int i = 0; i < 9; i++) printf("[%3d %3d] = %d\n", i*20,(i+1)*20, Histo[i]);

	//feature descriptor
	for (y = 0; y < height - BLK; y += BLK / 2) {
		for (x = 0; x < width - BLK; x += BLK / 2) {

		}
	}
	//
	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			result_Edge.at<uchar>(y, x) = 255*((mag[y * width + x]-min)/(max-min));
		}
	}
	imwrite("C:\\22-2_CV\\images\\result_Edge.png", result_Edge);
	imwrite("C:\\22-2_CV\\images\\result_Histo.png", result_Histo);

	free(mag);
	//free(dir);
}



int main() {
	int height, width;
	Mat imgColor = imread("C:\\22-2_CV\\images\\lena.png");

	Mat	imgGray = imread("C:\\22-2_CV\\images\\lecture3.bmp", CV_LOAD_IMAGE_GRAYSCALE);

	gradient_computation(imgGray);
}
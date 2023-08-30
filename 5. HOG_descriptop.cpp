#define _CRT_SECURE_NO_WARNINGS
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <math.h>
#define PI 3.1415926
#define BLK 16
#define EX_NUM 3780

using namespace cv;

float* HOG(Mat input) {
	int x, y, xx, yy;
	int height = input.rows;
	int width = input.cols;
	float conv_x, conv_y;
	float* mag = (float*)calloc(height * width, sizeof(float));
	float* deg = (float*)calloc(height * width, sizeof(float));

	int mask_x[9] = { -1,-2,-1,0,0,0,1,2,1 };
	int mask_y[9] = { -1,0,1,-2,0,2,-1,0,1 };

	//find magnitude & deg(tan)
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
			dir = dir * 180.0 / PI;
			dir = (dir < 0) ? dir += 180 : dir;

			deg[y * width + x] = dir;
		}
	}

	float Histo_cell[9] = { 0, };	// 8by8 Cell의 Histogram.
	float Block[9 * 4] = { 0, };	// Cell을 4개 모은 Block의 Histogram. 
	//모든 Block의 Histogram을 연결한 Array. ((128/16)-1)((64/16)-1) = 3780
	float* link_block = (float*)calloc((height/BLK) * (width/BLK) * 144, sizeof(float));

	int li = 0;

	for (y = 0; y <= height - BLK; y += BLK / 2) {
		for (x = 0; x <= width - BLK; x += BLK / 2) {
			int loop = 0;
			for (yy = y; yy <= y + BLK / 2; yy++) {
				for (xx = x; xx <= x + BLK / 2; xx++) {
					int idx = (int)deg[yy * 8 + xx] / 20.0;
					Histo_cell[idx] += mag[yy * 8 + xx];
				}
			}
			while (loop < 9) {
				Block[loop] = Histo_cell[loop % 9];
				loop++;
			}
			for (yy = y; yy <= y + BLK / 2; yy++) {
				for (xx = x + BLK / 2; xx <= x + BLK; xx++) {
					int idx = (int)deg[yy * 8 + xx] / 20.0;
					Histo_cell[idx] += mag[yy * 8 + xx];
				}
			}
			while (loop < 18) {
				Block[loop] = Histo_cell[loop % 9];
				loop++;
			}
			for (yy = y + BLK / 2; yy <= y + BLK; yy++) {
				for (xx = x; xx <= x + BLK / 2; xx++) {
					int idx = (int)deg[yy * 8 + xx] / 20.0;
					Histo_cell[idx] += mag[yy * 8 + xx];
				}
			}
			while (loop < 27) {
				Block[loop] = Histo_cell[loop % 9];
				loop++;
			}
			for (yy = y + BLK / 2; yy <= y + BLK; yy++) {
				for (xx = x + BLK / 2; xx <= x + BLK; xx++) {
					int idx = (int)deg[yy * 8 + xx] / 20.0;
					Histo_cell[idx] += mag[yy * 8 + xx];
				}
			}
			while (loop < 36) {
				Block[loop] = Histo_cell[loop % 9];
				loop++;
			}
			for (int i = 0; i < loop; i++) {
				link_block[loop * li + i] = Block[i];
			}
			li++;
		}
	}
	return link_block;
}


float *Comparor(float org[], float cp[]) {
	float* distance = (float*)calloc(EX_NUM, sizeof(float));

	for (int i = 0; i < EX_NUM; i++) {
		distance[i] = org[i] - cp[i];
		distance[i] = (distance[i] < 0) ? sqrt((-1) * distance[i]) : sqrt(distance[i]);
	}
	
	return distance;
}



int main() {
	Mat img_org = imread("C:\\22-2_CV\\images\\lecture3.bmp",CV_LOAD_IMAGE_GRAYSCALE);
	Mat imgCp1 = imread("C:\\22-2_CV\\images\\compare1.bmp", CV_LOAD_IMAGE_GRAYSCALE);
	Mat imgCp2 = imread("C:\\22-2_CV\\images\\compare2.bmp", CV_LOAD_IMAGE_GRAYSCALE);
	
	FILE* Fp1 = fopen("Histo_cp1.txt", "w");
	FILE* Fp2 = fopen("Histo_cp2.txt", "w");
	int h_org, w_org;

	h_org = img_org.rows;
	w_org = img_org.cols;

	float* Histo_org = (float*)calloc(EX_NUM, sizeof(float));
	float* Histo_cp1 = (float*)calloc(EX_NUM, sizeof(float));
	float* Histo_cp2 = (float*)calloc(EX_NUM, sizeof(float));
	Histo_org = HOG(img_org);
	Histo_cp1 = HOG(imgCp1);
	Histo_cp2 = HOG(imgCp2);

	float* Histo_org_cp1 = (float*)calloc(EX_NUM, sizeof(float));
	float* Histo_org_cp2 = (float*)calloc(EX_NUM, sizeof(float));

	Histo_org_cp1 = Comparor(Histo_org, Histo_cp1);
	Histo_org_cp2 = Comparor(Histo_org, Histo_cp2);

	for (int i = 0; i < EX_NUM; i++) {
		fprintf(Fp1, "%11.5f, ", Histo_org_cp1[i]);
		fprintf(Fp2, "%11.5f, ", Histo_org_cp2[i]);
	}

	fclose(Fp1);
	fclose(Fp2);

	free(Histo_org);
	free(Histo_cp1);
	free(Histo_cp2);
	//free(Histo_org_cp1);
	//free(Histo_org_cp2);
}

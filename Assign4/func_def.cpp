#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "func_def.h"
#define W 3
#define PI 3.1415926
#define BLK 16
#define BIN 9
#define Window 15

int mask_x[9]={
	-1, 0, 1,
	-1, 0, 1,
	-1, 0, 1
}
int mask_y[9]={
	-1, -1, -1,
	 0,  0,  0,
	 1,  1,  1 
}

int* non_maxima_supp(float* R, int* cornerMap, int height, int width){
	int x,y,xx,yy;
	int new_x, new_y;
	float max;
	int* maxima_res = (int*)calloc(height*width,sizeof(int));

	for(y=0; y<height; y++){
		for(x=0; x<width; x++){
			new_x=new_y=0;
			max = -1000;
			if(cornerMap[y*width+x]==1){
				for(yy=y-(Window/2); yy<=y+(Window/2); yy++){
					for(xx=x-(Window/2); xx<=x+(Window/2); xx++){
						if(cornerMap[yy*width+xx]==1){
							if(max<R[yy*width+xx]){
								max = R[yy*width+xx];
								new_x=xx;
								new_y=yy;
							}
						}
						cornerMap[yy*width+xx]=0;
					}
				}
			}
			maxima_res[new_y*width+new_x]=1;
		}
	}
	return maxima_res;
}

int* get_corner(Mat input){
	int x, y, xx, yy;
	float k=0.04, th=7;

	int height = input.rows;
	int width = input.cols;

	//Make corner result img
	Mat get_corner_res(hegiht,width, CV_8UC3);

	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			get_corner_res.at<Vec3b>(y, x)[0] = input.at<uchar>(y, x);
			get_corner_res.at<Vec3b>(y, x)[1] = input.at<uchar>(y, x);
			get_corner_res.at<Vec3b>(y, x)[2] = input.at<uchar>(y, x);
		}
	}

	//for get magnitude
	float* mag_x = (float*)calloc(height*width, sizeof(float));
	float* mag_y = (float*)calloc(height*width, sizeof(float));
	//for get R and corner pos
	float* R = (float*)calloc(height*width, sizeof(float));
	float* cornerMap = (float*)calloc(height*width, sizeof(float));
	float conv_x, conv_y;
	float Ix, Iy, IxIy, det, tr;

	//Compute magniutde
	for(y=0; y<height; y++){
		for(x=0; x<width; x++){
			conv_x=conv_y=0;
			for(yy=y-W/2; yy<=y+W/2; yy++){
				for(xx=x-W/2; xx<=x+W/2; xx++){
					if(yy>=0 && yy<=height && xx>=0 && xx<=width){
						conv_x += (input.at<uchar>(yy, xx) * mask_x[(yy - (y - W / 2)) * 3 + (xx - (x - W / 2))] / 255.0);
						conv_y += (input.at<uchar>(yy, xx) * mask_y[(yy - (y - W / 2)) * 3 + (xx - (x - W / 2))] / 255.0);
					}
				}
			}
			mag_x[y*width+x]=conv_x;
			mag_y[y*width+x]=conv_y;
		}
	}

	//Compute R
	for(y=0; y<height; y++){
		for(x=0; x<width; x++){
			Ix=Iy=IxIy=0;
			for(yy=y-(Window/2); yy<=y+(Window/2); yy++){
				for(xx=x-(Window/2); xx<=x+(Window/2); xx++){
					if(yy>=0 && yy<=height && xx>=0 && xx<=width){
						Ix += mag_x[yy*width+xx]*mag_x[yy*width+xx];
						Iy += mag_y[yy*width+xx]*mag_y[yy*width+xx];
						IxIy += mag_x[yy*width+xx]*mag_y[yy*width+xx];
					}
				}
			}
			det = Ix*Iy - IxIy*IxIy;
			tr = Ix*Iy;
			R[y*width+x]=det-k*tr*tr;

			if(R[y*width+x]>th) cornerMap[y*width+x]=1;
		}
	}
	cornerMap = non_maxima_supp(R, cornerMap, heigh, width);

	free(mag_x);
	free(mag_y);
	free(R);

	return cornerMap;
}

float** get_HOG_of_corner(Mat input, int* cornerMap){
	int x, y, xx, yy, xxx, yyy;
	float conv_x, conv_y;
	int loop=-1;
	int height = input.rows;
	int width = input.cols;

	float** HOG = (float**)calloc(height*width, sizeof(float));

	for(int i=0; i<height*widht; i++){
		*(HOG+i) = (float*)calloc(BIN, sizeof(float));
	}

	for(y=0; y<height; y++){
		for(x=0; x<width; x++){
			loop++;
			if(cornerMap[y*width+x]==1){
				for(yy=y-(Window/2); yy<=y+(Window/2); yy++){
					for(xx=x-(Window/2); xx<=x+(Window/2); xx++){
						if(yy>=0 && yy<=height && xx>=0 && xx<=width){
							for(yyy=yy-W/2; yyy<=yy+W/2; yyy++){
								for(xxx=xx-W/2; xxx<=xx+W/2; xxx++){
									if(yyy>=0 && yyy<=height && xxx>=0 && xxx<=width){
										conv_x += (input.at<uchar>(yyy,xxx)*mask_x[(yyy-(yy-W/2))*3+(xxx-(xx-W/2))]/255.0);
										conv_y += (input.at<uchar>(yyy,xxx)*mask_y[(yyy-(yy-W/2))*3+(xxx-(xx-W/2))]/255.0);
									}
								}
							}
						}
						conv_x /= 9.0;
						conv_y /= 9.0;

						float mag = sqrt(conv_x*conv_x+conv_y*conv_y);
						float dir = atan2(conv_y, conv_x);
						dir = dir*180/PI;
						dir = (dir<0) ? dir+=180 : dir;

						int idx = (int) dir/20.0;

						HOG[loop][idx] += mag;
					}
				}
			}
		}
	}


	return HOG;
}

Point** Matching_corner(Mat ref, Mat tar, int* cornerMap_ref, int* cornerMap_tar){
	int x, y, xx, yy;
	int h_r = ref.rows;
	int w_r = ref.cols;
	int h_t = tar.rows;
	int w_t = tar.cols;

	float distance;

	Point** cornerPos = (Point**)calloc(2, sizeof(Point*));

	cornerPos[0] = (Point*)calloc(h_r*w_r, sizeof(Point));
	cornerPos[1] = (Point*)calloc(h_t*w_t, sizeof(Point));

	float** HOG_ref = (float**)get_HOG(ref, cornerMap_ref);
	float** HOG_tar = (float**)get_HOG(tar, cornerMap_tar);

	for(y=0; y<h_r; y++){
		for(x=0; x<w_r; x++){
			if(cornerMap_ref[y*w_r+x]==1){
				for(yy=0; yy<h_t; yy++){
					for(xx=0; xx<w_t; xx++){
						if(cornerMap_tar[y*w_t+x]==1){
							distance = 0;
							for(int i=0; i<BIN; i++){
								int d = HOG_ref[y*w_r+x][i] - HOG_tar[yy*w_t+xx][i];
								d = (d<0) ? d*=(-1) : d;
								distance += d;
							}

							if(distance == 0) break;
							if(distance < 0.4){
								cornerPos[0][y*w_r+x].x = x;
								cornerPos[0][y*w_r+x].y = y;
								cornerPos[1][yy*w_t+xx].x = xx;
								cornerPos[1][yy*w_t+xx].y = yy;
							}
						}
					}
				}
			}
		}
	}
	return cornerPos;
}

Mat make_res_img(Mat ref, Mat tar, Point** cornerPos){
	int x,y;
	int height = ref.rows;
	int width_ref = ref.cols;
	int width_tar = tar.cols;

	Mat res(height, width_ref + width_tar, CV_8UC3);
	for(y=0; y<height; y++){
		for(x=0; x<width_ref; x++){
			res.at<Vec3b>(y,x)[0] = ref.at<uchar>(y,x);
			res.at<Vec3b>(y,x)[1] = ref.at<uchar>(y,x);
			res.at<Vec3b>(y,x)[2] = ref.at<uchar>(y,x);
		}
		for(x=width_ref; x<width_ref+width_tar; x++){
			res.at<Vec3b>(y,x)[0] = ref.at<uchar>(y,x-width_ref);
			res.at<Vec3b>(y,x)[1] = ref.at<uchar>(y,x-width_ref);
			res.at<Vec3b>(y,x)[2] = ref.at<uchar>(y,x-width_ref);
		}
	}

	Point P_r, P_t;
	int radius = 4;
	Scalar circle_ref, circle_tar, line_col;
	circle_ref.val[0] = 0;
	circle_ref.val[1] = 255;
	circle_ref.val[2] = 0;

	circle_tar.val[0] = 0;
	circle_tar.val[1] = 0;
	circle_tar.val[2] = 255;

	line_col.val[0] = 255;
	line_col.val[1] = 0;
	line_col.val[2] = 0;

	for(y=0; y<height; y++){
		for(x=0; x<width_ref; x++){
			P_r.x = cornerPos[0][y*width_ref+x].x;
			P_r.y = cornerPos[0][y*width_ref+x].y;
			P_t.x = cornerPos[1][y*width_tar+x].x + width_ref;
			P_t.y = cornerPos[1][y*width_tar+x].y;

			if(P_r.x>=0 && P_r.y>=0 && P_t.x>=0 && P_t.y>=0){
				circle(res, P_r, radius, 1, 8, 0);
				circle(res, P_t, radius, 1, 8, 0);
				line(res, P_r, P_t, line_col, 1, 9, 0);
			}
		}
	}
	return res;
}

int main(){
	Mat ref = imread("C:\\22-2_CV\\images\\ref.bmp", CV_LOAD_IMAGE_GRAYSCALE);
	Mat tar = imread("C:\\22-2_CV\\images\\tar.bmp", CV_LOAD_IMAGE_GRAYSCALE);

	int* cornerMap_ref = (int*)get_corner(ref);
	int* cornerMap_tar = (int*)get_corner(tar);

	Point** cornerPos = (Point **) Matching_corner(ref,tar,cornerMap_ref, cornerMap_tar);

	Mat res = make_res_img(ref, tar, cornerPos);
	imwrite("C:\\22-2_cV\\images\\Assign4.bmp",res);
	return 0;
}
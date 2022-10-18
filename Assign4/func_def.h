#ifndef func_def_H
#define fucn_def_H


int* non_maxima_supp(float* R, int* cornerMap, int height, int width);
int* get_corner(Mat input);
float** get_HOG_of_corner(Mat input, int* cornerMap);
Point** Matching_corner(Mat ref, Mat tar,int* conrerMap_ref, int* cornerMap_tar);
Mat make_res_img(Mat ref, Mat tar, Point** cornerPos);

#endif
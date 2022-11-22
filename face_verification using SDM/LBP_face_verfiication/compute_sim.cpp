#include <stdio.h>
#include <math.h>

#define UNIFORM 60
#define LANDMARK_NUM 70
float compute_sim(float* ref, float* tar) {
	int dim = UNIFORM * LANDMARK_NUM;
	float nomi = 0, denomi;

	int k;
	float refH = 0, tarH = 0;
	float score = 0;

	for (k = 0; k < dim; k++) {
		nomi += ref[k] * tar[k];
		refH += ref[k] * ref[k];
		tarH += tar[k] * tar[k];
	}
	denomi = sqrt(refH) * sqrt(tarH);
	score = nomi / denomi;

	return score;
}
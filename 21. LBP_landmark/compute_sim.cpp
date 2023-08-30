#include <stdio.h>
#include <math.h>

float compute_sim(float* ref, float* tar) {
	int dim = 256 * 49;
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
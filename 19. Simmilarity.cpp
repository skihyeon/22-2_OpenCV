#include <stdio.h>
#include <math.h>

float compute_sim(float* ref, float* tar) {
	int dim = 25 * 9;
	float nomi = 0, denomi;

	int k;
	float refMag = 0, tarMag = 0;
	float score = 0;

	for (k = 0; k < dim; k++) {
		nomi += ref[k] * tar[k];
		refMag += ref[k] * ref[k];
		tarMag += tar[k] * tar[k];
	}
	denomi = sqrt(refMag) * sqrt(tarMag);
	score = nomi / denomi;

	return score;
}
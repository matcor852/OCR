#include "Cost.h"


float CrossEntropy(float *predicted, float *expected, cui Size) {
	float cost = .0f;
	for (ui i=0; i<Size; i++)
		cost += expected[i] * logf(predicted[i]) +
				(1-expected[i]) * logf(1-predicted[i]);
	return -1.0f/(float)Size * cost;
}

float RMSE(float *predicted, float *expected, cui Size) {
	float cost = .0f;
	for (ui i=0; i<Size; i++) cost += powf((predicted[i] - expected[i]), 2);
	return sqrtf(1.0f/(float)Size * cost);
}

float MAE(float *predicted, float *expected, cui Size) {
	float cost = .0f;
	for (ui i=0; i<Size; i++) cost += absf(predicted[i]-expected[i]);
	return 1.0f/(float)Size * cost;
}

float MSE(float *predicted, float *expected, cui Size) {
	float cost = .0f;
	for (ui i=0; i<Size; i++) cost += powf((predicted[i] - expected[i]), 2);
	return 1.0f/(float)Size * cost;
}

float none_(float *arr) {

}

float sigmoid_(float *arr) {

}

float softmax_(float *arr) {

}

float argmax_(float *arr) {

}

float step_(float *arr) {

}

float relu_(float *arr) {

}

float leakyrelu_(float *arr) {

}











#include "Cost.h"


float CrossEntropy(float *predicted, float *expected, cui Size) {
	float cost = .0f;
	for (ui i=0; i<Size; i++) {
		if (predicted[i] <= 0 || 1-predicted[i] <= 0)
			printf("CrossEntropy encountered <= 0");
		cost += expected[i] * logf(predicted[i]) +
				(1-expected[i]) * logf(1-predicted[i]);
	}
	return (-1.0f/(float)Size) * cost;
}

float RMSE(float *predicted, float *expected, cui Size) {
	float res = MSE(predicted, expected, Size);
	if (res < 0) printf("RMSE encountered < 0");
	return sqrtf(res);
}

float MAE(float *predicted, float *expected, cui Size) {
	float cost = .0f;
	for (ui i=0; i<Size; i++) cost += absf(predicted[i]-expected[i]);
	return (1.0f/(float)Size) * cost;
}

float MSE(float *predicted, float *expected, cui Size) {
	float cost = .0f;
	for (ui i=0; i<Size; i++) cost += powf((predicted[i] - expected[i]), 2);
	return (1.0f/(float)Size) * cost;
}



float CrossEntropy_(float predicted, float expected) {
	if (absf(predicted - .0f) < FLT_EPSILON || absf(1-predicted - .0f) < FLT_EPSILON)
		printf("CrossEntropy_ encountered == 0");
	return -(expected*(1/predicted)+(1-expected)*(1/(1-predicted)));
}

float RMSE_(float predicted, float expected) {
	if (powf(predicted-expected, 2) < 0 ||
		absf(sqrtf(powf(predicted-expected, 2)) - .0f) < FLT_EPSILON)
			printf("RMSE_ encountered <= 0");
	return (predicted-expected)/sqrtf(powf(predicted-expected, 2));
}

float MAE_(float predicted, float expected) {
	if (absf(absf(predicted-expected) - .0f) < FLT_EPSILON) printf("MAE_ encountered <= 0");
	return (predicted-expected)/absf(predicted-expected);
}

float MSE_(float predicted, float expected) {
	return 2*(predicted-expected);
}



float none_(float *arr, cui Size, cui ieme) {
	return 0;
}

float sigmoid_(float *arr, cui Size, cui ieme) {
	if (ieme >= Size) {
		printf("Warning: sigmoid derivative index out of bound.\n");
		return 0;
	}
	float sigm = 1/(1+expf(-arr[ieme]));
	return sigm*(1-sigm);
}

float softmax_(float *arr, cui Size, cui ieme) {
	if (ieme >= Size) {
		printf("Warning: softmax derivative index out of bound.\n");
		return 0;
	}
	float sT = 0, tgt = expf(arr[ieme]);
	for (ui i=0; i<Size; i++) sT += expf(arr[i]);
	return tgt*(sT-tgt)/powf(sT, 2);
}

float argmax_(float *arr, cui Size, cui ieme) {
	if (ieme >= Size) {
		printf("Warning: argmax derivative index out of bound.\n");
		return 0;
	}
	printf("WARNING: argmax derivative not implemented");
	return 0;
}

float step_(float *arr, cui Size, cui ieme) {
	if (ieme >= Size) {
		printf("Warning: step derivative index out of bound.\n");
		return 0;
	}
	return (absf(arr[ieme] - .0f) < FLT_EPSILON) ? (float)ran_number() : .5f;
}

float relu_(float *arr, cui Size, cui ieme) {
	if (ieme >= Size) {
		printf("Warning: relu derivative index out of bound.\n");
		return 0;
	}
	return (arr[ieme] < 0) ? 0 : 1;
}

float leakyrelu_(float *arr, cui Size, cui ieme) {
	if (ieme >= Size) {
		printf("Warning: leakyrelu derivative index out of bound.\n");
		return 0;
	}
	return (arr[ieme] < 0) ? .01f : 1;
}


float (*get_deriv(const char *name))(float *arr, cui Size, cui ieme) {
	for (ui i=0; i < (sizeof(deriv_map)/sizeof(deriv_map[0])); i++)
		if (!strcmp(deriv_map[i].name,name)) return deriv_map[i].func;
	return NULL;
}

float (*get_cost(const char *name))(float *predicted,float *expected,cui Size) {
	for (ui i=0; i < (sizeof(cost_map)/sizeof(cost_map[0])); i++)
		if (!strcmp(cost_map[i].name,name)) return cost_map[i].func;
	return NULL;
}

float (*get_cost_deriv(const char *name))(float predicted,float expected) {
	for (ui i=0; i < (sizeof(cost_deriv_map)/sizeof(cost_deriv_map[0])); i++)
		if (!strcmp(cost_deriv_map[i].name,name)) return cost_deriv_map[i].func;
	return NULL;
}









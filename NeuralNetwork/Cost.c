#include "Cost.h"


long double CrossEntropy(long double *predicted, long double *expected, cui Size) {
	long double cost = .0L;
	for (ui i=0; i<Size; i++) {

        if (isnan(expected[i]) || isnan(predicted[i]) ||
            isnan(logl(predicted[i]+LDBL_EPSILON)))
        {
            printf("\n%LF, %LF\n%LF\n",
                   predicted[i], expected[i],
                   logl(predicted[i]+LDBL_EPSILON));
            exit(1);
        }
        cost += expected[i] * logl(predicted[i]+LDBL_EPSILON);
	}
	return -cost;
}

long double RMSE(long double *predicted, long double *expected, cui Size) {
	long double res = MSE(predicted, expected, Size);
	if (res < 0) printf("RMSE encountered < 0");
	return sqrtl(res+LDBL_EPSILON);
}

long double MAE(long double *predicted, long double *expected, cui Size) {
	long double cost = .0L;
	for (ui i=0; i<Size; i++) cost += absl(predicted[i]-expected[i]);
	return (1.0L/(long double)Size) * cost;
}

long double MSE(long double *predicted, long double *expected, cui Size) {
	long double cost = .0L;
	for (ui i=0; i<Size; i++) cost += powl((predicted[i] - expected[i]), 2);
	return (1.0L/(long double)Size) * cost;
}



long double CrossEntropy_(long double predicted, long double expected) {
    return -expected/(predicted+LDBL_EPSILON);
}

long double RMSE_(long double predicted, long double expected) {
	return (predicted-expected)/sqrtl(powl(predicted-expected, 2)+LDBL_EPSILON);
}

long double MAE_(long double predicted, long double expected) {
	return (predicted-expected)/(absl(predicted-expected)+LDBL_EPSILON);
}

long double MSE_(long double predicted, long double expected) {
	return 2*(predicted-expected);
}



long double none_(long double *arr, cui Size, cui ieme) {
	return 0;
}

long double sigmoid_(long double *arr, cui Size, cui ieme) {
	if (ieme >= Size) {
		printf("Warning: sigmoid derivative index out of bound.\n");
		return 0;
	}
	long double sigm = 1/(1+expl(-arr[ieme]));
	return sigm*(1-sigm);
}

long double softmax_(long double *arr, cui Size, cui ieme) {
	if (ieme >= Size) {
		printf("Warning: softmax derivative index out of bound.\n");
		return 0;
	}
	long double sT = 0, tgt = expl(arr[ieme]);
	for (ui i=0; i<Size; i++) sT += expl(arr[i]);
	if (isnan(tgt*(sT-tgt)/(powl(sT, 2)+LDBL_EPSILON))) {
        printf("\nNaN softmax_ : %LF, %LF, %LF, %LF\n", sT, tgt, tgt*(sT-tgt), powl(sT, 2));
        exit(2);
	}
	return tgt*(sT-tgt)/(powl(sT, 2)+LDBL_EPSILON);
}

long double argmax_(long double *arr, cui Size, cui ieme) {
	if (ieme >= Size) {
		printf("Warning: argmax derivative index out of bound.\n");
		return 0;
	}
	printf("WARNING: argmax derivative not implemented");
	return 0;
}

long double step_(long double *arr, cui Size, cui ieme) {
	if (ieme >= Size) {
		printf("Warning: step derivative index out of bound.\n");
		return 0;
	}
	return (absl(arr[ieme] - .0F) < LDBL_EPSILON) ? (long double)(ran_number()) : .5L;
}

long double relu_(long double *arr, cui Size, cui ieme) {
	if (ieme >= Size) {
		printf("Warning: relu derivative index out of bound.\n");
		return 0;
	}
	return (arr[ieme] < 0) ? 0 : 1;
}

long double leakyrelu_(long double *arr, cui Size, cui ieme) {
	if (ieme >= Size) {
		printf("Warning: leakyrelu derivative index out of bound.\n");
		return 0;
	}
	return (arr[ieme] < 0) ? .01L : 1.0L;
}


long double (*get_deriv(const char *name))(long double *arr, cui Size, cui ieme) {
	for (ui i=0; i < (sizeof(deriv_map)/sizeof(deriv_map[0])); i++)
		if (!strcmp(deriv_map[i].name,name)) return deriv_map[i].func;
	return NULL;
}

long double (*get_cost(const char *name))(long double *predicted,long double *expected,cui Size) {
	for (ui i=0; i < (sizeof(cost_map)/sizeof(cost_map[0])); i++)
		if (!strcmp(cost_map[i].name,name)) return cost_map[i].func;
	return NULL;
}

long double (*get_cost_deriv(const char *name))(long double predicted,long double expected) {
	for (ui i=0; i < (sizeof(cost_deriv_map)/sizeof(cost_deriv_map[0])); i++)
		if (!strcmp(cost_deriv_map[i].name,name)) return cost_deriv_map[i].func;
	return NULL;
}









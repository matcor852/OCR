#include "Cost.h"


ld CrossEntropy(ld *predicted, ld *expected, cui Size) {
	ld cost = .0L;
	for (ui i=0; i<Size; i++) {
        cost += expected[i] * logl(predicted[i]+EPS);
	}
	if (isnan(-cost)) {
        printf("\nnan in crossentropy\n");
        exit(4);
	}
	return -cost;
}

ld RMSE(ld *predicted, ld *expected, cui Size) {
	ld res = MSE(predicted, expected, Size);
	if (res < 0) printf("RMSE encountered < 0");
	return sqrtl(res+LDBL_EPSILON);
}

ld MAE(ld *predicted, ld *expected, cui Size) {
	ld cost = .0L;
	for (ui i=0; i<Size; i++) cost += absl(predicted[i]-expected[i]);
	return (1.0L/(ld)Size) * cost;
}

ld MSE(ld *predicted, ld *expected, cui Size) {
	ld cost = .0L;
	for (ui i=0; i<Size; i++) cost += powl((predicted[i] - expected[i]), 2);
	return (1.0L/(ld)Size) * cost;
}



ld CrossEntropy_(cld predicted, cld expected) {
    return -expected/(predicted+EPS);
}

ld RMSE_(cld predicted, cld expected) {
	return (predicted-expected)/sqrtl(powl(predicted-expected, 2)+LDBL_EPSILON);
}

ld MAE_(cld predicted, cld expected) {
	return (predicted-expected)/(absl(predicted-expected)+LDBL_EPSILON);
}

ld MSE_(cld predicted, cld expected) {
	return 2*(predicted-expected);
}



ld none_(ld *arr, cui Size, cui ieme) {
	return 0;
}

ld sigmoid_(ld *arr, cui Size, cui ieme) {
	if (ieme >= Size) {
		printf("Warning: sigmoid derivative index out of bound.\n");
		return 0;
	}
	cld sigm = 1/(1+expl(-arr[ieme]));
	return sigm*(1-sigm);
}

ld softmax_(ld *arr, cui Size, cui ieme) {
	if (ieme >= Size) {
		printf("Warning: softmax derivative index out of bound.\n");
		return 0;
	}
	ld max = arr[ieme];
	for (ui i=0; i<Size; i++) if (arr[i] > max) max = arr[i];
	for (ui i=0; i<Size; i++) arr[i] -= max;

	ld sT = 0, tgt = expl(arr[ieme]);
	for (ui i=0; i<Size; i++) {
        if (isnan(expl(arr[i]))) {
            printf("nan ,n1 : %LF", arr[i]);
            exit(4);
        }
        sT += expl(arr[i]);
	}

	if (isnan(tgt*(sT-tgt)/(powl(sT, 2)+LDBL_EPSILON))) {
        printf("\nNaN softmax_ : %LF, %LF, %LF, %LF, %LF\n",
               sT, tgt, tgt*(sT-tgt), powl(sT, 2), arr[ieme]);
        exit(2);
	}

	return tgt*(sT-tgt)/(powl(sT, 2)+EPS);
}

ld argmax_(ld *arr, cui Size, cui ieme) {
	if (ieme >= Size) {
		printf("Warning: argmax derivative index out of bound.\n");
		return 0;
	}
	printf("WARNING: argmax derivative not implemented");
	return 0;
}

ld step_(ld *arr, cui Size, cui ieme) {
	if (ieme >= Size) {
		printf("Warning: step derivative index out of bound.\n");
		return 0;
	}
	return (absl(arr[ieme] - .0F) < LDBL_EPSILON) ? (ld)(ran_number()) : .5L;
}

ld relu_(ld *arr, cui Size, cui ieme) {
	if (ieme >= Size) {
		printf("Warning: relu derivative index out of bound.\n");
		return 0;
	}
	return (arr[ieme] < 0) ? 0 : 1;
}

ld leakyrelu_(ld *arr, cui Size, cui ieme) {
	if (ieme >= Size) {
		printf("Warning: leakyrelu derivative index out of bound.\n");
		return 0;
	}
	return (arr[ieme] < 0) ? .01L : 1.0L;
}


ld (*get_deriv(const char *name))(ld *arr, cui Size, cui ieme) {
	for (ui i=0; i < (sizeof(deriv_map)/sizeof(deriv_map[0])); i++)
		if (!strcmp(deriv_map[i].name,name)) return deriv_map[i].func;
	return NULL;
}

ld (*get_cost(const char *name))(ld *predicted,ld *expected,cui Size) {
	for (ui i=0; i < (sizeof(cost_map)/sizeof(cost_map[0])); i++)
		if (!strcmp(cost_map[i].name,name)) return cost_map[i].func;
	return NULL;
}

ld (*get_cost_deriv(const char *name))(ld predicted, ld expected) {
	for (ui i=0; i < (sizeof(cost_deriv_map)/sizeof(cost_deriv_map[0])); i++)
		if (!strcmp(cost_deriv_map[i].name,name)) return cost_deriv_map[i].func;
	return NULL;
}









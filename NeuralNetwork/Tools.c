
#include "Tools.h"

void matr_display(float *arr, cui s, cui cols) {
	printf("\n\t[");
	for(ui i=0; i<s; i++) {
		if(i>0 && i%cols == 0) printf("\n\t ");
		printf("%f", (double)arr[i]);
		if (i<s-1) printf(", ");
	}
	printf("]\n");
}

float *fvec_alloc(cui n, bool zInit) {
	float *tmp = (float*) (zInit) ?
		calloc(n, sizeof(float)) : malloc(sizeof(float) * n);
	if (tmp == NULL) {
		printf("Error: Out of memory ...\n");
		exit(1);
	}
	return tmp;
}

char *cvec_alloc(cui n) {
	char *tmp = (char*) malloc(sizeof(char) * n);
	if (tmp == NULL) {
		printf("Error: Out of memory ...\n");
		exit(1);
	}
	return tmp;
}

float *fvec_rInit(cui n) {
	float *tmp = fvec_alloc(n, false);
	for (ui i=0; i<n; i++) {
		tmp[i] = (((float)ran_number() > .5f) ? -1 : 1) * (float)ran_number();
	}
	return tmp;
}

float absf(float nb) {
	return (nb < 0) ? -nb : nb;
}

void arr_shuffle(float *arr[], float *paired_arr[], cui Size) {
	for(ui i=0; i<Size; i++) {
		int n = (int)((ui)(ran_number() * 100000) % Size);
		float *temp = arr[i], *temp_paired = paired_arr[i];
		arr[i] = arr[n];
		arr[n] = temp;
		paired_arr[i] = paired_arr[n];
		paired_arr[n] = temp_paired;
	}
}














#include "Tools.h"

void matr_display(long double *arr, cui s, cui cols) {
	printf("\n\t[");
	for(ui i=0; i<s; i++) {
		if(i>0 && i%cols == 0) printf("\n\t ");
		printf("%f", (double)arr[i]);
		if (i<s-1) printf(", ");
	}
	printf("]\n");
}

long double *fvec_alloc(cui n, bool zInit) {
	long double *tmp = (long double*) (zInit) ?
		calloc(n, sizeof(long double)) : malloc(sizeof(long double) * n);
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

long double *fvec_rInit(cui n) {
	long double *tmp = fvec_alloc(n, false);
	for (ui i=0; i<n; i++) {
		tmp[i] = (((float)ran_number() > .5f) ? -1 : 1) * (long double)ran_number();
	}
	return tmp;
}

long double absl(long double nb) {
	return (nb < 0) ? -nb : nb;
}

void arr_shuffle(long double *arr[], long double *paired_arr[], cui Size) {
	for(ui i=0; i<Size; i++) {
		int n = (int)((ui)(ran_number() * 100000) % Size);
		long double *temp = arr[i], *temp_paired = paired_arr[i];
		arr[i] = arr[n];
		arr[n] = temp;
		paired_arr[i] = paired_arr[n];
		paired_arr[n] = temp_paired;
	}
}













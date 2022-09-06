
#include "Tools.h"

void matr_display(ld *arr, cui s, cui cols) {
	printf("\n\t[");
	for(ui i=0; i<s; i++) {
		if(i>0 && i%cols == 0) printf("\n\t ");
		printf("%f", (double)arr[i]);
		if (i<s-1) printf(", ");
	}
	printf("]\n");
}

ld *fvec_alloc(cui n, bool zInit) {
	ld *tmp = (ld*) (zInit) ? calloc(n, sizeof(ld)) : malloc(sizeof(ld) * n);
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

ld *fvec_rInit(cui n, cui conns) {
	ld *tmp = fvec_alloc(n, false);
	for (ui i=0; i<n; i++) {
	    //tmp[i] = ((long)rand()/RAND_MAX)*0.02L-0.01L;
		tmp[i] = (((float)ran_number() > .5f) ? -1 : 1) *
                    (ld)ran_number()/* * sqrtl(1.0L/(ld)conns)*/;
	}
	return tmp;
}

ld absl(cld nb) {
	return (nb < 0) ? -nb : nb;
}

void arr_shuffle(ld *arr[], ld *paired_arr[], cui Size) {
	for(ui i=0; i<Size; i++) {
		int n = (int)((ui)(ran_number() * 100000) % Size);
		ld *temp = arr[i], *temp_paired = paired_arr[i];
		arr[i] = arr[n];
		arr[n] = temp;
		paired_arr[i] = paired_arr[n];
		paired_arr[n] = temp_paired;
	}
}














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
    int seed = (int) time(NULL);
	ld *tmp = r8vec_uniform_01_new(n, &seed);
	ld min = tmp[0], max = tmp[0];
	for (ui i=1; i<n; i++) {
        if (tmp[i] < min) min = tmp[i];
        else if (tmp[i] > max) max = tmp[i];
	}
	ld ospan = max - min;
	ld fspan = 2;
	for (ui i=0; i<n; i++) tmp[i] = fspan*(tmp[i]-min)/ospan - 1;
	return tmp;
}

ld absl(cld nb) {
	return (nb < 0) ? -nb : nb;
}

void arr_shuffle(ld *arr[], ld *paired_arr[], cui Size) {
	for(ui i=0; i<Size; i++) {
		int n = (int)(rand() % Size);
		ld *temp = arr[i], *temp_paired = paired_arr[i];
		arr[i] = arr[n];
		arr[n] = temp;
		paired_arr[i] = paired_arr[n];
		paired_arr[n] = temp_paired;
	}
}






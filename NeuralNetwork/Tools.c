
#include "Tools.h"

void matr_display(float *arr, cui s, cui cols) {
	printf("\n[");
	for(ui i=0; i<s; i++) {
		if(i>0 && i%cols == 0) printf("\n ");
		printf("%f", arr[i]);
		if (i<s-1) printf(", ");
	}
	printf("]\n\n");
}

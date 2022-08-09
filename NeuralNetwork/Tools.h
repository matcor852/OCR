#ifndef TOOLS_H
#define TOOLS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>



typedef unsigned int ui;
typedef const ui cui;

void matr_display(float *arr, cui s, cui cols);
float *fvec_alloc(cui n, bool zInit);
char *cvec_alloc(cui n);
float *fvec_rInit(cui n, float minWeight, float maxWeight);



#endif // TOOLS_H

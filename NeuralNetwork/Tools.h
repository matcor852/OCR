#ifndef TOOLS_H
#define TOOLS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "RandUniform.h"



typedef unsigned int ui;
typedef const ui cui;

void matr_display(long double *arr, cui s, cui cols);
long double *fvec_alloc(cui n, bool zInit);
char *cvec_alloc(cui n);
long double *fvec_rInit(cui n);
long double absl(long double nb);
void arr_shuffle(long double *arr[], long double *paired_arr[], cui Size);

#endif // TOOLS_H

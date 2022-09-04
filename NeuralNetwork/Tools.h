#ifndef TOOLS_H
#define TOOLS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "RandUniform.h"

#define EPS 10E-5L


typedef unsigned int ui;
typedef const ui cui;
typedef long double ld;
typedef const ld cld;

void matr_display(ld *arr, cui s, cui cols);
ld *fvec_alloc(cui n, bool zInit);
char *cvec_alloc(cui n);
ld *fvec_rInit(cui n, cui conns);
ld absl(cld nb);
void arr_shuffle(ld *arr[], ld *paired_arr[], cui Size);

#endif // TOOLS_H

#ifndef RANDUNIFORM_H
#define RANDUNIFORM_H

#define NR 1000
#define NR250 1250
#define NRp1 1001
#define NR250p1 1251
#define MASK 2147483647

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifdef __linux__
#include <unistd.h>
#include <fcntl.h>

int getseed();
#endif



double lcgy(int *seed);
void r250(int *x,double *r,int n);
double ran_number(void/*int *seed*/);

//http://www.acclab.helsinki.fi/~aakurone/tl3/lecturenotes/09_random_numbers-1x2.pdf

#endif // RANDUNIFORM_H

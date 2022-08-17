#ifndef COST_H
#define COST_H

#include <math.h>
#include "Tools.h"


float CrossEntropy(float *predicted, float *expected, cui Size);
float RMSE(float *predicted, float *expected, cui Size);
float MAE(float *predicted, float *expected, cui Size);
float MSE(float *predicted, float *expected, cui Size);

float none_(float *arr);
float sigmoid_(float *arr);
float softmax_(float *arr);
float argmax_(float *arr);
float step_(float *arr);
float relu_(float *arr);
float leakyrelu_(float *arr);




const static struct {
  const char *name;
  float (*func)(float *arr);
} derivative_map [] = {
  { "none", none_ },
  { "sigmoid", sigmoid_ },
  { "softmax", softmax_ },
  { "argmax", argmax_ },
  { "step", step_ },
  { "relu", relu_ },
  { "leakyrelu", leakyrelu_ }
};

#endif // COST_H

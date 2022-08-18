#ifndef COST_H
#define COST_H

#include <math.h>
#include "Tools.h"


float CrossEntropy(float *predicted, float *expected, cui Size);
float RMSE(float *predicted, float *expected, cui Size);
float MAE(float *predicted, float *expected, cui Size);
float MSE(float *predicted, float *expected, cui Size);

float CrossEntropy_(float predicted, float expected);
float RMSE_(float predicted, float expected);
float MAE_(float predicted, float expected);
float MSE_(float predicted, float expected);

float none_(float *arr, cui Size, cui ieme);
float sigmoid_(float *arr, cui Size, cui ieme);
float softmax_(float *arr, cui Size, cui ieme);
float argmax_(float *arr, cui Size, cui ieme);
float step_(float *arr, cui Size, cui ieme);
float relu_(float *arr, cui Size, cui ieme);
float leakyrelu_(float *arr, cui Size, cui ieme);

float (*get_deriv(const char *name))(float *arr, cui Size, cui ieme);
float (*get_cost(const char *name))(float *predicted,float *expected,cui Size);
float (*get_cost_deriv(const char *name))(float predicted, float expected);

const static struct {
  const char *name;
  float (*func)(float *arr, cui Size, cui ieme);
} deriv_map [] = {
  { "none", none_ },
  { "sigmoid", sigmoid_ },
  { "softmax", softmax_ },
  { "argmax", argmax_ },
  { "step", step_ },
  { "relu", relu_ },
  { "leakyrelu", leakyrelu_ }
};

const static struct {
  const char *name;
  float (*func)(float *predicted, float *expected, cui Size);
} cost_map [] = {
  { "CrossEntropy", CrossEntropy },
  { "RMSE", RMSE },
  { "MAE", MAE },
  { "MSE", MSE }
};

const static struct {
  const char *name;
  float (*func)(float predicted, float expected);
} cost_deriv_map [] = {
  { "CrossEntropy", CrossEntropy_ },
  { "RMSE", RMSE_ },
  { "MAE", MAE_ },
  { "MSE", MSE_ }
};

#endif // COST_H

#ifndef COST_H
#define COST_H

#include <math.h>
#include <float.h>
#include "Tools.h"


long double CrossEntropy(long double *predicted, long double *expected, cui Size);
long double RMSE(long double *predicted, long double *expected, cui Size);
long double MAE(long double *predicted, long double *expected, cui Size);
long double MSE(long double *predicted, long double *expected, cui Size);

long double CrossEntropy_(long double predicted, long double expected);
long double RMSE_(long double predicted, long double expected);
long double MAE_(long double predicted, long double expected);
long double MSE_(long double predicted, long double expected);

long double none_(long double *arr, cui Size, cui ieme);
long double sigmoid_(long double *arr, cui Size, cui ieme);
long double softmax_(long double *arr, cui Size, cui ieme);
long double argmax_(long double *arr, cui Size, cui ieme);
long double step_(long double *arr, cui Size, cui ieme);
long double relu_(long double *arr, cui Size, cui ieme);
long double leakyrelu_(long double *arr, cui Size, cui ieme);

long double (*get_deriv(const char *name))(long double *arr, cui Size, cui ieme);
long double (*get_cost(const char *name))(long double *predicted,long double *expected,cui Size);
long double (*get_cost_deriv(const char *name))(long double predicted, long double expected);

const static struct {
  const char *name;
  long double (*func)(long double *arr, cui Size, cui ieme);
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
  long double (*func)(long double *predicted, long double *expected, cui Size);
} cost_map [] = {
  { "CrossEntropy", CrossEntropy },
  { "RMSE", RMSE },
  { "MAE", MAE },
  { "MSE", MSE }
};

const static struct {
  const char *name;
  long double (*func)(long double predicted, long double expected);
} cost_deriv_map [] = {
  { "CrossEntropy", CrossEntropy_ },
  { "RMSE", RMSE_ },
  { "MAE", MAE_ },
  { "MSE", MSE_ }
};

#endif // COST_H

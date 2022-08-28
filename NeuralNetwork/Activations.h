#ifndef ACTIVATIONS_H
#define ACTIVATIONS_H

#include <math.h>
#include <float.h>
#include "Tools.h"

void sigmoid(long double *input, long double *output, cui Size);
void softmax(long double *input, long double *output, cui Size);
void argmax(long double *input, long double *output, cui Size);
void step(long double *input, long double *output, cui Size);
void relu(long double *input, long double *output, cui Size);
void leakyrelu(long double *input, long double *output, cui Size);
void none(long double *input, long double *output, cui Size);

void (*get_activation(const char *name))(long double *input, long double *output, cui Size);

const static struct {
  const char *name;
  void (*func)(long double *input, long double *output, cui Size);
} function_map [] = {
  { "none", none },
  { "sigmoid", sigmoid },
  { "softmax", softmax },
  { "argmax", argmax },
  { "step", step },
  { "relu", relu },
  { "leakyrelu", leakyrelu }
};

#endif // ACTIVATIONS_H

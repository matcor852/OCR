#ifndef ACTIVATIONS_H
#define ACTIVATIONS_H

#include <math.h>
#include "Tools.h"

void sigmoid(float *input, float *output, cui Size);
void softmax(float *input, float *output, cui Size);
void argmax(float *input, float *output, cui Size);
void step(float *input, float *output, cui Size);
void relu(float *input, float *output, cui Size);
void leakyrelu(float *input, float *output, cui Size);
void none(float *input, float *output, cui Size);

void (*get_activation(const char *name))(float *input, float *output, cui Size);

const static struct {
  const char *name;
  void (*func)(float *input, float *output, cui Size);
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

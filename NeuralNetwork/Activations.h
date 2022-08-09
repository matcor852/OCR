#ifndef ACTIVATIONS_H
#define ACTIVATIONS_H

#include <math.h>
#include <Layer.h>


float fsigmoid(float x);
float frelu(float x);
float fleakyRelu(float x);
float fnone(float x);

void simpleMatchInOut(Layer *layer, float (*activation)(float x));


void sigmoid(Layer *layer);
void sigmoid_(Layer *layer);

void softmax(Layer *layer);
void softmax_(Layer *layer);

void argmax(Layer *layer);
void argmax_(Layer *layer);

void step(Layer *layer);
void step_(Layer *layer);

void relu(Layer *layer);
void relu_(Layer *layer);

void leakyrelu(Layer *layer);
void leakyrelu_(Layer *layer);

void none(Layer *layer);

void (*get_activation(const char *name))(Layer *layer);

const static struct {
  const char *name;
  void (*func)(Layer *layer);
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

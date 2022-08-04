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

void leakyRelu(Layer *layer);
void leakyRelu_(Layer *layer);

void None(Layer *layer);



#endif // ACTIVATIONS_H

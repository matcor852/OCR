#ifndef LAYER_H
#define LAYER_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef unsigned int ui;
typedef const ui cui;
typedef struct Layer Layer;

struct Layer
{
	ui Neurons;
	float *weights;
	float *bias;
	float *output;
	Layer *pLayer;
	Layer *nLayer;
	void (*activation)(Layer *layer);
};

void Layer_Init(Layer *layer, Layer *pLayer, Layer *nLayer, cui neurons);
void Layer_Dispose(Layer *layer);

float *fvec_alloc(cui n);


#endif // LAYER_H

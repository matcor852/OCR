#ifndef LAYER_H
#define LAYER_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#include "Tools.h"

#define MIN_WEIGHT -1
#define MAX_WEIGHT 1


typedef struct Layer Layer;

struct Layer
{
	ui Neurons;
	ui conns;
	float *weights;
	float *bias;
	float *input;
	float *output;
	Layer *pLayer;
	Layer *nLayer;
	void (*activation)(Layer *layer);
};

void Layer_Init(Layer *layer, Layer *pLayer, Layer *nLayer,
				cui neurons, float *weights, float *bias);
void Layer_Dispose(Layer *layer);
float *fvec_alloc(cui n, bool zInit);
float *fvec_rInit(cui n);

void Layer_SetInput(Layer *layer, float *input, cui inputSize);
void Layer_Activate(Layer *layer);



#endif // LAYER_H

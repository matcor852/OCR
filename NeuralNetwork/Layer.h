#ifndef LAYER_H
#define LAYER_H

#include <time.h>
#include "Activations.h"

#define MIN_WEIGHT -1
#define MAX_WEIGHT 1


typedef struct Layer Layer;
typedef struct LayerSave LayerSave;

struct Layer
{
	ui Neurons, conns;
	float *bias, *weights, *input, *output;
	Layer *pLayer, *nLayer;
	void (*activation)(float *input, float *output, cui Size);
	char *act_name;
	bool loaded;
};

struct LayerSave
{
	ui Neurons, conns, fSize;
};


void Layer_Init(Layer *layer, Layer *pLayer, Layer *nLayer, cui neurons,
				float *weights, float *bias, bool loaded, char *act_name);
void Layer_Dispose(Layer *layer);

void Layer_SetInput(Layer *layer, float *input, cui inputSize);
void Layer_Activate(Layer *layer);

void Layer_Display(Layer *layer, const ui ieme, bool display_matr);



#endif // LAYER_H

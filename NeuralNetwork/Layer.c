#include "Layer.h"

void Layer_Init(Layer *layer, Layer *pLayer, Layer *nLayer, cui neurons)
{
	layer->Neurons = neurons;
	if(pLayer != NULL) {
		layer->weights = fvec_alloc(pLayer->Neurons * neurons);
		layer->bias = fvec_alloc(neurons);
	}
	layer->output = fvec_alloc(neurons);
	layer->pLayer = pLayer;
	layer->nLayer = nLayer;

}

void Layer_Dispose(Layer *layer) {

	free(layer->output);
	if (layer->pLayer != NULL) {
		free(layer->weights);
		free(layer->bias);
	}
}

float *fvec_alloc(cui n)
{
	float *tmp = (float*) malloc(sizeof(float) * n);
	if (tmp == NULL) {
		printf("Error: Out of memory ...\n");
		exit(1);
	}
	return tmp;
}

#include "Layer.h"

void Layer_Init(Layer *layer, Layer *pLayer, Layer *nLayer, cui neurons,
				float *weights, float bias, bool loaded) {
	if(pLayer != NULL) {
		layer->weights = (weights == NULL) ?
			fvec_rInit(pLayer->Neurons*neurons,MIN_WEIGHT,MAX_WEIGHT) : weights;
		layer->conns = pLayer->Neurons * neurons;
		layer->input = fvec_alloc(neurons, false);
		layer->output = fvec_alloc(neurons, false);
		layer->bias = bias;
	}
	else {
		layer->conns = 0;
		layer->bias = 0;
	}
	layer->pLayer = pLayer;
	layer->nLayer = nLayer;
	layer->Neurons = neurons;
	layer->loaded = loaded;
}

void Layer_Dispose(Layer *layer) {
	if (layer->pLayer != NULL) {
		free(layer->input);
		free(layer->output);
		free(layer->weights);
		if(layer->loaded) free(layer->act_name);
	}
}

void Layer_SetInput(Layer *layer, float *input, cui inputSize) {
	if (inputSize != layer->Neurons) {
		printf("Error: Input data size has different size than neurons");
		exit(2);
	}
	layer->output = input;
}

void Layer_Activate(Layer *layer) {
	for(ui i=0; i<layer->Neurons; i++)
		layer->input[i] = layer->bias;
	ui w = 0;
	for(ui i=0; i<layer->pLayer->Neurons; i++) {
		for(ui j=0; j<layer->Neurons; j++) {
			layer->input[j] += layer->pLayer->output[i]*layer->weights[w];
			w += 1;
		}
	}
	layer->activation(layer);
}


#include "Layer.h"

void Layer_Init(Layer *layer, Layer *pLayer, Layer *nLayer,
				cui neurons, float *weights, float *bias) {
	layer->Neurons = neurons;
	if(pLayer != NULL) {
		layer->weights = (weights == NULL) ?
			fvec_rInit(pLayer->Neurons * neurons) : weights;
		layer->conns = pLayer->Neurons * neurons;
		layer->bias = (bias == NULL) ?
			fvec_alloc(neurons, true) : bias;
		layer->input = fvec_alloc(neurons, false);
	}
	layer->output = fvec_alloc(neurons, false);
	layer->pLayer = pLayer;
	layer->nLayer = nLayer;
}

void Layer_Dispose(Layer *layer) {
	if (layer->pLayer != NULL) {
		free(layer->input);
		free(layer->output);
		free(layer->weights);
		free(layer->bias);
	}
}

float *fvec_alloc(cui n, bool zInit) {
	float *tmp = (float*) (zInit) ?
		calloc(n, sizeof(float)) : malloc(sizeof(float) * n);
	if (tmp == NULL) {
		printf("Error: Out of memory ...\n");
		exit(1);
	}
	return tmp;
}

float *fvec_rInit(cui n) {
	float *tmp = fvec_alloc(n, false);
	for (ui i=0; i<n; i++) {
		tmp[i] = (float)((double)(rand())/(double)(RAND_MAX)
						* (MAX_WEIGHT - MIN_WEIGHT) + MIN_WEIGHT);
	}
	return tmp;
}

void Layer_SetInput(Layer *layer, float *input, cui inputSize) {
	if (inputSize != layer->Neurons) {
		printf("Error: Input data size has different size than neurons number");
		exit(2);
	}
	layer->output = input;
}

void Layer_Activate(Layer *layer) {

	for(ui i=0; i<layer->Neurons; i++)
		layer->input[i] = layer->bias[i];

	ui w = 0;
	for(ui i=0; i<layer->pLayer->Neurons; i++) {
		for(ui j=0; j<layer->Neurons; j++) {
			layer->input[j] += layer->pLayer->output[i]*layer->weights[w];
			w += 1;
		}
	}

	layer->activation(layer);

}














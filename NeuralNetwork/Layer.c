#include "Layer.h"

void Layer_Init(Layer *layer, Layer *pLayer, Layer *nLayer, cui neurons,
				float *weights, float bias, bool loaded, char *act_name) {
	layer->conns = 0;
	layer->bias = 0;
	if(pLayer != NULL) {
		layer->weights = (weights == NULL) ?
			fvec_rInit(pLayer->Neurons*neurons,MIN_WEIGHT,MAX_WEIGHT) : weights;
		layer->conns = pLayer->Neurons * neurons;
		layer->input = fvec_alloc(neurons, false);
		layer->output = fvec_alloc(neurons, false);
		layer->bias = bias;
	}
	layer->pLayer = pLayer;
	layer->nLayer = nLayer;
	layer->Neurons = neurons;
	layer->loaded = loaded;
	layer->act_name = act_name;
	layer->activation = get_activation(act_name);
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
	for(ui i=0; i<layer->Neurons; i++) layer->input[i] = layer->bias;
	ui w = 0;
	for(ui i=0; i<layer->pLayer->Neurons; i++) {
		for(ui j=0; j<layer->Neurons; j++) {
			layer->input[j] += layer->pLayer->output[i]*layer->weights[w];
			w += 1;
		}
	}
	layer->activation(layer->input, layer->output, layer->Neurons);
}

void Layer_Display(Layer *layer, const ui ieme) {
	printf("Layer %u :\n", ieme);
	printf("\t%u neurons", layer->Neurons);
	if (layer->pLayer == NULL) {
		printf("\n\t--[ Input Layer ]--\n");
		printf("\tnext layer : %s\n\n", layer->nLayer->act_name);
		return;
	}

	printf(", %u connections with previous layer\n", layer->conns);
	if (layer->nLayer == NULL) printf("\t--[ Output Layer ]--\n");
	printf("\tactivation function : %s\n", layer->act_name);
	printf("\tprevious layer : %s\n", layer->pLayer->act_name);
	if (layer->nLayer != NULL)
		printf("\tnext layer : %s\n", layer->nLayer->act_name);
	printf("\tbias : %f\n", (double)layer->bias);
	printf("\tweights :");
	matr_display(layer->weights, layer->conns, layer->Neurons);
}




#include "Layer.h"

void Layer_Init(Layer *layer, Layer *pLayer, Layer *nLayer, cui neurons,
				ld *weights, ld *bias, bool loaded, char *act_name) {
	layer->conns = 0;
	layer->bias = 0;
	if (pLayer != NULL) {
		layer->conns = pLayer->Neurons * neurons;
		layer->weights = (weights == NULL)
							 ? fvec_rInit(layer->conns, pLayer->Neurons,
										  !strcmp(act_name, "relu")
											  || !strcmp(act_name, "leakyrelu"))
							 : weights;
		layer->bias = (bias == NULL) ? fvec_alloc(neurons, true) : bias;
		layer->input = fvec_alloc(neurons, false);
		layer->output = fvec_alloc(neurons, false);
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
		free(layer->bias);
		if (layer->loaded) free(layer->act_name);
	}
}

void Layer_Activate(Layer *layer) {
	for (ld *pI = layer->input, *pB = layer->bias;
		 pI < layer->input + layer->Neurons; pI++, pB++)
		*pI = *pB;
	for (ld *lO = layer->pLayer->output, *lW = layer->weights;
		 lO < layer->pLayer->output + layer->pLayer->Neurons; lO++) {
		for (ld *lI = layer->input; lI < layer->input + layer->Neurons;
			 lI++, lW++)
			*lI += (*lO) * (*lW);
	}
	layer->activation(layer->input, layer->output, layer->Neurons);
}

Layer* Layer_DeepCopy(Layer *layer) {
    Layer *copy = malloc(sizeof(Layer));
    copy->Neurons = layer->Neurons;
    copy->conns = layer->conns;
    copy->loaded = false;
    copy->pLayer = layer->pLayer;
    copy->nLayer = layer->nLayer;
    copy->act_name = layer->act_name;
    copy->activation = layer->activation;
    copy->bias = malloc(sizeof(ld) * layer->Neurons);
    copy->weights = malloc(sizeof(ld) * layer->conns);
    if (layer->pLayer != NULL) {
        copy->input = fvec_alloc(layer->Neurons, false);
		copy->output = fvec_alloc(layer->Neurons, false);
    }
    for(ui i=0; i<layer->Neurons; i++) copy->bias[i] = layer->bias[i];
    for(ui i=0; i<layer->conns; i++) copy->weights[i] = layer->weights[i];
    return copy;
}

void Layer_Display(Layer *layer, const ui ieme, bool display_matr) {
	printf("Layer %u (%p) :\n", ieme, (void *)layer);
	printf("\t%u neurons", layer->Neurons);
	if (layer->pLayer == NULL) {
		printf("\n\t--[ Input Layer ]--\n");
		printf("\tnext layer : %s\n\n\n", layer->nLayer->act_name);
		return;
	}

	printf(", %u connections with previous layer\n", layer->conns);
	if (layer->nLayer == NULL) printf("\t--[ Output Layer ]--\n");
	printf("\tactivation function : %s\n", layer->act_name);
	printf("\tprevious layer : %s\n", layer->pLayer->act_name);
	if (layer->nLayer != NULL)
		printf("\tnext layer : %s\n", layer->nLayer->act_name);
	if (!display_matr) {
		printf("\n\n");
		return;
	}
	printf("\tbias :");
	matr_display(layer->bias, layer->Neurons, 1);
	printf("\tweights :");
	matr_display(layer->weights, layer->conns, 1);
	printf("\n\n");
}

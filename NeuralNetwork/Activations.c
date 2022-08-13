#include "Activations.h"

float fsigmoid(float x) {return 1/(1+expf(-x));}
float frelu(float x) {return max(.0f, x);}
float fleakyRelu(float x) {return max(0.1f * x, x);}
float fnone(float x) {return x;}

void simpleMatchInOut(Layer *layer, float (*activation)(float x)) {
	for (ui i=0; i<layer->Neurons; i++) {
		layer->output[i] = activation(layer->input[i]);
	}
}

void relu(Layer *layer) {
	simpleMatchInOut(layer, &frelu);
}
void leakyrelu(Layer *layer) {
	simpleMatchInOut(layer, &fleakyRelu);
}
void none(Layer *layer) {
	simpleMatchInOut(layer, &fnone);
}
void sigmoid(Layer *layer) {
	simpleMatchInOut(layer, &fsigmoid);
}

void softmax(Layer *layer) {
	float *expd = fvec_alloc(layer->Neurons, false);
	float s = 0;
	for (ui i=0; i<layer->Neurons; i++) {
		expd[i] = expf(layer->input[i]);
		s += expd[i];
	}
	for (ui i=0; i<layer->Neurons; i++) {
		layer->output[i] = expd[i]/s;
	}
	free(expd);
}

void argmax(Layer *layer) {

}

void step(Layer *layer) {

}

void (*get_activation(const char *name))(Layer *layer) {
	for (ui i = 0; i < (sizeof(function_map) / sizeof(function_map[0])); i++) {
		if (!strcmp(function_map[i].name, name)) {
			return function_map[i].func;
		}
	}
	return NULL;
}


#include "Activations.h"

void relu(float *input, float *output, cui Size) {
	for (ui i=0; i<Size; i++)
		output[i] = max(.0f, input[i]);
}

void leakyrelu(float *input, float *output, cui Size) {
	for (ui i=0; i<Size; i++) output[i] = max(.1f * input[i], input[i]);
}

void none(float *input, float *output, cui Size) {
	for (ui i=0; i<Size; i++) output[i] = input[i];
}

void sigmoid(float *input, float *output, cui Size) {
	for (ui i=0; i<Size; i++) output[i] = 1/(1+expf(-input[i]));
}

void softmax(float *input, float *output, cui Size) {
	float s = 0, *expd = fvec_alloc(Size, false);
	for (ui i=0; i<Size; i++) {
		expd[i] = expf(input[i]);
		s += expd[i];
	}
	for (ui i=0; i<Size; i++) output[i] = expd[i]/s;
	free(expd);
}

void argmax(float *input, float *output, cui Size) {
	ui p = 0;
	for (ui i=0; i<Size; i++) p = (input[i] > input[p]) ? i : p;
	for (ui i=0; i<Size; i++) output[i] = (i != p) ? .0f : 1.0f;
}

void step(float *input, float *output, cui Size) {
	for (ui i=0; i<Size; i++) output[i] = (input[i] < 0) ? .0f : 1.0f;
}


void (*get_activation(const char *name))(float *input, float *output, cui Size)
{
	for (ui i = 0; i < (sizeof(function_map) / sizeof(function_map[0])); i++) {
		if (!strcmp(function_map[i].name, name)) {
			return function_map[i].func;
		}
	}
	return NULL;
}



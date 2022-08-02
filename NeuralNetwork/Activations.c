#include "Activations.h"


float sigmoid(float x) {
	return 1 / (1 + expf(-x));
}

float sigmoid_(float x) {
	float sigm = sigmoid(x);
	return sigm*(1-sigm);
}

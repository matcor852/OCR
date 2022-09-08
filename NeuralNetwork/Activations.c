#include "Activations.h"

void relu(ld *input, ld *output, cui Size) {
	for (ui i=0; i<Size; i++) output[i] = max(.0, input[i]);
}

void leakyrelu(ld *input, ld *output, cui Size) {
	for (ui i=0; i<Size; i++) output[i] = max(.01 * input[i], input[i]);
}

void none(ld *input, ld *output, cui Size) {
	for (ui i=0; i<Size; i++) output[i] = input[i];
}

void sigmoid(ld *input, ld *output, cui Size) {
	for (ui i=0; i<Size; i++) output[i] = 1/(1 + expl(-input[i]));
}

void softmax(ld *input, ld *output, cui Size) {
    ld max = input[0];
	for (ui i=0; i<Size; i++) if (input[i] > max) max = input[i];
	for (ui i=0; i<Size; i++) input[i] -= max;
	ld s = .0, *expd = fvec_alloc(Size, false);
	for (ui i=0; i<Size; i++) {
        if (isnan(expl(input[i]))) {
            printf("\nnan in softmax caused by exp(%LF)\n", input[i]);
            exit(3);
        }
		expd[i] = expl(input[i]);
		s += expd[i];
	}
	for (ui i=0; i<Size; i++) {
        if (isnan(expd[i]/(s+EPS))) {
            puts("nan 2 in softmax");
            exit(4);
        }
        output[i] = expd[i]/(s+10E-8);
	}
	free(expd);
}

void argmax(ld *input, ld *output, cui Size) {
	ui c = 0, p[Size];
	p[c] = 0;
	for (ui i=0; i<Size; i++) {
        if (input[i] > input[p[c]]) {
            c = 0;
            p[c] = i;
        }
        else if (absl(input[i] - input[p[c]]) < LDBL_EPSILON) {
            c++;
            p[c] = i;
        }
	}
	for (ui i=0; i<Size; i++) output[i] = .0L;
	for (ui i=0; i<c+1; i++) output[p[i]] = 1.0L;
}

void step(ld *input, ld *output, cui Size) {
	for (ui i=0; i<Size; i++) output[i] = (input[i] < .5L) ? .0L : 1.0L;
}

void (*get_activation(const char *name))(ld *input, ld *output, cui Size)
{
	for (ui i = 0; i < (sizeof(function_map) / sizeof(function_map[0])); i++)
		if (!strcmp(function_map[i].name, name)) return function_map[i].func;
	return NULL;
}



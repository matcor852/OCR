#include "Network.h"
#include <stdio.h>

void Network_Init(Network *net, cui nbLayers) {
	net->layers = lvec_alloc(nbLayers);
	net->nbLayers = nbLayers;
	net->currentLayer = 0;
}

void Network_AddLayer(Network *net, Layer *layer) {
	if (net->currentLayer >= net->nbLayers) {
		printf("Attempting Layer overflow; Starting network purge...\n");
		Network_Purge(net);
		return;
	}
	net->layers[net->currentLayer] = *layer;
	net->currentLayer++;
}

void Network_Load(Network *net, char path[]) {
	if (sscanf_s(path, "%*[^_]%*[_]%u", &net->nbLayers) != 1) {
		printf("Could not read amount of layer in filename; Exiting...\n");
		exit(1);
	}
	Network_Init(net, net->nbLayers);
	FILE *fptr;
	int err;
	char errbuf[64];
	if ((err = fopen_s(&fptr, path, "rb")) != 0){
		strerror_s(errbuf,sizeof(errbuf), err);
		fprintf(stderr, "Cannot open file '%s': %s\n", path, errbuf);
		exit(1);
	}
	Layer layer, *lSave = NULL;
	LayerSave saved;
	for(ui i=0; i<net->nbLayers; i++) {
		fread(&saved, sizeof(LayerSave), 1, fptr);
		if (saved.conns > 0) {
			char *name = cvec_alloc(saved.fSize+1);
			fread(name, sizeof(char), saved.fSize, fptr);
			name[saved.fSize] = '\0';
			float *bias = fvec_alloc(saved.Neurons, false);
			fread(bias, sizeof(float), saved.Neurons, fptr);
			float *weights = fvec_alloc(saved.conns, false);
			fread(weights, sizeof(float), saved.conns, fptr);
			Layer_Init(&layer, lSave, NULL, saved.Neurons, weights,
						bias, true, name);
		}
		else {
			Layer_Init(&layer, NULL, NULL, saved.Neurons, NULL,
						NULL, false, "none");
		}
		Network_AddLayer(net, &layer);
		lSave = &layer;
	}
	fclose(fptr);
	if (net->currentLayer != net->nbLayers) {
		printf("Corrupted amount of layer [%u/%u]; Starting purge...",
				net->currentLayer+1, net->nbLayers);
		Network_Purge(net);
		exit(1);
	}

	for (ui i=1; i<net->nbLayers; i++) {
		net->layers[i-1].nLayer = &net->layers[i];
		net->layers[i].pLayer = &net->layers[i-1];
		if (i+1 < net->nbLayers) net->layers[i].nLayer = &net->layers[i+1];
	}

}

void Network_Save(Network *net) {
	FILE *fptr;
	int err;
	char errbuf[64], filename[64], NNName[16];
	printf("Neural Network Name : ");
	scanf_s("%s", NNName, sizeof(NNName));
	snprintf(filename, 64, "NeuralNetData_%ulayers_%s.bin",
			net->currentLayer, NNName);
	if ((err = fopen_s(&fptr, filename, "wb")) != 0){
		strerror_s(errbuf,sizeof(errbuf), err);
		fprintf(stderr, "Cannot open file '%s': %s\n", filename, errbuf);
       exit(1);
	}
	for(ui i=0; i<net->nbLayers; i++) {
		ui len = (ui)strlen(net->layers[i].act_name);
		LayerSave saved = {	net->layers[i].Neurons,
							net->layers[i].conns,
							len};
		fwrite(&saved, sizeof(saved), 1, fptr);
		if (net->layers[i].pLayer != NULL) {
			fwrite(net->layers[i].act_name,sizeof(char), len, fptr);
			fwrite(&net->layers[i].bias[0], sizeof(float),
					net->layers[i].Neurons, fptr);
			fwrite(&net->layers[i].weights[0], sizeof(float),
					net->layers[i].conns, fptr);
		}
	}
	fclose(fptr);
}

void Network_Purge(Network *net) {
	for (ui i=0; i<net->nbLayers && net->currentLayer>0; i++) {
		Layer_Dispose(&net->layers[i]);
	}
	free(net->layers);
}

void Network_Display(Network *net, bool display_matr) {
	for(ui i=0; i<net->nbLayers; i++) {
		Layer_Display(&net->layers[i], i, display_matr);
	}
}

Layer *lvec_alloc(cui n) {
	Layer *tmp = (Layer*) malloc(sizeof(Layer) * n);
	if (tmp == NULL) {
		printf("Error: Out of memory ...\n");
		exit(1);
	}
	return tmp;
}

void Network_Train(Network *net, float *input[], float *expected_output[],
				cui iSize, cui oSize, cui Size, cui epoch, float l_rate,
				char cost_func[]) {
	if (net->nbLayers < 2) {
		printf("Attempting train on incomplete network; Starting purge...\n");
		Network_Purge(net);
		exit(1);
	}
	if (iSize != net->layers[0].Neurons ||
		oSize != net->layers[net->nbLayers-1].Neurons) {
		printf("Differing I/0 sizes; Starting purge...\n");
		Network_Purge(net);
		exit(1);
	}

	for (ui e=0; e<epoch; e++) {
		for (ui s=0; s<Size; s++) {
			Network_Forward(net, input[s], iSize);
			float error = Network_BackProp(net, expected_output[s], oSize,
											l_rate, cost_func);
			if (e==0 || e==epoch-1) printf("error : %f\n", (double)error);
		}
		if (e==0 || e==epoch-1) printf("\nFinished epoch %u\n", e+1);
	}
}

void Network_Forward(Network *net, float *input, cui iSize) {
	Layer_SetInput(&net->layers[0], input, iSize);
	for (ui i=1; i<net->currentLayer; i++) Layer_Activate(&net->layers[i]);
}

float Network_BackProp(Network *net, float *expected, cui oSize, float l_rate,
					char cost_func[])
{
	Layer *L = &net->layers[net->nbLayers-1];
	float (*cost_deriv)(float, float) = get_cost_deriv(cost_func);
	float (*deriv)(float*,cui,cui) = get_deriv(L->act_name);

	float error = get_cost(cost_func)(L->output, expected, oSize);

	float *CostOut = fvec_alloc(L->Neurons, false);
	float *OutIn = fvec_alloc(L->Neurons, false);
	for (ui i=0; i<L->Neurons; i++) {
		CostOut[i] = cost_deriv(L->output[i], expected[i]);
		OutIn[i] = deriv(L->input, L->Neurons, i);
	}

	float *Legacy = fvec_alloc(L->pLayer->Neurons, true);
	ui w = 0;
	bool bias_done = false;
	for (ui i=0; i<L->pLayer->Neurons; i++) {
		for (ui j=0; j<L->Neurons; j++) {
			float ml = CostOut[j] * OutIn[j];
			Legacy[i] += ml * L->weights[w];
			L->weights[w] = L->weights[w] - l_rate * ml * L->pLayer->output[i];
			w += 1;
			if (!bias_done) L->bias[j] = L->bias[j] - l_rate * ml;
		}
		bias_done = true;
	}
	free(CostOut);
	free(OutIn);


	float *tempLegacy;
	for (ui X=net->nbLayers-2; X>0; X--) {
		L = &net->layers[X];
		float (*deriv_i)(float*,cui,cui) = get_deriv(L->act_name);
		tempLegacy = fvec_alloc(L->pLayer->Neurons, true);
		float *OutIn_i = fvec_alloc(L->Neurons, false);
		for (ui i=0; i<L->Neurons; i++)
			OutIn_i[i] = deriv_i(L->input, L->Neurons, i);
		ui w_i = 0;
		bool bias_done_i = false;
		for (ui i=0; i<L->pLayer->Neurons; i++) {
			for (ui j=0; j<L->Neurons; j++) {
				float ml = Legacy[j] * OutIn_i[j];
				tempLegacy[i] += ml * L->weights[w_i];
				L->weights[w_i] = L->weights[w_i] - l_rate * ml *
											L->pLayer->output[i];
				w += 1;
				if (!bias_done_i) L->bias[j] = L->bias[j] - l_rate * ml;
			}
			bias_done_i = true;
		}
		free(Legacy);
		Legacy = tempLegacy;
		free(OutIn_i);
	}
	free(Legacy);
	return error;
}



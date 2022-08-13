#include "Network.h"
#include <stdio.h>

void Network_Init(Network *net, cui nbLayers, cui mode, float l_rate) {
	net->layers = lvec_alloc(nbLayers);
	net->nbLayers = nbLayers;
	net->currentLayer = 0;
	net->mode = mode;
	net->l_rate = l_rate;
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

void Network_Load(Network *net, char path[], cui mode, float l_rate) {
	if (sscanf_s(path, "%*[^_]%*[_]%u", &net->nbLayers) != 1) {
		printf("Could not read amount of layer in filename; Exiting...\n");
		exit(1);
	}
	Network_Init(net, net->nbLayers, mode, l_rate);
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
			layer.act_name = name;
			layer.activation = get_activation(name);
			float *weights = fvec_alloc(saved.conns, false);
			fread(weights, sizeof(float), saved.conns, fptr);
			Layer_Init(&layer, lSave, NULL, saved.Neurons, weights,
						saved.bias, true);
		}
		else {
			Layer_Init(&layer,NULL,NULL,saved.Neurons,NULL,saved.bias, false);
			layer.act_name = "none";
			layer.activation = &none;
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
							len, net->layers[i].bias };
		fwrite(&saved, sizeof(saved), 1, fptr);
		if (net->layers[i].pLayer != NULL) {
			fwrite(net->layers[i].act_name,sizeof(char), len, fptr);
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

void Network_Display(Network *net) {
	for(ui i=0; i<net->nbLayers; i++) {
		Layer_Display(&net->layers[i], i);
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
				ui iSize, ui oSize, ui Size, ui epoch) {
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
			Layer_SetInput(&net->layers[0], input[s], iSize);

			for (ui i=1; i<net->currentLayer; i++) {
				Layer_Activate(&net->layers[i]);
			}

			for (ui i=0; i<oSize; i++) {
				printf("predicted : %f\t\texpected : %f\n",
						net->layers[net->nbLayers-1].output[i],
						expected_output[s][0]);
			}
			printf("\n");
		}
		printf("Finished epoch %u\n", e+1);
	}



}






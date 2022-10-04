#include "Network.h"

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

void Network_Wire(Network *net) {
    for (ui i=1; i<net->nbLayers; i++) {
		net->layers[i-1].nLayer = &net->layers[i];
		net->layers[i].pLayer = &net->layers[i-1];
		if (i+1 < net->nbLayers) net->layers[i].nLayer = &net->layers[i+1];
	}
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
			ld *bias = fvec_alloc(saved.Neurons, false);
			fread(bias, sizeof(ld), saved.Neurons, fptr);
			ld *weights = fvec_alloc(saved.conns, false);
			fread(weights, sizeof(ld), saved.conns, fptr);
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
    Network_Wire(net);
}

void Network_Save(Network *net, char name[]) {
	FILE *fptr;
	int err;
	char errbuf[64], filename[64], NNName[16];

	if (name == NULL) {
        printf("\nNeural Network Name : ");
        scanf_s("%s", NNName, sizeof(NNName));
	}
	snprintf(filename, 64, "NeuralNetData_%ulayers_%s.bin",
			net->currentLayer, name == NULL ? NNName : name);
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
			fwrite(&net->layers[i].bias[0], sizeof(ld),
					net->layers[i].Neurons, fptr);
			fwrite(&net->layers[i].weights[0], sizeof(ld),
					net->layers[i].conns, fptr);
		}
	}
	fclose(fptr);
}

void Network_Purge(Network *net) {
    for(Layer *l=net->layers;l<net->layers+net->nbLayers;l++) Layer_Dispose(l);
	free(net->layers);
	free(net);
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

void Network_Predict(Network *net, ld *input, cui Size) {
	Network_Forward(net, input, Size);
}

ld *Network_Validate(Network *net, ld *input, cui Size, bool os1) {
    Network_Forward(net, input, Size);
    (os1 ? step : argmax)(net->layers[net->nbLayers-1].output,
           net->layers[net->nbLayers-1].output,
           net->layers[net->nbLayers-1].Neurons);
    return net->layers[net->nbLayers-1].output;
}

void Network_Train(Network *net, NNParam *params)
{
    if (net->nbLayers < 2) {
		printf("Attempting train on incomplete network; Starting purge...\n");
		Network_Purge(net);
		exit(1);
	}
	if (params->iSize != net->layers[0].Neurons ||
		params->oSize != net->layers[net->nbLayers-1].Neurons) {
		printf("Differing I/0 sizes; Starting purge...\n");
		Network_Purge(net);
		exit(1);
	}

	FILE *f = NULL;
	int c = 0;
	if(params->track) {
        c = 1;
        f = fopen("stats.txt", "w");
        if (f == NULL) params->track = false;
	}

	for (ui e=0; e<params->epoch; e++) {
		arr_shuffle(params->inputTrain, params->outputTrain, params->toLoopTrain);
		for (ui s=0; s<params->toLoopTrain; s++) {
			Network_Forward(net, params->inputTrain[s], params->iSize);
			ld error = Network_BackProp(net, params, s);
			if (params->track) {
                fprintf(f, "%u %f\n", c, (double)error);
                c++;
			}
		}
	}

	if (params->track) fclose(f);

}

/*
void Network_Train(Network *net, ld *input[], ld *expected_output[], cui iSize,
                   cui oSize, cui Size, cui epoch, char cost_func[], ld l_rate,
                   bool adam)
{
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


    //ADAM
    ld *Mwt[net->nbLayers-1], *Vwt[net->nbLayers-1],
                *Mbt[net->nbLayers-1], *Vbt[net->nbLayers-1];
    if (adam) {
        for (ui i=0; i<net->nbLayers-1; i++) {
            Mwt[i] = fvec_alloc(net->layers[i+1].conns, true);
            Vwt[i] = fvec_alloc(net->layers[i+1].conns, true);
            Mbt[i] = fvec_alloc(net->layers[i+1].Neurons, true);
            Vbt[i] = fvec_alloc(net->layers[i+1].Neurons, true);
        }
    }


	bool track = true;
	int c = 1;
	FILE *f = fopen("stats.txt", "w");
	if (f == NULL) track = false;


	clock_t begin, end;
	for (ui e=0; e<epoch; e++) {
		arr_shuffle(input, expected_output, Size);
		for (ui s=0; s<Size; s++) {
            begin = clock();
			Network_Forward(net, input[s], iSize);
			ld error = Network_BackProp(net, expected_output[s], oSize, cost_func,
                               Mwt, Vwt, Mbt, Vbt, l_rate, e*Size+s+1, adam);

			if (track) fprintf(f, "%u %f\n", c, (double)error);
			c++;
            end = clock();

			printf("\repoch %u/%u, sample %u/%u: error = %f [ %.1fit/s ]      ",
                    e+1, epoch, s+1, Size, (double)error,
                    (double)(1000.0/(end-begin)));


		}
	}

	if (track) fclose(f);

    if (adam) {
        for (ui i=0; i<net->nbLayers-1; i++) {
            free(Mwt[i]);
            free(Vwt[i]);
            free(Mbt[i]);
            free(Vbt[i]);
        }
    }
}
*/

static void Network_Forward(Network *net, ld *input, cui iSize) {
    if (iSize != net->layers[0].Neurons) {
        printf("Error: Input data size has different size than neurons");
		exit(2);
    }
    net->layers[0].output = input;
	for (ui i=1; i<net->currentLayer; i++) Layer_Activate(&net->layers[i]);
}

static ld Network_BackProp(Network *net, NNParam *params, cui nth) {
    Layer *L = &net->layers[net->nbLayers-1];
	ld (*cost_deriv)(ld, ld) = get_cost_deriv(params->cost_func);
	ld (*deriv)(ld*,cui,cui) = get_deriv(L->act_name);
	ld *expected = params->outputTrain[nth];
	ld error = get_cost(params->cost_func)(L->output, expected, params->oSize);
	ld CostOut[L->Neurons];
	ld OutIn[L->Neurons];

	ui i=0;
	for(ld *cO=CostOut, *oI=OutIn, *o=L->output, *e=expected;
        cO<CostOut+L->Neurons; cO++, oI++, o++, e++, i++) {
        *cO = cost_deriv(*o, *e); //+L1+L2
        *oI = deriv(L->input, L->Neurons, i);
    }

	ld *Legacy = fvec_alloc(L->pLayer->Neurons, true);
	bool bias_done = false;
	for(ld *pO=L->pLayer->output, *leg=Legacy, *w=L->weights;
        pO<L->pLayer->output+L->pLayer->Neurons; pO++, leg++) {
        for(ld *cO=CostOut, *oI=OutIn, *b=L->bias;
            cO<CostOut+L->Neurons; cO++, oI++, w++) {
            ld ml = (* ) * (*oI);
            *leg += ml * (*w);
            *w -= l_rate * ml * (*pO);
            if (!bias_done) *b -= l_rate * ml;
        }
        bias_done = true;
    }

	ld *tempLegacy, *OutIn_i;
	for(L=net->layers+(net->nbLayers-2); L>net->layers; L--) {
        ld (*deriv_i)(ld*,cui,cui) = get_deriv(L->act_name);
        tempLegacy = fvec_alloc(L->pLayer->Neurons, true);
        OutIn_i = fvec_alloc(L->Neurons, false);
        i=0;
        for(ld *oI=OutIn_i; oI<OutIn_i+L->Neurons; oI++, i++)
            *oI = deriv_i(L->input, L->Neurons, i);
        bias_done = false;
        for(ld *tL=tempLegacy, *w=L->weights, *pO=L->pLayer->output;
            tL<tempLegacy+L->pLayer->Neurons; tL++, pO++) {
            for(ld *l=Legacy, *oI=OutIn_i, *b=L->bias; l<Legacy+L->Neurons;
                l++, oI++, b++, w++) {
                ld ml = (*l) * (*oI);
                *tL += ml * (*w);
                *w -= l_rate * ml * (*pO);
                if (!bias_done) *b -= l_rate * ml;
            }
            bias_done = true;
        }
        free(Legacy);
        Legacy = tempLegacy;
        free(OutIn_i);
	}
	free(Legacy);

	return error;
}

static void IntegrityCheck(Network *net) {
    for(ui i=1; i<net->nbLayers; i++) {
        for (ui j=0; j<net->layers[i].conns; j++) {
            //printf("\nw : %LF", net->layers[i].weights[j]);
            if (isnan(net->layers[i].weights[j]) ||
                isinf(net->layers[i].weights[j])) {
                printf("\nWeight Corruption : %LF\n",net->layers[i].weights[j]);
                exit(2);
            }
        }

        for (ui j=0; j<net->layers[i].Neurons; j++) {
            //printf("\nb : %LF", net->layers[i].bias[j]);
            if (isnan(net->layers[i].bias[j]) || isinf(net->layers[i].bias[j])) {
                printf("\nBias Corruption : %LF\n", net->layers[i].bias[j]);
                exit(2);
            }
        }
    }
    puts("No Integrity error.");
}

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
	for (ui i=0; i<net->nbLayers && net->currentLayer>0; i++) {
		Layer_Dispose(&net->layers[i]);
	}
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

ld *Network_Validate(Network *net, ld *input, cui Size) {
    Network_Forward(net, input, Size);
    argmax(net->layers[net->nbLayers-1].output,
           net->layers[net->nbLayers-1].output,
           net->layers[net->nbLayers-1].Neurons);
    return net->layers[net->nbLayers-1].output;
}

void Network_Train(Network *net, ld *input[], ld *expected_output[], cui iSize,
                   cui oSize, cui Size, cui epoch, char cost_func[], ld l_rate)
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
	long double *Mwt[net->nbLayers-1], *Vwt[net->nbLayers-1],
                *Mbt[net->nbLayers-1], *Vbt[net->nbLayers-1];
	for (ui i=0; i<net->nbLayers-1; i++) {
        Mwt[i] = fvec_alloc(net->layers[i+1].conns, true);
        Vwt[i] = fvec_alloc(net->layers[i+1].conns, true);
        Mbt[i] = fvec_alloc(net->layers[i+1].Neurons, true);
        Vbt[i] = fvec_alloc(net->layers[i+1].Neurons, true);
	}
	//

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
			ld error = Network_BackProp(net, expected_output[s], oSize,
                               cost_func, Mwt, Vwt, Mbt, Vbt, l_rate);
			if (track) fprintf(f, "%u %f\n", c, (double)error);
			c++;
            end = clock();
			printf("\repoch %u/%u, sample %u/%u: error = %f [ %.1fit/s ]      ",
                    e+1, epoch, s+1, Size, (double)error,
                    (double)(1000.0/(end-begin)));
		}
	}
	if (track) fclose(f);

	for (ui i=0; i<net->nbLayers-1; i++) {
        free(Mwt[i]);
        free(Vwt[i]);
        free(Mbt[i]);
        free(Vbt[i]);
	}

}

static void Network_Forward(Network *net, ld *input, cui iSize) {
    if (iSize != net->layers[0].Neurons) {
        printf("Error: Input data size has different size than neurons");
		exit(2);
    }
    //IntegrityCheck(net);
    net->layers[0].output = input;
	for (ui i=1; i<net->currentLayer; i++) {
        Layer_Activate(&net->layers[i]);
        /*
        for (ui j=0; j<net->layers[i].Neurons; j++)
            printf("\nout l%u n%u : %LF", i, j, net->layers[i].output[j]);
        */
	}
	for (ui i=0; i<net->layers[net->nbLayers-1].Neurons; i++) {
        if (isnan(net->layers[net->nbLayers-1].output[i])) {
            puts("nan in output");
            exit(2);
        }
	}
}

static ld Network_BackProp(Network *net, ld *expected, cui oSize,
                           char cost_func[], ld *Mwt[], ld *Vwt[],
                           ld *Mbt[], ld *Vbt[], ld l_rate)
{
    static bool fpass = true;
    //IntegrityCheck(net);

	Layer *L = &net->layers[net->nbLayers-1];
	ld (*cost_deriv)(ld, ld) = get_cost_deriv(cost_func);
	ld (*deriv)(ld*,cui,cui) = get_deriv(L->act_name);

	ld error = get_cost(cost_func)(L->output, expected, oSize);

	ld *CostOut = fvec_alloc(L->Neurons, false);
	ld *OutIn = fvec_alloc(L->Neurons, false);
	for (ui i=0; i<L->Neurons; i++) {
		CostOut[i] = cost_deriv(L->output[i], expected[i]);
		OutIn[i] = deriv(L->input, L->Neurons, i);
	}

	ld *Legacy = fvec_alloc(L->pLayer->Neurons, true);
	ui w = 0;
	bool bias_done = false;
	for (ui i=0; i<L->pLayer->Neurons; i++) {
		for (ui j=0; j<L->Neurons; j++) {
			ld ml = CostOut[j] * OutIn[j];
			ld gd = ml * L->pLayer->output[i];
			Legacy[i] += ml * L->weights[w];
			Mwt[net->nbLayers-2][w] = 0.9L*Mwt[net->nbLayers-2][w]+(1-0.9L)*gd;
			Vwt[net->nbLayers-2][w] = 0.999L*Vwt[net->nbLayers-2][w]+(1-0.999L)*
                                        powl(gd, 2);
            ld MwC = Mwt[net->nbLayers-2][w] / (1-0.9L);
            ld VwC = Vwt[net->nbLayers-2][w] / (1-0.999L);
			L->weights[w] = L->weights[w] - l_rate *
                            (MwC/sqrtl(VwC + LDBL_EPSILON));
            //printf("%LF\n", L->weights[w]);
            if (isnan(L->weights[w])) {
                puts("nan in t1\n");
                exit(3);
            }
			w++;
			if (!bias_done) {
                Mbt[net->nbLayers-2][j] = 0.9L*Mbt[net->nbLayers-2][j]+
                                        (1-0.9L) * ml;
                Vbt[net->nbLayers-2][j] = 0.999L*Vbt[net->nbLayers-2][j]+
                                        (1-0.999L) * ml;
                ld MbC = fpass ? Mbt[net->nbLayers-2][j] : Mbt[net->nbLayers-2][j] / (1-0.9L);
                ld VbC = Vbt[net->nbLayers-2][j] / (1-0.999L);
                ld nn = sqrtl(VbC);
                L->bias[j] = L->bias[j] - l_rate/((isnan(nn) ? 0 : nn)+FLT_EPSILON)
                                                                        * MbC;
                if (isnan(L->bias[j])) {
                    printf("\nnan in t2\n");
                    exit(3);
                }

                printf("\nbias : %LF, %LF, %LF, %LF\n", L->bias[j],
                       - l_rate/((isnan(nn) ? 0 : nn)+FLT_EPSILON),
                        VbC ,MbC);

			}
		}
		bias_done = true;
	}
	free(CostOut);
	free(OutIn);


	ld *tempLegacy;
	for (ui X=net->nbLayers-2; X>0; X--) {
		L = &net->layers[X];
		ld (*deriv_i)(ld*,cui,cui) = get_deriv(L->act_name);
		tempLegacy = fvec_alloc(L->pLayer->Neurons, true);
		ld *OutIn_i = fvec_alloc(L->Neurons, false);
		for (ui i=0; i<L->Neurons; i++)
			OutIn_i[i] = deriv_i(L->input, L->Neurons, i);
		ui w_i = 0;
		bool bias_done_i = false;
		for (ui i=0; i<L->pLayer->Neurons; i++) {
			for (ui j=0; j<L->Neurons; j++) {
				ld ml = Legacy[j] * OutIn_i[j];
				ld gd = ml * L->pLayer->output[i];
				tempLegacy[i] += ml * L->weights[w_i];
				Mwt[X-1][w_i] = 0.9L*Mwt[X-1][w_i]+(1-0.9L)*gd;
                Vwt[X-1][w_i] = 0.999L*Vwt[X-1][w_i]+(1-0.999L)*powl(gd, 2);
                ld MwC = Mwt[X-1][w_i] / (1-0.9L);
                ld VwC = Vwt[X-1][w_i] / (1-0.999L);
				L->weights[w_i] = L->weights[w_i] - l_rate *
                                    (MwC/sqrtl(VwC + LDBL_EPSILON));
                if (isnan(L->weights[w_i])) {
                    puts("nan in t3\n");
                    exit(3);
                }
				w_i++;
				if (!bias_done_i) {
                    Mbt[X-1][j] = 0.9L * Mbt[X-1][j] + (1-0.9L) * ml;
                    Vbt[X-1][j] = 0.999L * Vbt[X-1][j] + (1-0.999L) * ml;
                    ld MbC = Mbt[X-1][j] / (1-0.9L);
                    ld VbC = Vbt[X-1][j] / (1-0.999L);
                    ld nn = sqrtl(VbC);
                    L->bias[j] = L->bias[j] - l_rate/
                                    ((isnan(nn) ? 0 : nn)+FLT_EPSILON) * MbC;
                    if (isnan(L->bias[j])) {
                        printf("\nnan in t4 l%u n%u : %LF, %LF\n", X, j, l_rate/((isnan(nn) ?
                                                    0 : nn)+FLT_EPSILON), MbC);
                        exit(3);
                    }
				}
			}
			bias_done_i = true;
		}
		free(Legacy);
		Legacy = tempLegacy;
		free(OutIn_i);
	}
	free(Legacy);

	fpass = false;

	//IntegrityCheck(net);
	return error;
}

static void IntegrityCheck(Network *net) {
    for(ui i=1; i<net->nbLayers; i++) {
        for (ui j=0; j<net->layers[i].conns; j++) {
            printf("\nw : %LF", net->layers[i].weights[j]);
            if (isnan(net->layers[i].weights[j]) ||
                isinf(net->layers[i].weights[j])) {
                printf("\nWeight Corruption : %LF\n",net->layers[i].weights[j]);
                exit(2);
            }
        }

        for (ui j=0; j<net->layers[i].Neurons; j++) {
            printf("\nb : %LF", net->layers[i].bias[j]);
            if (isnan(net->layers[i].bias[j]) || isinf(net->layers[i].bias[j])) {
                printf("\nBias Corruption : %LF\n", net->layers[i].bias[j]);
                exit(2);
            }
        }
    }
    puts("No Integrity error.");
}


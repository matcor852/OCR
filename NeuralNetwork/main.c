#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#include "Activations.h"
#include "Layer.h"
#include "Tools.h"
#include "Network.h"

#define L_RATE 0.001L

static Network* Create(ui hn) {

	Network *net = (Network*) malloc(sizeof(Network));
    if (net == NULL) {
        printf("Error: Out of memory ...\n");
		exit(1);
    }
	Network_Init(net, 3);

	Layer *l1 = (Layer*) malloc(sizeof(Layer));
	Layer *l2 = (Layer*) malloc(sizeof(Layer));
	Layer *l3 = (Layer*) malloc(sizeof(Layer));
	Layer_Init(l1, NULL, l2, 100, NULL, NULL, false, "none");
	Layer_Init(l2, l1, l3, hn, NULL, NULL, false, "sigmoid");
	Layer_Init(l3, l2, NULL, 10, NULL, NULL, false, "softmax");

	Network_AddLayer(net, l1);
	Network_AddLayer(net, l2);
	Network_AddLayer(net, l3);
	Network_Wire(net);

    return net;
}

static void Train(Network *net) {

    ui inputSize = 100, outputSize = 10, startI = 48;
	char path[] = "D:/Code/C/OCR/NeuralNetwork/curated/hcd_100_9280_training.bin";

	ui Samples = 0;
	if (sscanf_s(path, "%*[^_]%*[_]%*[^_]%*[_]%u", &Samples) != 1) {
		printf("Could not read amount of samples in filename; Exiting...\n");
		exit(1);
	}

	FILE *fptr;
	int err;
	char errbuf[64];
	if ((err = fopen_s(&fptr, path, "rb")) != 0){
		strerror_s(errbuf,sizeof(errbuf), err);
		fprintf(stderr, "Cannot open file '%s': %s\n", path, errbuf);
		exit(1);
	}

	ui toLoop = Samples;
	long double *input[toLoop], *output[toLoop], *tempIn, *tempOut;
	float *temp;

	for(ui i=0; i<toLoop; i++) {
		tempIn = fvec_alloc(inputSize, false);
		tempOut = fvec_alloc(outputSize, true);
		temp = fvec_alloc(1, false);
		fread(temp, sizeof(float), 1, fptr);
		tempOut[(int)(temp[0]-startI)] = 1.0L;
		free(temp);
		fread(tempIn, sizeof(float), inputSize, fptr);
		input[i] = tempIn;
		output[i] = tempOut;
	}
	fclose(fptr);

	Network_Train(net, input, output, inputSize, outputSize,
                    toLoop, 1, "CrossEntropy", L_RATE);

	for(ui i=0; i<toLoop; i++) {
		free(input[i]);
		free(output[i]);
	}
}

static long double Validate(Network *net) {
    ui inputSize = 784, outputSize = 10, startI = 48;
	char path[] = "D:/Code/C/OCR/NeuralNetwork/curated/hcd_784_1025_validation.bin";
	ui Samples = 0;
	if (sscanf_s(path, "%*[^_]%*[_]%*[^_]%*[_]%u", &Samples) != 1) {
		printf("Could not read amount of samples in filename; Exiting...\n");
		exit(1);
	}
	FILE *fptr;
	int err;
	char errbuf[64];
	if ((err = fopen_s(&fptr, path, "rb")) != 0){
		strerror_s(errbuf,sizeof(errbuf), err);
		fprintf(stderr, "Cannot open file '%s': %s\n", path, errbuf);
		exit(1);
	}

	ui toLoop = Samples;
	long double *input[toLoop], *output[toLoop], *tempIn, *tempOut;
	float *temp;

	for(ui i=0; i<toLoop; i++) {
		tempIn = fvec_alloc(inputSize, false);
		tempOut = fvec_alloc(outputSize, true);
		temp = fvec_alloc(1, false);
		fread(temp, sizeof(float), 1, fptr);
		tempOut[(int)(temp[0]-startI)] = 1.0L;
		free(temp);
		fread(tempIn, sizeof(float), inputSize, fptr);
		input[i] = tempIn;
		output[i] = tempOut;
	}
	fclose(fptr);

	ui score[4] = {0, 0, 0, 0};    //TP, FP, TN, FN
	for (ui i=0; i<toLoop; i++) {
        long double *out = Network_Validate(net, input[i], inputSize);
        for (ui j=0; j<outputSize; j++) {
            if (out[j] + output[i][j] >= 2) score[0]++;
            else if (out[j] > output[i][j]) score[1]++;
            else if (absl(out[j] - output[i][j]) < LDBL_EPSILON) score[2]++;
            else if (out[j] < output[i][j]) score[3]++;
            else printf("Anomaly detected : predicted %LF, expected %LF\n",
                        out[j], output[i][j]);
        }
	}

	for(ui i=0; i<toLoop; i++) {
		free(input[i]);
		free(output[i]);
	}

    float accuracy = (score[0] + score[2])/(float)(score[0]+score[1]+score[2]+score[3]);
    float precision = score[0]/(float)(score[0]+score[1]);
    float recall = score[0]/(float)(score[0]+score[3]);
    float fscore = 2.0f*precision*recall/(precision+recall);

    printf("\nAccuracy : %f, Precision : %f, Recall : %f, Fscore : %f\n",
           accuracy, precision, recall, fscore);

	return fscore;
}



int main()
{
    srand(time(NULL));

    //216 best match ~13.5%     400
    ui min = 26, max = 26, perf_n = min, failed = 0;
    float perf_s = .0f;

    bool track = true;
	FILE *f = fopen("stats_n.txt", "w");
	if (f == NULL) track = false;

    for (ui hidden_neurons=min; hidden_neurons<=max; hidden_neurons++) {
        system("cls");
        Network *net = Create(hidden_neurons);
        printf("\n[ %u / %u ] best score %.2f%% for %u neurons (%u failed)\n",
               hidden_neurons, max, perf_s*100, perf_n, failed);
        Train(net);
        //long double score = Validate(net);
        Network_Purge(net);
        /*
        if (isnan(score) || isinf(score)) failed++;
        else {
            if (score > perf_s) {
            perf_n = hidden_neurons;
            perf_s = score;
            }
            if (track) fprintf(f, "%u %f\n", hidden_neurons, (double)score);
        }
        */
    }

    if (track) fclose(f);

    return 0;
}

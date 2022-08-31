#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#include "Activations.h"
#include "Layer.h"
#include "Tools.h"
#include "Network.h"


#define L_RATE 0.001L

static Network* CSave(ui hn) {

	Network *net = (Network*) malloc(sizeof(Network));
    if (net == NULL) {
        printf("Error: Out of memory ...\n");
		exit(1);
    }
	Network_Init(net, 3);

	Layer *l1 = (Layer*) malloc(sizeof(Layer));
	Layer *l2 = (Layer*) malloc(sizeof(Layer));
	Layer *l3 = (Layer*) malloc(sizeof(Layer));
	Layer_Init(l1, NULL, l2, 784, NULL, NULL, false, "none");
	Layer_Init(l2, l1, l3, hn, NULL, NULL, false, "sigmoid");
	Layer_Init(l3, l2, NULL, 10, NULL, NULL, false, "softmax");

	Network_AddLayer(net, l1);
	Network_AddLayer(net, l2);
	Network_AddLayer(net, l3);
	Network_Wire(net);

    return net;
}

static void Train(Network *net) {

    ui inputSize = 784, outputSize = 10, startI = 48;
	char path[] = "D:/Code/C/OCR/NeuralNetwork/curated/hcd_784_9280_training.bin";

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

    printf("\niSize = %u, oSize = %u\n", inputSize, outputSize);

	//OPEN SAVED NN
	//Network net;
	//Network_Load(&net, "NeuralNetData_3layers_OCR_digits.bin");

	Network_Train(net, input, output, inputSize, outputSize,
                    toLoop, 20, L_RATE, "CrossEntropy");

	//Network_Save(&net, name);
	//Network_Purge(&net);

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

	printf("TP : %u; FP : %u; TN : %u; FN : %u\n",
            score[0], score[1], score[2], score[3]);

    //float accuracy = (score[0] + score[2])/


	return .0L;
}



int main()
{
    for ()
    cui hidden_neurons = 1;
    Network *net = CSave(hidden_neurons);
    Train(net);
    long double score = Validate(net);
    Network_Purge(net);

    return 0;
}

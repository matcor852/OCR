#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

#include "Activations.h"
#include "Layer.h"
#include "Tools.h"
#include "Network.h"

#define MAX_THREADS 25


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



static void Train(Network *net, ui toLoop, cui epoch, cld l_rate) {

    ui inputSize = 784, outputSize = 10, startI = 0;

	char path[] = "D:/Code/C/OCR/NeuralNetwork/curated/hcd_784_846_training.bin";

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

	toLoop = Samples;
	long double *input[toLoop], *output[toLoop], *tempIn, *tempOut;
	float *temp;

	for(ui i=0; i<toLoop; i++) {
		tempIn = fvec_alloc(inputSize, false);
		tempOut = fvec_alloc(outputSize, true);
		temp = fvec_alloc(1, false);
		fread(temp, sizeof(float), 1, fptr);
		tempOut[(int)((ui)temp[0]-startI)] = 1.0L;
		free(temp);
		fread(tempIn, sizeof(float), inputSize, fptr);
		input[i] = tempIn;
		output[i] = tempOut;
	}
	fclose(fptr);

	Network_Train(net, input, output, inputSize, outputSize, toLoop,
               epoch, "CrossEntropy", l_rate, false);

	for(ui i=0; i<toLoop; i++) {
		free(input[i]);
		free(output[i]);
	}

}

static long double Validate(Network *net) {

    ui inputSize = 784, outputSize = 10, startI = 0;

	char path[] = "D:/Code/C/OCR/NeuralNetwork/curated/hcd_784_90_validation.bin";
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
        long double *out = Network_Validate(net, input[i], inputSize, outputSize == 1);
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

    /*
    printf("\nAccuracy : %.2f%%, Precision : %.2f%%, Recall : %.2f%%, Fscore : %.2f%%\n",
           accuracy*100, precision*100, recall*100, fscore*100);
    */

	return fscore;
}

typedef struct NNParam NNParam;
struct NNParam
{
    ui hiddenN, toLoop, epoch, iSize, oSize;
    ld l_rate, fscore;
    ld **inputTrain, **outputTrain;
    ld **inputTest, **outputTest;
};

static DWORD WINAPI test(LPVOID Param){
    NNParam *P = (NNParam*)Param;
    Network *net = CSave(P->hiddenN);
    Train(net, P->toLoop, P->epoch, P->l_rate);
    P->fscore = Validate(net);
    Network_Purge(net);
    return 0;
}

static void threadedSearch(void) {
    HANDLE handles[MAX_THREADS];
    DWORD threads_id[MAX_THREADS];
    NNParam params[MAX_THREADS];
    ld l_rate = 0.1L, bperf = .0L, brate = .0L;
    ui c = 0, pc = 0;

    while (bperf < .8L) {
        system("cls");


    }

    for (ui i=0; i<MAX_THREADS; i++) {
        params[i].hiddenN = 617;
        params[i].toLoop = 100;
        params[i].epoch = 10;
        params[i].l_rate = l_rate;
        handles[i] = CreateThread(NULL, 0, test, &params[i], 0, &threads_id[i]);
        if (handles[i] == NULL) {
            ExitProcess(handles[i]);
            printf("\nFailed thread %u\n", (ui)i);
        }
        l_rate *= 0.98;
    }
    WaitForMultipleObjects(MAX_THREADS, handles, TRUE, INFINITE);
    for (ui i=0; i<MAX_THREADS; i++) {
        printf("\nfscore : %LF", params[i].fscore);
        CloseHandle(handles[i]);
    }
}

int main()
{
    srand((ui) time(NULL));


    ui c = 0;
    ld score = .0L, l_rate = .1L, bperf = .0L;
    while (score < .8L) {
        c++;
        system("cls");
        Network *net = CSave(617);
        printf("\nAttempt %u (A-1 : %.2LF%%)\n", c, bperf*100);
        Train(net, 200, 10, l_rate);
        score = Validate(net);
        if (isnan(score) || isinf(score)) score = .0L;
        if (score >= .8L) Network_Save(net, "OCR");
        Network_Purge(net);
        if (score > bperf) bperf = score;
        l_rate *= 0.86;
    }



/*
    Network *net = (Network*) malloc(sizeof(Network));
    Network_Load(net, "NeuralNetData_3layers_OCR.bin");
    ld pixels[784] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};

    Network_Predict(net, pixels, 784);
    Layer *last = &net->layers[net->nbLayers-1];
    for (ui i=0; i<last->Neurons; i++) {
        printf("\n%LF\n", last->output[i]);
    }
    Network_Purge(net);
*/


/**
    //216 best match ~13.5%     400
    ui min = 617, max = 617, perf_n = min;
    float perf_s = .0f;

    bool track = true;
	FILE *f = fopen("stats_n.txt", "w");
	if (f == NULL) track = false;

    for (ui hidden_neurons=min; hidden_neurons<=max; hidden_neurons++) {
        system("cls");
        Network *net = CSave(hidden_neurons);
        printf("\n[ %u / %u ] best score %.2f%% for %u neurons\n",
               hidden_neurons, max, perf_s*100, perf_n);
        Train(net);
        long double score = Validate(net);
        Network_Purge(net);

        /*
        if (score > perf_s) {
            perf_n = hidden_neurons;
            perf_s = score;
        }
        if (track) fprintf(f, "%u %f\n", hidden_neurons, (double)score);

    }

    if (track) fclose(f);
**/
    return 0;
}

#include "IOHelper.h"

Network* CSave(ui hn) {

	Network *net = (Network*) malloc(sizeof(Network));
    if (net == NULL) {
        printf("Error: Out of memory ...\n");
		exit(1);
    }
	Network_Init(net, 3);

	Layer *l1 = (Layer*) malloc(sizeof(Layer));
	Layer *l2 = (Layer*) malloc(sizeof(Layer));
	Layer *l3 = (Layer*) malloc(sizeof(Layer));
	Layer_Init(l1, NULL, l2, 2, NULL, NULL, false, "none");
	Layer_Init(l2, l1, l3, hn, NULL, NULL, false, "leakyrelu");
	Layer_Init(l3, l2, NULL, 1, NULL, NULL, false, "sigmoid");

	Network_AddLayer(net, l1);
	Network_AddLayer(net, l2);
	Network_AddLayer(net, l3);
	Network_Wire(net);
    return net;
}

void LoadXOR(NNParam *param) {

    param->iSize = 2;
    param->oSize = 1;
    param->toLoopTrain = 4;
    param->toLoopValidate = 4;

    param->inputTrain = (ld**) malloc(sizeof(ld*) * param->toLoopTrain);
    param->outputTrain = (ld**) malloc(sizeof(ld*) * param->toLoopTrain);
    for(int i=0; i<param->toLoopTrain; i++) {
        param->inputTrain[i] = malloc(sizeof(ld) * param->iSize);
        param->outputTrain[i] = malloc(sizeof(ld) * param->oSize);
    }
    param->inputTrain[0][0] = .0L;
    param->inputTrain[0][1] = .0L;
    param->outputTrain[0][0] = .0L;

    param->inputTrain[1][0] = .0L;
    param->inputTrain[1][1] = 1.0L;
    param->outputTrain[1][0] = 1.0L;

    param->inputTrain[2][0] = 1.0L;
    param->inputTrain[2][1] = .0L;
    param->outputTrain[2][0] = 1.0L;

    param->inputTrain[3][0] = 1.0L;
    param->inputTrain[3][1] = 1.0L;
    param->outputTrain[3][0] = .0L;

    param->inputTest = param->inputTrain;
    param->outputTest = param->outputTrain;
}

void LoadData(NNParam *param) {

    param->iSize = 784;
    param->oSize = 10;

    ui startI = 0;
	char pathTrain[] = "D:/Code/C/OCR/NeuralNetwork/curated/hcd_784_95_training.bin";
	char pathValidate[] = "D:/Code/C/OCR/NeuralNetwork/curated/hcd_784_95_validation.bin";

	ui SamplesTrain = 0, SamplesValidate = 0;
	if (sscanf_s(pathTrain, "%*[^_]%*[_]%*[^_]%*[_]%u", &SamplesTrain) != 1) {
		printf("Could not read amount of samples in filename; Exiting...\n");
		exit(1);
	}
	if (sscanf_s(pathValidate, "%*[^_]%*[_]%*[^_]%*[_]%u", &SamplesValidate) != 1) {
		printf("Could not read amount of samples in filename; Exiting...\n");
		exit(1);
	}

	FILE *fptr1, *fptr2;

	int err;
	char errbuf[64];
	if ((err = fopen_s(&fptr1, pathTrain, "rb")) != 0){
		strerror_s(errbuf,sizeof(errbuf), err);
		fprintf(stderr, "Cannot open file '%s': %s\n", pathTrain, errbuf);
		exit(1);
	}
	param->toLoopTrain = min(SamplesTrain, param->toLoopTrain);
	param->inputTrain = (ld**) malloc(sizeof(float*) * param->toLoopTrain);
	param->outputTrain = (ld**) malloc(sizeof(float*) * param->toLoopTrain);
	ld *tempIn, *tempOut;
	float *temp;
	for(ui i=0; i<param->toLoopTrain; i++) {
		tempIn = fvec_alloc(param->iSize, false);
		tempOut = fvec_alloc(param->oSize, true);
		temp = fvec_alloc(1, false);
		fread(temp, sizeof(float), 1, fptr1);
		tempOut[(int)((ui)temp[0]-startI)] = 1.0L;
		free(temp);
		fread(tempIn, sizeof(float), param->iSize, fptr1);
		param->inputTrain[i] = tempIn;
		param->outputTrain[i] = tempOut;
	}
	fclose(fptr1);

	if ((err = fopen_s(&fptr2, pathValidate, "rb")) != 0){
		strerror_s(errbuf,sizeof(errbuf), err);
		fprintf(stderr, "Cannot open file '%s': %s\n", pathValidate, errbuf);
		exit(1);
	}
	param->toLoopValidate = min(SamplesValidate, param->toLoopValidate);
	param->inputTest = (ld**) malloc(sizeof(float*) * param->toLoopValidate);
	param->outputTest = (ld**) malloc(sizeof(float*) * param->toLoopValidate);
	for(ui i=0; i<param->toLoopValidate; i++) {
		tempIn = fvec_alloc(param->iSize, false);
		tempOut = fvec_alloc(param->oSize, true);
		temp = fvec_alloc(1, false);
		fread(temp, sizeof(float), 1, fptr2);
		tempOut[(int)((ui)temp[0]-startI)] = 1.0L;
		free(temp);
		fread(tempIn, sizeof(float), param->iSize, fptr2);
		param->inputTest[i] = tempIn;
		param->outputTest[i] = tempOut;
	}
	fclose(fptr2);
}

void PerfSearch(NNParam *origin, cui attempt) {
    ld bperf = .0L;
    ui c = 0;
    Network *net = NULL;
    while (c < attempt) {
        c++;
        //system("cls");
        printf("\nAttempt %u; best : %.2LF%%\n", c, bperf*100);
        net = Train_Perf(origin);
        if (origin->fscore > bperf) {
            bperf = origin->fscore;
            Network_Save(net, "XOR1");
        }
        Network_Purge(net);
    }
    //system("cls");
    printf("\nBest F-Score : %.2LF%%\n", bperf*100);
}

static Network* Train_Perf(NNParam *P) {
    Network *net = CSave(P->hiddenN);
    //Network_Display(net, true);
    Network_Train(net, P->inputTrain, P->outputTrain, P->iSize, P->oSize,
                  P->toLoopTrain, P->epoch, "MSE", P->l_rate, false);
    ui score = 0;
	for (ui i=0; i<P->toLoopValidate; i++) {
        ld *out = Network_Validate(net, P->inputTest[i], P->iSize, P->oSize == 1);
        for (ui j=0; j<P->oSize; j++) {
            printf("\n%u : %.0LF\t%.0LF", j, out[j], P->outputTest[i][j]);
            if (absl(out[j] - P->outputTest[i][j]) < LDBL_EPSILON) score++;
        }
	}
	printf("\nvalidation : %u/%u\n", score, P->toLoopValidate);
    P->fscore = score/(ld)P->toLoopValidate;
    return net;
}

static DWORD WINAPI Train(LPVOID Param) {
    NNParam *P = (NNParam*)Param;
    Network *net = CSave(P->hiddenN);
    Network_Train(net, P->inputTrain, P->outputTrain, P->iSize, P->oSize,
                  P->toLoopTrain, P->epoch, "MSE", P->l_rate, false);
    ui score = 0, all = 0;
	for (ui i=0; i<P->toLoopValidate; i++) {
        ld *out = Network_Validate(net, P->inputTest[i], P->iSize, P->oSize == 1);
        for (ui j=0; j<P->oSize; j++) {
            //printf("\n%u : %.0LF\t%.0LF", j, out[j], P->outputTest[i][j]);
            if (out[j] + P->outputTest[i][j] >= 2) score++;
            all++;
        }
	}
    P->fscore = score/(ld)all;
    Network_Purge(net);
    return 0;
}

void threadedSearch(cui threads, NNParam *origin, ld ldecay) {
    HANDLE handles[threads];
    DWORD threads_id[threads];
    NNParam params[threads];
    ld bperf = .0L, brate = .0L;
    ui bhn = 0, c = 0;

    while(c < 100) {
        ld l_rate = origin->l_rate;
        while (l_rate > 10E-10) {
            ld temp_rate = l_rate;
            c++;
            NNParam *p=params;
            DWORD *d=threads_id;
            HANDLE *h = handles;
            for(; p<params+threads; p++, d++, h++)
            {
                (*p).hiddenN = origin->hiddenN;
                (*p).toLoopTrain = origin->toLoopTrain;
                (*p).toLoopValidate = origin->toLoopValidate;
                (*p).epoch = origin->epoch;
                (*p).iSize = origin->iSize;
                (*p).oSize = origin->oSize;
                (*p).l_rate = l_rate;
                (*p).inputTrain = origin->inputTrain;
                (*p).outputTrain = origin->outputTrain;
                (*p).inputTest = origin->inputTest;
                (*p).outputTest = origin->outputTest;
                *h = CreateThread(NULL,0, Train, p, 0, d);
                if (*h == NULL) {
                    ExitProcess(*h);
                    printf("\nFailed thread %p\n", (void*)h);
                }
                l_rate *= ldecay;
            }
            system("cls");
            printf("\nAttempt %u/1000 : learning rate [%Lg - %Lg] best : %.2LF%% for %Lg\n", c, temp_rate,
                   l_rate/ldecay, bperf*100, brate);
            WaitForMultipleObjects(threads, handles, TRUE, INFINITE);
            p=params;
            h=handles;
            for(; p<params+threads; p++, h++) {
                if ((*p).fscore > bperf) {
                    bperf = (*p).fscore;
                    brate = (*p).l_rate;
                }
                CloseHandle(*h);
            }
        }
    }

    system("cls");
    printf("\nBest F-Score : %.2LF%% for %Lg\n", bperf*100, brate);
}

void NNParam_Display(NNParam *param) {
    printf("\nNeural Network Params :\n");
    printf("\tHidden neurons : %u\n", param->hiddenN);
    printf("\tTraining samples : %u\n", param->toLoopTrain);
    printf("\tValidation samples : %u\n", param->toLoopValidate);
    printf("\tEpoch : %u\n", param->epoch);
    printf("\tLearning Rate : %LF\n", param->l_rate);
    printf("\tF-Score : %.2LF%%\n", param->fscore*100);
}


void Purge_NNParam(NNParam *param) {
    for (ui i=0; i<param->toLoopTrain; i++) {
        free(param->inputTrain[i]);
        free(param->outputTrain[i]);
    }
    free(param->inputTrain);
    free(param->outputTrain);
    /*
    for (ui i=0; i<param->toLoopValidate; i++) {
        free(param->inputTest[i]);
        free(param->outputTest[i]);
    }
    free(param->inputTest);
    free(param->outputTest);
    */
    free(param);
}

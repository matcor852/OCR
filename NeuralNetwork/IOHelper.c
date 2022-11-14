#include "IOHelper.h"

Network *NetCreate(NNParam *param) {

	Network *net = (Network *)malloc(sizeof(Network));
	if (net == NULL) {
		printf("Error: Out of memory ...\n");
		exit(1);
	}
	Network_Init(net, 3);

	Layer *l1 = malloc(sizeof(Layer));
	Layer *l2 = malloc(sizeof(Layer));
	Layer *l3 = malloc(sizeof(Layer));
	Layer_Init(l1, NULL, l2, 784, NULL, NULL, false, "none");
	Layer_Init(l2, l1, l3, param->hiddenN, NULL, NULL, false, "leakyrelu");
	Layer_Init(l3, l2, NULL, 10, NULL, NULL, false, "softmax");

	Network_AddLayer(net, l1);
	Network_AddLayer(net, l2);
	Network_AddLayer(net, l3);
	Network_Wire(net);
	return net;
}

void LoadData(NNParam *param) {

	param->iSize = 784;
	param->oSize = 10;

	ui SamplesTrain = 0, SamplesValidate = 0;
	if (sscanf(param->trainingFile, "%*[^_]%*[_]%*[^_]%*[_]%u", &SamplesTrain)
		!= 1) {
		printf("Could not read amount of samples in "
			   "filename; Exiting...\n");
		exit(1);
	}
	if (sscanf(param->validationFile, "%*[^_]%*[_]%*[^_]%*[_]%u",
			   &SamplesValidate)
		!= 1) {
		printf("Could not read amount of samples in "
			   "filename; Exiting...\n");
		exit(1);
	}

	FILE *fptr1, *fptr2;

	if ((fptr1 = fopen(param->trainingFile, "rb")) == NULL) {
		fprintf(stderr, "Cannot open file '%s'\n", param->trainingFile);
		exit(1);
	}
	param->toLoopTrain = min(SamplesTrain, param->toLoopTrain);
	param->inputTrain = malloc(sizeof(float *) * param->toLoopTrain);
	param->outputTrain = malloc(sizeof(float *) * param->toLoopTrain);
	ld *tempIn, *tempOut;
	ui temp;
	for (ui i = 0; i < param->toLoopTrain; i++) {
        /*
        tempIn = malloc(sizeof(double) * param->iSize);
        tempOut = calloc(param->oSize, sizeof(double));
        */

		tempIn = fvec_alloc(param->iSize, false);
		tempOut = fvec_alloc(param->oSize, true);

		fread(&temp, sizeof(ui), 1, fptr1);
		tempOut[temp] = 1.0L;
		fread(tempIn, sizeof(double), param->iSize, fptr1);
		param->inputTrain[i] = tempIn;
		param->outputTrain[i] = tempOut;
	}
	fclose(fptr1);

	if ((fptr2 = fopen(param->validationFile, "rb")) == NULL) {
		fprintf(stderr, "Cannot open file '%s'\n", param->validationFile);
		exit(1);
	}
	param->toLoopValidate = min(SamplesValidate, param->toLoopValidate);
	param->inputTest = malloc(sizeof(float *) * param->toLoopValidate);
	param->outputTest = malloc(sizeof(float *) * param->toLoopValidate);
	for (ui i = 0; i < param->toLoopValidate; i++) {
		tempIn = fvec_alloc(param->iSize, false);
		tempOut = fvec_alloc(param->oSize, true);
		fread(&temp, sizeof(ui), 1, fptr2);
		tempOut[temp] = 1.0L;
		fread(tempIn, sizeof(double), param->iSize, fptr2);
		param->inputTest[i] = tempIn;
		param->outputTest[i] = tempOut;
	}
	fclose(fptr2);
}

float Validate(Network *net, const NNParam *P, float bperf) {
	ui score = 0, all = 0, pos = 0;
	for (ui i = 0; i < P->toLoopValidate; i++) {
		ld *out
			= Network_Validate(net, P->inputTest[i], P->iSize, P->oSize == 1);
		for (ui j = 0; j < P->oSize; j++) {
			// printf("\n%u : %.0LF\t%.0LF", j, out[j],
			// P->outputTest[i][j]);
			if (absl(out[j] - P->outputTest[i][j]) < LDBL_EPSILON) score++;
			if (P->outputTest[i][j] >= 1.0L && out[j] >= 1.0L) pos++;
			all++;
		}
		// printf("\n");
	}
	float AScore = 100.0 * score / (float)all;
	float VScore = 100.0 * pos / (float)P->toLoopValidate;
	bool single = P->oSize == 1;

	printf("%s%.2f%%\033[0m\n",
		   (single ? AScore : VScore) > bperf ? "\033[0;32m" : "\033[0;31m",
		   single ? AScore : min(AScore, VScore));
	return single ? AScore : min(AScore, VScore);
}

void ConfusionMatrix(Network *net, const NNParam *P) {
	if (P->oSize < 2) return;
	ui matrix[P->oSize][P->oSize];
	for (ui i = 0; i < P->oSize; i++) {
		for (ui j = 0; j < P->oSize; j++) matrix[i][j] = .0f;
	}

	ui bufferAct = 0, bufferExp = 0;
	bool Acted = false, Exped = false;
	for (ui i = 0; i < P->toLoopValidate; i++) {
		bufferAct = 0;
		bufferExp = 0;
		Acted = false;
		Exped = false;
		ld *out
			= Network_Validate(net, P->inputTest[i], P->iSize, P->oSize == 1);
		for (ui j = 0; j < P->oSize; j++) {
			if (P->outputTest[i][j] >= 1.0L) Exped = true;
			if (!Exped) bufferExp++;
			if (out[j] >= 1.0L) Acted = true;
			if (!Acted) bufferAct++;
		}
		matrix[bufferAct][bufferExp]++;
	}

	float bf = 0;
	printf("\n\t\033[0;31m");
	for (ui i = 0; i < P->oSize; i++) printf("%5d ", i);
	printf("\033[0m\n");
	for (ui i = 0; i < P->oSize; i++) {
		printf("\033[0;31m%d\t\033[0m", i);
		bf = 0;
		for (ui j = 0; j < P->oSize; j++) {
                if(i==j) printf("\033[0;32m");
                printf("%5d \033[0m", matrix[j][i]);
                bf += matrix[j][i];
        }
		printf("\t%.2f\n", matrix[i][i]/bf);
	}
	printf("\n");
}

void OverfitLoad(NNParam *param) {
	param->iSize = 784;
	param->oSize = 10;

	ui SamplesTrain = 0;
	if (sscanf(param->trainingFile, "%*[^_]%*[_]%*[^_]%*[_]%u", &SamplesTrain)
		!= 1) {
		printf("Could not read amount of samples in "
			   "filename; Exiting...\n");
		exit(1);
	}

	FILE *fptr1;
	if ((fptr1 = fopen(param->trainingFile, "rb")) == NULL) {
		fprintf(stderr, "Cannot open file '%s'\n", param->trainingFile);
		exit(1);
	}
	param->toLoopTrain = min(SamplesTrain, param->toLoopTrain);
	param->toLoopValidate = param->toLoopTrain;
	param->inputTrain = malloc(sizeof(float *) * param->toLoopTrain);
	param->outputTrain = malloc(sizeof(float *) * param->toLoopTrain);
	ld *tempIn, *tempOut;
	ld *temp;
	for (ui i = 0; i < param->toLoopTrain; i++) {
		tempIn = fvec_alloc(param->iSize, false);
		tempOut = fvec_alloc(param->oSize, true);
		temp = fvec_alloc(1, false);
		fread(temp, sizeof(ld), 1, fptr1);
		tempOut[(ui)temp[0]] = 1.0L;
		free(temp);
		fread(tempIn, sizeof(ld), param->iSize, fptr1);
		param->inputTrain[i] = tempIn;
		param->outputTrain[i] = tempOut;
	}
	fclose(fptr1);

	param->inputTest = param->inputTrain;
	param->outputTest = param->outputTrain;
}

void PerfSearch(NNParam *origin, Network *net, int attempt) {
	float bperf = (float)origin->toExceed, curr_perf = .0f;
	ui eSize = floor(log10(origin->epoch)) + 1;
	printf("\nBeginning Neural Network training with "
		   "following parameters :\n");
	NNParam_Display(origin);
	bool maxed = false;
	while (attempt > 0 && !maxed) {
		srand((ui)time(NULL));
		if (net == NULL) net = NetCreate(origin);
		if (origin->track) fclose(fopen(origin->StatsFile, "w"));
		Optimizer_Init(net, origin->optimizer);
		for (ui e = 0; e < origin->epoch && !maxed;) {
			printf("[ Epoch %*u/%u ] Accuracy : ", eSize, e, origin->epoch);
			curr_perf = Validate(net, origin, bperf);
			if (curr_perf > bperf) {
				bperf = curr_perf;
				char *s = malloc(sizeof(char) * 12);
				snprintf(s, 10, "%s_%.2f", origin->NNName, bperf);
				Network_Save(net, s);
				free(s);
			}
			if (curr_perf >= 100.0f) maxed = 1;
			else {
				Network_Train(net, origin);
				ui ne
					= min((int)origin->epochInterval, (int)(origin->epoch - e));
				origin->epochInterval = ne;
				e += ne;
			}
		}

		printf("[ Epoch %u/%u ] Accuracy : ", origin->epoch, origin->epoch);
		curr_perf = Validate(net, origin, bperf);
		if (curr_perf >= 100.0f) maxed = 1;
		if (curr_perf > bperf) {
			bperf = curr_perf;
			char *s = malloc(sizeof(char) * 12);
			snprintf(s, 10, "%s_%.2f", origin->NNName, bperf);
			Network_Save(net, s);
			free(s);
		}
		// Network_Display(net, true);
		ConfusionMatrix(net, origin);
		Optimizer_Dispose(net, origin->optimizer, !attempt);
		Network_Purge(net);
		net = NULL;
		attempt--;
	}
}

void NNParam_Display(NNParam *param) {
	// printf("\nNeural Network Params :\n");
	printf("\n\tHidden neurons : %u", param->hiddenN);
	printf("\t\t\tLearning Rate : %Lg\n", param->l_rate);
	printf("\tTraining samples : %u", param->toLoopTrain);
	printf("\t\t\tValidation samples : %u\n", param->toLoopValidate);
	printf("\tEpoch : %u", param->epoch);
	printf("\t\t\t\tEpoch interval : %u\n", param->epochInterval);
	printf("\tCost Function : %s", param->cost_func);
	printf("\t\t\tTracking : %s\n", param->track ? "true" : "false");
	printf("\tL1 Regularization : %LF", param->l1Norm);
	printf("\t\tL2 Regularization : %LF\n\n", param->l2Norm);
}

void Purge_NNParam(NNParam *param) {
	if (param->cost_func) free(param->cost_func);
	if (param->NNName != NULL) free(param->NNName);
	if (param->StatsFile != NULL) free(param->StatsFile);
	bool freeOnce = param->toLoopTrain == param->toLoopValidate;
	for (ui i = 0; i < param->toLoopTrain; i++) {
		free(param->inputTrain[i]);
		free(param->outputTrain[i]);
	}
	free(param->inputTrain);
	free(param->outputTrain);
	if (freeOnce) return;
	for (ui i = 0; i < param->toLoopValidate; i++) {
		free(param->inputTest[i]);
		free(param->outputTest[i]);
	}
	free(param->inputTest);
	free(param->outputTest);
	free(param);
}

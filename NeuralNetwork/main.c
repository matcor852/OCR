#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "IOHelper.h"
#define MAX_IN_LINE 50

static void matchParams(NNParam *origin, char *line) {
	char field[30], value[30];
	sscanf(line, "%s = %s", field, value);

	if (!strcmp(field, "hiddenN"))
		origin->hiddenN = (ui)strtol(value, NULL, 10);
	else if (!strcmp(field, "toLoopTrain"))
		origin->toLoopTrain = (ui)strtol(value, NULL, 10);
	else if (!strcmp(field, "toLoopValidate"))
		origin->toLoopValidate = (ui)strtol(value, NULL, 10);
	else if (!strcmp(field, "epoch"))
		origin->epoch = (ui)strtol(value, NULL, 10);
	else if (!strcmp(field, "epochInterval"))
		origin->epochInterval = (ui)strtol(value, NULL, 10);
	else if (!strcmp(field, "l_rate")) origin->l_rate = (ld)strtod(value, NULL);
	else if (!strcmp(field, "l1Norm")) origin->l1Norm = (ld)strtod(value, NULL);
	else if (!strcmp(field, "l2Norm")) origin->l2Norm = (ld)strtod(value, NULL);
	else if (!strcmp(field, "cost_func")) {
		origin->cost_func = (char *)malloc(sizeof(char) * (strlen(value) + 1));
		strcpy(origin->cost_func, value);
	} else if (!strcmp(field, "optimizer")) {
		origin->optimizer = !strcmp(value, "true")
								? (Optimizer *)malloc(sizeof(Optimizer))
								: NULL;
	} else if (!strcmp(field, "track"))
		origin->track = !strcmp(value, "true") ? true : false;
	else if (!strcmp(field, "StatsFile")) {
		origin->StatsFile = (char *)malloc(sizeof(char) * (strlen(value) + 1));
		strcpy(origin->StatsFile, value);
	} else if (!strcmp(field, "NNName")) {
		origin->NNName = (char *)malloc(sizeof(char) * (strlen(value) + 1));
		strcpy(origin->NNName, value);
	} else if (!strcmp(field, "toExceed"))
		origin->toExceed = (ld)strtod(value, NULL);
}

int main() {


	int argc = 5;
	char *argv[5] = {"Train", "TrainedNetwork/NN.cfg",
                    "DataSets/hcd_784_5235_training.bin",
                    "DataSets/hcd_784_578_validation.bin", "1"};


/*
	int argc = 4;
	char *argv[4] = {"Predict",
					 "TrainedNetwork/NeuralNetData_3layers_XOR_100.0.dnn",
					 "1",
					 "0"};

*/
	argc--;
	//argv++;


	srand((ui)time(NULL));

	if (argc < 1) {
		printf("Missing argument.");
		exit(1);
	}

	if (!strcmp(argv[0], "Predict")) {
		if (argc < 4) {
			printf("\nMissing Parameter: Expected 3; Got %d\n", argc - 1);
			printf("Help:\n\tPredictXOR [Neural Network "
				   "data (.dnn)] ");
			printf("[First value (0|1)] [Second value "
				   "(0|1)]\n");
			exit(1);
		}
		ld input[2] = {0, 0};
		input[0] = (ld)strtod(argv[2], NULL);
		input[1] = (ld)strtod(argv[3], NULL);
		Network *net = (Network *)malloc(sizeof(Network));
		Network_Load(net, argv[1]);
		printf("\n\t%Lg XOR %Lg = %u\n", input[0], input[1],
			   Network_Predict(net, input, 2));
		Network_Purge(net);
		exit(0);
	} else if (!strcmp(argv[0], "Train")) {
		if (argc != 4) {
			printf("\nParameter Error: Expected 4; Got %d\n", argc - 1);
			printf("Help:\n\tTrain [config file (.cfg)] ");
			printf("[Training data (.bin)] [Validation data (.bin)]\n");
			printf("[number of attempt]\n");
			exit(1);
		}

		FILE *configFile = NULL;
		if ((configFile = fopen(argv[1], "rb")) == NULL) {
			fprintf(stderr, "Cannot open file '%s'\n", argv[1]);
			exit(1);
		}

		NNParam *origin = malloc(sizeof(NNParam));
		char *buffer = malloc(sizeof(char) * MAX_IN_LINE);
		while (!feof(configFile)) {
			fgets(buffer, MAX_IN_LINE, configFile);
			if (ferror(configFile)) {
				fprintf(stderr, "Reading error with code %d\n", errno);
				exit(1);
			}
			matchParams(origin, buffer);
		}
		free(buffer);
		fclose(configFile);

        origin->trainingFile = argv[2];
        origin->validationFile = argv[3];
		LoadData(origin);
		PerfSearch(origin, NULL, (int)strtol(argv[4], NULL, 10));
		Purge_NNParam(origin);

	} else {
		printf("\nUnknown Command '%s'\n", argv[0]);
		exit(1);
	}

	return 0;

	/*
		Network *net = (Network*) malloc(sizeof(Network));
		Network_Load(net,
	   "NeuralNetData_3layers_OCR63p16.bin"); ld pixels[784]
	   = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	   0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	   0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	   0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	   0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	   0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	   0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	   0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	   0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	   0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	   0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	   0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	   0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	   0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	   0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	   0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	   0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	   0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	   0.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	   0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	   0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	   0.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0,
	   0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	   0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0,
	   0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0,
	   0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	   0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0,
	   0.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	   0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	   0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0,
	   0.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	   0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	   0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0,
	   0.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	   0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	   0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0,
	   0.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	   0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	   0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0,
	   0.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	   0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	   0.0, 0.0, 0.0,
	   0.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.0,
	   0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	   0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	   0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 1.0, 1.0, 0.0, 0.0,
	   0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	   0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	   0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0,
	   0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	   0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	   0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	   0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	   0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	   0.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	   0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	   0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
	   0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	   0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	   0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0,
	   0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	   0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	   0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	   0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	   0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	   0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	   0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	   0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	   0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	   0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	   0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	   0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	   0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	   0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	   0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	   0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	   0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	   0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	   0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	   0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	   0.0, 0.0, 0.0, 0.0, 0.0, 0.0};

		Network_Predict(net, pixels, 784);
		Layer *last = &net->layers[net->nbLayers-1];
		for (ui i=0; i<last->Neurons; i++) {
			printf("\n%LF\n", last->output[i]);
		}
		Network_Purge(net);
	*/

	return 0;
}

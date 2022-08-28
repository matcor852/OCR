#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#include "Activations.h"
#include "Layer.h"
#include "Tools.h"
#include "Network.h"


#define L_RATE 0.001L

static void CSave() {
    //SAVE NN

	Network net;
	Network_Init(&net, 5);

	Layer l1, l2, l3, l4, l5;
	Layer_Init(&l1, NULL, &l2, 784, NULL, NULL, false, "none");
	Layer_Init(&l2, &l1, &l3, 654, NULL, NULL, false, "leakyrelu");
	Layer_Init(&l3, &l2, &l4, 123, NULL, NULL, false, "sigmoid");
	Layer_Init(&l4, &l3, &l5, 32, NULL, NULL, false, "sigmoid");
	Layer_Init(&l5, &l4, NULL, 10, NULL, NULL, false, "softmax");

	Network_AddLayer(&net, &l1);
	Network_AddLayer(&net, &l2);
	Network_AddLayer(&net, &l3);
	Network_AddLayer(&net, &l4);
	Network_AddLayer(&net, &l5);


	Network_Save(&net);
	Network_Purge(&net);
}

static void Train() {
    ui inputSize = 784, outputSize = 10, startI = 48;
	char path[] = "D:/Code/C/OCR/NeuralNetwork/curated/hcd_784_10305.bin";

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
	long double *input[toLoop], *output[toLoop];

	for(ui i=0; i<toLoop; i++) {
		long double *tempIn = fvec_alloc(inputSize, false);
		long double *tempOut = fvec_alloc(outputSize, true);
		float *temp = fvec_alloc(1, false);
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

	Network net;
	Network_Load(&net, "NeuralNetData_5layers_OCR_digits.bin");

	Network_Train(&net, input, output, inputSize, outputSize,
                    toLoop, 10, L_RATE, "CrossEntropy");

	//Network_Save(&net);
	Network_Purge(&net);

	for(ui i=0; i<toLoop; i++) {
		free(input[i]);
		free(output[i]);
	}
}




int main()
{

    CSave();
    Train();

    return 0;
}

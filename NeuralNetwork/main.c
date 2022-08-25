#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "Activations.h"
#include "Layer.h"
#include "Tools.h"
#include "Network.h"

#define L_RATE 0.1f

int main()
{

	char path[] = "D:/Code/C/OCR/NeuralNetwork/curated/hcd_784_62382.bin";

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
	float *input[toLoop], *output[toLoop];

	for(ui i=0; i<toLoop; i++) {
		float *tempIn = fvec_alloc(784, false);
		float *tempOut = fvec_alloc(94, true);
		float *temp = fvec_alloc(1, false);
		fread(temp, sizeof(float), 1, fptr);
		tempOut[(int)(temp[0]-33)] = 1.0f;
		free(temp);
		fread(tempIn, sizeof(float), 784, fptr);
		input[i] = tempIn;
		output[i] = tempOut;
	}
	fclose(fptr);



	//OPEN SAVED NN

	Network net;
	Network_Load(&net, "NeuralNetData_3layers_OCR.bin");
	//Network_Display(&net, true);

	Network_Train(&net, input, output, 784, 94, toLoop, 5, L_RATE, "MSE");
	//Network_Display(&net, true);
	Network_Save(&net);
	Network_Purge(&net);

	for(ui i=0; i<toLoop; i++) {
		free(input[i]);
		free(output[i]);
	}


/*
	//SAVE NN

	Network net;
	Network_Init(&net, 3);

	Layer l1, l2, l3;
	Layer_Init(&l1, NULL, &l2, 784, NULL, NULL, false, "none");
	Layer_Init(&l2, &l1, &l3, 100, NULL, NULL, false, "sigmoid");
	Layer_Init(&l3, &l2, NULL, 94, NULL, NULL, false, "softmax");

	Network_AddLayer(&net, &l1);
	Network_AddLayer(&net, &l2);
	Network_AddLayer(&net, &l3);


	Network_Save(&net);
	Network_Purge(&net);
*/

    return 0;
}

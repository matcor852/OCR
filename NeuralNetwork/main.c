#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "Activations.h"
#include "Layer.h"
#include "Tools.h"

#include "Network.h"


#define L_RATE 0.01f


int main()
{
	srand((unsigned int) time(NULL));




	//OPEN SAVED NN


	Network net;
	Network_Load(&net, "NeuralNetData_3layers_XOR.bin", Train, L_RATE);
	//Network_Display(&net);


	float *input[] = {	(float[2]){0,0},
						(float[2]){0,1},
						(float[2]){1,0},
						(float[2]){1,1}};

	float *output[] = {	(float[1]){0},
						(float[1]){1},
						(float[1]){1},
						(float[1]){0}};


	Network_Train(&net, input, output, 2, 1, 4, 1);
	Network_Purge(&net);


/*
	//SAVE NN

	Network net;
	Network_Init(&net, 3, Train, L_RATE);

	Layer l1, l2, l3;
	Layer_Init(&l1, NULL, &l2, 2, NULL, 0, false);
	Layer_Init(&l2, &l1, &l3, 2, NULL, 0, false);
	Layer_Init(&l3, &l2, NULL, 1, NULL, 0, false);

	l1.activation = &none;
	l1.act_name = "none";
	l2.activation = &leakyrelu;
	l2.act_name = "leakyrelu";
	l3.activation = &sigmoid;
	l3.act_name = "sigmoid";

	Network_AddLayer(&net, &l1);
	Network_AddLayer(&net, &l2);
	Network_AddLayer(&net, &l3);


	Network_Save(&net);
	Network_Purge(&net);
*/

    return 0;
}

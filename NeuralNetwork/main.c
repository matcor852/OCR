#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "Activations.h"
#include "Cost.h"
#include "Layer.h"
#include "Tools.h"

#include "Network.h"

#define L_RATE 0.01f

int main()
{
	srand((unsigned int) time(NULL));




	//OPEN SAVED NN


	Network net;
	Network_Load(&net, "NeuralNetData_3layers_XOR.bin");
	Network_Display(&net, true);


	float *input[] = {	(float[2]){0,0}};	/*,
						(float[2]){0,1},
						(float[2]){1,0},
						(float[2]){1,1}};*/

	float *output[] = {	(float[1]){0}};		/*,
						(float[1]){1},
						(float[1]){1},
						(float[1]){0}};*/


	Network_Train(&net, input, output, 2, 1, 1, 1, L_RATE, "MSE");
	Network_Display(&net, true);
	Network_Purge(&net);


/*
	//SAVE NN

	Network net;
	Network_Init(&net, 3);

	Layer l1, l2, l3;
	Layer_Init(&l1, NULL, &l2, 2, NULL, NULL, false, "none");
	Layer_Init(&l2, &l1, &l3, 2, NULL, NULL, false, "leakyrelu");
	Layer_Init(&l3, &l2, NULL, 1, NULL, NULL, false, "sigmoid");

	Network_AddLayer(&net, &l1);
	Network_AddLayer(&net, &l2);
	Network_AddLayer(&net, &l3);


	Network_Save(&net);
	Network_Purge(&net);
*/

    return 0;
}

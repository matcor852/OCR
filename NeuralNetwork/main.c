#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "Activations.h"
#include "Layer.h"
#include "Tools.h"



int main()
{
	srand((unsigned int) time(NULL));

	Layer l1, l2, l3;
	Layer_Init(&l1, NULL, &l2, 2, NULL, NULL);
	Layer_Init(&l2, &l1, &l3, 2, NULL, NULL);
	Layer_Init(&l3, &l2, NULL, 1, NULL, NULL);
	Layer *temp = &l2;

	float input[] = {0,1};
	Layer_SetInput(&l1, input, 2);
	l2.activation = &sigmoid;
	l3.activation = &softmax;

/*
	printf("\nweights l1 :");
	matr_display(l1.weights, l1.conns, l1.Neurons);

	printf("\nweights l2 :");
	matr_display(l2.weights, l2.conns, l2.Neurons);
	printf("\nbias l2 :");
	matr_display(l2.bias, l2.Neurons, 1);

	printf("\nweights l3 :");
	matr_display(l3.weights, l3.conns, l3.Neurons);
	printf("\nbias l3 :");
	matr_display(l3.bias, l3.Neurons, 1);
*/
	ui layer = 2;
	while (temp != NULL) {
		Layer_Activate(temp);
		/*
		printf("Layer %d bias after activation :", layer);
		matr_display(temp->bias, temp->Neurons, 1);
		*/
		printf("Layer %d weights after activation :", layer);
		matr_display(temp->weights, temp->conns, 1);

		temp = temp->nLayer;
		layer += 1;
	}


	Layer_Dispose(&l1);
	Layer_Dispose(&l2);
	Layer_Dispose(&l3);


    return 0;
}

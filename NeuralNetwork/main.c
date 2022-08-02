#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "Activations.h"
#include "Layer.h"

int main()
{
	srand((unsigned int) time(NULL));

	Layer l1, l2, l3;
	Layer_Init(&l1, NULL, &l2, 2);
	Layer_Init(&l2, &l1, &l3, 2);
	Layer_Init(&l3, &l2, NULL, 1);

	//l2.activation = &sigmoid;


	//float res = l1.activation(0.5f);
	//printf("%f", res);





	Layer_Dispose(&l1);
	Layer_Dispose(&l2);
	Layer_Dispose(&l3);


    return 0;
}

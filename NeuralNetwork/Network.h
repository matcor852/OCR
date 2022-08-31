#ifndef NETWORK_H
#define NETWORK_H

#include <errno.h>
#include <time.h>
#include "Layer.h"
#include "Activations.h"
#include "Cost.h"


typedef struct Network Network;
struct Network
{
	Layer *layers;
	ui nbLayers, currentLayer;
};


void Network_Load(Network *net, char path[]);
void Network_Init(Network *net, cui nbLayers);
void Network_AddLayer(Network *net, Layer *layer);
void Network_Save(Network *net, char name[]);
void Network_Purge(Network *net);
void Network_Display(Network *net, bool display_matr);
void Network_Wire(Network *net);

void Network_Predict(Network *net, long double *input, cui Size);
long double *Network_Validate(Network *net, long double *input, cui Size);
void Network_Train(Network *net, long double *input[],
                   long double *expected_output[], cui iSize, cui oSize,
                   cui Size, cui epoch, long double l_rate, char cost_func[]);
void Network_Forward(Network *net, long double *input, cui iSize);
long double Network_BackProp(Network *net, long double *expected, cui oSize, long double l_rate,
					char cost_func[]);

Layer *lvec_alloc(cui n);


#endif // NETWORK_H

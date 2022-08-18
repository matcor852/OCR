#ifndef NETWORK_H
#define NETWORK_H

#include <errno.h>
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
void Network_Save(Network *net);
void Network_Purge(Network *net);
void Network_Display(Network *net, bool display_matr);

void Network_Train(Network *net, float *input[], float *expected_output[],
				cui iSize, cui oSize, cui Size, cui epoch, float l_rate,
				char cost_func[]);
void Network_Forward(Network *net, float *input, cui iSize);
void Network_BackProp(Network *net, float *expected, cui oSize, float l_rate,
					char cost_func[]);

void Network_Predict(Network *net, float *input, cui Size);

Layer *lvec_alloc(cui n);


#endif // NETWORK_H

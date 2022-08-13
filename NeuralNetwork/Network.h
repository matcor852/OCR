#ifndef NETWORK_H
#define NETWORK_H

#include <errno.h>
#include "Layer.h"
#include "Activations.h"

#define Train 0
#define Predict 1


typedef struct Network Network;

struct Network
{
	Layer *layers;
	ui nbLayers, currentLayer, mode;
	float l_rate;
};


void Network_Load(Network *net, char path[], cui mode, float l_rate);
void Network_Init(Network *net, cui nbLayers, cui mode, float l_rate);
void Network_AddLayer(Network *net, Layer *layer);
void Network_Save(Network *net);
void Network_Purge(Network *net);

void Network_Display(Network *net);
void Network_Train(Network *net, float *input[], float *expected_output[],
				ui iSize, ui oSize, ui Size, ui epoch);
void Network_Predict(Network *net, float *input);

Layer *lvec_alloc(cui n);


#endif // NETWORK_H

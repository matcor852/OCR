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
	ui nbLayers;
	ui currentLayer;
	ui mode;
};


void Network_Load(Network *net, char path[], cui mode);
void Network_Init(Network *net, cui nbLayers, cui mode);
void Network_AddLayer(Network *net, Layer *layer);

void Network_Save(Network *net);
void Network_Purge(Network *net);

Layer *lvec_alloc(cui n);


#endif // NETWORK_H

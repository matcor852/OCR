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

void Network_Predict(Network *net, ld *input, cui Size);
ld *Network_Validate(Network *net, ld *input, cui Size);
void Network_Train(Network *net, ld *input[],
                   ld *expected_output[], cui iSize, cui oSize,
                   cui Size, cui epoch, char cost_func[], ld l_rate);
static void Network_Forward(Network *net, ld *input, cui iSize);
static ld Network_BackProp(Network *net, ld *expected, cui oSize,
                           char cost_func[], ld *Mwt[], ld *Vwt[],
                           ld *Mbt[], ld *Vbt[], ld l_rate);

static void IntegrityCheck(Network *net);

Layer *lvec_alloc(cui n);


#endif // NETWORK_H

#include "Optimizers.h"

void Optimizer_Init(Network *net, Optimizer *optz)
{
    optz->Mwt = (ld**) malloc(sizeof(ld*) * net->nbLayers-1);
    optz->Mbt = (ld**) malloc(sizeof(ld*) * net->nbLayers-1);
    optz->Vwt = (ld**) malloc(sizeof(ld*) * net->nbLayers-1);
    optz->Vbt = (ld**) malloc(sizeof(ld*) * net->nbLayers-1);
    for (ui i=0; i<net->nbLayers-1; i++) {
        optz->Mwt[i] = fvec_alloc(net->layers[i+1].conns, true);
        optz->Vwt[i] = fvec_alloc(net->layers[i+1].conns, true);
        optz->Mbt[i] = fvec_alloc(net->layers[i+1].Neurons, true);
        optz->Vbt[i] = fvec_alloc(net->layers[i+1].Neurons, true);
    }
}

ld Penalty(Network *net, Optimizer *optz)
{
    return 0;
    if (absl(optz->l1Norm - optz->l2Norm) < LDBL_EPSILON) return 0;
    bool dn1 = absl(optz->l1Norm - .0L) < LDBL_EPSILON;
    bool dn2 = absl(optz->l2Norm - .0L) < LDBL_EPSILON;
    ld l1 = 0, l2 = 0;
    Layer *l = NULL;
    for (ui i=1; i<net->currentLayer; i++) {
        l = &net->layers[i];
        for (ld *w=l->weights; w<l->weights+l->conns; w++) {
            if (dn1) l1 += absl(*w);
            if (dn2) l2 += (*w) * (*w);
        }
    }
    return optz->l1Norm*l1 + optz->l2Norm*l2;
}

void Optimizer_Dispose(Network *net, Optimizer *optz)
{
    for (ui i=0; i<net->nbLayers-1; i++) {
        free(optz->Mwt[i]);
        free(optz->Mbt[i]);
        free(optz->Vwt[i]);
        free(optz->Vbt[i]);
    }
    free(optz->Mwt);
    free(optz->Mbt);
    free(optz->Vwt);
    free(optz->Vbt);
    free(optz);
}

#ifndef OPTIMIZERS_H
#define OPTIMIZERS_H

#include "Network.h"

void Optimizer_Init(Network *net, Optimizer *optz);
void Optimizer_Dispose(Network *net, Optimizer *optz);

ld Penalty(Network *net, Optimizer *optz);

#endif // OPTIMIZERS_H

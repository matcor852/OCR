#ifndef IOHELPER_H
#define IOHELPER_H

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#include "Network.h"

Network* CSave(ui hn);
void LoadXOR(NNParam *param);
void LoadData(NNParam* param);
float Validate(Network *net, const NNParam *P);
void OverfitLoad(NNParam *param);

void PerfSearch(NNParam *origin, Network *net);
static DWORD WINAPI Train(LPVOID Param);
void threadedSearch(cui threads, NNParam *origin, ld ldecay);
void NNParam_Display(NNParam *param);
void Purge_NNParam(NNParam *param);

#endif // IOHELPER_H

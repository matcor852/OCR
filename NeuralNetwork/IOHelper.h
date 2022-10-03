#ifndef IOHELPER_H
#define IOHELPER_H

#include "Tools.h"

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#include "Network.h"


typedef struct NNParam NNParam;
struct NNParam
{
    ui hiddenN, toLoopTrain, toLoopValidate,
        epoch, iSize, oSize;
    ld l_rate, fscore, l1Norm, l2Norm;
    ld **inputTrain, **outputTrain;
    ld **inputTest, **outputTest;
    char *cost_func;

};

Network* CSave(ui hn);
void LoadXOR(NNParam *param);
void LoadData(NNParam* param);

void PerfSearch(NNParam *origin, cui attempt);
static Network* Train_Perf(NNParam *P);
static DWORD WINAPI Train(LPVOID Param);
void threadedSearch(cui threads, NNParam *origin, ld ldecay);
void NNParam_Display(NNParam *param);
void Purge_NNParam(NNParam *param);

#endif // IOHELPER_H

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
    ld l_rate, fscore;
    ld **inputTrain, **outputTrain;
    ld **inputTest, **outputTest;

};

Network* CSave(ui hn);
void LoadData(NNParam* param);

static DWORD WINAPI Train(LPVOID Param);
void threadedSearch(cui threads, NNParam *origin, ld ldecay);
void NNParam_Display(NNParam *param);
void Purge_NNParam(NNParam *param);

#endif // IOHELPER_H

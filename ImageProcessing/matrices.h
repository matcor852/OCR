#pragma once

#include "tools.h"

void invMat33(float mat[3][3], float res[3][3]);
void matMul33_31(float mat33[3][3], float mat31[3], float res[3]);
void matMul33_33(float mat1[3][3], float mat2[3][3], float res[3][3]);
void getMatrixFromCorners(float corners[4][2], float res[3][3]);
void getTransformMatrix(float corners[4][2], st new_w, st new_h, float res[3][3]);

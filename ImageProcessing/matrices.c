#include "matrices.h"

void matMul33_31(float mat33[3][3], float mat31[3], float res[3])
{
	float a = mat33[0][0], b = mat33[0][1], c = mat33[0][2];
	float d = mat33[1][0], e = mat33[1][1], f = mat33[1][2];
	float g = mat33[2][0], h = mat33[2][1], i = mat33[2][2];
	float x = mat31[0], y = mat31[1], z = mat31[2];
	res[0] = a * x + b * y + c * z;
	res[1] = d * x + e * y + f * z;
	res[2] = g * x + h * y + i * z;
}

void invMat33(float mat[3][3], float res[3][3])
{
	float a = mat[0][0], b = mat[0][1], c = mat[0][2];
	float d = mat[1][0], e = mat[1][1], f = mat[1][2];
	float g = mat[2][0], h = mat[2][1], i = mat[2][2];
	float det = a * e * i + b * f * g + c * d * h
				- c * e * g - b * d * i - a* f * h;
	res[0][0] = (e*i-f*h)/det, res[0][1] = (c*h-b*i)/det, res[0][2] = (b*f-c*e)/det;
	res[1][0] = (f*g-d*i)/det, res[1][1] = (a*i-c*g)/det, res[1][2] = (c*d-a*f)/det;
	res[2][0] = (d*h-e*g)/det, res[2][1] = (b*g-a*h)/det, res[2][2] = (a*e-b*d)/det;
}

void matMul33_33(float mat1[3][3], float mat2[3][3], float res[3][3])
{
	float a = mat1[0][0], b = mat1[0][1], c = mat1[0][2];
	float d = mat1[1][0], e = mat1[1][1], f = mat1[1][2];
	float g = mat1[2][0], h = mat1[2][1], i = mat1[2][2];
	float j = mat2[0][0], k = mat2[0][1], l = mat2[0][2];
	float m = mat2[1][0], n = mat2[1][1], o = mat2[1][2];
	float p = mat2[2][0], q = mat2[2][1], r = mat2[2][2];
	res[0][0] =
}

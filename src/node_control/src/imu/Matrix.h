/********************************************************
* Matrix.h
* basic calculation functions Matrix
* Version 1.0
* date 2012.9.20
* Designed by Jingxiao
********************************************************/
#ifndef _MATRIX_H_
#define _MATRIX_H_

#include "Vector.h"
#include "Quaternion.h"

PMAT MZero(PMAT pm);
PMAT MInitialize(PMAT pm, double m00, double m01, double m02,
				          double m10, double m11, double m12,
				          double m20, double m21, double m22);
PMAT MEqual(PMAT pmOut, MAT pmIn);
PMAT MIdentity(PMAT pm);
double MNorm(MAT pmIN);
PMAT MTranspose(PMAT pmOut, MAT pmIn);
PMAT MInv(PMAT pmOut, MAT pmIn);
PMAT MAdd(PMAT pmOut, MAT pmIn1, MAT pmIn2);
PMAT MSubtract(PMAT pmOut, MAT pmIn1, MAT pmIn2);
PMAT MMulFactor(PMAT pmOut, MAT pmIn, double f);
PMAT MMultiply(PMAT pmOut, MAT pmIn1, MAT pmIn2);
PVECT MMulVector(PVECT pvOut, MAT pmIn, VECT pvIn);
PMAT MOptOrtho(PMAT pmOut, MAT pmIn);

PQUAT M2AttQuaternion(PQUAT pqOut, MAT pmIn);
PVECT M2Attitude(PVECT pvOut, MAT pmIn);
/////////////////////////
PMAT MStandardization(PMAT pmOut, MAT pmIn);

#endif

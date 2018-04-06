#ifndef _QUATERNION_H_
#define _QUATERNION_H_

#include <math.h>
#include "Vector.h"

/* quaternion */
typedef struct
{
  double q0, q1, q2, q3;
}QUAT, *PQUAT;

PQUAT QZero(PQUAT pq);
PQUAT QInitialize(PQUAT pq, double q0, double q1, double q2, double q3);
PQUAT QEqual(PQUAT pqOut, QUAT pqIn);
double QNorm(QUAT pqIn);
PQUAT QNormalize(PQUAT pqOut, QUAT pqIn);
PQUAT QMulFactor(PQUAT pqOut, QUAT pqIn, double f);
PQUAT QConjugate(PQUAT pqOut, QUAT pqIn);
PQUAT QMultiply(PQUAT pqOut, QUAT pqIn1, QUAT pqIn2);
PVECT QMulVector(PVECT pvOut, QUAT pqIn, VECT pvIn);//?????????????
PQUAT QMulRVector(PQUAT pqOut, QUAT pqIn, VECT pvIn);
PQUAT QDelPhi(PQUAT pqRl, VECT pvPhi, QUAT pqCl);
PMAT Q2AttMatrix(PMAT pmOut, QUAT pqIn);
PVECT Q2Attitude(PVECT pvOut, QUAT pqIn);
PQUAT V2RVQuaternion(PQUAT pqOut, VECT pvIn);



#endif

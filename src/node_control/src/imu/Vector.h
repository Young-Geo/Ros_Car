#ifndef _VECTOR_H_
#define _VECTOR_H_

#include <math.h>

/* vector3 */
typedef struct
{
  double i, j, k;
}VECT, *PVECT;

/* vector3 */
typedef struct
{
  float x, y, z;
}FVECT, *PFVECT;

/* matrix3 */
typedef struct
{
  double m[3][3];
}MAT, *PMAT;

////////////////////////////////////////////////////////////////
PVECT VZero(PVECT pv);
PVECT VInitialize(PVECT pv, double i, double j, double k);
PVECT VEqual(PVECT pvOut, VECT pvIn);
double VNorm(VECT pvIn);
PVECT VAdd(PVECT pvOut, VECT pvIn1, VECT pvIn2);
PVECT VAdd2(PVECT pvOut, VECT pvIn1, VECT pvIn2, VECT pvIn3);
PVECT VSubtract(PVECT pvOut, VECT pvIn1, VECT pvIn2);
PVECT VMulFactor(PVECT pvOut, VECT pvIn, double f);
PVECT VMulCross(PVECT pvOut, VECT pvIn1, VECT pvIn2);
PVECT VMulPoint(PVECT pvOut, VECT pvIn1, VECT pvIn2);
double VMulDot(VECT pvIn2, VECT pvIn1);

PVECT VMulMatrix(PVECT pvOut, VECT pvIn, MAT pmIn);
PMAT VMulVector(PMAT pmOut, VECT pvIn1, VECT pvIn2);
PMAT V2Matrix(PMAT pm, VECT v1, VECT v2, VECT v3);
PMAT V2CrossMatrix(PMAT pm, VECT vIn);
PMAT VCrossMulMat(PMAT pm, VECT vIn, MAT mIn);
/////////////////////////////////////////////////////////////////

#endif

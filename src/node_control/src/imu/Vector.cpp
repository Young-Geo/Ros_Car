#include "Vector.h"

/////////////////////////////////////////////////////////////////////////////////
/*	Fun describe: initialize vector pv to zero
	Para in		: pv
	Para out	: pv	*/
PVECT VZero(PVECT pv)
{
	pv->i = 0;
	pv->j = 0;
	pv->k = 0;

	return pv;
}

/*	Fun describe: initialize vector pv with i, j, k
	Para in		: i, j, k
	Para out	: pv	*/
PVECT VInitialize(PVECT pv, double i, double j, double k)
{
	pv->i = i;
	pv->j = j;
	pv->k = k;

	return pv;
}

/*	Fun describe: pvOut = pvIn
	Para in		: pvIn
	Para out	: pvOut	*/
PVECT VEqual(PVECT pvOut, VECT pvIn)
{
	pvOut->i = pvIn.i;
	pvOut->j = pvIn.j;
	pvOut->k = pvIn.k;

	return pvOut;
}

/*	Fun describe: vector norm
	Para in		: pvIn
	Para out	: return norm of vector pvIN	*/
double VNorm(VECT pvIn)
{
	return(sqrt(pvIn.i*pvIn.i + pvIn.j*pvIn.j + pvIn.k*pvIn.k));
}

/*	Fun describe: pvOut = pvIn1 + pvIn2
	Para in		: pvIn1, pvIn2
	Para out	: pvOut	*/
PVECT VAdd(PVECT pvOut, VECT pvIn1, VECT pvIn2)
{
	pvOut->i = pvIn1.i + pvIn2.i;
	pvOut->j = pvIn1.j + pvIn2.j;
	pvOut->k = pvIn1.k + pvIn2.k;

	return pvOut;
}

/*	Fun describe: pvOut = pvIn1 + pvIn2 + pvIn3
	Para in		: pvIn1, pvIn2, pvIn3
	Para out	: pvOut	*/
PVECT VAdd2(PVECT pvOut, VECT pvIn1, VECT pvIn2, VECT pvIn3)
{
	pvOut->i = pvIn1.i + pvIn2.i + pvIn3.i;
	pvOut->j = pvIn1.j + pvIn2.j + pvIn3.j;
	pvOut->k = pvIn1.k + pvIn2.k + pvIn3.k;

	return pvOut;
}

/*	Fun describe: pvOut = pvIn1 - pvIn2
	Para in		: pvIn1, pvIn2
	Para out	: pvOut	*/
PVECT VSubtract(PVECT pvOut, VECT pvIn1, VECT pvIn2)
{
	pvOut->i = pvIn1.i - pvIn2.i;
	pvOut->j = pvIn1.j - pvIn2.j;
	pvOut->k = pvIn1.k - pvIn2.k;

	return pvOut;
}

/*	Fun describe: pvOut = pvIn1 * f
	Para in		: pvIn1, f
	Para out	: pvOut	*/
PVECT VMulFactor(PVECT pvOut, VECT pvIn, double f)
{
	pvOut->i = pvIn.i * f;
	pvOut->j = pvIn.j * f;
	pvOut->k = pvIn.k * f;

	return pvOut;
}

/*	Fun describe: pvOut = cross product of pvIn1 and pvIn2
	Para in		: pvIn1, pvIn2
	Para out	: pvOut	*/
PVECT VMulCross(PVECT pvOut, VECT pvIn1, VECT pvIn2)
{
	pvOut->i = pvIn1.j*pvIn2.k - pvIn1.k*pvIn2.j;
	pvOut->j = pvIn1.k*pvIn2.i - pvIn1.i*pvIn2.k;
	pvOut->k = pvIn1.i*pvIn2.j - pvIn1.j*pvIn2.i;

	return pvOut;
}

/*	Fun describe: pvOut = point product of pvIn1 and pvIn2
	Para in		: pvIn1, pvIn2
	Para out	: pvOut
*/
PVECT VMulPoint(PVECT pvOut, VECT pvIn1, VECT pvIn2)
{
	pvOut->i = pvIn1.i * pvIn2.i;
	pvOut->j = pvIn1.j * pvIn2.j;
	pvOut->k = pvIn1.k * pvIn2.k;

	return pvOut;
}

/*	Fun describe: inner(dot) product of pvIn1 and pvIn2
	Para in		: pvIn1, pvIn2
	Para out	: return dot product result	*/
double VMulDot(VECT pvIn1, VECT pvIn2)
{
	return(pvIn1.i*pvIn2.i + pvIn1.j*pvIn2.j + pvIn1.k*pvIn2.k);
}

/*	Fun describe: pmOut = pmIn * pvIn
	Para in		: pmIn, pvIn
	Para out	: pvOut	*/
PVECT VMulMatrix(PVECT pvOut, VECT pvIn, MAT pmIn)
{
	pvOut->i = pmIn.m[0][0]*pvIn.i + pmIn.m[1][0]*pvIn.j + pmIn.m[2][0]*pvIn.k;
	pvOut->j = pmIn.m[0][1]*pvIn.i + pmIn.m[1][1]*pvIn.j + pmIn.m[2][1]*pvIn.k;
	pvOut->k = pmIn.m[0][2]*pvIn.i + pmIn.m[1][2]*pvIn.j + pmIn.m[2][2]*pvIn.k;

	return pvOut;
}

/*                       |i|           |ix iy iz|
	Fun describe: pmOut = |j|*|x y z| = |jx jy jz|
                          |k|           |kx ky kz|
	Para in		: pvIn1, pvIn2
	Para out	: pmOut	*/
PMAT VMulVector(PMAT pmOut, VECT pvIn1, VECT pvIn2)
{
	pmOut->m[0][0] = pvIn1.i*pvIn2.i;
	pmOut->m[0][1] = pvIn1.i*pvIn2.j;
	pmOut->m[0][2] = pvIn1.i*pvIn2.k;
	pmOut->m[1][0] = pvIn1.j*pvIn2.i;
	pmOut->m[1][1] = pvIn1.j*pvIn2.j;
	pmOut->m[1][2] = pvIn1.j*pvIn2.k;
	pmOut->m[2][0] = pvIn1.k*pvIn2.i;
	pmOut->m[2][1] = pvIn1.k*pvIn2.j;
	pmOut->m[2][2] = pvIn1.k*pvIn2.k;

	return pmOut;
}

PMAT V2Matrix(PMAT pm, VECT v1, VECT v2, VECT v3)
{
	pm->m[0][0] = v1.i;		pm->m[0][1] = v1.j;		pm->m[0][2] = v1.k;
	pm->m[1][0] = v2.i;		pm->m[1][1] = v2.j;		pm->m[1][2] = v2.k;
	pm->m[2][0] = v3.i;		pm->m[2][1] = v3.j;		pm->m[2][2] = v3.k;

	return pm;
}

PMAT V2CrossMatrix(PMAT pm, VECT vIn)
{
	pm->m[0][0] = 0;		pm->m[0][1] = -vIn.k;		pm->m[0][2] = vIn.j;
	pm->m[1][0] = vIn.k;	pm->m[1][1] = 0;			pm->m[1][2] = -vIn.i;
	pm->m[2][0] = -vIn.j;	pm->m[2][1] = vIn.i;		pm->m[2][2] = 0;

	return pm;
}

PMAT VCrossMulMat(PMAT pm, VECT vIn, MAT mIn)
{
	pm->m[0][0] = -vIn.k*mIn.m[1][0] + vIn.j*mIn.m[2][0];
	pm->m[0][1] = -vIn.k*mIn.m[1][1] + vIn.j*mIn.m[2][1];
	pm->m[0][2] = -vIn.k*mIn.m[1][2] + vIn.j*mIn.m[2][2];

	pm->m[1][0] = vIn.k*mIn.m[0][0] - vIn.i*mIn.m[2][0];
	pm->m[1][1] = vIn.k*mIn.m[0][1] - vIn.i*mIn.m[2][1];
	pm->m[1][2] = vIn.k*mIn.m[0][2] - vIn.i*mIn.m[2][2];

	pm->m[2][0] = -vIn.j*mIn.m[0][0] + vIn.i*mIn.m[1][0];
	pm->m[2][1] = -vIn.j*mIn.m[0][1] + vIn.i*mIn.m[1][1];
	pm->m[2][2] = -vIn.j*mIn.m[0][2] + vIn.i*mIn.m[1][2];

	return pm;
}
////////////////////////////////////////////////////////////////////////////////





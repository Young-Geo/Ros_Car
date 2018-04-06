#ifndef _KARMAN_H_
#define _KARMAN_H_

#include <stdlib.h>
#include "Vector.h"

typedef unsigned char Uint8 ;

#define KL		6
#define KM		3
#define VGL		6
#define TAO		3600.0

typedef struct
{
	double r;
	double Wm0;
	double Wc0;
	double Wmc;
}UKFPARA, *PUKFPARA;

typedef struct
{
	double m[KL];
}KVECTX, *PKVECTX;

typedef struct
{
	double m[KM];
}KVECTY, *PKVECTY;

typedef struct
{
	double m[KL][KL];
}KMATXX, *PKMATXX;

typedef struct
{
	double m[KL][KM];
}KMATXY, *PKMATXY;

typedef struct
{
	double m[KM][KL];
}KMATYX, *PKMATYX;

typedef struct
{
	double m[KM][KM];
}KMATYY, *PKMATYY;

typedef struct
{
	KVECTX X;
	KMATXX P;
	KVECTY Z;
	KMATXX Q;
	KMATYX H;
	KMATYY R;
}KARDATA, *PKARDATA;

PKVECTX KXZero(PKVECTX pkvOut);
PKMATXX KXXZero(PKMATXX pkmOut);
PKMATXX KXXEye(PKMATXX pkmOut);
PKMATXX KXXMultiply(PKMATXX pkmOut, KMATXX pkmIn1, KMATXX pkmIn2);
PKMATXX KXXAdd(PKMATXX pkmOut, KMATXX pkmIn1, KMATXX pkmIn2);
PKMATXX KXXAdd2(PKMATXX pkmOut, KMATXX pkmIn1, KMATXX pkmIn2, KMATXX pkmIn3);
PKMATXX KXXSubtract(PKMATXX pkmOut, KMATXX pkmIn1, KMATXX pkmIn2);
PKVECTX KXXMulVectX(PKVECTX pkvOut, KMATXX pkmIn, KVECTX pkvIn);
PKVECTX KVectXAdd(PKVECTX pkvOut, KVECTX pkvIn1, KVECTX pkvIn2);
PKMATXX KXXMulFactor(PKMATXX pkmOut, KMATXX pkmIn, double f);
PKMATXX KXXTranspose(PKMATXX pkmOut, KMATXX pkmIn);
PKMATXY KYXTranspose(PKMATXY pkmOut, KMATYX pkmIn);
PKMATYX KXYTranspose(PKMATYX pkmOut, KMATXY pkmIn);
PKMATYY KYYAdd(PKMATYY pkmOut, KMATYY pkmIn1, KMATYY pkmIn2);
PKMATYY KYY3MInv(PKMATYY pmOut, KMATYY pmIn);
PKMATYY KYY2MInv(PKMATYY pmOut, KMATYY pmIn);
PKVECTY KVectYSub(PKVECTY pkmvOut, KVECTY pkmvIn1, KVECTY pkmvIn2);
PKVECTY KYXMulVectX(PKVECTY pkmvOut, KMATYX pkmmIn, KVECTX pkvIn);
PKMATYX KYXMulXX(PKMATYX pkmOut, KMATYX pkmIn1, KMATXX pkmIn2);
PKVECTX KXYMulVectY(PKVECTX pkvOut, KMATXY pkkmat, KVECTY pkmv);
PKMATXY KXYMulYY(PKMATXY pkmOut, KMATXY pkmIn1, KMATYY pmIn2);
PKMATXX KXYMulYX(PKMATXX pkOut, KMATXY pkmIn1, KMATYX pkmIn2);
PKMATXY KXXMulXY(PKMATXY pkmOut, KMATXX pkmIn1, KMATXY pkmIn2);
PKMATYY KYXMulXY(PKMATYY pmOut, KMATYX pkmIn1, KMATXY pkmIn2);

////////////////////////////////
void KMatZero(double *mout, int x, int y);
void KMatEqual(double *mout, double *m, int x, int y);
void KMatAdd(double *mout, double *m1, double *m2, int x, int y);
void KMatAdd2(double *mout, double *m1, double *m2, double *m3, int x, int y);
void KMatSub(double *mout, double *m1, double *m2, int x, int y);
void KMatMulFactor(double *mout, double *m, int x, int y, double f);
void KMatMulFactorAdd(double *mout, double *m1, double *m2, int x, int y, double f);
void KMatMultiply(double *mout, double *m1, double *m2, int x, int y, int z);
void KMatEye(double *mout, int n);
void KMatDiag(double *mout, int n, double f);
void KMatTranspose(double *mout, double *m, int x, int y);
int KMatInv(double *a, double *b, int n);

void GetKalmanF(double *F, Uint8 line, MAT Cnb) ;
void GetKalmanQ(double *Q, double *M1, double *F, Uint8 line, double t);
void GetKalmanPhi(double *phi, double *F, Uint8 line, double t);
void KFTimeUpdate(double *Xk_1, double *Pk_1, double *phi, double *Q, Uint8 line);
void KFMeasureUpdate(double *Xk_1, double *Pk_1, double *Zk, double *H, double *R, Uint8 line);

void Equal_KLKL(double y[VGL][VGL], double *x, Uint8 line);
#endif

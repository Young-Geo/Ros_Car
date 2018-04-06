#include "Quaternion.h"
#include "Matrix.h"

///////////////////////////////////////////////////////////////////////////
/*	Fun describe: quaternion pq to zero
	Para in		: pq
	Para out	: pq	*/
PQUAT QZero(PQUAT pq)
{
	pq->q0 = 0;
	pq->q1 = 0;
	pq->q2 = 0;
	pq->q3 = 0;

	return pq;
}

/*	Fun describe: initialize quaternion pq with q0, q1, q2, q3
	Para in		: q0, q1, q2, q3
	Para out	: pq	*/
PQUAT QInitialize(PQUAT pq, double q0, double q1, double q2, double q3)
{
	pq->q0 = q0, pq->q1 = q1, pq->q2 = q2, pq->q3 = q3;

	return pq;
}

/*	Fun describe: pqOut = pqIn
	Para in		: pqIn
	Para out	: pqOut	*/
PQUAT QEqual(PQUAT pqOut, QUAT pqIn)
{
	pqOut->q0 = pqIn.q0;
	pqOut->q1 = pqIn.q1;
	pqOut->q2 = pqIn.q2;
	pqOut->q3 = pqIn.q3;

	return pqOut;
}

/*	Fun describe: quaternion norm
	Para in		: pqIn
	Para out	: return norm of quaternion pqIn	*/
double QNorm(QUAT pqIn)
{
	return(sqrt(pqIn.q0*pqIn.q0 + pqIn.q1*pqIn.q1 + pqIn.q2*pqIn.q2 + pqIn.q3*pqIn.q3));
}

/*	Fun describe: normalize quaternion
	Para in		: pqInOut
	Para out	: pqInOut	*/
PQUAT QNormalize(PQUAT pqOut, QUAT pqIn)
{
	double norm;

	norm = QNorm(pqIn);
	if(norm > 1.0e-20)
		QMulFactor(pqOut, pqIn, 1.0/norm);
	else
		QInitialize(pqOut, 1, 0, 0, 0);

	return pqOut;
}

/*	Fun describe: pqOut = pqIn * f
	Para in		: pqIn, f
	Para out	: pqOut	*/
PQUAT QMulFactor(PQUAT pqOut, QUAT pqIn, double f)
{
	pqOut->q0 = pqIn.q0 * f;
	pqOut->q1 = pqIn.q1 * f;
	pqOut->q2 = pqIn.q2 * f;
	pqOut->q3 = pqIn.q3 * f;

	return pqOut;
}

/*	Fun describe: pqOut is the conjugate of pqIn
	Para in		: pqIn
	Para out	: pqOut	*/
PQUAT QConjugate(PQUAT pqOut, QUAT pqIn)
{
	pqOut->q0 =  pqIn.q0;
	pqOut->q1 = -pqIn.q1;
	pqOut->q2 = -pqIn.q2;
	pqOut->q3 = -pqIn.q3;

	return pqOut;
}

/*	Fun describe: pqOut = pqIn1 * pqIn2
	Para in		: pqIn1, pqIn2
	Para out	: pqOut	*/
PQUAT QMultiply(PQUAT pqOut, QUAT pqIn1, QUAT pqIn2)
{
	pqOut->q0 = pqIn1.q0*pqIn2.q0 - pqIn1.q1*pqIn2.q1 - pqIn1.q2*pqIn2.q2 - pqIn1.q3*pqIn2.q3;
	pqOut->q1 = pqIn1.q0*pqIn2.q1 + pqIn1.q1*pqIn2.q0 + pqIn1.q2*pqIn2.q3 - pqIn1.q3*pqIn2.q2;
	pqOut->q2 = pqIn1.q0*pqIn2.q2 + pqIn1.q2*pqIn2.q0 + pqIn1.q3*pqIn2.q1 - pqIn1.q1*pqIn2.q3;
	pqOut->q3 = pqIn1.q0*pqIn2.q3 + pqIn1.q3*pqIn2.q0 + pqIn1.q1*pqIn2.q2 - pqIn1.q2*pqIn2.q1;

	return pqOut;
}

/*	Fun describe: [0 pvOut] = pqIn * [0 pvIn]^T *pqIn^T
	Para in		: pqIn, pvIn
	Para out	: pvOut	*/
PVECT QMulVector(PVECT pvOut, QUAT pqIn, VECT pvIn)
{
	QUAT qtmp;
  
	qtmp.q0 =                   - pqIn.q1*pvIn.i - pqIn.q2*pvIn.j - pqIn.q3*pvIn.k;
	qtmp.q1 = pqIn.q0*pvIn.i                     + pqIn.q2*pvIn.k - pqIn.q3*pvIn.j;
	qtmp.q2 = pqIn.q0*pvIn.j                     + pqIn.q3*pvIn.i - pqIn.q1*pvIn.k;
	qtmp.q3 = pqIn.q0*pvIn.k                     + pqIn.q1*pvIn.j - pqIn.q2*pvIn.i;
	pvOut->i = -qtmp.q0*pqIn.q1 + qtmp.q1*pqIn.q0 - qtmp.q2*pqIn.q3 + qtmp.q3*pqIn.q2;
	pvOut->j = -qtmp.q0*pqIn.q2 + qtmp.q2*pqIn.q0 - qtmp.q3*pqIn.q1 + qtmp.q1*pqIn.q3;
	pvOut->k = -qtmp.q0*pqIn.q3 + qtmp.q3*pqIn.q0 - qtmp.q1*pqIn.q2 + qtmp.q2*pqIn.q1;

	return pvOut;
}

/*	Fun describe: quaternion multiply rotation vector, i.e. qnb = qnb' * Q(vbnb)
	Para in		: pqIn, pvIn
	Para out	: pqOut	*/
PQUAT QMulRVector(PQUAT pqOut, QUAT pqIn, VECT pvIn)
{
	QUAT qtmp;

	V2RVQuaternion(&qtmp, pvIn);
	return QMultiply(pqOut, pqIn, qtmp);
}

/* qnb = qn'b - phi */
PQUAT QDelPhi(PQUAT pqRl, VECT pvPhi, QUAT pqCl)
{
	/*QReal = QuatMul(Rv2Quat(phi), QCal);*/
	QUAT qtmp;

	V2RVQuaternion(&qtmp, pvPhi);
	return QMultiply(pqRl, qtmp, pqCl);
}

/*	Fun describe: convert attitude quaternion to attitude matrix
					  | q0^2+q1^2+q2^2-q3^2, 2*(q1*q2-q0*q3),	  2*(q1*q3+q0*q2)	  |
				  m = | 2*(q1*q2+q0*q3),	 q0^2-q1^2+q2^2-q3^2, 2*(q2*q3-q0*q1)	  |
					  | 2*(q1*q3-q0*q2),	 2*(q2*q3+q0*q1),	  q0^2-q1^2-q2^2+q3^2 |
	Para in		: pqIn
	Para out	: pmOut	 */
PMAT Q2AttMatrix(PMAT pmOut, QUAT pqIn)
{
	double
		q11 = pqIn.q0*pqIn.q0, q12 = pqIn.q0*pqIn.q1, q13 = pqIn.q0*pqIn.q2, q14 = pqIn.q0*pqIn.q3,
		q22 = pqIn.q1*pqIn.q1, q23 = pqIn.q1*pqIn.q2, q24 = pqIn.q1*pqIn.q3,
		q33 = pqIn.q2*pqIn.q2, q34 = pqIn.q2*pqIn.q3,
		q44 = pqIn.q3*pqIn.q3;
    return MInitialize(pmOut,
						q11+q22-q33-q44,  2*(q23-q14),     2*(q24+q13),
						2*(q23+q14),      q11-q22+q33-q44, 2*(q34-q12),
						2*(q24-q13),      2*(q34+q12),     q11-q22-q33+q44 );
}

/*	Fun describe: convert attitude quaternion to attitude vector
	Para in		: pqIn
	Para out	: pvOut	 */
PVECT Q2Attitude(PVECT pvOut, QUAT pqIn)
{
	MAT mtmp;

	Q2AttMatrix(&mtmp, pqIn);
	return M2Attitude(pvOut, mtmp);
}

/*	Fun describe: transform rotation vector to quaternion
				  q = [cos(0.5*|v|),sin(0.5*|v|)/(0.5*|v|)*(0.5*v)]
	Para in		: pvIn
	Para out	: pqOut	*/
PQUAT V2RVQuaternion(PQUAT pqOut, VECT pvIn)
{
	double v_2, tmp;

	v_2 = VNorm(pvIn)/2.0;
	if(fabs(v_2)<1.0e-20)
	{
		QInitialize(pqOut, 1, 0, 0, 0);
	}
	else
	{
		tmp = sin(v_2)/v_2*0.5;
		QInitialize(pqOut, cos(v_2), tmp*pvIn.i, tmp*pvIn.j, tmp*pvIn.k);
	}

	return pqOut;
}
/////////////////////////////////////////////////////////////////////////////////

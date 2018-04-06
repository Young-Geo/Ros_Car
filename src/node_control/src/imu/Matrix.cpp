#include "Matrix.h"
/////////////////////////////////////////////////////////////////////
/*	Fun describe: initialize matrix pm with nine elements to 0
	Para in		: m00 ... m22
	Para out	: pm	*/
PMAT MZero(PMAT pm)
{
	pm->m[0][0] = pm->m[0][1] = pm->m[0][2] = 0;
	pm->m[1][0] = pm->m[1][1] = pm->m[1][2] = 0;
	pm->m[2][0] = pm->m[2][1] = pm->m[2][2] = 0;

	return pm;
}

/*	Fun describe: initialize matrix pm with nine elements
	Para in		: m00 ... m22
	Para out	: pm	*/
PMAT MInitialize(PMAT pm, double m00, double m01, double m02,
				          double m10, double m11, double m12,
				          double m20, double m21, double m22)
{
	pm->m[0][0] = m00;	pm->m[0][1] = m01;	pm->m[0][2] = m02;
	pm->m[1][0] = m10;	pm->m[1][1] = m11;	pm->m[1][2] = m12;
	pm->m[2][0] = m20;	pm->m[2][1] = m21;	pm->m[2][2] = m22;

	return pm;
}

/*	Fun describe: pmOut = pmIn
	Para in		: pmIn
	Para out	: pmOut	*/
PMAT MEqual(PMAT pmOut, MAT pmIn)
{
	pmOut->m[0][0] = pmIn.m[0][0];
	pmOut->m[0][1] = pmIn.m[0][1];
	pmOut->m[0][2] = pmIn.m[0][2];
	pmOut->m[1][0] = pmIn.m[1][0];
	pmOut->m[1][1] = pmIn.m[1][1];
	pmOut->m[1][2] = pmIn.m[1][2];
	pmOut->m[2][0] = pmIn.m[2][0];
	pmOut->m[2][1] = pmIn.m[2][1];
	pmOut->m[2][2] = pmIn.m[2][2];

	return pmOut;
}

/*	Fun describe: initialize matrix pm with nine elements to 0
	Para in		: m00 ... m22
	Para out	: pm	*/
PMAT MIdentity(PMAT pm)
{
	MZero(pm);
	pm->m[0][0] = pm->m[1][1] = pm->m[2][2] = 1;

	return pm;
}

/*	Fun describe: matrix det
	Para in		: pmIn
	Para out	: return det of matrix pmIn	*/
double MNorm(MAT pmIN)
{
/*
	return   pmIN.m[0][0]*(pmIN.m[1][1]*pmIN.m[2][2]-pmIN.m[1][2]*pmIN.m[2][1])
			-pmIN.m[0][1]*(pmIN.m[1][0]*pmIN.m[2][2]-pmIN.m[1][2]*pmIN.m[2][0])
			+pmIN.m[0][2]*(pmIN.m[1][0]*pmIN.m[2][1]-pmIN.m[1][1]*pmIN.m[2][0]);*/

	return   pmIN.m[0][0]*(pmIN.m[1][1]*pmIN.m[2][2]-pmIN.m[1][2]*pmIN.m[2][1])
			-pmIN.m[1][0]*(pmIN.m[0][1]*pmIN.m[2][2]-pmIN.m[0][2]*pmIN.m[2][1])
			+pmIN.m[2][0]*(pmIN.m[0][1]*pmIN.m[1][2]-pmIN.m[0][2]*pmIN.m[1][1]);

}

/*	Fun describe: pmOut = pmIn'
	Para in		: pmIn
	Para out	: pmOut	*/
PMAT MTranspose(PMAT pmOut, MAT pmIn)
{
	pmOut->m[0][0] = pmIn.m[0][0];
	pmOut->m[0][1] = pmIn.m[1][0];
	pmOut->m[0][2] = pmIn.m[2][0];
	pmOut->m[1][0] = pmIn.m[0][1];
	pmOut->m[1][1] = pmIn.m[1][1];
	pmOut->m[1][2] = pmIn.m[2][1];
	pmOut->m[2][0] = pmIn.m[0][2];
	pmOut->m[2][1] = pmIn.m[1][2];
	pmOut->m[2][2] = pmIn.m[2][2];

	return pmOut;
}

/*	Fun describe: pmOut = pmIn^-1 (Matrix inverse)
	Para in		: pmIn
	Para out	: pmOut	*/
PMAT MInv(PMAT pmOut, MAT pmIn)
{
	double n;
	MAT tmp;

	n = MNorm(pmIn);
	tmp.m[0][0] =   pmIn.m[1][1]*pmIn.m[2][2]-pmIn.m[1][2]*pmIn.m[2][1];
	tmp.m[1][0] = -(pmIn.m[1][0]*pmIn.m[2][2]-pmIn.m[1][2]*pmIn.m[2][0]);
	tmp.m[2][0] =   pmIn.m[1][0]*pmIn.m[2][1]-pmIn.m[1][1]*pmIn.m[2][0];
	tmp.m[0][1] = -(pmIn.m[0][1]*pmIn.m[2][2]-pmIn.m[0][2]*pmIn.m[2][1]);
	tmp.m[1][1] =   pmIn.m[0][0]*pmIn.m[2][2]-pmIn.m[0][2]*pmIn.m[2][0];
	tmp.m[2][1] = -(pmIn.m[0][0]*pmIn.m[2][1]-pmIn.m[0][1]*pmIn.m[2][0]);
	tmp.m[0][2] =   pmIn.m[0][1]*pmIn.m[1][2]-pmIn.m[0][2]*pmIn.m[1][1];
	tmp.m[1][2] = -(pmIn.m[0][0]*pmIn.m[1][2]-pmIn.m[0][2]*pmIn.m[1][0]);
	tmp.m[2][2] =   pmIn.m[0][0]*pmIn.m[1][1]-pmIn.m[0][1]*pmIn.m[1][0];

	//if you use mush please optimize it
	MMulFactor(pmOut, tmp, 1.0/n);

	return pmOut;
}

/*	Fun describe: pmOut = pmIn1 + pmIn2
	Para in		: pmIn1, pmIn2
	Para out	: pmOut	*/
PMAT MAdd(PMAT pmOut, MAT pmIn1, MAT pmIn2)
{
	pmOut->m[0][0] = pmIn1.m[0][0] + pmIn2.m[0][0];
	pmOut->m[0][1] = pmIn1.m[0][1] + pmIn2.m[0][1];
	pmOut->m[0][2] = pmIn1.m[0][2] + pmIn2.m[0][2];
	pmOut->m[1][0] = pmIn1.m[1][0] + pmIn2.m[1][0];
	pmOut->m[1][1] = pmIn1.m[1][1] + pmIn2.m[1][1];
	pmOut->m[1][2] = pmIn1.m[1][2] + pmIn2.m[1][2];
	pmOut->m[2][0] = pmIn1.m[2][0] + pmIn2.m[2][0];
	pmOut->m[2][1] = pmIn1.m[2][1] + pmIn2.m[2][1];
	pmOut->m[2][2] = pmIn1.m[2][2] + pmIn2.m[2][2];

	return pmOut;
}

/*	Fun describe: pmOut = pmIn1 - pmIn2
	Para in		: pmIn1, pmIn2
	Para out	: pmOut	*/
PMAT MSubtract(PMAT pmOut, MAT pmIn1, MAT pmIn2)
{
	pmOut->m[0][0] = pmIn1.m[0][0] - pmIn2.m[0][0];
	pmOut->m[0][1] = pmIn1.m[0][1] - pmIn2.m[0][1];
	pmOut->m[0][2] = pmIn1.m[0][2] - pmIn2.m[0][2];
	pmOut->m[1][0] = pmIn1.m[1][0] - pmIn2.m[1][0];
	pmOut->m[1][1] = pmIn1.m[1][1] - pmIn2.m[1][1];
	pmOut->m[1][2] = pmIn1.m[1][2] - pmIn2.m[1][2];
	pmOut->m[2][0] = pmIn1.m[2][0] - pmIn2.m[2][0];
	pmOut->m[2][1] = pmIn1.m[2][1] - pmIn2.m[2][1];
	pmOut->m[2][2] = pmIn1.m[2][2] - pmIn2.m[2][2];

	return pmOut;
}

/*	Fun describe: pmOut = pmIn * f
	Para in		: pmIn, f
	Para out	: pmOut	*/
PMAT MMulFactor(PMAT pmOut, MAT pmIn, double f)
{
  	pmOut->m[0][0] = pmIn.m[0][0] * f;
	pmOut->m[0][1] = pmIn.m[0][1] * f;
	pmOut->m[0][2] = pmIn.m[0][2] * f;
	pmOut->m[1][0] = pmIn.m[1][0] * f;
	pmOut->m[1][1] = pmIn.m[1][1] * f;
	pmOut->m[1][2] = pmIn.m[1][2] * f;
	pmOut->m[2][0] = pmIn.m[2][0] * f;
	pmOut->m[2][1] = pmIn.m[2][1] * f;
	pmOut->m[2][2] = pmIn.m[2][2] * f;

	return pmOut;
}

/*	Fun describe: pmOut = pmIn1 * pmIn2
	Para in		: pmIn1, pmIn2
	Para out	: pmOut	*/
PMAT MMultiply(PMAT pmOut, MAT pmIn1, MAT pmIn2)
{
	pmOut->m[0][0] = pmIn1.m[0][0]*pmIn2.m[0][0] + pmIn1.m[0][1]*pmIn2.m[1][0] + pmIn1.m[0][2]*pmIn2.m[2][0];
	pmOut->m[0][1] = pmIn1.m[0][0]*pmIn2.m[0][1] + pmIn1.m[0][1]*pmIn2.m[1][1] + pmIn1.m[0][2]*pmIn2.m[2][1];
	pmOut->m[0][2] = pmIn1.m[0][0]*pmIn2.m[0][2] + pmIn1.m[0][1]*pmIn2.m[1][2] + pmIn1.m[0][2]*pmIn2.m[2][2];
	pmOut->m[1][0] = pmIn1.m[1][0]*pmIn2.m[0][0] + pmIn1.m[1][1]*pmIn2.m[1][0] + pmIn1.m[1][2]*pmIn2.m[2][0];
	pmOut->m[1][1] = pmIn1.m[1][0]*pmIn2.m[0][1] + pmIn1.m[1][1]*pmIn2.m[1][1] + pmIn1.m[1][2]*pmIn2.m[2][1];
	pmOut->m[1][2] = pmIn1.m[1][0]*pmIn2.m[0][2] + pmIn1.m[1][1]*pmIn2.m[1][2] + pmIn1.m[1][2]*pmIn2.m[2][2];
	pmOut->m[2][0] = pmIn1.m[2][0]*pmIn2.m[0][0] + pmIn1.m[2][1]*pmIn2.m[1][0] + pmIn1.m[2][2]*pmIn2.m[2][0];
	pmOut->m[2][1] = pmIn1.m[2][0]*pmIn2.m[0][1] + pmIn1.m[2][1]*pmIn2.m[1][1] + pmIn1.m[2][2]*pmIn2.m[2][1];
	pmOut->m[2][2] = pmIn1.m[2][0]*pmIn2.m[0][2] + pmIn1.m[2][1]*pmIn2.m[1][2] + pmIn1.m[2][2]*pmIn2.m[2][2];

	return pmOut;
}

/*	Fun describe: pmOut = pmIn * pvIn
	Para in		: pmIn, pvIn
	Para out	: pvOut	*/
PVECT MMulVector(PVECT pvOut, MAT pmIn, VECT pvIn)
{
	pvOut->i = pmIn.m[0][0]*pvIn.i + pmIn.m[0][1]*pvIn.j + pmIn.m[0][2]*pvIn.k;
	pvOut->j = pmIn.m[1][0]*pvIn.i + pmIn.m[1][1]*pvIn.j + pmIn.m[1][2]*pvIn.k;
	pvOut->k = pmIn.m[2][0]*pvIn.i + pmIn.m[2][1]*pvIn.j + pmIn.m[2][2]*pvIn.k;

	return pvOut;
}

/*	Fun describe: pmOut = 1.0/2 * ( (pmIn T)^-1 + pmIn )
	Para in		: pmIn
	Para out	: pmOut	*/
PMAT MOptOrtho(PMAT pmOut, MAT pmIn)
{
	int i;
	MAT tmp, tmpT, tmpTI;

	MEqual(&tmp, pmIn);
	for(i=0; i<5; i++)
	{
		MTranspose(&tmpT, tmp);//转置
		MInv(&tmpTI, tmpT);	//求逆
		MAdd(&tmp, tmp, tmpTI);
		MMulFactor(&tmp, tmp, 1.0/2);
	}
	return MEqual(pmOut, tmp);
}

/*	Fun describe: convert attitude matrix to attitude quaternion
					q1 = sign(m21-m12) *1/2 *sqrt(1+m00-m11-m22)
					q2 = sign(m02-m20) *1/2 *sqrt(1-m00+m11-m22)
					q3 = sign(m10-m01) *1/2 *sqrt(1-m00-m11+m22)
					q0 =                     sqrt(1-q1^2-q2^-q3^2)
	Para in		: pmIn
	Para out	: pqOut	 */
PQUAT M2AttQuaternion(PQUAT pqOut, MAT pmIn)
{
	double tmp;
	QUAT qtemp;

	tmp = 1.0 + pmIn.m[0][0] - pmIn.m[1][1] - pmIn.m[2][2];
	qtemp.q1 = sqrt(fabs(tmp))/2.0;
	tmp = 1.0 - pmIn.m[0][0] + pmIn.m[1][1] - pmIn.m[2][2];
	qtemp.q2 = sqrt(fabs(tmp))/2.0;
	tmp = 1.0 - pmIn.m[0][0] - pmIn.m[1][1] + pmIn.m[2][2];
	qtemp.q3 = sqrt(fabs(tmp))/2.0;
	//amend on 2012.9.24, this is the way on Pro.yan's
	tmp = 1.0 + pmIn.m[0][0] + pmIn.m[1][1] + pmIn.m[2][2];
	qtemp.q0 = sqrt(fabs(tmp))/2.0;
//	tmp = 1.0 - qtemp.q1*qtemp.q1 - qtemp.q2*qtemp.q2 - qtemp.q3*qtemp.q3;
//	qtemp.q0 = sqrt(fabs(tmp));

	if(pmIn.m[2][1] - pmIn.m[1][2] < 0)	/* sign decision */
		qtemp.q1 = -qtemp.q1;
	if(pmIn.m[0][2] - pmIn.m[2][0] < 0)
		qtemp.q2 = -qtemp.q2;
	if(pmIn.m[1][0] - pmIn.m[0][1] < 0)
		qtemp.q3 = -qtemp.q3;

	return QNormalize(pqOut, qtemp);
}

/*	Fun describe: convert attitude matrix to attitude angle
	Para in		: pmIn
	Para out	: pvOut	(the attitude angle order is [pitch, roll, azimuth]) */
PVECT M2Attitude(PVECT pvOut, MAT pmIn)
{
//*	//rad
	if(pmIn.m[2][1] > 1.0)
        ;//pvOut->i = PI / 2;
    else if(pmIn.m[2][1] < -1.0)
        ;//pvOut->i = -PI / 2;
    else
		pvOut->i = asin(pmIn.m[2][1]);		// pitch: -pi/2 -- pi/2
	pvOut->j = atan2(-pmIn.m[2][0], pmIn.m[2][2]);		// roll: -pi -- pi
	pvOut->k = atan2(-pmIn.m[0][1], pmIn.m[1][1]);	// azimuth: -pi -- pi

//	if(pvOut->k < 0)
//		pvOut->k += 2*PI;
//*/
/*	//mil
//	VECT tmp;

	pvOut->i = asin(pmIn.m[2][1]) * MIL;		// pitch: -pi/2 -- pi/2
	pvOut->j = atan2(-pmIn.m[2][0], pmIn.m[2][2]) * MIL;		// roll: -pi -- pi
	pvOut->k = atan2(pmIn.m[0][1], pmIn.m[1][1]) * MIL;	// azimuth: -pi -- pi

//	VMulFactor(pvOut, tmp, MIL);
*/
	return pvOut;
}

/////////////////////////////////////////////////////////////////
/*********************************************
* 姿态阵单位正交化
**********************************************/
PMAT MStandardization(PMAT pmOut, MAT pmIn)
{
	MAT mtemp;
	int i;

	for(i=0; i<5; i++)
	{
		MTranspose(&mtemp, pmIn);
		MMultiply(&mtemp, pmIn, mtemp);
		MMultiply(&mtemp, mtemp, pmIn);
		MSubtract(&mtemp, mtemp, pmIn);
		MMulFactor(&mtemp, mtemp, 0.5);
		MSubtract(&mtemp, pmIn, mtemp);
		MEqual(&pmIn, mtemp);
	}

	return MEqual(pmOut, mtemp);
}

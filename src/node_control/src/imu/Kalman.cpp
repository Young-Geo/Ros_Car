#include "Kalman.h"
/////////////////////////////////////////////////////
void Equal_KL(double y[VGL], double *x, Uint8 line)
{
  int i;
  
  for(i=0; i<line; i++)
  {
    y[i]= x[i];
  }
}

void Equal_KLKL(double y[VGL][VGL], double *x, Uint8 line)
{
  int i, j;
  
  for(i=0; i<line; i++)
  {
    for(j=0; j<line; j++)
    {
      y[i][j] = x[i*line+j];
    }
  }
}

void Equal_KM(double y[KM], double *x)
{
  int i;
  
  for(i=0; i<KM; i++)
  {
    y[i]= x[i];
  }
}

void Equal_KMKM(double y[KM][KM], double *x)
{
  int i, j;
  
  for(i=0; i<KM; i++)
  {
    for(j=0; j<KM; j++)
    {
      y[i][j] = x[i*KM+j];
    }
  }
}

void Equal_KMKL(double y[KM][VGL], double *x, Uint8 line)
{
  int i, j;
  
  for(i=0; i<KM; i++)
  {
    for(j=0; j<line; j++)
    {
      y[i][j] = x[i*line+j];
    }
  }
}

void Equal_KLKM(double y[VGL][KM], double *x, Uint8 line)
{
  int i, j;
  
  for(i=0; i<line; i++)
  {
    for(j=0; j<KM; j++)
    {
      y[i][j] = x[i*KM+j];
    }
  }
}

////////////////////////////////
PKVECTX KXZero(PKVECTX pkvOut)
{
  int i;
  
  for(i=0; i<KL; i++)
  {
    pkvOut->m[i] = 0;
  }
  return pkvOut;
}

PKMATXX KXXZero(PKMATXX pkmOut)
{
  int i, j;
  for(i=0; i<KL; i++)
  {
    for(j=0; j<KL; j++)
    {
      pkmOut->m[i][j] = 0;
    }
  }
  return pkmOut;
}

PKMATXX KXXEye(PKMATXX pkmOut)
{
  int i, j;
  for(i=0; i<KL; i++)
  {
    for(j=0; j<KL; j++)
    {
      if(i == j)
        pkmOut->m[i][j] = 1;
      else
        pkmOut->m[i][j] = 0;
    }
  }
  return pkmOut;
}

PKMATXX KXXMultiply(PKMATXX pkmOut, KMATXX pkmIn1, KMATXX pkmIn2)
{
  int i, j, k;
  
  for(i=0; i<KL; i++)
  {
    for(j=0; j<KL; j++)
    {
      pkmOut->m[i][j] = pkmIn1.m[i][0] * pkmIn2.m[0][j];
      for(k=1; k<KL; k++)
      {
        pkmOut->m[i][j] += pkmIn1.m[i][k] * pkmIn2.m[k][j];
      }
    }
  }
  return pkmOut;
}

PKMATXX KXXAdd(PKMATXX pkmOut, KMATXX pkmIn1, KMATXX pkmIn2)
{
  int i, j;
  for(i=0; i<KL; i++)
  {
    for(j=0; j<KL; j++)
    {
      pkmOut->m[i][j] = pkmIn1.m[i][j] + pkmIn2.m[i][j];
    }
  }
  return pkmOut;
}

PKMATXX KXXAdd2(PKMATXX pkmOut, KMATXX pkmIn1, KMATXX pkmIn2, KMATXX pkmIn3)
{
  int i, j;
  for(i=0; i<KL; i++)
  {
    for(j=0; j<KL; j++)
    {
      pkmOut->m[i][j] = pkmIn1.m[i][j] + pkmIn2.m[i][j] + pkmIn3.m[i][j];
    }
  }
  return pkmOut;
}

PKMATXX KXXSubtract(PKMATXX pkmOut, KMATXX pkmIn1, KMATXX pkmIn2)
{
  int i, j;
  for(i=0; i<KL; i++)
  {
    for(j=0; j<KL; j++)
    {
      pkmOut->m[i][j] = pkmIn1.m[i][j] - pkmIn2.m[i][j];
    }
  }
  return pkmOut;
}

PKVECTX KXXMulVectX(PKVECTX pkvOut, KMATXX pkmIn, KVECTX pkvIn)
{
  int i, j;
  for(i=0; i<KL; i++)
  {
    pkvOut->m[i] = pkmIn.m[i][0] * pkvIn.m[0];
    for(j=1; j<KL; j++)
    {
      pkvOut->m[i] += pkmIn.m[i][j] * pkvIn.m[j];
    }
  }
  
  return pkvOut;
}

PKVECTX KVectXAdd(PKVECTX pkvOut, KVECTX pkvIn1, KVECTX pkvIn2)
{
  int i;
  for(i=0; i<KL; i++)
  {
    pkvOut->m[i] = pkvIn1.m[i] + pkvIn2.m[i];
  }
  
  return pkvOut;
}

PKMATXX KXXMulFactor(PKMATXX pkmOut, KMATXX pkmIn, double f)
{
  int i, j;
  for(i=0; i<KL; i++)
  {
    for(j=0; j<KL; j++)
    {
      pkmOut->m[i][j] = pkmIn.m[i][j] * f;
    }
  }
  return pkmOut;
}

PKMATXX KXXTranspose(PKMATXX pkmOut, KMATXX pkmIn)
{
  int i, j;
  
  for(i=0; i<KL; i++)
  {
    for(j=0; j<KL; j++)
    {
      pkmOut->m[i][j] = pkmIn.m[j][i];
    }
  }
  
  return pkmOut;
}

//////////////
PKMATXY KYXTranspose(PKMATXY pkmOut, KMATYX pkmIn)
{
  int i, j;
  for(i=0; i<KL; i++)
  {
    for(j=0; j<KM; j++)
    {
      pkmOut->m[i][j] = pkmIn.m[j][i];
    }
  }
  
  return pkmOut;
}

PKMATYX KXYTranspose(PKMATYX pkmOut, KMATXY pkmIn)
{
  int i, j;
  
  for(i=0; i<KM; i++)
  {
    for(j=0; j<KL; j++)
    {
      pkmOut->m[i][j] = pkmIn.m[j][i];
    }
  }
  
  return pkmOut;
}

PKMATYY KYYAdd(PKMATYY pkmOut, KMATYY pkmIn1, KMATYY pkmIn2)
{
  int i, j;
  for(i=0; i<KM; i++)
  {
    for(j=0; j<KM; j++)
    {
      pkmOut->m[i][j] = pkmIn1.m[i][j] + pkmIn2.m[i][j];
    }
  }
  
  return pkmOut;
}

PKMATYY KYY3MInv(PKMATYY pmOut, KMATYY pmIn)
{
  double n;
  
  n =  pmIn.m[0][0] * (pmIn.m[1][1]*pmIn.m[2][2] - pmIn.m[1][2]*pmIn.m[2][1])
		-pmIn.m[1][0] * (pmIn.m[0][1]*pmIn.m[2][2] - pmIn.m[0][2]*pmIn.m[2][1])
		+pmIn.m[2][0] * (pmIn.m[0][1]*pmIn.m[1][2] - pmIn.m[0][2]*pmIn.m[1][1]);
  
  pmOut->m[0][0] =  (pmIn.m[1][1]*pmIn.m[2][2]-pmIn.m[1][2]*pmIn.m[2][1]) / n;
  pmOut->m[1][0] = -(pmIn.m[1][0]*pmIn.m[2][2]-pmIn.m[1][2]*pmIn.m[2][0]) / n;
  pmOut->m[2][0] =  (pmIn.m[1][0]*pmIn.m[2][1]-pmIn.m[1][1]*pmIn.m[2][0]) / n;
  pmOut->m[0][1] = -(pmIn.m[0][1]*pmIn.m[2][2]-pmIn.m[0][2]*pmIn.m[2][1]) / n;
  pmOut->m[1][1] =  (pmIn.m[0][0]*pmIn.m[2][2]-pmIn.m[0][2]*pmIn.m[2][0]) / n;
  pmOut->m[2][1] = -(pmIn.m[0][0]*pmIn.m[2][1]-pmIn.m[0][1]*pmIn.m[2][0]) / n;
  pmOut->m[0][2] =  (pmIn.m[0][1]*pmIn.m[1][2]-pmIn.m[0][2]*pmIn.m[1][1]) / n;
  pmOut->m[1][2] = -(pmIn.m[0][0]*pmIn.m[1][2]-pmIn.m[0][2]*pmIn.m[1][0]) / n;
  pmOut->m[2][2] =  (pmIn.m[0][0]*pmIn.m[1][1]-pmIn.m[0][1]*pmIn.m[1][0]) / n;
  
  return pmOut;
}

PKMATYY KYY2MInv(PKMATYY pmOut, KMATYY pmIn)
{
  double n;
  
  n = pmIn.m[0][0]*pmIn.m[1][1] - pmIn.m[0][1]*pmIn.m[1][0];
  
  pmOut->m[0][0] =  pmIn.m[1][1] / n;
  pmOut->m[0][1] = -pmIn.m[0][1] / n;
  pmOut->m[1][0] = -pmIn.m[1][0] / n;
  pmOut->m[1][1] =  pmIn.m[0][0] / n;
  
  return pmOut;
}

PKVECTY KVectYSub(PKVECTY pkmvOut, KVECTY pkmvIn1, KVECTY pkmvIn2)
{
  int i;
  for(i=0; i<KM; i++)
    pkmvOut->m[i] = pkmvIn1.m[i] - pkmvIn2.m[i];
  
  return pkmvOut;
}

//3,12 * 12,1
PKVECTY KYXMulVectX(PKVECTY pkmvOut, KMATYX pkmmIn, KVECTX pkvIn)
{
  int i, j;
  for(i=0; i<KM; i++)
  {
    pkmvOut->m[i] = pkmmIn.m[i][0] * pkvIn.m[0];
    for(j=1; j<KL; j++)
    {
      pkmvOut->m[i] += pkmmIn.m[i][j] * pkvIn.m[j];
    }
  }
  
  return pkmvOut;
}

//3,12 * 12,12
PKMATYX KYXMulXX(PKMATYX pkmOut, KMATYX pkmIn1, KMATXX pkmIn2)
{
  int i, j, k;
  for(i=0; i<KM; i++)
  {
    for(j=0; j<KL; j++)
    {
      pkmOut->m[i][j] = pkmIn1.m[i][0] * pkmIn2.m[0][j];
      for(k=1; k<KL; k++)
      {
        pkmOut->m[i][j] += pkmIn1.m[i][k] * pkmIn2.m[k][j];
      }
    }
  }
  
  return pkmOut;
}

//12,3 * 3,1
PKVECTX KXYMulVectY(PKVECTX pkvOut, KMATXY pkkmat, KVECTY pkmv)
{
  int i, j;
  for(i=0; i<KL; i++)
  {
    pkvOut->m[i] = pkkmat.m[i][0] * pkmv.m[0];
    for(j=1; j<KM; j++)
    {
      pkvOut->m[i] += pkkmat.m[i][j] * pkmv.m[j];
    }
  }
  
  return pkvOut;
}

//12,3 * 3,3
PKMATXY KXYMulYY(PKMATXY pkmOut, KMATXY pkmIn1, KMATYY pmIn2)
{
  int i, j, k;
  for(i=0; i<KL; i++)
  {
    for(j=0; j<KM; j++)
    {
      pkmOut->m[i][j] = pkmIn1.m[i][0] * pmIn2.m[0][j];
      for(k=1; k<KM; k++)
      {
        pkmOut->m[i][j] += pkmIn1.m[i][k] * pmIn2.m[k][j];
      }
    }
  }
  
  return pkmOut;
}


//12,3 * 3,12
PKMATXX KXYMulYX(PKMATXX pkOut, KMATXY pkmIn1, KMATYX pkmIn2)
{
  int i, j, k;
  
  for(i=0; i<KL; i++)
  {
    for(j=0; j<KL; j++)
    {
      pkOut->m[i][j] = pkmIn1.m[i][0] * pkmIn2.m[0][j];
      for(k=1; k<KM; k++)
      {
        pkOut->m[i][j] += pkmIn1.m[i][k] * pkmIn2.m[k][j];
      }
    }
  }
  
  return pkOut;
}

//12,12 * 12,3
PKMATXY KXXMulXY(PKMATXY pkmOut, KMATXX pkmIn1, KMATXY pkmIn2)
{
  int i, j, k;
  
  for(i=0; i<KL; i++)
  {
    for(j=0; j<KM; j++)
    {
      pkmOut->m[i][j] = pkmIn1.m[i][0] * pkmIn2.m[0][j];
      for(k=1; k<KL; k++)
      {
        pkmOut->m[i][j] += pkmIn1.m[i][k] * pkmIn2.m[k][j];
      }
    }
  }
  
  return pkmOut;
}

//3,12 * 12,3
PKMATYY KYXMulXY(PKMATYY pmOut, KMATYX pkmIn1, KMATXY pkmIn2)
{
  int i, j, k;
  
  for(i=0; i<KM; i++)
  {
    for(j=0; j<KM; j++)
    {
      pmOut->m[i][j] = pkmIn1.m[i][0] * pkmIn2.m[0][j];
      for(k=1; k<KL; k++)
      {
        pmOut->m[i][j] += pkmIn1.m[i][k] * pkmIn2.m[k][j];
      }
    }
  }
  
  return pmOut;
}


//////////////////////////////////////////////////////////////////////
//*
void KMatZero(double *mout, int x, int y)
{
  int i, j;
  
  for(i=0; i<x; i++)
  {
    for(j=0; j<y; j++)
    {
      mout[i * y + j] = 0.0;
    }
  }
}

void KMatEqual(double *mout, double *m, int x, int y)
{
  int i, j;
  
  for(i=0; i<x; i++)
  {
    for(j=0; j<y; j++)
    {
      mout[i * y + j] = m[i * y + j];
    }
  }
}

void KMatAdd(double *mout, double *m1, double *m2, int x, int y)
{
  int i, j;
  
  for(i=0; i<x; i++)
  {
    for(j=0; j<y; j++)
    {
      mout[i * y + j] = m1[i * y + j] + m2[i * y + j];
    }
  }
}

void KMatAdd2(double *mout, double *m1, double *m2, double *m3, int x, int y)
{
  int i, j;
  
  for(i=0; i<x; i++)
  {
    for(j=0; j<y; j++)
    {
      mout[i * y + j] = m1[i * y + j] + m2[i * y + j] + m3[i * y + j];
    }
  }
}

void KMatSub(double *mout, double *m1, double *m2, int x, int y)
{
  int i, j;
  
  for(i=0; i<x; i++)
  {
    for(j=0; j<y; j++)
    {
      mout[i * y + j] = m1[i * y + j] - m2[i * y + j];
    }
  }
}

void KMatMulFactor(double *mout, double *m, int x, int y, double f)
{
  int i, j;
  
  for(i=0; i<x; i++)
  {
    for(j=0; j<y; j++)
    {
      mout[i * y + j] = m[i * y + j] * f;
    }
  }
}

void KMatMulFactorAdd(double *mout, double *m1, double *m2, int x, int y, double f)
{
  int i, j;
  
  for(i=0; i<x; i++)
  {
    for(j=0; j<y; j++)
    {
      mout[i * y + j] = m1[i * y + j] + m2[i * y + j] * f;
    }
  }
}

void KMatMultiply(double *mout, double *m1, double *m2, int x, int y, int z)
{
  int i, j, k;
  
  for(i=0; i<x; i++)
  {
    for(j=0; j<z; j++)
    {
      mout[i * z + j] = 0.0;
      for(k=0; k<y; k++)
      {
        mout[i * z + j] += m1[i * y + k] * m2[k * z + j];
      }
    }
  }
}

void KMatEye(double *mout, int n)
{
  int i, j;
  
  for(i=0; i<n; i++)
  {
    for(j=0; j<n; j++)
    {
      if(i == j)
        mout[i * n + j] = 1;
      else
        mout[i * n + j] = 0;
    }
  }
}

void KMatDiag(double *mout, int n, double f)
{
  int i, j;
  
  for(i=0; i<n; i++)
  {
    for(j=0; j<n; j++)
    {
      if(i == j)
        mout[i * n + j] = f;
      else
        mout[i * n + j] = 0;
    }
  }
}

void KMatTranspose(double *mout, double *m, int x, int y)
{
  int i, j;
  
  for(i=0; i<x; i++)
  {
    for(j=0; j<y; j++)
    {
      mout[j * x + i] = m[i * y + j];
    }
  }
}

int KMatInv(double *a, double *b, int n)
{
  int *is,*js,i,j,k,l,u,v;
  double d,p;
  
  for(i=0; i<n*n; i++)
  {
    a[i] = b[i];
  }
  is=(int*)malloc(n*sizeof(int));
  js=(int*)malloc(n*sizeof(int));
  for (k=0; k<=n-1; k++)
  {
    d=0.0;
    for (i=k; i<=n-1; i++)
    {
      for (j=k; j<=n-1; j++)
      {
        l=i*n+j; p=fabs(a[l]);
        if (p>d)
        {
          d=p;
          is[k]=i;
          js[k]=j;
        }
      }
    }
    if (d+1.0==1.0)
    {
      free(is);
      free(js);
      return(0);
    }
    if (is[k]!=k)
    {
      
      for (j=0; j<=n-1; j++)
      {
        u=k*n+j;
        v=is[k]*n+j;
        p=a[u];
        a[u]=a[v];
        a[v]=p;
      }
    }
    if (js[k]!=k)
    {
      
      for (i=0; i<=n-1; i++)
      {
        u=i*n+k;
        v=i*n+js[k];
        p=a[u];
        a[u]=a[v];
        a[v]=p;
      }
    }
    l=k*n+k;
    a[l]=1.0/a[l];
    for (j=0; j<=n-1; j++)
    {
      
      if (j!=k)
      {
        u=k*n+j;
        a[u]=a[u]*a[l];
      }
    }
    for (i=0; i<=n-1; i++)
    {
      
      if (i!=k)
        for (j=0; j<=n-1; j++)
        {
          
          if (j!=k)
          {
            u=i*n+j;
            a[u]=a[u]-a[i*n+k]*a[k*n+j];
          }
        }
    }
    for (i=0; i<=n-1; i++)
    {
      
      if (i!=k)
      {
        u=i*n+k;
        a[u]=-a[u]*a[l];
      }
    }
  }
  for (k=n-1; k>=0; k--)
  {
    if (js[k]!=k)
    {
      
      for (j=0; j<=n-1; j++)
      {
        u=k*n+j;
        v=js[k]*n+j;
        p=a[u];
        a[u]=a[v];
        a[v]=p;
      }
    }
    if (is[k]!=k)
    {
      for (i=0; i<=n-1; i++)
      {
        u=i*n+k;
        v=i*n+is[k];
        p=a[u];
        a[u]=a[v];
        a[v]=p;
      }
    }
  }
  free(is);
  free(js);
  return(1);
}

///////////////////////////
void SetMat(double *F, MAT mIn, Uint8 line, Uint8 x, Uint8 y)
{
  F[x*line+y] = mIn.m[0][0];
  F[x*line+y+1] = mIn.m[0][1];
  F[x*line+y+2] = mIn.m[0][2];
  
  F[(x+1)*line+y] = mIn.m[1][0];
  F[(x+1)*line+y+1] = mIn.m[1][1];
  F[(x+1)*line+y+2] = mIn.m[1][2];
  
  F[(x+2)*line+y] = mIn.m[2][0];
  F[(x+2)*line+y+1] = mIn.m[2][1];
  F[(x+2)*line+y+2] = mIn.m[2][2];
}
void GetKalmanF(double *F, Uint8 line, MAT Cnb)
{
  F[3*line+3] = 1.0/TAO;
  F[4*line+4] = 1.0/TAO;
  F[5*line+5] = 1.0/TAO;
  SetMat(F, Cnb, line, 3, 0);
}
void GetKalmanQ(double *Q, double *M1, double *F, Uint8 line, double t)
{
  KMatMulFactor(Q, M1, line, line, t);
}

void GetKalmanPhi(double *phi, double *F, Uint8 line, double t)
{
  double *tmp;
  tmp = (double*)malloc(line*line*sizeof(double));
  
  KMatDiag(phi, line, 1);
  KMatMulFactor(tmp, F, line, line, t);
  KMatAdd(phi, phi, tmp, line, line);
  free(tmp);
}

void KFTimeUpdate(double *Xk_1, double *Pk_1, double *phi, double *Q, Uint8 line)
{
  double *xk, *tmp1, *tmp2;
  
  xk = (double*)malloc(line*sizeof(double));
  tmp1 = (double*)malloc(line*line*sizeof(double));
  tmp2 = (double*)malloc(line*line*sizeof(double));
  
  KMatMultiply(xk, phi, Xk_1, line, line, 1);
  KMatEqual(Xk_1, xk, line, 1);
  
  KMatMultiply(tmp1, phi, Pk_1, line, line, line);
  KMatTranspose(tmp2, phi, line, line);
  KMatMultiply(Pk_1, tmp1, tmp2, line, line, line);
  KMatAdd(Pk_1, Pk_1, Q, line, line);
  
  free(xk);
  free(tmp1);
  free(tmp2);
}

void KFMeasureUpdate(double *Xk_1, double *Pk_1, double *Zk, double *H, double *R, Uint8 line)
{
  double *Pzz, *Kk;
  double *tmp, *tmp1, *trans;
  
  tmp = (double*)malloc(line*line*sizeof(double));
  tmp1 = (double*)malloc(line*KM*sizeof(double));
  trans = (double*)malloc(KM*line*sizeof(double));
  Pzz = (double*)malloc(KM*KM*sizeof(double));
  Kk = (double*)malloc(line*KM*sizeof(double));
  
  /////////////////////////////////////////////////////////////
  //Pzz = H*Pk_1*HT + Rk
  KMatTranspose(trans, H, KM, line);
  KMatMultiply(tmp, H, Pk_1, KM, line, line);
  KMatMultiply(tmp1, tmp, trans, KM, line, KM);
  KMatAdd(Pzz, tmp1, R, KM, KM);
  
  //Pxz = Pk_1*HT;
  KMatMultiply(tmp, Pk_1, trans, line, line, KM);
  
  //Kk = Pxz*Pzz^-1
  KMatInv(tmp1, Pzz, KM);
  KMatMultiply(Kk, tmp, tmp1, line, KM, KM);
  
  //Xk = Xk_1 + Kk(Zk - H*Xk_1)
  KMatMultiply(tmp1, H, Xk_1, KM, line, 1);
  KMatSub(tmp1, Zk, tmp1, KM, 1);
  KMatMultiply(tmp, Kk, tmp1, line, KM, 1);
  KMatAdd(Xk_1, Xk_1, tmp, line, 1);
  
  //Pk = Pk_1 - Kk*Pzz*KkT
  KMatTranspose(trans, Kk, line, KM);
  KMatMultiply(tmp1, Kk, Pzz, line, KM, KM);
  KMatMultiply(tmp, tmp1, trans, line, KM, line);
  KMatSub(Pk_1, Pk_1, tmp, line, line);
  
  free(tmp);
  free(tmp1);
  free(trans);
  free(Pzz);
  free(Kk);
}

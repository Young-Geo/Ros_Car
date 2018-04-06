#ifndef _SVDCMP_H_
#define _SVDCMP_H_

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

double **dmatrix(int nrl, int nrh, int ncl, int nch) ;
double *dvector(int nl, int nh) ;
void free_dvector(double *v, int nl, int nh) ;
void svdcmp(double **a, int m, int n, double w[], double **v) ;


#endif
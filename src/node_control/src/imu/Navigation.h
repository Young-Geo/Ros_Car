#ifndef _NAVIGATION_H_
#define _NAVIGATION_H_

#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "Vector.h"
#include "Matrix.h"
#include "Quaternion.h"
#include "Kalman.h"
#include "svdcmp.h"

#define PI			3.1415926535897932384626433832795
#define G0			9.80
#define MG			(1.0e-3*G0)
#define SAMPLES	1
#define TS			0.01
#define Tm			(SAMPLES*TS)
#define PN      60
#define FN      30

#define PPDIS		(0.2199/497.887) //每个脉冲代表的距离 = 周长 / 轮子一圈脉冲数
#define Todo		0.02 //频率：每20ms一次
#define TR			0.24245//轴距

#define ODO_GY_DELTA_X 0.0
#define ODO_GY_DELTA_Y 0.0
#define ODO_GY_DELTA_Z 0.0
#define ODO_GY_THETA 0.0
#define ODO_GY_PHI 0.0

#define HISTORY_MAX 128
#define HISTORY_SVD_ATT 10
#define HISTORY_MSINS 30
#define HISTORY_MSINS_RUMP 100

#define SVD_N 3
#define SVD_M 10
#define SVD_NM 10

typedef struct _IMUK_T
{
  int timestamp;//时间戳
  VECT dw; //角速度
  VECT dv; //加速度
  int vl;//左轮脉冲
  int vr;//右轮脉冲
} IMUK_T ;

typedef struct
{
  double att[3];
  double eb[3];
} KARMANPARA;

typedef union
{
  KARMANPARA kp;
  double db[VGL];
} KPFRAME ;

typedef struct _VG_T
{
  double Xk[VGL];
  double F[VGL*VGL];
  double H[KM*VGL];
  double Pk[VGL*VGL];
  double R[KM*KM] ;
  double Q[VGL*VGL];
  double Q0[VGL*VGL];
} VG_T ;

typedef struct _HISTORY_T
{
  IMUK_T imuk ;
  QUAT qnb ;
  VECT att ;
  VECT pos ;
  VECT velocity ;
} HISTORY_T ;

typedef struct _NAV_t
{
  IMUK_T imuk ;//陀螺加计里程计输入数据
  VG_T vg ; //陀螺加计融合卡尔曼算法上下文。
  QUAT qnb ;//位姿4元数
  VECT att ;//位姿欧拉角
  VECT eb  ;//陀螺零偏
  VECT pos ;//位置
  VECT velocity ;//行走速度
  VECT svd ;//奇导值
  
  HISTORY_T history[HISTORY_MAX] ;//历史数据
  int history_idx , history_backtracking_idx ;
  
  int eb_len ;//陀螺零偏采集帧数
  int svd_work_len ;//可正常进入svd检测的帧数
  int cnt ; //总运行次数
  int msins ;//是否处于纯惯导模式
  int msins_rump ;//纯惯导模式完成后，需要限制一段时间内不可再进入纯惯导模式
} NAV_T ;

void NavInit(NAV_T * nav) ;
void NavUpdate ( NAV_T * nav , IMUK_T * imuk ) ;

void NavSVD (NAV_T * nav, VECT * svd) ;
int NavSVDIsCrushed(VECT * svd);

void NavHistoryPush (NAV_T * nav) ;
void NavHistoryPop (NAV_T * nav , int idx ) ;
int NavHistoryPass (NAV_T * nav, int pass_count) ;
#define NavHistoryInc(INC) (INC = INC + 1 >= HISTORY_MAX ? 0 : INC + 1)

void NavInitMSINS(NAV_T * nav, int pass, int whole, int rump) ;
int NavUpdateMSINS(NAV_T * nav) ;
             
void InitVG(VG_T * vg) ;
void UpdateVG(VG_T * vg, IMUK_T * imuk , QUAT * qnb, VECT * att) ;
void UpdatePos(VECT * velocity, VECT * pos, QUAT * qnb, IMUK_T * imuk) ;

#endif

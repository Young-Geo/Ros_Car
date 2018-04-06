#include "Navigation.h"
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
void NavHistoryPush (NAV_T * nav)
{
  HISTORY_T * h ;
  
  h = &nav ->history[nav ->history_idx] ;
  h ->imuk = nav ->imuk ;
  h ->qnb = nav ->qnb ;
  h ->att = nav ->att ;
  h ->pos = nav ->pos ;
  h ->velocity = nav ->velocity ;
  
  NavHistoryInc(nav ->history_idx) ;
}
void NavHistoryPop (NAV_T * nav , int idx )
{
  HISTORY_T * h ;
  
  h = &nav ->history[idx] ;
  nav ->imuk = h ->imuk ;
  nav ->qnb = h ->qnb ;
  nav ->att = h ->att ;
  nav ->pos = h ->pos ;
  nav ->velocity = h ->velocity ;
}
int NavHistoryPass (NAV_T * nav, int pass_count)
{
  int i ;
  
  i = nav ->history_idx - pass_count ;
  if ( i < 0 )
  {
    i += HISTORY_MAX ;
  }
  return i ;
}
void NavSVD (NAV_T * nav, VECT * svd)
{
  int i , j , t ;
  
  static double **a = NULL ;
  static double *w  = NULL ;
  static double **v = NULL ;
  
  if ( !a )
  {
    //申请空间
    a = dmatrix(1,SVD_NM,1,SVD_NM);
    w = dvector(1,SVD_NM);
    v = dmatrix(1,SVD_NM,1,SVD_NM);
  }
  
  //将矩阵各向量都置零
  for ( i = 1 ; i <= SVD_NM ; i ++ )
  {
    for ( j = 1 ; j <= SVD_NM ; j ++ )
    {
      a[i][j] = 0 ;
      v[i][j] = 0 ;
    }
    w[i] = 0 ;
  }
  
  //得到过去10帧的位姿，赋值给a
  j = NavHistoryPass(nav, 10) ;
  for ( i = 1 ; i <= 10 ; i ++ )
  {
    a[i][1] = nav ->history[j].att.i ;
    a[i][2] = nav ->history[j].att.j ;
    a[i][3] = nav ->history[j].att.k ;
    //printf("svd: a[%2d][1]:%lf a[%2d][2]:%f a[%2d][3]:%f\n",i,a[i][1],i,a[i][2],i,a[i][3]);
    NavHistoryInc(j);
  }
  
  //进行奇异值计算
  svdcmp(a, SVD_NM, SVD_NM, w, v) ;
  
  //将计算结果使用一个冒泡排序，按大小进行排序
  for( i = 1 ; i <= SVD_N ; i ++ )
  {
    for( j = i + 1 ; j <= SVD_N ; j ++ )
    {
      if(w[i] < w[j])
      {
        t = w[i];
        w[i] = w[j];
        w[j] = t;
      }
    }
  }
  
  //将结果输出到返回值
  svd ->i = w[1];
  svd ->j = w[2];
  svd ->k = w[3];
  //printf("svd result: %lf %lf %lf\n",w[1],w[2],w[3]);
}
int NavSVDIsCrushed(VECT * svd)
{
  if((svd ->j * 500.0 > 600.0) && (svd ->k * 500.0 > 390.0))
  {
    return 1 ;
  }
  return 0 ;
}
PVECT VelocityUpdate(PVECT vnm, VECT vnm_1, QUAT qnb, VECT vsfm)
{
  VECT vtemp,vtemp1;
  VECT gn;
  
  gn.i = 0;
  gn.j = 0;
  gn.k = -G0;
  
  QMulVector(&vtemp, qnb, vsfm);
  VMulFactor(&vtemp1, gn, Tm);
  VAdd2(vnm, vnm_1, vtemp, vtemp1);
  return vnm;
}
PVECT NavSn(PVECT sn, VECT vnm, VECT vnm_1)
{
  VECT vtemp;
  VAdd(&vtemp,vnm,vnm_1);
  VMulFactor(sn,vtemp,0.5*Tm);
  return sn;
}
void MSINS(QUAT * qnb , VECT * att , VECT * velocity , VECT * pos , IMUK_T * imuk)
{
  VECT sn;
  
  VEqual(&sn, *velocity);
  
  VelocityUpdate(velocity, *velocity,*qnb, imuk ->dv);//纯加计更新速度
  velocity ->k = 0.0; //因为是平面运动，所以把z轴速度置0.
  
  NavSn(&sn,sn,*velocity);//用速度计算位移的delta量
  VAdd(pos, *pos, sn) ; //更新位置
  
  QMulRVector(qnb,  *qnb, imuk ->dw);//纯陀螺姿态更新
  QNormalize(qnb, *qnb); //四元数归一化
  Q2Attitude(att, *qnb) ;//将4元数转为欧拉角
  VMulFactor(att, *att, 180.0/PI) ;//转成角度制
}
void NavInitMSINS(NAV_T * nav, int pass, int whole, int rump)
{
  int s , e , i ;
  VECT velocity ;
  
  //计算回溯位置，并将当时的状态pop出来
  nav ->history_backtracking_idx = NavHistoryPass(nav, pass) ;
  NavHistoryPop(nav, nav ->history_backtracking_idx) ;
  
  //计算出当时的速度：需要差值计算，把往前5帧，往后4帧，及当前帧共10帧，进行均值计算.
  assert(pass >= 4) ;
  s = NavHistoryPass(nav, pass + 5) ;
  e = NavHistoryPass(nav, pass - 4) ;
  
  VZero(&velocity) ;
  for ( i = s ; i <= e ; )
  {
    VAdd(&velocity, velocity, nav ->history[i].velocity) ;
    NavHistoryInc(i) ;
  }
  VMulFactor(&nav ->velocity, velocity, 0.1) ;
  printf("MSINS init velocity %lf %lf %lf\n" , \
         nav ->velocity.i , \
         nav ->velocity.j , \
         nav ->velocity.k ) ;
  
  //总共需要进行MSINS解算多少次
  nav ->msins = whole ;
  
  //残余多少次
  nav ->msins_rump = rump ;
}
int NavUpdateMSINS(NAV_T * nav)
{
  HISTORY_T * h ;
  
  assert(nav ->msins > 0) ;
  
  h = &nav ->history[nav ->history_backtracking_idx] ;
  nav ->imuk = h ->imuk ;
  
  MSINS(&nav ->qnb, &nav ->att, &nav ->velocity, &nav ->pos, &nav ->imuk) ;
  NavHistoryInc(nav ->history_backtracking_idx) ;
  
  nav ->msins -- ;
  return ((nav ->msins > 0) ? 0 : 1) ;
}
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
static void InitDiag(double *mout, double *m_in, Uint8 size)
{
  int i, j;
  
  for(i=0; i<size; i++)
  {
    for(j=0; j<size; j++)
    {
      if(i == j)
      {
        mout[i * size + j] = m_in[i];
      }
      else
      {
        mout[i * size + j] = 0.0;
      }
    }
  }
}
void InitVG(VG_T * vg)
{
  Uint8 i ;
  KPFRAME kpFrm ;
  
  for(i=0; i<VGL; i++)
  {
    vg ->Xk[i] = 0;
  }
  KMatZero(vg ->F, VGL, VGL);
  KMatZero(vg ->H, KM, VGL);
  
  //P
  kpFrm.kp.att[0] = pow(5.0*PI/180.0, 2);
  kpFrm.kp.att[1] = pow(5.0*PI/180.0, 2);
  kpFrm.kp.att[2] = pow(5.0*PI/180.0, 2);
  kpFrm.kp.eb[0] = pow(60.0*PI/180.0/3600.0, 2);
  kpFrm.kp.eb[1] = pow(60.0*PI/180.0/3600.0, 2);
  kpFrm.kp.eb[2] = pow(60.0*PI/180.0/3600.0, 2);
  InitDiag(vg ->Pk, kpFrm.db, VGL);

  //Q
  kpFrm.kp.att[0] = pow(0.2*PI/180.0/60.0, 2);
  kpFrm.kp.att[1] = pow(0.2*PI/180.0/60.0, 2);
  kpFrm.kp.att[2] = pow(0.2*PI/180.0/60.0, 2);
  kpFrm.kp.eb[0] = pow(0.2*PI/180.0/3600.0, 2);
  kpFrm.kp.eb[1] = pow(0.2*PI/180.0/3600.0, 2);
  kpFrm.kp.eb[2] = pow(0.2*PI/180.0/3600.0, 2);
  InitDiag(vg ->Q0, kpFrm.db, VGL);
  
  vg ->H[1] = -G0;
  vg ->H[KL] = G0;
  
  //R
  kpFrm.kp.att[0] = pow(0.4*MG, 2);
  kpFrm.kp.att[1] = pow(0.4*MG, 2);
  kpFrm.kp.att[2] = pow(0.4*MG, 2);
  InitDiag(vg ->R, kpFrm.db, KM);
}
////////////////////////////////////////////////////////////////////////
void UpdateVG(VG_T * vg, IMUK_T * imuk , QUAT * qnb, VECT * att)
{
  MAT  Cnb;
  VECT phim, vsfm;
  VECT gn;
  VECT vTemp;
  
  double Zk[KM],  phi[VGL*VGL];
  
  gn.i = 0;
  gn.j = 0;
  gn.k = -G0;
  
  phim.i = imuk ->dw.i;
  phim.j = imuk ->dw.j;
  phim.k = imuk ->dw.k;
  
  vsfm.i = imuk ->dv.i;
  vsfm.j = imuk ->dv.j;
  vsfm.k = imuk ->dv.k;
  
  ////////////KF
  memset(vg ->Xk, 0, sizeof(vg ->Xk)) ;
  
  QMulRVector(qnb,  *qnb, phim);//纯陀螺姿态更新
  QMulVector(&vsfm, *qnb, vsfm);
  VMulFactor(&vsfm, vsfm, 1/Tm);//fnsf
  VAdd(&vTemp,vsfm,gn);
  
  Zk[0] = vTemp.i;
  Zk[1] = vTemp.j;
  Zk[2] = vTemp.k;
  
  Q2AttMatrix(&Cnb, *qnb);
  
  GetKalmanF(vg ->F, VGL, Cnb);
  GetKalmanQ(vg ->Q, vg ->Q0, vg ->F, VGL, Tm);
  GetKalmanPhi(phi, vg ->F, VGL, Tm);
  
  KFTimeUpdate(vg ->Xk, vg ->Pk, phi, vg ->Q, VGL);
  KFMeasureUpdate(vg ->Xk, vg ->Pk, Zk, vg ->H, vg ->R, VGL);
  
  phim.i = vg ->Xk[0];
  phim.j = vg ->Xk[1];
  phim.k = vg ->Xk[2];
  
  if(VNorm(imuk ->dv)<1.2)//1.3
  {
    imuk ->dv.k = 0;
    if(VNorm(imuk ->dv)<0.6)//0.7
    {
      QDelPhi(qnb, phim, *qnb);//加计修正姿态
    }
  }
  Q2Attitude(att, *qnb) ;//将4元数转成欧拉角
  VMulFactor(att, *att, 180.0/PI) ;//转成角度制
}
////////////////////////////////////////////////////////////////////////
PVECT DRSn(PVECT sn, double milemeter, QUAT qnb, IMUK_T * imuk)
{
  double scm = milemeter * PPDIS ;
  
  VECT dSb, vtmp, vtmp1;
  double meliPhi , meliTheta;
  
  //计算里程计中心相对于陀螺仪的安装角度
  meliTheta = ODO_GY_THETA ;
  meliPhi = ODO_GY_PHI ;
  
  dSb.i = scm * cos(meliTheta) * sin(meliPhi);
  dSb.j = scm * cos(meliTheta) * cos(meliPhi);
  dSb.k = scm * sin(meliTheta);

  //计算里程计中心相对于陀螺仪的安装位置
  vtmp1.i = ODO_GY_DELTA_X ;
  vtmp1.j = ODO_GY_DELTA_Y ;
  vtmp1.k = ODO_GY_DELTA_Z ;
  
  VMulCross(&vtmp, imuk ->dw, vtmp1);
  VMulFactor(&vtmp1, vtmp, -1.0);
  VAdd(&vtmp, vtmp, vtmp1);
  
  //这里是不是还要加入对vtmp的使用代码？
  
  //计算SN
  QMulVector(sn, qnb, dSb);
  return sn;
}
void UpdatePos(VECT * velocity, VECT * pos, QUAT * qnb, IMUK_T * imuk)
{
  VECT sn ;
  double milemeter ;
  
  milemeter = (imuk ->vl + imuk ->vr) / 2.0;
  
  DRSn(&sn, milemeter, *qnb, imuk);//得到位置增量
  VMulFactor(velocity, sn, 1 / Tm);//计算速度
  
  //位置更新
  VAdd(pos, *pos, sn) ;
}
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
void NavInit(NAV_T * nav)
{
  memset(nav, 0, sizeof(NAV_T)) ;
  InitVG(&nav ->vg) ;
  QInitialize(&nav ->qnb, 1, 0, 0, 0) ;
  
  nav ->eb_len = 1200 ;
  nav ->svd_work_len = nav ->eb_len + 256 ;
}

void NavUpdate ( NAV_T * nav , IMUK_T * imuk )
{
  nav ->imuk = *imuk ;
  
#if 0
  printf("input data : %lf %lf %lf %lf %lf %lf %d %d %d\n",\
  nav ->imuk.dw.i,\
  nav ->imuk.dw.j,\
  nav ->imuk.dw.k,\
  nav ->imuk.dv.i,\
  nav ->imuk.dv.j,\
  nav ->imuk.dv.k,\
  nav ->imuk.vl,\
  nav ->imuk.vr,\
  nav ->imuk.timestamp);
#endif
  
  if( abs(nav ->imuk.vl) > 1024 )
  {
    nav ->imuk.vl = 0 ;
    nav ->imuk.vr = 0 ;
  }
  if( abs(nav ->imuk.vr) > 1024 )
  {
    nav ->imuk.vr = 0 ;
    nav ->imuk.vl = 0 ;
  }
  
  nav ->cnt ++ ;
  if ( nav ->cnt < nav ->eb_len )
  {
    VAdd(&nav ->eb,nav ->eb,nav ->imuk.dw);
    if(nav ->cnt % 100 == 0)
    {
      printf("System Init Plase Waiting...%d\n", 12 - nav ->cnt / 100);
    }
    return ;
  }
  
  if ( nav ->cnt == nav ->eb_len )
  {
    //计算零偏
    VAdd(&nav ->eb,nav ->eb,nav ->imuk.dw);
    VMulFactor(&nav ->eb,nav ->eb,-1.0 / nav ->cnt);
    printf("System Init  Result %e %e %e\n", nav ->eb.i , nav ->eb.j , nav ->eb.k) ;
  }
  //扣除零偏
  VAdd(&(nav ->imuk.dw),nav ->imuk.dw,nav ->eb);
  
  //判断当前是否是纯惯导模式
  if ( nav ->msins > 0 )
  {
    //纯惯导模式
    int i ;
    
    //在纯惯导模式下，也需要记录imuk的实时数据.
    NavHistoryPush(nav) ;
    
    //每次进行3次纯惯导运算，可用100ms追上实时更新的数据.
    //即，从开始纯惯导，到纯惯导结束，共用时100ms，10个周期。这10个周期内又产生了10组新数据，由于每周期解算3次，那这10个周期内解算了
    //30组数据，刚好解算完成。
    for ( i = 0 ; i < 3 ; i ++ )
    {
      printf("MSINS ing...%d\n" , nav ->msins) ;
      if ( NavUpdateMSINS(nav) )
      {
        printf("MSINS finished!\n") ;
        break ;
      }
    }
  }
  //正常模式
  else
  {
    //更新位姿
    UpdateVG(&nav ->vg, &nav ->imuk, &nav ->qnb, &nav ->att) ;
    
    //通过里程计进行位置推算
    UpdatePos(&nav ->velocity, &nav ->pos, &nav ->qnb, &nav ->imuk) ;
    
    //将数据存储于到历史数据区中，用作纯惯导检测、回溯、曲率计算、打滑锁定
    NavHistoryPush(nav) ;
    
    //在残余阶段，不能进入纯惯导模式
    if ( nav ->msins_rump > 0 )
    {
      nav ->msins_rump -- ;
    }
    if ( (nav ->cnt > nav ->svd_work_len) && (nav ->msins_rump <= 0) )
    {
      //进行奇异值计算
      NavSVD(nav, &nav ->svd) ;
      if ( NavSVDIsCrushed(&nav ->svd) )
      {
        //位姿有比较大的变化，说明刚体在进行比较剧烈的运动，这个时候要进入纯惯导。
        //纯惯导有效性时间是300MS，所以回溯20帧，之后每秒算3帧数据，恰好10个周期算守30组数据（20个历史数据+新产生的10帧数据）实时数据。

        printf("MSINS enable!\n") ;
        NavInitMSINS(nav, HISTORY_MSINS - 10, HISTORY_MSINS, HISTORY_MSINS_RUMP) ;
      }
    }
  }
  
  if ( nav ->cnt % 300 == 0 )
  {
    printf("odo: [%d %d] att: [%lf %lf %lf] pos: [%lf %lf %lf] \n\t  - svd: <%lf %lf %lf> velocity: <%lf %lf %lf>\n",\
           nav ->imuk.vl,\
           nav ->imuk.vr,\
           nav ->att.i,\
           nav ->att.j,\
           nav ->att.k,\
           nav ->pos.i,\
           nav ->pos.j,\
           nav ->pos.k,\
           nav ->svd.i,\
           nav ->svd.j,\
           nav ->svd.k,\
           nav ->velocity.i,\
           nav ->velocity.j,\
           nav ->velocity.k);
  }
}
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

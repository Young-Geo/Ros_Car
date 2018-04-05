/*
 *  Created on: 2017-12-26
 *      Author: Young.Geo
 *          QQ: 473763733
 *       Email: anxan524@126.com
 */

#ifndef __SOLUTION_H__
#define __SOLUTION_H__

#include "afx.h"
#include "xlog.h"
#include "xsocket.h"
#include "xchain.h"
#include "xlist.h"
#include "pkt.h"
#include "xserial.h"

#define IMUPACKSIZE 11


typedef struct _JY901_t
{
    unsigned char datatime[11];//datatime
    unsigned char acceleration[11];//加速度
    unsigned char angularvelocity[11];//角速度
    unsigned char angle[11];//角度
    unsigned char magnetic [11];//磁场
    unsigned char portstatus[11];//端口状态
    unsigned char pressureheight[11];//气压和高度
    unsigned char latitudelongitude[11];//经纬度
    unsigned char groundspeed[11];//地速
    unsigned char fourelements[11];//四元素
    unsigned char satellitepositioning[11];//卫星定位
    //int isStart;
} JY901_t;

int     serial_par(int fd, xchain *chain, xlist *list);
int     serial_data(int fd, char *buf, int len);


#define DERIVCE_IMU_NAME "/dev/imu"

typedef struct _imu_t
{
    int serial_fd;
    int isStart;
    xchain chain;
    xlist *lists;
} imu_t;

//void    CopeSerialData(char ucData[],unsigned short usLength);
imu_t * imu_init();
int     imu_data_processing(imu_t *imu);
int     imu_destory(imu_t *imu);
int     make_imu(imu_t *imu);









#endif

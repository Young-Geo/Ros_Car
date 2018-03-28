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

/*
struct STime
{
    unsigned char ucYear;
    unsigned char ucMonth;
    unsigned char ucDay;
    unsigned char ucHour;
    unsigned char ucMinute;
    unsigned char ucSecond;
    unsigned short usMiliSecond;
};
struct SAcc
{
    short a[3];
    short T;
};
struct SGyro
{
    short w[3];
    short T;
};
struct SAngle
{
    short Angle[3];
    short T;
};
struct SMag
{
    short h[3];
    short T;
};

struct SDStatus
{
    short sDStatus[4];
};

struct SPress
{
    long lPressure;
    long lAltitude;
};

struct SLonLat
{
    long lLon;
    long lLat;
};

struct SGPSV
{
    short sGPSHeight;
    short sGPSYaw;
    long lGPSVelocity;
};

typedef struct _JY901_t
{
    struct STime		stcTime;
    struct SAcc 		stcAcc;
    struct SGyro 		stcGyro;
    struct SAngle 		stcAngle;
    struct SMag 		stcMag;
    struct SDStatus 	stcDStatus;
    struct SPress 		stcPress;
    struct SLonLat 		stcLonLat;
    struct SGPSV 		stcGPSV;
} JY901_t;

extern JY901_t jy901;
*/

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
    int isStart;
} JY901_t;

//void    CopeSerialData(char ucData[],unsigned short usLength);
int     serial_par(JY901_t *jy901, int fd);
int     serial_data(int fd);









#endif

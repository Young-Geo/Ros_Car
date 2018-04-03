/*
 *  Created on: 2017-12-26
 *      Author: Young.Geo
 *          QQ: 473763733
 *       Email: anxan524@126.com
 */

#ifndef __LADAR_H__
#define __LADAR_H__

#include "afx.h"
#include "xlog.h"
#include "xserial.h"
#include "xchain.h"
#include "xlist.h"

#define DERIVCE_NAME "/dev/ttyUSB0"
#define LADARPACKSIZE 12

typedef struct _point_cloud_data_t
{
    unsigned char data_type;
    unsigned char data_len;
    unsigned short start_position, end_position;
    unsigned short data;
} point_cloud_data_t;


typedef struct _ladar_t
{
    int fd;
    int isStart, isSendStart;
    xchain chain;
    xlist *datas;
} ladar_t;

int     ladar_init(ladar_t *ladar);

int     ladar_start(ladar_t *ladar);
int     ladar_stop(ladar_t *ladar);

int     ladar_analytic_data(ladar_t *ladar);


#endif // __LADAR_H__

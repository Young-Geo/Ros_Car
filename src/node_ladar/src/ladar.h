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

#define DERIVCE_NAME "/dev/ttyUSB1"

typedef struct _ladar_t
{
    int fd;
    int isStart;
} ladar_t;

int     ladar_init(ladar_t *ladar);

int     ladar_start(ladar_t *ladar);
int     ladar_stop(ladar_t *ladar);


#endif // __LADAR_H__

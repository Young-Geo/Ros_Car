/*
 *  Created on: 2017-12-26
 *      Author: Young.Geo
 *          QQ: 473763733
 *       Email: anxan524@126.com
 */

#ifndef __MOTION_H__
#define __MOTION_H__

#include "afx.h"
#include "xserial.h"
#include "xlog.h"


#define DERIVCE_NAME "/dev/ttyUSB1"

#define A 100
#define B 120


typedef struct _motion_t
{
    int fd;
    int isStart;
} motion_t;


int     motion_init(motion_t *motion);

int     move(motion_t *motion, int wheelone, int wheeltwo, int wheelthree, int wheelfour);

int     movexyz(motion_t *motion, int x, int y, int z);

int     make_move(int wheelone, int wheeltwo, int wheelthree, int wheelfour, char *out_buf, int len);



#endif

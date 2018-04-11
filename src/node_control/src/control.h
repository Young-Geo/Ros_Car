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



#define DERIVCE_CHASSIS_NAME "/dev/motion"

#define A 160.5
#define B 160


typedef struct _control_t
{
    int fd;
    int isStart;
} control_t;


control_t *     control_init();
int     control_destory(control_t *control);

int     control_data_processing(control_t *control);

int     move(control_t *control, int wheelone, int wheeltwo, int wheelthree, int wheelfour);

int     movexyz(control_t *control, double x, double y, double z);

int     make_move(int wheelone, int wheeltwo, int wheelthree, int wheelfour, char *out_buf, int len);

int     stop_move(control_t *control);


#endif

/*
 *  Created on: 2017-12-26
 *      Author: Young.Geo
 *          QQ: 473763733
 *       Email: anxan524@126.com
 */

#ifndef __MOTION_H__
#define __MOTION_H__


#include <ros/ros.h>

#include "afx.h"
#include "xserial.h"
#include "xlog.h"
#include "pkt.h"
#include "xlist.h"
#include "xchain.h"
#include "pose.h"



#define DERIVCE_CHASSIS_NAME "/dev/motion"

#define MOTION2SHANGPACK 12
#define SHANG2MOTIONPACK 8


typedef struct _control_t
{
    int fd;
    int isStart;
    xchain chain;
    xlist *list;
    pose_t pose;
} control_t;


control_t *     control_init();
int     control_destory(control_t *control);

int     control_data_processing(control_t *control, ros::Publisher &pub);

int     move(control_t *control, int wheelone, int wheeltwo, int wheelthree, int wheelfour);

int     movexyz(control_t *control, double x, double y, double z);

int     make_move(int wheelone, int wheeltwo, int wheelthree, int wheelfour, char *out_buf, int len);

int     stop_move(control_t *control);


#endif

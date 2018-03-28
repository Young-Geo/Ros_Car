/*
 *  Created on: 2017-12-26
 *      Author: Young.Geo
 *          QQ: 473763733
 *       Email: anxan524@126.com
 */

#ifndef __NODE_IMU_H__
#define __NODE_IMU_H__

#include <ros/ros.h>

#include "afx.h"
#include "xlog.h"
#include "xsocket.h"
#include "xchain.h"
#include "solution.h"

typedef struct _imu_t
{
    int serial_fd;
    JY901_t jy901;
} imu_t;

int node_imu_main(ros::NodeHandle &n);


#endif

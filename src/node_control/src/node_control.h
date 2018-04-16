/*
 *  Created on: 2017-12-26
 *      Author: Young.Geo
 *          QQ: 473763733
 *       Email: anxan524@126.com
 */

#ifndef __NODE_MOTION_H__
#define __NODE_MOTION_H__

#include <ros/ros.h>
#include <tf/transform_broadcaster.h>
#include <nav_msgs/Odometry.h>
#include <geometry_msgs/Twist.h>
#include <nav_msgs/Odometry.h>


#include "control.h"
#include "imu.h"
#include "Ytimmer.h"

typedef struct _pub_t
{
    ros::Publisher odom_pub;
} pub_t;

typedef struct _control_imu_t
{
   control_t *control;
   imu_t *imu;
   timmer_t *timmer;
   //NAV_T nav;
   pub_t pub;
} control_imu_t;


int     node_control_main(ros::NodeHandle &n);

#endif // __NODE_MOTION_H__

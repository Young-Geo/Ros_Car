/*
 *  Created on: 2017-12-26
 *      Author: Young.Geo
 *          QQ: 473763733
 *       Email: anxan524@126.com
 */
#ifndef __POSE_H__
#define __POSE_H__

#include <ros/ros.h>
#include <tf/transform_broadcaster.h>
#include <nav_msgs/Odometry.h>
#include <geometry_msgs/Twist.h>
#include <nav_msgs/Odometry.h>
#include <tf/transform_listener.h>
#include <tf/tf.h>

#include "global.h"
#include "xlist.h"


typedef struct _add_t
{
    int x, y, w_z;
} add_t __attribute__((packed));

typedef struct _motion_t
{
    double one, two, three, four;
    double x, y, w_z;
} motion_t __attribute__((packed));

typedef struct _pose_t
{
    double x,y,z;//tf
    double w_x, w_y, w_z;
    xlist *add_list;
    motion_t motion;
} pose_t __attribute__((packed));

void    pose_init(pose_t *pose);

int     pose_calculation(ros::Publisher &pub, xlist *list, pose_t *pose);

int     motion_calculation(int *buf, motion_t *motion);

void    pose_destory(pose_t *pose);











#endif // __POSE_H__

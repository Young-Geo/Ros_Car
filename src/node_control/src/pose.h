/*
 *  Created on: 2017-12-26
 *      Author: Young.Geo
 *          QQ: 473763733
 *       Email: anxan524@126.com
 */
#ifndef __POSE_H__
#define __POSE_H__

#include "global.h"
#include "xlist.h"

typedef struct _motion_t
{
    double one, two, three, four;
    double x, y, w_z;
} motion_t __attribute__((packed));

typedef struct _pose_t
{
    int x,y,z;//tf
    int w_x, w_y, w_z;
    motion_t motion;
} pose_t __attribute__((packed));


int     pose_calculation(xlist *list, pose_t *pose);
int     motion_calculation(int *buf, motion_t *motion);












#endif // __POSE_H__

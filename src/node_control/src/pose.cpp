/*
 *  Created on: 2017-12-26
 *      Author: Young.Geo
 *          QQ: 473763733
 *       Email: anxan524@126.com
 */

#include "pose.h"


int     pose_calculation(xlist *list, pose_t *pose)
{
    int *buf = { 0 };
    xlist *start = NULL;

    xassert(list);
    xassert(pose);


    for (start = list; start && start->next; start = start->next)
    {
        if (!(buf = (int *)start->value))
            continue;

        motion_calculation(buf, &pose->motion);
        pose->x = pose->motion.x * MOTIONTIMER;
        pose->y = pose->motion.y * MOTIONTIMER;
        pose->w_z = pose->motion.w_z * MOTIONTIMER;
    }

    xlist_reset(list);
    return 0;
}

int     motion_calculation(int *buf, motion_t *motion)
{
    xassert(buf);
    xassert(motion);

    motion->one = ((MECANUMPERIMETER / MAICHONGNUM) * buf[0]) / MOTIONTIMER;
    motion->two = ((MECANUMPERIMETER / MAICHONGNUM) * buf[1]) / MOTIONTIMER;
    motion->three = ((MECANUMPERIMETER / MAICHONGNUM) * buf[2]) / MOTIONTIMER;
    motion->four = ((MECANUMPERIMETER / MAICHONGNUM) * buf[3]) / MOTIONTIMER;

    motion->x = (motion->one + motion->two + motion->three + motion->four) / 4;
    motion->y = (motion->one + motion->three - motion->two - motion->four) / 4;

    motion->w_z = (motion->three - motion->one) / (2 * (A + B));


    return 0;
}

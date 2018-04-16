/*
 *  Created on: 2017-12-26
 *      Author: Young.Geo
 *          QQ: 473763733
 *       Email: anxan524@126.com
 */

#include "pose.h"

void    pose_init(pose_t *pose)
{
    xassert(pose);
    xzero(pose, sizeof(pose_t));
    xassert(pose->add_list = xlist_init());
}

int     calculatexyz(pose_t *pose, add_t *add)
{
    xassert(pose);
    xassert(add);
    xassert(pose->add_list);

    pose->x += add->x;
    pose->y += add->y;
    pose->w_z += add->w_z;

    xlist_add(pose->add_list, NULL, XLIST_STRING, (char *)xmemdup((void *)add, sizeof(add_t)));

    return 0;
}

int     pose_calculation(xlist *list, pose_t *pose)
{
    int *buf = { 0 };
    xlist *start = NULL;
    add_t add;

    xassert(list);
    xassert(pose);


    for (start = list; start && start->next; start = start->next)
    {
        if (!(buf = (int *)start->value))
            continue;

        motion_calculation(buf, &pose->motion);

        add.x = pose->motion.x * MOTIONTIMER;
        add.y = pose->motion.y * MOTIONTIMER;
        add.w_z = pose->motion.w_z * MOTIONTIMER;
        calculatexyz(pose, &add);
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



void    pose_destory(pose_t *pose)
{
    xassert(pose);

    if (pose->add_list)
        xlist_clean(&pose->add_list);
}

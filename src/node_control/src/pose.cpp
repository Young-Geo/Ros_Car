/*
 *  Created on: 2017-12-26
 *      Author: Young.Geo
 *          QQ: 473763733
 *       Email: anxan524@126.com
 */

#include "pose.h"


int     pubdata(pose_t *pose, ros::Publisher &odom_pub);

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

int     pose_calculation(ros::Publisher &pub, xlist *list, pose_t *pose)
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
        xmessage("motion vx %lf, vy %lf, vz %lf, sum: x %lf, y %lf, th %lf\n", pose->motion.x, pose->motion.y, pose->motion.w_z, pose->x, pose->y, pose->w_z);
        pubdata(pose, pub);
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

int     pubdata(pose_t *pose, ros::Publisher &odom_pub)
{
    ros::Time cur;
    geometry_msgs::Quaternion odom_quat;
    tf::TransformBroadcaster br;
    tf::Transform tf;
    nav_msgs::Odometry odom;
    geometry_msgs::TransformStamped odom_trans;


    xassert(pose);


    cur = ros::Time::now();

    odom_quat = tf::createQuaternionMsgFromYaw(pose->w_z);

    odom_trans.header.stamp = cur;
    odom_trans.header.frame_id = "odom";
    odom_trans.child_frame_id = "base_link";

    odom_trans.transform.translation.x = 0.0;
    odom_trans.transform.translation.y = 0.0;
    odom_trans.transform.translation.z = 0.0;
    odom_trans.transform.rotation = odom_quat;


//    tf.setOrigin(tf::Vector3(0.0, 0.0, 0.0));//re
//    tf.setRotation(odom_quat);

    //send the transform
    br.sendTransform(odom_trans);

    odom.header.stamp = cur;
    odom.header.frame_id = "odom";

    //set the position
    odom.pose.pose.position.x = pose->x;
    odom.pose.pose.position.y = pose->y;
    odom.pose.pose.position.z = 0.0;
    odom.pose.pose.orientation = odom_quat;

    //set the velocity
    odom.child_frame_id = "base_link";
    odom.twist.twist.linear.x = pose->motion.x;
    odom.twist.twist.linear.y = pose->motion.y;
    odom.twist.twist.angular.z = pose->motion.w_z;

    //publish the message
    odom_pub.publish(odom);
    return 0;
}

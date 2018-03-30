/*
 *  Created on: 2017-12-26
 *      Author: Young.Geo
 *          QQ: 473763733
 *       Email: anxan524@126.com
 */
#include "node_motion.h"
#include <tf/transform_broadcaster.h>

static     motion_t motion;

void		motion_call_back(const node_motion::motion::ConstPtr &motionmsg)
{
    if (!motion.isStart) {
        xmessage("motion not Start");
        return ;
    }
    movexyz(&motion, motionmsg->x, motionmsg->y, motionmsg->z);
}

int     node_motion_main(ros::NodeHandle &n)
{
    //
    ros::Subscriber subclient;
    ros::Publisher odom_pub;
    tf::TransformBroadcaster br;
    ros::Rate r(1.0);



    xassert(0 == motion_init(&motion));

    subclient = n.subscribe<node_motion::motion>("motion", 1000, motion_call_back);
    odom_pub = n.advertise<nav_msgs::Odometry>("odom", 50);

    double x = 0.0;
    double y = 0.0;
    double th = 0.0;

    double vx = 0.1;
    double vy = -0.1;
    double vth = 0.1;

    ros::Time current_time, last_time;
    current_time = ros::Time::now();
    last_time = ros::Time::now();

    while (ros::ok())
    {
        ROS_INFO("motion ...\n");
        ros::spinOnce();               // check for incoming messages
        current_time = ros::Time::now();
        //compute odometry in a typical way given the velocities of the robot
        double dt = (current_time - last_time).toSec();
        double delta_x = (vx * cos(th) - vy * sin(th)) * dt;
        double delta_y = (vx * sin(th) + vy * cos(th)) * dt;
        double delta_th = vth * dt;

        x += delta_x;
        y += delta_y;
        th += delta_th;

        //since all odometry is 6DOF we'll need a quaternion created from yaw
        geometry_msgs::Quaternion odom_quat = tf::createQuaternionMsgFromYaw(th);




        tf::Transform tf;
        tf.setOrigin(tf::Vector3(x, y, 0));
        tf.setRotation(tf::Quaternion(0, 0, th, 0));

        br.sendTransform(tf::StampedTransform(tf, current_time, "base_link", "odom"));


        //next, we'll publish the odometry message over ROS
        nav_msgs::Odometry odom;
        odom.header.stamp = current_time;
        odom.header.frame_id = "odom";

        //set the position
        odom.pose.pose.position.x = x;
        odom.pose.pose.position.y = y;
        odom.pose.pose.position.z = 0.0;
        odom.pose.pose.orientation = odom_quat;

        //set the velocity
        odom.child_frame_id = "base_link";
        odom.twist.twist.linear.x = vx;
        odom.twist.twist.linear.y = vy;
        odom.twist.twist.angular.z = vth;

        //publish the message
        odom_pub.publish(odom);
        last_time = current_time;

        r.sleep();
    }

    return 0;
}

/*
 *  Created on: 2017-12-26
 *      Author: Young.Geo
 *          QQ: 473763733
 *       Email: anxan524@126.com
 */

#include <ros/ros.h>
#include "node_test01.h"
#include "std_msgs/String.h"
#include <sensor_msgs/LaserScan.h>

#include <sstream>


void pu(ros::Publisher &odom_pub, ros::Publisher &scan_pub)
{
    ros::Time cur;
    geometry_msgs::Quaternion odom_quat;
    tf::TransformBroadcaster br, brfream;
    tf::Transform tf, tffream;
    nav_msgs::Odometry odom;
    geometry_msgs::TransformStamped odom_trans;
    sensor_msgs::LaserScan scan_msg;

    static double z = 0.1;
    z += 0.1;




    cur = ros::Time::now();

    odom_quat = tf::createQuaternionMsgFromYaw(z);

    odom_trans.header.stamp = cur;
    odom_trans.header.frame_id = "odom";
    odom_trans.child_frame_id = "base_link";

    odom_trans.transform.translation.x = 0.0;
    odom_trans.transform.translation.y = 0.0;
    odom_trans.transform.translation.z = z;
    odom_trans.transform.rotation = odom_quat;


//    tf.setOrigin(tf::Vector3(0.0, 0.0, 0.0));//re
//    tf.setRotation(odom_quat);

    //send the transform
    br.sendTransform(odom_trans);

    odom.header.stamp = cur;
    odom.header.frame_id = "odom";

    //set the position
    odom.pose.pose.position.x = 0.1;
    odom.pose.pose.position.y = 0.1;
    odom.pose.pose.position.z = z;
    odom.pose.pose.orientation = odom_quat;

    //set the velocity
    odom.child_frame_id = "base_link";
    odom.twist.twist.linear.x = 0.1;
    odom.twist.twist.linear.y = 0.1;
    odom.twist.twist.angular.z = z;

    scan_msg.header.stamp = cur;
    scan_msg.header.frame_id = "laser_frame";

    //publish the message
    odom_pub.publish(odom);
    scan_pub.publish(scan_msg);
    printf("push\n");


    tf.setOrigin(tf::Vector3(0.0, 0.0, 0.0));
    tf.setRotation(tf::Quaternion(0, 0, 0, 1));
    br.sendTransform(tf::StampedTransform(tf, cur, "map", "odom"));



    tffream.setOrigin(tf::Vector3(0.0, 0.0, 8.0));
    tffream.setRotation(tf::Quaternion(0, 0, 0, 1));
    brfream.sendTransform(tf::StampedTransform(tf, cur, "base_link", "laser_frame"));
}

int main(int argc, const char **argv)
{
    ros::init(argc, (char **)argv, "node_test01");

    ros::NodeHandle n;

    ros::Publisher odom_pub = n.advertise<nav_msgs::Odometry>("odom", 20);
    ros::Publisher scan_pub = n.advertise<sensor_msgs::LaserScan>("scan", 20);

    ros::Rate loop_rate(10);

    while (ros::ok())
    {
      ros::spinOnce();
      pu(odom_pub, scan_pub);
      printf("publist\n");
      loop_rate.sleep();
    }
    return 0;
}

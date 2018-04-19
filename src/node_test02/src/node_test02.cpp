/*
 *  Created on: 2017-12-26
 *      Author: Young.Geo
 *          QQ: 473763733
 *       Email: anxan524@126.com
 */
#include "node_test02.h"
#include <tf/transform_broadcaster.h>
#include "xserial.h"
#include "afx.h"
#include "xlog.h"
#define DERIVCE_NAME "/dev/motion"
#include <ros/duration.h>
#include <sensor_msgs/LaserScan.h>

int test01()
{
    int fd = 0;
    xassert((fd = xserial_open(DERIVCE_NAME)) > 0);

    xassert(xserial_init(fd, 9600, 8, 1, 'N', 0));

    return fd;
}


void    timer_callback(const ros::TimerEvent& event)
{
    printf("hello >>> \n");
    ROS_INFO("anxan\n");
}

void scanback(const sensor_msgs::LaserScan::ConstPtr& scan)
{
    xmessage("rec laser data\n");
    printf("rec laser data\n");
}


int     node_test02_main(ros::NodeHandle &n)
{
    ros::Subscriber sub;

    sub = n.subscribe("scan", 1000, scanback); //订阅/scan


    ros::spin();

    return 0;
}

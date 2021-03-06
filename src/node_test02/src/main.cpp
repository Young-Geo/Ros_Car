/*
 *  Created on: 2017-12-26
 *      Author: Young.Geo
 *          QQ: 473763733
 *       Email: anxan524@126.com
 */

#include <ros/ros.h>
#include "node_test02.h"

#include "std_msgs/String.h"

void chatterCallback(const std_msgs::String::ConstPtr& msg)
{
  ROS_INFO("I heard: [%s]", msg->data.c_str());
}

int main(int argc, const char **argv)
{
    ros::init(argc, (char **)argv, "node_test02");

    ros::NodeHandle n;

    node_test02_main(n);

    return 0;
}

/*
 *  Created on: 2017-12-26
 *      Author: Young.Geo
 *          QQ: 473763733
 *       Email: anxan524@126.com
 */

#include <ros/ros.h>
#include "node_test01.h"
#include "std_msgs/String.h"

#include <sstream>

int main(int argc, const char **argv)
{
    ros::init(argc, (char **)argv, "node_test01");

    ros::NodeHandle n;

    //node_test01_main(n);

    ros::Publisher chatter_pub = n.advertise<std_msgs::String>("chatter", 1000);

    ros::Rate loop_rate(10);

    int count = 0;
    while (ros::ok())
    {

      std_msgs::String msg;

      std::stringstream ss;
      ss << "hello world " << count;
      msg.data = ss.str();

      ROS_INFO("%s", msg.data.c_str());

      chatter_pub.publish(msg);

      ros::spinOnce();

      loop_rate.sleep();
      ++count;
    }
    return 0;
}

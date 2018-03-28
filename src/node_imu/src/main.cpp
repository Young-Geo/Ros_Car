/*
 *  Created on: 2017-12-26
 *      Author: Young.Geo
 *          QQ: 473763733
 *       Email: anxan524@126.com
 */

#include <ros/ros.h>

#include "node_imu.h"
#include "global.h"

int main(int argc, const char **argv)
{
    ros::init(argc, (char **)argv, "node_imu");

    ros::NodeHandle n;

    node_imu_main(n);

    return 0;
}

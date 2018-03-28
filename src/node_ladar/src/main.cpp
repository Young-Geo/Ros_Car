/*
 *  Created on: 2017-12-26
 *      Author: Young.Geo
 *          QQ: 473763733
 *       Email: anxan524@126.com
 */


#include <ros/ros.h>
#include "node_ladar.h"

int main(int argc, const char **argv)
{
    ros::init(argc, (char **)argv, "node_ladar");

    ros::NodeHandle n;

    node_ladar_main(n);

    return 0;
}

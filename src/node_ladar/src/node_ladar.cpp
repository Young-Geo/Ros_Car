/*
 *  Created on: 2017-12-26
 *      Author: Young.Geo
 *          QQ: 473763733
 *       Email: anxan524@126.com
 */
#include "node_ladar.h"


int     node_ladar_main(ros::NodeHandle &n)
{
    ros::Rate r(10);
    ladar_t ladar;

    xassert(!ladar_init(&ladar));


    auto timer_callback = [=, &ladar](const ros::TimerEvent& event) -> void
    {

    };

    n.createTimer(ros::Duration(0.1), timer_callback);


    while (ros::ok())
    {
        ros::spinOnce();
        r.sleep();
    }
    return 0;
}

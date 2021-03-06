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
    int i = 0;

    xassert(!ladar_init(&ladar));
    for (i = 0; i < 100; ++i)
    {
        if (!ladar_start(&ladar))
            break;
        xmessage("continue start ladar\n");
    }


    auto timer_callback = [=, &ladar](const ros::TimerEvent& event) -> void
    {
        ladar_analytic_data(&ladar);
    };

    n.createTimer(ros::Duration(0.1), timer_callback);


    while (ros::ok())
    {
        ros::spinOnce();
        r.sleep();
    }

    ladar_destory(&ladar);
    ros::shutdown();
    return 0;
}

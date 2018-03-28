/*
 *  Created on: 2017-12-26
 *      Author: Young.Geo
 *          QQ: 473763733
 *       Email: anxan524@126.com
 */
#include "node_motion.h"

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


    xassert(0 == motion_init(&motion));

    subclient = n.subscribe<node_motion::motion>("motion", 1000, motion_call_back);

/*
    while (ros::ok())
    {
        ROS_INFO("motion ...\n");
        ros::spinOnce();
    }
*/
    ros::spin();
    return 0;
}

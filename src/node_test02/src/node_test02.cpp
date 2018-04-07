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


int     node_test02_main(ros::NodeHandle &n)
{

    int fd = 0, ret = 0, a = 0;
     ros::Rate r(10);
     char buf[1024];


     fd = test01();
     if (fd <= 0) {
         xerror("test01 error\n");
         return -1;
     }



    //n.createTimer(ros::Duration(0.1), timer_callback);
    //ros::spin();
    while (ros::ok())
    {

        if ((ret = xserial_recv(fd, buf, sizeof(buf), 0)) <= 0) {
        } else {
            xmessage("=================\nhex:");
            for (int i = 0; i < ret; ++i)
            {
                xmessage("%d == %d == %x <>   ", buf[i], 0xff&buf[i], 0xff&buf[i]);
                xmessage("+++ %d\n", (short)(*buf));
            }

            for (int i = 0; i < (ret *8); ++i)
            {
                if ((0x1 << i) & (*buf))
                    printf("1");
                else
                    printf("0");
            }
            xmessage("\n=================%d\n", ret);
        }
        r.sleep();

    }

    return 0;
}

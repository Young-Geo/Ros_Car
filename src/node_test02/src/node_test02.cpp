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
#define DERIVCE_NAME "/dev/ttyUSB1"

int test01()
{
    int fd = 0;
    xassert((fd = xserial_open(DERIVCE_NAME)) > 0);

    xassert(xserial_init(fd, 9600, 8, 1, 'N', 0));

    return fd;
}


int     node_test02_main(ros::NodeHandle &n)
{

    int fd = 0;
     ros::Rate r(10);
     char buf[1024];

     fd = test01();
     if (fd <= 0) {
         xerror("test01 error\n");
         return -1;
     }



    while (ros::ok())
    {
        if (xserial_recv(fd, buf, sizeof(buf), 0) <= 0) {
            xmessage("xserial_recv fail \n");
        } else
            xmessage("rec %s", buf);
        r.sleep();
    }

    return 0;
}

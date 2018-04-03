#include "node_imu.h"
#include "ros/duration.h"



/**
  this is node_imu main
*/

int node_imu_main(ros::NodeHandle &n)
{

    imu_t *imu = NULL;
    ros::Rate r(10);

    xassert((imu = imu_init()));


    auto timer_callback = [=, &imu](const ros::TimerEvent& event) -> void
    {

        if (!imu->isStart) {
            xdebug("imu not start");
            return;
        }

        serial_par(imu->serial_fd, &imu->chain, imu->lists);
        make_imu(imu);

    };

    ros::Timer timer = n.createTimer(ros::Duration(0.1), timer_callback);

    while (ros::ok())
    {
        ros::spinOnce();
        r.sleep();
    }
    return 0;
}

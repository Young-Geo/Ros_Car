#include "node_imu.h"
#include "ros/duration.h"


static int imu_init(imu_t *imu)
{
    int fd = 0;

    xzero(&imu, sizeof(imu_t));
    xassert((fd = xsk_init_client("192.168.0.103", 9001, 0)) > 0);

    imu->serial_fd = fd;

    return 0;
}

static int  make_imu(imu_t *imu)
{
    xassert(imu);
    //
}

/**
  this is node_imu main
*/

int node_imu_main(ros::NodeHandle &n)
{

    imu_t imu;
    ros::Rate r(10);

    xassert((0 == imu_init(&imu)));


    auto timer_callback = [=, &imu](const ros::TimerEvent& event) -> void
    {
        if (!imu.jy901.isStart) {
            xdebug("imu not start");
            return;
        }

        make_imu(&imu);
    };

    ros::Timer timer = n.createTimer(ros::Duration(0.1), timer_callback);

    while (ros::ok())
    {
        serial_par(&imu.jy901, imu.serial_fd);
        ros::spinOnce();
        r.sleep();
    }
    return 0;
}

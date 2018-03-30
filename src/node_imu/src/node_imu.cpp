#include "node_imu.h"
#include "ros/duration.h"
#include "xchain.h"

static int imu_init(imu_t *imu)
{
    int fd = 0;

    xzero(&imu, sizeof(imu_t));
    xassert((fd = xsk_init_client("192.168.0.103", 9001, 0)) > 0);

    imu->serial_fd = fd;
    xchain_init(&imu->chain);
    xassert((imu->lists = xlist_init()));

    imu->isStart = 1;
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
        serial_par(imu.serial_fd, &imu.chain, imu.lists);
        make_imu(&imu);
        /*
        if (!imu.jy901.isStart) {
            xdebug("imu not start");
            return;
        }*/
    };

    ros::Timer timer = n.createTimer(ros::Duration(0.1), timer_callback);

    while (ros::ok())
    {
        ros::spinOnce();
        r.sleep();
    }
    return 0;
}

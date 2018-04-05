/*
 *  Created on: 2017-12-26
 *      Author: Young.Geo
 *          QQ: 473763733
 *       Email: anxan524@126.com
 */
#include "node_control.h"

static      control_imu_t control_imu;

int     timmer_back(void *arg)
{
    control_imu_t *control_imu = NULL;

    xassert((control_imu = (control_imu_t*)arg));

    imu_data_processing(control_imu->imu);
    control_data_processing(control_imu->control);
    //
    return 0;
}


void vel_callback(const geometry_msgs::Twist::ConstPtr & input)//订阅/cmd_vel主题回调函数
{

    int linear_x = 0, linear_y = 0, angular_z = 0;

    //angular_temp = cmd_input.angular.z ;//获取/cmd_vel的角速度,rad/s
    //linear_temp = cmd_input.linear.x ;//获取/cmd_vel的线速度.m/s

    //将转换好的小车速度分量为左右轮速度
    //left_speed_data.d = linear_temp - 0.5f*angular_temp*D ;
    //right_speed_data.d = linear_temp + 0.5f*angular_temp*D ;

    //存入数据到要发布的左右轮速度消息
    //left_speed_data.d*=ratio;   //放大１０００倍，mm/s
    //right_speed_data.d*=ratio;//放大１０００倍，mm/s
    if (!control_imu.control || !control_imu.control->isStart) {
        xmessage("control not start\n");
        return;
    }

    linear_x = input->linear.x;
    linear_y = input->linear.y;
    angular_z = input->angular.z;
    xmessage("control linear_x %d, linear_y %d, angular_z %d\n", linear_x, linear_y, angular_z);

    if (movexyz(control_imu.control, linear_x, linear_y, angular_z)) {
        xerror("control  movexyz not chenggong\n");
    }
}

int     node_control_main(ros::NodeHandle &n)
{
    //
    ros::Subscriber subclient;
    ros::Publisher odom_pub;
    ros::Subscriber vel_sub;
    tf::TransformBroadcaster br;
    ros::Rate r(1.0);

    xassert((control_imu.control = control_init()));
    xassert((control_imu.imu = imu_init()));
    xassert((control_imu.timmer = timmer_init(timmer_back, &control_imu)));

    odom_pub = n.advertise<nav_msgs::Odometry>("odom", 20);
    vel_sub = n.subscribe("cmd_vel", 20, vel_callback); //订阅/cmd_vel主题


    while (ros::ok())
    {
        ROS_INFO("motion ...\n");
        timmer_spinOnce(control_imu.timmer);
        ros::spinOnce();

        r.sleep();
    }

    control_destory(control_imu.control);
    imu_destory(control_imu.imu);
    timmer_destory(control_imu.timmer);
    return 0;
}



/***
 *
 *     double x = 0.0;
    double y = 0.0;
    double th = 0.0;

    double vx = 0.1;
    double vy = -0.1;
    double vth = 0.1;

    ros::Time current_time, last_time;
    current_time = ros::Time::now();
    last_time = ros::Time::now();


        current_time = ros::Time::now();
        //compute odometry in a typical way given the velocities of the robot
        double dt = (current_time - last_time).toSec();
        double delta_x = (vx * cos(th) - vy * sin(th)) * dt;
        double delta_y = (vx * sin(th) + vy * cos(th)) * dt;
        double delta_th = vth * dt;

        x += delta_x;
        y += delta_y;
        th += delta_th;

        //since all odometry is 6DOF we'll need a quaternion created from yaw
        geometry_msgs::Quaternion odom_quat = tf::createQuaternionMsgFromYaw(th);




        tf::Transform tf;
        tf.setOrigin(tf::Vector3(x, y, 0));
        tf.setRotation(tf::Quaternion(0, 0, th, 0));

        br.sendTransform(tf::StampedTransform(tf, current_time, "base_link", "odom"));


        //next, we'll publish the odometry message over ROS
        nav_msgs::Odometry odom;
        odom.header.stamp = current_time;
        odom.header.frame_id = "odom";

        //set the position
        odom.pose.pose.position.x = x;
        odom.pose.pose.position.y = y;
        odom.pose.pose.position.z = 0.0;
        odom.pose.pose.orientation = odom_quat;

        //set the velocity
        odom.child_frame_id = "base_link";
        odom.twist.twist.linear.x = vx;
        odom.twist.twist.linear.y = vy;
        odom.twist.twist.angular.z = vth;

        //publish the message
        odom_pub.publish(odom);
        last_time = current_time;
*/

#include "ros/ros.h"
#include "std_msgs/String.h"

int main(int argc, char **argv)
{
    ros::init(argc, argv, "controller");
    ros::NodeHandle nh;

    ros::Publisher pub = nh.advertise<std_msgs::String>("cpp_topic", 10);
    ros::Rate rate(1);  // 1 Hz

    while (ros::ok())
    {
        std_msgs::String msg;
        msg.data = "Hello from C++!";
        ROS_INFO("%s", msg.data.c_str());
        pub.publish(msg);
        rate.sleep();
    }

    return 0;
}

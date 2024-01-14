#include "ros/ros.h"
#include "std_msgs/Float32.h"
#include "std_msgs/Int64.h"

void Callback(const std_msgs::Float32::ConstPtr& delta_pos) {
    
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "controller");
    ros::NodeHandle nh;

    ros::Subscriber sub = nh.subscribe("delta_pos_topic", 1000, Callback);

    ros::Publisher pub = nh.advertise<std_msgs::Int64>("servo_angle_topic", 1000);
    ros::Rate rate(1);

    while (ros::ok())
    {
        std_msgs::Int64 servo_angle_msg;
        servo_angle_msg.data = 90;
        pub.publish(servo_angle_msg);

        ros::spinOnce();

        rate.sleep();
    }

    return 0;
}

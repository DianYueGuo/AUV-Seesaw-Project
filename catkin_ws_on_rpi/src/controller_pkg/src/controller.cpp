#include <cmath>

#include "ros/ros.h"
#include "std_msgs/Float32.h"
#include "std_msgs/Int64.h"

const double garma = 10000.0;

double max_acceleration = 0.000001;
double delta_angle = 0.0;

double previous_delta_pos = 0.0;
ros::Time previous_time;

void Callback(const std_msgs::Float32::ConstPtr& delta_pos_msg) {
    ros::Time current_time = ros::Time::now();
    ros::Duration time_duration = current_time - previous_time;
    previous_time = current_time;

    double current_delta_pos = delta_pos_msg->data;
    double current_velocity = (current_delta_pos - previous_delta_pos) / time_duration.toSec();
    previous_delta_pos = current_delta_pos;

    double acceleration = -2 * garma * current_velocity - garma * garma * current_delta_pos;

    if(acceleration > max_acceleration)
        max_acceleration = acceleration;
    else if(acceleration < -max_acceleration)
        max_acceleration = -acceleration;

    delta_angle = asin(-acceleration / max_acceleration);
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "controller");
    ros::NodeHandle nh;

    ros::Subscriber sub = nh.subscribe("delta_pos_topic", 1000, Callback);

    ros::Publisher pub = nh.advertise<std_msgs::Int64>("servo_angle_topic", 1000);
    ros::Rate rate(5);

    previous_time = ros::Time::now();

    while (ros::ok())
    {
        std_msgs::Int64 servo_angle_msg;
        servo_angle_msg.data = (int) (90 + delta_angle / M_PI * 190);
        pub.publish(servo_angle_msg);

        ros::spinOnce();

        rate.sleep();
    }

    return 0;
}

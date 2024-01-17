#include <cmath>

#include "ros/ros.h"
#include "std_msgs/Float32.h"
#include "std_msgs/Int64.h"

const double topic_publishing_rate = 6.0;
const double position_limit = 100.0;
const double acceleration = 9.802 * 3 / 5;

double delta_pos = 50.0;
double ball_velocity = 5.0;
double delta_angle = 0.01;

ros::Time time_stamp_for_updating_physics;
ros::Time time_stamp_for_publishing_topic;

void update_physics()
{
    ros::Time current_time = ros::Time::now();
    ros::Duration time_duration = current_time - time_stamp_for_updating_physics;
    time_stamp_for_updating_physics = current_time;

    delta_pos += ball_velocity * time_duration.toSec();
    ball_velocity -= acceleration * time_duration.toSec() * sin(delta_angle);

    if (delta_pos > position_limit) {
        delta_pos = position_limit;
        ball_velocity = 0.0;
    } else if (delta_pos < -position_limit) {
        delta_pos = -position_limit;
        ball_velocity = 0.0;
    }
}

void Callback(const std_msgs::Int64::ConstPtr &servo_angle_msg)
{
    delta_angle = (servo_angle_msg->data - 90) / 180.0 * M_PI;
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "controller_tester");

    ros::NodeHandle nh;
    ros::Subscriber sub = nh.subscribe("servo_angle_topic", 1000, Callback);
    ros::Publisher pub = nh.advertise<std_msgs::Float32>("delta_pos_topic", 1000);

    ros::Rate rate(60);

    time_stamp_for_updating_physics = ros::Time::now();
    time_stamp_for_publishing_topic = ros::Time::now();

    while (ros::ok())
    {
        ROS_INFO("delta_angle: %.3f, delta_pos: %.3f, ball_velocity: %.3f", delta_angle, delta_pos, ball_velocity);

        update_physics();

        ros::Time current_time = ros::Time::now();
        ros::Duration time_duration = current_time - time_stamp_for_publishing_topic;

        if(time_duration.toSec() >= 1/topic_publishing_rate) {
            time_stamp_for_publishing_topic = current_time;

            std_msgs::Float32 delta_pos_msg;
            delta_pos_msg.data = delta_pos;
            pub.publish(delta_pos_msg);
        }

        ros::spinOnce();

        rate.sleep();
    }

    return 0;
}
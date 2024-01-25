#include <cmath>

#include "ros/ros.h"
#include "std_msgs/Float32.h"
#include "std_msgs/Int64.h"

double servo_angle = 90.0;

double previous_delta_pos = 0.0;
ros::Time previous_time;

double map(double x, double in_min, double in_max, double out_min, double out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void Callback(const std_msgs::Float32::ConstPtr& delta_pos_msg) {
    const double kp = 2.0;
    const double ki = 1.0;
    const double kd = 10.0;

    ros::Time current_time = ros::Time::now();
    ros::Duration time_duration = current_time - previous_time;
    previous_time = current_time;

    double current_delta_pos = delta_pos_msg->data;    
   
    double PID_p = kp * current_delta_pos;   
    double PID_d = kd * ((current_delta_pos - previous_delta_pos) / time_duration.toSec());
    
    double PID_i;
    if(-3 < current_delta_pos && current_delta_pos < 3)
    {
      PID_i = PID_i + (ki * current_delta_pos);
    }
    else
    {
      PID_i = 0;
    }
  
    double PID_total = PID_p + PID_i + PID_d;
    PID_total = map(PID_total, -150, 150, 0, 180);
  
    if(PID_total < 20) { PID_total = 20; }
    if(PID_total > 160) { PID_total = 160; } 
  
    servo_angle = PID_total;
    previous_delta_pos = current_delta_pos;
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "controller");
    ros::NodeHandle nh;

    ros::Subscriber sub = nh.subscribe("delta_pos_topic", 1000, Callback);

    ros::Publisher pub = nh.advertise<std_msgs::Int64>("servo_angle_topic", 1000);
    ros::Rate rate(60);

    previous_time = ros::Time::now();

    while (ros::ok())
    {
        std_msgs::Int64 servo_angle_msg;
        servo_angle_msg.data = (int) servo_angle;
        pub.publish(servo_angle_msg);

        ros::spinOnce();

        rate.sleep();
    }

    return 0;
}

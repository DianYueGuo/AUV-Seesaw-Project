#include "ros/ros.h"
#include "std_msgs/Int64.h"

int delta_angle = 0;
void Callback(const std_msgs::String::ConstPtr& msg) {
	
	// determine the change in angle of the servo motor
	float dpos = msg->data;
	float b0 = , b1 = ;	// boundaries of delta_angle = 0, 1 or 2

	if (dpos < b0 && dpos > (-1)*b0)						delta_angle = 0;
	else if (dpos > b0 && dpos < b1)						delta_angle = 1;
	else if (dpos < (-1)*b0 && dpos < (-1)*b1)	delta_angle = -1;
	else if (dpos > b1)													delta_angle = 2;
	else if (dpos < (-1)*b1)										delta_angle = -2;
  
	ROS_INFO("delta_pos: %f, delta_angle: %d", dpos, delta_angle);

}


int main(int argc, char **argv) {

	ros::init(argc, argv, "controller");
	ros::NodeHandle n;

	// * type of delta_pos?? *
	ros::Subscriber sub = n.subscribe("delta_pos", 1000, Callback);
	ros::Publisher chatter_pub = 
		n.advertise<std_msgs::Int64>("servo_topic", 1000);
	
	ros::Rate loop_rate(10);

	int angle = 90;
	while (ros::ok()) {

		std_msgs::Int64 msg;
		
		angle += delta_angle;
		msg->data = angle;

		ROS_INFO("angle: %d", msg->data);

		chatter_pub.publish(msg);
		ros::spinOnce();
		loop_rate.sleep();

	}

	return 0;

}

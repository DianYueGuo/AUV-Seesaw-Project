#include <ros.h>
#include <servo.h>
#include <std_msgs/Int64.h>

Servo servo;
ros::NodeHandle nh;

void messageCb(const std_msgs::Int64& msg)
{
  servo.write(msg.data);
}

ros::Subscriber<std_msgs::Int64> sub("servo_topic", &messageCb);

void setup()
{
  Serial.begin(9600);
  pinMode(9, OUTPUT);
  servo.attach(9);
  servo.write(1);

  nh.initNode();
  nh.subscribe(sub);
}

void loop()
{
  nh.spinOnce();
  delay(1);
}

#include <algorithm>
#include <boost/mpl/front_fwd.hpp>
#include <cmath>
#include <geometry_msgs/Twist.h>
#include <iostream>
#include <iterator>
#include <ros/ros.h>
#include <sensor_msgs/LaserScan.h>
#include <vector>
using namespace std;
ros::Publisher pub;
ros::Subscriber sub;
geometry_msgs::Twist action;

void mov() {
action.linear.x = 5;
action.angular.z = 0;
pub.publish(action);
}

void left() {
int count = 0;
while (count < 9) {
action.linear.x = 4;
action.angular.z = 13;
count++;
pub.publish(action);
}
}
void smallLeft() {
int count = 0;
while (count < 4) {
action.linear.x = 5;
action.angular.z = 13;
count++;
pub.publish(action);
}
}

void right() {
int count = 0;
while (count < 9){
action.linear.x = 3;
action.angular.z = -13;
count++;
pub.publish(action);
}
}
void smallRight() {
int count = 0;
while (count < 4) {
action.linear.x = 4.5;
action.angular.z = -13;
count++;
pub.publish(action);
}
}
void callBack(sensor_msgs::LaserScan msg) {

const auto frbeing = msg.ranges.begin() + 36;
const auto frend = frbeing + 36;

const auto fbegin = frend + 1;
const auto fend = fbegin + 36;

const auto flbegin = fend + 1;
const auto flend = flbegin + 36;

const auto leftStart = flend + 1;
const auto leftend = leftStart + 36;

float rightRange = *std::max_element(msg.ranges.begin(), msg.ranges.begin() + 36);
float rightFront = *std::min_element(frbeing,frend);
float front = *std::min_element(fbegin,fend);
float frontMax = *std::max_element(fbegin,fend);
float leftFront = *std::min_element(flbegin,flend);
float leftRange = *std::max_element(leftStart, leftend);

if (front > 3.8 && rightFront > 0.6 && leftFront > 0.6) {
mov();
} else if (front < 5 && rightRange < 4 && leftRange > 4) {
    left();
} else if (front < 5 && rightRange > 4 && leftRange < 4) {  
    right();
} else if (front > 1 && rightFront > 0.8 && leftFront > 0.6) {
    mov();
} else if (front > 1 && rightFront < 1 && leftFront > 1) {
    smallLeft();
} else if (front > 1 && rightFront > 1 && leftFront < 1) {
    smallRight();
}

cout <<" front: " << front << " right: " << rightFront << " Left: " << leftFront << " RightMAX: " << rightRange << " LeftMax: " << leftRange;

}
int main(int argc, char **argv) {
    ros::init(argc, argv, "race_robot_node");
    ros::NodeHandle nh;

    sub = nh.subscribe("/robot/base_scan", 1000, callBack);
    pub = nh.advertise<geometry_msgs::Twist>("/robot/cmd_vel", 1000);


   

    ros::spin();

}

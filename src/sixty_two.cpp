[200~#include <algorithm>
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
action.linear.x = 2.3;
action.angular.z = 15;
pub.publish(action);
}

void right() {
action.linear.x = 2.5;
action.angular.z = -15;
pub.publish(action);
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

if (frontMax > 2.3 && rightFront > 2 && leftFront > 2) {
mov();
} else if (front < 2.8 && leftFront > 2 && rightFront > 2) { 
    if (leftRange > 4) {
    left();
    } else {right();}
} else if (front > 2.3 && leftFront > 2 && rightFront < 2) {
    left();
} else if (front > 2.3 && leftFront < 2 && rightFront > 2) {
    if (frontMax > 4) {
    mov();} else {right();}
} else if (front < 2.5 && leftFront > 2 && rightFront < 2) {
    left();
} else if (front < 2.5 && leftFront < 2 && rightFront > 2) {
right();
} else if (front < 2.5 && leftFront < 2 && rightFront < 2) {
    if (rightRange > 3.8) {
    right();
    } else {
        left();
    }
} else if (front > 2.3 && leftFront < 2 && rightFront < 2) {
mov();
}






cout << " front : " << front << " right: " << rightFront << " left: " << leftFront << " right Range: " << rightRange << " left Range: " << leftRange;
}

int main(int argc, char **argv) {
    ros::init(argc, argv, "race_robot_node");
    ros::NodeHandle nh;

    sub = nh.subscribe("/robot/base_scan", 1000, callBack);
    pub = nh.advertise<geometry_msgs::Twist>("/robot/cmd_vel", 1000);


   

    ros::spin();

}

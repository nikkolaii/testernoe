#include <ros/ros.h>
#include <ros/topic.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>
#include <nav_msgs/Odometry.h>
#include <geometry_msgs/Point.h>
#include <std_msgs/Header.h>
#include "ros_detection/FloatVector.h"
#include <tf/transform_listener.h>
#include <cmath>
#include <iostream>
#include <std_msgs/Float64.h>
void movebaseaction(ros::NodeHandle& nh, tf::TransformListener& listener, move_base_msgs::MoveBaseGoal& goal,
  actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction>& myclient)
{

  geometry_msgs::Point point = *(ros::topic::waitForMessage<geometry_msgs::Point>("move_base_goal_topic", nh));
  std_msgs::Float64 FOV_angle = *(ros::topic::waitForMessage<std_msgs::Float64>("/FOV_angle", nh));
  std_msgs::Float64 distance = *(ros::topic::waitForMessage<std_msgs::Float64>("/distance", nh));

  tf::StampedTransform stamp;
  listener.waitForTransform("map","base_link",ros::Time::now(),ros::Duration(3.0));
  listener.lookupTransform("map","base_link",ros::Time(0), stamp);

  goal.target_pose.header.frame_id = "map";
  goal.target_pose.header.stamp = ros::Time::now();
  goal.target_pose.pose.position.x = point.x;
  goal.target_pose.pose.position.y = point.y;
  goal.target_pose.pose.position.z = 0;
  goal.target_pose.pose.orientation.x = stamp.getRotation().x();
  goal.target_pose.pose.orientation.y = stamp.getRotation().y();
  goal.target_pose.pose.orientation.z = stamp.getRotation().z();
  goal.target_pose.pose.orientation.w = stamp.getRotation().w();

  ROS_INFO_STREAM("Sending the following navigation goal: " << goal);
  myclient.sendGoal(goal);
  ROS_INFO_STREAM("Angle = "<<FOV_angle);
  ros::Duration(1).sleep();
    while (myclient.getState() == actionlib::SimpleClientGoalState::ACTIVE && std::abs(FOV_angle.data) <= 15 && distance.data < 1)
    {
      ROS_INFO_STREAM(myclient.getState().toString().c_str());
      FOV_angle = *(ros::topic::waitForMessage<std_msgs::Float64>("/FOV_angle", nh));
      ROS_INFO_STREAM("CURRENT ANGLE IS :" << FOV_angle);
      std_msgs::Float64 distance = *(ros::topic::waitForMessage<std_msgs::Float64>("/distance", nh));

    }
}

int main(int argc, char** argv)
{
  ros::init(argc,argv,"ControllerNode");
  ros::NodeHandle nh;
  actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> myclient("move_base", true);
  ROS_INFO("Waiting for the move_base action server");
  myclient.waitForServer();
  ROS_INFO("Connected to move base server");
  ROS_INFO("");

  tf::TransformListener listener;
  move_base_msgs::MoveBaseGoal goal;

  while(true)
  {
    movebaseaction(nh, listener, goal, myclient);
  }
  return 0;
}

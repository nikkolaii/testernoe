/* Created by Nikolai Persen 2021 */
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
void movebaseaction(ros::NodeHandle& nh, tf::TransformListener& listener, tf::StampedTransform& stamp, move_base_msgs::MoveBaseGoal& goal,
                    actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction>& myclient)
{
  geometry_msgs::Point point;
  point = *(ros::topic::waitForMessage<geometry_msgs::Point>("move_base_goal_topic", nh));
  std_msgs::Float64 FOV_angle = *(ros::topic::waitForMessage<std_msgs::Float64>("/FOV_angle", nh));
  ROS_INFO("Sending goal");

  listener.waitForTransform("map","base_link",ros::Time::now(),ros::Duration(3.0));
  listener.lookupTransform("map","base_link",ros::Time(0), stamp);

  goal.target_pose.pose.position.x = point.x;
  goal.target_pose.pose.position.y = point.y;
  goal.target_pose.pose.position.z = 0;
  goal.target_pose.header.frame_id = "map";
  goal.target_pose.header.stamp = ros::Time::now();
  goal.target_pose.pose.orientation.x = stamp.getRotation().x();
  goal.target_pose.pose.orientation.y = stamp.getRotation().y();
  goal.target_pose.pose.orientation.z = stamp.getRotation().z();
  goal.target_pose.pose.orientation.w = stamp.getRotation().w();

  ROS_INFO_STREAM("Sending the following navigation goal: " << goal);
  myclient.sendGoal(goal);
  ros::Duration(2).sleep();
  int f = 0;
  while (myclient.getState() == actionlib::SimpleClientGoalState::ACTIVE && FOV_angle.data > -20 && FOV_angle.data < 20 )
  {
    if (f == 0)
    {
        ROS_INFO("Active pursuit mode");
    }
    FOV_angle = *(ros::topic::waitForMessage<std_msgs::Float64>("/FOV_angle", nh));
  }
  ROS_INFO("Exiting active pursuit mode");
}

int main(int argc, char** argv)
{
  ros::init(argc,argv,"newnodes");
  ros::NodeHandle nh;
  actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> myclient("move_base", true);
  ROS_INFO("Waiting for the move_base action server");
  myclient.waitForServer(ros::Duration(5));
  ROS_INFO("Connected to move base server");
  tf::TransformListener listener;
  move_base_msgs::MoveBaseGoal goal;
  tf::StampedTransform stamp;
  float angle = 0;
  while (true)
  {
    movebaseaction(nh, listener, stamp, goal, myclient);
  }
return 0;
}

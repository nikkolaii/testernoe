#include <ros/ros.h>
#include <ros/topic.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>
#include <nav_msgs/Odometry.h>
#include <geometry_msgs/Point.h>
#include <std_msgs/Header.h>
#include <nav_msgs/Path.h>
#include "ros_detection/FloatVector.h"
#include <tf/transform_listener.h>
#include <cmath>
#include <iostream>
#include <std_msgs/Float64.h>
void movebaseaction(ros::NodeHandle& nh, tf::TransformListener& listener, move_base_msgs::MoveBaseGoal& goal,
  actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction>& myclient)
{
  tf::StampedTransform stamp;
  nav_msgs::Path path = *(ros::topic::waitForMessage<nav_msgs::Path>("person_path", nh));
  std_msgs::Float64 FOV_angle = *(ros::topic::waitForMessage<std_msgs::Float64>("/FOV_angle", nh));
  geometry_msgs::Point opt_point = *(ros::topic::waitForMessage<geometry_msgs::Point>("/Optimized_person_distance", nh));
  std_msgs::Float64 dist = *(ros::topic::waitForMessage<std_msgs::Float64>("/relative_distance", nh));

  listener.waitForTransform("map","base_link",ros::Time::now(),ros::Duration(3.0));
  listener.lookupTransform("map","base_link",ros::Time(0), stamp);
  int seq = path.header.seq;
  ROS_INFO_STREAM("SEQ = "<< seq);
  goal.target_pose.header.frame_id = "map";
  goal.target_pose.header.stamp = ros::Time::now();
  goal.target_pose.pose.position.x = path.poses[seq].pose.position.x;
  goal.target_pose.pose.position.y = path.poses[seq].pose.position.y;
  goal.target_pose.pose.position.z = 0;
  goal.target_pose.pose.orientation.x = stamp.getRotation().x();
  goal.target_pose.pose.orientation.y = stamp.getRotation().y();
  goal.target_pose.pose.orientation.z = stamp.getRotation().z();
  goal.target_pose.pose.orientation.w = stamp.getRotation().w();

  // ROS_INFO_STREAM("Sending the following navigation goal: " << goal);
  myclient.sendGoal(goal);


  // ROS_INFO_STREAM("Angle = "<<FOV_angle);
  dist = *(ros::topic::waitForMessage<std_msgs::Float64>("/relative_distance", nh));
  // ROS_INFO_STREAM("\n DISTNACE: = " << dist);
  if (dist.data > 1)
  {
    ros::Duration(1).sleep();
    ROS_INFO_STREAM("Current distance is: " << dist << "Generating new goal for the next 4 seconds to catch up with the person");
  }
  else
  {
  ros::Duration(1).sleep();
  }

    while (myclient.getState() == actionlib::SimpleClientGoalState::ACTIVE && FOV_angle.data > -15 && FOV_angle.data < 15 && dist.data <= 1)
    {
      ROS_INFO_STREAM(myclient.getState().toString().c_str());
      FOV_angle = *(ros::topic::waitForMessage<std_msgs::Float64>("/FOV_angle", nh));
      ROS_INFO_STREAM("CURRENT ANGLE IS :" << FOV_angle);
      dist = *(ros::topic::waitForMessage<std_msgs::Float64>("/relative_distance", nh));
      // ROS_INFO_STREAM("\n x : " << opt_point.x << "\n y : " << opt_point.y <<"\n z : " << opt_point.z);
      ROS_INFO_STREAM("\n DISTNACE: = " << dist);
    }
    if (myclient.getState() == actionlib::SimpleClientGoalState::ACTIVE) {
      ROS_INFO_STREAM("Cancelling the current goal");
      // myclient.cancelGoal();
    }

}

int main(int argc, char** argv)
{
  ros::init(argc,argv,"ControllerNode");
  ros::NodeHandle nh;
  actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> myclient("move_base", true);
  ROS_INFO("Waiting for the move_base action server");
  myclient.waitForServer(ros::Duration(5));
  ROS_INFO("Connected to move base server");
  ROS_INFO("");

  tf::TransformListener listener;
  move_base_msgs::MoveBaseGoal goal;
  float angle = 0;
  while(true)
  {
    movebaseaction(nh, listener, goal, myclient);
  }
  return 0;
}

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
  tf::StampedTransform stamp;
  geometry_msgs::Point point = *(ros::topic::waitForMessage<geometry_msgs::Point>("move_base_goal_topic", nh));
  std_msgs::Float64 FOV_angle = *(ros::topic::waitForMessage<std_msgs::Float64>("/FOV_angle", nh));

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
  ros::Duration(4).sleep();
    while (myclient.getState() == actionlib::SimpleClientGoalState::ACTIVE && FOV_angle.data > -15 && FOV_angle.data < 15 )
    {
      ROS_INFO_STREAM(myclient.getState().toString().c_str());
      FOV_angle = *(ros::topic::waitForMessage<std_msgs::Float64>("/FOV_angle", nh));
      ROS_INFO_STREAM("CURRENT ANGLE IS :" << FOV_angle);
    }
    if (myclient.getState() == actionlib::SimpleClientGoalState::ACTIVE) {
      ROS_INFO_STREAM("Cancelling the current goal");
      myclient.cancelGoal();
    }

  // result = myclient.waitForResult();/opt/ros/kinetic/include/ros/console.h:379:7: note: in expansion of macro ‘ROSCONSOLE_PRINT_AT
//   else{
//     if (myclient.getState() == actionlib::SimpleClientGoalState::LOST){
//       // ROS_INFO_STREAM("The current goal is: "<<goal);
//
// }
// else{
//   // ROS_INFO_STREAM("The current goal is: "<<goal);
//   // ROS_INFO_STREAM("IM FUCKING CANCELLING IT NOW, BE RIGHT");
//
//   myclient.cancelGoal();
// }
//   ROS_INFO_STREAM("The person detected is out of the field of view. Goal aborted"<<2);
//       }

      // stateofgoal = myclient.getState();

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

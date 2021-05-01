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
class myclass
 {
public:
  bool result;
  move_base_msgs::MoveBaseGoal goal;
  geometry_msgs::Point point;
  geometry_msgs::Point::ConstPtr pointptr;
  nav_msgs::Odometry odom;


  myclass(){

    point = *(ros::topic::waitForMessage<geometry_msgs::Point>("move_base_goal_topic", nh));
    odom = *(ros::topic::waitForMessage<nav_msgs::Odometry>("/odom", nh));
    FOV_angle = *(ros::topic::waitForMessage<std_msgs::Float64>("/FOV_angle", nh));


    ROS_INFO_STREAM(point);
    movebaseaction();
    ROS_INFO("FFAF");
    // stampsub = nh.subscribe("/person_position", 1, &myclass::callback, this);
    // goalsub = nh.subscribe("/move_base_goal_topic",1000, &myclass::cb_goal, this);
  }

  // void cb_stamp(const nav_msgs::Odometry::ConstPtr& headerdata)
  // {
  //   header.stamp = headerdata->header.stamp;
  //   header.frame_id = "map";
  //   x = 2;
  //   // goal.target_pose.header = header;
  // }


// void callback(const nav_msgs::Odometry::ConstPtr& data){
//   header.stamp = data->header.stamp;
//   header.frame_id = "map";
//   float y = data->pose.pose.orientation.w;
// }
// // void cb_goal(const geometry_msgs::Point::ConstPtr& pointData){
// //     point = *pointData;
// //     goal.target_pose.pose.position.x = 21.174;
// //   	goal.target_pose.pose.position.y = 10.876;
// //   	goal.target_pose.pose.orientation.w = 1;
// //     // goal.target_pose.pose.position = point;
// //     goal.target_pose.pose.orientation.w = 1;
// //     // ROS_INFO_STREAM("data = "<<pointData->x);
//
//   }

int movebaseaction(){
  for (int i = 0; i < 30; i++) {
    /* code */ROS_INFO_STREAM("N");
  }
  actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> myclient("move_base", false);
  FOV_angle = *(ros::topic::waitForMessage<std_msgs::Float64>("/FOV_angle", nh));
  ROS_INFO("Waiting for the move_base action server");
  myclient.waitForServer(ros::Duration(5));
  ROS_INFO("Connected to move base server");
  ROS_INFO("");
  ROS_INFO("Sending goal");
  ROS_INFO_STREAM("POINT IS; "<<point);
  goal.target_pose.pose.position.x = point.x;
  goal.target_pose.pose.position.y = point.y;
  goal.target_pose.pose.position.z = 0;
  _listener.waitForTransform("map","base_link",ros::Time::now(),ros::Duration(3.0));

  _listener.lookupTransform("map","base_link",ros::Time(0), stamp);

  // goal.target_pose.pose.position.x = point.y;
  // goal.target_pose.pose.position.x = point.z;
  goal.target_pose.header.frame_id = "map";
  goal.target_pose.header.stamp = ros::Time::now();
  goal.target_pose.pose.orientation.x = stamp.getRotation().x();
  goal.target_pose.pose.orientation.y = stamp.getRotation().y();
  goal.target_pose.pose.orientation.z = stamp.getRotation().z();
  goal.target_pose.pose.orientation.w = stamp.getRotation().w();
  ROS_INFO_STREAM("The current goal is: "<<goal);
  if (FOV_angle.data > -10 && FOV_angle.data < 10) {
    myclient.sendGoal(goal);
    ros::Duration(5).sleep();
    ROS_INFO_STREAM("Cancelled");
    myclient.cancelGoal();

  }
  // result = myclient.waitForResult();
  else{
  myclient.cancelGoal();
  ROS_INFO_STREAM("The person detected is out of the field of view. Goal aborted"<<2);
      }
      ROS_INFO_STREAM("N" <<goal);
  return 1;
}
 private:
   ros::NodeHandle nh;
   ros::Subscriber stampsub;
   ros::Subscriber goalsub;
   std_msgs::Header header;
   tf::TransformListener _listener;
   tf::StampedTransform stamp;
   std_msgs::Float64 FOV_angle;
 };


int main(int argc, char** argv) {
  ros::init(argc,argv,"newnodes");
for (int i = 0; i < 1; i++) {

  ROS_INFO_STREAM("NOW YOUR BHER");

myclass obj;
ROS_INFO_STREAM("NOW YOUR BHER");
}

for (int i = 0; i < 15; i++) {
  /* code */
  ROS_INFO_STREAM(",2"<<i);
}
return 0;
}

// Created by Nikolai Persen 2021

#include "ros/ros.h"
#include "geometry_msgs/Point.h"
#include "geometry_msgs/PoseStamped.h"
#include "std_msgs/Header.h"
#include "tf/transform_broadcaster.h"
#include <iostream>
#include "std_msgs/Float32MultiArray.h"
#include "ros_detection/FloatVector.h"
#include "tf/transform_broadcaster.h"
#include <tf/transform_listener.h>


class pathPublisher
{
public:
  pathPublisher(){
    pointsub_ = point_nh_.subscribe("/person_position", 1, &pathPublisher::callback, this);
    optimized_point_pub_ = point_nh_.advertise<geometry_msgs::Point>("Optimized_person_distance",10000,true);
  }
void callback(const ros_detection::FloatVector::ConstPtr& pointData)
{
  current_size_ = pointData->point.size();
  temp_point_.x = 0;
  temp_point_.y = 0;
  temp_point_.z = 0;
  int count = 0;
  /* Creating the person_detection_frame */
  quat_.setRPY(4.71238898,0,-1.57079633);
  quat_.normalize();
  transform_.setOrigin(tf::Vector3(0,0,0));
  transform_.setRotation(quat_);
  br_.sendTransform(tf::StampedTransform(transform_, ros::Time::now(), "zed_left_camera_frame", "person_detection_frame"));
  if (current_size_ >=10)
  {
    for (int i = current_size_-10; i < current_size_; i++)
    {
      temp_point_.x = temp_point_.x + pointData->point[i].x;
      temp_point_.y = temp_point_.y + pointData->point[i].y;
      temp_point_.z = temp_point_.z + pointData->point[i].z;
      count++;
    }
    testpoint_.x = temp_point_.x/count;
    testpoint_.y = temp_point_.y/count;
    testpoint_.z = temp_point_.z/count;
    optimized_point_pub_.publish(testpoint_);
  }
}

private:
  ros::NodeHandle point_nh_;
  ros::Subscriber pointsub_;
  ros::Publisher optimized_point_pub_;

  geometry_msgs::Point testpoint_;
  geometry_msgs::Point temp_point_;
  int current_size_;

  tf::Quaternion quat_;
  tf::TransformBroadcaster br_;
  tf::Transform transform_;
};
int main(int argc, char **argv){
  ros::init(argc, argv, "personTF");


  pathPublisher pathobj;
  ros::spin();
  return 0;
}

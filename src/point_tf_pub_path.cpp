// Created by Nikolai Persen 2021

#include "ros/ros.h"
#include "nav_msgs/Path.h"
#include "geometry_msgs/Point.h"
#include "geometry_msgs/PoseStamped.h"
#include "std_msgs/Header.h"
#include "tf/transform_broadcaster.h"
#include <iostream>
#include "std_msgs/Float32MultiArray.h"
#include "ros_detection/FloatVector.h"
class poseTF{

public:
  int testvar;
  poseTF(){
  _sub = _nh.subscribe("/person_position", 1, &poseTF::callback, this);
  _pub = _nh.advertise<nav_msgs::Path>("path",10000,true);
  _pub2 = _nh.advertise<ros_detection::FloatVector>("person_distance_vector",10000,true);
}

void callback(const geometry_msgs::Point::ConstPtr& data){
  geometry_msgs::Point point;
  // geometry_msgs::PoseStamped posestamped;
  // std_msgs::Header myheader;
  // myheader.stamp = ros::Time::now();
  // myheader.frame_id = "person_detection_frame";
  // path.header = myheader;
  point.x = data -> x;
  point.y = data -> y;
  point.z = data -> z;

  // float dist_x = 0;
  // float dist_y = 0;
  // float dist_z = 0;
  // // testvar = 1;
  // int count = 0;
  //
  // for (int i = 0; i < 1; i++) {
  //   dist_x = dist_x + data -> x;
  //   dist_y = dist_y + data -> y;
  //   dist_z = dist_z + data -> z;
  //   count++;
  // }
  // dist_x = dist_x / count);
  // dist_y = dist_y / count;
  // dist_z = dist_z / count;
  // posestamped.pose.position.x = dist_x;
  // posestamped.pose.position.y = dist_y;
  // posestamped.pose.position.z = dist_z;
  // posestamped.header = myheader;
  // posestamped.pose.orientation.x = 0;
  // posestamped.pose.orientation.y = 0;
  // posestamped.pose.orientation.z = 0;
  // posestamped.pose.orientation.w = 1;



  floatvector.point.push_back(point);
  // _pub.publish(path);
  _pub2.publish(floatvector);
  //std::cout<<"average x dist: "<<dist_x<<std::endl;
  //std::cout<<"real x dist: " << checkbig << std::endl;
  quat.setRPY(4.71238898,0,-1.57079633);
  quat.normalize();
  _transform.setOrigin(tf::Vector3(0,0,0));
  _transform.setRotation(quat);
  _br.sendTransform(tf::StampedTransform(_transform, ros::Time::now(), "zed_left_camera_frame", "person_detection_frame"));

  _transform2.setOrigin(tf::Vector3(dist_x, dist_y, dist_z));
  _transform2.setRotation(tf::Quaternion(0,0,0,1));
  _br2.sendTransform(tf::StampedTransform(_transform2, ros::Time::now(), "person_detection_frame", "person"));
}

private:
  ros::NodeHandle _nh;
  ros::Subscriber _sub;
  ros::Publisher _pub;
  ros::Publisher _pub2;
  tf::TransformBroadcaster _br;
  tf::Transform _transform;
  tf::TransformBroadcaster _br2;
  tf::Transform _transform2;
  tf::Quaternion quat;
  nav_msgs::Path path;
  ros_detection::FloatVector floatvector;
};

class pathPublisher
{
public:

  pathPublisher(){
    pathSub_ = pathNH_.subscribe("/person_distance_vector", 1, &pathPublisher::callback, this);
    pathpub = pathNH_.advertise<nav_msgs::Path>("pathh",10000,true);
  }
void callback(const ros_detection::FloatVector::ConstPtr& pointData) {
  test.point = pointData->point;
  current_size = pointData->point.size();
  average_dist_x = 0;
  average_dist_y = 0;
  average_dist_z = 0;
  int count = 0;
  std_msgs::Header myheader;
  geometry_msgs::PoseStamped posestamped;

  if (current_size >=25) {
    for (int i = current_size-25; i < current_size; i++) {
      average_dist_x = average_dist_x + pointData->point[i].x;
      average_dist_y = average_dist_y + pointData->point[i].y;
      average_dist_z = average_dist_z + pointData->point[i].z;
      count++;
    }
    average_dist_x = average_dist_x/count;
    average_dist_y = average_dist_y/count;
    average_dist_z = average_dist_z/count;
    myheader.stamp = ros::Time::now();
    myheader.frame_id = "person_detection_frame";
    path.header = myheader;

    posestamped.pose.position.x = average_dist_x;
    posestamped.pose.position.y = average_dist_y;
    posestamped.pose.position.z = average_dist_z;
    posestamped.header = myheader;
    posestamped.pose.orientation.x = 0;
    posestamped.pose.orientation.y = 0;
    posestamped.pose.orientation.z = 0;
    posestamped.pose.orientation.w = 1;
    path.poses.push_back(posestamped);
    pathpub.publish(path);
  }
}

private:
  ros::NodeHandle pathNH_;
  ros::Subscriber pathSub_;
  ros::Publisher pathpub;
  ros_detection::FloatVector test;
  geometry_msgs::Point testpoint;
  int current_size;
  float average_dist_x;
  float average_dist_y;
  float average_dist_z;
  nav_msgs::Path path;


};
int main(int argc, char **argv){
  ros::init(argc, argv, "personTF");

  poseTF tfobj;
  pathPublisher pathobj;
  ros::spin();
  return 0;
}

#include "ros/ros.h"
#include "nav_msgs/Path.h"
#include "geometry_msgs/PoseStamped.h"
#include "geometry_msgs/PointStamped.h"
#include "std_msgs/Header.h"
#include "tf/transform_broadcaster.h"
#include "nav_msgs/Path.h"
#include <tf/transform_listener.h>
#include <iostream>
#include <geometry_msgs/TransformStamped.h>

class move_base_goal
{
public:
  tf::StampedTransform stamp;
      tf::StampedTransform stamp2;
  geometry_msgs::TransformStamped transformedstamped;
move_base_goal(){
  // sub = _nh.subscribe("/Optimized_person_distance",1, &move_base_goal::callback, this
sub = _nh.subscribe("/Optimized_person_distance",1, &move_base_goal::callback, this);
_pub = _nh.advertise<nav_msgs::Path>("global_path",100,true);
_pub2 = _nh.advertise<geometry_msgs::Point>("move_base_goal_topic",100,true);
ros::Rate loop_rate(10);
  //publisher

}


void callback(const geometry_msgs::Point::ConstPtr& pointData)
{

  // _personPoint.header.frame_id = "/person_frame";
  // _personPoint.header.stamp = ros::Time::now();
  // _personPoint.point.x = pointData ->x;
  // _personPoint.point.y = pointData ->y;
  // _personPoint.point.z = pointData ->z;
  std::cout<<"ty0";
  _listener.waitForTransform("map","person",ros::Time::now(),ros::Duration(3.0));

  _listener.lookupTransform("map","person",ros::Time(0), stamp);
  //ROS_INFO_STREAM(" Transform: " << stamp.getOrigin().x() << ", " << stamp.getOrigin().y() << ", " <<stamp.getOrigin().z() << ", " << stamp.getRotation().x() << ", " << stamp.getRotation().y() << ", " << stamp.getRotation().z());
  point.x = stamp.getOrigin().x();
  point.y = stamp.getOrigin().y();
  point.z = stamp.getOrigin().z();


  quat.setRPY(stamp.getRotation().x(),stamp.getRotation().y(),stamp.getRotation().z());
  quat.normalize();
  header.stamp = ros::Time::now();
  header.frame_id = "map";
  posestamped.header = header;
  posestamped.pose.position.x = point.x;
  posestamped.pose.position.y = point.y;
  posestamped.pose.position.z = point.z;
  posestamped.pose.orientation.x = quat.x();
  posestamped.pose.orientation.y = quat.y();
  posestamped.pose.orientation.z = quat.z();
  posestamped.pose.orientation.w = quat.w();
  _personPoint.header.frame_id = "person";
  _personPoint.header.stamp = posestamped.header.stamp;
  _personPoint.point = posestamped.pose.position;
  point2 = posestamped.pose.position;
  _pub2.publish(point2);
  path.header = header;
  path.poses.push_back(posestamped);
  _pub.publish(path);
  // _transform.setOrigin(tf::Vector3(point.x,point.y,point.z));
  // _transform.setRotation(quat);
  // _br.sendTransform(tf::StampedTransform(_transform, ros::Time::now(), "map", "newperson"));
  //
  // point2 = posestamped.pose.position;
  // _pub2.publish(point2);
// /* Creating the person_frame */
// _transform2.setOrigin(tf::Vector3(pointData->x, 0, pointData->z));
// _transform2.setRotation(tf::Quaternion(0,0,0,1));
// _br2.sendTransform(tf::StampedTransform(_transform2, ros::Time::now(), "newperson", "person"));
// _listener2.waitForTransform("map","person_frame",ros::Time::now(),ros::Duration(6.0));
//
// _listener2.lookupTransform("map","person",ros::Time(0), stamp2);
// point2.x = stamp2.getOrigin().x();
// point.y = stamp2.getOrigin().y();
// point2.z = stamp2.getOrigin().z();
// _pub.publish(point2);

}

private:
  ros::NodeHandle _nh;
  ros::Subscriber sub;
  ros::Subscriber sub2;
  ros::Publisher _pub;
  ros::Publisher _pub2;

  tf::Transform _transform;
  tf::Transform _transform2;
  tf::TransformBroadcaster _br;
  tf::TransformBroadcaster _br2;
  tf::TransformListener _listener;
  tf::TransformListener _listener2;

  geometry_msgs::PointStamped _personPoint;
  geometry_msgs::PointStamped _mapPoint;
  geometry_msgs::PoseStamped posestamped;
  std_msgs::Header header;
nav_msgs::Path path;
  tf::Quaternion quat;
  geometry_msgs::Point point;
  geometry_msgs::Point point2;
};


// class newlistener
// {
// public:
//
//
//   newlistener(){
//
//     sub = _nh.subscribe("/Optimized_person_distance",1, &newlistener::callback, this);
//
//   }
//   void callback(const geometry_msgs::Point::ConstPtr& pointData)
//   {
//
//     _personPoint.header.frame_id = "person_detection_frame";
//     _personPoint.header.stamp = ros::Time::now();
//     _personPoint.point.x = pointData -> x;
//     _personPoint.point.y = pointData -> y;
//     _personPoint.point.z = pointData -> z;
//     _listener.waitForTransform("map","person_detection_frame",ros::Time::now(),ros::Duration(3.0));
//     _listener.transformPoint("map", _personPoint, _mapPoint);
//     ROS_INFO_STREAM(" Transform: " << _mapPoint.point.x);
//     quat.setRPY(4.71238898,0,-1.57079633);
//     quat.normalize();
//     _transform.setOrigin(tf::Vector3(_personPoint.point.x,_personPoint.point.y,_personPoint.point.z));
//     _transform.setRotation(quat);
//     _br.sendTransform(tf::StampedTransform(_transform, ros::Time::now(), "map", "PLEASEFUCKINGWORKGODDAMNIT"));
//   }
//
//
// private:
//   ros::NodeHandle _nh;
//   ros::Subscriber sub;
//   ros::Subscriber sub2;
//   ros::Publisher _pub;
//   tf::Transform _transform;
//   tf::Transform _transform2;
//   tf::TransformBroadcaster _br;
//   tf::TransformBroadcaster _br2;
//   tf::TransformListener _listener;
//   tf::TransformListener _listener2;
//
//   geometry_msgs::PointStamped _personPoint;
//   geometry_msgs::PointStamped _mapPoint;
//   tf::Quaternion quat;
//   geometry_msgs::Point point;
//   geometry_msgs::Point point2;
// };

int main(int argc, char **argv)
{
  ros::init(argc,argv,"newnode");
  move_base_goal obj;
  std::cout<<"lol2";
  ros::spin();

  return 0;
}

#include "ros/ros.h"
#include "nav_msgs/Path.h"
#include "geometry_msgs/Point.h"
#include "geometry_msgs/PoseStamped.h"
#include "std_msgs/Header.h"
#include "tf/transform_broadcaster.h"
#include <iostream>
#include "std_msgs/Float32MultiArray.h"
#include "ros_detection/FloatVector.h"

class getTF
{
public:
getTF(){
  dist_sub = nh.subscribe("/Optimized_person_distance",1,&getTF::callback, this);
  path_pub = nh.advertise<nav_msgs::Path>("path",100,true);

}

void callback(const geometry_msgs::Point::ConstPtr& pointData)
{
  point.x = pointData->x;
  point.y = pointData->y;
  point.z = pointData->z;



  header.stamp = ros::Time::now();
  header.frame_id = "person_detection_frame";
  posestamped.header = header;
  posestamped.pose.position.x = point.x;
  posestamped.pose.position.y = point.y;
  posestamped.pose.position.z = point.z;
  posestamped.pose.orientation.x = 0;
  posestamped.pose.orientation.y = 0;
  posestamped.pose.orientation.z = 0;
  posestamped.pose.orientation.w = 1;

  path.header = header;
  path.poses.push_back(posestamped);
  path_pub.publish(path);

  quat.setRPY(4.71238898,0,-1.57079633);
  quat.normalize();
  _transform.setOrigin(tf::Vector3(0,0,0));
  _transform.setRotation(quat);
  _br.sendTransform(tf::StampedTransform(_transform, ros::Time::now(), "zed_left_camera_frame", "person_detection_frame"));

  _transform2.setOrigin(tf::Vector3(point.x, point.y, point.z));
  _transform2.setRotation(tf::Quaternion(0,0,0,1));
  _br2.sendTransform(tf::StampedTransform(_transform2, ros::Time::now(), "person_detection_frame", "person"));
}

private:
  ros::NodeHandle nh;
  ros::Subscriber dist_sub;
  ros::Publisher path_pub;
  tf::TransformBroadcaster _br;
  tf::Transform _transform;
  tf::TransformBroadcaster _br2;
  tf::Transform _transform2;
  tf::Quaternion quat;
  geometry_msgs::Point point;
  geometry_msgs::PoseStamped posestamped;
  std_msgs::Header header;
  nav_msgs::Path path;

};
int main()
{

  return 0;
}

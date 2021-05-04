/* Created by Nikolai Persen 2021 */

#include "ros/ros.h"
#include "nav_msgs/Path.h"
#include "geometry_msgs/PoseStamped.h"
#include "tf/transform_broadcaster.h"
#include <tf/transform_listener.h>
#include <std_msgs/Float64.h>
#include <cmath>
class PersonBroadcaster
{
public:
PersonBroadcaster(){
  dist_sub_ = nh_.subscribe("/Optimized_person_distance",1, &PersonBroadcaster::callback, this);
  path_pub_ = nh_.advertise<nav_msgs::Path>("person_path",1, true);
  move_base_pub_ = nh_.advertise<geometry_msgs::Point>("move_base_goal_topic",1,true);
  anglePub_ = nh_.advertise<std_msgs::Float64>("FOV_angle", 1, true);
  distancePub_ = nh_.advertise<std_msgs::Float64>("distance", 1, true);
}
void callback(const geometry_msgs::Point::ConstPtr& pointData)
{

  personpose_.pose.position.x = pointData->x;
  personpose_.pose.position.y = 0;
  personpose_.pose.position.z = pointData->z;
  personpose_.pose.orientation.w = 1;
  personpose_.header.stamp = ros::Time(0);
  personpose_.header.frame_id = "person_detection_frame";


  listener_.waitForTransform("map","person_detection_frame",ros::Time::now(),ros::Duration(3.0));
  listener_.transformPose("map", personpose_, mappose_);
  transform_.setRotation(tf::Quaternion(mappose_.pose.orientation.x,mappose_.pose.orientation.y,mappose_.pose.orientation.z,mappose_.pose.orientation.w));
  transform_.setOrigin(tf::Vector3(mappose_.pose.position.x, mappose_.pose.position.y, mappose_.pose.position.z));
  br_.sendTransform(tf::StampedTransform(transform_, ros::Time::now(), "map", "person"));

  path_.header.stamp = ros::Time::now();
  path_.header.frame_id = "map";
  posestamped_.header.stamp = ros::Time::now();
  posestamped_.header.frame_id = "map";
  posestamped_.header.seq = seq;
  posestamped_.pose.position = mappose_.pose.position;
  posestamped_.pose.orientation = mappose_.pose.orientation;
  path_.poses.push_back(posestamped_);
  path_pub_.publish(path_);
  move_base_pub_.publish(posestamped_.pose.position);
  FOV_angle.data  = -std::atan2(pointData->z, pointData->x) * 180 / 3.141592 +90;
  anglePub_.publish(FOV_angle);
  seq++;

}

private:
  ros::NodeHandle nh_;
  ros::Subscriber dist_sub_;
  ros::Publisher path_pub_;
  ros::Publisher move_base_pub_;
  ros::Publisher anglePub_;
  ros::Publisher distancePub_;
  tf::TransformBroadcaster br_;
  tf::Transform transform_;
  tf::TransformListener listener_;

  geometry_msgs::PoseStamped personpose_;
  geometry_msgs::PoseStamped mappose_;
  geometry_msgs::PoseStamped posestamped_;
  std_msgs::Float64 FOV_angle;
  nav_msgs::Path path_;
  int seq = 0;
};

int main(int argc, char **argv)
{
  ros::init(argc,argv,"node");
  PersonBroadcaster tfobj;
  ros::spin();
  return 0;
}

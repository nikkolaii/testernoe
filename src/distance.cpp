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
  path_sub_ = nh_.subscribe("/person_path",1, &PersonBroadcaster::callback, this);
  dist_pub_ = nh_.advertise<std_msgs::Float64>("relative_distance",10000,true);
  global_pub_ = nh_.advertise<geometry_msgs::PoseStamped>("odom_map",10000,true);
  posestamped_pub_ = nh_.advertise<geometry_msgs::PoseStamped>("pathforbag",1, true);

}
void callback(const nav_msgs::Path::ConstPtr& pathData)
{
  seq = pathData ->header.seq;
  float x1 = pathData -> poses[seq].pose.position.x;
  float y1 = pathData -> poses[seq].pose.position.y;

  posestamped_ = pathData -> poses[seq];
  posestamped_pub_.publish(posestamped_);
  listener_.waitForTransform("map","person_detection_frame",ros::Time::now(),ros::Duration(3.0));
  listener_.lookupTransform("map",  "person_detection_frame", ros::Time(0),stamped_);

  float x2 = stamped_.getOrigin().x();
  float y2 = stamped_.getOrigin().y();

  ROS_INFO_STREAM("");
  float x = x2-x1;
  float y = y2 - y1;
  distance.data = std::sqrt(std::pow(x,2) + std::pow(y,2));


  global_stamp_.pose.position.x = stamped_.getOrigin().x();
  global_stamp_.pose.position.y = stamped_.getOrigin().y();
  global_stamp_.pose.position.z = stamped_.getOrigin().z();

  global_stamp_.pose.orientation.x = stamped_.getRotation().x();
  global_stamp_.pose.orientation.y = stamped_.getRotation().y();
  global_stamp_.pose.orientation.z = stamped_.getRotation().z();
  global_stamp_.pose.orientation.w = stamped_.getRotation().w();

  global_stamp_.header.frame_id = "person_detection_frame";
  ROS_INFO_STREAM("THE DISTANCE BETWEEN THE ROBOT AND THE PERSON IS: "<< distance.data);
  dist_pub_.publish(distance);
  global_pub_.publish(global_stamp_);
}

private:
  ros::NodeHandle nh_;
  ros::Subscriber path_sub_;
  ros::Publisher dist_pub_;
  ros::Publisher move_base_pub_;
  ros::Publisher anglePub_;
  ros::Publisher global_pub_;
  ros::Publisher posestamped_pub_;

  tf::TransformBroadcaster br_;
  tf::Transform transform_;
  tf::TransformListener listener_;
  tf::StampedTransform stamped_;
  geometry_msgs::PoseStamped personpose_;
  geometry_msgs::PoseStamped mappose_;
  geometry_msgs::PoseStamped posestamped_;
  geometry_msgs::PoseStamped global_stamp_;

  std_msgs::Float64 FOV_angle;
  std_msgs::Float64 distance;
  nav_msgs::Path path_;

  int seq;
};

int main(int argc, char **argv)
{
  ros::init(argc,argv,"distancenode");
  PersonBroadcaster tfobj;
  ros::spin();
  return 0;
}

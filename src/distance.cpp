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
}
void callback(const nav_msgs::Path::ConstPtr& pathData)
{
  seq = pathData ->header.seq;
  float x1 = pathData -> poses[seq].pose.position.x;
  float y1 = pathData -> poses[seq].pose.position.y;

  listener_.waitForTransform("map","person_detection_frame",ros::Time::now(),ros::Duration(3.0));
  listener_.lookupTransform("map",  "person_detection_frame", ros::Time(0),stamped_);

  float x2 = stamped_.getOrigin().x();
  float y2 = stamped_.getOrigin().y();

  ROS_INFO_STREAM("");
  float x = x2-x1;
  float y = y2 - y1;
  distance.data = std::sqrt(std::pow(x,2) + std::pow(y,2));
  ROS_INFO_STREAM("THE DISTANCE BETWEEN THE ROBOT AND THE PERSON IS: "<< distance.data);
  dist_pub_.publish(distance);
}

private:
  ros::NodeHandle nh_;
  ros::Subscriber path_sub_;
  ros::Publisher dist_pub_;
  ros::Publisher move_base_pub_;
  ros::Publisher anglePub_;

  tf::TransformBroadcaster br_;
  tf::Transform transform_;
  tf::TransformListener listener_;
  tf::StampedTransform stamped_;
  geometry_msgs::PoseStamped personpose_;
  geometry_msgs::PoseStamped mappose_;
  geometry_msgs::PoseStamped posestamped_;
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

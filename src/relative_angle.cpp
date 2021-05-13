// #include "ros/ros.h"
// #include "nav_msgs/Path.h"
// #include "geometry_msgs/PoseStamped.h"
// #include "tf/transform_broadcaster.h"
// #include <tf/transform_listener.h>
// #include <std_msgs/Float64.h>
// #include <cmath>
//
//
//
//
// void f(ros::NodeHandle& nh, ros::Publisher& pub, tf::StampedTransform& stamp, tf::TransformListener& listener,
// geometry_msgs::Point& point, std_msgs::Float64& FOV_angle){
// listener.waitForTransform("person_detection_frame","map",ros::Time::now(),ros::Duration(3.0));
// listener.lookupTransform("person_detection_frame", "map", ros::Time(0), stamp);
//
// point.x = stamp.getOrigin().x();
// point.z = stamp.getOrigin().z();
// FOV_angle.data  = -std::atan2(point.z, point.x) * 180 / 3.141592 +90;
// pub.publish(FOV_angle);
//
// }
// int main(int argc, char** argv){
// geometry_msgs::Point point;
// ros::init(argc,argv,"relativedistancenode");
//
// ros::NodeHandle nh;
// ros::Publisher pub;
//
// pub = nh.advertise<std_msgs::Float64>("FOV_angle", 1, true);
// tf::StampedTransform stamp;
// tf::TransformListener listener;
// std_msgs::Float64 FOV_angle;
// while (true) {
//   f(nh, pub, stamp, listener, point, FOV_angle);
//
// }
// return 0;
// }
//



/* Created by Nikolai Persen 2021 */

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
  dist_pub_ = nh_.advertise<std_msgs::Float64>("FOV_angle2",10000,true);
}
void callback(const nav_msgs::Path::ConstPtr& pathData)
{
  seq = pathData ->header.seq;
  mappose_.header.frame_id = "map";
  mappose_.pose.position.x = pathData -> poses[seq].pose.position.x;
  mappose_.pose.position.y = pathData -> poses[seq].pose.position.y;
  mappose_.pose.position.z = pathData -> poses[seq].pose.position.z;

  mappose_.pose.orientation.x = pathData -> poses[seq].pose.orientation.x;
  mappose_.pose.orientation.y = pathData -> poses[seq].pose.orientation.y;
  mappose_.pose.orientation.z = pathData -> poses[seq].pose.orientation.z;
  mappose_.pose.orientation.w = pathData -> poses[seq].pose.orientation.w;


  float y1 = pathData -> poses[seq].pose.position.y;

  listener_.waitForTransform("map","person_detection_frame",ros::Time::now(),ros::Duration(3.0));
  listener_.transformPose("person_detection_frame", mappose_, personpose_);

  float x = personpose_.pose.position.x;
  float z = personpose_.pose.position.z;
  FOV_angle.data  = -std::atan2(z,x) * 180 / 3.141592 +90;
  dist_pub_.publish(FOV_angle);
  ROS_INFO_STREAM(FOV_angle);
  // float x = x2-x1;
  // float y = y2 - y1;
  // distance.data = std::sqrt(std::pow(x,2) + std::pow(y,2));
  // ROS_INFO_STREAM("THE DISTANCE BETWEEN THE ROBOT AND THE PERSON IS: "<< distance.data);
  // dist_pub_.publish(distance);
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
  ros::init(argc,argv,"anglenode");
  PersonBroadcaster tfobj;
  ros::spin();
  return 0;
}

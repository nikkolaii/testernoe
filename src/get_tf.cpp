/* Created by Nikolai Persen 2021 */

#include "ros/ros.h"
#include "nav_msgs/Path.h"
#include "geometry_msgs/PoseStamped.h"
#include "tf/transform_broadcaster.h"
#include <tf/transform_listener.h>
#include <std_msgs/Float64.h>
#include <cmath>
class getTF
{
public:
getTF(){
  dist_sub_ = nh__.subscribe("/Optimized_person_distance",1, &getTF::callback, this);
  path_pub_ = nh__.advertise<nav_msgs::Path>("person_path",100, true);
  pub2_ = nh__.advertise<geometry_msgs::Point>("move_base_goal_topic",100,true);
  anglePub_ = nh__.advertise<std_msgs::Float64>("FOV_angle", 100, true);
}

void callback(const geometry_msgs::Point::ConstPtr& pointData)
{

  /* Creating a message containing the depth measurements relative to the person_detection_frame
     Note that y = 0 such that the person is confined to the 2D floor plane at the height of the camera */
  personpose_.pose.position.x = pointData->x;
  personpose_.pose.position.y = 0;
  personpose_.pose.position.z = pointData->z;
  personpose_.pose.orientation.w = 1;
  personpose_.header.stamp = ros::Time(0);
  personpose_.header.frame_id = "person_detection_frame";
  std_msgs::Float64 FOV_angle;
  FOV_angle.data  = -std::atan2(pointData->z, pointData->x) * 180 / 3.141592 +90;

  ROS_INFO_STREAM(FOV_angle);
/* Listening for a transform from person_detection_frame to the intertial map frame */
  listener_.waitForTransform("map","person_detection_frame",ros::Time::now(),ros::Duration(3.0));
  listener_.transformPose("map", personpose_, mappose_);

/* Broadcasting the coordinate frame "person" relative to the inertial map frame */
  transform_.setRotation(tf::Quaternion(mappose_.pose.orientation.x,mappose_.pose.orientation.y,mappose_.pose.orientation.z,mappose_.pose.orientation.w));
  transform_.setOrigin(tf::Vector3(mappose_.pose.position.x, mappose_.pose.position.y, mappose_.pose.position.z));
  br_.sendTransform(tf::StampedTransform(transform_, ros::Time::now(), "map", "person"));

/* Defining the path message for the path publisher */
path.header.stamp = ros::Time::now();
path.header.frame_id = "map";
posestamped_.header.stamp = ros::Time::now();
posestamped_.header.frame_id = "map";
posestamped_.pose.position = mappose_.pose.position;
posestamped_.pose.orientation = mappose_.pose.orientation;
path.poses.push_back(posestamped_);
path_pub_.publish(path);
pub2_.publish(posestamped_.pose.position);
anglePub_.publish(FOV_angle);
}

private:
  ros::NodeHandle nh__;
  ros::Subscriber dist_sub_;
  ros::Publisher path_pub_;
  ros::Publisher pub2_;
  ros::Publisher anglePub_;

  tf::TransformBroadcaster br_;
  tf::Transform transform_;
  tf::Quaternion quat_;
  tf::TransformListener listener_;

  geometry_msgs::PoseStamped personpose_;
  geometry_msgs::PoseStamped mappose_;
  geometry_msgs::PoseStamped posestamped_;
  nav_msgs::Path path;
};

int main(int argc, char **argv)
{
  ros::init(argc,argv,"node");
  getTF tfobj;
  ros::spin();
  return 0;
}

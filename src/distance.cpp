/* Created by Nikolai Persen 2021 */

#include "ros/ros.h"
#include "nav_msgs/Path.h"
#include "geometry_msgs/PoseStamped.h"
#include "tf/transform_broadcaster.h"
#include <tf/transform_listener.h>
#include <std_msgs/Float64.h>
#include <cmath>
class Distance
{
  public:
  Distance()
  {
    path_sub_ = nh_.subscribe("/person_path",1, &Distance::callback, this);
    dist_pub_ = nh_.advertise<std_msgs::Float64>("relative_distance",10000,true);
    global_pub_ = nh_.advertise<geometry_msgs::PoseStamped>("odom_map",10000,true);
    posestamped_pub_ = nh_.advertise<geometry_msgs::PoseStamped>("pathforbag",1, true);
    FOV_pub_ = nh_.advertise<std_msgs::Float64>("FOV_angle2",10000,true);
  }
  void callback(const nav_msgs::Path::ConstPtr& pathData)
  {
    seq = pathData ->header.seq;
    posestamped_ = pathData -> poses[seq];
    posestamped_.header.frame_id = "map";
    posestamped_.header.stamp = ros::Time(0);
    posestamped_pub_.publish(posestamped_);
    listener_.waitForTransform("person_detection_frame","map",ros::Time::now(),ros::Duration(3.0));
    listener_.transformPose("person_detection_frame", posestamped_,personpose_);
    float x = personpose_.pose.position.x;
    float z = personpose_.pose.position.z;
    distance.data = std::sqrt(std::pow(x,2) + std::pow(z,2));
    dist_pub_.publish(distance);

    global_stamp_.pose.position.x = stamped_.getOrigin().x();
    global_stamp_.pose.position.y = stamped_.getOrigin().y();
    global_stamp_.pose.position.z = stamped_.getOrigin().z();

    global_stamp_.pose.orientation.x = stamped_.getRotation().x();
    global_stamp_.pose.orientation.y = stamped_.getRotation().y();
    global_stamp_.pose.orientation.z = stamped_.getRotation().z();
    global_stamp_.pose.orientation.w = stamped_.getRotation().w();

    global_stamp_.header.frame_id = "person_detection_frame";
    FOV_angle.data  = -std::atan2(z,x) * 180 / 3.141592 +90;
    FOV_pub_.publish(FOV_angle);
    global_pub_.publish(global_stamp_);
}

private:
  ros::NodeHandle nh_;
  ros::Subscriber path_sub_;
  ros::Publisher dist_pub_;
  ros::Publisher FOV_pub_;
  ros::Publisher global_pub_;
  ros::Publisher posestamped_pub_;

  tf::TransformBroadcaster br_;
  tf::Transform transform_;
  tf::TransformListener listener_;
  tf::StampedTransform stamped_;
  geometry_msgs::PoseStamped personpose_;
  geometry_msgs::PoseStamped posestamped_;
  geometry_msgs::PoseStamped global_stamp_;

  std_msgs::Float64 FOV_angle;
  std_msgs::Float64 distance;
  int seq;
};

int main(int argc, char **argv)
{
  ros::init(argc,argv,"distancenode");
  Distance tfobj;
  ros::spin();
  return 0;
}

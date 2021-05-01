#include "ros/ros.h"
#include "geometry_msgs/Point.h"
#include "tf/transform_broadcaster.h"
#include <tf2_geometry_msgs/tf2_geometry_msgs.h>

class poseTF{

public:
  poseTF(){
  _sub = _nh.subscribe("/person_position", 1, &poseTF::callback, this);

}

void callback(const geometry_msgs::Point::ConstPtr data){
  geometry_msgs::Point point;
  point.x = data -> x;
  point.y = data -> y;
  point.z = data -> z;
  quat.setRPY(4.71238898,0,0);
  quat.normalize();

  _transform.setOrigin(tf::Vector3(0,0,0));
  _transform.setRotation(quat);
  _br.sendTransform(tf::StampedTransform(_transform, ros::Time::now(), "zed_left_camera_frame", "person_detection_frame"));

  _transform2.setOrigin(tf::Vector3(data -> x, data -> y, data -> z));
  _transform2.setRotation(tf::Quaternion(0,0,0,1));
  _br2.sendTransform(tf::StampedTransform(_transform2, ros::Time::now(), "person_detection_frame", "person"));
}
private:
  ros::NodeHandle _nh;
  ros::Subscriber _sub;

  tf::TransformBroadcaster _br;
  tf::Transform _transform;
  tf::TransformBroadcaster _br2;
  tf::Transform _transform2;
  tf::Quaternion quat;
};

int main(int argc, char **argv){
  ros::init(argc, argv, "personTF");

  poseTF tfobj;
  ros::spin();
  return 0;
}

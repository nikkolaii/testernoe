#!/usr/bin/env python
import rospy
from geometry_msgs.msg import Point
from nav_msgs.msg import Path
from std_msgs.msg import Header
from geometry_msgs.msg import PoseStamped

path = Path()


def odom_cb(data):
    global path
    myheader = Header()
    myheader.frame_id = "odom"
    myheader.stamp = rospy.Time.now()
    myheader.seq = 1
    path.header = myheader
    pose = PoseStamped()
    pose.header = myheader

    pose.pose.position = data
    pose.pose.orientation.x = 0
    pose.pose.orientation.y = 0
    pose.pose.orientation.z = 0
    pose.pose.orientation.w = 1

    path.poses.append(pose)
    path_pub.publish(path)
rospy.init_node('path_node')
odom_sub = rospy.Subscriber('/person_position', Point, odom_cb)
path_pub = rospy.Publisher('/path', Path, queue_size=10)

if __name__ == '__main__':
    rospy.spin()

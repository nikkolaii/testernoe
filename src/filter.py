#!/usr/bin/env python
import rospy
from std_msgs.msg import Float32MultiArray
i = 1
from nav_msgs.msg import Odometry

import message_filters

def callback(odom1):
 print("hei")
 x = odom1.data
 for i in range(100):
  print(x[i])



rospy.init_node('testnode', anonymous=True)
mysub = rospy.Subscriber("path",Float32MultiArray,callback)


rospy.spin()

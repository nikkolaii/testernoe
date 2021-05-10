#!/usr/bin/env python
import dynamic_reconfigure.client
import rospy
from std_msgs.msg import Float64
import numpy as np
import math

def f(x):
    x1 = 0.5
    x2 = 2.0
    y1 = 0.2
    y2 = 0.8
    b = (y1/y2)**(1/(x1-x2))
    a = y2/b**x2
    return(a*b**x)

def callback(data):
    distance = data.data
    if distance >= 0.5 and distance <= 2:
        cost = f(distance)
    if distance < 0.5:
        cost = 0.1
    if distance > 2:
        cost = 0.8
    client = dynamic_reconfigure.client.Client("move_base/TrajectoryPlannerROS", timeout=4, config_callback=None)
    client.update_configuration({"max_vel_x":cost})
    pub = rospy.Publisher('velocity', Float64, queue_size=10)
    pub.publish(cost)

def listener():
    rospy.init_node('listener', anonymous=True)
    rospy.Subscriber("relative_distance", Float64, callback)
    rospy.spin()
if __name__ == '__main__':
    try:
        listener()
    except rospy.ROSInterruptException: pass

# import numpy as np
# import matplotlib.pyplot as plt
# # t = np.linspace(0,3.1415,num = 100)
# # x = np.zeros(len(t))
# # for i in range(len(np.linspace(0,1,num = 100))):
# #         x[i] = np.tanh(t[i])
# #         print(x[i])
# # # plt.show()
# #
# # dist = np.linspace(0,4,num=100)
# # dist = dist * np.pi/4
# # print(dist)
# # plt.plot(t,np.tanh(dist))
# # plt.show()
# #
# import math
# def activation_funtion(x):
#     k = 0.0179862
#     h = 4.018149928
#     ans = math.exp(x-h)+k
#     return ans
#
#
# x1 = 0.1
# x2 = 1
#
# y1 = 0
# y2 = 0.8
#
# f(x) = a*b**x
# a = y1 / b**x1
# b = (y2/a)**(1/x2)
# # x = np.linspace(0,4,100)
# # y = np.zeros((len(x),1))
# # for i in range((len(y))):
# #     y[i] = activation_funtion(x[i])
# # plt.plot(x,y)
# # plt.show()
# #!/usr/bin/env python

#
import dynamic_reconfigure.client
import rospy
from std_msgs.msg import Float64
import numpy as np
import math
global costt
def f(x):
    x1 = 0.5
    x2 = 2

    y1 = 0.2
    y2 = 0.8


    b = (y1/y2)**(1/(x1-x2))
    a = y2/b**x2
    ans = a*b**x
    return ans
def callback(data):
    distance = data.data
    print(distance)
    if distance >= 0.5 and distance <= 2:
        costt = f(distance)
    if distance < 0.5:
        costt = 0.1
    if distance > 2:
        costt = 0.8
    print(costt)
    client = dynamic_reconfigure.client.Client("move_base/TrajectoryPlannerROS", timeout=4, config_callback=None)
    client.update_configuration({"max_vel_x":costt})
def listener():
    rospy.init_node('listener', anonymous=True)
    rospy.Subscriber("relative_distance", Float64, callback)


    rospy.spin()



if __name__ == '__main__':
    try:
        listener()
    except rospy.ROSInterruptException: pass

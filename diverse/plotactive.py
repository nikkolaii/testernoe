import numpy as np
import matplotlib.pyplot as plt
import math
x1 = 0.5
x2 = 2

y1 = 0.2
y2 = 0.8

def f(x):
        x1 = 0.5
        x2 = 2

        y1 = 0.2
        y2 = 0.8
        b = (y1/y2)**(1/(x1-x2))
        a = y2/b**x2
	print("a = ", a)
	print("\nb = ", b)
        return(a*b**x)

x = np.linspace(0,3,1000)
y = np.zeros([len(x),1])
for i in range(len(x)):
    if x[i]<= x1:
        y[i] = y1
    if x[i] > x1 and x[i] < x2:
        y[i] = f(x[i])
    if x[i] >= x2:
        y[i] = y2
plt.plot(x,y)
plt.xlim([0,2.5])
plt.xlabel("Distance from the robot to the student [m]")
plt.ylabel("Commanded velocity [%]")
plt.grid()
plt.ylim([0,1])
plt.show()

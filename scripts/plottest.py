#!/usr/bin/python

import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import serial
import time

PORT = '/dev/ttyACM0'
BAUD = 115200
ser = serial.Serial(PORT, BAUD)
time.sleep(3)
ser.write('p')
ypr = [0,0,0]

def data_gen():
    t = data_gen.t
    try:
        ypr = map(float, ser.readline().split(","))
        print ypr
    except ValueError:
        ypr = [0,0,0]
    cnt = 0
    while cnt < 1000:
        cnt+=1
        t += 0.05
        #yield t, np.sin(2*np.pi*t + 5*np.random.rand())
        yield t, ypr[0]
data_gen.t = 0

fig, ax = plt.subplots()
line, = ax.plot([], [], lw=2)
ax.set_ylim(-1.1, 1.1)
ax.set_xlim(0, 5)
ax.grid()
xdata, ydata = [], []
def run(data):
    # update the data
    t,y = data
    xdata.append(t)
    ydata.append(y)
    xmin, xmax = ax.get_xlim()
    #ypr = map(float, ser.readline().split(","))
    print ypr

    if t >= xmax:
        ax.set_xlim(xmin, 2*xmax)
        ax.figure.canvas.draw()
    line.set_data(xdata, ydata)

    return line,

ani = animation.FuncAnimation(fig, run, data_gen, blit=True, interval=10,
    repeat=False)
plt.show()

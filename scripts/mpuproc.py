#!/usr/bin/python

import serial
import time

PORT = '/dev/ttyACM0'
BAUD = 115200

ser = serial.Serial(PORT, BAUD)
i = 0

time.sleep(5)
#print ser.readline()

ser.write('p')

for i in xrange(100):
    print ser.readline()

ser.close()

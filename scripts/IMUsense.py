#!/usr/bin/python

import serial
import time
import numpy as np
from pylab import *

PORT = '/dev/ttyACM0'
BAUD = 115200

ser = serial.Serial(PORT, BAUD)

time.sleep(2)

ser.write('p')




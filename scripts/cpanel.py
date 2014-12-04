#!/usr/bin/python

from gi.repository import Gtk, GLib, GObject
import threading
import time
import serial
from scipy.signal import *

h = firwin(8, 1/18.0)
M = np.zeros(64)
t = np.linspace(0, 5, 5*18, endpoint=False)

class Handler:
    def onDeleteWindow(self, *args):
        Gtk.main_quit(*args)

def rtfilt(s):
    np.roll(M, 1)
    M[0] = s
    return np.dot(M, h)
    

def app_main():
    builder = Gtk.Builder()
    builder.add_from_file("cpanel-ui.glade")
    builder.connect_signals(Handler())

    window = builder.get_object("MainWindow")
    progress = builder.get_object("progressbar1")
    ypr_stat = builder.get_object("label6")

    #Setting the arrow motion indicators
    up_pic_chair = builder.get_object("image1")
    left_pic_chair = builder.get_object("image2")
    right_pic_chair = builder.get_object("image3")
    down_pic_chair = builder.get_object("image4")

    up_pic_arm = builder.get_object("image5")
    left_pic_arm = builder.get_object("image6")
    right_pic_arm = builder.get_object("image7")
    down_pic_arm = builder.get_object("image8")

    up_pic_chair.set_from_file("up.jpeg")
    left_pic_chair.set_from_file("left.jpeg")
    right_pic_chair.set_from_file("right.jpeg")
    down_pic_chair.set_from_file("down.jpeg")

    up_pic_arm.set_from_file("up.jpeg")
    left_pic_arm.set_from_file("left.jpeg")
    right_pic_arm.set_from_file("right.jpeg")
    down_pic_arm.set_from_file("down.jpeg")
    
    def update_progress(i):
        progress.pulse()
        progress.set_text(str(i))
        return False

    def t_target():
        for i in xrange(200):
            GLib.idle_add(update_progress, i)
            time.sleep(0.2)
            
    def ypr_collect(string):
        dispstr = "Yaw: "+string[0]+" Pitch: "+string[1]+" Roll: "+string[2]
        ypr_stat.set_text(dispstr)
        return False

    def ypr_thread():
        while True:
            #ser.write('p')
            #data = str(ser.readline())
            y = np.zeros(8)
            p = np.zeros(8)
            r = np.zeros(8)
            for i in xrange(8):
                try:
                    ypr = map(float, ser.readline().split(","))
                except ValueError:
                    ypr = [0, 0, 0]
                y[i] = ypr[0]
                p[i] = ypr[1]
                r[i] = ypr[2]
            scipy.signal.lfilter(y, h)
            scipy.signal.lfilter(p, h)
            scipy.signal.lfilter(r, h)
            
            GLib.idle_add(ypr_collect, (map(str, ypr)))
            time.sleep(0.2)
        
            
    window.show_all()
    thread = threading.Thread(target=t_target)
    ser_thread = threading.Thread(target=ypr_thread)
    ser_thread.daemon=True
    thread.daemon = True
    thread.start()
    ser_thread.start()

if __name__ == "__main__":

    PORT = '/dev/ttyACM0'
    BAUD = 115200
    
    ser = serial.Serial(PORT, BAUD)
    
    time.sleep(2)

    ser.write('p')
    GObject.threads_init()
    app_main()
    Gtk.main()

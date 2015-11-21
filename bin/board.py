from time import sleep
from serial import Serial

id = 255

s = Serial("/dev/ttyUSB1", 19200)
s.setStopbits(2)

def clear():
  while s.inWaiting():
    s.read()

def write(cmd):
  s.write("255:{0}\n".format(cmd)) 
  
def read(cmd):
  write(cmd)
  buf = "_"

  while buf[-1] != "\n":
    buf += s.read()

  clear()
  return buf[1:]

def dribbler(sp = 90):
  while 1:
    sleep(1)
    write("dm{0}".format(sp))

def ball_sense():
  while 1:
    print(read("bl"))
    sleep(1)

def coil():
  write("c")
  write("c")
  write("c")
  sleep(1)
  write("k")

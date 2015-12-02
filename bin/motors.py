# Meelik Kiik
# -*- coding: utf-8 -*-
from time import sleep
from serial import Serial

interface = Serial("/dev/ttyUSB0", 19200)

_CMD = "{0}:{1}\n"

def send_command(id, cmd):
  data = _CMD.format(id, cmd)
  print("send_command: %s" % data)
  interface.write(data)

def read_response():
  c = ""
  output = ""
  while c != '\n':
    c = interface.read()
    output += c

  return output
 
def read_info(m_id):
  send_command(m_id, "?")
  res = ""
  #res = read_response()
  return res

def clamp_velocity(v):
  return 255 if v > 255 else -255 if v < -255 else v


class Motor():
  id_seq = 0

  # Polarity relations
  #_fw = [(5, -1), (6, -1), (7, 1), (8, 1)]
  #_rotl = [(5, 1), (6, 1), (7, 1), (8, 1)]

  _fw = [(1, -1), (2, -1), (3, 1), (4, 1)]
  _rotl = [(1, 1), (2, 1), (3, 1), (4, 1)]



  @classmethod
  def getNewID(cls):
    id = cls.id_seq = cls.id_seq + 1
    return id

  def __init__(self):
    self.id = Motor.getNewID()
    print("Initializing motor %i" % self.id)
    self.write("fs1")
    #self.initialTests()

  def write(self, cmd):
    send_command(self.id, cmd)

  def initialTests(self):
    self.write("sd10")

  def setSpeed(self, v):
    self.write("sd%i" % v)

class MotorController():
  N_motors = 4

  def __init__(self):
    self.motors = set()
    for i in range(self.N_motors):
      self.motors.add(Motor())
    self.n_motors = len(self.motors)

    self.motor_map = {}
    for m in self.motors:
      self.motor_map[m.id] = m

  def getMotor(self, m_id):
    return self.motor_map[m_id]

  def _setSpeed(self, speed, pol_rel):
    v = clamp_velocity(speed)
    for m in self.motors:
      for p in pol_rel:
        if p[0] == m.id:
          m.setSpeed(v * p[1])
          break;

  def setSpeed1(self, speed):
    self._setSpeed(speed, Motor._fw)

  def setSpeed2(self, speed):
    self._setSpeed(speed, Motor._rotl)

if __name__=="__main__":
  mco = MotorController()
  #sleep(1)
  
  v = 15
  if 1:
    mco.setSpeed1(v)
    sleep(2)
    mco.setSpeed1(-v)
    sleep(2)
    mco.setSpeed2(v)
    sleep(2)
    mco.setSpeed2(-v)
    sleep(2)

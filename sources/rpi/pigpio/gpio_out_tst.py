#!/usr/bin/env python

import time
import pigpio

#wid=[0]*WAVES

pi = pigpio.pi()


pi.set_mode(6, pigpio.OUTPUT);
pi.set_mode(21, pigpio.OUTPUT);
pi.set_mode(19, pigpio.OUTPUT);
pi.set_mode(26, pigpio.OUTPUT);

#stability stests on osciloscope
pi.set_servo_pulsewidth(6, 2000),
pi.set_servo_pulsewidth(21, 2000),
pi.set_servo_pulsewidth(19, 2000),
pi.set_servo_pulsewidth(26, 2000),

pi.stop()

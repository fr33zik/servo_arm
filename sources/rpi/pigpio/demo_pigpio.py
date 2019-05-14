#!/usr/bin/env python

 # 
 # This file is part of the fr33zik distribution (https://github.com/fr33zik/servo_arm).
 # Copyright (c) 2019 Jaroslav Jablonicky.
 # 
 # This program is free software: you can redistribute it and/or modify  
 # it under the terms of the GNU General Public License as published by  
 # the Free Software Foundation, version 3.
 #
 # This program is distributed in the hope that it will be useful, but 
 # WITHOUT ANY WARRANTY; without even the implied warranty of 
 # MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 # General Public License for more details.
 #
 # You should have received a copy of the GNU General Public License 
 # along with this program. If not, see <http://www.gnu.org/licenses/>.
 #

import time
import pigpio

#WAVES=5
servo1=5
servo2=6
servo3=19
servo4=26

#wid=[0]*WAVES

pi = pigpio.pi()


pi.set_mode(13, pigpio.INPUT);
pi.set_mode(12, pigpio.INPUT);

pi.set_mode(servo1, pigpio.OUTPUT);
pi.set_mode(servo2, pigpio.OUTPUT);
pi.set_mode(servo3, pigpio.OUTPUT);
pi.set_mode(servo4, pigpio.OUTPUT);

print("starting pigpiod demo")

pi.set_servo_pulsewidth(servo1, 1500),
pi.set_servo_pulsewidth(servo2, 1500),
pi.set_servo_pulsewidth(servo3, 1500),
pi.set_servo_pulsewidth(servo4, 1500),

time.sleep(5)
#move to object
for i in range(1500,1301,-1):
    pi.set_servo_pulsewidth(servo1, i),
    for j in range(1500):
        continue
#open up
for i in range(1500,1901,1):
    pi.set_servo_pulsewidth(servo4, i),
    for j in range(1500):
        continue
time.sleep(2)

#go down
for i in range(1500,1301,-1):
    pi.set_servo_pulsewidth(servo3, i),
    for j in range(1500):
        continue

for i in range(1500,1001,-1):
    pi.set_servo_pulsewidth(servo2, i),
    for j in range(1500):
        continue

time.sleep(3)
#grab it
for i in range(1901,1101,-1):
    pi.set_servo_pulsewidth(servo4, i),
    for j in range(1500):
        continue
#go up
for i in range(1001,1500,1):
    pi.set_servo_pulsewidth(servo2, i),
    for j in range(1500):
        continue
for i in range(1201,1500,1):
    pi.set_servo_pulsewidth(servo3, i),
    for j in range(1500):
        continue
time.sleep(1)
#run away
for i in range(1301,1700,1):
    pi.set_servo_pulsewidth(servo1, i),
    for j in range(1500):
        continue
time.sleep(3)
#place down
for i in range(1500,1301,-1):
    pi.set_servo_pulsewidth(servo3, i),
    for j in range(1500):
        continue

for i in range(1500,1001,-1):
    pi.set_servo_pulsewidth(servo2, i),
    for j in range(1500):
        continue
#let go
for i in range(1801,1501,-1):
    pi.set_servo_pulsewidth(servo4, i),
    for j in range(1500):
        continue
time.sleep(1)
#center all
for i in range(1001,1500,1):
    pi.set_servo_pulsewidth(servo2, i),
    for j in range(1500):
        continue
for i in range(1201,1500,1):
    pi.set_servo_pulsewidth(servo3, i),
    for j in range(1500):
        continue
for i in range(1701,1501,-1):
    pi.set_servo_pulsewidth(servo1, i),
    for j in range(1500):
        continue
print("end of demo")
pi.stop()

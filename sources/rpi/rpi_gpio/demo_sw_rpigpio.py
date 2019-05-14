import RPi.GPIO as GPIO
from time import sleep
GPIO.setmode(GPIO.BCM)    
GPIO.setup(21, GPIO.OUT) 
sw_pwm = GPIO.PWM(21, 50)  
sw_pwm.start(2.5)                    
try:
    while 1:                    # Loop will run forever
        sw_pwm.ChangeDutyCycle(10)  # Move servo to 90 degrees
        sleep(1)                
        sleep(1)
except KeyboardInterrupt:
    pass 
GPIO.cleanup()

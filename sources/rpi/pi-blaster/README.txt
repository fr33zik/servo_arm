Here are the pi-blaster-edited.c that you can swap for original to get 20ms period pulse.

source: https://github.com/sarfata/pi-blaster


use:
to get 2ms width
change BCM pin to output then enable signal 
echo "17=0.1" > /dev/pi-blaster